#include <GSL.h>

#include "OnlineGlobal.h"
#include "OnlineMsgProc.h"
#include "GGuard.h"
#include    "OnlineSetting.h"
#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character

#include	"FieldHeroInfoMgr.h"

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

extern HWND g_hWnd;


UI16 OnlineMsgProc::OnCheckGameGuard(BYTE *pMsg)
{
	OnCheckGameGuardMsg *pCheckGameGuard;

	pCheckGameGuard = (OnCheckGameGuardMsg*)pMsg;

	if(pCheckGameGuard == NULL)	return 0;

	SetGameGuardKey(pCheckGameGuard->dwGameGuardKey);

	return( 0 );
}

UI16 OnlineMsgProc::OnResCheckGameGuard(BYTE *pMsg)
{
	OnResponseMsg *pOnResponseMsg;

	pOnResponseMsg = (OnResponseMsg*)pMsg;

	if(pOnResponseMsg == NULL)	return 0;

	if( pOnResponseMsg->dwResponse == ON_RET_NO )
	{
		MessageBox(g_hWnd, "Don't try hacking\n", "Msg", MB_OK);
		return(0);
	}

	return(0);
}

UI16 OnlineMsgProc::OnResLogOut(BYTE *pMsg)
{
	OnResGameServerLogOutMsg	*pOnResLogOutMsg = (OnResGameServerLogOutMsg*)pMsg;

	if(pOnResLogOutMsg == NULL)		return 0;

	if( pOnResLogOutMsg->dwResponse == ON_RET_LOGOUT_OK )
	{
		OnResGameServerLogOutMsg	*pLogOutMsg = (OnResGameServerLogOutMsg*)pOnResLogOutMsg;

		// ����ڸ� �޸𸮿��� ���ش�.						
		_pWorld->pIOnlineChar->Free(pLogOutMsg->uiCharID);	

		return ON_MAINPOLL_FORCED_LOGOUT;
//		SetLogOut(GS_INITINITMENU, 0);
	}

	return 0;
}

// �ð� ����
UI16 OnlineMsgProc::ChangedTime(BYTE *pMsg)
{
	OnChangedTimeMsg		*pChangedTimeMsg;

	pChangedTimeMsg	=	(OnChangedTimeMsg*)pMsg;

	if(pChangedTimeMsg == NULL)	return 0;

	_pWorld->pTimer->SetNowTime(pChangedTimeMsg->Time);

	_pWorld->fFirstGetTime	= TRUE;
	_pWorld->m_RealTime		= pChangedTimeMsg->RealWorldTime;

	return 0;
}

UI16 OnlineMsgProc::OnTargetMe(BYTE *pMsg)
{
	OnTargetMeMsg* lpOnTargetMeMsg = (OnTargetMeMsg*)pMsg;

	// �� Character�� ã�Ƽ� Flag�� ����
	if(_pWorld->pIOnlineChar)
		_pWorld->pIOnlineChar->SetFindTargetFlag(lpOnTargetMeMsg->uiMonsterID, 1);


	return 0;
}

UI16 OnlineMsgProc::OnReleaseMe(BYTE *pMsg)
{
	OnReleaseMeMsg* lpOnReleaseMeMsg = (OnReleaseMeMsg*)pMsg;

	// �� Character�� ã�Ƽ� Flag�� ����
	if(_pWorld->pIOnlineChar)
		_pWorld->pIOnlineChar->SetFindTargetFlag(lpOnReleaseMeMsg->uiMonsterID, 0);


	return 0;
}

// ���� �ٳ�	
UI16 OnlineMsgProc::OnChangeMoney(BYTE *pMsg)
{
	CHAR	szTemp[128];
	_pWorld->bPay = TRUE;

	OnResMoneyInfo *pOnResMoneyInfo	=	(OnResMoneyInfo*)pMsg;

	if(pOnResMoneyInfo->dwResponse == ON_RET_CHNGEMONEY_INVESTMENT)
	{
		sprintf( szTemp, _pWorld->pOnlineText->Get( ON_TEXT_PAY_INVESTMENT ), pOnResMoneyInfo->Money - _pWorld->pMyData->GetMoney() );
	}
	else if(pOnResMoneyInfo->dwResponse == ON_RET_CHNGEMONEY_FROMADMIN)
	{
		sprintf( szTemp, _pWorld->pOnlineText->Get( ON_TEXT_RECEIVE_MONEY_FROM_HOST ), pOnResMoneyInfo->Money - _pWorld->pMyData->GetMoney() );
	}
	else if(pOnResMoneyInfo->dwResponse == ON_RET_CHNGEMONEY_PRODUCTFACILITY)
	{
		sprintf( szTemp, _pWorld->pOnlineText->Get( ON_TEXT_PAY_PRODUCTFACILITY ), pOnResMoneyInfo->Money - _pWorld->pMyData->GetMoney() );
	}
	else
	{
		sprintf( szTemp, _pWorld->pOnlineText->Get( ON_TEXT_PAY_BATTLE ), pOnResMoneyInfo->Money - _pWorld->pMyData->GetMoney() );
	}

	_pWorld->pOnlineMsgBoxPay->SetMsg( ON_MSGBOX_TYPE_OK, "", szTemp );
	_pWorld->pMyData->SetMoney( pOnResMoneyInfo->Money );
	return 0;
}

// ���� ����
UI16 OnlineMsgProc::OnOfficaialAnnouncement(BYTE *pMsg)
{
	OnResponseMsg *lpOnResponseMsg = (OnResponseMsg*)pMsg;

	if( lpOnResponseMsg == NULL )	return 0;

	CHAR	szChattingString[1024];
	CHAR	szChattingString1[1024];
	CHAR	TempBuffer[128];

	switch(lpOnResponseMsg->dwResponse)
	{
		// actdoll (2004/06/03 10:34) : SMS ���񽺿� ���� �۾��̴�.
		case ON_RET_OA_SMS_MSG:						// �������� - SMS
			{
				OnOASMSMsg*			lpOnOASMSMsg;

				lpOnOASMSMsg	= (OnOASMSMsg*)lpOnResponseMsg;

				_pWorld->pOnlinePannel->AddSMSMessage( lpOnOASMSMsg->szRecvTime, lpOnOASMSMsg->szOrgPhone, lpOnOASMSMsg->szCallMessage );
			}
			break;

		case ON_RET_OA_TEXT:						// �������� - �Ϲ����� �ؽ�Ʈ 
			{
				OnOATextMsg*		lpOnOATextMsg;
				char*				TemplpBuffer;

				lpOnOATextMsg = (OnOATextMsg*)lpOnResponseMsg;
				TemplpBuffer  = (char*)((char*)lpOnOATextMsg + sizeof(OnOATextMsg));

				ZeroMemory(szChattingString1, 1024);
				strncpy((char*)szChattingString1, TemplpBuffer, lpOnOATextMsg->dwLength);
				sprintf((char*)szChattingString, "%s", szChattingString1);
				_pWorld->pOnlinePannel->AddChat((char*)szChattingString, ON_INN_COLOR_GREEN, CHATTINGMODE_NORMAL);
				SetMessage(szChattingString);
			}
			break;
			

		case ON_RET_OA_VILL_ATTACKED:				// �������� - ������ ������ ���ϰ� �ִ�.
			{
				OnOAVillAttackedMsg*		lpOnOAVillAttackedMsg;
				VillageHeader*				lpVillageHeader;

				lpOnOAVillAttackedMsg = (OnOAVillAttackedMsg*)lpOnResponseMsg;

				lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnOAVillAttackedMsg->uiVillageCode);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_VILLAGEATTACK), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName));
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
			}
			break;

		case ON_RET_OA_VILL_OCCUPIED:				// �������� - ������ ���ɵǾ���. ��� ��ܿ� �ҼӵǾ���
			{
				OnOAVillOccupiedMsg*		lpOnOAVillOccupiedMsg;
				VillageHeader*				lpVillageHeader;

				lpOnOAVillOccupiedMsg = (OnOAVillOccupiedMsg*)lpOnResponseMsg;

				ZeroMemory((char*)TempBuffer, 128);
				strncpy((char*)TempBuffer, (char*)lpOnOAVillOccupiedMsg->szGuildID, ON_ID_LENGTH);
				lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnOAVillOccupiedMsg->uiVillageCode);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_VILLAGEGET), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName), TempBuffer);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
			}
			break;

		case ON_RET_OA_VILL_SURRENDER:				// �������� - ������ ���ɵǾ���. ������ ���� ������ �μ�����
			{
				OnOAVillOccupiedMsg*		lpOnOAVillOccupiedMsg;
				VillageHeader*				lpVillageHeader;

				lpOnOAVillOccupiedMsg = (OnOAVillOccupiedMsg*)lpOnResponseMsg;

				ZeroMemory((char*)TempBuffer, 128);
				strncpy((char*)TempBuffer, (char*)lpOnOAVillOccupiedMsg->szGuildID, ON_ID_LENGTH);
				lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnOAVillOccupiedMsg->uiVillageCode);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(8100125), TempBuffer, _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName));
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
				SetMessage(szChattingString, -1, 0, RGB(255, 155, 0));	// robypark 2005/1/29 13:15 ���ɵ� ������ �������� ��� ��ġ�� ��Ȳ������ ���
			}
			break;

		case ON_RET_OA_MAXINVESTMENT_CHANGED:		// �������� - �ְ� �����ڰ� ���� �Ǿ���.
			{
				OnOAMaxInvChangedMsg*		lpOnOAMaxInvChangedMsg;
				VillageHeader*				lpVillageHeader;
			
				lpOnOAMaxInvChangedMsg = (OnOAMaxInvChangedMsg*)lpOnResponseMsg;

				ZeroMemory((char*)TempBuffer, 128);
				strncpy((char*)TempBuffer, (char*)lpOnOAMaxInvChangedMsg->szID, ON_ID_LENGTH);
				lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnOAMaxInvChangedMsg->uiVillageCode);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_CHANGEMAXINVESTMENTER), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName), TempBuffer);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));

				if(strcmp(lpOnOAMaxInvChangedMsg->szID, _pWorld->pMyData->GetMyCharName()) == 0)
				{
					sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_HIGH_INVESTOR), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName));
					_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", szChattingString);
				}
			}
			break;

		case ON_RET_OA_GUILD_CREATED:				// �������� - ����� ���������.
			{
				OnOAGuildCreatedMsg*		lpOnOAGuildCreatedMsg;

				lpOnOAGuildCreatedMsg = (OnOAGuildCreatedMsg*)lpOnResponseMsg;

				ZeroMemory((char*)TempBuffer, 128);
				strncpy((char*)TempBuffer, (char*)lpOnOAGuildCreatedMsg->szGuildID, ON_GUILDNAME_LENGTH);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_MAKEGUILD), TempBuffer);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
			}
			break;

		case ON_RET_OA_GUILD_DELETED:				// �������� - ����� �������
			{
				OnOAGuildDeletedMsg*		lpOnOAGuildDeletedMsg;

				lpOnOAGuildDeletedMsg = (OnOAGuildDeletedMsg*)lpOnResponseMsg;

				ZeroMemory((char*)TempBuffer, 128);
				strncpy((char*)TempBuffer, (char*)lpOnOAGuildDeletedMsg->szGuildID, ON_GUILDNAME_LENGTH);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_DELETEGUILD), TempBuffer);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
			}
			break;

		case ON_RET_OA_PAYING_BUSINESS:				// �������� - ������ ���� ����� ����
			{
				OnOAGuildPayingBusinessMsg*		lpOnOAGuildPayingBusinessMsg;
				VillageHeader*					lpVillageHeader1;
				VillageHeader*					lpVillageHeader2;
				CItemHeader*					pItem;
				OnlineVillageParser*			lpOnlineVillageParser;

				lpOnOAGuildPayingBusinessMsg = (OnOAGuildPayingBusinessMsg*)lpOnResponseMsg;
				lpOnlineVillageParser        = _pWorld->pOnlineVillageManager->GetVillageParser();

				lpVillageHeader1 = lpOnlineVillageParser->GetVillageHeaderCode(lpOnOAGuildPayingBusinessMsg->uiBuyVillageCode);
				lpVillageHeader2 = lpOnlineVillageParser->GetVillageHeaderCode(lpOnOAGuildPayingBusinessMsg->uiSellVillageCode);
				pItem            = _pWorld->pItemsInfo->GetItem(lpOnOAGuildPayingBusinessMsg->uiItemCode);
				if(lpVillageHeader1 && lpVillageHeader2 && pItem)
				{
					if(pItem->m_uiTradeGrade <= _pWorld->pMyData->GetTradeGrade())
					{
						sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_NOTICE_ITEMPIECECHANGE), _pWorld->pOnlineText->Get(pItem->m_siItemName)
							, _pWorld->pOnlineText->Get(lpVillageHeader1->siVillageName), (MONEY)lpOnOAGuildPayingBusinessMsg->uiBuyPrice
							, _pWorld->pOnlineText->Get(lpVillageHeader2->siVillageName), (MONEY)lpOnOAGuildPayingBusinessMsg->uiSellPrice);
						_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
					}
				}
			}
			break;
		case ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE:
			{
				OnOADAIGetMoneyByDestroyVillageMsg* lpOnOADAIGetMoneyByDestroyVillageMsg = (OnOADAIGetMoneyByDestroyVillageMsg*)lpOnResponseMsg;

				char szEarnMoney[32];
				MakeMoneyText(lpOnOADAIGetMoneyByDestroyVillageMsg->mnMoney, szEarnMoney);

				// �������� 
				ZeroMemory(szChattingString, sizeof(szChattingString));
				VillageHeader* lpVillageHeader = _pWorld->pOnlineVillageManager->GetVillageParser()->GetVillageHeaderCode(lpOnOADAIGetMoneyByDestroyVillageMsg->uiVillageCode);
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_ATTACK_EARNINGS1), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName), szEarnMoney);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(125, 167, 217));
				_pWorld->pOnlinePannel->AddChat((char*)szChattingString, ON_INN_COLOR_WHITE, CHATTINGMODE_GUILD);
				
				// ����� ��� �޽��� �ڽ� ���
				sprintf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_ATTACK_EARNINGS2), _pWorld->pOnlineText->Get(lpVillageHeader->siVillageName), szEarnMoney);
				if(_pWorld->pMyData->GetGuildClass() == _pWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
				{
					_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, _pWorld->pOnlineText->Get(ON_TEXT_PACKET_VILLAGE_ATTACK_EARNINGS3), szChattingString);
					_pWorld->pMyData->SetSaving(_pWorld->pMyData->GetSaving() + lpOnOADAIGetMoneyByDestroyVillageMsg->mnMoney);
				}
			}
			break;
	}

	return 0;
}

UI16 OnlineMsgProc::OnTextMessageg(BYTE *pMsg)
{
	char*			TemplpBuffer;

	OnTextMsg *pOnTextMsg   = (OnTextMsg*)pMsg;
	TemplpBuffer = (char*)pOnTextMsg + sizeof(OnTextMsg);

	
	char szChattingString1[1024], szChattingString[1024];

	switch(pOnTextMsg->dwResponse)
	{
		case ON_RET_TEXTMSG_WHISPER:
			ZeroMemory(szChattingString1, 1024);
			strncpy((char*)szChattingString1, TemplpBuffer, pOnTextMsg->siStringLength);
			sprintf((char*)szChattingString, "%s", szChattingString1);
			if(_pWorld->m_GMFlag != ON_USERGRADE_NORMAL)
			{
				if(strncmp((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_GMTEXT_MAXUSER), 20) == 0)
				{
					if(_pWorld->m_InfoFlag == FALSE)
					{
						SetMessage(szChattingString);
					}
				}
				else
				{
					SetMessage(szChattingString);
				}
			}
			else
			{
				SetMessage(szChattingString);
			}

			if(_pWorld->m_GMFlag != ON_USERGRADE_NORMAL)
			{
				if(strncmp((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_GMTEXT_MAXUSER), 20) == 0)
				{
					SI32		TempMaxUser;
					SI32		TempNowUser;

					sscanf((char*)szChattingString, _pWorld->pOnlineText->Get(ON_TEXT_GMTEXT_MAXUSER), &TempMaxUser, &TempNowUser);
					_pWorld->CheckNowConnectUser(TempMaxUser, TempNowUser);
				}
			}
			break;
			
		case ON_RET_TEXTMSG_GUILD:
			{
				if(!_pWorld->m_bGuildOn)			break;
				
				ZeroMemory(szChattingString1, 1024);
				strncpy((char*)szChattingString1, TemplpBuffer, pOnTextMsg->siStringLength);
				sprintf((char*)szChattingString, "%s", szChattingString1);
				_pWorld->pOnlineNoticeList->AddNoticeData((char*)szChattingString, RGB(255, 255, 255));
				
				// ��� ä�ø���Ʈ�� �ֱ�
				int		TempCount, TempSize, TempStart = 0;
				char	TempBuffer1[256], TempBuffer2[256];
				ZeroMemory(TempBuffer1, 256);
				ZeroMemory(TempBuffer2, 256);
				TempSize = strlen((char*)szChattingString);
				
				// ID �� �˻��Ѵ�.
				for(TempCount = 1; TempCount < TempSize; TempCount++)
				{
					if(szChattingString[TempCount] == ']' && szChattingString[TempCount+2] == ':')
					{
						TempStart = TempCount + 4;
						break;
					}
					else
					{
						sscanf(&szChattingString[TempCount], "%c", TempBuffer2);
						strcat(TempBuffer1, TempBuffer2);
					}
				}
				
				// ������ �˻��Ѵ�.
				strcpy((char*)TempBuffer2, (char*)&szChattingString[TempStart]);
				
				_pWorld->pOnlinePannel->AddChatID(TempBuffer1, TempBuffer2, CHATTINGMODE_GUILD);
			}
			break;
		case ON_RET_TEXTMSG_PARTYCHAT:
			{
				ZeroMemory(szChattingString1, 1024);
				strncpy((char*)szChattingString1, TemplpBuffer, pOnTextMsg->siStringLength);
				sprintf((char*)szChattingString, "%s", szChattingString1);
				
				// ��Ƽ ä�ø���Ʈ�� �ֱ�
				int		TempCount, TempSize, TempStart = 0;
				char	TempBuffer1[256], TempBuffer2[256];
				ZeroMemory(TempBuffer1, 256);
				ZeroMemory(TempBuffer2, 256);
				TempSize = strlen((char*)szChattingString);
				
				// ID �� �˻��Ѵ�. 
				for(TempCount = 1; TempCount < TempSize; TempCount++)
				{
					if(szChattingString[TempCount] == ']' && szChattingString[TempCount+2] == ':')
					{
						TempStart = TempCount + 4;
						break;
					}
					else
					{
						sscanf(&szChattingString[TempCount], "%c", TempBuffer2);
						strcat(TempBuffer1, TempBuffer2);
					}
				}
				
				// ������ �˻��Ѵ�.
				strcpy((char*)TempBuffer2, (char*)&szChattingString[TempStart]);
				
				_pWorld->pOnlinePannel->AddChatID(TempBuffer1, TempBuffer2, CHATTINGMODE_PARTY);
			}
			break;
	}

	return 0;
}

// ����
UI16 OnlineMsgProc::OnResWarp(BYTE *pMsg)
{
	OnlineReceiveFieldItemInfo*		pReceivedFieldItemInfo;
	OnResWarpMsg*					pOnResWarpMsg;
	OnResWarpOkMsg*					pOnResWarpOkMsg;

	OnCharBaseInfo*					lpOnCharBaseInfo;
	OnCharPlayerInfo*				lpOnCharPlayerInfo;
	OnBaseBoothItem					*lpOnBaseBoothItem, *pTempOnBaseBoothItem;
	SellItemInfo					TempSellItemInfo;
	OnCharMonsterInfo*				lpOnCharMonsterInfo;
	OnCharShipInfo*					lpOnCharShipInfo;

	CHAR							TempBuffer[128];
	DWORD							IP;

	pOnResWarpMsg = (OnResWarpMsg*)pMsg;
	if(pOnResWarpMsg == NULL)	return 0;
	
	if(pOnResWarpMsg->dwResponse == ON_RET_OK)
	{
		pOnResWarpOkMsg = (OnResWarpOkMsg*)pOnResWarpMsg;

		UI16	uiCurrentFocusCharID = _pWorld->pOnlineMap->m_uiCurrentFocusCharID;
		_pWorld->pOnlineMap->ChangeMap(pOnResWarpOkMsg->uiMapID, pOnResWarpOkMsg->uiX, pOnResWarpOkMsg->uiY);
		_pWorld->pOnlineClientChatting->MessageSend_MoveSell(pOnResWarpOkMsg->uiMapID, pOnResWarpOkMsg->uiX, pOnResWarpOkMsg->uiY);

		// Player
		// ��� ĳ���͵��� ���ش�.
		_pWorld->pIOnlineChar->AllFree();			
		lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)pOnResWarpOkMsg + sizeof(OnResWarpOkMsg));
		for(SI32 i = 0; i < pOnResWarpOkMsg->uiPlayerNum; i++)
		{			
			if(ISPLAYER(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpOnCharBaseInfo;

				ZeroMemory(TempBuffer, 128);
				if(_pWorld->pMyData->GetMyCharUniqueID() == lpOnCharPlayerInfo->uiAccount)
				{
					CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
					IP = lpOnCharPlayerInfo->siDBAccount;

					_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, (UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);
					_pWorld->pMyData->SetInVillageDefenceInterface(lpOnCharPlayerInfo->bVillageInLimit);

					_pWorld->pOnlineMap->SetCurrentFocusCharID(uiCurrentFocusCharID);
					_pWorld->pOnlineMap->PlayFitMusic();
					
					// ��� ó��
					if(_pWorld->pMyData->GetGuildName())
					{
						_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pMyData->GetGuildName(), _pWorld->pMyData->GetGuildClass(), _pWorld->pMyData->GetGuildFlag());
					}
				}
				else
				{
					IP = lpOnCharPlayerInfo->siDBAccount;

					_pWorld->pIOnlineChar->Init(lpOnCharPlayerInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharPlayerInfo->uiKind, lpOnCharPlayerInfo->siX, lpOnCharPlayerInfo->siY, IP, 0, lpOnCharPlayerInfo->siFightingPower, _pWorld,(UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
					_pWorld->pIOnlineChar->SetAttackMode(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bAttackMode);
					_pWorld->pIOnlineChar->SetEquipItem(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->uiEquipWeapon);							
					_pWorld->pIOnlineChar->SetSiegeWarfare_Hp(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siFieldHp);
					_pWorld->pIOnlineChar->SetCaptain(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->bCaptain);
					_pWorld->pIOnlineChar->SetSiegeWarfare_CurrentSupplyGoodsAmount(lpOnCharPlayerInfo->uiAccount,lpOnCharPlayerInfo->siSupplyCharge);

					// robypark 2004/12/17 17:29
					// �����������̽��� �ִ� ĳ�����ΰ��� ����
					_pWorld->pIOnlineChar->SetInVillageDefenceInterface(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->bVillageInLimit);

					// ���� ���� ���� ��� ���
					LPSSiegeUnitTable lpsSiegeUnitTable = _pWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(lpOnCharPlayerInfo->uiKind);

					// ������ ������ ���� ���
					if (NULL != lpsSiegeUnitTable)
					{
						// ������ ���� ����� ĳ������ �� ������ ���ٸ�
						if(_pWorld->pIOnlineChar->GetDataType(lpOnCharPlayerInfo->uiAccount) == 1)
						{
							if (_pWorld->GetIsShowInformationAboutSiegeWarfareUnit() & ON_GAMEOPTION_SIEGEWARFAREUNIT)
							{
								// �������� ��û�Ѵ�.
								_pWorld->pOnlineClient->SendPlayerDetailInfo(lpOnCharPlayerInfo->uiAccount);
								_pWorld->pIOnlineChar->SetDataType(lpOnCharPlayerInfo->uiAccount, 2);	// ���� �ٿ�ε� ��� ��...
							}
						}
					}
				}

				_pWorld->pIOnlineChar->SetGuildData(lpOnCharPlayerInfo->uiAccount, _pWorld->pIOnlineChar->GetGuildName(lpOnCharPlayerInfo->uiAccount), lpOnCharPlayerInfo->siGuildInGrade, _pWorld->pIOnlineChar->GetGuildFlag(lpOnCharPlayerInfo->uiAccount));
				_pWorld->pIOnlineChar->SetMercenary(lpOnCharPlayerInfo->uiAccount, lpOnCharPlayerInfo->siVMercenaryKind);

				// ���� ����
				if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) 
				{
					_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, TRUE);
					lpOnBaseBoothItem = (OnBaseBoothItem*)((CHAR*)lpOnCharPlayerInfo + sizeof(OnCharPlayerInfo));
					pTempOnBaseBoothItem = lpOnBaseBoothItem;
					for(SI32 TempCount1 = 0; TempCount1 < lpOnCharPlayerInfo->siPrivateMarketItemNum; TempCount1++)
					{
						ZeroMemory(&TempSellItemInfo, sizeof(SellItemInfo));
						if(pTempOnBaseBoothItem->siType == LIST_ITEM)
							TempSellItemInfo.Item.uiItemID = pTempOnBaseBoothItem->uiProductID;
						else if(pTempOnBaseBoothItem->siType == LIST_FOLLOWER)
							TempSellItemInfo.Follower.uiKind = pTempOnBaseBoothItem->uiProductID;
						TempSellItemInfo.siType = pTempOnBaseBoothItem->siType;
						_pWorld->pIOnlineChar->SetBoothItem(lpOnCharPlayerInfo->uiAccount, pTempOnBaseBoothItem->uiBoothSlot, TempSellItemInfo);
						pTempOnBaseBoothItem++;
					}

					_pWorld->pOnlineBooth->SetBoothSpeech(lpOnCharPlayerInfo->uiAccount, lpOnBaseBoothItem, lpOnCharPlayerInfo->siPrivateMarketItemNum);
				}
				else
				{
					_pWorld->pIOnlineChar->SetBoothOpen(lpOnCharPlayerInfo->uiAccount, FALSE);
				}

				if(lpOnCharPlayerInfo->siPrivateMarketItemNum >= 0) lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo) + (lpOnCharPlayerInfo->siPrivateMarketItemNum * sizeof(OnBaseBoothItem)));
				else                                                lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharPlayerInfo));
			}
			else if(ISMONSTER(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpOnCharBaseInfo;

				// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
				ZeroMemory(TempBuffer, 128);
				IP = 0;
				_pWorld->pIOnlineChar->Init(lpOnCharMonsterInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharMonsterInfo->uiKind, lpOnCharMonsterInfo->siX, lpOnCharMonsterInfo->siY, IP, 0.0f, lpOnCharMonsterInfo->siFightingPower, _pWorld,NULL);
				
				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharMonsterInfo));
			}
			else if(ISSHIP(lpOnCharBaseInfo->uiAccount))
			{
				lpOnCharShipInfo = (OnCharShipInfo*)lpOnCharBaseInfo;

				// �ٸ� ĳ������ ���� ������ �����Ѵ�.						
				ZeroMemory(TempBuffer, 128);
				IP = 0;
				_pWorld->pIOnlineChar->Init(lpOnCharShipInfo->uiAccount, (CHAR*)TempBuffer, lpOnCharShipInfo->uiKind, lpOnCharShipInfo->siX, lpOnCharShipInfo->siY, IP, 0.0f, 0, _pWorld,NULL);

				lpOnCharBaseInfo = (OnCharBaseInfo*)((CHAR*)lpOnCharBaseInfo + sizeof(OnCharShipInfo));
			}
		}

		// Item
		pReceivedFieldItemInfo = (OnlineReceiveFieldItemInfo*)((CHAR*)lpOnCharBaseInfo);
		for(i = 0; i < pOnResWarpOkMsg->uiFieldItemNum; i++)
		{
			// �ʵ忡 �������� �����Ѵ�.
			_pWorld->pOnlineMap->SetID(pReceivedFieldItemInfo[i].siX, pReceivedFieldItemInfo[i].siY, 1, 1, 0, 0, pReceivedFieldItemInfo[i].uiItemID, pReceivedFieldItemInfo[i].uiQuantity);
		}

		if(_pWorld->pOnlineMiniMap && pOnResWarpOkMsg->uiMapID == 0)
			_pWorld->pOnlineMiniMap->MakeTargetList();
	}

	return 0;
}

// ���ʽ� ��
UI16 OnlineMsgProc::OnResBonusUp(BYTE *pMsg)
{
	OnResIncAbilityByBonusMsg *pOnResIncAbilityByBonusMsg	=	(OnResIncAbilityByBonusMsg*)pMsg;

	if( pOnResIncAbilityByBonusMsg->dwResponse == ON_RET_OK )
		_pWorld->pOnlineTradeBook->BonusUp( TRUE );
	else 
		_pWorld->pOnlineTradeBook->BonusUp( FALSE );

	return 0;
}

// �����
UI16 OnlineMsgProc::OnHungry(BYTE *pMsg)
{
	OnHugryMsg		*pOnHugryMsg;
	CHAR			*pTempTalk;

	pOnHugryMsg	=	(OnHugryMsg*)pMsg;
	
	if(pOnHugryMsg == NULL)		return 0;

	if(_pWorld->pOnlineBooth->IsActionIn())		return 0;

	if( pOnHugryMsg->dwResponse == ON_RET_HUR_CHANGE_HEALTH )
	{
		OnHugryChangeHealthMsg	*pOnHugryDecreaseHealthMsg;
		pOnHugryDecreaseHealthMsg	=	(OnHugryChangeHealthMsg*)pOnHugryMsg;

		_pWorld->pMyData->SetHealth( pOnHugryDecreaseHealthMsg->siHealth );

		if( _pWorld->pMyData->GetHealth() == 0 )//�ſ� ����� ��
		{
			// robypark 2005/1/14 17:27
			// ���ΰ� ���� ������ ������ ���
			CFieldHeroInfo *pFieldHeroInfo = _pWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(_pWorld->pMyData->GetMyCharKind());

			if (pFieldHeroInfo)
			{
				if (pFieldHeroInfo->GetSndHungriest())
					HashCodePushEffect(pFieldHeroInfo->GetSndHungriest(), 0, 0);
			}
		
			pTempTalk	=	_pWorld->pOnlineText->Get( ON_TEXT_HUNGRY2 );
			_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), pTempTalk);
			_pWorld->pOnlinePannel->AddChat(0, pTempTalk, CHATTINGMODE_NORMAL);
		}

		else if( _pWorld->pMyData->GetHealth() <= 60 )//�߰� ����� ��
		{
			// robypark 2005/1/14 17:27
			// ���ΰ� ���� ������ ������ ���
			CFieldHeroInfo *pFieldHeroInfo = _pWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(_pWorld->pMyData->GetMyCharKind());

			if (pFieldHeroInfo)
			{
				if (pFieldHeroInfo->GetSndHungrier())
					HashCodePushEffect(pFieldHeroInfo->GetSndHungrier(), 0, 0);
			}

			pTempTalk	=	_pWorld->pOnlineText->Get( ON_TEXT_HUNGRY1 );
			_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), pTempTalk);
			_pWorld->pOnlinePannel->AddChat(0, pTempTalk, CHATTINGMODE_NORMAL);
		}

		else if( _pWorld->pMyData->GetHealth() <= 100 )// �ణ ����� ��
		{					
			// robypark 2005/1/14 17:27
			// ���ΰ� ���� ������ ������ ���
			CFieldHeroInfo *pFieldHeroInfo = _pWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(_pWorld->pMyData->GetMyCharKind());

			if (pFieldHeroInfo)
			{
				if (pFieldHeroInfo->GetSndHungry())
					HashCodePushEffect(pFieldHeroInfo->GetSndHungry(), 0, 0);
			}

			pTempTalk	=	_pWorld->pOnlineText->Get( ON_TEXT_HUNGRY0 );

			_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), pTempTalk);
			_pWorld->pOnlinePannel->AddChat( 0, pTempTalk, CHATTINGMODE_NORMAL);
		}
	}

	else if( pOnHugryMsg->dwResponse == ON_RET_HUR_CHANGE_HP )
	{
		OnHugryChangeHPMsg		*pOnHugryDecreaseHPMsg;
		strHungryFollowerInfo	*pstrHungryFollowerInfo;
		SI16					i;
		

		pOnHugryDecreaseHPMsg	=	(OnHugryChangeHPMsg*)pOnHugryMsg;
		pstrHungryFollowerInfo	=	(strHungryFollowerInfo*)(pOnHugryDecreaseHPMsg + 1);
		pTempTalk				=	_pWorld->pOnlineText->Get( ON_TEXT_HUNGRY6 + rand()%3 );	

		for( i=0; i<pOnHugryDecreaseHPMsg->uiFollowerNum; i++ )
			_pWorld->pMyData->SetFollowerParameterLife( pstrHungryFollowerInfo[i].uiFollowerID, pstrHungryFollowerInfo[i].siLife );

		_pWorld->pIOnlineChar->SetSpeech(_pWorld->pMyData->GetMyCharUniqueID(), pTempTalk);
		_pWorld->pOnlinePannel->AddChat( 0, pTempTalk, CHATTINGMODE_NORMAL);
	}

	return 0;
}


// 
UI16 OnlineMsgProc::ChangeWeather(BYTE *pMsg)
{
	OnChangeWeather*		lpOnChangeWeather = (OnChangeWeather*)pMsg;
	_pWorld->pOnlineFieldEffect->SetWeather(lpOnChangeWeather->fWeather);
	return 0;
}


// ������ ������ ������ ��ȯ 
UI16 OnlineMsgProc::OnResItemReceived(BYTE *pMsg)
{
	OnResItemReceivedMsg*	lpOnResItemReceivedMsg = (OnResItemReceivedMsg*)pMsg;

	if(lpOnResItemReceivedMsg->uiCount > 0 && !_pWorld->m_bReceivedItem)
	{
		_pWorld->m_bReceivedItem = TRUE;

		SetRect(&_pWorld->m_rtReceivedItem, 746, 465, 746 + _pWorld->m_ReceivedItemAniSpr.Header.Xsize, 465 + _pWorld->m_ReceivedItemAniSpr.Header.Ysize);
		_pWorld->m_dwDelay				= timeGetTime();
		_pWorld->m_bReceivdeItemShow	= TRUE;
		_pWorld->m_siReceivedItemCount	= lpOnResItemReceivedMsg->uiCount;
	}

	return 0;
}

// ���� ���� ���α׷�
UI16 OnlineMsgProc::RunProgramList(BYTE *pMsg)
{
	OnReqRunProgramListMsg* lpOnReqRunProgramListMsg = (OnReqRunProgramListMsg*)pMsg;

	// �������� ���α׷� ����� ���Ѵ�.
	ZeroMemory(_pWorld->m_szProgramList, 255*200);
	_pWorld->m_uiProgramNum = 0;

	// EnumWindows(MyEnumWindowsProc, NULL);
	HANDLE hSnap;
	PROCESSENTRY32 pe;
	
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnap == (HANDLE)-1)		return 0;
	
	pe.dwSize=sizeof(PROCESSENTRY32);
	if (Process32First(hSnap,&pe)) {
		do {
			strcpy(_pWorld->m_szProgramList[_pWorld->m_uiProgramNum], pe.szExeFile);
			_pWorld->m_uiProgramNum++;
		} while (Process32Next(hSnap,&pe));
	}
	CloseHandle(hSnap);

	// �������� ���α׷� ����� ������.
	CHAR szTempBuffer[30000];
	SI16 siSize, siLength;
	ZeroMemory(szTempBuffer, sizeof(szTempBuffer));
	OnResRunProgramListMsg	TempOnResRunProgramListMsg;
	ZeroMemory(&TempOnResRunProgramListMsg, sizeof(OnResRunProgramListMsg));
	TempOnResRunProgramListMsg.dwMsgType		= ON_RESPONSE_RUN_PROGRAM_LIST;
	TempOnResRunProgramListMsg.dwReason			= lpOnReqRunProgramListMsg->dwReason;
	TempOnResRunProgramListMsg.siRunProgramNum	= _pWorld->m_uiProgramNum;

	siSize		= sizeof(OnResRunProgramListMsg);
	siLength	= sizeof(OnResRunProgramListMsg) + 255 * _pWorld->m_uiProgramNum;
	memcpy(szTempBuffer, &TempOnResRunProgramListMsg, siSize);
	memcpy(&szTempBuffer[siSize], _pWorld->m_szProgramList, 255 * _pWorld->m_uiProgramNum);
	
	_pWorld->pOnlineClient->Write(&szTempBuffer, siLength);
	_pWorld->pOnlineClient->Close();
	_pWorld->pOnlineClientChatting->Close();

	// ���޼���
	if(lpOnReqRunProgramListMsg->dwReason == RUN_PROG_LIST_SPEEDHACK)
	{
		_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_SPEEDHACK_NOTICE));
		
		_pWorld->m_bForcedLogout	= TRUE;
		_pWorld->bCharMove			= FALSE;
	}
	return 0;
}

// ����	
UI16 OnlineMsgProc::MakeThunder(BYTE *pMsg)
{
	OnMakeThunder *pThunder = (OnMakeThunder*)pMsg;

	// ���� ������ ����
	if( _pWorld->pMyData->GetMyCharUniqueID() == pThunder->uiAccount )
	{
		// ü���� 5% ���ҽ�Ų��.
		_pWorld->pMyData->GetFollowersInterface()->AllCharacterDeleteLifePercent(5);
	}

	
	// ���� ���� �� ��ġ�� ���Ѵ�.
//	_pWorld->pIOnlineChar->GetRPos(pThunder->uiAccount, x, y);


	// ���� Effet�� ����Ѵ�.
	SI16 siHeroX, siHeroY;
	SI32 siX, siY;
	
	if( _pWorld->pIOnlineChar->GetPos(pThunder->uiAccount, &siHeroX, &siHeroY) )
	{
		XSPR spr;
		CHAR szPath[512];
		sprintf(szPath, "Online\\GameSpr\\Lightning.Spr");

		_pWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(siHeroX, siHeroY, siX, siY);

		clGrp.LoadXspr(szPath, spr);

		siX -= spr.Header.Xsize/3;
		siY -= spr.Header.Ysize;		
		
		_pWorld->pOnlineMap->SetAnimation(siX-3, siY, 4, szPath, 2);
		_pWorld->pOnlineMap->SetAnimation(siX-6, siY, 4, szPath, 2);		
		_pWorld->pOnlineMap->SetAnimation(siX-9, siY, 4, szPath, 2);
		PushEffect(EFFECT_THUNDER, 0, 0);

		if(spr.Image)	 {clGrp.FreeXspr(spr);   spr.Image  = NULL;}
	}

	return 0;
}

UI16 OnlineMsgProc::LeaveVillage(BYTE *pMsg)
{
	return _pWorld->LeaveVillage(pMsg);
}

UI16 OnlineMsgProc::ReturnOnline(BYTE *pMsg)
{
	return _pWorld->ReturnOnline(pMsg);
}

UI16 OnlineMsgProc::InvestmentList(BYTE *pMsg)
{
	if(_pWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_TOTAL_MONEY &&
		(_pWorld->pOnlineTradeBook->GetTotalMoney()->m_MyMoneyStatus == ON_BOOK_TOTAL_MONEY_SELECT))
		_pWorld->pOnlineTradeBook->InvestmentList(pMsg);
	else
	{
		if (_pWorld->pOnlineVillage)
		{
			OnlineGovernMgr *pStructure = (OnlineGovernMgr*)_pWorld->pOnlineVillage->pVillageStructManager->GetStructure();

			if (pStructure)
				return pStructure->InvestmentList(pMsg);
		}
	}
	return 0;
}

UI16 OnlineMsgProc::SetAttackMode(BYTE *pMsg)
{	
	OnResSetAttackMode	*pOnResSetAttackMode = (OnResSetAttackMode*)pMsg;
	_pWorld->ToggleAttackMode(pOnResSetAttackMode->uiAccount, pOnResSetAttackMode->bAttackMode);
	return 0;
}

UI16 OnlineMsgProc::ResRank(BYTE *pMsg)
{
	OnEventUserRankings	*pRankings = (OnEventUserRankings*)pMsg;

	struct Rank
	{
		UI16	uAccont;
		UI16	uRank;
	};

	Rank *pRank = (Rank*)pRankings + sizeof(OnEventUserRankings);

	for(UI16 i=0; i<pRankings->uiPlayerNum; ++i, ++pRank)
	{
		if(pRank->uAccont == _pWorld->pMyData->GetMyCharUniqueID())
			_pWorld->pMyData->nRank = pRank->uRank;

		_pWorld->pIOnlineChar->SetRank(pRank->uAccont, pRank->uRank);
	}

	return 0;
}

// robypark 2004/12/6 17:11
// �������� ����Ǿ���. ��� ������ �������� �κ��丮���� �����ϸ� ���� ���� ���ΰ� �������� �������.
// ON_CHANGED_WAR_END
UI16	OnlineMsgProc::OnResChangedWarEnd(BYTE *pMsg)
{
	OnChangedWarEnd	*pOnChangedWarEnd	= (OnChangedWarEnd *)pMsg;

	DWORD		IP;
	CHAR		TempBuffer[128];
	
	// ĳ�� ���� ���
	UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
	SI16 siX, siY;
	SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind, siPrevImjinKindIdx;
	CHAR TempBuffer1[128];
	ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

	uiUniqueID				= pOnChangedWarEnd->uiPlayerID;
	uiKind					= pOnChangedWarEnd->uiKind;
	siPrevImjinKindIdx		= _pWorld->pIOnlineChar->GetImjinKind(uiUniqueID);

	// ��ϵ��� ���� ĳ���Ͷ��
	if ((KIND_ON_INVALID == siPrevImjinKindIdx) || (0 == siPrevImjinKindIdx))
	{
		// ����� ĳ�����̸�
		if(_pWorld->pMyData->GetMyCharUniqueID() == pOnChangedWarEnd->uiPlayerID)
		{
			// �κ��丮�� ���� ������ �ݾ��ش�.
			if(_pWorld->pOnlineInventory->IsActive())
				_pWorld->pOnlineInventory->SetActive(FALSE);

			// ��ΰ� ���� ������ �ݾ��ش�.
			if(_pWorld->pOnlineTradeBook->IsAction())
				_pWorld->pOnlineTradeBook->SetAction(FALSE);

			// �̴ϸʵ� �ݴ´�!!
			if(_pWorld->pOnlineMiniMap->IsActive())
				_pWorld->pOnlineMiniMap->SetActive(FALSE);

			// �� �ݾ�!!
			if(_pWorld->pOnlineSetting->IsAction())
				_pWorld->pOnlineSetting->SetAction(FALSE);

			// ��� ������ �������� �κ��丮���� ����
			_pWorld->DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit();

			_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);	// ������ ������ �ƴϴ�.

			// robypark 2005/1/28 14:59
			// ������ ���� �������̽� �ݱ�
			_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

			return 0;
		}
	}

	// ���� ���ֿ��� => ���ΰ� ĳ���ͷ� �����ϴ� ���̶��
	if (_pWorld->IsSiegeWarfareUnit(siPrevImjinKindIdx))
	{
		IP				= _pWorld->pIOnlineChar->GetID(uiUniqueID);
		siFightPower	= _pWorld->pIOnlineChar->GetFightPower(uiUniqueID);
		siGuildInGrade	= _pWorld->pIOnlineChar->GetGuildInGrade(uiUniqueID);
		siGuildFlag		= _pWorld->pIOnlineChar->GetGuildFlag(uiUniqueID);
		siMercenaryKind = _pWorld->pIOnlineChar->GetMercenary(uiUniqueID);

		CopyMemory(TempBuffer, _pWorld->pMyData->GetMyCharName(), ON_ID_LENGTH);
		if(_pWorld->pIOnlineChar->GetGuildName(uiUniqueID))
			CopyMemory(TempBuffer1, _pWorld->pIOnlineChar->GetGuildName(uiUniqueID), ON_ID_LENGTH);

		_pWorld->pIOnlineChar->GetPos(uiUniqueID, &siX, &siY);

		// ���� ���ΰ� �������� ���ƿ��� ĳ���Ͱ� ������� ���
		if(_pWorld->pMyData->GetMyCharUniqueID() == pOnChangedWarEnd->uiPlayerID)
		{
			_pWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiEquipWeapon);
			_pWorld->pMyData->SetMyCharKind(pOnChangedWarEnd->uiKind);

			// ���� ȿ����
			PushEffect(EFFECT_UNITCHANGE, 0 , 0);	// ȿ���� ���
		}
		else
		{
			_pWorld->pIOnlineChar->GetEquip(uiUniqueID, uiEquipWeapon);
		}

		// ĳ���� ����
		_pWorld->pIOnlineChar->Free(uiUniqueID);
		_pWorld->pIOnlineChar->Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, _pWorld,(UI16*)uiEquipWeapon);
		_pWorld->pIOnlineChar->SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);

		_pWorld->pIOnlineChar->SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
		_pWorld->pIOnlineChar->SetMercenary(uiUniqueID, siMercenaryKind);

		_pWorld->pIOnlineChar->SetChangeEffect(uiUniqueID);

		_pWorld->pOnlineInventory->SetMercenarySlot(0);

		_pWorld->pOnlineCharUI->SetAction(TRUE);
	}
	else	// if (FALSE == (_pWorld->IsSiegeWarfareUnit(siPrevImjinKindIdx)))
	{
		// �̹� ���ΰ� ĳ�����̹Ƿ� ���Ž�ų �ʿ� ����.
	}

	// ���� ���ΰ� �������� ���ƿ��� ĳ���Ͱ� ������� ���
	if(_pWorld->pMyData->GetMyCharUniqueID() == pOnChangedWarEnd->uiPlayerID)
	{
		// �κ��丮�� ���� ������ �ݾ��ش�.
		if(_pWorld->pOnlineInventory->IsActive())
			_pWorld->pOnlineInventory->SetActive(FALSE);

		// ��ΰ� ���� ������ �ݾ��ش�.
		if(_pWorld->pOnlineTradeBook->IsAction())
			_pWorld->pOnlineTradeBook->SetAction(FALSE);

		// �̴ϸʵ� �ݴ´�!!
		if(_pWorld->pOnlineMiniMap->IsActive())
			_pWorld->pOnlineMiniMap->SetActive(FALSE);

		// �� �ݾ�!!
		if(_pWorld->pOnlineSetting->IsAction())
			_pWorld->pOnlineSetting->SetAction(FALSE);

		// ��� ������ �������� �κ��丮���� ����
		_pWorld->DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit();

		_pWorld->SetIsShowInformationAboutSiegeWarfareUnit(0);	// ������ ������ �ƴϴ�.

		// robypark 2005/1/28 14:59
		// ������ ���� �������̽� �ݱ�
		_pWorld->pOnlinePannel->SiegeWarfareUnitUI_DestroyWindow();

		// ���� ����ڰ� ���� �������̽� �� �ִ� ���̶�� �ش� ������ �鿩������.
		if (TRUE == _pWorld->pMyData->IsInVillageDefenceInterface())
		{
			_pWorld->pMyData->SetInVillageDefenceInterface(FALSE);

			// ������ ���� �������̽� �󿡼� �����ڴٰ� ��û
			if (_pWorld->GetVillageDefenceInterfaceInfo()->m_uiVillageCode)
				_pWorld->pOnlineClient->SendLeaveVillageDefenceInterface(_pWorld->GetVillageDefenceInterfaceInfo()->m_uiVillageCode);
		}
	}

	// ���� ���ֿ��� => ���ΰ� ĳ���ͷ� �����ϴ� ���̶��
	if (_pWorld->IsSiegeWarfareUnit(siPrevImjinKindIdx))
		_pWorld->pOnlineCharUI->SetAction(TRUE);

	return 0;
}


// robypark 2004/12/7 12:59
// ���������� �׾ ���� �г�Ƽ�� ������� ������.
// ON_CHANGED_PLAYER_DEAD_PENALTY
UI16 OnlineMsgProc::OnResChangedPlayerDeadPenalty(BYTE *pMsg)
{
	// �ٽ� ���� �����ϴ�
	_pWorld->pMyData->SetTransformSiegeWarfareUnit(true);

	// ���� �޽��� �ڽ� ���
	_pWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", _pWorld->pOnlineText->Get(ON_TEXT_DESTROYED_DEAD_PENALTY));

	return 0;
}

// robypark 2005/2/15 17:42
// �������� ȭģ���ǿ� ���ؼ� �������
// ON_RESPONSE_NOT_WAR
UI16 OnlineMsgProc::OnResponseNotWar(BYTE *pMsg)
{
	_pWorld->pMyData->SetSiegeWarfareDay(FALSE);	// ������ ��������.

	return 0;
}

// robypark 2005/2/16 2:52
// ������ �ð��� �Ǿ ����ڰ� ������ �ִٰ� �˷���
// ON_RESPONE_WAR_TIME
UI16 OnlineMsgProc::OnResponseWarTime(BYTE *pMsg)
{
	_pWorld->pMyData->SetSiegeWarfareDay(TRUE);	// ������ �ִ�.

	// �ʺ��� ���������� ���� �� �� �ִٴ� ��� �޽��� ����
	_pWorld->GetWarningMsgInSiegeWarfare()->m_dwLeftTime = 0;

	return 0;
}
