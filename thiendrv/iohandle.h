#pragma once
#include <ntddk.h>
#include "defines.h"
#include "memory.h"
#include "process.h"
NTSTATUS HandleMWBaseAddrRequest(PIRP Irp, PNTSTATUS Status, PULONG ByteIO, ULONG mw_addr);
NTSTATUS HandleKeReadRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_READ_REQUEST ReadInput);
NTSTATUS HandleKeWriteRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_WRITE_REQUEST ReadInput);
NTSTATUS HandleKeKillProcessRequest(PNTSTATUS Status, PKERNEL_KILL_PROCESS_REQUEST ReadInput);

NTSTATUS HandleKeHideProcessRequest(PKERNEL_HIDE_PROCESS_REQUEST ReadInput);
void remove_links(PLIST_ENTRY Current);

ULONG find_eprocess_pid_offset();