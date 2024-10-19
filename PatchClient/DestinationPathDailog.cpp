// DestinationPathDailog.cpp : implementation file
//

#include "stdafx.h"
#include "PatchTool.h"
#include "DestinationPathDailog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDestinationPathDailog dialog


CDestinationPathDailog::CDestinationPathDailog(CWnd* pParent /*=NULL*/)
	: CDialog(CDestinationPathDailog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDestinationPathDailog)
	m_wDestPathCount = 0;
	m_byChanged = 0;
	m_strEditString = _T("");
	m_strTempEdit	= _T("");
	//}}AFX_DATA_INIT
}

void CDestinationPathDailog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDestinationPathDailog)
	DDX_Text(pDX, IDC_DESTINATIONPATHEDIT, m_strEditString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDestinationPathDailog, CDialog)
	//{{AFX_MSG_MAP(CDestinationPathDailog)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_DESTINATIONPATHLIST, OnSelchangeDestinationpathlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDestinationPathDailog message handlers

BOOL CDestinationPathDailog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	DisplayPath();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::DisplayPath()
{
	if		(m_wDestPathCount <= 0) return;	
	else if	(m_wDestPathCount > 32) m_wDestPathCount = 32;

	CListBox* pListBox = 
		static_cast<CListBox*>(GetDlgItem(IDC_DESTINATIONPATHLIST));
	for(WORD n = 0 ; n < m_wDestPathCount ; n++)
	{
		pListBox->AddString(m_strDestPath[n]);
	}
	pListBox->SetCurSel(0);
	CWnd* pWnd = GetDlgItem(IDC_DESTINATIONPATHEDIT);
	pListBox->GetText(0, m_strEditString);
	pWnd->SetWindowText(m_strEditString);	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::OnOK() 
{
	// TODO: Add extra validation here

	if(m_strEditString.GetLength() == 0)
	{
		MessageBox("Path input.", "Message", MB_OK);
		return;
	}

	CWnd* pWnd = GetDlgItem(IDC_DESTINATIONPATHEDIT);
	pWnd->GetWindowText(m_strTempEdit);

	AddNewPath();
	
	CDialog::OnOK();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::SetDestinationPath(WORD wCount, CString* pDestPath)
{
	if		(wCount <= 0)	return;
	else if	(wCount > 32)	m_wDestPathCount = 32;
	else					m_wDestPathCount = wCount;

	for(int n = 0 ; n < m_wDestPathCount ; n++)
	{
		m_strDestPath[n] = pDestPath[n];
	}	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::AddNewPath()
{
	if(m_strTempEdit.GetLength() == 0)	return;
	
	CListBox* pListBox= 
		static_cast<CListBox*>(GetDlgItem(IDC_DESTINATIONPATHLIST));

	int nCount = pListBox->GetCount();
	CString strCmp;
	BOOL bChanged = TRUE;

	if(nCount >= 32) 
		m_byChanged = 2;
	else{
		for(int n = 0 ; n < nCount ; n++)
		{
			pListBox->GetText(n, strCmp);
			if(m_strTempEdit == strCmp)
			{
				bChanged = FALSE;
			}
		}
		m_byChanged = (bChanged == TRUE ? 1 : 0);
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
void CDestinationPathDailog::OnSelchangeDestinationpathlist() 
{
	CListBox* pListBox= 
		static_cast<CListBox*>(GetDlgItem(IDC_DESTINATIONPATHLIST));

	int nCursel = pListBox->GetCurSel();
	pListBox->GetText(nCursel, m_strTempEdit);

	CWnd* pWnd = GetDlgItem(IDC_DESTINATIONPATHEDIT);
	pWnd->SetWindowText(m_strTempEdit);	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
BYTE CDestinationPathDailog::SetModifyPath(CString& str)
{
	str =_T("");
	str = m_strTempEdit;

	return m_byChanged;
}
