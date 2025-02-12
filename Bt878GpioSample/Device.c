/*++

Conexant CX25878(a.k.a.Bt878) GPIO Driver

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, Bt878GpioSampleCreateDevice)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtDevicePrepareHardware)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtDeviceReleaseHardware)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtDeviceD0Entry)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtDeviceD0Exit)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtDeviceFileCreate)
#pragma alloc_text (PAGE, Bt878GpioSampleEvtFileClose)
#endif


NTSTATUS
Bt878GpioSampleCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES   deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

	WDF_PNPPOWER_EVENT_CALLBACKS		pnpPowerCallbacks;
	WDF_FILEOBJECT_CONFIG	fileConfig;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoDirect);

	//PnPと電源イベント
	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
	pnpPowerCallbacks.EvtDevicePrepareHardware = Bt878GpioSampleEvtDevicePrepareHardware;
	pnpPowerCallbacks.EvtDeviceReleaseHardware = Bt878GpioSampleEvtDeviceReleaseHardware;
	pnpPowerCallbacks.EvtDeviceD0Entry = Bt878GpioSampleEvtDeviceD0Entry;
	pnpPowerCallbacks.EvtDeviceD0Exit = Bt878GpioSampleEvtDeviceD0Exit;
	WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

	//ファイルI/O
	WDF_FILEOBJECT_CONFIG_INIT(
		&fileConfig,
		Bt878GpioSampleEvtDeviceFileCreate,
		Bt878GpioSampleEvtFileClose,
		WDF_NO_EVENT_CALLBACK // not interested in Cleanup
		);
	WdfDeviceInitSetFileObjectConfig(DeviceInit,
		&fileConfig,
		WDF_NO_OBJECT_ATTRIBUTES);



    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = DeviceGetContext(device);

        //
        // Initialize the context.
        //
        deviceContext->PrivateDeviceData = 0;
		deviceContext->BtMemLength = 0;
		deviceContext->BtRegsBase = NULL;

        //
        // Create a device interface so that applications can find and talk
        // to us.
        //
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_Bt878GpioSample,	
            NULL // ReferenceString
            );

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
			KdPrint(("### GUID_DEVINTERFACE_Bt878GpioSample registard. ###\n"));
            status = Bt878GpioSampleQueueInitialize(device);
        }
    }

    return status;
}





NTSTATUS
Bt878GpioSampleEvtDevicePrepareHardware(
WDFDEVICE      Device,
WDFCMRESLIST   ResourcesRaw,
WDFCMRESLIST   ResourcesTranslated
)

{
	//PFDO_DATA   fdoData;
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR descriptor;
	PDEVICE_CONTEXT	deviceContext;

	//fdoData = Bt878GPIOFdoGetData(Device);

	UNREFERENCED_PARAMETER(Device);
	UNREFERENCED_PARAMETER(ResourcesRaw);

	KdPrint(("Bt878GpioSampleEvtDevicePrepareHardware called\n"));

	PAGED_CODE();

	deviceContext = DeviceGetContext(Device);

	for (i = 0; i < WdfCmResourceListGetCount(ResourcesTranslated); i++) {

		descriptor = WdfCmResourceListGetDescriptor(ResourcesTranslated, i);

		switch (descriptor->Type) {

		case CmResourceTypePort:
			KdPrint(("I/O Port: (%x) Length: (%d)\n",
				descriptor->u.Port.Start.LowPart,
				descriptor->u.Port.Length));
			break;

		case CmResourceTypeMemory:
			KdPrint(("CmResourceTypeMemory \n"));
			TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
				" - Memory Resource [%I64X-%I64X]",
				descriptor->u.Memory.Start.QuadPart,
				descriptor->u.Memory.Start.QuadPart +
				descriptor->u.Memory.Length);

			KdPrint((" - Memory Resource [%I64X-%I64X]",
				descriptor->u.Memory.Start.QuadPart,
				descriptor->u.Memory.Start.QuadPart +
				descriptor->u.Memory.Length));
			KdPrint(("Length = %04X\n", descriptor->u.Memory.Length));

			deviceContext->PA_BtCard = descriptor->u.Memory.Start;
			deviceContext->BtMemLength = descriptor->u.Memory.Length;
			break;

		case CmResourceTypeInterrupt:
			KdPrint(("Interrupt level: 0x%0x, Vector: 0x%0x, Affinity: 0x%0Ix\n",
				descriptor->u.Interrupt.Level,
				descriptor->u.Interrupt.Vector,
				descriptor->u.Interrupt.Affinity));
			break;

		default:
			break;
		}

	}

	// Map in the Registers Memory resource: Function 0
	deviceContext->BtRegsBase = (PUCHAR)MmMapIoSpace(
		deviceContext->PA_BtCard,
		deviceContext->BtMemLength,
		MmNonCached);

	if (!deviceContext->BtRegsBase) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE,
			" - Unable to map Registers memory %08I64X, length %d",
			deviceContext->PA_BtCard.QuadPart, deviceContext->BtMemLength);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	//ここまで成功すればメモリマップされたBt878のレジスタを操作可能。
	//レジスタマップについてはBt878のデータシートを参照の事。
	// ※以下の操作例程度を知っておけば十分

	//レジスタ操作例：軽く初期化
	WRITE_REGISTER_ULONG((PULONG)&deviceContext->BtRegsBase[INT_MASK], 0);		//割り込みを使わない
	WRITE_REGISTER_ULONG((PULONG)&deviceContext->BtRegsBase[GPIO_DMA_CTL], 0);	//DMAを使わない
	WRITE_REGISTER_ULONG((PULONG)&deviceContext->BtRegsBase[GPIO_OUT_EN], 0);	//出力有効レジスタ、ビット単位で指定可能。0で全ピンを入力に

	return status;
}



NTSTATUS
Bt878GpioSampleEvtDeviceReleaseHardware(
IN  WDFDEVICE    Device,
IN  WDFCMRESLIST ResourcesTranslated
)
{

	PDEVICE_CONTEXT	deviceContext;

	//	PFDO_DATA  fdoData = NULL;
	//	UNREFERENCED_PARAMETER(Device);
	UNREFERENCED_PARAMETER(ResourcesTranslated);

	//	PAGED_CODE();

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
		"--> Bt878GpioSampleEvtDeviceReleaseHardware\n");

	deviceContext = DeviceGetContext(Device);

	if (deviceContext->BtRegsBase) {

		MmUnmapIoSpace(deviceContext->BtRegsBase, deviceContext->BtMemLength);	//忘れずに
		deviceContext->BtRegsBase = NULL;
	}

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
		"<-- Bt878GpioSampleEvtDeviceReleaseHardware\n");

	return STATUS_SUCCESS;
}


NTSTATUS
Bt878GpioSampleEvtDeviceD0Entry(
IN  WDFDEVICE Device,
IN  WDF_POWER_DEVICE_STATE PreviousState
)
{
	NTSTATUS	status;
	PDEVICE_CONTEXT	deviceContext;
	UNREFERENCED_PARAMETER(Device);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER,
		"-->Bt878GpioSampleEvtDeviceD0Entry - coming from %s\n",
		DbgDevicePowerString(PreviousState));


	deviceContext = DeviceGetContext(Device);

	/*
	ASSERT(PowerDeviceD0 != PreviousState);

	//	fdoData->DevicePowerState = PowerDeviceD0;

	if (IsPoMgmtSupported(fdoData)){

	TraceEvents(TRACE_LEVEL_VERBOSE, DBG_POWER,
	"Entering fully on state\n");
	//		MPSetPowerD0(fdoData);
	}
	*/
	status = WdfIoTargetStart(WdfDeviceGetIoTarget(Device));

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "<--Bt878GpioSampleEvtDeviceD0Entry\n");

	return status;
}


NTSTATUS
Bt878GpioSampleEvtDeviceD0Exit(
IN  WDFDEVICE Device,
IN  WDF_POWER_DEVICE_STATE TargetState
)
{
	//	PFDO_DATA    fdoData;

	UNREFERENCED_PARAMETER(Device);

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
		"-->Bt878GpioSampleEvtDeviceD0Exit - moving to %s\n",
		DbgDevicePowerString(TargetState));

	WdfIoTargetStop(WdfDeviceGetIoTarget(Device), WdfIoTargetCancelSentIo);

	/*
	switch (TargetState) {
	case WdfPowerDeviceD1:
	case WdfPowerDeviceD2:
	case WdfPowerDeviceD3:

	//		if (IsPoMgmtSupported(fdoData)){
	//			TraceEvents(TRACE_LEVEL_VERBOSE, DBG_POWER,
	//				"Entering a deeper sleep state\n");
	//			MPSetPowerLow(fdoData, TargetState);
	//		}
	break;

	case WdfPowerDevicePrepareForHibernation:

	//
	// Fill in any code to save hardware state here.  Do not put in any
	// code to shut the device off.  If this device cannot support being
	// in the paging path (or being a parent or grandparent of a paging
	// path device) then this whole case can be deleted.
	//
	ASSERT(FALSE); // This driver shouldn't get this.
	break;

	case WdfPowerDeviceD3Final:
	//
	// Reset and put the device into a known initial state we're shutting
	// down for the last time.
	//
	//		NICShutdown(fdoData);
	break;

	default:
	break;
	}
	*/

	TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE, "<--Bt878GpioSampleEvtDeviceD0Exit\n");

	return STATUS_SUCCESS;

}


PCHAR
DbgDevicePowerString(
IN WDF_POWER_DEVICE_STATE Type
)
/*++

Updated Routine Description:
DbgDevicePowerString does not change in this stage of the function driver.

--*/
{
	switch (Type)
	{
	case WdfPowerDeviceInvalid:
		return "WdfPowerDeviceInvalid";
	case WdfPowerDeviceD0:
		return "WdfPowerDeviceD0";
	case WdfPowerDeviceD1:
		return "WdfPowerDeviceD1";
	case WdfPowerDeviceD2:
		return "WdfPowerDeviceD2";
	case WdfPowerDeviceD3:
		return "WdfPowerDeviceD3";
	case WdfPowerDeviceD3Final:
		return "WdfPowerDeviceD3Final";
	case WdfPowerDevicePrepareForHibernation:
		return "WdfPowerDevicePrepareForHibernation";
	case WdfPowerDeviceMaximum:
		return "WdfPowerDeviceMaximum";
	default:
		return "UnKnown Device Power State";
	}
}




// ファイル関連


VOID
Bt878GpioSampleEvtDeviceFileCreate(
IN WDFDEVICE Device,
IN WDFREQUEST Request,
IN WDFFILEOBJECT FileObject
)
/*++

Routine Description:

The framework calls a driver's EvtDeviceFileCreate callback
when the framework receives an IRP_MJ_CREATE request.
The system sends this request when a user application opens the
device to perform an I/O operation, such as reading or writing to a device.
This callback is called in the context of the thread
that created the IRP_MJ_CREATE request.

Arguments:

Device - Handle to a framework device object.
FileObject - Pointer to fileobject that represents the open handle.
CreateParams - Parameters for create

Return Value:

None

--*/
{
	//PFDO_DATA   fdoData;

	UNREFERENCED_PARAMETER(FileObject);
	UNREFERENCED_PARAMETER(Device);

	KdPrint(("Bt878GpioSampleEvtDeviceFileCreate %p\n", Device));

	PAGED_CODE();

	//
	// Get the device context given the device handle.
	//
	//fdoData = Bt878GpioSampleFdoGetData(Device);

	WdfRequestComplete(Request, STATUS_SUCCESS);

	return;
}


VOID
Bt878GpioSampleEvtFileClose(
IN WDFFILEOBJECT    FileObject
)

/*++

Routine Description:

EvtFileClose is called when all the handles represented by the FileObject
is closed and all the references to FileObject is removed. This callback
may get called in an arbitrary thread context instead of the thread that
called CloseHandle. If you want to delete any per FileObject context that
must be done in the context of the user thread that made the Create call,
you should do that in the EvtDeviceCleanp callback.

Arguments:

FileObject - Pointer to fileobject that represents the open handle.

Return Value:

None

--*/
{
	//PFDO_DATA    fdoData;
	UNREFERENCED_PARAMETER(FileObject);
	PAGED_CODE();

	//fdoData = Bt878GPIOFdoGetData(WdfFileObjectGetDevice(FileObject));

	KdPrint(("Bt878GpioSampleEvtFileClose\n"));

	return;
}

