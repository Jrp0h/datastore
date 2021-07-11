#pragma once

#include <string>
#include <fmt/format.h>

class Logger {
public:
    enum Level {
        DEBUG,
        TRACE,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    static void log_files(Level level, const char* component, std::string message, std::string file, int line, bool error = false);
    static void log(Level level, const char* component, std::string message, bool error = false);
};

#ifdef DS_DEBUG
#define LOG_DEBUG(component, ...) Logger::log_files(Logger::DEBUG, component, fmt::format(__VA_ARGS__), __FILE__, __LINE__);
#else
#define LOG_DEBUG(c, ...) ;
#endif

#define LOG_TRACE(component, ...) Logger::log(Logger::TRACE, component, fmt::format(__VA_ARGS__));
#define LOG_INFO(component, ...) Logger::log(Logger::INFO, component, fmt::format(__VA_ARGS__));
#define LOG_WARN(component, ...) Logger::log(Logger::WARN, component, fmt::format(__VA_ARGS__));
#define LOG_ERROR(component, ...) Logger::log(Logger::ERROR, component, fmt::format(__VA_ARGS__), true);
#define LOG_FATAL(component, ...) { Logger::log_files(Logger::FATAL, component, fmt::format(__VA_ARGS__), __FILE__, __LINE__, true); ::exit(1); }
