#pragma once

#include <string>

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

    static void log_files(Level level, const char* component, const char* message, std::string file, int line, bool error = false);
    static void log(Level level, const char* component, const char* message, bool error = false);
};

#ifdef DS_DEBUG
#define LOG_DEBUG(component, message) Logger::log_files(Logger::DEBUG, component, message, __FILE__, __LINE__);
#else
#define LOG_DEBUG(c, m) ;
#endif

#define LOG_TRACE(component, message) Logger::log(Logger::TRACE, component, message);
#define LOG_INFO(component, message) Logger::log(Logger::INFO, component, message);
#define LOG_WARN(component, message) Logger::log(Logger::WARN, component, message);
#define LOG_ERROR(component, message) Logger::log(Logger::ERROR, component, message, true);
#define LOG_FATAL(component, message) { Logger::log_files(Logger::FATAL, component, message, __FILE__, __LINE__, true); ::exit(1); }
