#include "WindowsCpu.h"

WindowsCpu :: WindowsCpu() : previousIdleTime(0), previousKernelTime(0), previousUserTime(0)
{

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