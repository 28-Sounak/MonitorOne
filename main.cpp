#include <iostream>

#include <thread>

#include <chrono>

#include "src/Core/CpuMonitor.h"

int main()
{
    CpuMonitor cpuMonitor;

    while(true)
    {
        float CpuUsage = cpuMonitor.getUsage();

        std :: cout << "CPU Ysage: " << cpuUsage << "%\n";

        std :: this_thread :: sleep_for(std :: chrono :: seconds(1));
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

std::cout << "\n================ REAL-TIME MONITORING =================\n";

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