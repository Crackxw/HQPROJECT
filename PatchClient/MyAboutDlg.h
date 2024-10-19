#if !defined(AFX_MYABOUTDLG_H__15F81850_E2D6_4458_B98E_CBEBA161E7EA__INCLUDED_)
#define AFX_MYABOUTDLG_H__15F81850_E2D6_4458_B98E_CBEBA161E7EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyAboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyAboutDlg dialog

class CMyAboutDlg : public CDialog
{
// Construction
public:
	CMyAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyAboutDlg)
	enum { IDD = IDD_ABOUTDDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyAboutDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYABOUTDLG_H__15F81850_E2D6_4458_B98E_CBEBA161E7EA__INCLUDED_)
