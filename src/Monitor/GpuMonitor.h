#pragma once

#include<cstdint>
#include<string>

class GpuMonitor
{
    public:
    GpuMonitor();
    ~GpuMonitor();

    //Returns GPU usage percentage
    float getGpuUsage();

    //Dedicated VRAM
    uint64_t getDedicatedMemoryUsed();
    uint64_t getDedicatedMemoryTotal();

    //Shared system memory used by the GPU
    uint64_t getSharedMemoryUsed();
    uint64_t getSharedMemoryTotal();

    //GPU temperature, core clock, memory clock, fan speed, vendor, driver version
    
    float getTemperature();
    float getCoreClock();
    float getMemoryClock();
    float getFanSpeed();

    //GPU information

    //const char* getGpuName();
    std::string getGpuName();
    std::string getVendorName();
    std::string getDriverVersion();

    uint32_t getVendorId();
    uint32_t getDeviceId();
};