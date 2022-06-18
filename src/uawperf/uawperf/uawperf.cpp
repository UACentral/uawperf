//#include <iostream>
#include <signal.h>
#include "open62541.h"
#include "CpuUsage.h"

CpuUsage cpu;

static UA_StatusCode
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

static void
addCpuUsageDataSourceVariable(UA_Server* server) {
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

    UA_DataSource cpuDataSource;
    cpuDataSource.read = readCurrentValue;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
        parentReferenceNodeId, currentName,
        variableTypeNodeId, attr,
        cpuDataSource, NULL, NULL);
}

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}



int main() {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server* server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    addCpuUsageDataSourceVariable(server);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}