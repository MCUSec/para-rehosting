#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "RTOS_BE.h"

void TickSignalHandler(int sig){
    (void)sig;
    void* current = NULL, *next = NULL;
    int cur_idx = -1, next_idx = -1;
    if(PMCU_IsInterruptEnabled() == 1 && PMCU_IsServicingTick() != 1){
        if(PMCU_SingleTryLock() == 0){
            PMCU_EnterServicingTick();
            xTaskIncrementTick();
            current = xTaskGetCurrentTaskHandle();
            cur_idx = PMCU_Find_Idx(current);
            vTaskSwitchContext();
            next = xTaskGetCurrentTaskHandle();
            next_idx = PMCU_Find_Idx(next);
            if(cur_idx != next_idx){
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
portSTACK_TYPE *pxPortInitialiseStack(portSTACK_TYPE *pxTopOfStack, void(* pxCode)(void*), void *pvParameters ){
    PMCU_CreateNewThread(pxTopOfStack, pxCode, pvParameters, TickSignalHandler);
    return pxTopOfStack;
}
portBASE_TYPE xPortStartScheduler( void ){
    void *first_task = xTaskGetCurrentTaskHandle();
    PMCU_StartScheduler(first_task, portTICK_RATE_MICROSECONDS);
    // should never reach here
    return 1;
}
void vPortEndScheduler( void ){
    PMCU_EndScheduler();
}
void PMCU_FE_Yield(){
    void *cur = NULL, *next = NULL; 
    int cur_idx = -1, next_idx = -1;
    PMCU_SingleLock();
    cur = xTaskGetCurrentTaskHandle();
    cur_idx = PMCU_Find_Idx(cur);
    vTaskSwitchContext();
    next = xTaskGetCurrentTaskHandle();
    next_idx = PMCU_Find_Idx(next);
    if(cur_idx != next_idx){
        PMCU_Schedule(cur_idx, next_idx);
    }else{
        PMCU_SingleUnlock();
    }
}
void vPortDisableInterrupts( void ){
    PMCU_DisableInterrupts();
}
void vPortEnableInterrupts( void ){
    PMCU_EnableInterrupts();
}
void vPortEnterCritical( void ){
    PMCU_EnterCritical();
}
void vPortExitCritical( void ){
    PMCU_ExitCritical();
}
void vPortAddTaskHandle( void *pxTaskHandle ){
    PMCU_Add_Task_ID(pxTaskHandle);
}
void FE_Thread_Exit(){}
