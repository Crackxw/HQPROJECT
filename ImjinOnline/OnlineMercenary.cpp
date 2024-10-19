//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �뺴 ��� Ŭ����
//	File Name		: OnlineMercenary.cpp
//	Birth Date		: 2003.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		�ʵ� �׷��� ��� �κ� �� �뺴â ����� ����ϴ� �κ��Դϴ�.
//		������ �����ʹ� ���������� �ʻ� �����͸� �����Ͽ� �뺴â�� ����ϵ��� �۾��Ǿ����ϴٸ�,
//		�̹� �۾��� ���� ���� �κп��� �ʻ� �׷����� ������ �� �ֵ��� �ʻ� ���� �� �׸��� �κ���
//		�ܰ����� ���½��ϴ�. ���� Ŭ������ ū ��ȭ�� ������ϴ�.
//
//===================================================================================================

//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: ��� �����
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <clGame.h>
#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineHelp.h"			// actdoll (2004/01/02 10:46) : ǳ������ Ŭ���� �߰�
#include "OnlinePortrait.h"		// actdoll (2004/01/02 10:46) : �ʻ� Ŭ���� �߰�
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
// Desc: Ŭ���� ���� ����
//-----------------------------------------------------------------------------
void OnlineMercenary::Init()
{
//	_ASSERTE( m_pOnlinePortrait );

	SI32	siWidth, siHeight;
	m_pOnlinePortrait->GetRangeOfMercenary( siWidth, siHeight );

	// �� ��ư�� ������ �ʱ�ȭ
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
// Desc: Ŭ���� ���� ����
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
// Desc: ���콺 ��ġ�� ���� üũ
//-----------------------------------------------------------------------------
void	OnlineMercenary::Update_MousePosition()
{	
	POINT	pt;
	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;

	// �� ��ȹ���� ���콺 ���� üũ
	m_siMouseOverSlot	= -1;
	for( int i=0; i<m_siEnableSlotMaxCount; i++)
	{
		if( m_stSlotInfo[i].m_siDBID < 0 )					break;		// ���Կ� ����� ������ ���� ��� ����
		if( PtInRect( &m_stSlotInfo[i].m_Rect, pt ) )					// ��ȹ�� ������ ���
		{
			// �ش� ���Թ�ȣ ����
			m_siMouseOverSlot		= i;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Update_MouseBtn()											[private]
// Code: actdoll (2003-12-23)
// Desc: ���콺 ��ư�� ���� üũ
//-----------------------------------------------------------------------------
void OnlineMercenary::Update_MouseBtn()
{	
	if( !m_bMouseLPush && !m_bMouseRPush )		return;
	if( m_siMouseOverSlot < 0 )					return;

	// ��ư�� ���� �����̹Ƿ� ���� ���� ������ ������ Ȯ���Ͽ� ���콺�� �ö� �������� ��ü
//	if( m_siSelectedSlot != m_siMouseOverSlot )
//	{
		Set_SelectedSlot( m_siMouseOverSlot, false );
//	}

	// ������ ���Կ� ���Ͽ� �ܺ� ������ ����
	SI32	siDBID		= m_stSlotInfo[m_siSelectedSlot].m_siDBID;
	SI32	siDBSlotNo	= m_stSlotInfo[m_siSelectedSlot].m_siDBSlotNo;
	m_pMyOnlineWorld->pOnlineTradeBook->SetSelect		( siDBSlotNo );	// �ش� DB������ ���������� �˸�

	// ���ù�ȣ�� Ŭ����ȣ�� ������ ���
	if( m_siPrevSelectSlot == siDBSlotNo )
	{
		// �� ��ư�� ������ ��
		if		( m_bMouseLPush )
		{			
			// ���� �ŷ� �߿��� ���� ĳ����ĭ�� Ŭ���ϴ��� ��� �۾��� ���� �ʴ´�.
			if( m_pMyOnlineWorld->pOnlineTrade->IsActive() == FALSE )
			{

				BOOL	bIsActive = m_pMyOnlineWorld->pOnlineInventory->IsActive();
				
				// �κ��丮�� ��Ȱ��ȭ ���̶�� â ����
				if( !bIsActive )
				{
					m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
					m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
				}			
										
				// �κ��丮â ��۸�
				m_pMyOnlineWorld->pOnlineInventory->SetActive			( !bIsActive );							
			}

			IpD.LeftPressSwitch		= FALSE;
		}
		// ���� ��ư�� ������ ��
		else if	( m_bMouseRPush )
		{
			BOOL	bIsActive = m_pMyOnlineWorld->pOnlineTradeBook->IsAction();
			// �������ͽ� â ��۸�
			m_pMyOnlineWorld->pOnlineTradeBook->SetAction			( !bIsActive, ON_BOOK_STATUS );
			IpD.RightPressSwitch	= FALSE;
		}
	}
	// ���ù�ȣ�� Ŭ����ȣ�� �ٸ� ���
	else
	{
		// �� ��ư�� ������ ��
		if		( m_bMouseLPush )
		{
			// �κ��丮 â ����
			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
			m_pMyOnlineWorld->pOnlineInventory->SetActive		( true );
			IpD.LeftPressSwitch		= FALSE;
		}
		// ���� ��ư�� ������ ��
		else if	( m_bMouseRPush )
		{
			// �κ��丮 â�� �������� ��� ������ �����ش�.
			if( m_pMyOnlineWorld->pOnlineInventory->IsActive() )
			{
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot	( siDBSlotNo );
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox	();
				m_pMyOnlineWorld->pOnlineInventory->SetActive		( true );
			}
			// �������ͽ� â ����
			m_pMyOnlineWorld->pOnlineTradeBook->SetAction		( true, ON_BOOK_STATUS );
			IpD.RightPressSwitch	= FALSE;
		}
	}

	// �ű� ���� ���Թ�ȣ ����
	m_siPrevSelectSlot = siDBSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Check_SlotInfo()											[private]
// Code: actdoll (2003-12-23)
// Desc: �뺴 �ʻ� ���� ��ġ�� ������ ���� ó��.
//-----------------------------------------------------------------------------
void OnlineMercenary::Check_SlotInfo()
{
	SI32	i;

	// ���� ���� �������� �ִ� �뺴 ������ �����Ѵ�.
	m_siEnableSlotMaxCount	= m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum( (SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0) ) + 1;

	// ���� DB�� ����Ǿ� �ִ� �뺴 ������ üũ�Ͽ� ���Ժ��� �뺴�� ä���ִ´�.
	SI32	iSlotNo = 0;
	for( i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		// �ش� DB ���Կ� �뺴�� ������ ���
		if( m_pMyOnlineWorld->pMyData->IsValidFollower( i ) )
		{
			// ���� ����
			m_stSlotInfo[iSlotNo].m_siDBSlotNo	= i;
			m_stSlotInfo[iSlotNo].m_siDBID		= m_pMyOnlineWorld->pMyData->GetFollowerParameterKind( i );
			iSlotNo++;
		}
	}

	// ����ִ� ������ �뺴 ������ ������ ó���Ѵ�.
	for( i=iSlotNo; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		m_stSlotInfo[i].m_siDBSlotNo	= -1;
		m_stSlotInfo[i].m_siDBID		= -1;
	}

	// �����ϴ� �뺴�� ������ �޴´�.(�� �ڽ� ����)
	m_siCurrentSlotCount	= iSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Check_SlotArea()											[private]
// Code: actdoll (2003-12-23)
// Desc: �뺴 �ʻ� ���� ��ġ�� ���� ��ȹ�� ����.
//-----------------------------------------------------------------------------
void OnlineMercenary::Check_SlotArea()
{
	// �뺴 ��ưâ �� ������ �ٽ� ä��
	SI32	siWidth, siHeight, siWidthLarge, siHeightLarge;
	SI32	siSizeX, siSizeY, siTotalY = 0;
	m_pOnlinePortrait->GetRangeOfMercenary( siWidth,		siHeight,		false );
	m_pOnlinePortrait->GetRangeOfMercenary( siWidthLarge,	siHeightLarge,	true );
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		// ���õ� ��ư�� ���õ� ���� �׸��� ��ǥ�� �޴´�.
		if( m_siSelectedSlot == i )		{ siSizeX = siWidthLarge;	siSizeY = siHeightLarge; }
		// �׷��� �ʴٸ� �Ϲ� ���� �׸��� ��ǥ�� �޴´�.
		else							{ siSizeX = siWidth;		siSizeY = siHeight; }

		// ��ȹ ����
		SetRect( &m_stSlotInfo[i].m_Rect, 0, 5 + siTotalY, siSizeX, 5 + siTotalY + siSizeY + 3);
		siTotalY	+= siSizeY + 3;
	}
}

//-----------------------------------------------------------------------------
// Name: Disable_SlotArea()											[private]
// Code: actdoll (2003-12-23)
// Desc: �ʵ� ������ �뺴 �ʻ��� �����ϴ� ������ �����Ѵ�.
//		��, �ٽ� �ʵ�� ��ȣ�� ���Բ� �ȴ�.
//-----------------------------------------------------------------------------
void OnlineMercenary::Disable_SlotArea()
{
	// �ϴ� �ش� ������ �ʵ� ���� ������ �����Ѵ�.
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
// Desc: �ʵ� ������ �뺴 �ʻ��� �����ϴ� ������ �����Ѵ�.
//		��, ����� ������ ���ؼ��� �ʵ�� ��ȣ�� ���� �ʴ´�.
//-----------------------------------------------------------------------------
void OnlineMercenary::Enable_SlotArea()
{
	// �뺴 ������ �����ϴ� ������ �����Ѵ�.
	SI32	siDBSlotNo, siDBID;
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		siDBSlotNo	= m_stSlotInfo[i].m_siDBSlotNo;
		siDBID		= m_stSlotInfo[i].m_siDBID;
		if( siDBID < 0 )		break;		// ���������� �� �ֱ� ������ �ش� ������ ������� ��� ��ȹ������ �׸��д�.

//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower					( siDBSlotNo ) );	// Ȯ�λ��
//		_ASSERTE( siDBID == m_pMyOnlineWorld->pMyData->GetFollowerParameterKind	( siDBSlotNo ) );	// Ȯ�λ��

		m_stSlotInfo[i].m_siSlotAreaAccessID = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(&m_stSlotInfo[i].m_Rect);
	}
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
void	OnlineMercenary::Initialize()
{
	// �ſ�,������ ���� �ִ� �������� �뺴��(�ڽ����� �ִ�10��)
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
// Desc: �뺴â ǥ�� ��ư�� ������ ��
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
// Desc: ����޴� ������ ���� ������ ó��
//-----------------------------------------------------------------------------
void OnlineMercenary::Action()
{
	if(!IsAction())		return;

	if(m_pMyOnlineWorld->pOnlineInventory->IsItemMove())	return;					// ���� �������� �ű�� ���̶�� �׳� �ѱ�

	Update_MousePosition();
	Update_MouseBtn		();
	Refresh_Slot		();

	m_bMouseLPush = IpD.LeftPressSwitch;
	m_bMouseRPush = IpD.RightPressSwitch;
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2003-12-23)
// Desc: ���� �뺴 ���Կ� ���õ� �׸��⸦ �Ѱ�
//		pSurface	- �׸��� �׷��� ��� ���ǽ�
//-----------------------------------------------------------------------------
void OnlineMercenary::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!IsAction())		return;

	if(clGrp.LockSurface(pSurface))
	{
		// ��� ������ �˻��Ͽ� ��´�.
		for( SI32 i=0; i<m_siEnableSlotMaxCount; i++ )
		{
			UI32	uiDrawFlag	= 0;
			RECT	*prcDest	= &m_stSlotInfo[i].m_Rect;
			SI32	siDBID		= m_stSlotInfo[i].m_siDBID;
			SI32	siHPBase = 0, siHPNow = 0, siMPBase = 0, siMPNow = 0;

			// �ش� ������ ������ ���� ��� ���ǿ� ���缭 ����
			if( m_stSlotInfo[i].m_siDBID >= 0 )
			{
				MyData	*pMD		= m_pMyOnlineWorld->pMyData;
				SI32	siDBSlotNo	= m_stSlotInfo[i].m_siDBSlotNo;

				// �ش� ĳ������ ������, ���� ��ġ�� �޴´�.
				siHPNow		= pMD->GetFollowerParameterLife		( siDBSlotNo );
				siHPBase	= pMD->GetFollowerParameterMaxLife	( siDBSlotNo );
				siMPNow		= pMD->GetFollowerParameterMana		( siDBSlotNo );
				siMPBase	= pMD->GetFollowerParameterMaxMana	( siDBSlotNo );

				// �뺴 �׸��� ���� �۾� �ɼ��� �����Ѵ�.
				uiDrawFlag |= DF_FLAG_PORTRAIT_BG;											// �� �׶��� �׷��ش�.
				if( i == m_siSelectedSlot )		uiDrawFlag |= DF_FLAG_PORTRAIT_LARGE;		// ���õ� �뺴�� ũ�� �׷��ش�.
				if( i == m_siMouseOverSlot )	uiDrawFlag |= DF_FLAG_PORTRAIT_HIGHLIGHT;	// ���콺�� �ö� �뺴�� ���̶���Ʈ �׷��ش�.
				if( siHPNow <= 0 )				uiDrawFlag |= DF_FLAG_PORTRAIT_HALFALPHA;	// ���� �뺴�� ���������� �׷��ش�.

				if(m_bHighLight)
				{
					SI16 siGeneral = m_pMyOnlineWorld->pIOnlineCharKI->GetGeneral(siDBID);
					if(siGeneral != 1 && i != 0 && siGeneral != 3)					// ���ΰ�, ��� ����
						uiDrawFlag |= DF_FLAG_PORTRAIT_HIGHLIGHT;	// �ŷ������� ��� �뺴�� ���̶���Ʈ �����ش�.
				}

				// ������ �۾� �ɼ�
//				double ret = ( 1.0 - (double)pMD->GetFollowerParameterCurExp( siDBSlotNo ) / (double)pMD->GetFollowerParameterNextExp( siDBSlotNo ) );
//				if( ret <= 0.03 && ret > 0.0 )												uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_IMPEND;	// ������ �������

				// robypark 2004/7/30 14:17
				// �������� �ӹ��Ͽ��� �� ������ �������� ������ ��Ȯ�ϰ� ǥ�ð� �����ʴ� �������� �־� ����
				SI32 siExpNext = pMD->GetFollowerParameterNextExp(siDBSlotNo);	// �ش� ĳ���Ͱ� ���� ������ ������ ���� �ʿ� ����ġ
				SI32 siExpPrev = pMD->GetFollowerParameterPrevExp(siDBSlotNo);	// �ش� ĳ���� ������ �Ǳ� ���� �ʿ��� ����ġ
				SI32 siExpCur  = pMD->GetFollowerParameterCurExp(siDBSlotNo);	// �ش� ĳ������ ���� ����ġ

				// ���� �������� �� %�� ���Ҵ��� ���
				SI16 ret = 100 - (SI16)(((double)(siExpCur - siExpPrev) / (double)(siExpNext - siExpPrev)) * 100);

				// 10%�̸� ������ �� ���� �� �ӹ� �������� ǥ���Ѵ�.
				if ((ret <= 10) && (ret > 0))
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_IMPEND;	// ������ �������

				if (pMD->GetFollowerParameterBonus( siDBSlotNo ) )
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_LVUP;	// ������

				if (pMD->GetFollowersInterface()->IsItemWeightOverflowed( siDBSlotNo ) )
					uiDrawFlag |= DF_FLAG_PORTRAIT_ICON_OVERLOAD;// �߷��ʰ�
			
				// ���콺�� �ö� �뺴�� ǳ������ ���
				if( i == m_siMouseOverSlot )	m_pMyOnlineWorld->pOnlineHelp->SetTextMercenary( prcDest->right + 1, prcDest->top, siDBSlotNo );

			}
			// �ƴ϶�� �� ���Ը� ����
			else
			{
				uiDrawFlag |= DF_FLAG_PORTRAIT_BG;
			}

			// robypark 2004/11/19 16:52
			// ������ ������ �� ���ΰ� �ʻ�ȭ�� ���� ���� �ʻ�ȭ�� �׸����� �߰�
			if (0 == m_stSlotInfo[i].m_siDBSlotNo)	// ���ΰ� ĳ����
			{
				// ������ �����̶��
				if (m_pMyOnlineWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
				{
					// ���� ���� ĳ���� DB ID ����
					siDBID = m_pMyOnlineWorld->pMyData->GetPrevCharOnlineKind();
				}
			}

			// �뺴�� �׷���
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
	
		// ��� ������ �˻��Ͽ� ��´�.
		for( SI32 i=0; i<m_siEnableSlotMaxCount; i++ )
		{
			// �ش� ������ ������ ���� ��� ���ǿ� ���缭 ����
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
// Desc: ��ư�� ��ġ �� ��ȹ�� �������Ѵ�.
//-----------------------------------------------------------------------------
void OnlineMercenary::Refresh_Slot()
{
	Check_SlotInfo	();		// ���� ���� ����

	Disable_SlotArea();		// ���� ����� ���ְ�
	Check_SlotArea	();		// ���� ��ȹ �ٽ� üũ�ϰ�
	if( IsAction() )	Enable_SlotArea();		// �������� ��� ���� ����� ���ش�.
}

//-----------------------------------------------------------------------------
// Name: DrawSelectPortait()										[private]
// Code: actdoll (2003-12-23)
// Desc: ���õ� �뺴â�� �׷��ش�.
//		x		- �뺴��ư ��ȹ�� �»�� x
//		y		- �뺴��ư ��ȹ�� �»�� y
//		nKind	- �׷���� �� ĳ���� ����
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
// Desc: ����/������ ���� �׷��ش�.
//		x		- �뺴��ư ��ȹ�� �»�� x
//		y		- �뺴��ư ��ȹ�� �»�� y
//		nSlot	- �ش� �ٰ� ǥ�õǴ� ���� ��ȣ
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawBar2(int x, int y, int nSlot)
{
/*
	// ����/�ִ�ġ 
	int nCurLife = m_pMyOnlineWorld->pMyData->GetFollowerParameterLife		( (UI08)nSlot );
	int nMaxLife = m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxLife	( (UI08)nSlot );

	// ����/�ִ�ġ
	int nCurMana = m_pMyOnlineWorld->pMyData->GetFollowerParameterMana		( (UI08)nSlot );
	int nMaxMana = m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxMana	( (UI08)nSlot );

	// ������ Ŭ���� ������ �޾Ƴ��´�.
	RECT rt;
	clGrp.GetClipRect(&rt);	

	// ���� ���(%)
	int nLifeBar, nManaBar;
	nLifeBar = ( nCurLife != 0 ? (float)m_HpSelectSpr.Header.Xsize * ( (float)nCurLife / (float)nMaxLife ) : 0 );
	nManaBar = ( nCurMana != 0 ? (float)m_MpSelectSpr.Header.Xsize * ( (float)nCurMana / (float)nMaxMana ) : 0 );

	// �׸� �׸���
	int		left, top;
	if(nLifeBar)	// ����
	{
		left	= x + 2;
		top		= y + 2	+ m_PortraitSelectSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, left + nLifeBar, top + m_HpSelectSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_HpSelectSpr.Header.Xsize, m_HpSelectSpr.Header.Ysize, m_HpSelectSpr.Image );
	}
	if(nManaBar)	// ����
	{
		left	= x + 2;
		top		= y + 2 + m_PortraitSelectSpr.Header.Ysize + 1 + m_HpSelectSpr.Header.Ysize + 1;
		clGrp.SetClipArea	( left, top, left + nManaBar, top + m_MpSelectSpr.Header.Ysize );
		clGrp.PutSpriteT	( left, top, m_MpSelectSpr.Header.Xsize, m_MpSelectSpr.Header.Ysize, m_MpSelectSpr.Image );	
	}

	// Ŭ���� ������ ������� ������.
	clGrp.SetClipArea(rt.left, rt.top, rt.right, rt.bottom);
*/
}

//-----------------------------------------------------------------------------
// Name: DrawIcon()													[private]
// Code: actdoll (2003-12-23)
// Desc: �ش� ���Կ� �������� ǥ���Ѵ�.
//		x		- �뺴��ư ��ȹ�� �»�� x
//		y		- �뺴��ư ��ȹ�� �»�� y
//		nSlot	- �ش� �ٰ� ǥ�õǴ� ���� ��ȣ
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawIcon(int x, int y, int nSlot)
{
/*
	int		x1, x2, y1, y2;
	// �ش� ������ ������ ������ ���
	if( nSlot == m_siSelectedSlot )
	{
		x1 = x + 71;	y1 = y + 2;
		x2 = x + 76;	y2 = y + 24;
	}
	// �׷��� �ʴٸ�
	else
	{
		x1 = x + 59;	y1 = y + 2;
		x2 = x + 56;	y2 = y + 24;
	}

	// �ø� ���ʽ��� ���� �ֳ�?
	if( m_pMyOnlineWorld->pMyData->GetFollowerParameterBonus( nSlot ) )
		clGrp.PutSpriteT( x1, y1, m_LvSpr.Header.Xsize, m_LvSpr.Header.Ysize, m_LvSpr.Image );
	
	// ������ �߷��� �Ѿ��?
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
// Desc: �־��� ���� ��ȣ�� �뺴 ������ �����Ѵ�. 
//		���� �־��� ���� ���ǿ� ��߳��� ��찡 �߻��Ѵٸ�, ���� ������ �ڵ�������
//		�ֻ��� ��������(���ΰ�) ��������.
//		siSelectSlotNo	- ������ ���� ��ȣ(0~ON_MAX_FOLLOWER_NUM)
//		bSetAsDBSlotNo	- �־��� ���� ��ȣ�� DB�� ����� ���Թ�ȣ�� ���� ��� true, 
//						���� �뺴 ���� ������ ��� false (�⺻�� true)
//-----------------------------------------------------------------------------
void	OnlineMercenary::Set_SelectedSlot( SI32 siSelectSlotNo, BOOL bSetAsDBSlotNo )
{
	if( siSelectSlotNo < 0 || siSelectSlotNo >= ON_MAX_FOLLOWER_NUM )	siSelectSlotNo = 0;	// ���� �Ѿ�� ������ ù��°

	SI32	siSlotNo;
	if( bSetAsDBSlotNo )		// DB�� ����� ���� ��ȣ ���·� ���� ���
	{
		siSlotNo = Convert_DBSlotToSlot( siSelectSlotNo );						// ���� �뺴â ������ �޾ƺ���
		( siSlotNo < 0 )	?	siSelectSlotNo	= 0	:	siSelectSlotNo	= siSlotNo;
	}
	else						// �뺴â ���� ��ȣ ���·� ���� ���
	{
		if( m_stSlotInfo[siSelectSlotNo].m_siDBID < 0 )				return;
	}

	m_siSelectedSlot		= siSelectSlotNo;
	pGame->m_pEffectDataMgr->PushEffectByKindWhenSelected( m_stSlotInfo[siSelectSlotNo].m_siDBID );									// ��Ҹ� �︲

	Refresh_Slot();
}

//-----------------------------------------------------------------------------
// Name: Get_SelectedSlot()
// Code: actdoll (2003-12-23)
// Desc: ���õ� �뺴 ������ ��ȣ�� �޾ƿ´�.
//		bGetAsDBSlotNo	- �־��� ���� ��ȣ�� DB�� ����� ���Թ�ȣ�� ���� ��� true, 
//						���� �뺴 ���� ������ ��� false (�⺻�� true)
//		return			- �ش� ���� ��ȣ�� �����Ѵ�. ���� ���õ� ������ ���ٸ� -1�� �����Ѵ�.
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Get_SelectedSlot( BOOL bGetAsDBSlotNo )
{
	if( m_siSelectedSlot < 0 )						return -1;
	SI32	siSlotNo;
	if( bGetAsDBSlotNo )		// DB�� ����� ���� ��ȣ ���·� ���� ���
	{
		siSlotNo = Convert_DBSlotToSlot( m_siSelectedSlot );						// ���� �뺴â ������ �޾ƺ���
		if( siSlotNo < 0 )											return -1;
//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower( m_siSelectedSlot ) );							// Ȯ�� ���
	}
	else						// �뺴â ���� ��ȣ ���·� ���� ���
	{
		if( m_stSlotInfo[m_siSelectedSlot].m_siDBID < 0 )				return -1;
//		_ASSERTE( m_pMyOnlineWorld->pMyData->IsValidFollower( m_stSlotInfo[m_siSelectedSlot].m_siDBSlotNo ) );// Ȯ�� ���
	}

	return siSlotNo;
}

//-----------------------------------------------------------------------------
// Name: GetMouseOverSlotNo()
// Code: actdoll (2003-12-23)
// Desc: ���� ���콺�� �ö� �ִ� ������ ��ȣ�� �����Ѵ�.
//		bGetAsDBSlotNo	- ���콺�� �ö� ������ ���ϰ��� true�̸� DB ���� ��ȣ�� �ް�, false��� �뺴â ���� ��ȣ�� �޴´�. (�⺻�� false)
//		return			- ���콺�� �ö� �ִ� �뺴 ������ �ش� ��°(0~), �ƹ��͵� ���ٸ� -1
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::GetMouseOverSlotNo( BOOL bGetAsDBSlotNo )
{
	// ���� ��ȹ�� ���ٸ� �׳� ����
	if( m_siMouseOverSlot < 0 )						return -1;

	if( bGetAsDBSlotNo )	// DB�������� ���� ���
	{
		return Convert_SlotToDBSlot( m_siMouseOverSlot );
	}

	return m_siMouseOverSlot;
}

//-----------------------------------------------------------------------------
// Name: GetMouseOverSlotDBID()
// Code: actdoll (2003-12-23)
// Desc: ���� ���콺�� �ö� �ִ� ���Կ� �ش��ϴ� �뺴�� DB ID�� �����Ѵ�.
//		return	- ���콺�� �ö� �ִ� �뺴 ������ �ش� ��°(0~), �ƹ��͵� ���ٸ� -1
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
// Desc: �� ������ ���ʵ� �������� ����
//-----------------------------------------------------------------------------
void OnlineMercenary::Lock()
{
	if( m_siDisableFieldAreaID != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siDisableFieldAreaID );
	m_siDisableFieldAreaID	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( 0, 0, 799, 599 );
}

//-----------------------------------------------------------------------------
// Name: UnLock()
// Code: actdoll (2003-12-23)
// Desc: �� ������ �ʵ� �������� ����
//-----------------------------------------------------------------------------
void OnlineMercenary::UnLock()
{
	if( m_siDisableFieldAreaID != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siDisableFieldAreaID );
	m_siDisableFieldAreaID = -1;
}

//-----------------------------------------------------------------------------
// Name: DrawPortait()
// Code: actdoll (2003-12-23)
// Desc: �Ϲ� �뺴â�� �׷��ش�.
//		x		- �뺴��ư ��ȹ�� �»�� x
//		y		- �뺴��ư ��ȹ�� �»�� y
//		nKind	- �׷���� �� ĳ���� ����
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
// Desc: �뺴���Լ� ǥ�õ� ���� �� ���� �׷��ش�.
//		x, y	- �뺴��ư ��ȹ�� �»�� x, y
//		nSlot	- �ش� �ٰ� ǥ�õǴ� ���� ��ȣ
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
// Desc: �뺴�� HP�� MP�� ǥ���Ѵ�.
//		x, y		- �뺴��ư ��ȹ�� �»�� x, y
//		fHp, fMp	- �ش� HP, MP�� �ۼ�������(0.0%~100.0%)
//-----------------------------------------------------------------------------
void OnlineMercenary::DrawHpMp_NOUSE(int x, int y, float fHp, float fMp)
{
/*
	// ������ Ŭ�� ���� ����
	RECT rt;
	clGrp.GetClipRect( &rt );

	// ���� ��ǥ ��ȹ�� ���
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
// Desc: �ش� �뺴â�� ����� �뺴�� DB ���� ��ȣ�� ���´�.
//		siMercenarySlotNo	- ã���� �ϴ� �뺴 ������ ��ȣ
//		return				- �ش� ���Կ� ���� �뺴�� DB�� �����ִ� ���� ��ȣ, ���� ��� -1 ����
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Convert_SlotToDBSlot( SI32 siMercenarySlotNo )
{
	if( siMercenarySlotNo < 0 || siMercenarySlotNo >= ON_MAX_FOLLOWER_NUM )	return -1;	// ���� �Ѿ ������ ����
	return m_stSlotInfo[siMercenarySlotNo].m_siDBSlotNo;
}

//-----------------------------------------------------------------------------
// Name: Convert_DBSlotToSlot()
// Code: actdoll (2003-12-23)
// Desc: �ش� DB ���� ��ȣ�� ����� �뺴�� �����ִ� �뺴â ���� ��ȣ�� ���´�.
//		siMercenarySlotNo	- ã���� �ϴ� �뺴�� DB ���� ��ȣ
//		return				- �ش� DB ���Կ� ���� �뺴�� �뺴â�� �����ִ� ���� ��ȣ, ���� ��� -1 ����
//-----------------------------------------------------------------------------
SI32	OnlineMercenary::Convert_DBSlotToSlot( SI32 siDBSlotNo )
{
	if( siDBSlotNo < 0 || siDBSlotNo >= ON_MAX_FOLLOWER_NUM )	return -1;	// ���� �Ѿ ������ ����
	for( SI32 i=0; i<ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( m_stSlotInfo[i].m_siDBID < 0 )						return -1;	// �ش� ������ ID�� -1�� ���۵ȴٸ� ���̻� ���� ���̴�.
		if( m_stSlotInfo[i].m_siDBSlotNo == siDBSlotNo )		return i;	// ������ ã�Ҵٸ� �ش� ��ȣ ����
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
// Name: Is_ValidSlot()
// Code: actdoll (2003-12-23)
// Desc: �ش� ���Կ� ����� �뺴�� �ִ°�?
//		siSlotNo	- ã���� �ϴ� �뺴�� �뺴â ���� ��ȣ. -1�� ��� ���õ� ���Կ� ���Ͽ� ����
//		return		- �ش� ���Կ� ���� �뺴�� ���� ���� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineMercenary::Is_ValidSlot( SI32 siSlotNo )
{
	if( siSlotNo >= ON_MAX_FOLLOWER_NUM )					return FALSE;	// ���� �Ѿ ������ ����
	if( siSlotNo < 0 && m_siSelectedSlot >= 0 )
	{
		if( m_stSlotInfo[m_siSelectedSlot].m_siDBID < 0 )	return FALSE;	// ����Ǿ� ���� �ʴٸ� ����
		return TRUE;
	}
	if( m_stSlotInfo[siSlotNo].m_siDBID < 0 )				return FALSE;	// ����Ǿ� ���� �ʴٸ� ����

	return TRUE;
}