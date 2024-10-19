#if !defined(AFX_CRASHINFORECVPROCESSDLG_H__0F708627_1ABC_4F51_9949_83156A539165__INCLUDED_)
#define AFX_CRASHINFORECVPROCESSDLG_H__0F708627_1ABC_4F51_9949_83156A539165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrashInfoRecvProcessDlg.h : header file
//

#include ".\\Ctrl\\TextProgressCtrl.h"

#define	PROGRESS_TIMER		WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CCrashInfoRecvProcessDlg dialog

class CCrashInfoRecvProcessDlg : public CDialog
{
// Construction
public:
	CCrashInfoRecvProcessDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCrashInfoRecvProcessDlg)
	enum { IDD = IDD_CRASHINFOPROCESSDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashInfoRecvProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private : 
		CTextProgressCtrl m_CopyFileProgressCtrl;
public : 
		virtual void OnCancel();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCrashInfoRecvProcessDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHINFORECVPROCESSDLG_H__0F708627_1ABC_4F51_9949_83156A539165__INCLUDED_)
