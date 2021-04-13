#pragma once
#include <ntddk.h>

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegPath);
extern "C" NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObj);