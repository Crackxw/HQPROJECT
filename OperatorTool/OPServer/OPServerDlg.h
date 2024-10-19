// OPServerDlg.h : header file
//

#if !defined(AFX_OPSERVERDLG_H__F3146EAF_4F42_4EA5_9A33_7EE77D17D5E6__INCLUDED_)
#define AFX_OPSERVERDLG_H__F3146EAF_4F42_4EA5_9A33_7EE77D17D5E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "server.h"
#include "UserCatalog.h"
#include "ConnectionUser.h"

/////////////////////////////////////////////////////////////////////////////
// COPServerDlg dialog

class CClient;
class COPServerDlg : public CDialog
{
// Construction
public:
	COPServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COPServerDlg)
	enum { IDD = IDD_OPSERVER_DIALOG };
	CListBox	m_UserList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPServerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CServer				m_ServerSocket;	
	UserCatalog			m_UserCatalog;
	ConnectionUser		m_ConnectedUser;


	BOOL				m_IsItemEditPossible;

	// Generated message map functions
	//{{AFX_MSG(COPServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	afx_msg void OnReqLogin(WPARAM, LPARAM);
	afx_msg void OnReqItemEdit(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPSERVERDLG_H__F3146EAF_4F42_4EA5_9A33_7EE77D17D5E6__INCLUDED_)
