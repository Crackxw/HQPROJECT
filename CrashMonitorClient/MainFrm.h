// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8034F9F1_D078_40F5_98D3_1BBE496F4745__INCLUDED_)
#define AFX_MAINFRM_H__8034F9F1_D078_40F5_98D3_1BBE496F4745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\\Ctrl\\AW_CMultiViewSplitter.h"

class CCrashMonitorClientView;
class CCrashClientFormView;
class CCrashTopFormView;
class CStatisticsFormView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public : 
	void ChangeView(int nView);

public : 
	CCrashMonitorClientView*	m_pCrashInfoView;
	CCrashClientFormView*		m_pCrashFormView;
	CCrashTopFormView*			m_pCrashTopFormView;
	CStatisticsFormView*		m_pCrashStatisticsFormView;

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CSplitterWnd			m_wndSplitter;
	CSplitterWnd			m_wndCrashFormView;
	AW_CMultiViewSplitter	m_wndCrashInfoView;	

	int						m_nCrashTopFormView;
	int						m_nCrashStatisticsFormView;
	BOOL					m_bInit;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCrashserverconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8034F9F1_D078_40F5_98D3_1BBE496F4745__INCLUDED_)
