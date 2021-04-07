#pragma once
#include <ntddk.h>
#include "data.h"
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
);

PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessCreationCallbackForbidden(
	PEPROCESS Process,
	HANDLE ProcessId,
	PPS_CREATE_NOTIFY_INFO CreateInfo
);

PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessCreationCallback(
	PEPROCESS Process,
	HANDLE ProcessId,
	PPS_CREATE_NOTIFY_INFO CreateInfo
);


