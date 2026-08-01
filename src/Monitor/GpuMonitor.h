#pragma once

#include<cstdint>

#include<string>

#include <dxgi1_6.h> //DXGI 1.6 Interfacess

#include <wrl/client.h> //provides Microsoft::WRL::ComPtr

class GpuMonitor
{
    public:
    GpuMonitor();
    ~GpuMonitor();
    
    //Returns GPU usage percentage
    float getGpuUsage();

    //GPU temperature, core clock, memory clock, fan speed, vendor, driver version
    float getTemperature();
    float getCoreClock();
    float getMemoryClock();
    float getFanSpeed();

    std::string getGpuName();
    std::string getVendorName();
    std::string getDriverVersion();
    
    //Dedicated VRAM
    uint64_t getDedicatedMemoryUsed();
    uint64_t getDedicatedMemoryTotal();
    
    

    
    //Shared system memory used by the GPU
    uint64_t getSharedMemoryUsed();
    uint64_t getSharedMemoryTotal();
    
    uint32_t getVendorId();
    uint32_t getDeviceId();

    private:

    Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
    Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;

    std::string gpuName;
    std::string vendorName;
    std::string driverVersion;

    uint64_t dedicatedMemoryTotal;
    uint64_t sharedMemoryTotal;
    

    uint32_t vendorId;
    uint32_t deviceId;

    void initializeDXGI();

    
    //GPU information

    //const char* getGpuName();
    

};