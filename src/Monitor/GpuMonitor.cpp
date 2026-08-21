#include "../Core/GpuMonitor.h"

#include "../Platform/Windows/WindowsGpu.h"

namespace
{
    WindowsGpu windowsGpu;
}

//Constructor

GpuMonitor :: GpuMonitor()
{

}

//Identification

std::string GpuMonitor::getGpuName()
{
    return windowsGpu.getGpuName();
}


std::string GpuMonitor::getVendorName()
{
    return windowsGpu.getVendorName();
}


uint32_t GpuMonitor::getVendorId()
{
    return windowsGpu.getVendorId();
}


uint32_t GpuMonitor::getDeviceId()
{
    return windowsGpu.getDeviceId();
}

//Dedicated Memory

uint64_t
GpuMonitor::getDedicatedMemoryTotal()
{
    return windowsGpu.getDedicatedMemoryTotal();
}


uint64_t
GpuMonitor::getDedicatedMemoryUsed()
{
    return windowsGpu.getDedicatedMemoryUsed();
}


uint64_t
GpuMonitor::getDedicatedMemoryAvailable()
{
    return windowsGpu.getDedicatedMemoryAvailable();
}


float
GpuMonitor::getDedicatedMemoryUsage()
{
    return windowsGpu.getDedicatedMemoryUsage();
}

//Shared Memory

uint64_t
GpuMonitor::getSharedMemoryTotal()
{
    return windowsGpu.getSharedMemoryTotal();
}


uint64_t
GpuMonitor::getSharedMemoryUsed()
{
    return windowsGpu.getSharedMemoryUsed();
}


uint64_t
GpuMonitor::getSharedMemoryAvailable()
{
    return windowsGpu.getSharedMemoryAvailable();
}

// GPU telemetry

float
GpuMonitor::getGpuUsage()
{
    return windowsGpu.getGpuUsage();
}


float
GpuMonitor::getTemperature()
{
    return windowsGpu.getTemperature();
}


uint32_t
GpuMonitor::getFanSpeed()
{
    return windowsGpu.getFanSpeed();
}


uint32_t
GpuMonitor::getCoreClock()
{
    return windowsGpu.getCoreClock();
}


uint32_t
GpuMonitor::getMemoryClock()
{
    return windowsGpu.getMemoryClock();
}

/*#include "GpuMonitor.h"

#include <windows.h>

#include <stdexcept>
#include <string>

#include <dxgi1_6.h>

// ============================================================
// Constants
// ============================================================

// NVML return code
// NVML_SUCCESS = 0
static constexpr int NVML_SUCCESS = 0;

// NVML temperature sensor
// NVML_TEMPERATURE_GPU = 0
static constexpr int NVML_TEMPERATURE_GPU = 0;

// NVML clock types
// NVML_CLOCK_GRAPHICS = 0
// NVML_CLOCK_MEM      = 2
static constexpr int NVML_CLOCK_GRAPHICS = 0;
static constexpr int NVML_CLOCK_MEM = 2;


// ============================================================
// Constructor
// ============================================================

GpuMonitor::GpuMonitor()
{
    initializeDXGI();

    // NVML is required only for NVIDIA telemetry.
    if (isNvidiaGPU())
    {
        initializeNVML();
    }
}


// ============================================================
// Destructor
// ============================================================

GpuMonitor::~GpuMonitor()
{
    shutdownNVML();

    if (comInitialized)
    {
        CoUninitialize();
        comInitialized = false;
    }
}


// ============================================================
// Wide string -> UTF-8
// ============================================================

std::string GpuMonitor::wideToString(
    const wchar_t* value)
{
    if (value == nullptr)
    {
        return "";
    }

    int requiredSize =
        WideCharToMultiByte(
            CP_UTF8,
            0,
            value,
            -1,
            nullptr,
            0,
            nullptr,
            nullptr
        );

    if (requiredSize <= 0)
    {
        return "";
    }

    std::string result(
        requiredSize - 1,
        '\0'
    );

    WideCharToMultiByte(
        CP_UTF8,
        0,
        value,
        -1,
        result.data(),
        requiredSize,
        nullptr,
        nullptr
    );

    return result;
}


// ============================================================
// Initialize DXGI
// ============================================================

void GpuMonitor::initializeDXGI()
{
    HRESULT hr =
        CoInitializeEx(
            nullptr,
            COINIT_MULTITHREADED
        );

    if (SUCCEEDED(hr))
    {
        comInitialized = true;
    }
    else if (hr != RPC_E_CHANGED_MODE)
    {
        throw std::runtime_error(
            "Failed to initialize COM."
        );
    }


    // --------------------------------------------------------
    // Create DXGI factory
    // --------------------------------------------------------

    hr =
        CreateDXGIFactory1(
            IID_PPV_ARGS(&factory)
        );

    if (FAILED(hr))
    {
        throw std::runtime_error(
            "Failed to create DXGI factory."
        );
    }


    // --------------------------------------------------------
    // Select high-performance GPU
    // --------------------------------------------------------

    hr =
        factory->EnumAdapterByGpuPreference(
            0,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(&adapter)
        );

    if (FAILED(hr) || !adapter)
    {
        throw std::runtime_error(
            "Failed to find GPU adapter."
        );
    }


    // --------------------------------------------------------
    // Get GPU description
    // --------------------------------------------------------

    DXGI_ADAPTER_DESC3 desc{};

    hr =
        adapter->GetDesc3(&desc);

    if (FAILED(hr))
    {
        throw std::runtime_error(
            "Failed to get GPU description."
        );
    }


    // --------------------------------------------------------
    // GPU name
    // --------------------------------------------------------

    gpuName =
        wideToString(desc.Description);


    // --------------------------------------------------------
    // Vendor / Device IDs
    // --------------------------------------------------------

    vendorId =
        desc.VendorId;

    deviceId =
        desc.DeviceId;


    // --------------------------------------------------------
    // VRAM
    // --------------------------------------------------------

    dedicatedMemoryTotal =
        static_cast<uint64_t>(
            desc.DedicatedVideoMemory
        );

    sharedMemoryTotal =
        static_cast<uint64_t>(
            desc.SharedSystemMemory
        );


    // --------------------------------------------------------
    // Vendor name
    // --------------------------------------------------------

    switch (vendorId)
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
}


// ============================================================
// Check NVIDIA GPU
// ============================================================

bool GpuMonitor::isNvidiaGPU() const
{
    return vendorId == 0x10DE;
}


// ============================================================
// Load NVML
// ============================================================

void GpuMonitor::initializeNVML()
{
    // --------------------------------------------------------
    // Load NVIDIA Management Library
    // --------------------------------------------------------

    nvmlLibrary =
        LoadLibraryA("nvml.dll");

    if (nvmlLibrary == nullptr)
    {
        return;
    }


    // --------------------------------------------------------
    // Load required functions
    // --------------------------------------------------------

    if (!loadNVMLFunctionPointers())
    {
        FreeLibrary(nvmlLibrary);

        nvmlLibrary = nullptr;

        return;
    }


    // --------------------------------------------------------
    // Initialize NVML
    // --------------------------------------------------------

    if (nvmlInit_v2() != NVML_SUCCESS)
    {
        return;
    }

    nvmlInitialized = true;


    // --------------------------------------------------------
    // Get first NVIDIA GPU
    // --------------------------------------------------------

    if (nvmlDeviceGetHandleByIndex_v2(
            0,
            &nvmlDevice
        ) != NVML_SUCCESS)
    {
        nvmlDevice = nullptr;

        return;
    }


    // --------------------------------------------------------
    // Driver version
    // --------------------------------------------------------

    char driverBuffer[256]{};

    if (nvmlSystemGetDriverVersion(
            driverBuffer,
            sizeof(driverBuffer)
        ) == NVML_SUCCESS)
    {
        driverVersion =
            driverBuffer;
    }
    else
    {
        driverVersion =
            "Unknown";
    }
}


// ============================================================
// Load NVML function pointers
// ============================================================

bool GpuMonitor::loadNVMLFunctionPointers()
{
    nvmlInit_v2 =
        reinterpret_cast<nvmlInit_v2_t>(
            GetProcAddress(
                nvmlLibrary,
                "nvmlInit_v2"
            )
        );


    nvmlShutdown =
        reinterpret_cast<nvmlShutdown_t>(
            GetProcAddress(
                nvmlLibrary,
                "nvmlShutdown"
            )
        );


    nvmlDeviceGetHandleByIndex_v2 =
        reinterpret_cast<
            nvmlDeviceGetHandleByIndex_v2_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetHandleByIndex_v2"
            )
        );


    nvmlDeviceGetTemperature =
        reinterpret_cast<
            nvmlDeviceGetTemperature_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetTemperature"
            )
        );


    nvmlDeviceGetClockInfo =
        reinterpret_cast<
            nvmlDeviceGetClockInfo_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetClockInfo"
            )
        );


    nvmlDeviceGetFanSpeed =
        reinterpret_cast<
            nvmlDeviceGetFanSpeed_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetFanSpeed"
            )
        );


    nvmlDeviceGetUtilizationRates =
        reinterpret_cast<
            nvmlDeviceGetUtilizationRates_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetUtilizationRates"
            )
        );


    nvmlDeviceGetMemoryInfo =
        reinterpret_cast<
            nvmlDeviceGetMemoryInfo_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlDeviceGetMemoryInfo"
            )
        );


    nvmlSystemGetDriverVersion =
        reinterpret_cast<
            nvmlSystemGetDriverVersion_t
        >(
            GetProcAddress(
                nvmlLibrary,
                "nvmlSystemGetDriverVersion"
            )
        );


    return
        nvmlInit_v2 != nullptr &&
        nvmlShutdown != nullptr &&
        nvmlDeviceGetHandleByIndex_v2 != nullptr &&
        nvmlDeviceGetTemperature != nullptr &&
        nvmlDeviceGetClockInfo != nullptr &&
        nvmlDeviceGetFanSpeed != nullptr &&
        nvmlDeviceGetUtilizationRates != nullptr &&
        nvmlDeviceGetMemoryInfo != nullptr &&
        nvmlSystemGetDriverVersion != nullptr;
}


// ============================================================
// Shutdown NVML
// ============================================================

void GpuMonitor::shutdownNVML()
{
    if (nvmlInitialized &&
        nvmlShutdown != nullptr)
    {
        nvmlShutdown();

        nvmlInitialized = false;
    }


    if (nvmlLibrary != nullptr)
    {
        FreeLibrary(nvmlLibrary);

        nvmlLibrary = nullptr;
    }

    nvmlDevice = nullptr;
}


// ============================================================
// GPU Name
// ============================================================

std::string GpuMonitor::getGpuName()
{
    return gpuName;
}


// ============================================================
// Vendor Name
// ============================================================

std::string GpuMonitor::getVendorName()
{
    return vendorName;
}


// ============================================================
// Driver Version
// ============================================================

std::string GpuMonitor::getDriverVersion()
{
    return driverVersion;
}


// ============================================================
// Vendor ID
// ============================================================

uint32_t GpuMonitor::getVendorId()
{
    return vendorId;
}


// ============================================================
// Device ID
// ============================================================

uint32_t GpuMonitor::getDeviceId()
{
    return deviceId;
}


// ============================================================
// Dedicated Memory Total
// ============================================================

uint64_t GpuMonitor::getDedicatedMemoryTotal()
{
    return dedicatedMemoryTotal;
}


// ============================================================
// Shared Memory Total
// ============================================================

uint64_t GpuMonitor::getSharedMemoryTotal()
{
    return sharedMemoryTotal;
}


// ============================================================
// GPU Usage
// ============================================================

float GpuMonitor::getGpuUsage()
{
    if (!nvmlInitialized ||
        nvmlDevice == nullptr ||
        nvmlDeviceGetUtilizationRates == nullptr)
    {
        return 0.0f;
    }


    unsigned int gpuUsage = 0;

    unsigned int memoryUsage = 0;


    int result =
        nvmlDeviceGetUtilizationRates(
            nvmlDevice,
            &gpuUsage,
            &memoryUsage
        );


    if (result != NVML_SUCCESS)
    {
        return 0.0f;
    }


    return static_cast<float>(
        gpuUsage
    );
}


// ============================================================
// GPU Temperature
// ============================================================

float GpuMonitor::getTemperature()
{
    if (!nvmlInitialized ||
        nvmlDevice == nullptr ||
        nvmlDeviceGetTemperature == nullptr)
    {
        return 0.0f;
    }


    unsigned int temperature = 0;


    int result =
        nvmlDeviceGetTemperature(
            nvmlDevice,
            NVML_TEMPERATURE_GPU,
            &temperature
        );


    if (result != NVML_SUCCESS)
    {
        return 0.0f;
    }


    return static_cast<float>(
        temperature
    );
}


// ============================================================
// Core / Graphics Clock
// ============================================================

float GpuMonitor::getCoreClock()
{
    if (!nvmlInitialized ||
        nvmlDevice == nullptr ||
        nvmlDeviceGetClockInfo == nullptr)
    {
        return 0.0f;
    }


    unsigned int clockMHz = 0;


    int result =
        nvmlDeviceGetClockInfo(
            nvmlDevice,
            NVML_CLOCK_GRAPHICS,
            &clockMHz
        );


    if (result != NVML_SUCCESS)
    {
        return 0.0f;
    }


    return static_cast<float>(
        clockMHz
    );
}


// ============================================================
// Memory Clock
// ============================================================

float GpuMonitor::getMemoryClock()
{
    if (!nvmlInitialized ||
        nvmlDevice == nullptr ||
        nvmlDeviceGetClockInfo == nullptr)
    {
        return 0.0f;
    }


    unsigned int clockMHz = 0;


    int result =
        nvmlDeviceGetClockInfo(
            nvmlDevice,
            NVML_CLOCK_MEM,
            &clockMHz
        );


    if (result != NVML_SUCCESS)
    {
        return 0.0f;
    }


    return static_cast<float>(
        clockMHz
    );
}


// ============================================================
// Fan Speed
// ============================================================

float GpuMonitor::getFanSpeed()
{
    if (!nvmlInitialized ||
        nvmlDevice == nullptr ||
        nvmlDeviceGetFanSpeed == nullptr)
    {
        return 0.0f;
    }


    unsigned int fanSpeed = 0;


    int result =
        nvmlDeviceGetFanSpeed(
            nvmlDevice,
            &fanSpeed
        );


    if (result != NVML_SUCCESS)
    {
        return 0.0f;
    }


    return static_cast<float>(
        fanSpeed
    );
}


// ============================================================
// Dedicated VRAM Used
// ============================================================

uint64_t GpuMonitor::getDedicatedMemoryUsed()
{
    if (!adapter)
    {
        return 0;
    }

    DXGI_QUERY_VIDEO_MEMORY_INFO memoryInfo{};

    HRESULT hr =
        adapter->QueryVideoMemoryInfo(
            0,
            DXGI_MEMORY_SEGMENT_GROUP_LOCAL,
            &memoryInfo
        );

    if (FAILED(hr))
    {
        return 0;
    }

    return memoryInfo.CurrentUsage;
}


// ============================================================
// Shared GPU Memory Used
// ============================================================

uint64_t GpuMonitor::getSharedMemoryUsed()
{
    if (!adapter)
    {
        return 0;
    }

    DXGI_QUERY_VIDEO_MEMORY_INFO memoryInfo{};

    HRESULT hr =
        adapter->QueryVideoMemoryInfo(
            0,
            DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL,
            &memoryInfo
        );

    if (FAILED(hr))
    {
        return 0;
    }

    return memoryInfo.CurrentUsage;
} */