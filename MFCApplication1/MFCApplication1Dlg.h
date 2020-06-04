﻿
// MFCApplication1Dlg.h: 头文件
//

#pragma once


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent); 
protected:
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl mList; 
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	afx_msg LRESULT OnUser2(WPARAM wParam, LPARAM lParam);
private:
	HMODULE hModule;
public: 
	afx_msg void OnDestroy(); 
private:
	int mSelectedItemIndex;
public: 
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2); 
};