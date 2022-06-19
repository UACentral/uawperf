#pragma once


#include "open62541.h"
#include "pdh.h"

#pragma comment(lib, "pdh.lib")

class CpuUsage
{
public:
	CpuUsage();
	double getCurrentValue();
	//void addCpuUsageDataSourceVariable(UA_Server* server);
	//UA_StatusCode
	//	readCurrentValue(UA_Server* server,
	//		const UA_NodeId* sessionId, void* sessionContext,
	//		const UA_NodeId* nodeId, void* nodeContext,
	//		UA_Boolean sourceTimeStamp, const UA_NumericRange* range,
	//		UA_DataValue* dataValue);
private:
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;
};

void
addCpuUsageDataSourceVariable(UA_Server* server);
