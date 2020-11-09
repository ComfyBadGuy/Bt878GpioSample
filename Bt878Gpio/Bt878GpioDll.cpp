/*

DLL for Conexant CX25878(a.k.a.Bt878) GPIO Driver

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

*/

// Bt878Gpio.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "Bt878GpioDll.h"

#include <setupapi.h>
#include <initguid.h>
#pragma comment (lib,"Setupapi.lib")
#include "..\Bt878GpioSample\Public.h"

HANDLE	g_hBt878Device[MAX_BT878IO_DEVICES];
ULONG	g_board_count = 0;
TCHAR	szDebug[MAX_PATH];


DLLAPI void Bt878Io_SetDir(ULONG index, ULONG dir)
{
	BOOL	bResult;
	DWORD	dwBytes;
	ULONG	Value = dir & 0x00ffffff;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SETIODIR,
		&Value,
		sizeof(ULONG),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI ULONG Bt878Io_GetDir(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	ULONG	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xffffffff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GETIODIR,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	Value & 0x00ffffff;
}

DLLAPI void Bt878Io_SetVal(ULONG index, ULONG val)
{
	BOOL	bResult;
	DWORD	dwBytes;
	ULONG	Value = val & 0x00ffffff;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SET,
		&Value,
		sizeof(ULONG),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI ULONG Bt878Io_GetVal(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	ULONG	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xffffffff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GET,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	Value & 0x00ffffff;
}


DLLAPI void Bt878Io_SetDir0_7(ULONG index, UCHAR dir)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SETIODIR0_7,
		&dir,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI void Bt878Io_SetDir8_15(ULONG index, UCHAR dir)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SETIODIR8_15,
		&dir,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI void Bt878Io_SetDir16_23(ULONG index, UCHAR dir)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SETIODIR16_23,
		&dir,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}


DLLAPI UCHAR Bt878Io_GetDir0_7(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GETIODIR0_7,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}


DLLAPI UCHAR Bt878Io_GetDir8_15(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GETIODIR8_15,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}

DLLAPI UCHAR Bt878Io_GetDir16_23(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GETIODIR16_23,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}


DLLAPI void Bt878Io_SetVal0_7(ULONG index, UCHAR val)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SET0_7,
		&val,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI void Bt878Io_SetVal8_15(ULONG index, UCHAR val)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SET8_15,
		&val,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}

DLLAPI void Bt878Io_SetVal16_23(ULONG index, UCHAR val)
{
	BOOL	bResult;
	DWORD	dwBytes;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_SET16_23,
		&val,
		sizeof(UCHAR),
		NULL,
		0,
		&dwBytes,
		NULL);
}


DLLAPI UCHAR Bt878Io_GetVal0_7(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GET0_7,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}

DLLAPI UCHAR Bt878Io_GetVal8_15(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GET8_15,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}

DLLAPI UCHAR Bt878Io_GetVal16_23(ULONG index)
{
	BOOL	bResult;
	DWORD	dwBytes;
	UCHAR	Value;

	if (g_hBt878Device[index] == INVALID_HANDLE_VALUE)	return	0xff;

	bResult = DeviceIoControl(
		g_hBt878Device[index],
		IOCTL_BT878GPIO_GET16_23,
		NULL,
		0,
		&Value,
		sizeof(ULONG),
		&dwBytes,
		NULL);

	return	bResult ? Value : 0xff;
}

DLLAPI ULONG Bt878Io_GetBoardCount()
{
	return	g_board_count;
}

BOOL OpenDevices()
{

	BOOL	bResult;
	ULONG                               predictedLength = 0;
	ULONG                               requiredLength = 0;
	DWORD	index;

	HDEVINFO hDevInfo;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pdeviceInterfaceDetailData;
	SP_DEVICE_INTERFACE_DATA	DeviceInterfaceData;

	OutputDebugString(_T("-->OpenDevices()\n"));


	for (index = 0; index < MAX_BT878IO_DEVICES; index++){
		g_hBt878Device[index] = INVALID_HANDLE_VALUE;
	}

	hDevInfo = INVALID_HANDLE_VALUE;
	pdeviceInterfaceDetailData = NULL;

	hDevInfo = SetupDiGetClassDevs(
		(LPGUID)&GUID_DEVINTERFACE_Bt878GpioSample,
		NULL, // Define no enumerator (global)
		NULL, // Define no
		(DIGCF_PRESENT | // Only Devices present
		DIGCF_DEVICEINTERFACE)); // Function class devices.

	if (INVALID_HANDLE_VALUE == hDevInfo){
		StringCchPrintf(szDebug,
			MAX_PATH,
			_T("SetupDiGetClassDevs failed: %x\n"), GetLastError());
		OutputDebugString(szDebug);
		return	FALSE;
	}

	index = 0;
	do{
		DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		bResult = SetupDiEnumDeviceInterfaces(hDevInfo,
			0, // No care about specific PDOs
			(LPGUID)&GUID_DEVINTERFACE_Bt878GpioSample,
			index, //
			&DeviceInterfaceData);

		if (bResult){

			//必要バッファサイズ調査
			SetupDiGetDeviceInterfaceDetail(
				hDevInfo,
				&DeviceInterfaceData,
				NULL, // probing so no output buffer yet
				0, // probing so output buffer length of zero
				&requiredLength,
				NULL); // not interested in the specific dev-node

			if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) {
				StringCchPrintf(szDebug,
					MAX_PATH,
					_T("SetupDiGetDeviceInterfaceDetail() 不明なエラー：バッファサイズ取得に失敗\n"));
				OutputDebugString(szDebug);
				return	FALSE;
			}

			//バッファメモリ確保
			predictedLength = requiredLength;
			pdeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(predictedLength);
			if (pdeviceInterfaceDetailData) {
				ZeroMemory(pdeviceInterfaceDetailData, predictedLength);
				pdeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			}
			else {
				StringCchPrintf(szDebug,
					MAX_PATH,
					_T("Couldn't allocate buffer for device interface details.\n"));
				OutputDebugString(szDebug);
				return	FALSE;
			}

			//バッファ内にdeviceInterfaceDetailData取得
			bResult = SetupDiGetDeviceInterfaceDetail(
				hDevInfo,
				&DeviceInterfaceData,
				pdeviceInterfaceDetailData,
				predictedLength,
				&requiredLength,
				NULL);
			if (bResult == FALSE){
				StringCchPrintf(szDebug,
					MAX_PATH,
					_T("Error in SetupDiGetDeviceInterfaceDetail\n"));
				OutputDebugString(szDebug);
				free(pdeviceInterfaceDetailData);
				return	FALSE;
			}

			OutputDebugString(_T("デバイスパスを取得："));
			OutputDebugString(pdeviceInterfaceDetailData->DevicePath);
			OutputDebugString(_T("\n"));

			g_hBt878Device[index] = CreateFile(pdeviceInterfaceDetailData->DevicePath,
				GENERIC_READ | GENERIC_WRITE, // Only read access
				0, // FILE_SHARE_READ | FILE_SHARE_WRITE
				NULL, // no SECURITY_ATTRIBUTES structure
				OPEN_EXISTING, // No special create flags
				FILE_FLAG_OVERLAPPED, // No special attributes
				NULL); // No template file

			free(pdeviceInterfaceDetailData);

			//次を探す？
			if (index < (MAX_BT878IO_DEVICES -1)){
				index++;
			}
			else{
				bResult = FALSE;
			}
		}
		else{
			if (ERROR_NO_MORE_ITEMS == GetLastError()) {
				StringCchPrintf(szDebug,
					MAX_PATH,
					_T("Error:Interface GUID_DEVINTERFACE_BT878GPIO is not registered.\n"));
			}
			else{
				StringCchPrintf(szDebug,
					MAX_PATH,
					_T("SetupDiEnumDeviceInterfaces failed\n"));
			}
			if(index == 0)	OutputDebugString(szDebug);
		}

	} while (bResult);

	SetupDiDestroyDeviceInfoList(hDevInfo);

	StringCchPrintf(szDebug,
		MAX_PATH,
		_T("found %u device(s).\n"), index);
	OutputDebugString(szDebug);

	OutputDebugString(_T("<--OpenDevices()\n"));

	g_board_count = index;

	return	TRUE;

}

void CloseDevices()
{
	ULONG	i;
	for (i = 0; i < MAX_BT878IO_DEVICES; i++){
		if (g_hBt878Device[i] != INVALID_HANDLE_VALUE){
			CloseHandle(g_hBt878Device[i]);
			g_hBt878Device[i] = INVALID_HANDLE_VALUE;
		}
	}
}