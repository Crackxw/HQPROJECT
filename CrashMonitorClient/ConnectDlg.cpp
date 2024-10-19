// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "ConnectDlg.h"
#include "JOCrashClientConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

void CConnectDlg::OnOK() 
{
	// TODO: Add extra validation here

	((CWnd*)(GetDlgItem(IDC_CRASHIPADDRESS)))->GetWindowText(m_strIP);

	if(m_strIP.GetLength() == 0)
	{
		MessageBox("IP를 입력하세요", MB_OK);
		return;
	}

	CJOCrashClientConfig Config;
	Config.SetConfigFileName(GERSANG_CRASH_SERVER_INFO_FILE);

	Config.SetCrashServerIP(m_strIP.GetBuffer(0));
	
	CDialog::OnOK();
}

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CJOCrashClientConfig Config;

	Config.SetConfigFileName(GERSANG_CRASH_SERVER_INFO_FILE);

	CString str(Config.GetCrashServerIP());

	((CWnd*)(GetDlgItem(IDC_CRASHIPADDRESS)))->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
