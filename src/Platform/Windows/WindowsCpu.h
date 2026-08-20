#pragma once

#include <windows.h>

class WindowsCpu
{
    public:
        WindoesCpu();

        float getUsage();

    private:
        ULONGLONG previousIdletime;
        
        ULONGLONG previousKernelTime;

        ULONGLONG previousUserTime;

        ULONG fileTimeToULL(const FILETIME& fileTime);
}