#pragma once

#include <cstdint>

#include <string>

class GpuMonitor
{
    public:
        GpuMonitor();

        //Identification
        std :: string getGpuName();

        std :: string getVendorName();

        uint32_t getVendorId();

        uint32_t getDeviceId();

        //Dedicated GPU Memory

        uint64_t getDedicatedMemoryTotal();

        uint64_t getDedicatedMemoryUsed();

        uint64_t getDedicatedMemoryAvailable();

        //Shared GPU Memory

        uint64_t getSharedMemoryTotal();

        uint64_t getSharedMemoryUsed();

        uint64_t getSharedMemoryAvailable();

        //float getDedicatedMemoryUsage();
        
        //GPU Usage

        float getGpuUsage();

        //GPU Temperature

        float getTemperature();

        //GPU Fan

        float getFanSpeed();

        //GPU Clocks

        float getCoreClock();

        float getMemoryClock();
}