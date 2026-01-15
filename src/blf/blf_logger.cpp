#include "blf_logger.h"
#include "can/can_message_blf_writer.h"

#include <iostream>

#include "can_message.h"
#include "blf_object_header.h"
#include "../include/message_factory.h"
#include "../api/imessage_reader.h"
#include "../registry/reader_registry.h"
#include "../registry/reader_registrar.h"
#include "zlib.h"

namespace BLF
{

BlfLogger::BlfLogger()
	: frame_count_(0),
	compression_method_(0),
	compression_level_(0)
{
	writer_ = WriterRegistry::get_instance().create_writers(FileFormat::BLF);
}

bool BlfLogger::open(const std::string& filepath, OpenMode mode)
{
	if (filepath.empty()) return false;
	mode_ = mode;
	if (OpenMode::Write == mode)
	{
		file_writer_.open(filepath);
		file_statistics_writer_.write_file_header(file_writer_);

		is_running_.store(true);
		writer_thread_ = std::thread([this](){ return writer_thread_handler(); });
	}
	else if (OpenMode::Read == mode)
	{
		file_reader_.open(filepath);
		FileStatistics file_stats{};
		if (!file_reader_.read_struct(file_stats))
		{
			return false;
		}

		file_statistics_writer_.update_file_statistics(file_stats);

		is_running_.store(true);
		read_logcontainer_thread_ =
			std::thread(&BlfLogger::read_logcontainer_thread_handler, this);

		read_busmsg_thread_
			= std::thread(&BlfLogger::read_busmsg_thread_handler, this);
	}
	return true;
}

void BlfLogger::close()
{
	is_running_.store(false);
	msg_cv_.notify_all();
	if (writer_thread_.joinable())
	{
		writer_thread_.join();
	}

	if (read_logcontainer_thread_.joinable())
		read_logcontainer_thread_.join();

	if (read_busmsg_thread_.joinable())
		read_busmsg_thread_.join();

	if (OpenMode::Write == mode_)
	{
		if (file_writer_.is_open())
		{
			flush_logcontainer(log_container_.get_logcontainer());

			file_statistics_writer_.update_frame_count(frame_count_);
			file_statistics_writer_.update_file_size(file_writer_.tell());
			file_writer_.seek(0);
			file_statistics_writer_.update_file_header(file_writer_);
			file_writer_.close();
		}
		if (file_reader_.is_open())
		{
			file_reader_.close();
		}

		frame_count_.store(0);
	}
}

bool BlfLogger::write(BusMessagePtr msg)
{
	if (!msg || !file_writer_.is_open()) return false;

	{
		std::unique_lock lock(msg_mtx_);
		if (msg_queue_.size() >= MAX_FRAME_CACHE_COUNT)
		{
			auto next = std::chrono::steady_clock::now();
			constexpr auto period = std::chrono::microseconds(10);
			next += period;
			std::this_thread::sleep_until(next);
		}
	}

	std::unique_lock lock(msg_mtx_);
	msg_queue_.push(std::move(msg));
	msg_cv_.notify_one();

	return true;
}

void BlfLogger::flush_logcontainer(LogContainer& log_container)
{
	auto& lc = log_container_.get_logcontainer();

	log_container_.set_buffer(file_writer_.get_buffer(), file_writer_.get_pos());
	log_container_.compress(compression_method_, compression_level_);

	LogContainerDiskHeader hdr{};
	hdr.base.signature      = BL_OBJ_SIGNATURE;
	hdr.base.header_size    = sizeof(ObjectHeaderBase);
	hdr.base.header_version = 1;
	hdr.base.object_type    = BL_OBJ_TYPE_LOG_CONTAINER;

	hdr.compressionMethod = lc.compression_method;
	hdr.reserved1         = 0;
	hdr.reserved2         = 0;
	hdr.uncompressedSize  = lc.uncompressed_file_size;
	hdr.reserved3         = 0;


	const uint32_t raw = lc.compressed_file_size;

	hdr.base.object_size = static_cast<uint32_t>(sizeof(LogContainerDiskHeader) + raw);

	file_writer_.write(reinterpret_cast<const uint8_t*>(&hdr), sizeof(hdr));
	file_writer_.write(lc.compressed_file, raw);

	const uint32_t pad = hdr.base.object_size % 4;
	if (pad)
	{
		static const uint8_t zeros[3] = {0,0,0};
		file_writer_.write(zeros, pad);
	}

	file_writer_.set_pos(0);
}

bool BlfLogger::is_open() const
{
	bool is_open = false;
	if (OpenMode::Write == mode_)
	{
		is_open = file_writer_.is_open();
	}
	else if (OpenMode::Read == mode_)
	{
		is_open = file_reader_.is_open();
	}
	return is_open;
}

uint64_t BlfLogger::get_message_count() const
{
	return msg_queue_.size();
}

uint64_t BlfLogger::get_file_size() const
{
	uint64_t file_size = 0;
	// if (OpenMode::Write == mode_)
	// {
	// 	file_size = file_writer_.file_size();
	// }
	// else if (OpenMode::Read == mode_)
	// {
	// 	file_size = file_reader_.get_pos();
	// }
	return file_size;
}

void BlfLogger::set_compres_level(int32_t compres_level)
{
	if (compres_level == 0)
	{
		compression_method_ = 0;
	}
	else
	{
		compression_method_ = 2;
	}
	compression_level_ = compres_level;
	file_statistics_writer_.set_compres_level(compres_level);
}

void BlfLogger::set_timestamp_unit(int32_t unit)
{
	for (auto& it : writer_)
	{
		it.second->set_timestamp_unit(unit);
	}
}

void BlfLogger::read(BusMessagePtr& msg)
{
	std::lock_guard<std::mutex> lock(msg_mtx_);
	if (msg_queue_.empty())
	{
		msg = nullptr;
		return;
	}

	msg = std::move(msg_queue_.front());
	msg_queue_.pop();
}

void BlfLogger::get_measure_time(uint64_t& start_time, uint64_t& stop_time)
{
	file_statistics_writer_.get_measure_time(start_time, stop_time);
}

uint32_t BlfLogger::align_pad_like_writer(uint32_t object_size)
{
	return object_size % 4u;
}

void BlfLogger::writer_thread_handler()
{
	constexpr auto kWakeInterval = std::chrono::microseconds(10);

	while (is_running_.load())
	{
		BusMessagePtr msg;
		std::queue<BusMessagePtr> msg_queue;
		{
			std::unique_lock lock(msg_mtx_);
			msg_cv_.wait_for(lock, kWakeInterval, [this]()
			{
				return !msg_queue_.empty() || !is_running_.load();
			});
			if (!is_running_.load() && msg_queue_.empty())
				break;
			if (msg_queue_.empty()) continue;

			msg_queue = std::move(msg_queue_);
		}

		while (!msg_queue.empty())
		{
			msg = std::move(msg_queue.front());
			const auto bus_type = msg->get_bus_type();
			auto it = writer_.find(bus_type);
			if (it == writer_.end()) continue;

			if ((file_writer_.get_pos() + 400) >= BUFFER_MAX_SIZE)
			{
				flush_logcontainer(log_container_.get_logcontainer());
			}

			auto retult = it->second->write(*msg, file_writer_);
			frame_count_.fetch_add(1);

			msg_queue.pop();
		}
	}
}

void BlfLogger::read_logcontainer_thread_handler()
{
    constexpr auto kWakeInterval = std::chrono::microseconds(100);

    while (is_running_.load())
    {
        ObjectHeaderBase base{};
        if (!file_reader_.read_struct(base))
        {
            is_running_.store(false);
            log_cv_.notify_all();
            msg_cv_.notify_all();
            break;
        }

        if (base.signature != BL_OBJ_SIGNATURE ||
            base.object_size < sizeof(ObjectHeaderBase))
        {
            continue;
        }

        // 非 LogContainer：跳过
        if (base.object_type != BL_OBJ_TYPE_LOG_CONTAINER)
        {
            const uint32_t skip = base.object_size - sizeof(ObjectHeaderBase);
            file_reader_.skip(skip);
            file_reader_.skip(align_pad_like_writer(base.object_size));
            continue;
        }

        LogContainerDiskHeader hdr{};
        hdr.base = base;

        const size_t rest =
            sizeof(LogContainerDiskHeader) - sizeof(ObjectHeaderBase);

        if (rest &&
            !file_reader_.read(reinterpret_cast<uint8_t*>(&hdr) +
                               sizeof(ObjectHeaderBase),
                               rest))
        {
            break;
        }

        const uint32_t raw =
            hdr.base.object_size - sizeof(LogContainerDiskHeader);

        std::vector<uint8_t> compressed(raw);
        if (raw && !file_reader_.read(compressed.data(), raw))
            break;

        file_reader_.skip(align_pad_like_writer(hdr.base.object_size));

        LogContainerBlock block{};
        block.compression_method = hdr.compressionMethod;
        block.uncompressed_size  = hdr.uncompressedSize;
        block.uncompressed.resize(block.uncompressed_size);

        // 解压
        if (block.compression_method == 0)
        {
            memcpy(block.uncompressed.data(),
                   compressed.data(),
                   block.uncompressed_size);
        }
        else if (block.compression_method == 2)
        {
            uLong size = block.uncompressed_size;
            if (::uncompress(block.uncompressed.data(),
                             &size,
                             compressed.data(),
                             raw) != Z_OK ||
                size != block.uncompressed_size)
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        {
            std::lock_guard lk(log_mtx_);
            log_queue_.push(std::move(block));
        }
        log_cv_.notify_one();
    }
}

void BlfLogger::read_busmsg_thread_handler()
{
    constexpr auto kWakeInterval = std::chrono::microseconds(100);

    std::unordered_map<uint32_t, std::unique_ptr<IMessageReader>> reader_cache;

    uint64_t file_start_us = 0, file_stop_us = 0;
    file_statistics_writer_.get_measure_time(file_start_us, file_stop_us);
    const uint64_t start_ns = file_start_us * 1000ULL;

    while (is_running_.load() || !log_queue_.empty())
    {
        LogContainerBlock block;

        {
            std::unique_lock lk(log_mtx_);
            log_cv_.wait_for(lk, kWakeInterval, [&] {
                return !log_queue_.empty() || !is_running_.load();
            });

            if (log_queue_.empty())
            {
                if (!is_running_.load())
                    break;
                continue;
            }

            block = std::move(log_queue_.front());
            log_queue_.pop();
        }

        const uint8_t* buf = block.uncompressed.data();
        size_t len = block.uncompressed.size();
        size_t off = 0;

        while (off + sizeof(ObjectHeaderBase) <= len)
        {
            ObjectHeaderBase base{};
            memcpy(&base, buf + off, sizeof(base));

            if (base.signature != BL_OBJ_SIGNATURE ||
                off + base.object_size > len)
            {
                ++off;   // resync
                continue;
            }

            IMessageReader* reader = nullptr;
            auto it = reader_cache.find(base.object_type);
            if (it == reader_cache.end())
            {
                auto created =
                    ReaderRegistry::instance().create(base.object_type);
                if (created)
                {
                    reader = created.get();
                    reader_cache.emplace(base.object_type,
                                         std::move(created));
                }
            }
            else
            {
                reader = it->second.get();
            }

            if (reader)
            {
                BusMessagePtr msg =
                    reader->read(buf + off, base.object_size);

                if (msg)
                {
                    // ===== 时间戳还原（核心）=====
                    if (base.header_version == 1 &&
                        base.header_size >= sizeof(ObjectHeaderBase) +
                                             sizeof(ObjectHeader))
                    {
                        ObjectHeader hdr{};
                        memcpy(&hdr,
                               buf + off + sizeof(ObjectHeaderBase),
                               sizeof(hdr));

                        uint64_t delta_ns =
                            (hdr.time_flags == BL_OBJ_FLAG_TIME_TEN_MICS)
                                ? hdr.object_timestamp * 10000ULL
                                : hdr.object_timestamp;

                        msg->set_timestamp(start_ns + delta_ns);
                    }

                    {
                        std::lock_guard qlk(msg_mtx_);
                        msg_queue_.push(std::move(msg));
                    }
                    msg_cv_.notify_one();
                }
            }

            off += base.object_size;
        }
    }
}



}
