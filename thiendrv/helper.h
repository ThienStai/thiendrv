#pragma once
#include <ntddk.h>
ULONG find_eprocess_pid_offset();
NTSTATUS OpenProcess(PHANDLE hProc, ULONG ProcId);
