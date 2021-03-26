#include <Windows.h>
#include <iostream>
#include <cstdio>
#include "kernelinterface.hpp"
#include "mem.h"
#include "proc.h"
void print_ivec3(ivec3 vec) {
	printf("X: %6ld Y: %6ld Z: *6ld", vec.x, vec.y, vec.z);
}
int main()
{
	KernelInterface thiendrv = KernelInterface("\\\\.\\thiendrv");
	DWORD BaseAddress = GetModuleBaseAddress(GetProcId(L"iworldpc3.exe"), L"libiworld_micro.dll");
	ivec3 PlayerCoord;
	READ_REQUEST_DATA playerPosRequest;

	//playerPosRequest.Address = mem::FindDMAAddy(BaseAddress, { 0x666 });
	//playerPosRequest.pBuff = pPlayer;
	//playerPosRequest.ProcessID = GetProcId(L"iworldpc3.exe");
	//playerPosRequest.Size = sizeof(ivec3);
	PlayerCoord = thiendrv.ReadVirtualMemory<ivec3>(
		GetProcId(L"iworldpc3.exe"),
		mem::FindDMAAddy(BaseAddress, { 0x666 })
		);
	while (true) {
		//print_ivec3();
	}

}
