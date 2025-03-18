#include "cdm_ipc.h"

#include "cdm_host.h"



//Create Instance;


uintptr_t CreateCdmIpcInstance(const std::string& key_system, const std::string& host_name, uintptr_t host_this)
{
    return CdmHost::CreateCdmIpcInstance(key_system, host_name, host_this, true);
}
