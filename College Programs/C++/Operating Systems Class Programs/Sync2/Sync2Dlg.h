// Sync2Dlg.h : header file
//

#if !defined(AFX_SYNC2DLG_H__F1FA94DA_1C2C_4211_A6EA_9850D33E481E__INCLUDED_)
#define AFX_SYNC2DLG_H__F1FA94DA_1C2C_4211_A6EA_9850D33E481E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSync2Dlg dialog

class CSync2Dlg : public CDialog
{
// Construction
public:
	CSync2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSync2Dlg)
	enum { IDD = IDD_SYNC2_DIALOG };
	CEdit	ctlConsumer;
	CEdit	txtProducer;
	CString	strProducer;
	CString	strConsumer;
	CString	strBuffer0;
	CString	strBuffer1;
	CString	strBuffer2;
	CString	strMessProduced;
	CString	strMessConsumed;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSync2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSync2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowsClicked();
	afx_msg void OnMineClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnUpdate(WPARAM w, LPARAM l);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

void consumer(LPVOID lpvThreadParam);
void producer(LPVOID lpvThreadParam);
void myconsumer(LPVOID lpvThreadParam);
void myproducer(LPVOID lpvThreadParam);
int TestAndSet(int*);
class MySemaphore {

public:
	MySemaphore(){}
	MySemaphore(int max, int count);
	int wait();
	int signal();

private:
	int maxcount;
	int curcount;
	int mutex;

};

class CBuffer {

public:
	CBuffer();
	CString pop();
	void push(CString value);
	CString print();
	HWND mutex;
	HWND full;
	HWND empty;
	MySemaphore mymutex;
	MySemaphore myfull;
	MySemaphore myempty;

private:
	CString values[5];
	int numvalues;

};


#endif // !defined(AFX_SYNC2DLG_H__F1FA94DA_1C2C_4211_A6EA_9850D33E481E__INCLUDED_)
