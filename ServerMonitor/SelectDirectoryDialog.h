#if !defined(AFX_SELECTDIRECTORYDIALOG_H__5A412929_B4C3_449A_8D39_D144E513DA2F__INCLUDED_)
#define AFX_SELECTDIRECTORYDIALOG_H__5A412929_B4C3_449A_8D39_D144E513DA2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDirectoryDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectDirectoryDialog dialog

class CSelectDirectoryDialog : public CDialog
{
// Construction
public:
	CSelectDirectoryDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectDirectoryDialog)
	enum { IDD = IDD_SELECTDIRECTORY };
	CComboBox	m_DirectoryComboBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectDirectoryDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectDirectoryDialog)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDIRECTORYDIALOG_H__5A412929_B4C3_449A_8D39_D144E513DA2F__INCLUDED_)
