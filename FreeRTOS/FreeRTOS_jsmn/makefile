CC=AFL_USE_ASAN=1 afl-clang-fast
VERSION ?= FreeRTOSv10.0.0

CFLAGS += -m32 -Wno-unused -Wno-unused-parameter -Wextra -Wpedantic -Wno-gnu -Wno-sign-compare -Wno-variadic-macros -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-pedantic -Wno-unused-parameter -pthread  -O0 -g 
LDFLAGS += -m32 -Wno-unused -Wno-unused-parameter -Wextra -Wpedantic -Wno-gnu -Wno-sign-compare -Wno-variadic-macros -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-pedantic -Wno-unused-parameter -pthread  -O0 -g 

FREERTOS_SRC_DIR=./${VERSION}/FreeRTOS/Source
PORTABLE_SRC_DIR=../../
PORTABLE_SRC_MEM_MANG_DIR=./${VERSION}/FreeRTOS/Source/portable/MemMang
SRC_DIR = ./src
DEMOS_DIR = ./demos

SRC_INC = ./src
FREERTOS_INC_DIR=./${VERSION}/FreeRTOS/Source/include
PORTABLE_INC_DIR=../../

INCLUDE_DIRS +=	-I. \
				-I${SRC_INC} \
				-I${FREERTOS_INC_DIR} \
				-I${PORTABLE_INC_DIR} \

VPATH += \
		${FREERTOS_SRC_DIR} \
		${PORTABLE_SRC_DIR} \
		${PORTABLE_SRC_MEM_MANG_DIR} \
		${SRC_DIR} \
		${DEMOS_DIR} \


TARGET = jsmn

default: all

all: ${TARGET}

SRC = \
	event_groups.c \
	list.c \
	queue.c \
	tasks.c \
	timers.c \
	heap_4.c \
	croutine.c \
	jsmn.c \
	main.c \
	FreeRTOS_FE.c \
	RTOS_BE.c \

	

OBJ = ${SRC:.c=.o}
${OBJ}: %.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@

$(TARGET): ${OBJ}
	${CC} ${LDFLAGS} ${LD_EXE_FLAGS} -o $@ ${OBJ} ${LIBS} 


.PHONY : clean
clean:
	-rm ${OBJ}
	-rm *~
	-rm ${TARGET}
	-rm *.o

