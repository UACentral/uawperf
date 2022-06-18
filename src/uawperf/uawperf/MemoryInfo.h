#pragma once
#include "open62541.h"

#pragma comment(lib, "kernel32.lib")

double getMemoryUsage();
void addMemoryPercentUsageDataSourceVariable(UA_Server* server);