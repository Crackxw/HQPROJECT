#if !defined(AFX_DESTINATIONPATHDAILOG_H__3085B554_3728_46D1_B3DE_63A182912A27__INCLUDED_)
#define AFX_DESTINATIONPATHDAILOG_H__3085B554_3728_46D1_B3DE_63A182912A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DestinationPathDailog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDestinationPathDailog dialog

class CDestinationPathDailog : public CDialog
{
// Construction
public:
	CDestinationPathDailog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDestinationPathDailog)
	enum { IDD = IDD_DESTINATIONPATHDIALOG };
	CString	m_strEditString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDestinationPathDailog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private : 
	WORD		m_wDestPathCount;
	CString		m_strDestPath[32];
	CString		m_strTempEdit;
	BYTE		m_byChanged;

public : 
	void SetDestinationPath(WORD wCount, CString* pDestPath);
	BYTE SetModifyPath(CString& str);
	void AddNewPath();

private : 
	void DisplayPath();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDestinationPathDailog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeDestinationpathlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESTINATIONPATHDAILOG_H__3085B554_3728_46D1_B3DE_63A182912A27__INCLUDED_)
