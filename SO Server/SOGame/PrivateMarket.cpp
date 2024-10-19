#include "SOMain.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-07-02 螃�� 8:10:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltPrivateMarket::Init(SOPlayer *pOwnerPlayer)
{
	m_pOwnerPlayer	=	pOwnerPlayer;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-07-03 螃�� 7:29:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltPrivateMarket::Init()
{
	SI32	i;

	m_bOpen				=	FALSE;
	
	// �公籀� 氈擊雖 賅腦朝 賅萇 嬴檜蠱擊 雖遴棻.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄擊 翱棻.
//	熱薑 橾濠	:	2002-07-03 螃�� 7:27:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltPrivateMarket::Open()
{
	m_bOpen	=	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄擊 殘朝棻.
//	熱薑 橾濠	:	2002-07-03 螃�� 7:27:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltPrivateMarket::Close()
{
	SI32	i;	

	// 賅萇 嬴檜蠱擊 薯剪и棻.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);

	m_bOpen	=	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� 蝸煜檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 7:48:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltPrivateMarket::IsValidSlot(UI08 uiSlot)
{
	if(uiSlot >= 0 && uiSlot < ON_MAX_BOOTH_ITEM)	return	TRUE;
	else											return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄檜 �側瘓� 腎歷朝陛?
//	熱薑 橾濠	:	2002-07-03 螃�� 7:28:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltPrivateMarket::IsOpen()
{
	return m_bOpen;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑 嬴檜蠱擊 蹺陛и棻. ( 蹺陛 : 偃檣 鼻薄縑 辨煽擊 蹺陛и棻. �醴� 勒僭 剪楚蒂 嬪и 遽綠紫 渠楞擎 п 場懊棻.)
//	熱薑 橾濠	:	2002-07-02 螃�� 7:50:31 - 曄薑賅 ( 檜鼻團 : 2003-03-14 螃瞪 11 : 45 )
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//BOOL	cltPrivateMarket::AddItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote)
BOOL	cltPrivateMarket::AddItem( OnBoothItemForSell	*pBoothItem )
{	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 衛濛 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
	if ( ! IsValidSlot (pBoothItem->uiBoothSlot ) )
	{
		return FALSE;
	}

	// 緒艇 薯橫蒂 嬪п ん檣攪蒂 盪濰и棻.
	strPMItem *pstPMItem;

	pstPMItem	=	&m_stPMItem[pBoothItem->uiBoothSlot];

	// п渡 謝っ擎 奩萄衛 綠錶 氈橫撿 и棻.
	//п渡 謝っ縑 item 檜釭 辨煽檜 氈朝 雖 羹觼 и棻.
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
// 檜 鼻 團 熱薑 ( 2003. 3. 14 )
// 辨煽 剪楚 睡碟 蹺陛 部 睡碟
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	/*
	strPMItem *pstPMItem;

	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// 緒艇 薯橫蒂 嬪п ん檣攪蒂 盪濰и棻.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// п渡 謝っ擎 奩萄衛 綠錶 氈橫撿 и棻.
	if(pstPMItem->uiItemID != 0)
	{
		return	FALSE;
	}

	// 謝っ擊 瓣錶 厥朝棻.
	pstPMItem->uiFollowerID		=	uiFollowerID;
	pstPMItem->Price			=	Price;

	// 撲貲擊 盪濰и棻.
	ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
	trncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

	// 謝っ曖 嬴檜蠱擊 瓣錶 厥朝棻.	
	pstPMItem->uiItemID	=	uiItemID;
	*/

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 嬴檜蠱擊 餉薯и棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 7:58:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltPrivateMarket::DelItem(UI08 uiSlotID)
{
	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
	if( IsValidSlot(uiSlotID) == FALSE )
	{
		return	FALSE;
	}

	ZeroMemory(&m_stPMItem[uiSlotID], sizeof(strPMItem));	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 嬴檜蠱擊 骯棻. ( 辨煽擊 掘殮ж朝 匙擎 檜 л熱蒂 噙雖 彊朝棻.)
//	熱薑 橾濠	:	2002-07-02 螃�� 8:00:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltPrivateMarket::BuyItem(UI08 uiSlotID, UI16 uiQuantity, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *psiFromMercenarySlot)
{
	strPMItem		*pstPMItem;		
	MyItemData		*pMyItem;

	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// 緒艇 薯橫蒂 嬪п ん檣攪蒂 盪濰и棻.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// っ衙ж朝 嬴檜蠱檣雖 匐餌и棻.
	if(pstPMItem->uiItemID == 0)
	{
		return	FALSE;
	}

	// 嬴檜蠱曖 薑爾蒂 橢橫螞棻.	
	if((pMyItem	= m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) == NULL)
	{
		return	FALSE;
	}

	// 髦溥堅 ж朝 熱榆婁 つ溥堅 ж朝 熱榆檜 蜃朝雖 匐餌и棻.
	if(pMyItem->uiQuantity < uiQuantity)
	{
		return	FALSE;
	}

	// 嬴檜蠱擊 貍遽棻.
	if(m_pOwnerPlayer->DelItemNoQuery(pstPMItem->uiFollowerID, pstPMItem->uiItemID, uiQuantity) == FALSE)
	{
		return	FALSE;
	}
	
	// ⑷營 陴擎 熱榆婁 陛問擊 憲溥遽棻.
	*puiRemainQuantity	=	pMyItem->uiQuantity;
	*pTotalItemPrice	=	pstPMItem->Price * uiQuantity;
	*psiFromMercenarySlot = pstPMItem->uiFollowerID;
	

	// ⑷營 陴擎 熱榆檜 0 檜塭賊 鏃晝ж朝 嬴檜蠱 跡煙縑憮 雖遴棻.
	if(*puiRemainQuantity == 0)	
	{
		DelItem(uiSlotID);
	}

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 っ衙ж朝 嬴檜蠱擊 夥羞棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 8:29:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltPrivateMarket::ChangeItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote, UI08	uiTradeFollowerID)
{
	strPMItem	*pstPMItem;

	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
	if( ! IsValidSlot(uiSlotID) )
	{
		return	FALSE;
	}
	
	// 緒艇 薯橫蒂 嬪п ん檣攪蒂 盪濰и棻.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	if ( uiItemID )
	{
		pstPMItem->siType = LIST_ITEM;
		// 謝っ擊 瓣錶 厥朝棻.
		pstPMItem->uiFollowerID		=	uiFollowerID;
		pstPMItem->Price			=	Price;

		// 撲貲擊 盪濰и棻.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

		// 謝っ曖 嬴檜蠱擊 瓣錶 厥朝棻.	
		pstPMItem->uiItemID	=	uiItemID;
	}
	else if ( uiTradeFollowerID )
	{
		pstPMItem->siType = LIST_FOLLOWER;
		// 謝っ曖 辨煽擊 瓣錶 厥朝棻.	
		pstPMItem->uiTradeFollowerSlotID = uiTradeFollowerID;
		pstPMItem->Price = Price;

		// 撲貲擊 盪濰и棻.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);
	}
	else
	{
		return FALSE;
	}

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 っ衙ж朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-02 螃�� 8:57:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strPMItem*	cltPrivateMarket::GetItem(UI08 uiSlotID)
{
	// 蝸煜曖 ID陛 嶸�褲捘� 匐餌и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 っ衙ж朝 嬴檜蠱曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-11 螃�� 2:47:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 ⑷營 っ衙ж朝 嬴檜蠱菟擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-03 螃�� 9:06:08 - 曄薑賅
//  2離 熱薑    :   2003-03-17 螃瞪 3:00:00 - 檜鼻團 (辨煽縑 渠и 薑爾紫 瞪歎п邀)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltPrivateMarket::GetBaseItems(OnBaseBoothItem *pBaseBoothItem, SI08 *psiItemNum)
{
	DWORD		dwUsedSize;
	SI32		i;
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	
	// 舒 高擊 蟾晦�倆挬�.
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
			

			// 嶸�褲� 蝸煜檜棻.
			siItemNum++;			// 嬴檜蠱擊 ж釭 隸陛衛鑑棻.			
		}
	}

	dwUsedSize	=	sizeof(OnBaseBoothItem) * siItemNum;

END:	
	*psiItemNum	=	siItemNum;

	return	dwUsedSize;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 ⑷營 っ衙ж朝 嬴檜蠱菟擊 橢橫螞棻.	
//	熱薑 橾濠	:	2002-07-05 螃瞪 11:08:55 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltPrivateMarket::GetItems(OnBoothItemInfo *pBoothItem, SI08 *psiItemNum, SOPlayer* pPlayer, MyItemData *pFollowerItems, UI16 &numFollowerItems)
{
	DWORD		dwUsedSize;
	SI32		i;	
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	MyItemData	*pItemData;
	MyItemData	*pCurFollowerItem	=	pFollowerItems;
	
	// 舒 高擊 蟾晦�倆挬�.
	siItemNum	=	-1;
	dwUsedSize	=	0;

	numFollowerItems	=	0;	// 霤褻煎 給溥還 高, Booth縑 螢塭螞 賅萇 辨煽曖 MyItemData菟曖 偃熱曖 м(模雖и 嬴檜蠱)

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

						ZeroMemory( &pBoothItem[siItemNum].Follower, sizeof(OnFollowerInfo) );		// 辨煽 薑爾 橈擠

						ZeroMemory( pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note) );
						strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);

						// 嶸�褲� 蝸煜檜棻.
						siItemNum++;			// 嬴檜蠱擊 ж釭 隸陛衛鑑棻.			
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
					pPlayer->GetFollowerInfo( pstPMItem->uiTradeFollowerSlotID, (OnFollowerInfo*)cFollowerInfoBuf );	// 辨煽 薑爾 嫡嬴螃晦
					memcpy( &pBoothItem[siItemNum].Follower, cFollowerInfoBuf, sizeof(OnFollowerInfo) );	// 辨煽 薑爾 瓣辦晦

					SI32	numItem		=	pBoothItem[siItemNum].Follower.uiItemNum;			// 辨煽檜 乾堅 氈朝 嬴檜蠱 偃熱(濰雜 嬴檜蠱 諼)
					memcpy( pCurFollowerItem, cFollowerInfoBuf + sizeof(OnFollowerInfo), sizeof(MyItemData) * numItem );	// 辨煽曖 嬴檜蠱 薑爾 瓣辦晦
					pCurFollowerItem	+=	numItem;
					numFollowerItems	+=	numItem;

					ZeroMemory(pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note));
					strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);
					
					siItemNum++;			// 嬴檜蠱擊 ж釭 隸陛衛鑑棻.			

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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 っ衙ж堅 氈朝 嬴檜蠱曖 熱榆擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-05 螃�� 8:44:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 熱榆擊 盪濰и棻.
	*puiQuantity	=	pItemData->uiQuantity;

	bSuccess	=	TRUE;
END:
	return	bSuccess;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	偃檣 鼻薄縑憮 嬴檜蠱擊 髦 熱 氈朝陛?	
//	熱薑 橾濠	:	2002-07-05 螃�� 9:24:47 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

// 偃檣 鼻薄縑憮 っ衙ж堅 氈朝 嬴檜蠱曖 陛問擊 憲嬴螞棻.
// 辨煽橾 唳辦縑朝 uiQuantity朝 鼠衛脹棻.
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
		// 辨煽橾 唳辦縑朝 uiQuantity朝 鼠衛脹棻.
		*pPrice		=	pstPMItem->Price;
		break;


	default:
		return	FALSE;

	}

	return	TRUE;
}
