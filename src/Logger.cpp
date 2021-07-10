#include "Logger.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>

void Logger::log_files(Level level, const char* component, const char* message, std::string file, int line, bool error){
    std::stringstream ss;
    
    ss << "[";

    switch(level) {
        case Level::DEBUG:
            ss << "DEBUG";
            break;
        case Level::TRACE:
            ss << "TRACE";
            break;
        case Level::INFO:
            ss << "INFO";
            break;
        case Level::WARN:
            ss << "WARN";
            break;
        case Level::ERROR:
            ss << "ERROR";
            break;
        case Level::FATAL:
            ss << "FATAL";
            break;
    }

    ss << "] [" << component << "] [" << file << ":" << line << "] [";

    auto t = std::time(nullptr);
    ss << std::put_time(std::localtime(&t), "%H:%M:%S") << "] ";
    ss << message << std::endl;

    if(error)
        std::cerr << ss.str();
    else
        std::cout << ss.str();

}
void Logger::log(Level level, const char* component, const char* message, bool error) {
    std::stringstream ss;
    
    ss << "[";

    switch(level) {
        case Level::DEBUG:
            ss << "DEBUG";
            break;
        case Level::TRACE:
            ss << "TRACE";
            break;
        case Level::INFO:
            ss << "INFO";
            break;
        case Level::WARN:
            ss << "WARN";
            break;
        case Level::ERROR:
            ss << "ERROR";
            break;
        case Level::FATAL:
            ss << "FATAL";
            break;
    }

    ss << "] [" << component << "] [";

    auto t = std::time(nullptr);
    ss << std::put_time(std::localtime(&t), "%H:%M:%S") << "] ";
    ss << message << std::endl;

    if(error)
        std::cerr << ss.str();
    else
        std::cout << ss.str();
}
