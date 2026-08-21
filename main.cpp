#include <iostream>

#include <thread>

#include <chrono>

#include <iomanip>

#include <cstdint>

#include "src/Core/CpuMonitor.h"

#include "src/Core/MemoryMonitor.h"


//Helper: Convert Bytes to GB

double bytesToGB(uint64_t bytes)
{
    return static_cast<double>(bytes) /
           (1024.0 * 1024.0 * 1024.0);
}

//MAIN

int main()
{
    CpuMonitor cpuMonitor;

    MemoryMonitor memoryMonitor;

    GpuMonitor gpu;

    //CPU  Information

    std :: cout << "CPU Information:\n";
    std::cout << "============================\n";

    std::cout << "Name: " << cpu.getName() << '\n';

    std::cout << "Vendor: " << cpu.getVendor() << '\n';

    std::cout << "Architecture: " << cpu.getArchitecture() << '\n';

    std::cout << "Physical Cores: " << cpu.getPhysicalCoreCount() << '\n';

    std::cout << "Logical Processors: " << cpu.getLogicalProcessorCount() << '\n';

    //Memory Module Information

    std :: cout << "Memory Modules \n";

    std::vector<MemoryModule> modules =
        memory.getModules();


    if (modules.empty())
    {
        std::cout << "No memory module information available.\n";
    }
    else
    {
        for (size_t i = 0; i < modules.size(); ++i)
        {
            const MemoryModule& module =
                modules[i];

            std::cout << "Module " << i + 1 << '\n';
            std::cout << "----------------------------------------\n";

            std::cout << "Size: "
                      << std::fixed
                      << std::setprecision(2)
                      << bytesToGB(module.size)
                      << " GB\n";

            std::cout << "Manufacturer: "
                      << module.manufacturer
                      << '\n';

            std::cout << "Type: "
                      << module.type
                      << '\n';

            std::cout << "Form Factor: "
                      << module.formFactor
                      << '\n';

            std::cout << "Speed: "
                      << module.speed
                      << " MT/s\n";

            std::cout << "Configured Speed: ";

            if (!module.configuredClockSpeed.empty())
            {
                std::cout
                    << module.configuredClockSpeed
                    << " MT/s\n";
            }
            else
            {
                std::cout
                    << "Unknown\n";
            }

            std::cout << "Part Number: "
                      << module.partNumber
                      << '\n';

            std::cout << "Serial Number: "
                      << module.serialNumber
                      << "\n\n";
        }
    }

    // GPU INFORMATION


    std::cout << "             GPU INFORMATION\n"
        << "========================================\n\n";

    std::cout
        << "GPU: "
        << gpu.getGpuName()
        << '\n';


    std::cout
        << "Vendor: "
        << gpu.getVendorName()
        << '\n';


    std::cout
        << "Vendor ID: 0x"
        << std::hex
        << gpu.getVendorId()
        << std::dec
        << '\n';


    std::cout
        << "Device ID: 0x"
        << std::hex
        << gpu.getDeviceId()
        << std::dec
        << '\n';


    // --------------------------------------------------------
    // VRAM
    // --------------------------------------------------------

    std::cout
        << "\nDedicated VRAM: "
        << bytesToGB(
               gpu.getDedicatedMemoryTotal()
           )
        << " GB\n";


    std::cout
        << "Shared GPU Memory: "
        << bytesToGB(
               gpu.getSharedMemoryTotal()
           )
        << " GB\n";


    // --------------------------------------------------------
    // Telemetry
    // --------------------------------------------------------

    std::cout
        << "\nGPU Usage: "
        << gpu.getGpuUsage()
        << "%\n";


    std::cout
        << "Temperature: "
        << gpu.getTemperature()
        << " °C\n";


    std::cout
        << "Fan Speed: "
        << gpu.getFanSpeed()
        << " RPM\n";


    std::cout
        << "Core Clock: "
        << gpu.getCoreClock()
        << " MHz\n";


    std::cout
        << "Memory Clock: "
        << gpu.getMemoryClock()
        << " MHz\n";


    //Live System

    std::cout << "Live System Monitor\n";
    std::cout << "============================\n";

    while (true)
    {
        float cpuUsage =
            cpu.getUsage();


        uint64_t totalMemory =
            memory.getTotalMemory();


        uint64_t usedMemory =
            memory.getUsedMemory();


        uint64_t availableMemory =
            memory.getAvailableMemory();


        float memoryUsage =
            memory.getUsage();


        uint64_t gpuMemoryTotal =
            gpu.getDedicatedMemoryTotal();


        uint64_t gpuMemoryUsed =
            gpu.getDedicatedMemoryUsed();


        uint64_t gpuMemoryAvailable =
            gpu.getDedicatedMemoryAvailable();


        float gpuMemoryUsage =
            gpu.getDedicatedMemoryUsage();


        float gpuUsage =
            gpu.getGpuUsage();


        float temperature =
            gpu.getTemperature();


        uint32_t fanSpeed =
            gpu.getFanSpeed();


        uint32_t coreClock =
            gpu.getCoreClock();


        uint32_t memoryClock =
            gpu.getMemoryClock();


        // ----------------------------------------------------
        // Clear console
        // ----------------------------------------------------

        std::cout
            << "\033[2J\033[H";


        // ====================================================
        // CPU
        // ====================================================

        std::cout
            << "========================================\n"
            << "                CPU\n"
            << "========================================\n";


        std::cout
            << "Usage: "
            << std::fixed
            << std::setprecision(2)
            << cpuUsage
            << "%\n";


        // ====================================================
        // MEMORY
        // ====================================================

        std::cout
            << "\n========================================\n"
            << "               MEMORY\n"
            << "========================================\n";


        std::cout
            << "Total: "
            << bytesToGB(totalMemory)
            << " GB\n";


        std::cout
            << "Used: "
            << bytesToGB(usedMemory)
            << " GB\n";


        std::cout
            << "Available: "
            << bytesToGB(availableMemory)
            << " GB\n";


        std::cout
            << "Usage: "
            << memoryUsage
            << "%\n";


        // ====================================================
        // GPU
        // ====================================================

        std::cout
            << "\n========================================\n"
            << "                 GPU\n"
            << "========================================\n";


        std::cout
            << "GPU: "
            << gpu.getGpuName()
            << '\n';


        std::cout
            << "GPU Usage: "
            << gpuUsage
            << "%\n";


        std::cout
            << "VRAM Total: "
            << bytesToGB(gpuMemoryTotal)
            << " GB\n";


        std::cout
            << "VRAM Used: "
            << bytesToGB(gpuMemoryUsed)
            << " GB\n";


        std::cout
            << "VRAM Available: "
            << bytesToGB(gpuMemoryAvailable)
            << " GB\n";


        std::cout
            << "VRAM Usage: "
            << gpuMemoryUsage
            << "%\n";


        std::cout
            << "Temperature: "
            << temperature
            << " °C\n";


        std::cout
            << "Fan Speed: "
            << fanSpeed
            << " RPM\n";


        std::cout
            << "Core Clock: "
            << coreClock
            << " MHz\n";


        std::cout
            << "Memory Clock: "
            << memoryClock
            << " MHz\n";


        // ----------------------------------------------------
        // Refresh
        // ----------------------------------------------------

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }
    
    return 0;
}


/*#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "CpuMonitor.h"
#include "GpuMonitor.h"
#include "MemoryMonitor.h"
#include "NetworkMonitor.h"

int main()
{
    CpuMonitor cpu;
    GpuMonitor gpu;
    MemoryMonitor memory;
    NetworkMonitor network;

    // =========================================================
    // CPU INFORMATION
    // =========================================================

    std::cout << "================ CPU =================\n";

    std::cout << "CPU Name: "
              << cpu.getCpuName() << '\n';

    std::cout << "Vendor: "
              << cpu.getVendorName() << '\n';

    std::cout << "Logical Processors: "
              << cpu.getNumberOfLogicalProcessors() << '\n';

    std::cout << "Base Clock: "
              << cpu.getBaseClock()
              << " MHz\n";

    std::cout << "Current Clock: "
              << cpu.getCurrentClock()
              << " MHz\n";


    // =========================================================
    // GPU INFORMATION
    // =========================================================

    std::cout << "\n================ GPU =================\n";

    std::cout << "GPU Name: "
              << gpu.getGpuName() << '\n';

    std::cout << "Vendor: "
              << gpu.getVendorName() << '\n';

    std::cout << "Vendor ID: "
              << gpu.getVendorId() << '\n';

    std::cout << "Device ID: "
              << gpu.getDeviceId() << '\n';

    std::cout << "Dedicated VRAM: "
              << gpu.getDedicatedMemoryTotal() / (1024 * 1024)
              << " MiB\n";

    std::cout << "Shared System Memory: "
              << gpu.getSharedMemoryTotal() / (1024 * 1024)
              << " MiB\n";


    // =========================================================
    // MEMORY INFORMATION
    // =========================================================

    std::cout << "\n================ MEMORY =================\n";

    std::cout << "Total Memory: "
              << memory.getTotalMemory() / (1024 * 1024)
              << " MB\n";

    std::cout << "Available Memory: "
              << memory.getAvailableMemory() / (1024 * 1024)
              << " MB\n";

    std::cout << "Used Memory: "
              << memory.getUsedMemory() / (1024 * 1024)
              << " MB\n";

    std::cout << "Memory Usage: "
              << memory.getMemoryUsage()
              << "%\n";


    // =========================================================
    // NETWORK INFORMATION
    // =========================================================

    std::cout << "\n================ NETWORK =================\n";

    const auto& adapters = network.getAdapters();

    for (const auto& adapter : adapters)
    {
        std::cout << "\n------------------------------------------\n";

        std::cout << "Adapter Name: "
                  << adapter.adapterName << '\n';

        std::cout << "Description: "
                  << adapter.description << '\n';

        std::cout << "Interface Type: "
                  << adapter.interfaceType << '\n';

        std::cout << "Status: "
                  << adapter.status << '\n';

        std::cout << "MAC Address: "
                  << adapter.macAddress << '\n';

        std::cout << "IPv4 Address: "
                  << adapter.ipv4Address << '\n';

        std::cout << "IPv6 Address: "
                  << adapter.ipv6Address << '\n';

        std::cout << "Subnet Mask: "
                  << adapter.subnetMask << '\n';

        std::cout << "Default Gateway: "
                  << adapter.defaultGateway << '\n';

        std::cout << "DNS Servers: "
                  << adapter.dnsServers << '\n';

        std::cout << "DHCP: "
                  << adapter.dhcp << '\n';

        std::cout << "DHCP Server: "
                  << adapter.dhcpServer << '\n';

        std::cout << "Interface Index: "
                  << adapter.interfaceIndex << '\n';

        std::cout << "Link Speed: "
                  << adapter.linkSpeed
                  << " bps\n";

        std::cout << "MTU: "
                  << adapter.mtu << '\n';

        std::cout << "Wi-Fi: "
                  << (adapter.isWifi ? "Yes" : "No")
                  << '\n';

        if (adapter.isWifi)
        {
            std::cout << "SSID: "
                      << adapter.wifiSSID << '\n';

            std::cout << "BSSID: "
                      << adapter.wifiBSSID << '\n';

            std::cout << "Signal Strength: "
                      << adapter.wifiSignalStrength
                      << "%\n";
        }
    }


    // =========================================================
    // CONTINUOUS MONITORING
    // =========================================================

    /*std::cout << "\n================ REAL-TIME MONITORING =================\n";

    while (true)
    {
        // CPU
        float cpuUsage =
            cpu.getCpuUsage();


        // GPU
        float gpuUsage =
            gpu.getGpuUsage();

        uint64_t gpuMemoryUsed =
            gpu.getDedicatedMemoryUsed();


        // Memory
        float memoryUsage =
            memory.getMemoryUsage();


        // Network
        uint64_t downloadSpeed =
            network.getDownloadSpeed();

        uint64_t uploadSpeed =
            network.getUploadSpeed();


        // -----------------------------------------------------
        // Display
        // -----------------------------------------------------

        std::cout << "\n------------------------------------------\n";

        std::cout << std::fixed
                  << std::setprecision(2);

        std::cout << "CPU Usage: "
                  << cpuUsage
                  << "%\n";

        std::cout << "GPU Usage: "
                  << gpuUsage
                  << "%\n";

        std::cout << "GPU Dedicated Memory Used: "
                  << gpuMemoryUsed / (1024 * 1024)
                  << " MB\n";

        std::cout << "Memory Usage: "
                  << memoryUsage
                  << "%\n";

        std::cout << "Download Speed: "
          << network.getDownloadSpeed()
          << " B/s\n";

        std::cout << "Upload Speed: "
          << network.getUploadSpeed()
          << " B/s\n";


        // Wait one second
        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }*/

    // =========================================================
// CONTINUOUS MONITORING
// =========================================================

/*std::cout << "\n================ REAL-TIME MONITORING =================\n";

while (true)
{
    std::cout << "Getting CPU...\n";

    float cpuUsage = cpu.getCpuUsage();

    std::cout << "Getting GPU...\n";

    float gpuUsage = gpu.getGpuUsage();

    std::cout << "Getting GPU memory...\n";

    uint64_t gpuMemoryUsed =
        gpu.getDedicatedMemoryUsed();

    std::cout << "Getting memory...\n";

    float memoryUsage =
        memory.getMemoryUsage();

    std::cout << "Getting download speed...\n";

    uint64_t downloadSpeed =
        network.getDownloadSpeed();

    std::cout << "Getting upload speed...\n";

    uint64_t uploadSpeed =
        network.getUploadSpeed();

    std::cout << "\n------------------------------------------\n";

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "CPU Usage: "
              << cpuUsage
              << "%\n";

    std::cout << "GPU Usage: "
              << gpuUsage
              << "%\n";

    std::cout << "GPU Dedicated Memory Used: "
              << gpuMemoryUsed / (1024 * 1024)
              << " MB\n";

    std::cout << "Memory Usage: "
              << memoryUsage
              << "%\n";

    std::cout << "Download Speed: "
              << downloadSpeed
              << " B/s\n";

    std::cout << "Upload Speed: "
              << uploadSpeed
              << " B/s\n";

    std::this_thread::sleep_for(
        std::chrono::seconds(1)
    );
}

    return 0;
}*/