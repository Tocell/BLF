/**
 * @file log.cpp
 * @brief 日志系统实现
 */

#include "log.h"
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <libgen.h>
#include <linux/limits.h>
#endif

// 静态成员初始化
std::string LogConfig::log_path_ = "";
size_t LogConfig::max_file_size_mb_ = 100;  // 默认100MB
size_t LogConfig::max_files_ = 3;             // 默认3个文件
spdlog::level::level_enum LogConfig::global_level_ = spdlog::level::info;
std::mutex LogConfig::config_mutex_;
bool LogConfig::enable_file_load_ = false;

void LogConfig::SetLogPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    log_path_ = path;

    // 创建目录（如果不存在）
    if (!path.empty()) {
        try {
            std::filesystem::create_directories(path);
        } catch (const std::exception& e) {
            std::cerr << "创建日志目录失败: " << e.what() << std::endl;
        }
    }
}

void LogConfig::SetMaxFileSize(size_t size_mb) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    max_file_size_mb_ = size_mb;
}

void LogConfig::SetMaxFiles(size_t max_files) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    max_files_ = max_files;
}

void LogConfig::SetGlobalLogLevel(spdlog::level::level_enum level) {
    {
        std::lock_guard<std::mutex> lock(config_mutex_);
        global_level_ = level;

        // 1. 设置 spdlog 全局默认级别（影响新logger）
        spdlog::set_level(level);
    }

    // 2. 更新所有已存在的logger的级别
    LogManager::GetInstance().UpdateAllLogLevels(level);
}

void LogConfig::SetModuleLogLevel(const std::string& module_name,
                                   spdlog::level::level_enum level) {
    auto logger = LogManager::GetInstance().GetLogger(module_name);
    if (logger) {
        logger->set_level(level);
    }
}

std::string LogConfig::GetLogPath() {
    std::lock_guard<std::mutex> lock(config_mutex_);

    // 如果未设置，使用可执行文件目录
    if (log_path_.empty()) {
#ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string exe_path(buffer);
        size_t pos = exe_path.find_last_of("\\/");
        return exe_path.substr(0, pos) + "\\logs";
#else
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            std::string exe_path = dirname(buffer);
            return exe_path + "/logs";
        }
        return "./logs";  // 备用方案
#endif
    }

    return log_path_;
}

size_t LogConfig::GetMaxFileSize() {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return max_file_size_mb_;
}

size_t LogConfig::GetMaxFiles() {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return max_files_;
}

spdlog::level::level_enum LogConfig::GetGlobalLogLevel() {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return global_level_;
}

void LogConfig::SetEnableFileLoad(bool enable)
{
    std::lock_guard lock(config_mutex_);
    enable_file_load_ = enable;
}

bool LogConfig::IsFileLoadEnable()
{
    std::lock_guard lock(config_mutex_);
    return enable_file_load_;
}

/* ============================================================================
 * LogManager 实现
 * ============================================================================ */

// 线程局部存储
thread_local std::string LogManager::current_module_ = "default";

LogManager::LogManager() {
    // std::string log_path = LogConfig::GetLogPath();
    try
    {
        shared_console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        // std::filesystem::create_directories(log_path);
    } catch (const spdlog::spdlog_ex& e)
    {
        std::cerr << "控制台日志初始化失败：" << e.what() << std::endl;
    }

    if (LogConfig::IsFileLoadEnable())
    {
        std::string log_path = LogConfig::GetLogPath();

        try
        {
            if (!log_path.empty()) std::filesystem::create_directories(log_path);
        } catch (...) {}

        std::string log_file = log_path + "/applog.log";
        size_t max_size = LogConfig::GetMaxFileSize() * 1024 * 1024;
        size_t max_files = LogConfig::GetMaxFiles();

        try
        {
            shared_file_sink_ = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                log_file, max_size, max_files);
        } catch (const spdlog::spdlog_ex& e)
        {
            std::cerr << "文件日志初始化失败：" << e.what() << std::endl;
        }
    }
    else
    {
        shared_file_sink_ = nullptr;
    }

    spdlog::flush_on(spdlog::level::info);

    RegisterLogger("default");



    // std::string log_file = log_path + "/applog.log";
    // size_t max_size = LogConfig::GetMaxFileSize() * 1024 * 1024;
    // size_t max_files = LogConfig::GetMaxFiles();
    //
    // try
    // {
    //     shared_file_sink_ = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
    //         log_file, max_size, max_files);
    //
    //     shared_console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // } catch (const spdlog::spdlog_ex& e)
    // {
    //     std::cerr << "LogManager 初始化失败: " << e.what() << std::endl;
    // }
    //
    // // 设置全局刷新策略（warn及以上立即刷新）
    // spdlog::flush_on(spdlog::level::warn);
    //
    // // 创建默认logger
    // RegisterLogger("default");
}

LogManager::~LogManager() {
    ShutdownAll();
}

LogManager& LogManager::GetInstance() {
    static LogManager instance;
    return instance;
}

std::shared_ptr<spdlog::logger> LogManager::CreateLogger(const std::string& module_name) {
    // if (!shared_file_sink_ || !shared_console_sink_)
    // {
    //     return nullptr;
    // }

    try {
        std::vector<spdlog::sink_ptr> sinks;

        if (shared_file_sink_)
        {
            sinks.push_back(shared_file_sink_);
        }

        if (shared_console_sink_)
        {
            sinks.push_back(shared_console_sink_);
        }

        if (sinks.empty())
        {
            return nullptr;
        }

        auto logger = std::make_shared<spdlog::logger>(module_name, sinks.begin(), sinks.end());

        // 设置级别
        logger->set_level(LogConfig::GetGlobalLogLevel());

        // spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%t] [%l] [%s %@] %v");
        // 设置模式
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%t] %^[%l]%$ [%s:%#] %v");

        // 注册到 spdlog（可选，便于全局访问）
        spdlog::register_logger(logger);

        return logger;

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "创建logger失败: " << ex.what() << std::endl;
        return nullptr;
    }
}

bool LogManager::RegisterLogger(const std::string& module_name) {
    std::lock_guard<std::mutex> lock(mutex_);

    // 检查是否已存在
    if (loggers_.find(module_name) != loggers_.end()) {
        std::cout << "Logger '" << module_name << "' 已存在" << std::endl;
        return true;
    }

    // 创建logger
    auto logger = CreateLogger(module_name);
    if (!logger) {
        return false;
    }

    // 保存
    loggers_[module_name] = logger;

    return true;
}

std::shared_ptr<spdlog::logger> LogManager::GetLogger(const std::string& module_name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = loggers_.find(module_name);
    if (it != loggers_.end()) {
        return it->second;
    }

    // 如果不存在，返回默认logger
    auto default_it = loggers_.find("default");
    if (default_it != loggers_.end()) {
        return default_it->second;
    }

    return nullptr;
}

void LogManager::ShutdownLogger(const std::string& module_name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = loggers_.find(module_name);
    if (it != loggers_.end()) {
        // 刷新并关闭
        it->second->flush();
        spdlog::drop(module_name);
        loggers_.erase(it);
    }
}

void LogManager::ShutdownAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    // 刷新所有logger
    for (auto& pair : loggers_) {
        pair.second->flush();
    }

    // 关闭所有
    spdlog::shutdown();
    loggers_.clear();
}

void LogManager::UpdateAllLogLevels(spdlog::level::level_enum level) {
    std::lock_guard<std::mutex> lock(mutex_);

    // 遍历并更新所有logger的级别
    for (auto& pair : loggers_) {
        pair.second->set_level(level);
    }
}

const std::string& LogManager::GetCurrentModule() {
    return current_module_;
}

void LogManager::SetCurrentModule(const std::string& module_name) {
    current_module_ = module_name;
}


