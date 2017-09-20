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
#define SERVERPORT 3030
#define LOCALSERVERPORT 3031
#define RETURN "\r\n"
HWND winHandle;
CString gstrClient;
CString gstrServer;
CString strhostname;
HWND clientmutex;
HWND servermutex;
HWND listener;
sockaddr_in myserver;
bool serverStarted;
int random;
/////////////////////////////////////////////////////////////////////////////
// CSocketsDlg dialog

CSocketsDlg::CSocketsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketsDlg)
	strClient = _T("");
	strServer = _T("");
	strHostName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketsDlg)
	DDX_Text(pDX, IDC_EDIT1, strClient);
	DDX_Text(pDX, IDC_EDIT2, strServer);
	DDX_Text(pDX, IDC_EDIT3, strHostName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketsDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketsDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnClient)
	ON_BN_CLICKED(IDCANCEL, OnServer)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnKillfocusHostName)
	ON_COMMAND(mnuClear, OnmnuClear)
	ON_COMMAND(mnuExit, OnmnuExit)
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
	strhostname = strHostName = "orion-16.cs.byu.edu";
	clientmutex = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	servermutex = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	listener = (HWND) CreateSemaphore(NULL, 1,1, NULL);
	serverStarted = false;
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

void CSocketsDlg::OnClient() 
{
	DWORD threadid;
	HANDLE threadHandle;
	char* param = new char(3);
	//char temp[2];
	//itoa(i, temp, 10);
	//strcpy(param, temp);
	
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdClient, (LPVOID) this, 0, &threadid);
	CloseHandle(threadHandle);
	//MessageBox("This is to seem if we made it here", NULL, MB_OK);
	
}

void CSocketsDlg::OnServer() 
{
	DWORD threadid;
	HANDLE threadHandle;
	char* param = new char(3);
	sockaddr_in remote_server;
	SOCKET mysocket;
	HOSTENT *host;
	WORD 		versionRequested;
	WSADATA	wsaData;
    int nRet;
	char inbuffer[100];
	char outbuffer[100];
	int chars_read;
	char server_name[100];
	char localhostname[100] = "";
	
	strcpy(server_name, strhostname);
	
	//Set up Winsock
	if (serverStarted == false) {
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	}

	//Get host and localhost information
	host=gethostbyname(server_name);
	gethostname(localhostname, sizeof(localhostname));
	
	//Fill in Sock_addr_in structure
	ZeroMemory(&remote_server, sizeof(SOCKADDR_IN));
	CopyMemory(&remote_server.sin_addr, host->h_addr_list[0], sizeof(host->h_addr_list[0]));//host->h_length
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons((u_short) 3030);

	//Create Socket
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

	//Connect
	nRet = connect(mysocket, (SOCKADDR*) &remote_server, sizeof(sockaddr_in));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(winHandle,
						 "bind() error",
						 WSAGetLastError());
		closesocket(mysocket);
		ExitThread(0);
	}
	//Create thread to receive client request
	if (serverStarted == false) {
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdServer, (LPVOID) param, 0, &threadid);
	CloseHandle(threadHandle);
	}

	//Communcation
	LogEvent(winHandle, 2, "CONNECTING....");
	chars_read = recv(mysocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	//LogEvent(winHandle, 0, "The server says....");
	//LogEvent(winHandle, 1, inbuffer);
	strcpy(outbuffer,"REQ:CLIENT:3031:Bryan");
    LogEvent(winHandle, 2, "%s says....", "Bryan");
	LogEvent(winHandle, 3, "%s", outbuffer);
	send(mysocket, outbuffer, sizeof(outbuffer), 0);

	//Receive request
	chars_read = recv(mysocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	LogEvent(winHandle, 2, "The Remote says....");
	LogEvent(winHandle, 3, inbuffer);
	
	//close socket
	closesocket(mysocket);
	//WSACleanup();

	
}
void  CSocketsDlg::sendMessage(CString mymess) {
	MessageBox(mymess, NULL, MB_OK);
}
void CSocketsDlg::OnUpdate(WPARAM iFlag, LPARAM strMess) {

	if (iFlag == 0) {
		strClient = gstrClient;	
	}
	else {
		strServer = gstrServer;
	}
	UpdateData(false);
}
void thrdClient(LPVOID lpvThreadParam) {
	SOCKET mysocket;
	sockaddr_in remote_server;
	HOSTENT *host;
	WORD 		versionRequested;
	WSADATA	wsaData;
    int nRet;
	char inbuffer[100];
	char outbuffer[100];
	int chars_read;
	char server_name[100];
	char localhostname[100] = "";
	char request[100];
	char*  data = new char(100);
	HOSTENT FAR * namestruct;

	//((CSocketsDlg*)lpvThreadParam)->sendMessage("This is to seem if we made it here");
	
	strcpy(server_name, strhostname);
	//strcpy(server_name, strhostname);
	//Set up Winsock
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	
	//Get host and localhost information
	host=gethostbyname(server_name);
	gethostname(localhostname, sizeof(localhostname));
	
	//Fill in Sock_addr_in structure
	ZeroMemory(&remote_server, sizeof(SOCKADDR_IN));
	CopyMemory(&remote_server.sin_addr, host->h_addr_list[0], sizeof(host->h_addr_list[0]));//host->h_length
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons((u_short) 3030);

	//Create Socket
    mysocket = socket(AF_INET, SOCK_STREAM, 0);

	//Connect
	nRet = connect(mysocket, (SOCKADDR*) &remote_server, sizeof(sockaddr_in));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(winHandle,
						 "bind() error",
						 WSAGetLastError());
		closesocket(mysocket);
		ExitThread(0);
	}

	//Communcation
	LogEvent(winHandle, 0, "CONNECTING....");
	chars_read = recv(mysocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	LogEvent(winHandle, 0, "The server says....");
	LogEvent(winHandle, 1, inbuffer);
	strcpy(outbuffer,"REQ:SERVER:Bryan");
    LogEvent(winHandle, 0, "%s says....", localhostname);
	LogEvent(winHandle, 1, "%s", outbuffer);
	send(mysocket, outbuffer, sizeof(outbuffer), 0);

	//Receive and decode request
	chars_read = recv(mysocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	LogEvent(winHandle, 0, "The server says....");
	LogEvent(winHandle, 1, inbuffer);
	strtok(inbuffer, ":");
	strcpy(request,strtok(NULL, ":"));
	
	switch (*request) {
	case 'D':
			strcpy(outbuffer, "ACK:");
			_strdate(data);
			strcat(outbuffer, data);
			break;
	case 'T':
			strcpy(outbuffer, "ACK:");
			_strtime(data);
			strcat(outbuffer, data);
			break;
	case 'H':
			strcpy(outbuffer, "ACK:");
			strcat(outbuffer, localhostname);
			break;
	case 'I':
			
			in_addr x;
			namestruct = gethostbyname(localhostname);
			memcpy(&x.S_un.S_addr,namestruct->h_addr_list[0], 4);
			strcpy(outbuffer, "ACK:");
			strcpy(data,inet_ntoa(x));
			strcat(outbuffer, data);
			break;
	case 'M':
			MEMORYSTATUS lpMem;
			GlobalMemoryStatus(&lpMem);
			strcpy(outbuffer, "ACK:");
			ZeroMemory(data, strlen(data));
			_ultoa(lpMem.dwTotalPhys, data, 10 );
			strcat(outbuffer, data);
			break;
	
	}
	
	//Send requested data
	LogEvent(winHandle, 0, "%s says....", localhostname);
	LogEvent(winHandle, 1, "%s", outbuffer);
	send(mysocket, outbuffer, sizeof(outbuffer), 0);

	//Receive acknowledgement
	chars_read = recv(mysocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	LogEvent(winHandle, 0, "The server says....");
	LogEvent(winHandle, 1, inbuffer);


	closesocket(mysocket);
	WSACleanup();
	ExitThread(0);

}//end of Client thread



void thrdServer(LPVOID lpvThreadParam) {
	DWORD threadid;
	HANDLE threadHandle;
	SOCKET connectSocket, listenSocket;
	WORD 	versionRequested;
	WSADATA	wsaData;
	int nRet;
	sockaddr_in clientName;
	LPSocket structSocket;
	
	versionRequested = MAKEWORD(2,0);
	WSAStartup(versionRequested, &wsaData);
	
	serverStarted = true;
	ZeroMemory(&myserver, sizeof(SOCKADDR_IN));
	myserver.sin_family = AF_INET;
	myserver.sin_port = htons((u_short) LOCALSERVERPORT);
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);
	
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (listenSocket == INVALID_SOCKET)
	{
		LogWinSockError(winHandle, 
						"Could not create listen socket",
						WSAGetLastError());
		ExitThread(1);
	}

	nRet = bind(listenSocket, (SOCKADDR*) &myserver, sizeof(sockaddr_in));

	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(winHandle,
						 "bind() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		ExitThread(1);
	}

	nRet = listen(listenSocket, SOMAXCONN);
	
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(winHandle,
						 "listen() error",
						 WSAGetLastError());
		ExitThread(1);
	}
		
	while (1) {
	
	int temp=sizeof(SOCKADDR_IN);
	connectSocket = accept(listenSocket, (SOCKADDR*) &clientName, &temp);
	structSocket =  new Socket();
	structSocket->Socket = connectSocket;
	if (connectSocket != INVALID_SOCKET) {
	threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &thrdServerClient, structSocket, 0, &threadid);
	CloseHandle(threadHandle);
	}
	Sleep(200);
	}
	closesocket(listenSocket);
}
void thrdServerClient(LPVOID lpvThreadParam) {	
	SOCKET connectSocket = ((LPSocket) lpvThreadParam)->Socket;
	int nRet;
	char inbuffer[50];
	char outbuffer[50];
	int chars_read;
	char* tempStr = new char(100);
	char localhostname[50] = "";
	char  data[100];
	HOSTENT FAR * namestruct1;
	in_addr x;
	char* ipaddr;
	//long hostaddr;
	long hour2;
	long minute2;
	long second2;
	double test;
	CTime clientTime = CTime::GetCurrentTime();

	
	//Communcation
	strcpy(outbuffer,"Welcome to Bryan's SERVER");
    LogEvent(winHandle, 2, "%s says....", "Bryan");
	LogEvent(winHandle, 3, "%s", outbuffer);
	nRet = send(connectSocket, outbuffer, sizeof(outbuffer), 0);

	if (nRet == -1) {
			LogWinSockError(winHandle,
						 "listen() error",
						 WSAGetLastError());
		ExitThread(1);

	}
	//Receive and decode request

	chars_read = recv(connectSocket, inbuffer, 100, 0);
	inbuffer[chars_read] = 0;
	LogEvent(winHandle, 2, "The Client says....");
	LogEvent(winHandle, 3, inbuffer);


	//strtok(inbuffer, ":");
	//request = strtok(NULL, ":");
		CTime clientTime2;
		int count; 
		count = time(0) + random;
		//LogEvent(winHandle, 3, "The rand value is %d", count);
	switch (count % 5) {
	case 0:
			//Communcation
			strcpy(outbuffer,"REQ:DATE");
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);

			//Receive and decode request
			chars_read = recv(connectSocket, inbuffer, 100, 0);
			inbuffer[chars_read] = 0;
			LogEvent(winHandle, 2, "The Client says....");
			LogEvent(winHandle, 3, inbuffer);
			
			_strdate(data);
			strcpy(tempStr, inbuffer);
			tempStr += 4;
			if (strcmp(tempStr, data) == 0) {
				strcpy(outbuffer, "ACK:VALIDATED:BRYAN");	
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");	
			}
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);
			random = 1;
			break;
	case 1:
			//Communcation
			strcpy(outbuffer,"REQ:TIME");
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);

			//Receive and decode request
			chars_read = recv(connectSocket, inbuffer, 100, 0);
			inbuffer[chars_read] = 0;
			LogEvent(winHandle, 2, "The Client says....");
			LogEvent(winHandle, 3, inbuffer);
			
			_strtime(data);
			strcpy(tempStr, inbuffer);
			tempStr += 4;
			test = strlen(tempStr);
			count = 0;
			while (test != 0) {
				if (isdigit(tempStr[count]) == false && tempStr[count] != ':') break;
				count++;
				test--;
			}
			if (test == 0) {
			hour2 = atol(strtok(tempStr, ":"));
			minute2 = atol(strtok(NULL, ":"));
			second2 = atol(strtok(NULL, ":"));
			clientTime2 = CTime(clientTime.GetYear(), clientTime.GetMonth(), clientTime.GetDay(), hour2, minute2, second2);
			
			if (difftime(time(0), clientTime2.GetTime()) <= 600 && difftime(time(0), clientTime2.GetTime()) >= -600) {
			
				strcpy(outbuffer, "ACK:VALIDATED:BRYAN");	
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");	
			}
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");	
			}
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);
			random = 2;
			break;
	case 2:
			//Communcation
			strcpy(outbuffer,"REQ:HOSTNAME");
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);

			//Receive and decode request
			chars_read = recv(connectSocket, inbuffer, 100, 0);
			inbuffer[chars_read] = 0;
			LogEvent(winHandle, 2, "The Client says....");
			LogEvent(winHandle, 3, inbuffer);
			
			strcpy(tempStr, inbuffer);
			tempStr += 4;
			//hostaddr = inet_addr(inet_ntoa(clientName.sin_addr));
			//namestruct1 = gethostbyaddr((const char*) &hostaddr, 4, AF_INET);
			strcpy(data, strhostname);
			if (strcmp(tempStr, strtok(data, ".")) == 0) {
				strcpy(outbuffer, "ACK:VALIDATED:BRYAN");
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");
			}
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);
			random = 3;
			break;
	case 3:
			//Communcation
			strcpy(outbuffer,"REQ:IPADDR");
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);

			//Receive and decode request
			chars_read = recv(connectSocket, inbuffer, 100, 0);
			inbuffer[chars_read] = 0;
			LogEvent(winHandle, 2, "The Client says....");
			LogEvent(winHandle, 3, inbuffer);
			
			//strcpy(data, inet_ntoa(clientName.sin_addr));
			namestruct1 = gethostbyname(strhostname);
			memcpy(&x.S_un.S_addr, namestruct1->h_addr_list[0], 4);
			ipaddr = inet_ntoa(x);
			strcpy(tempStr, inbuffer);
			tempStr += 4;
			if (strcmp(tempStr, ipaddr) == 0) {
				strcpy(outbuffer, "ACK:VALIDATED:BRYAN");	
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");	
			}
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);
			random = 4;
			break;
	case 4:
			//Communcation
			strcpy(outbuffer,"REQ:MEM");
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);

			//Receive and decode request
			chars_read = recv(connectSocket, inbuffer, 100, 0);
			inbuffer[chars_read] = 0;
			LogEvent(winHandle, 2, "The Client says....");
			LogEvent(winHandle, 3, inbuffer);
			strcpy(tempStr, inbuffer);
			tempStr += 4;
			test = strlen(tempStr);
			count = 0;
			while (test != 0) {
				if (isdigit(tempStr[count]) == false) break;
				count++;
				test--;
			}
			if (test == (float) 0) {
				strcpy(outbuffer, "ACK:VALIDATED:BRYAN");	
			}
			else {
				strcpy(outbuffer, "ACK:INVALID DATA");	
			}
			LogEvent(winHandle, 2, "%s says....", "Bryan");
			LogEvent(winHandle, 3, "%s", outbuffer);
			send(connectSocket, outbuffer, sizeof(outbuffer), 0);
			random = 4;
			break;
	
	}
	
	//Clean-up	
	closesocket(connectSocket);
	//tempStr -= 4;
	//delete tempStr;
	//delete lpvThreadParam;
	ExitThread(0);
}


////////////////////////////////////////////////////////////

void LogEvent(HWND hwnd, int source, LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[256];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	if (source == 0) {
		WaitForSingleObject(clientmutex, INFINITE);
		gstrClient += (CString) szBuf + RETURN;
		ReleaseSemaphore(clientmutex, 1, NULL);
	}
	else if (source == 1) {
		WaitForSingleObject(clientmutex, INFINITE);
		gstrClient += (CString) szBuf + RETURN + RETURN;
		ReleaseSemaphore(clientmutex, 1, NULL);
	}
	else if (source == 2) {
		WaitForSingleObject(servermutex, INFINITE);
		gstrServer += (CString) szBuf + RETURN;
		ReleaseSemaphore(servermutex, 1, NULL);
	}
	else if (source == 3) {
		WaitForSingleObject(servermutex, INFINITE);
		gstrServer += (CString) szBuf + RETURN + RETURN;
		ReleaseSemaphore(servermutex, 1, NULL);
	}
	PostMessage(hwnd, UPDATE_DATA,
				(WPARAM) source,
				0);
}

////////////////////////////////////////////////////////////

void LogWinSockError(HWND hwnd, LPCSTR lpText, int nErrorCode)
{
	char szBuf[256];

	szBuf[0] = '\0';
	//LoadString(GetWindowInstance(hwnd),
	//		   nErrorCode,
	//		   szBuf,
	//		   sizeof(szBuf));
	LogEvent(hwnd, 0,"%s : %s", lpText, szBuf);
}

void CSocketsDlg::OnKillfocusHostName() 
{
	UpdateData(true);
	strhostname = strHostName;
}

void CSocketsDlg::OnmnuClear() 
{
	strClient = "";
	gstrClient = "";
	strServer = "";
	gstrServer = "";
	UpdateData(false);
	
}

void CSocketsDlg::OnmnuExit() 
{
	ExitProcess(0);
	
}

