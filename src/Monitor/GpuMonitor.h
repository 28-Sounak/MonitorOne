#pragma once

#include <cstdint>
#include <string>

#include <dxgi1_6.h>
#include <wrl/client.h>

#ifdef _WIN32
#include <windows.h>
#include <pdh.h>
#endif


class GpuMonitor
{
public:

    GpuMonitor();
    ~GpuMonitor();


    // ========================================================
    // GPU Usage
    // ========================================================

    float getGpuUsage();


    // ========================================================
    // GPU Telemetry
    // ========================================================

    float getTemperature();

    float getCoreClock();

    float getMemoryClock();

    float getFanSpeed();


    // ========================================================
    // GPU Information
    // ========================================================

    std::string getGpuName();

    std::string getVendorName();

    std::string getDriverVersion();


    // ========================================================
    // GPU Memory
    // ========================================================

    uint64_t getDedicatedMemoryUsed();

    uint64_t getDedicatedMemoryTotal();

    uint64_t getSharedMemoryUsed();

    uint64_t getSharedMemoryTotal();


    // ========================================================
    // Device Identification
    // ========================================================

    uint32_t getVendorId();

    uint32_t getDeviceId();


private:

    // ========================================================
    // DXGI
    // ========================================================

    Microsoft::WRL::ComPtr<IDXGIFactory6> factory;

    Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;


    // ========================================================
    // Cached GPU Information
    // ========================================================

    std::string gpuName;

    std::string vendorName;

    std::string driverVersion;


    uint64_t dedicatedMemoryTotal = 0;

    uint64_t dedicatedMemoryUsed = 0;


    uint64_t sharedMemoryTotal = 0;

    uint64_t sharedMemoryUsed = 0;


    uint32_t vendorId = 0;

    uint32_t deviceId = 0;


    // ========================================================
    // Windows GPU Performance Counters
    // ========================================================

#ifdef _WIN32

    struct PDH_HQUERY__* gpuPdhQuery = nullptr;

    struct PDH_HCOUNTER__* gpuUtilizationCounter = nullptr;

    struct PDH_HCOUNTER__* gpuMemoryCounter = nullptr;

#endif

    bool gpuCountersInitialized = false;


    // ========================================================
    // Cached Runtime GPU Usage
    // ========================================================

    float gpuUsage = 0.0f;


    // ========================================================
    // COM
    // ========================================================

    bool comInitialized = false;


    // ========================================================
    // NVIDIA NVML
    // ========================================================

#ifdef _WIN32

    HMODULE nvmlLibrary = nullptr;

#endif

    void* nvmlDevice = nullptr;

    bool nvmlInitialized = false;


    // ========================================================
    // NVML Types
    // ========================================================

    using nvmlReturn_t = int;


    using nvmlInit_v2_t =
        nvmlReturn_t (*)();


    using nvmlShutdown_t =
        nvmlReturn_t (*)();


    using nvmlDeviceGetHandleByIndex_v2_t =
        nvmlReturn_t (*)
        (
            unsigned int,
            void**
        );


    using nvmlDeviceGetTemperature_t =
        nvmlReturn_t (*)
        (
            void*,
            int,
            unsigned int*
        );


    using nvmlDeviceGetClockInfo_t =
        nvmlReturn_t (*)
        (
            void*,
            int,
            unsigned int*
        );


    using nvmlDeviceGetFanSpeed_t =
        nvmlReturn_t (*)
        (
            void*,
            unsigned int*
        );


    using nvmlDeviceGetUtilizationRates_t =
        nvmlReturn_t (*)
        (
            void*,
            unsigned int*,
            unsigned int*
        );


    using nvmlDeviceGetMemoryInfo_t =
        nvmlReturn_t (*)
        (
            void*,
            uint64_t*,
            uint64_t*,
            uint64_t*
        );


    using nvmlSystemGetDriverVersion_t =
        nvmlReturn_t (*)
        (
            char*,
            unsigned int
        );


    // ========================================================
    // Loaded NVML Functions
    // ========================================================

    nvmlInit_v2_t nvmlInit_v2 = nullptr;

    nvmlShutdown_t nvmlShutdown = nullptr;


    nvmlDeviceGetHandleByIndex_v2_t
        nvmlDeviceGetHandleByIndex_v2 = nullptr;


    nvmlDeviceGetTemperature_t
        nvmlDeviceGetTemperature = nullptr;


    nvmlDeviceGetClockInfo_t
        nvmlDeviceGetClockInfo = nullptr;


    nvmlDeviceGetFanSpeed_t
        nvmlDeviceGetFanSpeed = nullptr;


    nvmlDeviceGetUtilizationRates_t
        nvmlDeviceGetUtilizationRates = nullptr;


    nvmlDeviceGetMemoryInfo_t
        nvmlDeviceGetMemoryInfo = nullptr;


    nvmlSystemGetDriverVersion_t
        nvmlSystemGetDriverVersion = nullptr;


    // ========================================================
    // Initialization
    // ========================================================

    void initializeDXGI();

    void initializeNVML();

#ifdef _WIN32

    void initializeGpuPerformanceCounters();

#endif


    // ========================================================
    // Runtime Updates
    // ========================================================

    void updateGpuUsage();

    void updateGpuMemoryUsage();


    // ========================================================
    // Cleanup
    // ========================================================

    void shutdownNVML();


    // ========================================================
    // Helpers
    // ========================================================

    std::string wideToString(
        const wchar_t* value
    );


    bool loadNVMLFunctionPointers();


    bool isNvidiaGPU() const;
};