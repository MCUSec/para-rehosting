CC=AFL_USE_ASAN=1 AFL_DONT_OPTIMIZE=1 afl-clang-fast 
VERSION ?= aws131_001

LIB_UTILS_DIR=./${VERSION}/lib/utils
LIB_TLS_DIR=./${VERSION}/lib/tls
LIB_MBEDTLS_LIB_DIR=./${VERSION}/lib/third_party/mbedtls/library
LIB_SECURE_SOCKET_PORT_DIR=./${VERSION}/lib/secure_sockets/portable/freertos_plus_tcp
LIB_PKCS11_PORT_MBEDTLS_DIR=./${VERSION}/lib/pkcs11/mbedtls
LIB_MQTT_DIR=./${VERSION}/lib/mqtt
LIB_FREERTOS_DIR=./${VERSION}/lib/FreeRTOS
PORTABLE_LINUX_DIR=../../
LIB_FREERTOS_PORT_MEMMANG_DIR=./${VERSION}/lib/FreeRTOS/portable/MemMang
LIB_FREERTOSTCP_PORT_BUFFER_DIR=./${VERSION}/lib/FreeRTOS-Plus-TCP/source/portable/BufferManagement
LIB_FREERTOSTCP_SRC_DIR=./${VERSION}/lib/FreeRTOS-Plus-TCP/source
LIB_CRYPTO_DIR=./${VERSION}/lib/crypto
LIB_BUFFERPOLL_DIR=./${VERSION}/lib/bufferpool
LIB_NETWORK_DIR=./port_network

INC_LIB_DIR=./${VERSION}/lib/include
INC_PRIVATE_DIR=./${VERSION}/lib/include/private
INC_PORT_LINUX_DIR=../../
INC_PKCK11_DIR=./${VERSION}/lib/third_party/pkcs11
INC_MBEDTLS_DIR=./${VERSION}/lib/third_party/mbedtls/include
INC_FREERTOS_TCP_DIR=./${VERSION}/lib/FreeRTOS-Plus-TCP/include
INC_FREERTOS_TCP_COMPILER_GCC_DIR=./${VERSION}/lib/FreeRTOS-Plus-TCP/source/portable/Compiler/GCC

CLANGVERSION := $(shell expr `clang --version | grep clang | cut -f3 -d\ `)

CFLAGS += -m32 -Wall -Wno-unused -Werror -Wno-format -Wextra -Wno-gnu -Wno-sign-compare -Wno-missing-field-initializers -pthread  -O0 -g
LDFLAGS += -m32 -Wall -Wno-unused -Werror -Wno-format -Wextra -Wno-gnu -Wno-sign-compare -Wno-missing-field-initializers -pthread  -O0 -g 

CFLAGS += -Wno-address-of-packed-member
LDFLAGS += -Wno-address-of-packed-member

INCLUDE_DIRS +=	-I./include \
				-I${INC_LIB_DIR} \
				-I${INC_PRIVATE_DIR} \
				-I${INC_PORT_LINUX_DIR} \
				-I${INC_FREERTOS_TCP_DIR} \
				-I${INC_FREERTOS_TCP_COMPILER_GCC_DIR} \
				-I${INC_PKCK11_DIR} \
				-I${INC_MBEDTLS_DIR} \

VPATH += \
		${LIB_MQTT_DIR} \
		${LIB_FREERTOS_DIR} \
		${PORTABLE_LINUX_DIR} \
		${LIB_FREERTOS_PORT_MEMMANG_DIR} \
		${LIB_FREERTOSTCP_PORT_BUFFER_DIR} \
		${LIB_FREERTOSTCP_SRC_DIR} \
		${LIB_NETWORK_DIR} \
		${LIB_SECURE_SOCKET_PORT_DIR} \
		${LIB_BUFFERPOLL_DIR} \
		${LIB_UTILS_DIR} \
		${LIB_TLS_DIR} \
		${LIB_PKCS11_PORT_MBEDTLS_DIR} \
		${LIB_MBEDTLS_LIB_DIR} \
		${LIB_CRYPTO_DIR} \

TARGET = Linux_MQTT

default: all

all: ${TARGET}

SRC = \
	main.c \



FREERTOS_SRC= \
			timers.c \
			tasks.c \
			stream_buffer.c \
			queue.c \
			FreeRTOS_FE.c \
			RTOS_BE.c \
			heap_4.c \
			list.c \
			event_groups.c \
			BufferAllocation_2.c \
			FreeRTOS_UDP_IP.c \
			FreeRTOS_TCP_WIN.c \
			FreeRTOS_TCP_IP.c \
			FreeRTOS_Stream_Buffer.c \
			FreeRTOS_Sockets.c \
			FreeRTOS_IP.c \
			FreeRTOS_DNS.c \
			FreeRTOS_DHCP.c \
			FreeRTOS_ARP.c \
			NetworkInterface.c \
			aws_bufferpool_static_thread_safe.c \
			aws_secure_sockets.c \
			aws_mqtt_lib.c \
			aws_mqtt_agent.c \
			aws_system_init.c \
			aws_tls.c \
			aws_pkcs11_mbedtls.c \
			x509_crt.c \
			platform.c \
			pk.c \
			md.c \
			md_wrap.c \
			sha256.c \
			sha512.c \
			sha1.c \
			pk_wrap.c \
			ecdsa.c \
			ecp.c \
			rsa.c \
			bignum.c \
			rsa_internal.c \
			x509_csr.c \
			x509_crl.c \
			x509_create.c \
			x509write_csr.c \
			x509write_crt.c \
			x509.c \
			md5.c \
			md4.c \
			md2.c \
			pkcs5.c \
			pkcs12.c \
			oid.c \
			base64.c \
			asn1write.c \
			asn1parse.c \
			ecp_curves.c \
			pem.c \
			hmac_drbg.c \
			ecdh.c \
			dhm.c \
			des.c \
			aesni.c \
			aes.c \
			cipher_wrap.c \
			cipher.c \
			certs.c \
			gcm.c \
			pkparse.c \
			padlock.c \
			net_sockets.c \
			memory_buffer_alloc.c \
			havege.c \
			pkwrite.c \
			entropy_poll.c \
			entropy.c \
			ctr_drbg.c \
			blowfish.c \
			ssl_tls.c \
			ssl_ticket.c \
			ssl_srv.c \
			ssl_cookie.c \
			ssl_cli.c \
			ssl_ciphersuites.c \
			ssl_cache.c \
			aws_pkcs11_pal.c \
			aws_crypto.c \
			aws_hello_world.c \
			aws_entropy_hardware_poll.c \

OBJ = ${SRC:.c=.o}
${OBJ}: %.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@ -lpcap

FREERTOS_OJB = ${FREERTOS_SRC:.c=.o} 
${FREERTOS_OJB}: %.o: %.c
	${CC} ${INCLUDE_DIRS} ${CFLAGS} -c $< -o $@ -lpcap


$(TARGET):	${FREERTOS_OJB} ${OBJ}
	${CC} ${LDFLAGS} ${LD_EXE_FLAGS} -o $@ ${FREERTOS_OJB} ${OBJ} ${LIBS} -lpcap 


.PHONY : clean
clean:
	-rm ${OBJ}
	-rm *~
	-rm ${TARGET}
	-rm *.o


