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
	CEdit	m_ctlData;
	CStatic	m_ctlInstructions;
	CButton	m_radio8;
	CButton	m_radio7;
	CButton	m_radio6;
	CButton	m_radio5;
	CButton	m_radio4;
	CButton	m_radio3;
	CButton	m_radio2;
	CButton	m_radio1;
	CString	m_lblInstructions;
	CString	m_txtData;
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
	afx_msg void OnRadio2Clicked();
	afx_msg void OnRadio3Clicked();
	afx_msg void OnRadio4Clicked();
	afx_msg void OnRadio5Clicked();
	afx_msg void OnRadio6Clicked();
	afx_msg void OnRadio7Clicked();
	afx_msg void OnRadio8Clicked();
	afx_msg void OnRadio9Clicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void CheckNumber(int);
	void GetNewValue();
	void SetChecked(int);
	void Clear();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEGUIDLG_H__4159B2C6_7DE4_11D4_AE11_0048546DFC2D__INCLUDED_)
