// Lab2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Lab2.h"
#include "Lab2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLab2App

BEGIN_MESSAGE_MAP(CLab2App, CWinApp)
	//{{AFX_MSG_MAP(CLab2App)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLab2App construction

CLab2App::CLab2App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLab2App object

CLab2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CLab2App initialization

BOOL CLab2App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

	CLab2Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
