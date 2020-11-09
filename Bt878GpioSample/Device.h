/*++

Conexant CX25878(a.k.a.Bt878) GPIO Driver

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder
	PHYSICAL_ADDRESS	PA_BtCard;
	PUCHAR				BtRegsBase;
	ULONG				BtMemLength;

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// WDFDRIVER Events
//
EVT_WDF_DEVICE_PREPARE_HARDWARE	Bt878GpioSampleEvtDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE	Bt878GpioSampleEvtDeviceReleaseHardware;
EVT_WDF_DEVICE_D0_ENTRY	Bt878GpioSampleEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT	Bt878GpioSampleEvtDeviceD0Exit;
EVT_WDF_DEVICE_FILE_CREATE Bt878GpioSampleEvtDeviceFileCreate;
EVT_WDF_FILE_CLOSE Bt878GpioSampleEvtFileClose;


//
// Function to initialize the device and its callbacks
//
NTSTATUS
Bt878GpioSampleCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

PCHAR
DbgDevicePowerString(
IN WDF_POWER_DEVICE_STATE Type
);

