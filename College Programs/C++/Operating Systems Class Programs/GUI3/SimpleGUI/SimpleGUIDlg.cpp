// SimpleGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleGUI.h"
#include "SimpleGUIDlg.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define RETURN "\r\n"
#define UPDATE_DATA WM_USER+1

int count;
HWND winHandle;

void myThreadFunc(LPVOID lpvThreadParam) {
	int i;
	char message[100];
    char cTime[9];
	for (i = 0; i < 10; i++) {
		count++;
		strcpy(message, "Integer changed to ");
		strcat(message, itoa(count, cTime,10));
		strcat(message, " by thread ");
		strcat(message,  (char*) lpvThreadParam);
		strcat(message, " at ");
        _strtime( cTime );
		strcat(message, cTime);
		strcat(message, RETURN);
		PostMessage(winHandle, UPDATE_DATA, 0, (long)message);
		Sleep(500);
	}
	ExitThread(0);

}
/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIDlg dialog

CSimpleGUIDlg::CSimpleGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleGUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimpleGUIDlg)
	m_txtData = _T("");
	m_Enter = _T("");
	m_valBold = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	count = 0;
	
}

void CSimpleGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleGUIDlg)
	DDX_Control(pDX, picMain, m_Pic);
	DDX_Control(pDX, chkBold, m_Bold);
	DDX_Control(pDX, chkPlain, m_Plain);
	DDX_Control(pDX, txtData, m_ctlData);
	DDX_Text(pDX, txtData, m_txtData);
	DDX_Check(pDX, chkBold, m_valBold);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimpleGUIDlg, CDialog)
	//{{AFX_MSG_MAP(CSimpleGUIDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(cmdClear, OncmdClearClicked)
	ON_BN_CLICKED(cmdExit, OncmdExitClicked)
	ON_BN_CLICKED(chkBold, OnchkBold)
	ON_BN_CLICKED(chkPlain, OnchkPlain)
	ON_BN_CLICKED(cmdThreadA, OncmdThreadA)
	ON_BN_CLICKED(cmdThreadB, OncmdThreadB)
	ON_BN_CLICKED(cmdThreadC, OncmdThreadC)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(UPDATE_DATA, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIDlg message handlers

BOOL CSimpleGUIDlg::OnInitDialog()
{
	winHandle = GetSafeHwnd();
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);	// Set small icon
	OnchkPlain();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleGUIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleGUIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSimpleGUIDlg::OncmdClearClicked() 
{
	Clear();	
}

void CSimpleGUIDlg::OncmdExitClicked() 
{
	ExitProcess(1);
	
}

void CSimpleGUIDlg::Clear() {
	m_txtData = "";
	m_Enter = "";
	count = 0;
	UpdateData(false);

}

void CSimpleGUIDlg::OnchkBold() 
{
	CFont *BoldFont = new CFont();
	BoldFont->CreateFont(0,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,"Garamond");
	CWnd *mywin = GetDlgItem(txtData);
	mywin->SetFont(BoldFont, true);
	m_Plain.SetCheck(0);
	m_Bold.SetCheck(1);
}

void CSimpleGUIDlg::OnchkPlain() 
{
	CFont *NormalFont = new CFont();
	NormalFont->CreateFont(0,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"Garamond");
	CWnd *mywin = GetDlgItem(txtData);
	mywin->SetFont(NormalFont, true);
	m_Plain.SetCheck(1);
	m_Bold.SetCheck(0);

	
}

void CSimpleGUIDlg::Launch() {
	HANDLE hChildStderrRd, hChildStderrWr, hChildStdoutRd, hChildStdoutWr, hChildStdinRd, hChildStdinWr;
	PROCESS_INFORMATION pInfo;
	unsigned long cnt;
	char c[2];
	BOOL retval;
	SECURITY_ATTRIBUTES saAttr;
	char stuff[100];

	memset(c, 0, 2);
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (! CreatePipe(&hChildStderrRd, &hChildStderrWr, &saAttr, 0)) MessageBox("Stderr pipe creation failed\n");
	if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) MessageBox("Stdout pipe creation failed\n");
	if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) MessageBox("Stdin pipe creation failed\n");

	STARTUPINFO siStartInfo; 
 
// Set up members of STARTUPINFO structure. 
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.lpReserved = NULL;
   siStartInfo.lpReserved2 = NULL;
	siStartInfo.cbReserved2 = 0;
   siStartInfo.hStdError = hChildStderrWr;  //Your error write pipe
   siStartInfo.hStdInput = hChildStdinRd;  //Your input read pipe
   siStartInfo.hStdOutput = hChildStdoutWr;  //Your output write pipe
   siStartInfo.dwFlags = STARTF_USESTDHANDLES;
   UpdateData(true);
   strcpy(stuff, m_Enter);
   CreateProcess(stuff, NULL , NULL, NULL, TRUE, CREATE_NO_WINDOW | DETACHED_PROCESS | IDLE_PRIORITY_CLASS ,NULL, NULL, &siStartInfo, &pInfo);
	CloseHandle(hChildStderrWr);
   CloseHandle(hChildStdoutWr);
   CloseHandle(hChildStdinRd);
	while(1) {
	retval = ReadFile(hChildStdoutRd, &c, 1, &cnt, NULL);
	if (retval == 0 && cnt == 0)	break;
		m_txtData += c;
		UpdateData(FALSE);
	}


}

void CSimpleGUIDlg::OnUpdate(WPARAM w, LPARAM l) {
	m_txtData += (char*) l;
	UpdateData(false);
}

void CSimpleGUIDlg::OncmdThreadA() 
{
	char* param = "A";
	DWORD threadid;
	HANDLE threadHandle;
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &myThreadFunc, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);
}

void CSimpleGUIDlg::OncmdThreadB() 
{
	char* param = "B";
	DWORD threadid;
	HANDLE threadHandle;
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &myThreadFunc, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);	
}

void CSimpleGUIDlg::OncmdThreadC() 
{
	char* param = "C";
	DWORD threadid;
	HANDLE threadHandle;
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &myThreadFunc, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);
}
