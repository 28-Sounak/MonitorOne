#include<iostream>
#include<thread>
#include<chrono>

#include "CpuMonitor.h"
#include "GpuMonitor.h"

int main()
{
    CpuMonitor cpu;
    GpuMonitor gpu;

    std::cout << "GPU Name: " << gpu.getGpuName() << "\n";
    std::cout << "Vendor: " << gpu.getVendorName() << "\n";
    std::cout << "Vendor ID: " << gpu.getVendorId() << "\n";
    std::cout << "Device ID: " << gpu.getDeviceId() << "\n";

    std::cout << "Dedicated VRAM: " << gpu.getDedicatedMemoryTotal() / (1024 * 1024) << " MiB\n";

    std::cout << "Shared System Memory: " << gpu.getSharedMemoryTotal() / (1024 * 1024) << " MiB\n\n";

    std::cout << "CPU Name: " << cpu.getCpuName() << '\n';
    std::cout << "Vendor: " << cpu.getVendorName() << '\n';
    std::cout << "Logical Processors: "
          << cpu.getNumberOfLogicalProcessors() << '\n';
    std::cout << "Base Clock: " << cpu.getBaseClock() << " MHz\n";
    std::cout << "Current Clock: " << cpu.getCurrentClock() << " MHz\n";

    while(true)
    {
        std::cout
        <<"CPU Usage: "
        <<cpu.getCpuUsage()
        <<"%\n";

        std::cout
        <<"GPU Usage: "
        <<gpu.getGpuUsage()
        <<"%\n";

        std::cout 
        << "Dedicated Memory Used : "
        << gpu.getDedicatedMemoryUsed() / (1024 * 1024)
        << " MB\n";

        std::cout << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}