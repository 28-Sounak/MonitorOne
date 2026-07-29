#include "CpuMonitor.h"

#include<windows.h>

#include<iostream>

//Convert FILETIME to unsigned long long
static unsigned long long fileTimeToULL(FILETIME ft)
{
    ULARGE_INTEGER value;

    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;
    
    return value.QuadPart;
}

float CpuMonitor::getCpuUsage()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    //Get current CPU times
    if(!GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        return 0.0f;
    }

    unsigned long long idle = fileTimeToULL(idleTime);

    unsigned long long kernel = fileTimeToULL(kernelTime);

    unsigned long long user = fileTimeToULL(userTime);

    //std::cout << "Idle: " << idle << "\n";

    //std::cout << "Kernel: " << kernel << "\n";

    //std::cout << "User: " << user << "\n";

    //First call - no previous data
    //if(previousIdleTime == 0)
    if(firstSample)
    {
        previousIdleTime = idle;
        previousKernelTime = kernel;
        previousUserTime = user;

        firstSample = false;

        return 0.0f;
    }

    //Calculate differences
    unsigned long long idleDifference = idle - previousIdleTime;

    unsigned long long kernelDifference = kernel - previousKernelTime;

    unsigned long long userDifference = user - previousUserTime;

    unsigned long long totalDifference = kernelDifference + userDifference;

    //std::cout << "Idle Diff: " << idleDifference << '\n';
    
    //std::cout << "Kernel Diff: " << kernelDifference << '\n';

    //std::cout << "User Diff: " << userDifference << '\n';

    //std::cout << "Total Diff: " << totalDifference << '\n';

    float cpuUsage = 0.0f;

    if(totalDifference > 0)
    {
        cpuUsage = ((float)(totalDifference - idleDifference) / totalDifference) * 100.0f;
    }

    //Save current values for next calculation
    previousIdleTime = idle;
    previousKernelTime = kernel;
    previousUserTime = user;

    return cpuUsage;
}