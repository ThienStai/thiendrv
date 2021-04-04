#pragma warning(disable: 4100 4022 4311 4002 6064 4047 4024)
#include "ioctl.h"
#include "defines.h"
#include "memory.h"
#include <ntifs.h>
#include "iohandle.h"
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
	DbgPrintEx(0, 0, "Connection terminated!");
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;

}

NTSTATUS IoControl(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(Irp);

	NTSTATUS Status = STATUS_SUCCESS;
	ULONG ByteIO = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;


	switch (ControlCode) {
	case IO_GETMW_BASEADDR: {
		HandleMWBaseAddrRequest(Irp, &Status, &ByteIO, libiworld_micro_ADDR);
		break;
	}
	case IO_READ_REQUEST: {
		PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeReadRequest(&Status, &ByteIO, ReadInput);
		break;
	}
	case IO_WRITE_REQUEST: {
		PKERNEL_WRITE_REQUEST ReadInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeWriteRequest(&Status, &ByteIO, ReadInput);
		break;
	}
	case IO_KILL_PROCESS: {
		PKERNEL_KILL_PROCESS_REQUEST ReadInput = (PKERNEL_KILL_PROCESS_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeKillProcessRequest(&Status, ReadInput);
		break;
	}
	case IO_HIDE_PROCESS: {
		PKERNEL_HIDE_PROCESS_REQUEST ReadInput = (PKERNEL_HIDE_PROCESS_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeHideProcessRequest(ReadInput);
		break;
	}



	case IO_SYS_SHUTDOWN: {
		DbgPrintEx(0, 0, "Shutdown system!");
		NtShutdownSystem(ShutdownNoReboot);
		break;
	}
	case IO_SYS_REBOOT: {
		DbgPrintEx(0, 0, "Reboot system!");
		NtShutdownSystem(ShutdownReboot);
		break;
	}
	case IO_SYS_POWEROFF: {
		DbgPrintEx(0, 0, "Powwr off system!");
		NtShutdownSystem(ShutdownPowerOff);
		break;
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
