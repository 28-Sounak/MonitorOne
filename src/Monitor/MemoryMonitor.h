#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct MemoryModule
{
    std::string manufacturer;
    std::string partNumber;
    std::string serialNumber;
    std::string memoryType;
    std::string formFactor;
    std::string deviceLocator;

    uint64_t capacity = 0;

    uint32_t speed = 0;
    uint32_t configuredSpeed = 0;

    uint32_t dataWidth = 0;
    uint32_t totalWidth = 0;
};

class MemoryMonitor
{
private:

    // Runtime memory information
    uint64_t totalMemory = 0;
    uint64_t availableMemory = 0;
    uint64_t usedMemory = 0;

    // Physical RAM module information
    std::vector<MemoryModule> memoryModules;

    // Updates runtime memory information
    void updateMemoryInfo();

    // Initializes physical RAM module information
    void initializeMemoryModules();

    // Converts SMBIOS memory type to readable text
    std::string getMemoryTypeName(uint64_t type);

    // Converts WMI form factor value to readable text
    std::string getFormFactorName(uint16_t formFactor);

public:

    // Constructor
    MemoryMonitor();

    // Runtime memory
    uint64_t getTotalMemory();
    uint64_t getAvailableMemory();
    uint64_t getUsedMemory();
    float getMemoryUsage();

    // RAM module information
    const std::vector<MemoryModule>& getMemoryModules();
};