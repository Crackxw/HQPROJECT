// CrashMonitorClientView.h : interface of the CCrashMonitorClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRASHMONITORCLIENTVIEW_H__F98CCEAC_E49C_4CEB_A152_C5422996CCBB__INCLUDED_)
#define AFX_CRASHMONITORCLIENTVIEW_H__F98CCEAC_E49C_4CEB_A152_C5422996CCBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CrashMonitorClientDoc.h"
#include "CrashInfo.h"
#include "..\\XCommon\\JXPacket.h"

class CGridCtrl;
class JXConnectSocket;
class CCrashInfo;
class CMainFrame;

#define	 EDITOR_LOG_GRID_ID		WM_USER+1

class CCrashMonitorClientView : public CScrollView
{
protected: // create from serialization only
	CCrashMonitorClientView();
	DECLARE_DYNCREATE(CCrashMonitorClientView)

// Attributes
public:
	CCrashMonitorClientDoc* GetDocument();
	CMainFrame* GetMainFrm();

private : 
	CGridCtrl*	m_pGridCtrl;

private : 
	void Initialize();
	void InitFormView();

private : 
	CCrashInfo*		m_pCrashInfo;

	/////////////////////////////////////////////////////////////////////
	// Network and Thread ฐüทร
private : 
	static unsigned int WINAPI Run_Process(void* vpThis);
	unsigned int Process();

	BOOL NetWorkdInitialize();

	BOOL Create
		(
				const char* pIP, 
				WORD wPort, 
				WORD snRecvBufSize, 
				WORD snSendBufSize, 
				WORD snPacketQueueSize
		);	
	void StartProcessThread();
	void StopProcessThread();
	void StartSendRecvThread();
	void StopSendRecvThread();

	void RecvMsgProc();
	void AddCrashInfo(const ON_P_RESP_SEARCHCRASHINFO pCrashInfo, int nCrashCount);	

	int  GridDraw(CMainFrame* pFrm, int nRowCount);	
	int  SetItemText(int nRow, int nCol, int nSel, char* pData, CMainFrame* pFrm);
	void SetItemText(int nRow, int nCol, char* pData);
	void AddPageNumber();

	void StatisticsGridDraw(CMainFrame* pFrm);
public : 
	BOOL Connect(const char* pIP);
	void SetConnection();
	void CrashInfoDraw(int nPage);
	void CrashStatisticsInfoDraw();
	BOOL RemoveCrashInfo();
	void SetListMode(BOOL bRetVal);

private : 
	JXConnectSocket*	m_pConnection;
	BOOL				m_bThreadRun;
	HANDLE				m_hExitEvent;
	HANDLE				m_hBreakEvent;
	JXPacket			m_LocalPacket;
	/////////////////////////////////////////////////////////////////////
	
private : 	
	ON_RESP_CRASHSTATISTICSINFO m_sCrashStatisticsInfo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashMonitorClientView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCrashMonitorClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCrashMonitorClientView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CrashMonitorClientView.cpp
inline CCrashMonitorClientDoc* CCrashMonitorClientView::GetDocument()
   { return (CCrashMonitorClientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHMONITORCLIENTVIEW_H__F98CCEAC_E49C_4CEB_A152_C5422996CCBB__INCLUDED_)
