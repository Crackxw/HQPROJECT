// OPClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OPClient.h"
#include "OPClientDlg.h"

#include "Undo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COPClientApp

BEGIN_MESSAGE_MAP(COPClientApp, CWinApp)
	//{{AFX_MSG_MAP(COPClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPClientApp construction

COPClientApp::COPClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COPClientApp object

COPClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COPClientApp initialization

BOOL COPClientApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
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

	COPClientDlg dlg;
	m_pMainWnd = &dlg;

	m_pUndo = new UnDo(&dlg);

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
	delete m_pUndo;
	return FALSE;
}

BOOL COPClientApp::PreTranslateMessage(MSG* pMsg) 
{
/*
	if(pMsg->message == WM_KEYUP)		
	{
		if(pMsg->wParam == 90 && GetAsyncKeyState(VK_CONTROL))
		{
			// 언두할 내용을 얻어 온다.		
			int nCount = m_pUndo->m_pDlg->m_ModifyDlg.m_TextList.GetCount() - 1;
			
			if(nCount < 0)	return 0;

			CString Data;

			m_pUndo->m_pDlg->m_ModifyDlg.m_TextList.GetText(nCount, Data);
			m_pUndo->AnalysisUndo((LPSTR)(LPCTSTR)Data);

			m_pUndo->m_pDlg->m_ModifyDlg.m_TextList.DeleteString(nCount);
		}

		if(GetAsyncKeyState(89) && GetAsyncKeyState(VK_CONTROL))
		{
		}
	}
*/
	return CWinApp::PreTranslateMessage(pMsg);
}
