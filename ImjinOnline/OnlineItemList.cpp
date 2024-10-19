/*****************************************************************************************************************
	파일명	:	OnlineItemList.cpp
	작성자  :	정 진 욱
	담당자	:	이 준 석
	작성일	:	2001.12.21
*****************************************************************************************************************/

#include <GSL.h>

#include	<Mouse.h>

#include	"OnlineWorld.h"
#include	"OnlineVillageSave.h"
#include	"OnlineItemList.h"
#include	"OnlineMegaText.h"
#include	"OnlineFieldArea.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineItem.h"
#include	"OnlineResource.h"
#include	"OnlineMyData.h"
#include	"OnlineFont.h"
#include	"OnlineFollower.h"
#include	"OnlineVillageManager.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineMercenary.h"
///////////////////////////////////
// STL
//#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>

using namespace std;
extern _InputDevice				IpD;



#define ON_LIST_LINE			12


OnlineItemList::OnlineItemList(cltOnlineWorld *pOnlineWorld)
{
	m_uiType			=	0;
	m_pSearchHeader		=	NULL;
	m_pMyOnlineWorld	=	pOnlineWorld;
	m_ShowItemList		=	NULL;
	m_SwordItemList		=	NULL;
	m_uiSearchItemID	=	0;
	m_siVillageNum		=	-1;	
	m_siX = m_siY		=	0;
	m_siScrollPos		=	0;
	m_siShowItemNum		=	0;
	m_siSwordItemNum	=	-1;
	m_siNoField			=	-1;

	m_pWeapon		=	NULL;
	m_pArmor		=	NULL;
	m_pBelt			=	NULL;
	m_pBoots		=	NULL;
	m_pGloves		=	NULL;
	m_pHelmat		=	NULL;
	m_pRing			=	NULL;
	m_pGuardian		=   NULL;
}

OnlineItemList::~OnlineItemList()
{
	Free();
}


bool greater<SortList>::operator() (const SortList& x, const SortList & y) const
{
	if( x.m_siData < y.m_siData )		return true;
	else								return false;
}

// 초기화
VOID	OnlineItemList::Initialize()
{
	m_uiType			=	0;
	m_siScroll			=	0;
	m_pSearchHeader		=	NULL;
	m_uiSearchItemID	=	0;
	m_ShowItemList		=	NULL;
	m_siShowItemNum		=	0;
//	m_SwordItemList		=	NULL;
	m_siSwordItemNum	=	-1;
}

// 초기화
VOID	OnlineItemList::Init()
{
	m_siX = 489;
	m_siY = 86;
	SetRect(&m_rtScroll, m_siX + 261, m_siY + 49, m_siX + 261 + 12, m_siY + 49 + 199);
	m_siScrollPos = m_siY + 49;
	m_siScroll = 0;

	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_iteminfo.Spr", m_SubImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_button.Spr", m_SButImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_scr_back.Spr", m_SScrBackImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_scr_bar.Spr", m_SScrBarImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_scr_up.Spr", m_SScrUpImage);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\sub_scr_down.Spr", m_SScrDownImage);

	m_BExit.Create(m_siX + 12, m_siY + 265, m_SButImage.Header.Xsize, m_SButImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );
	m_BDress.Create(m_siX + 145, m_siY + 265, m_SButImage.Header.Xsize, m_SButImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_DRESS ), BUTTON_PUT_LEFT, TRUE );
	m_BArmor.Create(m_siX + 206, m_siY + 265, m_SButImage.Header.Xsize, m_SButImage.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_ARMOR1 ), BUTTON_PUT_LEFT, TRUE );
	m_BUp.Create(m_siX + 261, m_siY + 37, m_SScrUpImage.Header.Xsize, m_SScrUpImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
	m_BDown.Create(m_siX + 261, m_siY + 246, m_SScrDownImage.Header.Xsize, m_SScrDownImage.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );

	// 임시로 아이템 아디를 저장할 버퍼를 생성을 해준다.
	m_pWeapon		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetWeaponNum() ];
	m_SwordItemList	=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetWeaponNum() ];
	m_pArmor		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetArmorNum() ];
	m_pBelt			=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetBeltNum() ];
	m_pBoots		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetBootsNum() ];
	m_pGloves		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetGlovesNum() ];
	m_pHelmat		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetHelmatNum() ];
	m_pRing			=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetRingNum() ];
	m_pDress		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetDressNum() ];
	m_pGuardian		=	new UI16[ m_pMyOnlineWorld->pItemsInfo->GetGuardianNum() ] ;

	CItemHeader	*pItem;
	SortList	TempData;
	SI16	siTemp1, siTemp2, siTemp3, siTemp4, siTemp5, siTemp6, siTemp7, siTemp8, siTemp9, siTemp10;

	typedef list<SortList> ItemList;
	typedef ItemList::iterator IntVectorIt;
	IntVectorIt start, end, it ;

	ItemList	Weapon;
	ItemList	Armor;
	ItemList	Acessary;
	ItemList	Belt;
	ItemList	Boots;
	ItemList	Gloves;
	ItemList	Helmat;
	ItemList	Ring;
	ItemList	Dress;
	ItemList	Guardian;

	siTemp1 = siTemp2 = siTemp3 = siTemp4 = siTemp5 = siTemp6 = siTemp7 = siTemp8 = siTemp9 = siTemp10 = 0;

	// 장착 부위별 아이템 아디를 저장을 해둔다.
	for( SI16 i=0; i<m_pMyOnlineWorld->pItemsInfo->GetCount(); i++ )
	{
		pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItemHeader(i);

		if( pItem )
		{
			if( pItem->pWeaponHeader )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pWeapon[siTemp1]	=	(UI16)pItem->m_uiID;
				Weapon.push_back( TempData );
				siTemp1++;
			}
			else if(pItem->m_siGroupCode & ON_ITEM_ARMOR)
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pArmor[siTemp2]	=	(UI16)pItem->m_uiID;
				Armor.push_back( TempData );
				siTemp2++;
			}
			else if( pItem->m_siGroupCode & ON_ITEM_GLOVES )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pGloves[siTemp4]	=	(UI16)pItem->m_uiID;
				Gloves.push_back( TempData );
				siTemp4++;
			}
			else if( pItem->m_siGroupCode & ON_ITEM_BELT )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pBelt[siTemp5]	=	(UI16)pItem->m_uiID;
				Belt.push_back( TempData );
				siTemp5++;
			}
			else if( pItem->m_siGroupCode & ON_ITEM_BOOTS )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pBoots[siTemp6]	=	(UI16)pItem->m_uiID;
				Boots.push_back( TempData );
				siTemp6++;
			}
			else if( pItem->m_siGroupCode & ON_ITEM_HELMAT )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pHelmat[siTemp7]	=	(UI16)pItem->m_uiID;
				Helmat.push_back( TempData );
				siTemp7++;
			}

			else if( pItem->m_siGroupCode & ON_ITEM_RING )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pRing[siTemp8]	=	(UI16)pItem->m_uiID;
				Ring.push_back( TempData );
				siTemp8++;
			}

			else if( pItem->m_siGroupCode & ON_ITEM_DRESS )
			{
				TempData.m_uiID		=	(UI16)pItem->m_uiID;
				TempData.m_siData	=	pItem->m_siReqLv;				
				m_pDress[siTemp9]	=	(UI16)pItem->m_uiID;
				Dress.push_back( TempData );
				siTemp9++;
			}
			else if( pItem->m_siGroupCode & ON_ITEM_GUARDIAN)
			{
				TempData.m_uiID			=   (UI16)pItem->m_uiID;
				TempData.m_siData		=	pItem->m_siReqLv;
				m_pGuardian[siTemp10]	=	(UI16)pItem->m_uiID;
				Guardian.push_back(TempData);
				siTemp10++;

			}
		}
	}

	// 소트를 한번 해준다.( 공격력 & 방어력 순으로 )
	// 무기류	
	Weapon.sort( greater<SortList>() );
	Armor.sort( greater<SortList>() );
	Acessary.sort( greater<SortList>() );
	Helmat.sort( greater<SortList>() );
	Boots.sort( greater<SortList>() );
	Gloves.sort( greater<SortList>() );
	Belt.sort( greater<SortList>() );
	Ring.sort( greater<SortList>() );
	Dress.sort( greater<SortList>() );
	Guardian.sort(greater<SortList>() );

	// 칼
	start	= Weapon.begin();
	end		= Weapon.end();
	siTemp1 = 0;
	
	for( it=start; it!=end; it++ )
	{
		m_pWeapon[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 갑옷
	start	= Armor.begin();
	end		= Armor.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pArmor[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 벨트
	start	= Belt.begin();
	end		= Belt.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pBelt[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 신발
	start	= Boots.begin();
	end		= Boots.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pBoots[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 장갑
	start	= Gloves.begin();
	end		= Gloves.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pGloves[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 하이바
	start	= Helmat.begin();
	end		= Helmat.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pHelmat[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 반지
	start	= Ring.begin();
	end		= Ring.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pRing[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 의복
	start	= Dress.begin();
	end		= Dress.end();
	siTemp1 = 0;	

	for( it=start; it!=end; it++ )
	{
		m_pDress[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	// 수호부.
	start   = Guardian.begin();
	end		= Guardian.end();
	siTemp1	= 0;

	for(it = start ; it != end; it++)
	{
		m_pGuardian[siTemp1] = it->m_uiID;
		siTemp1++;
	}

	Weapon.clear();	Helmat.clear();	Gloves.clear();	Boots.clear();	Belt.clear();	Acessary.clear();	Armor.clear();	Ring.clear();	Dress.clear(); Guardian.clear();
}

// 액숀
VOID	OnlineItemList::Action()
{
	if(!IsAction())		return;

	SI16	siHeight, siListCount;
	POINT	pt;
	pt.x	=	IpD.Mouse_X;
	pt.y	=	IpD.Mouse_Y;

	if(m_BExit.Process(m_bMouseDown) == TRUE)
	{
		SetAction(ON_ITEMLIST_TYPE_NOACTION, 0, 0);
		return;
	}

	if(m_BArmor.Process(m_bMouseDown) == TRUE)
	{
		SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_ARMOR, 0);	
	}

	if(m_BDress.Process(m_bMouseDown) == TRUE)
	{
		SetAction(ON_ITEMLIST_TYPE_ITEMLIST, ON_ITEM_DRESS, 0);
	}

	if(m_uiType == ON_ITEMLIST_TYPE_VILLAGELIST)
	{
		siListCount = m_siVillageNum;
	}
	else if(m_uiType == ON_ITEMLIST_TYPE_ITEMLIST)
	{
		if(m_siSwordItemNum != -1)
			m_siShowItemNum = m_siSwordItemNum;

		siListCount = m_siShowItemNum;
	}

	// 스크롤 처리
	if(m_bMouseDown && (siListCount > ON_LIST_LINE))
	{
		if(m_BUp.Process(m_bMouseDown))
		{
			m_siScroll--;
			if(m_siScroll < 0)		m_siScroll = 0;
			
			siHeight		=	(SI16)(m_rtScroll.bottom - m_rtScroll.top - m_SScrBarImage.Header.Ysize);
			m_siScrollPos	=	m_siY + 49 + (SI16)(((float)siHeight / (float)(siListCount - ON_LIST_LINE)) * (float)m_siScroll);
		}		
		else if(m_BDown.Process(m_bMouseDown))
		{
			m_siScroll++;
			if(m_siScroll > (siListCount - ON_LIST_LINE))	m_siScroll = siListCount - ON_LIST_LINE;

			siHeight		=	(SI16)((m_rtScroll.bottom - m_rtScroll.top - m_SScrBarImage.Header.Ysize));
			m_siScrollPos	=	m_siY + 49 + (SI16)(((float)siHeight / (float)(siListCount - ON_LIST_LINE))  * (float)m_siScroll);
		}
		else if(PtInRect(&m_rtScroll, pt) == TRUE)
		{
			siHeight		=	(SI16)(m_rtScroll.bottom - m_rtScroll.top - m_SScrBarImage.Header.Ysize);
			m_siScrollPos	=	pt.y;
			if(m_siScrollPos > (m_rtScroll.bottom - m_SScrBarImage.Header.Ysize))
				m_siScrollPos = m_rtScroll.bottom - m_SScrBarImage.Header.Ysize;

			m_siScroll		=	max(0, (m_siScrollPos - (m_siY + 49)) / ((float)siHeight / (float)(siListCount - ON_LIST_LINE)));
		}
	}
	
	if(IpD.LeftPressSwitch)		m_bMouseDown = TRUE;
	else						m_bMouseDown = FALSE;
}

// 드로우
VOID	OnlineItemList::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_uiType == ON_ITEMLIST_TYPE_NOACTION)		return;

	SI16			i;
	HDC				hDC;	
	CHAR			szTemp[100];
	CItemHeader		*pItem;
	VillageHeader	*pHeader;

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{
		clGrp.PutSpriteT(m_siX, m_siY, m_SubImage.Header.Xsize, m_SubImage.Header.Ysize, m_SubImage.Image);

		// 스크롤
		clGrp.PutSpriteT(m_siX + 261, m_siY + 37, m_SScrBackImage.Header.Xsize, m_SScrBackImage.Header.Ysize, m_SScrBackImage.Image );
		clGrp.PutSpriteT(m_siX + 261, m_siScrollPos, m_SScrBarImage.Header.Xsize, m_SScrBarImage.Header.Ysize, m_SScrBarImage.Image );
		m_BUp.Put(&m_SScrUpImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		m_BDown.Put(&m_SScrDownImage, 0, 1, 0, BUTTON_PUT_NOMOVE);

		m_BExit.Put(&m_SButImage, 0, 2, 1, BUTTON_PUT_NOMOVE);

		if(m_uiType == ON_ITEMLIST_TYPE_ITEMLIST)
		{
			// 갑옷 및 의복일 경우
			if(m_siSwordItemNum == -1)
			{
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_ShowItemList[0]);
				if(pItem)
				{
					if( pItem->m_siGroupCode & ON_ITEM_DRESS || pItem->m_siGroupCode & ON_ITEM_ARMOR)
					{
						m_BArmor.Put( &m_SButImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
						m_BDress.Put( &m_SButImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
					}
				}
			}
		}

		clGrp.UnlockSurface( pSurface );
	}

	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SelectObject( hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
		SetBkMode( hDC, TRANSPARENT );

		if(m_uiType == ON_ITEMLIST_TYPE_VILLAGELIST)
		{
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000501), RGB(10, 10, 10));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000501), RGB(210, 210, 210));

			if(m_pSearchHeader)
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 9,  m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_SELLVILLAGE), RGB( 10, 10, 50 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 8,  m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_SELLVILLAGE), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 96, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_TIME), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 95, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_TIME), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 161, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_SELL), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 160, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_SELL), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 213, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_BUY), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 212, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_BUY), RGB( 210, 210, 210 ) );

				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_uiSearchItemID );

				if( pItem )
				{
					for( i=0 ; i<min( m_siVillageNum, ON_LIST_LINE ) ; i++ )
					{			
						pHeader	=	m_pMyOnlineWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode( m_pSearchHeader[i + m_siScroll].m_uiVillageCode );

						if( pHeader )
						{
							m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 9, m_siY + 41 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pHeader->siVillageName ), RGB( 10, 10, 10 ) );
							m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 8, m_siY + 42 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pHeader->siVillageName ), RGB( 210, 210, 210 ) );
							
							sprintf( szTemp, "%4d.%2d.%2d.%2d", m_pSearchHeader[i + m_siScroll].m_Date.m_siYear, m_pSearchHeader[i + m_siScroll].m_Date.m_siMonth, m_pSearchHeader[i + m_siScroll].m_Date.m_siDay, m_pSearchHeader[i + m_siScroll].m_Date.m_siHour );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 86, m_siY + 41 + ( i * 18 ), 50, 13, szTemp, RGB( 10, 10, 10 ) );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 85, m_siY + 42 + ( i * 18 ), 50, 13, szTemp, RGB( 210, 210, 210 ) );

							sprintf( szTemp, "%d", m_pSearchHeader[i + m_siScroll].m_Data.m_uiBuyValue );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 154, m_siY + 41 + ( i * 18 ), 40, 13, szTemp, RGB( 10, 10, 10 ) );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 153, m_siY + 42 + ( i * 18 ), 40, 13, szTemp, RGB( 210, 210, 210 ) );

							sprintf( szTemp, "%d", m_pSearchHeader[i + m_siScroll].m_Data.m_uiSellValue );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 207, m_siY + 41 + ( i * 18 ), 40, 13, szTemp, RGB( 10, 10, 10 ) );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, m_siX + 206, m_siY + 42 + ( i * 18 ), 40, 13, szTemp, RGB( 210, 210, 210 ) );
						}
					}
				}
			}
		}
		else if(m_uiType == ON_ITEMLIST_TYPE_ITEMLIST)
		{
			COLORREF	rgb;

			if(m_siSwordItemNum != -1)
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_SwordItemList[0]);
			else
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem(m_ShowItemList[0]);

			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 9,  m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_NAME), RGB( 10, 10, 10 ) );
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 8,  m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_NAME), RGB( 210, 210, 210 ) );	

			if(	!pItem )	return;

			if( pItem->pWeaponHeader )							// 무기
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000502), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000502), RGB(210, 210, 210));
				
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 116, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_ATTACK), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 115, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_ATTACK), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_HELMAT )	// 투구
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000503), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000503), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 86, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 85, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 154, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_DEMAGEREG), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 153, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_DEMAGEREG), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_GLOVES )	// 장갑
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000504), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000504), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 123, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_STR), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 122, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_STR), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_ARMOR )		// 갑옷
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000505), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000505), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 81, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 80, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_AP), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 128, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_DEMAGEREG), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 127, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_DEMAGEREG), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 169, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_MAGICREG), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 168, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_MAGICREG), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_DRESS )		// 의복
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000506), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000506), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 101, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ATTACK), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 100, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_ATTACK), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 159, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_DEFFENCE), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 158, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_FOLLOWER_DEFFENCE), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADITION), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADITION), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_BELT )		// 요대
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000507), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000507), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 113, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_MAGICREG), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 112, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_MAGICREG), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_RING )		// 반지
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000508), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000508), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 121, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INT), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 120, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_INT), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_BOOTS )		// 신발
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000509), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(8000509), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 116, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 115, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), RGB( 210, 210, 210 ) );

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			else if( pItem->m_siGroupCode & ON_ITEM_GUARDIAN )		// 수호부
			{
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 3, m_siY + 1, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_GUARDIAN_LIST), RGB(10, 10, 10));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC, m_siX + 2, m_siY + 2, 270, 14, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_GUARDIAN_LIST), RGB(210, 210, 210));

				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 201, m_siY + 24, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 10, 10, 10 ) );
				m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 200, m_siY + 25, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ITEMLIST_REQLV), RGB( 210, 210, 210 ) );
			}
			// 수호부 추가해야 함.
			
			SI16	ix, i = 0;
			UI08	uiWeapon = m_pMyOnlineWorld->pIOnlineCharKI->GetWeaponType( m_pMyOnlineWorld->pMyData->GetFollowerCharID( m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) );
			
			for( ix=0; ix < ON_LIST_LINE; ix++ )
			{	
				// 무기라면
				if(m_siSwordItemNum != -1)
				{
					pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_SwordItemList[ix+m_siScroll] );

					if(pItem)
					{
						if(uiWeapon != ON_ITEM_MISSLE_TYPE_NONE)
						{
							if((uiWeapon == ON_ITEM_MISSLE_TYPE_FREE) || (pItem->pWeaponHeader->siWeaponType == uiWeapon))
							{
								rgb	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) < pItem->m_siReqLv ? RGB( 250, 0, 0 ) : RGB( 210, 210, 210 );
								m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 9, m_siY + 42 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), RGB(10, 10, 10));
								m_pMyOnlineWorld->pOnlineMegaText->DrawText( hDC, m_siX + 8, m_siY + 43 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), rgb );
								
								sprintf( szTemp, "%d", pItem->pWeaponHeader->siMaxDam );
								m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_siX + 138, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
								m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 137, m_siY + 43 + ( i * 18 ), szTemp, rgb );

								sprintf( szTemp, "%d", pItem->m_siReqLv );
								m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 226, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
								m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 225, m_siY + 43 + ( i * 18 ), szTemp, rgb );

								i++;
							}
						}
					}
				}
				else
				{
					// 각각의 테이블에서 아이템 아디를 얻어 아이템 헤더를 구한다.
					pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_ShowItemList[ix+m_siScroll] );
					
					if( pItem )
					{
						if(pItem->m_siGroupCode & ON_ITEM_DRESS)
							rgb	= m_pMyOnlineWorld->pMyData->GetTradeGrade() < pItem->m_siReqLv ? RGB( 250, 0, 0 ) : RGB( 210, 210, 210 );
						else
							rgb	= m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel( m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) < pItem->m_siReqLv ? RGB( 250, 0, 0 ) : RGB( 210, 210, 210 );
						
						// 아이템 이름
						m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 9, m_siY + 42 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), RGB(10, 10, 10));
						m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siX + 8, m_siY + 43 + ( i * 18 ), m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ), rgb);

						// 필요레벨
						sprintf( szTemp, "%d", pItem->m_siReqLv );
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_siX + 229, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
						m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight(hDC, m_siX + 228, m_siY + 43 + ( i * 18 ), szTemp, rgb);

						if( pItem->m_siGroupCode & ON_ITEM_HELMAT )			// 투구(방어력, 타격저항)
						{
							sprintf( szTemp, "%d", pItem->m_siDefense );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 111, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 110, m_siY + 43 + ( i * 18 ), szTemp, rgb );

							sprintf( szTemp, "%d", pItem->m_siDamageRegist );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 171, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 170, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_GLOVES )	// 장갑(힘)
						{
							sprintf( szTemp, "%d", pItem->m_siStrength );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 136, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 135, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_ARMOR )		// 갑옷(방어력, 타격저항, 마법저항)
						{
							sprintf( szTemp, "%d", pItem->m_siDefense );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 105, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 104, m_siY + 43 + ( i * 18 ), szTemp, rgb );

							sprintf( szTemp, "%d", pItem->m_siDamageRegist );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 147, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 146, m_siY + 43 + ( i * 18 ), szTemp, rgb );

							sprintf( szTemp, "%d", pItem->m_siMagicRegist );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 188, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 187, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_DRESS )		// 의복(용공, 용방)
						{
							sprintf( szTemp, "%d", pItem->m_siReqDex );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 116, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 115, m_siY + 43 + ( i * 18 ), szTemp, rgb );

							sprintf( szTemp, "%d", pItem->m_siReqStr );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 175, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 174, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_BELT )		// 요대(마법저항)
						{
							sprintf( szTemp, "%d", pItem->m_siMagicRegist );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 131, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 130, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_RING )		// 반지(지력)
						{
							sprintf( szTemp, "%d", pItem->m_siInt );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 135, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 134, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						else if( pItem->m_siGroupCode & ON_ITEM_BOOTS )		// 신발(민첩)
						{
							sprintf( szTemp, "%d", pItem->m_siDex );
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 135, m_siY + 42 + ( i * 18 ), szTemp, RGB(10, 10, 10));
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextRight( hDC, m_siX + 134, m_siY + 43 + ( i * 18 ), szTemp, rgb );
						}
						//수호부 추가해야함.
						i++;
					}
				}
			}
			if(pItem)
			{
				if( pItem->m_siGroupCode & ON_ITEM_DRESS || pItem->m_siGroupCode & ON_ITEM_ARMOR)
				{
					m_BArmor.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
					m_BDress.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				}
			}
		}

		m_BExit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		m_BUp.Put(hDC);
		m_BDown.Put(hDC);

		pSurface->ReleaseDC(hDC);
	}
}

bool greater<SAVEFILE_SEARCH>::operator() (const SAVEFILE_SEARCH& x, const SAVEFILE_SEARCH & y) const
{
	if( x.m_Data.m_uiBuyValue > y.m_Data.m_uiBuyValue )		return true;
	else													return false;
}

// 아이템 아이디 설정함
VOID	OnlineItemList::SetAction(UI08 uiType, SI32 siAttri, UI32 uiItemID)
{
	SI16 i;
	m_uiType = uiType;

	m_siScrollPos = m_siY + 49;
	m_siScroll = 0;

	if(uiType == ON_ITEMLIST_TYPE_NOACTION)
	{
		Initialize();

		if( m_siNoField != -1 )
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

		m_siNoField	=	-1;
		m_pMyOnlineWorld->m_pMercenary->UnLock();
		m_pMyOnlineWorld->m_pMercenary->NoAction(false);

		return;
	}
	else if(uiType == ON_ITEMLIST_TYPE_VILLAGELIST)
	{
		m_uiSearchItemID = uiItemID;
		m_pSearchHeader = m_pMyOnlineWorld->pOnlineVillageSave->Search(m_uiSearchItemID, m_siVillageNum);
		
		if(!m_pSearchHeader)			Initialize();
		else 
		{
			// 소트를 해준다.
			typedef list<SAVEFILE_SEARCH> ItemList;
			typedef ItemList::iterator IntVectorIt;
			IntVectorIt start, end, it;
			
			ItemList	SortList;
			
			for(i=0; i<m_siVillageNum; i++ )
				SortList.push_back( m_pSearchHeader[i] );	
			
			SortList.sort( greater<SAVEFILE_SEARCH>() );
			
			start =	SortList.begin();
			end	= SortList.end();
			i = 0;
			
			for( it=start; it != end; it++ )
			{
				m_pSearchHeader[i].m_Data			=	it->m_Data;
				m_pSearchHeader[i].m_Date			=	it->m_Date;
				m_pSearchHeader[i].m_uiVillageCode	=	it->m_uiVillageCode;
				i++;
			}
			
			SortList.clear();
		}
	}
	else if(uiType == ON_ITEMLIST_TYPE_ITEMLIST)
	{
		CItemHeader		*pItem;
		
		if( siAttri & ON_ITEM_SWORD )
		{
			m_siSwordItemNum = 0;
			UI08	uiWeapon = m_pMyOnlineWorld->pIOnlineCharKI->GetWeaponType( m_pMyOnlineWorld->pMyData->GetFollowerCharID( m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot() ) );
			for(i=0; i < m_pMyOnlineWorld->pItemsInfo->GetWeaponNum(); i++)
			{
				// 각각의 테이블에서 아이템 아디를 얻어 아이템 헤더를 구한다.
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_pWeapon[i] );
				
				if( pItem )
				{
					// ON_ITEM_MISSLE_TYPE_FREE라고 다찰수있는것이 아니다 주인공은 목탁 염주를 차지못한다.
					if(uiWeapon == ON_ITEM_MISSLE_TYPE_FREE)
					{
						//if (pItem->pWeaponHeader->siWeaponType != ON_ITEM_MISSLE_TYPE_BEADS && pItem->pWeaponHeader->siWeaponType != ON_ITEM_MISSLE_TYPE_BUDDIST)
						//{
						if(m_pMyOnlineWorld->WeaponWearCheck(m_pMyOnlineWorld->pMyData->GetFollowerParameterKind(0), pItem->pWeaponHeader->siWeaponType))
						{
							m_SwordItemList[m_siSwordItemNum] = m_pWeapon[i];
							m_siSwordItemNum++;
						}
						
					}
					
					// 일반 용병들 
					else if(pItem->pWeaponHeader->siWeaponType == uiWeapon)
					{
						m_SwordItemList[m_siSwordItemNum] = m_pWeapon[i];
						m_siSwordItemNum++;
					}
				}
			}
		}
		else if( (siAttri & ON_ITEM_ARMOR) )
		{
			m_ShowItemList		=	m_pArmor;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetArmorNum();
		}
		else if( siAttri & ON_ITEM_HELMAT )
		{
			m_ShowItemList		=	m_pHelmat;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetHelmatNum();
		}
		else if( siAttri & ON_ITEM_GLOVES )
		{
			m_ShowItemList		=	m_pGloves;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetGlovesNum();
		}
		else if( siAttri & ON_ITEM_BELT )
		{
			m_ShowItemList		=	m_pBelt;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetBeltNum();
		}
		else if( siAttri & ON_ITEM_BOOTS )
		{
			m_ShowItemList		=	m_pBoots;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetBootsNum();
		}
		else if( siAttri & ON_ITEM_RING )
		{
			m_ShowItemList		=	m_pRing;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetRingNum();
		}
		else if( siAttri & ON_ITEM_DRESS )
		{
			m_ShowItemList		=	m_pDress;
			m_siShowItemNum	=	m_pMyOnlineWorld->pItemsInfo->GetDressNum();
		}
		else if(siAttri & ON_ITEM_GUARDIAN)
		{
			m_ShowItemList		=   m_pGuardian;
			m_siShowItemNum		=   m_pMyOnlineWorld->pItemsInfo->GetGuardianNum();
		}
	}

	// 노필드 영역 설정
	if( m_siNoField == -1 )
		m_siNoField	=	m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siX, m_siY, m_siX + m_SubImage.Header.Xsize, m_siY + m_SubImage.Header.Ysize + 40);
	
	// 용병창을 클릭을 불가능 하게 만듬
	m_pMyOnlineWorld->m_pMercenary->Lock();
	m_pMyOnlineWorld->m_pMercenary->NoAction(true);
}


BOOL	OnlineItemList::IsAction()
{
	if(m_uiType == ON_ITEMLIST_TYPE_NOACTION)	return FALSE;
	else										return TRUE;	
}

VOID	OnlineItemList::Free()
{
	if(m_pWeapon)
	{
		clGrp.FreeXspr(m_SubImage);
		clGrp.FreeXspr(m_SButImage);
		clGrp.FreeXspr(m_SScrBackImage);
		clGrp.FreeXspr(m_SScrBarImage);
		clGrp.FreeXspr(m_SScrUpImage);
		clGrp.FreeXspr(m_SScrDownImage);
	}

	if( m_pWeapon )			delete []m_pWeapon;	
	if( m_pArmor )			delete []m_pArmor;
	if( m_pBelt )			delete []m_pBelt;
	if( m_pBoots )			delete []m_pBoots;
	if( m_pGloves )			delete []m_pGloves;
	if( m_pHelmat )			delete []m_pHelmat;
	if( m_pRing )			delete []m_pRing;
	if(	m_SwordItemList )	delete []m_SwordItemList;
	if(	m_pDress )			delete []m_pDress;
	if( m_pGuardian)		delete []m_pGuardian; 

	m_pWeapon		=	NULL;
	m_pArmor		=	NULL;
	m_pBelt			=	NULL;
	m_pBoots		=	NULL;
	m_pGloves		=	NULL;
	m_pHelmat		=	NULL;
	m_pRing			=	NULL;
	m_SwordItemList	=	NULL;
	m_pDress		=	NULL;
	m_pGuardian		=   NULL;

	m_ShowItemList = NULL;
	m_SwordItemList = NULL;
}

