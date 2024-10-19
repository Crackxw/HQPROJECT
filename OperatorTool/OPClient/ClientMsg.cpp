#include "stdafx.h"
#include "OPClient.h"

#include "Graphics16.h"
#include "OPClientDlg.h"
#include "TextDlg.h"

#include "..\\common\\defines.h"

void COPClientDlg::OnLogin(WPARAM wParam, LPARAM lParam)
{
	ResLogin	*pLogin = (ResLogin*)lParam;

	if(pLogin->nLoginState == ResLogin::LOGIN_OK)
	{
		GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText("로그인 성공");

		// 로그인 박스를 숨긴다.
		disableLogin(SW_HIDE);

		GetDlgItem(IDC_BUTTON_ITEM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_CHAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_TEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_QUIT)->ShowWindow(SW_SHOW);

		return;
	}

	GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText(pLogin->strLoginMsg);
}

void COPClientDlg::OnItemEdit(WPARAM wParam, LPARAM lParam)
{
	ResItemEdit	*pItemEdit = (ResItemEdit*)lParam;


	m_ItemDlg.SetReadOnlyMode(pItemEdit->bUsePossible);

	// 사용 불가능
	if(!pItemEdit->bUsePossible)
		AfxMessageBox(pItemEdit->strUserName);

	RECT rt;
	GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(&rt);

	::MoveWindow(m_ItemDlg.m_hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
	m_ItemDlg.ShowWindow(SW_SHOW);

	m_CharDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
//	m_ModifyDlg.ShowWindow(SW_HIDE);
}