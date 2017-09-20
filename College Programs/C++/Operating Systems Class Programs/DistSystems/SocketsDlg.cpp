// SocketsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sockets.h"
#include "SocketsDlg.h"
#include "time.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define UPDATE_DATA WM_USER+1
#define BASEPORT 5000
#define RETURN "\r\n"
#define NUMBEROFPROCESSES 6
HWND winHandle;
CString gMessageLog;
HWND sClock;
HWND sRequest;
HWND sAlive;
HWND sReply;
HWND sSend;
HWND sLog;
int RequestQueue[NUMBEROFPROCESSES] = {-1, -1, -1,-1,-1,-1};
bool waitingForReply;
int availRequest;
int topOfQueue;
long myclock;
bool AliveQueue[NUMBEROFPROCESSES] = {true,true,true,true,true,true};
bool ReplyQueue[NUMBEROFPROCESSES] = {false,false,false,false,false,false};
bool startFlag;
bool exitFlag;
int gProcessID;
/////////////////////////////////////////////////////////////////////////////
// CSocketsDlg dialog

CSocketsDlg::CSocketsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketsDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CSocketsDlg)
	processID = 0;
	strMessageLog = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketsDlg)
	DDX_Text(pDX, txtProcID, processID);
	DDX_Text(pDX, txtMessageLog, strMessageLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketsDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketsDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(mnuClear, OnmnuClear)
	ON_COMMAND(mnuExit, OnmnuExit)
	ON_BN_CLICKED(cmdStart, cmdStart_ButtonClick)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(UPDATE_DATA, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketsDlg message handlers

BOOL CSocketsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	winHandle = GetSafeHwnd();
	sClock  = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	sRequest = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	sAlive  = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	sReply  = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	sSend  = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	sLog  = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	myclock = 0;
	topOfQueue = 0;
	availRequest = 0;
	startFlag = false;
	exitFlag = false;
	gProcessID = 0;
	waitingForReply = false;
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void  CSocketsDlg::popMessage(CString mymess) {
	MessageBox(mymess, NULL, MB_OK);
}
void CSocketsDlg::OnUpdate(WPARAM iFlag, LPARAM strMess) {
	
	strMessageLog = gMessageLog;
	UpdateData(false);
}

void sendMessage(CString message, int destinationProcessID) {
	
	WaitForSingleObject(sSend, INFINITE);

	SOCKET mysocket;
	sockaddr_in remote_server;
	HOSTENT *host;
	WORD 		versionRequested;
	WSADATA	wsaData;

    long nRet;
	char outbuffer[100];
	char localhostname[100] = "";
	//HOSTENT FAR * namestruct;

	//Get data from message parameter
	strcpy(outbuffer, message);

	//Set up Winsock
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	
	//Get host and localhost information
	host=gethostbyname("localhost");
	//host = gethostname(localhostname, sizeof(localhostname));
	
	//Fill in Sock_addr_in structure
	ZeroMemory(&remote_server, sizeof(SOCKADDR_IN));
	//We are calling the localhost so copy the address from the host structure to remote address structure
	CopyMemory(&remote_server.sin_addr, host->h_addr_list[0], sizeof(host->h_addr_list[0]));//host->h_length
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons((u_short) BASEPORT + destinationProcessID);

	//Create Socket
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

	//Connect
	nRet = connect(mysocket, (SOCKADDR*) &remote_server, sizeof(sockaddr_in));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError("bind() error",WSAGetLastError());
		closesocket(mysocket);
		ReleaseSemaphore(sSend, 1, &nRet);
		return;
	}

	//Communcation
	//chars_read = recv(mysocket, inbuffer, 100, 0);
	LogEvent("Sending message to Process %d", destinationProcessID);
	LogEvent("Message Says:");
	LogEvent("%s", outbuffer);
	nRet = send(mysocket, outbuffer, sizeof(outbuffer), 0);
	
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError("bind() error", WSAGetLastError());
	}
	
	closesocket(mysocket);
	WSACleanup();
	ReleaseSemaphore(sSend, 1, &nRet);

}//end of sendMessage function

void criticalsection() {
	char buffer[10];
	unsigned long dwRead = 0;
	int value=0;
	HANDLE fileHandle;
	
	//Open File
	LogEvent("Entering Critical Section on clock cycle - %lu", myclock);
	fileHandle =  CreateFile("c:\\temp\\cs345shared.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS ,0,  NULL);
	if (fileHandle != INVALID_HANDLE_VALUE) {
		if (ReadFile(fileHandle, buffer, 4, &dwRead, NULL)) {
			if (dwRead > 0) {
				value = atoi(buffer);
			}
			else value = 0;
		}
		else value = 0;
		CloseHandle(fileHandle);
	}
	value++;
	fileHandle =  CreateFile("c:\\temp\\cs345shared.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS ,0,  NULL);
	if (fileHandle != INVALID_HANDLE_VALUE) {
		itoa(value, buffer, 10);
		strcat(buffer, "");
		WriteFile(fileHandle, buffer,4, &dwRead, NULL);
		CloseHandle(fileHandle);
	}
}//END OF CRITICAL SECTION FUNCTION

void insertRequest(int pID, int tempclock) {
	RequestQueue[pID-1]  = tempclock;	
}
void incrementRequest() {
	topOfQueue++;
	if (topOfQueue == 0) topOfQueue = 0;
}
void thrdRequest(LPVOID lpvThreadParam) {
	long i;
	bool waiting = true;
	CString outbuffer;
	char tempclock[100];

	while (!exitFlag) {
		Sleep(4000);
		LogEvent("Current Process Requesting Access to Critical Section");
		WaitForSingleObject(sRequest, INFINITE);
		WaitForSingleObject(sReply, INFINITE);
		WaitForSingleObject(sClock, INFINITE);
		insertRequest(gProcessID, myclock);

		for (i=1; i<= NUMBEROFPROCESSES; i++) {
			if (i != gProcessID) {
				ReplyQueue[i-1] = false;
				outbuffer = "REQUEST:";
				outbuffer += ltoa(gProcessID,tempclock, 10);
				outbuffer += ":";
				outbuffer += ltoa(myclock, tempclock, 10);
				sendMessage(outbuffer, i);
			}
		}
		waitingForReply = true;
		myclock++;
		ReleaseSemaphore(sClock, 1, &i);
		ReleaseSemaphore(sReply, 1, &i);
		ReleaseSemaphore(sRequest, 1, &i);
		while (waiting) {
			waiting = false;
			WaitForSingleObject(sReply, INFINITE);
			WaitForSingleObject(sAlive, INFINITE);
			for (i=0; i<NUMBEROFPROCESSES; i++) {
				if ((i+1) != gProcessID) {
					if (AliveQueue[i]) {
						if (ReplyQueue[i] == false) waiting = true;
					}
				}
			}
			ReleaseSemaphore(sAlive, 1, &i);
			ReleaseSemaphore(sReply, 1, &i);
		}
		waiting = true;
		WaitForSingleObject(sClock, INFINITE);
		criticalsection();
		ReleaseSemaphore(sClock, 1, &i);
		WaitForSingleObject(sRequest, INFINITE);
		RequestQueue[gProcessID -1] = -1;
		ReleaseSemaphore(sRequest, 1, &i);
	}
	ExitThread(0);
}//end of Request thread

void thrdReply(LPVOID lpvThreadParam) {
	long i,smallv,smallclock;
	
	
	CString outbuffer;
	char tempclock[100];
	
	while (!exitFlag) {
		WaitForSingleObject(sRequest, INFINITE);
		WaitForSingleObject(sClock, INFINITE);
		smallv = 1;
		smallclock =-2;
		for (i=0;i<NUMBEROFPROCESSES; i++) {
			if (((RequestQueue[i] < RequestQueue[smallv-1]) || (smallclock == -2)) && (RequestQueue[i] != -1)) {
				smallv = i+1;
				smallclock = RequestQueue[i];
			}
		}
		if (smallv != gProcessID && smallclock > -1) {
			outbuffer = "REPLY:";
			outbuffer += ltoa(gProcessID,tempclock, 10);
			outbuffer += ":";
			outbuffer += ltoa(myclock, tempclock, 10);
			sendMessage(outbuffer, smallv);
			LogEvent("Sending a Reply to %d requested at %d", smallv, RequestQueue[smallv-1]);
			RequestQueue[smallv-1] = -1;
			myclock++;
		}
		ReleaseSemaphore(sClock, 1, &i);
		ReleaseSemaphore(sRequest, 1, &i);
	}
	ExitThread(0);

}//end of Reply thread

void thrdAlive(LPVOID lpvThreadParam) {
	long i;
	CString outbuffer;
	char tempclock[100];
	
	while (!exitFlag) {
		Sleep(4000);
		LogEvent("Checking for the Death of Friendly Threads");
		WaitForSingleObject(sAlive, INFINITE);
		for (i=1; i<= NUMBEROFPROCESSES; i++) {
			if (i != gProcessID) {
				outbuffer = "ALIVE:";
				outbuffer += ltoa(gProcessID,tempclock, 10);
				outbuffer += ":";
				outbuffer += ltoa(myclock, tempclock, 10);
				sendMessage(outbuffer, i);
				AliveQueue[i-1] = false;
			}
		}
		Sleep(4000);
		ReleaseSemaphore(sAlive, 1, &i);
		
	}
	ExitThread(0);
}//end of Alive thread

void thrdServer(LPVOID lpvThreadParam) {
	DWORD threadid;
	HANDLE threadHandle;
	SOCKET connectSocket, listenSocket;
	WORD 	versionRequested;
	WSADATA	wsaData;
	int nRet;
	sockaddr_in clientName;
	LPSocket structSocket;
	int sockaddrSize;
	sockaddr_in myserver;
	int i;
	CString outbuffer;
	char tempclock[100];
	
	//Set up Socket information
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	ZeroMemory(&myserver, sizeof(SOCKADDR_IN));
	myserver.sin_family = AF_INET;
	myserver.sin_port = htons((u_short) BASEPORT + gProcessID);
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (listenSocket == INVALID_SOCKET)
	{
		LogWinSockError("Could not create listen socket",WSAGetLastError());
		ExitThread(1);
	}

	nRet = bind(listenSocket, (SOCKADDR*) &myserver, sizeof(sockaddr_in));

	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError("bind() error",WSAGetLastError());
		closesocket(listenSocket);
		ExitThread(1);
	}

	nRet = listen(listenSocket, SOMAXCONN);
	
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError("listen() error", WSAGetLastError());
		ExitThread(1);
	}

	if (gProcessID == 6) {
		for (i=1; i<= NUMBEROFPROCESSES; i++) {
			if (i != gProcessID) {
				outbuffer = "GO:";
				outbuffer += ltoa(gProcessID,tempclock, 10);
				outbuffer += ":";
				outbuffer += ltoa(myclock, tempclock, 10);
				sendMessage(outbuffer, i);
			}
		}
		startThreads();
	}

	//Loop and accept connections until told to exit
	while (!exitFlag) {
		sockaddrSize=sizeof(SOCKADDR_IN);
		connectSocket = accept(listenSocket, (SOCKADDR*) &clientName, &sockaddrSize);
		structSocket =  new Socket();
		memcpy((void*)&structSocket->Socket,&connectSocket, sizeof(connectSocket));
		if (connectSocket != INVALID_SOCKET) {
			threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdMessage, structSocket, 0, &threadid);
			CloseHandle(threadHandle);
		}
		Sleep(200);
	}
	closesocket(listenSocket);
}

void startThreads() {
	DWORD threadid;
	HANDLE threadHandle;
	int param = 0;

	//Start the thread to check if who is alive
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdAlive, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);

	//Start the thread to Reply
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdReply, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);

	//Start the RequestThread
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdRequest, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);
	
}

void thrdMessage(LPVOID lpvThreadParam) {	
	SOCKET connectSocket = ((LPSocket) lpvThreadParam)->Socket;
	char inbuffer[250];
	CString message1;
	char strSender[10];
	char strClock[100];
	char message[100];
	int iSender;
	int iClock;
	long i;
	long chars_read;
	CString outbuffer;
	char tempclock[100];
	
	chars_read = recv(connectSocket, inbuffer, 250, 0);
	inbuffer[chars_read] = 0;
	

	//Parse Message
	message1.Format("%s", inbuffer);
	
	if (message1.Find(":", 0) != -1) {
		strcpy(message,strtok(inbuffer, ":"));	
		strcpy(strSender,strtok(NULL, ":"));
		strcpy(strClock,strtok(NULL, ":"));
		iSender = atoi(strSender);
		iClock = atoi(strClock);
		if (strcmp(message, "REQUEST") == 0) {
			WaitForSingleObject(sRequest, INFINITE);
			WaitForSingleObject(sReply, INFINITE);
			WaitForSingleObject(sClock, INFINITE);
			LogEvent("Message Received -- %s from %d", inbuffer, iSender);
			if (myclock < iClock) {
				myclock = iClock + 1;
			}
			else {
				myclock++;
			}
				//outbuffer = "REPLY:";
				//outbuffer += ltoa(gProcessID,tempclock, 10);
				//outbuffer += ":";
				//outbuffer += ltoa(myclock, tempclock, 10);
				//sendMessage(outbuffer, iSender);
				//LogEvent("Reply Sent immediately to Request for %d", iSender);
			insertRequest(iSender, iClock);
			LogEvent("Request Queued");
			ReleaseSemaphore(sClock, 1, &i);
			ReleaseSemaphore(sReply, 1, &i);
			ReleaseSemaphore(sRequest, 1, &i);
		}
		else if (strcmp(message, "GO") == 0) {
			startThreads();
		}
		else if (strcmp(message, "REPLY") == 0) {
			WaitForSingleObject(sReply, INFINITE);
			WaitForSingleObject(sClock, INFINITE);
			LogEvent("Message Received -- %s from %d", inbuffer, iSender);
			ReplyQueue[iSender-1] = true;
			if (myclock < iClock) myclock = iClock + 1;
			else myclock++;
			ReleaseSemaphore(sClock, 1, &i);
			ReleaseSemaphore(sReply, 1, &i);
		}
		else if (strcmp(message, "ALIVE") == 0) {
			LogEvent("Message Received -- %s from %d", inbuffer, iSender);
			WaitForSingleObject(sAlive, INFINITE);
			AliveQueue[iSender-1] = true;
			ReleaseSemaphore(sAlive, 1, &i);
		}
	}
	else {
		LogEvent("Invalid Message");	
	}
		
	
	//Clean-up	
	closesocket(connectSocket);
	ExitThread(0);
}

void LogEvent(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[256];
	long i;
	
	// Write text to string
	// and append to edit control
	WaitForSingleObject(sLog, INFINITE);
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	if (strcmp(szBuf, "") == 0) {
		gMessageLog = "";
	}
	else {
		gMessageLog += szBuf;
		gMessageLog += RETURN;
	}

	PostMessage(winHandle, UPDATE_DATA,0,0);
	ReleaseSemaphore(sLog, 1, &i);
}


void LogWinSockError(LPCSTR lpText, int nErrorCode)
{
	char szBuf[256];

	szBuf[0] = '\0';
	LogEvent("%s : %s", lpText, szBuf);
}


void CSocketsDlg::OnmnuClear() 
{
	
	LogEvent("");
	UpdateData(false);
	
}

void CSocketsDlg::OnmnuExit() 
{
	exitFlag =1;
	
}


void CSocketsDlg::cmdStart_ButtonClick() 
{
	DWORD threadid;
	HANDLE threadHandle;
	int param=0;

	UpdateData(true);
	gProcessID = processID;
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdServer, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);
	
}
