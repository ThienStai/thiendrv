#pragma once
#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegPath);

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObj);