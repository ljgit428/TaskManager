
// TaskManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTaskManagerDlg dialog



CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTaskManagerDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BTN_EndTask, &CTaskManagerDlg::OnBnClickedBtnEndtask)
	ON_BN_CLICKED(IDC_BTN_Refresh, &CTaskManagerDlg::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


// CTaskManagerDlg message handlers

BOOL CTaskManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_list.InsertColumn(0, _T("Number"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, _T("Task Name"), LVCFMT_LEFT, 200);
	m_list.InsertColumn(2, _T("PID"), LVCFMT_LEFT, 100);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	BrowserProcess();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTaskManagerDlg::OnPaint()
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
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTaskManagerDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CTaskManagerDlg::BrowserProcess() {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	BOOL bMore = Process32First(hProcessSnap, &pe32);
	int i = 0;
	CString str;
	while (bMore) {
		str.Format(_T("%d"), i+1);
		m_list.InsertItem(i, str);
		m_list.SetItemText(i, 1, pe32.szExeFile);

		str.Format(_T("%d"), pe32.th32ProcessID);
		m_list.SetItemText(i, 2, str);

		bMore = Process32Next(hProcessSnap, &pe32);
		i++;
	}
}

void CTaskManagerDlg::OnBnClickedBtnEndtask()
{
	int nSel = m_list.GetSelectionMark();

	if (nSel < 0) {
		return;
	}

	CString strPID = m_list.GetItemText(nSel, 2);
	DWORD dwPID = _wtoi(strPID);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (hProcess == NULL) {
		MessageBox(L"Fail to open tasks.");
		return;
	}

	TerminateProcess(hProcess, 0);
	Sleep(200);
	OnBnClickedBtnRefresh();
}


void CTaskManagerDlg::OnBnClickedBtnRefresh()
{
	m_list.DeleteAllItems();
	BrowserProcess();
}
