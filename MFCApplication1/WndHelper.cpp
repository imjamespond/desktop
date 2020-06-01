#include "pch.h"
#include "WndHelper.h"


using namespace km;

void WndHelper::Test(bool debug)
{
	//CPoint point;//定义一个用于确定光标位置的位置
	//GetCursorPos(&point);//获取当前光标的位置，以便使得菜单可以跟随光标  
	//ScreenToClient(nullptr,&point);
	////得到窗体的控件句柄
	//CWnd* hControlWnd = (CWnd*)ChildWindowFromPoint(nullptr, point);
	//UINT nID = hControlWnd->GetDlgCtrlID(); 

	HWND wnd;//窗口句柄
	wnd = GetForegroundWindow();//获得当前激活的窗口句柄
	DWORD SelfThreadId = GetCurrentThreadId();//获取本身的线程ID
	DWORD ForeThreadId = GetWindowThreadProcessId(wnd, NULL);//根据窗口句柄获取线程ID
	AttachThreadInput(ForeThreadId, SelfThreadId, true);//附加线程
	wnd = GetFocus();//获取具有输入焦点的窗口句柄
	AttachThreadInput(ForeThreadId, SelfThreadId, false);//取消附加的线程

	wchar_t szText[1024]; 
	SendMessage(wnd, WM_GETTEXT, 1024, (LPARAM)szText);

	if (::wcslen(szText))
	{ 
		if(debug)
		{
			TRACE("send a\n");
			//SendMessage(wnd, WM_CHAR, WPARAM('a'), 0);//发送一个字消息
		}
		TRACE("WM_GETTEXT %S\n", szText);
	}
	
}


// Hook procedures
// https://docs.microsoft.com/en-us/windows/win32/winmsg/using-hooks
//LRESULT WINAPI FocusHook(int , WPARAM , LPARAM );
//HHOOK hhook;
VOID CALLBACK WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
void km::SetHook(HWND hWnd)
{
  //  HINSTANCE hIns = (HINSTANCE)::GetWindowLong(hWnd, GWLP_HINSTANCE);
  //  hhook = ::SetWindowsHookEx(
  //      WH_CALLWNDPROC,
  //      FocusHook,
		//(HINSTANCE)NULL, /*::GetCurrentThreadId()*/ 0);

	::SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL,
		WinEventProcCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

VOID CALLBACK WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	if (dwEvent == EVENT_SYSTEM_FOREGROUND)
	{
		/* do something */
		TRACE("EVENT_SYSTEM_FOREGROUND\n");
	}
	else if (dwEvent == EVENT_OBJECT_FOCUS)
	{
		//TRACE("EVENT_OBJECT_FOCUS\n");
		WndHelper::Test();
	}
	/*else if (dwEvent == EVENT_OBJECT_VALUECHANGE)
	{
		TRACE("EVENT_OBJECT_VALUECHANGE\n");
		WndHelper::Test();
	}*/
	else 
	{
		//TRACE("dwEvent: %u\n", dwEvent);
	}
}

