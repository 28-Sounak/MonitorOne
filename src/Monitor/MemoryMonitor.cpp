#include "MemoryMonitor.h"

#include <windows.h>


// ============================================================
// Constructor
// ============================================================

MemoryMonitor::MemoryMonitor()
{
    updateMemoryInfo();

    // Physical RAM module information
    // will be initialized later using WMI.
    // initializeMemoryModules();
}


// ============================================================
// Update
// ============================================================

void MemoryMonitor::update()
{
    updateMemoryInfo();
}


// ============================================================
// Update Runtime Memory Information
// ============================================================

void MemoryMonitor::updateMemoryInfo()
{
    MEMORYSTATUSEX memoryStatus{};

    memoryStatus.dwLength =
        sizeof(MEMORYSTATUSEX);


    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return;
    }


    // --------------------------------------------------------
    // Total physical memory
    // --------------------------------------------------------

    totalMemory =
        memoryStatus.ullTotalPhys;


    // --------------------------------------------------------
    // Available physical memory
    // --------------------------------------------------------

    availableMemory =
        memoryStatus.ullAvailPhys;


    // --------------------------------------------------------
    // Used physical memory
    // --------------------------------------------------------

    if (totalMemory >= availableMemory)
    {
        usedMemory =
            totalMemory -
            availableMemory;
    }
    else
    {
        usedMemory = 0;
    }
}


// ============================================================
// Initialize Physical RAM Modules
// ============================================================

void MemoryMonitor::initializeMemoryModules()
{
    // --------------------------------------------------------
    // WMI implementation will be added here.
    //
    // Information to retrieve:
    //
    // Manufacturer
    // Part Number
    // Serial Number
    // Memory Type
    // Form Factor
    // Device Locator
    // Capacity
    // Speed
    // Configured Speed
    // Data Width
    // Total Width
    // --------------------------------------------------------
}


// ============================================================
// Get Total Memory
// ============================================================

uint64_t MemoryMonitor::getTotalMemory()
{
    return totalMemory;
}


// ============================================================
// Get Available Memory
// ============================================================

uint64_t MemoryMonitor::getAvailableMemory()
{
    return availableMemory;
}


// ============================================================
// Get Used Memory
// ============================================================

uint64_t MemoryMonitor::getUsedMemory()
{
    return usedMemory;
}


// ============================================================
// Get Memory Usage
// ============================================================

float MemoryMonitor::getMemoryUsage()
{
    if (totalMemory == 0)
    {
        return 0.0f;
    }


    return (
        static_cast<float>(usedMemory) /
        static_cast<float>(totalMemory)
    ) * 100.0f;
}


// ============================================================
// Convert Memory Type to Readable Name
// ============================================================

std::string MemoryMonitor::getMemoryTypeName(
    uint64_t type
)
{
    switch (type)
    {
        case 0:
            return "Unknown";

        case 1:
            return "Other";

        case 2:
            return "DRAM";

        case 3:
            return "Synchronous DRAM";

        case 4:
            return "Cache DRAM";

        case 5:
            return "EDO";

        case 6:
            return "EDRAM";

        case 7:
            return "VRAM";

        case 8:
            return "SRAM";

        case 9:
            return "RAM";

        case 10:
            return "ROM";

        case 11:
            return "Flash";

        case 12:
            return "EEPROM";

        case 13:
            return "FEPROM";

        case 14:
            return "EPROM";

        case 15:
            return "CDRAM";

        case 16:
            return "3DRAM";

        case 17:
            return "SDRAM";

        case 18:
            return "SGRAM";

        case 19:
            return "RDRAM";

        case 20:
            return "DDR";

        case 21:
            return "DDR2";

        case 22:
            return "DDR2 FB-DIMM";

        case 24:
            return "DDR3";

        case 25:
            return "FBD2";

        case 26:
            return "DDR4";

        case 27:
            return "LPDDR";

        case 28:
            return "LPDDR2";

        case 29:
            return "LPDDR3";

        case 30:
            return "LPDDR4";

        case 31:
            return "Logical non-volatile device";

        case 32:
            return "HBM";

        case 33:
            return "HBM2";

        case 34:
            return "DDR5";

        case 35:
            return "LPDDR5";

        default:
            return "Unknown";
    }
}


// ============================================================
// Convert Form Factor to Readable Name
// ============================================================

std::string MemoryMonitor::getFormFactorName(
    uint16_t formFactor
)
{
    switch (formFactor)
    {
        case 0:
            return "Unknown";

        case 1:
            return "Other";

        case 2:
            return "SIP";

        case 3:
            return "DIP";

        case 4:
            return "ZIP";

        case 5:
            return "SOJ";

        case 6:
            return "Proprietary";

        case 7:
            return "SIMM";

        case 8:
            return "DIMM";

        case 9:
            return "TSOP";

        case 10:
            return "PGA";

        case 11:
            return "RIMM";

        case 12:
            return "SODIMM";

        case 13:
            return "SRIMM";

        case 14:
            return "SMD";

        case 15:
            return "SSMP";

        case 16:
            return "QFP";

        case 17:
            return "TQFP";

        case 18:
            return "SOIC";

        case 19:
            return "LCC";

        case 20:
            return "PLCC";

        case 21:
            return "BGA";

        case 22:
            return "FPBGA";

        case 23:
            return "LGA";

        default:
            return "Unknown";
    }
}


// ============================================================
// Get Memory Modules
// ============================================================

const std::vector<MemoryModule>&
MemoryMonitor::getMemoryModules()
{
    return memoryModules;
}