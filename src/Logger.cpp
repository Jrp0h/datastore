#include "Logger.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fmt/core.h>
#include <fmt/chrono.h>

#define LEVEL_CASE(level) case level: type = "" #level; break; 

void Logger::log_files(Level level, const char* component, std::string message, std::string file, int line, bool error){
    std::string type;

    switch(level) {
        LEVEL_CASE(DEBUG)
        LEVEL_CASE(TRACE)
        LEVEL_CASE(INFO)
        LEVEL_CASE(WARN)
        LEVEL_CASE(ERROR)
        LEVEL_CASE(FATAL)
    }

    std::string ss = fmt::format("[{}] [{}] [{}:{}] [{:%H:%M:%S}] {}", type, component, file, line, fmt::localtime(std::time(nullptr)), message);

    if(error) std::cerr << ss << std::endl;
    else std::cout << ss << std::endl;

}
void Logger::log(Level level, const char* component, std::string message, bool error) {
    std::string type;

    switch(level) {
        LEVEL_CASE(DEBUG)
        LEVEL_CASE(TRACE)
        LEVEL_CASE(INFO)
        LEVEL_CASE(WARN)
        LEVEL_CASE(ERROR)
        LEVEL_CASE(FATAL)
    }

    std::string ss = fmt::format("[{}] [{}] [{:%H:%M:%S}] {}", type, component, fmt::localtime(std::time(nullptr)), message);

    if(error) std::cerr << ss << std::endl;
    else std::cout << ss << std::endl;
}
