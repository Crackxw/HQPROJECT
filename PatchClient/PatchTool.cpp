// PatchTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PatchTool.h"
#include "PatchToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchToolApp

BEGIN_MESSAGE_MAP(CPatchToolApp, CWinApp)
	//{{AFX_MSG_MAP(CPatchToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchToolApp construction

CPatchToolApp::CPatchToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPatchToolApp object

CPatchToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPatchToolApp initialization

BOOL CPatchToolApp::InitInstance()
{

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

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Initialize OLE libraris.
	//
	if(!AfxOleInit())
		return FALSE;

	CPatchToolDlg dlg;
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
