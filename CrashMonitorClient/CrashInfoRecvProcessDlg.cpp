// CrashInfoRecvProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "CrashInfoRecvProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashInfoRecvProcessDlg dialog


CCrashInfoRecvProcessDlg::CCrashInfoRecvProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashInfoRecvProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrashInfoRecvProcessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCrashInfoRecvProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrashInfoRecvProcessDlg)
	DDX_Control(pDX, IDC_CRASHINFORECVPROCESSBAR, m_CopyFileProgressCtrl);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrashInfoRecvProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CCrashInfoRecvProcessDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashInfoRecvProcessDlg message handlers

BOOL CCrashInfoRecvProcessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CopyFileProgressCtrl.SetRange(0, 100);
	m_CopyFileProgressCtrl.SetStep(1);

	SetTimer(PROGRESS_TIMER, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCrashInfoRecvProcessDlg::OnOK() 
{
	KillTimer(PROGRESS_TIMER);
	CDialog::OnOK();
}

void CCrashInfoRecvProcessDlg::OnCancel() 
{	
	KillTimer(PROGRESS_TIMER);
	CDialog::OnCancel();
}

void CCrashInfoRecvProcessDlg::OnTimer(UINT nIDEvent) 
{
	
	if(nIDEvent == PROGRESS_TIMER)
	{
		m_CopyFileProgressCtrl.StepIt();
	}
	
	CDialog::OnTimer(nIDEvent);
}
