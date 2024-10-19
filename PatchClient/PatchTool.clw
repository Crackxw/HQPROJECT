; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPatchToolDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PatchTool.h"

ClassCount=8
Class1=CPatchToolApp
Class2=CPatchToolDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_PATH
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTDDLG
Class4=CMyAboutDlg
Resource4=IDD_ABOUTBOX
Class5=CAttachPathDlg
Resource5=IDD_IPDIALOG
Class6=CLoginDlg
Resource6=IDD_PATCHTOOL_DIALOG
Class7=CDestinationDirDlg
Class8=CDestinationPathDailog
Resource7=IDD_DESTINATIONPATHDIALOG

[CLS:CPatchToolApp]
Type=0
HeaderFile=PatchTool.h
ImplementationFile=PatchTool.cpp
Filter=N

[CLS:CPatchToolDlg]
Type=0
HeaderFile=PatchToolDlg.h
ImplementationFile=PatchToolDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MSGDISPLAYLIST

[CLS:CAboutDlg]
Type=0
HeaderFile=PatchToolDlg.h
ImplementationFile=PatchToolDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PATCHTOOL_DIALOG]
Type=1
Class=CPatchToolDlg
ControlCount=24
Control1=IDC_SOURCEDIRECTORYPATHBTN,button,1342242816
Control2=IDC_DELETEPATCHBTN,button,1342242816
Control3=IDC_STARTPATCHBTN,button,1342242816
Control4=IDC_ALLPATCHCHOICEBTN,button,1342242816
Control5=IDC_CHOICEPATCHCANCELBTN,button,1342242816
Control6=IDCANCEL,button,1342242816
Control7=IDC_ABOUT,button,1342242816
Control8=IDC_MASTER_SERVER_CONNECT,button,1342242816
Control9=IDC_COPYFILEPROGRESS,msctls_progress32,1342177280
Control10=IDC_SOURCEFILELIST,SysListView32,1350633475
Control11=IDC_ALL_FILE_ADD_BTN,button,1342242816
Control12=IDC_ALL_FILE_REMOVE_BTN,button,1342242816
Control13=IDC_TARGETFILELIST,SysListView32,1350633475
Control14=IDC_SERVERSELECTLIST,listbox,1350631761
Control15=IDC_STATIC,static,1342308352
Control16=IDC_SOURCEDIRECTORYPATHEDIT,edit,1350633600
Control17=IDC_STATIC,button,1342177287
Control18=IDC_NEWVISIONRADIO,button,1342308361
Control19=IDC_ATTACHRADIO,button,1342177289
Control20=IDC_STATIC,button,1342177287
Control21=IDC_MSGDISPLAYLIST,listbox,1352728833
Control22=IDC_FILETRANSFERBYTESTATIC,static,1342308352
Control23=IDC_FILETRANSFRERNAMESTATIC,static,1342308352
Control24=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTDDLG]
Type=1
Class=CMyAboutDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352

[CLS:CMyAboutDlg]
Type=0
HeaderFile=MyAboutDlg.h
ImplementationFile=MyAboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMyAboutDlg

[DLG:IDD_PATH]
Type=1
Class=CAttachPathDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ATTACHVERSIONEDIT,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CAttachPathDlg]
Type=0
HeaderFile=AttachPathDlg.h
ImplementationFile=AttachPathDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAttachPathDlg
VirtualFilter=dWC

[DLG:IDD_IPDIALOG]
Type=1
Class=CLoginDlg
ControlCount=6
Control1=IDC_IDEDIT,edit,1350631552
Control2=IDC_PWEDIT,edit,1350631584
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CLoginDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLoginDlg
VirtualFilter=dWC

[CLS:CDestinationDirDlg]
Type=0
HeaderFile=DestinationDirDlg.h
ImplementationFile=DestinationDirDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDestinationDirDlg

[DLG:IDD_DESTINATIONPATHDIALOG]
Type=1
Class=CDestinationPathDailog
ControlCount=5
Control1=IDC_DESTINATIONPATHEDIT,edit,1350631552
Control2=IDC_DESTINATIONPATHLIST,listbox,1352728833
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352

[CLS:CDestinationPathDailog]
Type=0
HeaderFile=DestinationPathDailog.h
ImplementationFile=DestinationPathDailog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_DESTINATIONPATHLIST
VirtualFilter=dWC

