#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대에 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvChargeBooth()
{
	OnReqChargeBoothMsg				*pChargeBoothMsg;
	stBOOTH							*pBooth;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pChargeBoothMsg					=	(OnReqChargeBoothMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(pChargeBoothMsg->uiBoothID);

	if( pBooth )
	{
		if( pBooth->EnterBooth( this ) == TRUE )
		{
			dwResult	=	ON_RET_MARKETPLACE_OK;
		}
		else
			dwResult	=	ON_RET_MARKETPLACE_PLAYER;
	}
	SendFieldMsg(ON_RESPONSE_CHARGEBOOTH,SO_SFM_ONLYME, LPVOID(dwResult));
	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대를 나간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvLeaveBooth()
{
	OnReqLeaveBoothMsg				*pLeaveBoothMsg;
	stBOOTH							*pBooth;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pLeaveBoothMsg					=	(OnReqLeaveBoothMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(pLeaveBoothMsg->uiBoothID);

	if( pBooth )
	{
		if( pBooth->LeaveBooth(this) == TRUE )
			dwResult	=	ON_RET_MARKETPLACE_OK;
	}
	SendFieldMsg(ON_RESPONSE_LEAVEBOOTH,SO_SFM_ONLYME, LPVOID(dwResult),LPVOID(pBooth));

	if( dwResult == ON_RET_MARKETPLACE_OK)	//	주변사람들에게 좌판대 아이템을 보낸다.( SO_SFM_ONLYNEARCHAR )
		SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_MARKETPLACE_EMPTY), LPVOID(pLeaveBoothMsg->uiBoothID) );
	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	(좌판대를 표현하기위해) 좌판대의 아이템을 받는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvDisplayBooth()
{
	OnReqDisplayBoothItemMsg		*pDisplayBoothItemMsg;
	stBOOTH							*pBooth;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pDisplayBoothItemMsg			=	(OnReqDisplayBoothItemMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(	pDisplayBoothItemMsg->uiBoothID );
	
	if( pBooth )
	{
		if(pBooth->pSeller)
		{
			dwResult	=	ON_RET_MARKETPLACE_OK;
		}
		else
			dwResult	=	ON_RET_MARKETPLACE_EMPTY;
	}
	SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH, SO_SFM_ONLYME, LPVOID(dwResult), LPVOID(pDisplayBoothItemMsg->uiBoothID), LPVOID(pBooth));
	return;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대에 아이템을 놓는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvSetBoothItem()
{
	OnReqSetBoothItemMsg			*pSetBoothItemMsg;
	stBOOTH							*pBooth;
	MyItemData						*pItemData;
	OnBoothItem						*pBoothItem;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pSetBoothItemMsg			=	(OnReqSetBoothItemMsg*) cMsg;
	pBooth		=	pMyWorld->GetBooth(	pSetBoothItemMsg->uiBoothID);
	pBoothItem	=	(OnBoothItem*)( (CHAR*)pSetBoothItemMsg + sizeof(OnReqSetBoothItemMsg));
	if(pBooth)
	{
		if( pBooth->pSeller == this )
		{
			if( pSetBoothItemMsg->uiNum <= ON_MAX_BOOTH_ITEM )
			{
				for(int i=0; i<pSetBoothItemMsg->uiNum; i++)
				{			
					pItemData	=	IFollower.GetItem(pBoothItem[i].uiFollowerID, pBoothItem[i].uiItemID, pBoothItem[i].uiInvenPos);
					
					if( pItemData )
					{
						if( pBooth->SetItem(i, pBoothItem[i].uiFollowerID, pItemData, pBoothItem[i].siPrice) == TRUE )
						{
							if(pBoothItem[i].Desc[0] != '\0' )
								pBooth->SetNote(i, pBoothItem[i].Desc);
							dwResult = ON_RET_MARKETPLACE_OK;
						}
						else
						{
							dwResult = ON_RET_MARKETPLACE_NO;
							pBooth->DelItemAll();
							break;
						}
					}
				}
			}
		}
	}
	SendFieldMsg(ON_RESPONSE_SETBOOTHITEM, SO_SFM_ONLYME, LPVOID(dwResult));

	if( dwResult == ON_RET_MARKETPLACE_OK)	//	주변사람들에게 좌판대 아이템을 보낸다.( SO_SFM_ONLYNEARCHAR )
		SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH, SO_SFM_MEANDNEARCHAR, LPVOID(dwResult), LPVOID(pSetBoothItemMsg->uiBoothID), LPVOID(pBooth));
	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대의 아이템들을 다시 걷어 들인다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvCancelBoothItem()
{
	OnReqCancelBoothItemMsg			*pCancelBoothItemMsg;
	stBOOTH							*pBooth;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pCancelBoothItemMsg			=	(OnReqCancelBoothItemMsg*) cMsg;
	pBooth		=	pMyWorld->GetBooth(	pCancelBoothItemMsg->uiBoothID);

	if( pBooth )
	{
		if( pBooth->pSeller == this )
		{
			if( pBooth->DelItem(pCancelBoothItemMsg->uiNum) == TRUE )
				dwResult = ON_RET_MARKETPLACE_OK;
		}
	}
	SendFieldMsg(ON_RESPONSE_CANCELBOOTHITEM, SO_SFM_ONLYME, LPVOID(dwResult));

	if( dwResult == ON_RET_MARKETPLACE_OK)	//	주변사람들에게 좌판대 아이템을 보낸다.( SO_SFM_ONLYNEARCHAR )
		SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH, SO_SFM_MEANDNEARCHAR, LPVOID(dwResult), LPVOID(pCancelBoothItemMsg->uiBoothID), LPVOID(pBooth));
	return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대의 아이템을 클릭했을때, 아이템을 사기전에 보여주는 정보 메세지
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBoothInfo()
{
	OnReqGetBoothInfoMsg			*pGetBoothInfoMsg;
	stBOOTH							*pBooth;
	stBOOTHITEM						*pBoothItem;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;
	SI32							siNum	=	0;
	OnBoothItemInfo					BoothItemInfo[ON_MAX_BOOTH_ITEM];

	pGetBoothInfoMsg			=	(OnReqGetBoothInfoMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(	pGetBoothInfoMsg->uiBoothID );
	
	if( pBooth )
	{
		if(pBooth->pSeller)
		{
			for(int i=0; i<ON_MAX_BOOTH_ITEM; i++)
			{
				pBoothItem		=	pBooth->GetBoothItem(i);
				if(pBoothItem)
				{
					if( pBoothItem->pItem )
					{
						BoothItemInfo[siNum].uiItemID	=	pBoothItem->pItem->uiID;
						BoothItemInfo[siNum].uiQuantity	=	pBoothItem->pItem->uiQuantity;
						BoothItemInfo[siNum].siPrice	=	pBoothItem->siPrice;
						memcpy(BoothItemInfo[siNum].Note, pBoothItem->Note, ON_MAX_BOOTH_NOTE);
						siNum++;
						dwResult	=	ON_RET_MARKETPLACE_OK;
					}
				}
			}
		}
		else
		{
			dwResult	=	ON_RET_MARKETPLACE_EMPTY;
		}
	}
	SendFieldMsg(ON_RESPONSE_GETBOOTHINFO, SO_SFM_ONLYME, LPVOID(dwResult), LPVOID(BoothItemInfo), LPVOID(siNum));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대에서 아이템을 거래한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvBuyBoothItem()
{	
	OnReqBuyBoothItemMsg			*pBuyBoothItemMsg;
	MONEY							TotalMoney;
	MyItemData						TempItem;
	stBOOTH							*pBooth;
	stBOOTHITEM						*pBoothItem;
	BOOL							bBuyerProcess	= FALSE;
	BOOL							bSellorProcess	= FALSE;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pBuyBoothItemMsg			=	(OnReqBuyBoothItemMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(	pBuyBoothItemMsg->uiBoothID );	
	if( pBooth )
	{
		pBoothItem		=	pBooth->SearchBoothItem(pBuyBoothItemMsg->uiItemID);
		if( pBoothItem && pBooth->pSeller)
		{
			if( pBoothItem->pItem)
			{
				memcpy(&TempItem, pBoothItem->pItem, sizeof(MyItemData));
				if( pBoothItem->pItem->uiQuantity >= pBuyBoothItemMsg->uiQnt )
				{
					TotalMoney		=	pBuyBoothItemMsg->uiQnt * pBoothItem->siPrice;
					if( siMoney >= TotalMoney)
					{
						if( AddItem(pBuyBoothItemMsg->uiFollowerID, pBoothItem->pItem->uiID, pBuyBoothItemMsg->uiQnt, pBuyBoothItemMsg->uiPos) != NULL )
						{
							bBuyerProcess		=	TRUE;
							if( pBooth->pSeller->DelItem(pBoothItem->FollowerID, pBoothItem->pItem->uiID, pBuyBoothItemMsg->uiQnt, pBoothItem->pItem->siPosInInventory) != NULL )
							{
								TempItem.uiQuantity	-=	pBuyBoothItemMsg->uiQnt;
								bSellorProcess	= TRUE;
							}
							else
							{
								DelItem(pBuyBoothItemMsg->uiFollowerID, pBoothItem->pItem->uiID, pBuyBoothItemMsg->uiQnt, pBuyBoothItemMsg->uiPos);
							}
							if(bBuyerProcess && bSellorProcess)
							{
								dwResult = ON_RET_MARKETPLACE_OK;
								DecreaseMoney(TotalMoney);
								pBooth->pSeller->IncreaseMoney(TotalMoney);
								pBooth->pSeller->SendFieldMsg(ON_RESPONSE_SELLBOOTHITEM, SO_SFM_ONLYME, LPVOID(pBooth), LPVOID(pBoothItem), LPVOID(&TempItem));
								if( pBoothItem->pItem->uiQuantity <= 0)
								{
									pBoothItem->Init();
									SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH, SO_SFM_MEANDNEARCHAR, LPVOID(ON_RET_MARKETPLACE_OK), LPVOID(pBuyBoothItemMsg->uiBoothID), LPVOID(pBooth));
								}
							}
						}
					}
				}
			}
		}
	}
	SendFieldMsg(ON_RESPONSE_BUYBOOTHITEM, SO_SFM_ONLYME, LPVOID(dwResult));
}
/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대의 아이템들에 대한 설명만 입력  (지금은 않쓰는 함수)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvInputBoothDesc()
{
	OnReqInputBoothDescMsg			*pInputBoothDescMsg;
	stBOOTH							*pBooth;
	CHAR							*pNote;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;

	pInputBoothDescMsg			=	(OnReqInputBoothDescMsg*)	cMsg;
	pBooth		=	pMyWorld->GetBooth(	pInputBoothDescMsg->uiBoothID);
	pNote		=	(CHAR*)pInputBoothDescMsg + sizeof(OnReqInputBoothDescMsg);
	
	if( pBooth )
	{
		if( pBooth->pSeller == this )
		{			
			if( pBooth->SetNote(pInputBoothDescMsg->uiNum, pNote, pInputBoothDescMsg->uiLength) == TRUE )
				dwResult = ON_RET_MARKETPLACE_OK;
		}
	}
	SendFieldMsg(ON_RESPONSE_INPUTBOOTH_DESC, SO_SFM_ONLYME, LPVOID(dwResult));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	좌판대의 아이템들에 대한 설명만 받음  (지금은 않쓰는 함수)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	SOPlayer::RecvGetBoothDesc()
{
	OnReqGetBoothItemDescMsg		*pGetBoothItemDescMsg;
	stBOOTH							*pBooth;
	DWORD							dwResult = ON_RET_MARKETPLACE_NO;


	pGetBoothItemDescMsg			=	(OnReqGetBoothItemDescMsg*) cMsg;
	pBooth		=	pMyWorld->GetBooth(	pGetBoothItemDescMsg->uiBoothID );
	
	if( pBooth )
	{
		if(pBooth->pSeller)
		{
			dwResult	=	ON_RET_MARKETPLACE_OK;
		}
		else
			dwResult	=	ON_RET_MARKETPLACE_EMPTY;
	}
	SendFieldMsg(ON_RESPONSE_GETBOOTHDESC, SO_SFM_ONLYME, LPVOID(dwResult), LPVOID(pGetBoothItemDescMsg), LPVOID(pBooth));
	return;
}
*/
/*
	OnReqAuditVillageInfoMsg		*pAuditVillageInfoMsg;
	DBIKAuditVillageInfo			stAuditVillageInfo;

	pAuditVillageInfoMsg			=	(OnReqAuditVillageInfoMsg*)	cMsg;

	stAuditVillageInfo.pOwner		=	this;
	stAuditVillageInfo.uiVillageID	=	pAuditVillageInfoMsg->uiVillageID;
	
	if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_VILLAGEINFO, LPVOID(&stAuditVillageInfo), LPVOID(&stAuditVillageInfo)) == TRUE )
	{
		SendFieldMsg(ON_RESPONSE_AUDIT_VILLAGE_INFO,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK),LPVOID(&stAuditVillageInfo));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_AUDIT_VILLAGE_INFO,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));
	}

*/