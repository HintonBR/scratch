// Sync2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sync2.h"
#include "Sync2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSync2App

BEGIN_MESSAGE_MAP(CSync2App, CWinApp)
	//{{AFX_MSG_MAP(CSync2App)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSync2App construction

CSync2App::CSync2App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSync2App object

CSync2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CSync2App initialization

BOOL CSync2App::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSync2Dlg dlg;
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
