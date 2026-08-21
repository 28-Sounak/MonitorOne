#include "IntelTelemetry.h"


IntelTelemetry::IntelTelemetry()
    : initialized(false)
{
    initialize();
}


IntelTelemetry::~IntelTelemetry()
{
    shutdown();
}


// ============================================================
// Initialize
// ============================================================

void IntelTelemetry::initialize()
{
    /*
        Intel telemetry implementation will depend on the
        specific Intel GPU generation and supported API.

        Intel GPA exposes metrics such as GPU Busy and
        GPU frequency, but it is not a universal replacement
        for NVML/ADLX hardware telemetry.
    */

    initialized = false;
}


// ============================================================
// Shutdown
// ============================================================

void IntelTelemetry::shutdown()
{
    initialized = false;
}


// ============================================================
// GPU Usage
// ============================================================

float IntelTelemetry::getGpuUsage()
{
    if (!initialized)
    {
        return -1.0f;
    }


    return -1.0f;
}


// ============================================================
// Temperature
// ============================================================

float IntelTelemetry::getTemperature()
{
    if (!initialized)
    {
        return -1.0f;
    }


    return -1.0f;
}


// ============================================================
// Fan Speed
// ============================================================

uint32_t IntelTelemetry::getFanSpeed()
{
    if (!initialized)
    {
        return 0;
    }


    return 0;
}


// ============================================================
// Core Clock
// ============================================================

uint32_t IntelTelemetry::getCoreClock()
{
    if (!initialized)
    {
        return 0;
    }


    return 0;
}


// ============================================================
// Memory Clock
// ============================================================

uint32_t IntelTelemetry::getMemoryClock()
{
    if (!initialized)
    {
        return 0;
    }


    return 0;
}