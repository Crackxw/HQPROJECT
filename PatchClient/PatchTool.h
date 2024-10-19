// PatchTool.h : main header file for the PATCHTOOL application
//

#if !defined(AFX_PATCHTOOL_H__4F622CB6_E40D_416C_88FC_68354A7D863C__INCLUDED_)
#define AFX_PATCHTOOL_H__4F622CB6_E40D_416C_88FC_68354A7D863C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPatchToolApp:
// See PatchTool.cpp for the implementation of this class
//

class CPatchToolApp : public CWinApp
{
public:
	CPatchToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPatchToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHTOOL_H__4F622CB6_E40D_416C_88FC_68354A7D863C__INCLUDED_)
