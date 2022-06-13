#include <iostream>
#include "CpuUsage.h"

CpuUsage::CpuUsage()
{
    PDH_STATUS ret = PdhOpenQuery(NULL, NULL, &cpuQuery);
    // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
    PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}

double CpuUsage::getCurrentValue()
{
    PDH_FMT_COUNTERVALUE counterVal;

    PDH_STATUS ret = PdhCollectQueryData(cpuQuery);
    if (ret != ERROR_SUCCESS) {
        std::cout << "Query error detected " << ret << "\n";
    }
    ret = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    if (ret != ERROR_SUCCESS) {
        std::cout << "Error detected " << ret << "\n";

        HANDLE hPdhLibrary = NULL;
        hPdhLibrary = LoadLibrary(L"pdh.dll");
        if (NULL == hPdhLibrary)
        {
            wprintf(L"LoadLibrary failed with %lu\n", GetLastError());
            return -2.0;
        }

        LPWSTR pMessage = NULL;
        if (!FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            hPdhLibrary,
            ret,
            0,
            (LPWSTR)&pMessage,
            0,
            NULL))
        {
            wprintf(L"Format message failed with 0x%x\n", GetLastError());
            return -1.0;
        }

        wprintf(L"Formatted message: %ls\n", pMessage);
        LocalFree(pMessage);
    }
    return counterVal.doubleValue;

}