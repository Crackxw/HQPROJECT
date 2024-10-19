; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CStatisticsFormView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CrashMonitorClient.h"
LastPage=0

ClassCount=10
Class1=CCrashMonitorClientApp
Class2=CCrashMonitorClientDoc
Class3=CCrashMonitorClientView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDD_CONNECTDGL
Class6=CCrashClientFormView
Resource3=IDD_CRASHTOPFORMDLG
Class7=CCrashTopFormView
Resource4=IDR_MAINFRAME
Class8=CStatisticsFormView
Resource5=IDD_CRASHFORMDLG
Class9=CConnectDlg
Resource6=IDD_CRASHINFOPROCESSDLG
Class10=CCrashInfoRecvProcessDlg
Resource7=IDD_CRASHSTATISTICSDLG

[CLS:CCrashMonitorClientApp]
Type=0
HeaderFile=CrashMonitorClient.h
ImplementationFile=CrashMonitorClient.cpp
Filter=N
LastObject=CCrashMonitorClientApp

[CLS:CCrashMonitorClientDoc]
Type=0
HeaderFile=CrashMonitorClientDoc.h
ImplementationFile=CrashMonitorClientDoc.cpp
Filter=N

[CLS:CCrashMonitorClientView]
Type=0
HeaderFile=CrashMonitorClientView.h
ImplementationFile=CrashMonitorClientView.cpp
Filter=C
LastObject=CCrashMonitorClientView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=CrashMonitorClient.cpp
ImplementationFile=CrashMonitorClient.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=IDM_CRASHSERVERCONNECT
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=IDM_CRASHSERVERCONNECT
Command2=ID_APP_ABOUT
CommandCount=2

[CLS:CCrashFormView]
Type=0
HeaderFile=CrashFormView.h
ImplementationFile=CrashFormView.cpp
BaseClass=CFormView
Filter=D
LastObject=CCrashFormView

[DLG:IDD_CRASHFORMDLG]
Type=1
Class=CCrashClientFormView
ControlCount=5
Control1=IDC_CRASHINFOSPLITTERBTN,button,1342312457
Control2=IDC_CRASHSTATISTICSSPLITTERBTN,button,1342181385
Control3=IDC_SELECTMODERADIO,button,1342308361
Control4=IDC_SORTMODERADIO,button,1342177289
Control5=IDC_STATIC,button,1342308359

[CLS:CCrashClientFormView]
Type=0
HeaderFile=CrashClientFormView.h
ImplementationFile=CrashClientFormView.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_CRASHSTATISTICSSPLITTERBTN
VirtualFilter=VWC

[DLG:IDD_CRASHTOPFORMDLG]
Type=1
Class=CCrashTopFormView
ControlCount=36
Control1=IDC_ALLFIELDOPT,button,1342308361
Control2=IDC_ALLWHEREOPT,button,1342177289
Control3=IDC_FIELDOPERATORCOMBO1,combobox,1344339970
Control4=IDC_FIELDVALUECOMBO1,combobox,1344339970
Control5=IDC_FIELDANDORCOMBO1,combobox,1344339970
Control6=IDC_FIELDVALUECOMBO2,combobox,1344339970
Control7=IDC_FIELDVALUECOMBO3,combobox,1344339970
Control8=IDC_FIELDVALUECOMBO4,combobox,1344339970
Control9=IDC_FIELDVALUECOMBO5,combobox,1344339970
Control10=IDC_FIELDANDORCOMBO2,combobox,1344339970
Control11=IDC_FIELDANDORCOMBO3,combobox,1344339970
Control12=IDC_FIELDANDORCOMBO4,combobox,1344339970
Control13=IDC_FIELDOPERATORCOMBO2,combobox,1344339970
Control14=IDC_FIELDOPERATORCOMBO3,combobox,1344339970
Control15=IDC_FIELDOPERATORCOMBO4,combobox,1344339970
Control16=IDC_FIELDOPERATORCOMBO5,combobox,1344339970
Control17=IDC_FIELDSELECTEDIT1,edit,1350633600
Control18=IDC_FIELDSELECTEDIT2,edit,1350633600
Control19=IDC_FIELDSELECTEDIT3,edit,1350633600
Control20=IDC_FIELDSELECTEDIT4,edit,1350633600
Control21=IDC_FIELDSELECTEDIT5,edit,1350633600
Control22=IDC_FILEDSELECTLIST,listbox,1352728913
Control23=IDC_FIELDSELECTOKBTN,button,1342242816
Control24=IDC_FIELDSELECTCANCELBTN,button,1342242816
Control25=IDC_SELECTWHERECHECK2,button,1342242819
Control26=IDC_SELECTWHERECHECK3,button,1342242819
Control27=IDC_SELECTWHERECHECK4,button,1342242819
Control28=IDC_SELECTWHERECHECK5,button,1342242819
Control29=IDC_SEARCHBTN,button,1342242816
Control30=IDC_CRASHPAGENOCOMBO,combobox,1344339970
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,button,1342177287
Control33=IDC_STATIC,button,1342177287
Control34=IDC_STATIC,static,1342308352
Control35=IDC_SELECTWHERECHECK1,button,1342242819
Control36=IDC_SEARCHNUMBERSTATIC,static,1342308352

[CLS:CCrashTopFormView]
Type=0
HeaderFile=CrashTopFormView.h
ImplementationFile=CrashTopFormView.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_SORTMODERADIO
VirtualFilter=VWC

[DLG:IDD_CRASHSTATISTICSDLG]
Type=1
Class=CStatisticsFormView
ControlCount=14
Control1=IDC_STARTHOUREDIT,edit,1350631552
Control2=IDC_STARTHOURSPIN,msctls_updown32,1342177330
Control3=IDC_STARTMINUTEEDIT,edit,1350631552
Control4=IDC_STARTMINUTESPIN,msctls_updown32,1342177330
Control5=IDC_ENDHOUREDIT,edit,1350631552
Control6=IDC_ENDHOURSPIN,msctls_updown32,1342177330
Control7=IDC_ENDMINUTEEDIT,edit,1350631552
Control8=IDC_ENDMINUTESPIN,msctls_updown32,1342177330
Control9=IDC_STARTDATETIMEPICKER,SysDateTimePick32,1342242848
Control10=IDC_STATIC,static,1342308352
Control11=IDC_ENDDATETIMEPICKER,SysDateTimePick32,1342242848
Control12=IDC_ALLSTATISTICSRADIO,button,1342308361
Control13=IDC_DATESTATISTICSRADIO,button,1342177289
Control14=IDC_CRASHSTATISTICSBTN,button,1342242816

[CLS:CStatisticsFormView]
Type=0
HeaderFile=StatisticsFormView.h
ImplementationFile=StatisticsFormView.cpp
BaseClass=CFormView
Filter=D
LastObject=CStatisticsFormView
VirtualFilter=VWC

[DLG:IDD_CONNECTDGL]
Type=1
Class=CConnectDlg
ControlCount=3
Control1=IDC_CRASHIPADDRESS,SysIPAddress32,1342242816
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816

[CLS:CConnectDlg]
Type=0
HeaderFile=ConnectDlg.h
ImplementationFile=ConnectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CConnectDlg
VirtualFilter=dWC

[DLG:IDD_CRASHINFOPROCESSDLG]
Type=1
Class=CCrashInfoRecvProcessDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CRASHINFORECVPROCESSBAR,msctls_progress32,1350565888

[CLS:CCrashInfoRecvProcessDlg]
Type=0
HeaderFile=CrashInfoRecvProcessDlg.h
ImplementationFile=CrashInfoRecvProcessDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCrashInfoRecvProcessDlg
VirtualFilter=dWC

