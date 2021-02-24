# para-rehosting
Finding bugs in microcontroller (MCU) firmware is challenging, even for device manufacturers who own the source code. The MCU runs different instruction sets than x86 and  exposes a very different development environment. This invalidates many existing sophisticated software testing tools on x86. To maintain a unified developing and testing environment, a straightforward way is to re-compile the source code into the native executable for a commodity machine (called rehosting). However, ad-hoc re-hosting is a daunting and tedious task and subject to many issues (library-dependence, kernel-dependence and hardware-dependence). In this work, we systematically explore the portability problem of MCU software and propose para-rehosting to ease the porting process. Specifically, we abstract and implement a portable MCU (PMCU) using the POSIX interface. It models common functions of the MCU cores. For peripheral specific logic, we propose HAL-based peripheral function replacement, in which high-level hardware functions are replaced with an equivalent backend driver on the host. These backend drivers are invoked by well-designed para-APIs and can be reused across many MCU OSs.  We categorize common HAL functions into four types and implement templates  for quick backend development. Using the proposed approach, we have successfully rehosted nine MCU OSs including the widely deployed Amazon FreeRTOS, ARM Mbed OS, Zephyr and LiteOS. To demonstrate the superiority of our approach in terms of security testing, we used off-the-shelf dynamic analysis tools (AFL and ASAN) against the rehosted programs and discovered 28 previously-unknown bugs, among which 5 were confirmed by CVE and the other 19 were confirmed by vendors at the time of writing.

This repo is for the paper **From Library Portability to Para-rehosting: Natively Executing Microcontroller Software on Commodity Hardware**.
You may download a full copy  [here](https://www.ndss-symposium.org/wp-content/uploads/ndss2021_6B-3_24308_paper.pdf).

## Directory Arch
	- FreeRTOS/: Fuzzing demos for FreeRTOS
	- MbedOS/: Fuzzing demos for Mbed OS5
	- FreeRTOS_FE.c: Source code for FreeRTOS frontend
	- MbedOS_FE.c: Source code for Mbed OS frontend
	- RTOS_BE.c: Source code for RTOS backend
	- RTOS_BE.h: Header file for RTOS backend
	- debug_log.h: Mbed OS debug log header
	- port.h: Header file for Mbed OS frontend
	- portmacro.h: Header file for FreeRTOS frontend

## How to setup
To fuzz with provided demos, please prepare the enviroment below.
- Ubuntu 18.04
- AFL with LLVM mode
- Higher than LLVM 11
	- Please refer to https://github.com/google/AFL and https://apt.llvm.org/
- Pcap
	- sudo apt install libpcap0.8-dev:i386 libpcap0.8:i386

## How to fuzz
For FreeRTOS, each demo contains a makefile and you only need to run the command `make` to build them.

For Mbed OS, all demos are included in the file main.cpp and, an option `FUZZ_TARGET` is provided to choose which demo is target.

To fuzz, please use the command `afl-fuzz -m none -t 1000 -i ./in_dir -o ./our_dir ./binary_file`


> P.S.
> 1. FreeRTOS_FATFS is patched and won't crash. If not, we couldn't reach deeper path.
> 2. Only partial demos are uploaded. Others will come soon.

## How to port
To port a new RTOS, you need to implement a frondend source file and a header file with the help of RTOS backend. 
Specifically, you have to implement two frontend functions for the RTOS backend.
- PMCU_FE_Yield(): The RTOS operations to switch the tasks with RTOS API.
- FE_Thread_Exit(): The RTOS operations when a task exit.
