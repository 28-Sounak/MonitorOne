# <div align="center">MonitorOne</div>

<div align="center">

### A Lightweight, Native & Cross-Platform System Monitoring Engine

<img src="assets/icons/logo.png" alt="MonitorOne Logo" width="160"/>

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus)
![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C?style=for-the-badge&logo=cmake)
![Windows](https://img.shields.io/badge/Windows-Supported-0078D6?style=for-the-badge&logo=windows)
![Linux](https://img.shields.io/badge/Linux-Planned-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![macOS](https://img.shields.io/badge/macOS-Planned-000000?style=for-the-badge&logo=apple)

---

**MonitorOne** is a modern native desktop system monitor built from the ground up in **C++** with a strong emphasis on **performance**, **minimal resource consumption**, and **cross-platform compatibility**.

Unlike many monitoring applications that rely on heavy frameworks or expensive polling mechanisms, MonitorOne is designed to communicate directly with operating system APIs to retrieve accurate real-time system statistics while keeping CPU and memory usage extremely low.

</div>

---

# Overview

MonitorOne aims to provide a clean, responsive, and efficient monitoring experience by separating platform-specific implementations from the core monitoring engine.

The project is designed around a modular architecture where each hardware component has its own independent monitoring module. This makes the codebase easier to maintain, extend, and port to different operating systems.

---

# Features

## CPU

- Real-time CPU utilization
- Low-overhead polling
- Per-second updates
- Native operating system APIs

---

## Memory

- RAM usage
- Total physical memory
- Available memory
- Memory utilization percentage

---

## Disk

- Disk utilization
- Read speed
- Write speed
- Available storage
- Total storage

---

## GPU

- GPU utilization
- Dedicated VRAM usage
- Shared memory usage
- GPU information
- Vendor information
- Driver version
- Temperature
- Core clock
- Memory clock
- Fan speed

---

## Network

- Upload speed
- Download speed
- Network adapter information
- Real-time throughput

---

# Project Goals

- Native desktop application
- Extremely lightweight
- Minimal CPU overhead
- Minimal RAM usage
- Cross-platform architecture
- Modular backend
- High refresh rate
- Modern UI
- Clean codebase
- Easy to extend

---

# Project Structure

```text
MonitorOne/
│
├── assets/
│   ├── fonts/
│   ├── icons/
│   └── themes/
│
├── src/
│   ├── Core/
│   │   ├── SystemMonitor.cpp
│   │   ├── SystemStats.h
│   │   └── ThreadPool.cpp
│   │
│   ├── Monitor/
│   │   ├── CpuMonitor.cpp
│   │   ├── CpuMonitor.h
│   │   ├── MemoryMonitor.cpp
│   │   ├── MemoryMonitor.h
│   │   ├── DiskMonitor.cpp
│   │   ├── DiskMonitor.h
│   │   ├── GpuMonitor.cpp
│   │   ├── GpuMonitor.h
│   │   ├── NetworkMonitor.cpp
│   │   └── NetworkMonitor.h
│   │
│   ├── Platform/
│   │   ├── Windows/
│   │   ├── Linux/
│   │   └── MacOS/
│   │
│   └── UI/
│
├── main.cpp
├── CMakeLists.txt
└── README.md
```

---

# Architecture

```
                Dashboard / UI
                      │
                      ▼
               SystemMonitor
                      │
      ┌───────────────┼────────────────┐
      │               │                │
      ▼               ▼                ▼
 CpuMonitor      MemoryMonitor    DiskMonitor
      │
      ├───────────────┐
      ▼               ▼
 NetworkMonitor   GpuMonitor
                      │
              Platform Layer
      ┌──────────┼──────────┐
      ▼          ▼          ▼
   Windows     Linux      macOS
```

---

# Platform APIs

| Component | Windows | Linux | macOS |
|----------|----------|--------|--------|
| CPU | Win32 API | `/proc/stat` | Mach API |
| Memory | Win32 API | `/proc/meminfo` | Mach API |
| Disk | Win32 API | `statvfs` | POSIX |
| Network | IP Helper API | Netlink | SystemConfiguration |
| GPU | DXGI | DRM / Vendor APIs | Metal |

---

# Technology Stack

| Category | Technology |
|----------|------------|
| Language | C++20 |
| Build System | CMake |
| Graphics API | DXGI |
| Platform APIs | Native OS APIs |
| Threading | C++ Standard Library |
| UI | Planned |

---

# Design Philosophy

MonitorOne follows several important engineering principles.

- Modular architecture
- Platform abstraction
- Low latency
- Low memory footprint
- Native implementations
- Cross-platform compatibility
- High maintainability

---

# Build

## Clone

```bash
git clone https://github.com/28-Sounak/MonitorOne
```

```bash
cd MonitorOne
```

---

## Configure

```bash
cmake -S . -B build
```

---

## Build

```bash
cmake --build build
```

---

## Run

```bash
./build/MonitorOne
```

Windows

```bash
build\MonitorOne.exe
```

---

# Development Roadmap

## Backend

- [x] CPU Monitor
- [ ] Memory Monitor
- [ ] Disk Monitor
- [ ] GPU Monitor
- [ ] Network Monitor
- [ ] Thread Pool
- [ ] System Monitor

---

## Frontend

- [ ] Dashboard
- [ ] Graph Widgets
- [ ] Sidebar
- [ ] Theme System
- [ ] Settings

---

## Cross Platform

- [x] Windows
- [ ] Linux
- [ ] macOS

---

# Future Enhancements

- Historical performance graphs
- Process monitoring
- Per-core CPU graphs
- Per-process GPU usage
- Multiple GPU support
- Export performance logs
- Plugin system
- Custom dashboards
- Theme customization
- System notifications

---

# License

This project is currently under development.

A license will be added before the first stable release.

---

<div align="center">

**MonitorOne**

*A fast, lightweight and modern native system monitor built with C++.*

</div>