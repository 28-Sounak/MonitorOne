#include "MemoryMonitor.h"

#include <windows.h>

MemoryMonitor::MemoryMonitor()
{
    updateMemoryInfo();
    //initializeMemoryModules();
}

void MemoryMonitor::updateMemoryInfo()
{
    MEMORYSTATUSEX memoryStatus{};

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if(GlobalMemoryStatusEx(&memoryStatus))
    {
        totalMemory = memoryStatus.ullTotalPhys;
        availableMemory = memoryStatus.ullAvailPhys;
        usedMemory = totalMemory - availableMemory;
    }
}

uint64_t MemoryMonitor::getTotalMemory()
{
    updateMemoryInfo();
    return totalMemory;
}

uint64_t MemoryMonitor::getAvailableMemory()
{
    updateMemoryInfo();
    return availableMemory;
}

uint64_t MemoryMonitor::getUsedMemory()
{
    updateMemoryInfo();
    return usedMemory;
}

float MemoryMonitor::getMemoryUsage()
{
    updateMemoryInfo();

    if(totalMemory == 0)
    {
        return 0.0f;
    }

    return (static_cast<float>(usedMemory) /
            static_cast<float>(totalMemory)) * 100.0f;
}

