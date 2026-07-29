cmake_minimum_required(VERSION 3.20)

project(MonitorOne)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB MONITOR_SOURCES
src/Monitor/*.cpp
)

add_executable(MonitorOne
main.cpp
${MONITOR_SOURCES}
)

target_include_directories(MonitorOne PRIVATE
src/Monitor
)