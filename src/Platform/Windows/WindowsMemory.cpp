#include "WindowsMemory.h"

#include <windows.h>

#include <wbemidl.h>

#include <comdef.h>

#include <string>

#include <vector>

#include <cstdint>

#pragma comment(lib, "wbemuuid.lib")

//Helper Function

namespace
{
    //Convert BSTR to std :: string

    std :: string bstToString(BSTR value)
    {
        if(value == nullptr)
        {
            return "";
        }

        _bstr_t bstr(value);

        return static_cast <const char *> (bstr);
    }

    // Convert VARIANT containing a string to std::string

    std::string variantToString(const VARIANT& value)
    {
        if (value.vt == VT_BSTR && value.bstrVal != nullptr)
        {
            return bstrToString(value.bstrVal);
        }

        return "";
    }

    //Convert VARIANT containing an unsigned integer

    uint64_t variantToUInt64(const VARIANT& value)
    {
        switch (value.vt)
        {
            case VT_UI1:
                return value.bVal;

            case VT_UI2:
                return value.uiVal;

            case VT_UI4:
                return value.ulVal;

            case VT_UI8:
                return value.ullVal;

            case VT_I1:
                return value.cVal;

            case VT_I2:
                return value.iVal;

            case VT_I4:
                return value.lVal;

            case VT_I8:
                return static_cast<uint64_t>(value.llVal);

            default:
                return 0;
        }
    }

    //Convert WIndows memory type to readable string

    std::string memoryTypeToString(uint16_t type)
    {
        switch (type)
        {
            case 0:
                return "Unknown";

            case 1:
                return "Other";

            case 2:
                return "DRAM";

            case 3:
                return "Synchronous DRAM";

            case 4:
                return "Cache DRAM";

            case 5:
                return "EDO";

            case 6:
                return "EDRAM";

            case 7:
                return "VRAM";

            case 8:
                return "SRAM";

            case 9:
                return "RAM";

            case 10:
                return "ROM";

            case 11:
                return "Flash";

            case 12:
                return "EEPROM";

            case 13:
                return "FEPROM";

            case 14:
                return "EPROM";

            case 15:
                return "CDRAM";

            case 16:
                return "3DRAM";

            case 17:
                return "SDRAM";

            case 18:
                return "SGRAM";

            case 19:
                return "RDRAM";

            case 20:
                return "DDR";

            case 21:
                return "DDR2";

            case 22:
                return "DDR2 FB-DIMM";

            case 24:
                return "DDR3";

            case 26:
                return "DDR4";

            case 27:
                return "DDR5";

            default:
                return "Unknown";
        }
    }

    //Convert form factor code to readable string

    std::string formFactorToString(uint16_t formFactor)
    {
        switch (formFactor)
        {
            case 0:
                return "Unknown";

            case 1:
                return "Other";

            case 2:
                return "SIP";

            case 3:
                return "DIP";

            case 4:
                return "ZIP";

            case 5:
                return "SOJ";

            case 6:
                return "Proprietary";

            case 7:
                return "SIMM";

            case 8:
                return "DIMM";

            case 9:
                return "TSOP";

            case 10:
                return "PGA";

            case 11:
                return "RIMM";

            case 12:
                return "SODIMM";

            case 13:
                return "SRIMM";

            case 14:
                return "SMD";

            case 15:
                return "SSMP";

            case 16:
                return "QFP";

            case 17:
                return "TQFP";

            case 18:
                return "SOIC";

            case 19:
                return "LCC";

            case 20:
                return "PLCC";

            case 21:
                return "BGA";

            case 22:
                return "FPBGA";

            case 23:
                return "LGA";

            default:
                return "Unknown";
        }
    }

    //Convert bytes to GB

    double bytesToGB(uint64_t bytes)
    {
        return static_cast <double> (bytes) / (1024.0 * 1024.0 * 1024.0);
    }
}

//Constructor

WindowsMemory :: WindowsMemory()
{

}

//Total Physical Memory

uint64_t WindowsMemory::getTotalMemory()
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return 0;
    }

    return memoryStatus.ullTotalPhys;
}

//Available Physical Memory

uint64_t WindowsMemory::getAvailableMemory()
{
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&memoryStatus))
    {
        return 0;
    }

    return memoryStatus.ullAvailPhys;
}

//Used Phyical Memory

uint64_t WindowsMemory::getUsedMemory()
{
    uint64_t total = getTotalMemory();
    uint64_t available = getAvailableMemory();

    if (total < available)
    {
        return 0;
    }

    return total - available;
}


//Memory Usage Percentage

float WindowsMemory::getUsage()
{
    uint64_t total = getTotalMemory();
    uint64_t available = getAvailableMemory();

    if (total == 0)
    {
        return 0.0f;
    }

    uint64_t used = total - available;

    return static_cast<float>(
        (static_cast<double>(used) /
         static_cast<double>(total)) * 100.0
    );
}

//Get Physical RAM modules through WMI

std :: vector <MemodyModule> WindowsMemory :: getModules()
{
    std :: vector <MempryModule> modules;

    //Initialize COM

    HRESULT hres = CoInitializeEx(
        nullptr,
        COINIT_MULTITHREADED
    );

    bool comInitialized = SUCCEEDED(hres);

    if (FAILED(hres) &&
        hres != RPC_E_CHANGED_MODE)
    {
        return modules;
    }

    //Initialize COM security

    hres = CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr
    );

    if (FAILED(hres) && hres != RPC_E_TOO_LATE)
    {
        if (comInitialized)
        {
            CoUninitialize();
        }

        return modules;
    }

    //Create WMI Locator

    IWbemLocator* locator = nullptr;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        reinterpret_cast<void**>(&locator)
    );

    if (FAILED(hres))
    {
        if (comInitialized)
        {
            CoUninitialize();
        }

        return modules;
    }

    //Connect to WMI Namespace

    IWbemServices* services = nullptr;

    hres = locator->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr,
        &services
    );

    if (FAILED(hres))
    {
        locator->Release();

        if (comInitialized)
        {
            CoUninitialize();
        }

        return modules;
    }

    //Set WMI Proxy Blanket

    hres = CoSetProxyBlanket(
        services,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE
    );

    if (FAILED(hres))
    {
        services->Release();
        locator->Release();

        if (comInitialized)
        {
            CoUninitialize();
        }

        return modules;
    }

    //Query Physical Memory Modules

    IEnumWbemClassObject* enumerator = nullptr;

    hres = services->ExecQuery(
        _bstr_t(L"WQL"),
        _bstr_t(
            L"SELECT "
            L"Capacity,"
            L"Manufacturer,"
            L"MemoryType,"
            L"FormFactor,"
            L"Speed,"
            L"ConfiguredClockSpeed,"
            L"PartNumber,"
            L"SerialNumber "
            L"FROM Win32_PhysicalMemory"
        ),
        WBEM_FLAG_FORWARD_ONLY |
        WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &enumerator
    );

    if (FAILED(hres))
    {
        services->Release();
        locator->Release();

        if (comInitialized)
        {
            CoUninitialize();
        }

        return modules;
    }

    //Read each RAM module

    IWbemClassObject* object = nullptr;

    ULONG returned = 0;

    while (enumerator)
    {
        hres = enumerator->Next(
            WBEM_INFINITE,
            1,
            &object,
            &returned
        );

        if (FAILED(hres) || returned == 0)
        {
            break;
        }

        MemoryModule module{};

        //Capacity

        VARIANT value;

        VariantInit(&value);

        hres = object->Get(
            L"Capacity",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            module.size =
                variantToUInt64(value);
        }

        VariantClear(&value);

        //Manufacturer

        VariantInit(&value);

        hres = object->Get(
            L"Manufacturer",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            module.manufacturer =
                variantToString(value);
        }

        VariantClear(&value);

        //Memoy Type

        VariantInit(&value);

        hres = object->Get(
            L"MemoryType",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            uint16_t memoryType =
                static_cast<uint16_t>(
                    variantToUInt64(value)
                );

            module.type =
                memoryTypeToString(memoryType);
        }

        VariantClear(&value);

        //Form Factor

        VariantInit(&value);

        hres = object->Get(
            L"FormFactor",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            uint16_t formFactor =
                static_cast<uint16_t>(
                    variantToUInt64(value)
                );

            module.formFactor =
                formFactorToString(formFactor);
        }

        VariantClear(&value);

        //Speed

        VariantInit(&value);

        hres = object->Get(
            L"Speed",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            module.speed =
                variantToUInt64(value);
        }

        VariantClear(&value);

        //Configured Clock Speed

        VariantInit(&value);

        hres = object->Get(
            L"ConfiguredClockSpeed",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            uint64_t configuredSpeed =
                variantToUInt64(value);

            if (configuredSpeed != 0)
            {
                module.configuredClockSpeed =
                    std::to_string(configuredSpeed);
            }
        }

        VariantClear(&value);

        //Part Number

        VariantInit(&value);

        hres = object->Get(
            L"PartNumber",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            module.partNumber =
                variantToString(value);
        }

        VariantClear(&value);

        //Serial Number

        VariantInit(&value);

        hres = object->Get(
            L"SerialNumber",
            0,
            &value,
            nullptr,
            nullptr
        );

        if (SUCCEEDED(hres))
        {
            module.serialNumber =
                variantToString(value);
        }

        VariantClear(&value);

        //Add Module

        modules.push_back(module);

        object->Release();
        object = nullptr;
    }

    //Cleanup WPI

    if(enumerator)
    {
        enumerator->Release();
    }

    services->Release();

    locator->Release();

    //Cleanup COM

    if(comInitializes)
    {
        CoUninitialize();
    }

    return modules;
}