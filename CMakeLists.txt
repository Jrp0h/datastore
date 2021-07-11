cmake_minimum_required(VERSION 3.5)

project(datastore)

include_directories(include/Datastore)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)

    # Enable color output
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    else()
        add_compile_options (-fcolor-diagnostics)
    endif()
endif()

set(SOURCE_FILES 
    src/main.cpp
    src/Datastore.cpp
    src/TTL.cpp
    src/Record.cpp
    src/Table.cpp
    src/Logger.cpp

    # Language
    src/Language/Token.cpp
    src/Language/Action.cpp
    src/Language/Parser.cpp
    src/Language/Lexer.cpp

    # Networking
    src/Network/Server.cpp
    src/Network/Response.cpp

    # Datastructures
    src/Datastructure/LinkedList.cpp
)

if(DS_DEBUG)
    message(STATUS "Building for debug, add -DDS_DEBUG=false to remove debug mode")
    add_compile_options(-DDS_DEBUG)
else()
    message(STATUS "Building for release, add -DDS_DEBUG=true to set debug mode")
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3)
    endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# External
add_subdirectory(vendor/fmt)
target_include_directories(${PROJECT_NAME} PUBLIC vendor/fmt/include)
target_link_libraries(${PROJECT_NAME} fmt::fmt)

find_package (Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build/compile_commands.json)
    file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/build/compile_commands.json 
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/)
endif()
