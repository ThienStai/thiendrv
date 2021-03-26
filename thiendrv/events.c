#pragma once
#pragma warning(disable : 4100 6273 4047)
#include "events.h"
#include "data.h"
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadcallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo)
{
	if (wcsstr(FullImageName->Buffer, L"libiworld_micro.dll")) {
		DbgPrintEx(0, 0, "Miniworld's main dll found!!!\n");
		DbgPrintEx(0, 0, "PID=%ud\nBase address=0x%x\nSize=0x%x\n", (void*)ProcessId, ImageInfo->ImageBase, (DWORD32)ImageInfo->ImageSize);
		libiworld_micro_ADDR = ImageInfo->ImageBase;
	}
	return STATUS_SUCCESS;

}
PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessCreationCallback(
	PEPROCESS Process,
	HANDLE ProcessId,
	PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	if (CreateInfo == NULL) {
		DbgPrintEx(0, 0, "Process PID=%d exiting\n", (void*)ProcessId);
	}
	else {
		DbgPrintEx(0, 0, "Process PID=%d created\n", (void*)ProcessId);
	}
	return 0;
}


