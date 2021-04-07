#pragma once
#include <ntddk.h>
#include <ntifs.h>
#define NT_DEVICE_NAME      L"\\Device\\thiendrv"
#define DOS_DEVICE_NAME     L"\\DosDevices\\thiendrv"

#define IO_GETMW_BASEADDR                          CTL_CODE(FILE_DEVICE_UNKNOWN,0x21653,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_READ_REQUEST                            CTL_CODE(FILE_DEVICE_UNKNOWN,0x21656,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_REQUEST                           CTL_CODE(FILE_DEVICE_UNKNOWN,0x21657,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_KILL_PROCESS                            CTL_CODE(FILE_DEVICE_UNKNOWN,0x21666,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_HIDE_PROCESS                            CTL_CODE(FILE_DEVICE_UNKNOWN,0x48256,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_SET_CRITICAL_STAT                       CTL_CODE(FILE_DEVICE_UNKNOWN,0x78256,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_DELETE_CRITICAL_STAT                    CTL_CODE(FILE_DEVICE_UNKNOWN,0x18256,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_SYS_SHUTDOWN                            CTL_CODE(FILE_DEVICE_UNKNOWN,0x64646,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_SYS_REBOOT                              CTL_CODE(FILE_DEVICE_UNKNOWN,0x64645,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_SYS_POWEROFF                            CTL_CODE(FILE_DEVICE_UNKNOWN,0x64644,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_LOCKDOWN_PROCESS_CREATION               CTL_CODE(FILE_DEVICE_UNKNOWN,0x66666,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_UNLOCK_PROCESS_CREATION                 CTL_CODE(FILE_DEVICE_UNKNOWN,0x12345,METHOD_BUFFERED, FILE_SPECIAL_ACCESS)


typedef struct _KERNEL_READ_REQUEST {
	ULONG ProcessID, Address, Size;
	PVOID pBuff;
} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;


typedef struct _KERNEL_WRITE_REQUEST {
	ULONG ProcessID, Address, Size;
	PVOID pBuff;
} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;


typedef struct _KERNEL_KILL_PROCESS_REQUEST {
	ULONG ProcId;
} KERNEL_KILL_PROCESS_REQUEST, * PKERNEL_KILL_PROCESS_REQUEST;

typedef struct _KERNEL_HIDE_PROCESS_REQUEST {
	ULONG ProcId;
} KERNEL_HIDE_PROCESS_REQUEST, * PKERNEL_HIDE_PROCESS_REQUEST;

typedef struct _KERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST {
	ULONG ProcId;
} KERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST, * PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST;


typedef enum _SHUTDOWN_ACTION {
	ShutdownNoReboot,
	ShutdownReboot,
	ShutdownPowerOff
} SHUTDOWN_ACTION, * PSHUTDOWN_ACTION;


NTSYSAPI NTSTATUS NTAPI NtShutdownSystem(IN SHUTDOWN_ACTION Action);

//NTSYSAPI NTSTATUS NTAPI NtTerminateProcess(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus);
NTSTATUS ZwQueryInformationProcess(
	_In_      HANDLE           ProcessHandle,
	_In_      PROCESSINFOCLASS ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
);
PVOID PsGetProcessSectionBaseAddress(PEPROCESS Process);

NTSTATUS MmUnmapViewOfSection(PEPROCESS Process, PVOID BaseAddress);

extern  NTSTATUS ZwOpenThread(
	_Out_ PHANDLE ThreadHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PCLIENT_ID ClientId);

extern  NTSTATUS NTAPI ZwQueryInformationProcess(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_Out_writes_bytes_(ProcessInformationLength) PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength,
	_Out_opt_ PULONG ReturnLength
);

extern  NTSTATUS ObOpenObjectByName(
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ POBJECT_TYPE ObjectType,
	_In_ KPROCESSOR_MODE AccessMode,
	_Inout_opt_ PACCESS_STATE AccessState,
	_In_opt_ ACCESS_MASK DesiredAccess,
	_Inout_opt_ PVOID ParseContext,
	_Out_ PHANDLE Handle);

