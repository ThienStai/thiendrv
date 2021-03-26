#pragma once
#include <Windows.h>
#include "defines.hpp"

class KernelInterface {
public:
	HANDLE hDriver;
	KernelInterface(LPCSTR RegPath) {
		hDriver = CreateFileA(
			RegPath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			0,
			0
		);
	}

	DWORD GetMWBaseAddress() {
		if (hDriver == INVALID_HANDLE_VALUE) return -1;
		ULONG Address;
		DWORD Bytes;
		BOOL stat = DeviceIoControl(hDriver, IO_GETMW_BASEADDR, &Address, sizeof(Address), &Address, sizeof(Address), &Bytes, nullptr);
		if (stat) return Address;
		return -1;
	}
	template <typename type>
	type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress) {
		type buffer;
		READ_REQUEST_DATA ReadRequest;
		ReadRequest.ProcessID = ProcessId;
		ReadRequest.Address = ReadAddress;
		ReadRequest.pBuff = &buffer;
		ReadRequest.Size = sizeof(type);
		DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0);
		return buffer;
	}

	template <typename type>
	type WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress, type WriteValue) {
		WRITE_REQUEST_DATA WriteRequest;
		DWORD Bytes;
		WriteRequest.ProcessID = ProcessId;
		WriteRequest.Address = WriteAddress;
		WriteRequest.pBuff = &WriteValue;
		WriteRequest.Size = sizeof(type);
		DeviceIoControl(hDriver, IO_READ_REQUEST, &WriteRequest, sizeof(WriteRequest), 0, 0, &Bytes, NULL);
	}
};