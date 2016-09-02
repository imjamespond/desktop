
// SuperTimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuperTimer.h"
#include "SuperTimerDlg.h"
#include "afxdialogex.h"
#include "shockwaveflash1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	explicit CAboutDlg(CSuperTimerDlg* );
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
	CSuperTimerDlg* _dlg;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CShockwaveflash1 _flash;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD),_dlg(nullptr)
{
}

CAboutDlg::CAboutDlg(CSuperTimerDlg* dlg) : CDialogEx(CAboutDlg::IDD),_dlg(dlg)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, _flash);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd* wmpVersionCtrl = GetDlgItem(IDC_WMP_VERSION);
	if(wmpVersionCtrl)
	{
		// Show the version in a message box.
		COLE2T pStr(_dlg->bstrVersionInfo);
		//::MessageBox( NULL, (LPCWSTR)pStr, _T("Windows Media Player Version"), MB_OK );
		CString cstr(pStr);
		wmpVersionCtrl->SetWindowTextW(cstr);

		//flash
		RECT rc;
		// 获取客户端窗口
		this->GetClientRect(&rc);
		// 更新位置
		_flash.MoveWindow(&rc, true);
		// 加载flash文件
		TCHAR curPath[500];  
		TCHAR filePath[500];
		// 获取当前路径  
		int nLength = ::GetCurrentDirectory(500, curPath);  
		// flash文件名  
		::wsprintf(filePath, L"%s\\%s", curPath, L"donkey.swf");
		_flash.LoadMovie(0, filePath);
		// 播放
		_flash.Play();		
	}

	return TRUE;//simply return true
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSuperTimerDlg dialog



CSuperTimerDlg::CSuperTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSuperTimerDlg::IDD, pParent),
	_seconds(0),
	_timerID(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSuperTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSuperTimerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOW_TRAY,&CSuperTimerDlg::OnShowTray)
	ON_BN_CLICKED(IDOK, &CSuperTimerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSuperTimerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDSTART, &CSuperTimerDlg::OnBnClickedStart)
	ON_STN_CLICKED(IDC_ADD1, &CSuperTimerDlg::OnStnClickedAdd1)
	ON_STN_CLICKED(IDC_ADD5, &CSuperTimerDlg::OnStnClickedAdd5)
	ON_STN_CLICKED(IDC_ADD10, &CSuperTimerDlg::OnStnClickedAdd10)
END_MESSAGE_MAP()


// CSuperTimerDlg message handlers

BOOL CSuperTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	init();
	initWmp();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSuperTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout(this);
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSuperTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSuperTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSuperTimerDlg::init()
{
	//设定tray
	memset(&_notifyData,0,sizeof NOTIFYICONDATAW);
	_notifyData.hWnd = this->m_hWnd;
	_notifyData.uID = IDR_MAINFRAME;
	_notifyData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;//means it has icon,msg callback and tips properties
	_notifyData.uCallbackMessage = WM_SHOW_TRAY;//(左右键)点击tray时,都会向hWnd传送消息
	_notifyData.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	wcscpy_s(_notifyData.szTip,_T("this supertimer"));
	Shell_NotifyIcon(NIM_ADD, &_notifyData);

	//设定控件字体及相关属性
	HFONT hFont=CreateFont(49,21,0,0,0,0,0,0,0,0,0,0,0,TEXT("Courier New"));
	CWnd* edit = GetDlgItem(IDC_TIME_EDIT);
	if(edit)
	{
		::SendMessage(edit->m_hWnd, WM_SETFONT,(WPARAM)hFont,0);
		CEdit* cedit = static_cast<CEdit*>(edit);
		cedit->SetLimitText(TEXT_LIMIT);
	}
	CWnd* textCtrl = GetDlgItem(IDC_TIME);
	if(textCtrl)
	{
		::SendMessage(textCtrl->m_hWnd, WM_SETFONT,(WPARAM)hFont,0);
	}	
}

void CSuperTimerDlg::update(int diff)
{
	_seconds-=diff;

	CWnd* textCtrl = GetDlgItem(IDC_TIME);
	if(textCtrl)
	{
		std::string str = Utils::formatTime(_seconds);
		CString cstr(str.c_str());
		textCtrl->SetWindowTextW(cstr);
	}
	if(_seconds<=0)
	{
		stop();
		setTop();
		playWmp();
		::SendMessage(this->m_hWnd, WM_SYSCOMMAND, IDM_ABOUTBOX, 0);//CSuperTimerDlg::OnSysCommand handle
	}
}

void CSuperTimerDlg::addEditTime(UINT minu)
{
	UINT seconds=getEditTime()+minu*60;

	CWnd* editCtrl = GetDlgItem(IDC_TIME_EDIT);
	if(editCtrl)
	{
		std::string str = Utils::numberToString<UINT>(seconds);
		CString cstr(str.c_str());
		editCtrl->SetWindowTextW(cstr);
	}
}

UINT CSuperTimerDlg::getEditTime()
{
	CWnd* editCtrl = GetDlgItem(IDC_TIME_EDIT);
	if(editCtrl)
	{
		CString cStr;
		editCtrl->GetWindowText(cStr);
		if(cStr.GetLength()==0){
			return 0;
		}
		std::string str = Utils::cstringToStd(cStr);
		return Utils::stringToNumber<u_int>(str);
	}
	return 0;
}

void CSuperTimerDlg::setTop()
{
	int X = GetSystemMetrics( SM_CXSCREEN );
	int Y = GetSystemMetrics( SM_CYSCREEN );
	RECT rect;
	this->GetWindowRect(&rect);
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,X/2-(rect.right-rect.left)/2,Y/2-(rect.bottom-rect.top)/2,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CSuperTimerDlg::stop()
{
	::MessageBeep(MB_ICONEXCLAMATION);
	if(_timerID){
		KillTimer(_timerID);
	}
	CWnd* start = GetDlgItem(IDSTART);
	if(start){
		start->SetWindowTextW(_T("Restart"));
	}
	_seconds = 0;
}

void CSuperTimerDlg::OnBnClickedOk()
{
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnOK();
}

void CSuperTimerDlg::OnBnClickedCancel()
{
	Shell_NotifyIcon(NIM_DELETE, &_notifyData);
	CDialogEx::OnCancel();
}

afx_msg LRESULT 
	CSuperTimerDlg::OnShowTray(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT) lParam; 
	switch (uMsg ) 
	{ 
	case WM_LBUTTONDOWN:
		this->ShowWindow(SW_SHOW);
		break;

	case WM_RBUTTONDOWN:
		tagPOINT point;
		LPPOINT lPoint = &point;
		::GetCursorPos(lPoint);//得到鼠标位置
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenuW(MF_STRING,WM_DESTROY,_T("close app"));
		menu.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);	

		HMENU hmenu=menu.Detach();//资源回收
		menu.DestroyMenu();

		Shell_NotifyIcon(NIM_DELETE, &_notifyData);
		break;
	}
	return 0;
}

void CSuperTimerDlg::OnBnClickedStart()
{
	CWnd* start = GetDlgItem(IDSTART);
	CWnd* edit = GetDlgItem(IDC_TIME_EDIT);
	if(edit && start)
	{
		//已经开始
		if(_seconds>0){				
			std::string str = Utils::numberToString<int>(_seconds);
			CString cstr(str.c_str());
			edit->SetWindowTextW(cstr);

			stop();
		}
		//没开始
		else{
			_seconds = getEditTime();
			//set timer
			_timerID = SetTimer( reinterpret_cast<UINT_PTR>(this) ,1000, &CSuperTimerDlg::onTimerCallback);
			//set btn caption
			start->SetWindowTextW(_T("Stop"));

			update(0);
		}
	}
}

void CALLBACK 
	CSuperTimerDlg::onTimerCallback(
	_In_ HWND     hwnd,
	_In_ UINT     uMsg,
	_In_ UINT_PTR idEvent,
	_In_ DWORD    dwTime
	)
{
	CSuperTimerDlg* pDlg = reinterpret_cast<CSuperTimerDlg*>(idEvent);
	if(pDlg){	
		pDlg->update(1);
	}
}

//media player atl control

void CSuperTimerDlg::initWmp()
{
	CoInitialize(NULL);

	HRESULT hr = S_OK;
	hr = _spPlayer.CoCreateInstance( __uuidof(WindowsMediaPlayer), 0, CLSCTX_INPROC_SERVER );

	if(SUCCEEDED(hr))
	{
		hr = _spPlayer->get_versionInfo(&bstrVersionInfo);
	}

	//wmp control
	_pPlayer = NULL;
	hr = _spPlayer->QueryInterface(__uuidof(IWMPPlayer4), (void**)&_pPlayer);
	_pPlayer->put_uiMode(_T("full"));
	_pSettings=NULL;
	hr = _pPlayer->QueryInterface(__uuidof(IWMPSettings), (void **)&_pSettings);
	_pControls = NULL;
	hr = _pPlayer->QueryInterface(__uuidof(IWMPControls), (void **)&_pControls);

	hr = _pSettings->put_volume(20);
	hr = _pPlayer->put_enabled(VARIANT_TRUE);
	hr = _pPlayer->put_URL(_T("Lambada.wma"));
	hr = _pControls->stop();
	//wmp control
}

void CSuperTimerDlg::playWmp()
{
	HRESULT hr = S_OK;
	hr = _pControls->play();
}

void CSuperTimerDlg::stopWmp()
{
	HRESULT hr = S_OK;
	hr = _pControls->pause();
}

void CSuperTimerDlg::destroyWmp()
{
	HRESULT hr = S_OK;
	hr = _pControls->Release();
	hr = _pPlayer->Release();
	// Clean up.
	_spPlayer.Release();
	CoUninitialize();
}

CSuperTimerDlg::~CSuperTimerDlg()
{
	destroyWmp();
}

void CAboutDlg::OnBnClickedOk()
{
	if(_dlg){
		_dlg->stopWmp();
	}
	CDialogEx::OnOK();
}


void CSuperTimerDlg::OnStnClickedAdd1()
{
	addEditTime(1);
}


void CSuperTimerDlg::OnStnClickedAdd5()
{
	addEditTime(5);
}


void CSuperTimerDlg::OnStnClickedAdd10()
{
	addEditTime(10);
}
