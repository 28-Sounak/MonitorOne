#pragma once

#include<cstdint>
#include<string>

#ifdef _WIN32
#include <windows.h>
#endif

class CpuMonitor
{
    //CPU usage values
    private:

    #ifdef _WIN32

    //CPU Usage Calculation
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

    //CPU Information Initialization
    void initializeCpuInfo();

    //Individual CPU Information Functions
    std::string readCpuName();
    std::string readVendorName();

    uint32_t readNumberOfCores();
    uint32_t readNumberOfLogicalProcessors();

    float readBaseClock();
    float readCurrentClock();

    #endif

    //Returns CPU usage percentage 
    public:

    //Constructor
    CpuMonitor();

    //CPU Usage
    float getCpuUsage();

    //CPU Information
    std::string getCpuName() const;
    std::string getVendorName() const;

    uint32_t getNumberOfCores() const;
    uint32_t getNumberOfLogicalProcessors() const;

    float getBaseClock() const;
    float getCurrentClock() const;
};