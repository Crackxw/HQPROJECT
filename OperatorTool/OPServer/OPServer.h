// OPServer.h : main header file for the OPSERVER application
//

#if !defined(AFX_OPSERVER_H__D68597B2_8414_4EEB_AF47_AAB4E3400233__INCLUDED_)
#define AFX_OPSERVER_H__D68597B2_8414_4EEB_AF47_AAB4E3400233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COPServerApp:
// See OPServer.cpp for the implementation of this class
//

class COPServerApp : public CWinApp
{
public:
	COPServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COPServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPSERVER_H__D68597B2_8414_4EEB_AF47_AAB4E3400233__INCLUDED_)
