//#pragma warning(disable: 4100 4022 4311 4002 6064 4047 4024)
#include "ioctl.h"
#include "defines.h"
#include "memory.h"
#include <ntifs.h>
#include "iohandle.h"
NTSTATUS CreateCloseCall(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	auto status = STATUS_SUCCESS;
	auto stack = IoGetCurrentIrpStackLocation(Irp);
	if (stack->MajorFunction == IRP_MJ_CREATE) {
		// verify it's System explorer client (very simple at the moment)
		HANDLE hProcess;
		status = ObOpenObjectByPointer(PsGetCurrentProcess(), OBJ_KERNEL_HANDLE, nullptr, 0, *PsProcessType, KernelMode, &hProcess);
		NT_ASSERT(NT_SUCCESS(status));
		if (NT_SUCCESS(status)) {
			UCHAR buffer[280] = { 0 };
			status = ZwQueryInformationProcess(hProcess, ProcessImageFileName, buffer, sizeof(buffer) - sizeof(WCHAR), nullptr);
			if (NT_SUCCESS(status)) {
				auto path = (UNICODE_STRING*)buffer;
				auto bs = wcsrchr(path->Buffer, L'\\');
				NT_ASSERT(bs);
				if (bs == nullptr || 0 != _wcsicmp(bs, L"\\thienctl.exe"))
				{
					status = STATUS_ACCESS_DENIED;
					DbgPrintEx(0, 0, "Access denied muahahahahahaha");
				}
			}
			ZwClose(hProcess);
		}
	}
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return status;
}

NTSTATUS IoControl(PDEVICE_OBJECT pDevObj, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDevObj);
	UNREFERENCED_PARAMETER(Irp);

	NTSTATUS Status = STATUS_SUCCESS;
	ULONG ByteIO = 0;

	ULONG ControlCode = IoGetCurrentIrpStackLocation(Irp)->Parameters.DeviceIoControl.IoControlCode;


	switch (ControlCode) {
	case IO_READ_REQUEST: {
		auto ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeReadRequest(&Status, &ByteIO, ReadInput);
		break;
	}
	case IO_WRITE_REQUEST: {
		auto ReadInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeWriteRequest(&Status, &ByteIO, ReadInput);
		break;
	}
	case IO_KILL_PROCESS: {
		auto ReadInput = (PKERNEL_KILL_PROCESS_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeKillProcessRequest(&Status, ReadInput);
		break;
	}
	case IO_HIDE_PROCESS: {
		auto ReadInput = (PKERNEL_HIDE_PROCESS_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleKeHideProcessRequest(ReadInput);

		break;
	}
	case IO_SET_CRITICAL_STAT: {
		auto  ReadInput = (PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleSetProcessCriticalStatRequest(&Status, ReadInput);
		break;
	}
	case IO_DELETE_CRITICAL_STAT: {
		auto ReadInput = (PKERNEL_SET_DELETE_CRITICAL_PROCESS_STAT_REQUEST)Irp->AssociatedIrp.SystemBuffer;
		HandleDeleteProcessCriticalStatRequest(&Status, ReadInput);
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
	case IO_LOCKDOWN_PROCESS_CREATION: {
		DbgPrintEx(0, 0, "Lockdown process creation!!!!!!");
		HandleLockdownProcessCreation(&Status);
		break;
	}

	case IO_UNLOCK_PROCESS_CREATION: {
		DbgPrintEx(0, 0, "Unlock process creation!!!!!!");
		HandleLockdownProcessCreation(&Status);
		break;
	}
	default: {
		ByteIO = 0;
		break;
	}

	}
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = ByteIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;

}
