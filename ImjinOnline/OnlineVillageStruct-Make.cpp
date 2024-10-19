/*********************************************************************************************************
	파일명 : OnlineVillageStruct-Make.cpp
	작성자 : 정진욱
	작성일 : 2001.11.21
	수정일 : 2001.12.27
	내  용 : 생산 시설 클릭시 리스트 출력
*********************************************************************************************************/

#include <GSL.h>
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineVillageStruct-Make.h"
#include "OnlineProductStructure.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineMyData.h"
#include "OnlineVillageManager.h"
#include "OnlineVillage.h"
#include "OnlineResource.h"
#include "OnlineMsgBox.h"


extern _InputDevice				IpD;

OnlineVillageStructMake::OnlineVillageStructMake()
{	
	m_siStatus			=	0;
	m_siScroll			=	0;
	m_siSelectList		=	-1;
	m_siSelect			=	-1;
	m_pMyOnlineWorld	=	NULL;
	m_bActive			=	FALSE;
	m_bMaster			=	FALSE;
	ZeroMemory( m_MakeList, sizeof( MakeList ) * ON_MAKE_MAXLIST );
}

OnlineVillageStructMake::~OnlineVillageStructMake()
{
	Free();
}
///////////////////////////////////////////////////////
// 날림
void	OnlineVillageStructMake::Free()
{
	clGrp.FreeXspr( m_ButtonSpr );	
	clGrp.FreeXspr( m_IsBuySpr );
	clGrp.FreeXspr( m_IsBuyLineSpr);
	clGrp.FreeXspr( m_ListBorderSpr );
	clGrp.FreeXspr(m_ListBorderLineSpr);
}

///////////////////////////////////////////////////////
// 초기화
void	OnlineVillageStructMake::Init(cltOnlineWorld *pOnlineWorld)
{
	m_pMyOnlineWorld	=	pOnlineWorld;		
	m_pScrollBar		=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_NOTICE_SCROLL_BAR );

	clGrp.LoadXspr( "Online\\GameSpr\\inventory\\Sub_Button.Spr", m_ButtonSpr );	
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\SMALL_WINDOW.Spr", m_IsBuySpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\SMALL_WINDOW_LINE.Spr",m_IsBuyLineSpr);


	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_PRODUCTION_LIST.Spr", m_ListBorderSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_PRODUCTION_LIST_LINE.Spr",m_ListBorderLineSpr);


	m_siX = 252;
	m_siY = 45;

	SetRect( &m_rtList, m_siX, m_siY, m_siX + m_ListBorderSpr.Header.Xsize, m_siY + m_ListBorderSpr.Header.Ysize );

	m_BBuy.Create( m_siX + 23, m_siY + 412, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_BUY ), BUTTON_PUT_LEFT, TRUE );
	m_BEnter.Create( m_siX + 156, m_siY + 412, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_ENTER ), BUTTON_PUT_LEFT, TRUE );		
	m_BExit.Create( m_siX + 217, m_siY + 412, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

	SI16	x, y;

	x = ( ON_SCREEN_XSIZE - m_IsBuySpr.Header.Xsize ) / 2;
	y = ( ON_SCREEN_YSIZE - m_IsBuySpr.Header.Ysize ) / 2;

	m_BIsBuy.Create( x + 50, y + 148, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_OK ), BUTTON_PUT_LEFT, FALSE );
	m_BBack.Create( x + 170, y + 148, m_ButtonSpr.Header.Xsize, m_ButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BACK ), BUTTON_PUT_LEFT, FALSE );
}

///////////////////////////////////////////////////////
// 메인 폴
VOID	OnlineVillageStructMake::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( m_bActive == FALSE )		return;



	HDC				hDC;
	SI16			i, ix;
	SI16			siTemp;
	CHAR			szTemp[100], szTemp1[100];
	stPlants		*pPlants;
	
	switch(m_siStatus)
	{
	case ON_MAKE_INIT:
		break;
	case ON_MAKE_POLL:
		{
			if( m_BBuy.Process( m_bMouseDown ) == TRUE )
			{
				if( m_siSelect != -1 )
				{
					// 신용도를 알아 본다.
					stPlants	*pPlants;
					
					pPlants	=	m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );
					
					if( pPlants )
					{
						m_siStatus = ON_MAKE_BUY;
						m_BBack.SetAction( TRUE );
						
						// Skill(건물구입가능)
						if( m_pMyOnlineWorld->pMyData->GetTradeGrade() >= (UI32)pPlants->siReqCredit )
						{
							m_BIsBuy.SetAction( TRUE );
						}
						else
						{
							m_BIsBuy.SetAction(FALSE);
						}
					}
				}
			}
			
			else if( m_BEnter.Process( m_bMouseDown ) == TRUE )
			{		
				if( m_siSelect != -1 )
				{
					m_bActive	=	FALSE;
					m_pMyOnlineWorld->pOnlineClient->SendRequestSlotInfo( GetPlantsKind(), m_siSelect );
					
					if( stricmp( m_MakeList[ m_siSelect ].m_szID, m_pMyOnlineWorld->pMyData->GetMyCharName() ) == 0 )	m_bMaster	=	TRUE;
					else																								m_bMaster	=	FALSE;
					
					m_uiSlotNum		=	m_MakeList[ m_siSelect ].m_uiSlotNum;
					
					m_siSelectList	=	m_siSelect;
					m_siStatus		=	ON_MAKE_POLL;
					m_siSelect		=	-1;
					m_siScroll		=	0;
					m_bSuccess		=	TRUE;			
					return;
				}
			}
			
			// 각 시설에서 나간다.
			else if( m_BExit.Process( m_bMouseDown ) == TRUE )	
			{
				m_bActive	=	FALSE;
				m_siStatus	=	ON_MAKE_POLL;
				m_siSelect	=	-1;
				m_siScroll	=	0;
				
				return;
			}
			
			// 시설물을 더블클릭해도 들어갈수있음 들어간다
			else if (m_siSelect != -1 && m_siSelect >= m_siScroll && m_siSelect < m_siScroll + ON_MAKE_SHOW_LIST) {
				
				if  (IpD.IsDoubleClick() && m_BEnter.IsAction() && PtInRect(&m_rtButton[m_siSelect - m_siScroll],m_pt) )
				{
					if( m_siSelect != -1 )
					{
						m_bActive	=	FALSE;
						m_pMyOnlineWorld->pOnlineClient->SendRequestSlotInfo( GetPlantsKind(), m_siSelect );
						
						if( stricmp( m_MakeList[ m_siSelect ].m_szID, m_pMyOnlineWorld->pMyData->GetMyCharName() ) == 0 )	m_bMaster	=	TRUE;
						else																								m_bMaster	=	FALSE;
						
						m_uiSlotNum		=	m_MakeList[ m_siSelect ].m_uiSlotNum;
						
						m_siSelectList	=	m_siSelect;
						m_siStatus		=	ON_MAKE_POLL;
						m_siSelect		=	-1;
						m_siScroll		=	0;
						m_bSuccess		=	TRUE;			
						return;
					}
					
				}
			}
			
			for( ix=0 ; ix<ON_MAKE_MAXLIST ; ix++ )
				m_MakeList[ix].m_bOver = FALSE;
						
			for( i=0 ; i<ON_MAKE_SHOW_LIST ; i++ )
			{
				if( PtInRect( &m_rtButton[i], m_pt ) == TRUE && m_bMouseDown == FALSE )
					m_MakeList[i + m_siScroll].m_bOver	=	TRUE;
				
				if( PtInRect( &m_rtButton[i], m_pt ) == TRUE && m_bMouseDown == TRUE )
				{
					for( ix=0 ; ix<ON_MAKE_MAXLIST ; ix++ )
						m_MakeList[ix].m_bSelect = FALSE;
					
					m_MakeList[i + m_siScroll].m_bSelect =	TRUE;		
					m_siSelect		  					 =	i + m_siScroll;
					break;
				}
			}
		}
		break;
	case ON_MAKE_BUY:
		{
			if( m_BIsBuy.Process( m_bMouseDown ) == TRUE )
			{
				pPlants			=	m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );

				if( m_pMyOnlineWorld->pMyData->GetMoney() >= pPlants->uiCost )
				{
					m_pMyOnlineWorld->pOnlineClient->SendRequestMakeBuy( GetPlantsKind(), m_siSelect );
					m_siStatus = ON_MAKE_BUY_WAIT;
				}
				else
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "",	m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_MONEYLEFTNO) );
					m_siSelect	=	-1;
					m_siStatus	=	ON_MAKE_POLL;
				}	
				
				m_BBack.SetAction( FALSE );
				m_BIsBuy.SetAction( FALSE );
			}
			else if( m_BBack.Process( m_bMouseDown ) == TRUE )
			{
				m_siStatus	=	ON_MAKE_POLL;
				m_BBack.SetAction( FALSE );
				m_BIsBuy.SetAction( FALSE );
			}
		}
		break;
	case ON_MAKE_BUY_WAIT:
		break;
	}

	if( m_siSelect != -1 )
	{

		if( m_MakeList[m_siSelect].m_szID[0] == 0 )
		{
//			m_BIsBuy.SetAction( TRUE );
			m_BBuy.SetAction( TRUE );
			m_BEnter.SetAction( FALSE );
		}
		else
		{
			m_BBuy.SetAction( FALSE );
			m_BEnter.SetAction( TRUE );
		}

	}
	else
		m_BEnter.SetAction( FALSE );
	
	// 리스트 내용을 출력을 해준다.
	if( m_siStatus != ON_MAKE_INIT )
	{
		//m_pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_ListBorderLineSpr.Header.Xsize,m_ListBorderLineSpr.Header.Ysize,m_ListBorderLineSpr.Image);
			clGrp.PutSpriteJin( m_siX, m_siY, m_ListBorderSpr.Header.Xsize, m_ListBorderSpr.Header.Ysize, m_ListBorderSpr.Image );
			
			m_BBuy.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BEnter.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BExit.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			
			clGrp.UnlockSurface( pSurface );
		}
		
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			::SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
			::SetBkMode( hDC, TRANSPARENT );
			
			CHAR szTemp[256];
			stPlants	*pPlants;
			pPlants = m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );
			
			ZeroMemory( szTemp, sizeof( szTemp ) );
			
			// 농장/무기장등의 리스트 명칭
			if(m_uiKind == ON_PLANT_FARM)
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 4, m_siY + 2,286,14, m_pMyOnlineWorld->pOnlineText->Get(8000557), RGB(255, 255, 255));
			else if(m_uiKind == ON_PLANT_WEAPON)
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 4, m_siY + 2,286,14, m_pMyOnlineWorld->pOnlineText->Get(8000524), RGB(255, 255, 255));
			else if(m_uiKind == ON_PLANT_ARMOR)
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 4, m_siY + 2,286,14, m_pMyOnlineWorld->pOnlineText->Get(8000558), RGB(255, 255, 255));
			else if(m_uiKind == ON_PLANT_RANCH)
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 4, m_siY + 2,286,14, m_pMyOnlineWorld->pOnlineText->Get(8000559), RGB(255, 255, 255));
			else if(m_uiKind == ON_PLANT_FACTORY)
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 4, m_siY + 2,286,14, m_pMyOnlineWorld->pOnlineText->Get(8000560), RGB(255, 255, 255));


						
//			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 84, m_siY + 23,84,13, szTemp, RGB(255, 255, 255));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 84, m_siY + 23,84,13, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_MASTER) , RGB(255, 255, 255));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 173, m_siY + 23,53,13, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_REMAINING_WORK), RGB(255, 255, 255) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 231, m_siY + 23,50,13, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAX_PAY), RGB(255, 255, 255) );
						
			for( i=0 ; i<ON_MAKE_SHOW_LIST ; i++ )
			{
				siTemp	=	i + m_siScroll;
				sprintf( szTemp, "%d", m_MakeList[siTemp].m_siWork );
				sprintf( szTemp1, "%d", m_MakeList[siTemp].m_moMaxPay );
				
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 10, m_siY + 45 + ( i * 18 ), m_MakeList[siTemp].m_szFarmName, RGB(210, 210, 210) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 87, m_siY + 45 + ( i * 18 ) , m_MakeList[siTemp].m_szID, RGB(210, 210, 210) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 226, m_siY + 45 + ( i * 18 ) , szTemp, RGB(210, 210, 210) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 281, m_siY + 45 + ( i * 18 ) , szTemp1, RGB(210, 210, 210) );
				
				if( m_MakeList[siTemp].m_bSelect )
				{
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 10, m_siY + 45 + ( i * 18 ) , m_MakeList[siTemp].m_szFarmName, RGB(0, 255, 0) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 87, m_siY + 45 + ( i * 18 ) , m_MakeList[siTemp].m_szID, RGB(0, 255, 0) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 226, m_siY + 45 + ( i * 18 ) , szTemp, RGB(0, 255, 0) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 281, m_siY + 45 + ( i * 18 ) , szTemp1, RGB(0, 255, 0) );
				}
//				else if( m_MakeList[siTemp].m_bOver )
//				{
//					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 75, m_siY + 44 + ( i * 18 ), m_MakeList[siTemp].m_szFarmName, RGB(155, 155, 155) );
//					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 84, m_siY + 44 + ( i * 18 ), m_MakeList[siTemp].m_szID, RGB(100, 100, 100) );
///					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 226, m_siY + 44 + ( i * 18 ), szTemp, RGB(100, 100, 100) );
//					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 281, m_siY + 44 + ( i * 18 ), szTemp1, RGB(100, 100, 100) );
//				}
//				else
///				{
//					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 75, m_siY + 44 + ( i * 18 ), m_MakeList[siTemp].m_szFarmName, RGB(10, 10, 10) );
//					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 84, m_siY + 44 + ( i * 18 ), m_MakeList[siTemp].m_szID, RGB(10, 10, 10) );
//					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 226, m_siY + 44 + ( i * 18 ), szTemp, RGB(10, 10, 10) );
//					m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 281, m_siY + 44 + ( i * 18 ), szTemp1, RGB(10, 10, 10) );
//				}
			}
			
			m_BBuy.Put( hDC , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BEnter.Put( hDC , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			//m_BRename.Put( hDC );
			m_BExit.Put( hDC , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			
			pSurface->ReleaseDC( hDC );
		}
	}

	if( m_siStatus == ON_MAKE_BUY )
	{
		SI16 x, y;

		x = ( ON_SCREEN_XSIZE - m_IsBuySpr.Header.Xsize ) / 2;
		y = ( ON_SCREEN_YSIZE - m_IsBuySpr.Header.Ysize ) / 2;


		if( clGrp.LockSurface( pSurface ) == TRUE )
		{			
			clGrp.PutSpriteT( x, y, m_ListBorderLineSpr.Header.Xsize,m_ListBorderLineSpr.Header.Ysize,m_ListBorderLineSpr.Image);
			clGrp.PutSpriteJin( x, y, m_IsBuySpr.Header.Xsize, m_IsBuySpr.Header.Ysize, m_IsBuySpr.Image );
			m_BBack.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BIsBuy.Put( &m_ButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

			clGrp.UnlockSurface( pSurface );
		}

		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			::SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
			::SetBkMode( hDC, TRANSPARENT );		
			
			pPlants			=	m_pMyOnlineWorld->pOnlineBuilding->GetPlants( m_szKind );

			wsprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_PRICE), pPlants->uiCost );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, x + 85,y + 70, szTemp, RGB( 255, 255, 255 ) );

			wsprintf( szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NEEDCREDIT), pPlants->siReqCredit );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, x + 85,y + 90, szTemp, RGB( 255, 255, 255 ) );

			m_BBack.Put( hDC , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BIsBuy.Put( hDC , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

			pSurface->ReleaseDC( hDC );
		}
	}

	m_pt.x = (SI16)IpD.Mouse_X;
	m_pt.y = (SI16)IpD.Mouse_Y;

	if( LOBYTE(m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )		m_bActive	=	FALSE;

	if( IpD.LeftPressSwitch )
	{		
		if(!(PtInRect( &m_rtList, m_pt ) == FALSE && m_siStatus == ON_MAKE_POLL))
			m_bMouseDown = TRUE;
	}
	else
		m_bMouseDown = FALSE;
}


///////////////////////////////////////////////////////////
// 리스트를 설정한다.
VOID	OnlineVillageStructMake::SetList( SI32 siKind, SI16 x, SI16 y )
{
	m_siX		=	252;
	m_siY		=	45;
	m_bActive	=	TRUE;
	m_bSuccess	=	FALSE;
	m_siStatus	=	ON_MAKE_INIT;
	m_siScroll	=	0;
	m_siSelect	=	-1;	


	SetRect( &m_rtList, m_siX, m_siY, m_siX + m_ListBorderSpr.Header.Xsize, m_siY + m_ListBorderSpr.Header.Ysize );	
	ZeroMemory( m_MakeList, sizeof( MakeList ) * ON_MAKE_MAXLIST );

	SI16		i;


	for( i=0 ; i<ON_MAKE_SHOW_LIST; i++ )
		SetRect( &m_rtButton[i], m_siX + 10, m_siY + 45 + ( i * 18 ), m_siX + 10 + 216, m_siY + 45 + ( i * 18 ) + 18 );

	SetRect( &m_rtScroll, m_siX + 285, m_siY + 5, m_siX + 300 + 
		m_pScrollBar->Header.Xsize, m_siY + 456 - m_pScrollBar->Header.Ysize);
	m_siScrollPos	=	m_siY + 5;

	
	// 무슨 건물인가 ???	
	switch( siKind )
	{
	case ON_VIL_ST_KIND_FARM:										// 농장
		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_FARM );
		m_uiKind	=	ON_PLANT_FARM;
		break;

	case ON_VIL_ST_KIND_WEAPON:
		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_WEAPON );
		m_uiKind	=	ON_PLANT_WEAPON;
		break;

	case ON_VIL_ST_KIND_ARMOR:
		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_ARMOR );
		m_uiKind	=	ON_PLANT_ARMOR;
		break;

	case ON_VIL_ST_KIND_PASTURE:
		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_PASTURE );
		m_uiKind	=	ON_PLANT_RANCH;
		break;

	case ON_VIL_ST_KIND_FACTORY:
		strcpy( m_szKind, ON_VIL_ST_KIND_CODE_FACTORY );
		m_uiKind	=	ON_PLANT_FACTORY;
		break;
	}

	// 서버로 리스트를 요청함
	m_pMyOnlineWorld->pOnlineClient->SendRequestFramInfo(GetPlantsKind());
}

BOOL	OnlineVillageStructMake::IsActive()
{
	return m_bActive;
}

SI16	OnlineVillageStructMake::GetListNum()
{
	return m_siSelect;
}

BOOL	OnlineVillageStructMake::IsEnter()
{
	return m_bSuccess;
}

UI08	OnlineVillageStructMake::GetPlantsKind()
{
	return m_uiKind;
}

MakeList	*OnlineVillageStructMake::GetMakeList()
{
	return &m_MakeList[ m_siSelectList ];
}

// robypark 2004/8/25 18:4
// 열려진 생산 시설 목록 리스트 상자를 닫는다.
VOID OnlineVillageStructMake::CloseList_ProductionEstablishments(VOID)
{
	if (m_bActive)
	{
		m_bActive	=	FALSE;
		m_siStatus	=	ON_MAKE_POLL;
		m_siSelect	=	-1;
		m_siScroll	=	0;
	}
}
