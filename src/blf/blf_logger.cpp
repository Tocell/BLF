#include "blf_logger.h"
#include "can/can_message_blf_writer.h"

#include <iostream>

#include "can_message.h"
#include "blf_object_header.h"
#include "../include/message_factory.h"
#include "../api/imessage_reader.h"
#include "../registry/reader_registry.h"
#include "../registry/reader_registrar.h"

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
		file_reader_.read_struct(file_stats);
		file_statistics_writer_.update_file_statistics(file_stats);

		is_running_.store(true);
		read_logcontainer_thread_ = std::thread([this]()
		{
			constexpr auto kWakeInterval = std::chrono::microseconds(100);
			while (is_running_.load())
			{
				if (!file_reader_.is_open())
					break;
				{
					std::unique_lock lock(log_mtx_);
					log_cv_.wait_for(lock, kWakeInterval, [this]()
					{
						return (log_queue_.size() < 3) || !is_running_.load();
					});
					if (!is_running_.load())
						break;
					if (log_queue_.size() >= 3)
						continue;
				}

				ObjectHeaderBase base{};
				if (!file_reader_.read_struct(base))
				{
					is_running_.store(false);
					log_cv_.notify_all();
					msg_cv_.notify_all();
					break;
				}
				if (base.signature != BL_OBJ_SIGNATURE || base.object_size < sizeof(ObjectHeader))
				{
					continue;
				}

				if (base.object_type != BL_OBJ_TYPE_LOG_CONTAINER)
				{
					const uint32_t remain = base.object_size - static_cast<uint32_t>(sizeof(ObjectHeaderBase));

					if (remain && !file_reader_.skip(remain))
					{
						is_running_.store(false);
						log_cv_.notify_all();
						msg_cv_.notify_all();
						break;
					}

					const uint32_t pad = align_pad_like_writer(base.object_size);
					if (pad && !file_reader_.skip(pad))
					{
						is_running_.store(false);
						log_cv_.notify_all();
						msg_cv_.notify_all();
						break;
					}
					continue;
				}

				LogContainerDiskHeader hdr{};
				hdr.base = base;
				const size_t rest = sizeof(LogContainerDiskHeader) - sizeof(ObjectHeaderBase);
				if (rest)
				{
					if (!file_reader_.read(reinterpret_cast<uint8_t*>(&hdr) + sizeof(ObjectHeaderBase), rest))
					{
						is_running_.store(false);
						log_cv_.notify_all();
						msg_cv_.notify_all();
						break;
					}
				}
				if (hdr.base.object_size < sizeof(LogContainerDiskHeader))
					continue;

				const uint32_t raw = hdr.base.object_size - static_cast<uint32_t>(sizeof(LogContainerDiskHeader));

				auto& lc = log_container_.get_logcontainer();
				lc.compression_method     = hdr.compressionMethod;
				lc.uncompressed_file_size = hdr.uncompressedSize;
				lc.compressed_file_size   = raw;

				if (raw > 0)
				{
					if (!file_reader_.read(lc.compressed_file, raw))
					{
						is_running_.store(false);
						log_cv_.notify_all();
						msg_cv_.notify_all();
						break;
					}
				}

				// padding
				const uint32_t pad = align_pad_like_writer(hdr.base.object_size);
				if (pad && !file_reader_.skip(pad))
				{
					is_running_.store(false);
					log_cv_.notify_all();
					msg_cv_.notify_all();
					break;
				}

				// 解压
				log_container_.uncompress();
				if (lc.uncompressed_file == nullptr || lc.uncompressed_file_size == 0)
					continue;

				// push log_queue_
				{
					std::lock_guard lk(log_mtx_);
					log_queue_.push(std::move(lc));
				}
				log_cv_.notify_one();
			}
		});

		read_busmsg_thread_ = std::thread([this]()
		{
#if 0
			constexpr auto kWakeInterval = std::chrono::microseconds(100);

		    // 缓存：object_type -> reader
		    std::unordered_map<uint32_t, std::unique_ptr<IMessageReader>> reader_cache;

		    while (is_running_.load() || !log_queue_.empty())
		    {
		        LogContainer lc;
		        {
		            std::unique_lock lk(log_mtx_);
		            log_cv_.wait_for(lk, kWakeInterval, [this]()
		            {
		                return !log_queue_.empty() || !is_running_.load();
		            });

		            if (log_queue_.empty())
		            {
		                if (!is_running_.load())
		                    break;
		                continue;
		            }

		            lc = std::move(log_queue_.front());
		            log_queue_.pop();
		        }
		        log_cv_.notify_one(); // 释放 producer（log_queue_ 变小了）

		        if (lc.uncompressed_file == nullptr || lc.uncompressed_file_size == 0)
		            continue;

		        const uint8_t* buf = lc.uncompressed_file;
		        const uint64_t len = lc.uncompressed_file_size;

		        uint64_t off = 0;
		        while (off + sizeof(ObjectHeaderBase) <= len)
		        {
		            ObjectHeaderBase base{};
		            std::memcpy(&base, buf + off, sizeof(ObjectHeaderBase));

		            // 基本校验
		            if (base.signature != BL_OBJ_SIGNATURE ||
		                base.object_size < sizeof(ObjectHeaderBase) ||
		                off + base.object_size > len)
		            {
		                // 当前 logcontainer 内部流错位/损坏：放弃这个 container
		                break;
		            }

		            const uint8_t* obj_begin = buf + off;
		            const uint32_t obj_size  = base.object_size;

		            const uint8_t* payload   = obj_begin + sizeof(ObjectHeaderBase);
		            const uint32_t payload_sz = obj_size - static_cast<uint32_t>(sizeof(ObjectHeaderBase));

		            // 从注册表取 reader（带缓存）
		            IMessageReader* reader = nullptr;
		            {
		                auto it = reader_cache.find(base.object_type);
		                if (it == reader_cache.end())
		                {
		                    auto created = ReaderRegistry::instance().create(base.object_type);
		                    if (created)
		                    {
		                        reader = created.get();
		                        reader_cache.emplace(base.object_type, std::move(created));
		                    }
		                }
		                else
		                {
		                    reader = it->second.get();
		                }
		            }

		            if (reader)
		            {
		                BusMessagePtr msg(nullptr);
		                const bool ok = decode_with_reader(*reader, base, obj_begin, obj_size, payload, payload_sz, msg);

		                if (ok && msg)
		                {
		                    // 入队 msg_queue_
		                    // 注意：如果你的主线程 pop 后不会 notify，这里不要用 cv 等待满队列，简单 sleep 限流最稳
		                    for (;;)
		                    {
		                        {
		                            std::unique_lock qlk(msg_mtx_);
		                            if (msg_queue_.size() < MAX_FRAME_CACHE_COUNT)
		                            {
		                                msg_queue_.push(std::move(msg));
		                                break;
		                            }
		                        }
		                        std::this_thread::sleep_for(std::chrono::microseconds(10));
		                    }
		                    msg_cv_.notify_one();
		                }
		            }

		            // 下一个对象（含 padding）
		            off += obj_size;
		            const uint32_t pad = align_pad_like_writer(obj_size);
		            if (pad)
		            {
		                if (off + pad > len) break;
		                off += pad;
		            }
		        }
		    }


#endif
		});
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
	if (!msg_queue_.empty())
	{
		msg = std::move(msg_queue_.front());
		msg_queue_.pop();
	}
	else
	{
		msg = nullptr;
	}
}

void BlfLogger::get_measure_time(uint64_t& start_time, uint64_t& stop_time)
{
	file_statistics_writer_.get_measure_time(start_time, stop_time);
}

uint32_t BlfLogger::align_pad_like_writer(uint32_t object_size)
{
	return (4u - (object_size % 4)) % 4u;
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

}
