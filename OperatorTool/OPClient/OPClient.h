// OPClient.h : main header file for the OPCLIENT application
//

#if !defined(AFX_OPCLIENT_H__5EB8DB74_BFE4_455D_B064_4C570480C267__INCLUDED_)
#define AFX_OPCLIENT_H__5EB8DB74_BFE4_455D_B064_4C570480C267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COPClientApp:
// See OPClient.cpp for the implementation of this class
//

class UnDo;
class COPClientApp : public CWinApp
{
public:
	COPClientApp();
	
	UnDo		*m_pUndo;	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPClientApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COPClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPCLIENT_H__5EB8DB74_BFE4_455D_B064_4C570480C267__INCLUDED_)
