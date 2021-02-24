#include "RTOS_BE.h"
#include "cmsis_os2.h"
#include "rtx_lib.h"
#include "assert.h"

#define MAX_NUMBER_OF_TASKS     16
#define ARM_REGS_NUM            4

typedef struct{
    int valid ;
    void *thread ;
    uint32_t* regs[ARM_REGS_NUM];
}Thread_Info;

static Thread_Info Mbed_Threads[MAX_NUMBER_OF_TASKS];
static int last_index = 0;
void Port_Init_Regs(void *task_id){
    Mbed_Threads[last_index].thread = task_id;
    last_index++;
}
void PortSetRegs(int index, uint32_t* param){
	int i;
	for(i=0;i<MAX_NUMBER_OF_TASKS;i++){
        if(Mbed_Threads[i].thread == osRtxInfo.thread.run.curr){
            Mbed_Threads[i].regs[index] = param;
            break;
        }
	}
}
uint32_t** PortGetRegs(void* param){
	int i;
	os_thread_t* thread = (os_thread_t*)param;
	for(i=0;i<MAX_NUMBER_OF_TASKS;i++){
		if(Mbed_Threads[i].thread == thread){
			return Mbed_Threads[i].regs;
		}
	}
	assert(1==0);
}
void FE_Thread_Exit(){
	int i;
	for(i=0;i<MAX_NUMBER_OF_TASKS;i++){
        if(Mbed_Threads[i].thread == osRtxInfo.thread.run.curr){
            Mbed_Threads[i].thread = NULL;
            break;
        }
	}    
    osThreadExit();
}
void MbedOS_Context_Switch(int *cur_idx, int *next_idx){
    void *current = NULL, *next = NULL;
    os_thread_t *thread;
    osRtxInfo.kernel.tick++;
    if(osRtxInfo.timer.tick!=NULL){
        osRtxInfo.timer.tick();
    }
    osRtxThreadDelayTick();

    current = osRtxThreadGetRunning();
    assert(current != NULL);
    *cur_idx = PMCU_Find_Idx(current);

    osRtxThreadDispatch(NULL);
    if (osRtxInfo.thread.robin.timeout != 0U) {
        if (osRtxInfo.thread.robin.thread != osRtxInfo.thread.run.next) {
            // Reset Round Robin
            osRtxInfo.thread.robin.thread = osRtxInfo.thread.run.next;
            osRtxInfo.thread.robin.tick   = osRtxInfo.thread.robin.timeout;
        } else {
            if (osRtxInfo.thread.robin.tick != 0U) {
                osRtxInfo.thread.robin.tick--;
            }
            if (osRtxInfo.thread.robin.tick == 0U) {
                // Round Robin Timeout
                if (osRtxKernelGetState() == osRtxKernelRunning) {
                    thread = osRtxInfo.thread.ready.thread_list;
                    if ((thread != NULL) && (thread->priority == osRtxInfo.thread.robin.thread->priority)) {
                        osRtxThreadListRemove(thread);
                        osRtxThreadReadyPut(osRtxInfo.thread.robin.thread);
                        EvrRtxThreadPreempted(osRtxInfo.thread.robin.thread);
                        osRtxThreadSwitch(thread);;
                        osRtxInfo.thread.robin.thread = thread;
                        osRtxInfo.thread.robin.tick = osRtxInfo.thread.robin.timeout;
                    }
                }
            }
        }
    }
    next = osRtxInfo.thread.run.next;
    *next_idx = PMCU_Find_Idx(next);
}

void TickSignalHandler(int sig){
    (void)sig;
    int cur_idx = -1, next_idx = -1;
    if(PMCU_IsInterruptEnabled() == 1 && PMCU_IsServicingTick() != 1 && PMCU_GetSVC() == 0){
        if(PMCU_SingleTryLock() == 0){
            PMCU_EnterServicingTick();
            MbedOS_Context_Switch(&cur_idx, &next_idx);
            if(cur_idx != next_idx){
                osRtxThreadSetRunning(osRtxInfo.thread.run.next);
                PMCU_Schedule(cur_idx, next_idx);
            }else{
                PMCU_SingleUnlock();
            }
            PMCU_ExitServicingTick();
        }else{
            PMCU_SetPending();
        }
    }else{
        PMCU_SetPending();
    }
}

void MbedOS_FE_svcRtxThreadNew(void *task_id, osThreadFunc_t func, void *argument){
    PMCU_CreateNewThread(task_id, func, argument, TickSignalHandler);
    PMCU_Add_Task_ID(task_id);
    Port_Init_Regs(task_id);
}

void StartScheduler(void){
    void *FirstThread;
    FirstThread = osRtxInfo.thread.run.next;
	osRtxInfo.thread.run.curr = osRtxInfo.thread.run.next;
    PMCU_StartScheduler(FirstThread, 1000000/osRtxConfig.tick_freq);   
}

void PMCU_FE_Yield(){
    int cur_idx = -1, next_idx = -1;
    int i = 0;
    MbedOS_Context_Switch(&cur_idx, &next_idx);
    if(cur_idx != next_idx){
        osRtxInfo.thread.run.curr = osRtxInfo.thread.run.next;
        PMCU_Schedule(cur_idx, next_idx);
    }
}

void port_svc_call_enter(void){
	PMCU_SetSVC();
}

void port_svc_call_exit(uint32_t* ret){
    int i = 0;
    int cur_idx = -1, next_idx = -1;
    if(PMCU_GetPending() == 1){
        if(PMCU_IsInterruptEnabled() == 1){
            MbedOS_Context_Switch(&cur_idx, &next_idx);
            PMCU_ClearPending();
        }
    }
    if(osRtxInfo.thread.run.curr!=osRtxInfo.thread.run.next){
        if(ret != NULL){
            for(i=0;i<MAX_NUMBER_OF_TASKS;i++){
                if(Mbed_Threads[i].thread == osRtxInfo.thread.run.curr){
                    Mbed_Threads[i].regs[0] = ret;
                    break;
                }
            }
        }
        cur_idx = PMCU_Find_Idx(osRtxInfo.thread.run.curr);
        next_idx = PMCU_Find_Idx(osRtxInfo.thread.run.next);
        osRtxInfo.thread.run.curr = osRtxInfo.thread.run.next;
        PMCU_Schedule(cur_idx, next_idx);
    }else{
        PMCU_ClearSVC();
    }
}