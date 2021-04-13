#pragma once
#include <ntddk.h>
#include "defines.h"
#include "memory.h"
EXTERN_C NTSTATUS HandleKeReadRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_READ_REQUEST ReadInput);
EXTERN_C NTSTATUS HandleKeWriteRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_WRITE_REQUEST ReadInput);
EXTERN_C NTSTATUS HandleKeKillProcessRequest(PNTSTATUS Status, PKERNEL_KILL_PROCESS_REQUEST ReadInput);
EXTERN_C NTSTATUS HandleKeHideProcessRequest(PKERNEL_HIDE_PROCESS_REQUEST ReadInput);

void remove_links(PLIST_ENTRY Current);


//prototype
EXTERN_C NTSTATUS NTAPI NtSetInformationProcess(HANDLE, ULONG, PVOID, ULONG);
EXTERN_C NTSTATUS HandleSetProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput);
EXTERN_C NTSTATUS HandleDeleteProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput);
EXTERN_C NTSTATUS HandleLockdownProcessCreation(PNTSTATUS Status);
EXTERN_C NTSTATUS HandleUnlockProcessCreation(PNTSTATUS Status);

EXTERN_C ULONG find_eprocess_pid_offset();

