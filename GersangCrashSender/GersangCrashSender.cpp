// GersangCrashSender.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GersangCrashSender.h"
#include "GersangCrashSenderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderApp

BEGIN_MESSAGE_MAP(CGersangCrashSenderApp, CWinApp)
	//{{AFX_MSG_MAP(CGersangCrashSenderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderApp construction

CGersangCrashSenderApp::CGersangCrashSenderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGersangCrashSenderApp object

CGersangCrashSenderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderApp initialization

BOOL CGersangCrashSenderApp::InitInstance()
{
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

	CGersangCrashSenderDlg dlg;

	if ( m_lpCmdLine && strlen(m_lpCmdLine) > 0 )
	{
		char szStartTime[256] = "", szCrashInfo[MAX_PATH*2];
		char szLineAddress[2048+512];
		strncpy( szLineAddress, m_lpCmdLine, 2048+512 );	
		strncpy( szStartTime, strtok( m_lpCmdLine, " " ), 256 );		
		strncpy( szCrashInfo, strtok( NULL, " " ), 256 );
		dlg.SetCrashInfo( szStartTime, szCrashInfo, szLineAddress );
	}
	else
		return FALSE;

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
