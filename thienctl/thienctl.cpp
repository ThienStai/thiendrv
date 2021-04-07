//#pragma warning(disable : 6031)
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "kernelinterface.hpp"
#include "mem.h"
#include "proc.h"
int main()
{
	KernelInterface thiendrv = KernelInterface(L"\\\\.\\thiendrv");
	ULONG input = 0;
	while (true) {
		system("cls");
		printf("\r\nEnter option:\r\n");
		printf("0. Exit \r\n");
		printf("1. Kill process\r\n");
		printf("2. Hide process\r\n");
		printf("3. Set process as 'critical'\r\n");
		printf("4. Clear process 'critical' status\r\n");
		printf("5. Lockdown process creation\r\n");
		printf("6. Unlock process creation\r\n");
		printf("7. Shutdown/Reboot/Power off\r\n>> ");
		std::cin >> input; //avoid buggy scanf
		if (!input) {
			return 0;
		}
		if (input == 1) {
			system("cls");
			printf("\r\nEnter process id to kill or 0 to exit:\r\n>> ");
			std::cin >> input;
			if (input == 0)
				continue;
			thiendrv.KillProcess(input);
		}
		else if (input == 2) {
			system("cls");
			printf("\r\nEnter process id to hide or 0 to exit\r\n>> ");
			std::cin >> input;
			if (input == 0)
				continue;
			thiendrv.HideProcess(input);
		}
		else if (input == 3) {
			system("cls");
			printf("\r\nEnter process id to set critical or 0 to exit\r\n>> ");
			std::cin >> input;
			if (input == 0)
				continue;
			thiendrv.SetCriticalStatus(input);
		}
		else if (input == 4) {
			system("cls");
			printf("\r\nEnter process id to clear critical or 0 to exit\r\n>> ");
			std::cin >> input;
			if (input == 0)
				continue;
			thiendrv.DeleteCriticalStatus(input);
		}
		else if (input == 5) {
			thiendrv.LockdownProcessCreation();
		}
		else if (input == 5) {
			thiendrv.UnlockProcessCreation();
		}
		else if (input == 7) {
			system("cls");
			printf("\r\nChoose an option:\r\n");
			printf("\r\n0. Return to menu");
			printf("\r\n1. Shutdown");
			printf("\r\n2. Reboot");
			printf("\r\n3. Poweroff\r\n>> ");
			std::cin >> input;
			if (input == 0)
				continue;
			switch (input) {
			case 1:
				thiendrv.ShutdownSystem();
				break;
			case 2:
				thiendrv.RebootSystem();
				break;
			case 3:
				thiendrv.PowerOffSystem();
				break;
			default:
				MessageBoxExW(GetForegroundWindow(), L"Invalid option!", L"thiendrv", MB_ICONERROR | MB_SYSTEMMODAL, 0);
				Sleep(1000);
				continue;
			}
		}
		else {
			MessageBoxExW(GetForegroundWindow(), L"Invalid option!", L"thiendrv", MB_ICONERROR | MB_SYSTEMMODAL, 0);
			Sleep(1000);
			continue;
		}
	}

}
