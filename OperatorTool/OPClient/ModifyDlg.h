#if !defined(AFX_MODIFYDLG_H__BADC30F6_257D_4BF5_965F_63BF24E60003__INCLUDED_)
#define AFX_MODIFYDLG_H__BADC30F6_257D_4BF5_965F_63BF24E60003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyDlg.h : header file
//

#pragma warning ( disable : 4786 )

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "TextModify.h"

/////////////////////////////////////////////////////////////////////////////
// CModifyDlg dialog

class CModifyDlg : public CDialog
{
// Construction
public:
	CModifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifyDlg)
	enum { IDD = IDD_DIALOG_MODIFY };
	CListBox	m_TextList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYDLG_H__BADC30F6_257D_4BF5_965F_63BF24E60003__INCLUDED_)
