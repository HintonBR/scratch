// Lab2Dlg.h : header file
//

#if !defined(AFX_LAB2DLG_H__D10869FA_E93B_4D60_AB3B_4C92EB3D53C9__INCLUDED_)
#define AFX_LAB2DLG_H__D10869FA_E93B_4D60_AB3B_4C92EB3D53C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLab2Dlg dialog

class CLab2Dlg : public CDialog
{
// Construction
public:
	CLab2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLab2Dlg)
	enum { IDD = IDD_LAB2_DIALOG };
	CString	m_window;
	CString	m_insert;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLab2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLab2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onplain();
	afx_msg void Onbold();
	afx_msg void Ondoitnow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAB2DLG_H__D10869FA_E93B_4D60_AB3B_4C92EB3D53C9__INCLUDED_)
