//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 용병 출력 클래스
//	File Name		: OnlineMercenary.cpp
//	Birth Date		: 2003.
//	Creator			: 이 준 석
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 필드에서 ESC버튼을 누를 경우 뜨는 옵션창입니다.
//
//===================================================================================================

#include <GSL.h>

#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineSetting.h"
#include "OnlineFont.h"
#include "OnlineChatMacro.h"
#include "OnlineMegaText.h"
#include "OnlineText.h"
#include "OnlineFieldArea.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineMinimap.h"
#include "OnlineTradeBook.h"
#include "OnlineVillageSave.h"
#include "OnlineTrade.h"
#include "OnlineMydata.h"
#include "OnlineMessenger.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "OnlineFieldChat.h"
#include "OnlineVillageDescriptList.h"
#include "OnlineNoticeList.h"
#include "yOptionMenu.h"
#include "OnlineQuestPlay.h"
#include "OnlineSmith.h"
#include "OnlineTip.h"
#include "Onlinemfgs.h"
#include "OnlineParty.h"
#include "OnlineMercenary.h"		// actdoll (2004/01/12 12:04) : 용병창 초기화 추가



extern _InputDevice			IpD;  


void OnlineSetting::LoadImage()
{
	// 이미지를 로드 한다.
	clGrp.LoadXspr( "Online\\GameSpr\\Option\\SYSTEM_MENU.Spr", m_BackImageSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Option\\small_window_medium_button.Spr", m_ButtonSpr);

}

void OnlineSetting::FreeImage()
{
	clGrp.FreeXspr(m_BackImageSpr);
	clGrp.FreeXspr(m_ButtonSpr);
}


VOID OnlineSetting::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld = pOnlineWorld;
	m_bMouseDown = FALSE;

	LoadImage();

	// 기준 좌표를 구한다. 
	m_siX = 336;
	m_siY = 154;

	// 쇼핑몰이 있으면 쇼핑몰 세팅을 해준다.
	if( g_stConnectServerInfo.m_ServiceShopName[0] == '0' )	m_bExistShop	= false;
	else													m_bExistShop	= true;

	// 공통 버튼을 만들어 준다.
	int		iAddPoint	= 25;
	m_BOption.Create	( m_siX + 23, m_siY + iAddPoint,										m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SETTING_OPTION),		BUTTON_PUT_LEFT, TRUE );	
	m_BLogIn.Create		( m_siX + 23, m_siY + (iAddPoint+=6) + m_ButtonSpr.Header.Ysize,		m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SETTING_LOGOUT),		BUTTON_PUT_LEFT, TRUE );	
	m_BExit.Create		( m_siX + 23, m_siY + (iAddPoint+=6) + m_ButtonSpr.Header.Ysize * 2,	m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXITTEXT),			BUTTON_PUT_LEFT, TRUE );	
	m_BCompany.Create	( m_siX + 23, m_siY + (iAddPoint+=6) + m_ButtonSpr.Header.Ysize * 3,	m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, g_stConnectServerInfo.m_ServiceName,							BUTTON_PUT_LEFT, TRUE );
	// 별도의 버튼을 만들어준다.
	if( m_bExistShop )	m_BShopping.Create	( m_siX + 23, m_siY + (iAddPoint+=6) + m_ButtonSpr.Header.Ysize * 4,	m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, g_stConnectServerInfo.m_ServiceShopName,	BUTTON_PUT_LEFT, TRUE );	
	else				iAddPoint+=6;
	m_BBack.Create		( m_siX + 23, m_siY + (iAddPoint+=6) + m_ButtonSpr.Header.Ysize * 5,	m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SETTING_CONTINUE),	BUTTON_PUT_LEFT, TRUE );	

	FreeImage();

	m_OptionMode    = FALSE;
	m_OptionNoField = -1;
}


VOID OnlineSetting::Free()
{
	if(m_OptionMode)
	{
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_OptionNoField);
		FreeOptionMenu();

		m_OptionNoField = -1;
		m_OptionMode    = FALSE;
	}
	
	FreeImage();
}

VOID OnlineSetting::Action()
{
	if( !m_bAction )	return;

	if(m_OptionMode)
	{
		if(OptionActionMenuFromOnline() == 0)
		{
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_OptionNoField);

			FreeOptionMenu();			

			m_OptionNoField = -1;
			m_OptionMode    = FALSE;
		}
	}
	else
	{
		if( m_BOption.Process( m_bMouseDown ) )
		{
			m_OptionNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(0, 0, 799, 599);
			InitOptionMenu();

			m_OptionMode = TRUE;
		}

		if( m_BLogIn.Process( m_bMouseDown ) )
		{
			if(g_AutoServerCode != -1)
			{
				m_pMyOnlineWorld->SetMainPollStatus(ON_MAINPOLL_FORCED_LOGOUT);
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
		}

		if( m_BCompany.Process( m_bMouseDown ) )
		{
			SetAction(false);
			ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceURL, NULL, NULL, SW_SHOWNORMAL );
		}

		// actdoll (2004/05/26 18:01) : 쇼핑몰 버튼 추가
		if( m_BShopping.Process( m_bMouseDown ) && m_bExistShop )
		{
			SetAction(false);
			ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceShopURL, NULL, NULL, SW_SHOWNORMAL );
		}

		if( m_BBack.Process( m_bMouseDown ) )
		{
			SetAction(false);
		}

		if( IpD.LeftPressSwitch )		m_bMouseDown = TRUE;
		else							m_bMouseDown = FALSE;
	}
}

VOID OnlineSetting::SetAction(BOOL bFlag)
{
	m_bAction	= bFlag;

	if(m_bAction)
	{
		m_siNoField	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siX, m_siY, m_siX + m_BackImageSpr.Header.Xsize, m_siY + m_BackImageSpr.Header.Ysize );
		LoadImage();
	}
	else
	{
		if( m_siNoField != -1 )
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
		m_siNoField	=	-1;
		FreeImage();
	}

	if(m_OptionMode)
	{
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_OptionNoField);
		FreeOptionMenu();

		m_OptionNoField = -1;
		m_OptionMode    = FALSE;
	}
}

VOID OnlineSetting::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( !m_bAction )	return;

	if(m_OptionMode)
	{
		OptionDrawMenuFromOnline(pSurface);
	}
	else
	{
		HDC hDC;

		if( clGrp.LockSurface( pSurface ) )
		{
			clGrp.PutSpriteJin(m_siX, m_siY, m_BackImageSpr.Header.Xsize, m_BackImageSpr.Header.Ysize, &m_BackImageSpr.Image[m_BackImageSpr.Header.Start[0]]);
			clGrp.PutSpriteT(m_siX, m_siY, m_BackImageSpr.Header.Xsize, m_BackImageSpr.Header.Ysize, &m_BackImageSpr.Image[m_BackImageSpr.Header.Start[1]]);

			m_BOption.Put( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );		
			m_BLogIn.Put( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );		
			m_BExit.Put ( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );
			m_BCompany.Put ( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );
		
			// actdoll (2004/05/26 18:02) : 쇼핑몰 버튼 추가
			if( m_bExistShop )	m_BShopping.Put ( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );

			m_BBack.Put ( &m_ButtonSpr, 0, 0, 1, BUTTON_PUT_NOMOVE );		

			clGrp.UnlockSurface(pSurface);
		}

		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode( hDC, TRANSPARENT );
			SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ));
		
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX+3, m_siY+4, 118, 13, m_pMyOnlineWorld->pOnlineText->Get(8000517), RGB(245, 245, 245));
			

			m_BOption.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));	
			m_BLogIn.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));
			m_BExit.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));
			m_BCompany.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));

			// actdoll (2004/05/26 18:02) : 쇼핑몰 버튼 추가
			if( m_bExistShop )	m_BShopping.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));

			m_BBack.Put(hDC, BUTTON_PUT_DEFAULT, RGB(245, 245, 245), RGB(245, 245, 245));
			pSurface->ReleaseDC( hDC );
		}
	}
}

VOID OnlineSetting::GameInit()
{
	if(m_OptionMode)
	{
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_OptionNoField);
		FreeOptionMenu();

		m_OptionNoField = -1;
		m_OptionMode    = FALSE;
	}

	if(m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->IsActive())
		m_pMyOnlineWorld->pOnlineMsgBoxUsePortal->SetAction(FALSE);

	if(m_pMyOnlineWorld->pOnlineMsgBox->IsActive())
		m_pMyOnlineWorld->pOnlineMsgBox->SetAction(FALSE);

	m_pMyOnlineWorld->pOnlineBooth->Initialize();
	m_pMyOnlineWorld->pOnlineInventory->Initialize();
	m_pMyOnlineWorld->pOnlineMiniMap->Initialize();
	m_pMyOnlineWorld->pOnlineTradeBook->Initialize();
	m_pMyOnlineWorld->pOnlineParty->Initialize();
	m_pMyOnlineWorld->pOnlineVillageSave->Initialize();
	m_pMyOnlineWorld->pOnlineTrade->Initialize();
	m_pMyOnlineWorld->pMyData->Initialize();
	m_pMyOnlineWorld->pOnlineMessenger->SetAction(FALSE);
	m_pMyOnlineWorld->pOnlinePannel->Initialize();
	m_pMyOnlineWorld->pOnlineNoticeList->Initialize();
	m_pMyOnlineWorld->pOnlineVillageDescriptList->Initialize();
	m_pMyOnlineWorld->pOnlineQuestPlay->SetAction(FALSE); 
	m_pMyOnlineWorld->pOnlineSmith->Initialize();
	m_pMyOnlineWorld->pOnlineClient->InitZero();
	m_pMyOnlineWorld->pIOnlineChar->AllFree();
	m_pMyOnlineWorld->pOnlineTip->SetAction(FALSE);
	m_pMyOnlineWorld->m_pMercenary->Initialize();	// actdoll (2004/01/12 12:02) : 용병창 재초기화
	m_pMyOnlineWorld->m_pChatMacro->Initialize();	// actdoll (2004/01/12 12:02) : 채팅 매크로 재초기화
	m_pMyOnlineWorld->Initialize();

	if(clGrp.lpDDSPrimary->IsLost() != DD_OK)
	{
		clGrp.lpDDSPrimary->Restore();
	}
	clGrp.ClearSurface(clGrp.lpDDSPrimary);
}
