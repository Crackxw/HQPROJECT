#if !defined(AFX_IPDIALOG_H__2D1BD646_4C43_4AD5_9981_61F5D6A4DDCF__INCLUDED_)
#define AFX_IPDIALOG_H__2D1BD646_4C43_4AD5_9981_61F5D6A4DDCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IpDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIpDialog dialog

class CIpDialog : public CDialog
{
// Construction
public:
	CIpDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIpDialog)
	enum { IDD = IDD_IPDIALOG };
	CComboBox	m_IpComboBox;
	CString	m_IpString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIpDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPDIALOG_H__2D1BD646_4C43_4AD5_9981_61F5D6A4DDCF__INCLUDED_)
