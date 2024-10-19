// CharListUpdateTool.h : main header file for the CHARLISTUPDATETOOL application
//

#if !defined(AFX_CHARLISTUPDATETOOL_H__7644344D_BD40_4E6B_8866_548DE9CC2C5E__INCLUDED_)
#define AFX_CHARLISTUPDATETOOL_H__7644344D_BD40_4E6B_8866_548DE9CC2C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCharListUpdateToolApp:
// See CharListUpdateTool.cpp for the implementation of this class
//

class CCharListUpdateToolApp : public CWinApp
{
public:
	CCharListUpdateToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharListUpdateToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCharListUpdateToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARLISTUPDATETOOL_H__7644344D_BD40_4E6B_8866_548DE9CC2C5E__INCLUDED_)
