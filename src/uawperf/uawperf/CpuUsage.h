#include "pdh.h"
#pragma once
#pragma comment(lib, "pdh.lib")

class CpuUsage
{
public:
	CpuUsage();
	double getCurrentValue();
private:
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;
};