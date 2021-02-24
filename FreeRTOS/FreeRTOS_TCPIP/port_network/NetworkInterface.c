/*
FreeRTOS+TCP V2.0.11
Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/
// added header file

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <sys/sysinfo.h>
#include <sys/prctl.h>

/* LibPcap includes. */
#include <pcap/pcap.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"

/* Thread-safe circular buffers are being used to pass data to and from the PCAP
access functions. */
// #include "Win32-Extensions.h"
#include "FreeRTOS_Stream_Buffer.h"

/* Sizes of the thread safe circular buffers used to pass data to and from the
WinPCAP Windows threads. */
#define xSEND_BUFFER_SIZE  32768
#define xRECV_BUFFER_SIZE  32768

/* If ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES is set to 1, then the Ethernet
driver will filter incoming packets and only pass the stack those packets it
considers need processing. */
#if( ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0 )
	#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eProcessBuffer
#else
	#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eConsiderFrameForProcessing( ( pucEthernetBuffer ) )
#endif

/* Used to insert test code only. */
#define niDISRUPT_PACKETS	0

// added define

#ifndef FALSE
	#define FALSE   (0)
#endif

#ifndef TRUE
	#define TRUE    (!FALSE)
#endif

/*-----------------------------------------------------------*/

/*-------------------------fuzz------------------------------*/
#define USE_PCAP FALSE
#define USE_FILE (!USE_PCAP)
/*-------------------------fuzz------------------------------*/

/*
 * Windows threads that are outside of the control of the FreeRTOS simulator are
 * used to interface with the WinPCAP libraries.
 */
// DWORD WINAPI prvWinPcapRecvThread( void *pvParam );
// DWORD WINAPI prvWinPcapSendThread( void *pvParam );
void *prvLibPcapRecvThread( void *pvParam );
void *prvLibPcapSendThread( void *pvParam );

/*
 * Print out a numbered list of network interfaces that are available on the
 * host computer.
 */
static pcap_if_t * prvPrintAvailableNetworkInterfaces( void );

/*
 * Open the network interface.  The number of the interface to be opened is set
 * by the configNETWORK_INTERFACE_TO_USE constant in FreeRTOSConfig.h.
 */
static void prvOpenSelectedNetworkInterface( pcap_if_t *pxAllNetworkInterfaces );
static int prvOpenInterface( const char *pucName );

/*
 * Configure the capture filter to allow blocking reads, and to filter out
 * packets that are not of interest to this demo.
 */
static void prvConfigureCaptureBehaviour( void );

/*
 * A function that simulates Ethernet interrupts by periodically polling the
 * WinPCap interface for new data.
 */
static void prvInterruptSimulatorTask( void *pvParameters );

/*
 * Create the buffers that are used to pass data between the FreeRTOS simulator
 * and the Win32 threads that manage WinPCAP.
 */
static void prvCreateThreadSafeBuffers( void );

/*
 * Utility function used to format print messages only.
 */
static const char *prvRemoveSpaces( char *pcBuffer, int aBuflen, const char *pcMessage );

/*-----------------------------------------------------------*/

/* Required by the WinPCap library. */
static char cErrorBuffer[ PCAP_ERRBUF_SIZE ];

/* An event used to wake up the Win32 thread that sends data through the WinPCAP
library. */
// static void *pvSendEvent = NULL;
static pthread_cond_t xSendEvent;
static int SendEventInited = 0;

/* _HT_ made the PCAP interface number configurable through the program's
parameters in order to test in different machines. */
static BaseType_t xConfigNextworkInterfaceToUse = configNETWORK_INTERFACE_TO_USE;

/* Handles to the Windows threads that handle the PCAP IO. */
/*static HANDLE vWinPcapRecvThreadHandle = NULL;
static HANDLE vWinPcapSendThreadHandle = NULL;*/
static pthread_t hLibPcapRecvThreadHandle;
static pthread_t hLibPcapSendThreadHandle;

/* The interface being used by WinPCap. */
static pcap_t *pxOpenedInterfaceHandle = NULL;

/* Circular buffers used by the PCAP Win32 threads. */
static StreamBuffer_t *xSendBuffer = NULL;
static StreamBuffer_t *xRecvBuffer = NULL;

/* The MAC address initially set to the constants defined in FreeRTOSConfig.h. */
extern uint8_t ucMACAddress[ 6 ];

/* Logs the number of WinPCAP send failures, for viewing in the debugger only. */
static volatile uint32_t ulWinPCAPSendFailures = 0;

// An mutex for xSendEvent
static pthread_mutex_t xSendEventMutex = PTHREAD_MUTEX_INITIALIZER;

/*-----------------------------------------------------------*/

BaseType_t xNetworkInterfaceInitialise( void )
{
BaseType_t xReturn = pdFALSE;
pcap_if_t *pxAllNetworkInterfaces;

	/* Query the computer the simulation is being executed on to find the
	network interfaces it has installed. */
	pxAllNetworkInterfaces = prvPrintAvailableNetworkInterfaces();

	/* Open the network interface.  The number of the interface to be opened is
	set by the configNETWORK_INTERFACE_TO_USE constant in FreeRTOSConfig.h.
	Calling this function will set the pxOpenedInterfaceHandle variable.  If,
	after calling this function, pxOpenedInterfaceHandle is equal to NULL, then
	the interface could not be opened. */
#if USE_PCAP
	if( pxAllNetworkInterfaces != NULL )
	{
		prvOpenSelectedNetworkInterface( pxAllNetworkInterfaces );
	}

	if( pxOpenedInterfaceHandle != NULL )
	{
		xReturn = pdPASS;
	}
#elif USE_FILE
	prvOpenSelectedNetworkInterface( NULL );
	xReturn = pdPASS;
#endif

	return xReturn;
}
/*-----------------------------------------------------------*/

static void prvCreateThreadSafeBuffers( void )
{
	/* The buffer used to pass data to be transmitted from a FreeRTOS task to
	the Win32 thread that sends via the WinPCAP library. */
	if( xSendBuffer == NULL)
	{
		xSendBuffer = ( StreamBuffer_t * ) malloc( sizeof( *xSendBuffer ) - sizeof( xSendBuffer->ucArray ) + xSEND_BUFFER_SIZE + 1 );
		configASSERT( xSendBuffer );
		memset( xSendBuffer, '\0', sizeof( *xSendBuffer ) - sizeof( xSendBuffer->ucArray ) );
		xSendBuffer->LENGTH = xSEND_BUFFER_SIZE + 1;
	}

	/* The buffer used to pass received data from the Win32 thread that receives
	via the WinPCAP library to the FreeRTOS task. */
	if( xRecvBuffer == NULL)
	{
		xRecvBuffer = ( StreamBuffer_t * ) malloc( sizeof( *xRecvBuffer ) - sizeof( xRecvBuffer->ucArray ) + xRECV_BUFFER_SIZE + 1 );
		configASSERT( xRecvBuffer );
		memset( xRecvBuffer, '\0', sizeof( *xRecvBuffer ) - sizeof( xRecvBuffer->ucArray ) );
		xRecvBuffer->LENGTH = xRECV_BUFFER_SIZE + 1;
	}
}
/*-----------------------------------------------------------*/

BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxNetworkBuffer, BaseType_t bReleaseAfterSend )
{
size_t xSpace;

	iptraceNETWORK_INTERFACE_TRANSMIT();
	configASSERT( xIsCallingFromIPTask() == pdTRUE );

	/* Both the length of the data being sent and the actual data being sent
	are placed in the thread safe buffer used to pass data between the FreeRTOS
	tasks and the Win32 thread that sends data via the WinPCAP library.  Drop
	the packet if there is insufficient space in the buffer to hold both. */
	xSpace = uxStreamBufferGetSpace( xSendBuffer );

	if( ( pxNetworkBuffer->xDataLength <= ( ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER ) ) &&
		( xSpace >= ( pxNetworkBuffer->xDataLength + sizeof( pxNetworkBuffer->xDataLength ) ) ) )
	{
		/* First write in the length of the data, then write in the data
		itself. */
#if USE_PCAP
		uxStreamBufferAdd( xSendBuffer, 0, ( const uint8_t * ) &( pxNetworkBuffer->xDataLength ), sizeof( pxNetworkBuffer->xDataLength ) );
		uxStreamBufferAdd( xSendBuffer, 0, ( const uint8_t * ) pxNetworkBuffer->pucEthernetBuffer, pxNetworkBuffer->xDataLength );
#endif
	}
	else
	{
		FreeRTOS_debug_printf( ( "xNetworkInterfaceOutput: send buffers full to store %lu\n", pxNetworkBuffer->xDataLength ) );
	}

	/* Kick the Tx task in either case in case it doesn't know the buffer is
	full. */
	// SetEvent( pvSendEvent );
#if USE_PCAP
	pthread_mutex_lock( &xSendEventMutex );
	pthread_cond_signal( &xSendEvent );
	pthread_mutex_unlock( &xSendEventMutex );
#endif
	/* The buffer has been sent so can be released. */
	if( bReleaseAfterSend != pdFALSE )
	{
		vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
	}

	return pdPASS;
}
/*-----------------------------------------------------------*/

static pcap_if_t * prvPrintAvailableNetworkInterfaces( void )
{
#if USE_PCAP
pcap_if_t * pxAllNetworkInterfaces = NULL, *xInterface;
int32_t lInterfaceNumber = 1;
char cBuffer[ 512 ];
static BaseType_t xInvalidInterfaceDetected = pdFALSE;

	if( xInvalidInterfaceDetected == pdFALSE )
	{
		if( pcap_findalldevs( &pxAllNetworkInterfaces, cErrorBuffer ) == -1 )
		{
			printf( "Could not obtain a list of network interfaces\n%s\n", cErrorBuffer );
			pxAllNetworkInterfaces = NULL;
		}
		else
		{
			printf( "\r\n\r\nThe following network interfaces are available:\r\n\r\n" );
		}

		if( pxAllNetworkInterfaces != NULL )
		{
			/* Print out the list of network interfaces.  The first in the list
			is interface '1', not interface '0'. */
			for( xInterface = pxAllNetworkInterfaces; xInterface != NULL; xInterface = xInterface->next )
			{
				/* The descriptions of the devices can be full of spaces, clean them
				a little.  printf() can only be used here because the network is not
				up yet - so no other network tasks will be running. */
				printf( "Interface %d - %s\n", lInterfaceNumber, prvRemoveSpaces( cBuffer, sizeof( cBuffer ), xInterface->name ) );
				printf( "              (%s)\n", prvRemoveSpaces(cBuffer, sizeof( cBuffer ), xInterface->description ? xInterface->description : "No description" ) );
				printf( "\n" );
				lInterfaceNumber++;
			}
		}

		if( lInterfaceNumber == 1 )
		{
			/* The interface number was never incremented, so the above for() loop
			did not execute meaning no interfaces were found. */
			printf( " \nNo network interfaces were found.\n" );
			pxAllNetworkInterfaces = NULL;
		}

		printf( "\r\nThe interface that will be opened is set by " );
		printf( "\"configNETWORK_INTERFACE_TO_USE\", which\r\nshould be defined in FreeRTOSConfig.h\r\n" );

		if( ( xConfigNextworkInterfaceToUse < 0L ) || ( xConfigNextworkInterfaceToUse >= lInterfaceNumber ) )
		{
			printf( "\r\nERROR:  configNETWORK_INTERFACE_TO_USE is set to %ld, which is an invalid value.\r\n", xConfigNextworkInterfaceToUse );
			printf( "Please set configNETWORK_INTERFACE_TO_USE to one of the interface numbers listed above,\r\n" );
			printf( "then re-compile and re-start the application.  Only Ethernet (as opposed to WiFi)\r\n" );
			printf( "interfaces are supported.\r\n\r\nHALTING\r\n\r\n\r\n" );
			xInvalidInterfaceDetected = pdTRUE;

			if( pxAllNetworkInterfaces != NULL )
			{
				/* Free the device list, as no devices are going to be opened. */
				pcap_freealldevs( pxAllNetworkInterfaces );
				pxAllNetworkInterfaces = NULL;
			}
		}
		else
		{
			printf( "Attempting to open interface number %ld.\n", xConfigNextworkInterfaceToUse );
		}
	}

	return pxAllNetworkInterfaces;
#elif USE_FILE
	return NULL;
#endif
}
/*-----------------------------------------------------------*/

static int prvOpenInterface( const char *pucName )
{
#if USE_PCAP
static char pucInterfaceName[ 256 ];

	if( pucName != NULL )
	{
		strncpy( pucInterfaceName, pucName, sizeof( pucInterfaceName ) );
	}

	pxOpenedInterfaceHandle = pcap_open_live(	pucInterfaceName,          	/* The name of the selected interface. */
											ipTOTAL_ETHERNET_FRAME_SIZE, /* The size of the packet to capture. */
											1,							/* Open in promiscuous mode as the MAC and
																		IP address is going to be "simulated", and
																		not be the real MAC and IP address.  This allows
																		traffic to the simulated IP address to be routed
																		to uIP, and traffic to the real IP address to be
																		routed to the Windows TCP/IP stack. */
											100,
											cErrorBuffer
									   );

	if ( pxOpenedInterfaceHandle == NULL )
	{
		printf( "\n%s is not supported by LibPcap and cannot be opened\n", pucInterfaceName );
		return 1;
	}
	else
	{
		/* Configure the capture filter to allow blocking reads, and to filter
		out packets that are not of interest to this demo. */
		prvConfigureCaptureBehaviour();
	}
#elif USE_FILE
	( void * )pucName;
	prvConfigureCaptureBehaviour();
#endif
	return 0;
}
/*-----------------------------------------------------------*/

static void prvOpenSelectedNetworkInterface( pcap_if_t *pxAllNetworkInterfaces )
{
#if USE_PCAP
pcap_if_t *xInterface;
int32_t x;

	/* Walk the list of devices until the selected device is located. */
	xInterface = pxAllNetworkInterfaces;
	if (0 == xConfigNextworkInterfaceToUse) {
		while (NULL != xInterface) {
			xInterface = xInterface->next;
			if (0 == prvOpenInterface(xInterface->name)) {
				break;
			}
		}
	}
	else {
		for (x = 1L; x < xConfigNextworkInterfaceToUse; x++)
		{
			xInterface = xInterface->next;
		}
		/* Open the selected interface. */
		(void) prvOpenInterface(xInterface->name);
	}

	/* The device list is no longer required. */
	pcap_freealldevs( pxAllNetworkInterfaces );
#elif USE_FILE
	( void * )pxAllNetworkInterfaces;
	prvOpenInterface(NULL);
#endif
}
/*-----------------------------------------------------------*/

static void prvConfigureCaptureBehaviour( void )
{
struct bpf_program xFilterCode;
uint32_t ulNetMask;
int rc;
cpu_set_t cpu_set;
int cores;
int i;
#if USE_PCAP
	/* Set up a filter so only the packets of interest are passed to the IP
	stack.  cErrorBuffer is used for convenience to create the string.  Don't
	confuse this with an error message. */
	sprintf( cErrorBuffer, "broadcast or multicast or ether host %x:%x:%x:%x:%x:%x",
		ucMACAddress[0], ucMACAddress[1], ucMACAddress[2], ucMACAddress[3], ucMACAddress[4], ucMACAddress[5] );

	ulNetMask = ( configNET_MASK3 << 24UL ) | ( configNET_MASK2 << 16UL ) | ( configNET_MASK1 << 8L ) | configNET_MASK0;

	if( pcap_compile( pxOpenedInterfaceHandle, &xFilterCode, cErrorBuffer, 1, ulNetMask ) < 0 )
	{
		printf( "\nThe packet filter string is invalid\n" );
	}
	else
	{
		if( pcap_setfilter( pxOpenedInterfaceHandle, &xFilterCode ) < 0 )
		{
			printf( "\nAn error occurred setting the packet filter.\n" );
		}
	}

	/* Create the buffers used to pass packets between the FreeRTOS simulator
	and the Win32 threads that are handling WinPCAP. */
#endif
	prvCreateThreadSafeBuffers();

	if( SendEventInited == 0 )
	{
		/* Create event used to signal the Win32 WinPCAP Tx thread. */
		// xSendEvent = CreateEvent( NULL, FALSE, TRUE, NULL );
		SendEventInited = 1;
		pthread_cond_init( &xSendEvent, NULL );

		/* Create the Win32 thread that handles WinPCAP Rx. */
		// vWinPcapRecvThreadHandle = CreateThread(
			// NULL,	/* Pointer to thread security attributes. */
			// 0,		/* Initial thread stack size, in bytes. */
			// prvWinPcapRecvThread,	/* Pointer to thread function. */
			// NULL,	/* Argument for new thread. */
			// 0,		/* Creation flags. */
			// NULL );
		// add mutex for Synchronize
#if USE_PCAP
		rc = pthread_create( &hLibPcapRecvThreadHandle, NULL, prvLibPcapRecvThread, NULL );
		assert(rc == 0);

		/* Use the cores that are not used by the FreeRTOS tasks. */
		// SetThreadAffinityMask( vWinPcapRecvThreadHandle, ~0x01u );
		// cores = get_nprocs();
		// CPU_ZERO(&cpu_set);
  //   	for( i = 1; i < cores; i++ )
  //   	{
  //   		CPU_SET( i, &cpu_set );
  //   	}
  //   	// CPU_SET( 2, &cpu_set );
		// pthread_setaffinity_np( hLibPcapRecvThreadHandle, sizeof(cpu_set_t), &cpu_set );


		/* Create the Win32 thread that handlers WinPCAP Tx. */
		// vWinPcapSendThreadHandle = CreateThread(
			// NULL,	/* Pointer to thread security attributes. */
			// 0,		/* initial thread stack size, in bytes. */
			// prvWinPcapSendThread,	/* Pointer to thread function. */
			// NULL,	/* Argument for new thread. */
			// 0,		/* Creation flags. */
			// NULL );
		rc = pthread_create( &hLibPcapSendThreadHandle, NULL, prvLibPcapSendThread, NULL );
		assert(rc == 0);

		/* Use the cores that are not used by the FreeRTOS tasks. */
		// SetThreadAffinityMask( vWinPcapSendThreadHandle, ~0x01u );
		// cores = get_nprocs();
		// CPU_ZERO(&cpu_set);
  //   	for( i = 1; i < cores; i++ )
  //   	{
  //   		CPU_SET( i, &cpu_set );
  //   	}
  //   	// CPU_SET( 3, &cpu_set );
		// pthread_setaffinity_np( hLibPcapSendThreadHandle, sizeof(cpu_set_t), &cpu_set );
#endif

		/* Create a task that simulates an interrupt in a real system.  This will
		block waiting for packets, then send a message to the IP task when data
		is available. */
		xTaskCreate( prvInterruptSimulatorTask, "MAC_ISR", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		//  configMAC_ISR_SIMULATOR_PRIORITY
	}
}
/*-----------------------------------------------------------*/

/* WinPCAP function. */
void pcap_callback( u_char *user, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data )
{
	(void)user;

	/* THIS IS CALLED FROM A WINDOWS THREAD - DO NOT ATTEMPT ANY FREERTOS CALLS
	OR TO PRINT OUT MESSAGES HERE. */

	/* Pass data to the FreeRTOS simulator on a thread safe circular buffer. */
	// if(pkt_header->caplen <= ( ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER ))
	// 	printf("first yes\n");
	// else
	// 	printf("first no\n");
	// if( uxStreamBufferGetSpace( xRecvBuffer ) >= ( ( ( size_t ) pkt_header->caplen ) + sizeof( *pkt_header ) ) )
	// 	printf("second yes\n");
	// else
	// 	printf("second no\n");

	if( ( pkt_header->caplen <= ( ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER ) ) &&
		( uxStreamBufferGetSpace( xRecvBuffer ) >= ( ( ( size_t ) pkt_header->caplen ) + sizeof( *pkt_header ) ) ) )
	{
		// printf("Before Recv Buffer Size:%d\n", uxStreamBufferGetSize(xRecvBuffer));
		uxStreamBufferAdd( xRecvBuffer, 0, ( const uint8_t* ) pkt_header, sizeof( *pkt_header ) );
		uxStreamBufferAdd( xRecvBuffer, 0, ( const uint8_t* ) pkt_data, ( size_t ) pkt_header->caplen );
		// printf("After Recv Buffer Size:%d\n", uxStreamBufferGetSize(xRecvBuffer));
		// printf("pcap_callback success\n");
	}
	// else
		// printf("pcap_callback fail\n");
}
/*-----------------------------------------------------------*/
// DWORD WINAPI prvWinPcapRecvThread ( void *pvParam )
void *prvLibPcapRecvThread ( void *pvParam )
{
	( void ) pvParam;

	/* THIS IS A WINDOWS THREAD - DO NOT ATTEMPT ANY FREERTOS CALLS	OR TO PRINT
	OUT MESSAGES HERE. */
	unsigned long i = 0;
	prctl(PR_SET_NAME,"RecvThread\n");
	for( ;;i++ )
	{
		pthread_attr_t attr;
		struct sched_param param;
		pthread_attr_getschedparam( &attr, &param);
		pcap_dispatch( pxOpenedInterfaceHandle, 1, pcap_callback, ( u_char * ) "mydata" );
	}
}
/*-----------------------------------------------------------*/
// DWORD WINAPI prvWinPcapSendThread( void *pvParam )
void *prvLibPcapSendThread( void *pvParam )
{
size_t xLength;
uint8_t ucBuffer[ ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER ];
static char cErrorMessage[ 1024 ];
const unsigned long xMaxMSToWait = 1000;
struct timeval now;
struct timespec outtime;


	/* THIS IS A WINDOWS THREAD - DO NOT ATTEMPT ANY FREERTOS CALLS	OR TO PRINT
	OUT MESSAGES HERE. */

	/* Remove compiler warnings about unused parameters. */
	( void ) pvParam;
	prctl(PR_SET_NAME,"SendThread\n");
	unsigned long i = 0;
	for( ;; i++)
	{
		pthread_attr_t attr;
		struct sched_param param;
		pthread_attr_getschedparam( &attr, &param);
		// printf("send priority = %d\n", param.__sched_priority);
		/* Wait until notified of something to send. */
		// WaitForSingleObject( pvSendEvent, xMaxMSToWait );
		pthread_mutex_lock( &xSendEventMutex );
		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + xMaxMSToWait / 1000;
		outtime.tv_nsec = (now.tv_usec + xMaxMSToWait % 1000 ) * 1000 ;
		pthread_cond_timedwait( &xSendEvent, &xSendEventMutex, &outtime );
		pthread_mutex_unlock( &xSendEventMutex );

		/* Is there more than the length value stored in the circular buffer
		used to pass data from the FreeRTOS simulator into this Win32 thread? */
		// printf("Enter send while:%lu\n",i);
		// printf("Before Send Buffer size: %d\n",uxStreamBufferGetSize( xSendBuffer ));
		while( uxStreamBufferGetSize( xSendBuffer ) > sizeof( xLength ) )
		{
			uxStreamBufferGet( xSendBuffer, 0, ( uint8_t * ) &xLength, sizeof( xLength ), pdFALSE );
			uxStreamBufferGet( xSendBuffer, 0, ( uint8_t* ) ucBuffer, xLength, pdFALSE );
			// printf("try to send a packet:%lu\n",i);
#if USE_PCAP
			if( pcap_sendpacket( pxOpenedInterfaceHandle, ucBuffer, xLength  ) != 0 )
			{
				ulWinPCAPSendFailures++;
				// printf("fail:%d\n",ulWinPCAPSendFailures);
			}
#elif USE_FILE
			continue;
#endif
		}
	}
}
/*-----------------------------------------------------------*/

static void prvInterruptSimulatorTask( void *pvParameters )
{
    struct pcap_pkthdr xHeader;
    static struct pcap_pkthdr *pxHeader;
    const uint8_t *pucPacketData;
    uint8_t ucRecvBuffer[ ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER ];
    NetworkBufferDescriptor_t *pxNetworkBuffer;
    IPStackEvent_t xRxEvent = { eNetworkRxEvent, NULL };
    eFrameProcessingResult_t eResult;

	/* Remove compiler warnings about unused parameters. */
	( void ) pvParameters;
	memset(ucRecvBuffer, 0, ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER);

	for( ;; )
	{
#if USE_PCAP
		if( uxStreamBufferGetSize( xRecvBuffer ) > sizeof( xHeader ) )
		{
			/* Get the next packet. */
			uxStreamBufferGet( xRecvBuffer, 0, (uint8_t*)&xHeader, sizeof( xHeader ), pdFALSE );
			uxStreamBufferGet( xRecvBuffer, 0, (uint8_t*)ucRecvBuffer, ( size_t ) xHeader.len, pdFALSE );
#elif USE_FILE
		if(1)
		{
			FreeRTOS_Socket_t *pxSocket ;
			while( !( pxSocket = pxUDPSocketLookup( FreeRTOS_htons(10000) ) ) ){
				vTaskDelay( configWINDOWS_MAC_INTERRUPT_SIMULATOR_DELAY );
			}
			extern QueueHandle_t xNetworkEventQueue;

		#ifdef __AFL_HAVE_MANUAL_CONTROL
			while (__AFL_LOOP(1000)){
		#endif
			    xHeader.len = read(STDIN_FILENO, ucRecvBuffer, ipconfigNETWORK_MTU + ipSIZE_OF_ETH_HEADER);
#endif
                pucPacketData = ucRecvBuffer;
                pxHeader = &xHeader;

                iptraceNETWORK_INTERFACE_RECEIVE();

                /* Check for minimal size. */
                if( pxHeader->len >= sizeof( EthernetHeader_t ) )
                {
                    eResult = ipCONSIDER_FRAME_FOR_PROCESSING( pucPacketData );
                }
                else
                {
                    eResult = eReleaseBuffer;
                }

                if( eResult == eProcessBuffer )
                {
                    /* Will the data fit into the frame buffer? */
                    if( pxHeader->len <= ipTOTAL_ETHERNET_FRAME_SIZE )
                    {
                        /* Obtain a buffer into which the data can be placed.  This
                        is only	an interrupt simulator, not a real interrupt, so it
                        is ok to call the task level function here, but note that
                        some buffer implementations cannot be called from a real
                        interrupt. */
                        pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( pxHeader->len, 0 );
                        if( pxNetworkBuffer != NULL )
                        {
                            memcpy( pxNetworkBuffer->pucEthernetBuffer, pucPacketData, pxHeader->len );
                            pxNetworkBuffer->xDataLength = ( size_t ) pxHeader->len;

                            #if( niDISRUPT_PACKETS == 1 )
                            {
                                pxNetworkBuffer = vRxFaultInjection( pxNetworkBuffer, pucPacketData );
                            }
                            #endif /* niDISRUPT_PACKETS */

                            if( pxNetworkBuffer != NULL )
                            {
                                // printf("11\t");
                                xRxEvent.pvData = ( void * ) pxNetworkBuffer;

                                /* Data was received and stored.  Send a message to
                                the IP task to let it know. */
                                // Fuzz
                                // taskENTER_CRITICAL();
                                if( xSendEventStructToIPTask( &xRxEvent, ( TickType_t ) 0 ) == pdFAIL )
                                {
                                    /* The buffer could not be sent to the stack so
                                    must be released again.  This is only an
                                    interrupt simulator, not a real interrupt, so it
                                    is ok to use the task level function here, but
                                    note no all buffer implementations will allow
                                    this function to be executed from a real
                                    interrupt. */
                                    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                                    iptraceETHERNET_RX_EVENT_LOST();
                                    // printf("13\t");
                                }
                                else{
                                    // FUZZ_NETWORK_RX_QUEUE += 1;
                                }
                                // taskEXIT_CRITICAL();
                            }
                            else
                            {
                                // printf("14\t");
                                /* The packet was already released or stored inside
                                vRxFaultInjection().  Don't release it here. */
                            }
                            while( pxNetworkBuffer->pucEthernetBuffer != NULL ){
								continue;
							}
                        }
                        else
                        {
                            // printf("15\t");
                            iptraceETHERNET_RX_EVENT_LOST();
                        }
                    }
                    else
                    {
                        /* Log that a packet was dropped because it would have
                        overflowed the buffer, but there may be more buffers to
                        process. */
                    }
                }
		#ifdef __AFL_HAVE_MANUAL_CONTROL
			}
		#endif
		}
		else
		{
			/* There is no real way of simulating an interrupt.  Make sure
			other tasks can run. */
			vTaskDelay( configWINDOWS_MAC_INTERRUPT_SIMULATOR_DELAY );
		}
	}
}
/*-----------------------------------------------------------*/

static const char *prvRemoveSpaces( char *pcBuffer, int aBuflen, const char *pcMessage )
{
	char *pcTarget = pcBuffer;

	/* Utility function used to formap messages being printed only. */
	while( ( *pcMessage != 0 ) && ( pcTarget < ( pcBuffer + aBuflen - 1 ) ) )
	{
		*( pcTarget++ ) = *pcMessage;

		if( isspace( *pcMessage ) != pdFALSE )
		{
			while( isspace( *pcMessage ) != pdFALSE )
			{
				pcMessage++;
			}
		}
		else
		{
			pcMessage++;
		}
	}

	*pcTarget = '\0';

	return pcBuffer;
}

