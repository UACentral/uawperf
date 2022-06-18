#include <Windows.h>
#include <Psapi.h>

#include "MemoryInfo.h"

double getMemoryUsage() {
    PERFORMANCE_INFORMATION pi;
    pi.cb = sizeof(pi);
    auto ret = GetPerformanceInfo(&pi, pi.cb);
    auto mem_total = pi.PhysicalTotal * pi.PageSize * 1.0 / (1024 * 1024 * 1024);
    auto mem_used = pi.PhysicalAvailable * pi.PageSize * 1.0 / (1024 * 1024 * 1024);
    auto pct = 100.0 - mem_used * 100.0 / mem_total;
    return pct;
}