#ifndef LOGGER_H
#define LOGGER_H
#include <memory>
#include <string>

#include "bus_message.h"
#include "message_factory.h"

namespace BLF
{

class BLF_API Logger {
public:
    // 创建记录对象
    static std::unique_ptr<Logger> create(FileFormat format);

    virtual ~Logger() = default;

    // 打开文件
    virtual bool open(const std::string& filepath, OpenMode mode) = 0;
    // 关闭文件
    virtual void close() = 0;
    // 检查打开状态
    [[nodiscard]] virtual bool is_open() const = 0;
    // 获取消息数量
    [[nodiscard]] virtual uint64_t get_message_count() const = 0;
    [[nodiscard]] virtual uint64_t get_file_size() const = 0;
    // 设置 blf 格式压缩级别，默认为 6
    virtual void set_compres_level(int32_t compres_level) = 0;
    // 设置时间戳单位
    virtual void set_timestamp_unit(int32_t unit) = 0;
    // 写入接口
    virtual bool write(BusMessagePtr msg) = 0;

    // 阻塞模式等待数据 当读取到文件末尾 返回 false
    virtual bool read(BusMessagePtr& msg) = 0;
    // 用于读取时 获取文件开始和停止时间  asc 文件没有停止时间
    virtual void get_measure_time(uint64_t& start_time, uint64_t& stop_time) = 0;
};

}

#endif //LOGGER_H
