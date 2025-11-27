#include "file.h"
#include <codecvt>
#include <cstring>

BLFSTART
SYSTEMTIME getCurrentSystemTime() {
    SYSTEMTIME st = {};
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    struct tm* timeinfo = localtime(&time);
    st.year = timeinfo->tm_year + 1900;
    st.month = timeinfo->tm_mon + 1;
    st.dayOfWeek = timeinfo->tm_wday;
    st.day = timeinfo->tm_mday;
    st.hour = timeinfo->tm_hour;
    st.minute = timeinfo->tm_min;
    st.second = timeinfo->tm_sec;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch() % std::chrono::seconds(1));
    st.milliseconds = ms.count();
    return st;
}

File::File() :
    object_count_(0),
    serialize_thread_running_(true),
    log_continer_write_(false),
    read_thread_running_(true)
{}

uint64_t posix_time_ms_uint64()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

uint64_t posix_time_us_uint64()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

uint64_t posix_time_ns_uint64()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

File::~File() = default;

void File::open(const char* fileName, const std::ios_base::openmode& mode)
{
    if (file_.is_open())
        return;

    file_.open(fileName, mode | std::ios::binary);
    if (!file_.is_open())
        return;

    openmode_ = mode;

    if (mode & std::ios_base::in)
    {
        file_statistics_.read(*this);
        read_thread_ = std::thread(&File::read_log_container_handler_thread, this);
    }
    else if (mode & std::ios_base::out)
    {
        file_statistics_.signature = BL_FILE_SIGNATURE;
        file_statistics_.statistics_size = file_statistics_.file_statistics_object_size();//FILE_STATISTICS_SIZE;
        file_statistics_.measurement_start_time = getCurrentSystemTime();
        file_statistics_.write(*this);

        serialize_thread_ = std::thread(&File::write_log_container_handler_thread, this);
    }
}

void File::open(const std::string& filename, const std::ios_base::openmode& mode)
{
    open(filename.c_str(), mode);
}

void File::write_log_container_handler_thread()
{
    uint32_t offset = 0;
    ObjectHeaderBase* object;
    uint64_t start_time = posix_time_us_uint64();
    while (serialize_thread_running_.load())
    {
        {
            std::unique_lock lock(queue_mutex_);
            queue_condition_.wait_for(lock, std::chrono::microseconds(10), [&]{return !object_queue_.empty() ||
                !serialize_thread_running_.load();});
            if (!serialize_thread_running_.load())
            {
                break;
            }
            if (object_queue_.empty())
            {
                continue;
            }

            tmp_queue_ = std::move(object_queue_);
        }

        while (!tmp_queue_.empty())
        {
            if (offset + 400 <= UNCOMPRESS_FILE_SIZE)
            {
                object = tmp_queue_.front();
                log_continer_write_.store(false);
                this->serialize(object, offset);
                tmp_queue_.pop_front();
                object_count_++;
                delete object;
            }
            if (offset + 400 >= UNCOMPRESS_FILE_SIZE || posix_time_us_uint64() - start_time >= 4 * 1000 * 1000)
            {
                log_container_.uncompressed_file_size = offset;
                log_container_.compressed_file_size = 0;
                log_container_.compress(2, file_statistics_.compression_level);
                log_container_.object_size = log_container_.log_container_object_size();//LOGCONTAINER_OBJECT_SIZE(log_container_.compressed_file_size);
                log_container_.header_size = log_container_.object_header_base_object_size();//OBJECT_OBJECT_BASE_OBJECT_SIZE;
                log_container_.write(*this);
                offset = 0;
                log_continer_write_.store(true);
            }
        }
        start_time = posix_time_us_uint64();
        tmp_queue_.clear();
    }
    // 线程结束将缓存中数据写入磁盘
    if (offset > 0)
    {
        log_container_.uncompressed_file_size = offset;
        log_container_.compressed_file_size = 0;
        log_container_.compress(2, file_statistics_.compression_level);
        log_container_.object_size = log_container_.log_container_object_size();//LOGCONTAINER_OBJECT_SIZE(log_container_.compressed_file_size);
        log_container_.header_size = log_container_.object_header_base_object_size();//OBJECT_OBJECT_BASE_OBJECT_SIZE;
        log_container_.write(*this);
    }
}

static int count = 0;
void File::read_log_container_handler_thread()
{
    uint32_t offset = 0;
    ObjectHeaderBase* object{};
    while (read_thread_running_.load())
    {
        if (!file_.good()) break;
        // 判断是否到达文件末尾
        if (file_.peek() == std::ifstream::traits_type::eof()) {
            printf("Reached end of file\n");
            break;
        }
        if (object_queue_.size() > 100000)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        log_container_.read(log_container_, file_);
        if (log_container_.signature != BL_OBJ_SIGNATURE)
        {
            printf("LogContainer read error\n");
            continue;
        }
        log_container_.uncompress();
        while (offset < log_container_.uncompressed_file_size)
        {
            object_count_++;
            object = reinterpret_cast<ObjectHeaderBase*>(log_container_.uncompressed_file.data() + offset);
            {
                std::unique_lock lock(queue_mutex_);
                object_queue_.push_back(object);
                queue_condition_.notify_one();
            }
            offset += object->object_size;
        }
        offset = 0;
    }
}

void File::close()
{
    if (!is_open())
        return;

    if (openmode_ & std::ios_base::in)
    {
        read_thread_running_.store(false);
        queue_condition_.notify_all();

        if (read_thread_.joinable())
            read_thread_.join();

        file_.close();
        printf("read count = %d  object_count_ = %d \n",count, object_count_);
    }

    if (openmode_ & std::ios_base::out)
    {
        queue_condition_.notify_all();
        while (!object_queue_.empty() || !tmp_queue_.empty())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        serialize_thread_running_.store(false);

        if (serialize_thread_.joinable())
            serialize_thread_.join();

        file_statistics_.file_size = static_cast<uint64_t>(file_.tellp());
        file_statistics_.uncompressed_file_size = file_statistics_.statistics_size;
        file_statistics_.object_count = object_count_;
        file_statistics_.last_object_time = getCurrentSystemTime();
        file_.seekp(0);
        file_statistics_.write(*this);
        printf("  end time : %ld object_count = %d  file_statistics_.object_count = %d \n", posix_time_ns_uint64(), object_count_, file_statistics_.object_count);
        file_.close();
    }
}



bool File::is_open() const
{
    return file_.is_open();
}

// 提供给用户调用，
// 用于存储对象
void File::write(ObjectHeaderBase* ohb)
{
    std::unique_lock lock(mem_mtx_);
    mem_cv_.wait_for(lock, std::chrono::microseconds(10) , [this]()
    {
        return object_queue_.size() <= MAX_FRAME_CNT;
    });
    lock.unlock();
    std::unique_lock lock2(queue_mutex_);
    object_queue_.emplace_back(std::move(ohb));
    queue_condition_.notify_one();
}

ObjectHeaderBase* File::read()
{
    std::unique_lock lock(queue_mutex_);
    queue_condition_.wait(lock, [&]{return !object_queue_.empty() ||
                !read_thread_running_.load();});
    if (!read_thread_running_.load() && object_queue_.empty())
        return nullptr;
    auto* object = object_queue_.front();
    object_queue_.pop_front();
    return object;
}


std::streamsize File::gcount() const
{
    return file_.gcount();
}

void File::read(char * s, std::streamsize n)
{
    file_.read(s, n);
}

std::streampos File::tellg()
{
    return file_.tellg();
}

void File::seekg(std::streamoff off, const std::ios_base::seekdir way)
{
    file_.seekg(off, way);
}

// 重写虚函数
// 写入实际物理文件接口
void File::write(const char * s, std::streamsize n)
{
    file_.write(s, n);
}

std::streampos File::tellp()
{
    return file_.tellp();
}

bool File::good() const
{
    return file_.good();
}

bool File::eof() const
{
    return file_.eof();
}

void File::serialize(ObjectHeaderBase* ohb, uint32_t& offset)
{
    // CAN/FD handle
    if (BL_OBJ_TYPE_CAN_MESSAGE == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanMessage*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_message_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_MESSAGE2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanMessage2*>(ohb);
        uint32_t parted_len1 = object->can_message2_object_first_parted_size();
        uint32_t parted_len2 = object->can_message2_object_second_parted_size();
        object->header_size = object->object_header_header_size();
        object->object_size = parted_len1 + parted_len2;
        memcpy(log_container_.uncompressed_file.data() + offset, object, parted_len1);
        offset += parted_len1;
        memcpy(log_container_.uncompressed_file.data() + offset, object + parted_len1, parted_len2);
        offset += parted_len2;
    }
    else if (BL_OBJ_TYPE_CAN_FD_MESSAGE == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanFdMessage*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->canfd_message_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_FD_MESSAGE_64 ==  ohb->object_type)
    {
        auto* object = reinterpret_cast<CanFdMessage64*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->canfd_message64_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_FD_ERROR_64 ==  ohb->object_type)
    {
        auto* object = reinterpret_cast<CanFdErrorFrame64*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->canfd_error_frame64_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_DRIVER_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanDriverError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_driver_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_DRIVER_ERROR_EXT == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanDriverErrorExt*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_driver_error_ext_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_DRIVER_SYNC == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanDriverHWSync*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_driver_hw_sync_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_STATISTIC == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanDriverStatistic*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_driver_statistic_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanErrorFrame*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_error_frame_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_ERROR_EXT == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanErrorFrameExt*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_error_frame_ext_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_OVERLOAD == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanOverloadFrame*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_overload_frame_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_CAN_SETTING_CHANGED == ohb->object_type)
    {
        auto* object = reinterpret_cast<CanSettingChanged*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->can_setting_changed_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }


    // Flexray handle
    else if (BL_OBJ_TYPE_FLEXRAY_DATA == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayData*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_data_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FLEXRAY_STATUS == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayStatusEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_status_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FLEXRAY_SYNC == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexraySync*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_sync_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FLEXRAY_MESSAGE == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayV6Message*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_v6_message_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FLEXRAY_CYCLE == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexRayV6StartCycleEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_v6_start_cycle_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FR_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayVFrError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_vfr_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FR_RCVMESSAGE == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayVFrReceiveMsg*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_vfr_receive_msg_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FR_RCVMESSAGE_EX == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayVFrReceiveMsgEx*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_vfr_receive_msg_ex_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FR_STARTCYCLE == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayVFrStartCycle*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_vfr_start_cycle_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_FR_STATUS == ohb->object_type)
    {
        auto* object = reinterpret_cast<FlexrayVFrStatus*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->flexray_vfr_status_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }


    // Lin handle
    else if (BL_OBJ_TYPE_LIN_BAUDRATE == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinBaudrateEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_baudrate_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_CHECKSUM_INFO == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinChecksumInfo*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_checksum_info_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_CRC_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinCrcError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_crc_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_CRC_ERROR2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinCrcError2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_crc_error2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_DISTURBANCE_EVENT == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinDisturbanceEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_disturbance_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_DLC_INFO == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinDlcInfo*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_dlc_info_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_LONG_DOM_SIG == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinLongDomSignalEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_long_dom_signal_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_LONG_DOM_SIG2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinLongDomSignalEvent2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_long_dom_signal_event2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_MESSAGE == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinMessage*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_message_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_MESSAGE2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinMessage2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_message2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_RCV_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinReceiveError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_receive_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_RCV_ERROR2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinReceiveError2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_receive_error2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SCHED_MODCH == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSchedulerModeChange*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_scheduler_mode_change_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SND_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSendError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_send_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SND_ERROR2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSendError2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_send_error2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SHORT_OR_SLOW_RESPONSE == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinShortOrSlowResponse*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_short_or_slow_response_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SHORT_OR_SLOW_RESPONSE2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinShortOrSlowResponse2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_short_or_slow_response2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SLV_TIMEOUT == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSlaveTimeout*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_slave_timeout_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SLEEP == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSleepModeEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_sleep_mode_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SPIKE_EVENT == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSpikeEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_spike_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SPIKE_EVENT2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSpikeEvent2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_spike_event2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_STATISTIC == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinStatisticEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_statistic_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SYN_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSyncError*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_sync_error_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_SYN_ERROR == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinSyncError2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_sync_error2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_UNEXPECTED_WAKEUP == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinUnexpectedWakeup*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_unexpected_wakeup_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_WAKEUP == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinWakeupEvent*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_wakeup_event_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else if (BL_OBJ_TYPE_LIN_WAKEUP2 == ohb->object_type)
    {
        auto* object = reinterpret_cast<LinWakeupEvent2*>(ohb);
        object->header_size = object->object_header_header_size();
        object->object_size = object->lin_wakeup_event2_object_size();
        memcpy(log_container_.uncompressed_file.data() + offset, object, object->object_size);
        offset += object->object_size;
    }
    else
    {
        std::cout << "object type error\n";
    }
}

uint64_t File::get_object_queue_size()
{
    std::unique_lock lock(queue_mutex_);
    return object_queue_.size() + tmp_queue_.size();
}

uint64_t File::get_object_count() const
{
    return object_count_;
}

FileStatistics File::read_file_statistics() const
{
    return file_statistics_;
}
BLFEND