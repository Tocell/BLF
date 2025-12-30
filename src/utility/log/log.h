/**
 * @file log.h
 * @brief 基于 spdlog 的日志系统封装
 *
 * 功能特性：
 * - 双输出：控制台 + 文件
 * - 日志回滚：可配置大小（默认300MB）
 * - 多模块管理：支持模块注册
 * - 动态级别：运行时可调整日志级别
 * - 简洁API：LOG_INFO() 等宏
 */

#ifndef LOG_H
#define LOG_H

#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

// 保留 TRANCE 和 DEBUG 级别日志代码， 必须放在 spdlog 头文件之前
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


/**
 * @class LogConfig
 * @brief 日志配置类（静态方法）
 */
class LogConfig {
public:
    /**
     * @brief 设置日志文件路径
     * @param path 日志文件存储路径（默认：当前可执行文件目录）
     */
    static void SetLogPath(const std::string& path);

    /**
     * @brief 设置单个日志文件最大大小
     * @param size_mb 文件大小（MB），默认100MB
     */
    static void SetMaxFileSize(size_t size_mb);

    /**
     * @brief 设置最大日志文件数量
     * @param max_files 最大文件数（默认3个，总共300MB）
     */
    static void SetMaxFiles(size_t max_files);

    /**
     * @brief 设置全局日志级别（影响所有logger）
     * @param level 日志级别
     * 级别: trace < debug < info < warn < error < critical
     */
    static void SetGlobalLogLevel(spdlog::level::level_enum level);

    /**
     * @brief 设置指定模块的日志级别（运行时动态调整）
     * @param module_name 模块名称
     * @param level 日志级别
     */
    static void SetModuleLogLevel(const std::string& module_name,
                                   spdlog::level::level_enum level);

    // 获取配置（内部使用）
    static std::string GetLogPath();
    static size_t GetMaxFileSize();
    static size_t GetMaxFiles();
    static spdlog::level::level_enum GetGlobalLogLevel();

    static void SetEnableFileLoad(bool enable);

    static bool IsFileLoadEnable();

private:
    static std::string log_path_;
    static size_t max_file_size_mb_;
    static size_t max_files_;
    static spdlog::level::level_enum global_level_;
    static std::mutex config_mutex_;
    static bool enable_file_load_;
};

/**
 * @class LogManager
 * @brief 日志管理器（单例）
 */
class LogManager {
public:
    /**
     * @brief 获取单例实例
     */
    static LogManager& GetInstance();

    /**
     * @brief 注册logger（为模块创建logger）
     * @param module_name 模块名称
     * @return 成功返回true，失败返回false
     */
    bool RegisterLogger(const std::string& module_name);

    /**
     * @brief 获取logger
     * @param module_name 模块名称
     * @return logger智能指针，如果不存在则返回默认logger
     */
    std::shared_ptr<spdlog::logger> GetLogger(const std::string& module_name);

    /**
     * @brief 关闭指定logger
     * @param module_name 模块名称
     */
    void ShutdownLogger(const std::string& module_name);

    /**
     * @brief 关闭所有logger
     */
    void ShutdownAll();

    /**
     * @brief 更新所有logger的级别
     * @param level 新的日志级别
     */
    void UpdateAllLogLevels(spdlog::level::level_enum level);

    /**
     * @brief 获取当前模块名（用于宏）
     */
    static const std::string& GetCurrentModule();

    /**
     * @brief 设置当前模块名（用于宏）
     */
    static void SetCurrentModule(const std::string& module_name);

private:
    LogManager();
    ~LogManager();

    // 禁止拷贝
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    /**
     * @brief 创建logger
     */
    std::shared_ptr<spdlog::logger> CreateLogger(const std::string& module_name);

private:
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers_;
    std::mutex mutex_;

    spdlog::sink_ptr shared_file_sink_;
    spdlog::sink_ptr shared_console_sink_;

    // 线程局部存储：当前模块名
    static thread_local std::string current_module_;
};

/* ============================================================================
 * 日志宏定义
 * ============================================================================ */

// 获取当前logger
#define GET_LOGGER() LogManager::GetInstance().GetLogger(LogManager::GetCurrentModule())

// 六个等级的日志宏（支持格式化参数）
#define LOG_TRACE(...)    SPDLOG_LOGGER_TRACE(GET_LOGGER(), __VA_ARGS__)
#define LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(GET_LOGGER(), __VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_LOGGER_INFO(GET_LOGGER(), __VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_LOGGER_WARN(GET_LOGGER(), __VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(GET_LOGGER(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(GET_LOGGER(), __VA_ARGS__)

// 便捷宏：设置当前模块
#define LOG_SET_MODULE(module_name) LogManager::SetCurrentModule(module_name)

// 便捷宏：注册logger
#define LOG_REGISTER(module_name) do { \
    LogManager::GetInstance().RegisterLogger(module_name); \
    LOG_SET_MODULE(module_name); \
} while(0)

// 便捷宏：关闭logger
#define LOG_SHUTDOWN(module_name) LogManager::GetInstance().ShutdownLogger(module_name)
#define LOG_SHUTDOWN_ALL() LogManager::GetInstance().ShutdownAll()

#endif // LOG_H
