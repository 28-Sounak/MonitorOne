#pragma once

#include<cstdint>

struct SystemStats
{
    float CpuUsage = 0.0f;
    float MemoryUsage = 0.0f;
    float DiskUsage = 0.0f;
    float GpuUsage = 0.0f;

    double UploadSpeed = 0.0;
    double DownloadSpeed = 0.0;

    uint64_t RamUsed = 0;
    uint64_t RamTotal = 0;

    uint64_t GpuMemoryUsed = 0;
    uint64_t GpuMemoryTotal = 0;
};
