cmake_minimum_required(VERSION 3.20)

project(MonitorOne)

# ------------------------------------------------------------
# C++ Standard
# ------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ------------------------------------------------------------
# Source Files
# ------------------------------------------------------------

file(GLOB MONITOR_SOURCES
    src/Monitor/*.cpp
)


# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------

add_executable(MonitorOne
    main.cpp
    ${MONITOR_SOURCES}
)


# ------------------------------------------------------------
# Include Directories
# ------------------------------------------------------------

target_include_directories(MonitorOne PRIVATE
    src
    src/Monitor
)


# ------------------------------------------------------------
# Windows Libraries
# ------------------------------------------------------------

target_link_libraries(MonitorOne PRIVATE

    # GPU / DXGI
    dxgi

    # Windows Socket API
    ws2_32

    # IP Helper API
    iphlpapi

    # Windows WLAN API
    wlanapi

    # COM / Windows components
    ole32
)