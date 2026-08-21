#include "AmdTelemetry.h"


AmdTelemetry::AmdTelemetry()
    : initialized(false)
{
    initialize();
}


AmdTelemetry::~AmdTelemetry()
{
    shutdown();
}


// ============================================================
// Initialize ADLX
// ============================================================

void AmdTelemetry::initialize()
{
    /*
        AMD ADLX initialization will go here.

        Required AMD dependency:

            ADLX headers
            ADLX library
            ADLX runtime components
    */

    initialized = false;
}


// ============================================================
// Shutdown
// ============================================================

void AmdTelemetry::shutdown()
{
    if (!initialized)
    {
        return;
    }


    /*
        ADLX shutdown code.
    */


    initialized = false;
}


// ============================================================
// GPU Usage
// ============================================================

float AmdTelemetry::getGpuUsage()
{
    if (!initialized)
    {
        return -1.0f;
    }


    /*
        ADLX GPU metrics.
    */


    return -1.0f;
}


// ============================================================
// Temperature
// ============================================================

float AmdTelemetry::getTemperature()
{
    if (!initialized)
    {
        return -1.0f;
    }


    /*
        ADLX GPU temperature metric.
    */


    return -1.0f;
}


// ============================================================
// Fan RPM
// ============================================================

uint32_t AmdTelemetry::getFanSpeed()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        ADLX provides GPU fan-speed metrics.

        ADLX 1.5 also documents a GPU fan-duty metric,
        while fan speed is available as an RPM metric.
    */


    return 0;
}


// ============================================================
// Core Clock
// ============================================================

uint32_t AmdTelemetry::getCoreClock()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        ADLX GPU clock metric.
    */


    return 0;
}


// ============================================================
// Memory Clock
// ============================================================

uint32_t AmdTelemetry::getMemoryClock()
{
    if (!initialized)
    {
        return 0;
    }


    /*
        ADLX memory clock metric.
    */


    return 0;
}