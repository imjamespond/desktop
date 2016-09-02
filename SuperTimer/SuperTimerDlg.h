
// SuperTimerDlg.h : header file
//

#pragma once

#include "atlbase.h"
#include "atlwin.h"
#include "wmp.h"

#include "Utils.h"

#define WM_SHOW_TRAY (WM_USER + 1)
#define TEXT_LIMIT 4

// CSuperTimerDlg dialog
class CSuperTimerDlg : public CDialogEx
{
	// Construction
public:
	CSuperTimerDlg(CWnd* pParent = NULL);	// standard constructor

	//=======================
	virtual ~CSuperTimerDlg();
	void initWmp();
	void playWmp();
	void stopWmp();
	void destroyWmp();
	CComBSTR bstrVersionInfo; // Contains the version string.//or u can add player obj by drag activeX ctrl
	//=======================

	// Dialog Data
	enum { IDD = IDD_SUPERTIMER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
	afx_msg LRESULT OnShowTray(WPARAM wParam, LPARAM lParam);
	static void CALLBACK onTimerCallback(
		_In_ HWND     hwnd,
		_In_ UINT     uMsg,
		_In_ UINT_PTR idEvent,
		_In_ DWORD    dwTime
		);
	void init();
	void update(int);
	void addEditTime(UINT);
	UINT getEditTime();
	void setTop();
	void stop();

protected:
	HICON m_hIcon;

	//members
	NOTIFYICONDATAW _notifyData;
	UINT_PTR _timerID;
	int _seconds;
	//wmp members
	CComPtr<IWMPPlayer> _spPlayer;
	IWMPSettings* _pSettings;
	IWMPPlayer4* _pPlayer;
	IWMPControls* _pControls;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();

	afx_msg void OnStnClickedAdd1();
	afx_msg void OnStnClickedAdd5();
	afx_msg void OnStnClickedAdd10();
};
