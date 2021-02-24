/*
 * Amazon FreeRTOS V1.3.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */
// source file:amazon-freertos-1.3.1/demos/pc/windows/common/config_files/aws_demo_config.h
#ifndef _AWS_MQTT_CONFIG_H_
#define _AWS_MQTT_CONFIG_H_

#include <stdio.h>

#define configMQTT_TASK_STACK_SIZE                  ( configMINIMAL_STACK_SIZE * 2 )
#define configMQTT_TASK_PRIORITY                    ( tskIDLE_PRIORITY + 1 )
#define configMQTT_AGENT_CONNECT_FLAGS              ( mqttagentREQUIRE_TLS | mqttagentUSE_AWS_IOT_ALPN_443 )
#define configMQTT_TLS_NEGOTIATION_TIMEOUT     		pdMS_TO_TICKS( 12000 )
#define configMQTT_TIMEOUT                          pdMS_TO_TICKS( 300 )

#define mqttconfigENABLE_DEBUG_LOGS 				0
#if ( mqttconfigENABLE_DEBUG_LOGS == 1 )
    #define mqttconfigDEBUG_LOG( x )    printf x
#else
	#define mqttconfigDEBUG_LOG( x )
#endif

/* Number of sub pub tasks that connect to a broker that is not using TLS. */
// #define democonfigMQTT_SUB_PUB_NUM_UNSECURE_TASKS            ( 0 )

/* Number of sub pub tasks that connect to a broker that is using TLS. */
// #define democonfigMQTT_SUB_PUB_NUM_SECURE_TASKS              ( 2 )

/* IoT simple subscribe/publish example task parameters. */
// #define democonfigMQTT_SUB_PUB_TASK_STACK_SIZE               ( configMINIMAL_STACK_SIZE * 5 )
// #define democonfigMQTT_SUB_PUB_TASK_PRIORITY                 ( tskIDLE_PRIORITY )

/* Greengrass discovery example task parameters. */
// #define democonfigGREENGRASS_DISCOVERY_TASK_STACK_SIZE       ( configMINIMAL_STACK_SIZE * 16 )
// #define democonfigGREENGRASS_DISCOVERY_TASK_PRIORITY         ( tskIDLE_PRIORITY )

/* Shadow demo task parameters. */
// #define democonfigSHADOW_DEMO_TASK_STACK_SIZE                ( configMINIMAL_STACK_SIZE * 4 )
// #define democonfigSHADOW_DEMO_TASK_PRIORITY                  ( tskIDLE_PRIORITY )

/* Number of shadow light switch tasks running. */
// #define democonfigSHADOW_DEMO_NUM_TASKS                      ( 4 )

/* TCP Echo Client tasks single example parameters. */
// #define democonfigTCP_ECHO_TASKS_SINGLE_TASK_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 4 )
// #define democonfigTCP_ECHO_TASKS_SINGLE_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* OTA Update task example parameters. */
// #define democonfigOTA_UPDATE_TASK_STACK_SIZE                 ( configMINIMAL_STACK_SIZE * 4 )
// #define democonfigOTA_UPDATE_TASK_TASK_PRIORITY              ( tskIDLE_PRIORITY )

/* Simple TCP Echo Server task example parameters */
// #define democonfigTCP_ECHO_SERVER_TASK_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 6 )
// #define democonfigTCP_ECHO_SERVER_TASK_PRIORITY              ( tskIDLE_PRIORITY )

/* TCP Echo Client tasks multi task example parameters. */
// #define democonfigTCP_ECHO_TASKS_SEPARATE_TASK_STACK_SIZE    ( configMINIMAL_STACK_SIZE * 4 )
// #define democonfigTCP_ECHO_TASKS_SEPARATE_TASK_PRIORITY      ( tskIDLE_PRIORITY )


#endif /* _AWS_DEMO_CONFIG_H_ */
