// VirtualMemDlg.cpp : implementation file
//


#include "stdafx.h"
#include "VirtualMem.h"
#include "VirtualMemDlg.h"
#include "fstream.h"
#include "Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RETURN "\r\n"
#define UPDATE_DATA WM_USER+1

Frametable sysFrametable;
Swapspace  sysSwapspace;
Proc sysProctable[10];
ifstream inFile;
HWND winHandle;
long hits;
long misses;
long pageSize;
long swapSize;
long totalMem;
/////////////////////////////////////////////////////////////////////////////
// CVirtualMemDlg dialog

CVirtualMemDlg::CVirtualMemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualMemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirtualMemDlg)
	strViewSwap = _T("");
	strViewFrame = _T("");
	strFrameTable = _T("");
	strPageNum = _T("");
	strFrameNum = _T("");
	strTotalMem = _T("8");
	strPageSize = _T("4");
	strSwapSize = _T("1024");
	strNumHits = _T("0");
	strNumMisses = _T("0");
	strFilename = _T("Virtmem.inst");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualMemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualMemDlg)
	DDX_Text(pDX, IDC_EDIT1, strViewSwap);
	DDX_Text(pDX, IDC_EDIT2, strViewFrame);
	DDX_Text(pDX, IDC_EDIT3, strFrameTable);
	DDX_Text(pDX, IDC_EDIT4, strPageNum);
	DDX_Text(pDX, IDC_EDIT5, strFrameNum);
	DDX_Text(pDX, IDC_EDIT6, strTotalMem);
	DDX_Text(pDX, IDC_EDIT7, strPageSize);
	DDX_Text(pDX, IDC_EDIT8, strSwapSize);
	DDX_Text(pDX, IDC_EDIT9, strNumHits);
	DDX_Text(pDX, IDC_EDIT10, strNumMisses);
	DDX_Text(pDX, IDC_EDIT11, strFilename);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVirtualMemDlg, CDialog)
	//{{AFX_MSG_MAP(CVirtualMemDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnViewPage)
	ON_BN_CLICKED(IDC_BUTTON2, OnViewFrame)
	ON_BN_CLICKED(IDOK, OnGo)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(UPDATE_DATA, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemDlg message handlers

BOOL CVirtualMemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	winHandle = GetSafeHwnd();
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVirtualMemDlg::OnPaint() 
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
HCURSOR CVirtualMemDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVirtualMemDlg::OnViewPage() 
{
	UpdateData(TRUE);
	strViewSwap = sysSwapspace.printpage(atoi(strPageNum));
	PostMessage(UPDATE_DATA, 0, 2);
	
}

void CVirtualMemDlg::OnViewFrame() 
{
	UpdateData(TRUE);
	strViewFrame = sysFrametable.printpage(atoi(strFrameNum));
	PostMessage(UPDATE_DATA, 0, 2);
	
}

void CVirtualMemDlg::OnGo() 
{
	
	ifstream tempFile;
	char myString[256];
	CString tempString;
	long vAddress;
	long val;
	long procNum;
	int frameNum;
	int swapNum;
	int pagenum;
	long offset;
	Page page;
	long data;
	long temp1;
	long temp2;
	
	UpdateData(true);
	if (inFile.is_open() == 0 ) {
       inFile.open(strFilename, ios::in);
	   strFrameTable = "";
	   hits = 0;
	   misses = 0;
	   UpdateData(false);
	   if (!inFile) {
			cout << "File could not be opened" << endl;
			exit(1);
	   }
	   pageSize = atoi(strPageSize) * 1024;
	   swapSize = atoi(strSwapSize) * 1024;
	   totalMem = atoi(strTotalMem) * 1024;
	   sysFrametable = Frametable( totalMem / pageSize, pageSize);
	   sysSwapspace = Swapspace( swapSize / pageSize, pageSize);
	}
        inFile >> myString;
		tempString.Format(myString);
		tempString.MakeUpper();
	while (inFile.eof() == 0 && tempString != "") {
		
		switch ((int) tempString.GetAt(0)) {
			case 'L':
					inFile >> myString;
					inFile >> procNum;
					
					tempFile.open(myString, ios::in);
					if (!tempFile) {
						cout << "File could not be opened" << endl;
						exit(1);
					}
					sysProctable[procNum] = Proc(procNum, pageSize);
						tempFile >> vAddress;
						tempFile >> val;
					while (tempFile.eof() == 0 && vAddress != 0) {
					
						pagenum = vAddress / pageSize;
						offset = vAddress % pageSize;

						//Check if the page is in memory
						frameNum = sysProctable[procNum].myPT.getValBit(pagenum);
						//page is in memory
						if (frameNum != -1) {
							//set dirty and ref bits
							sysFrametable.setRefBit(frameNum, 1);
							sysFrametable.setModBit(frameNum, 0);
							//hits++;
							//ltoa(hits, myString, 10);
							//strNumHits.Format(myString);
							PostMessage(UPDATE_DATA, 0, 2);
							//modify the value
							page = sysFrametable.get(frameNum);
							page.setvalue(offset, val);
						}
						else {
							//misses++;
							//ltoa(misses, myString, 10);
							//strNumMisses.Format(myString);
							//UpdateData(false);
							//get the location of the page in swap
							swapNum = sysProctable[procNum].myPT.getSwap(pagenum);

							if (swapNum == -1) {
								//Frame does not exist in swap yet
								swapNum = sysSwapspace.allocate();
								sysProctable[procNum].myPT.setSwap(pagenum, swapNum);
							}
								/*temp1 = sysFrametable.emptyframe();
								if (temp1 == -1) {
								//Get page number 1 to replace it with page coming from swap
								page = sysFrametable.get(1);
								temp1 = sysFrametable.getprocess(1);
								temp2 = sysFrametable.getvirtualPage(1);
								//make valid bit of page invalid
								sysProctable[temp1].myPT.setValBit(temp2, -1);
								temp1 = sysProctable[temp1].myPT.getSwap(temp2);
								//write old page to disk
								sysSwapspace.set(temp1, page);
								temp1 = 1;
								}*/
								//get the new page
								page = sysSwapspace.get(swapNum);
								page.setvalue(offset, val);
								//set the appropriate values in proctable
								sysProctable[procNum].myPT.setSwap(pagenum, swapNum);
								/*
								sysProctable[procNum].myPT.setValBit(pagenum, temp1);
								//set appropriate values in frametable
								sysFrametable.set(temp1, page);
								sysFrametable.setRefBit(temp1, 1);
								sysFrametable.setvirtualPage(temp1, pagenum);
								sysFrametable.setprocess(temp1, procNum);
								sysFrametable.setModBit(temp1, 0);
								*/
							
							
						}	
						tempFile >> vAddress;
						tempFile >> val;
					}//end of while
					tempFile.close();
					break;
			case 'S':
					strFrameTable += sysFrametable.printStatus();
					PostMessage(UPDATE_DATA, 0, 2);
					return;
					break;
			case 'R':
					inFile >> vAddress;
					inFile >> val;
					inFile >> procNum;
					pagenum = vAddress / pageSize;
					offset = vAddress % pageSize;
					frameNum = sysProctable[procNum].myPT.getValBit(pagenum);
					if (frameNum != -1) {
						sysFrametable.setRefBit(frameNum, 1);
						hits++;
						ltoa(hits, myString, 10);
						strNumHits.Format(myString);
						PostMessage(UPDATE_DATA, 0, 2);
						page = sysFrametable.get(frameNum);
						data = page.getvalue(offset);
						if (data != val) {
							sprintf(myString, "The real value was supposed to be %l and the value we read was %l", val, data);
							//MessageBox(myString, "Error - Wrong Data", MB_OK);
						}
					}
					else {
						misses++;
						ltoa(misses, myString, 10);
						strNumMisses.Format(myString);
						PostMessage(UPDATE_DATA, 0, 2);
						//Page not in memory - load in memory and then read
						//Get page number 1 to replace it with page coming from swap
						swapNum = sysProctable[procNum].myPT.getSwap(pagenum);
								frameNum = sysFrametable.emptyframe();
								if (frameNum == -1) {
								//Get page number 1 to replace it with page coming from swap
								frameNum = sysFrametable.getPageToReplace();
								page = sysFrametable.get(frameNum);
								temp1 = sysFrametable.getprocess(frameNum);
								temp2 = sysFrametable.getvirtualPage(frameNum);
								//make valid bit of page invalid
								sysProctable[temp1].myPT.setValBit(temp2, -1);
								temp1 = sysProctable[temp1].myPT.getSwap(temp2);
								//write old page to disk
								sysSwapspace.set(temp1, page);
								}
								//get the new page
								page = sysSwapspace.get(swapNum);
								//set the appropriate values in proctable
								sysProctable[procNum].myPT.setSwap(pagenum, swapNum);
								sysProctable[procNum].myPT.setValBit(pagenum, frameNum);
								//set appropriate values in frametable
								sysFrametable.set(frameNum, page);
								sysFrametable.setRefBit(frameNum, 1);
								sysFrametable.setvirtualPage(frameNum, pagenum);
								sysFrametable.setprocess(frameNum, procNum);
						page = sysFrametable.get(frameNum);
						data = page.getvalue(offset);
						if (data != val) {
							sprintf(myString, "The real value was supposed to be %l and the value we read was %l", val, data);
							//MessageBox(myString, "Error - Wrong Data", MB_OK);
						}
					}
					break;
			case 'W':
					inFile >> vAddress;
					inFile >> val;
					inFile >> procNum;
					pagenum = vAddress / pageSize;
					offset = vAddress % pageSize;
					frameNum = sysProctable[procNum].myPT.getValBit(pagenum);
					if (frameNum != -1) {
						sysFrametable.setRefBit(frameNum, 1);
					sysFrametable.setModBit(frameNum, 1);
						hits++;
						ltoa(hits, myString, 10);
						strNumHits.Format(myString);
						PostMessage(UPDATE_DATA, 0, 2);
						page = sysFrametable.get(frameNum);
						page.setvalue(offset, val);
						sysFrametable.setRefBit(frameNum, 1);
					}
					else {
						misses++;
						ltoa(misses, myString, 10);
						strNumMisses.Format(myString);
						PostMessage(UPDATE_DATA, 0, 2);
						//Page not in memory - load in memory and then read
						//Get page number 1 to replace it with page coming from swap
						swapNum = sysProctable[procNum].myPT.getSwap(pagenum);
								frameNum = sysFrametable.emptyframe();
								if (frameNum == -1) {
								//Get page number 1 to replace it with page coming from swap
								frameNum = sysFrametable.getPageToReplace();
								page = sysFrametable.get(frameNum);
								temp1 = sysFrametable.getprocess(frameNum);
								temp2 = sysFrametable.getvirtualPage(frameNum);
								//make valid bit of page invalid
								sysProctable[temp1].myPT.setValBit(temp2, -1);
								temp1 = sysProctable[temp1].myPT.getSwap(temp2);
								//write old page to disk
								sysSwapspace.set(temp1, page);
								}
								//get the new page
								page = sysSwapspace.get(swapNum);
								//set the appropriate values in proctable
								sysProctable[procNum].myPT.setSwap(pagenum, swapNum);
								sysProctable[procNum].myPT.setValBit(pagenum, frameNum);
								//set appropriate values in frametable
								sysFrametable.set(frameNum, page);
								sysFrametable.setRefBit(frameNum, 1);
								sysFrametable.setvirtualPage(frameNum, pagenum);
								sysFrametable.setprocess(frameNum, procNum);
								sysFrametable.setModBit(frameNum, 1);
								page = sysFrametable.get(frameNum);
						        page.setvalue(offset, val);
					}
					break;
		}
		//strViewFrame = tempString + RETURN;
		//PostMessage(UPDATE_DATA, 0, 2);
		//MessageBox(tempString, NULL, MB_OK);
		inFile >> myString;
		tempString.Format(myString);
		tempString.MakeUpper();
		PostMessage(UPDATE_DATA, 0, 2);
	  }
	PostMessage(UPDATE_DATA, 0, 2);
	inFile.close();
	MessageBox("Input file closed", "File closed", MB_OK); 
}

void CVirtualMemDlg::OnUpdate(WPARAM w, LPARAM l)  {
	UpdateData(false);	
	
}


