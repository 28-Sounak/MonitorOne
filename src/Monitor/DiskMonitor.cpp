#include "DiskMonitor.h"

#include <windows.h>
#include <pdh.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>


#pragma comment(lib, "pdh.lib")


// ============================================================
// Convert Windows wide string to UTF-8 std::string
// ============================================================

std::string DiskMonitor::wideToString(
    const wchar_t* value
)
{
    if (value == nullptr)
    {
        return "";
    }


    int sizeNeeded =
        WideCharToMultiByte(
            CP_UTF8,
            0,
            value,
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


    std::string result(
        sizeNeeded,
        '\0'
    );


    WideCharToMultiByte(
        CP_UTF8,
        0,
        value,
        -1,
        result.data(),
        sizeNeeded,
        nullptr,
        nullptr
    );


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

DiskMonitor::DiskMonitor()
{
    initializeDisks();


#ifdef _WIN32

    // --------------------------------------------------------
    // Create PDH query
    // --------------------------------------------------------

    PDH_STATUS status =
        PdhOpenQueryW(
            nullptr,
            0,
            &pdhQuery
        );


    if (status != ERROR_SUCCESS)
    {
        std::cerr
            << "Failed to create PDH query.\n";

        return;
    }


    // --------------------------------------------------------
    // Add disk read/write counters
    // --------------------------------------------------------

    for (const auto& disk : disks)
    {
        if (disk.driveLetter.empty())
        {
            continue;
        }


        // ----------------------------------------------------
        // PDH disk instance
        //
        // Example:
        // \PhysicalDisk(0 C:)\Disk Read Bytes/sec
        // ----------------------------------------------------

        std::wstring driveLetter(
            disk.driveLetter.begin(),
            disk.driveLetter.end()
        );


        std::wstring instance =
            L"\\PhysicalDisk(" +
            driveLetter +
            L")";


        std::wstring readPath =
            instance +
            L"\\Disk Read Bytes/sec";


        std::wstring writePath =
            instance +
            L"\\Disk Write Bytes/sec";


        DiskCounter counter;

        counter.driveLetter =
            disk.driveLetter;


        status =
            PdhAddEnglishCounterW(
                pdhQuery,
                readPath.c_str(),
                0,
                &counter.readCounter
            );


        if (status != ERROR_SUCCESS)
        {
            counter.readCounter = nullptr;
        }


        status =
            PdhAddEnglishCounterW(
                pdhQuery,
                writePath.c_str(),
                0,
                &counter.writeCounter
            );


        if (status != ERROR_SUCCESS)
        {
            counter.writeCounter = nullptr;
        }


        diskCounters.push_back(
            counter
        );
    }


    // --------------------------------------------------------
    // First PDH collection
    // --------------------------------------------------------

    if (pdhQuery != nullptr)
    {
        PdhCollectQueryData(
            pdhQuery
        );

        pdhInitialized = true;
    }

#endif
}


// ============================================================
// Destructor
// ============================================================

DiskMonitor::~DiskMonitor()
{
#ifdef _WIN32

    if (pdhQuery != nullptr)
    {
        PdhCloseQuery(
            pdhQuery
        );

        pdhQuery = nullptr;
    }

#endif
}


// ============================================================
// Initialize disks
// ============================================================

void DiskMonitor::initializeDisks()
{
    disks.clear();


    // --------------------------------------------------------
    // Get all logical drives
    // --------------------------------------------------------

    DWORD driveMask =
        GetLogicalDrives();


    if (driveMask == 0)
    {
        std::cerr
            << "GetLogicalDrives failed.\n";

        return;
    }


    // --------------------------------------------------------
    // Iterate through drive letters
    // --------------------------------------------------------

    for (char letter = 'A';
         letter <= 'Z';
         ++letter)
    {
        int driveIndex =
            letter - 'A';


        if (!(driveMask & (1 << driveIndex)))
        {
            continue;
        }


        std::string root;

        root += letter;
        root += ":\\";


        UINT driveType =
            GetDriveTypeA(
                root.c_str()
            );


        // ----------------------------------------------------
        // Only include fixed disks
        // ----------------------------------------------------

        if (driveType != DRIVE_FIXED)
        {
            continue;
        }


        DiskInfo disk;


        disk.driveLetter =
            std::string(1, letter) + ":";


        // ----------------------------------------------------
        // Volume information
        // ----------------------------------------------------

        char volumeName[MAX_PATH] = {};

        char fileSystemName[MAX_PATH] = {};

        DWORD serialNumber = 0;

        DWORD maxComponentLength = 0;

        DWORD fileSystemFlags = 0;


        if (
            GetVolumeInformationA(
                root.c_str(),
                volumeName,
                sizeof(volumeName),
                &serialNumber,
                &maxComponentLength,
                &fileSystemFlags,
                fileSystemName,
                sizeof(fileSystemName)
            )
        )
        {
            disk.volumeName =
                volumeName;

            disk.fileSystem =
                fileSystemName;
        }


        // ----------------------------------------------------
        // Storage capacity
        // ----------------------------------------------------

        ULARGE_INTEGER freeBytesAvailable{};

        ULARGE_INTEGER totalNumberOfBytes{};

        ULARGE_INTEGER totalNumberOfFreeBytes{};


        if (
            GetDiskFreeSpaceExA(
                root.c_str(),
                &freeBytesAvailable,
                &totalNumberOfBytes,
                &totalNumberOfFreeBytes
            )
        )
        {
            disk.totalSpace =
                totalNumberOfBytes.QuadPart;


            disk.freeSpace =
                totalNumberOfFreeBytes.QuadPart;


            if (
                disk.totalSpace >=
                disk.freeSpace
            )
            {
                disk.usedSpace =
                    disk.totalSpace -
                    disk.freeSpace;
            }


            if (disk.totalSpace > 0)
            {
                disk.usagePercentage =
                    (
                        static_cast<float>(
                            disk.usedSpace
                        ) /
                        static_cast<float>(
                            disk.totalSpace
                        )
                    ) * 100.0f;
            }
        }


        disks.push_back(
            disk
        );
    }
}


// ============================================================
// Update
// ============================================================

void DiskMonitor::update()
{
    // --------------------------------------------------------
    // Storage information can change while the application
    // is running, so refresh it.
    // --------------------------------------------------------

    initializeDisks();


    // --------------------------------------------------------
    // Update disk I/O statistics.
    //
    // NOTE:
    // initializeDisks() recreates the disk list, so PDH
    // counters are associated with the disks discovered
    // during construction.
    // --------------------------------------------------------

    updateDiskStats();
}


// ============================================================
// Update disk statistics
// ============================================================

void DiskMonitor::updateDiskStats()
{
#ifdef _WIN32

    if (!pdhInitialized ||
        pdhQuery == nullptr)
    {
        return;
    }


    // --------------------------------------------------------
    // Collect current counter values
    // --------------------------------------------------------

    PDH_STATUS status =
        PdhCollectQueryData(
            pdhQuery
        );


    if (status != ERROR_SUCCESS)
    {
        return;
    }


    // --------------------------------------------------------
    // Read counter values
    // --------------------------------------------------------

    for (const auto& counter :
         diskCounters)
    {
        double readSpeed = 0.0;

        double writeSpeed = 0.0;


        // ----------------------------------------------------
        // Read speed
        // ----------------------------------------------------

        if (counter.readCounter != nullptr)
        {
            PDH_FMT_COUNTERVALUE value{};


            status =
                PdhGetFormattedCounterValue(
                    counter.readCounter,
                    PDH_FMT_DOUBLE,
                    nullptr,
                    &value
                );


            if (status == ERROR_SUCCESS)
            {
                readSpeed =
                    value.doubleValue;
            }
        }


        // ----------------------------------------------------
        // Write speed
        // ----------------------------------------------------

        if (counter.writeCounter != nullptr)
        {
            PDH_FMT_COUNTERVALUE value{};


            status =
                PdhGetFormattedCounterValue(
                    counter.writeCounter,
                    PDH_FMT_DOUBLE,
                    nullptr,
                    &value
                );


            if (status == ERROR_SUCCESS)
            {
                writeSpeed =
                    value.doubleValue;
            }
        }


        // ----------------------------------------------------
        // Find corresponding disk
        // ----------------------------------------------------

        for (auto& disk : disks)
        {
            if (
                disk.driveLetter !=
                counter.driveLetter
            )
            {
                continue;
            }


            disk.readSpeed =
                static_cast<uint64_t>(
                    readSpeed
                );


            disk.writeSpeed =
                static_cast<uint64_t>(
                    writeSpeed
                );


            break;
        }
    }

#endif
}


// ============================================================
// Get all disks
// ============================================================

const std::vector<DiskInfo>&
DiskMonitor::getDisks()
{
    return disks;
}


// ============================================================
// Get total read speed
// ============================================================

uint64_t DiskMonitor::getTotalReadSpeed()
{
    uint64_t total = 0;


    for (const auto& disk : disks)
    {
        total +=
            disk.readSpeed;
    }


    return total;
}


// ============================================================
// Get total write speed
// ============================================================

uint64_t DiskMonitor::getTotalWriteSpeed()
{
    uint64_t total = 0;


    for (const auto& disk : disks)
    {
        total +=
            disk.writeSpeed;
    }


    return total;
}