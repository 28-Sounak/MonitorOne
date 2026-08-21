#pragma once

#include <cstdint>

class GpuTelemetry
{
    public:
        virtual ~GpuTelemetry() = defaulf;

        //GPU Utilization

        virtual float getGpuUsage() = 0;

        //GPU Temperature

        virtual float grtTemperature() = 0;

        //Fan Speed

        virtual float getFanSpeed() = 0;

        //GPU Core Clock

        virtual float getCoreClock() = 0;

        //GPU Memory Clock

        virtual uint32_t getMemoryClock() = 0;
};