#pragma once

#include <chrono.h>

#include<winsock2.h> //Necessary for basic network hostname resolution, IP definitions, and socket operations.

#include<windows.h>

#include<iphlpapi.h> //For getting IP addresses, network adapter configurations, routing tables, and statistics.

#include<wlanapi.h> //For specific Wi-Fi network details like signal strength or available wireless SSIDs. 

#include<cstdint>

#include<string>

#include<vector>

struct NetworkAdapter
{
    //Basic Information
    std::string adapterName;
    std::string description;
    std::string interfaceType;
    std::string status;

    //Hardware Information
    std::string MACAddress;

    //IP Configuration
    std::string IPv4Address;
    std::string IPv6Address;
    std::string SubnetMask;
    std::string DefaultGateway;

    //DNS / DHCP
    std::string DNSServers;
    std::string DHCP;
    std::string DHCPServer;

    //Interface Information
    uint32_t InterfaceIndex = 0;
    uint32_t LinkSpeed = 0;
    uint32_t MTU = 0;

    //Runtime Statistice
    uint64_t downloadSpeed = 0;
    uint64_t uploadSpeed = 0;
    uint64_t bytesReceived = 0;
    uint64_t bytesSent = 0;

    //Wi-Fi Specific Information
    bool isWifi = false;
    std::string wifiSSID;
    std::string WifiBSSID;
    int wifiSignalStrength = 0;

};

class NetworkMonitor
{
private:

    // Stores information about all detected network adapters
    std::vector<NetworkAdapter> adapters;

    // Initializes network adapter information
    void initializeAdapters();

    // Updates runtime network statistics
    void updateNetworkStats();

    // Converts interface type to readable text
    std::string getInterfaceTypeName(uint32_t type);

    // Converts adapter status to readable text
    std::string getAdapterStatusName(uint32_t status);

    // Converts a Windows socket address to a readable IP address
    std::string getIPAddress(const SOCKADDR* address);

    // Determines whether an adapter is Wi-Fi
    bool isWiFiAdapter(const NetworkAdapter& adapter);


public:

    // Constructor
    NetworkMonitor();

    // Returns all detected network adapters
    const std::vector<NetworkAdapter>& getAdapters();

    // Returns total download speed
    uint64_t getDownloadSpeed();

    // Returns total upload speed
    uint64_t getUploadSpeed();
};