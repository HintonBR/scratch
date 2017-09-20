; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSocketsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Sockets.h"

ClassCount=2
Class1=CSocketsApp
Class2=CSocketsDlg

ResourceCount=3
Resource1=IDD_SOCKETS_DIALOG
Resource2=IDR_MENU1
Resource3=IDR_ACCELERATOR1

[CLS:CSocketsApp]
Type=0
HeaderFile=Sockets.h
ImplementationFile=Sockets.cpp
Filter=N
LastObject=mnuClear

[CLS:CSocketsDlg]
Type=0
HeaderFile=SocketsDlg.h
ImplementationFile=SocketsDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSocketsDlg



[DLG:IDD_SOCKETS_DIALOG]
Type=1
Class=CSocketsDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1352734788
Control4=IDC_EDIT2,edit,1352734788
Control5=IDC_STATIC,static,1342177294
Control6=IDC_STATIC,static,1342308865
Control7=IDC_STATIC,static,1342308865
Control8=IDC_EDIT3,edit,1350631552
Control9=IDC_STATIC,static,1342308865

[MNU:IDR_MENU1]
Type=1
Class=CSocketsDlg
Command1=mnuClear
Command2=mnuExit
CommandCount=2

[ACL:IDR_ACCELERATOR1]
Type=1
Class=CSocketsDlg
Command1=mnuClear
CommandCount=1

