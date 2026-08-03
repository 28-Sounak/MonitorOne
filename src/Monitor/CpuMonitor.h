#pragma once

#include<cstdint>
#include<string>

class CpuMonitor
{
    //CPU usage values
    private:

    unsigned long long previousIdleTime = 0;
    unsigned long long previousKernelTime = 0;
    unsigned long long previousUserTime = 0;
    bool firstSample = true;

    //Cached CPU information
    std::string cpuName;
    std::string vendorName;

    uint32_t numberOfCores = 0;
    uint32_t numberOfLogicalProcessors = 0;

    float baseClock = 0.0f;
    float currentClock = 0.0f;

    void initializeCpuInfo();

    //Returns CPU usage percentage 
    public:
    CpuMonitor();

    float getCpuUsage();

    std::string getCpuName();
    std::string getVendorName();

    uint32_t getNumberOfCores();
    uint32_t getNumberOfLogicalProcessors();

    float getBaseClock();
    float getCurrentClock();
};