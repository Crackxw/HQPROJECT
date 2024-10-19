/********************************************************************************************
	파일명	:	OnlineFollower.cpp
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2002.03.30
********************************************************************************************/
#include <GSL.h>
#include <clGame.h>
#include <Main.h>
#include <Mouse.h>
#include "charanimation.h"
#include <kindinfo.h>


#include "OnlineWorld.h"
#include "OnlineFollower.h"
#include "OnlineHelp.h"
#include "OnlineTradeBook.h"
#include "..\\Gersang\\Music.h"
#include "OnlineMydata.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineFieldArea.h"
#include "OnlineMercenary-Parser.h"
#include "..\\Gersang\\CharInfo\\CharFileLoadManager.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "OnlineFieldAuction.h"
#include "OnlineTrade.h"


extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 
extern _clGame* pGame;

OnlineFollowerInfo::OnlineFollowerInfo()
{
	m_pMyOnlineWorld	=	NULL;	
	m_bAction			=	FALSE;
	m_bMouseDown		=	FALSE;
	m_bNoAction			=	FALSE;
	m_bNoMove			=	FALSE;
	m_siNoField			=	-1;
	m_siNoFieldAll		=	-1;
	m_dwDelay			=	0;
	m_siFont			=	0;
	m_dwAniFrame		=	0;
	m_uiSelectSlot		=	0;

	for(int i=0; i<ON_MAX_FOLLOWER_NUM; i++)
		m_index[i] = -1;
}

OnlineFollowerInfo::~OnlineFollowerInfo()
{
	Free();
}

// 초기화
VOID	OnlineFollowerInfo::Initialize()
{
	SetAction(FALSE);
	m_bMouseDown		=	FALSE;
	m_bNoAction			=	FALSE;
	m_dwDelay			=	0;
	m_siFont			=	0;
	m_dwAniFrame		=	0;
	m_uiSelectSlot		=	0;
	for(int i=0; i<ON_MAX_FOLLOWER_NUM; i++)
		m_index[i] = -1;
	m_bScrollRight		=	FALSE;
	m_bScrollLeft		=	FALSE;
	m_bOpenNow			=	FALSE;
}

VOID	OnlineFollowerInfo::Init( cltOnlineWorld *pOnlineWorld)
{
	SI16	i;

	m_pMyOnlineWorld	=	pOnlineWorld;
	m_pButtonSpr		=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_CHARACTERSLOT );	
	m_siX				=	14 - m_pButtonSpr->Header.Xsize;
	m_siY				=	0;
	m_siScrollX			=	10;
	m_bScrollRight		=	FALSE;
	m_bScrollLeft		=	FALSE;
	m_bOpenNow			=	FALSE;

	// 캐릭터 버튼
	for( i = 0 ; i < ON_MAX_FOLLOWER_NUM; i++ )
		m_BChar[i].Create(m_siX , m_siY + (i * m_pButtonSpr->Header.Ysize), m_pButtonSpr->Header.Xsize, m_pButtonSpr->Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );

	m_siMaxFollowerNum	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)) + 1;		// 신용,레벨에 따른 최대 보유가능 용병수(자신포함 최대10명)
	m_CurFollowerNum	= m_pMyOnlineWorld->pMyData->GetFollowerNum();
	// 밑에 출력 되는 용병창의 크기
	SetRect( &m_rt, m_siX, m_siY, m_siX + m_pButtonSpr->Header.Xsize, m_siY + m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM);

	// 이지미를 로드 한다.
	clGrp.LoadXspr( "Online\\GameSpr\\LvUp01.Spr", m_LvSpr01 );
	clGrp.LoadXspr( "Online\\GameSpr\\LvUp02.Spr", m_LvSpr02 );
	clGrp.LoadXspr( "Online\\GameSpr\\LifeBar.Spr", m_LifeSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\ExpBar.Spr", m_ExpSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\FollowerInfo.Spr", m_FollowerInfo );
	clGrp.LoadXspr( "Online\\GameSpr\\CharacterSlotSelect.Spr",m_CharacterSlotSelect);
	clGrp.LoadXspr( "Online\\GameSpr\\CharacterSlotX.Spr", m_CharacterSlotX);
	clGrp.LoadXspr( "Online\\GameSpr\\WeightOver.Spr", m_WeightOverSpr);
	
	clGrp.LoadXspr( "Online\\GameSpr\\CharImage.Spr", m_CahrSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Charimage_die.Spr", m_CharDieSpr );
}

// 액숀
VOID	OnlineFollowerInfo::Action()
{
	SI16	i, index = 0;	

	if(!m_bNoMove)
	{
		POINT pt;
		pt.x = IpD.Mouse_X;
		pt.y = IpD.Mouse_Y;

		if(CheckRect(pt) && !m_bOpenNow)		
			SetAction(TRUE, FALSE);
		else if(!CheckRect(pt) && m_bOpenNow) 	
			SetAction(FALSE);
	}

	if( !m_bAction )			return;

	// 용병 인덱스 초기화
	for(i=0; i<ON_MAX_FOLLOWER_NUM; i++)	m_index[i] = -1;

	// 현재 용병에 대해 차례로 index를 붙여준다.
	for( i=0 ; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( m_pMyOnlineWorld->pMyData->IsValidFollower( (UI08)i ) )	
		{
			m_index[index] = i;
			index++;
		}
	}
	
	if( m_siNoFieldAll == -1 )
	{
		for( i=0 ; i<ON_MAX_FOLLOWER_NUM; i++ )
		{
			if( !m_bNoAction )
			{	
				// 오른쪽으로 클릭시 장부
				if( m_BChar[i].Process( m_bMouseRight) )
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)	
						return;

					// 어느 버튼을 눌렸는지 확인을 한다.
					if( m_pMyOnlineWorld->pMyData->IsValidFollower( (UI08)m_index[i] ) )
					{
						if(m_pMyOnlineWorld->pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_STATUS))
						{
							// 만약 지금 선택한 용병이 유효한 캐릭터 이면
							// 효과음을 출력을 해주고
							pGame->m_pEffectDataMgr->PushEffectByKindWhenSelected(m_pMyOnlineWorld->pMyData->GetFollowerParameterKind((UI08)m_index[i]));
							
							if(m_uiSelectSlot == m_index[i])
							{
								m_uiSelectSlot	=	(UI08)m_index[i];
								
								if( m_pMyOnlineWorld->pOnlineTradeBook->IsAction() )
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE, ON_BOOK_STATUS );
								else
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_STATUS );
							}
							else
							{
								m_uiSelectSlot	=	(UI08)m_index[i];
								
								m_pMyOnlineWorld->pOnlineTradeBook->SetSelect( m_uiSelectSlot );
								m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot( m_uiSelectSlot );
								m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
								
								
								if( !m_pMyOnlineWorld->pOnlineTradeBook->IsAction() )
									m_pMyOnlineWorld->pOnlineTradeBook->SetAction(TRUE, ON_BOOK_STATUS );
								else
									m_pMyOnlineWorld->pOnlineTradeBook->SetStatus( ON_BOOK_STATUS );
							}						
						}
					}
					break;
				}

				// 왼쪽으로 클릭시 인벤토리
				else if( m_BChar[i].Process( m_bMouseDown ) )
				{
					// 경매에 참여하면 인벤토리를 열지 못한다!!

					if (m_pMyOnlineWorld->pOnlineFieldAuction->IsEntered())
					{
						;
						
					}
					else {
						m_dwDelay = timeGetTime();
						
						if( m_pMyOnlineWorld->pMyData->IsValidFollower( (UI08)m_index[i] ) )
						{
							pGame->m_pEffectDataMgr->PushEffectByKindWhenSelected(m_pMyOnlineWorld->pMyData->GetFollowerParameterKind((UI08)m_index[i]));
							
							if(m_uiSelectSlot == m_index[i])
							{
								m_uiSelectSlot	=	(UI08)m_index[i];
								
								if( m_pMyOnlineWorld->pOnlineInventory->IsActive() && !m_pMyOnlineWorld->pOnlineTrade->IsActive())
									m_pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
								else
									m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
							}
							else if(!m_pMyOnlineWorld->pOnlineInventory->IsItemMove() && m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() == -1)
							{
								m_uiSelectSlot	=	(UI08)m_index[i];
								
								m_pMyOnlineWorld->pOnlineTradeBook->SetSelect( m_uiSelectSlot );
								m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot( m_uiSelectSlot );
								m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
								
								if( !m_pMyOnlineWorld->pOnlineInventory->IsActive() )
									m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
							}
							
						}
					}
				}
			}
		}
	}

	DWORD	dwTemp;

	dwTemp = timeGetTime();

	if( ( dwTemp - m_dwAniFrame ) >= 400 )
	{
		m_dwAniFrame	=	dwTemp;
		m_siFont++;

		if( m_siFont > 5 )			m_siFont = 0;
	}
	
	if( IpD.LeftPressSwitch )			m_bMouseDown = TRUE;
	else								m_bMouseDown = FALSE;

	if( IpD.RightPressSwitch )
	{
		if( (dwTemp - m_dwDelay) >= 200 )
		{
			m_bMouseRight	=	TRUE;
			m_dwDelay		=	dwTemp;
		}
		else
			m_bMouseRight	=	FALSE;
	}
	else								m_bMouseRight = FALSE;
}


VOID	OnlineFollowerInfo::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_bScrollRight)			{ ScrollDrawRight(pSurface); return; }
	else if(m_bScrollLeft)		{ ScrollDrawLeft(pSurface); return; }

	HDC		hDC;
	SI16	i, j, TempFollowerNum = 0;
	SI32	CharID, siFont;
	SI16	siY, siX;

	m_uiImpend	=	GetLvUpImpend();

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		SI32 CurLife, MaxLife, CurExp, NextExp, PrevExp, LifeBarHeight, ExpBarHeight;
		for( i=0 ; i < ON_MAX_FOLLOWER_NUM; i++ )
		{
			// 용병을 출력 해준다.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );

			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

			if( CharID != 0 )
			{				
				// 용병 테두리 창을 그린다.
				SI16 siIndex;
				siIndex = 0;
				for(j = 0 ; j < max(m_siMaxFollowerNum, m_CurFollowerNum); j++)
				{
					if(m_index[j] == i)
					{
						siIndex = j;
						break;
					}
				}

				if( m_uiSelectSlot == m_index[siIndex] )	
				{
					m_BChar[TempFollowerNum].Put( m_pButtonSpr, 1, 1, 1, BUTTON_PUT_NOMOVE );
//					clGrp.PutSpriteT( m_BChar[siIndex].GetX() , m_BChar[siIndex].GetY() , 
//					m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize , m_CharacterSlotSelect.Image );
				}
				else
				{
					m_BChar[TempFollowerNum].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );
				}
				
				// 종류별로 중심 좌표를 기준으로 캐릭터를 찍어준다. 
				if(m_bAction)
				{
					siFont = TransKindToFont( CharID );
					XSPR *pTempChar = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i) == 0 ? &m_CharDieSpr : &m_CahrSpr;

					clGrp.PutSpriteT( 4, m_siY + 5 + m_pButtonSpr->Header.Ysize * TempFollowerNum, pTempChar->Header.Xsize
						, pTempChar->Header.Ysize, &pTempChar->Image[pTempChar->Header.Start[siFont]] );
				}
				if( m_uiSelectSlot == m_index[siIndex] )	
				{
					clGrp.PutSpriteT( m_BChar[siIndex].GetX() , m_BChar[siIndex].GetY() , 
					m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize , m_CharacterSlotSelect.Image );
				}

				

				// 생명력, 체력을 그려준다.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				for(j = 0; j < LifeBarHeight-2; j++)
				{
					// 생명력
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j, 
					m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image );
				}

				for(j = 0; j < ExpBarHeight-2; j++)
				{
					// 체력
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 57, m_BChar[TempFollowerNum].GetY() + 43 - j, 
					m_ExpSpr.Header.Xsize, m_ExpSpr.Header.Ysize, m_ExpSpr.Image );
				}
			}

			if(i >= m_siMaxFollowerNum)
			{	
				clGrp.PutSpriteT(m_BChar[i].GetX() + 4, m_BChar[i].GetY() + 3, m_CharacterSlotX.Header.Xsize, m_CharacterSlotX.Header.Ysize, m_CharacterSlotX.Image );
			}

			if( m_pMyOnlineWorld->pMyData->GetFollowerParameterBonus( (UI08)i ) > 0 )
			{			
				if(m_siFont % 2)
				{
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() +5, m_BChar[TempFollowerNum].GetY() +5 ,
					m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[0]] );
				}
				else
				{
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() +5 , m_BChar[TempFollowerNum].GetY() +5 ,
					m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[1]] );
				}
			}
			else if( m_uiImpend == (UI08)i )
			{
				if( m_siFont == 0 || m_siFont == 3 )
				{
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() , m_BChar[TempFollowerNum].GetY() , 
					m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize, m_CharacterSlotSelect.Image );
				}
			}

			// 무게초과 이미지
			if(m_pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed((UI08)i))
			{
				clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + m_pButtonSpr->Header.Xsize , m_BChar[TempFollowerNum].GetY(),
					m_WeightOverSpr.Header.Xsize, m_WeightOverSpr.Header.Ysize, m_WeightOverSpr.Image);
			}

			if( CharID != 0 )
			{
				TempFollowerNum++;
			}

			
			// 마우스가 위에 이따면 용병에 대한 정보를 표시해 준다.
			if( m_BChar[i].IsInMouse() )
			{
				if( 0 <= m_index[i] && m_index[i] < ON_MAX_FOLLOWER_NUM)
				{
					siX = m_BChar[i].GetX() + m_pButtonSpr->Header.Xsize;
					siY = m_BChar[i].GetY() + m_pButtonSpr->Header.Ysize / 2 - m_FollowerInfo.Header.Ysize / 3;

					if(siX < 0)		siX = 0;
					if(siY < 0)		siY = 0;
					if(siY+m_FollowerInfo.Header.Ysize >= ON_SCREEN_YSIZE )	
						siY = ON_SCREEN_YSIZE - (m_FollowerInfo.Header.Ysize + 1);


					clGrp.PutSpriteJin(siX, siY, m_FollowerInfo.Header.Xsize, m_FollowerInfo.Header.Ysize, m_FollowerInfo.Image);
					clGrp.PutSpriteT( m_BChar[i].GetX() , m_BChar[i].GetY() , 
					m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize, m_CharacterSlotSelect.Image );
				}
				
			}
		}


		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

		for( i=0 ; i<ON_MAX_FOLLOWER_NUM; i++ )
			m_BChar[i].Put( hDC );
		
		// 마우스가 위에 이따면 용병에 대한 정보를 표시해 준다.
		for( j=0 ; j<ON_MAX_FOLLOWER_NUM; j++ )
		{
			if( m_BChar[j].IsInMouse() )
			{
				if( 0 <= m_index[j] && m_index[j] < ON_MAX_FOLLOWER_NUM)
				{
					CHAR	szTemp0[64], szTemp1[64], szTemp2[64], szTemp3[64];
					ZeroMemory(szTemp0, sizeof(szTemp0));
					ZeroMemory(szTemp1, sizeof(szTemp1));
					ZeroMemory(szTemp2, sizeof(szTemp2));
					ZeroMemory(szTemp3, sizeof(szTemp3));

					j != 11 ?
						siY = m_BChar[j].GetY() + m_pButtonSpr->Header.Ysize / 2:
						siY = ON_SCREEN_YSIZE - (m_FollowerInfo.Header.Ysize - 20);
					
					sprintf( szTemp0, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_LVPP),  m_pMyOnlineWorld->pMyData->GetFollowerCharName( (UI08)m_index[j] ), m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( (UI08)m_index[j] ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 20,  szTemp0, RGB( 150, 150, 150 ) );

					sprintf( szTemp1, "%s", m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_HP ));
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 38,  szTemp1, RGB( 150, 150, 150 ) );


					sprintf( szTemp1, "%d / %d", m_pMyOnlineWorld->pMyData->GetFollowerParameterLife( (UI08)m_index[j] ), m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife( (UI08)m_index[j] ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35 + 40, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 38,  szTemp1, RGB( 150, 150, 150 ) );

					sprintf( szTemp2, "%s", m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ITEMHELP_MP ));
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 56,  szTemp2, RGB( 150, 150, 150 ) );

					sprintf( szTemp2, "%d / %d", m_pMyOnlineWorld->pMyData->GetFollowerParameterMana( (UI08)m_index[j] ), m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana( (UI08)m_index[j] ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35 + 40, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 56,  szTemp2, RGB( 150, 150, 150 ) );

					sprintf( szTemp3, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_HELP_EXPPP) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 74,  szTemp3, RGB( 150, 150, 150 ) );

					sprintf( szTemp3, "%d / %d", m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp( (UI08)m_index[j] ), m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp( (UI08)m_index[j] ) );
					m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_BChar[j].GetX() + m_pButtonSpr->Header.Xsize + 35 + 40, 
						siY - m_FollowerInfo.Header.Ysize / 2 + 74,  szTemp3, RGB( 150, 150, 150 ) );
				}
			}
		}
		
		pSurface->ReleaseDC( hDC );
	}

	m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( 0 );
	m_pMyOnlineWorld->pMyData->GetTradeCredit();

	if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)
		m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->Draw(pSurface);
}


VOID	OnlineFollowerInfo::Free()
{
	clGrp.FreeXspr( m_LvSpr01 );
	clGrp.FreeXspr( m_LvSpr02 );
	clGrp.FreeXspr( m_LifeSpr );
	clGrp.FreeXspr( m_ExpSpr );
	clGrp.FreeXspr( m_CahrSpr );
	clGrp.FreeXspr( m_CharDieSpr );
	clGrp.FreeXspr(m_FollowerInfo);
	clGrp.FreeXspr(m_CharacterSlotSelect);
	clGrp.FreeXspr(m_CharacterSlotX);
	clGrp.FreeXspr(m_WeightOverSpr);
}


VOID	OnlineFollowerInfo::SetAction( BOOL bflag, BOOL bNomove )
{
	SI16 i;
	if(!m_bOpenNow && bflag && bNomove)
		m_bNoMove =	TRUE;

	if( bflag )	
	{
		if(m_bScrollRight)	return;

		m_bAction		=	bflag;
		// 경험치가 임박한 캐릭터를 구한다.
		m_uiImpend		=	GetLvUpImpend();
		m_bScrollRight  =	TRUE;		

		m_siMaxFollowerNum	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)) + 1;		// 신용,레벨에 따른 최대 보유가능 용병수(자신포함 최대10명)
		m_CurFollowerNum	= m_pMyOnlineWorld->pMyData->GetFollowerNum();

		for( i = 0 ; i < m_siMaxFollowerNum; i++ )
			m_BChar[i].SetAction(TRUE);
	}
	else
	{
		if(m_bScrollLeft)	return;

		if( m_siNoFieldAll != -1 ) m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldAll );
		if( m_siNoField    != -1 ) m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

		m_siNoFieldAll	=	-1;
		m_siNoField		=	-1;		
		m_bScrollLeft   =	TRUE;
		m_bNoMove		=	FALSE;

		UnLock();
		NoAction( FALSE );
	}
}

VOID	OnlineFollowerInfo::ScrollDrawRight(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( clGrp.LockSurface(pSurface))	
	{
		SI16 i, j, TempFollowerNum=0, CharID, siFont;
		SI32 CurLife, MaxLife, CurExp, NextExp, PrevExp, LifeBarHeight, ExpBarHeight;

		for( i=0 ; i < ON_MAX_FOLLOWER_NUM; i++ )
		{
			// 용병을 출력 해준다.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );

			m_BChar[i].SetX(m_BChar[i].GetX() + m_siScrollX);
			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );
			if(m_BChar[i].GetX() >= 0 )
			{
				m_BChar[i].SetX(0);
				m_bScrollRight = FALSE;	

				m_bOpenNow = TRUE;

				// 밑에 출력 되는 용병창의 크기
				SetRect( &m_rt, m_BChar[0].GetX(), m_BChar[0].GetY(), m_BChar[0].GetX() + m_pButtonSpr->Header.Xsize, m_BChar[0].GetY() +
					m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM );
				if( m_siNoField == -1 )		m_siNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_BChar[0].GetX(), 
					m_BChar[0].GetY(), m_BChar[0].GetX() + m_BChar[0].GetWidth(), m_BChar[11].GetY() + 
					m_BChar[11].GetHeight() );
			}

			if( CharID != 0 )
			{				
				// 용병 테두리 창을 그린다.
				//if( m_uiSelectSlot == m_index[i] )		m_BChar[TempFollowerNum].Put( m_pButtonSpr, 1, 1, 1, BUTTON_PUT_NOMOVE );
				//else										m_BChar[TempFollowerNum].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

				siFont = TransKindToFont( CharID );
				XSPR *pTempChar = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i) == 0 ? &m_CharDieSpr : &m_CahrSpr;
				
				clGrp.PutSpriteT( m_BChar[i].GetX()+4, m_siY + 5 + m_pButtonSpr->Header.Ysize * TempFollowerNum, pTempChar->Header.Xsize
					, pTempChar->Header.Ysize, &pTempChar->Image[pTempChar->Header.Start[siFont]] );


				// 생명력, 체력을 그려준다.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				// 생명력
				for(j = 0; j < LifeBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j,m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image);

				// 체력
				for(j = 0; j < ExpBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 57, m_BChar[TempFollowerNum].GetY() + 43 - j,m_ExpSpr.Header.Xsize, m_ExpSpr.Header.Ysize, m_ExpSpr.Image);

				if( m_uiImpend == (UI08)i )
				{
					if( m_siFont == 0 || m_siFont == 3 )
					{
						clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() , m_BChar[TempFollowerNum].GetY(), 
						m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize, m_CharacterSlotSelect.Image );					
					}
				}

				if( m_pMyOnlineWorld->pMyData->GetFollowerParameterBonus( (UI08)i ) > 0 )
				{			
					if(m_siFont % 2)
					{
						clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX()+5 , m_BChar[TempFollowerNum].GetY()+5 ,
						m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[0]] );
					}
					else
					{
						clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX()+5 , m_BChar[TempFollowerNum].GetY()+5 ,
						m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[1]] );
					}
				}

			}
			
			if(i >= m_siMaxFollowerNum)
			{	
				clGrp.PutSpriteT(m_BChar[i].GetX() + 4 , m_BChar[i].GetY() + 3, m_CharacterSlotX.Header.Xsize, m_CharacterSlotX.Header.Ysize, m_CharacterSlotX.Image );
			}

			// 무게초과 이미지
			if(m_pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed((UI08)i))
			{
				clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + m_pButtonSpr->Header.Xsize , m_BChar[TempFollowerNum].GetY(),
					m_WeightOverSpr.Header.Xsize, m_WeightOverSpr.Header.Ysize, m_WeightOverSpr.Image);
			}

			if( CharID != 0 )		TempFollowerNum++;
			
		}
		
		//for( ; TempFollowerNum < max(m_siMaxFollowerNum, m_CurFollowerNum); TempFollowerNum++)		m_BChar[TempFollowerNum].Put(m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE);
		clGrp.UnlockSurface(pSurface);
	}
}

VOID	OnlineFollowerInfo::ScrollDrawLeft(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( clGrp.LockSurface(pSurface))	
	{
		SI16 i, j, TempFollowerNum=0, CharID, siFont;
		SI32 CurLife, MaxLife, CurExp, NextExp, PrevExp, LifeBarHeight, ExpBarHeight;

		for( i=0 ; i < ON_MAX_FOLLOWER_NUM; i++ )
		{
			// 용병을 출력 해준다.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );


			m_BChar[i].SetX( m_BChar[i].GetX() - m_siScrollX);

			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

			if(m_BChar[i].GetX() <= (14-m_pButtonSpr->Header.Xsize))
			{
				m_BChar[i].SetX(14 - m_pButtonSpr->Header.Xsize );
				m_bScrollLeft = FALSE;
				m_bAction = m_bScrollRight;
				m_bOpenNow = FALSE;
				// 밑에 출력 되는 용병창의 크기
				SetRect( &m_rt, m_BChar[0].GetX(), m_BChar[0].GetY(), m_BChar[0].GetX() + m_pButtonSpr->Header.Xsize, m_BChar[0].GetY()
					+ m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM );				
			}

			if( CharID != 0 )
			{				
				// 용병 테두리 창을 그린다.
//				if( m_uiSelectSlot == m_index[i] )		m_BChar[TempFollowerNum].Put( m_pButtonSpr, 1, 1, 1, BUTTON_PUT_NOMOVE );
//				else									m_BChar[TempFollowerNum].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

				siFont = TransKindToFont( CharID );
				XSPR *pTempChar = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i) == 0 ? &m_CharDieSpr : &m_CahrSpr;
				
				clGrp.PutSpriteT( m_BChar[i].GetX()+4, m_siY + 5 + m_pButtonSpr->Header.Ysize * TempFollowerNum, pTempChar->Header.Xsize
					, pTempChar->Header.Ysize, &pTempChar->Image[pTempChar->Header.Start[siFont]] );

				// 생명력, 체력을 그려준다.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				// 생명력
				for(j = 0; j < LifeBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j,m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image);

				// 경험치 
				for(j = 0; j < ExpBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 57, m_BChar[TempFollowerNum].GetY() + 43 - j,m_ExpSpr.Header.Xsize, m_ExpSpr.Header.Ysize, m_ExpSpr.Image);

				if( m_uiImpend == (UI08)i )
				{
					if( m_siFont == 0 || m_siFont == 3 )
					{
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() , m_BChar[TempFollowerNum].GetY() , 
					m_CharacterSlotSelect.Header.Xsize, m_CharacterSlotSelect.Header.Ysize, m_CharacterSlotSelect.Image );					}
				}


				if( m_pMyOnlineWorld->pMyData->GetFollowerParameterBonus( (UI08)i ) > 0 )
				{			
					if(m_siFont % 2)
					{
						clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() +5, m_BChar[TempFollowerNum].GetY() +5,
							m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[0]] );
					}
					else
					{
						clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() +5, m_BChar[TempFollowerNum].GetY() +5 ,
							m_LvSpr02.Header.Xsize, m_LvSpr02.Header.Ysize, &m_LvSpr02.Image[m_LvSpr02.Header.Start[1]] );
					}
				}

			}
			if(i >= m_siMaxFollowerNum)
			{	
				clGrp.PutSpriteT(m_BChar[i].GetX() + 4 , m_BChar[i].GetY() + 3, m_CharacterSlotX.Header.Xsize, m_CharacterSlotX.Header.Ysize, m_CharacterSlotX.Image );
			}

			// 무게초과 이미지
			if(m_pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed((UI08)i))
			{
				clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + m_pButtonSpr->Header.Xsize , m_BChar[TempFollowerNum].GetY(),
					m_WeightOverSpr.Header.Xsize, m_WeightOverSpr.Header.Ysize, m_WeightOverSpr.Image);
			}

			if( CharID != 0 )		TempFollowerNum++;
		}

		

		//for( ; TempFollowerNum < max(m_siMaxFollowerNum, m_CurFollowerNum); TempFollowerNum++)		m_BChar[TempFollowerNum].Put(m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE);
		clGrp.UnlockSurface(pSurface);
	}
}

SI16	OnlineFollowerInfo::CheckSlot()
{
	SI16	i, j;

	for( i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		j = m_index[i];
		if( (m_BChar[i].IsInMouse()) && (m_pMyOnlineWorld->pMyData->IsValidFollower((UI08)j)) )
			return j;
	}

	return -1;
}

// 용병창의 크기를 구한다.
BOOL	OnlineFollowerInfo::CheckRect(POINT pt)
{
	if( PtInRect( &m_rt, pt ) )
		return TRUE;

	return FALSE;
}

VOID	OnlineFollowerInfo::Lock()
{
	if(m_siNoFieldAll != -1) m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldAll );
	m_siNoFieldAll	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( 0, 0, 799, 599 );
}


VOID	OnlineFollowerInfo::UnLock()
{
	if( m_siNoFieldAll != -1 )
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoFieldAll );

	m_siNoFieldAll = -1;
}

// 레벨업이 임박한 캐릭터를 찾는다.
UI08	OnlineFollowerInfo::GetLvUpImpend()
{
	UI08	i, uiTempFollower;
	UI32	uiRet[ ON_MAX_FOLLOWER_NUM ], uiTempRet = 0;

	ZeroMemory( uiRet, sizeof( UI32 ) * ON_MAX_FOLLOWER_NUM );

	for( i=0 ; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( m_pMyOnlineWorld->pMyData->IsValidFollower(i) )
			uiRet[i] = m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp(i) - m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp(i);
	}

	uiTempRet		=	uiRet[0];
	uiTempFollower	=	0;

	// 가장 조금 남아 있는 경험치를 찾는다.
	for( i=0 ; i<ON_MAX_FOLLOWER_NUM-1; i++ )
	{
		if( m_pMyOnlineWorld->pMyData->IsValidFollower(i+1) )
		{
			if( uiTempRet > uiRet[i+1] )
			{
				uiTempRet		= uiRet[i+1];
				uiTempFollower	= i+1;
			}			
		}
	}

	return uiTempFollower;
}

SI16	OnlineFollowerInfo::TransKindToFont(SI32 CharID)
{
	SI16 siFont;
	switch(CharID)
	{
	// 주인공
	case 67:		siFont = 0;				break;	// 건
	case 323:		siFont = 1;				break;	// 이명화
	case 8771:		siFont = 2;				break;	// 시마다 료
	case 9027:		siFont = 3;				break;	// 겐노 하나히네
	case 10307:		siFont = 16;			break;	// 왕대룡
	case 10563:		siFont = 17;			break;	// 왕미령
	case 12867:		siFont = 34;			break;	// 당호엽
	case 13123:		siFont = 35;			break;	// 노수연
	case 23107:		siFont = 0;				break;	// 운영자

	// 조선용병	
	case 2133:		siFont = 4;				break;	// 조선 유생
	case 11861:		siFont = 4;				break;	// 조선 유생(Upgrade)
	case 9301:		siFont = 26;			break;	// 유성룡

	case 341:		siFont = 5;				break;	// 조선 창잡이
	case 12117:		siFont = 5;				break;	// 조선 창잡이(Upgrade)
	case 8533:		siFont = 27;			break;	// 김시민

	case 85:		siFont = 6;				break;	// 짐꾼

	case 853:		siFont = 7;				break;	// 조선 파계승
	case 12373:		siFont = 7;				break;	// 조선 파계승(Upgrade)
	case 9045:		siFont = 25;			break;	// 사명대사	
		
	case 597:		siFont = 8;				break;	// 조선 활잡이
	case 11349:		siFont = 8;				break;	// 조선 활잡이(Upgrade)
	case 7765:		siFont = 28;			break;	// 이순신

	case 1365:		siFont = 9;				break;	// 조선 기마궁수
	case 11605:		siFont = 9;				break;	// 조선 기마궁수(Upgrade)
	case 8277:		siFont = 24;			break;	// 신립

	case 22101:		siFont = 80;			break;	// 칼잡이
	case 22357:		siFont = 78;			break;	// 권율

	case 24149:		siFont = 89;			break;	// 의술사
	case 24405:		siFont = 85;			break;	// 허준

	// 일본용병
	case 6229:		siFont = 10;			break;	// 일본 농부
	
	case 2645:		siFont = 11;			break;	// 일본 조총낭인
	case 12885:		siFont = 11;			break;	// 일본 조총낭인(Upgrade)
	case 10069:		siFont = 32;			break;	// 와키자카

	case 3157:		siFont = 12;			break;	// 퇴마사
	case 13397:		siFont = 12;			break;	// 퇴마사(Upgrade)
	case 11093:		siFont = 33;			break;	// 세이쇼오

	case 3413:		siFont = 13;			break;	// 닌자
	case 13653:		siFont = 13;			break;	// 닌자(Upgrade)
	case 10837:		siFont = 31;			break;	// 아사코

	case 2901:		siFont = 14;			break;	// 일본 늑대낭인
	case 13141:		siFont = 14;			break;	// 일본 늑대낭인(Upgrade)
	case 9557:		siFont = 29;			break;	// 도쿠가와

	case 2389:		siFont = 15;			break;	// 일본 검술낭인
	case 12629:		siFont = 15;			break;	// 일본 검술낭인(Upgrade)
	case 10325:		siFont = 30;			break;	// 가토

	case 22613:		siFont = 82;			break;	// 기마전사
	case 22869:		siFont = 76;			break;	// 고니시

	case 24661:		siFont = 88;			break;	// 음양사
	case 24917:		siFont = 91;			break;	// 세이메이

	// 대만용병	
	case 13909:		siFont = 18;			break;	// 대만 짐꾼

	case 14165:		siFont = 19;			break;	// 도끼거한
	case 16469:		siFont = 19;			break;	// 고급도끼거한
	case 18005:		siFont = 36;			break;	// 왕거한

	case 14933:		siFont = 20;			break;	// 여전사
	case 16213:		siFont = 20;			break;	// 고급여전사
	case 19029:		siFont = 37;			break;	// 조세림

	case 15189:		siFont = 21;			break;	// 봉술가
	case 15957:		siFont = 21;			break;	// 고급봉술가
	case 18261:		siFont = 38;			break;	// 유영복

	case 14677:		siFont = 22;			break;	// 주술사
	case 15701:		siFont = 22;			break;	// 고급주술사
	case 18773:		siFont = 39;			break;	// 임평후

	case 14421:		siFont = 23;			break;	// 서양총수
	case 15445:		siFont = 23;			break;	// 고급서양총수
	case 18517:		siFont = 40;			break;	// 페르난데스

	case 23637:		siFont = 81;			break;	// 야수전사
	case 23893:		siFont = 77;			break;	// 손유창

	case 25173:		siFont = 87;			break;	// 염력사
	case 25429:		siFont = 84;			break;	// 장선화

	// 중국용병	
	case 6485:		siFont = 66;			break;	// 중국 짐꾼

	case 17237:		siFont = 63;			break;	// 모험가
	case 19797:		siFont = 63;			break;	// 고급모험가
	case 21333:		siFont = 70;			break;	// 축융

	case 16981:		siFont = 64;			break;	// 무도가
	case 19541:		siFont = 64;			break;	// 고급무도가
	case 21077:		siFont = 69;			break;	// 동방불패

	case 17493:		siFont = 65;			break;	// 수도승
	case 20053:		siFont = 65;			break;	// 고급수도승
	case 21589:		siFont = 71;			break;	// 제갈공명

	case 16725:		siFont = 67;			break;	// 큰칼무사
	case 19285:		siFont = 67;			break;	// 고급큰칼무사
	case 20565:		siFont = 72;			break;	// 조승훈

	case 17749:		siFont = 68;			break;	// 화포수
	case 20309:		siFont = 68;			break;	// 고급화포수
	case 20821:		siFont = 73;			break;	// 이령

	case 23125:		siFont = 83;			break;	// 도술사
	case 23381:		siFont = 79;			break;	// 순비연

	case 25685:		siFont = 90;			break;	// 장창무사
	case 25941:		siFont = 86;			break;	// 곽후

	// 조선 몬스터
	case 3149:		siFont = 41;			break;	// 구미호
	case 4685:		siFont = 42;			break;	// 궁수산적
	case 11341:		siFont = 43;			break;	// 도굴꾼
	case 8525:		siFont = 44;			break;	// 백호
	case 3405:		siFont = 45;			break;	// 불가사리
	case 6221:		siFont = 46;			break;	// 사슴
	case 11085:		siFont = 47;			break;	// 해마
	case 7757:		siFont = 62;			break;	// 돌하루방

	// 일본 몬스터
	case 1357:		siFont = 48;			break;	// 견신
	case 8269:		siFont = 49;			break;	// 귀묘
	case 5709:		siFont = 50;			break;	// 독지네
	case 9549:		siFont = 51;			break;	// 사마귀
	case 7245:		siFont = 52;			break;	// 설녀
	case 6733:		siFont = 53;			break;	// 왜구
	case 4429:		siFont = 54;			break;	// 털가면

	// 대만 몬스터
	case 18509:		siFont = 55;			break;	// 광우
	case 15181:		siFont = 56;			break;	// 괴물선인장
	case 12877:		siFont = 57;			break;	// 대만왕벌
	case 16461:		siFont = 58;			break;	// 마두인
	case 15949:		siFont = 59;			break;	// 식인악어
	case 13133:		siFont = 60;			break;	// 악덕상인
	case 14413:		siFont = 61;			break;	// 원주민무당
	
	case 21837:		siFont = 74;			break;  //화산의불꽃
	case 21845:		siFont = 75;			break;  //당나귀.

	// 중국 몬스터
	case 14669:		siFont = 93;			break;	// 해적
	case 21069:		siFont = 94;			break;	// 족제비
	case 17997:		siFont = 95;			break;	// 얼음나무
	case 11853:		siFont = 96;			break;	// 반달곰
	case 18253:		siFont = 97;			break;	// 눈도마뱀
	case 19021:		siFont = 98;			break;	// 괴물덩굴손

	default:		siFont = 0;				break;
		//clGrp.Error("No Char", "[%d]", CharID);		siFont = -1;		break;
	}
	
	return siFont;
}

XSPR	*OnlineFollowerInfo::GetFollowerSpr(BOOL bDie)
{
	if(bDie)
		return 	&m_CharDieSpr;
	else
		return	&m_CahrSpr;
}

XSPR	*OnlineFollowerInfo::GetFollowerBarSpr(BOOL bLife)
{
	if(bLife)
		return 	&m_LifeSpr;
	else
		return	&m_ExpSpr;
}

SI08	OnlineFollowerInfo::ValidSlot(UI08 uiSlot)
{
	return (SI08)m_index[uiSlot];
}
