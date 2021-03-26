#pragma once
#include <ntddk.h>

#include "data.h"

NTSTATUS IoControl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

NTSTATUS CloseCall(PDEVICE_OBJECT pDevObj, PIRP pIrp);

NTSTATUS CreateCall(PDEVICE_OBJECT pDevObj, PIRP pIrp);
