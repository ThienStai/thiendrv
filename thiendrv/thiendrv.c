#pragma warning(disable : 4100 4047 4024 4133 6011)
#pragma once
#include "thiendrv.h"
#include "events.h"
#include "defines.hpp"
#include "ioctl.h"
PDEVICE_OBJECT pDeviceObject;
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegPath)
{
	UNREFERENCED_PARAMETER(pDriverObj);
	UNREFERENCED_PARAMETER(pRegPath);
	pDriverObj->DriverUnload = UnloadDriver;
	DbgPrintEx(0, 0, "Started thiendrv kernel driver.\n");
	PsSetLoadImageNotifyRoutineEx(ImageLoadcallback, 0);
	PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallback, 0);
	RtlInitUnicodeString(&dev, L"\\Device\\thiendrv");
	RtlInitUnicodeString(&dos, L"\\DosDevice\\thiendrv");
	IoCreateDevice(pDriverObj, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&dos, &dev);

	pDriverObj->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}



NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObj)
{
	DbgPrintEx(0, 0, "Im leaving...\n");

	//cleanup or it will BSOD
	PsSetCreateProcessNotifyRoutineEx(ProcessCreationCallback, 1);
	PsRemoveLoadImageNotifyRoutine(ImageLoadcallback);
	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObj->DeviceObject);
	return STATUS_SUCCESS;

}