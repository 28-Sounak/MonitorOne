#include "NetworkMonitor.h"

// ------------------------------------------------------------
// C++ headers
// ------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <algorithm>


// ============================================================
// Helper: Convert Windows wide string to UTF-8 std::string
// ============================================================

static std::string wideToString(const wchar_t* wideString)
{
    if (wideString == nullptr)
    {
        return "";
    }


    int sizeNeeded =
        WideCharToMultiByte(
            CP_UTF8,
            0,
            wideString,
            -1,
            nullptr,
            0,
            nullptr,
            nullptr
        );


    if (sizeNeeded <= 0)
    {
        return "";
    }


    // Allocate space including terminating null character
    std::string result(
        sizeNeeded,
        '\0'
    );


    WideCharToMultiByte(
        CP_UTF8,
        0,
        wideString,
        -1,
        result.data(),
        sizeNeeded,
        nullptr,
        nullptr
    );


    // Remove terminating null character
    if (!result.empty() &&
        result.back() == '\0')
    {
        result.pop_back();
    }


    return result;
}


// ============================================================
// Constructor
// ============================================================

NetworkMonitor::NetworkMonitor()
{
    initializeAdapters();


    lastStatsUpdate =
        std::chrono::steady_clock::now();


    // Take initial statistics sample
    updateNetworkStats();
}


// ============================================================
// Initialize network adapters
// ============================================================

void NetworkMonitor::initializeAdapters()
{
    adapters.clear();


    // --------------------------------------------------------
    // Allocate buffer for adapter information
    // --------------------------------------------------------

    ULONG bufferSize = 15000;


    std::vector<unsigned char> buffer(
        bufferSize
    );


    PIP_ADAPTER_ADDRESSES adapterAddresses =
        reinterpret_cast<PIP_ADAPTER_ADDRESSES>(
            buffer.data()
        );


    ULONG flags =
        GAA_FLAG_INCLUDE_PREFIX |
        GAA_FLAG_INCLUDE_GATEWAYS;


    // --------------------------------------------------------
    // Get adapter information
    // --------------------------------------------------------

    ULONG result =
        GetAdaptersAddresses(
            AF_UNSPEC,
            flags,
            nullptr,
            adapterAddresses,
            &bufferSize
        );


    // --------------------------------------------------------
    // Buffer was too small
    // --------------------------------------------------------

    if (result == ERROR_BUFFER_OVERFLOW)
    {
        buffer.resize(bufferSize);


        adapterAddresses =
            reinterpret_cast<PIP_ADAPTER_ADDRESSES>(
                buffer.data()
            );


        result =
            GetAdaptersAddresses(
                AF_UNSPEC,
                flags,
                nullptr,
                adapterAddresses,
                &bufferSize
            );
    }


    // --------------------------------------------------------
    // Check result
    // --------------------------------------------------------

    if (result != NO_ERROR)
    {
        std::cerr
            << "GetAdaptersAddresses failed: "
            << result
            << '\n';

        return;
    }


    // ========================================================
    // Iterate through every network adapter
    // ========================================================

    for (
        PIP_ADAPTER_ADDRESSES adapter =
            adapterAddresses;

        adapter != nullptr;

        adapter = adapter->Next
    )
    {
        NetworkAdapter networkAdapter;


        // ----------------------------------------------------
        // Basic Information
        // ----------------------------------------------------

        if (adapter->AdapterName != nullptr)
        {
            networkAdapter.adapterName =
                adapter->AdapterName;
        }


        networkAdapter.description =
            wideToString(
                adapter->Description
            );


        networkAdapter.interfaceType =
            getInterfaceTypeName(
                adapter->IfType
            );


        networkAdapter.status =
            getAdapterStatusName(
                adapter->OperStatus
            );


        // ----------------------------------------------------
        // Interface Information
        // ----------------------------------------------------

        networkAdapter.interfaceIndex =
            adapter->IfIndex;


        networkAdapter.mtu =
            adapter->Mtu;


        networkAdapter.linkSpeed =
            adapter->TransmitLinkSpeed;


        // ----------------------------------------------------
        // MAC Address
        // ----------------------------------------------------

        if (adapter->PhysicalAddressLength > 0)
        {
            std::ostringstream mac;


            for (
                ULONG i = 0;

                i < adapter->PhysicalAddressLength;

                ++i
            )
            {
                if (i > 0)
                {
                    mac << ":";
                }


                mac
                    << std::uppercase
                    << std::hex
                    << std::setw(2)
                    << std::setfill('0')
                    << static_cast<int>(
                        adapter->PhysicalAddress[i]
                    );
            }


            networkAdapter.macAddress =
                mac.str();
        }


        // ====================================================
        // IP Addresses
        // ====================================================

        for (
            PIP_ADAPTER_UNICAST_ADDRESS unicast =
                adapter->FirstUnicastAddress;

            unicast != nullptr;

            unicast = unicast->Next
        )
        {
            if (
                unicast->Address.lpSockaddr ==
                nullptr
            )
            {
                continue;
            }


            ADDRESS_FAMILY family =
                unicast->Address.lpSockaddr->sa_family;


            std::string address =
                getIPAddress(
                    unicast->Address.lpSockaddr
                );


            // ------------------------------------------------
            // IPv4
            // ------------------------------------------------

            if (family == AF_INET)
            {
                if (
                    networkAdapter.ipv4Address.empty()
                )
                {
                    networkAdapter.ipv4Address =
                        address;
                }


                // --------------------------------------------
                // IPv4 subnet mask
                // --------------------------------------------

                ULONG prefixLength =
                    unicast->OnLinkPrefixLength;


                if (prefixLength <= 32)
                {
                    ULONG mask;


                    if (prefixLength == 0)
                    {
                        mask = 0;
                    }
                    else
                    {
                        mask =
                            0xFFFFFFFFu <<
                            (32 - prefixLength);
                    }


                    IN_ADDR subnetMask;


                    subnetMask.S_un.S_addr =
                        htonl(mask);


                    char maskBuffer[
                        INET_ADDRSTRLEN
                    ];


                    if (
                        inet_ntop(
                            AF_INET,
                            &subnetMask,
                            maskBuffer,
                            sizeof(maskBuffer)
                        ) != nullptr
                    )
                    {
                        networkAdapter.subnetMask =
                            maskBuffer;
                    }
                }
            }


            // ------------------------------------------------
            // IPv6
            // ------------------------------------------------

            else if (family == AF_INET6)
            {
                if (
                    networkAdapter.ipv6Address.empty()
                )
                {
                    networkAdapter.ipv6Address =
                        address;
                }
            }
        }


        // ====================================================
        // Default Gateway
        // ====================================================

        if (
            adapter->FirstGatewayAddress != nullptr &&
            adapter->FirstGatewayAddress->Address.lpSockaddr != nullptr
        )
        {
            networkAdapter.defaultGateway =
                getIPAddress(
                    adapter
                        ->FirstGatewayAddress
                        ->Address.lpSockaddr
                );
        }


        // ====================================================
        // DNS Servers
        // ====================================================

        for (
            PIP_ADAPTER_DNS_SERVER_ADDRESS dns =
                adapter->FirstDnsServerAddress;

            dns != nullptr;

            dns = dns->Next
        )
        {
            if (
                dns->Address.lpSockaddr ==
                nullptr
            )
            {
                continue;
            }


            std::string dnsAddress =
                getIPAddress(
                    dns->Address.lpSockaddr
                );


            if (
                !networkAdapter.dnsServers.empty()
            )
            {
                networkAdapter.dnsServers +=
                    ", ";
            }


            networkAdapter.dnsServers +=
                dnsAddress;
        }


        // ====================================================
        // DHCP
        // ====================================================

        if (adapter->Dhcpv4Enabled)
        {
            networkAdapter.dhcp =
                "Enabled";


            networkAdapter.dhcpServer =
                getIPAddress(
                    reinterpret_cast<const SOCKADDR*>(
                        &adapter->Dhcpv4Server
                    )
                );
        }
        else
        {
            networkAdapter.dhcp =
                "Disabled";
        }


        // ====================================================
        // Determine whether adapter is Wi-Fi
        // ====================================================

        networkAdapter.isWifi =
            isWiFiAdapter(
                networkAdapter
            );


        // ----------------------------------------------------
        // Add adapter
        // ----------------------------------------------------

        adapters.push_back(
            networkAdapter
        );
    }


    // ========================================================
    // Wi-Fi information
    // ========================================================

    DWORD negotiatedVersion = 0;

    HANDLE wlanHandle = nullptr;


    DWORD wlanResult =
        WlanOpenHandle(
            2,
            nullptr,
            &negotiatedVersion,
            &wlanHandle
        );


    if (
        wlanResult != ERROR_SUCCESS ||
        wlanHandle == nullptr
    )
    {
        return;
    }


    PWLAN_INTERFACE_INFO_LIST interfaceList =
        nullptr;


    wlanResult =
        WlanEnumInterfaces(
            wlanHandle,
            nullptr,
            &interfaceList
        );


    if (
        wlanResult == ERROR_SUCCESS &&
        interfaceList != nullptr
    )
    {
        // ----------------------------------------------------
        // Iterate through Wi-Fi interfaces
        // ----------------------------------------------------

        for (
            DWORD i = 0;

            i < interfaceList->dwNumberOfItems;

            ++i
        )
        {
            PWLAN_INTERFACE_INFO wifiInterface =
                &interfaceList->InterfaceInfo[i];


            // ------------------------------------------------
            // Convert GUID to string
            // ------------------------------------------------

            wchar_t guidString[64];


            int guidLength =
                StringFromGUID2(
                    wifiInterface->InterfaceGuid,
                    guidString,
                    64
                );


            if (guidLength <= 0)
            {
                continue;
            }


            std::string interfaceGuid =
                wideToString(
                    guidString
                );


            // ------------------------------------------------
            // Find corresponding adapter
            // ------------------------------------------------

            for (auto& adapter : adapters)
            {
                if (
                    _stricmp(
                        adapter.adapterName.c_str(),
                        interfaceGuid.c_str()
                    ) != 0
                )
                {
                    continue;
                }


                adapter.isWifi = true;


                // --------------------------------------------
                // Only query connected Wi-Fi
                // --------------------------------------------

                if (
                    wifiInterface->isState !=
                    wlan_interface_state_connected
                )
                {
                    continue;
                }


                PWLAN_CONNECTION_ATTRIBUTES connectionInfo =
                    nullptr;


                DWORD connectionInfoSize =
                    sizeof(
                        WLAN_CONNECTION_ATTRIBUTES
                    );


                WLAN_OPCODE_VALUE_TYPE opcodeValueType;


                DWORD queryResult =
                    WlanQueryInterface(
                        wlanHandle,
                        &wifiInterface->InterfaceGuid,
                        wlan_intf_opcode_current_connection,
                        nullptr,
                        &connectionInfoSize,
                        reinterpret_cast<PVOID*>(
                            &connectionInfo
                        ),
                        &opcodeValueType
                    );


                if (
                    queryResult != ERROR_SUCCESS ||
                    connectionInfo == nullptr
                )
                {
                    continue;
                }


                // ============================================
                // SSID
                // ============================================

                DOT11_SSID& ssid =
                    connectionInfo
                        ->wlanAssociationAttributes
                        .dot11Ssid;


                if (ssid.uSSIDLength > 0)
                {
                    adapter.wifiSSID =
                        std::string(
                            reinterpret_cast<char*>(
                                ssid.ucSSID
                            ),
                            ssid.uSSIDLength
                        );
                }


                // ============================================
                // BSSID
                // ============================================

                const UCHAR* bssid =
                    connectionInfo
                        ->wlanAssociationAttributes
                        .dot11Bssid;


                std::ostringstream bssidStream;


                for (int j = 0; j < 6; ++j)
                {
                    if (j > 0)
                    {
                        bssidStream << ":";
                    }


                    bssidStream
                        << std::uppercase
                        << std::hex
                        << std::setw(2)
                        << std::setfill('0')
                        << static_cast<int>(
                            bssid[j]
                        );
                }


                adapter.wifiBSSID =
                    bssidStream.str();


                // ============================================
                // Signal Strength
                // ============================================

                adapter.wifiSignalStrength =
                    connectionInfo
                        ->wlanAssociationAttributes
                        .wlanSignalQuality;


                WlanFreeMemory(
                    connectionInfo
                );
            }
        }
    }


    // --------------------------------------------------------
    // Free WLAN interface list
    // --------------------------------------------------------

    if (interfaceList != nullptr)
    {
        WlanFreeMemory(
            interfaceList
        );
    }


    // --------------------------------------------------------
    // Close WLAN handle
    // --------------------------------------------------------

    WlanCloseHandle(
        wlanHandle,
        nullptr
    );
}


// ============================================================
// Update Network Statistics
// ============================================================

void NetworkMonitor::updateNetworkStats()
{
    auto currentTime =
        std::chrono::steady_clock::now();


    double elapsedSeconds =
        std::chrono::duration<double>(
            currentTime -
            lastStatsUpdate
        ).count();


    // --------------------------------------------------------
    // Ignore extremely small intervals
    // --------------------------------------------------------

    if (
        elapsedSeconds < 0.25
    )
    {
        return;
    }


    // ========================================================
    // Process every adapter
    // ========================================================

    for (auto& adapter : adapters)
    {
        // ----------------------------------------------------
        // MIB_IF_ROW2 contains 64-bit network counters
        // ----------------------------------------------------

        MIB_IF_ROW2 row{};


        row.InterfaceIndex =
            adapter.interfaceIndex;


        // ----------------------------------------------------
        // Get current interface statistics
        // ----------------------------------------------------

        if (
            GetIfEntry2(&row) != NO_ERROR
        )
        {
            continue;
        }


        uint64_t currentReceived =
            row.InOctets;


        uint64_t currentSent =
            row.OutOctets;


        // ====================================================
        // First sample
        // ====================================================

        if (firstStatsSample)
        {
            adapter.bytesReceived =
                currentReceived;


            adapter.bytesSent =
                currentSent;


            adapter.downloadSpeed = 0;

            adapter.uploadSpeed = 0;


            continue;
        }


        // ====================================================
        // Calculate received bytes
        // ====================================================

        uint64_t receivedDifference = 0;


        if (
            currentReceived >=
            adapter.bytesReceived
        )
        {
            receivedDifference =
                currentReceived -
                adapter.bytesReceived;
        }


        // ====================================================
        // Calculate sent bytes
        // ====================================================

        uint64_t sentDifference = 0;


        if (
            currentSent >=
            adapter.bytesSent
        )
        {
            sentDifference =
                currentSent -
                adapter.bytesSent;
        }


        // ====================================================
        // Calculate bytes per second
        // ====================================================

        adapter.downloadSpeed =
            static_cast<uint64_t>(
                receivedDifference /
                elapsedSeconds
            );


        adapter.uploadSpeed =
            static_cast<uint64_t>(
                sentDifference /
                elapsedSeconds
            );


        // ====================================================
        // Save current counters
        // ====================================================

        adapter.bytesReceived =
            currentReceived;


        adapter.bytesSent =
            currentSent;
    }


    // --------------------------------------------------------
    // First sample completed
    // --------------------------------------------------------

    firstStatsSample = false;


    lastStatsUpdate =
        currentTime;
}


// ============================================================
// Interface Type
// ============================================================

std::string NetworkMonitor::getInterfaceTypeName(
    uint32_t type
)
{
    switch (type)
    {
        case IF_TYPE_ETHERNET_CSMACD:
            return "Ethernet";


        case IF_TYPE_IEEE80211:
            return "Wi-Fi";


        case IF_TYPE_SOFTWARE_LOOPBACK:
            return "Loopback";


        case IF_TYPE_TUNNEL:
            return "Tunnel / VPN";


        case IF_TYPE_PPP:
            return "PPP";


        case IF_TYPE_IEEE1394:
            return "FireWire";


        default:
            return "Other";
    }
}


// ============================================================
// Adapter Status
// ============================================================

std::string NetworkMonitor::getAdapterStatusName(
    uint32_t status
)
{
    switch (status)
    {
        case IfOperStatusUp:
            return "Connected";


        case IfOperStatusDown:
            return "Disconnected";


        case IfOperStatusTesting:
            return "Testing";


        case IfOperStatusDormant:
            return "Dormant";


        case IfOperStatusNotPresent:
            return "Not Present";


        case IfOperStatusLowerLayerDown:
            return "Lower Layer Down";


        default:
            return "Unknown";
    }
}


// ============================================================
// Convert SOCKADDR to IP address
// ============================================================

std::string NetworkMonitor::getIPAddress(
    const SOCKADDR* address
)
{
    if (address == nullptr)
    {
        return "";
    }


    char buffer[
        INET6_ADDRSTRLEN
    ];


    // --------------------------------------------------------
    // IPv4
    // --------------------------------------------------------

    if (
        address->sa_family ==
        AF_INET
    )
    {
        const SOCKADDR_IN* ipv4 =
            reinterpret_cast<
                const SOCKADDR_IN*
            >(address);


        if (
            inet_ntop(
                AF_INET,
                &ipv4->sin_addr,
                buffer,
                sizeof(buffer)
            ) == nullptr
        )
        {
            return "";
        }


        return buffer;
    }


    // --------------------------------------------------------
    // IPv6
    // --------------------------------------------------------

    if (
        address->sa_family ==
        AF_INET6
    )
    {
        const SOCKADDR_IN6* ipv6 =
            reinterpret_cast<
                const SOCKADDR_IN6*
            >(address);


        if (
            inet_ntop(
                AF_INET6,
                &ipv6->sin6_addr,
                buffer,
                sizeof(buffer)
            ) == nullptr
        )
        {
            return "";
        }


        return buffer;
    }


    return "";
}


// ============================================================
// Determine whether adapter is Wi-Fi
// ============================================================

bool NetworkMonitor::isWiFiAdapter(
    const NetworkAdapter& adapter
)
{
    return (
        adapter.interfaceType ==
        "Wi-Fi"
    );
}


// ============================================================
// Get all adapters
// ============================================================

const std::vector<NetworkAdapter>&
NetworkMonitor::getAdapters()
{
    updateNetworkStats();

    return adapters;
}


// ============================================================
// Get Total Download Speed
// ============================================================

uint64_t NetworkMonitor::getDownloadSpeed()
{
    updateNetworkStats();


    uint64_t total = 0;


    for (
        const auto& adapter :
        adapters
    )
    {
        total +=
            adapter.downloadSpeed;
    }


    return total;
}


// ============================================================
// Get Total Upload Speed
// ============================================================

uint64_t NetworkMonitor::getUploadSpeed()
{
    updateNetworkStats();


    uint64_t total = 0;


    for (
        const auto& adapter :
        adapters
    )
    {
        total +=
            adapter.uploadSpeed;
    }


    return total;
}