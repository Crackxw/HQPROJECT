/************************************************************************************************
	だ橾貲 : IOnlineFollower.cpp
	氬渡濠 : 檜彌戮
	熱  薑 : 檜撩營 (嬴檜蠱 婦溼餌о 籀葬ж賊憮 夥脣棻.)
	睡ж菟曖 賅萇 薑爾蒂 婦葬и棻.
************************************************************************************************/
#include <GSL.h>
#include <limits.h>
#include "OnlineItem.h"
#include "IOnlineFollower.h"
#include "Mmsystem.h"
#include "OnlineChar-Parser.h"




CItem*				OnlineFollower::pCItem;								// Item Class.

//***********************************************************************************************
//	OnlineFollower 掘褻羹
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
	// ID蒂 蟾晦�� и棻.
	uiCharID	=	0;	
	// 嬴檜蠱擊 蟾晦�� и棻.	
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		Items[i].siPosInInventory	=	i;
		Items[i].uiID				=	0;
		Items[i].uiQuantity			=	0;
	}
	ZeroMemory(Name, sizeof(Name));
	// 鼠晦蒂 蟾晦�� и棻.
	Weapon.Init();

	dwSaveTime = timeGetTime();

	dwTotalItemWeight	=	0;
}

MyItemData*		OnlineFollower::GetItem(UI16 uiItemID)	// 嬴檜蠱擊 橢橫螞棻.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return &Items[i];
	}
	return NULL;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID)	// 嬴檜蠱擊 橢橫螞棻.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return i;
	}
	return -1;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID,UI16 uiQuantity)	// 嬴檜蠱擊 橢橫螞棻.
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
//	撲貲	:	輿橫霞 曄虜躑曖 鼠啪陛 蹺陛 腆熱 氈朝雖 匐餌.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::CheckWeight(UI32 Weight)
{
	return ParametaBox.GetMaxWeight() >= (ParametaBox.GetCurWeight() + (SI32)Weight) ? TRUE:FALSE ;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱 蹺陛, (DB盪濰寰脾, 棻艇 л熱縑憮 ��轎ж溥賊 Player 縑 氈朝 AddItem() 賦撿л.)
//	熱薑 橾濠	:	2002-07-12 螃�� 3:15:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::AddItem(MyItemData* pMyItem, BOOL bTakeOff)
{
	MyItemData		*pItem;		
	DWORD			dwRet  = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;	
	UI16			uiAddedItemID, uiAddedItemQuantity;		

	uiAddedItemID		=	pMyItem->uiID;
	uiAddedItemQuantity	=	pMyItem->uiQuantity;
		
	// 1. 偽擎 嬴檜蠱檜 氈棻賊, 斜夠縑 蹺陛и棻.
	pItem	=	GetItem(pMyItem->uiID);
	if(pItem)
	{
		pItem->uiQuantity			+=	pMyItem->uiQuantity;	
		
		// ⑷營 陛雖堅 氈朝 嬴檜蠱曖 薑爾蒂 憲溥遽棻.
		pMyItem->uiQuantity			=   pItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.

		// siPosInInventory高檜 澀跤脹 唳辦 DB縑 撮たж雖 彊紫煙 FALSE 籀葬и棻.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;
		goto	END;		
	}

	// 2. 偽擎 嬴檜蠱檜 橈棻賊, 雖薑и 蝸煜縑 虜菟晦 衛紫.
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

	// 3. 偽擎 嬴檜蠱檜 橈堅, 雖薑и 蝸煜縑紫 厥擊熱 橈棻賊, 綴蝸煜擊 瓊嬴 厥朝棻..
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	pMyItem->uiID;
		pItem->uiQuantity			=	pMyItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.

		// siPosInInventory高檜 澀跤脹 唳辦 DB縑 撮たж雖 彊紫煙 FALSE 籀葬и棻.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;
		goto	END;
	}

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// 嬴檜蠱曖 鼠啪蒂 渦п遽棻.
		// 嬴檜蠱擊 慇朝剪塭賊 渦п輿雖 彊朝棻.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:47:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::AddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff )
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// 嶸�褲� 嬴檜蠱檣雖 羹觼и棻.
	if( uiItemID == 0)			goto	END;

	uiAddedItemID		=	uiItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. 偽擎 嬴檜蠱檜 氈棻賊, 斜夠縑 蹺陛и棻.
	pItem	=	GetItem( uiItemID );
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;	

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. 綠橫 氈朝 夠擊 瓊嬴 蹺陛и棻.
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
		// 嬴檜蠱曖 鼠啪蒂 渦п遽棻.
		// 嬴檜蠱擊 慇朝剪塭賊 渦п輿雖 彊朝棻.
		if(bTakeOff == FALSE)				AddItemWeight( uiAddedItemID, uiAddedItemQuantity );	
	}

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛и棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 3:17:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::AddItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// 嶸�褲� 嬴檜蠱檣雖 羹觼и棻.
	if(ItemID == 0)		goto	END;

	uiAddedItemID		=	ItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. 偽擎 嬴檜蠱檜 氈棻賊, 斜夠縑 蹺陛и棻.
	pItem	=	GetItem(ItemID);
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. 綠橫 氈朝 夠擊 瓊嬴 蹺陛и棻.
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	ItemID;
		pItem->uiQuantity			=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;		
		goto	END;
	}	

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// 嬴檜蠱曖 鼠啪蒂 渦п遽棻.
		// 嬴檜蠱擊 慇朝剪塭賊 渦п輿雖 彊朝棻.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 10:28:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	pMyItem->siPosInInventory	=   pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.


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
		// 嬴檜蠱擊 餉薯п遽棻.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-11-19 螃瞪 10:26:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	*psiPosInInventory		=   pItem->siPosInInventory;	// 檣漸饜葬 嬪纂蒂 憲溥醜撿 DB縑 噹棻.

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
		// 嬴檜蠱擊 餉薯п遽棻.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п渡 嬴檜蠱曖 熱榆擊 賓棻.
//	熱薑 橾濠	:	2002-07-05 螃�� 4:16:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 嬴檜蠱擊 餉薯п遽棻.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦�� 醞縑 嬴檜蠱擊 濰雜и棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 4:05:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::SetWearItem(UI08 uiLoc, UI16 uiItemID,SI16 siItemLimitTime)
{
	// 晦襄 濰雜 薑爾蒂 橢橫憮
	UI16	uiOldItemID	= Weapon.GetWearItem( uiLoc );

	// 鼠啪蒂 貍輿堅
	DelItemWeight( uiOldItemID, 1 );

	// 褐敘 鼠晦薑爾蒂 濰雜ж堅
	Weapon.SetWearItem(uiLoc, uiItemID,siItemLimitTime);
	
	// ⑷營 菟堅 氈朝 嬴檜蠱曖 鼠啪縑 蹺陛п遽棻.
	AddItemWeight(uiItemID, 1);

	// 議葛攪曖 棟溘纂菟擊 營褻薑и棻.
	ParametaBox.Action(pCItem, Weapon);		

	return	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱 雜辨
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
//	撲貲	:	陛雖堅 氈朝 嬴檜蠱紫 橈堅, 雜辨и 嬴檜蠱紫 橈橫撿 и棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::IsEmpty()
{
	// 1. 陛雖堅 氈朝 嬴檜蠱檜 橈橫撿 и棻.
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID != 0)
			return FALSE;
	}

	// 2. 雜辨ж堅 氈朝 嬴檜蠱紫 橈橫撿 и棻.
	return Weapon.IsEmpty();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-05 螃�� 7:23:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// 嬴檜蠱擊 慇朝 醞檜 嬴棍陽縑虜 鼠啪蒂 堅溥и棻.
	if(bTakeOff == FALSE)
	{		
		// 蹺陛 衛酈溥朝 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// 嬴檜蠱曖 識 鼠啪蒂 蟾婁ж朝雖 憲嬴螞棻.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// 偽擎 嬴檜蠱檜 氈朝雖 瓊嬴獄棻.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// 檣漸饜葬縑 檜嘐 偽擎 嬴檜蠱檜 氈棻.
		
		// 嬴檜蠱 熱陛 螃幗Ы煎辦 釭雖 彊朝雖 匐餌и棻.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// 檣漸饜葬縑 偽擎 嬴檜蠱檜 橈棻.		
	
		// п渡 嬪纂縑 嬴檜蠱擊 厥擊熱 氈朝雖 憲嬴獄棻.
		if( GetItemByPos(siPosInInventory, &pItemInfo) == FALSE )			return FALSE;		// 檣漸饜葬 嬪纂陛 ぎ葬朝 蛔曖 檜嶸煎 GetItemByPos褒ぬ
		if( pItemInfo != NULL)												return	FALSE;		// 嬴檜蠱檜 檜嘐 襄營л
	}

	return	TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-12 螃�� 7:56:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// 嬴檜蠱擊 慇朝 醞檜 嬴棍陽縑虜 鼠啪蒂 堅溥и棻.
	if(bTakeOff == FALSE)
	{		
		// 蹺陛 衛酈溥朝 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// 嬴檜蠱曖 識 鼠啪蒂 蟾婁ж朝雖 憲嬴螞棻.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// 偽擎 嬴檜蠱檜 氈朝雖 瓊嬴獄棻.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// 檣漸饜葬縑 檜嘐 偽擎 嬴檜蠱檜 氈棻.
		
		// 嬴檜蠱 熱陛 螃幗Ы煎辦 釭雖 彊朝雖 匐餌и棻.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// 檣漸饜葬縑 偽擎 嬴檜蠱檜 橈棻.		
	
		// 綴 蝸煜檜 氈朝雖 匐餌и棻. 綴 蝸煜檜 橈戲賊 嬴檜蠱擊 蹺陛 衛鑒 熱 橈棻.
		if(GetEmptyItem() == NULL)											return	FALSE;
		
	}

	return	TRUE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-05 螃�� 7:25:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::GetItemByPos( SI16 siPosInInventory, MyItemData **ppItemData )
{	
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )				return FALSE;

	if( Items[siPosInInventory].uiID != 0 )			*ppItemData	=	&Items[siPosInInventory];
	else											*ppItemData	=	NULL;

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-07 螃瞪 4:39:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 嬴檜蠱擊 慇朝 醞檜 嬴棍陽縑虜 鼠啪蒂 堅溥и棻.
		if(bTakeOff == FALSE)
		{			
			// 蹺陛 衛酈溥朝 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.
			if((pItemHeader = pCItem->GetItem(puiID[i])) == NULL)								return	FALSE;		
			dwAddItemWeight		+=	(pItemHeader->m_uiWeight * puiQuantity[i]);

			// 嬴檜蠱曖 識 鼠啪蒂 蟾婁ж朝雖 憲嬴螞棻.
			if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)									return	FALSE;
		}

		if((pItemInfo = GetItem(puiID[i])) == NULL)	
		{
			// 億煎遴 濠葬陛 в蹂ж棻.
			siNeedSlot++;
		}
		else
		{
			// 晦襄縑 檜嘐 嬴檜蠱檜 氈棻.

			// 晦襄縑 嬴檜蠱縑 檜 嬴檜蠱擊 渦й 熱 氈朝雖 憲嬴螞棻.
			// 虜擒 渦й 熱 橈棻賊 嬴檜蠱擊 蹺陛 衛鑒 熱 橈棻.
			if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity) < puiQuantity[ i ] )		return	FALSE;					
		}		
	}
	
	// в蹂и 濠葬陛 橈棻賊 TRUE蒂 葬欐и棻.
	if(siNeedSlot == 0)	return	TRUE;

	// ⑷營 厥擊 熱 氈朝 嬴檜蠱曖 熱蒂 橢橫螞棻.
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == 0)
		{
			siNeedSlot--;

			// в蹂и 蝸煜擊 賅舒 瓊懊棻.
			if(siNeedSlot == 0)	return	TRUE;
		}
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛衛儷擊 唳辦 鼠啪蒂 蟾婁ж朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 2:52:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::IsOverflowItemWeight(DWORD dwAddItemWeight)
{

	if( ( GetTotalItemWeight() + dwAddItemWeight ) > GetMaxWeight() )		return	TRUE;
	else																	return	FALSE;

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 鼠啪 蟾婁檣雖 匐餌ж罹 務晦 ��萇 鼻鷓檜賊 TRUE蒂 葬欐и棻.
//	熱薑 橾濠	:	2003-03-19 螃瞪 10:57:23 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::IsOverWeight()
{
	if( GetTotalItemWeight() > GetMaxWeight() )		return	TRUE;
	else											return	FALSE;

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 鼠啪蒂 蹺陛衛麵遽棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 2:58:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::AddItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// 蹺陛 衛酈溥朝 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// 嬴檜蠱曖 鼠啪蒂 渦и棻.
	dwTotalItemWeight	+=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 鼠啪蒂 貍遽棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 2:58:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::DelItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// 蹺陛 衛酈溥朝 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// 嬴檜蠱曖 鼠啪蒂 渦и棻.
	dwTotalItemWeight	-=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱 蟾晦��.
//	熱薑 橾濠	:	2002-07-12 螃�� 3:27:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineFollower::InitStaticVariable(CItem* pItem)
{
	pCItem			=	pItem;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 檜 議葛攪曖 瞪羹 嬴檜蠱曖 賅鼠啪蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 2:45:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::GetTotalItemWeight()
{
	return dwTotalItemWeight;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 議葛攪陛 譆堅 菟 熱 氈朝 鼠啪.
//	熱薑 橾濠	:	2002-07-12 螃�� 4:17:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	OnlineFollower::GetMaxWeight()
{
	return ParametaBox.GetMaxWeight();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 議葛攪陛 譆堅 陛韓 熱 氈朝 嬴檜蠱曖 熱榆擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 5:52:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	OnlineFollower::GetItemQuantityToTake(UI16 uiItemID)
{
	MyItemData		*pItem;			
	UI16			uiItemQuantity;
	CItemHeader		*pItemHeader;
	
	// 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)	return	0;		

	// ⑷營 鼠啪陛 螃幗 Ы煎辦 腎歷朝雖 憲嬴螞棻.
	if(IsItemWeightOverflowed() == TRUE)					return	0;
	
	if((pItem = GetItem(uiItemID)) != NULL)
	{	
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM - pItem->uiQuantity;
	}
	else
	{
		// ⑷營 綠橫氈朝 蝸煜擊 瓊朝棻.
		if((pItem = GetEmptyItem()) == NULL)	return 0;
		
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM;
	}
			

	// 鼠啪蒂 堅溥п憮 菟 熱 氈朝 嬴檜蠱曖 鼠啪蒂 羹觼и棻.
	return UI16(min((GetMaxWeight() - GetTotalItemWeight()) / pItemHeader->m_uiWeight, uiItemQuantity));	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 議葛攪陛 濠晦陛 熱辨й 熱 氈朝匙 爾棻 嬴檜蠱擊 蟾婁 п憮 萄朝雖 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 8:19:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::IsItemWeightOverflowed()
{
	if(GetMaxWeight() < GetTotalItemWeight())	return	TRUE;
	else										return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜ж堅 氈朝 嬴檜蠱擊 慇朝棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 9:17:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::TakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// 檣漸饜葬縑 嬴檜蠱擊 厥橫遽棻.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	1;
	ItemData.siPosInInventory	=	siPosInInventory;
	if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// 嬴檜蠱擊 慇朝棻.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// 檣漸饜葬縑 厥擊 薑爾蒂 橢橫螞棻.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// 議葛攪曖 棟溘纂菟擊 營褻薑и棻.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

BOOL	OnlineFollower::DisAppearItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// 檣漸饜葬縑 嬴檜蠱擊 厥橫遽棻.
	ItemData.uiID				=	0;
	ItemData.uiQuantity			=	0;
	ItemData.siPosInInventory	=	0;
	//if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// 嬴檜蠱擊 慇朝棻.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// 檣漸饜葬縑 厥擊 薑爾蒂 橢橫螞棻.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// 議葛攪曖 棟溘纂菟擊 營褻薑и棻.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:17:37 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::PutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{	
	MyItemData			DeleteItemInfo;	

	// 檣漸饜葬縑憮 嬴檜蠱擊 賓棻.
	DeleteItemInfo.uiID					=	uiItemID;
	DeleteItemInfo.uiQuantity			=	1;
	DeleteItemInfo.siPosInInventory		=	siPosInInventory;
	if( DelItem(&DeleteItemInfo, TRUE) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )		return FALSE;

	// 嬴檜蠱擊 濰雜 衛鑑棻.
	SI32	siMaxLimitDate	= pCItem->GetItemUsingDay(uiItemID);
	( siMaxLimitDate > 0 )	?	siMaxLimitDate++ : siMaxLimitDate = 0;
	Weapon.SetWearItem( uiWearLoc, uiItemID, siMaxLimitDate );
	
	// 檣漸饜葬縑 厥擊 薑爾蒂 橢橫螞棻.
	if(pMyItemData != NULL)
		*pMyItemData			=	DeleteItemInfo;

	// 議葛攪曖 棟溘纂菟擊 營褻薑и棻.
	ParametaBox.Action(pCItem, Weapon);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetWearItem()
// Code: actdoll (2004-07-19)
// Desc: ⑷營 檜 議葛攪陛 か滌и 嬪纂縑 濰雜ж堅 氈朝 嬴檜蠱曖 薑爾蒂 橢朝棻.
//	uiWearLoc			- 濰雜 嬪纂 檣策蝶
//	bIsTimerItem		- 濰雜 嬴檜蠱檜 衛除薯 嬴檜蠱檜賊 true 葬欐
//	siRemainDate		- 衛除薯 嬴檜蠱檜 濰雜腎歷擊 唳辦 陴擎 餌辨晦橾 葬欐
//	return				- 濰雜и 嬴檜蠱曖 ID. 橈棻賊 0擊 葬欐
//-----------------------------------------------------------------------------
UI16	OnlineFollower::GetWearItem( UI08 uiWearLoc, BOOL &bIsTimerItem, SI16 &siRemainDate )
{
	bIsTimerItem	= false;
	siRemainDate	= 0;
	UI16 uiItemID = Weapon.GetWearItem( uiWearLoc );
	if( uiItemID == 0 )		return uiItemID;

	// 檜 嬴檜蠱檜 衛除薯 嬴檜蠱檣雖 っ欽
	SI16	siTemp;
	CItemHeader *pItemHeader = pCItem->GetItemInfo( uiItemID, siTemp );
	if( pItemHeader->m_bTimeLimit )		// 衛除薯 嬴檜蠱橾 唳辦
	{
		bIsTimerItem	= (BOOL)pItemHeader->m_bTimeLimit;
		siRemainDate	= Weapon.GetWearItemLimitTime( uiWearLoc );
	}

	return	uiItemID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濰雜脹 嬴檜蠱菟曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:56:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM])
{
	Weapon.GetWearItems(puiWearItems);
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 4:00:36 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanPutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	MyItemData			*pItemDataInInventory;
	CItemHeader			*pItem;	

	// 褒薯 嬴檜蠱擊 陛雖堅 氈朝雖 �挫恉挬�.
	if( GetItemByPos(siPosInInventory, &pItemDataInInventory) )	
	{
		if( pItemDataInInventory == NULL )
		{
			ErrorCode = NOFIND_ERROR;
			return	FALSE;
		}
	}
	// 偽擎 嬴檜蠱擊 陛雖堅 氈朝雖 �挫恉挬�.
	if(pItemDataInInventory->uiID != uiItemID)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// 濰雜ж溥朝 夠縑 嬴檜蠱檜 綠橫氈朝雖 �挫恉挬�.
	if(Weapon.GetWearItem(uiWearLoc) != 0)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// 嬴檜蠱 薑爾陛 氈朝雖 �挫�
	if((pItem = pCItem->GetItem(uiItemID)) == NULL)						
	{
		ErrorCode = NOFIND_ERROR;
		return  FALSE;	
	}
	// 嬴檜蠱 謙盟陛 熱�ㄩ怹恔� �挫� - 輿檣奢虜 雜辨陛棟
	if(pItem->m_siGroupCode & ON_ITEM_GUARDIAN)
	{
		if((uiCharID  & 255) != 'C')
		{
			ErrorCode = CANTWEARGAURDIAN_ERROR;
			return FALSE;
		}
	}
	// 嬴檜蠱 謙盟陛 衛除薯 嬴檜蠱檣雖 �挫� - 輿檣奢虜 雜辨 陛棟
	// actdoll (2005/02/01 17:00) : 
	//	嬪縑 僅偽擎 掘僥檜 氈朝等 檜勒 謁 罹晦 厥歷擊梱? 橾欽 覬 雖釭堅 僥薯 橈戲賊 餉薯
	if(pCItem->IsTimeLimitItem(uiItemID))
	{
		if((uiCharID & 255) != 'C')
		{
			ErrorCode = CANTWEARTIMELIMITITEM_ERROR;
			return FALSE;
		}
	}
	
	// 嬴檜蠱 謙盟陛 萄溯蝶檣雖 �挫� - 輿檣奢虜 雜辨陛棟ж貊 褐辨紫 в蹂
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
		// 嬴檜蠱擊 濰雜й 熱 氈朝 溯漣檜 腎朝雖 匐餌и棻.
		if(pItem->m_siReqLv > ParametaBox.IP.GetLevel())
		{
			ErrorCode = LEVEL_ERROR;
			return	FALSE;
		}
	}

	// 鼠晦橾 唳辦賊 (濰雜鼠晦 謙盟朝 擅縑憮 棻羹觼 腎嘎煎 輿檣奢橾 唳辦虜 匐儀и棻.)
	if(pItem->pWeaponHeader && (uiCharID & 255) == 'C')
	{
		switch(pItem->pWeaponHeader->siWeaponType)
		{
		case ON_ITEM_MISSLE_TYPE_FAN:			// 橾獄 罹濠虜 睡瓣蒂 濰綠 й 熱 檜評.			
			// 橾獄 罹濠虜 TRUE
			if (uiCharID == 9027) return TRUE;
			else
			{
				ErrorCode = FAN_ERROR;
				return FALSE;
			}			
			break;			
		case ON_ITEM_MISSLE_TYPE_DOUBLESTICK:	// 褻摹 罹濠虜 誰瞰堆蒂 濰綠 й 熱 檜評.								
			if( uiCharID == 323 ) return TRUE;
			else {			
				ErrorCode = DOUBLESTICK_ERROR;
				return FALSE;
			}
			break;
									
		case ON_ITEM_MISSLE_TYPE_SPINSTONE:		// 褻摹 陴濠虜 餌傖給蒂 濰綠 й 熱 檜評.								
			if( uiCharID == 67 ) return TRUE;
			else {			
				ErrorCode = SPINSTONE_ERROR;
				return FALSE;
			}		
			break;									
		case ON_ITEM_MISSLE_TYPE_SCYTHE:		// 橾獄 陴濠虜 陷蒂 濰綠 й 熱 檜評.							
			if( uiCharID == 8771 )	return TRUE;
			else {				
				ErrorCode = SCYTHE_ERROR;
				return FALSE;
			}
			break;	
		case ON_ITEM_MISSLE_TYPE_BEADS:         // 渠虜 陴 渠虜 喪朝 膩輿蒂 雞熱橈棻.
			/*if( uiCharID == 10307 || uiCharID == 10563) return FALSE;

			break;*/
		case ON_ITEM_MISSLE_TYPE_BUDDIST:		// 輿檣奢擎 跡饕 膩輿朝 雞 熱 橈棻.
			return FALSE;
			break;
		case ON_ITEM_MISSLE_TYPE_FIREGUN:		// �倖鰲� 輿檣奢檜 雞熱 橈棻.
		case ON_ITEM_MISSLE_TYPE_STICK:
		case ON_ITEM_MISSLE_TYPE_DSWORD:
		case ON_ITEM_MISSLE_TYPE_PEARL:
			return FALSE;
			break;

									
		case ON_ITEM_MISSLE_TYPE_FREE:			// 賅萇 馨葛攪陛 雞 熱 檜評.
			break;
		}
	}	
	return	TRUE;
}
	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 4:01:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanTakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	// п渡 辨煽檜 褒薯 慇戲溥堅 ж朝 嬴檜蠱檜 濰雜ж堅 氈朝 嬴檜蠱婁 偽擎雖 綠掖и棻.
	if(Weapon.GetWearItem(uiWearLoc) != uiItemID)						return	FALSE;

	// 慇戲溥堅 ж朝 嬴檜蠱擊 檣漸饜葬縑 厥擊 熱 氈朝雖 匐餌и棻.
	if(CanAddItem(uiItemID, 1, siPosInInventory, TRUE) == FALSE)		return	FALSE;

	if(pCItem->IsTimeLimitItem(uiItemID) == TRUE)						return  FALSE;

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 陛雖堅 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 10:27:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	MyItemData	*pMyItemData;

	if( ( pMyItemData = GetItem( uiItemID ) ) == NULL )			return	FALSE;

	if( pMyItemData->uiQuantity < uiItemQuantity )				return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 衡望 熱 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:48:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanMoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory )
{
	// 嬴檜蠱檜 氈朝雖 匐餌и棻.
	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )			return	FALSE;
		
	// 嬴檜蠱擊 衡晦溥朝 濠葬陛 綠橫 氈朝雖 匐餌и棻.
	if( IsEmptyItemInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 螢曹棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:57:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::MoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 *psiPosInInventory )
{
	MyItemData			ItemData;

	if( CanMoveItem( uiItemID, uiItemQuantity, *psiPosInInventory ) == FALSE )			return	FALSE;

	// 嬴檜蠱擊 賓棻.
	if( DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	return	FALSE;
	
	// 嬴檜蠱擊 渦и棻.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	uiItemQuantity;
	ItemData.siPosInInventory	=	*psiPosInInventory;
	if( AddItem(&ItemData) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )					return	FALSE;

	*psiPosInInventory			=	ItemData.siPosInInventory;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬頂曖 嬴檜蠱擊 厥朝 嬪纂陛 綠錫朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:52:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::IsEmptyItemInInventory( SI16 siPosInInventory )
{
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	if( Items[ siPosInInventory ].uiID == 0 )						return	TRUE;
	else															return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬頂縑憮 嬴檜蠱擊 厥朝 嬪纂陛 嶸�褲捘� 匐餌и棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:56:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::IsValidItemPosInInventory( SI16 siPosInInventory )
{
	if( ( siPosInInventory >= 0 ) && ( siPosInInventory < ON_MYDATA_MAX_ITEM_NUM ) )	return	TRUE;
	else																				return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜醞檣 鼠晦陛 機斜溯檜萄 陛棟и雖 憲嬴螞棻(堅晝鼠晦檣陛?)
//	熱薑 橾濠	:	2003-3-31 螃瞪 17:33 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineFollower::CanWeaponBeUpgraded()
{
	return pCItem->IsUpgradable( Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM) );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜醞檣 鼠晦蒂 か薑 鼠晦煎 機斜溯檜萄п遽棻.
//	熱薑 橾濠	:	2003-3-31 螃瞪 17:35 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineFollower::UpgradeWeapon( UI16 uiItemID )
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, uiItemID ,0);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜醞檣 鼠晦蒂 梟嗑萼棻. (餉薯)
//	熱薑 橾濠	:	2003-3-31 螃瞪 17:38 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineFollower::BrakeWeapon()
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, 0 ,0);
}



//***********************************************************************************************
//	IOnlineFollower 掘褻羹
//***********************************************************************************************

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::IOnlineFollower()
{	
	Init();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	模資濠.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::~IOnlineFollower()
{	
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	滲熱菟曖 蟾晦�飛� 熱чж堅 в蹂и 詭賅葬蒂 й渡 嫡朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Init()
{	
	SI32	i;

	// 睡ж菟曖 等檜顫蒂 蟾晦�� и棻.
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		Followers[i].Init();
	} 
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜 贗楚蝶縑憮 餌辨ц湍 詭賅葬蒂 瞪睡 п薯и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Free()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱 幗ぷ 醞縑 綠橫氈朝 幗ぷ蒂 瓊朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFollower*		IOnlineFollower::GetFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// 褒薯 斜 蝸煜縑 睡ж陛 氈朝雖 獄棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱 幗ぷ 醞縑 綠橫氈朝 幗ぷ蒂 瓊朝棻.
//	熱薑 橾濠	:	2002-07-03 螃�� 3:34:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱 幗ぷ 醞縑 偽擎 嬴檜蠱檜 菟橫氈朝 幗ぷ蒂 瓊朝棻.
//	熱薑 橾濠	:	2002-07-03 螃�� 3:34:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	鼠啪陛 菟橫陞 熱 氈朝雖 �挫恉挬�.
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
//	撲貲	:	鼠啪蟾婁檣 辨煽檜 и貲檜塭紫 襄營ж罹 務晦 ��萇 鼻鷓檜賊 TRUE蒂 葬欐и棻.
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
//	撲貲	:	嬴檜蠱檜 氈朝雖 匐餌
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
			// 濰綠и Item
			siCount -= Followers[TempCount].Weapon.CheckItemCount(uiItemID);
			if(siCount <= 0) return TRUE;
			*/
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	陛雖堅 氈朝 Item曖 熱蒂 橢橫螞棻.
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

			// 濰綠и Item
			TempItemCount += Followers[TempCount].Weapon.CheckItemCount(uiItemID);
		}
	}

	return TempItemCount;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑 蹺陛и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;

	// 辦摹 晦襄曖 偽擎 嬴檜蠱檜 氈朝雖 憲嬴螞棻.
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->AddItem(pItem);
	}
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑 蹺陛и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 11:44:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower(uiFollowerSlot) ) == NULL )	return	ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	
	return lpFollower->AddItem( uiItemID, uiQuantity );	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑 蹺陛и棻.
//	熱薑 橾濠	:	2002-07-03 螃瞪 10:29:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory)
{
	MyItemData			ItemData;

	ItemData.uiID				=	uiItemID;
	ItemData.siPosInInventory	=	siPosInInventory;
	ItemData.uiQuantity			=	uiQuantity;

	return	AddItem(uiFollowerSlot, &ItemData);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑 蹺陛и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 11:28:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory)
{
	OnlineFollower*		lpFollower;

	// 辦摹 晦襄曖 偽擎 嬴檜蠱檜 氈朝雖 憲嬴螞棻.
	lpFollower = GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->AddItem(uiItemID, uiQuantity, psiPosInInventory);
	
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 餉薯и棻.	
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 餉薯и棻.				
//	熱薑 橾濠	:	2002-07-05 螃�� 4:15:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 餉薯и棻.
//	熱薑 橾濠	:	2002-07-07 螃瞪 11:36:23 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 橢橫螞棻
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
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 橢橫螞棻
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	IOnlineFollower::GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory<ON_MYDATA_MAX_ITEM_NUM)
		{
			// 1. 辦摹 輿橫霞 夠縑 嬴檜蠱檜 氈棻賊 斜勘 葬欐и棻.
			if(lpFollower->Items[siPosInInventory].uiID	==	uiItemID )
			{
				return &lpFollower->Items[siPosInInventory];
			}
		}
		// 2. 斜 嬪纂縑 斜 嬴檜蠱檜 橈棻賊, 偽擎 嬴檜蠱擊 瓊嬴獄棻.
		return lpFollower->GetItem(uiItemID);
	}
	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 ID蒂 餌辨ж罹 嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-03 螃瞪 10:15:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MyItemData*	IOnlineFollower::GetItemByID(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->GetItem(uiItemID);		
	else
		return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱曖 ID蒂 餌辨ж罹 嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-03 螃瞪 10:15:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::GetItemByPos( UI08 uiFollowerSlot, SI16 siItemPos, MyItemData **ppItemData )
{
	OnlineFollower*		lpFollower;

	lpFollower	=	GetFollower( uiFollowerSlot );
	
	if( !lpFollower )												return FALSE;

	if( lpFollower->GetItemByPos(siItemPos, ppItemData) == FALSE )	return FALSE;

	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 陛雖堅 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 10:25:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::HasItem( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity )
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	FALSE;

	return	lpFollower->HasItem( uiItemID, uiItemQuantity );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 陛雖堅 氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-14 螃瞪 10:25:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	撲貲	:	嬴檜蠱檜 氈朝雖 匐餌
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
//	撲貲	:	偽擎 嬴檜蠱檣雖 匐餌и棻.
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
//	撲貲	:	嬴檜蠱葬蝶お蒂 給溥遽棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData* IOnlineFollower::GetItemList(UI08 uiFollowerSlot)
{
	return (MyItemData*)Followers[uiFollowerSlot].Items;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 睡ж 蝸煜檣雖 匐餌и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::IsValidFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// 褒薯 斜 蝸煜縑 睡ж陛 氈朝雖 獄棻.
		if(Followers[uiFollowerSlot].uiCharID != 0)
			return TRUE;
	}
	return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж 議葛攪曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-10-29 螃�� 9:31:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

			// 嬴檜蠱擊 濰雜 衛鑑棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	睡ж蒂 蟾晦�� и棻.	
//	熱薑 橾濠	:	2002-07-08 螃瞪 12:30:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::InitFollower(OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	SI32						i;
	OnlineFollower*				lpFollower;
	SI16						siEmptyFollowerSlot;
	OnlineReceivedMyItemInfo	*pItemInfo;

	if((siEmptyFollowerSlot = FindEmptyFollowerSlot()) != -1)
	{
		// lpFollowerInfo 偌羹蒂 熱薑и棻.
		lpFollowerInfo->uiSlot	=	UI08(siEmptyFollowerSlot);

		if(InitFollower(lpFollowerInfo->uiSlot, lpFollowerInfo, Nation, pItem) == TRUE)
		{
			// 偌羹蒂 橢橫螞棻.
			lpFollower	=	&Followers[lpFollowerInfo->uiSlot];

			pItemInfo	=	(OnlineReceivedMyItemInfo*)((CHAR*)lpFollowerInfo + sizeof(OnFollowerInfo));

			// 嬴檜蠱檜 氈棻賊 嬴檜蠱擊 蹺陛衛麵遽棻.
			for(i = 0; i < lpFollowerInfo->uiItemNum; i++)			
				lpFollower->AddItem((MyItemData*)&pItemInfo[i]);							
			
			return	TRUE;
		}
	}

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж蒂 Update и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж 餉薯
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Get Follower Base Parameter Data
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Get Follower Parameter Data
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽曖 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-25 螃�� 3:00:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	IOnlineFollower::GetFollowerNum()
{	
	UI08	i;
	UI08	siTotalFollowerNum; 

	// 睡ж菟曖 等檜顫蒂 蟾晦�� и棻.
	siTotalFollowerNum	=	0;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsValidFollower(i) == TRUE)
			siTotalFollowerNum++;
	} 

	return	siTotalFollowerNum ;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱 薑爾蒂 橢橫螞棻
//	熱薑 橾濠	:	2002-05-02 螃�� 3:02:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::GetBattleInfo(UI08 uiFollowerSlot, OnImjinBattleFollowerInfo *pFollowerInfo)
{
	OnlineFollower		*pOnlineFollower;
	MyItemData			*pMyItemData;	
	SI32				i;
	DWORD				dwAllItemStructSize;
	

	pOnlineFollower	=	&Followers[uiFollowerSlot];	

	if(IsValidFollower(uiFollowerSlot) == TRUE)
	{
		// 蝸煜 盪濰.
		pFollowerInfo->uiSlotID		=	uiFollowerSlot;

		// 檜葷 犒餌.
		strncpy(pFollowerInfo->szPlayerName, pOnlineFollower->Name, ON_ID_LENGTH);
	
		pFollowerInfo->uiKind	=	pOnlineFollower->uiCharID;			

		// Parameta Box 犒餌.
		memcpy(&pFollowerInfo->Iparameta, &pOnlineFollower->ParametaBox.IP, sizeof(_ImportantParameta));

		// My Weapon 犒餌.
		memcpy(&pFollowerInfo->MW, &pOnlineFollower->Weapon, sizeof(MyWeapons));

		pMyItemData				=	(MyItemData*)((CHAR*)(pFollowerInfo) + sizeof(OnImjinBattleFollowerInfo));

		dwAllItemStructSize				=	0;
		pFollowerInfo->uiItemNum		=	0;
		for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		{
			if(pOnlineFollower->Items[i].uiID != 0)
			{
				*pMyItemData	=	pOnlineFollower->Items[i];
				
				// ん檣攪蒂 隸陛衛鑑棻.
				// MyItemData⑽檜嘎煎 sizeof(MyItemData) 虜躑 隸陛脹棻.
				pMyItemData++;

				pFollowerInfo->uiItemNum++;
			}
		}

		return	DWORD(sizeof(OnImjinBattleFollowerInfo) +  (pFollowerInfo->uiItemNum * sizeof(MyItemData)));
	}
	else
		return	0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱�� 餌辨й ん虜馬 熱纂 匐餌
//	熱薑 橾濠	:	2002-06-19 螃瞪 9 - 檜彌戮
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-05 螃�� 7:20:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-07 螃瞪 4:38:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(puiID, puiQuantity, uiItemNum);
	else				return FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-07 螃瞪 5:01:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	綠橫氈朝 辨煽 幗ぷ蒂 瓊朝棻.
//	熱薑 橾濠	:	2002-07-08 螃�� 5:01:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	IOnlineFollower::FindEmptyFollowerSlot()
{
	SI32				i;	

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		if(Followers[i].uiCharID == 0)	return	i;

	return	-1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽曖 掖羲 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-09 螃�� 1:59:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱 蟾晦��.
//	熱薑 橾濠	:	2002-07-12 螃�� 3:28:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineFollower::InitStaticVariable(CItem* pItem)
{
	OnlineFollower::InitStaticVariable(pItem);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 檜 議葛攪曖 瞪羹 嬴檜蠱曖 鼠啪蒂 橢橫螞棻.	
//	熱薑 橾濠	:	2002-07-12 螃�� 3:41:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::GetTotalItemWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetTotalItemWeight();
	else				return 0;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 議葛攪陛 譆堅 菟 熱 氈朝 鼠啪.
//	熱薑 橾濠	:	2002-07-12 螃�� 5:34:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	IOnlineFollower::GetMaxWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetMaxWeight();
	else				return 0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 議葛攪陛 譆堅 陛韓 熱 氈朝 嬴檜蠱曖 熱榆擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 5:51:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	IOnlineFollower::GetItemQuantityToTake(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetItemQuantityToTake(uiItemID);
	else				return 0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 蹺陛 衛鑒 熱 氈替傍?
//	熱薑 橾濠	:	2002-07-12 螃�� 7:58:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(uiItemID, uiQuantity);
	else				return FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽菟 醞縑 濠晦陛 熱辨й 熱 氈朝匙 爾棻 嬴檜蠱擊 蟾婁 п憮 萄朝雖 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 8:21:02 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	か薑 辨煽檜 濠晦陛 熱辨й 熱 氈朝匙 爾棻 嬴檜蠱擊 蟾婁 п憮 萄朝雖 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 8:22:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::IsItemWeightOverflowed(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->IsItemWeightOverflowed();
	else				return 0;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜ж堅 氈朝 嬴檜蠱擊 慇朝棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 9:16:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:15:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濰雜脹 嬴檜蠱菟曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 2:54:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::GetWearItems(UI08 uiFollowerSlot, UI16 puiWearItems[WEAR_ITEM_NUM])
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetWearItems(puiWearItems);
	else				return FALSE;			
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 濰雜и棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 12:43:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::SetWearItem2(UI08 uiFollowerSlot, UI08 uiLoc, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->SetWearItem(uiLoc, uiItemID,OnlineFollower::pCItem->GetItemUsingDay(uiItemID));
	else				return FALSE;				
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 3:58:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanPutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanPutOnItem(uiWearLoc, uiItemID, siPosInInventory, siTradeGrade, ErrorCode);
	else				return FALSE;				
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檣漸饜葬縑 氈朝 嬴檜蠱擊 濰雜й 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 3:59:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanTakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanTakeOffItem(uiWearLoc, uiItemID, siPosInInventory);
	else				return FALSE;					
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 檜翕 衛鑒 熱 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:30:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanMoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{
	OnlineFollower		*pFollower1;
	OnlineFollower		*pFollower2;
	

	if( ( pFollower1 = GetFollower( uiFollowerSlot1 ) ) == NULL )										return	FALSE;
	if( ( pFollower2 = GetFollower( uiFollowerSlot2 ) ) == NULL )										return	FALSE;

	if( pFollower1 != pFollower2 )
	{
		// 嬴檜蠱擊 陛雖堅 氈朝雖 匐餌и棻.
		if( pFollower1->HasItem( uiItemID, uiItemQuantity ) == FALSE )									return	FALSE;

		// 嬴檜蠱擊 嫡朝 議葛攪陛 嬴檜蠱擊 陛韓 熱 氈朝雖 匐餌и棻.
		if( pFollower2->CanAddItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )				return	FALSE;
	}
	else
	{
		if( pFollower1->CanMoveItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )			return	FALSE;
	}


	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 檜翕 衛鑑棻.
//	熱薑 橾濠	:	2002-11-12 螃瞪 10:24:46 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
		// 嬴檜蠱擊 餉薯и棻.
		if( pFollower1->DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )					return	FALSE;

		// 嬴檜蠱擊 蹺陛и棻.		
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	辨煽 蝶邐擊 爾傘蝶 熱纂煎 夥羞棻.
//	熱薑 橾濠	:	2002-12-22 - 檜遽戮
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser *pCharParser, UI16 uiFollowerID , SI32 *pBonus, CItem* pItem)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE)  return FALSE;

	SI32 BaseStr, BaseInt, BaseDex, BaseVit, BaseHP, BaseMP, BaseTot;
	SI32 CurStr, CurInt, CurDex, CurVit, CurBonus, CurTot;
	SI32 TotBonus;

	// 辨煽曖 晦獄熱纂
	BaseStr = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetStr();
	BaseInt = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetInt();
	BaseDex = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetDex();
	BaseVit = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetVit();
	BaseHP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetLife();
	BaseMP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetMana();
	BaseTot = BaseStr + BaseInt + BaseDex + BaseVit;

	// 辨煽曖 ⑷營熱纂
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

	// 辨煽 晦獄熱纂煎 蝶邐 蟾晦��
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	機斜溯檜萄 陛棟и 鼠晦檣雖 憲嬴螞棻. (堅晝鼠晦)
//	熱薑 橾濠	:	2003-3-31 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineFollower::CanWeaponBeUpgraded( UI16 uiFollowerSlot )
{
	return Followers[uiFollowerSlot].CanWeaponBeUpgraded();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜 鼠晦 嬴檜蠱擊 か薑 嬴檜蠱戲煎 機斜溯檜萄 衛麵遽棻.
//	熱薑 橾濠	:	2003-3-31 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineFollower::UpgradeWeapon( UI16 uiFollowerSlot, UI16 uiItemID )
{
	Followers[uiFollowerSlot].UpgradeWeapon( uiItemID );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰雜醞檣 鼠晦 嬴檜蠱擊 だ惚衛鑑棻.
//	熱薑 橾濠	:	2003-3-31 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineFollower::BrakeWeapon( UI16 uiFollowerSlot )
{
	Followers[uiFollowerSlot].BrakeWeapon();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 辨煽菟曖 羹溘擊 100% �蛹厭藥捘奡�.
//	熱薑 橾濠	:	2003-09-18 16:28 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	賅萇 辨煽菟曖 葆釭蒂 100% �蛹厭藥捘奡�.
//	熱薑 橾濠	:	2003-09-18 16:33 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
