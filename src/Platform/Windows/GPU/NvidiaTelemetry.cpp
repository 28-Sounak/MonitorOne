#include "NvidiaTelemetry.h"


NvidiaTelemetry::NvidiaTelemetry()
    : initialized(false)
{
    initialize();
}


NvidiaTelemetry::~NvidiaTelemetry()
{
    shutdown();
}


// ============================================================
// Initialize NVIDIA telemetry
// ============================================================

void NvidiaTelemetry::initialize()
{
    /*
        NVML initialization will go here.

        Required NVIDIA dependency:

            nvml.h
            nvml.lib
            nvml.dll

        Once NVML is installed/configured, this class will
        obtain the NVIDIA device handle here.
    */

    initialized = false;
}


// ============================================================
// Shutdown
// ============================================================

void NvidiaTelemetry::shutdown()
{
    if (!initialized)
    {
        return;
    }


    /*
        nvmlShutdown();
    */


    initialized = false;
}


// ============================================================
// GPU Usage
// ============================================================

float NvidiaTelemetry::getGpuUsage()
{
    if (!initialized)
    {
        return -1.0f;
    }


    /*
        NVML:
        nvmlDeviceGetUtilizationRates()
    */


    return -1.0f;
}


// ============================================================
// Temperature
// ============================================================

float NvidiaTelemetry::getTemperature()
{
    if (!initialized)
    {
        return -1.0f;
    }


    /*
        NVML:
        nvmlDeviceGetTemperature()
    */


    return -1.0f;
}


// ============================================================
// Fan Speed
// ============================================================

uint32_t NvidiaTelemetry::getFanSpeed()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        NVML exposes fan-speed information.

        Important:
        NVML's standard fan-speed API reports fan speed
        as a percentage on supported devices, not necessarily
        RPM.

        If MonitorOne requires actual RPM, use a telemetry
        source that exposes RPM for that specific hardware.
    */


    return 0;
}


// ============================================================
// Core Clock
// ============================================================

uint32_t NvidiaTelemetry::getCoreClock()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        NVML:
        nvmlDeviceGetClockInfo()
    */


    return 0;
}


// ============================================================
// Memory Clock
// ============================================================

uint32_t NvidiaTelemetry::getMemoryClock()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        NVML:
        nvmlDeviceGetClockInfo()
    */


    return 0;
}