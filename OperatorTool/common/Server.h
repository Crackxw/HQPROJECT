#if !defined(AFX_SERVER_H__6D0CF2F1_CE54_43C8_A121_ECA6B7DD9036__INCLUDED_)
#define AFX_SERVER_H__6D0CF2F1_CE54_43C8_A121_ECA6B7DD9036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Server.h : header file
//

#include "Client.h"

#include <iostream>
#include <list>

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CServer command target
class CServer : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CServer();
	virtual ~CServer();

	virtual void OnAccept(int nErrorCode);

	void		SetHwnd(HWND hWnd)	{ m_hWnd = hWnd; }

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServer)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	HWND			m_hWnd;

	list<CClient*>	m_ClientSocket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__6D0CF2F1_CE54_43C8_A121_ECA6B7DD9036__INCLUDED_)
