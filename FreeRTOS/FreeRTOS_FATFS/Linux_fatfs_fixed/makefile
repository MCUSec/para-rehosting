CC=AFL_USE_ASAN=1 afl-clang-fast
AFL_CC=AFL_USE_ASAN=1 afl-clang-fast

VERSION ?= FreeRTOSv10.2.0

FREERTOS_SRC_DIR=../../${VERSION}/FreeRTOS/Source
PORTABLE_SRC_MEM_MANG_DIR=../../${VERSION}/FreeRTOS/Source/portable/MemMang
FREERTOS_PLUS_FAT_SRC=../FreeRTOS-Plus-FAT
FREERTPS_PLUS_FAT_PORT=../FreeRTOS-Plus-FAT/portable/common
PORT_LINUX_SRC=../../../

FREERTOS_INC_DIR=../../${VERSION}/FreeRTOS/Source/include
FREERTOS_PLUS_FAT_INC=../FreeRTOS-Plus-FAT/include
PORT_LINUX_INC=../../../
CONFIG_INC=./

CFLAGS += -m32 -Wall -Wformat=0 -Wno-unused -Wextra -Wpedantic -Wno-sign-compare -Wno-gnu-statement-expression -Wno-unused-parameter -pthread  -O0 -g
LDFLAGS += -m32 -Wall -Wformat=0 -Wno-unused -Wextra -Wpedantic -Wno-sign-compare -Wno-gnu-statement-expression -Wno-unused-parameter -pthread  -O0 -g 

INCLUDE_DIRS +=	-I. \
				-I${FREERTOS_INC_DIR} \
				-I${FREERTOS_PLUS_FAT_INC} \
				-I${FREERTPS_PLUS_FAT_PORT} \
				-I${PORT_LINUX_INC} \
				-I${CONFIG_INC} \

VPATH += ${FREERTOS_SRC_DIR} \
		 ${PORTABLE_SRC_MEM_MANG_DIR} \
		 ${FREERTOS_PLUS_FAT_SRC} \
		 ${FREERTPS_PLUS_FAT_PORT} \
		 ${PORT_LINUX_SRC} \

TARGET = Linux_fatfs

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

AFL_SRC = \
	main.c \
	heap_4.c \
	ff_crc.c \
	ff_dir.c \
	ff_error.c \
	ff_fat.c \
	ff_file.c \
	ff_format.c \
	ff_ioman.c \
	ff_locking.c \
	ff_memory.c \
	ff_stdio.c \
	ff_string.c \
	ff_sys.c \
	ff_time.c \
	ff_ramdisk.c \

OBJ = ${SRC:.c=.o}
${OBJ}: %.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@

AFL_OBJ = ${AFL_SRC:.c=.o} 
${AFL_OBJ}: %.o: %.c
	${AFL_CC} -I${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@

$(TARGET):	${AFL_OBJ} ${OBJ}
	${AFL_CC} ${LDFLAGS} -o $@ ${AFL_OBJ} ${OBJ}

.PHONY : clean
clean:
	-rm ${OBJ}
	-rm *~
	-rm ${TARGET}
	-rm *.o


