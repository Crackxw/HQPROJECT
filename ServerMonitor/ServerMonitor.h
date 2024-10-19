// ServerMonitor.h : main header file for the SERVERMONITOR application
//

#if !defined(AFX_SERVERMONITOR_H__DD7758D2_F068_453C_A5A2_0B59B035E734__INCLUDED_)
#define AFX_SERVERMONITOR_H__DD7758D2_F068_453C_A5A2_0B59B035E734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorApp:
// See ServerMonitor.cpp for the implementation of this class
//

class CServerMonitorApp : public CWinApp
{
public:
	CServerMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerMonitorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CServerMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERMONITOR_H__DD7758D2_F068_453C_A5A2_0B59B035E734__INCLUDED_)
