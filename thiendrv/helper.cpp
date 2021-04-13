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
EXTERN_C NTSTATUS OpenProcess(PHANDLE hProc, ULONG ProcId) {
	OBJECT_ATTRIBUTES procAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(0, OBJ_KERNEL_HANDLE);
	NTSTATUS stat = STATUS_UNSUCCESSFUL;
	CLIENT_ID pid{};
	pid.UniqueProcess = UlongToHandle(ProcId);
	if (NT_SUCCESS(stat = ZwOpenProcess(hProc, PROCESS_ALL_ACCESS, &procAttributes, &pid))) {
		stat = STATUS_SUCCESS;
		DbgPrintEx(0, 0, "Opened process with PID=%ul",ProcId);
	} else DbgPrintEx(0, 0, "Failed to open process with exit code = %x", stat);

	return stat;
}
