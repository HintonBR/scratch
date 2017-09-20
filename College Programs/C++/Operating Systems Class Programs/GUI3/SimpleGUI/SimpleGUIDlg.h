// SimpleGUIDlg.h : header file
//

#if !defined(AFX_SIMPLEGUIDLG_H__4159B2C6_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_)
#define AFX_SIMPLEGUIDLG_H__4159B2C6_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimpleGUIDlg dialog

class CSimpleGUIDlg : public CDialog
{
// Construction
public:
	CSimpleGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimpleGUIDlg)
	enum { IDD = IDD_SIMPLEGUI_DIALOG };
	CStatic	m_Pic;
	CButton	m_Bold;
	CButton	m_Plain;
	CEdit	m_ctlData;
	CString	m_txtData;
	CString	m_Enter;
	BOOL	m_valBold;
	HBITMAP myBitmap;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleGUIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int randomValue;

	// Generated message map functions
	//{{AFX_MSG(CSimpleGUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OncmdClearClicked();
	afx_msg void OncmdExitClicked();
	afx_msg void OnchkBold();
	afx_msg void OnchkPlain();
	afx_msg void OncmdExecute();
	afx_msg void OncmdThreadA();
	afx_msg void OncmdThreadB();
	afx_msg void OncmdThreadC();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void Clear();
	void Launch();
	void OnUpdate(WPARAM w, LPARAM l);
};
	void myThreadFunc(LPVOID lpvThreadParam);


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEGUIDLG_H__4159B2C6_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_)
