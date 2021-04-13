#include "memory.h"
NTSTATUS KernelReadProcessMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	PEPROCESS SourceProcess = Process;
	PEPROCESS TargetProcess = PsGetCurrentProcess();
	SIZE_T Result;
	if (NT_SUCCESS(MmCopyVirtualMemory(SourceProcess, SourceAddress, TargetProcess, TargetAddress, Size, KernelMode, &Result)))
		return STATUS_SUCCESS; // operation was successful
	else
		return STATUS_ACCESS_DENIED;
}

NTSTATUS KernelWriteProcessMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{

	PEPROCESS SourceProcess = PsGetCurrentProcess();
	PEPROCESS TargetProcess = Process;
	SIZE_T Result;

	if (NT_SUCCESS(MmCopyVirtualMemory(SourceProcess, SourceAddress, TargetProcess, TargetAddress, Size, KernelMode, &Result)))
		return STATUS_SUCCESS;
	else
		return STATUS_ACCESS_DENIED;

}
