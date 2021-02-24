#ifndef __PORT_H__
#define __PORT_H__

#include "cmsis_os2.h"
#include "debug_log.h"
void MbedOS_FE_svcRtxThreadNew(void *task_id, osThreadFunc_t func, void *argument);
void StartScheduler(void);
void port_svc_call_enter(void);
void port_svc_call_exit(uint32_t* ret);
uint32_t** PortGetRegs(void* param);
#endif