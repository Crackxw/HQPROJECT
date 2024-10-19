// WebCtrlMgr.cpp: implementation of the CWebCtrlMgr class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include "CommCtrl.h" 
#include "iOnlineMouse.h"
#include "Graphics16.h"

#include "..\WebControl\precomp.h"
#include "WebCtrlMgr.h"

CWebCtrlMgr* CWebCtrlMgr::m_pThisPointer = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebCtrlMgr::CWebCtrlMgr() : m_hDlg(NULL), 
						     m_hWndParent(NULL),
						     m_hInstParent(NULL),
							 m_ddClipper(NULL),
							 m_pOnlineMouse(NULL)
{
	m_pThisPointer = this;
}

CWebCtrlMgr::~CWebCtrlMgr()
{
	UninitWebCtrl();

	if( m_ddClipper)
	{
		m_ddClipper->Release();
		m_ddClipper = NULL;
	}
}

BOOL CWebCtrlMgr::Create(HWND hWndParent)
{
	m_hWndParent = hWndParent;	

    DDCAPS ddcaps;
    ZeroMemory( &ddcaps, sizeof(ddcaps) );
    ddcaps.dwSize = sizeof(ddcaps);
    clGrp.lpDD7->GetCaps( &ddcaps, NULL );

    if( (ddcaps.dwCaps2 & DDCAPS2_CANRENDERWINDOWED) == 0 )
        return FALSE;

	InitWebCtrl();
	m_hDlg = CreateDialog(m_hInstParent, MAKEINTRESOURCE(IDD_WEBCONTROL), m_hWndParent, CWebCtrlMgr::WebCtrlProc);
	ShowWindow(m_hDlg, SW_SHOWNORMAL);

	if( clGrp.lpDD7->CreateClipper(0, &m_ddClipper, NULL) == DD_OK)
		m_ddClipper->SetHWnd(0, m_hWndParent);

 	clGrp.FlipToGDISurface();
	return TRUE;
}

void CWebCtrlMgr::DoSystemPaletteUse()
{
	HDC hDC = GetDC(m_hDlg);
	SetSystemPaletteUse(hDC, SYSPAL_STATIC);
	ReleaseDC(m_hDlg, hDC);
}

void CWebCtrlMgr::DetermineShowCursor()
{
	static int bShowCursor = 0;

	RECT rt;
	POINT ptMouse;
	
	GetCursorPos(&ptMouse);
	GetWindowRect(m_hDlg, &rt);
	
	HWND hWnd = GetDlgItem(m_hDlg, IDD_WEBCONTROL);

	if( rt.left <= ptMouse.x && rt.right  >= ptMouse.x &&
		rt.top  <= ptMouse.y && rt.bottom >= ptMouse.y )
	{
		if( !bShowCursor)
		{
			SetCapture(hWnd);
			while( ShowCursor(TRUE) < 0);
			bShowCursor = TRUE;
		} 
	}
	else
	{
		ReleaseCapture();
		while( ShowCursor(FALSE) >= 0);
		bShowCursor = FALSE;
	}
}

BOOL CALLBACK CWebCtrlMgr::WebCtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bIn = FALSE;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText( hWnd, IDC_WEBCTRL, "http://www.google.co.kr" );
		FORWARD_WM_NEXTDLGCTL( hWnd, GetDlgItem( hWnd, IDOK ), 1, PostMessage );
		break;

	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID( wParam, lParam ) )
		{
			case IDOK:
			case IDCANCEL:
				DestroyWindow( hWnd );
				break;
			default:
				return FALSE;
		}
		break;

	case WM_QUERYNEWPALETTE:
	case WM_PALETTECHANGED:
		GetWebControl()->DoSystemPaletteUse();
		break;
		
	default:
		return FALSE;
	}
	return TRUE;
}
