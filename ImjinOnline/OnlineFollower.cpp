/********************************************************************************************
	���ϸ�	:	OnlineFollower.cpp
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.03.30
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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include "OnlineFieldAuction.h"
#include "OnlineTrade.h"


extern _InputDevice				IpD;
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
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

// �ʱ�ȭ
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

	// ĳ���� ��ư
	for( i = 0 ; i < ON_MAX_FOLLOWER_NUM; i++ )
		m_BChar[i].Create(m_siX , m_siY + (i * m_pButtonSpr->Header.Ysize), m_pButtonSpr->Header.Xsize, m_pButtonSpr->Header.Ysize, "", BUTTON_PUT_LEFT, FALSE );

	m_siMaxFollowerNum	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)) + 1;		// �ſ�,������ ���� �ִ� �������� �뺴��(�ڽ����� �ִ�10��)
	m_CurFollowerNum	= m_pMyOnlineWorld->pMyData->GetFollowerNum();
	// �ؿ� ��� �Ǵ� �뺴â�� ũ��
	SetRect( &m_rt, m_siX, m_siY, m_siX + m_pButtonSpr->Header.Xsize, m_siY + m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM);

	// �����̸� �ε� �Ѵ�.
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

// �׼�
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

	// �뺴 �ε��� �ʱ�ȭ
	for(i=0; i<ON_MAX_FOLLOWER_NUM; i++)	m_index[i] = -1;

	// ���� �뺴�� ���� ���ʷ� index�� �ٿ��ش�.
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
				// ���������� Ŭ���� ���
				if( m_BChar[i].Process( m_bMouseRight) )
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)	
						return;

					// ��� ��ư�� ���ȴ��� Ȯ���� �Ѵ�.
					if( m_pMyOnlineWorld->pMyData->IsValidFollower( (UI08)m_index[i] ) )
					{
						if(m_pMyOnlineWorld->pOnlinePannel->GetFlagOptionWindow(ONLINEPANNEL_DISABLEFLAG_STATUS))
						{
							// ���� ���� ������ �뺴�� ��ȿ�� ĳ���� �̸�
							// ȿ������ ����� ���ְ�
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

				// �������� Ŭ���� �κ��丮
				else if( m_BChar[i].Process( m_bMouseDown ) )
				{
					// ��ſ� �����ϸ� �κ��丮�� ���� ���Ѵ�!!

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
			// �뺴�� ��� ���ش�.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );

			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

			if( CharID != 0 )
			{				
				// �뺴 �׵θ� â�� �׸���.
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
				
				// �������� �߽� ��ǥ�� �������� ĳ���͸� ����ش�. 
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

				

				// �����, ü���� �׷��ش�.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				for(j = 0; j < LifeBarHeight-2; j++)
				{
					// �����
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j, 
					m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image );
				}

				for(j = 0; j < ExpBarHeight-2; j++)
				{
					// ü��
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

			// �����ʰ� �̹���
			if(m_pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed((UI08)i))
			{
				clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + m_pButtonSpr->Header.Xsize , m_BChar[TempFollowerNum].GetY(),
					m_WeightOverSpr.Header.Xsize, m_WeightOverSpr.Header.Ysize, m_WeightOverSpr.Image);
			}

			if( CharID != 0 )
			{
				TempFollowerNum++;
			}

			
			// ���콺�� ���� �̵��� �뺴�� ���� ������ ǥ���� �ش�.
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
		
		// ���콺�� ���� �̵��� �뺴�� ���� ������ ǥ���� �ش�.
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
		// ����ġ�� �ӹ��� ĳ���͸� ���Ѵ�.
		m_uiImpend		=	GetLvUpImpend();
		m_bScrollRight  =	TRUE;		

		m_siMaxFollowerNum	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)) + 1;		// �ſ�,������ ���� �ִ� �������� �뺴��(�ڽ����� �ִ�10��)
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
			// �뺴�� ��� ���ش�.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );

			m_BChar[i].SetX(m_BChar[i].GetX() + m_siScrollX);
			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );
			if(m_BChar[i].GetX() >= 0 )
			{
				m_BChar[i].SetX(0);
				m_bScrollRight = FALSE;	

				m_bOpenNow = TRUE;

				// �ؿ� ��� �Ǵ� �뺴â�� ũ��
				SetRect( &m_rt, m_BChar[0].GetX(), m_BChar[0].GetY(), m_BChar[0].GetX() + m_pButtonSpr->Header.Xsize, m_BChar[0].GetY() +
					m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM );
				if( m_siNoField == -1 )		m_siNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_BChar[0].GetX(), 
					m_BChar[0].GetY(), m_BChar[0].GetX() + m_BChar[0].GetWidth(), m_BChar[11].GetY() + 
					m_BChar[11].GetHeight() );
			}

			if( CharID != 0 )
			{				
				// �뺴 �׵θ� â�� �׸���.
				//if( m_uiSelectSlot == m_index[i] )		m_BChar[TempFollowerNum].Put( m_pButtonSpr, 1, 1, 1, BUTTON_PUT_NOMOVE );
				//else										m_BChar[TempFollowerNum].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

				siFont = TransKindToFont( CharID );
				XSPR *pTempChar = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i) == 0 ? &m_CharDieSpr : &m_CahrSpr;
				
				clGrp.PutSpriteT( m_BChar[i].GetX()+4, m_siY + 5 + m_pButtonSpr->Header.Ysize * TempFollowerNum, pTempChar->Header.Xsize
					, pTempChar->Header.Ysize, &pTempChar->Image[pTempChar->Header.Start[siFont]] );


				// �����, ü���� �׷��ش�.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				// �����
				for(j = 0; j < LifeBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j,m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image);

				// ü��
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

			// �����ʰ� �̹���
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
			// �뺴�� ��� ���ش�.
			CharID = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( (UI08)i );


			m_BChar[i].SetX( m_BChar[i].GetX() - m_siScrollX);

			m_BChar[i].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

			if(m_BChar[i].GetX() <= (14-m_pButtonSpr->Header.Xsize))
			{
				m_BChar[i].SetX(14 - m_pButtonSpr->Header.Xsize );
				m_bScrollLeft = FALSE;
				m_bAction = m_bScrollRight;
				m_bOpenNow = FALSE;
				// �ؿ� ��� �Ǵ� �뺴â�� ũ��
				SetRect( &m_rt, m_BChar[0].GetX(), m_BChar[0].GetY(), m_BChar[0].GetX() + m_pButtonSpr->Header.Xsize, m_BChar[0].GetY()
					+ m_pButtonSpr->Header.Ysize * ON_MAX_FOLLOWER_NUM );				
			}

			if( CharID != 0 )
			{				
				// �뺴 �׵θ� â�� �׸���.
//				if( m_uiSelectSlot == m_index[i] )		m_BChar[TempFollowerNum].Put( m_pButtonSpr, 1, 1, 1, BUTTON_PUT_NOMOVE );
//				else									m_BChar[TempFollowerNum].Put( m_pButtonSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

				siFont = TransKindToFont( CharID );
				XSPR *pTempChar = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i) == 0 ? &m_CharDieSpr : &m_CahrSpr;
				
				clGrp.PutSpriteT( m_BChar[i].GetX()+4, m_siY + 5 + m_pButtonSpr->Header.Ysize * TempFollowerNum, pTempChar->Header.Xsize
					, pTempChar->Header.Ysize, &pTempChar->Image[pTempChar->Header.Start[siFont]] );

				// �����, ü���� �׷��ش�.
				CurLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLife((UI08)i);
				MaxLife	= m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife((UI08)i);

				CurExp 	= m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp((UI08)i);
				NextExp	= m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp((UI08)i);
				PrevExp = m_pMyOnlineWorld->pMyData->GetFollowerParameterPrevExp((UI08)i);
				
				LifeBarHeight = (SI16)(((R32)CurLife / (R32)MaxLife) * (R32)(m_pButtonSpr->Header.Ysize - 10));
				ExpBarHeight = (SI16)min((R32)(m_pButtonSpr->Header.Ysize - 10), (((R32)(CurExp-PrevExp) / (R32)(NextExp-PrevExp)) * (R32)(m_pButtonSpr->Header.Ysize - 10)));

				// �����
				for(j = 0; j < LifeBarHeight-2; j++)					
					clGrp.PutSpriteT( m_BChar[TempFollowerNum].GetX() + 53, m_BChar[TempFollowerNum].GetY() + 43 - j,m_LifeSpr.Header.Xsize, m_LifeSpr.Header.Ysize, m_LifeSpr.Image);

				// ����ġ 
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

			// �����ʰ� �̹���
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

// �뺴â�� ũ�⸦ ���Ѵ�.
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

// �������� �ӹ��� ĳ���͸� ã�´�.
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

	// ���� ���� ���� �ִ� ����ġ�� ã�´�.
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
	// ���ΰ�
	case 67:		siFont = 0;				break;	// ��
	case 323:		siFont = 1;				break;	// �̸�ȭ
	case 8771:		siFont = 2;				break;	// �ø��� ��
	case 9027:		siFont = 3;				break;	// �ճ� �ϳ�����
	case 10307:		siFont = 16;			break;	// �մ��
	case 10563:		siFont = 17;			break;	// �չ̷�
	case 12867:		siFont = 34;			break;	// ��ȣ��
	case 13123:		siFont = 35;			break;	// �����
	case 23107:		siFont = 0;				break;	// ���

	// �����뺴	
	case 2133:		siFont = 4;				break;	// ���� ����
	case 11861:		siFont = 4;				break;	// ���� ����(Upgrade)
	case 9301:		siFont = 26;			break;	// ������

	case 341:		siFont = 5;				break;	// ���� â����
	case 12117:		siFont = 5;				break;	// ���� â����(Upgrade)
	case 8533:		siFont = 27;			break;	// ��ù�

	case 85:		siFont = 6;				break;	// ����

	case 853:		siFont = 7;				break;	// ���� �İ��
	case 12373:		siFont = 7;				break;	// ���� �İ��(Upgrade)
	case 9045:		siFont = 25;			break;	// �����	
		
	case 597:		siFont = 8;				break;	// ���� Ȱ����
	case 11349:		siFont = 8;				break;	// ���� Ȱ����(Upgrade)
	case 7765:		siFont = 28;			break;	// �̼���

	case 1365:		siFont = 9;				break;	// ���� �⸶�ü�
	case 11605:		siFont = 9;				break;	// ���� �⸶�ü�(Upgrade)
	case 8277:		siFont = 24;			break;	// �Ÿ�

	case 22101:		siFont = 80;			break;	// Į����
	case 22357:		siFont = 78;			break;	// ����

	case 24149:		siFont = 89;			break;	// �Ǽ���
	case 24405:		siFont = 85;			break;	// ����

	// �Ϻ��뺴
	case 6229:		siFont = 10;			break;	// �Ϻ� ���
	
	case 2645:		siFont = 11;			break;	// �Ϻ� ���ѳ���
	case 12885:		siFont = 11;			break;	// �Ϻ� ���ѳ���(Upgrade)
	case 10069:		siFont = 32;			break;	// ��Ű��ī

	case 3157:		siFont = 12;			break;	// �𸶻�
	case 13397:		siFont = 12;			break;	// �𸶻�(Upgrade)
	case 11093:		siFont = 33;			break;	// ���̼��

	case 3413:		siFont = 13;			break;	// ����
	case 13653:		siFont = 13;			break;	// ����(Upgrade)
	case 10837:		siFont = 31;			break;	// �ƻ���

	case 2901:		siFont = 14;			break;	// �Ϻ� ���볶��
	case 13141:		siFont = 14;			break;	// �Ϻ� ���볶��(Upgrade)
	case 9557:		siFont = 29;			break;	// ������

	case 2389:		siFont = 15;			break;	// �Ϻ� �˼�����
	case 12629:		siFont = 15;			break;	// �Ϻ� �˼�����(Upgrade)
	case 10325:		siFont = 30;			break;	// ����

	case 22613:		siFont = 82;			break;	// �⸶����
	case 22869:		siFont = 76;			break;	// ��Ͻ�

	case 24661:		siFont = 88;			break;	// �����
	case 24917:		siFont = 91;			break;	// ���̸���

	// �븸�뺴	
	case 13909:		siFont = 18;			break;	// �븸 ����

	case 14165:		siFont = 19;			break;	// ��������
	case 16469:		siFont = 19;			break;	// ��޵�������
	case 18005:		siFont = 36;			break;	// �հ���

	case 14933:		siFont = 20;			break;	// ������
	case 16213:		siFont = 20;			break;	// ��޿�����
	case 19029:		siFont = 37;			break;	// ������

	case 15189:		siFont = 21;			break;	// ������
	case 15957:		siFont = 21;			break;	// ��޺�����
	case 18261:		siFont = 38;			break;	// ������

	case 14677:		siFont = 22;			break;	// �ּ���
	case 15701:		siFont = 22;			break;	// ����ּ���
	case 18773:		siFont = 39;			break;	// ������

	case 14421:		siFont = 23;			break;	// �����Ѽ�
	case 15445:		siFont = 23;			break;	// ��޼����Ѽ�
	case 18517:		siFont = 40;			break;	// �丣������

	case 23637:		siFont = 81;			break;	// �߼�����
	case 23893:		siFont = 77;			break;	// ����â

	case 25173:		siFont = 87;			break;	// ���»�
	case 25429:		siFont = 84;			break;	// �弱ȭ

	// �߱��뺴	
	case 6485:		siFont = 66;			break;	// �߱� ����

	case 17237:		siFont = 63;			break;	// ���谡
	case 19797:		siFont = 63;			break;	// ��޸��谡
	case 21333:		siFont = 70;			break;	// ����

	case 16981:		siFont = 64;			break;	// ������
	case 19541:		siFont = 64;			break;	// ��޹�����
	case 21077:		siFont = 69;			break;	// �������

	case 17493:		siFont = 65;			break;	// ������
	case 20053:		siFont = 65;			break;	// ��޼�����
	case 21589:		siFont = 71;			break;	// ��������

	case 16725:		siFont = 67;			break;	// ūĮ����
	case 19285:		siFont = 67;			break;	// ���ūĮ����
	case 20565:		siFont = 72;			break;	// ������

	case 17749:		siFont = 68;			break;	// ȭ����
	case 20309:		siFont = 68;			break;	// ���ȭ����
	case 20821:		siFont = 73;			break;	// �̷�

	case 23125:		siFont = 83;			break;	// ������
	case 23381:		siFont = 79;			break;	// ����

	case 25685:		siFont = 90;			break;	// ��â����
	case 25941:		siFont = 86;			break;	// ����

	// ���� ����
	case 3149:		siFont = 41;			break;	// ����ȣ
	case 4685:		siFont = 42;			break;	// �ü�����
	case 11341:		siFont = 43;			break;	// ������
	case 8525:		siFont = 44;			break;	// ��ȣ
	case 3405:		siFont = 45;			break;	// �Ұ��縮
	case 6221:		siFont = 46;			break;	// �罿
	case 11085:		siFont = 47;			break;	// �ظ�
	case 7757:		siFont = 62;			break;	// ���Ϸ��

	// �Ϻ� ����
	case 1357:		siFont = 48;			break;	// �߽�
	case 8269:		siFont = 49;			break;	// �͹�
	case 5709:		siFont = 50;			break;	// ������
	case 9549:		siFont = 51;			break;	// �縶��
	case 7245:		siFont = 52;			break;	// ����
	case 6733:		siFont = 53;			break;	// �ֱ�
	case 4429:		siFont = 54;			break;	// �а���

	// �븸 ����
	case 18509:		siFont = 55;			break;	// ����
	case 15181:		siFont = 56;			break;	// ����������
	case 12877:		siFont = 57;			break;	// �븸�չ�
	case 16461:		siFont = 58;			break;	// ������
	case 15949:		siFont = 59;			break;	// ���ξǾ�
	case 13133:		siFont = 60;			break;	// �Ǵ�����
	case 14413:		siFont = 61;			break;	// ���ֹι���
	
	case 21837:		siFont = 74;			break;  //ȭ���ǺҲ�
	case 21845:		siFont = 75;			break;  //�糪��.

	// �߱� ����
	case 14669:		siFont = 93;			break;	// ����
	case 21069:		siFont = 94;			break;	// ������
	case 17997:		siFont = 95;			break;	// ��������
	case 11853:		siFont = 96;			break;	// �ݴް�
	case 18253:		siFont = 97;			break;	// ��������
	case 19021:		siFont = 98;			break;	// ����������

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
