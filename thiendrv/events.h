#pragma once
#include <ntddk.h>
#include "data.h"
PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessCreationCallback(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo);
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadcallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);