// ServerMonitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerMonitor.h"
#include "ServerMonitorDlg.h"
#include "GersangServerMonitor.h"
#include "LoginDialog.h"
#include "MySocket.h"
#include "IpDialog.h"
#include "MonitorProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE hRichModule;

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorApp

BEGIN_MESSAGE_MAP(CServerMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CServerMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorApp construction

CServerMonitorApp::CServerMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerMonitorApp object

CServerMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorApp initialization

BOOL CServerMonitorApp::InitInstance()
{
/*	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}*/

	WSADATA wData;

	if (WSAStartup(0x0202, &wData) != 0)
	{
		AfxMessageBox(_T("WSAStartup Failed"));
		return FALSE;

	}



	

	AfxEnableControlContainer();

	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

//	hRichModule = LoadLibrary(_T("riched32.dll"));  
	AfxInitRichEdit();



	

	g_GersangServerMonitor.Init();



	//g_GersangServerMonitor.SetAddress(IpDlg.m_IpString,MASTER_SERVER_PORT);




	

	CServerMonitorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CServerMonitorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (hRichModule) FreeLibrary(hRichModule);
	
	return CWinApp::ExitInstance();
	return FALSE;
}
