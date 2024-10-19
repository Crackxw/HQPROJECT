#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �̵� ��Ų��.
//	���� ����	:	2002-11-12 ���� 2:20:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ��� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// ������� Ȯ�εǾ���.
	if( pMyWorld->clCommand.ProcessCMD(uiAccount, szCommand) == FALSE )
	{
		ZeroMemory( szCommandError, sizeof( szCommandError ) );

		sprintf( szCommandError, "Command Error : %s", szCommand );

		SendFieldMsg( ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID( ON_RET_TEXTMSG_WHISPER ), LPVOID( szCommandError ) );
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ʽ��� ����Ͽ� �ɷ�ġ�� �ø��ٴ� �޽����� �޾Ҵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvIncAbilityByBonus()
{
	OnReqIncAbilityByBonusMsg	*pIncAbilityByBonusMsg;

	pIncAbilityByBonusMsg	=	(OnReqIncAbilityByBonusMsg*)cMsg;

	if(IFollower.Bonus_FollowerParameterUp(pIncAbilityByBonusMsg->uiFollowerID, pIncAbilityByBonusMsg->AbilityKind, 1, &pMyWorld->clItem) == TRUE)
	{
		// ���ʽ��� ����Ͽ� �ɷ�ġ�� �ø��µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_INC_ABILITY_BYBONUS, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
	}
	else
	{
		// ���ʽ��� ����Ͽ� �ɷ�ġ�� �ø��µ� �����ߴ�.
		SendFieldMsg(ON_RESPONSE_INC_ABILITY_BYBONUS, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�̴ϸʿ��� �÷��̾��� ������ �䱸�Ѵ�.
//	���� ����	:	2002-04-12 ���� 5:47:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvReqPIInMinimap()
{
	OnReqPIInMiniMapMsg		*pRPIInMiniMapMsg;
	SOPlayer				*pPlayer;
	CHAR					szName[ON_ID_LENGTH + 1];

	pRPIInMiniMapMsg	=	(OnReqPIInMiniMapMsg*)cMsg;

	// �ش� �÷��̾��� ������ ���´�.
	ZeroMemory(szName, sizeof(szName));
	strncpy(szName, pRPIInMiniMapMsg->szName, ON_ID_LENGTH);
	pPlayer				=	pMyWorld->GetPlayer(szName);

	if(pPlayer != NULL)
	{
		// �÷��̾ ������ �ִ�.	
		switch(pPlayer->siStatus)
		{
		case ON_PLAYER_STATUS_INFIELD:
			// �ʵ忡 �ִ�.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_ON_FIELD), LPVOID(pPlayer));
			break;
		case ON_PLAYER_STATUS_INVILLAGE:
			// �����ȿ� �ִ�.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_IN_VILLAGE), 
						LPVOID(pPlayer), LPVOID(pPlayer->stAccountInfo.pVillage));
			break;
		case ON_PLAYER_STATUS_INBATTLE:
			// �������̴�.
			SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_IN_BATTLE), LPVOID(pPlayer));
			break;
		}
	}
	else
	{
		// �÷��̾ �������� ���� �����̴�.
		SendFieldMsg(ON_RESPONSE_PI_INMINIMAP, SO_SFM_ONLYME, LPVOID(ON_RET_NOT_CONNECTED));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Ư�� ������ ������ ��û�Ѵ�.
//	���� ����	:	2002-09-30 ���� 12:59:39 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ٴϴ� �뺴 Change
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ٴϴ� �뺴 Change
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
