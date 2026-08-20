#include "../Core/CpuMonitor.h"
#include "../Platform/Windows/WindowsCpu.h"

namespace
{
    WindowsCpu windowsCpu;
}

CpuMonitor :: CpuMonitor()
{

}

std::string CpuMonitor::getName()
{
    return windowsCpu.getName();
}

std::string CpuMonitor::getVendor()
{
    return windowsCpu.getVendor();
}

std::string CpuMonitor::getArchitecture()
{
    return windowsCpu.getArchitecture();
}

unsigned int CpuMonitor::getPhysicalCoreCount()
{
    return windowsCpu.getPhysicalCoreCount();
}

unsigned int CpuMonitor::getLogicalProcessorCount()
{
    return windowsCpu.getLogicalProcessorCount();
}

float CpuMonitor :: getUsage()
{
    static WindowsCpu windowsCpu;

    return windowsCpu.getUsage();
}



/*#include "CpuMonitor.h"

#include <windows.h>
#include <winreg.h>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>


// ============================================================
// Helper: Convert FILETIME to unsigned long long
// ============================================================

static unsigned long long fileTimeToULL(FILETIME ft)
{
    ULARGE_INTEGER value;

    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;

    return value.QuadPart;
}


// ============================================================
// Constructor
// ============================================================

CpuMonitor::CpuMonitor()
{
    initializeCpuInfo();
}


// ============================================================
// Initialize CPU Information
// ============================================================

void CpuMonitor::initializeCpuInfo()
{
    HKEY hKey = nullptr;

    LONG result = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0,
        KEY_READ,
        &hKey
    );

    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to open CPU registry key.\n";
        return;
    }


    // ========================================================
    // CPU Name
    // ========================================================

    char buffer[256] = {};
    DWORD bufferSize = sizeof(buffer);

    if (RegQueryValueExA(
            hKey,
            "ProcessorNameString",
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(buffer),
            &bufferSize
        ) == ERROR_SUCCESS)
    {
        cpuName = buffer;
    }


    // ========================================================
    // CPU Vendor
    // ========================================================

    bufferSize = sizeof(buffer);
    ZeroMemory(buffer, sizeof(buffer));

    if (RegQueryValueExA(
            hKey,
            "VendorIdentifier",
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(buffer),
            &bufferSize
        ) == ERROR_SUCCESS)
    {
        vendorName = buffer;
    }
    else
    {
        vendorName = "Unknown";
    }


    // ========================================================
    // Base / Current Clock
    //
    // "~MHz" is the frequency reported by Windows for the
    // processor.
    // ========================================================

    DWORD mhz = 0;
    DWORD mhzSize = sizeof(mhz);

    if (RegQueryValueExA(
            hKey,
            "~MHz",
            nullptr,
            nullptr,
            reinterpret_cast<LPBYTE>(&mhz),
            &mhzSize
        ) == ERROR_SUCCESS)
    {
        baseClock = static_cast<float>(mhz);

        currentClock = static_cast<float>(mhz);
    }


    RegCloseKey(hKey);


    // ========================================================
    // Processor Information
    // ========================================================

    SYSTEM_INFO info;

    GetSystemInfo(&info);

    numberOfLogicalProcessors =
        info.dwNumberOfProcessors;


    // ========================================================
    // Physical Core Count
    //
    // GetActiveProcessorCount() gives the number of logical
    // processors, so use GetLogicalProcessorInformationEx()
    // to determine physical cores.
    // ========================================================

    DWORD length = 0;

    GetLogicalProcessorInformationEx(
        RelationProcessorCore,
        nullptr,
        &length
    );

    if (length > 0)
    {
        std::vector<unsigned char> buffer(length);

        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
            processorInfo =
            reinterpret_cast<
                PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
            >(buffer.data());

        if (GetLogicalProcessorInformationEx(
                RelationProcessorCore,
                processorInfo,
                &length
            ))
        {
            DWORD offset = 0;

            numberOfCores = 0;

            while (offset < length)
            {
                PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
                    current =
                    reinterpret_cast<
                        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX
                    >(buffer.data() + offset);

                if (current->Relationship ==
                    RelationProcessorCore)
                {
                    numberOfCores++;
                }

                offset += current->Size;
            }
        }
    }


    // ========================================================
    // Fallback
    // ========================================================

    if (numberOfCores == 0)
    {
        numberOfCores =
            numberOfLogicalProcessors;
    }
}


// ============================================================
// CPU Name
// ============================================================

std::string CpuMonitor::getCpuName() const
{
    return cpuName;
}


// ============================================================
// CPU Vendor
// ============================================================

std::string CpuMonitor::getVendorName() const
{
    return vendorName;
}


// ============================================================
// Number of Physical Cores
// ============================================================

uint32_t CpuMonitor::getNumberOfCores() const
{
    return numberOfCores;
}


// ============================================================
// Number of Logical Processors
// ============================================================

uint32_t CpuMonitor::getNumberOfLogicalProcessors() const
{
    return numberOfLogicalProcessors;
}


// ============================================================
// Base Clock
// ============================================================

float CpuMonitor::getBaseClock() const
{
    return baseClock;
}


// ============================================================
// Current Clock
// ============================================================

float CpuMonitor::getCurrentClock() const
{
    return currentClock;
}


// ============================================================
// CPU Usage
// ============================================================

float CpuMonitor::getCpuUsage()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;


    // ========================================================
    // Get current CPU times
    // ========================================================
\
    if (!GetSystemTimes(
            &idleTime,
            &kernelTime,
            &userTime
        ))
    {
        return 0.0f;
    }


    unsigned long long idle =
        fileTimeToULL(idleTime);

    unsigned long long kernel =
        fileTimeToULL(kernelTime);

    unsigned long long user =
        fileTimeToULL(userTime);


    // ========================================================
    // First sample
    // ========================================================

    if (firstSample)
    {
        previousIdleTime = idle;
        previousKernelTime = kernel;
        previousUserTime = user;

        firstSample = false;

        return 0.0f;
    }


    // ========================================================
    // Calculate differences
    // ========================================================

    unsigned long long idleDifference =
        idle - previousIdleTime;

    unsigned long long kernelDifference =
        kernel - previousKernelTime;

    unsigned long long userDifference =
        user - previousUserTime;


    unsigned long long totalDifference =
        kernelDifference +
        userDifference;


    // ========================================================
    // Calculate CPU usage
    // ========================================================

    float cpuUsage = 0.0f;

    if (totalDifference > 0)
    {
        cpuUsage =
            static_cast<float>(
                totalDifference -
                idleDifference
            )
            /
            static_cast<float>(
                totalDifference
            )
            *
            100.0f;
    }


    // ========================================================
    // Save current values
    // ========================================================

    previousIdleTime = idle;
    previousKernelTime = kernel;
    previousUserTime = user;


    return cpuUsage;
}*/