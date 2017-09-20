// Lab2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "Lab2Dlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLab2Dlg dialog

CLab2Dlg::CLab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLab2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLab2Dlg)
	m_window = _T("");
	m_insert = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLab2Dlg)
	DDX_Text(pDX, IDC_window, m_window);
	DDX_Text(pDX, IDC_insert, m_insert);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLab2Dlg, CDialog)
	//{{AFX_MSG_MAP(CLab2Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_plain, Onplain)
	ON_BN_CLICKED(IDC_bold, Onbold)
	ON_BN_CLICKED(IDC_doitnow, Ondoitnow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLab2Dlg message handlers

BOOL CLab2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLab2Dlg::OnPaint() 
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

HCURSOR CLab2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


CFont* original;
CFont* myfont;


void CLab2Dlg::Onplain(){
	
	CWnd *mywin = GetDlgItem(IDC_window); 
	mywin->SetFont(original,true);
	UpdateData(FALSE);
	
}//Onplain



void CLab2Dlg::Onbold(){

	original = GetFont();
	myfont = new CFont;
	myfont->CreateFont(0,0,0,0,700,0,0,0,0,0,0,0,0,NULL);
	CWnd *mywin = GetDlgItem(IDC_window); 
	mywin->SetFont(myfont,true);
	UpdateData(FALSE);
}//Onbold

void CLab2Dlg::Ondoitnow(){

	
	UpdateData(true);

	

	char* part1 = NULL;
	char* part2 = NULL;
	char* part3 = NULL;
	char* part4 = NULL;

	char *string=(char *)(LPCTSTR)m_insert;
	char temp[] = " ";

	part1 = strtok( string, temp );
	if(part1!=NULL){
		part2 = strtok( NULL, temp );
		if(part2!=NULL){
			part3 = strtok( NULL, temp );
			if(part3!=NULL){
				part4 = strtok( NULL, temp );
			}//if
		}//if
	}//if

	
	if(part1 == NULL){
		m_window += "Please enter something...anything!\r\n";
	}//if

	else if(strcmp(part1,"mkdir")==0){
		if(part2 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* name = part2;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdMakeDir");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    
				if(lpfnDllFunc1(name)){
					m_window += "mkdir Completed\r\n";
				}
				else m_window += "mkdir Failed\r\n";
			}
		}

	}//if
	

	else if(strcmp(part1,"close")==0){
		if(part2 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if
		typedef UINT (__cdecl* LPFNDLLFUNC1)(int);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		int index = atoi(part2);
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdCloseFile");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    
				if(lpfnDllFunc1(index) != -1){
					m_window += "close Completed\r\n";
				}
				else m_window += "close Failed\r\n";
			}
		}

	}//if



	else if(strcmp(part1,"dir")==0){
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char**);
		typedef UINT (__cdecl* LPFNDLLFUNC2)(char**);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		LPFNDLLFUNC2 lpfnDllFunc2;
		
		HINSTANCE hDLL;               // Handle to DLL
		char* output;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdListDir");
			lpfnDllFunc2 = (LPFNDLLFUNC2)GetProcAddress(hDLL,"fdDelMem");
			
			if (!lpfnDllFunc1)
			{    // handle the error
				m_window += "Can't load fdListDir\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);       
				return ;
			}
			else if(!lpfnDllFunc2){
				m_window += "Can't load fdDelMem\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);       
				return;
			}
			else
			{    // call the function
				if(lpfnDllFunc1(&output)){
					m_window += "     -----------------------------------------\r\n";
					m_window += output;
					m_window += "     -----------------------------------------\r\n";
				}
				else m_window += "dir Failed\r\n";
				if(lpfnDllFunc2(&output)){
					m_window += "DelMem Success\r\n";
				}//if
				else m_window += "DelMem Fail\r\n";
			}
		}
	}//if



	
	else if(strcmp(part1,"read")==0){
		if(part2 == NULL || part3 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if

		typedef UINT (__cdecl* LPFNDLLFUNC1)(int, char*, unsigned long);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		int index = atoi(part2);
		unsigned long length = atoi(part3);

		char* output = new char[length];

		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdReadFile");
			
			if (!lpfnDllFunc1)
			{    // handle the error
				m_window += "Can't load fdReadFile\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);       
				return ;
			}
			else
			{    // call the function

				int count = lpfnDllFunc1(index, output, length);
				if(count >= 0){
					m_window += "     -----------------------------------------\r\n";
					m_window += output;
					m_window += "\r\nBytesRead = ";
					char temp[30] = "";
					itoa(count, temp, 10);
					m_window += temp;
					m_window += "\r\n     -----------------------------------------\r\n";
				}
				else m_window += "read Failed\r\n";

			}//else
		}//if
		delete[] output;
	}//else if read






	else if(strcmp(part1,"write")==0){
		if(part2 == NULL || part3 == NULL || part4 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if

		typedef UINT (__cdecl* LPFNDLLFUNC1)(int, char*, unsigned long);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL

		int index = atoi(part2);
		unsigned long length = atoi(part4);

		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdWriteFile");
			
			if (!lpfnDllFunc1)
			{    // handle the error
				m_window += "Can't load fdWriteFile\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);
				return ;
			}
			
			else
			{    // call the function
				int count = lpfnDllFunc1(index, part3, length);
				if(count >= 0){
					char temp[200] = "";
					itoa(count, temp, 10);
					m_window += "write Success\r\nBytes written = ";
					m_window += temp;
					m_window += "\r\n";
				}
				else m_window += "write Failed\r\n";
				
			}//else
		}//if
	}//if



	else if(strcmp(part1,"print")==0){
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*, char**);
		typedef UINT (__cdecl* LPFNDLLFUNC2)(char**);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		LPFNDLLFUNC2 lpfnDllFunc2;
		
		HINSTANCE hDLL;               // Handle to DLL
		char* output;
		char* file = part2;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdPrintFile");
			lpfnDllFunc2 = (LPFNDLLFUNC2)GetProcAddress(hDLL,"fdDelMem");
			
			if (!lpfnDllFunc1)
			{    // handle the error
				m_window += "Can't load fdPrintFile\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);       
				return ;
			}
			else if(!lpfnDllFunc2){
				m_window += "Can't load fdDelMem\r\n";
				UpdateData(false);
				FreeLibrary(hDLL);       
				return;
			}
			else
			{    // call the function
				if(lpfnDllFunc1(file, &output)){
					m_window += "     -----------------------------------------\r\n";
					m_window += output;
					m_window += "\r\n     -----------------------------------------\r\n";
				}
				else m_window += "dir Failed\r\n";
				if(lpfnDllFunc2(&output)){
					m_window += "DelMem Success\r\n";
				}//if
				else m_window += "DelMem Fail\r\n";
			}
		}


	}//if

	else if(strcmp(part1,"cd")==0){
		if(part2 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if

		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* name = part2;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdChangeDir");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    
				if(lpfnDllFunc1(name)){
					m_window += "cd Completed\r\n";
				}
				else m_window += "cd Failed\r\n";
			}
		}

	}//if


	

	else if(strcmp(part1,"del")==0){

		if(part2 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* name = part2;
		
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdEraseFile");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return;
			 }
			else
			{    // call the function
				if(lpfnDllFunc1(name)){
					m_window += "del Completed\r\n";
				}
				else m_window += "del Failed\r\n";
			}
		}
	}//if

	else if(strcmp(part1,"copy")==0){
	
		if(part2 == NULL || part3 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*, char*);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* source = part2;
		char* dest = part3;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdCopyFile");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    // call the function
				if(lpfnDllFunc1(source, dest)){
					m_window+="copy Completed\r\n";
				}else{
					m_window+="copy Failed\r\n";
				}
			}
		}

	}//if


	else if(strcmp(part1,"seek")==0){
	
		if(part2 == NULL || part3 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if

		typedef UINT (__cdecl* LPFNDLLFUNC1)(int, unsigned long);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		int index = atoi(part2);
		unsigned long location = atoi(part3);
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdSeek");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    // call the function
				if(lpfnDllFunc1(index, location) == 1){
					m_window+="seek Completed\r\n";
				}else{
					m_window+="seek Failed\r\n";
				}
			}
		}

	}//if




	else if(strcmp(part1,"open")==0){
	
		if(part2 == NULL || part3 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if

		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*, int);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* filename = part2;
		int mode = atoi(part3);
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdOpenFile");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    // call the function
				int check = lpfnDllFunc1(filename, mode);
				if(check != -1){
					m_window+="open Completed.  ";
					m_window+="File:  ";
					m_window+=filename;
					m_window+=",  Index:  ";
					char temp[5] = "";
					itoa(check, temp, 10);
					m_window+=temp;
					m_window+="\r\n";
				}else{
					m_window+="open Failed\r\n";
				}
			}
		}

	}//if



	else if(strcmp(part1,"rmdir")==0){
		
		if(part2 == NULL){
			m_insert = "";
			m_window += "ERROR: not enough parameters\r\n";
			UpdateData(false);
			CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
			pEdit->LineScroll( pEdit->GetLineCount()-1 );
			return;
		}//if
		typedef UINT (__cdecl* LPFNDLLFUNC1)(char*);
		LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer.
		
		HINSTANCE hDLL;               // Handle to DLL
		char* name = part2;
		hDLL = LoadLibrary("filedll.dll");

		//Use this form to load each of your functions from the DLL

		if (hDLL != NULL)
		{   // get the address of "DLLFunc1" from the dll.
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"fdDelDir");
			if (!lpfnDllFunc1)
			{    // handle the error
				FreeLibrary(hDLL);       
				return ;
			 }
			else
			{    // call the function
				if(lpfnDllFunc1(name)){
					m_window+="rmdir Completed\r\n";
				}else{
					m_window+="rmdir Failed\r\n";
				}
			}
		}

	}//if
	else if(strcmp(part1,"echo")==0){

		UpdateData(true);

		CString temp = m_insert;

		int j = temp.FindOneOf(" ");
		
		//m_window += temp.Delete(0, 4);
		m_window += m_insert;
		m_window += "\r\n";

	}//if


	  
	else{

		original = GetFont();
		UpdateData(false);

		HANDLE error_read;
		HANDLE output_read;
		HANDLE input_read;
		HANDLE error_write;
		HANDLE output_write;
		HANDLE input_write;


		
		SECURITY_ATTRIBUTES saAttr; 
 
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
		saAttr.bInheritHandle = TRUE; 
		saAttr.lpSecurityDescriptor = NULL; 
 
		CreatePipe(&error_read, &error_write, &saAttr, 0); 
		CreatePipe(&output_read, &output_write, &saAttr, 0); 
		CreatePipe(&input_read, &input_write, &saAttr, 0); 

		STARTUPINFO siStartInfo;
		PROCESS_INFORMATION pInfo;
 
	// Set up members of STARTUPINFO structure. 
 
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO); 
		siStartInfo.hStdError = error_write;  //Your error write pipe
		siStartInfo.hStdInput = input_read;  //Your input read pipe
		siStartInfo.hStdOutput = output_write;  //Your output write pipe
		siStartInfo.dwFlags = STARTF_USESTDHANDLES;

		//Create the process

		int process = CreateProcess(m_insert, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW | DETACHED_PROCESS | IDLE_PRIORITY_CLASS, NULL, NULL, &siStartInfo, &pInfo);

		CloseHandle(error_write);
		CloseHandle(input_read);
		CloseHandle(output_write);


		bool retval;
		DWORD cnt;
		char c;

		if(process){
			while(1)
			{
				retval = ReadFile(output_read, &c, 1, &cnt, NULL);
				if (retval == 0 && cnt == 0)	break;
				m_window += c;
				UpdateData(FALSE);
			}
		}//if

		

		CloseHandle(error_read);
		CloseHandle(input_write);
		CloseHandle(output_read);
	}//else

		m_insert = "";
		UpdateData(false);
		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_window);
	    pEdit->LineScroll( pEdit->GetLineCount()-1 );

}//Ondoitnow  //D:\cs345\testlab2\Debug\testlab2.exe Hello World
