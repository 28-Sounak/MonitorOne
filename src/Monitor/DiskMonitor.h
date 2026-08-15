#pragma once

#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <pdh.h>
#endif


// ============================================================
// Disk Information
// ============================================================

struct DiskInfo
{
    // --------------------------------------------------------
    // Basic information
    // --------------------------------------------------------

    std::string driveLetter;
    std::string volumeName;
    std::string fileSystem;


    // --------------------------------------------------------
    // Storage information
    // --------------------------------------------------------

    uint64_t totalSpace = 0;
    uint64_t freeSpace = 0;
    uint64_t usedSpace = 0;

    float usagePercentage = 0.0f;


    // --------------------------------------------------------
    // Runtime disk activity
    // --------------------------------------------------------

    uint64_t readSpeed = 0;
    uint64_t writeSpeed = 0;

    uint64_t bytesRead = 0;
    uint64_t bytesWritten = 0;
};


// ============================================================
// Disk Monitor
// ============================================================

class DiskMonitor
{
private:

    // --------------------------------------------------------
    // Disk information
    // --------------------------------------------------------

    std::vector<DiskInfo> disks;


    // --------------------------------------------------------
    // Initialization
    // --------------------------------------------------------

    void initializeDisks();


    // --------------------------------------------------------
    // Runtime statistics
    // --------------------------------------------------------

    void updateDiskStats();


    // --------------------------------------------------------
    // Helper functions
    // --------------------------------------------------------

    static std::string wideToString(
        const wchar_t* value
    );


    // --------------------------------------------------------
    // PDH
    // --------------------------------------------------------

#ifdef _WIN32

    PDH_HQUERY pdhQuery = nullptr;

    struct DiskCounter
    {
        std::string driveLetter;

        PDH_HCOUNTER readCounter = nullptr;
        PDH_HCOUNTER writeCounter = nullptr;
    };

    std::vector<DiskCounter> diskCounters;

    bool pdhInitialized = false;

#endif


public:

    // --------------------------------------------------------
    // Constructor / Destructor
    // --------------------------------------------------------

    DiskMonitor();

    ~DiskMonitor();


    // --------------------------------------------------------
    // Update
    // --------------------------------------------------------

    void update();


    // --------------------------------------------------------
    // Disk information
    // --------------------------------------------------------

    const std::vector<DiskInfo>&
    getDisks();


    // --------------------------------------------------------
    // Total disk read/write speed
    // --------------------------------------------------------

    uint64_t getTotalReadSpeed();

    uint64_t getTotalWriteSpeed();
};