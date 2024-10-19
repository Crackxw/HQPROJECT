/************************************************************************************************
	파일명 : OnlineSetting.cpp
	담당자 : 이 준 석
************************************************************************************************/
/*
#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineSetting.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineText.h"
#include "OnlineFieldArea.h"
#include "OnlineBooth.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineFollower.h"
#include "OnlineInventory.h"
#include "OnlineMinimap.h"
#include "OnlineTradeBook.h"
#include "OnlineVillageSave.h"
#include "OnlineTrade.h"
#include "OnlineMydata.h"
#include "OnlineMessenger.h"
#include "OnlinePannel.h"
#include "OnlineFieldChat.h"
#include "OnlineVillageDescriptList.h"
#include "OnlineNoticeList.h"


extern _InputDevice			IpD;

VOID OnlineSetting::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld = pOnlineWorld;
	m_bMouseDown = FALSE;

	// 이미지를 로드 한다.
	clGrp.LoadXspr( "Online\\GameSpr\\Setting.Spr", m_BackImageSpr );		
	clGrp.LoadXspr( "Online\\GameSpr\\SERVER_BUTTON.Spr", m_ButtonSpr );	

	// 기준 좌표를 구한다.
	m_siX = ( ON_SCREEN_XSIZE - m_BackImageSpr.Header.Xsize ) / 2;
	m_siY = ( ON_SCREEN_YSIZE - m_BackImageSpr.Header.Ysize ) / 2;

	// 버튼을 만들어 준다.
	m_BLogIn.Create	( m_siX + 38, m_siY + 50,  m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SETTING_LOGOUT), BUTTON_PUT_LEFT, TRUE );	
	m_BExit.Create	( m_siX + 38, m_siY + 85,  m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXITTEXT), BUTTON_PUT_LEFT, TRUE );	
	m_BJoyon.Create	( m_siX + 38, m_siY + 120, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->m_LogInInfo_Data->m_ServiceName, BUTTON_PUT_LEFT, TRUE );	
	m_BBack.Create	( m_siX + 38, m_siY + 155, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SETTING_CONTINUE), BUTTON_PUT_LEFT, TRUE );	
}


VOID OnlineSetting::Free()
{
	clGrp.FreeXspr( m_BackImageSpr );		
	clGrp.FreeXspr( m_ButtonSpr );

	//m_pMyOnlineWorld->pIOnlineChar->AllFree();
}

VOID OnlineSetting::Action()
{
	if( !m_bAction )	return;

	if( m_BLogIn.Process( m_bMouseDown ) )
	{
		if(g_AutoServerCode != -1)
		{
			m_pMyOnlineWorld->SetMainPollStatus(ON_MAINPOLL_FORCED_LOGOUT);
//			m_pMyOnlineWorld->SetLogOut(GS_INITINITMENU, 0);
		}
		else
		{
			SetAction(false);
			m_pMyOnlineWorld->pOnlineClient->Close();
			m_pMyOnlineWorld->pOnlineClientChatting->Close();

			m_pMyOnlineWorld->m_WaitTickCount = max(GetTickCount(), 1);
			m_pMyOnlineWorld->bCharMove = FALSE;
		}
	}

	if( m_BExit.Process( m_bMouseDown ) )
	{
		m_pMyOnlineWorld->SetMainPollStatus(ON_MAINPOLL_FORCED_LOGOUT);
//		m_pMyOnlineWorld->SetLogOut(GS_INITINITMENU, 0);
	}

	if( m_BJoyon.Process( m_bMouseDown ) )
	{
		SetAction(false);
		ShellExecute (  NULL, "open", m_pMyOnlineWorld->m_LogInInfo_Data->m_ServiceURL, NULL, NULL, SW_SHOWNORMAL );
	}

	if( m_BBack.Process( m_bMouseDown ) )
	{
		SetAction(false);
	}

	if( IpD.LeftPressSwitch )		m_bMouseDown = TRUE;
	else							m_bMouseDown = FALSE;
}

VOID OnlineSetting::SetAction(BOOL bFlag)
{
	m_bAction	= bFlag;

	if(m_bAction)
	{
		m_siNoField	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siX, m_siY, m_siX + m_BackImageSpr.Header.Xsize, m_siY + m_BackImageSpr.Header.Ysize );
	}
	else
	{
		if( m_siNoField != -1 )
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField	=	-1;
	}
}

VOID OnlineSetting::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( !m_bAction )	return;

	HDC hDC;

	if( clGrp.LockSurface( pSurface ) )
	{
		clGrp.PutSpriteT( m_siX, m_siY, m_BackImageSpr.Header.Xsize, m_BackImageSpr.Header.Ysize, m_BackImageSpr.Image );

		m_BLogIn.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );		
		m_BExit.Put ( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BJoyon.Put ( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
		m_BBack.Put ( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );		

		clGrp.UnlockSurface(pSurface);
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_BLogIn.Put(hDC);	
		m_BExit.Put(hDC);
		m_BJoyon.Put(hDC);
		m_BBack.Put(hDC);		
		pSurface->ReleaseDC( hDC );
	}
}

VOID OnlineSetting::GameInit()
{
	m_pMyOnlineWorld->pOnlineBooth->Initialize();
	m_pMyOnlineWorld->pOnlineFollowerInfo->Initialize();
	m_pMyOnlineWorld->pOnlineInventory->Initialize();
	m_pMyOnlineWorld->pOnlineMiniMap->Initialize();
	m_pMyOnlineWorld->pOnlineTradeBook->Initialize();
	m_pMyOnlineWorld->pOnlineVillageSave->Initialize();
	m_pMyOnlineWorld->pOnlineTrade->Initialize();
	m_pMyOnlineWorld->pMyData->Initialize();
	m_pMyOnlineWorld->pOnlineMessenger->SetAction(FALSE);
	m_pMyOnlineWorld->pOnlinePannel->Initialize();
	m_pMyOnlineWorld->pOnlineFieldChat->Initialize();
	m_pMyOnlineWorld->pOnlineGuildChat->Initialize();
	m_pMyOnlineWorld->pOnlineNoticeList->Initialize();
	m_pMyOnlineWorld->pOnlineVillageDescriptList->Initialize();
	m_pMyOnlineWorld->Initialize();
}
*/