// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "servermonitor.h"
#include "LoginDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog


CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDialog)
	m_szID = _T("");
	m_szPassword = _T("");
	//}}AFX_DATA_INIT
}


void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDialog)
	DDX_Control(pDX, IDC_EDITPASSWORD, m_PasswordEdit);
	DDX_Control(pDX, IDC_EDITID, m_IDEdit);
	DDX_Text(pDX, IDC_EDITID, m_szID);
	DDV_MaxChars(pDX, m_szID, 32);
	DDX_Text(pDX, IDC_EDITPASSWORD, m_szPassword);
	DDV_MaxChars(pDX, m_szPassword, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	//{{AFX_MSG_MAP(CLoginDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog message handlers

BOOL CLoginDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_IDEdit.SetLimitText(31);
	m_PasswordEdit.SetLimitText(31);

    m_IDEdit.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
