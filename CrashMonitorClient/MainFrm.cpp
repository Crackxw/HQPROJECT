// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "MainFrm.h"

#include "CrashMonitorClientView.h"
#include "CrashClientFormView.h"
#include "CrashTopFormView.h"
#include "StatisticsFormView.h"
#include "ConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDM_CRASHSERVERCONNECT, OnCrashserverconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_bInit(FALSE)
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	int x = GetSystemMetrics( SM_CXMAXTRACK );
	int y = GetSystemMetrics( SM_CYMAXTRACK );
	cs.cx = x-10;
	cs.cy = y-10;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

		// TODO: Add your specialized code here and/or call the base class

	CRect rtCltRect;
	GetClientRect(rtCltRect);
	m_wndSplitter.CreateStatic(this, 1, 2);

	m_wndCrashInfoView.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, 
		m_wndSplitter.IdFromRowCol(0, 1));

	m_wndSplitter.CreateView(0, 0, 
		RUNTIME_CLASS(CCrashClientFormView), CSize( 0, 0 ), pContext);
	m_wndCrashInfoView.CreateView(1, 0, 
		RUNTIME_CLASS(CCrashMonitorClientView), CSize( 0, 0 ), pContext);	


	m_nCrashTopFormView = 
		m_wndCrashInfoView.AddView(0, 0, RUNTIME_CLASS(CCrashTopFormView),pContext);	
	m_nCrashStatisticsFormView = 
		m_wndCrashInfoView.AddView(0, 0, RUNTIME_CLASS(CStatisticsFormView),pContext);		
	

	m_wndSplitter.SetColumnInfo(0, rtCltRect.Width() / 5, 10);
	m_wndCrashInfoView.SetRowInfo(0, rtCltRect.Height() / 3+30, 10);

	m_pCrashTopFormView = 
		(CCrashTopFormView*)m_wndCrashInfoView.GetView((m_nCrashTopFormView));	
	m_pCrashStatisticsFormView = 
		(CStatisticsFormView*)m_wndCrashInfoView.GetView((m_nCrashStatisticsFormView));	

	m_pCrashFormView	= (CCrashClientFormView*)m_wndSplitter.GetPane( 0, 0 );
	m_pCrashInfoView	= (CCrashMonitorClientView*)m_wndCrashInfoView.GetPane( 1, 0 );

	return TRUE;
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::ChangeView(int nView)
{
	if(nView >= 1 && nView <= 2)
	{
		m_wndCrashInfoView.ShowView(nView);
		if(nView == 1 && m_bInit == TRUE)
		{
			m_pCrashInfoView->CrashInfoDraw(1);
		}
		else if(nView == 2 && m_bInit == TRUE)
		{
			m_pCrashInfoView->CrashStatisticsInfoDraw();
		}

		m_bInit = TRUE;
	}
}

void CMainFrame::OnCrashserverconnect() 
{
	CConnectDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		if(m_pCrashInfoView->Connect(dlg.m_strIP.GetBuffer(0)) == TRUE)
		{			
			MessageBox("연결 성공", MB_OK);
		}
		else
		{
			MessageBox("연결 실패", MB_OK);
		}
	}
}
