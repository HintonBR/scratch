// SimpleGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleGUI.h"
#include "SimpleGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define RETURN "\r\n"
/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIDlg dialog

CSimpleGUIDlg::CSimpleGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleGUIDlg::IDD, pParent)
{
	CFont *myFont = new CFont();
	myFont->CreatePointFont(14, "TIMES NEW ROMAN", NULL);
	//{{AFX_DATA_INIT(CSimpleGUIDlg)
	m_lblInstructions = _T("");
	m_txtData = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lblInstructions = "This program randomly generates a number between 1 and 8!";
	m_lblInstructions += RETURN;
	m_lblInstructions += "Guess what number I am thinking of by clicking on the ";
	m_lblInstructions += RETURN;
	m_lblInstructions += "corresponding radio button - the answer will then be printed out in";
	m_lblInstructions += RETURN;
	m_lblInstructions += "the edit window.";
	m_lblInstructions += RETURN;
	m_lblInstructions += "Good Luck!";
	//m_ctlInstructions.SetFont(myFont, FALSE);
	delete myFont;
}

void CSimpleGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleGUIDlg)
	DDX_Control(pDX, txtData, m_ctlData);
	DDX_Control(pDX, lblInstructions, m_ctlInstructions);
	DDX_Control(pDX, IDC_RADIO9, m_radio8);
	DDX_Control(pDX, IDC_RADIO8, m_radio7);
	DDX_Control(pDX, IDC_RADIO7, m_radio6);
	DDX_Control(pDX, IDC_RADIO6, m_radio5);
	DDX_Control(pDX, IDC_RADIO5, m_radio4);
	DDX_Control(pDX, IDC_RADIO4, m_radio3);
	DDX_Control(pDX, IDC_RADIO3, m_radio2);
	DDX_Control(pDX, IDC_RADIO2, m_radio1);
	DDX_Text(pDX, lblInstructions, m_lblInstructions);
	DDX_Text(pDX, txtData, m_txtData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimpleGUIDlg, CDialog)
	//{{AFX_MSG_MAP(CSimpleGUIDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(cmdClear, OncmdClearClicked)
	ON_BN_CLICKED(cmdExit, OncmdExitClicked)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2Clicked)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3Clicked)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4Clicked)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5Clicked)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6Clicked)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7Clicked)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8Clicked)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9Clicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIDlg message handlers

BOOL CSimpleGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);	// Set small icon
	srand( (unsigned)time( NULL ) );
	GetNewValue();
	
	
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
	CSimpleGUIDlg::DestroyWindow();
	
}

void CSimpleGUIDlg::OnRadio2Clicked() 
{
	CheckNumber(1);
}

void CSimpleGUIDlg::OnRadio3Clicked() 
{
	CheckNumber(2);
}

void CSimpleGUIDlg::OnRadio4Clicked() 
{
	CheckNumber(3);
}

void CSimpleGUIDlg::OnRadio5Clicked() 
{
	CheckNumber(4);
}

void CSimpleGUIDlg::OnRadio6Clicked() 
{
	CheckNumber(5);
}

void CSimpleGUIDlg::OnRadio7Clicked() 
{
	CheckNumber(6);
}

void CSimpleGUIDlg::OnRadio8Clicked() 
{
	CheckNumber(7);
}

void CSimpleGUIDlg::OnRadio9Clicked() 
{
	CheckNumber(8);
}
void CSimpleGUIDlg::SetChecked(int num) {
	m_radio1.SetCheck(0);
	m_radio2.SetCheck(0);
	m_radio3.SetCheck(0);
	m_radio4.SetCheck(0);
	m_radio5.SetCheck(0);
	m_radio6.SetCheck(0);
	m_radio7.SetCheck(0);
	m_radio8.SetCheck(0);
	switch (num) {
	case 1: m_radio1.SetCheck(1);
			break;
	case 2:	m_radio2.SetCheck(1);
			break;
	case 3:	m_radio3.SetCheck(1);
			break;
	case 4:	m_radio4.SetCheck(1);
			break;
	case 5:	m_radio5.SetCheck(1);
			break;
	case 6:	m_radio6.SetCheck(1);
			break;
	case 7:	m_radio7.SetCheck(1);
			break;
	case 8:	m_radio8.SetCheck(1);
			break;

	}

}

void CSimpleGUIDlg::CheckNumber(int num) {
	SetChecked(num);
	if (num == randomValue) {
		m_txtData += "You got it!";
		m_txtData += RETURN;
		GetNewValue();
	}
	else {
		m_txtData += "Wrong - Try Again!";
		m_txtData += RETURN;
	}
	UpdateData(false);
	
}

void CSimpleGUIDlg::GetNewValue() {
	randomValue = rand() % 8;
	if (randomValue == 0) {
		GetNewValue();
	}
}

void CSimpleGUIDlg::Clear() {
	m_txtData = "";
	UpdateData(false);

}
