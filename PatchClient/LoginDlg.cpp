// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchTool.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_strID = _T("");
	m_strPassword = _T("");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnClose();
}

void CLoginDlg::SetID(char* pStr)
{
	strcpy(pStr, m_strID.GetBuffer(0));
}

void CLoginDlg::SetPW(char* pStr)
{
	strcpy(pStr, m_strPassword.GetBuffer(0));
}

void CLoginDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CWnd* pID = GetDlgItem(IDC_IDEDIT);
	CWnd* pPW = GetDlgItem(IDC_PWEDIT);

	pID->GetWindowText(m_strID);
	pPW->GetWindowText(m_strPassword);
	
	CDialog::OnCancel();
}

void CLoginDlg::OnOK() 
{
	// TODO: Add extra validation here

	CWnd* pID = GetDlgItem(IDC_IDEDIT);
	CWnd* pPW = GetDlgItem(IDC_PWEDIT);

	pID->GetWindowText(m_strID);
	pPW->GetWindowText(m_strPassword);
	
	CDialog::OnOK();
}
