/*

Conexant CX25878(a.k.a.Bt878) GPIO Test Application

Copyright(c) 2020 Satoshi Hiruta.

License: BSD 3-Clause License

*/

#include "stdafx.h"
#include "Bt878GpioTestApp.h"
#include "..\Bt878Gpio\Bt878GpioDll.h"
#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

#define	MAX_BOARD_COUNT	10

ULONG	g_val[MAX_BOARD_COUNT];
ULONG	g_dir[MAX_BOARD_COUNT];

#define	TIMER_DISPLAY	10
#define	TIMER_UPDATE	11

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HMODULE	hDLL = NULL;

PFNBt878Io_GetBoardCount	pfnBt878Io_GetBoardCount = NULL;

PFNBt878Io_SetDir	pfnBt878Io_SetDir = NULL;
PFNBt878Io_GetDir	pfnBt878Io_GetDir = NULL;
PFNBt878Io_SetVal	pfnBt878Io_SetVal = NULL;
PFNBt878Io_GetVal	pfnBt878Io_GetVal = NULL;

PFNBt878Io_SetDir0_7	pfnPFNBt878Io_SetDir0_7 = NULL;
PFNBt878Io_SetDir8_15	pfnPFNBt878Io_SetDir8_15 = NULL;
PFNBt878Io_SetDir16_23	pfnPFNBt878Io_SetDir16_23 = NULL;
PFNBt878Io_GetDir0_7	pfnPFNBt878Io_GetDir0_7 = NULL;
PFNBt878Io_GetDir8_15	pfnPFNBt878Io_GetDir8_15 = NULL;
PFNBt878Io_GetDir16_23	pfnPFNBt878Io_GetDir16_23 = NULL;

PFNBt878Io_SetVal0_7	pfnPFNBt878Io_SetVal0_7 = NULL;
PFNBt878Io_SetVal8_15	pfnPFNBt878Io_SetVal8_15 = NULL;
PFNBt878Io_SetVal16_23	pfnPFNBt878Io_SetVal16_23 = NULL;
PFNBt878Io_GetVal0_7	pfnPFNBt878Io_GetVal0_7 = NULL;
PFNBt878Io_GetVal8_15	pfnPFNBt878Io_GetVal8_15 = NULL;
PFNBt878Io_GetVal16_23	pfnPFNBt878Io_GetVal16_23 = NULL;


BOOL InitBt878IoDll()
{
	ULONG	i;

	hDLL = ::LoadLibrary(_T("Bt878GpioDll.dll"));
	if (hDLL == NULL){
		MessageBox(NULL, _T("Bt878GpioDll.dll not dound."), _T("ERROR"), MB_ICONWARNING);
		return	FALSE;
	}
	else{
		OutputDebugString(_T("Bt878GpioDll.dll loaded.\n"));
	}

	pfnBt878Io_GetBoardCount = (PFNBt878Io_GetBoardCount)GetProcAddress(hDLL, "Bt878Io_GetBoardCount");	//戻り値 ULONG board_count(搭載ボード数を返す)

	//Set系ファンクションの第一引数はボードID、第二引数は設定する値
	//Get系ファンクションの第一引数はボードID、戻り値はレジスタ値

	pfnBt878Io_SetDir = (PFNBt878Io_SetDir)GetProcAddress(hDLL, "Bt878Io_SetDir");	//引数：ULONG board_index, ULONG io_direction_value
	pfnBt878Io_GetDir = (PFNBt878Io_GetDir)GetProcAddress(hDLL, "Bt878Io_GetDir");	//引数：ULONG board_index	戻り値：ULONG io_direction_value
	pfnBt878Io_SetVal = (PFNBt878Io_SetVal)GetProcAddress(hDLL, "Bt878Io_SetVal");	//引数：ULONG board_index, ULONG GPIO_output_value
	pfnBt878Io_GetVal = (PFNBt878Io_GetVal)GetProcAddress(hDLL, "Bt878Io_GetVal");	//引数：ULONG board_index	戻り値：ULONG GPIO_value

	pfnPFNBt878Io_SetDir0_7 = (PFNBt878Io_SetDir0_7)GetProcAddress(hDLL, "Bt878Io_SetDir0_7");			//引数：ULONG board_index, UCHAR io_direction_value(0-7 bit)
	pfnPFNBt878Io_SetDir8_15 = (PFNBt878Io_SetDir8_15)GetProcAddress(hDLL, "Bt878Io_SetDir8_15");		//引数：ULONG board_index, UCHAR io_direction_value(8-15 bit)
	pfnPFNBt878Io_SetDir16_23 = (PFNBt878Io_SetDir16_23)GetProcAddress(hDLL, "Bt878Io_SetDir16_23");	//引数：ULONG board_index, UCHAR io_direction_value(16-23 bit)
	pfnPFNBt878Io_GetDir0_7 = (PFNBt878Io_GetDir0_7)GetProcAddress(hDLL, "Bt878Io_GetDir0_7");			//引数：ULONG board_index	戻り値：UCHAR io_direction_value(0-7bit)
	pfnPFNBt878Io_GetDir8_15 = (PFNBt878Io_GetDir8_15)GetProcAddress(hDLL, "Bt878Io_GetDir8_15");		//引数：ULONG board_index	戻り値：UCHAR io_direction_value(8-15bit)
	pfnPFNBt878Io_GetDir16_23 = (PFNBt878Io_GetDir16_23)GetProcAddress(hDLL, "Bt878Io_GetDir16_23");	//引数：ULONG board_index	戻り値：UCHAR io_direction_value(16-23bit)

	pfnPFNBt878Io_SetVal0_7 = (PFNBt878Io_SetVal0_7)GetProcAddress(hDLL, "Bt878Io_SetVal0_7");			//引数：ULONG board_index, UCHAR GPIO_output_value(0-7bit)
	pfnPFNBt878Io_SetVal8_15 = (PFNBt878Io_SetVal8_15)GetProcAddress(hDLL, "Bt878Io_SetVal8_15");		//引数：ULONG board_index, UCHAR GPIO_output_value(8-15bit)
	pfnPFNBt878Io_SetVal16_23 = (PFNBt878Io_SetVal16_23)GetProcAddress(hDLL, "Bt878Io_SetVal16_23");	//引数：ULONG board_index, UCHAR GPIO_output_value(16-23bit)
	pfnPFNBt878Io_GetVal0_7 = (PFNBt878Io_GetVal0_7)GetProcAddress(hDLL, "Bt878Io_GetVal0_7");			//引数：ULONG board_index	戻り値：UCHAR GPIO_value(0-7bit)
	pfnPFNBt878Io_GetVal8_15 = (PFNBt878Io_GetVal8_15)GetProcAddress(hDLL, "Bt878Io_GetVal8_15");		//引数：ULONG board_index	戻り値：UCHAR GPIO_value(8-15bit)
	pfnPFNBt878Io_GetVal16_23 = (PFNBt878Io_GetVal16_23)GetProcAddress(hDLL, "Bt878Io_GetVal16_23");	//引数：ULONG board_index	戻り値：UCHAR GPIO_value(16-23bit)


	if (pfnBt878Io_GetBoardCount == NULL)		return	FALSE;

	if (pfnBt878Io_SetDir == NULL)		return	FALSE;
	if (pfnBt878Io_GetDir == NULL)		return	FALSE;
	if (pfnBt878Io_SetVal == NULL)		return	FALSE;
	if (pfnBt878Io_GetVal == NULL)		return	FALSE;

	if (pfnPFNBt878Io_SetDir0_7 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_SetDir8_15 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_SetDir16_23 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetDir0_7 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetDir8_15 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetDir16_23 == NULL)	return	FALSE;

	if (pfnPFNBt878Io_SetVal0_7 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_SetVal8_15 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_SetVal16_23 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetVal0_7 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetVal8_15 == NULL)	return	FALSE;
	if (pfnPFNBt878Io_GetVal16_23 == NULL)	return	FALSE;


	for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
		pfnBt878Io_SetDir(i, 0x00000000);
		pfnBt878Io_SetVal(i, 0x00000000);
	}

	StringCchPrintf(szTitle, 100, _T("Bt878GPIO: found %u device(s).\n"), pfnBt878Io_GetBoardCount());

	return	TRUE;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DLLTEST00, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DLLTEST00));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLLTEST00));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DLLTEST00);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   if(FALSE == InitBt878IoDll())	return	FALSE;


   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DrawInfo(HWND hWnd)
{
	HDC		hDC, hMemDC;
	HBITMAP	hMemBM, hPrevBM;
	HBRUSH	hBrushBG;
	COLORREF	color_text;
	TCHAR	*pStr = NULL;
	TCHAR	tmpstr[MAX_PATH];

	PAINTSTRUCT ps;
	RECT	rc;
	ULONG	i;

	GetClientRect(hWnd, &rc);
	hDC = BeginPaint(hWnd, &ps);

	hMemDC = CreateCompatibleDC(hDC);
	hMemBM = CreateCompatibleBitmap(hDC, (rc.right - rc.left), (rc.bottom - rc.top));
	hPrevBM = (HBITMAP)SelectObject(hMemDC, hMemBM);

	hBrushBG = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	color_text = RGB(0xfa, 0xfa, 0xfa);

	FillRect(hMemDC, &rc, hBrushBG);

	SetBkMode(hMemDC, TRANSPARENT);
	SetTextColor(hMemDC, color_text);

	pStr = (TCHAR*)new TCHAR[4000];
	if (pStr){
		ZeroMemory(pStr, (sizeof(TCHAR) * 4000));
		for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
			StringCchPrintf(tmpstr, MAX_PATH, _T("%u:Direction  = 0x%08X\n%u:Read value = 0x%08X\n"), i, g_dir[i], i,g_val[i]);
			StringCchCat(pStr, 4000, tmpstr);
		}
		DrawText(hMemDC, (LPCTSTR)pStr, lstrlen(pStr), &rc, DT_VCENTER | DT_CENTER | DT_NOPREFIX | DT_NOCLIP);
		delete[] pStr;
	}
	BitBlt(hDC, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), hMemDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);

	SelectObject(hMemDC, hPrevBM);
	DeleteObject(hMemBM);
	DeleteDC(hMemDC);
}



//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;


	static UINT_PTR timer_display = 0, timer_update = 0;
	ULONG	i, val;
	static ULONG	counter = 0;
	static BOOL bOutput = FALSE;

	switch (message)
	{
	case WM_CREATE:
		timer_display = SetTimer(hWnd, TIMER_DISPLAY, 200, NULL);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case ID_TEST_INPUTTEST:
			bOutput = FALSE;
			for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
				pfnBt878Io_SetDir(i, 0);
			}
			if (timer_update)	KillTimer(hWnd, timer_update);
			timer_update = 0;
			break;
		case ID_TEST_OUTPUTTEST:
			bOutput = TRUE;
			for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
				pfnBt878Io_SetDir(i, 0x00ffffff);
			}
			timer_update = SetTimer(hWnd, TIMER_UPDATE, 1000, NULL);
			counter = 0;
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:

		switch (wParam){
		case TIMER_DISPLAY:
			for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
				g_dir[i] = pfnBt878Io_GetDir(i);
				g_val[i] = pfnBt878Io_GetVal(i);
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case TIMER_UPDATE:
			val = 1;
			//val <<= (counter % 24);
			for (i = 0; i < (counter % 24); i++){
				val <<= 1;
				val |= 1;
			}

			for (i = 0; i < pfnBt878Io_GetBoardCount(); i++){
				val = (i % 2) ? ~val : val;
				pfnBt878Io_SetVal(i, val);
			}
			counter++;
			break;
		}

		break;

	case WM_PAINT:
		DrawInfo(hWnd);
		break;

	case WM_DESTROY:
		if (timer_display)	KillTimer(hWnd, timer_display);
		if (timer_update)	KillTimer(hWnd, timer_update);
		if (pfnBt878Io_SetDir)	pfnBt878Io_SetDir(0, 0x00000000);
		if (hDLL != NULL)	FreeLibrary(hDLL);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
