#if !defined(AFX_ATTACHPATHDLG_H__F78F73A3_62C3_44DC_B29F_3FA4C132C067__INCLUDED_)
#define AFX_ATTACHPATHDLG_H__F78F73A3_62C3_44DC_B29F_3FA4C132C067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AttachPathDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAttachPathDlg dialog

class CAttachPathDlg : public CDialog
{
// Construction
public:
	CAttachPathDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttachPathDlg)
	enum { IDD = IDD_PATH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttachPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private : 
	CString		m_strAttachPath;

public : 
	BOOL		GetAttach(CString& str);
	void		SetAttach(CString str);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttachPathDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTACHPATHDLG_H__F78F73A3_62C3_44DC_B29F_3FA4C132C067__INCLUDED_)
