
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include "Erwin.h"
#include "WndHelper.h"
#include "Http.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HMODULE InitDllHook();
void InitListCtrl(CListCtrl& m_List);
void SetListCtrl(CListCtrl&, const km::Result&);

static CString SearchText = L"";

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

public:
private:
	HMODULE hModule;
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mSelectedItemIndex = 0;
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, mList);
	//  DDX_Control(pDX, IDC_EDIT1, mEdit1);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER+1, &CMFCApplication1Dlg::OnUser)
	ON_MESSAGE(WM_USER+2, &CMFCApplication1Dlg::OnUser2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CMFCApplication1Dlg::OnClickList2)
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
//	ON_NOTIFY(NM_SETFOCUS, IDC_LIST2, &CMFCApplication1Dlg::OnSetfocusList2)
//ON_NOTIFY(NM_KILLFOCUS, IDC_LIST2, &CMFCApplication1Dlg::OnKillfocusList2)
ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	//km::SetTestWinEventHook(::GetDlgItem(this->m_hWnd, IDC_COMBO1));
	//Erwin erwin;
	this->SetTimer(1, 3000, NULL);//启动定时器1,定时时间是1秒
	this->hModule = InitDllHook();
	this->GetDlgItem(IDC_EDIT1)->SetWindowText(TEXT("suggest.taobao.com"));
	InitListCtrl(this->mList);
	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	bool rs(false);
	rs = ::RegisterHotKey(GetSafeHwnd(), 1, MOD_ALT | MOD_WIN, VK_UP);
	rs = ::RegisterHotKey(GetSafeHwnd(), 2, MOD_ALT | MOD_WIN, VK_DOWN);
	rs = ::RegisterHotKey(GetSafeHwnd(), 3, MOD_ALT | MOD_WIN, VK_RETURN);

	rs = ::RegisterHotKey(GetSafeHwnd(), 4, MOD_CONTROL | MOD_ALT | MOD_WIN, '1');
	rs = ::RegisterHotKey(GetSafeHwnd(), 5, MOD_CONTROL | MOD_ALT | MOD_WIN, '2');
	rs = ::RegisterHotKey(GetSafeHwnd(), 6, MOD_CONTROL | MOD_ALT | MOD_WIN, '3');



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef void(*WndHelperTest)(bool);
typedef void(__cdecl* SetHook)();
typedef void(__cdecl* UnSetHook)();

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	TRACE("OnBnClickedButton1\n");

	////::SetFocus(::GetDlgItem(this->m_hWnd, IDC_COMBO1));
	//::SendMessage(::GetDlgItem(this->m_hWnd, IDC_COMBO1), WM_SETFOCUS, NULL, NULL);


	//km::Http::Test();
	km::Result rs;
	SetListCtrl(this->mList, rs);
}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{ 
		if (WndHelper::CurText.GetLength()>0 && WndHelper::CurText != SearchText)
		{
			//TRACE(L"OnTimer: %s, %s\n", WndHelper::CurText.GetString(), SearchText.GetString());
			SearchText = WndHelper::CurText;

			//search.. 
			CString host;
			GetDlgItem(IDC_EDIT1)->GetWindowText(host);
			km::Result rs = km::Http::Test(host, SearchText);
			SetListCtrl(mList,rs);
		}
		break;
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFCApplication1Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("OnHotKey %u, %u, %u\n",nHotKeyId,nKey1,nKey2);


	int i = mList.GetSelectionMark();
	if (nHotKeyId == 3 || nHotKeyId == 4 || nHotKeyId == 5 || nHotKeyId == 6 )
	{
		int subIdx = 0;
		if (nHotKeyId == 5) subIdx = 1;
		if (nHotKeyId == 6) subIdx = 2; 
		CString str = mList.GetItemText(i, subIdx);
		::SendMessage(WndHelper::CurWnd, WM_SETTEXT, 0, (LPARAM)str.GetString());

		SearchText = str;
		WndHelper::CurText = str;
	}
	else if (nHotKeyId == 1 || nHotKeyId == 2)
	{
		int ii = nHotKeyId == 1 ? i - 1 : i + 1;
		int count = mList.GetItemCount();
		ii = ii < 0 ? count - 1 : ii;
		ii = ii > count - 1 ? 0 : ii;
		mList.SetItemState(-1, 0, LVIS_SELECTED);
		mList.SetItemState(ii, LVIS_SELECTED, LVIS_SELECTED);
		mList.SetSelectionMark(ii);
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


afx_msg LRESULT CMFCApplication1Dlg::OnUser(WPARAM wParam, LPARAM lParam)
{ 
	
	TRACE("OnUser Keyboard,\n");
	WndHelper::Test();
	return 0;
}



afx_msg LRESULT CMFCApplication1Dlg::OnUser2(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnUser2 Focus %lu, %lu\n", wParam, lParam);
	WndHelper::Test((HWND)wParam);
	return 0;
}


//void CMFCApplication1Dlg::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnClose();
//}

void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	SetHook func = (SetHook)GetProcAddress(this->hModule, "UnsetHook");
	if (func)
	{
		func();
	}

	UnregisterHotKey(GetSafeHwnd(), 1);
	UnregisterHotKey(GetSafeHwnd(), 2);
	UnregisterHotKey(GetSafeHwnd(), 3);
	UnregisterHotKey(GetSafeHwnd(), 4);
	UnregisterHotKey(GetSafeHwnd(), 5);
	UnregisterHotKey(GetSafeHwnd(), 6);
}


void CMFCApplication1Dlg::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int index = pNMItemActivate->iItem, subIdx = pNMItemActivate->iSubItem;
	if (WndHelper::CurWnd)
	{
		/*::SetForegroundWindow(WndHelper::CurWnd);
		::SetActiveWindow(WndHelper::CurWnd);
		::SetFocus(WndHelper::CurWnd);
		LPARAM lParam = MAKELPARAM(5, 5);
		::SendMessage(WndHelper::CurWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		::SendMessage(WndHelper::CurWnd, WM_LBUTTONUP, 0, lParam);*/
		CString str = mList.GetItemText(index, subIdx);
		::SendMessage(WndHelper::CurWnd, WM_SETTEXT, 0, (LPARAM)str.GetString());  
		SearchText = str;
		WndHelper::CurText = str;
	}

	//TRACE("%d,%d, %S\n", index, subIdx, mList.GetItemText(index, subIdx).GetString());

	*pResult = 0;
} 





HMODULE InitDllHook()
{ 
	HMODULE hMODULE = LoadLibrary(LR"(D:\projects\vs2019\Dll1\x64\Debug\Dll1.dll)"); //可以直接写Dll1.dll，调试dll路径，属性->调试->环境：PATH=D:\projects\vs2019\Dll1\x64\Debug, 但调试dll附加进程时要copy exe到dll目录
	if (hMODULE)
	{
		static bool toggle = true;
		if (toggle)
		{ 
			SetHook func = (SetHook)GetProcAddress(hMODULE, "SetHook");
			if (func)
			{
				func();
			}
		} 
		toggle = !toggle;
	}
	return hMODULE;
}




void InitListCtrl(CListCtrl& m_List)
{
	CRect rect; 

	// 获取编程语言列表视图控件的位置和大小   
	m_List.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_List.InsertColumn(0, _T("名称"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_List.InsertColumn(1, _T("测试1"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_List.InsertColumn(2, _T("测试2"), LVCFMT_CENTER, rect.Width() / 3, 2); 
}

void SetListCtrl(CListCtrl& m_List, const km::Result& rs)
{ 
	m_List.DeleteAllItems();

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	if (rs.size()>0)
	{
		for (int i = 0; i < rs.size(); ++i)
		{
			km::Row row = rs.at(i);
			for (int j = 0; j < row.size(); ++j)
			{
				CString str = row.at(j);

				if (j == 0)
				{
					m_List.InsertItem(i, str);
				} 
				else
				{
					m_List.SetItemText(i, j, str);
				}
			}
		}
	}
	else
	{ 
		m_List.InsertItem(0, _T("Java"));
		m_List.SetItemText(0, 1, _T("1"));
		m_List.SetItemText(0, 2, _T("1"));
		m_List.InsertItem(1, _T("C"));
		m_List.SetItemText(1, 1, _T("2"));
		m_List.SetItemText(1, 2, _T("2"));
		m_List.InsertItem(2, _T("C#"));
		m_List.SetItemText(2, 1, _T("3"));
		m_List.SetItemText(2, 2, _T("6"));
		m_List.InsertItem(3, _T("C++"));
		m_List.SetItemText(3, 1, _T("4"));
		m_List.SetItemText(3, 2, _T("3"));
	}
}



