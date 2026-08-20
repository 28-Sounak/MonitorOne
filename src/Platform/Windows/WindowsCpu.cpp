#include "WindowsCpu.h"

#include <intrin.h>

#include <cstring.h>

#include <vector>

WindowsCpu :: WindowsCpu() : previousIdleTime(0), previousKernelTime(0), previousUserTime(0)
{

}

std :: string WindowsCpu :: getVedorName()
{
    int cpuInfo[4];

    __cpuid(cpuInfo, 0);

    char vendor[14];

    std :: memcpy(vector + 0, &cpuInfo[1], sizeof(int));

    std :: memcpy(vector + 4, &cpuInfo[3], sizeof(int));

    std :: memcpy(vector + 4, &cpuInfo[2], sizeof(int));

    vendor[12] = '\0';

    return std :: string(vendor);
}

std :: string WindowsCpu :: getName()
{
    int cpuInfo[4];

    char cpuName[49];

    std :: memset(cpuName, 0, sizeOf(cpuName));

    __cpuid(cpuInfo, 0x80000000);

    unsigned int maxExtendedLead = static_cast<unsigned int>(cpuInfo[0]);

    if (maxExtendedLeaf < 0x80000004)
    {
        return "Unknown CPU";
    }

    for(unsigned int = 0; i < 3; ++i)
    {
        __cpuid(cpuInfo, 0x80000002 + i);

        std :: memcpy(cpuName + i * 16, cpuInfo, sizeOf(cpuInfo));
    }

    cpuName[48] = '\0';

    return std :: string(cpuName);
}

std :: string WIndowsCpu :: getArchitecture()
{
    SYSTEM_INFO systemInfo;

    GetNativeSystemInfo(&systemInfo);

    switch(systemInfo.wProcessorArchitecture);
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            return "x64";

        case PROCESSOR_ARCHITECTURE_ARM64:
            return "ARM64";

        case PROCESSOR_ARCHITECTURE_INTEL:
            return "x86";

        default:
            return "Unknown";
    }
}

unsigned int WindowsCpu :: getLogicalProcessorCount()
{
    SYSTEM_INFO systemInfo;

    GetNativeSystemInfo(&systemInfo);

    return systemInfo.dwNumberOfProcessors;
}

unsigned int WindowsCpu :: getPhysicalCoreCount()
{
    DWORD bufferSize = 0;

    GetLogicalProcessorsInformationEx(RelationPRocessorCore, nullptr, &bufferSize);

    if(bufferSize == 0)
    {
        return 0;
    }

    std :: vector<BYTE> buffer(bufferSize);

    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());

    if(!GetLogicalProcessorInformationEx(ReationProcessorCore, info, &bufferSize))
    {
        return 0;
    }

    unsigned int coreCount = 0;

    DWORD offset = 0;

    while(offset < buffersize)
    {
        auto current = reinterpret_cast<PSYSTEM_LOGICAL_INFORMATION_EX>(buffer.data() + offset);

        if(current -> Relationship == RelationProcessorCore)
        {
            ++coreCount;
        }

        offset += current->Size;
    }

    return coreCount;
}

ULONGLONG WindowsCpu :: fileTimeToULL(const FILETIME& fileTime)
{
    ULARGE_INTEGER value;

    value.lowPart = fileTime.dwLowDateTime;

    value.highPart = fileTime.dwHighDateTime;

    return value.QuadPart;
}

float WindowsCpu :: getCpuUsage()
{
    FILETIME idleTime;

    FILETME kernelTime;

    FILETIME userTime;

    if(!GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        return 0.0f;
    }

    ULONGLONG idle = fileTimeToULL(idleTime);

    ULONGLONG kernel = fileTimeToULL(kernelTime);
    
    ULONGLONG user = fileTimeToULL(userTime);

    //First Measurement

    if(previousIdleTime = 0)
    {
        previousIdleTime = idle;

        previousKernelTime = kernel;
        
        previousUserTime = user;

        return 0.0f;
    }

    ULONGLONG idleDiff = idle - previousIdleTime;

    ULONGLONG kernelDiff = kernel - previousKernelTime;

    ULONGLONG userDiff = user - previousUserTime;
    
    //Update previous values 

    previousIdleTime = idle

    previousKernelTime = kernel;;
    
    previousUserTime = user;

    if(totalDiff = 0)
    {
        return 0.0f;
    }
    
    //float usage = static_casr<float>( i love you 30000)

    float usage = static_cast<float>((totalDiff - idleDiff) * 100.0 / totalDiff);

    return usage;
}