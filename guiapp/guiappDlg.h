
// guiappDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CguiappDlg dialog
class CguiappDlg : public CDialog
{
// Construction
public:
	CguiappDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GUIAPP_DIALOG };

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
	afx_msg void OnBnClickedBtnFname();
	CEdit m_edt_fname;
	CEdit m_edt_lname;
	CEdit m_edt_sname;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnWrite();
	void Log(CString msg);
	void vspf(char *fmt, va_list argptr);
	void PrintLastError(char *fmt, va_list argptr);
	CListBox m_lst_log;
	char tmpbuf[1000];
};
