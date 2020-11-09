/*++

Conexant CX25878(a.k.a.Bt878) GPIO Driver

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#define INITGUID

#include <ntddk.h>
#include <wdf.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD Bt878GpioSampleEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP Bt878GpioSampleEvtDriverContextCleanup;

//
// Bt878 レジスタアドレス
//

#define INT_MASK		0x104
#define	GPIO_DMA_CTL	0x10C
#define	GPIO_OUT_EN		0x118
#define	GPIO_DATA		0x200