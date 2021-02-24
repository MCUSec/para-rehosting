CC=AFL_USE_ASAN=1 afl-clang-fast
AFL_CC=AFL_USE_ASAN=1 afl-clang-fast

VERSION ?= FreeRTOSv10.0.0

FREERTOS_SRC_DIR=../${VERSION}/FreeRTOS/Source
PORTABLE_SRC_DIR=../../../
PORTABLE_SRC_MEM_MANG_DIR=../${VERSION}/FreeRTOS/Source/portable/MemMang
FREERTOS_PLUS_TCP_SRC_DIR=../${VERSION}/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP
FREERTOS_PLUS_TCP_DEMO_SRC_DIR=./DemoTasks
FREERTOS_PLUS_TCP_BUF_MANG_DIR=../${VERSION}/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/BufferManagement
FREERTOS_PLUS_TCP_NETWORKINTERFACE_SRC_DIR=../port_network

FREERTOS_INC_DIR=../${VERSION}/FreeRTOS/Source/include
PORTABLE_INC_DIR=../../../
FREERTOS_PLUS_TCP_INC_DIR=../${VERSION}/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/include
FREERTOS_PLUS_TCP_COMPILER_GCC_INC_DIR=../${VERSION}/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/Compiler/GCC
FREERTOS_PLUS_TCP_DEMO_INC_DIR=./DemoTasks/include

CFLAGS += -m32 -Wall -Werror -Wno-unused -Wextra -Wpedantic -Wno-gnu -Wno-sign-compare -Wno-unused-command-line-argument -Wno-unused-parameter -pthread  -O0 -g
LDFLAGS += -m32 -Wall -Werror -Wno-unused -Wextra -Wpedantic -Wno-gnu -Wno-sign-compare -Wno-unused-command-line-argument -Wno-unused-parameter -pthread  -O0 -g 

CFLAGS += -Wno-address-of-packed-member
LDFLAGS += -Wno-address-of-packed-member

INCLUDE_DIRS +=	-I. \
				-I${FREERTOS_INC_DIR} \
				-I${PORTABLE_INC_DIR} \
				-I${FREERTOS_PLUS_TCP_INC_DIR} \
				-I${FREERTOS_PLUS_TCP_COMPILER_GCC_INC_DIR} \
				-I${FREERTOS_PLUS_TCP_DEMO_INC_DIR} \

VPATH += ${FREERTOS_SRC_DIR} \
		 ${PORTABLE_SRC_DIR} \
		 ${PORTABLE_SRC_MEM_MANG_DIR} \
		 ${FREERTOS_PLUS_TCP_SRC_DIR} \
		 ${FREERTOS_PLUS_TCP_BUF_MANG_DIR} \
		 ${FREERTOS_PLUS_TCP_NETWORKINTERFACE_SRC_DIR} \
		 ${FREERTOS_PLUS_TCP_DEMO_SRC_DIR} \


TARGET = Linux_gcc_TCP_Minimal

default: all

all: ${TARGET}

SRC = \
	event_groups.c \
	list.c \
	queue.c \
	tasks.c \
	timers.c \
	croutine.c \
	FreeRTOS_FE.c \
	RTOS_BE.c \

AFL_SRC= \
	main.c \
	heap_4.c \
	FreeRTOS_ARP.c \
	FreeRTOS_DHCP.c \
	FreeRTOS_DNS.c \
	FreeRTOS_IP.c \
	FreeRTOS_Sockets.c \
	FreeRTOS_Stream_Buffer.c \
	FreeRTOS_TCP_IP.c \
	FreeRTOS_TCP_WIN.c \
	FreeRTOS_UDP_IP.c \
	BufferAllocation_2.c \
	NetworkInterface.c \
	SimpleTCPEchoServer.c\
	SimpleUDPClientAndServer.c \
	TCPEchoClient_SingleTasks.c \
	demo_logging.c \

OBJ = ${SRC:.c=.o}
${OBJ}: %.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@ -lpcap

AFL_OBJ = ${AFL_SRC:.c=.o} 
${AFL_OBJ}: %.o: %.c
	${AFL_CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@ -lpcap


$(TARGET):	${AFL_OBJ} ${OBJ}
	${AFL_CC} ${LDFLAGS} -o $@ ${AFL_OBJ} ${OBJ} -lpcap 


.PHONY : clean
clean:
	-rm ${OBJ}
	-rm *~
	-rm ${TARGET}
	-rm *.o


