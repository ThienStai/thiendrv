#pragma warning(disable: 4100 4022)
#include "ioctl.h"
#include "defines.hpp"
#include "memory.h"
NTSTATUS CreateCall(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(Irp);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	DbgPrintEx(0, 0, "CreateCall is called, connection established");
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;

}

NTSTATUS CloseCall(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(Irp);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	DbgPrintEx(0, 0, "Connecton terminated!");
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;

}

NTSTATUS IoControl(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(Irp);

	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	ULONG ByteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;


	switch (ControlCode) {
		//Request miniworld base addr
	case IO_GETMW_BASEADDR: {
		PULONG Output = (PULONG)Irp->AssociatedIrp.SystemBuffer;
		*Output = libiworld_micro_ADDR;
		DbgPrintEx(0, 0, "Replied to libiworld_micro_ADDR request!\n");
		Status = STATUS_SUCCESS;
		ByteIO = sizeof(*Output);
		break;
	}

						  //request read
	case IO_READ_REQUEST: {
		PREAD_REQUEST_DATA ReadInput = (PREAD_REQUEST_DATA)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ReadInput->ProcessID, &Process))) {
			if NT_SUCCESS(KernelReadProcessMemory(Process, (PVOID)ReadInput->Address, (PVOID)ReadInput->pBuff, ReadInput->Size)) {
				Status = STATUS_SUCCESS;
				ByteIO = sizeof(READ_REQUEST_DATA);
			}
			else break;
		}
		else break;
	}

						//request write
	case IO_WRITE_REQUEST: {
		PWRITE_REQUEST_DATA WriteInput = (PWRITE_REQUEST_DATA)Irp->AssociatedIrp.SystemBuffer;
		PEPROCESS Process;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)WriteInput->ProcessID, &Process))) {
			if NT_SUCCESS(KernelWriteProcessMemory(Process, WriteInput->pBuff, WriteInput->Address, WriteInput->Size)) {
				Status = STATUS_SUCCESS;
				ByteIO = sizeof(WRITE_REQUEST_DATA);
			}
			else break;
		}
		else break;

	}
	default:
		ByteIO = 0;
		break;
	}

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = ByteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}
