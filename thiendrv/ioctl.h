#pragma once
#include <ntddk.h>

NTSTATUS IoControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);


NTSTATUS CreateCloseCall(PDEVICE_OBJECT pDevObj, PIRP pIrp);
