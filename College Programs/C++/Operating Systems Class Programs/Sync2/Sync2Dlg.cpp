// Sync2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sync2.h"
#include "Sync2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define RETURN "\r\n"
#define messconsumed "Messages Consumed = "
#define messproduced "Messages Produced = "
#define UPDATE_DATA WM_USER+1
HWND winHandle;
CString gstrproducer;
CString gstrconsumer;
CBuffer buf1;
CBuffer buf2;
CBuffer buf3;
int numproduced;
int numconsumed;
HWND producemutex;
HWND consumemutex;
HWND mainmutex;

//my mutexes
MySemaphore myproducemutex;
MySemaphore myconsumemutex;
MySemaphore mymainmutex;

//used to determine whether to use my semaphores or NT's
int decision;
/////////////////////////////////////////////////////////////////////////////
// CSync2Dlg dialog

CSync2Dlg::CSync2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSync2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSync2Dlg)
	strProducer = _T("");
	strConsumer = _T("");
	strBuffer0 = _T("");
	strBuffer1 = _T("");
	strBuffer2 = _T("");
	strMessProduced = _T("");
	strMessConsumed = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSync2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSync2Dlg)
	DDX_Control(pDX, IDC_EDIT2, ctlConsumer);
	DDX_Control(pDX, IDC_EDIT1, txtProducer);
	DDX_Text(pDX, IDC_EDIT1, strProducer);
	DDX_Text(pDX, IDC_EDIT2, strConsumer);
	DDX_Text(pDX, IDC_EDIT3, strBuffer0);
	DDX_Text(pDX, IDC_EDIT4, strBuffer1);
	DDX_Text(pDX, IDC_EDIT5, strBuffer2);
	DDX_Text(pDX, IDC_EDIT6, strMessProduced);
	DDX_Text(pDX, IDC_EDIT7, strMessConsumed);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSync2Dlg, CDialog)
	//{{AFX_MSG_MAP(CSync2Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDWINDOWS, OnWindowsClicked)
	ON_BN_CLICKED(IDMINE, OnMineClicked)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(UPDATE_DATA, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSync2Dlg message handlers

BOOL CSync2Dlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	winHandle = GetSafeHwnd();
	buf1 = CBuffer();
	buf2 = CBuffer();
	buf3 = CBuffer();
	numproduced = 0;
	numconsumed = 0;
	producemutex = (HWND) CreateSemaphore(NULL, 1, 1, NULL);
	consumemutex = (HWND) CreateSemaphore(NULL, 1, 1, NULL);
	mainmutex = (HWND) CreateSemaphore(NULL, 1, 1, NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSync2Dlg::OnPaint() 
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

HCURSOR CSync2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSync2Dlg::OnWindowsClicked() 
{	
	DWORD threadid;
	HANDLE threadHandle;
	decision = 0;
	int i = 0;
	for (i =0; i < 6; i++) {
		char* param = new char(3);
		char temp[2];
		itoa(i, temp, 10);
		strcpy(param, temp);
		threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &producer, (LPVOID) param, 0, &threadid);
		CloseHandle(threadHandle);	
	}
	for (i =0; i < 6; i++) {
		char* param = new char(3);
		char temp[2];
		itoa(i, temp, 10);
		strcpy(param, temp);
		threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &consumer, (LPVOID) param, 0, &threadid);
		CloseHandle(threadHandle);	
	}
	
}

void CSync2Dlg::OnMineClicked() 
{
	DWORD threadid;
	HANDLE threadHandle;
	decision = 1;
	myproducemutex = MySemaphore(1, 1);
	myconsumemutex = MySemaphore(1, 1);
	mymainmutex = MySemaphore(1, 1);
	int i = 0;
	for (i =0; i < 6; i++) {
		char* param = new char(3);
		char temp[2];
		itoa(i, temp, 10);
		strcpy(param, temp);
		threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &myproducer, (LPVOID) param, 0, &threadid);
		CloseHandle(threadHandle);	
	}

	for (i =0; i < 6; i++) {
		char* param = new char(3);
		char temp[2];
		itoa(i, temp, 10);
		strcpy(param, temp);
		threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) &myconsumer, (LPVOID) param, 0, &threadid);
		CloseHandle(threadHandle);	
	}
	
}



void CSync2Dlg::OnUpdate(WPARAM w, LPARAM l) {
	if (decision == 0) {
	WaitForSingleObject(mainmutex, INFINITE);
	char strnumproduced[4];
	char strnumconsumed[4];
	long buffernum = (long) l;
	if (buffernum == 0) strBuffer0 = buf1.print();
	else if (buffernum == 1) strBuffer1 = buf2.print();
	else if (buffernum == 2) strBuffer2 = buf3.print();
	strProducer = gstrproducer;
	strConsumer = gstrconsumer;
	itoa(numproduced, strnumproduced, 10);
	itoa(numconsumed, strnumconsumed, 10);
	strMessProduced = messproduced + (CString) strnumproduced;
	strMessConsumed = messconsumed + (CString)  strnumconsumed;

	UpdateData(false);
	ReleaseSemaphore(mainmutex, 1, NULL);
	}
	else {
		mymainmutex.wait();
		char strnumproduced[4];
		char strnumconsumed[4];
		long buffernum = (long) l;
		if (buffernum == 0) strBuffer0 = buf1.print();
		else if (buffernum == 1) strBuffer1 = buf2.print();
		else if (buffernum == 2) strBuffer2 = buf3.print();
		strProducer = gstrproducer;
		strConsumer = gstrconsumer;
		itoa(numproduced, strnumproduced, 10);
		itoa(numconsumed, strnumconsumed, 10);
		strMessProduced = messproduced + (CString) strnumproduced;
		strMessConsumed = messconsumed + (CString)  strnumconsumed;

		UpdateData(false);
		mymainmutex.signal();
	}
}

CBuffer::CBuffer() {
	int i;
	mutex = (HWND) CreateSemaphore(NULL, 1, 1, NULL);
	full = (HWND) CreateSemaphore(NULL, 5, 5, NULL);
	empty = (HWND) CreateSemaphore(NULL, 0,5, NULL);
	mymutex = MySemaphore(1,1);
	myfull = MySemaphore(5,5);
	myempty = MySemaphore(5,0);
	for (i =0; i<5; i++) {
		values[i] = "";
	}
	numvalues = 0;
}
CString CBuffer::pop() {
	CString temp;
	numvalues -= 1;
	temp =  values[numvalues];
	return temp;
}
void CBuffer::push(CString value) {
	values[numvalues] = value;
	numvalues += 1;
}

CString CBuffer::print() {
	int i;
	CString temp = "";
	for (i = 0; i<numvalues; i++) {
		temp += values[i];
	}
	return temp;
}

MySemaphore::MySemaphore(int max, int count) {
	maxcount = max;
	curcount = count;
	mutex = 1;
}

int MySemaphore::wait() {
	while (!(TestAndSet(&mutex))) Sleep(100);
	curcount--;
	if (curcount > 0) {
		mutex = 1;
	}

	return 0;
}

int MySemaphore::signal() {
	curcount++;
	mutex = 1;
	return 0;
}

int TestAndSet(int* addr) {
	_asm mov eax, addr
		_asm bts [eax], 0
		_asm jc bitSet
		return 0;
bitSet:
		return 1;
}
void myproducer(LPVOID lpvThreadParam) {
	int i;
	int count = 0;
    char mycopy[3];
	CString number;
	char strbufnum[3];
	strcpy(mycopy,(char*) lpvThreadParam);
	number = mycopy[0];
	int bufnum = atoi(number) % 3;
	itoa(bufnum, strbufnum, 10);
	CString mess = "Producer " + number + " to Buffer " + strbufnum + "\r\n";
	for (i = 0; i <= 4; i++) {
		myproducemutex.wait();
		switch (bufnum) {
		case 0:
			buf1.myfull.wait();
			buf1.mymutex.wait();
			buf1.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 0);
			buf1.mymutex.signal();
			buf1.myempty.signal();			break;
		case 1:
			buf2.myfull.wait();
			buf2.mymutex.wait();
			buf2.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 1);
			buf2.mymutex.signal();
			buf2.myempty.signal();
			break;
		case 2:
			buf3.myfull.wait();
			buf3.mymutex.wait();
			buf3.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 2);
			buf3.mymutex.signal();
			buf3.myempty.signal();
			break;
		}//end of switch
		myproducemutex.signal();
		Sleep(500);
	}
	//delete lpvThreadParam;
	ExitThread(0);

}

void myconsumer(LPVOID lpvThreadParam) {
	int i;
    char mycopy[3];
	CString number;
	CString mess;
	strcpy(mycopy,(char*) lpvThreadParam);
	number = mycopy[0];
	int bufnum = atoi(number) % 3;
	for (i = 0; i <= 4; i++) {
		myconsumemutex.wait();
		switch (bufnum) {
		case 0:
			buf1.myempty.wait();
			buf1.mymutex.wait();
			mess = buf1.pop();
			numconsumed += 1;
			gstrconsumer += mess;			
			PostMessage(winHandle, UPDATE_DATA, 0, 0);
			buf1.mymutex.signal();
			buf1.myfull.signal();
			break;
		case 1:
			buf2.myempty.wait();
			buf2.mymutex.wait();
			mess = buf2.pop();
			numconsumed += 1;		
			gstrconsumer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 1);
			buf2.mymutex.signal();
			buf2.myfull.signal();
			break;
		case 2:
			buf3.myempty.wait();
			buf3.mymutex.wait();
			mess = buf3.pop();
			numconsumed += 1;		
			gstrconsumer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 2);
			buf3.mymutex.signal();
			buf3.myfull.signal();
			break;
		}//end of switch
		myconsumemutex.signal();
		Sleep(500);
	}
	//delete lpvThreadParam;
	ExitThread(0);
}
void producer(LPVOID lpvThreadParam) {
	int i;
	int count = 0;
    char mycopy[3];
	CString number;
	char strbufnum[3];
	strcpy(mycopy,(char*) lpvThreadParam);
	number = mycopy[0];
	int bufnum = atoi(number) % 3;
	itoa(bufnum, strbufnum, 10);
	CString mess = "Producer " + number + " to Buffer " + strbufnum + "\r\n";
	for (i = 0; i <= 4; i++) {
		WaitForSingleObject(producemutex, INFINITE);
		switch (bufnum) {
		case 0:
			WaitForSingleObject(buf1.full, INFINITE);
			WaitForSingleObject(buf1.mutex, INFINITE);
			buf1.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 0);
			ReleaseSemaphore(buf1.mutex, 1, NULL);
			ReleaseSemaphore(buf1.empty, 1, NULL);
			break;
		case 1:
			WaitForSingleObject(buf2.full, INFINITE);
			WaitForSingleObject(buf2.mutex, INFINITE);
			buf2.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 1);
			ReleaseSemaphore(buf2.mutex, 1, NULL);
			ReleaseSemaphore(buf2.empty, 1, NULL);
			break;
		case 2:
			WaitForSingleObject(buf3.full, INFINITE);
			WaitForSingleObject(buf3.mutex, INFINITE);
			buf3.push(mess);
			numproduced += 1;
			gstrproducer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 2);
			ReleaseSemaphore(buf3.mutex, 1, NULL);
			ReleaseSemaphore(buf3.empty, 1, NULL);
			break;
		}//end of switch
		ReleaseSemaphore(producemutex, 1, NULL);
		Sleep(500);
	}
	//delete lpvThreadParam;
	ExitThread(0);

}

void consumer(LPVOID lpvThreadParam) {
	int i;
    char mycopy[3];
	CString number;
	CString mess;
	strcpy(mycopy,(char*) lpvThreadParam);
	number = mycopy[0];
	int bufnum = atoi(number) % 3;
	for (i = 0; i <= 4; i++) {
		WaitForSingleObject(consumemutex, INFINITE);
		switch (bufnum) {
		case 0:
			WaitForSingleObject(buf1.empty, INFINITE);
			WaitForSingleObject(buf1.mutex, INFINITE);
			mess = buf1.pop();
			numconsumed += 1;
			gstrconsumer += mess;			
			PostMessage(winHandle, UPDATE_DATA, 0, 0);
			ReleaseSemaphore(buf1.mutex, 1, NULL);
			ReleaseSemaphore(buf1.full, 1, NULL);
			break;
		case 1:
			WaitForSingleObject(buf2.empty, INFINITE);
			WaitForSingleObject(buf2.mutex, INFINITE);
			mess = buf2.pop();
			numconsumed += 1;		
			gstrconsumer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 1);
			ReleaseSemaphore(buf2.mutex, 1, NULL);
			ReleaseSemaphore(buf2.full, 1, NULL);
			break;
		case 2:
			WaitForSingleObject(buf3.empty, INFINITE);
			WaitForSingleObject(buf3.mutex, INFINITE);
			mess = buf3.pop();
			numconsumed += 1;		
			gstrconsumer += mess;
			PostMessage(winHandle, UPDATE_DATA, 0, 2);
			ReleaseSemaphore(buf3.mutex, 1, NULL);
			ReleaseSemaphore(buf3.full, 1, NULL);
			break;
		}//end of switch
		ReleaseSemaphore(consumemutex, 1, NULL);
		Sleep(500);
	}
	//delete lpvThreadParam;
	ExitThread(0);
}

