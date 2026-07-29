#pragma once

#include<cstdint>

class CpuMonitor
{
    //CPU usage values
    private:
    unsigned long long previousIdleTime = 0;
    unsigned long long previousKernelTime = 0;
    unsigned long long previousUserTime = 0;
    bool firstSample = true;

    //Returns CPU usage percentage
    public:
    float getCpuUsage();
};