// AttachPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchTool.h"
#include "AttachPathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttachPathDlg dialog


CAttachPathDlg::CAttachPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttachPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttachPathDlg)
		// NOTE: the ClassWizard will add member initialization here

	m_strAttachPath = _T("");
	//}}AFX_DATA_INIT
}


void CAttachPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttachPathDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttachPathDlg, CDialog)
	//{{AFX_MSG_MAP(CAttachPathDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttachPathDlg message handlers

BOOL CAttachPathDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_ATTACHVERSIONEDIT);
	pWnd->SetWindowText(m_strAttachPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAttachPathDlg::GetAttach(CString& str)
{
	if(m_strAttachPath.GetLength() == 0)
		return FALSE;

	str = _T("");
	str = m_strAttachPath;

	return TRUE;
}

void CAttachPathDlg::SetAttach(CString str)
{
	m_strAttachPath = str;
}

void CAttachPathDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CWnd* pWnd = GetDlgItem(IDC_ATTACHVERSIONEDIT);
	pWnd->GetWindowText(m_strAttachPath);

	CDialog::OnCancel();
}

void CAttachPathDlg::OnOK() 
{
	// TODO: Add extra validation here

	CWnd* pWnd = GetDlgItem(IDC_ATTACHVERSIONEDIT);
	pWnd->GetWindowText(m_strAttachPath);
	CDialog::OnOK();
}
