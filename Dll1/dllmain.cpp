// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h" 
#include <iostream>

typedef void(*WndHelperTest)();
//WndHelperTest sg_WndHelperTest = NULL; //在KeyboradProc中只能从注册hook的进程访问
int g_test = 0;

HHOOK g_Hook = NULL;
HHOOK g_Hook2 = NULL;
HINSTANCE g_Instance = NULL;

LRESULT _stdcall KeyboradProc(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

extern "C" __declspec(dllexport) VOID SetHook(); 
extern "C" __declspec(dllexport) VOID UnsetHook();

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	g_Instance = (HINSTANCE)hModule;  //保存DLL模块句柄

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			g_test = 88;
			OutputDebugString(TEXT("加载dll"));
		}
		break;
		case DLL_THREAD_ATTACH:
		{
			OutputDebugString(TEXT("新建线程"));
		}
		break;
		case DLL_THREAD_DETACH:
		{
			OutputDebugString(TEXT("线程退出"));
		}
		break;
		case DLL_PROCESS_DETACH:
		{
			OutputDebugString(TEXT("释放dll"));
			UnsetHook();
		}
		break;
	}
	return TRUE;
}



VOID SetHook()
{
	OutputDebugString(L"SetHook\n");

	//g_WndHelperTest = func;
	g_test = 99;

	//普通的键盘钩子  最后一个参数为NULL全局钩子,
	// WH_KEYBOARD并不能捕获所有的系统按键, 必须WH_KEYBOARD_LL （低级键盘钩子）
	g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboradProc, g_Instance, NULL);
	g_Hook2= SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTProc, g_Instance, NULL);
}

void UnsetHook()
{
	if (g_Hook)
	{
		UnhookWindowsHookEx(g_Hook);
		g_Hook = NULL;
	}
	if (g_Hook2)
	{
		UnhookWindowsHookEx(g_Hook2);
		g_Hook2 = NULL;
	}

	::SendNotifyMessage(HWND_BROADCAST, WM_NULL, 0, 0);//some background processes may still keep the library locked long after your hook was removed. Broadcasting a WM_NULL message usually helps. 
}

LRESULT _stdcall KeyboradProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		return ::CallNextHookEx(g_Hook, code, wParam, lParam);
	}
	if (code == HC_ACTION && lParam > 0)
	{
		//char Buffer[999] = { 0 };
		//GetKeyNameText(lParam, (LPWSTR)Buffer, 10);
		//wsprintf((LPWSTR)Buffer, L"%d", g_test);
		//::MessageBox(NULL, (LPCWSTR)Buffer, NULL, MB_OK);

		HWND hWnd = ::FindWindow(NULL, L"foobar");
		if (hWnd)
		{
			KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
			if (WM_KEYUP == wParam)
			{
				::SendMessage(hWnd, WM_USER+1, wParam, lParam);
			} 
		}


		

	}
	return  ::CallNextHookEx(g_Hook, code, wParam, lParam);
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{ 

	if (nCode < 0)  // do not process message 
		return CallNextHookEx(g_Hook2, nCode, wParam, lParam);
	 

	switch (nCode)
	{
		case HCBT_SETFOCUS: 
			HWND hWnd = ::FindWindow(NULL, L"foobar");
			if (hWnd)
			{
				HWND h_currentFocus = (HWND)wParam;
				HWND h_lastFocus = (HWND)lParam;
				::SendMessage(hWnd, WM_USER + 2, wParam, lParam);
				//::SendMessage(h_currentFocus, WM_CHAR, WPARAM('a'), 0);
			}
		break;
	}
	return CallNextHookEx(g_Hook2, nCode, wParam, lParam);
}