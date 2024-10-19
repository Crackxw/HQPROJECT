// MyAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchTool.h"
#include "MyAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyAboutDlg dialog


CMyAboutDlg::CMyAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CMyAboutDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyAboutDlg message handlers
