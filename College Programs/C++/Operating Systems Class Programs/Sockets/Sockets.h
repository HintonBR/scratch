// Sockets.h : main header file for the SOCKETS application
//

#if !defined(AFX_SOCKETS_H__F62D8A35_0A57_40E3_9D3F_7D6E7B7F2F00__INCLUDED_)
#define AFX_SOCKETS_H__F62D8A35_0A57_40E3_9D3F_7D6E7B7F2F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSocketsApp:
// See Sockets.cpp for the implementation of this class
//

class CSocketsApp : public CWinApp
{
public:
	CSocketsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSocketsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETS_H__F62D8A35_0A57_40E3_9D3F_7D6E7B7F2F00__INCLUDED_)
