#if !defined(AFX_STATISTICSFORMVIEW_H__B7235A55_5C89_4DBB_A2DB_C38E9D460284__INCLUDED_)
#define AFX_STATISTICSFORMVIEW_H__B7235A55_5C89_4DBB_A2DB_C38E9D460284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class JXConnectSocket;

class CStatisticsFormView : public CFormView
{
protected:
	CStatisticsFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStatisticsFormView)

public : 
void SetConnection(JXConnectSocket* pConnection) 
		{ m_pConnection = pConnection; }

private : 
	void EnableCtrlWindow(BOOL bEnable);
	BOOL SendCrashStatisticsInfo();

private : 
	BOOL		m_bAllStatistics;
	DWORD		m_dwCtrlID[2][5];

private : 
	JXConnectSocket* m_pConnection;

// Form Data
public:
	//{{AFX_DATA(CStatisticsFormView)
	enum { IDD = IDD_CRASHSTATISTICSDLG };
	CSpinButtonCtrl	m_StartMinuteSpin;
	CSpinButtonCtrl	m_StartHourSpin;
	CSpinButtonCtrl	m_EndMinuteSpin;
	CSpinButtonCtrl	m_EndHourSpin;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStatisticsFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStatisticsFormView)
	afx_msg void OnAllstatisticsradio();
	afx_msg void OnDatestatisticsradio();
	afx_msg void OnCrashstatisticsbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSFORMVIEW_H__B7235A55_5C89_4DBB_A2DB_C38E9D460284__INCLUDED_)
