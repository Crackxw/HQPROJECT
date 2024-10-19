// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "OPClient.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN)
	{
		::SendMessage(m_hParent, WM_RETURN_MSG, 0, 0);

		SetWindowText("");
		ShowWindow(SW_HIDE);
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}
