//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 용병 출력 클래스
//	File Name		: OnlineMercenary.cpp
//	Birth Date		: 2003.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		필드 그래픽 출력 부분 중 용병창 출력을 담당하는 부분입니다.
//		기존의 데이터는 내부적으로 초상 데이터를 생성하여 용병창을 출력하도록 작업되었습니다만,
//		이번 작업을 통해 여러 부분에서 초상 그래픽을 응용할 수 있도록 초상 생성 및 그리는 부분을
//		외곽으로 꺼냈습니다. 따라서 클래스에 큰 변화가 생겼습니다.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: 헤더 선언부
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <clGame.h>
#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineHelp.h"			// actdoll (2004/01/02 10:46) : 풍선도움말 클래스 추가
#include "OnlinePortrait.h"		// actdoll (2004/01/02 10:46) : 초상 클래스 추가
#include "OnlineMercenary-Parser.h"
#include "OnlineMercenary.h"

#include "GSCDefine.h"
#include "OnlineFieldArea.h"
#include "..\\Gersang\\Music.h"
#include "OnlineTradeBook.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineTrade.h"

// #include <crtdbg.h>
#include <assert.h>
using namespace std;

extern _InputDevice		IpD;
extern _clGame			*pGame;


//-----------------------------------------------------------------------------
// Name: Init()														[private]
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 생성
//-----------------------------------------------------------------------------
void OnlineMercenary::Init()
{
//	_ASSERTE( m_pOnlinePortrait );

	SI32	siWidth, siHeight;
	m_pOnlinePortrait->GetRangeOfMercenary( siWidth, siHeight );

	// 각 버튼의 정보를 초기화
	for( int i=0; i<ON_MAX_FOLLOWER_NUM; ++i )
	{
		SetRect( &m_stSlotInfo[i].m_Rect,	4,				5 + ( siHeight + 3 ) * i,
											4 + siWidth,	5 + ( siHeight + 3 ) * i + siHeight );
		m_stSlotInfo[i].m_siDBID			= -1;
		m_stSlotInfo[i].m_siDBSlotNo		= -1;
		m_stSlotInfo[i].m_siSlotAreaAccessID= -1;
	}
	m_siEnableSlotMaxCount	= 0;
	m_siCurrentSlotCount	= 0;

	m_siMouseOverSlot		= -1;
	m_siSelectedSlot		= 0;
	m_siDisableFieldAreaID	= -1;	
	m_siPrevSelectSlot		= -1;

	m_bAction		= FALSE;
	m_bMouseLPush	= FALSE;
	m_bMouseRPush	= FALSE;
	m_bLock			= FALSE;
	m_bTradeMode	= FALSE;
	m_bLoadSpr		= FALSE;
	m_bHighLight	= FALSE;

	Refresh_Slot();
}

//-----------------------------------------------------------------------------
// Name: Free()														[private]
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 제거
//-----------------------------------------------------------------------------
void OnlineMercenary::Free()
{
	for( int i=0; i<ON_MAX_FOLLOWER_NUM; ++i )
	{
		SetRect( &m_stSlotInfo[i].m_Rect,	0,	0,	0,	0 );
		m_stSlotInfo[i].m_siDBID			= -1;
		m_stSlotInfo[i].m_siDBSlotNo		= -1;
		m_stSlotInfo[i].m_siSlotAreaAccessID= -1;
	}
	m_siEnableSlotMaxCount	= 0;
	m_siCurrentSlotCount	= 0;

	m_siMouseOverSlot		= -1;
	m_siSelectedSlot		= 0;
	m_siDisableFieldAreaID	= -1;	

	m_bAction		= FALSE;
	m_bMouseLPush	= FALSE;
	m_bMouseRPush	= FALSE;
	m_bLock			= FALSE;
	m_bTradeMode	= FALSE;
	m_bLoadSpr		= FALSE;
}

//-----------------------------------------------------------------------------
// Name: Update_MousePosition()										[private]
// Code: actdoll (2003-12-23)
// Desc: 마우스 위치에 대한 체크
//-----------------------------------------------------------------------------
void	OnlineMercenary::Update_MousePosition()
{	
	POINT	pt;
	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;

	// 각 구획별로 마우스 범위 체크
	m_siMouseOverSlot	= -1;
	for( int i=0; i<m_siEnableSlotMaxCount; i++)
	{
		if( m_stSlotInfo[i].m_siDBID < 0 )					break;		// 슬롯에 적재된 정보가 없을 경우 리턴
		if( PtInRect( &m_stSlotInfo[i].m_Rect, pt ) )					// 구획에 들어왔을 경우
		{
			// 해당 슬롯번호 적재
			m_siMouseOverSlot		= i;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Update_MouseBtn()											[private]
// Code: actdoll (2003-12-23)
// Desc: 마우스 버튼에 대한 체크
//-----------------------------------------------------------------------------
void OnlineMercenary::Update_MouseBtn()
{	
	if( !m_bMouseLPush && !m_bMouseRPush )		return;
	if( m_siMouseOverSlot < 0 )					return;

	// 버튼을 누른 상태이므로 기존 선택 슬롯의 정보를 확인하여 마우스가 올라간 슬롯으로 교체
//	if( m_siSelectedSlot != m_siMouseOverSlot )
//	{
		Set_SelectedSlot( m_siMouseOverSlot, false );
//	}

	// 선택한 슬롯에 대하여 외부 정보의 갱신
	SI32	siDBID		= m_stSlotInfo[m_siSelectedSlot].m_siDBID;
	SI32	siDBSlotNo	= m_stSlotInfo[m_siSelectedSlot].m_siDBSlotNo;
	m_pMyOnlineWorld->pOnlineTradeBook->SetSelect		( siDBSlotNo );	// 해당 DB슬롯을 선택했음을 알림

	// 선택번호가 클릭번호와 동일한 경우
	if( m_siPrevSelectSlot == siDBSlotNo )
	{
		// 왼 버튼을 눌렀을 때
		if		( m_bMouseLPush )
		{			
			// 개인 거래 중에는 동일 캐릭터칸을 클릭하더라도 어떠한 작업도 하지 않는다.
			if( m_pMyOnlineWorld->pOnlineTrade->IsActive() == FALSE )
			{

				BOOL	bIsActive = m_pMyOnlineWorld->pOnlineInventory->IsActive();
				
				// 인벤토리가 비활성화 중이라면 창 세팅
				if( !bIsActive )
				{
					m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
					m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
				}			
										
				// 인벤토리창 토글링
				m_pMyOnlineWorld->pOnlineInventory->SetActive			( !bIsActive );							
			}

			IpD.LeftPressSwitch		= FALSE;
		}
		// 오른 버튼을 눌렀을 때
		else if	( m_bMouseRPush )
		{
			BOOL	bIsActive = m_pMyOnlineWorld->pOnlineTradeBook->IsAction();
			// 스테이터스 창 토글링
			m_pMyOnlineWorld->pOnlineTradeBook->SetAction			( !bIsActive, ON_BOOK_STATUS );
			IpD.RightPressSwitch	= FALSE;
		}
	}
	// 선택번호가 클릭번호와 다른 경우
	else
	{
		// 왼 버튼을 눌렀을 때
		if		( m_bMouseLPush )
		{
			// 인벤토리 창 세팅
			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
			m_pMyOnlineWorld->pOnlineInventory->SetActive		( true );
			IpD.LeftPressSwitch		= FALSE;
		}
		// 오른 버튼을 눌렀을 때
		else if	( m_bMouseRPush )
		{
			// 인벤토리 창이 켜져있을 경우 세팅을 맞춰준다.
			if( m_pMyOnlineWorld->pOnlineInventory->IsActive() )
			{
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
				m_pMyOnlineWorld->pOnlineInventory->SetActive		( true );
			}
			// 스테이터스 창 세팅
			m_pMyOnlineWorld->pOnlineTradeBook->SetAction		( true, ON_BOOK_STATUS );
			IpD.RightPressSwitch	= FALSE;
		}
	}

	// 신규 선택 슬롯번호 설정
	m_siPrevSelectSlot = siDBSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Check_SlotInfo()											[private]
// Code: actdoll (2003-12-23)
// Desc: 용병 초상에 의해 배치될 슬롯의 정보 처리.
//-----------------------------------------------------------------------------
void OnlineMercenary::Check_SlotInfo()
{
	SI32	i;

	// 현재 레벨 기준으로 최대 용병 갯수를 갱신한다.
	m_siEnableSlotMaxCount	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum( (SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) ) + 1;

	// 현재 DB에 적재되어 있는 용병 정보를 체크하여 슬롯별로 용병을 채워넣는다.
	SI32	iSlotNo = 0;
	for( i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		// 해당 DB 슬롯에 용병이 존재할 경우
		if( m_pMyOnlineWorld->pMyData->IsValidFollower( i ) )
		{
			// 정보 적재
			m_stSlotInfo[iSlotNo].m_siDBSlotNo	= i;
			m_stSlotInfo[iSlotNo].m_siDBID		= m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( i );
			iSlotNo++;
		}
	}

	// 비어있는 나머지 용병 슬롯을 깨끗이 처리한다.
	for( i=iSlotNo; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		m_stSlotInfo[i].m_siDBSlotNo	= -1;
		m_stSlotInfo[i].m_siDBID		= -1;
	}

	// 현존하는 용병의 갯수를 받는다.(나 자신 포함)
	m_siCurrentSlotCount	= iSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Check_SlotArea()											[private]
// Code: actdoll (2003-12-23)
// Desc: 용병 초상에 의해 배치될 슬롯 구획의 재계산.
//-----------------------------------------------------------------------------
void OnlineMercenary::Check_SlotArea()
{
	// 용병 버튼창 전 구간을 다시 채움
	SI32	siWidth, siHeight, siWidthLarge, siHeightLarge;
	SI32	siSizeX, siSizeY, siTotalY = 0;
	m_pOnlinePortrait->GetRangeOfMercenary( siWidth,		siHeight,		false );
	m_pOnlinePortrait->GetRangeOfMercenary( siWidthLarge,	siHeightLarge,	true );
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		// 선택된 버튼은 선택된 바탕 그림의 좌표를 받는다.
		if( m_siSelectedSlot == i )		{ siSizeX = siWidthLarge;	siSizeY = siHeightLarge; }
		// 그렇지 않다면 일반 바탕 그림의 좌표를 받는다.
		else							{ siSizeX = siWidth;		siSizeY = siHeight; }

		// 구획 조정
		SetRect( &m_stSlotInfo[i].m_Rect, 0, 5 + siTotalY, siSizeX, 5 + siTotalY + siSizeY + 3);
		siTotalY	+= siSizeY + 3;
	}
}

//-----------------------------------------------------------------------------
// Name: Disable_SlotArea()											[private]
// Code: actdoll (2003-12-23)
// Desc: 필드 위에서 용병 초상이 차지하던 영역을 해제한다.
//		즉, 다시 필드로 신호가 가게끔 된다.
//-----------------------------------------------------------------------------
void OnlineMercenary::Disable_SlotArea()
{
	// 일단 해당 슬롯의 필드 에어리어를 모조리 해제한다.
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; ++i )
	{
		if( m_stSlotInfo[i].m_siSlotAreaAccessID < 0 )		continue;
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_stSlotInfo[i].m_siSlotAreaAccessID );
		m_stSlotInfo[i].m_siSlotAreaAccessID	= -1;
	}
}

//-----------------------------------------------------------------------------
// Name: Enable_SlotArea()											[private]
// Code: actdoll (2003-12-23)
// Desc: 필드 위에서 용병 초상이 차지하는 영역을 설정한다.
//		즉, 적용된 영역에 대해서는 필드로 신호가 가지 않는다.
//-----------------------------------------------------------------------------
void OnlineMercenary::Enable_SlotArea()
{
	// 용병 슬롯이 차지하는 영역을 세팅한다.
	SI32	siDBSlotNo, siDBID;
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		siDBSlotNo	= m_stSlotInfo[i].m_siDBSlotNo;
		siDBID		= m_stSlotInfo[i].m_siDBID;
		if( siDBID < 0 )		break;		// 순차적으로 들어가 있기 때문에 해당 슬롯이 비어있을 경우 구획설정을 그만둔다.

//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower					( siDBSlotNo ) );	// 확인사살
//		_ASSERTE( siDBID == m_pMyOnlineWorld->pMyData->GetFollowerParameterKind	( siDBSlotNo ) );	// 확인사살

		m_stSlotInfo[i].m_siSlotAreaAccessID = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_stSlotInfo[i].m_Rect);
	}
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 내용 초기화
//-----------------------------------------------------------------------------
void	OnlineMercenary::Initialize()
{
	// 신용,레벨에 따른 최대 보유가능 용병수(자신포함 최대10명)
	m_siEnableSlotMaxCount	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0)) + 1;

	m_siMouseOverSlot		= -1;
	m_siSelectedSlot		= 0;
	m_siDisableFieldAreaID	= -1;	
	m_siPrevSelectSlot		= -1;

	m_bAction		= FALSE;
	m_bMouseLPush	= FALSE;
	m_bMouseRPush	= FALSE;
	m_bLock			= FALSE;
	m_bTradeMode	= FALSE;
	m_bLoadSpr		= FALSE;
	m_bHighLight	= FALSE;

	Refresh_Slot();
}

//-----------------------------------------------------------------------------
// Name: OnButtonPush()
// Code: actdoll (2003-12-23)
// Desc: 용병창 표시 버튼을 눌렀을 때
//-----------------------------------------------------------------------------
void OnlineMercenary::OnButtonPush()
{
	toggleAction();

	if(m_pMyOnlineWorld->pOnlineTrade->IsActive())	m_bAction = TRUE;

	Refresh_Slot();
}

//-----------------------------------------------------------------------------
// Name: Action()
// Code: actdoll (2003-12-23)
// Desc: 적용받는 구역에 대한 행위의 처리
//-----------------------------------------------------------------------------
void OnlineMercenary::Action()
{
	if(!IsAction())		return;

	if(m_pMyOnlineWorld->pOnlineInventory->IsItemMove())	return;					// 현재 아이템을 옮기는 중이라면 그냥 넘김

	Update_MousePosition();
	Update_MouseBtn		();
	Refresh_Slot		();

	m_bMouseLPush = IpD.LeftPressSwitch;
	m_bMouseRPush = IpD.RightPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2003-12-23)
// Desc: 현재 용병 슬롯에 관련된 그리기를 총괄
//		pSurface	- 그림이 그려질 대상 서피스
//-----------------------------------------------------------------------------
void OnlineMercenary::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!IsAction())		return;

	if(clGrp.LockSurface(pSurface))
	{
		// 모든 슬롯을 검색하여 찍는다.
		for( SI32 i=0; i<m_siEnableSlotMaxCount; i++ )
		{
			UI32	uiDrawFlag	= 0;
			RECT	*prcDest	= &m_stSlotInfo[i].m_Rect;
			SI32	siDBID		= m_stSlotInfo[i].m_siDBID;
			SI32	siHPBase = 0, siHPNow = 0, siMPBase = 0, siMPNow = 0;

			// 해당 슬롯의 정보가 있을 경우 조건에 맞춰서 찍어라
			if( m_stSlotInfo[i].m_siDBID >= 0 )
			{
				MyData	*pMD		= m_pMyOnlineWorld->pMyData;
				SI32	siDBSlotNo	= m_stSlotInfo[i].m_siDBSlotNo;

				// 해당 캐릭터의 라이프, 마나 수치를 받는다.
				siHPNow		= pMD->GetFollowerParameterLife		( siDBSlotNo );
				siHPBase	= pMD->GetFollowerParameterMaxLife	( siDBSlotNo );
				siMPNow		= pMD->GetFollowerParameterMana		( siDBSlotNo );
				siMPBase	= pMD->GetFollowerParameterMaxMana	( siDBSlotNo );

				// 용병 그림에 대한 작업 옵션을 선택한다.
				uiDrawFlag |= DF_FLAG_PORTRAIT_BG;											// 백 그라운드 그려준다.
				if( i == m_siSelectedSlot )		uiDrawFlag |= DF_FLAG_PORTRAIT_LARGE;		// 선택된 용병은 크게 그려준다.
				if( i == m_siMouseOverSlot )	uiDrawFlag |= DF_FLAG_PORTRAIT_HIGHLIGHT;	// 마우스가 올라간 용병은 하이라이트 그려준다.
				if( siHPNow <= 0 )				uiDrawFlag |= DF_FLAG_PORTRAIT_HALFALPHA;	// 죽은 용병은 반투명으로 그려준다.

				if(m_bHighLight)
				{
					SI16 siGeneral = m_pMyOnlineWorld->pIOnlineCharKI->GetGeneral(siDBID);
					if(siGeneral != 1 && i != 0 && siGeneral != 3)					// 주인공, 장수 제외
						uiDrawFlag |= DF_FLAG_PORTRAIT_HIGHLIGHT;	// 거래가능한 모든 용병을 하이라이트 시켜준다.
				}

				// 아이콘 작업 옵션
//				double ret = ( 1.0 - (double)pMD->GetFollowerParameterCurExp( siDBSlotNo ) / (double)pMD->GetFollowerParameterNextExp( siDBSlotNo ) );
//				if( ret <= 0.03 && ret > 0.0 )												uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_IMPEND;	// 레벨업 가까워짐

				// robypark 2004/7/30 14:17
				// 레벨업이 임박하였을 때 보여지 아이콘이 등장이 정확하게 표시가 되지않는 문제점이 있어 수정
				SI32 siExpNext = pMD->GetFollowerParameterNextExp(siDBSlotNo);	// 해당 캐릭터가 다음 레벨로 오르기 위한 필요 경험치
				SI32 siExpPrev = pMD->GetFollowerParameterPrevExp(siDBSlotNo);	// 해당 캐릭터 레벨이 되기 위해 필요한 경험치
				SI32 siExpCur  = pMD->GetFollowerParameterCurExp(siDBSlotNo);	// 해당 캐릭터의 현재 경험치

				// 다음 레벨까지 몇 %가 남았는지 계산
				SI16 ret = 100 - (SI16)(((double)(siExpCur - siExpPrev) / (double)(siExpNext - siExpPrev)) * 100);

				// 10%미만 남았을 때 레벨 업 임박 아이콘을 표시한다.
				if ((ret <= 10) && (ret > 0))
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_IMPEND;	// 레벨업 가까워짐

				if (pMD->GetFollowerParameterBonus( siDBSlotNo ) )
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_LVUP;	// 레벨업

				if (pMD->GetFollowersInterface()->IsItemWeightOverflowed( siDBSlotNo ) )
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_OVERLOAD;// 중량초과
			
				// 마우스가 올라간 용병은 풍선도움말 출력
				if( i == m_siMouseOverSlot )	m_pMyOnlineWorld->pOnlineHelp->SetTextMercenary( prcDest->right + 1, prcDest->top, siDBSlotNo );

			}
			// 아니라면 빈 슬롯만 찍어라
			else
			{
				uiDrawFlag |= DF_FLAG_PORTRAIT_BG;
			}

			// robypark 2004/11/19 16:52
			// 공성전 유닛일 때 주인공 초상화를 변신 전의 초상화로 그리도록 추가
			if (0 == m_stSlotInfo[i].m_siDBSlotNo)	// 주인공 캐릭터
			{
				// 공성전 유닛이라면
				if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// 변신 전의 캐릭터 DB ID 설정
					siDBID = m_pMyOnlineWorld->pMyData->GetPrevCharOnlineKind();
				}
			}

			// 용병을 그려라
			m_pOnlinePortrait->DrawAsMercenary(prcDest->left+4, prcDest->top, siDBID, true, uiDrawFlag, siHPBase, siHPNow, siMPBase, siMPNow );
		}

		clGrp.UnlockSurface(pSurface);
	}

#ifdef	_DEBUG
	HDC		hDC;

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, OPAQUE );
		SetBkColor( hDC, RGB( 240, 240, 240 ) );
		SetTextColor( hDC, RGB( 0, 0, 0 ) );
//		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	
		// 모든 슬롯을 검색하여 찍는다.
		for( SI32 i=0; i<m_siEnableSlotMaxCount; i++ )
		{
			// 해당 슬롯의 정보가 있을 경우 조건에 맞춰서 찍어라
			if( m_stSlotInfo[i].m_siDBID >= 0 )
			{
				char	pszDBAccount[16];
				sprintf( pszDBAccount, "DBSN:%d", m_stSlotInfo[i].m_siDBSlotNo );
				DrawTextEx( hDC, pszDBAccount, -1, &m_stSlotInfo[i].m_Rect, DT_LEFT, NULL );
			}
		}
		SetBkMode( hDC, TRANSPARENT );
		pSurface->ReleaseDC( hDC );
	}
#endif//_DEBUG
}

//-----------------------------------------------------------------------------
// Name: Refresh_Slot()
// Code: actdoll (2003-12-23)
// Desc: 버튼의 배치 및 구획을 재조정한다.
//-----------------------------------------------------------------------------
void OnlineMercenary::Refresh_Slot()
{
	Check_SlotInfo	();		// 슬롯 정보 갱신

	Disable_SlotArea();		// 슬롯 에어리어 꺼주고
	Check_SlotArea	();		// 슬롯 구획 다시 체크하고
	if( IsAction() )	Enable_SlotArea();		// 동작중일 경우 슬롯 에어리어 켜준다.
}

//-----------------------------------------------------------------------------
// Name: DrawSelectPortait()										[private]
// Code: actdoll (2003-12-23)
// Desc: 선택된 용병창을 그려준다.
//		x		- 용병버튼 구획의 좌상단 x
//		y		- 용병버튼 구획의 좌상단 y
//		nKind	- 그려줘야 할 캐릭터 종류
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawSelectPortait(int x, int y, int nKind)
{
/*
	if(nKind == -1)		return;

	clGrp.PutSpriteJin	(	x, 
							y, 
							m_BaseSelectSpr.Header.Xsize, 
							m_BaseSelectSpr.Header.Ysize, 
							m_BaseSelectSpr.Image );

	clGrp.PutSpriteT	(	x + 2, 
							y + 2, 
							m_PortraitSelectSpr.Header.Xsize, 
							m_PortraitSelectSpr.Header.Ysize, 
							&m_PortraitSelectSpr.Image[m_PortraitSelectSpr.Header.Start[nKind]] );

	if(m_bTradeMode)	clGrp.PutSpriteT(	x + 1, 
											y - 1, 
											m_HighLightSelectSpr.Header.Xsize, 
											m_HighLightSelectSpr.Header.Ysize, 
											m_HighLightSelectSpr.Image );
*/
}

//-----------------------------------------------------------------------------
// Name: DrawBar2()													[private]
// Code: actdoll (2003-12-23)
// Desc: 생명/마법력 등을 그려준다.
//		x		- 용병버튼 구획의 좌상단 x
//		y		- 용병버튼 구획의 좌상단 y
//		nSlot	- 해당 바가 표시되는 슬롯 번호
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawBar2(int x, int y, int nSlot)
{
/*
	// 생명/최대치 
	int nCurLife = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife		( (UI08)nSlot );
	int nMaxLife = m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife	( (UI08)nSlot );

	// 마법/최대치
	int nCurMana = m_pMyOnlineWorld->pMyData->GetFollowerParameterMana		( (UI08)nSlot );
	int nMaxMana = m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana	( (UI08)nSlot );

	// 기존의 클리핑 영역을 받아놓는다.
	RECT rt;
	clGrp.GetClipRect(&rt);	

	// 비율 계산(%)
	int nLifeBar, nManaBar;
	nLifeBar = ( nCurLife != 0 ? (float)m_HpSelectSpr.Header.Xsize * ( (float)nCurLife / (float)nMaxLife ) : 0 );
	nManaBar = ( nCurMana != 0 ? (float)m_MpSelectSpr.Header.Xsize * ( (float)nCurMana / (float)nMaxMana ) : 0 );

	// 그림 그리기
	int		left, top;
	if(nLifeBar)	// 생명
	{
		left	= x + 2;
		top		= y + 2	+ m_PortraitSelectSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, left + nLifeBar, top + m_HpSelectSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_HpSelectSpr.Header.Xsize, m_HpSelectSpr.Header.Ysize, m_HpSelectSpr.Image );
	}
	if(nManaBar)	// 마법
	{
		left	= x + 2;
		top		= y + 2 + m_PortraitSelectSpr.Header.Ysize + 1 + m_HpSelectSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, left + nManaBar, top + m_MpSelectSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_MpSelectSpr.Header.Xsize, m_MpSelectSpr.Header.Ysize, m_MpSelectSpr.Image );	
	}

	// 클리핑 영역을 원래대로 돌린다.
	clGrp.SetClipArea(rt.left, rt.top, rt.right, rt.bottom);
*/
}

//-----------------------------------------------------------------------------
// Name: DrawIcon()													[private]
// Code: actdoll (2003-12-23)
// Desc: 해당 슬롯에 아이콘을 표시한다.
//		x		- 용병버튼 구획의 좌상단 x
//		y		- 용병버튼 구획의 좌상단 y
//		nSlot	- 해당 바가 표시되는 슬롯 번호
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawIcon(int x, int y, int nSlot)
{
/*
	int		x1, x2, y1, y2;
	// 해당 슬롯이 선택한 슬롯일 경우
	if( nSlot == m_siSelectedSlot )
	{
		x1 = x + 71;	y1 = y + 2;
		x2 = x + 76;	y2 = y + 24;
	}
	// 그렇지 않다면
	else
	{
		x1 = x + 59;	y1 = y + 2;
		x2 = x + 56;	y2 = y + 24;
	}

	// 올릴 보너스가 남아 있나?
	if( m_pMyOnlineWorld->pMyData->GetFollowerParameterBonus( nSlot ) )
		clGrp.PutSpriteT( x1, y1, m_LvSpr.Header.Xsize, m_LvSpr.Header.Ysize, m_LvSpr.Image );
	
	// 아이템 중량이 넘어갔나?
	if( m_pMyOnlineWorld->pMyData->GetFollowersInterface()->IsItemWeightOverflowed( (UI08)nSlot ) )
		clGrp.PutSpriteT( x2, y2, m_IconHeavySpr.Header.Xsize, m_IconHeavySpr.Header.Ysize, m_IconHeavySpr.Image );
	
	double ret = ( 1.0 - (double)m_pMyOnlineWorld->pMyData->GetFollowerParameterCurExp( (UI08)nSlot ) / (double)m_pMyOnlineWorld->pMyData->GetFollowerParameterNextExp( (UI08)nSlot ) );
	if( ret <= 0.03 && ret > 0.0 )
		clGrp.PutSpriteT( x1, y1, m_Impend_lv_Spr.Header.Xsize, m_Impend_lv_Spr.Header.Ysize, m_Impend_lv_Spr.Image );
*/
}

//-----------------------------------------------------------------------------
// Name: Set_SelectedSlot()
// Code: actdoll (2003-12-23)
// Desc: 주어진 슬롯 번호로 용병 슬롯을 선택한다. 
//		만약 주어진 값이 조건에 어긋나는 경우가 발생한다면, 슬롯 선택은 자동적으로
//		최상위 슬롯으로(주인공) 맞춰진다.
//		siSelectSlotNo	- 선택할 슬롯 번호(0~ON_MAX_FOLLOWER_NUM)
//		bSetAsDBSlotNo	- 주어진 슬롯 번호가 DB에 적재된 슬롯번호로 왔을 경우 true, 
//						현재 용병 슬롯 기준일 경우 false (기본값 true)
//-----------------------------------------------------------------------------
void	OnlineMercenary::Set_SelectedSlot( SI32 siSelectSlotNo, BOOL bSetAsDBSlotNo )
{
	if( siSelectSlotNo < 0 || siSelectSlotNo >= ON_MAX_FOLLOWER_NUM )	siSelectSlotNo = 0;	// 범위 넘어가면 무조건 첫번째

	SI32	siSlotNo;
	if( bSetAsDBSlotNo )		// DB에 적재된 슬롯 번호 형태로 왔을 경우
	{
		siSlotNo = Convert_DBSlotToSlot( siSelectSlotNo );						// 현재 용병창 슬롯을 받아보고
		( siSlotNo < 0 )	?	siSelectSlotNo	= 0	:	siSelectSlotNo	= siSlotNo;
	}
	else						// 용병창 슬롯 번호 형태로 왔을 경우
	{
		if( m_stSlotInfo[siSelectSlotNo].m_siDBID < 0 )				return;
	}

	m_siSelectedSlot		= siSelectSlotNo;
	pGame->m_pEffectDataMgr->PushEffectByKindWhenSelected( m_stSlotInfo[siSelectSlotNo].m_siDBID );									// 목소리 울림

	Refresh_Slot();
}

//-----------------------------------------------------------------------------
// Name: Get_SelectedSlot()
// Code: actdoll (2003-12-23)
// Desc: 선택된 용병 슬롯의 번호를 받아온다.
//		bGetAsDBSlotNo	- 주어진 슬롯 번호를 DB에 적재된 슬롯번호로 받을 경우 true, 
//						현재 용병 슬롯 기준일 경우 false (기본값 true)
//		return			- 해당 슬롯 번호를 리턴한다. 만약 선택된 슬롯이 없다면 -1을 리턴한다.
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Get_SelectedSlot( BOOL bGetAsDBSlotNo )
{
	if( m_siSelectedSlot < 0 )						return -1;
	SI32	siSlotNo;
	if( bGetAsDBSlotNo )		// DB에 적재된 슬롯 번호 형태로 왔을 경우
	{
		siSlotNo = Convert_DBSlotToSlot( m_siSelectedSlot );						// 현재 용병창 슬롯을 받아보고
		if( siSlotNo < 0 )											return -1;
//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower( m_siSelectedSlot ) );							// 확인 사살
	}
	else						// 용병창 슬롯 번호 형태로 왔을 경우
	{
		if( m_stSlotInfo[m_siSelectedSlot].m_siDBID < 0 )				return -1;
//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower( m_stSlotInfo[m_siSelectedSlot].m_siDBSlotNo ) );// 확인 사살
	}

	return siSlotNo;
}

//-----------------------------------------------------------------------------
// Name: GetMouseOverSlotNo()
// Code: actdoll (2003-12-23)
// Desc: 현재 마우스가 올라가 있는 슬롯의 번호를 리턴한다.
//		bGetAsDBSlotNo	- 마우스가 올라간 슬롯의 리턴값을 true이면 DB 슬롯 번호로 받고, false라면 용병창 슬롯 번호로 받는다. (기본값 false)
//		return			- 마우스가 올라가 있는 용병 슬롯의 해당 번째(0~), 아무것도 없다면 -1
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::GetMouseOverSlotNo( BOOL bGetAsDBSlotNo )
{
	// 선택 구획이 없다면 그냥 리턴
	if( m_siMouseOverSlot < 0 )						return -1;

	if( bGetAsDBSlotNo )	// DB슬롯으로 받을 경우
	{
		return Convert_SlotToDBSlot( m_siMouseOverSlot );
	}

	return m_siMouseOverSlot;
}

//-----------------------------------------------------------------------------
// Name: GetMouseOverSlotDBID()
// Code: actdoll (2003-12-23)
// Desc: 현재 마우스가 올라가 있는 슬롯에 해당하는 용병의 DB ID를 리턴한다.
//		return	- 마우스가 올라가 있는 용병 슬롯의 해당 번째(0~), 아무것도 없다면 -1
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::GetMouseOverSlotDBID()
{
	SI32	iSlotNo = GetMouseOverSlotNo();
	if( iSlotNo < 0 )		return -1;

	return m_stSlotInfo[iSlotNo].m_siDBID;
}

//-----------------------------------------------------------------------------
// Name: Lock()
// Code: actdoll (2003-12-23)
// Desc: 전 구역을 비필드 영역으로 설정
//-----------------------------------------------------------------------------
void OnlineMercenary::Lock()
{
	if( m_siDisableFieldAreaID != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siDisableFieldAreaID );
	m_siDisableFieldAreaID	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( 0, 0, 799, 599 );
}

//-----------------------------------------------------------------------------
// Name: UnLock()
// Code: actdoll (2003-12-23)
// Desc: 전 구역을 필드 영역으로 설정
//-----------------------------------------------------------------------------
void OnlineMercenary::UnLock()
{
	if( m_siDisableFieldAreaID != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siDisableFieldAreaID );
	m_siDisableFieldAreaID = -1;
}

//-----------------------------------------------------------------------------
// Name: DrawPortait()
// Code: actdoll (2003-12-23)
// Desc: 일반 용병창을 그려준다.
//		x		- 용병버튼 구획의 좌상단 x
//		y		- 용병버튼 구획의 좌상단 y
//		nKind	- 그려줘야 할 캐릭터 종류
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawPortait_NOUSE(int x, int y, int nKind)
{
/*
	if(nKind == -1)		return;
//	if(!m_bLoadSpr)		LoadImage();

	clGrp.PutSpriteJin	(	x, 
							y, 
							m_BaseSpr.Header.Xsize, 
							m_BaseSpr.Header.Ysize, 
							m_BaseSpr.Image );

	clGrp.PutSpriteT	(	x + 2, 
							y + 2, 
							m_PortraitSpr.Header.Xsize, 
							m_PortraitSpr.Header.Ysize, 
							&m_PortraitSpr.Image[m_PortraitSpr.Header.Start[nKind]] );

	if(m_bTradeMode)	clGrp.PutSpriteT(	x + 1, 
											y - 1, 
											m_HighLightSpr.Header.Xsize, 
											m_HighLightSpr.Header.Ysize, 
											m_HighLightSpr.Image);
*/
}

//-----------------------------------------------------------------------------
// Name: DrawBar1()
// Code: actdoll (2003-12-23)
// Desc: 용병에게서 표시될 각종 바 들을 그려준다.
//		x, y	- 용병버튼 구획의 좌상단 x, y
//		nSlot	- 해당 바가 표시되는 슬롯 번호
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawBar1_NOUSE(int x, int y, int nSlot)
{
/*
	float fCurLife = (float)m_pMyOnlineWorld->pMyData->GetFollowerParameterLife		( (UI08)nSlot );
	float fMaxLife = (float)m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife	( (UI08)nSlot );

	float fCurMana = (float)m_pMyOnlineWorld->pMyData->GetFollowerParameterMana		( (UI08)nSlot );
	float fMaxMana = (float)m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana	( (UI08)nSlot );

	DrawHpMp( x, y, fCurLife / fMaxLife * 100.0f, fCurMana / fMaxMana * 100.0f );
*/
}

//-----------------------------------------------------------------------------
// Name: DrawHpMp()
// Code: actdoll (2003-12-23)
// Desc: 용병의 HP와 MP를 표시한다.
//		x, y		- 용병버튼 구획의 좌상단 x, y
//		fHp, fMp	- 해당 HP, MP의 퍼센테이지(0.0%~100.0%)
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawHpMp_NOUSE(int x, int y, float fHp, float fMp)
{
/*
	// 기존의 클립 영역 보관
	RECT rt;
	clGrp.GetClipRect( &rt );

	// 실제 좌표 구획을 계산
	int		nLifeBar, nManaBar;
	fHp /= 100.0f;
	fMp /= 100.0f;
	nLifeBar = (int)( fHp != 0.0f	?	(float)m_HpSpr.Header.Xsize * fHp	: 0 );
	nManaBar = (int)( fMp != 0.0f	?	(float)m_MpSpr.Header.Xsize * fMp	: 0 );

	int		left, top;
	if(nLifeBar)
	{
		left	= x + 2;
		top		= y + 2	+ m_PortraitSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, left + nLifeBar, top + m_HpSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_HpSpr.Header.Xsize, m_HpSpr.Header.Ysize, m_HpSpr.Image );
	}
	if(nManaBar)
	{
		left	= x + 2;
		top		= y + 2 + m_PortraitSpr.Header.Ysize + 1 + m_HpSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, nManaBar + x + 2, top + m_MpSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_MpSpr.Header.Xsize, m_MpSpr.Header.Ysize, m_MpSpr.Image);	
	}

	clGrp.SetClipArea(rt.left, rt.top, rt.right, rt.bottom);
*/
}

//-----------------------------------------------------------------------------
// Name: Convert_SlotToDBSlot()
// Code: actdoll (2003-12-23)
// Desc: 해당 용병창에 적재된 용병의 DB 슬롯 번호를 얻어온다.
//		siMercenarySlotNo	- 찾고자 하는 용병 슬롯의 번호
//		return				- 해당 슬롯에 속한 용병이 DB에 속해있는 슬롯 번호, 없을 경우 -1 리턴
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Convert_SlotToDBSlot( SI32 siMercenarySlotNo )
{
	if( siMercenarySlotNo < 0 || siMercenarySlotNo >= ON_MAX_FOLLOWER_NUM )	return -1;	// 범위 넘어가 있으면 무시
	return m_stSlotInfo[siMercenarySlotNo].m_siDBSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Convert_DBSlotToSlot()
// Code: actdoll (2003-12-23)
// Desc: 해당 DB 슬롯 번호로 적재된 용병이 속해있는 용병창 슬롯 번호를 얻어온다.
//		siMercenarySlotNo	- 찾고자 하는 용병의 DB 슬롯 번호
//		return				- 해당 DB 슬롯에 속한 용병이 용병창에 속해있는 슬롯 번호, 없을 경우 -1 리턴
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Convert_DBSlotToSlot( SI32 siDBSlotNo )
{
	if( siDBSlotNo < 0 || siDBSlotNo >= ON_MAX_FOLLOWER_NUM )	return -1;	// 범위 넘어가 있으면 무시
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( m_stSlotInfo[i].m_siDBID < 0 )						return -1;	// 해당 슬롯의 ID가 -1로 시작된다면 더이상 없는 것이다.
		if( m_stSlotInfo[i].m_siDBSlotNo == siDBSlotNo )		return i;	// 슬롯을 찾았다면 해당 번호 리턴
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
// Name: Is_ValidSlot()
// Code: actdoll (2003-12-23)
// Desc: 해당 슬롯에 적재된 용병이 있는가?
//		siSlotNo	- 찾고자 하는 용병의 용병창 슬롯 번호. -1일 경우 선택된 슬롯에 대하여 점검
//		return		- 해당 슬롯에 속한 용병이 있을 때는 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineMercenary::Is_ValidSlot( SI32 siSlotNo )
{
	if( siSlotNo >= ON_MAX_FOLLOWER_NUM )					return FALSE;	// 범위 넘어가 있으면 실패
	if( siSlotNo < 0 && m_siSelectedSlot >= 0 )
	{
		if( m_stSlotInfo[m_siSelectedSlot].m_siDBID < 0 )	return FALSE;	// 적재되어 있지 않다면 실패
		return TRUE;
	}
	if( m_stSlotInfo[siSlotNo].m_siDBID < 0 )				return FALSE;	// 적재되어 있지 않다면 실패

	return TRUE;
}