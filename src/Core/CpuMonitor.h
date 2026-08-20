#pragma once

class CpuMonitor
{
    public:
        CpuMonitor();

        std :: string getName();

        std :: string getVendor();

        std :: string getArchitecture();

        unsigned int getPhysicalCoreCount();

        unsigned int getLogicalProcessorCount();

        float getUsage();

    /*private:
        unsigned long long previousIdleTime;

        unsigned long long perviousKernelTime;

        unsigned long long previousUserTime;*/
}