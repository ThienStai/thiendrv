#pragma warning(disable : 6031)
#include <Windows.h>
#include <iostream>
#include <cstdio>
#include "kernelinterface.hpp"
#include "mem.h"
#include "proc.h"

int main()
{
	KernelInterface thiendrv = KernelInterface(L"\\\\.\\thiendrv");
	ULONG input = 0;
	while (true) {
		system("cls");
		printf("Enter option:\r\n");
		printf("0. Exit \r\n");
		printf("1. Kill process\r\n");
		printf("2. Hide process\r\n");
		printf("3. Shutdown/Reboot/Power off\r\n>> ");
		scanf("%uld", &input);
		if (!input) {
			return 0;
		}
		if (input == 1) {
			printf("\r\nEnter process id to kill or 0 to exit:\r\n>> ");
			scanf("%uld", &input);
			if (input == 0)
				continue;
			thiendrv.KillProcess(input);
		}
		else if (input == 2) {
			printf("\r\nEnter process id to hide or 0 to exit\r\n>> ");
			scanf("%uld", &input);
			if (input == 0)
				continue;
			thiendrv.HideProcess(input);
		}
		else if (input == 3) {
			printf("\r\nChoose an option:\r\n");
			printf("\r\n0. Return to menu");
			printf("\r\n1. Shutdown");
			printf("\r\n2. Reboot");
			printf("\r\n3. Poweroff");
			scanf("%uld", &input);
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
				printf("Invalid option!\r\n");
				Sleep(1000);
				continue;
			}
		}
		else {
			printf("Invalid option!\r\n");
			Sleep(1000);
			continue;
		}
	}

}
