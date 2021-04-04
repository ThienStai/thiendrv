#pragma once
#include <Windows.h>
#include "defines.hpp"
#include <iostream>
class KernelInterface {
public:
	HANDLE hDriver;
	KernelInterface(LPCWSTR RegPath) {
		hDriver = CreateFileW(RegPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDriver == INVALID_HANDLE_VALUE) {
			std::cout << "Unable to open handle to thiendrv\n";
			std::cout << "CreateFileA return 0x" << std::uppercase << std::hex << hDriver << std::endl;
			throw 0xffffffff;
		}
		std::cout << "Handle to thiendrv opened at 0x" << &hDriver << std::endl;
	}

	DWORD GetMWBaseAddress() {
		if (hDriver == INVALID_HANDLE_VALUE) return -1;
		ULONG Address = 0;
		DWORD Bytes;
		BOOL stat = DeviceIoControl(hDriver, IO_GETMW_BASEADDR, &Address, sizeof(Address), &Address, sizeof(Address), &Bytes, nullptr);
		if (stat) return Address;
		return -1;
	}
	template <typename type>
	type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress) {
		type buffer;
		KERNEL_READ_REQUEST ReadRequest;
		ReadRequest.ProcessID = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.pBuff = &buffer;
		ReadRequest.Size = sizeof(type);
		DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0);
		return buffer;
	}

	template <typename type>
	void WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress, type WriteValue) {
		KERNEL_WRITE_REQUEST WriteRequest;
		DWORD Bytes;
		WriteRequest.ProcessID = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.pBuff = &WriteValue;
		WriteRequest.Size = sizeof(type);
		DeviceIoControl(hDriver, IO_READ_REQUEST, &WriteRequest, sizeof(WriteRequest), 0, 0, &Bytes, NULL);
	}
	void KillProcess(DWORD ProcessId) {
		KERNEL_KILL_PROCESS_REQUEST KillRequest;
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessId);
		if (hProc == INVALID_HANDLE_VALUE) {
			std::cout << "Failed to open handle to process!\n";
			std::cout << "OpenProcess return " << hProc;
			throw 0xffffffff;
		}
		KillRequest.hProcess = hProc;
		KillRequest.ExitCode = 0;
		KillRequest.ProcId = ProcessId;
		DWORD Bytes;
		DeviceIoControl(hDriver, IO_KILL_PROCESS, &KillRequest, sizeof(KillRequest), 0, 0, &Bytes, NULL);
	}
	void HideProcess(ULONG ProcessId) {
		KERNEL_HIDE_PROCESS_REQUEST HideRequest;
		HideRequest.ProcId = ProcessId;
		DeviceIoControl(hDriver, IO_HIDE_PROCESS, &HideRequest, sizeof(HideRequest), 0, 0, 0, 0);

	}
	void ShutdownSystem() {
		DeviceIoControl(hDriver, IO_SYS_SHUTDOWN, NULL, NULL, NULL, NULL, NULL, NULL);
	}
	void RebootSystem() {
		DeviceIoControl(hDriver, IO_SYS_REBOOT, NULL, NULL, NULL, NULL, NULL, NULL);
	}
	void PowerOffSystem() {
		DeviceIoControl(hDriver, IO_SYS_POWEROFF, NULL, NULL, NULL, NULL, NULL, NULL);
	}

};