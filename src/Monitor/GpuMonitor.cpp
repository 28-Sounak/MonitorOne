#include "GpuMonitor.h"

#include <windows.h>

#include <stdexcept>
//#include <comdef.h>

bool comInitialized = false;

std::string GpuMonitor::getGpuName()
{
    return gpuName;
}

std::string GpuMonitor::getVendorName()
{
    return vendorName;
}

std::string GpuMonitor::getDriverVersion()
{
    return driverVersion;
}

uint32_t GpuMonitor::getVendorId()
{
    return vendorId;
}

uint32_t GpuMonitor::getDeviceId()
{
    return deviceId;
}

uint64_t GpuMonitor::getDedicatedMemoryTotal()
{
    return dedicatedMemoryTotal;
}

uint64_t GpuMonitor::getSharedMemoryTotal()
{
    return sharedMemoryTotal;
}

float GpuMonitor::getGpuUsage()
{
    return 0.0f;
}

float GpuMonitor::getTemperature()
{
    return 0.0f;
}

float GpuMonitor::getCoreClock()
{
    return 0.0f;
}

float GpuMonitor::getMemoryClock()
{
    return 0.0f;
}

float GpuMonitor::getFanSpeed()
{
    return 0.0f;
}

uint64_t GpuMonitor::getDedicatedMemoryUsed()
{
    return 0;
}

uint64_t GpuMonitor::getSharedMemoryUsed()
{
    return 0;
}

//Constructor
GpuMonitor::GpuMonitor()
{
    initializeDXGI();
}

//Destructor
GpuMonitor::~GpuMonitor()
{
    if(comInitialized)
    {
        CoUninitialize();
    }
}

//Initialize DXGI
void GpuMonitor::initializeDXGI()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if(SUCCEEDED(hr))
    {
        comInitialized = true;
    }
    else 
    if(hr != RPC_E_CHANGED_MODE)
    {
        throw std::runtime_error("Failed to initialize COM library.");
    }

    //Create DXGI factory
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    if(FAILED(hr))
    {
        throw std::runtime_error("Failed to create DXGI factory.");
    }
    
    //Get the first hardware adapter (GPU)
    hr = factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
    
    if(FAILED(hr))
    {
        throw std::runtime_error("Failed to enumerate DXGI adapter.");
    }

    if(!adapter)
    {
        throw std::runtime_error("No suitable GPU adapter found.");
    }
    
    //Read adapter description
    DXGI_ADAPTER_DESC3 desc;

    hr = adapter->GetDesc3(&desc);

    if(FAILED(hr))
    {
        throw std::runtime_error("Failed to read adapter description.");
    }

    //Cache information
    //gpuName = _bstr_t(desc.Description); //later for UTF-8 support, replace it with an explicit UTF-16 to UTF-8 conversion.
    int length = WideCharToMultiByte(
        CP_UTF8,
        0,
        desc.Description,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );
    std::string name(length - 1, '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        desc.Description,
        -1,
        name.data(),
        length,
        nullptr,
        nullptr
    );
    gpuName = name;
    vendorId = desc.VendorId;
    deviceId = desc.DeviceId;

    dedicatedMemoryTotal = desc.DedicatedVideoMemory;
    sharedMemoryTotal = desc.SharedSystemMemory;

    //Convert Vendor ID to a readable string
    switch(vendorId)
    {
        case 0x10DE:
            vendorName = "NVIDIA";
            break;

        case 0x1002:
        case 0x1022:
            vendorName = "AMD";
            break;

        case 0x8086:
            vendorName = "Intel";
            break;
        
        default:
            vendorName = "Unknown";
            break;
    }
    driverVersion = "Unknown";
}