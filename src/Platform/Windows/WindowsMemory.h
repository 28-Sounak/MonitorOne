#pragma once

#include <windows.h>

#include <vector>

#include <cstdint>

#include "../../Core/MemoryMonitor.h"

class MemoryMonitor
{
    public:
        WindowsMemory();

        uint64_t getTotalMemory();

        uint64_t getAvailableMemory();

        uint64_t getUsedMemory();

        float getUsage();

        std :: vector<MemoryModule> getModules();
    
    //private: 
        //uint64_t byteToGB(uint64_t bytes);
};