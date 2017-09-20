// VirtualMem.h : main header file for the VIRTUALMEM application
//

#if !defined(AFX_VIRTUALMEM_H__609AED77_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_)
#define AFX_VIRTUALMEM_H__609AED77_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemApp:
// See VirtualMem.cpp for the implementation of this class
//

class CVirtualMemApp : public CWinApp
{
public:
	CVirtualMemApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualMemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVirtualMemApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALMEM_H__609AED77_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_)
