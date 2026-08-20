#pragma once

#include <cstdint>

#include <vector>

#include <string>

struct MemoryModule
{
    uint64_t size;

    std :: string manufacturer;

    std :: string type;

    std :: string formFactor;

    uint64_t speed;

    std :: string partNumber;

    std :: string serialNumber;

    std :: string configuredClockSpeed;
};

class MemoryMonitor
{
    public: 
        MemoryMonitor();

        uint64_t getTotalMemory();

        uint64_t getAvailableMemory();

        uint64_t getUsedMemory();

        float getUsage();

        std :: vector<MemoryModule> getModules();
};