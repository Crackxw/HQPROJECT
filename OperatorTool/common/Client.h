#if !defined(AFX_CLIENT_H__CED0517B_7124_49CD_A26D_7D78DF09FDF6__INCLUDED_)
#define AFX_CLIENT_H__CED0517B_7124_49CD_A26D_7D78DF09FDF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Client.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClient command target

class CClient : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClient();
	virtual ~CClient();

	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	void		SetHwnd(HWND hWnd)	{ m_hWnd = hWnd; }

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClient)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	HWND			m_hWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__CED0517B_7124_49CD_A26D_7D78DF09FDF6__INCLUDED_)
