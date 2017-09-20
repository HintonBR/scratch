// VirtualMemDlg.h : header file
//

#if !defined(AFX_VIRTUALMEMDLG_H__609AED79_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_)
#define AFX_VIRTUALMEMDLG_H__609AED79_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemDlg dialog

class CVirtualMemDlg : public CDialog
{
// Construction
public:
	CVirtualMemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVirtualMemDlg)
	enum { IDD = IDD_VIRTUALMEM_DIALOG };
	CString	strViewSwap;
	CString	strViewFrame;
	CString	strFrameTable;
	CString	strPageNum;
	CString	strFrameNum;
	CString	strTotalMem;
	CString	strPageSize;
	CString	strSwapSize;
	CString	strNumHits;
	CString	strNumMisses;
	CString	strFilename;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualMemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVirtualMemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnViewPage();
	afx_msg void OnViewFrame();
	afx_msg void OnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    void OnUpdate(WPARAM w, LPARAM l);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALMEMDLG_H__609AED79_A8B5_11D4_AA77_D8BAE4D5287A__INCLUDED_)
