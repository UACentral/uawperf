#include "open62541.h"
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

static UA_StatusCode
readMemoryPercentUsage(UA_Server* server,
    const UA_NodeId* sessionId, void* sessionContext,
    const UA_NodeId* nodeId, void* nodeContext,
    UA_Boolean sourceTimeStamp, const UA_NumericRange* range,
    UA_DataValue* dataValue) {

    UA_Double memUsage = getMemoryUsage();

    UA_Variant_setScalarCopy(&dataValue->value, &memUsage,
        &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

void
addMemoryPercentUsageDataSourceVariable(UA_Server* server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    char nameLocalized[36] = "\\Memory(_Total)\\% Usage";
    char name[36] = "\\Memory(_Total)\\% Usage";
    attr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", nameLocalized);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, name);
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, name);
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource cpuDataSource;
    cpuDataSource.read = readMemoryPercentUsage;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
        parentReferenceNodeId, currentName,
        variableTypeNodeId, attr,
        cpuDataSource, NULL, NULL);
}
