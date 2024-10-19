#if !defined(AFX_CRASHTOPFORMVIEW_H__1227602C_F8AC_4A01_875C_1769E8B1B3B1__INCLUDED_)
#define AFX_CRASHTOPFORMVIEW_H__1227602C_F8AC_4A01_875C_1769E8B1B3B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrashTopFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrashTopFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Defines.h"

#include "CrashInfoRecvProcessDlg.h"

class JXConnectSocket;
class CMainFrame;

#define ALL_SELECT_OK		BST_CHECKED
#define ALL_SELECT_CANCEL   0


class CCrashTopFormView : public CFormView
{
protected:
	CCrashTopFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrashTopFormView)

// Form Data
public:
	//{{AFX_DATA(CCrashTopFormView)
	enum { IDD = IDD_CRASHTOPFORMDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
// Attributes
private : 
	void InitFormView();
	void FileldSelectList(int nChecked);
	void InitCtrlData();
	void SelectWhere(int nSel);
	void EnableWhereCtrl(int nRowWhere, BOOL bCheck);
	void AllEnalbeWhereCtrl(BOOL bCheck);
	void CheckCtrlAble(BOOL bCheck);
	void SetCheck(BOOL bCheck);	

private : 
	BOOL SendSearchCrashInfo();
	BOOL SendWhereCrashInfo();

public	: 
	void SetConnection(JXConnectSocket* pConnection) 
		{ m_pConnection = pConnection; }
	BOOL IsFieldChecked(int nCur);
	BOOL GetFieldCount();
	CMainFrame* GetMainFrm();
	void AddPageNumber(int nCurRow, int nMaxNum);
	void SetSearchNumber(int nCount);

private : 
	JXConnectSocket* m_pConnection;
// Attributes
private:
	CCheckListBox	m_FieldSelectList;

private : 
	BOOL			m_bAllField;
	int				m_nWhereSel[MAX_SELECT_WHERE_NUMBER+1];
	DWORD			m_dwWhereCtrnID[5][MAX_SELECT_WHERE_NUMBER+1];

	int				m_nCurSel;
	
private : 
	BOOL						m_bDlgOpen;
	CCrashInfoRecvProcessDlg	m_ProcessDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashTopFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCrashTopFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCrashTopFormView)
	afx_msg void OnFieldselectokbtn();
	afx_msg void OnFieldselectcancelbtn();
	afx_msg void OnSelectwherecheck1();
	afx_msg void OnSelectwherecheck2();
	afx_msg void OnSelectwherecheck3();
	afx_msg void OnSelectwherecheck4();
	afx_msg void OnAllfieldopt();
	afx_msg void OnAllwhereopt();
	afx_msg void OnSearchbtn();
	afx_msg void OnSelchangeFiledselectlist();
	afx_msg void OnSelchangeCrashpagenocombo();
	afx_msg void OnSelectwherecheck5();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHTOPFORMVIEW_H__1227602C_F8AC_4A01_875C_1769E8B1B3B1__INCLUDED_)
