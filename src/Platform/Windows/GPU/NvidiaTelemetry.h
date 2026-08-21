#pragma once

#include "GpuTelemetry.h"

class NvidiaTelemetry : public GpuTelemetry
{
public:

    NvidiaTelemetry();

    ~NvidiaTelemetry() override;


    float getGpuUsage() override;

    float getTemperature() override;

    uint32_t getFanSpeed() override;

    uint32_t getCoreClock() override;

    uint32_t getMemoryClock() override;


private:

    bool initialized;

    void initialize();

    void shutdown();
};