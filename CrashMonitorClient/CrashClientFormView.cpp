// CrashClientFormView.cpp : implementation file
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "CrashClientFormView.h"
#include "MainFrm.h"

#include "CrashMonitorClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashClientFormView

IMPLEMENT_DYNCREATE(CCrashClientFormView, CFormView)

CCrashClientFormView::CCrashClientFormView()
	: CFormView(CCrashClientFormView::IDD), m_pConnection(NULL)
{
	//{{AFX_DATA_INIT(CCrashClientFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCrashClientFormView::~CCrashClientFormView()
{
}

void CCrashClientFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrashClientFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrashClientFormView, CFormView)
	//{{AFX_MSG_MAP(CCrashClientFormView)	
	ON_BN_CLICKED(IDC_CRASHINFOSPLITTERBTN, OnCrashinfosplitterbtn)
	ON_BN_CLICKED(IDC_CRASHSTATISTICSSPLITTERBTN, OnCrashstatisticssplitterbtn)
	ON_BN_CLICKED(IDC_SELECTMODERADIO, OnSelectmoderadio)
	ON_BN_CLICKED(IDC_SORTMODERADIO, OnSortmoderadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashClientFormView diagnostics

#ifdef _DEBUG
void CCrashClientFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCrashClientFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCrashClientFormView message handlers

void CCrashClientFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	((CButton*)(GetDlgItem(IDC_CRASHINFOSPLITTERBTN)))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_SELECTMODERADIO))->SetCheck(TRUE);
}

/////////////////////////////////////////////////////////////////////////////

CMainFrame* CCrashClientFormView::GetMainFrm()
{
	CCrashMonitorClientApp * pApp = ( CCrashMonitorClientApp * )AfxGetApp();
	if( pApp == NULL )	return NULL;
	CMainFrame * pFrm = NULL;
	pFrm = ( CMainFrame * )pApp->GetMainWnd();
	return pFrm;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashClientFormView::OnCrashinfosplitterbtn() 
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->ChangeView(1);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashClientFormView::OnCrashstatisticssplitterbtn() 
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->ChangeView(2);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashClientFormView::OnSelectmoderadio() 
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->m_pCrashInfoView->SetListMode(FALSE);
	}	
}

/////////////////////////////////////////////////////////////////////////////

void CCrashClientFormView::OnSortmoderadio() 
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->m_pCrashInfoView->SetListMode(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////