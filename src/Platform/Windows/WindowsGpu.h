#pragma once

#include <windows.h>

#include <memory>

#include <string>

#include <cstdint>

#include <dxgi1_6.h>

#include <wrl/client.h>

#include "GPU/GpuTelemetry.h"

class WindowsGpu
{
    public:
        WindowsGpu();

        //Identification

        std :: string getGpuName();

        std :: string getVendorName();

        uint32_t getVendorId();

        uint32_t getDeviceId();

        //Dedicated Memory

        uint64_t getDedicatedMemoryTotal();

        uint64_t getDedicatedMemoryUsed();

        uint64_t getDedicatedMemoryAvailable();

        float getDedicatedMemoryUsage();

        //Shared memory

        uint64_t getSharedMemoryTotal();

        uint64_t getSharedMemoryUsed();

        uint64_t getSharedMemoryAvailable();

        //GPU Telemetry

        float getGpuUsage();

        float getGpuTemperature();

        uint32_t getFanSpeed();

        uint32_t getCoreClock();

        uint32_t getMemoryClock();

    private:

        //DXGI

        Microsoft::WRL::ComPtr<IDXGIFactory6> factory;

        Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;

        DXGI_ADAPTER_DESC3 adapterDescription{};

        //Memory Information

        DXGI_QUERY_VIDEO_MEMORY_INFO dedicatedMemoryInfo{};

        DXGI_QUERY_VIDEO_MEMORY_INFO sharedMemoryInfo{};

        bool memoryInfoValid;

        //Vendor

        enum class GpuVendor
        {
            Unknown,
            NVIDIA,
            AMD,
            Intel
        };

        GpuVendor vendor;

        //Vendor Telemetry

        std :: unique_ptr<GpuTelemetry> telemetry;

        //Initialization

        bool initialize()

        bool queryAdapter();

        void updateMemoryInfo();

        void initializeTelemetry();

        //Vendor identification

        GpuVendor identifyVendor() const;
}