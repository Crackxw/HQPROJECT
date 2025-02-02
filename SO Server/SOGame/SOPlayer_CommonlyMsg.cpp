#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 檜翕 衛鑑棻.
//	熱薑 橾濠	:	2002-11-12 螃�� 2:20:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvShareItem()
{
	OnReqShareItemMsg	*pReqShareItem;
	DWORD				dwResponse = ON_RET_NO;
	BOOL				bSuccess;

	pReqShareItem	=	( OnReqShareItemMsg* )cMsg;		

	bSuccess		=	MoveItem( pReqShareItem->uiFollowerID, pReqShareItem->uiItemID, pReqShareItem->uiItemQuantity, 
												pReqShareItem->uiReceiverID, pReqShareItem->uiNewPosInInventory );
		

	if( bSuccess == TRUE )
	{
		SendFieldMsg( ON_RESPONSE_MOVEITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_MOVEITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	遴艙濠 貲滄 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvCommand()
{
	CHAR			szCommand[4096];
	OnCommandMsg	*pCommandMsg;
	CHAR			*pCommandString;
	CHAR			szCommandError[ 1024 ];
	
	pCommandMsg		=	(OnCommandMsg*)cMsg;
	pCommandString	=	(CHAR*)pCommandMsg + sizeof(OnCommandMsg);

	ZeroMemory(szCommand, sizeof(szCommand));
	strncpy(szCommand, pCommandString, pCommandMsg->siStringLength);

	// 遴艙濠歜檜 �挫庰Ц�棻.
	if( pMyWorld->clCommand.ProcessCMD(uiAccount, szCommand) == FALSE )
	{
		ZeroMemory( szCommandError, sizeof( szCommandError ) );

		sprintf( szCommandError, "Command Error : %s", szCommand );

		SendFieldMsg( ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID( ON_RET_TEXTMSG_WHISPER ), LPVOID( szCommandError ) );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	爾傘蝶蒂 餌辨ж罹 棟溘纂蒂 螢萼棻朝 詭衛雖蒂 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvIncAbilityByBonus()
{
	OnReqIncAbilityByBonusMsg	*pIncAbilityByBonusMsg;

	pIncAbilityByBonusMsg	=	(OnReqIncAbilityByBonusMsg*)cMsg;

	if(IFollower.Bonus_FollowerParameterUp(pIncAbilityByBonusMsg->uiFollowerID, pIncAbilityByBonusMsg->AbilityKind, 1, &pMyWorld->clItem) == TRUE)
	{
		// 爾傘蝶蒂 餌辨ж罹 棟溘纂蒂 螢葬朝等 撩奢ц棻.
		SendFieldMsg(ON_RESPONSE_INC_ABILITY_BYBONUS, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		// 爾傘蝶蒂 餌辨ж罹 棟溘纂蒂 螢葬朝等 褒ぬц棻.
		SendFieldMsg(ON_RESPONSE_INC_ABILITY_BYBONUS, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嘐棲裘縑憮 Ы溯檜橫曖 薑爾蒂 蹂掘и棻.
//	熱薑 橾濠	:	2002-04-12 螃�� 5:47:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvReqPIInMinimap()
{
	OnReqPIInMiniMapMsg		*pRPIInMiniMapMsg;
	SOPlayer				*pPlayer;
	CHAR					szName[ON_ID_LENGTH + 1];

	pRPIInMiniMapMsg	=	(OnReqPIInMiniMapMsg*)cMsg;

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	ZeroMemory(szName, sizeof(szName));
	strncpy(szName, pRPIInMiniMapMsg->szName, ON_ID_LENGTH);
	pPlayer				=	pMyWorld->GetPlayer(szName);

	if(pPlayer != NULL)
	{
		// Ы溯檜橫陛 蕾樓п 氈棻.	
		switch(pPlayer->siStatus)
		{
		case ON_PLAYER_STATUS_INFIELD:
			// в萄縑 氈棻.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_ON_FIELD), LPVOID(pPlayer));
			break;
		case ON_PLAYER_STATUS_INVILLAGE:
			// 葆擊寰縑 氈棻.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_IN_VILLAGE), 
						LPVOID(pPlayer), LPVOID(pPlayer->stAccountInfo.pVillage));
			break;
		case ON_PLAYER_STATUS_INBATTLE:
			// 瞪癱醞檜棻.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_IN_BATTLE), LPVOID(pPlayer));
			break;
		}
	}
	else
	{
		// Ы溯檜橫陛 蕾樓ж雖 彊擎 鼻鷓檜棻.
		SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_NOT_CONNECTED));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	か薑 葆擊曖 薑爾蒂 蹂羶и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 12:59:39 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvRequestVillageInfo()
{
	OnReqVillageInfoMsg		*pReqVillageInfoMsg;
	SOVillage				*pVillage;
	

	pReqVillageInfoMsg		=	(OnReqVillageInfoMsg*)cMsg;
	
	if( ( pVillage = pMyWorld->GetVillageByCode( pReqVillageInfoMsg->uiVillageCode ) ) != NULL)
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_OK ), LPVOID( pVillage ) );
	}	
	else
	{
		SendFieldMsg( ON_RESPONSE_VILLAGE_INFO, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	評塭棻棲朝 辨煽 Change
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvVMercenaryChange(SI08 siSlot)
{
	if( siSlot < 0 )					siSlot = 0;
	if( siSlot >= ON_MAX_FOLLOWER_NUM ) siSlot = 0;

	if(siVMercenarySlot != siSlot)
	{
		siVMercenarySlot = siSlot;

		UpdateVMercenary();
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	評塭棻棲朝 辨煽 Change
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::UpdateVMercenary(void)
{
	SI32		Kind;

	Kind = 0;
	if(siVMercenarySlot > 0)
	{
		if(IFollower.IsValidFollower(siVMercenarySlot))
		{
			Kind = IFollower.GetFollowerParameterKind(siVMercenarySlot);
		}
	}

	SendFieldMsg(ON_RESPONSE_VMERCENARY_CHANGE,     SO_SFM_MEANDNEARCHAR, LPVOID(Kind));
	SendFieldMsg(ON_RESPONSE_VMERCENARY_CHANGEMAIN, SO_SFM_ONLYME,        LPVOID(siVMercenarySlot));
}


VOID	SOPlayer::RecvRefineItem()
{
	OnRequestRefineItem	*pMsg = (OnRequestRefineItem	*)cMsg;

	CSmith::RecvSmithItem(this,pMsg->uiFollowerSlot,pMsg->uiItemID);
	

}
