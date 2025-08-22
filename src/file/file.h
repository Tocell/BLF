#ifndef FILE_H
#define FILE_H

#include <atomic>
#include <ios>
#include <queue>
#include <fstream>
#include <mutex>
#include <iostream>

#include "file_statistics.h"
#include "object_header_base.h"
#include "log_container.h"

#include "common.h"
#include "canfd_message.h"
#include "canfd_message64.h"
#include "can_message.h"
#include "can_driver_error.h"
#include "can_driver_error_ext.h"
#include "can_driver_hw_sync.h"
#include "can_driver_statistic.h"
#include "can_error_frame.h"
#include "can_error_frame_ext.h"
#include "can_message2.h"
#include "can_overload_frame.h"
#include "can_setting_changed.h"
#include "canfd_error_frame64.h"

#include "flexray_data.h"
#include "flexray_status_event.h"
#include "flexray_sync.h"
#include "flexray_v6_message.h"
#include "flexray_v6_start_cycle_event.h"
#include "flexray_vfr_error.h"
#include "flexray_vfr_start_cycle.h"
#include "flexray_vfr_status.h"
#include "flexray_vfr_receive_msg.h"
#include "flexray_vfr_receive_msg_ex.h"

#include "lin_baudrate_event.h"
#include "lin_bus_event.h"
#include "lin_check_suminfo.h"
#include "lin_crc_error.h"
#include "lin_crc_error2.h"
#include "lin_databyte_timestamp_event.h"
#include "lin_disturbance_event.h"
#include "lin_dlc_info.h"
#include "lin_long_dom_signal_event.h"
#include "lin_long_dom_signal_event2.h"
#include "lin_message.h"
#include "lin_message2.h"
#include "lin_message_descriptor.h"
#include "lin_receive_error.h"
#include "lin_receive_error2.h"
#include "lin_scheduler_mode_change.h"
#include "lin_send_error.h"
#include "lin_send_error2.h"
#include "lin_short_or_slow_response.h"
#include "lin_short_or_slow_response2.h"
#include "lin_slave_timeout.h"
#include "lin_sleep_mode_event.h"
#include "lin_spike_event.h"
#include "lin_spike_event2.h"
#include "lin_statistic_event.h"
#include "lin_sync_error.h"
#include "lin_sync_error2.h"
#include "lin_synch_field_event.h"
#include "lin_unexpected_wakeup.h"
#include "lin_wakeup_event.h"
#include "lin_wakeup_event2.h"

BLFSTART

class TWBLF_API File final : public AbstractFile
{
public:
    File();
    ~File() override;

    virtual void open(const char* fileName, const std::ios_base::openmode& mode );
    virtual void open(const std::string& filename, const std::ios_base::openmode& mode = std::ios_base::in );
    virtual void close();
    [[nodiscard]] bool is_open() const;

    uint64_t get_object_queue_size();
    [[nodiscard]] uint64_t get_object_count() const;

    virtual void write(ObjectHeaderBase * ohb);

    [[nodiscard]] std::streamsize gcount() const override;
    void read(char * s, std::streamsize n) override;
    ObjectHeaderBase* read();
    std::streampos tellg() override;
    void seekg(std::streamoff off, std::ios_base::seekdir way) override;
    void write(const char * s, std::streamsize n) override;
    std::streampos tellp() override;
    [[nodiscard]] bool good() const override;
    [[nodiscard]] bool eof() const override;
    void serialize(ObjectHeaderBase* ohb, uint32_t& offset) override;

    void write_log_container_handler_thread();
    void read_log_container_handler_thread();

    [[nodiscard]] FileStatistics read_file_statistics() const;

public:
    FileStatistics                      file_statistics_;
    uint32_t                            object_count_;
    std::ios_base::openmode             openmode_{};

private:
    std::fstream                        file_;
    std::deque<ObjectHeaderBase*>       object_queue_;
    std::deque<ObjectHeaderBase*>       tmp_queue_;
    std::mutex                          queue_mutex_;
    std::condition_variable             queue_condition_;

    std::mutex                          mem_mtx_;
    std::condition_variable             mem_cv_;

    std::thread                         serialize_thread_;
    std::atomic<bool>                   serialize_thread_running_;

    std::thread                         read_thread_;
    std::atomic<bool>                   read_thread_running_;

    LogContainer                        log_container_;
    std::atomic<bool>                   log_continer_write_;
};

BLFEND
#endif //FILE_H
