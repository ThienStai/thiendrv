#pragma once
#include <ntddk.h>
#include <ntstatus.h>
ULONG find_eprocess_pid_offset();
EXTERN_C NTSTATUS OpenProcess(PHANDLE hProc, ULONG ProcId);
