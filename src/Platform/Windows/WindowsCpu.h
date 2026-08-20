#pragma once

#include <windows.h>

class WindowsCpu
{
    public:
        WindoesCpu();

        std :: string getName();

        std :: string getVendor();

        std :: string getArchitecture();

        unsigned int getPhysicalCoreCount();

        unsigned int getLogicalProcessorCount();

        float getUsage();

    private:
        ULONGLONG previousIdletime;
        
        ULONGLONG previousKernelTime;

        ULONGLONG previousUserTime;

        ULONG fileTimeToULL(const FILETIME& fileTime);
}