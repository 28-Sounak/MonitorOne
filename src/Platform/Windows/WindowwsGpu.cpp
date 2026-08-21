#include <WindowsGpu.h>

#include <string>

#include<windows.h>

#include "GPU/NvidiaTelemetry.h"

#include "GPU/AmdTelemetry.h"

#include "GPU/IntelTelemetry.h"

#pragma comment(lib, "dxgi.lib")

using Microsoft : WML :: ComPtr;

//Helper function

namespace
{
    std::string wideStringToString(const wchar_t* text)
    {
        if (text == nullptr)
        {
            return "";
        }

        int size =
            WideCharToMultiByte(
                CP_UTF8,
                0,
                text,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr
            );

        if (size <= 0)
        {
            return "";
        }

        std::string result(
            size - 1,
            '\0'
        );

        WideCharToMultiByte(
            CP_UTF8,
            0,
            text,
            -1,
            result.data(),
            size,
            nullptr,
            nullptr
        );

        return result;
    }


    std::string vendorIdToName(
        uint32_t vendorId)
    {
        switch (vendorId)
        {
            case 0x10DE:
                return "NVIDIA";

            case 0x1002:
            case 0x1022:
                return "AMD";

            case 0x8086:
                return "Intel";

            case 0x1414:
                return "Microsoft";

            default:
                return "Unknown";
        }
    }
}

//Constructor

WindowsGpu :: WindowsGpu() : memoryInfoValid(false), vendor(GpuVendor :: Unknown)
{
    initialize();
}

//Initialize DXGI

bool WindowsGpu::initialize()
{
    HRESULT result =
        CreateDXGIFactory1(
            IID_PPV_ARGS(&factory)
        );

    if (FAILED(result))
    {
        return false;
    }

    return queryAdapter();
}

//Find GPU Adapter

bool WindowsGpu::queryAdapter()
{
    if (!factory)
    {
        return false;
    }


    for (UINT index = 0; ; ++index)
    {
        ComPtr<IDXGIAdapter1> adapter1;


        HRESULT result =
            factory->EnumAdapters1(
                index,
                &adapter1
            );


        if (result == DXGI_ERROR_NOT_FOUND)
        {
            break;
        }


        if (FAILED(result))
        {
            continue;
        }


        DXGI_ADAPTER_DESC1 desc1{};


        if (FAILED(
                adapter1->GetDesc1(&desc1)
            ))
        {
            continue;
        }

        //Ignore Software Adapters

        if(desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            continue;
        }

        //Ignore Software Engineering

        if(desc1.Flags &  DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            continue;
        }

        //Convert to IDXGIAdapter4

        comPtr<IDXGIAdapter4> adapter4;

        result = adapter1.As(&adapter4);

        if(FAILED(result))
        {
            continue;
        }

        //Get Adapter Descristion

        DXGI_ADAPTER_DESC3 desc3();

        result = adapter4->GetDesc3(&desc3);

        if(FAILED(result))
        {
            continue;
        }
        
        adapter = adapter4;

        adapterDescription = desc3;

        vendor = identifyVendor();

        updateMemoryInfo();

        return true;
    }
    return false;
}

//Identify GPU Vendor

WindowsGpu :: GpuVendor
WindowewGpu :: identifyVendor() const
{
    switch(adapterDescription.VendorId)
    {
        case 0x10DE:
            return GpuVendor::NVIDIA;

        case 0x1002:
        case 0x1022:
            return GpuVendor::AMD;

        case 0x8086:
            return GpuVendor::Intel;

        default:
            return GpuVendor::Unknown;
    }
}

//Initialize Vendor Information

void WindowsGpu::initializeTelemetry()
{
    telemetry.reset();

    switch (vendor)
    {
        case GpuVendor::NVIDIA:
        {
            telemetry =
                std::make_unique<NvidiaTelemetry>();

            break;
        }

        case GpuVendor::AMD:
        {
            telemetry =
                std::make_unique<AmdTelemetry>();

            break;
        }

        case GpuVendor::Intel:
        {
            telemetry =
                std::make_unique<IntelTelemetry>();

            break;
        }

        default:
        {
            telemetry.reset();

            break;
        }
    }
}


//Update Memory Information

void WindowsGpy :: updateMemoryInfo()
{
    memoryInfoValid = false;

    if(!adapter)
    {
        return;
    }

    Const<IDXGIAdapter3> adapter3;

    HRESULT result = adapter.As(&adapter3);

    if(FAILED(result))
    {
        return;
    }

    //Dedicated / local video memory

    result =
        adapter3->QueryVideoMemoryInfo(
            0,
            DXGI_MEMORY_SEGMENT_GROUP_LOCAL,
            &dedicatedMemoryInfo
        );

    if(FAILED(result))
    {
        return;
    }

    //Shared / non-local memory

    result =
        adapter3->QueryVideoMemoryInfo(
            0,
            DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL,
            &sharedMemoryInfo
        );


    if (FAILED(result))
    {
        return;
    }

    memoryInfoValid = true;
}

//GPU Name

std :: string WIndowsGpu :: getGpuName()
{
    if(!adapter)
    {
        return "Unknown GPU";
    }
    
    return wideStringToString(adapterDescription.Description);
}

//Vendor Name

std :: string WindowsGpu :: getVendorName()
{
    return vendorIdToNAme(adapterDescription.VendorId);
}

//Vendor ID

uint32_t WIndowsGpu :: getVendorId()
{
    return adapterDescription.VendorId;
}

//Device Id

uint32_t WindowsGpu :: getDeviceId()
{
    return adapterDescription.DeviceId;
}

//Dedicated Memory Total

uint64_t WindowsGpu :: getDedicatedMemoryTotal()
{
    return static_cast<uint64_t>(adapterDescription.DedicatedVideoMemory);
}

//Dedicated Memory Used

uint64_t WindowsGpu :: getDedicatedMemoryUsed()
{
    updateMemoryInfo();

    if(!memoryInfoValid)
    {
        return 0;
    }

    return dedicatedMemoryInfo.CurrentUsage;    
}

// Dedicated memory available

uint64_t
WindowsGpu::getDedicatedMemoryAvailable()
{
    uint64_t total =
        getDedicatedMemoryTotal();


    uint64_t used =
        getDedicatedMemoryUsed();


    if (total < used)
    {
        return 0;
    }


    return total - used;
}

//Dedicated Memory Usage

float
WindowsGpu::getDedicatedMemoryUsage()
{
    uint64_t total =
        getDedicatedMemoryTotal();


    uint64_t used =
        getDedicatedMemoryUsed();


    if (total == 0)
    {
        return 0.0f;
    }


    return static_cast<float>(
        (
            static_cast<double>(used) /
            static_cast<double>(total)
        ) * 100.0
    );
}

//Shared Memory Total

uint64_t WindowsGpu :: getSharedMemoryTotal()
{

    return static_cast<uint64_t>(adapterDescription.SharedSystemMemory);
}

//Shared Memory Used

uint64_t
WindowsGpu::getSharedMemoryUsed()
{
    updateMemoryInfo();


    if (!memoryInfoValid)
    {
        return 0;
    }


    return sharedMemoryInfo.CurrentUsage;
}

//Shared Memory Available

uint64_t
WindowsGpu::getSharedMemoryAvailable()
{
    uint64_t total =
        getSharedMemoryTotal();


    uint64_t used =
        getSharedMemoryUsed();


    if (total < used)
    {
        return 0;
    }


    return total - used;
}

//GPU Utilization

float WindowsGpu :: getGpuUSage()
{
     if (!telemetry)
    {
        return -1.0f;
    }

    return telemetry->getGpuUsage();
}

//GPU Temperature

float WindowsGpu :: getTemperatur()
{
    if (!telemetry)
    {
        return -1.0f;
    }


    return telemetry->getTemperature();
}

//Fan Speed

float WindowsGpu :: getFanSpeed()
{
    if (!telemetry)
    {
        return 0;
    }


    return telemetry->getFanSpeed();
}

//Core Clock

uint32_t WindowsGpu :: getCoreClock()
{
    if (!telemetry)
    {
        return 0;
    }


    return telemetry->getCoreClock();
}

//Memory Clock

uint32_t WindowsGpu :: getMemoryClock()
{
    if (!telemetry)
    {
        return 0;
    }


    return telemetry->getMemoryClock();
}