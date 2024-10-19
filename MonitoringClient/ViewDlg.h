#if !defined(AFX_VIEWDLG_H__281918B3_3DB0_47E9_BE4C_C5DA64B252F3__INCLUDED_)
#define AFX_VIEWDLG_H__281918B3_3DB0_47E9_BE4C_C5DA64B252F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog

class CViewDlg : public CDialog
{
// Construction
public:
	CViewDlg(CWnd* pParent = NULL);   // standard constructor

	void OnOK();
	void OnCancel();

// Dialog Data
	//{{AFX_DATA(CViewDlg)
	enum { IDD = IDD_VIEW_DIALOG };
	CButton	m_ServerOnBtn;
	CButton	m_ServerOffBtn;
	CListCtrl	m_ServerListCtrl;
	CEdit	m_NoticeIntervalEdit;
	CEdit	m_NoticeEdit;
	CButton	m_NoticeBtn;
	CEdit	m_ChatInputEdit;
	CEdit	m_KickTimerEdit;
	CEdit	m_strLogEdit;
	CButton	m_KickAllBtn;
	CButton	m_FileTransBtn;
	CListBox	m_ChatViewerListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewDlg)
	afx_msg void OnNoticeButton();
	afx_msg void OnFiletransButton();
	afx_msg void OnServeroffButton();
	afx_msg void OnServeronButton();
	afx_msg void OnKickallButton();
	afx_msg void OnExitButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDLG_H__281918B3_3DB0_47E9_BE4C_C5DA64B252F3__INCLUDED_)
