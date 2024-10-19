#if !defined(AFX_MYEDIT_H__576871C5_AFAA_41A9_92F1_3D97618556C5__INCLUDED_)
#define AFX_MYEDIT_H__576871C5_AFAA_41A9_92F1_3D97618556C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

#define WM_RETURN_MSG	WM_USER + 3

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:
	HWND	m_hParent;	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();	

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__576871C5_AFAA_41A9_92F1_3D97618556C5__INCLUDED_)
