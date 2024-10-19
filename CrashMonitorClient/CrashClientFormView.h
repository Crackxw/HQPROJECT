#if !defined(AFX_CRASHCLIENTFORMVIEW_H__531FCECC_3F51_4B2E_9153_4EB9AF92670E__INCLUDED_)
#define AFX_CRASHCLIENTFORMVIEW_H__531FCECC_3F51_4B2E_9153_4EB9AF92670E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrashClientFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrashClientFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMainFrame;
class JXConnectSocket;

class CCrashClientFormView : public CFormView
{
protected:
	CCrashClientFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrashClientFormView)

// Form Data
public:
	//{{AFX_DATA(CCrashClientFormView)
	enum { IDD = IDD_CRASHFORMDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private : 
	CMainFrame* GetMainFrm();

public	: 
	void SetConnection(JXConnectSocket* pConnection) 
		{ m_pConnection = pConnection; }

private : 
	JXConnectSocket* m_pConnection;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashClientFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCrashClientFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCrashClientFormView)	
	afx_msg void OnCrashinfosplitterbtn();
	afx_msg void OnCrashstatisticssplitterbtn();
	afx_msg void OnSelectmoderadio();
	afx_msg void OnSortmoderadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHCLIENTFORMVIEW_H__531FCECC_3F51_4B2E_9153_4EB9AF92670E__INCLUDED_)
