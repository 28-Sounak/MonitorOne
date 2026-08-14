#pragma once

// ------------------------------------------------------------
// Windows networking headers
// IMPORTANT: winsock2.h must come before windows.h
// ------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <wlanapi.h>

// ------------------------------------------------------------
// C++ headers
// ------------------------------------------------------------

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>


// ============================================================
// NetworkAdapter
// ============================================================

struct NetworkAdapter
{
    // --------------------------------------------------------
    // Basic Information
    // --------------------------------------------------------

    std::string adapterName;
    std::string description;
    std::string interfaceType;
    std::string status;


    // --------------------------------------------------------
    // Hardware Information
    // --------------------------------------------------------

    std::string macAddress;


    // --------------------------------------------------------
    // IP Configuration
    // --------------------------------------------------------

    std::string ipv4Address;
    std::string ipv6Address;
    std::string subnetMask;
    std::string defaultGateway;


    // --------------------------------------------------------
    // DNS / DHCP
    // --------------------------------------------------------

    std::string dnsServers;
    std::string dhcp;
    std::string dhcpServer;


    // --------------------------------------------------------
    // Interface Information
    // --------------------------------------------------------

    uint32_t interfaceIndex = 0;

    // Link speed is reported by Windows in bits per second
    uint64_t linkSpeed = 0;

    uint32_t mtu = 0;


    // --------------------------------------------------------
    // Runtime Statistics
    // --------------------------------------------------------

    uint64_t downloadSpeed = 0;
    uint64_t uploadSpeed = 0;

    uint64_t bytesReceived = 0;
    uint64_t bytesSent = 0;


    // --------------------------------------------------------
    // Wi-Fi Specific Information
    // --------------------------------------------------------

    bool isWifi = false;

    std::string wifiSSID;
    std::string wifiBSSID;

    // Wi-Fi signal quality: 0 - 100
    int wifiSignalStrength = 0;
};


// ============================================================
// NetworkMonitor
// ============================================================

class NetworkMonitor
{
private:

    // --------------------------------------------------------
    // Adapter information
    // --------------------------------------------------------

    std::vector<NetworkAdapter> adapters;


    // --------------------------------------------------------
    // Runtime statistics timing
    // --------------------------------------------------------

    std::chrono::steady_clock::time_point lastStatsUpdate;

    bool firstStatsSample = true;


    // --------------------------------------------------------
    // Initialization
    // --------------------------------------------------------

    void initializeAdapters();


    // --------------------------------------------------------
    // Network statistics
    // --------------------------------------------------------

    void updateNetworkStats();


    // --------------------------------------------------------
    // Helper functions
    // --------------------------------------------------------

    static std::string getInterfaceTypeName(
        uint32_t type
    );

    static std::string getAdapterStatusName(
        uint32_t status
    );

    static std::string getIPAddress(
        const SOCKADDR* address
    );

    static bool isWiFiAdapter(
        const NetworkAdapter& adapter
    );


public:

    // --------------------------------------------------------
    // Constructor
    // --------------------------------------------------------

    NetworkMonitor();


    // --------------------------------------------------------
    // Adapter information
    // --------------------------------------------------------

    const std::vector<NetworkAdapter>& getAdapters();


    // --------------------------------------------------------
    // Total network speeds
    // --------------------------------------------------------

    uint64_t getDownloadSpeed();

    uint64_t getUploadSpeed();
};