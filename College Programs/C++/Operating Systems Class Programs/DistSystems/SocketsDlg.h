// SocketsDlg.h : header file
//

#if !defined(AFX_SOCKETSDLG_H__6FFBFBEE_2042_4159_A5B8_D6D805885DAD__INCLUDED_)
#define AFX_SOCKETSDLG_H__6FFBFBEE_2042_4159_A5B8_D6D805885DAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSocketsDlg dialog

class CSocketsDlg : public CDialog
{
// Construction
public:
	CSocketsDlg(CWnd* pParent = NULL);	// standard constructor
	void  CSocketsDlg::popMessage(CString mymess) ;
// Dialog Data
	//{{AFX_DATA(CSocketsDlg)
	enum { IDD = IDD_SOCKETS_DIALOG };
	int		processID;
	CString	strMessageLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSocketsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnmnuClear();
	afx_msg void OnmnuExit();
	afx_msg void cmdStart_ButtonClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    void OnUpdate(WPARAM, LPARAM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
void thrdServer(LPVOID lpvThreadParam);
void thrdReply(LPVOID lpvThreadParam);
void thrdRequest(LPVOID lpvThreadParam);
void thrdAlive(LPVOID lpvThreadParam);
void thrdMessage(LPVOID lpvThreadParam);
void criticalsection();
void LogEvent(LPCSTR lpFormat, ...);
void LogWinSockError(LPCSTR lpText, int nErrorCode);
void sendMessage(CString message ,int destinationProcessID);
void insertRequest(int pID, int tempclock);
void startThreads();
//
// CONNECTION structure
//
typedef struct tagREQUEST
{
  HANDLE		hExit;
  SOCKET		Socket;
  int			nMethod;
  DWORD			dwConnectTime;
  DWORD			dwRecv;
  DWORD			dwSend;
  HANDLE		hFile;
  char			szFileName[_MAX_PATH];
}Socket, *LPSocket;

#endif // !defined(AFX_SOCKETSDLG_H__6FFBFBEE_2042_4159_A5B8_D6D805885DAD__INCLUDED_)
