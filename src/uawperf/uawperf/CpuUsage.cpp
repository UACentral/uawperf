#include <functional>
#include <iostream>
#include "open62541.h"
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

CpuUsage cpu;

static
UA_StatusCode
//CpuUsage::
readCurrentValue(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionContext,
    const UA_NodeId* nodeId, void* nodeContext,
    UA_Boolean sourceTimeStamp, const UA_NumericRange* range,
    UA_DataValue* dataValue) {
    UA_Double cpuCurrent = cpu.getCurrentValue();
    UA_Variant_setScalarCopy(&dataValue->value, &cpuCurrent,
        &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

void
//CpuUsage::
addCpuUsageDataSourceVariable(UA_Server* server) 
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    char nameLocalized[36] = "\\Processor(_Total)\\% Processor Time";
    char name[36] = "\\Processor(_Total)\\% Processor Time";
    attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", nameLocalized);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, name);
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, name);
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    //auto cb = std::bind(&CpuUsage::readCurrentValue, CpuUsage());
    UA_DataSource cpuDataSource;
    cpuDataSource.read = &readCurrentValue;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
        parentReferenceNodeId, currentName,
        variableTypeNodeId, attr,
        cpuDataSource, NULL, NULL);
}
