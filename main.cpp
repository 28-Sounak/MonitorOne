#include<iostream>
#include<thread>
#include<chrono>
#include<CpuMonitor.h>

int main()
{
    CpuMonitor cpu;

    while(true)
    {
        std::cout
        <<"CPU Usage: "
        <<cpu.getCpuUsage()
        <<"\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}