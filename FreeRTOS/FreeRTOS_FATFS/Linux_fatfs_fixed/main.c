/*
 * FreeRTOS Kernel V10.2.0
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * This project is a cut down version of the project described on the following
 * link.  Only the simple UDP client and server and the TCP echo clients are
 * included in the build:
 * http://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/examples_FreeRTOS_simulator.html
 */

/* Standard includes. */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+FAT includes. */
#include "ff_headers.h"
#include "ff_stdio.h"
#include "ff_ramdisk.h"

/* The number and size of sectors that will make up the RAM disk.  The RAM disk
is huge to allow some verbose FTP tests. */
#define mainRAM_DISK_SECTOR_SIZE	512UL /* Currently fixed! */
#define mainRAM_DISK_SECTORS		( ( 3 * 1024 * 1024UL ) / mainRAM_DISK_SECTOR_SIZE ) /* 5M bytes. */
#define mainIO_MANAGER_CACHE_SIZE	(2UL * mainRAM_DISK_SECTOR_SIZE )

// #define mainRAM_DISK_SECTORS		( (2 * 512UL ) / mainRAM_DISK_SECTOR_SIZE ) /* 1 sector, 512 bytes. */
// #define mainIO_MANAGER_CACHE_SIZE	( mainRAM_DISK_SECTOR_SIZE )

/* Where the RAM disk is mounted. */
#define mainRAM_DISK_NAME			"/ram"

/* The number of bytes read/written to the example files at a time. */
#define fsRAM_BUFFER_SIZE 				200

/* The number of bytes written to the file that uses f_putc() and f_getc(). */
#define fsPUTC_FILE_SIZE				100

/* Names of directories that are created. */
static const char *pcDirectory1 = "SUB1", *pcDirectory2 = "SUB2", *pcFullPath = "/SUB1/SUB2";

void prvCreateDiskAndExampleFiles( void* param );
void vCreateAndVerifyExampleFiles( const char *pcMountPath );
static void prvCreateDemoFilesUsing_ff_fwrite( const char *pcMountPath );
static void prvVerifyDemoFileUsing_ff_fread( void );
static void prvCreateDemoFileUsing_ff_fputc( const char *pcMountPath );
static void prvVerifyDemoFileUsing_ff_fgetc( const char *pcMountPath );
void FuzzOperations(const char *Dir);
void fileRead(const char *fileName);
void fileWrite(const char *fileName);

/*-------------------------------------------------------------------*/
int main( void )
{
const uint32_t ulLongTime_ms = pdMS_TO_TICKS( 1000UL );

	xTaskCreate(prvCreateDiskAndExampleFiles, "FATFS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	/* Start the RTOS scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details (this is standard text that is not not
	really applicable to the Win32 simulator port). */
	for( ;; )
	{
		usleep( ulLongTime_ms );
	}
}

/*-------------------------------------------------------------------*/
void prvCreateDiskAndExampleFiles( void* param )
{
// static uint8_t ucRAMDisk[ mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE ];
// static uint8_t backup[ mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE ];
uint8_t* ucRAMDisk = (uint8_t*)malloc(mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE);
uint8_t* backup = (uint8_t*)malloc(mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE);
FF_Disk_t *pxDisk;
int fuzz_read = 20992;

	(void*)param;
	/* Create the RAM disk. */



	/* Print out information on the disk. */
	// FF_RAMDiskShowPartition( pxDisk );
	FILE *fp;
	fp = fopen("./init.img","rb");
	memset(backup, 0x0, mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE);
	fread( backup, sizeof(uint8_t), mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE, fp );
	fclose(fp);
	fp = NULL;
#ifdef __AFL_HAVE_MANUAL_CONTROL
	memcpy(ucRAMDisk, backup, mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE);
	while( __AFL_LOOP(1000) ){
		pxDisk = FF_RAMDiskInit( mainRAM_DISK_NAME, ucRAMDisk, mainRAM_DISK_SECTORS, mainIO_MANAGER_CACHE_SIZE );
		configASSERT( pxDisk );
		memcpy(ucRAMDisk, backup, fuzz_read);
		read(STDIN_FILENO, ucRAMDisk, fuzz_read);
		FuzzOperations(mainRAM_DISK_NAME);
	}
#else
	pxDisk = FF_RAMDiskInit( mainRAM_DISK_NAME, ucRAMDisk, mainRAM_DISK_SECTORS, mainIO_MANAGER_CACHE_SIZE );
	configASSERT( pxDisk );
	memcpy(ucRAMDisk, backup, mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE);
	read(STDIN_FILENO, ucRAMDisk, fuzz_read);

	FuzzOperations(mainRAM_DISK_NAME);

#endif
	exit(0);
	/* Create a few example files on the disk.  These are not deleted again. */
	// vCreateAndVerifyExampleFiles( mainRAM_DISK_NAME );
	// FF_FILE * fd;
	// ff_chdir(mainRAM_DISK_NAME);
	// fd = ff_fopen("log1","w");
	// ff_fprintf(fd, "This is inner text of log1 in %s\n", mainRAM_DISK_NAME);
	// ff_fclose(fd);
	// fd = NULL;
	// fd = ff_fopen("log1","r");
	// ff_fclose(fd);
	// fd = NULL;
	// fd = ff_fopen("log1","w");
	// ff_fprintf(fd, "This is inner text of log1 in %s\n", mainRAM_DISK_NAME);
	// ff_fclose(fd);
	// fd = NULL;

	// ff_mkdir(pcDirectory1);
	// ff_chdir(pcDirectory1);
	// fd = ff_fopen("log2","w");
	// ff_fprintf(fd, "This is inner text of log2 in %s\n", pcDirectory1);
	// ff_fclose(fd);
	// fd = NULL;

	// ff_mkdir(pcDirectory2);
	// ff_chdir(pcDirectory2);
	// fd = ff_fopen("log3","w");
	// ff_fprintf(fd, "This is inner text of log3 in %s\n", pcDirectory2);
	// ff_fclose(fd);
	// fd = NULL;

	// FILE *fp = NULL;
	// fp = fopen("./init.img","wb");
	// fwrite( ucRAMDisk, sizeof(uint8_t), mainRAM_DISK_SECTORS * mainRAM_DISK_SECTOR_SIZE, fp );
	// fclose(fp);
	// fp = NULL;
	// printf("Task will delete itself\n");
	vTaskDelete(NULL);
}

void FuzzOperations(const char *Dir)
{
int32_t lResult = -1;
char cwd[50] = {0x0};
FF_FindData_t pxFindStruct;
FF_Stat_t fileStat;
const char dirTemp[] = {"temp"};
const char fileTemp[] = {"log.temp"};
const char fileNewName[] = {"log.temp.new"};
FF_FILE *fd;

	lResult = ff_chdir( Dir );
	if(lResult >= 0){
		ff_getcwd( cwd, sizeof(cwd) );
		/* /ram */
		fileRead("log1");
		fileWrite("log1");

		ff_mkdir(dirTemp);
		ff_rmdir(dirTemp);

		fd = ff_fopen(fileTemp,"w");
		ff_fclose(fd);
		ff_rename(fileTemp,fileNewName, pdFALSE);
		ff_remove(fileNewName);
		ff_rename(fileTemp,fileNewName, pdTRUE);
		/* /ram/SUB1 */
		lResult = ff_chdir(pcDirectory1);
		if(lResult >= 0){
			fileRead("log2");
			fileWrite("log2");

			ff_mkdir(dirTemp);
			ff_rmdir(dirTemp);

			fd = ff_fopen(fileTemp,"w");
			ff_fclose(fd);
			ff_rename(fileTemp,fileNewName, pdFALSE);
			ff_remove(fileNewName);
			ff_rename(fileTemp,fileNewName, pdTRUE);
			/* /ram/SUB2 */
			lResult = ff_chdir(pcDirectory2);
			if(lResult >= 0){
				fileRead("log3");
				fileWrite("log3");

				ff_mkdir(dirTemp);
				ff_rmdir(dirTemp);

				fd = ff_fopen(fileTemp,"w");
				ff_fclose(fd);
				ff_rename(fileTemp,fileNewName, pdFALSE);
				ff_remove(fileNewName);
				ff_rename(fileTemp,fileNewName, pdTRUE);
			}
		}
	}	
}

void fileRead(const char *fileName )
{
int lResult = -1;
FF_FILE *fd;
const int OFFSET = 11;
char readBuf[20];
/* open file and get attr */
	fd = ff_fopen( fileName, "r" );
	if( fd != NULL ){
		ff_filelength( fd );

		/* normal read string */
		// ff_fread( readBuf, sizeof(char), OFFSET, fd );
		// ff_fread( readBuf, sizeof(char), sizeof(readBuf), fd);
		/* move pointer position */
		ff_fseek( fd, OFFSET, FF_SEEK_SET );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fgetc( fd );
		ff_fread( readBuf, sizeof(char), OFFSET, fd );
		ff_fread( readBuf, sizeof(char), sizeof(readBuf), fd);

		ff_fseek( fd, OFFSET, FF_SEEK_CUR );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fgetc( fd );
		ff_fread( readBuf, sizeof(char), OFFSET, fd );
		ff_fread( readBuf, sizeof(char), sizeof(readBuf), fd);

		ff_fseek( fd, OFFSET, FF_SEEK_END );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fgetc( fd );
		ff_fread( readBuf, sizeof(char), OFFSET, fd );
		ff_fread( readBuf, sizeof(char), sizeof(readBuf), fd);

		ff_rewind( fd );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fgetc( fd );
		ff_fread( readBuf, sizeof(char), OFFSET, fd );
		ff_fread( readBuf, sizeof(char), sizeof(readBuf), fd);
		ff_fclose( fd );
	}
}
void fileWrite( const char *fileName)
{
int lResult = -1;
FF_FILE *fd = NULL;
const int OFFSET = 11;
char writeBuf[20] = {0x0};

	fd = ff_fopen( fileName, "w" );
	if( fd != NULL ){
		ff_fseek( fd, OFFSET, FF_SEEK_SET );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fseek( fd, OFFSET, FF_SEEK_CUR );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fseek( fd, OFFSET, FF_SEEK_END );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_rewind( fd );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fclose(fd);
	}
	/* append */
	fd = ff_fopen( fileName, "a" );
	if( fd != NULL ){
		ff_fseek( fd, OFFSET, FF_SEEK_SET );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fseek( fd, OFFSET, FF_SEEK_CUR );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fseek( fd, OFFSET, FF_SEEK_END );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_rewind( fd );
		ff_ftell( fd );
		ff_feof( fd );
		ff_fputc( writeBuf[0], fd );
		ff_fwrite( writeBuf, sizeof(char), OFFSET, fd );
		ff_fwrite( writeBuf, sizeof(char), sizeof(writeBuf), fd );
		ff_fprintf( fd, "%s", writeBuf );

		ff_fclose(fd);
	}
	/* truncate */
	fd = ff_truncate( fileName, OFFSET );
	if( fd != NULL ){
		ff_fclose( fd );
	}
}

void vCreateAndVerifyExampleFiles( const char *pcMountPath )
{
	/* Create and verify a few example files using both line based and character
	based reads and writes. */
	prvCreateDemoFilesUsing_ff_fwrite( pcMountPath );
	prvVerifyDemoFileUsing_ff_fread();
	prvCreateDemoFileUsing_ff_fputc( pcMountPath );
	prvVerifyDemoFileUsing_ff_fgetc( pcMountPath );
}

static void prvCreateDemoFilesUsing_ff_fwrite( const char *pcMountPath )
{
BaseType_t xFileNumber, xWriteNumber;
const BaseType_t xMaxFiles = 5;
int32_t lItemsWritten;
int32_t lResult;
FF_FILE *pxFile;
char *pcRAMBuffer, *pcFileName;

	/* Allocate buffers used to hold date written to/from the disk, and the
	file names. */
	pcRAMBuffer = ( char * ) pvPortMalloc( fsRAM_BUFFER_SIZE );
	pcFileName = ( char * ) pvPortMalloc( ffconfigMAX_FILENAME );
	configASSERT( pcRAMBuffer );
	configASSERT( pcFileName );

	/* Ensure in the root of the mount being used. */
	lResult = ff_chdir( pcMountPath );
	configASSERT( lResult >= 0 );

	/* Create xMaxFiles files.  Each created file will be
	( xFileNumber * fsRAM_BUFFER_SIZE ) bytes in length, and filled
	with a different repeating character. */
	for( xFileNumber = 1; xFileNumber <= xMaxFiles; xFileNumber++ )
	{
		/* Generate a file name. */
		snprintf( pcFileName, ffconfigMAX_FILENAME, "root%03d.txt", ( int ) xFileNumber );

		/* Obtain the current working directory and print out the file name and
		the	directory into which the file is being written. */
		ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
		FF_PRINTF( "Creating file %s in %s\n", pcFileName, pcRAMBuffer );

		/* Open the file, creating the file if it does not already exist. */
		pxFile = ff_fopen( pcFileName, "w" );
		configASSERT( pxFile );

		/* Fill the RAM buffer with data that will be written to the file.  This
		is just a repeating ascii character that indicates the file number. */
		memset( pcRAMBuffer, ( int ) ( '0' + xFileNumber ), fsRAM_BUFFER_SIZE );

		/* Write the RAM buffer to the opened file a number of times.  The
		number of times the RAM buffer is written to the file depends on the
		file number, so the length of each created file will be different. */
		for( xWriteNumber = 0; xWriteNumber < xFileNumber; xWriteNumber++ )
		{
			lItemsWritten = ff_fwrite( pcRAMBuffer, fsRAM_BUFFER_SIZE, 1, pxFile );
			configASSERT( lItemsWritten == 1 );
		}

		/* Close the file so another file can be created. */
		ff_fclose( pxFile );
	}

	vPortFree( pcRAMBuffer );
	vPortFree( pcFileName );
}
/*-----------------------------------------------------------*/

static void prvVerifyDemoFileUsing_ff_fread( void )
{
BaseType_t xFileNumber, xReadNumber;
const BaseType_t xMaxFiles = 5;
size_t xItemsRead, xChar;
FF_FILE *pxFile;
char *pcRAMBuffer, *pcFileName;

	/* Allocate buffers used to hold date written to/from the disk, and the
	file names. */
	pcRAMBuffer = ( char * ) pvPortMalloc( fsRAM_BUFFER_SIZE );
	pcFileName = ( char * ) pvPortMalloc( ffconfigMAX_FILENAME );
	configASSERT( pcRAMBuffer );
	configASSERT( pcFileName );

	/* Read back the files that were created by
	prvCreateDemoFilesUsing_ff_fwrite(). */
	for( xFileNumber = 1; xFileNumber <= xMaxFiles; xFileNumber++ )
	{
		/* Generate the file name. */
		snprintf( pcFileName, ffconfigMAX_FILENAME, "root%03d.txt", ( int ) xFileNumber );

		/* Obtain the current working directory and print out the file name and
		the	directory from which the file is being read. */
		ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
		FF_PRINTF( "Reading file %s from %s\n", pcFileName, pcRAMBuffer );

		/* Open the file for reading. */
		pxFile = ff_fopen( pcFileName, "r" );
		configASSERT( pxFile );

		/* Read the file into the RAM buffer, checking the file contents are as
		expected.  The size of the file depends on the file number. */
		for( xReadNumber = 0; xReadNumber < xFileNumber; xReadNumber++ )
		{
			/* Start with the RAM buffer clear. */
			memset( pcRAMBuffer, 0x00, fsRAM_BUFFER_SIZE );

			xItemsRead = ff_fread( pcRAMBuffer, fsRAM_BUFFER_SIZE, 1, pxFile );
			configASSERT( xItemsRead == 1 );

			/* Check the RAM buffer is filled with the expected data.  Each
			file contains a different repeating ascii character that indicates
			the number of the file. */
			for( xChar = 0; xChar < fsRAM_BUFFER_SIZE; xChar++ )
			{
				configASSERT( pcRAMBuffer[ xChar ] == ( '0' + ( char ) xFileNumber ) );
			}
		}

		/* Close the file. */
		ff_fclose( pxFile );
	}

	vPortFree( pcRAMBuffer );
	vPortFree( pcFileName );

	/*_RB_ also test what happens when attempting to read using too large item
	sizes, etc. */
}
/*-----------------------------------------------------------*/

static void prvCreateDemoFileUsing_ff_fputc( const char *pcMountPath )
{
int32_t iReturn, iByte, iReturned;
FF_FILE *pxFile;
char *pcRAMBuffer, *pcFileName;

	/* Allocate buffers used to hold date written to/from the disk, and the
	file names. */
	pcRAMBuffer = ( char * ) pvPortMalloc( fsRAM_BUFFER_SIZE );
	pcFileName = ( char * ) pvPortMalloc( ffconfigMAX_FILENAME );
	configASSERT( pcRAMBuffer );
	configASSERT( pcFileName );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "In directory %s\n", pcRAMBuffer );

	/* Create a sub directory. */
	iReturn = ff_mkdir( pcDirectory1 );
	configASSERT( iReturn == pdFREERTOS_ERRNO_NONE );

	/* Move into the created sub-directory. */
	iReturn = ff_chdir( pcDirectory1 );
	configASSERT( iReturn == pdFREERTOS_ERRNO_NONE );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "In directory %s\n", pcRAMBuffer );

	/* Create a subdirectory in the new directory. */
	iReturn = ff_mkdir( pcDirectory2 );
	configASSERT( iReturn == pdFREERTOS_ERRNO_NONE );

	/* Move into the directory just created - now two directories down from
	the root. */
	iReturn = ff_chdir( pcDirectory2 );
	configASSERT( iReturn == pdFREERTOS_ERRNO_NONE );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "In directory %s\n", pcRAMBuffer );
	snprintf( pcFileName, ffconfigMAX_FILENAME, "%s%s", pcMountPath, pcFullPath );
	configASSERT( strcmp( pcRAMBuffer, pcFileName ) == 0 );

	/* Generate the file name. */
	snprintf( pcFileName, ffconfigMAX_FILENAME, "%s.txt", pcDirectory2 );

	/* Print out the file name and the directory into which the file is being
	written. */
	FF_PRINTF( "Writing file %s in %s\n", pcFileName, pcRAMBuffer );

	pxFile = ff_fopen( pcFileName, "w" );
	configASSERT( pxFile );

	/* Create a file 1 byte at a time.  The file is filled with incrementing
	ascii characters starting from '0'. */
	for( iByte = 0; iByte < fsPUTC_FILE_SIZE; iByte++ )
	{
		iReturned = ff_fputc( ( ( int ) '0' + iByte ), pxFile );
		configASSERT( iReturned ==  ( ( int ) '0' + iByte ) );
	}

	/* Finished so close the file. */
	ff_fclose( pxFile );

	/* Move back to the root directory. */
	iReturned = ff_chdir( "../.." );
	configASSERT( iReturn == pdFREERTOS_ERRNO_NONE );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "Back in root directory %s\n", pcRAMBuffer );
	configASSERT( strcmp( pcRAMBuffer, pcMountPath ) == 0 );

	vPortFree( pcRAMBuffer );
	vPortFree( pcFileName );
}
/*-----------------------------------------------------------*/

static void prvVerifyDemoFileUsing_ff_fgetc( const char *pcMountPath )
{
int iByte, iReturned;
FF_FILE *pxFile;
char *pcRAMBuffer, *pcFileName;

	/* Allocate buffers used to hold date written to/from the disk, and the
	file names. */
	pcRAMBuffer = ( char * ) pvPortMalloc( fsRAM_BUFFER_SIZE );
	pcFileName = ( char * ) pvPortMalloc( ffconfigMAX_FILENAME );
	configASSERT( pcRAMBuffer );
	configASSERT( pcFileName );

	/* Move into the directory in which the file was created. */
	snprintf( pcFileName, ffconfigMAX_FILENAME, "%s%s", pcMountPath, pcFullPath );
	iReturned = ff_chdir( pcFileName );
	configASSERT( iReturned == pdFREERTOS_ERRNO_NONE );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "Back in directory %s\n", pcRAMBuffer );
	configASSERT( strcmp( pcRAMBuffer, pcFileName ) == 0 );

	/* pcFileName is about to be overwritten - take a copy. */
	strcpy( pcRAMBuffer, pcFileName );

	/* Generate the file name. */
	sprintf( pcFileName, "%s.txt", pcDirectory2 );

	/* Print out the file name and the directory from which the file is being
	read. */
	FF_PRINTF( "Reading file %s in %s\n", pcFileName, pcRAMBuffer );

	/* This time the file is opened for reading. */
	pxFile = ff_fopen( pcFileName, "r" );

	/* Read the file 1 byte at a time. */
	for( iByte = 0; iByte < fsPUTC_FILE_SIZE; iByte++ )
	{
		iReturned = ff_fgetc( pxFile );
		configASSERT( iReturned ==  ( ( int ) '0' + iByte ) );
	}

	/* Should not be able to read another bytes. */
	iReturned = ff_fgetc( pxFile );
	configASSERT( iReturned ==  FF_EOF );

	/* Finished so close the file. */
	ff_fclose( pxFile );

	/* Move back to the root directory. */
	iReturned = ff_chdir( "../.." );

	/* Obtain and print out the working directory. */
	ff_getcwd( pcRAMBuffer, fsRAM_BUFFER_SIZE );
	FF_PRINTF( "Back in root directory %s\n", pcRAMBuffer );

	vPortFree( pcRAMBuffer );
	vPortFree( pcFileName );
}


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	printf("malloc error!!!\n");
}