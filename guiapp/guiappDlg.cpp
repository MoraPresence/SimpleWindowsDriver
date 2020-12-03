
// guiappDlg.cpp : implementation file
//

#include "stdafx.h"
#include "guiapp.h"
#include "guiappDlg.h"
#include "SCMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CguiappDlg dialog




CguiappDlg::CguiappDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CguiappDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CguiappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_FNAME, m_edt_fname);
	DDX_Control(pDX, IDC_EDT_LNAME, m_edt_lname);
	DDX_Control(pDX, IDC_EDT_SNAME, m_edt_sname);
	DDX_Control(pDX, IDC_LST_LOG, m_lst_log);
}

BEGIN_MESSAGE_MAP(CguiappDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_FNAME, &CguiappDlg::OnBnClickedBtnFname)
	ON_BN_CLICKED(IDC_BTN_START, &CguiappDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_ADD, &CguiappDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CguiappDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_STOP, &CguiappDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CguiappDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CguiappDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_READ, &CguiappDlg::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_WRITE, &CguiappDlg::OnBnClickedBtnWrite)
END_MESSAGE_MAP()


// CguiappDlg message handlers

BOOL CguiappDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CguiappDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CguiappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CguiappDlg::OnBnClickedBtnFname()
{
	// TODO: Add your control notification handler code here
	CFileDialog cfd(TRUE);

	if(IDOK != cfd.DoModal()){
		Log(L"Error: OnBnClickedBtnFname->DoModal");
		return;
	}
		
	//get filename and put him in fname
	m_edt_fname.SetWindowText(cfd.GetPathName());
	Log(L"Success: Fname Added");
}

void CguiappDlg::OnBnClickedBtnAdd()
{
	CSCMgr scm;

	CString sname, fname;
	m_edt_sname.GetWindowText(sname);
	m_edt_fname.GetWindowText(fname);
	try{
		scm.Add(sname, fname);
		Log(L"Success: Driver Added");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Add");
	}
} 

void CguiappDlg::OnBnClickedBtnStart()
{
	CSCMgr scm;

	CString sname;
	m_edt_sname.GetWindowText(sname);
	try{
		scm.Start(sname);
		Log(L"Success: Driver Started");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Start");
	}
}



void CguiappDlg::OnBnClickedBtnDel()
{
	CSCMgr scm;

	CString sname;
	m_edt_sname.GetWindowText(sname);
	try{
		scm.Del(sname);
		Log(L"Success: Driver Deleted");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Del");
	}
}

void CguiappDlg::OnBnClickedBtnStop()
{
	CSCMgr scm;

	CString sname;
	m_edt_sname.GetWindowText(sname);
	try{
		scm.Stop(sname);
		Log(L"Success: Driver Stoped");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Stop");
	}	
}

void CguiappDlg::OnBnClickedBtnOpen()
{
	CSCMgr scm;

	CString lname;
	m_edt_lname.GetWindowText(lname);
	try{
		scm.Open(lname);
		Log(L"Success: Driver Opened");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Open");
	}	
}

void CguiappDlg::OnBnClickedBtnClose()
{
	CSCMgr scm;
	try{
		scm.Close();
		Log(L"Success: Driver Closed");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Close");
	}
}

void CguiappDlg::OnBnClickedBtnRead()
{
	CSCMgr scm;
	try{
		unsigned char* buf = new unsigned char[8];
		scm.Read(buf, 8);
		Log(L"Success: Driver Read");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Read");
	}
}


void CguiappDlg::OnBnClickedBtnWrite()
{
	CSCMgr scm;
	try{
		unsigned char* buf = new unsigned char[16];
		scm.Write(buf, 16);
		Log(L"Success: Driver Write");
	}catch(...){
		Log(L"Error: OnBnClickedBtnAdd->scm.Write");
	}
}
void CguiappDlg::Log(CString msg){
	m_lst_log.AddString(msg);
}

void CguiappDlg::vspf(char *fmt, va_list argptr)
{

	memset(tmpbuf, 0, sizeof(tmpbuf));

	vsprintf(tmpbuf, fmt, argptr);

	strcat(tmpbuf, "\r\n");
	//m_lst_log.ReplaceSel(tmpbuf);
}

void CguiappDlg::PrintLastError(char *fmt, va_list argptr)
{
	LPVOID lpMsgBuf;
	int cnt;

	memset(tmpbuf, 0, sizeof(tmpbuf));

	FormatMessage(
       FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
       NULL,
       GetLastError(),
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
       (LPTSTR) &lpMsgBuf,
       0,
       NULL
	);

	cnt = vsprintf(tmpbuf, fmt, argptr);

	strcat(tmpbuf, (char*)lpMsgBuf);
	strcat(tmpbuf, "\r\n");
	//m_lst_log.ReplaceSel(tmpbuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}



