#pragma once
#include <windows.h>

#ifndef DLLAPI
#define DLLAPI extern "C" __declspec( dllexport ) 
#endif

#define	MAX_BT878IO_DEVICES	8

typedef ULONG(*PFNBt878Io_GetBoardCount)();

typedef void(*PFNBt878Io_SetDir)(ULONG, ULONG);
typedef ULONG(*PFNBt878Io_GetDir)(ULONG);
typedef void(*PFNBt878Io_SetVal)(ULONG, ULONG);
typedef ULONG(*PFNBt878Io_GetVal)(ULONG);

typedef void(*PFNBt878Io_SetDir0_7)(ULONG, UCHAR);
typedef void(*PFNBt878Io_SetDir8_15)(ULONG, UCHAR);
typedef void(*PFNBt878Io_SetDir16_23)(ULONG, UCHAR);
typedef UCHAR(*PFNBt878Io_GetDir0_7)(ULONG);
typedef UCHAR(*PFNBt878Io_GetDir8_15)(ULONG);
typedef UCHAR(*PFNBt878Io_GetDir16_23)(ULONG);

typedef void(*PFNBt878Io_SetVal0_7)(ULONG, UCHAR);
typedef void(*PFNBt878Io_SetVal8_15)(ULONG, UCHAR);
typedef void(*PFNBt878Io_SetVal16_23)(ULONG, UCHAR);
typedef UCHAR(*PFNBt878Io_GetVal0_7)(ULONG);
typedef UCHAR(*PFNBt878Io_GetVal8_15)(ULONG);
typedef UCHAR(*PFNBt878Io_GetVal16_23)(ULONG);

//認識したBt878Gpioデバイスの数を返す
DLLAPI ULONG Bt878Io_GetBoardCount();


DLLAPI void Bt878Io_SetDir(ULONG index, ULONG dir);
DLLAPI ULONG Bt878Io_GetDir(ULONG index);
DLLAPI void Bt878Io_SetVal(ULONG index, ULONG val);
DLLAPI ULONG Bt878Io_GetVal(ULONG index);

DLLAPI void Bt878Io_SetDir0_7(ULONG index, UCHAR dir);
DLLAPI void Bt878Io_SetDir8_15(ULONG index, UCHAR dir);
DLLAPI void Bt878Io_SetDir16_23(ULONG index, UCHAR dir);
DLLAPI UCHAR Bt878Io_GetDir0_7(ULONG index);
DLLAPI UCHAR Bt878Io_GetDir8_15(ULONG index);
DLLAPI UCHAR Bt878Io_GetDir16_23(ULONG index);

DLLAPI void Bt878Io_SetVal0_7(ULONG index, UCHAR val);
DLLAPI void Bt878Io_SetVal8_15(ULONG index, UCHAR val);
DLLAPI void Bt878Io_SetVal16_23(ULONG index, UCHAR val);
DLLAPI UCHAR Bt878Io_GetVal0_7(ULONG index);
DLLAPI UCHAR Bt878Io_GetVal8_15(ULONG index);
DLLAPI UCHAR Bt878Io_GetVal16_23(ULONG index);
