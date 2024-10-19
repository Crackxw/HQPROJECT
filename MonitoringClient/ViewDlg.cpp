// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonitoringClient.h"
#include "ViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog


CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDlg)
	//}}AFX_DATA_INIT
}


void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDlg)
	DDX_Control(pDX, IDC_SERVERON_BUTTON, m_ServerOnBtn);
	DDX_Control(pDX, IDC_SERVEROFF_BUTTON, m_ServerOffBtn);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerListCtrl);
	DDX_Control(pDX, IDC_NOTICEINTERVAL_EDIT, m_NoticeIntervalEdit);
	DDX_Control(pDX, IDC_NOTICE_EDIT, m_NoticeEdit);
	DDX_Control(pDX, IDC_NOTICE_BUTTON, m_NoticeBtn);
	DDX_Control(pDX, IDC_CHATINPUT_EDIT, m_ChatInputEdit);
	DDX_Control(pDX, IDC_KICKTIMER_EDIT, m_KickTimerEdit);
	DDX_Control(pDX, IDC_LOG_EDIT, m_strLogEdit);
	DDX_Control(pDX, IDC_KICKALL_BUTTON, m_KickAllBtn);
	DDX_Control(pDX, IDC_FILETRANS_BUTTON, m_FileTransBtn);
	DDX_Control(pDX, IDC_CHATVIEWER_LIST, m_ChatViewerListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
	//{{AFX_MSG_MAP(CViewDlg)
	ON_BN_CLICKED(IDC_NOTICE_BUTTON, OnNoticeButton)
	ON_BN_CLICKED(IDC_FILETRANS_BUTTON, OnFiletransButton)
	ON_BN_CLICKED(IDC_SERVEROFF_BUTTON, OnServeroffButton)
	ON_BN_CLICKED(IDC_SERVERON_BUTTON, OnServeronButton)
	ON_BN_CLICKED(IDC_KICKALL_BUTTON, OnKickallButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDlg message handlers

void CViewDlg::OnOK()
{
	return;
}

void CViewDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CViewDlg::OnNoticeButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDlg::OnFiletransButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDlg::OnServeroffButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDlg::OnServeronButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDlg::OnKickallButton() 
{
	// TODO: Add your control notification handler code here
	
}

void CViewDlg::OnExitButton() 
{
	// TODO: Add your control notification handler code here

	CDialog::OnCancel();	
}
