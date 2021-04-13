#pragma once
//#pragma warning(disable : 4100 6273 4047 4002 4024)
#include "events.h"
#include "data.h"
#include "defines.h"
#include "helper.h"

void  ImageLoadcallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo)
{

}
void ProcessCreationCallback(
	PEPROCESS Process,
	HANDLE ProcessId,
	PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	if (CreateInfo == NULL) {
		DbgPrintEx(0, 0, "Process with PID=%ld exiting", (void*)ProcessId);
	}
	else {
		DbgPrintEx(0, 0, "Process %wZ with PID=%ld PPID=%ld created", CreateInfo->ImageFileName, (void*)ProcessId, CreateInfo->ParentProcessId);
	}
}
void ProcessCreationCallbackForbidden(
	PEPROCESS Process,
	HANDLE ProcessId,
	PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	CreateInfo->CreationStatus = STATUS_ACCESS_DENIED;
}


