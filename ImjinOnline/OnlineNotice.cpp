/************************************************************************************************
	파일명 : OnlineNotice.cpp
	담당자 : 이상원

	내  용 : 공지사항
************************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineNotice.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineText.h"
#include "OnlineChatList.h"
#include "OnlineKeyboard.h"
#include <string>

using namespace std;


extern _InputDevice			IpD;

VOID OnlineNotice::Init( cltOnlineWorld *pOnlineWorld )
{
	m_pMyOnlineWorld = pOnlineWorld;
	m_bAction = FALSE;

	// 이미지를 로드 한다.
	clGrp.LoadXspr( "Online\\GameSpr\\Login\\Notice.Spr", m_BackImageSpr );			//	뒷 배경
	clGrp.LoadXspr( "Online\\GameSpr\\Login\\Close.Spr", m_Button2 );				//	닫기 이미지	


	// 기준 좌표를 구한다.
	m_siX = ( ON_SCREEN_XSIZE - m_BackImageSpr.Header.Xsize ) / 2;
	m_siY = ( ON_SCREEN_YSIZE - m_BackImageSpr.Header.Ysize ) / 2;

	// 버튼을 만들어 준다.
	m_BClose.Create( m_siX + 94, m_siY + 285, m_Button2.Header.Xsize, m_Button2.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OK1), BUTTON_PUT_LEFT, TRUE );	

	INT	siScrollBarX = m_siX + 262;
	INT	siScrollBarY = m_siY + 68;

	pNoticeList	= new OnlineChatList;
	pNoticeList->Init(pOnlineWorld,  ON_INN_CONTENT_ONLY, -1, ON_GAMESPR_NOTICE_SCROLL_BAR,
					 12, 41, m_siX, m_siY, siScrollBarX, siScrollBarY,
					 siScrollBarX-10, siScrollBarY, 30, 190, m_siX + 18, m_siY + 72);			
	
	m_bMouseDown = FALSE;
}


VOID OnlineNotice::Free()
{
	clGrp.FreeXspr( m_BackImageSpr );			//	뒷 배경
	clGrp.FreeXspr( m_Button2 );				//	닫기 이미지	

	if( pNoticeList )
	{
		delete pNoticeList;
		pNoticeList	=	NULL;
	}
}

VOID OnlineNotice::Action()
{
	if( !m_bAction )	return;

	if( m_BClose.Process( m_bMouseDown ) || LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
	{
		m_bAction = FALSE;
		m_bClose  = TRUE;
	}

	if( IpD.LeftPressSwitch )		m_bMouseDown = TRUE;
	else							m_bMouseDown = FALSE;
}

VOID OnlineNotice::SetAction(const char* pNotice, BOOL bFlag)
{
	pNoticeList->Initialize();

	SI16	i=	0;
	SI16	siCnt =	0;
	CHAR	szTempBuffer[ON_INN_MAX_USER_CHAT_STRING];
	ZeroMemory( szTempBuffer, sizeof( szTempBuffer ) );
	

	while(pNotice[i])
	{		
		if( pNotice[i] == '\n' || pNotice[i] == '\r' )
		{			
			pNoticeList->Add( szTempBuffer, ON_INN_COLOR_GRAY );
			ZeroMemory( szTempBuffer, sizeof( szTempBuffer ) );
			siCnt = 0;
		}
		else
		{
			szTempBuffer[ siCnt ] = pNotice[i];
			siCnt++;
		}
		
		i++;		
	}	


	pNoticeList->Add( szTempBuffer, ON_INN_COLOR_GRAY );
	

	m_bAction = bFlag;
	m_bClose  = FALSE;
};

VOID OnlineNotice::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( !m_bAction )	return;

	HDC hDC;

	if( clGrp.LockSurface( pSurface ) )
	{
		clGrp.PutSpriteT( m_siX, m_siY, m_BackImageSpr.Header.Xsize, m_BackImageSpr.Header.Ysize, m_BackImageSpr.Image );

		m_BClose.Put( &m_Button2, 0, 0, 0, BUTTON_PUT_NOMOVE );		

		clGrp.UnlockSurface(pSurface);
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		m_BClose.Put(hDC);		
		pSurface->ReleaseDC( hDC );
	}

	pNoticeList->Process(pSurface,ON_FONT_BUTTON);
}