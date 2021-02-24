#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <unistd.h>
#include <stdint.h>

#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		int
#define portSHORT		short
#define portSTACK_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE size_t

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
    typedef uint16_t TickType_t;
	#define portMAX_DELAY ( portTickType ) 0xffff
#else
    typedef uint32_t TickType_t;
	#define portMAX_DELAY ( portTickType ) 0xffffffff
#endif

portBASE_TYPE xPortStartScheduler( void );
void vPortEndScheduler();
void vPortDisableInterrupts( void );
void vPortEnableInterrupts( void );
void vPortEnterCritical( void );
void vPortExitCritical( void );
void vPortAddTaskHandle( void *pxTaskHandle );
void PMCU_FE_Yield();

#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS		( ( portTickType ) 1000000 / configTICK_RATE_HZ )
#define portINLINE __inline

#if defined( __x86_64__) || defined( _M_X64 )
	#define portBYTE_ALIGNMENT		8
#else
	#define portBYTE_ALIGNMENT		4
#endif

#define portREMOVE_STATIC_QUALIFIER

#define portYIELD()					PMCU_FE_Yield()

#define portSET_INTERRUPT_MASK()	( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()	( vPortEnableInterrupts() )

#define portDISABLE_INTERRUPTS()	( vPortDisableInterrupts() )
#define portENABLE_INTERRUPTS()		( vPortEnableInterrupts() )
#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()

#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
#error We are not supporting configUSE_PORT_OPTIMISED_TASK_SELECTION in the Linux Simulator.
#endif

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#define portNOP()

#define portOUTPUT_BYTE( a, b )

#define traceTASK_CREATE( pxNewTCB )			vPortAddTaskHandle( pxNewTCB )

#endif
