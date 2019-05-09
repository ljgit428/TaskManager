
// TaskManagerDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CTaskManagerDlg dialog
class CTaskManagerDlg : public CDialogEx
{
// Construction
public:
	CTaskManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TASKMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	void BrowserProcess();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedBtnEndtask();
	afx_msg void OnBnClickedBtnRefresh();
};
