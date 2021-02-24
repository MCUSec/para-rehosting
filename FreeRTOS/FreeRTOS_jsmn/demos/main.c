#include "FreeRTOS.h"
#include "task.h"
#include "jsmn.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#define LEN 1024
int jsmn(){
    jsmn_parser parser;
    jsmntok_t tok[LEN];
    
#ifdef __AFL_HAVE_MANUAL_CONTROL
    while (__AFL_LOOP(1000)){
#endif    

    unsigned int len = 0;
    read(0, &len, sizeof(len));
    if(len > LEN)
        len = LEN;
    char* js = pvPortMalloc(len);
    memset(js, 0x0, 0);
    len = read(0, js, len);
    jsmn_init(&parser);
    jsmn_parse(&parser, js, len, tok, LEN);
    vPortFree(js);

#ifdef __AFL_HAVE_MANUAL_CONTROL
    }
#endif  

    return 0;

}

void vApplicationIdleHook( void )
{
    const uint32_t ulMSToSleep = 1;
	usleep( ulMSToSleep );
}

void vApplicationMallocFailedHook( void )
{
    assert(0==1);
}

int main(){
    const uint32_t ulLongTime_ms = pdMS_TO_TICKS( 1000UL );
    TaskHandle_t xHandle = NULL;
	BaseType_t xReturned;
	xReturned = xTaskCreate(
        jsmn,
        "jsmn",
        configMINIMAL_STACK_SIZE,
        (void*)NULL,
        1,
        &xHandle
    );
    vTaskStartScheduler();
}