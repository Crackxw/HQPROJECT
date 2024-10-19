#include "SOMain.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-07-02 오후 8:10:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltPrivateMarket::Init(SOPlayer *pOwnerPlayer)
{
	m_pOwnerPlayer	=	pOwnerPlayer;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-07-03 오후 7:29:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltPrivateMarket::Init()
{
	SI32	i;

	m_bOpen				=	FALSE;
	
	// 혹시나 있을지 모르는 모든 아이템을 지운다.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점을 연다.
//	수정 일자	:	2002-07-03 오후 7:27:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltPrivateMarket::Open()
{
	m_bOpen	=	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점을 닫는다.
//	수정 일자	:	2002-07-03 오후 7:27:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltPrivateMarket::Close()
{
	SI32	i;	

	// 모든 아이템을 제거한다.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);

	m_bOpen	=	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	유효한 슬롯인지 알아온다.
//	수정 일자	:	2002-07-02 오후 7:48:57 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::IsValidSlot(UI08 uiSlot)
{
	if(uiSlot >= 0 && uiSlot < ON_MAX_BOOTH_ITEM)	return	TRUE;
	else											return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점이 활성화 되었는가?
//	수정 일자	:	2002-07-03 오후 7:28:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::IsOpen()
{
	return m_bOpen;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에 아이템을 추가한다. ( 추가 : 개인 상점에 용병을 추가한다. 후에 건물 거래를 위한 준비도 대략은 해 놓았다.)
//	수정 일자	:	2002-07-02 오후 7:50:31 - 양정모 ( 이상민 : 2003-03-14 오전 11 : 45 )
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//BOOL	cltPrivateMarket::AddItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote)
BOOL	cltPrivateMarket::AddItem( OnBoothItemForSell	*pBoothItem )
{	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 시작 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 슬롯의 ID가 유효한지 검사한다.
	if ( ! IsValidSlot (pBoothItem->uiBoothSlot ) )
	{
		return FALSE;
	}

	// 빠른 제어를 위해 포인터를 저장한다.
	strPMItem *pstPMItem;

	pstPMItem	=	&m_stPMItem[pBoothItem->uiBoothSlot];

	// 해당 좌판은 반드시 비워 있어야 한다.
	//해당 좌판에 item 이나 용병이 있는 지 체크 한다.
	if( pstPMItem->uiItemID || pstPMItem->uiTradeFollowerSlotID )
	{
		return	FALSE;
	}

	ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));

	if ( pBoothItem->uiTradeFollowerID )
	{
		pstPMItem->uiTradeFollowerSlotID = pBoothItem->uiTradeFollowerID;
		pstPMItem->Price			=	pBoothItem->Price;
		strncpy(pstPMItem->szNote, pBoothItem->Desc, ON_MAX_BOOTH_NOTE);
		pstPMItem->siType = LIST_FOLLOWER;
	}
	else if ( pBoothItem->uiItemID )
	{
		pstPMItem->uiFollowerID		=	pBoothItem->uiFollowerID;		
		pstPMItem->Price = pBoothItem->Price;
		strncpy(pstPMItem->szNote, pBoothItem->Desc, ON_MAX_BOOTH_NOTE);
		pstPMItem->uiItemID = pBoothItem->uiItemID;
		pstPMItem->siType = LIST_ITEM;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이 상 민 수정 ( 2003. 3. 14 )
// 용병 거래 부분 추가 끝 부분
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	/*
	strPMItem *pstPMItem;

	// 슬롯의 ID가 유효한지 검사한다.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// 빠른 제어를 위해 포인터를 저장한다.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// 해당 좌판은 반드시 비워 있어야 한다.
	if(pstPMItem->uiItemID != 0)
	{
		return	FALSE;
	}

	// 좌판을 채워 넣는다.
	pstPMItem->uiFollowerID		=	uiFollowerID;
	pstPMItem->Price			=	Price;

	// 설명을 저장한다.
	ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
	trncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

	// 좌판의 아이템을 채워 넣는다.	
	pstPMItem->uiItemID	=	uiItemID;
	*/

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 아이템을 삭제한다.
//	수정 일자	:	2002-07-02 오후 7:58:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::DelItem(UI08 uiSlotID)
{
	// 슬롯의 ID가 유효한지 검사한다.
	if( IsValidSlot(uiSlotID) == FALSE )
	{
		return	FALSE;
	}

	ZeroMemory(&m_stPMItem[uiSlotID], sizeof(strPMItem));	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 아이템을 산다. ( 용병을 구입하는 것은 이 함수를 쓰지 않는다.)
//	수정 일자	:	2002-07-02 오후 8:00:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::BuyItem(UI08 uiSlotID, UI16 uiQuantity, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *psiFromMercenarySlot)
{
	strPMItem		*pstPMItem;		
	MyItemData		*pMyItem;

	// 슬롯의 ID가 유효한지 검사한다.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// 빠른 제어를 위해 포인터를 저장한다.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// 판매하는 아이템인지 검사한다.
	if(pstPMItem->uiItemID == 0)
	{
		return	FALSE;
	}

	// 아이템의 정보를 얻어온다.	
	if((pMyItem	= m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) == NULL)
	{
		return	FALSE;
	}

	// 살려고 하는 수량과 팔려고 하는 수량이 맞는지 검사한다.
	if(pMyItem->uiQuantity < uiQuantity)
	{
		return	FALSE;
	}

	// 아이템을 빼준다.
	if(m_pOwnerPlayer->DelItemNoQuery(pstPMItem->uiFollowerID, pstPMItem->uiItemID, uiQuantity) == FALSE)
	{
		return	FALSE;
	}
	
	// 현재 남은 수량과 가격을 알려준다.
	*puiRemainQuantity	=	pMyItem->uiQuantity;
	*pTotalItemPrice	=	pstPMItem->Price * uiQuantity;
	*psiFromMercenarySlot = pstPMItem->uiFollowerID;
	

	// 현재 남은 수량이 0 이라면 취급하는 아이템 목록에서 지운다.
	if(*puiRemainQuantity == 0)	
	{
		DelItem(uiSlotID);
	}

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 판매하는 아이템을 바꾼다.
//	수정 일자	:	2002-07-02 오후 8:29:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::ChangeItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote, UI08	uiTradeFollowerID)
{
	strPMItem	*pstPMItem;

	// 슬롯의 ID가 유효한지 검사한다.
	if( ! IsValidSlot(uiSlotID) )
	{
		return	FALSE;
	}
	
	// 빠른 제어를 위해 포인터를 저장한다.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	if ( uiItemID )
	{
		pstPMItem->siType = LIST_ITEM;
		// 좌판을 채워 넣는다.
		pstPMItem->uiFollowerID		=	uiFollowerID;
		pstPMItem->Price			=	Price;

		// 설명을 저장한다.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

		// 좌판의 아이템을 채워 넣는다.	
		pstPMItem->uiItemID	=	uiItemID;
	}
	else if ( uiTradeFollowerID )
	{
		pstPMItem->siType = LIST_FOLLOWER;
		// 좌판의 용병을 채워 넣는다.	
		pstPMItem->uiTradeFollowerSlotID = uiTradeFollowerID;
		pstPMItem->Price = Price;

		// 설명을 저장한다.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);
	}
	else
	{
		return FALSE;
	}

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 판매하는 아이템의 정보를 얻어온다.
//	수정 일자	:	2002-07-02 오후 8:57:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
strPMItem*	cltPrivateMarket::GetItem(UI08 uiSlotID)
{
	// 슬롯의 ID가 유효한지 검사한다.
	if( ! IsValidSlot(uiSlotID) )
	{
		return	NULL;
	}
	else
	{
		if( ( m_stPMItem[uiSlotID].uiItemID != 0 ) || ( m_stPMItem[uiSlotID].uiTradeFollowerSlotID != 0 ) )
		{
			return	&m_stPMItem[uiSlotID];
		}		
		else
		{
			return	NULL;
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 판매하는 아이템의 정보를 얻어온다.
//	수정 일자	:	2002-07-11 오후 2:47:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
strPMItem*	cltPrivateMarket::GetItem(UI08 uiSlotID, UI16 uiItemID)
{	
	strPMItem	*pstPMItem;

	if((pstPMItem = GetItem(uiSlotID)) != NULL)
	{
		if(pstPMItem->uiItemID == uiItemID)
			return pstPMItem;
	}
	
	return	NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 현재 판매하는 아이템들을 얻어온다.
//	수정 일자	:	2002-07-03 오후 9:06:08 - 양정모
//  2차 수정    :   2003-03-17 오전 3:00:00 - 이상민 (용병에 대한 정보도 전송해줌)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltPrivateMarket::GetBaseItems(OnBaseBoothItem *pBaseBoothItem, SI08 *psiItemNum)
{
	DWORD		dwUsedSize;
	SI32		i;
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	
	// 두 값을 초기화한다.
	siItemNum	=	-1;
	dwUsedSize	=	0;

	if(IsOpen() == FALSE)
	{
		goto END;
	}

	for(siItemNum = i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		if((pstPMItem = GetItem(i)) != NULL)
		{
			switch( pstPMItem->siType )
			{
			case LIST_ITEM:
				pBaseBoothItem[siItemNum].siType = pstPMItem->siType;
				pBaseBoothItem[siItemNum].uiBoothSlot	=	i;
				pBaseBoothItem[siItemNum].uiProductID		=	pstPMItem->uiItemID;
				break;
			case LIST_FOLLOWER:
				pBaseBoothItem[siItemNum].siType = pstPMItem->siType;
				pBaseBoothItem[siItemNum].uiBoothSlot	=	i;
				pBaseBoothItem[siItemNum].uiProductID	=	m_pOwnerPlayer->IFollower.GetFollowerParameterKind(pstPMItem->uiTradeFollowerSlotID);
				break;
			}
			

			// 유효한 슬롯이다.
			siItemNum++;			// 아이템을 하나 증가시킨다.			
		}
	}

	dwUsedSize	=	sizeof(OnBaseBoothItem) * siItemNum;

END:	
	*psiItemNum	=	siItemNum;

	return	dwUsedSize;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 현재 판매하는 아이템들을 얻어온다.	
//	수정 일자	:	2002-07-05 오전 11:08:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltPrivateMarket::GetItems(OnBoothItemInfo *pBoothItem, SI08 *psiItemNum, SOPlayer* pPlayer, MyItemData *pFollowerItems, UI16 &numFollowerItems)
{
	DWORD		dwUsedSize;
	SI32		i;	
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	MyItemData	*pItemData;
	MyItemData	*pCurFollowerItem	=	pFollowerItems;
	
	// 두 값을 초기화한다.
	siItemNum	=	-1;
	dwUsedSize	=	0;

	numFollowerItems	=	0;	// 참조로 돌려줄 값, Booth에 올라온 모든 용병의 MyItemData들의 개수의 합(소지한 아이템)

	if( IsOpen() == FALSE )
	{
		goto END;
	}

	for( siItemNum = i = 0; i < ON_MAX_BOOTH_ITEM; i++ )
	{
		if( ( pstPMItem = GetItem(i) ) != NULL )
		{
			switch( pstPMItem->siType ) 
			{
			case LIST_ITEM:
				{
					if( ( pItemData = m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) != NULL )
					{
						pBoothItem[siItemNum].uiBoothSlot	=	i;
						pBoothItem[siItemNum].Item.uiItemID	=	pItemData->uiID;
						pBoothItem[siItemNum].Item.uiItemCount =	pItemData->uiQuantity;
						pBoothItem[siItemNum].siPrice	=	pstPMItem->Price;
						pBoothItem[siItemNum].siType	= LIST_ITEM;

						ZeroMemory( &pBoothItem[siItemNum].Follower, sizeof(OnFollowerInfo) );		// 용병 정보 없음

						ZeroMemory( pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note) );
						strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);

						// 유효한 슬롯이다.
						siItemNum++;			// 아이템을 하나 증가시킨다.			
					}
					break;
				}
			case LIST_FOLLOWER:
				{
					pBoothItem[siItemNum].uiBoothSlot	=	i;
					pBoothItem[siItemNum].Item.uiItemID	=	0;
					pBoothItem[siItemNum].Item.uiItemCount = 1;
					pBoothItem[siItemNum].siPrice		=	pstPMItem->Price;
					pBoothItem[siItemNum].siType		= LIST_FOLLOWER;

					CHAR	cFollowerInfoBuf[8192];
					pPlayer->GetFollowerInfo( pstPMItem->uiTradeFollowerSlotID, (OnFollowerInfo*)cFollowerInfoBuf );	// 용병 정보 받아오기
					memcpy( &pBoothItem[siItemNum].Follower, cFollowerInfoBuf, sizeof(OnFollowerInfo) );	// 용병 정보 채우기

					SI32	numItem		=	pBoothItem[siItemNum].Follower.uiItemNum;			// 용병이 갖고 있는 아이템 개수(장착 아이템 외)
					memcpy( pCurFollowerItem, cFollowerInfoBuf + sizeof(OnFollowerInfo), sizeof(MyItemData) * numItem );	// 용병의 아이템 정보 채우기
					pCurFollowerItem	+=	numItem;
					numFollowerItems	+=	numItem;

					ZeroMemory(pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note));
					strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);
					
					siItemNum++;			// 아이템을 하나 증가시킨다.			

					break;
				}
			}	// end of switch
		}	// end of if
	}	// end of for

	dwUsedSize	=	sizeof(OnBoothItemInfo) * siItemNum;

END:	
	*psiItemNum	=	siItemNum;

	return	dwUsedSize;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 판매하고 있는 아이템의 수량을 얻어온다.
//	수정 일자	:	2002-07-05 오후 8:44:30 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL cltPrivateMarket::GetItemQuantity(UI08 uiSlotID, UI16 *puiQuantity)
{
	BOOL			bSuccess;
	strPMItem		*pstPMItem;
	MyItemData		*pItemData;

	if( IsOpen() == FALSE )
	{
		goto	END;
	}
	if( ( pstPMItem = GetItem(uiSlotID) ) == NULL )
	{
		goto	END;
	}
	if( ( pItemData = m_pOwnerPlayer->IFollower.GetItemByID( pstPMItem->uiFollowerID, pstPMItem->uiItemID ) ) == NULL )
	{
		goto	END;
	}

	// 수량을 저장한다.
	*puiQuantity	=	pItemData->uiQuantity;

	bSuccess	=	TRUE;
END:
	return	bSuccess;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	개인 상점에서 아이템을 살 수 있는가?	
//	수정 일자	:	2002-07-05 오후 9:24:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltPrivateMarket::CanBuyItem(UI08 uiSlotID, UI16 uiQuantity, MONEY Money)
{
	strPMItem		*pstPMItem;
	MyItemData		*pItemData;

	if( IsOpen() == FALSE)
	{
		return	FALSE;
	}
	if((pstPMItem = GetItem(uiSlotID)) == NULL)
	{
		return	FALSE;
	}
	if((pItemData = m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) == NULL)
	{
		return	FALSE;
	}
	if(pItemData->uiQuantity < uiQuantity)
	{
		return	FALSE;
	}
	if((uiQuantity * pstPMItem->Price) > Money)
	{
		return	FALSE;
	}

	return	TRUE;
}

// 개인 상점에서 판매하고 있는 아이템의 가격을 알아온다.
// 용병일 경우에는 uiQuantity는 무시된다.
BOOL	cltPrivateMarket::GetTotalPrice( UI08 uiSlotID, UI16 uiQuantity, MONEY *pPrice )
{
	strPMItem		*pstPMItem;
	
	if( IsOpen() == FALSE )									return	FALSE;
	if( ( pstPMItem = GetItem( uiSlotID ) ) == NULL )		return	FALSE;

	switch( pstPMItem->siType )
	{
	case LIST_ITEM:
		*pPrice		=	( pstPMItem->Price * uiQuantity );
		break;

	case LIST_FOLLOWER:	
		// 용병일 경우에는 uiQuantity는 무시된다.
		*pPrice		=	pstPMItem->Price;
		break;


	default:
		return	FALSE;

	}

	return	TRUE;
}
