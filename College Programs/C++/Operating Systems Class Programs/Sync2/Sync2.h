// Sync2.h : main header file for the SYNC2 application
//

#if !defined(AFX_SYNC2_H__C42449EE_1BFF_4CC1_8557_7CDE902D32E2__INCLUDED_)
#define AFX_SYNC2_H__C42449EE_1BFF_4CC1_8557_7CDE902D32E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSync2App:
// See Sync2.cpp for the implementation of this class
//

class CSync2App : public CWinApp
{
public:
	CSync2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSync2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSync2App)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNC2_H__C42449EE_1BFF_4CC1_8557_7CDE902D32E2__INCLUDED_)
