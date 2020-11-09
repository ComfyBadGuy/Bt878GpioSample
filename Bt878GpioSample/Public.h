/*++

Conexant CX25878(a.k.a.Bt878) GPIO Driver

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/
#pragma once 
//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_Bt878GpioSample,
    0x4e738667,0xee03,0x45cf,0xbe,0x17,0x0e,0xe7,0x6c,0xa0,0x2f,0xb1);
// {4e738667-ee03-45cf-be17-0ee76ca02fb1}

#ifndef FILE_DEVICE_BUS_EXTENDER
#define FILE_DEVICE_BUS_EXTENDER        0x0000002a
#endif

#define BT878GPIO_IOCTL(_index_) \
    CTL_CODE (FILE_DEVICE_BUS_EXTENDER, _index_, METHOD_BUFFERED, FILE_READ_DATA)

#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED                 0
#endif

#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#endif

#define IOCTL_BT878GPIO_SETIODIR			BT878GPIO_IOCTL (0x10)
#define IOCTL_BT878GPIO_GETIODIR			BT878GPIO_IOCTL (0x11)
#define IOCTL_BT878GPIO_SET					BT878GPIO_IOCTL (0x12)
#define IOCTL_BT878GPIO_GET					BT878GPIO_IOCTL (0x13)

//new
#define IOCTL_BT878GPIO_SETIODIR0_7			BT878GPIO_IOCTL (0x20)
#define IOCTL_BT878GPIO_SETIODIR8_15		BT878GPIO_IOCTL (0x21)
#define IOCTL_BT878GPIO_SETIODIR16_23		BT878GPIO_IOCTL (0x22)

#define IOCTL_BT878GPIO_GETIODIR0_7			BT878GPIO_IOCTL (0x23)
#define IOCTL_BT878GPIO_GETIODIR8_15		BT878GPIO_IOCTL (0x24)
#define IOCTL_BT878GPIO_GETIODIR16_23		BT878GPIO_IOCTL (0x25)

#define IOCTL_BT878GPIO_SET0_7				BT878GPIO_IOCTL (0x26)
#define IOCTL_BT878GPIO_SET8_15				BT878GPIO_IOCTL (0x27)
#define IOCTL_BT878GPIO_SET16_23			BT878GPIO_IOCTL (0x28)

#define IOCTL_BT878GPIO_GET0_7				BT878GPIO_IOCTL (0x29)
#define IOCTL_BT878GPIO_GET8_15				BT878GPIO_IOCTL (0x30)
#define IOCTL_BT878GPIO_GET16_23			BT878GPIO_IOCTL (0x31)
