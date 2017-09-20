// SimpleGUI.h : main header file for the SIMPLEGUI application
//

#if !defined(AFX_SIMPLEGUI_H__4159B2C4_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_)
#define AFX_SIMPLEGUI_H__4159B2C4_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIApp:
// See SimpleGUI.cpp for the implementation of this class
//

class CSimpleGUIApp : public CWinApp
{
public:
	CSimpleGUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleGUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimpleGUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEGUI_H__4159B2C4_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_)
