#ifndef _RTOS_BE_H_
#define _RTOS_BE_H_

int PMCU_SingleLock();
int PMCU_SingleUnlock();
int PMCU_SingleTryLock();

void PMCU_SetPending();
void PMCU_ClearPending();
int PMCU_GetPending();

void PMCU_SetSVC();
void PMCU_ClearSVC();
int PMCU_GetSVC();

void PMCU_DisableInterrupts();
void PMCU_EnableInterrupts();
int PMCU_IsInterruptEnabled();

void PMCU_EnterCritical();
int PMCU_ExitCritical();

void PMCU_EnterServicingTick();
void PMCU_ExitServicingTick();
int PMCU_IsServicingTick();

void *PMCU_CreateNewThread(void* top_of_stack, void(*func)(void*), void* param, void(*systick_func)(int));
int PMCU_Find_Idx(void *task_id);
void PMCU_Schedule(int cur_idx, int next_idx);

void PMCU_StartScheduler(void *first_task_id, int tick_rate);
void PMCU_EndScheduler();

void PMCU_Add_Task_ID(void *task_id);
#endif
