// CrashMonitorClient.h : main header file for the CRASHMONITORCLIENT application
//

#if !defined(AFX_CRASHMONITORCLIENT_H__9C82BDAC_54AC_4359_8B04_58E33622417B__INCLUDED_)
#define AFX_CRASHMONITORCLIENT_H__9C82BDAC_54AC_4359_8B04_58E33622417B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientApp:
// See CrashMonitorClient.cpp for the implementation of this class
//

class CCrashMonitorClientApp : public CWinApp
{
public:
	CCrashMonitorClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashMonitorClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCrashMonitorClientApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHMONITORCLIENT_H__9C82BDAC_54AC_4359_8B04_58E33622417B__INCLUDED_)
