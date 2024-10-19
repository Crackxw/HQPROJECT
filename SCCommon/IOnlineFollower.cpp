/************************************************************************************************
	파일명 : IOnlineFollower.cpp
	담당자 : 이윤석
	수  정 : 이성재 (아이템 관련사항 처리하면서 바꿨다.)
	부하들의 모든 정보를 관리한다.
************************************************************************************************/
#include <GSL.h>
#include <limits.h>
#include "OnlineItem.h"
#include "IOnlineFollower.h"
#include "Mmsystem.h"
#include "OnlineChar-Parser.h"




CItem*				OnlineFollower::pCItem;								// Item Class.

//***********************************************************************************************
//	OnlineFollower 구조체
//***********************************************************************************************
OnlineFollower::OnlineFollower()
{
	Init();
}

OnlineFollower::~OnlineFollower()
{
	Init();
}

VOID	OnlineFollower::Init()
{
	// ID를 초기화 한다.
	uiCharID	=	0;	
	// 아이템을 초기화 한다.	
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		Items[i].siPosInInventory	=	i;
		Items[i].uiID				=	0;
		Items[i].uiQuantity			=	0;
	}
	ZeroMemory(Name, sizeof(Name));
	// 무기를 초기화 한다.
	Weapon.Init();

	dwSaveTime = timeGetTime();

	dwTotalItemWeight	=	0;
}

MyItemData*		OnlineFollower::GetItem(UI16 uiItemID)	// 아이템을 얻어온다.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return &Items[i];
	}
	return NULL;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID)	// 아이템을 얻어온다.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return i;
	}
	return -1;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID,UI16 uiQuantity)	// 아이템을 얻어온다.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			if (Items[i].uiQuantity >= uiQuantity)
			return i;
	}
	return -1;
}



MyItemData*		OnlineFollower::GetEmptyItem()
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == 0)
			return &Items[i];
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	주어진 양만큼의 무게가 추가 될수 있는지 검사.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::CheckWeight(UI32 Weight)
{
	return ParametaBox.GetMaxWeight() >= (ParametaBox.GetCurWeight() + (SI32)Weight) ? TRUE:FALSE ;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템 추가, (DB저장안됨, 다른 함수에서 호출하려면 Player 에 있는 AddItem() 써야함.)
//	수정 일자	:	2002-07-12 오후 3:15:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::AddItem(MyItemData* pMyItem, BOOL bTakeOff)
{
	MyItemData		*pItem;		
	DWORD			dwRet  = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;	
	UI16			uiAddedItemID, uiAddedItemQuantity;		

	uiAddedItemID		=	pMyItem->uiID;
	uiAddedItemQuantity	=	pMyItem->uiQuantity;
		
	// 1. 같은 아이템이 있다면, 그곳에 추가한다.
	pItem	=	GetItem(pMyItem->uiID);
	if(pItem)
	{
		pItem->uiQuantity			+=	pMyItem->uiQuantity;	
		
		// 현재 가지고 있는 아이템의 정보를 알려준다.
		pMyItem->uiQuantity			=   pItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.

		// siPosInInventory값이 잘못된 경우 DB에 세팅하지 않도록 FALSE 처리한다.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;
		goto	END;		
	}

	// 2. 같은 아이템이 없다면, 지정한 슬롯에 만들기 시도.
	if(pMyItem->siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
	{
		pItem	=	&Items[pMyItem->siPosInInventory];
		if( pItem->uiID	==	0 )
		{
			pItem->uiID				=	pMyItem->uiID;
			pItem->uiQuantity		=	pMyItem->uiQuantity;			

			dwRet					=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;
			goto	END;
		}
	}

	// 3. 같은 아이템이 없고, 지정한 슬롯에도 넣을수 없다면, 빈슬롯을 찾아 넣는다..
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	pMyItem->uiID;
		pItem->uiQuantity			=	pMyItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.

		// siPosInInventory값이 잘못된 경우 DB에 세팅하지 않도록 FALSE 처리한다.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;
		goto	END;
	}

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// 아이템의 무게를 더해준다.
		// 아이템을 벗는거라면 더해주지 않는다.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가한다.
//	수정 일자	:	2002-10-14 오전 11:47:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::AddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff )
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// 유효한 아이템인지 체크한다.
	if( uiItemID == 0)			goto	END;

	uiAddedItemID		=	uiItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. 같은 아이템이 있다면, 그곳에 추가한다.
	pItem	=	GetItem( uiItemID );
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;	

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. 비어 있는 곳을 찾아 추가한다.
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	uiItemID;
		pItem->uiQuantity			=	uiQuantity;

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;		
		goto	END;
	}	

END:

	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// 아이템의 무게를 더해준다.
		// 아이템을 벗는거라면 더해주지 않는다.
		if(bTakeOff == FALSE)				AddItemWeight( uiAddedItemID, uiAddedItemQuantity );	
	}

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가한다.
//	수정 일자	:	2002-07-12 오후 3:17:35 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::AddItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// 유효한 아이템인지 체크한다.
	if(ItemID == 0)		goto	END;

	uiAddedItemID		=	ItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. 같은 아이템이 있다면, 그곳에 추가한다.
	pItem	=	GetItem(ItemID);
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. 비어 있는 곳을 찾아 추가한다.
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	ItemID;
		pItem->uiQuantity			=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;		
		goto	END;
	}	

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// 아이템의 무게를 더해준다.
		// 아이템을 벗는거라면 더해주지 않는다.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 삭제한다.
//	수정 일자	:	2002-11-19 오전 10:28:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::DelItem(MyItemData* pMyItem, BOOL bPutOn)
{
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	pMyItem->uiID;
	uiDeletedItemQuantity	=	pMyItem->uiQuantity;

	if( ( pItem = GetItem( uiDeletedItemID ) ) == NULL )				goto	END;
	if( pItem->uiQuantity < uiDeletedItemQuantity )						goto	END;
	if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

	pItem->uiQuantity			-=	pMyItem->uiQuantity;
	pMyItem->uiQuantity			=   pItem->uiQuantity;
	pMyItem->siPosInInventory	=   pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.


	if( pItem->uiQuantity == 0 )
	{
		pItem->uiID				=	0;
		pMyItem->uiID			=   0;		
		
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;		
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;	
	}

END:

	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// 아이템을 삭제해준다.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 삭제한다.
//	수정 일자	:	2002-11-19 오전 10:26:55 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::DelItem( UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bPutOn )
{
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	ItemID;
	uiDeletedItemQuantity	=	uiQuantity;

	if( ( pItem = GetItem( ItemID ) ) == NULL )		goto	END;
	if( pItem->uiQuantity < uiQuantity )			goto	END;	

	pItem->uiQuantity		-=	uiQuantity;			
	*psiPosInInventory		=   pItem->siPosInInventory;	// 인벤토리 위치를 알려줘야 DB에 쓴다.

	if(pItem->uiQuantity == 0)
	{
		pItem->uiID				=	0;				
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;	
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;
	}	

END:

	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// 아이템을 삭제해준다.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해당 아이템의 수량을 뺀다.
//	수정 일자	:	2002-07-05 오후 4:16:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::DelItem( UI16 uiItemID, UI16 uiQuantity, BOOL bPutOn )
{	
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	uiItemID;
	uiDeletedItemQuantity	=	uiQuantity;

	if( ( pItem = GetItem( uiItemID ) ) == NULL )			goto	END;
	if( pItem->uiQuantity < uiQuantity )					goto	END;	
	
	pItem->uiQuantity			-=	uiQuantity;			

	if(pItem->uiQuantity == 0)
	{
		pItem->uiID				=	0;				

		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;	
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;	
	}	

END:
	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// 아이템을 삭제해준다.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화 중에 아이템을 장착한다.
//	수정 일자	:	2002-07-12 오후 4:05:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::SetWearItem(UI08 uiLoc, UI16 uiItemID,SI16 siItemLimitTime)
{
	// 기존 장착 정보를 얻어서
	UI16	uiOldItemID	= Weapon.GetWearItem( uiLoc );

	// 무게를 빼주고
	DelItemWeight( uiOldItemID, 1 );

	// 신규 무기정보를 장착하고
	Weapon.SetWearItem(uiLoc, uiItemID,siItemLimitTime);
	
	// 현재 들고 있는 아이템의 무게에 추가해준다.
	AddItemWeight(uiItemID, 1);

	// 캐릭터의 능력치들을 재조정한다.
	ParametaBox.Action(pCItem, Weapon);		

	return	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	아이템 착용
//----------------------------------------------------------------------------------------------------------------
/*MyItemData*	OnlineFollower::WearItem(CItemHeader* pItemHeader, SI08 uiLoc, SI32 siTradeGrade)
{	
	MyItemData		*pItem;	

	if( Weapon.GetWearItem(uiLoc) != 0 )		return NULL;

	if( pItemHeader->m_Attribute & ON_ITEM_ATTACH_BODY_POSSIBLE )
	{
		if( pItemHeader->m_siGroupCode & ON_ITEM_DRESS )
		{			
			if( pItemHeader->m_siReqLv > siTradeGrade)
				return NULL;
		}
		else
		{
			if( pItemHeader->m_siReqLv > ParametaBox.IP.GetLevel() )
				return NULL;
		}

		pItem	=	GetItem( pItemHeader->m_uiID );
		if(pItem)
		{
			if( pItem->uiQuantity >= 1 )
			{
				pItem->uiQuantity--;
				if(pItem->uiQuantity == 0)			pItem->uiID	=	0;
				Weapon.SetWearItem(uiLoc,pItemHeader->m_uiID);
				return pItem;
			}
		}
	}
	return NULL;
}*/

//----------------------------------------------------------------------------------------------------------------
//	설명	:	가지고 있는 아이템도 없고, 착용한 아이템도 없어야 한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::IsEmpty()
{
	// 1. 가지고 있는 아이템이 없어야 한다.
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID != 0)
			return FALSE;
	}

	// 2. 착용하고 있는 아이템도 없어야 한다.
	return Weapon.IsEmpty();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-05 오후 7:23:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// 아이템을 벗는 중이 아닐때에만 무게를 고려한다.
	if(bTakeOff == FALSE)
	{		
		// 추가 시키려는 아이템의 무게를 얻어온다.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// 아이템의 총 무게를 초과하는지 알아온다.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// 같은 아이템이 있는지 찾아본다.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// 인벤토리에 이미 같은 아이템이 있다.
		
		// 아이템 수가 오버플로우 나지 않는지 검사한다.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// 인벤토리에 같은 아이템이 없다.		
	
		// 해당 위치에 아이템을 넣을수 있는지 알아본다.
		if( GetItemByPos(siPosInInventory, &pItemInfo) == FALSE )			return FALSE;		// 인벤토리 위치가 틀리는 등의 이유로 GetItemByPos실패
		if( pItemInfo != NULL)												return	FALSE;		// 아이템이 이미 존재함
	}

	return	TRUE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-12 오후 7:56:42 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// 아이템을 벗는 중이 아닐때에만 무게를 고려한다.
	if(bTakeOff == FALSE)
	{		
		// 추가 시키려는 아이템의 무게를 얻어온다.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// 아이템의 총 무게를 초과하는지 알아온다.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// 같은 아이템이 있는지 찾아본다.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// 인벤토리에 이미 같은 아이템이 있다.
		
		// 아이템 수가 오버플로우 나지 않는지 검사한다.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// 인벤토리에 같은 아이템이 없다.		
	
		// 빈 슬롯이 있는지 검사한다. 빈 슬롯이 없으면 아이템을 추가 시킬 수 없다.
		if(GetEmptyItem() == NULL)											return	FALSE;
		
	}

	return	TRUE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 얻어온다.
//	수정 일자	:	2002-07-05 오후 7:25:53 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::GetItemByPos( SI16 siPosInInventory, MyItemData **ppItemData )
{	
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )				return FALSE;

	if( Items[siPosInInventory].uiID != 0 )			*ppItemData	=	&Items[siPosInInventory];
	else											*ppItemData	=	NULL;

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-07 오전 4:39:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanAddItem(UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum, BOOL bTakeOff)
{
	SI32			siNeedSlot;
	MyItemData		*pItemInfo;
	SI32			i;
	DWORD			dwAddItemWeight;
	CItemHeader		*pItemHeader;
	
	siNeedSlot			=	0;
	dwAddItemWeight		=	0;
	for(i = 0; i < 	uiItemNum; i++)
	{
		// 아이템을 벗는 중이 아닐때에만 무게를 고려한다.
		if(bTakeOff == FALSE)
		{			
			// 추가 시키려는 아이템의 무게를 얻어온다.
			if((pItemHeader = pCItem->GetItem(puiID[i])) == NULL)								return	FALSE;		
			dwAddItemWeight		+=	(pItemHeader->m_uiWeight * puiQuantity[i]);

			// 아이템의 총 무게를 초과하는지 알아온다.
			if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)									return	FALSE;
		}

		if((pItemInfo = GetItem(puiID[i])) == NULL)	
		{
			// 새로운 자리가 필요하다.
			siNeedSlot++;
		}
		else
		{
			// 기존에 이미 아이템이 있다.

			// 기존에 아이템에 이 아이템을 더할 수 있는지 알아온다.
			// 만약 더할 수 없다면 아이템을 추가 시킬 수 없다.
			if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity) < puiQuantity[ i ] )		return	FALSE;					
		}		
	}
	
	// 필요한 자리가 없다면 TRUE를 리턴한다.
	if(siNeedSlot == 0)	return	TRUE;

	// 현재 넣을 수 있는 아이템의 수를 얻어온다.
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == 0)
		{
			siNeedSlot--;

			// 필요한 슬롯을 모두 찾았다.
			if(siNeedSlot == 0)	return	TRUE;
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가시켰을 경우 무게를 초과하는지 알아온다.
//	수정 일자	:	2002-07-12 오후 2:52:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::IsOverflowItemWeight(DWORD dwAddItemWeight)
{

	if( ( GetTotalItemWeight() + dwAddItemWeight ) > GetMaxWeight() )		return	TRUE;
	else																	return	FALSE;

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 무게 초과인지 검사하여 걷기 힘든 상태이면 TRUE를 리턴한다.
//	수정 일자	:	2003-03-19 오전 10:57:23 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::IsOverWeight()
{
	if( GetTotalItemWeight() > GetMaxWeight() )		return	TRUE;
	else											return	FALSE;

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 무게를 추가시켜준다.
//	수정 일자	:	2002-07-12 오후 2:58:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::AddItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// 추가 시키려는 아이템의 무게를 얻어온다.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// 아이템의 무게를 더한다.
	dwTotalItemWeight	+=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 무게를 빼준다.
//	수정 일자	:	2002-07-12 오후 2:58:33 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::DelItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// 추가 시키려는 아이템의 무게를 얻어온다.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// 아이템의 무게를 더한다.
	dwTotalItemWeight	-=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수 초기화.
//	수정 일자	:	2002-07-12 오후 3:27:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineFollower::InitStaticVariable(CItem* pItem)
{
	pCItem			=	pItem;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 이 캐릭터의 전체 아이템의 모무게를 얻어온다.
//	수정 일자	:	2002-07-12 오후 2:45:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::GetTotalItemWeight()
{
	return dwTotalItemWeight;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 캐릭터가 최고 들 수 있는 무게.
//	수정 일자	:	2002-07-12 오후 4:17:11 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	OnlineFollower::GetMaxWeight()
{
	return ParametaBox.GetMaxWeight();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 캐릭터가 최고 가질 수 있는 아이템의 수량을 얻어온다.
//	수정 일자	:	2002-07-12 오후 5:52:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	OnlineFollower::GetItemQuantityToTake(UI16 uiItemID)
{
	MyItemData		*pItem;			
	UI16			uiItemQuantity;
	CItemHeader		*pItemHeader;
	
	// 아이템의 정보를 얻어온다.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)	return	0;		

	// 현재 무게가 오버 플로우 되었는지 알아온다.
	if(IsItemWeightOverflowed() == TRUE)					return	0;
	
	if((pItem = GetItem(uiItemID)) != NULL)
	{	
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM - pItem->uiQuantity;
	}
	else
	{
		// 현재 비어있는 슬롯을 찾는다.
		if((pItem = GetEmptyItem()) == NULL)	return 0;
		
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM;
	}
			

	// 무게를 고려해서 들 수 있는 아이템의 무게를 체크한다.
	return UI16(min((GetMaxWeight() - GetTotalItemWeight()) / pItemHeader->m_uiWeight, uiItemQuantity));	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 캐릭터가 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.
//	수정 일자	:	2002-07-12 오후 8:19:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::IsItemWeightOverflowed()
{
	if(GetMaxWeight() < GetTotalItemWeight())	return	TRUE;
	else										return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착하고 있는 아이템을 벗는다.
//	수정 일자	:	2002-07-12 오후 9:17:00 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::TakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// 인벤토리에 아이템을 넣어준다.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	1;
	ItemData.siPosInInventory	=	siPosInInventory;
	if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// 아이템을 벗는다.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// 인벤토리에 넣을 정보를 얻어온다.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// 캐릭터의 능력치들을 재조정한다.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

BOOL	OnlineFollower::DisAppearItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// 인벤토리에 아이템을 넣어준다.
	ItemData.uiID				=	0;
	ItemData.uiQuantity			=	0;
	ItemData.siPosInInventory	=	0;
	//if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// 아이템을 벗는다.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// 인벤토리에 넣을 정보를 얻어온다.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// 캐릭터의 능력치들을 재조정한다.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착한다.
//	수정 일자	:	2002-07-13 오후 2:17:37 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::PutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{	
	MyItemData			DeleteItemInfo;	

	// 인벤토리에서 아이템을 뺀다.
	DeleteItemInfo.uiID					=	uiItemID;
	DeleteItemInfo.uiQuantity			=	1;
	DeleteItemInfo.siPosInInventory		=	siPosInInventory;
	if( DelItem(&DeleteItemInfo, TRUE) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )		return FALSE;

	// 아이템을 장착 시킨다.
	SI32	siMaxLimitDate	= pCItem->GetItemUsingDay(uiItemID);
	( siMaxLimitDate > 0 )	?	siMaxLimitDate++ : siMaxLimitDate = 0;
	Weapon.SetWearItem( uiWearLoc, uiItemID, siMaxLimitDate );
	
	// 인벤토리에 넣을 정보를 얻어온다.
	if(pMyItemData != NULL)
		*pMyItemData			=	DeleteItemInfo;

	// 캐릭터의 능력치들을 재조정한다.
	ParametaBox.Action(pCItem, Weapon);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetWearItem()
// Code: actdoll (2004-07-19)
// Desc: 현재 이 캐릭터가 특별한 위치에 장착하고 있는 아이템의 정보를 얻는다.
//	uiWearLoc			- 장착 위치 인덱스
//	bIsTimerItem		- 장착 아이템이 시간제 아이템이면 true 리턴
//	siRemainDate		- 시간제 아이템이 장착되었을 경우 남은 사용기일 리턴
//	return				- 장착한 아이템의 ID. 없다면 0을 리턴
//-----------------------------------------------------------------------------
UI16	OnlineFollower::GetWearItem( UI08 uiWearLoc, BOOL &bIsTimerItem, SI16 &siRemainDate )
{
	bIsTimerItem	= false;
	siRemainDate	= 0;
	UI16 uiItemID = Weapon.GetWearItem( uiWearLoc );
	if( uiItemID == 0 )		return uiItemID;

	// 이 아이템이 시간제 아이템인지 판단
	SI16	siTemp;
	CItemHeader *pItemHeader = pCItem->GetItemInfo( uiItemID, siTemp );
	if( pItemHeader->m_bTimeLimit )		// 시간제 아이템일 경우
	{
		bIsTimerItem	= (BOOL)pItemHeader->m_bTimeLimit;
		siRemainDate	= Weapon.GetWearItemLimitTime( uiWearLoc );
	}

	return	uiItemID;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 장착된 아이템들의 리스트를 얻어온다.
//	수정 일자	:	2002-07-13 오후 2:56:26 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM])
{
	Weapon.GetWearItems(puiWearItems);
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
//	수정 일자	:	2002-07-15 오후 4:00:36 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanPutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	MyItemData			*pItemDataInInventory;
	CItemHeader			*pItem;	

	// 실제 아이템을 가지고 있는지 확인한다.
	if( GetItemByPos(siPosInInventory, &pItemDataInInventory) )	
	{
		if( pItemDataInInventory == NULL )
		{
			ErrorCode = NOFIND_ERROR;
			return	FALSE;
		}
	}
	// 같은 아이템을 가지고 있는지 확인한다.
	if(pItemDataInInventory->uiID != uiItemID)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// 장착하려는 곳에 아이템이 비어있는지 확인한다.
	if(Weapon.GetWearItem(uiWearLoc) != 0)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// 아이템 정보가 있는지 확인
	if((pItem = pCItem->GetItem(uiItemID)) == NULL)						
	{
		ErrorCode = NOFIND_ERROR;
		return  FALSE;	
	}
	// 아이템 종류가 수호부인지 확인 - 주인공만 착용가능
	if(pItem->m_siGroupCode & ON_ITEM_GUARDIAN)
	{
		if((uiCharID  & 255) != 'C')
		{
			ErrorCode = CANTWEARGAURDIAN_ERROR;
			return FALSE;
		}
	}
	// 아이템 종류가 시간제 아이템인지 확인 - 주인공만 착용 가능
	// actdoll (2005/02/01 17:00) : 
	//	위에 똑같은 구문이 있는데 이건 왜 여기 넣었을까? 일단 좀 지나고 문제 없으면 삭제
	if(pCItem->IsTimeLimitItem(uiItemID))
	{
		if((uiCharID & 255) != 'C')
		{
			ErrorCode = CANTWEARTIMELIMITITEM_ERROR;
			return FALSE;
		}
	}
	
	// 아이템 종류가 드레스인지 확인 - 주인공만 착용가능하며 신용도 필요
	if(pItem->m_siGroupCode & ON_ITEM_DRESS)
	{	
		if ((uiCharID & 255) == 'C')
		{			
			if(pItem->m_siReqLv > siTradeGrade)
			{
				ErrorCode = CREDIT_ERROR;
				return	FALSE;
			}
		}
		else
		{
			ErrorCode = CANTPUTONDRESS_ERROR;
			return FALSE;
		}
	}
	else
	{
		// 아이템을 장착할 수 있는 레벨이 되는지 검사한다.
		if(pItem->m_siReqLv > ParametaBox.IP.GetLevel())
		{
			ErrorCode = LEVEL_ERROR;
			return	FALSE;
		}
	}

	// 무기일 경우면 (장착무기 종류는 앞에서 다체크 되므로 주인공일 경우만 검색한다.)
	if(pItem->pWeaponHeader && (uiCharID & 255) == 'C')
	{
		switch(pItem->pWeaponHeader->siWeaponType)
		{
		case ON_ITEM_MISSLE_TYPE_FAN:			// 일본 여자만 부채를 장비 할 수 이따.			
			// 일본 여자만 TRUE
			if (uiCharID == 9027) return TRUE;
			else
			{
				ErrorCode = FAN_ERROR;
				return FALSE;
			}			
			break;			
		case ON_ITEM_MISSLE_TYPE_DOUBLESTICK:	// 조선 여자만 쌍절곤를 장비 할 수 이따.								
			if( uiCharID == 323 ) return TRUE;
			else {			
				ErrorCode = DOUBLESTICK_ERROR;
				return FALSE;
			}
			break;
									
		case ON_ITEM_MISSLE_TYPE_SPINSTONE:		// 조선 남자만 사냥돌를 장비 할 수 이따.								
			if( uiCharID == 67 ) return TRUE;
			else {			
				ErrorCode = SPINSTONE_ERROR;
				return FALSE;
			}		
			break;									
		case ON_ITEM_MISSLE_TYPE_SCYTHE:		// 일본 남자만 낫를 장비 할 수 이따.							
			if( uiCharID == 8771 )	return TRUE;
			else {				
				ErrorCode = SCYTHE_ERROR;
				return FALSE;
			}
			break;	
		case ON_ITEM_MISSLE_TYPE_BEADS:         // 대만 남 대만 녀는 염주를 찰수없다.
			/*if( uiCharID == 10307 || uiCharID == 10563) return FALSE;

			break;*/
		case ON_ITEM_MISSLE_TYPE_BUDDIST:		// 주인공은 목탁 염주는 찰 수 없다.
			return FALSE;
			break;
		case ON_ITEM_MISSLE_TYPE_FIREGUN:		// 화포는 주인공이 찰수 없다.
		case ON_ITEM_MISSLE_TYPE_STICK:
		case ON_ITEM_MISSLE_TYPE_DSWORD:
		case ON_ITEM_MISSLE_TYPE_PEARL:
			return FALSE;
			break;

									
		case ON_ITEM_MISSLE_TYPE_FREE:			// 모든 케릭터가 찰 수 이따.
			break;
		}
	}	
	return	TRUE;
}
	

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
//	수정 일자	:	2002-07-15 오후 4:01:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanTakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	// 해당 용병이 실제 벗으려고 하는 아이템이 장착하고 있는 아이템과 같은지 비교한다.
	if(Weapon.GetWearItem(uiWearLoc) != uiItemID)						return	FALSE;

	// 벗으려고 하는 아이템을 인벤토리에 넣을 수 있는지 검사한다.
	if(CanAddItem(uiItemID, 1, siPosInInventory, TRUE) == FALSE)		return	FALSE;

	if(pCItem->IsTimeLimitItem(uiItemID) == TRUE)						return  FALSE;

	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 가지고 있는지 검사한다.
//	수정 일자	:	2002-10-14 오전 10:27:28 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	MyItemData	*pMyItemData;

	if( ( pMyItemData = GetItem( uiItemID ) ) == NULL )			return	FALSE;

	if( pMyItemData->uiQuantity < uiItemQuantity )				return	FALSE;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 옮길 수 있는지 검사한다.
//	수정 일자	:	2002-11-12 오전 10:48:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanMoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory )
{
	// 아이템이 있는지 검사한다.
	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )			return	FALSE;
		
	// 아이템을 옮기려는 자리가 비어 있는지 검사한다.
	if( IsEmptyItemInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 올긴다.
//	수정 일자	:	2002-11-12 오전 10:57:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::MoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 *psiPosInInventory )
{
	MyItemData			ItemData;

	if( CanMoveItem( uiItemID, uiItemQuantity, *psiPosInInventory ) == FALSE )			return	FALSE;

	// 아이템을 뺀다.
	if( DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	return	FALSE;
	
	// 아이템을 더한다.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	uiItemQuantity;
	ItemData.siPosInInventory	=	*psiPosInInventory;
	if( AddItem(&ItemData) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )					return	FALSE;

	*psiPosInInventory			=	ItemData.siPosInInventory;

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리내의 아이템을 넣는 위치가 비웠는지 알아온다.
//	수정 일자	:	2002-11-12 오전 10:52:19 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::IsEmptyItemInInventory( SI16 siPosInInventory )
{
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	if( Items[ siPosInInventory ].uiID == 0 )						return	TRUE;
	else															return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리내에서 아이템을 넣는 위치가 유효한지 검사한다.
//	수정 일자	:	2002-11-12 오전 10:56:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::IsValidItemPosInInventory( SI16 siPosInInventory )
{
	if( ( siPosInInventory >= 0 ) && ( siPosInInventory < ON_MYDATA_MAX_ITEM_NUM ) )	return	TRUE;
	else																				return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착중인 무기가 업그레이드 가능한지 알아온다(고급무기인가?)
//	수정 일자	:	2003-3-31 오전 17:33 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	OnlineFollower::CanWeaponBeUpgraded()
{
	return pCItem->IsUpgradable( Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM) );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착중인 무기를 특정 무기로 업그레이드해준다.
//	수정 일자	:	2003-3-31 오전 17:35 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineFollower::UpgradeWeapon( UI16 uiItemID )
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, uiItemID ,0);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착중인 무기를 깨뜨린다. (삭제)
//	수정 일자	:	2003-3-31 오전 17:38 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineFollower::BrakeWeapon()
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, 0 ,0);
}



//***********************************************************************************************
//	IOnlineFollower 구조체
//***********************************************************************************************

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::IOnlineFollower()
{	
	Init();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::~IOnlineFollower()
{	
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	변수들의 초기화를 수행하고 필요한 메모리를 할당 받는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Init()
{	
	SI32	i;

	// 부하들의 데이타를 초기화 한다.
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		Followers[i].Init();
	} 
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이 클래스에서 사용했던 메모리를 전부 해제한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Free()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템 버퍼 중에 비어있는 버퍼를 찾는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFollower*		IOnlineFollower::GetFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// 실제 그 슬롯에 부하가 있는지 본다.
		if(Followers[uiFollowerSlot].uiCharID == 0)
			return NULL;
		else
			return &Followers[uiFollowerSlot];
	}
	else
		return NULL;
}

OnlineFollower*		IOnlineFollower::GetHero()
{
	return Followers;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템 버퍼 중에 비어있는 버퍼를 찾는다.
//	수정 일자	:	2002-07-03 오후 3:34:22 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	IOnlineFollower::FindEmptyItemBuffer(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;
	SI32				i;
//	MyItemData			*pItemData;

	pFollower	=	GetFollower(uiFollowerSlot);
	if(pFollower)
	{
		for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		{
			if(pFollower->Items[i].uiID == 0)
				return i;
		}
	}
	return -1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템 버퍼 중에 같은 아이템이 들어있는 버퍼를 찾는다.
//	수정 일자	:	2002-07-03 오후 3:34:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	IOnlineFollower::FindSameItemBuffer(UI08 uiFollowerSlot, UI16 ItemID)
{
	SI32	i;
	
	if(IsValidFollower(uiFollowerSlot) == FALSE)
		return 0;

	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Followers[uiFollowerSlot].Items[i].uiID == ItemID)
			return i;
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	무게가 들어갈 수 있는지 확인한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::CheckWeight(UI08 uiFollowerSlot, UI32 Weight)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);
	if(pFollower)
	{
		return pFollower->CheckWeight(Weight);
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	무게초과인 용병이 한명이라도 존재하여 걷기 힘든 상태이면 TRUE를 리턴한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::IsOverWeight()
{
	SI32		i;
	BOOL		bOverWeight		=	FALSE;

	for( i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			if( Followers[i].IsOverWeight() )
			{
				bOverWeight	=	TRUE;
				break;
			}
		}
	}
	return bOverWeight;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템이 있는지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::CheckItem(UI16 uiItemID, SI32 siCount)
{
	SI32	TempCount;
	SI32	TempCount1;
	
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				if(Followers[TempCount].Items[TempCount1].uiID == uiItemID)
				{
					siCount -= Followers[TempCount].Items[TempCount1].uiQuantity;
					if(siCount <= 0) return TRUE;
				}
			}

			/*
			// 장비한 Item
			siCount -= Followers[TempCount].Weapon.CheckItemCount(uiItemID);
			if(siCount <= 0) return TRUE;
			*/
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	가지고 있는 Item의 수를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineFollower::GetItemCount(UI16 uiItemID)
{
	SI32	TempCount;
	SI32	TempCount1;
	SI32	TempItemCount;
	
	TempItemCount = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				if(Followers[TempCount].Items[TempCount1].uiID == uiItemID)
				{
					TempItemCount += Followers[TempCount].Items[TempCount1].uiQuantity;
				}
			}

			// 장비한 Item
			TempItemCount += Followers[TempCount].Weapon.CheckItemCount(uiItemID);
		}
	}

	return TempItemCount;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에 추가한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;

	// 우선 기존의 같은 아이템이 있는지 알아온다.
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->AddItem(pItem);
	}
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 아이템 리스트에 추가한다.
//	수정 일자	:	2002-10-14 오전 11:44:29 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower(uiFollowerSlot) ) == NULL )	return	ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	
	return lpFollower->AddItem( uiItemID, uiQuantity );	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 아이템 리스트에 추가한다.
//	수정 일자	:	2002-07-03 오전 10:29:51 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory)
{
	MyItemData			ItemData;

	ItemData.uiID				=	uiItemID;
	ItemData.siPosInInventory	=	siPosInInventory;
	ItemData.uiQuantity			=	uiQuantity;

	return	AddItem(uiFollowerSlot, &ItemData);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 아이템 리스트에 추가한다.
//	수정 일자	:	2002-07-07 오전 11:28:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory)
{
	OnlineFollower*		lpFollower;

	// 우선 기존의 같은 아이템이 있는지 알아온다.
	lpFollower = GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->AddItem(uiItemID, uiQuantity, psiPosInInventory);
	
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에서 삭제한다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(pItem);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 아이템 리스트에서 삭제한다.				
//	수정 일자	:	2002-07-05 오후 4:15:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(uiItemID, uiQuantity);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 아이템 리스트에서 삭제한다.
//	수정 일자	:	2002-07-07 오전 11:36:23 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(uiItemID, uiQuantity, psiPosInInventory);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에서 얻어온다
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(lpFollower->Items[uiIndex].uiID != 0)
		{
			memcpy(pItem, &lpFollower->Items[uiIndex], sizeof(MyItemData));
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에서 얻어온다
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	IOnlineFollower::GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory<ON_MYDATA_MAX_ITEM_NUM)
		{
			// 1. 우선 주어진 곳에 아이템이 있다면 그걸 리턴한다.
			if(lpFollower->Items[siPosInInventory].uiID	==	uiItemID )
			{
				return &lpFollower->Items[siPosInInventory];
			}
		}
		// 2. 그 위치에 그 아이템이 없다면, 같은 아이템을 찾아본다.
		return lpFollower->GetItem(uiItemID);
	}
	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 ID를 사용하여 아이템을 얻어온다.
//	수정 일자	:	2002-07-03 오전 10:15:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
MyItemData*	IOnlineFollower::GetItemByID(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->GetItem(uiItemID);		
	else
		return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템의 ID를 사용하여 아이템을 얻어온다.
//	수정 일자	:	2002-07-03 오전 10:15:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::GetItemByPos( UI08 uiFollowerSlot, SI16 siItemPos, MyItemData **ppItemData )
{
	OnlineFollower*		lpFollower;

	lpFollower	=	GetFollower( uiFollowerSlot );
	
	if( !lpFollower )												return FALSE;

	if( lpFollower->GetItemByPos(siItemPos, ppItemData) == FALSE )	return FALSE;

	return TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 가지고 있는지 검사한다.
//	수정 일자	:	2002-10-14 오전 10:25:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::HasItem( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity )
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	FALSE;

	return	lpFollower->HasItem( uiItemID, uiItemQuantity );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 가지고 있는지 검사한다.
//	수정 일자	:	2002-10-14 오전 10:25:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineFollower::GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity )
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	-1;

	return	lpFollower->GetItemIndex( uiItemID, uiItemQuantity );
}

SI32	IOnlineFollower::GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	-1;

	return	lpFollower->GetItemIndex( uiItemID);
}



//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템이 있는지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL IOnlineFollower::IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
		{
			if(lpFollower->Items[siPosInInventory].uiID != 0 )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	같은 아이템인지 검사한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL IOnlineFollower::IsSameItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
		{
			if((lpFollower->Items[siPosInInventory].uiID == uiItemID))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템리스트를 돌려준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData* IOnlineFollower::GetItemList(UI08 uiFollowerSlot)
{
	return (MyItemData*)Followers[uiFollowerSlot].Items;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	유효한 부하 슬롯인지 검사한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::IsValidFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// 실제 그 슬롯에 부하가 있는지 본다.
		if(Followers[uiFollowerSlot].uiCharID != 0)
			return TRUE;
	}
	return FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하 캐릭터의 ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	IOnlineFollower::GetFollowerCharID(UI08 uiFollowerSlot)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->uiCharID;
	}
	return 0;
}

VOID	IOnlineFollower::SetFollowerCharID(UI08 uiFollowerSlot, UI16 uiCharID)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE)	return;

	Followers[uiFollowerSlot].SetCharID(uiCharID);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하를 초기화 한다.
//	수정 일자	:	2002-10-29 오후 9:31:44 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	OnlineFollower*		lpFollower;
	SI32				i;

	
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		lpFollower	=	&Followers[uiFollowerSlot];
		if(lpFollower->uiCharID	==	0)
		{
			lpFollower->Init();
			lpFollower->ParametaBox.Init(lpFollowerInfo->siExp, lpFollowerInfo->siLevel, lpFollowerInfo->siStr, lpFollowerInfo->siDex, lpFollowerInfo->siVit, lpFollowerInfo->siInt, lpFollowerInfo->siLife, lpFollowerInfo->siMana, lpFollowerInfo->siAC, lpFollowerInfo->siBonus, lpFollowerInfo->siDamageRegist, lpFollowerInfo->siMagicRegist, lpFollowerInfo->siEquipMinDamage, lpFollowerInfo->siEquipMaxDamage, lpFollowerInfo->siLiftConstant);

			// 아이템을 장착 시킨다.
			for( i = 0; i < WEAR_ITEM_NUM; i++ )
			{
				lpFollower->SetWearItem( i, lpFollowerInfo->stWearItemInfo[i].uiItemID,  lpFollowerInfo->stWearItemInfo[i].siLimitTime);
			}

			lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);
			lpFollower->uiCharID		=	lpFollowerInfo->uiKind;	
			lpFollower->Nation			=   Nation;
			strncpy( lpFollower->Name, lpFollowerInfo->Name, ON_ID_LENGTH );

			lpFollower->dwSaveTime = timeGetTime();
			lpFollower->ParametaBox.IP.setCycleSaveToDB(FALSE);
			lpFollower->ParametaBox.IP.setSaveToDB(FALSE);

			return TRUE;
		}
	}
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	부하를 초기화 한다.	
//	수정 일자	:	2002-07-08 오전 12:30:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::InitFollower(OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	SI32						i;
	OnlineFollower*				lpFollower;
	SI16						siEmptyFollowerSlot;
	OnlineReceivedMyItemInfo	*pItemInfo;

	if((siEmptyFollowerSlot = FindEmptyFollowerSlot()) != -1)
	{
		// lpFollowerInfo 객체를 수정한다.
		lpFollowerInfo->uiSlot	=	UI08(siEmptyFollowerSlot);

		if(InitFollower(lpFollowerInfo->uiSlot, lpFollowerInfo, Nation, pItem) == TRUE)
		{
			// 객체를 얻어온다.
			lpFollower	=	&Followers[lpFollowerInfo->uiSlot];

			pItemInfo	=	(OnlineReceivedMyItemInfo*)((CHAR*)lpFollowerInfo + sizeof(OnFollowerInfo));

			// 아이템이 있다면 아이템을 추가시켜준다.
			for(i = 0; i < lpFollowerInfo->uiItemNum; i++)			
				lpFollower->AddItem((MyItemData*)&pItemInfo[i]);							
			
			return	TRUE;
		}
	}

	return	FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하를 Update 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::UpdataFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	OnlineFollower*		lpFollower;
	
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		lpFollower	=	&Followers[uiFollowerSlot];
		if(lpFollower->uiCharID	!= 0)
		{
			lpFollower->ParametaBox.Init(lpFollowerInfo->siExp, lpFollowerInfo->siLevel, lpFollowerInfo->siStr, lpFollowerInfo->siDex, lpFollowerInfo->siVit, lpFollowerInfo->siInt, lpFollowerInfo->siLife, lpFollowerInfo->siMana, lpFollowerInfo->siAC, lpFollowerInfo->siBonus, lpFollowerInfo->siDamageRegist, lpFollowerInfo->siMagicRegist, lpFollowerInfo->siEquipMinDamage, lpFollowerInfo->siEquipMaxDamage, lpFollowerInfo->siLiftConstant);
			lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);
			lpFollower->uiCharID		=	lpFollowerInfo->uiKind;	
			lpFollower->Nation			=   Nation;
			strncpy( lpFollower->Name, lpFollowerInfo->Name, ON_ID_LENGTH );

			lpFollower->dwSaveTime = timeGetTime();
			lpFollower->ParametaBox.IP.setCycleSaveToDB(FALSE);
			lpFollower->ParametaBox.IP.setSaveToDB(FALSE);

			return TRUE;
		}
	}
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하 삭제
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::DeleteFollower(UI08 uiFollowerSlot)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		lpFollower->Init();		
		return TRUE;
	}
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get Follower Base Parameter Data
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineFollower::GetFollowerBaseParameterCurExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetCurExp();
}

SI32	IOnlineFollower::GetFollowerBaseParameterSTR(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetStr();
}

SI32	IOnlineFollower::GetFollowerBaseParameterDEX(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetDex();
}

SI32	IOnlineFollower::GetFollowerBaseParameterVIT(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetVit();
}

SI32	IOnlineFollower::GetFollowerBaseParameterINT(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetInt();
}

SI32	IOnlineFollower::GetFollowerBaseParameterAC(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetAC();
}

SI32	IOnlineFollower::GetFollowerBaseParameterBonus(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
}

BOOL	IOnlineFollower::SetFollowerBaseParameterBonus(UI08 uiFollowerSlot, SI32 siBonus)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(siBonus);
	return TRUE;
}

BOOL	IOnlineFollower::SetFollowerParameterBonusSubTract1(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot)                  == FALSE) return FALSE;
	if(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() <  1)     return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - 1);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerBaseParameterDamageResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.DamageResist;
}

SI32	IOnlineFollower::GetFollowerBaseParameterMagicResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.MagicResist;
}

SI32	IOnlineFollower::GetFollowerBaseParameterEquipMinDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.EquipMinDamage;
}

SI32	IOnlineFollower::GetFollowerBaseParameterEquipMaxDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.EquipMaxDamage;
}

SI32	IOnlineFollower::GetFollowerBaseParameterLiftConstance(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.LiftConstant;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get Follower Parameter Data
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	IOnlineFollower::GetFollowerParameterLevel(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetLevel();	
}

SI32	IOnlineFollower::GetFollowerParameterCurExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCurExp();
}

SI32	IOnlineFollower::GetFollowerParameterPrevExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return GetRequiredExpForLevel(GetFollowerParameterLevel(uiFollowerSlot));
}

SI32	IOnlineFollower::GetFollowerParameterNextExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetNextExp();
}

SI32	IOnlineFollower::GetFollowerParameterNation(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].Nation;
}

VOID	IOnlineFollower::SetFollowerParameterNation(UI08 uiFollowerSlot, SI32 Nation)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return;

	Followers[uiFollowerSlot].SetFollowerParameterNation(Nation);
}

SI32	IOnlineFollower::GetFollowerParameterTotalStr(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalStr();
}

SI32	IOnlineFollower::GetFollowerParameterTotalDex(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalDex();
}

SI32	IOnlineFollower::GetFollowerParameterTotalVit(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalVit();
}

SI32	IOnlineFollower::GetFollowerParameterTotalInt(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalInt();
}

SI32	IOnlineFollower::GetFollowerParameterTotalAC(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetDpower();
}


SI32	IOnlineFollower::GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalDamageResist();
}

SI32	IOnlineFollower::GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalMagicResist();
}

SI32	IOnlineFollower::GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMinApower();
}

SI32	IOnlineFollower::GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxApower();
}

SI32	IOnlineFollower::GetFollowerParameterLife(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetLife();
}

BOOL	IOnlineFollower::SetFollowerParameterLife(UI08 uiFollowerSlot, SI32 Life)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetLife(Life);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerParameterMana(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetMana();
}

BOOL	IOnlineFollower::SetFollowerParameterMana(UI08 uiFollowerSlot, SI32 Mana)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetMana(Mana);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMoveSpeed();
}

SI32	IOnlineFollower::GetFollowerParameterMaxWeight(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxWeight();
}

SI32	IOnlineFollower::GetFollowerParameterCurWeight(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCurWeight();
}

SI32	IOnlineFollower::GetFollowerParameterCredit(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCredit();
}

SI32	IOnlineFollower::GetFollowerParameterLiftPower(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetLiftPower();
}

SI16	IOnlineFollower::GetFollowerParameterBonus(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
}


SI32	IOnlineFollower::GetFollowerParameterMaxLife(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxLife();
}
SI32	IOnlineFollower::GetFollowerParameterMaxMana(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxMana();
}


SI32	IOnlineFollower::GetFollowerParameterBattlePower(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetBattlePower();
}

SI32	IOnlineFollower::GetFollowerParameterBattlePower(void)
{
	SI32		TempCount;
	SI32		TempBattlePower;

	TempBattlePower = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		TempBattlePower += GetFollowerParameterBattlePower(TempCount);
	}

	return TempBattlePower;
}

SI32	IOnlineFollower::GetFollowerParameterKind(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].uiCharID;
}

char*	IOnlineFollower::GetFollowerName(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return NULL;

	return (char*)Followers[uiFollowerSlot].Name;
}

BOOL	IOnlineFollower::SetFollowerName(UI08 uiFollowerSlot, char* lpName)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{		
		ZeroMemory(lpFollower->Name, sizeof(Followers[uiFollowerSlot].Name));
		if(lpName)
		{
			memcpy(lpFollower->Name, lpName,ON_ID_LENGTH);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL	IOnlineFollower::Bonus_FollowerParameterUp(UI08 uiFollowerSlot, AbilityKindType Type, SI16 Bonus, CItem* pItem)
{
	if(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() < Bonus) return FALSE;

	switch(Type)
	{
		case ON_ABILITY_KIND_STR:
			Followers[uiFollowerSlot].ParametaBox.IP.SetStr(Followers[uiFollowerSlot].ParametaBox.IP.GetStr() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_DEX:
			Followers[uiFollowerSlot].ParametaBox.IP.SetDex(Followers[uiFollowerSlot].ParametaBox.IP.GetDex() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_VIT:
			Followers[uiFollowerSlot].ParametaBox.IP.SetVit(Followers[uiFollowerSlot].ParametaBox.IP.GetVit() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_INT:
			Followers[uiFollowerSlot].ParametaBox.IP.SetInt(Followers[uiFollowerSlot].ParametaBox.IP.GetInt() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;
	}

	return FALSE;
}

BOOL	IOnlineFollower::AllCharacterDeleteLife(SI32 DeleteLife)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - ((Followers[TempCount].ParametaBox.GetMaxLife() * DeleteLife) / 100);
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}
BOOL	IOnlineFollower::AllCharacterDeleteLifePoint(SI32 DeleteLife)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - DeleteLife;
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}

BOOL	IOnlineFollower::AllCharacterDeleteLifePercent(SI32 DeleteLifeP)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - ((GetFollowerParameterMaxLife(TempCount) * DeleteLifeP) / 100);
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병의 수를 얻어온다.
//	수정 일자	:	2002-03-25 오후 3:00:40 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	IOnlineFollower::GetFollowerNum()
{	
	UI08	i;
	UI08	siTotalFollowerNum; 

	// 부하들의 데이타를 초기화 한다.
	siTotalFollowerNum	=	0;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsValidFollower(i) == TRUE)
			siTotalFollowerNum++;
	} 

	return	siTotalFollowerNum ;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투 정보를 얻어온다
//	수정 일자	:	2002-05-02 오후 3:02:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::GetBattleInfo(UI08 uiFollowerSlot, OnImjinBattleFollowerInfo *pFollowerInfo)
{
	OnlineFollower		*pOnlineFollower;
	MyItemData			*pMyItemData;	
	SI32				i;
	DWORD				dwAllItemStructSize;
	

	pOnlineFollower	=	&Followers[uiFollowerSlot];	

	if(IsValidFollower(uiFollowerSlot) == TRUE)
	{
		// 슬롯 저장.
		pFollowerInfo->uiSlotID		=	uiFollowerSlot;

		// 이름 복사.
		strncpy(pFollowerInfo->szPlayerName, pOnlineFollower->Name, ON_ID_LENGTH);
	
		pFollowerInfo->uiKind	=	pOnlineFollower->uiCharID;			

		// Parameta Box 복사.
		memcpy(&pFollowerInfo->Iparameta, &pOnlineFollower->ParametaBox.IP, sizeof(_ImportantParameta));

		// My Weapon 복사.
		memcpy(&pFollowerInfo->MW, &pOnlineFollower->Weapon, sizeof(MyWeapons));

		pMyItemData				=	(MyItemData*)((CHAR*)(pFollowerInfo) + sizeof(OnImjinBattleFollowerInfo));

		dwAllItemStructSize				=	0;
		pFollowerInfo->uiItemNum		=	0;
		for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		{
			if(pOnlineFollower->Items[i].uiID != 0)
			{
				*pMyItemData	=	pOnlineFollower->Items[i];
				
				// 포인터를 증가시킨다.
				// MyItemData형이므로 sizeof(MyItemData) 만큼 증가된다.
				pMyItemData++;

				pFollowerInfo->uiItemNum++;
			}
		}

		return	DWORD(sizeof(OnImjinBattleFollowerInfo) +  (pFollowerInfo->uiItemNum * sizeof(MyItemData)));
	}
	else
		return	0;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	전투후 사용할 포만감 수치 검사
//	수정 일자	:	2002-06-19 오전 9 - 이윤석
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32 IOnlineFollower::GetUseSatietyWarAfter(void)
{
	SI32		TempCount;
	SI32		TempUseSatiety;

	TempUseSatiety = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			TempUseSatiety += 1 + ( GetFollowerParameterTotalStr( TempCount ) / 20 ) ;
		}
	}

	return TempUseSatiety;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-05 오후 7:20:32 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		
	{
		return pFollower->CanAddItem(uiItemID, uiQuantity, siPosInInventory);
	}
	else
	{
		return FALSE;	
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-07 오전 4:38:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(puiID, puiQuantity, uiItemNum);
	else				return FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-07 오전 5:01:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanAddFollower(UI08 uiAddFollowerNum)
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(Followers[i].uiCharID == 0)
		{
			uiAddFollowerNum--;

			if(uiAddFollowerNum == 0)	return	TRUE;
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	비어있는 용병 버퍼를 찾는다.
//	수정 일자	:	2002-07-08 오후 5:01:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	IOnlineFollower::FindEmptyFollowerSlot()
{
	SI32				i;	

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		if(Followers[i].uiCharID == 0)	return	i;

	return	-1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병의 교역 정보를 얻어온다.
//	수정 일자	:	2002-07-09 오후 1:59:13 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::GetFollowerTradeInfo(UI08 uiFollowerSlot, OnTradeFollowerInfo *pTradeFollowerInfo)
{
	OnlineFollower	*pFollower;
	
	if((pFollower = GetFollower(uiFollowerSlot)) == FALSE)	return	FALSE;

	pTradeFollowerInfo->uiKind				=		pFollower->uiCharID;
	pTradeFollowerInfo->siLevel				=		pFollower->ParametaBox.IP.GetLevel();
	pTradeFollowerInfo->siExp				=		pFollower->ParametaBox.IP.GetCurExp();
	pTradeFollowerInfo->siSTR				=		pFollower->ParametaBox.IP.GetStr();
	pTradeFollowerInfo->siDEX				=		pFollower->ParametaBox.IP.GetDex();
	pTradeFollowerInfo->siVIT				=		pFollower->ParametaBox.IP.GetVit();
	pTradeFollowerInfo->siINT				=		pFollower->ParametaBox.IP.GetInt();
	
	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	정적 변수 초기화.
//	수정 일자	:	2002-07-12 오후 3:28:25 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineFollower::InitStaticVariable(CItem* pItem)
{
	OnlineFollower::InitStaticVariable(pItem);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 이 캐릭터의 전체 아이템의 무게를 얻어온다.	
//	수정 일자	:	2002-07-12 오후 3:41:59 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::GetTotalItemWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetTotalItemWeight();
	else				return 0;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 캐릭터가 최고 들 수 있는 무게.
//	수정 일자	:	2002-07-12 오후 5:34:50 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	IOnlineFollower::GetMaxWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetMaxWeight();
	else				return 0;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	이 캐릭터가 최고 가질 수 있는 아이템의 수량을 얻어온다.
//	수정 일자	:	2002-07-12 오후 5:51:38 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	IOnlineFollower::GetItemQuantityToTake(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetItemQuantityToTake(uiItemID);
	else				return 0;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 추가 시킬 수 있느냐?
//	수정 일자	:	2002-07-12 오후 7:58:01 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(uiItemID, uiQuantity);
	else				return FALSE;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병들 중에 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.
//	수정 일자	:	2002-07-12 오후 8:21:02 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::IsItemWeightOverflowed()
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsValidFollower(i) == TRUE)
		{
			if(Followers[i].IsItemWeightOverflowed() == TRUE)	return	TRUE;
		}
	}

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	특정 용병이 자기가 수용할 수 있는것 보다 아이템을 초과 해서 드는지 얻어온다.
//	수정 일자	:	2002-07-12 오후 8:22:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::IsItemWeightOverflowed(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->IsItemWeightOverflowed();
	else				return 0;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착하고 있는 아이템을 벗는다.
//	수정 일자	:	2002-07-12 오후 9:16:04 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::TakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return	pFollower->TakeOffItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);	
	}

	return FALSE;
}

BOOL	IOnlineFollower::DisAppearItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return	pFollower->DisAppearItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);	
	}

	return FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착한다.
//	수정 일자	:	2002-07-13 오후 2:15:49 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::PutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return pFollower->PutOnItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);
	}
	
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 장착된 아이템들의 리스트를 얻어온다.
//	수정 일자	:	2002-07-13 오후 2:54:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::GetWearItems(UI08 uiFollowerSlot, UI16 puiWearItems[WEAR_ITEM_NUM])
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetWearItems(puiWearItems);
	else				return FALSE;			
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 장착한다.
//	수정 일자	:	2002-07-15 오후 12:43:31 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::SetWearItem2(UI08 uiFollowerSlot, UI08 uiLoc, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->SetWearItem(uiLoc, uiItemID,OnlineFollower::pCItem->GetItemUsingDay(uiItemID));
	else				return FALSE;				
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
//	수정 일자	:	2002-07-15 오후 3:58:47 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanPutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanPutOnItem(uiWearLoc, uiItemID, siPosInInventory, siTradeGrade, ErrorCode);
	else				return FALSE;				
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	인벤토리에 있는 아이템을 장착할 수 있는지 알아온다.
//	수정 일자	:	2002-07-15 오후 3:59:48 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanTakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanTakeOffItem(uiWearLoc, uiItemID, siPosInInventory);
	else				return FALSE;					
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 이동 시킬 수 있는지 알아온다.
//	수정 일자	:	2002-11-12 오전 10:30:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanMoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{
	OnlineFollower		*pFollower1;
	OnlineFollower		*pFollower2;
	

	if( ( pFollower1 = GetFollower( uiFollowerSlot1 ) ) == NULL )										return	FALSE;
	if( ( pFollower2 = GetFollower( uiFollowerSlot2 ) ) == NULL )										return	FALSE;

	if( pFollower1 != pFollower2 )
	{
		// 아이템을 가지고 있는지 검사한다.
		if( pFollower1->HasItem( uiItemID, uiItemQuantity ) == FALSE )									return	FALSE;

		// 아이템을 받는 캐릭터가 아이템을 가질 수 있는지 검사한다.
		if( pFollower2->CanAddItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )				return	FALSE;
	}
	else
	{
		if( pFollower1->CanMoveItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )			return	FALSE;
	}


	return	TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 이동 시킨다.
//	수정 일자	:	2002-11-12 오전 10:24:46 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 *psiPosInInventory )
{
	OnlineFollower		*pFollower1;
	OnlineFollower		*pFollower2;
	MyItemData			ItemData;

	if( CanMoveItem( uiFollowerSlot1, uiItemID, uiItemQuantity, uiFollowerSlot2, *psiPosInInventory ) == FALSE )		return	FALSE;

	pFollower1 = GetFollower( uiFollowerSlot1 );
	pFollower2 = GetFollower( uiFollowerSlot2 );

	if( pFollower1 != pFollower2 )
	{
		// 아이템을 삭제한다.
		if( pFollower1->DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )					return	FALSE;

		// 아이템을 추가한다.		
		ItemData.uiID				=	uiItemID;
		ItemData.uiQuantity			=	uiItemQuantity;
		ItemData.siPosInInventory	=	*psiPosInInventory;
		if( pFollower2->AddItem(&ItemData) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )									return	FALSE;
	
		*psiPosInInventory			=	ItemData.siPosInInventory;
	}
	else
	{
		pFollower1->MoveItem( uiItemID, uiItemQuantity, psiPosInInventory );
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	용병 스텟을 보너스 수치로 바꾼다.
//	수정 일자	:	2002-12-22 - 이준석
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser *pCharParser, UI16 uiFollowerID , SI32 *pBonus, CItem* pItem)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE)  return FALSE;

	SI32 BaseStr, BaseInt, BaseDex, BaseVit, BaseHP, BaseMP, BaseTot;
	SI32 CurStr, CurInt, CurDex, CurVit, CurBonus, CurTot;
	SI32 TotBonus;

	// 용병의 기본수치
	BaseStr = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetStr();
	BaseInt = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetInt();
	BaseDex = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetDex();
	BaseVit = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetVit();
	BaseHP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetLife();
	BaseMP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetMana();
	BaseTot = BaseStr + BaseInt + BaseDex + BaseVit;

	// 용병의 현재수치
	CurStr	= Followers[uiFollowerSlot].ParametaBox.IP.GetStr();
	CurInt	= Followers[uiFollowerSlot].ParametaBox.IP.GetInt();
	CurDex	= Followers[uiFollowerSlot].ParametaBox.IP.GetDex();
	CurVit	= Followers[uiFollowerSlot].ParametaBox.IP.GetVit();
	CurBonus= Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
	CurTot	= CurStr + CurInt + CurDex + CurVit + CurBonus;

	TotBonus = CurTot - BaseTot;

	if (pBonus) {
		*pBonus = TotBonus;
	}

	// 용병 기본수치로 스텟 초기화
	Followers[uiFollowerSlot].ParametaBox.IP.SetStr(BaseStr);
	Followers[uiFollowerSlot].ParametaBox.IP.SetInt(BaseInt);
	Followers[uiFollowerSlot].ParametaBox.IP.SetDex(BaseDex);
	Followers[uiFollowerSlot].ParametaBox.IP.SetVit(BaseVit);
	Followers[uiFollowerSlot].ParametaBox.IP.SetLife(BaseHP);
	Followers[uiFollowerSlot].ParametaBox.IP.SetMana(BaseMP);
	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(TotBonus);

	OnlineFollower *lpFollower = &Followers[uiFollowerSlot];
	lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);

	return TRUE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	업그레이드 가능한 무기인지 알아온다. (고급무기)
//	수정 일자	:	2003-3-31 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	IOnlineFollower::CanWeaponBeUpgraded( UI16 uiFollowerSlot )
{
	return Followers[uiFollowerSlot].CanWeaponBeUpgraded();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착 무기 아이템을 특정 아이템으로 업그레이드 시켜준다.
//	수정 일자	:	2003-3-31 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineFollower::UpgradeWeapon( UI16 uiFollowerSlot, UI16 uiItemID )
{
	Followers[uiFollowerSlot].UpgradeWeapon( uiItemID );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	장착중인 무기 아이템을 파괴시킨다.
//	수정 일자	:	2003-3-31 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineFollower::BrakeWeapon( UI16 uiFollowerSlot )
{
	Followers[uiFollowerSlot].BrakeWeapon();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 용병들의 체력을 100% 회복시켜준다.
//	수정 일자	:	2003-09-18 16:28 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineFollower::SetFullLifeOfAllChar()
{
	OnlineFollower				*pOnlineFollower;

	for( SI32 i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			pOnlineFollower		=	GetFollower(i);
			pOnlineFollower->ParametaBox.IP.SetLife( pOnlineFollower->ParametaBox.GetMaxLife() );
		}
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	모든 용병들의 마나를 100% 회복시켜준다.
//	수정 일자	:	2003-09-18 16:33 - 백상현
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	IOnlineFollower::SetFullManaOfAllChar()
{
	OnlineFollower				*pOnlineFollower;

	for( SI32 i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			pOnlineFollower		=	GetFollower(i);
			pOnlineFollower->ParametaBox.IP.SetMana( pOnlineFollower->ParametaBox.GetMaxMana() );
		}
	}
}
