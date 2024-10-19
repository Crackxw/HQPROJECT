#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"

#include "../SCCommon/OnlineGameMsg.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	해당하는지점부근에 있는 다른플레이어에게 메세지를 보낸다.
//	bSend가 TRUE면 자기자신에게도 메세지를 보내고 FALSE면 보내지 않는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOPlayer::SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5, LPVOID pParam6)
{	
//	SOPlayer		*pPlayer;
	static UI16		uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];	
	SI32			siSendCharNum;
	BOOL			bSendMe = TRUE;	
	SI16			siLength;	
	static CHAR		cLocalBuffer[ON_MAX_LOCALBUFFER];	
	SI32			i;	
	BOOL			bRet = TRUE;						// 이 함수에서 메시지를 다뤘는가?	

	if(BaseChar::SendFieldMsg(siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5) == TRUE)
		return TRUE;

	switch(siMsg)
	{
	case ON_CHANGE_FIELDITEM:
		{
			OnChangeFieldItemMsg *pFieldItem					=	(OnChangeFieldItemMsg *)cLocalBuffer;

			*pFieldItem = *(OnChangeFieldItemMsg *)pParam3;
			
            
			siLength = sizeof(OnChangeFieldItemMsg);
			

			if(siSendInfo == SO_SFM_CUSTOM)
			{
				siSendCharNum				=	SI32(pParam1);
				memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);			
			}	

		}
		break;
	case ON_RESPONSE_CHAR_DETAILINFO:
		{
			OnResCharDetailInfoMsg		*pCharDetailInfoMsg;			
			OnCharPlayerDetailInfo		*pCharDetailInfo;
			BaseChar					*pBaseChar;
			SOPlayer					*pPlayer;
			
			pCharDetailInfoMsg					=	(OnResCharDetailInfoMsg*)cLocalBuffer;
			pCharDetailInfoMsg->dwMsgType		=	ON_RESPONSE_CHAR_DETAILINFO;
			pCharDetailInfoMsg->dwResponse		=	DWORD(pParam1);

			siLength						=	sizeof(OnResCharDetailInfoMsg);

			if(pCharDetailInfoMsg->dwResponse == ON_RET_OK)
			{
				pCharDetailInfo					=	(OnCharPlayerDetailInfo*)((CHAR*)pCharDetailInfoMsg + sizeof(OnResCharDetailInfoMsg));
				pBaseChar						=	(BaseChar*)pParam2;				
			
				pCharDetailInfoMsg->uiAccount	=	pBaseChar->uiAccount;

				if(ISPLAYER(pBaseChar->uiAccount) == TRUE)
				{
					pPlayer						=	(SOPlayer*)pBaseChar;
					siLength					+=	pPlayer->GetDetailInfo(pCharDetailInfo, uiGuildID, siClassInGuild, stAccountInfo.siDBAccount );
				}
				else if(ISMONSTER(pBaseChar->uiAccount) == TRUE)
				{
				}
				else if(ISSHIP(pBaseChar->uiAccount) == TRUE)
				{
				}
			}
		}	
		break;
	case ON_NEWINFOONMOVE:
		// 캐릭터가 이동시 새로운 정보.
		{
			
			OnNewInfoOnMoveMsg			*pNewInfoOnMoveMsg;
			OnlineReceiveFieldItemInfo	*pFieldItemInfoIn, *pFieldItemInfoOut;

			SI32						*psiParam1;
			UI16						*puiDelCharID;			
			CHAR						*pReceivedPlayerInfo;			
			UI16						*puiInAddDelCharID;		
			DWORD						uiPlayersInfoMsgLength;

			psiParam1				=	(SI32 *)pParam1;
			puiInAddDelCharID		=	(UI16 *)pParam2;
			pFieldItemInfoIn		=	(OnlineReceiveFieldItemInfo*)pParam3;

			pNewInfoOnMoveMsg					=	(OnNewInfoOnMoveMsg*)cLocalBuffer;
			pNewInfoOnMoveMsg->dwMsgType		=	ON_NEWINFOONMOVE;
			pNewInfoOnMoveMsg->uiDelCharNum		=	psiParam1[0];
			pNewInfoOnMoveMsg->uiAddCharNum		=	psiParam1[1];
			pNewInfoOnMoveMsg->uiItemNum		=	psiParam1[2];
			
			puiDelCharID		=	(UI16*)((CHAR*)pNewInfoOnMoveMsg + sizeof(OnNewInfoOnMoveMsg));
			
			// 삭제해야 할 캐릭터를 복사한다.
			memcpy( puiDelCharID, puiInAddDelCharID, sizeof(UI16) * pNewInfoOnMoveMsg->uiDelCharNum );			
			
			pReceivedPlayerInfo	=	(CHAR*)((CHAR*)puiDelCharID + sizeof(UI16) * pNewInfoOnMoveMsg->uiDelCharNum);
			
			// 추가해야할 캐릭터...
			uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(
												pReceivedPlayerInfo, 
												pNewInfoOnMoveMsg->uiAddCharNum, 
												&puiInAddDelCharID[pNewInfoOnMoveMsg->uiDelCharNum], 												
												&pNewInfoOnMoveMsg->uiAddCharNum, 1);
			
			pFieldItemInfoOut	=	(OnlineReceiveFieldItemInfo*)( (CHAR*)pReceivedPlayerInfo + uiPlayersInfoMsgLength );


			// 추가해야 할 아이템을 복사한다.
			memcpy( pFieldItemInfoOut, pFieldItemInfoIn, sizeof(OnlineReceiveFieldItemInfo) * pNewInfoOnMoveMsg->uiItemNum );
		

			siLength				=	sizeof(OnNewInfoOnMoveMsg) + 
										sizeof(UI16) * pNewInfoOnMoveMsg->uiDelCharNum + 
										uiPlayersInfoMsgLength + 	
										sizeof(OnlineReceiveFieldItemInfo) * pNewInfoOnMoveMsg->uiItemNum; 
		}
		break;		

	case ON_RESPONSE_SET_ATTACK_MODE:
		{
			OnResSetAttackMode			*pSendMsg;

			pSendMsg					=	(OnResSetAttackMode*)cLocalBuffer;
			pSendMsg->dwMsgType			=	siMsg;
			pSendMsg->uiAccount			=	uiAccount;
			pSendMsg->bAttackMode		=	DWORD(pParam1);

			siLength					=	sizeof(OnResSetAttackMode);
		}
		break;
	case ON_RESPONSE_GUILD_SET_RELATION:					// 일반적인 Response이다.
		{			
			OnResRelationMsg			*pResRelationMsg;

			pResRelationMsg				=	(OnResRelationMsg*)cLocalBuffer;
			pResRelationMsg->dwMsgType	=	siMsg;
			pResRelationMsg->dwResponse	=	DWORD(pParam1);
			pResRelationMsg->bRelation  =   DWORD(pParam2);
		
			siLength					=	sizeof(OnResRelationMsg);
		}
		break;
	case ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER:
		{
			OnResGuildFriendlyMessageOffer*		pResGuildFriendlyMessageOffer;

			pResGuildFriendlyMessageOffer	=	(OnResGuildFriendlyMessageOffer*)cLocalBuffer;
			pResGuildFriendlyMessageOffer->dwMsgType	=	ON_RESPONSE_GUILD_FRIELDLY_MESSAGE_OFFER;
			pResGuildFriendlyMessageOffer->dwResponse	=	DWORD(pParam1);

			siLength					=	sizeof(OnResGuildFriendlyMessageOffer);
		}
		break;
	case ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT:
		{
			OnResGuildFriendlyMessageAccept*	pResGuildFriendlyMessageAccept;

			pResGuildFriendlyMessageAccept	=	(OnResGuildFriendlyMessageAccept*)cLocalBuffer;
			pResGuildFriendlyMessageAccept->dwMsgType	=	ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_ACCEPT;

			siLength					=	sizeof(OnResGuildFriendlyMessageAccept);
		}
		break;
	case ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL:
		{
			OnResGuildFriendlyMessageCancel*	pResGuildFriendlyMessageCancel;

			pResGuildFriendlyMessageCancel	=	(OnResGuildFriendlyMessageCancel*)cLocalBuffer;
			pResGuildFriendlyMessageCancel->dwMsgType	=	ON_RESPONSE_GUILD_FRIENDLY_MESSAGE_CANCEL;

			siLength					=	sizeof(OnResGuildFriendlyMessageCancel);
		}
		break;
	case ON_RESPONSE_MOVEITEM:
		{
			OnResMoveItemMsg	*pResMoveItemMsg;

			pResMoveItemMsg				=	(OnResMoveItemMsg*)cLocalBuffer;
			pResMoveItemMsg->dwMsgType	=	ON_RESPONSE_MOVEITEM;
			pResMoveItemMsg->dwResponse	=	DWORD(pParam1);

			siLength			=	sizeof(OnResMoveItemMsg);

		}	
		break;	
	case ON_RESPONSE_LEAVE_VILLAGE:
		{
			OnResGoOutVillageMsg		*pGoOutVillageMsg;
			OnResGoOutVillageOkMsg		*pGoOutVillageOkMsg;
			OnResGoOutVillageNoMsg		*pGoOutVillageNoMsg;
			CHAR						*pReceivedPlayerInfo;
			OnlineReceiveFieldItemInfo	*pRecvFItemInfoIn, *pRecvFItemInfoOut;
			UI16						uiNearCharNum, uiNearFItemNum;					
			UI16						*puiCharUniIDs;	
			UI32						uiPlayersInfoMsgLength;		

			pGoOutVillageMsg	=	(OnResGoOutVillageMsg*)cLocalBuffer;
			pGoOutVillageMsg->dwMsgType		=	ON_RESPONSE_LEAVE_VILLAGE;
			pGoOutVillageMsg->dwResponse	=	DWORD(pParam1);	
			
			
			if(pGoOutVillageMsg->dwResponse == ON_RET_LEAVE_VILLAGE_OK)
			{
				pGoOutVillageOkMsg	=	(OnResGoOutVillageOkMsg*)pGoOutVillageMsg;

				uiNearCharNum		=	((UI16 *)pParam2)[0];
				uiNearFItemNum		=	((UI16 *)pParam2)[1];
				puiCharUniIDs		=	(UI16 *)pParam3;
				pRecvFItemInfoIn	=	(OnlineReceiveFieldItemInfo*)pParam4;				

				pGoOutVillageOkMsg->uiPlayerNum		=	uiNearCharNum;
				pGoOutVillageOkMsg->uiFieldItemNum	=	uiNearFItemNum;							

				pReceivedPlayerInfo		=	(CHAR*)((CHAR*)pGoOutVillageOkMsg + sizeof(OnResGoOutVillageOkMsg));

				uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pReceivedPlayerInfo, pGoOutVillageOkMsg->uiPlayerNum, 
											puiCharUniIDs, &pGoOutVillageOkMsg->uiPlayerNum, 2);
				
				pRecvFItemInfoOut	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pReceivedPlayerInfo + uiPlayersInfoMsgLength);
				memcpy(pRecvFItemInfoOut, pRecvFItemInfoIn, 
						sizeof(OnlineReceiveFieldItemInfo) * pGoOutVillageOkMsg->uiFieldItemNum);				

				siLength				=	sizeof(OnResGoOutVillageOkMsg) + 
											uiPlayersInfoMsgLength +
											(sizeof(OnlineReceiveFieldItemInfo) * pGoOutVillageOkMsg->uiFieldItemNum);
			}
			else if(pGoOutVillageMsg->dwResponse == ON_RET_LEAVE_VILLAGE_NO)
			{
				pGoOutVillageNoMsg		=	(OnResGoOutVillageNoMsg*)pGoOutVillageMsg;
				siLength				=	sizeof(OnResGoOutVillageNoMsg);
			}

		}
		break;
	case ON_RESPONSE_ENTER_VILLAGE:	
		{			
			OnResGotoVillageOkMsg	*pGotoVillageOkMsg;
			OnResponseMsg			*pResponseMsg;			
			OnRecvVillageInfo		*pVillageInfo;						
			SOVillage				*pVillage;

			pResponseMsg		=	(OnResponseMsg*)cLocalBuffer;			
			pResponseMsg->dwMsgType		=	ON_RESPONSE_ENTER_VILLAGE;
			pResponseMsg->dwResponse	=	DWORD(pParam1);						

			pVillage					=	(SOVillage*)pParam2;

			switch(pResponseMsg->dwResponse)
			{
			case ON_RET_OK:			
				pGotoVillageOkMsg				=	(OnResGotoVillageOkMsg*)pResponseMsg;				
				pGotoVillageOkMsg->uiCharID		=	uiAccount;			

				switch(siSendInfo)
				{
				case SO_SFM_ONLYNEARCHAR:
					// 오직 내 주위에게만 보내는 메시지이다.
					// 마을의 정보는 보낼 필요가 없다.
					siLength						=	sizeof(OnResGotoVillageOkMsg);			
					break;
				case SO_SFM_ONLYME:
					// 오직 나에게만 보내는 메시지이다.
					// 마을의 정보까지 포함해서 같이 보내준다.					
					pVillageInfo					=	(OnRecvVillageInfo*)((CHAR*)pGotoVillageOkMsg + sizeof(OnResGotoVillageOkMsg));				
					pVillage->GetVillageInfo(this, pVillageInfo);
					
					siLength						=	sizeof(OnResGotoVillageOkMsg) + sizeof(OnRecvVillageInfo);										
					break;
				}			
				break;
			case ON_RET_NO:
			case ON_RET_ENEMY_GUILD_VILLAGE:
				siLength						=	sizeof(OnResponseMsg);
				break;
			}		
			
		}
		break;	
	case ON_RESPONSE_VILLAGE_INFO:
		{			
			OnResVillageInfoMsg		*pVillageInfoMsg;
			SOVillage				*pVillage;
			OnRecvVillageInfo		*pVillageInfo;
			

			pVillageInfoMsg					=	(OnResVillageInfoMsg*)cLocalBuffer;
			pVillageInfoMsg->dwMsgType		=	ON_RESPONSE_VILLAGE_INFO;
			pVillageInfoMsg->dwResponse		=	DWORD(pParam1);			
			
			siLength					=	sizeof( OnResVillageInfoMsg );

			switch( pVillageInfoMsg->dwResponse )
			{
			case ON_RET_OK:
				pVillageInfo					=	(OnRecvVillageInfo*)( ( CHAR* )pVillageInfoMsg + siLength );
				pVillage						=	( SOVillage* )pParam2;

				pVillageInfoMsg->uiVillageCode	=	pVillage->uiVillageCode;
				siLength					+=	pVillage->GetVillageInfo( this, pVillageInfo );							
				break;			
			}		
		}
		break;
	case ON_RESPONSE_LIST_ITEM:
		{			
			OnlineReceivedItemInfo		*pRecvOutItemInfo;
			OnResItemListInMarketMsg	*pItemListInMarketMsg;		

			pItemListInMarketMsg			=	(OnResItemListInMarketMsg*)cLocalBuffer;			
			pRecvOutItemInfo				=	(OnlineReceivedItemInfo*)((CHAR*)pItemListInMarketMsg + sizeof(OnResItemListInMarketMsg));
	
			pItemListInMarketMsg->dwMsgType	=	ON_RESPONSE_LIST_ITEM;
			pItemListInMarketMsg->dwItemNum	=	(SI32)(pParam1);			

			memcpy(pRecvOutItemInfo, pParam2, sizeof(OnlineReceivedItemInfo) * pItemListInMarketMsg->dwItemNum);

			siLength						=	sizeof(OnResItemListInMarketMsg) +
												(sizeof(OnlineReceivedItemInfo) * pItemListInMarketMsg->dwItemNum);		
		}
		break;
	case ON_RESPONSE_THROWITEM:
		{			
			OnResThrowItemMsg		*pThrowItemMsg;						

			pThrowItemMsg				=	( OnResThrowItemMsg* )cLocalBuffer;			

			pThrowItemMsg->dwMsgType	=	ON_RESPONSE_THROWITEM;
			pThrowItemMsg->dwResponse	=	DWORD( pParam1 );			

			siLength					=	sizeof( OnResThrowItemMsg );			
		}
		break;
	case ON_RESPONSE_PICKUPITEM:
		{
			OnResPickUpItemMsg		*pResPickUpItemMsg;			
			
			pResPickUpItemMsg				=	(OnResPickUpItemMsg*)cLocalBuffer;			

			pResPickUpItemMsg->dwMsgType	=	ON_RESPONSE_PICKUPITEM;
			pResPickUpItemMsg->dwResponse	=	DWORD( pParam1 );			

			siLength						=	sizeof( OnResPickUpItemMsg );			
		}

		break;
	case ON_RESPONSE_BUY_ITEM:
		{
			OnResBuyItemMsg			*pResBuyItemMsg;
			OnResBuyItemOkMsg		*pResBuyItemOkMsg;		

			pResBuyItemMsg				=	(OnResBuyItemMsg*)cLocalBuffer;
			pResBuyItemMsg->dwMsgType	=	ON_RESPONSE_BUY_ITEM;
			pResBuyItemMsg->dwResponse	=	DWORD(pParam1);

			switch(pResBuyItemMsg->dwResponse)
			{
			case ON_RET_OK:
				pResBuyItemOkMsg				=	(OnResBuyItemOkMsg*)pResBuyItemMsg;
				//pResBuyItemOkMsg->siMoney		=	(MONEY)(*pParam2);
				memcpy(&pResBuyItemOkMsg->siMoney,pParam2,sizeof(MONEY));
				pResBuyItemOkMsg->uiTradeCredit	=	(UI32)(pParam3);
				siLength						=	sizeof(OnResBuyItemOkMsg);
				break;
			default:
				siLength					=	sizeof(OnResBuyItemMsg);
				break;
			}			
		}
		break;
	case ON_RESPONSE_SELL_ITEM:
		{
			OnResSellItemMsg		*pResSellItemMsg;
			OnResSellItemOkMsg		*pResSellItemOkMsg;			

			pResSellItemMsg				=	(OnResSellItemMsg*)cLocalBuffer;
			pResSellItemMsg->dwMsgType	=	ON_RESPONSE_SELL_ITEM;
			pResSellItemMsg->dwResponse	=	DWORD(pParam1);

			switch(pResSellItemMsg->dwResponse)
			{
			case ON_RET_OK:
				pResSellItemOkMsg					=	(OnResSellItemOkMsg*)pResSellItemMsg;				
				memcpy(&pResSellItemOkMsg->siMoney,pParam2,sizeof(MONEY));
				pResSellItemOkMsg->uiTradeCredit	=	(UI32)(pParam3);
				siLength							=	sizeof(OnResSellItemOkMsg);
				break;
			default:
				siLength					=	sizeof(OnResSellItemMsg);
				break;
			}			
		}	
		break;
	case ON_RESPONSE_RETURNONLINE:	
		{			

			OnResReturnOnlineMsg		*pReturnOnlineMsg;			
			OnResReturnOnlineFieldMsg	*pReturnOnlineFieldMsg;
			OnResReturnOnlineVillageMsg	*pReturnOnlineVillageMsg;
			CHAR						*pReceivedPlayerInfo;
			OnlineReceiveFieldItemInfo	*pRecvFItemInfoIn, *pRecvFItemInfoOut;
			OnFollowerInfo				*pSendFollowerInfo;			
			UI16						*puiCharUniIDs;							
			UI32						uiPlayersInfoMsgLength;			
			SOVillage					*pVillage;
			DWORD						dwFollowerSendSize;			
			
			pReturnOnlineMsg				=	(OnResReturnOnlineMsg*)cLocalBuffer;
			pReturnOnlineMsg->dwMsgType		=	ON_RESPONSE_RETURNONLINE;												
			pReturnOnlineMsg->dwResponse	=	DWORD(pParam1);
			pReturnOnlineMsg->siMoney		=	this->siMoney;
			pReturnOnlineMsg->siHealth		=	m_satiety;			
			
			switch(pReturnOnlineMsg->dwResponse)
			{
			case ON_RET_RETURNONLINE_FIELD:
				pReturnOnlineFieldMsg					=	(OnResReturnOnlineFieldMsg*)pReturnOnlineMsg;
				pReturnOnlineFieldMsg->uiPlayerNum		=	((UI16 *)pParam2)[0];
				pReturnOnlineFieldMsg->uiFieldItemNum	=	((UI16 *)pParam2)[1];
				puiCharUniIDs							=	(UI16 *)pParam3;
				pRecvFItemInfoIn						=	(OnlineReceiveFieldItemInfo*)pParam4;							
				
				pReturnOnlineFieldMsg->siEventRanking	=	pMyWorld->m_ExpoRankingSystem.GetRank( stAccountInfo.siDBAccount );
				pReturnOnlineFieldMsg->siEventScore		=	pMyWorld->m_ExpoRankingSystem.GetScore( stAccountInfo.siDBAccount );

				// 포인터를 설정해준다.			
				pReceivedPlayerInfo	=	(CHAR*)((CHAR*)pReturnOnlineFieldMsg + sizeof(OnResReturnOnlineFieldMsg));
				
				uiPlayersInfoMsgLength	=	MakeOnReceivedPlayerInfoPacket(pReceivedPlayerInfo, pReturnOnlineFieldMsg->uiPlayerNum, 
									puiCharUniIDs, &pReturnOnlineFieldMsg->uiPlayerNum, 3 );
				
				// 내 주위의 아이템 정보.			
				pRecvFItemInfoOut	=	(OnlineReceiveFieldItemInfo*)((CHAR*)pReceivedPlayerInfo + uiPlayersInfoMsgLength);
				memcpy(pRecvFItemInfoOut, pRecvFItemInfoIn, sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum);

				pSendFollowerInfo	=	(OnFollowerInfo*)((CHAR*)pRecvFItemInfoOut + 
										sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum);			
				break;
			case ON_RET_RETURNONLINE_VILLAGE:			//	죽었을 경우
				pReturnOnlineVillageMsg					=	(OnResReturnOnlineVillageMsg*)pReturnOnlineMsg;
				pVillage								=	(SOVillage*)pParam2;
	
				pReturnOnlineVillageMsg->uiVillageCode				=	pVillage->uiVillageCode;
				pVillage->GetVillageInfo(this, &pReturnOnlineVillageMsg->VillageInfo);		

				pSendFollowerInfo									=	(OnFollowerInfo*)((CHAR*)pReturnOnlineVillageMsg + sizeof(OnResReturnOnlineVillageMsg));
				break;
			}
			
			// 부하들의 정보를 채워 넣는다.
			dwFollowerSendSize	=	GetAllFollowerInfo(pSendFollowerInfo, &pReturnOnlineMsg->uiFollowerNum);		
			
			// 보낼 메시지의 길이를 설정한다.
			switch(pReturnOnlineMsg->dwResponse)
			{
			case ON_RET_RETURNONLINE_FIELD:
				siLength				=	sizeof(OnResReturnOnlineFieldMsg) + 	
											uiPlayersInfoMsgLength +
											(sizeof(OnlineReceiveFieldItemInfo) * pReturnOnlineFieldMsg->uiFieldItemNum) + dwFollowerSendSize;
				break;
			case ON_RET_RETURNONLINE_VILLAGE:
				siLength				=	sizeof(OnResReturnOnlineVillageMsg) + dwFollowerSendSize;
				break;
			}			
		}		
		break;
	case ON_RESPONSE_INN_CREATECHATROOM:
		{
			OnResCreateChatRoomMsg		*pCreateChatRoomMsg;

			pCreateChatRoomMsg				=	(OnResCreateChatRoomMsg*)cLocalBuffer;
			pCreateChatRoomMsg->dwMsgType	=	ON_RESPONSE_INN_CREATECHATROOM;
			pCreateChatRoomMsg->dwResponse	=	DWORD(pParam1);

			siLength						=	sizeof(OnResCreateChatRoomMsg);			
		}
		break;
	case ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM:
		{
			UI16							*puiCharUniIDs;
			OnResJoinVilInnChatRoomMsg		*pJoinVilInnChatRoomMsg;
			OnResJoinVilInnChatRoomOkMsg	*pJoinVilInnChatRoomOkMsg;
			OnVilINNChatPlayerInfo			*pPlayerInfo;		
			UI32							i;					// Unsigned를 위해 i를 또 선언한다.

			pJoinVilInnChatRoomMsg	=	(OnResJoinVilInnChatRoomMsg*)cLocalBuffer;

			pJoinVilInnChatRoomMsg->dwMsgType	=	ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM;
			pJoinVilInnChatRoomMsg->dwResponse	=	DWORD(pParam1);

			switch(pJoinVilInnChatRoomMsg->dwResponse)
			{
			case ON_RET_INN_OK:				
				pJoinVilInnChatRoomOkMsg	=	(OnResJoinVilInnChatRoomOkMsg*)pJoinVilInnChatRoomMsg;
				pPlayerInfo					=	(OnVilINNChatPlayerInfo*)((CHAR*)pJoinVilInnChatRoomOkMsg + sizeof(OnResJoinVilInnChatRoomOkMsg));

				pJoinVilInnChatRoomOkMsg->uiPlayerNum	=	(UI32)(pParam2);
				puiCharUniIDs							=	(UI16*)pParam3;

				pJoinVilInnChatRoomOkMsg->dwUniqueID	=	DWORD(pParam4);	

				for(i = 0; i < pJoinVilInnChatRoomOkMsg->uiPlayerNum; i++)							
				{
					strncpy(pPlayerInfo[i].szName,
							pMyWorld->pPlayerOnAccount[puiCharUniIDs[i]]->szID, ON_ID_LENGTH);

					pPlayerInfo[i].siDBAccount			=	pMyWorld->pPlayerOnAccount[puiCharUniIDs[i]]->stAccountInfo.siDBAccount;
					pPlayerInfo[i].uiPlayerUniqueID		=	puiCharUniIDs[i];
				}					
					
				siLength		=	sizeof(OnResJoinVilInnChatRoomOkMsg) + 
									(sizeof(OnVilINNChatPlayerInfo) * pJoinVilInnChatRoomOkMsg->uiPlayerNum);
				break;
			case ON_RET_INN_NO:
				siLength		=	sizeof(OnResJoinVilInnChatRoomMsg);
				break;
			}			
		}
		break;
	case ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM:
		{
			OnResLeaveVilInnChatRoomMsg		*pLeaveVilInnChatRoomMsg;
			OnResLeaveVilInnChatRoomOkMsg	*pLeaveVilInnChatRoomOkMsg;
			OnInnChatRoomInfo				*pInnChatRoomInfo;
			stChatRoom						*pChatRoom;
			CHAR							*pszChatRoomName;
			UI32							uiChatRoomIndex;
			UI32							uiTotalChatRoomNameStringLength;

			pLeaveVilInnChatRoomMsg				=	(OnResLeaveVilInnChatRoomMsg*)cLocalBuffer;
			pLeaveVilInnChatRoomMsg->dwMsgType	=	ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM;
			pLeaveVilInnChatRoomMsg->dwResponse	=	DWORD(pParam1);		
			pChatRoom							=	(stChatRoom*)pParam2;

			switch(pLeaveVilInnChatRoomMsg->dwResponse)
			{
			case ON_RET_INN_OK:
				// 성공적으로 들어갈 수 있다.
				pLeaveVilInnChatRoomOkMsg			=	(OnResLeaveVilInnChatRoomOkMsg*)pLeaveVilInnChatRoomMsg;			
				pInnChatRoomInfo					=	(OnInnChatRoomInfo*)((CHAR*)pLeaveVilInnChatRoomMsg + sizeof(OnResLeaveVilInnChatRoomOkMsg));				
				
				// 기본적인 방의 정보를 준비해준다.
				pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum	=	0;
				for(uiChatRoomIndex = 0; uiChatRoomIndex < ON_MAX_CHATROOM_NUM; uiChatRoomIndex++)
				{
					if(pChatRoom[uiChatRoomIndex].uiJoinedPlayerNum >= 1)
					{						
						// 한 사람 이상이라두 있어야 한다.

						pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiChatRoomID				=	CHATROOMINDEXTOID(uiChatRoomIndex);
						pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiJoinedPlayerNum			=	pChatRoom[uiChatRoomIndex].uiJoinedPlayerNum;
						pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiChatRoomNameStringLength	=	strlen(pChatRoom[uiChatRoomIndex].szChatRoomName);						

/*						printf("방 ID : [%u], 플레이어 수 : [%u], 방 이름 : [%s], 방 이름 길이 :[%d]\n", 
							pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiChatRoomID,
							pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiJoinedPlayerNum,
							pChatRoom[uiChatRoomIndex].szChatRoomName,
							pInnChatRoomInfo[pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum].uiChatRoomNameStringLength);*/

						pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum++;
					}
				}

//				printf("총 방 개수 : [%d]\n", pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum);

				pszChatRoomName		=	(CHAR*)pInnChatRoomInfo + (sizeof(OnInnChatRoomInfo) * pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum);

				// 대화방의 이름을 보내준다.
				uiTotalChatRoomNameStringLength		=	0;
				for(i = 0; i < pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum; i++)
				{
					strncpy(pszChatRoomName, pChatRoom[CHATROOMIDTOINDEX(pInnChatRoomInfo[i].uiChatRoomID)].szChatRoomName, pInnChatRoomInfo[i].uiChatRoomNameStringLength);
					pszChatRoomName					+=	pInnChatRoomInfo[i].uiChatRoomNameStringLength;
					uiTotalChatRoomNameStringLength	+=	pInnChatRoomInfo[i].uiChatRoomNameStringLength;
				}

				// 보내는 사이즈를 결정한다.
				siLength							=	sizeof(OnResLeaveVilInnChatRoomOkMsg) + (sizeof(OnInnChatRoomInfo) * pLeaveVilInnChatRoomOkMsg->uiTotalChatRoomNum) + 
														uiTotalChatRoomNameStringLength;
														
				break;
			case ON_RET_INN_NO:
				// 들어가는데 실패했다.
				siLength							=	sizeof(OnResLeaveVilInnChatRoomMsg);
				break;
			}		
		}
		break;
	case ON_VIL_INN_JOIN_PLAYER:
		{
			OnVilInnChatRoomJoinMsg	*pVilInnChatRoomJoinMsg;			

			pVilInnChatRoomJoinMsg										=	(OnVilInnChatRoomJoinMsg*)cLocalBuffer;
			pVilInnChatRoomJoinMsg->dwMsgType							=	ON_VIL_INN_JOIN_PLAYER;				
			pVilInnChatRoomJoinMsg->JoinPlayerInfo.siDBAccount			=	stAccountInfo.siDBAccount;
			pVilInnChatRoomJoinMsg->JoinPlayerInfo.uiPlayerUniqueID		=	uiAccount;
			strncpy(pVilInnChatRoomJoinMsg->JoinPlayerInfo.szName, szID, ON_ID_LENGTH);

			// 보낼 사람들.
			siSendCharNum					=	(SI32)(pParam1);
			memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);		
			
			// 보낼 사이즈.
			siLength						=	sizeof(OnVilInnChatRoomJoinMsg);			
		}
		break;
	case ON_VIL_INN_LEAVE_PLAYER:
		{
			OnVilInnChatRoomLeaveMsg	*pVilInnChatRoomLeaveMsg;

			pVilInnChatRoomLeaveMsg						=	(OnVilInnChatRoomLeaveMsg*)cLocalBuffer;
			pVilInnChatRoomLeaveMsg->dwMsgType			=	ON_VIL_INN_LEAVE_PLAYER;
			pVilInnChatRoomLeaveMsg->uiPlayerUniqueID	=	uiAccount;

			// 보낼 사람들.
			siSendCharNum						=	(SI32)(pParam1);
			memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);		
			
			// 보낼 사이즈.
			siLength							=	sizeof(OnVilInnChatRoomLeaveMsg);			
		}
		break;
	case ON_RESPONSE_ENTER_MARKET:
		{
			OnResEnterMarketMsg		*pResEnterMarketMsg;

			pResEnterMarketMsg					=	(OnResEnterMarketMsg*)cLocalBuffer;
			pResEnterMarketMsg->dwMsgType		=	ON_RESPONSE_ENTER_MARKET;
			pResEnterMarketMsg->dwResponse		=	DWORD(pParam1);

			siLength							=	sizeof(OnResEnterMarketMsg);			
		}
		break;
	case ON_RESPONSE_LEAVE_MARKET:
		{
			OnResLeaveMarketMsg		*pResLeaveMarketMsg;

			pResLeaveMarketMsg					=	(OnResLeaveMarketMsg*)cLocalBuffer;
			pResLeaveMarketMsg->dwMsgType		=	ON_RESPONSE_LEAVE_MARKET;
			pResLeaveMarketMsg->dwResponse		=	DWORD(pParam1);

			siLength							=	sizeof(OnResLeaveMarketMsg);			
		}
		break;
	case ON_RESPONSE_CHANGED_ITEMINFO:
		{
			OnChangeItemInfoMsg		*pChangeItemInfoMsg;			
			
			siSendCharNum									=	SI32( pParam1 );
			memcpy( uiCharUniIDs, pParam2, sizeof( UI16 ) * siSendCharNum );
	
			pChangeItemInfoMsg								=	(OnChangeItemInfoMsg*)cLocalBuffer;
			pChangeItemInfoMsg->dwMsgType					=	ON_RESPONSE_CHANGED_ITEMINFO;
			pChangeItemInfoMsg->ItemInfo.uiID				=	UI16( pParam3 );
			pChangeItemInfoMsg->ItemInfo.uiQuantity			=	UI16( pParam4 );
			pChangeItemInfoMsg->ItemInfo.BasePrice			=	*(( MONEY* )pParam5);			

			siLength										=	sizeof(OnChangeItemInfoMsg);			
		}
		break;
	case ON_CHANGED_TIME:
		{
			OnChangedTimeMsg					*pChangedTimeMsg;

			pChangedTimeMsg						=	(OnChangedTimeMsg*)cLocalBuffer;

			pChangedTimeMsg->dwMsgType			=	ON_CHANGED_TIME;

			memcpy(&pChangedTimeMsg->Time, &pMyWorld->CurrentTime, sizeof(SOnlineTime));	// 게임에서의 시간을 보낸다.
			pChangedTimeMsg->RealWorldTime		=	pMyWorld->RealWorldTime;				// 실세계의 시간을 보낸다.			

			siLength							=	sizeof(OnChangedTimeMsg);			

//			printf("Send ON_CHANGED_TIME Msg\n");
		}
		break;
	case ON_RESPONSE_IMOTICON:
		{
			OnResImoticonMsg					*pImoticonMsg;
			pImoticonMsg						=	(OnResImoticonMsg*)	cLocalBuffer;
			pImoticonMsg->dwMsgType				=	ON_RESPONSE_IMOTICON;
			pImoticonMsg->UserID				=	(UI16)(pParam1);
			pImoticonMsg->uiImoticonID			=	(UI16)(pParam2);

			siLength							=	sizeof(OnResImoticonMsg);
		}
		break;
	case ON_TARGETME:
		{
			OnTargetMeMsg						*pTargetMeMsg;
			pTargetMeMsg						=	(OnTargetMeMsg*) cLocalBuffer;
			pTargetMeMsg->dwMsgType				=	ON_TARGETME;
			pTargetMeMsg->uiMonsterID			=	WORD(pParam1);

			siLength							=	sizeof(OnTargetMeMsg);
		}
		break;
	case ON_RELEASEME:
		{
			OnReleaseMeMsg						*pReleaseMeMsg;
			pReleaseMeMsg						=	(OnReleaseMeMsg*) cLocalBuffer;
			pReleaseMeMsg->dwMsgType			=	ON_RELEASEME;
			pReleaseMeMsg->uiMonsterID			=	WORD(pParam1);

			siLength							=	sizeof(OnReleaseMeMsg);
		}
		break;
	case ON_RESPONSE_VILLAGE_NATION:
		{
			OnResVillageGetNation				*pVillageGetNation;
			SOVillage							*pVillage;
			OnRecvVillageInfo					stVillageInfo;
			
			pVillageGetNation					=	(OnResVillageGetNation*) cLocalBuffer;
			pVillageGetNation->dwMsgType		=	ON_RESPONSE_VILLAGE_NATION;			
			pVillageGetNation->dwResponse		=	DWORD(pParam1);
			pVillage							=	(SOVillage*)pParam2;

			switch(pVillageGetNation->dwResponse)
			{
			case ON_RET_OK:
				pVillage->GetVillageInfo(this, &stVillageInfo);

				pVillageGetNation->uiVillageCode	=	pVillage->uiVillageCode;
				pVillageGetNation->cNationCode		=	stVillageInfo.cNationCode;
				pVillageGetNation->Durability		=	stVillageInfo.vdVillageDefence;
				pVillageGetNation->MaxDurability	=	stVillageInfo.vdVillageMaxDefence;
				memcpy(pVillageGetNation->GuildName, stVillageInfo.szGuildID, ON_GUILDNAME_LENGTH);
				pVillageGetNation->siGuildFlag		=	stVillageInfo.siGuildFlag;
				pVillageGetNation->dwGuildRelation	=	stVillageInfo.dwGuildRelation;
				pVillageGetNation->siDefenceSoldier	=	stVillageInfo.siDefenceSoldier;
				pVillageGetNation->siAttackType		=   stVillageInfo.siAttackType;
				break;
			case ON_RET_NO:
				break;
			}

			siLength							=	sizeof(OnResVillageGetNation);
		}
		break;
	case ON_RESPONSE_READ_DESCRIPT:
		{
			OnResVillageDescript				*pVillageDescript;
			CHAR								*szDesc;
			CHAR								*szTemp;
			CHAR								*pszGuildName;

			// 3번째 인자는 길드의 이름이다.
			pszGuildName	=	(CHAR*)pParam3;

			pVillageDescript					=	(OnResVillageDescript*)	cLocalBuffer;
			szDesc								=	(CHAR*) pParam2;
			pVillageDescript->dwMsgType			=	ON_RESPONSE_READ_DESCRIPT;
			pVillageDescript->uiVillageCode		=	(UI16)	pParam1;
			pVillageDescript->uiLength			=	strlen(szDesc);
			szTemp	=	(CHAR*)pVillageDescript + sizeof(OnResVillageDescript);
			strcpy(szTemp, szDesc);

			ZeroMemory(pVillageDescript->szGuildID, sizeof(pVillageDescript->szGuildID));
			if(pszGuildName != NULL)	strncpy(pVillageDescript->szGuildID, pszGuildName, ON_GUILDNAME_LENGTH);					

			siLength	=	sizeof(OnResVillageDescript) + strlen(szDesc);

		}
		break;
	case ON_RESPONSE_WRITE_DESCRIPT:
		{
			OnResWriteDescript					*pWriteDescript;
			pWriteDescript						=	(OnResWriteDescript*) cLocalBuffer;
			pWriteDescript->dwMsgType			=	ON_RESPONSE_WRITE_DESCRIPT;
			pWriteDescript->dwResponse			=	DWORD(pParam1);

			siLength							=	sizeof(OnResWriteDescript);
		}
		break;
/*	case ON_RESPONSE_CHARGEBOOTH:
		{
			OnResChargeBoothMsg					*pChargeBoothMsg;
			pChargeBoothMsg						=	(OnResChargeBoothMsg*) cLocalBuffer;
			pChargeBoothMsg->dwMsgType			=	ON_RESPONSE_CHARGEBOOTH;
			pChargeBoothMsg->dwResponse			=	DWORD(pParam1);
			
			siLength							=	sizeof(OnResChargeBoothMsg);
		}
		break;
	case ON_RESPONSE_LEAVEBOOTH:
		{
			OnResLeaveBoothMsg					*pLeaveBoothMsg;
			pLeaveBoothMsg						=	(OnResLeaveBoothMsg*)	cLocalBuffer;
			pLeaveBoothMsg->dwMsgType			=	ON_RESPONSE_LEAVEBOOTH;
			pLeaveBoothMsg->dwResponse			=	DWORD(pParam1);

			siLength							=	sizeof(OnResLeaveBoothMsg);
		}
		break;
	case ON_RESPONSE_DISPLAYBOOTH:
		{
			OnResDisplayBoothItemMsg			*pDisplayBoothItemMsg;
			OnResDisplayBoothItemOkMsg			*pDisplayBoothItemOkMsg;
			stBOOTH								*pBooth;

			pDisplayBoothItemMsg				=	(OnResDisplayBoothItemMsg*)	cLocalBuffer;
			pDisplayBoothItemMsg->dwMsgType		=	ON_RESPONSE_DISPLAYBOOTH;
			pDisplayBoothItemMsg->dwResponse	=	DWORD(pParam1);
			pDisplayBoothItemMsg->uiBoothID		=	UI16(pParam2);
			switch(pDisplayBoothItemMsg->dwResponse)
			{
			case ON_RET_MARKETPLACE_OK:
				{
					pDisplayBoothItemOkMsg		=	(OnResDisplayBoothItemOkMsg*)pDisplayBoothItemMsg;
					pBooth						=	(stBOOTH*) pParam3;
					for(i=0;i<ON_MAX_BOOTH_ITEM;i++)
					{
						if(pBooth->BoothItem[i].pItem)
							pDisplayBoothItemOkMsg->uiItemID[i]	=	pBooth->BoothItem[i].pItem->uiID;
						else
							pDisplayBoothItemOkMsg->uiItemID[i]	=	0;
					}

					siLength	=	sizeof(OnResDisplayBoothItemOkMsg);
				}
				break;
			default:
				{
					siLength	=	sizeof(OnResDisplayBoothItemMsg);
				}
				break;
			}
		}
		break;
	case ON_RESPONSE_SETBOOTHITEM:
		{
			OnResSetBoothItemMsg			*pSetBoothItemMsg;
			pSetBoothItemMsg				=	(OnResSetBoothItemMsg*)	cLocalBuffer;
			pSetBoothItemMsg->dwMsgType		=	ON_RESPONSE_SETBOOTHITEM;
			pSetBoothItemMsg->dwResponse	=	DWORD(pParam1);

			siLength			=	sizeof(OnResSetBoothItemMsg);
		}
		break;
	case ON_RESPONSE_CANCELBOOTHITEM:
		{
			OnResCancelBoothItemMsg			*pSetBoothItemMsg;
			pSetBoothItemMsg				=	(OnResCancelBoothItemMsg*)	cLocalBuffer;
			pSetBoothItemMsg->dwMsgType		=	ON_REQUEST_CANCELBOOTHITEM;
			pSetBoothItemMsg->dwResponse	=	DWORD(pParam1);

			siLength			=	sizeof(OnResCancelBoothItemMsg);
		}
		break;
	case ON_RESPONSE_GETBOOTHINFO:
		{
			OnResGetBoothInfoMsg			*pGetBoothInfoMsg;
			OnBoothItemInfo					*pBoothItemInfo;
			LPVOID							Pointer;

			pGetBoothInfoMsg				=	(OnResGetBoothInfoMsg*)	cLocalBuffer;
			pBoothItemInfo					=	(OnBoothItemInfo*)	pParam2;

			pGetBoothInfoMsg->dwMsgType		=	ON_RESPONSE_GETBOOTHINFO;
			pGetBoothInfoMsg->dwResponse	=	DWORD( pParam1 );
			pGetBoothInfoMsg->uiNum			=	DWORD( pParam3 );
			switch(pGetBoothInfoMsg->dwResponse)
			{
			case ON_RET_MARKETPLACE_OK:
				Pointer						=	(CHAR*)pGetBoothInfoMsg+sizeof(OnResGetBoothInfoMsg);
				memcpy(Pointer, pParam2, pGetBoothInfoMsg->uiNum*sizeof(OnBoothItemInfo));
				siLength			=	sizeof(OnResGetBoothInfoMsg)+ pGetBoothInfoMsg->uiNum *sizeof(OnBoothItemInfo);
				break;

			default:
				siLength			=	sizeof(OnResGetBoothInfoMsg);
				break;
			}

		}
		break;
	case ON_RESPONSE_SELLBOOTHITEM:
		{
			OnResSellBoothItemMsg			*pSellBoothItemMsg;

			stBOOTH						*pBooth		=	(stBOOTH*)		pParam1;
			stBOOTHITEM					*pBoothItem	=	(stBOOTHITEM*)	pParam2;
			MyItemData					*pItemData	=	(MyItemData*)	pParam3;

			pSellBoothItemMsg				=	(OnResSellBoothItemMsg*)cLocalBuffer;
			pSellBoothItemMsg->dwMsgType	=	ON_RESPONSE_SELLBOOTHITEM;
			pSellBoothItemMsg->uiFollowerID	=	pBoothItem->FollowerID;
			pSellBoothItemMsg->uiInvenPos	=	pItemData->siPosInInventory;
			pSellBoothItemMsg->uiItemID		=	pItemData->uiID;
			pSellBoothItemMsg->uiQuantity	=	pItemData->uiQuantity;
			pSellBoothItemMsg->siMoney		=	pBooth->pSeller->siMoney;

			siLength	=	sizeof(OnResSellBoothItemMsg);
		}
		break;
	case ON_RESPONSE_BUYBOOTHITEM:
		{
			OnResBuyBoothItemMsg			*pBuyBoothItemMsg;

			pBuyBoothItemMsg				=	(OnResBuyBoothItemMsg*)	cLocalBuffer;
			pBuyBoothItemMsg->dwMsgType		=	ON_RESPONSE_BUYBOOTHITEM;
			pBuyBoothItemMsg->dwResponse	=	DWORD(pParam1);

			siLength		=	sizeof(OnResBuyBoothItemMsg);
		}
		break;*/
	case ON_RESPONSE_AUDIT_TOTAL:
		{			
			OnResAuditTotalMsg				*pAuditTotalMsg;
			OnResAuditTotalOkMsg			*pAuditTotalOkMsg;
			PROPERTY_INFO					*pPropertyData;

			pAuditTotalMsg					=	(OnResAuditTotalMsg*)	cLocalBuffer;
			pAuditTotalMsg->dwMsgType		=	ON_RESPONSE_AUDIT_TOTAL;
			pAuditTotalMsg->dwReponse		=	DWORD( pParam1 );

			if(pAuditTotalMsg->dwReponse == ON_RET_AUDIT_OK)
			{				
				pPropertyData						=	(PROPERTY_INFO*)	pParam2;
				
				pAuditTotalOkMsg					=	(OnResAuditTotalOkMsg*)	pAuditTotalMsg;
				pAuditTotalOkMsg->siLoanMoney		=	pPropertyData->siLoanProperty;
				pAuditTotalOkMsg->siBankMoney		=	pPropertyData->siSaveProperty;
				pAuditTotalOkMsg->siProperty		=	pPropertyData->siTotalProperty;				
				pAuditTotalOkMsg->uiTotalUser		=	0; //** kojimodo
				
				
				siLength						=	sizeof(OnResAuditTotalOkMsg);
			}
			else
			{
				siLength						=	sizeof(OnResAuditTotalMsg);
			}
		}
		break;
	case ON_RESPONSE_AUDIT_ACCOUNT:
		{
			//SendFieldMsg(ON_RESPONSE_AUDIT_ACCOUNT,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK),LPVOID(&BankBookList));
			OnResAuditAccountListMsg			*pAuditAccountListMsg;
			OnResAuditAccountListOkMsg			*pAuditAccountListOkMsg;
			DBIKBankBookList					*pBankBookList;
			OnAccountNumber						*pAccountNumber;
			MONEY								*psiMoney;

			pAuditAccountListMsg				=	(OnResAuditAccountListMsg*)	cLocalBuffer;
			pAuditAccountListMsg->dwMsgType		=	ON_RESPONSE_AUDIT_ACCOUNT;
			pAuditAccountListMsg->dwResponse	=	DWORD( pParam1 );

			if(pAuditAccountListMsg->dwResponse	== ON_RET_AUDIT_OK )
			{
				pAuditAccountListOkMsg			=	(OnResAuditAccountListOkMsg*)	pAuditAccountListMsg;
				pBankBookList					=	(DBIKBankBookList*)	pParam2;

				pAuditAccountListOkMsg->dwBankBookNum	=	pBankBookList->siBankBookNum;
								
				pAccountNumber							=	(OnAccountNumber*)((CHAR*)pAuditAccountListOkMsg + sizeof(OnResAuditAccountListOkMsg));
				psiMoney								=	(MONEY*)((CHAR*)pAccountNumber + sizeof(OnAccountNumber) * pAuditAccountListOkMsg->dwBankBookNum);

				//printf("ON_RET_BANK_OK : 4 [%d]\n", pDBIKBankBookList->siBankBookNum);
				for(i = 0; i < (SI32)pAuditAccountListOkMsg->dwBankBookNum; i++)
				{
					memcpy(&pAccountNumber[i], &pBankBookList->AccountNumber[i], sizeof(OnAccountNumber));
					psiMoney[i]							=	pBankBookList->siMoney[i];
				}
 
				siLength					=	sizeof(OnResAuditAccountListOkMsg) + 
															(pAuditAccountListOkMsg->dwBankBookNum * (sizeof(OnAccountNumber) + sizeof(MONEY)));
//				printf("★★[%u][%u][%d]\n",pAuditAccountListOkMsg->dwBankBookNum,pAccountNumber->uiAccountIndex,siLength);
				break;
			}
			else
			{
				siLength					=	sizeof(OnResAuditAccountListMsg);
			}

		}
		break;
	case ON_RESPONSE_AUDIT_OWN:
		{
			OnResAuditOwnListMsg				*pResAuditOwnListMsg;
			OnResAuditOwnListOkMsg				*pResAuditOwnListOkMsg;
			DBIKAuditOwnList					*pAuditOwnList;

			pResAuditOwnListMsg				=	(OnResAuditOwnListMsg*) cLocalBuffer;
			pResAuditOwnListMsg->dwMsgType	=	ON_RESPONSE_AUDIT_OWN;
			pResAuditOwnListMsg->dwResponse	=	DWORD(pParam1);
			switch(pResAuditOwnListMsg->dwResponse)
			{
			case ON_RET_AUDIT_OK:
				pResAuditOwnListOkMsg		=	(OnResAuditOwnListOkMsg*)	pResAuditOwnListMsg;
				pAuditOwnList				=	(DBIKAuditOwnList*)	pParam2;
				memcpy(&pResAuditOwnListOkMsg->dwOwnNum, &pAuditOwnList->OwnNum, sizeof(DBIKAuditOwnList));
				siLength	=	sizeof(OnResAuditOwnListOkMsg) + (pResAuditOwnListOkMsg->dwOwnNum * sizeof(OnAuditOwnInfo));
				break;
			case ON_RET_AUDIT_NO:
				siLength	=	sizeof(OnResAuditOwnListMsg);
				break;
			default:
				break;
			}
		}
		break;
	case ON_RESPONSE_AUDIT_SETNAME:
		{
			OnResSetNameMsg						*pSetNameMsg;
			pSetNameMsg						=	(OnResSetNameMsg*)	cLocalBuffer;
			pSetNameMsg->dwMsgType			=	ON_RESPONSE_AUDIT_SETNAME;
			pSetNameMsg->dwResponse			=	DWORD(pParam1);

			siLength						=	sizeof(OnResSetNameMsg);
		}
		break;
	case ON_RESPONSE_AUDIT_VILLAGE_LIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 투자된 마을의 수
			// pParam3 : 성공시 투자된 마을의 정보들
			OnResAuditVillageList				*pAuditVillageList;
			OnResAuditVillageListOK				*pAuditVillageListOK;
			BYTE								*pTempBuffer;

			pAuditVillageList				=	(OnResAuditVillageList*)cLocalBuffer;
			pAuditVillageList->dwMsgType	=	ON_RESPONSE_AUDIT_VILLAGE_LIST;
			pAuditVillageList->dwResponse	=	DWORD(pParam1);
			if( pAuditVillageList->dwResponse	== ON_RET_AUDIT_OK )
			{
				pAuditVillageListOK					=	(OnResAuditVillageListOK*)pAuditVillageList;
				pAuditVillageListOK->siVillageCount	=	DWORD(pParam2);

				pTempBuffer	= (BYTE*)( (BYTE*)pAuditVillageListOK + sizeof(OnResAuditVillageListOK) );
				CopyMemory( pTempBuffer, (BYTE*)pParam3, sizeof(OnAuditVillageInfo) * pAuditVillageListOK->siVillageCount );
				siLength	=	sizeof(OnResAuditVillageListOK) + ( sizeof(OnAuditVillageInfo) * pAuditVillageListOK->siVillageCount );
			}
			else
			{
				siLength	=	sizeof(pAuditVillageList);
			}		
		}
		break;
	case ON_RESPONSE_AUDIT_VILLAGE_INFO:
		{
		}
		break;
	case ON_RESPONSE_AUDIT_STATUS:
		{
		}
		break;
	case ON_RESPONSE_AUDIT_FRIEND_LIST:
		{
			// 친구 리스트를 보내준다.
			// pParam1 : 성공여부
			// pParam2 : 등록된 친구의 수
			// pParam3 : 등록된 친구(들)의 정보
			OnResAuditFriendList				*pAuditFriendList;
			BYTE								*pTempBuffer;

			pAuditFriendList				=	(OnResAuditFriendList*)cLocalBuffer;
			pAuditFriendList->dwMsgType		=	ON_RESPONSE_AUDIT_FRIEND_LIST;
			pAuditFriendList->dwResponse	=	DWORD(pParam1);
			pAuditFriendList->siFriendCount	=	DWORD(pParam2);

			if( pAuditFriendList->dwResponse	== ON_RET_AUDIT_OK )
			{
				// 친구가 한명 이상 있는 것이므로 뒤에 친구 정보를 붙여서 보내준다.
				pTempBuffer	= (BYTE*)( (BYTE*)pAuditFriendList + sizeof(OnResAuditFriendList) );
				CopyMemory( pTempBuffer, (BYTE*)pParam3, sizeof(OnAuditFriendInfo) * pAuditFriendList->siFriendCount );
				siLength	=	sizeof(OnResAuditFriendList) + ( sizeof(OnAuditFriendInfo) * pAuditFriendList->siFriendCount );
			}
			else
			{
				siLength	=	sizeof(OnResAuditFriendList);
			}		
		}
		break;
	case ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL:
		{
			// 친구의 (수신거부/해제) 상태를 설정한다.
			OnResAuditFriendSetDenial			*pstResMsg;

			pstResMsg					=	(OnResAuditFriendSetDenial*)cLocalBuffer;
			pstResMsg->dwMsgType		=	ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL;
			pstResMsg->dwResponse		=	DWORD(pParam1);

			siLength					=	sizeof( OnResAuditFriendSetDenial );
		}
		break;
	case ON_RESPONSE_AUDIT_FRIEND_ADD_ONE:
		{
			// 친구 한명을 추가한다.
			OnResAuditFriendAddOne			*pstResMsg;

			pstResMsg					=	(OnResAuditFriendAddOne*)cLocalBuffer;
			pstResMsg->dwMsgType		=	ON_RESPONSE_AUDIT_FRIEND_ADD_ONE;
			pstResMsg->dwResponse		=	DWORD(pParam1);
			pstResMsg->bLogin			=	BOOL(pParam2);

			siLength					=	sizeof( OnResAuditFriendAddOne );
		}
		break;
	case ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME:
		{
			// 친구 한명을 삭제한다.
			OnResAuditFriendRemoveSome		*pstResMsg;

			pstResMsg					=	(OnResAuditFriendRemoveSome*)cLocalBuffer;
			pstResMsg->dwMsgType		=	ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME;
			pstResMsg->dwResponse		=	DWORD(pParam1);

			siLength					=	sizeof( OnResAuditFriendRemoveSome );
		}
		break;
	case ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL:
		{
			// 친구 전체를 삭제한다.
			OnResAuditFriendRemoveAll		*pstResMsg;

			pstResMsg					=	(OnResAuditFriendRemoveAll*)cLocalBuffer;
			pstResMsg->dwMsgType		=	ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL;
			pstResMsg->dwResponse		=	DWORD(pParam1);

			siLength					=	sizeof( OnResAuditFriendRemoveAll );
		}
		break;
	case ON_RESPONSE_BANK_MAXCREDITLOAN:
		{
			OnResBankMaxCreditLoanMsg		*pMaxCreditLoanMsg;
			OnResBankMaxCreditLoanOkMsg		*pMaxCreditLoanOkMsg;

			pMaxCreditLoanMsg				=	(OnResBankMaxCreditLoanMsg*)cLocalBuffer;
			pMaxCreditLoanMsg->dwMSgType	=	ON_RESPONSE_BANK_MAXCREDITLOAN;
			pMaxCreditLoanMsg->dwResponse	=	DWORD(pParam1);
			if( pMaxCreditLoanMsg->dwResponse	== ON_RET_BANK_OK)
			{
				pMaxCreditLoanOkMsg				=	(OnResBankMaxCreditLoanOkMsg*) pMaxCreditLoanMsg;

				pMaxCreditLoanOkMsg->siProperty		=	stPropertyData.siTotalProperty;
				//pMaxCreditLoanOkMsg->uiCreditRank	=	DWORD(pParam3);
				pMaxCreditLoanOkMsg->uiMaxMoney		=	DWORD(pParam2);

				siLength			=	sizeof(OnResBankMaxCreditLoanOkMsg);
			}
			else
				siLength			=	sizeof(OnResBankMaxCreditLoanMsg);
		}
		break;
	case ON_RESPONSE_BANK_OPENSAVINGACCOUNT:
		{
			OnResBankOpenSavingAccountMsg	*pOpenSavingAccountMsg;
			OnResBankOpenSavingAccountOkMsg	*pOpenSavingAccountOkMsg;
			OnAccountNumber					*pNewAccountNumber;

			pOpenSavingAccountMsg					=	(OnResBankOpenSavingAccountMsg*)cLocalBuffer;
			pOpenSavingAccountMsg->dwMsgType		=	ON_RESPONSE_BANK_OPENSAVINGACCOUNT;
			pOpenSavingAccountMsg->dwResponse		=	DWORD(pParam1);			
			
			switch(pOpenSavingAccountMsg->dwResponse)
			{
			case ON_RET_BANK_OK:
				pNewAccountNumber					=	(OnAccountNumber*)pParam2;

				pOpenSavingAccountOkMsg				=	(OnResBankOpenSavingAccountOkMsg*)pOpenSavingAccountMsg;

				// 사용자의 계정과 돈을 보내준다.
				memcpy(&pOpenSavingAccountOkMsg->AccountNumber, pNewAccountNumber, sizeof(OnAccountNumber));
				pOpenSavingAccountOkMsg->siMoney	=	siMoney;
/*				printf("잔고: [%I64d], 계좌 번호 : [%03d - %04d - %07d - %d - %d ] \n", 
										pOpenSavingAccountOkMsg->siMoney,
										pOpenSavingAccountOkMsg->AccountNumber.uiVillageCode,
										pOpenSavingAccountOkMsg->AccountNumber.uiAccountKind, 
										pOpenSavingAccountOkMsg->AccountNumber.uiUserDBAccount,
										pOpenSavingAccountOkMsg->AccountNumber.uiCharSlot,
										pOpenSavingAccountOkMsg->AccountNumber.uiAccountIndex);*/
					
				
				siLength							=	sizeof(OnResBankOpenSavingAccountOkMsg);
				break;
			default:
				siLength							=	sizeof(OnResBankOpenSavingAccountMsg);		
				printf("open account failed [%u][%u][%d]\n",pOpenSavingAccountMsg->dwMsgType,pOpenSavingAccountMsg->dwResponse,siLength);
				break;
			}			
		}
		break;
	case ON_RESPONSE_BANK_BANKBOOK_LIST:
		{
			OnResBankBankBookListMsg		*pBankBankBookListMsg;
			OnResBankBankBookListOkMsg		*pBankBankBookListOkMsg;
			DBIKBankBookList				*pDBIKBankBookList;
			OnAccountNumber					*pAccountNumber;
			MONEY							*psiMoney;

			pBankBankBookListMsg					=	(OnResBankBankBookListMsg*)cLocalBuffer;
			pBankBankBookListMsg->dwMsgType			=	ON_RESPONSE_BANK_BANKBOOK_LIST;
			pBankBankBookListMsg->dwResponse		=	DWORD(pParam1);
			pDBIKBankBookList						=	(DBIKBankBookList*)pParam2;

			switch(pBankBankBookListMsg->dwResponse)
			{
			case ON_RET_BANK_OK:
				pBankBankBookListOkMsg					=	(OnResBankBankBookListOkMsg*)pBankBankBookListMsg;								
				
				pBankBankBookListOkMsg->dwBankBookNum	=	pDBIKBankBookList->siBankBookNum;

				pAccountNumber							=	(OnAccountNumber*)((CHAR*)pBankBankBookListOkMsg + sizeof(OnResBankBankBookListOkMsg));
				psiMoney								=	(MONEY*)((CHAR*)pAccountNumber + sizeof(OnAccountNumber) * pBankBankBookListOkMsg->dwBankBookNum);

				for(i = 0; i < pBankBankBookListOkMsg->dwBankBookNum; i++)
				{
					memcpy(&pAccountNumber[i], &pDBIKBankBookList->AccountNumber[i], sizeof(OnAccountNumber));
					psiMoney[i]							=	pDBIKBankBookList->siMoney[i];
				}

 
				siLength								=	sizeof(OnResBankBankBookListOkMsg) + 
															(pBankBankBookListOkMsg->dwBankBookNum * (sizeof(OnAccountNumber) + sizeof(MONEY)));
				break;
			case ON_RET_BANK_NO:
				siLength								=	sizeof(OnResBankBankBookListMsg);
				break;
			}			
		}
		break;
	case ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT:
		{
			OnResBankBankBookListCountMsg*	pOnResBankBankBookListCountMsg;

			pOnResBankBankBookListCountMsg					=	(OnResBankBankBookListCountMsg*)cLocalBuffer;
			pOnResBankBankBookListCountMsg->dwMsgType		=	ON_RESPONSE_BANK_BANKBOOK_LISTCOUNT;
			pOnResBankBankBookListCountMsg->dwResponse		=	DWORD(pParam1);
			pOnResBankBankBookListCountMsg->BankBookCount	=	*( ( MONEY* )pParam2 );

			siLength								=	sizeof(OnResBankBankBookListCountMsg);
		}
		break;
	case ON_RESPONSE_BANK_DEPOSIT:
		{
			OnResBankDepositMsg				*pBankDepositMsg;
			OnResBankDepositOkMsg			*pBankDepositOkMsg;

			pBankDepositMsg							=	(OnResBankDepositMsg*)cLocalBuffer;
			pBankDepositMsg->dwMsgType				=	ON_RESPONSE_BANK_DEPOSIT;
			pBankDepositMsg->dwResponse				=	DWORD(pParam1);					// 응답

			switch(pBankDepositMsg->dwResponse)
			{
			case ON_RET_BANK_OK:
				pBankDepositOkMsg					=	(OnResBankDepositOkMsg*)pBankDepositMsg;
				memcpy(&pBankDepositOkMsg->siBalance,pParam2,sizeof(MONEY));				// 은행 잔고
				memcpy(&pBankDepositOkMsg->siMoney,pParam3,sizeof(MONEY));					// 내 잔고

				siLength							=	sizeof(OnResBankDepositOkMsg);
				break;
			case ON_RET_BANK_NO:
				siLength							=	sizeof(OnResBankDepositMsg);
				break;
			}						
		}
		break;
	case ON_RESPONSE_BANK_DRAWING:
		{
			OnResBankDrawingMsg				*pBankDrawingMsg;
			OnResBankDrawingOkMsg			*pBankDrawingOkMsg;

			pBankDrawingMsg							=	(OnResBankDrawingMsg*)cLocalBuffer;
			pBankDrawingMsg->dwMsgType				=	ON_RESPONSE_BANK_DRAWING;
			pBankDrawingMsg->dwResponse				=	DWORD(pParam1);					// 응답

			switch(pBankDrawingMsg->dwResponse)
			{
			case ON_RET_BANK_OK:
				pBankDrawingOkMsg					=	(OnResBankDrawingOkMsg*)pBankDrawingMsg;
				memcpy(&pBankDrawingOkMsg->siBalance,pParam2,sizeof(MONEY));				// 은행 잔고
				memcpy(&pBankDrawingOkMsg->siMoney,pParam3,sizeof(MONEY));					// 내 잔고

				siLength							=	sizeof(OnResBankDrawingOkMsg);
				break;
			case ON_RET_BANK_NO:
				siLength							=	sizeof(OnResBankDrawingMsg);
				break;
			}						
		}
		break;
	case ON_RESPONSE_BANK_COLLATERAL:
		{
			OnResCollateralLoanOkMsg		*pCollateralLoanOkMsg;
			OnResCollateralLoanMsg			*pCollateralLoanMsg;			
			OnAccountNumber					*pNewAccountNumber;

			pCollateralLoanMsg					=	(OnResCollateralLoanMsg*)cLocalBuffer;
			pCollateralLoanMsg->dwMsgType 		=	ON_RESPONSE_BANK_COLLATERAL;
			pCollateralLoanMsg->dwResponse		=	DWORD(pParam1);

			switch(pCollateralLoanMsg->dwResponse)
			{
			case ON_RET_BANK_OK:
				pCollateralLoanOkMsg				=	(OnResCollateralLoanOkMsg*)cLocalBuffer;
				pNewAccountNumber					=	(OnAccountNumber*)pParam2;
				memcpy(&pCollateralLoanOkMsg->AccountNumber, pNewAccountNumber, sizeof(OnAccountNumber));
				pCollateralLoanOkMsg->siMoney		=	siMoney;

/*				printf("잔고: [%u], 계좌 번호 : [%03d - %04d - %07d - %d - %d ] \n", 
										pCollateralLoanOkMsg->siMoney,
										pCollateralLoanOkMsg->AccountNumber.uiVillageCode,
										pCollateralLoanOkMsg->AccountNumber.uiAccountKind, 
										pCollateralLoanOkMsg->AccountNumber.uiUserDBAccount,
										pCollateralLoanOkMsg->AccountNumber.uiCharSlot,
										pCollateralLoanOkMsg->AccountNumber.uiAccountIndex);*/

				siLength							=	sizeof(OnResCollateralLoanOkMsg);
				break;

			default:
				siLength							=	sizeof(OnResCollateralLoanMsg);
				break;
			}

		}
		break;
	case ON_RESPONSE_BANK_COLLATERALLIST:
		{
			OnResCollateralListOkMsg			*pCollateralListOKMsg;
			OnResCollateralListMsg				*pCollateralListMsg;
			DBIKCollateralList					*pCollateralList;
			UI16								*pItemCode;
			UI08								*pNum;
			
			pCollateralListMsg					=	(OnResCollateralListMsg*)cLocalBuffer;
			pCollateralListMsg->dwMsgType		=	ON_RESPONSE_BANK_COLLATERALLIST;
			pCollateralListMsg->dwResponse		=	DWORD(pParam1);
			pCollateralList						=	(DBIKCollateralList*)pParam2;

			if(pCollateralListMsg->dwResponse == ON_RET_BANK_OK)
			{
				pCollateralListOKMsg				=	(OnResCollateralListOkMsg*)pCollateralListMsg;
				pCollateralListOKMsg->dwItemNum		=	pCollateralList->siCollateralNum;
				pItemCode							=	(UI16*) ((CHAR*)pCollateralListMsg + sizeof(OnResCollateralListOkMsg));
				pNum								=	(UI08*)	((CHAR*)pItemCode + (sizeof(UI16)*pCollateralList->siCollateralNum));

				for(i=0; i < (SI32)pCollateralListOKMsg->dwItemNum ; i++)
				{
					pItemCode[i]		=	pCollateralList->uiITemCode[i];
					pNum[i]				=	pCollateralList->uiNum[i];
				}
				siLength				= sizeof(OnResCollateralListOkMsg)+(pCollateralListOKMsg->dwItemNum * (sizeof(UI16)+sizeof(UI08)));
			}
			else	//ON_RET_BANK_OK가 아닐 경우.
			{
				siLength				= sizeof(OnResCollateralListMsg);
			}
		}
		break;

	case ON_RESPONSE_BANK_TAKECOLLATERAL:
		{
			OnResTakeCollateralMsg				*pTakeCollateralMsg;

			pTakeCollateralMsg					=	(OnResTakeCollateralMsg*)cLocalBuffer;
			pTakeCollateralMsg->dwMsgType		=	ON_RESPONSE_BANK_TAKECOLLATERAL;
			pTakeCollateralMsg->dwResponse		=	DWORD(pParam1);					// 응답

			siLength							=	sizeof(OnResTakeCollateralMsg);
		}
		break;

	case ON_RESPONSE_BANK_SAVE:
		{
			siLength						=	sizeof(OnResBankSaveMsg);

			memcpy(cLocalBuffer,pParam1,sizeof(OnResBankSaveMsg));


		}
		break;
	case ON_RESPONSE_BANK_WITHDRAW:
		{
			siLength						=	sizeof(OnResBankWithdrawMsg);

			memcpy(cLocalBuffer,pParam1,sizeof(OnResBankWithdrawMsg));

		}
		break;


		
	case ON_RESPONSE_ENTER_INN:
		{
			OnResEnterInnMsg					*pEnterInnMsg;

			pEnterInnMsg							=	(OnResEnterInnMsg*)cLocalBuffer;
			pEnterInnMsg->dwMsgType					=	ON_RESPONSE_ENTER_INN;

			siLength								=	sizeof(OnResEnterInnMsg);			
		}		
		break;

	case ON_RESPONSE_ENTER_VWR:
		{			
			OnResEnterWaitRoomMsg			*pEnterWaitRoomMsg;
			OnResEnterWaitRoomOkMsg			*pEnterWaitRoomOkMsg;
			OnInnChatRoomInfo				*pInnChatRoomInfo;
			CHAR							*pszChatRoomName;
			UI32							i, uiTotalChatRoomNameStringLength;						
			UI32							uiChatRoomIndex;
			stChatRoom						*pChatRoom;
			
			pEnterWaitRoomMsg					=	(OnResEnterWaitRoomMsg*)cLocalBuffer;
			pEnterWaitRoomMsg->dwMsgType		=	ON_RESPONSE_ENTER_VWR;
			pEnterWaitRoomMsg->dwResponse		=	DWORD(pParam1);
			pChatRoom							=	(stChatRoom*)pParam2;
			
			switch(pEnterWaitRoomMsg->dwResponse)
			{
			case ON_RET_INN_OK:
				// 성공적으로 들어갈 수 있다.
				pEnterWaitRoomOkMsg					=	(OnResEnterWaitRoomOkMsg*)pEnterWaitRoomMsg;			
				pInnChatRoomInfo					=	(OnInnChatRoomInfo*)((CHAR*)pEnterWaitRoomMsg + sizeof(OnResEnterWaitRoomOkMsg));
				
				// 기본적인 방의 정보를 준비해준다.
				pEnterWaitRoomOkMsg->uiTotalChatRoomNum	=	0;
				for(uiChatRoomIndex = 0; uiChatRoomIndex < ON_MAX_CHATROOM_NUM; uiChatRoomIndex++)
				{
					if(pChatRoom[uiChatRoomIndex].uiJoinedPlayerNum >= 1)
					{						
						// 한 사람 이상이라두 있어야 한다.
						
						pInnChatRoomInfo[pEnterWaitRoomOkMsg->uiTotalChatRoomNum].uiChatRoomID					=	CHATROOMINDEXTOID(uiChatRoomIndex);
						pInnChatRoomInfo[pEnterWaitRoomOkMsg->uiTotalChatRoomNum].uiJoinedPlayerNum				=	pChatRoom[uiChatRoomIndex].uiJoinedPlayerNum;
						pInnChatRoomInfo[pEnterWaitRoomOkMsg->uiTotalChatRoomNum].uiChatRoomNameStringLength	=	strlen(pChatRoom[uiChatRoomIndex].szChatRoomName);
						
						pEnterWaitRoomOkMsg->uiTotalChatRoomNum++;
					}
				}
				
//				printf("방의 대한 정보 : [%d], [%d]\n", pEnterWaitRoomOkMsg->uiTotalChatRoomNum, pChatRoom[0].uiJoinedPlayerNum);
				
				pszChatRoomName		=	(CHAR*)pInnChatRoomInfo + (sizeof(OnInnChatRoomInfo) * pEnterWaitRoomOkMsg->uiTotalChatRoomNum);
				
				// 대화방의 이름을 보내준다.
				uiTotalChatRoomNameStringLength		=	0;
				for(i = 0; i < pEnterWaitRoomOkMsg->uiTotalChatRoomNum; i++)
				{
					strncpy(pszChatRoomName, pChatRoom[CHATROOMIDTOINDEX(pInnChatRoomInfo[i].uiChatRoomID)].szChatRoomName, pInnChatRoomInfo[i].uiChatRoomNameStringLength);
					pszChatRoomName					+=	pInnChatRoomInfo[i].uiChatRoomNameStringLength;
					uiTotalChatRoomNameStringLength	+=	pInnChatRoomInfo[i].uiChatRoomNameStringLength;
				}
				
				// 보내는 사이즈를 결정한다.
				siLength							=	sizeof(OnResEnterWaitRoomOkMsg) + (sizeof(OnInnChatRoomInfo) * pEnterWaitRoomOkMsg->uiTotalChatRoomNum) + 
														uiTotalChatRoomNameStringLength;
				
				break;
			case ON_RET_INN_NO:
				// 들어가는데 실패했다.
				siLength							=	sizeof(OnResEnterWaitRoomMsg);
				break;
			}					
		}
		break;		
	case ON_RESPONSE_LEAVE_INN:
		{
			ResLeaveInnMsg						*pLeaveInnMsg;

			pLeaveInnMsg								=	(ResLeaveInnMsg*)cLocalBuffer;
			pLeaveInnMsg->dwMsgType						=	ON_RESPONSE_LEAVE_INN;
			pLeaveInnMsg->dwResponse					=	DWORD(pParam1);

			siLength									=	sizeof(ResLeaveInnMsg);			
		}
		break;
	case ON_WAITROOM_CHANGE_INFO:
		{
			OnReqWaitRoomChangeInfoMsg		*pWaitRoomChangeInfoMsg;			
			CHAR							*pTempMsg;
			UI16							*puiChatRoomID;


			pWaitRoomChangeInfoMsg						=	(OnReqWaitRoomChangeInfoMsg*)cLocalBuffer;
			pWaitRoomChangeInfoMsg->dwMsgType			=	ON_WAITROOM_CHANGE_INFO;
			pWaitRoomChangeInfoMsg->dwChangeInfoKind	=	DWORD(pParam3);
			pTempMsg									=	(CHAR*)pWaitRoomChangeInfoMsg + sizeof(OnReqWaitRoomChangeInfoMsg);

//			printf("1\n");
			switch(pWaitRoomChangeInfoMsg->dwChangeInfoKind)
			{
			case ON_VWR_CREATEROOM:
//				printf("1-1\n");
				puiChatRoomID							=	(UI16*)pTempMsg;
				*puiChatRoomID							=	(UI16)(pParam4);											// 방의 ID를 설정한다.

				// 방을 생성했다고 알려준다.								
				pTempMsg[2]								=	CHAR(strlen((CHAR*)pParam5));							// 방의 길이.
				strncpy(&pTempMsg[3], (CHAR*)pParam5, pTempMsg[2]);
						
//				printf("ON_VWR_CREATEROOM, [%d], [%s]", pTempMsg[3], (CHAR*)&pTempMsg[3]);
				siLength								=	sizeof(OnReqWaitRoomChangeInfoMsg) + 3 + pTempMsg[2];
				break;
			case ON_VWR_DELETEROOM:
//				printf("1-2\n");
				// 방을 삭제했다고 알려준다.
				puiChatRoomID							=	(UI16*)pTempMsg;
				*puiChatRoomID							=	(UI16)(pParam4);											// 방의 ID를 설정한다.				
				siLength								=	sizeof(OnReqWaitRoomChangeInfoMsg) + 2;
				break;
			case ON_VWR_CHANGEPLAYERNUM:
//				printf("1-3\n");
				// 방의 사용자 수가 수정되었다고 알려준다.
				puiChatRoomID							=	(UI16*)pTempMsg;
				*puiChatRoomID							=	(UI16)(pParam4);											// 방의 ID를 설정한다.
				pTempMsg[2]								=	CHAR(pParam5);											// 플레이어의 수를 설정한다.

				siLength								=	sizeof(OnReqWaitRoomChangeInfoMsg) + 3;
				break;
			}

//			printf("2\n");
			// 보내는 사람을 설정한다.
			siSendCharNum								=	(SI32)(pParam1);
			memcpy(uiCharUniIDs, pParam2, sizeof(UI16) * siSendCharNum);		

//			printf("3\n");			
		}
		break;
	case ON_RESPONSE_ENTER_NOTICEBOARD:
		{
			OnResEnterNoticeBoardMsg		*pEnterNoticeBoardMsg;
			OnResEnterNoticeBoardOkMsg		*pEnterNoticeBoardOkMsg;			

			pEnterNoticeBoardMsg						=	(OnResEnterNoticeBoardMsg*)cLocalBuffer;
			pEnterNoticeBoardMsg->dwMsgType				=	ON_RESPONSE_ENTER_NOTICEBOARD;
			pEnterNoticeBoardMsg->dwResponse			=	DWORD(pParam1);

			switch(pEnterNoticeBoardMsg->dwResponse)			
			{
			case ON_RET_INN_OK:
				pEnterNoticeBoardOkMsg					=	(OnResEnterNoticeBoardOkMsg*)pEnterNoticeBoardMsg;
				pEnterNoticeBoardOkMsg->dwTotalPageNum	=	DWORD(pParam2);				
				
				siLength								=	sizeof(OnResEnterNoticeBoardOkMsg);
				break;
			case ON_RET_INN_NO:
				siLength								=	sizeof(OnResEnterNoticeBoardMsg);
				break;
			}
		}
		break;			
	case ON_RESPONSE_REGIST_NOTICE:
		{
			OnResRegistNoticeMsg			*pRegistNoticeMsg;

			pRegistNoticeMsg							=	(OnResRegistNoticeMsg*)cLocalBuffer;
			pRegistNoticeMsg->dwMsgType					=	ON_RESPONSE_REGIST_NOTICE;
			pRegistNoticeMsg->uiResponse				=	UI08(pParam1);
			siLength									=	sizeof(OnResRegistNoticeMsg);
		}
		break;
	case ON_RESPONSE_NOTICEBOARD_NOTICELIST:
		{
			OnResNoticesListMsg				*pNoticesListMsg;
			OnResNoticesListOkMsg			*pNoticesListOkMsg;
			OnInnNBNoticeInfo				*pInnNBNoticeInfo;
			stNoticeHeader					*pstNoticeHeader;
			UI32							uiAddMsgLength;
			CHAR							*pszID, *pszTitle;

			pNoticesListMsg								=	(OnResNoticesListMsg*)cLocalBuffer;
			pNoticesListMsg->dwMsgType					=	ON_RESPONSE_NOTICEBOARD_NOTICELIST;
			pNoticesListMsg->dwResponse					=	DWORD(pParam1);
			pNoticesListMsg->dwTotalPageNum				=	DWORD(pParam2);			

//			printf("Total Page Num : [%d]\n", pNoticesListMsg->dwTotalPageNum);

			switch(pNoticesListMsg->dwResponse)
			{
			case ON_RET_INN_OK:
				pNoticesListOkMsg						=	(OnResNoticesListOkMsg*)pNoticesListMsg;
				pNoticesListOkMsg->uiNoticesNum			=	UI08(pParam3);
				pstNoticeHeader							=	(stNoticeHeader*)pParam4;

				pInnNBNoticeInfo						=	(OnInnNBNoticeInfo*)((CHAR*)pNoticesListOkMsg + sizeof(OnResNoticesListOkMsg));
		
				uiAddMsgLength	=	0;
				for(i = 0; i < pNoticesListOkMsg->uiNoticesNum; i++)
				{		
					pInnNBNoticeInfo->uiIndex					=	pstNoticeHeader[i].uiIndex;
					pInnNBNoticeInfo->uiWroteIDStringLength		=	strlen(pstNoticeHeader[i].szID);
					pInnNBNoticeInfo->uiTitleStringLength		=	strlen(pstNoticeHeader[i].szTitle);
					pInnNBNoticeInfo->dwReadCount				=	pstNoticeHeader[i].dwReadCount;


//					printf("[%d] : [%d] : [%d],  [%s] : [%s]\n", pNoticesListOkMsg->uiNoticesNum, pInnNBNoticeInfo->uiWroteIDStringLength, pInnNBNoticeInfo->uiTitleStringLength, 
//														pstNoticeHeader[i].szID, pstNoticeHeader[i].szTitle);

					uiAddMsgLength								+=	sizeof(OnInnNBNoticeInfo);
//					printf("Add Msg Lenght : [%d]\n", uiAddMsgLength);
					uiAddMsgLength								+=	pInnNBNoticeInfo->uiWroteIDStringLength;
//					printf("Add Msg Lenght : [%d]\n", uiAddMsgLength);
					uiAddMsgLength								+=	pInnNBNoticeInfo->uiTitleStringLength;
//					printf("Add Msg Lenght : [%d]\n", uiAddMsgLength);

					pszID										=	(CHAR*)pInnNBNoticeInfo + sizeof(OnInnNBNoticeInfo);
					pszTitle									=	(CHAR*)pszID + pInnNBNoticeInfo->uiWroteIDStringLength;

					strncpy(pszID, pstNoticeHeader[i].szID, pInnNBNoticeInfo->uiWroteIDStringLength);
					strncpy(pszTitle, pstNoticeHeader[i].szTitle, pInnNBNoticeInfo->uiTitleStringLength);
																	
					
					// 날짜와 시간을 복사한다.
					strncpy(pInnNBNoticeInfo->szDate, pstNoticeHeader[i].szDate, 6);					

					// Count를 저장한다.
					pInnNBNoticeInfo->dwReadCount				=	pstNoticeHeader[i].dwReadCount;
					

					pInnNBNoticeInfo							=	(OnInnNBNoticeInfo*)((CHAR*)pInnNBNoticeInfo + sizeof(OnInnNBNoticeInfo) + 
																	pInnNBNoticeInfo->uiWroteIDStringLength + pInnNBNoticeInfo->uiTitleStringLength);

//					printf("Add Msg Lenght : [%d]\n", uiAddMsgLength);
				}

				siLength								=	sizeof(OnResNoticesListOkMsg) + uiAddMsgLength;
				
//				printf("siLength : [%d] [%d]\n", siLength, uiAddMsgLength);
				break;			
			case ON_RET_INN_NO:
				siLength								=	sizeof(OnResNoticesListMsg);
				break;
			}
		}
		break;
	case ON_RESPONSE_VIEW_NOTICE:
		{
			OnResViewNoticeMsg			*pViewNoticeMsg;
			OnResViewNoticeOkMsg		*pViewNoticeOkMsg;
			CHAR						*pszNoticeContent;

			pViewNoticeMsg					=		(OnResViewNoticeMsg*)cLocalBuffer;
			pViewNoticeMsg->dwMsgType		=		ON_RESPONSE_VIEW_NOTICE;
			pViewNoticeMsg->dwResponse		=		DWORD(pParam1);
		
			switch(pViewNoticeMsg->dwResponse)
			{
			case ON_RET_INN_OK:
				pViewNoticeOkMsg			=		(OnResViewNoticeOkMsg*)pViewNoticeMsg;

				pViewNoticeOkMsg->uiLength	=		(UI16)(pParam2);
				
				pszNoticeContent			=		(CHAR*)((CHAR*)pViewNoticeOkMsg + sizeof(OnResViewNoticeOkMsg));
				strncpy(pszNoticeContent, (CHAR*)pParam3, pViewNoticeOkMsg->uiLength);
 
				siLength					=		sizeof(OnResViewNoticeOkMsg) + pViewNoticeOkMsg->uiLength;
				break;
			case ON_RET_INN_NO:				
				siLength					=		sizeof(OnResViewNoticeMsg);
				break;
			}
		}
		break;
	case ON_RESPONSE_WEARITEM_INFO:
		{
			OnResWearItemInfoMsg			*pWearItemInfoMsg;
			MyWeapons						*pWeapons;
			pWearItemInfoMsg			=	(OnResWearItemInfoMsg*)	cLocalBuffer;
			pWeapons					=	(MyWeapons*) pParam2;

			pWearItemInfoMsg->dwMsgType	=	ON_RESPONSE_WEARITEM_INFO;
			pWearItemInfoMsg->uiArmorID	=	pWeapons->GetWearItem(ON_WEAR_ARMOR_ITEM);
			pWearItemInfoMsg->uiBeltID	=	pWeapons->GetWearItem(ON_WEAR_BELT_ITEM);
			pWearItemInfoMsg->uiGloveID	=	pWeapons->GetWearItem(ON_WEAR_GLOVE_ITEM);
			pWearItemInfoMsg->uiHelmetID=	pWeapons->GetWearItem(ON_WEAR_HELMET_ITEM);
			pWearItemInfoMsg->uiLRingID	=	pWeapons->GetWearItem(ON_WEAR_LRING_ITEM);
			pWearItemInfoMsg->uiRRingID	=	pWeapons->GetWearItem(ON_WEAR_RRING_ITEM);
			pWearItemInfoMsg->uiShoesID	=	pWeapons->GetWearItem(ON_WEAR_SHOES_ITEM);
			pWearItemInfoMsg->uiWeaponID=	pWeapons->GetWearItem(ON_WEAR_WEAPON_ITEM);
			pWearItemInfoMsg->uiGuardianID= pWeapons->GetWearItem(ON_WEAR_GUARDIAN_ITEM); 
			/*ON_WEAR_HELMET_ITEM				0
			#define	ON_WEAR_WEAPON_ITEM				1
			#define	ON_WEAR_ARMOR_ITEM				2
			#define	ON_WEAR_BELT_ITEM				3
			#define	ON_WEAR_GLOVE_ITEM				4
			#define	ON_WEAR_SHOES_ITEM				5
			#define	ON_WEAR_RRING_ITEM				6
			#define	ON_WEAR_LRING_ITEM				7
			#define ON_WEAR_GUARDIAN_ITEM			8*/
			siLength		=	sizeof(OnResWearItemInfoMsg);
		}
		break;
	case ON_RESPONSE_STRIPITEM:
		{
			OnResStripItemMsg				*pStripItemMsg;
			pStripItemMsg				=	(OnResStripItemMsg*) cLocalBuffer;
			pStripItemMsg->dwMsgType	=	ON_RESPONSE_STRIPITEM;
			pStripItemMsg->dwResponse	=	DWORD(pParam1);

			siLength		=	sizeof(OnResStripItemMsg);
		}
		break;
	case ON_RESPONSE_WEARITEM:
		{
			OnResWearItemMsg				*pWearItemMsg;

			pWearItemMsg				=	(OnResWearItemMsg*)	cLocalBuffer;
			pWearItemMsg->dwMsgType		=	ON_RESPONSE_WEARITEM;
			pWearItemMsg->dwResponse	=	DWORD(pParam1);

			siLength		=	sizeof(OnResWearItemMsg);
		}
		break;
	case ON_CHANGE_CLOTHES:
		{
			SOPlayer						*pPlayer;
			OnChangeClothesMsg				*pChangeClothesMsg;
			OnlineFollower					*pFollower;

			pPlayer							=	(SOPlayer*)	pParam1;
			pFollower		=	pPlayer->IFollower.GetFollower(0);
			pChangeClothesMsg				=	(OnChangeClothesMsg*)	cLocalBuffer;
			pChangeClothesMsg->dwMSgType	=	ON_CHANGE_CLOTHES;
			pChangeClothesMsg->uiCharacID	=	pPlayer->uiAccount;
			for(int i=0; i<WEAR_ITEM_NUM; i++)
				pChangeClothesMsg->uiItemID[i]	=	pFollower->Weapon.GetWearItem(i);

			siLength		=	sizeof(OnChangeClothesMsg);
		}
		break;
	case ON_CHANGE_CHARACTERCODE:
		{
			SOPlayer						*pPlayer;			
			OnChangeCharacterCodeMsg        *pChangeCharacterCodeMsg;
			

			pPlayer							=	(SOPlayer*)	pParam1;
			
			pChangeCharacterCodeMsg				=	(OnChangeCharacterCodeMsg*)	cLocalBuffer;
   			pChangeCharacterCodeMsg->dwMSgType	=	ON_CHANGE_CHARACTERCODE;
			pChangeCharacterCodeMsg->uiCharacID		=	pPlayer->uiAccount;
			pChangeCharacterCodeMsg->uiNewCharCode = (UI16) pParam2;
			
			siLength		=	sizeof(OnChangeCharacterCodeMsg);
		}
		break;
	case ON_IMPERIUM_USE:
		{
			SOPlayer*						pPlayer;
			OnImperiumUseMsg*				pOnImperiumMsg;

			pPlayer							=	(SOPlayer*)pParam1;

			pOnImperiumMsg					=	(OnImperiumUseMsg*) cLocalBuffer;
			pOnImperiumMsg->dwMsgType		=	ON_IMPERIUM_USE;
			pOnImperiumMsg->uiUniqueID		=	(UI16)pPlayer->uiAccount;
			pOnImperiumMsg->uiKind			=	(UI16)pParam2;
			pOnImperiumMsg->siHp			=	(SI16)pParam3;
			pOnImperiumMsg->siSupply		=	(SI32)pParam4;
			pOnImperiumMsg->bCaptain		=   (BOOL)pParam5;
			pOnImperiumMsg->uiPrevKind		=	(UI16)pParam6;

			siLength         =	sizeof(OnImperiumUseMsg);
		}
		break;
	case ON_RESPONSE_FARM_ITEMPRODUCT:
		{
			OnResFarmItemProductMsg			*pFarmItemProductMsg;

			pFarmItemProductMsg				=	(OnResFarmItemProductMsg*)	cLocalBuffer;
			pFarmItemProductMsg->dwMsgType	=	ON_RESPONSE_FARM_ITEMPRODUCT;
			pFarmItemProductMsg->dwResponse	=	DWORD(pParam1);

//			printf("{ON_RESPONSE_FARM_ITEMPRODUCT}에 대해 결과값 [%d]를 보냈다.\n", pFarmItemProductMsg->dwResponse);
			siLength		=	sizeof(OnResFarmItemProductMsg);
		}
		break;		
	case ON_RESPONSE_FARM_UPGRADESLOT:
		{
			OnResFarmUpgradeSlotMsg			*pFarmItemProductMsg;
			DWORD							dwResponse;

			dwResponse						=	DWORD( pParam1 );

			pFarmItemProductMsg				=	(OnResFarmUpgradeSlotMsg*)	cLocalBuffer;
			pFarmItemProductMsg->dwMsgType	=	ON_RESPONSE_FARM_UPGRADESLOT;
			pFarmItemProductMsg->dwResponse	=	dwResponse;

			siLength						=	sizeof(OnResFarmUpgradeSlotMsg);
		}
		break;
	case ON_RESPONSE_FARM_WORKSTART:
		{	
			
			OnResFarmWorkStartMsg			*pFarmWorkStartMsg;

			pFarmWorkStartMsg				=	( OnResFarmWorkStartMsg* )cLocalBuffer;
			pFarmWorkStartMsg->dwMsgType	=	ON_RESPONSE_FARM_WORKSTART;
			pFarmWorkStartMsg->dwResponse	=	DWORD( pParam1 );

			siLength						=	sizeof(OnResFarmWorkStartMsg);

		}
		break;
	case ON_RESPONSE_FARM_WORK:
		{
			OnResFarmWorkMsg			*pFarmWorkMsg;
			DWORD						dwResponse;
			UI32						uiRemainLabor;
			MONEY						mnPay;

			dwResponse						=	DWORD( pParam1 );
			
			pFarmWorkMsg					=	(OnResFarmWorkMsg*)	cLocalBuffer;			

			pFarmWorkMsg->dwMsgType			=	ON_RESPONSE_FARM_WORK;
			pFarmWorkMsg->dwResponse		=	dwResponse;

			if( dwResponse == ON_RET_OK )
			{
				uiRemainLabor					=	UI32( pParam2 );
				mnPay							=	*( ( MONEY* ) ( pParam3 ) );			

				pFarmWorkMsg->uiSlotRemainLabor		=	uiRemainLabor;
				pFarmWorkMsg->siPay					=	mnPay;
			}
			
			siLength		=	sizeof(OnResFarmWorkMsg);
		}
		break;
	case ON_RESPONSE_FARM_WORKINFO:
		{
			OnResFarmWorkInfoMsg		*pFarmWorkInfoMsg;
			OnResFarmWorkInfoOkMsg		*pFarmWorkInfoOkMsg;			
			UI16						uiProducingItemID;
			UI32						uiAmountOfWork;
			MONEY						mnPay;
			UI32						uiProducedItemQuantity;					// 생산한 아이템의 수량.
			UI32						uiMaxProduceItemQuantity;				// 생산해야 하는 아이템의 수량.

			pFarmWorkInfoMsg				=	(OnResFarmWorkInfoMsg*) cLocalBuffer;
			pFarmWorkInfoMsg->dwMsgType		=	ON_RESPONSE_FARM_WORKINFO;
			pFarmWorkInfoMsg->dwResponse	=	DWORD(pParam1);

			uiProducingItemID				=	UI16( pParam2 );			

			switch(pFarmWorkInfoMsg->dwResponse)
			{
			case ON_RET_OK:
				pFarmWorkInfoOkMsg		=	(OnResFarmWorkInfoOkMsg*)	pFarmWorkInfoMsg;				

				if( uiProducingItemID == ON_PLANT_SLOTUPGRADE_CODE )
				{	
					// 건설중이다.

					uiAmountOfWork					=	UI32( pParam3 );
					mnPay							=	*( ( MONEY* ) ( pParam4 ) );

					pFarmWorkInfoOkMsg->uiItem			=	ON_PLANT_SLOTUPGRADE_CODE;	//	창고라면 업그레이드 작업이다.
					pFarmWorkInfoOkMsg->uiNum			=	uiAmountOfWork;
					pFarmWorkInfoOkMsg->uiFinalNum		=	1;
					pFarmWorkInfoOkMsg->siPay			=	SI16( mnPay );
				}
				else	
				{
					// 아이템을 생산중이다.

					uiProducedItemQuantity				=	UI32( pParam3 );
					uiMaxProduceItemQuantity			=	UI32( pParam4 );
					mnPay								=	*( ( MONEY* ) ( pParam5 ) );					

					pFarmWorkInfoOkMsg->uiItem			=	uiProducingItemID;
					pFarmWorkInfoOkMsg->uiNum			=	SI32( uiProducedItemQuantity );
					pFarmWorkInfoOkMsg->uiFinalNum		=	UI32( uiMaxProduceItemQuantity );
					pFarmWorkInfoOkMsg->siPay			=	SI16( mnPay );
				}

				siLength	=	sizeof(OnResFarmWorkInfoOkMsg);
				break;
			case ON_RET_NO:
				siLength	=	sizeof(OnResFarmWorkInfoMsg);
				break;
			}
		}
		break;
	case ON_RESPONSE_STORAGE_INFO:
		{
			OnResStorageInfoMsg				*pStorageInfoMsg;
			OnResStorageInfoOkMsg			*pStorageInfoOkMsg;			
			UI32							uiCount=0;
			CHAR							*Pos;

			DWORD							dwResponse;
			STORAGE_ITEM					*pStorageItem;

			dwResponse						=	DWORD( pParam1 );
			pStorageItem					=	( STORAGE_ITEM* )( pParam2 );

			pStorageInfoMsg					=	(OnResStorageInfoMsg*)	cLocalBuffer;
			pStorageInfoMsg->dwMsgType		=	ON_RESPONSE_STORAGE_INFO;
			pStorageInfoMsg->dwResponse		=	dwResponse;

			switch( pStorageInfoMsg->dwResponse )
			{
			case ON_RET_OK:

				pStorageInfoOkMsg			=	(OnResStorageInfoOkMsg*)	pStorageInfoMsg;
				Pos							=	(CHAR*)((CHAR*)pStorageInfoOkMsg + sizeof(OnResStorageInfoOkMsg));

				for( i = 0; i < ON_MAX_STORAGEITEM; i++ )
				{
					if( pStorageItem[ i ].uiQuantity	!= 0 )
					{
						memcpy( Pos, &pStorageItem[ i ],sizeof( STORAGE_ITEM ) );
						Pos	+=	sizeof( STORAGE_ITEM );
						uiCount++;
					}
				}
				
				pStorageInfoOkMsg->uiNum	=	uiCount;

				siLength	=	sizeof( OnResStorageInfoOkMsg )+ uiCount * sizeof( STORAGE_ITEM );

				break;

			case ON_RET_NO:

				siLength	=	sizeof( OnResStorageInfoMsg );

				break;
			}
		}
		break;
	case ON_RESPONSE_STORAGE_INPUTITEM:
		{
			OnResStorageInputItemMsg			*pStorageInputItemMsg;

			pStorageInputItemMsg				=	(OnResStorageInputItemMsg*)	cLocalBuffer;
			pStorageInputItemMsg->dwMsgType		=	ON_RESPONSE_STORAGE_INPUTITEM;
			pStorageInputItemMsg->dwResponse	=	DWORD(pParam1);
			siLength		=		sizeof(OnResStorageInputItemMsg);
		}
		break;
	case ON_RESPONSE_STORAGE_OUTPUTITEM:
		{
			OnResStorageOutputItemMsg			*pStorageOutputItemMsg;
			pStorageOutputItemMsg				=	(OnResStorageOutputItemMsg*)	cLocalBuffer;
			pStorageOutputItemMsg->dwMsgType	=	ON_RESPONSE_STORAGE_OUTPUTITEM;
			pStorageOutputItemMsg->dwResponse	=	DWORD(pParam1);
			siLength		=	sizeof(OnResStorageOutputItemMsg);
		}
		break;
	case ON_RESPONSE_PLANT_CANCEL_PLAN:
		{
			OnResPlantCancelPlanMsg				*pPlantCancelPlanMsg;
			DWORD								dwResponse;
			MONEY								mnPlayerMoney = 0;

			dwResponse							=	DWORD( pParam1 );						
			
			if( dwResponse == ON_RET_OK )
			{
				mnPlayerMoney			=	*( ( MONEY* ) ( pParam2 ) );			
			}			

			pPlantCancelPlanMsg					=	(OnResPlantCancelPlanMsg*)	cLocalBuffer;
			pPlantCancelPlanMsg->dwMsgType		=	ON_RESPONSE_PLANT_CANCEL_PLAN;
			pPlantCancelPlanMsg->dwResponse		=	dwResponse;
			pPlantCancelPlanMsg->ChangedMoney	=	mnPlayerMoney;

			siLength		=	sizeof(OnResPlantCancelPlanMsg);
		}
		break;
	case ON_RESPONSE_PLANT_CHANGE_PAY:
		{
			OnResPlantChangePayMsg				*pPlantChangePayMsg;
			DWORD								dwResponse;
			MONEY								mnPlayerMoney = 0;

			dwResponse		=	DWORD( pParam1 );

			if( dwResponse == ON_RET_OK )
			{
				mnPlayerMoney			=	*( ( MONEY* ) ( pParam2 ) );			
			}
			
			pPlantChangePayMsg					=	(OnResPlantChangePayMsg*)	cLocalBuffer;
			pPlantChangePayMsg->dwMsgType		=	ON_RESPONSE_PLANT_CHANGE_PAY;
			pPlantChangePayMsg->dwResponse		=	dwResponse;
			pPlantChangePayMsg->ChangedMoney	=	mnPlayerMoney;			
			
			siLength		=	sizeof( OnResPlantChangePayMsg );

		}
		break;
	case ON_RESPONSE_FARM_SETEQUIPMENT:
		{
			OnResFarmSetEquipmentMsg			*pFarmSetEquipmentMsg;

			pFarmSetEquipmentMsg				=	(OnResFarmSetEquipmentMsg*)	cLocalBuffer;
			pFarmSetEquipmentMsg->dwMsgType		=	ON_RESPONSE_FARM_SETEQUIPMENT;
			pFarmSetEquipmentMsg->dwResponse	=	DWORD(pParam1);

			siLength		=	sizeof(OnResFarmSetEquipmentMsg);
		}
		break;
	case ON_RESPONSE_FARM_DESTROYSTRUCT:
		{
			OnResFarmDestroyStructMsg		*pFarmDestroyStructMsg;

			pFarmDestroyStructMsg				=	(OnResFarmDestroyStructMsg*)	cLocalBuffer;
			pFarmDestroyStructMsg->dwMsgType	=	ON_RESPONSE_FARM_DESTROYSTRUCT;
			pFarmDestroyStructMsg->dwResponse	=	DWORD(pParam1);

			siLength		=	sizeof(OnResFarmDestroyStructMsg);
		}
		break;
	case ON_RESPONSE_FARM_EXPANDSLOT:
		{
			OnResFarmExpandSlotMsg		*pFarmExpandSlotMsg;

			pFarmExpandSlotMsg				=	(OnResFarmExpandSlotMsg*)cLocalBuffer;
			pFarmExpandSlotMsg->dwMsgType	=	ON_RESPONSE_FARM_EXPANDSLOT;
			pFarmExpandSlotMsg->dwResponse	=	DWORD(pParam1);

			siLength		=		sizeof(OnResFarmExpandSlotMsg);
		}
		break;
	case ON_RESPONSE_FARM_BUILDSTRUCT:
		{
			OnResFarmBuildStructMsg		*pFarmBuildStructMsg;

			pFarmBuildStructMsg				=	(OnResFarmBuildStructMsg*)cLocalBuffer;
			pFarmBuildStructMsg->dwMsgType	=	ON_RESPONSE_FARM_BUILDSTRUCT;
			pFarmBuildStructMsg->dwResponse	=	DWORD(pParam1);

			siLength	=	sizeof(OnResFarmBuildStructMsg);
		}
		break;
	case ON_RESPONSE_FARM_SLOTINFO:
		{			
			OnResFarmSlotInfoMsg			*pFarmSlotInfoMsg;		
	
			SI32							siFactoryNum;
			OnFarmSlotList					*pFactoryInfoIn, *pFactoryInfoOut;
			DWORD							dwInfoLength;
			SI32							siMaxConstructibleFactoryNum;

			siFactoryNum					=	SI32( pParam1 );
			pFactoryInfoIn					=	( OnFarmSlotList* )pParam2;
			dwInfoLength					=	DWORD( pParam3 );
			siMaxConstructibleFactoryNum	=	SI32( pParam4 );

			
			pFarmSlotInfoMsg				=	( OnResFarmSlotInfoMsg* ) cLocalBuffer;
			pFactoryInfoOut					=	( OnFarmSlotList* ) ( ( CHAR* )pFarmSlotInfoMsg + sizeof( OnResFarmSlotInfoMsg ) );

			pFarmSlotInfoMsg->dwMsgType						=	ON_RESPONSE_FARM_SLOTINFO;			
			pFarmSlotInfoMsg->siFactoryNum					=	SI16( siFactoryNum );
			pFarmSlotInfoMsg->siMaxConstructibleFactoryNum	=	SI16( siMaxConstructibleFactoryNum );
			
			siLength						=	sizeof( OnResFarmSlotInfoMsg );

			if( pFarmSlotInfoMsg->siFactoryNum != -1 )
			{
				memcpy( pFactoryInfoOut, pFactoryInfoIn, dwInfoLength );
				
				siLength					+=	dwInfoLength;
			}			
			
			
			/*
			OnResFarmSlotInfoMsg		*pFarmSlotInfoMsg;
			OnResFarmSlotInfoOkMsg		*pFarmSlotInfoOkMsg;
			SI32						siCount=0, i;
			OnFarmSlotList				*FarmSlotList;
			stFarmData					*pFarmData;
			stFarmSlot					*pFarmSlot;

			pFarmSlotInfoMsg			=	(OnResFarmSlotInfoMsg*)cLocalBuffer;
			pFarmSlotInfoMsg->dwMsgType	=	ON_RESPONSE_FARM_SLOTINFO;
			pFarmSlotInfoMsg->dwResponse=	DWORD(pParam1);

			switch(pFarmSlotInfoMsg->dwResponse)
			{
			case ON_RET_OK:
				pFarmSlotInfoOkMsg			=	(OnResFarmSlotInfoOkMsg*)	pFarmSlotInfoMsg;
				FarmSlotList		=	(OnFarmSlotList*) ((CHAR*)pFarmSlotInfoOkMsg+sizeof(OnResFarmSlotInfoOkMsg));
				pFarmData					=	stAccountInfo.pStructure;
				for(i=0; i<pFarmData->FarmInfo.uiAvailSlot; i++)
				{
					pFarmSlot	=	pFarmData->GetFarmSlot(i);
					if( pFarmSlot == NULL )			break;
					if( pFarmSlot->pFacility )
					{
						FarmSlotList[siCount].SlotIndex		=	i;
						FarmSlotList[siCount].siPay			=	pFarmSlot->siPay;
						FarmSlotList[siCount].uiLabor		=	pFarmSlot->uiRemainLabor;
						FarmSlotList[siCount].Level			=	pFarmSlot->uiLevel;

						strcpy(FarmSlotList[siCount].StructCode, pFarmSlot->pFacility->Code);
						if( pFarmSlot->bStore )
						{
							FarmSlotList[siCount].Up1Level	=	0;
							FarmSlotList[siCount].Up2Level	=	0;
							FarmSlotList[siCount].Up3Level	=	0;
							FarmSlotList[siCount].Up4Level	=	0;
							FarmSlotList[siCount].Up5Level	=	0;							
						}
						else
						{
							FarmSlotList[siCount].Up1Level	=	pFarmSlot->Structure.Equipment[0].uiLevel;
							FarmSlotList[siCount].Up2Level	=	pFarmSlot->Structure.Equipment[1].uiLevel;
							FarmSlotList[siCount].Up3Level	=	pFarmSlot->Structure.Equipment[2].uiLevel;
							FarmSlotList[siCount].Up4Level	=	pFarmSlot->Structure.Equipment[3].uiLevel;
							FarmSlotList[siCount].Up5Level	=	pFarmSlot->Structure.Equipment[4].uiLevel;
						}
						siCount++;
					}
				}
				pFarmSlotInfoOkMsg->dwListNum	=	siCount;
				siLength	=	sizeof(OnResFarmSlotInfoOkMsg) + (siCount*sizeof(OnFarmSlotList));
				break;
			case ON_RET_NO:
			default:
				siLength					=	sizeof(OnResFarmSlotInfoMsg);
				break;
			}
			*/
		}
		break;
	case ON_RESPONSE_FARM_BUY:
		{
			OnResFarmBuyMsg				*pFarmBuyMsg;

			pFarmBuyMsg					=	(OnResFarmBuyMsg*)cLocalBuffer;
			pFarmBuyMsg->dwMsgType		=	ON_RESPONSE_FARM_BUY;
			pFarmBuyMsg->dwResponse		=	DWORD(pParam1);
			
			siLength					=	sizeof(OnResFarmBuyMsg);
		}
		break;
	case ON_RESPONSE_FARM_INFO:
		{
			OnResFarmInfoMsg			*pFarmInfoMsg;
			OnIndustrialComplexInfo		*pIndustrialComplexInfoIn, *pIndustrialComplexInfoOut;
			UI32						uiIndustrialComplexNum;
			DWORD						dwInfoLength;
			
			uiIndustrialComplexNum		=	UI32( pParam1 );
			pIndustrialComplexInfoIn	=	( OnIndustrialComplexInfo* )pParam2;
			dwInfoLength				=	DWORD( pParam3 );

			pFarmInfoMsg				=	( OnResFarmInfoMsg* ) cLocalBuffer;			
			pIndustrialComplexInfoOut	=	( OnIndustrialComplexInfo* ) ( ( CHAR* )pFarmInfoMsg + sizeof( OnResFarmInfoMsg ) ) ;

			pFarmInfoMsg->dwMsgType					=	ON_RESPONSE_FARM_INFO;		
			pFarmInfoMsg->uiIndustrialComplexNum	=	uiIndustrialComplexNum;

			if( dwInfoLength != 0 )
			{
				memcpy( pIndustrialComplexInfoOut, pIndustrialComplexInfoIn, dwInfoLength );			
			}
			
			siLength					=	sizeof( OnResFarmInfoMsg ) + dwInfoLength;
		}
		break;
	case ON_RESPONSE_BANK_INFO:
		{
			OnResBankInfoMsg			*pBankInfoMsg;
			SI16						siInterest;
			MONEY						mnBankCapital;
			

			siInterest		=	SI16( pParam1 );
			mnBankCapital	=	*( ( MONEY* ) ( pParam2 ) );			

			pBankInfoMsg						=	(OnResBankInfoMsg*)cLocalBuffer;			
			pBankInfoMsg->dwMsgType				=	ON_RESPONSE_BANK_INFO;
			pBankInfoMsg->siInterest			=	siInterest;
			pBankInfoMsg->siBankCapital			=	mnBankCapital;

			siLength							=	sizeof(OnResBankInfoMsg);
		}
		break;
	case ON_RESPONSE_DELETE_NOTICE:
		{
			OnResDeleteNoticeMsg		*pDeleteNoticeMsg;
			OnResDeleteNoticeOkMsg		*pDeleteNoticeOkMsg;

			pDeleteNoticeMsg					=	(OnResDeleteNoticeMsg*)cLocalBuffer;
			
			pDeleteNoticeMsg->dwMsgType			=	ON_RESPONSE_DELETE_NOTICE;
			pDeleteNoticeMsg->dwResponse		=	DWORD(pParam1);

			switch(pDeleteNoticeMsg->dwResponse)
			{
			case ON_RET_INN_OK:
				pDeleteNoticeOkMsg						=	(OnResDeleteNoticeOkMsg*)pDeleteNoticeMsg;
				pDeleteNoticeOkMsg->dwTotalPageNum		=	DWORD(pParam2);

				siLength								=	sizeof(OnResDeleteNoticeOkMsg);
				break;
			case ON_RET_INN_NO:
				siLength								=	sizeof(OnResDeleteNoticeMsg);
				break;
			}
		}
		break;	
	case ON_RESPONSE_BARRACK_GETSOLDERLIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			// pParam4 : 할인가
			OnResBarrackGetListOK*		pOnResBarrackGetListOK;
			OnResBarrackGetList*		pOnResBarrackGetList;
			BYTE*						TempBuffer;

			pOnResBarrackGetList			 = (OnResBarrackGetList*)cLocalBuffer;
			
			pOnResBarrackGetList->dwMsgType	 = ON_RESPONSE_BARRACK_GETSOLDERLIST;
			pOnResBarrackGetList->dwResponse = DWORD(pParam1);

			switch(pOnResBarrackGetList->dwResponse)
			{
				case ON_RET_OK:
					pOnResBarrackGetListOK = (OnResBarrackGetListOK*)cLocalBuffer;
					TempBuffer			   = (BYTE*)((BYTE*)pOnResBarrackGetListOK + sizeof(OnResBarrackGetListOK));
					pOnResBarrackGetListOK->dwBuySolderDiscountPercent = SI32(pParam4);

					pOnResBarrackGetListOK->dwBuySolderCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnInnBuySolderInfo) * pOnResBarrackGetListOK->dwBuySolderCount);

					siLength =	sizeof(OnResBarrackGetListOK) + (sizeof(OnInnBuySolderInfo) * pOnResBarrackGetListOK->dwBuySolderCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResBarrackGetList);
					break;
			}
		}
		break;
	case ON_RESPONSE_BARRACK_BUYSOLDER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			// pParam4 : 성공시 고용한 짐꾼의 Info
			OnResBarrackBuySolder*		pOnResBarrackBuySolder;
			OnResBarrackBuySolderOK*	pOnResBarrackBuySolderOK;

			pOnResBarrackBuySolder			   = (OnResBarrackBuySolder*)cLocalBuffer;

			pOnResBarrackBuySolder->dwMsgType  = ON_RESPONSE_BARRACK_BUYSOLDER;
			pOnResBarrackBuySolder->dwResponse = DWORD(pParam1);
			switch(pOnResBarrackBuySolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResBarrackBuySolderOK        = (OnResBarrackBuySolderOK*)cLocalBuffer;
					pOnResBarrackBuySolderOK->Money = *((__int64*)pParam2);
					pOnResBarrackBuySolderOK->Slot  = (BYTE)pParam3;
					CopyMemory(&pOnResBarrackBuySolderOK->FollowerInfo, (char*)pParam4, sizeof(OnFollowerInfo));

					siLength =	sizeof(OnResBarrackBuySolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResBarrackBuySolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			OnResBarrackGetDismissalListOK*		pOnResBarrackGetDismissalListOK;
			OnResBarrackGetDismissalList*		pOnResBarrackGetDismissalList;
			BYTE*								TempBuffer;

			pOnResBarrackGetDismissalList			 = (OnResBarrackGetDismissalList*)cLocalBuffer;
			
			pOnResBarrackGetDismissalList->dwMsgType  = ON_RESPONSE_BARRACK_GETDISMISSALSOLDERLIST;
			pOnResBarrackGetDismissalList->dwResponse = DWORD(pParam1);

			switch(pOnResBarrackGetDismissalList->dwResponse)
			{
				case ON_RET_OK:
					pOnResBarrackGetDismissalListOK = (OnResBarrackGetDismissalListOK*)cLocalBuffer;
					TempBuffer			            = (BYTE*)((BYTE*)pOnResBarrackGetDismissalListOK + sizeof(OnResBarrackGetDismissalListOK));

					pOnResBarrackGetDismissalListOK->dwDismissalSolderCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnDismissalSolderInfo) * pOnResBarrackGetDismissalListOK->dwDismissalSolderCount);

					siLength =	sizeof(OnResBarrackGetDismissalListOK) + (sizeof(OnDismissalSolderInfo) * pOnResBarrackGetDismissalListOK->dwDismissalSolderCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResBarrackGetDismissalList);
					break;
			}
		}
		break;
	case ON_RESPONSE_BARRACK_DISMISSALSOLDER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			OnResBarrackDismissalSolder*	pOnResBarrackDismissalSolder;
			OnResBarrackDismissalSolderOK*	pOnResBarrackDismissalSolderOK;

			pOnResBarrackDismissalSolder			   = (OnResBarrackDismissalSolder*)cLocalBuffer;

			pOnResBarrackDismissalSolder->dwMsgType  = ON_RESPONSE_BARRACK_DISMISSALSOLDER;
			pOnResBarrackDismissalSolder->dwResponse = DWORD(pParam1);
			switch(pOnResBarrackDismissalSolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResBarrackDismissalSolderOK        = (OnResBarrackDismissalSolderOK*)cLocalBuffer;
					pOnResBarrackDismissalSolderOK->Money = *((__int64*)pParam2);
					pOnResBarrackDismissalSolderOK->Slot  = (BYTE)((SI32)pParam3);

					siLength =	sizeof(OnResBarrackDismissalSolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResBarrackDismissalSolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_GETSOLDERLIST:			//장수 고용 리스트를 받아온다.
	case ON_RESPONSE_GBARRACK_GET_GENERAL_LIST:			//2차 장수 고용 리스트를 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			// pParam4 : 할인가
			OnResGBarrackGetListOK*		pOnResGBarrackGetListOK;
			OnResGBarrackGetList*		pOnResGBarrackGetList;
			BYTE*						TempBuffer;

			pOnResGBarrackGetList			  = (OnResGBarrackGetList*)cLocalBuffer;
			
			pOnResGBarrackGetList->dwMsgType  = siMsg; //ON_RESPONSE_GBARRACK_GETSOLDERLIST or ON_RESPONSE_GBARRACK_GET_GENERAL_LIST
			pOnResGBarrackGetList->dwResponse = DWORD(pParam1);

			switch(pOnResGBarrackGetList->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackGetListOK = (OnResGBarrackGetListOK*)cLocalBuffer;
					TempBuffer			    = (BYTE*)((BYTE*)pOnResGBarrackGetListOK + sizeof(OnResGBarrackGetListOK));
					pOnResGBarrackGetListOK->dwBuySolderDiscountPercent = SI32(pParam4);

					pOnResGBarrackGetListOK->dwBuySolderCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnInnBuySolderInfo) * pOnResGBarrackGetListOK->dwBuySolderCount);

					siLength =	sizeof(OnResGBarrackGetListOK) + (sizeof(OnInnBuySolderInfo) * pOnResGBarrackGetListOK->dwBuySolderCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackGetList);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_BUYSOLDER:		//  장수 고용 응답을 받아온다.
	case ON_RESPONSE_GBARRACK_BUY_GENERAL:		//  2차 장수 고용 응답을 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			// pParam4 : 성공시 고용한 짐꾼의 Info
			OnResGBarrackBuySolder*		pOnResGBarrackBuySolder;
			OnResGBarrackBuySolderOK*	pOnResGBarrackBuySolderOK;

			pOnResGBarrackBuySolder			   = (OnResGBarrackBuySolder*)cLocalBuffer;

			pOnResGBarrackBuySolder->dwMsgType  = siMsg; //ON_RESPONSE_GBARRACK_BUYSOLDER or ON_RESPONSE_GBARRACK_BUY_GENERAL
			pOnResGBarrackBuySolder->dwResponse = DWORD(pParam1);
			switch(pOnResGBarrackBuySolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackBuySolderOK        = (OnResGBarrackBuySolderOK*)cLocalBuffer;
					pOnResGBarrackBuySolderOK->Money = *((__int64*)pParam2);
					pOnResGBarrackBuySolderOK->Slot  = (BYTE)pParam3;
					CopyMemory(&pOnResGBarrackBuySolderOK->FollowerInfo, (char*)pParam4, sizeof(OnFollowerInfo));

					siLength =	sizeof(OnResGBarrackBuySolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackBuySolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST:	//장수 전직 리스트를 받아온다.
	case ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST:	//2차 장수 전직 리스트를 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			OnResGBarrackGetChangeJobListOK*	pOnResGBarrackGetChangeJobListOK;
			OnResGBarrackGetChangeJobList*		pOnResGBarrackGetChangeJobList;
			BYTE*								TempBuffer;

			pOnResGBarrackGetChangeJobList			 = (OnResGBarrackGetChangeJobList*)cLocalBuffer;
			
			pOnResGBarrackGetChangeJobList->dwMsgType  = siMsg; //ON_RESPONSE_GBARRACK_GETCHANGEJOBSOLDERLIST or ON_RESPONSE_GBARRACK_GET_CHANGE_GENERAL_LIST
			pOnResGBarrackGetChangeJobList->dwResponse = DWORD(pParam1);

			switch(pOnResGBarrackGetChangeJobList->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackGetChangeJobListOK = (OnResGBarrackGetChangeJobListOK*)cLocalBuffer;
					TempBuffer		 	             = (BYTE*)((BYTE*)pOnResGBarrackGetChangeJobListOK + sizeof(OnResGBarrackGetChangeJobListOK));

					pOnResGBarrackGetChangeJobListOK->dwChangeJobSolderCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnChangeJobSolderInfo) * pOnResGBarrackGetChangeJobListOK->dwChangeJobSolderCount);

					siLength =	sizeof(OnResGBarrackGetChangeJobListOK) + (sizeof(OnChangeJobSolderInfo) * pOnResGBarrackGetChangeJobListOK->dwChangeJobSolderCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackGetChangeJobList);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER:		//장수 고용 응답을 받아온다.
	case ON_RESPONSE_GBARRACK_CHANGE_GENERAL:		//2차 장수 고용 응답을 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			// pParam4 : 성공시 고용한 짐꾼의 Info
			OnResGBarrackChangeJobSolder*		pOnResGBarrackChangeJobSolder;
			OnResGBarrackChangeJobSolderOK*		pOnResGBarrackChangeJobSolderOK;

			pOnResGBarrackChangeJobSolder		   = (OnResGBarrackChangeJobSolder*)cLocalBuffer;

			pOnResGBarrackChangeJobSolder->dwMsgType  = siMsg;	//ON_RESPONSE_GBARRACK_CHANGEJOBSOLDER or ON_RESPONSE_GBARRACK_CHANGE_GENERAL
			pOnResGBarrackChangeJobSolder->dwResponse = DWORD(pParam1);
			switch(pOnResGBarrackChangeJobSolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackChangeJobSolderOK        = (OnResGBarrackChangeJobSolderOK*)cLocalBuffer;
					pOnResGBarrackChangeJobSolderOK->Money = *((__int64*)pParam2);
					pOnResGBarrackChangeJobSolderOK->Slot  = (BYTE)pParam3;
					CopyMemory(&pOnResGBarrackChangeJobSolderOK->FollowerInfo, (char*)pParam4, sizeof(OnChangeJobFollowerInfo));

					siLength =	sizeof(OnResGBarrackChangeJobSolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackChangeJobSolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST:		//장수 해고 리스트를 받아온다.
	case ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST:	//2차 장수 해고 리스트를 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			OnResGBarrackGetDismissalListOK*	pOnResGBarrackGetDismissalListOK;
			OnResGBarrackGetDismissalList*		pOnResGBarrackGetDismissalList;
			BYTE*								TempBuffer;

			pOnResGBarrackGetDismissalList			 = (OnResGBarrackGetDismissalList*)cLocalBuffer;
			
			pOnResGBarrackGetDismissalList->dwMsgType  = siMsg;	//ON_RESPONSE_GBARRACK_GETDISMISSALSOLDERLIST or ON_RESPONSE_GBARRACK_GETDISMISSAL_GENERAL_LIST
			pOnResGBarrackGetDismissalList->dwResponse = DWORD(pParam1);

			switch(pOnResGBarrackGetDismissalList->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackGetDismissalListOK = (OnResGBarrackGetDismissalListOK*)cLocalBuffer;
					TempBuffer		 	             = (BYTE*)((BYTE*)pOnResGBarrackGetDismissalListOK + sizeof(OnResGBarrackGetDismissalListOK));

					pOnResGBarrackGetDismissalListOK->dwDismissalSolderCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnDismissalSolderInfo) * pOnResGBarrackGetDismissalListOK->dwDismissalSolderCount);

					siLength =	sizeof(OnResGBarrackGetDismissalListOK) + (sizeof(OnDismissalSolderInfo) * pOnResGBarrackGetDismissalListOK->dwDismissalSolderCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackGetDismissalList);
					break;
			}
		}
		break;
	case ON_RESPONSE_GBARRACK_DISMISSALSOLDER:		// 장수 해고 응답을 받아온다.
	case ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL:	// 2차 장수 해고 응답을 받아온다.
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			OnResGBarrackDismissalSolder*	pOnResGBarrackDismissalSolder;
			OnResGBarrackDismissalSolderOK*	pOnResGBarrackDismissalSolderOK;

			pOnResGBarrackDismissalSolder			   = (OnResGBarrackDismissalSolder*)cLocalBuffer;

			pOnResGBarrackDismissalSolder->dwMsgType  = siMsg; //ON_RESPONSE_GBARRACK_DISMISSALSOLDER or ON_RESPONSE_GBARRACK_DISMISSAL_GENERAL
			pOnResGBarrackDismissalSolder->dwResponse = DWORD(pParam1);
			switch(pOnResGBarrackDismissalSolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResGBarrackDismissalSolderOK        = (OnResGBarrackDismissalSolderOK*)cLocalBuffer;
					pOnResGBarrackDismissalSolderOK->Money = *((__int64*)pParam2);
					pOnResGBarrackDismissalSolderOK->Slot  = (BYTE)((SI32)pParam3);

					siLength =	sizeof(OnResGBarrackDismissalSolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResGBarrackDismissalSolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_MBARRACK_GETMONSTERLIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			// pParam4 : 할인가
			OnResMBarrackGetListOK*		pOnResMBarrackGetListOK;
			OnResMBarrackGetList*		pOnResMBarrackGetList;
			BYTE*						TempBuffer;

			pOnResMBarrackGetList			  = (OnResMBarrackGetList*)cLocalBuffer;
			
			pOnResMBarrackGetList->dwMsgType  = ON_RESPONSE_MBARRACK_GETMONSTERLIST;
			pOnResMBarrackGetList->dwResponse = DWORD(pParam1);

			switch( pOnResMBarrackGetList->dwResponse )
			{
				case ON_RET_OK:
					pOnResMBarrackGetListOK = (OnResMBarrackGetListOK*)cLocalBuffer;
					TempBuffer			    = (BYTE*)((BYTE*)pOnResMBarrackGetListOK + sizeof(OnResMBarrackGetListOK));
					pOnResMBarrackGetListOK->dwBuyMonsterDiscountPercent = SI32(pParam4);

					pOnResMBarrackGetListOK->dwBuyMonsterCount = DWORD(pParam2);
					CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(OnInnBuyMonsterInfo) * pOnResMBarrackGetListOK->dwBuyMonsterCount);

					siLength =	sizeof(OnResMBarrackGetListOK) + (sizeof(OnInnBuyMonsterInfo) * pOnResMBarrackGetListOK->dwBuyMonsterCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResMBarrackGetList);
					break;
			}
		}
		break;
	case ON_RESPONSE_MBARRACK_BUYMONSTER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			// pParam4 : 성공시 고용한 짐꾼의 Info
			OnResMBarrackBuyMonster*		pOnResMBarrackBuyMonster;
			OnResMBarrackBuyMonsterOK*		pOnResMBarrackBuyMonsterOK;

			pOnResMBarrackBuyMonster	   = (OnResMBarrackBuyMonster*)cLocalBuffer;

			pOnResMBarrackBuyMonster->dwMsgType  = ON_RESPONSE_MBARRACK_BUYMONSTER;
			pOnResMBarrackBuyMonster->dwResponse = DWORD(pParam1);
			switch(pOnResMBarrackBuyMonster->dwResponse)
			{
				case ON_RET_OK:
					pOnResMBarrackBuyMonsterOK        = (OnResMBarrackBuyMonsterOK*)cLocalBuffer;
					pOnResMBarrackBuyMonsterOK->Money = *((__int64*)pParam2);
					pOnResMBarrackBuyMonsterOK->Slot  = (BYTE)pParam3;
					CopyMemory(&pOnResMBarrackBuyMonsterOK->FollowerInfo, (char*)pParam4, sizeof(OnFollowerInfo));

					siLength =	sizeof(OnResMBarrackBuyMonsterOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResMBarrackBuyMonster);
					break;
			}
		}
		break;
	case ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 용병 종류의 수
			// pParam3 : 성공시 용병 종류의 정보들
			OnResMBarrackGetDismissListOK*		pOnResMBarrackGetDismissListOK;
			OnResMBarrackGetDismissList*		pOnResMBarrackGetDismissList;
			BYTE*								TempBuffer;

			pOnResMBarrackGetDismissList		= (OnResMBarrackGetDismissList*)cLocalBuffer;
			
			pOnResMBarrackGetDismissList->dwMsgType  = ON_RESPONSE_MBARRACK_GETDISMISSMONSTERLIST;
			pOnResMBarrackGetDismissList->dwResponse = DWORD(pParam1);

			switch( pOnResMBarrackGetDismissList->dwResponse )
			{
				case ON_RET_OK:
					pOnResMBarrackGetDismissListOK	 = (OnResMBarrackGetDismissListOK*)cLocalBuffer;
					TempBuffer		 	             = (BYTE*)((BYTE*)pOnResMBarrackGetDismissListOK + sizeof(OnResMBarrackGetDismissListOK));

					pOnResMBarrackGetDismissListOK->dwDismissMonsterCount = DWORD(pParam2);
					CopyMemory( TempBuffer, (BYTE*)pParam3, sizeof(OnDismissalSolderInfo) * pOnResMBarrackGetDismissListOK->dwDismissMonsterCount );

					siLength =	sizeof(OnResMBarrackGetDismissListOK) + (sizeof(OnDismissalSolderInfo) * pOnResMBarrackGetDismissListOK->dwDismissMonsterCount);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResMBarrackGetDismissList);
					break;
			}
		}
		break;
	case ON_RESPONSE_MBARRACK_DISMISSMONSTER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 고용한 짐꾼이 들어가는 Slot
			OnResMBarrackDismissMonster*	pOnResMBarrackDismissMonster;
			OnResMBarrackDismissMonsterOK*	pOnResMBarrackDismissMonsterOK;

			pOnResMBarrackDismissMonster	   = (OnResMBarrackDismissMonster*)cLocalBuffer;

			pOnResMBarrackDismissMonster->dwMsgType  = ON_RESPONSE_MBARRACK_DISMISSMONSTER;
			pOnResMBarrackDismissMonster->dwResponse = DWORD(pParam1);
			switch( pOnResMBarrackDismissMonster->dwResponse )
			{
				case ON_RET_OK:
					pOnResMBarrackDismissMonsterOK        = (OnResMBarrackDismissMonsterOK*)cLocalBuffer;
					pOnResMBarrackDismissMonsterOK->Money = *((__int64*)pParam2);
					pOnResMBarrackDismissMonsterOK->Slot  = (BYTE)((SI32)pParam3);

					siLength =	sizeof(OnResMBarrackDismissMonsterOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResMBarrackDismissMonster);
					break;
			}
		}
		break;
	case ON_RESPONSE_HOSPITAL_GETCURELIST:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 치료 용병 수
			// pParam3 : 성공시 치료 용병 Slot
			// pParam4 : 성공시 치료 용병 가격
			// pParam5 : 성공시 치료 용병 가격
			OnResHospitalGetCureListOK*		pOnResHospitalGetCureListOK;
			OnResHospitalGetCureList*		pOnResHospitalGetCureList;
			BYTE*							TempBuffer;

			pOnResHospitalGetCureList			  = (OnResHospitalGetCureList*)cLocalBuffer;
			
			pOnResHospitalGetCureList->dwMsgType  = ON_RESPONSE_HOSPITAL_GETCURELIST;

			if(pParam3 != NULL)
			{
				pOnResHospitalGetCureList->dwResponse = ON_RET_OK;

				pOnResHospitalGetCureListOK = (OnResHospitalGetCureListOK*)cLocalBuffer;
				pOnResHospitalGetCureListOK->dwDiscount        = SI32(pParam1);
				pOnResHospitalGetCureListOK->dwCureSolderCount = SI32(pParam2);

				TempBuffer			        = (BYTE*)((BYTE*)pOnResHospitalGetCureListOK + sizeof(OnResHospitalGetCureListOK));
				CopyMemory(TempBuffer, (BYTE*)pParam3, sizeof(UI08) * pOnResHospitalGetCureListOK->dwCureSolderCount);

				TempBuffer			        = (BYTE*)((BYTE*)TempBuffer + (sizeof(UI08) * pOnResHospitalGetCureListOK->dwCureSolderCount));
				CopyMemory(TempBuffer, (BYTE*)pParam4, sizeof(MONEY) * pOnResHospitalGetCureListOK->dwCureSolderCount);

				TempBuffer			        = (BYTE*)((BYTE*)TempBuffer + (sizeof(MONEY) * pOnResHospitalGetCureListOK->dwCureSolderCount));
				CopyMemory(TempBuffer, (BYTE*)pParam5, sizeof(MONEY) * pOnResHospitalGetCureListOK->dwCureSolderCount);

				siLength =	sizeof(OnResHospitalGetCureListOK) + ((sizeof(UI08) + sizeof(MONEY) + sizeof(MONEY)) * pOnResHospitalGetCureListOK->dwCureSolderCount);
			}
			else
			{
				pOnResHospitalGetCureList->dwResponse = ON_RET_NO;

				siLength =	sizeof(OnResHospitalGetCureList);
			}
		}
		break;
	case ON_RESPONSE_HOSPITAL_CURESOLDER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, 자동치료시는 NULL, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 치료한 짐꾼이 들어가는 Slot
			// pParam4 : 성공시 치료한 짐꾼의 Life가 들어가는 Slot
			// pParam5 : 성공시 치료한 짐꾼의 Mana가 들어가는 Slot
			OnResHospitalCureSolder*		pOnResHospitalCureSolder;
			OnResHospitalCureSolderOK*		pOnResHospitalCureSolderOK;

			pOnResHospitalCureSolder			 = (OnResHospitalCureSolder*)cLocalBuffer;

			pOnResHospitalCureSolder->dwMsgType  = ON_RESPONSE_HOSPITAL_CURESOLDER;
			pOnResHospitalCureSolder->dwResponse = DWORD(pParam1);
			switch(pOnResHospitalCureSolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResHospitalCureSolderOK                   = (OnResHospitalCureSolderOK*)cLocalBuffer;
					pOnResHospitalCureSolderOK->Money            = *((__int64*)pParam2);
					pOnResHospitalCureSolderOK->dwCureSolderSlot = (UI08)pParam3;
					pOnResHospitalCureSolderOK->siLife           = (SI32)pParam4;
					pOnResHospitalCureSolderOK->siMana           = (SI32)pParam5;

					siLength =	sizeof(OnResHospitalCureSolderOK);
					break;

				case ON_RET_AUTOCUREOK:
					pOnResHospitalCureSolderOK                   = (OnResHospitalCureSolderOK*)cLocalBuffer;
					pOnResHospitalCureSolderOK->Money            = 0;
					pOnResHospitalCureSolderOK->dwCureSolderSlot = (UI08)pParam3;
					pOnResHospitalCureSolderOK->siLife           = (SI32)pParam4;
					pOnResHospitalCureSolderOK->siMana           = (SI32)pParam5;

					siLength =	sizeof(OnResHospitalCureSolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResHospitalCureSolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_HOSPITAL_CUREALLSOLDER:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, 자동치료시는 NULL, __int64 이기 때문에 Pointer로 전달하여야 함
			// pParam3 : 성공시 치료한 용병 Count
			// pParam4 : 성공시 치료한 용병 Data
			OnResHospitalCureAllSolder*		pOnResHospitalCureAllSolder;
			OnResHospitalCureAllSolderOK*	pOnResHospitalCureAllSolderOK;

			pOnResHospitalCureAllSolder	   	        = (OnResHospitalCureAllSolder*)cLocalBuffer;

			pOnResHospitalCureAllSolder->dwMsgType  = ON_RESPONSE_HOSPITAL_CUREALLSOLDER;
			pOnResHospitalCureAllSolder->dwResponse = DWORD(pParam1);
			switch(pOnResHospitalCureAllSolder->dwResponse)
			{
				case ON_RET_OK:
					pOnResHospitalCureAllSolderOK                   = (OnResHospitalCureAllSolderOK*)cLocalBuffer;
					pOnResHospitalCureAllSolderOK->Money            = *((__int64*)pParam2);
					pOnResHospitalCureAllSolderOK->CureSolderCount  = (SI16)pParam3;
					CopyMemory(pOnResHospitalCureAllSolderOK->CureSolder, pParam4, sizeof(strCureSolder) * pOnResHospitalCureAllSolderOK->CureSolderCount);

					siLength =	sizeof(OnResHospitalCureAllSolderOK);
					break;

				case ON_RET_AUTOCUREOK:
					pOnResHospitalCureAllSolderOK                   = (OnResHospitalCureAllSolderOK*)cLocalBuffer;
					pOnResHospitalCureAllSolderOK->Money            = 0;
					pOnResHospitalCureAllSolderOK->CureSolderCount  = (SI16)pParam3;
					CopyMemory(pOnResHospitalCureAllSolderOK->CureSolder, pParam4, sizeof(strCureSolder) * pOnResHospitalCureAllSolderOK->CureSolderCount);

					siLength =	sizeof(OnResHospitalCureAllSolderOK);
					break;

				case ON_RET_NO:
					siLength =	sizeof(OnResHospitalCureAllSolder);
					break;
			}
		}
		break;
	case ON_RESPONSE_WHARF_ENTER:
		{
			// pParam1 : 성공여부
			// Ticket 정보를 Player의 Data에서 검사한다.
			OnResWharf_Enter*		pOnResWharf_Enter;

			pOnResWharf_Enter			  = (OnResWharf_Enter*)cLocalBuffer;
			
			pOnResWharf_Enter->dwMsgType  = ON_RESPONSE_WHARF_ENTER;
			pOnResWharf_Enter->dwResponse = DWORD(pParam1);

			pOnResWharf_Enter->siTicketKind      = stTicketInfo.siTicketKind;

			siLength = sizeof(OnResWharf_Enter);
		}
		break;
	case ON_RESPONSE_WHARF_CHANGETICKETLIST:
		{
			// pParam1 : 성공여부
			// pParam2 : Ticket Count
			// pParam3 : Ticket Info(OnTicketInfo)
			// pParam4 : 할인액
			OnResWharf_ChangeTicketList*		pOnResWharf_ChangeTicketList;
			BYTE*								TempBuffer;

			pOnResWharf_ChangeTicketList			    = (OnResWharf_ChangeTicketList*)cLocalBuffer;
			TempBuffer                                  = (BYTE*)((BYTE*)pOnResWharf_ChangeTicketList + sizeof(OnResWharf_ChangeTicketList));

			pOnResWharf_ChangeTicketList->dwMsgType         = ON_RESPONSE_WHARF_CHANGETICKETLIST;
			pOnResWharf_ChangeTicketList->siTicketCount     = DWORD(pParam2);
			pOnResWharf_ChangeTicketList->siDiscountPercent = SI32(pParam4);

			CopyMemory(TempBuffer, pParam3, sizeof(OnTicketInfo) * pOnResWharf_ChangeTicketList->siTicketCount);
			siLength = sizeof(OnResWharf_ChangeTicketList) + sizeof(OnTicketInfo) * pOnResWharf_ChangeTicketList->siTicketCount;
		}
		break;
	case ON_RESPONSE_WHARF_BUYTICKET:
		{
			// pParam1 : 성공여부
			// pParam2 : 성공시 Money의 Pointer, __int64 이기 때문에 Pointer로 전달하여야 함
			// 새로산 Ticket 정보는 Player의 Data에서 검사한다.
			OnResWharf_BuyTicket*		pOnResWharf_BuyTicket;

			pOnResWharf_BuyTicket			  = (OnResWharf_BuyTicket*)cLocalBuffer;
			
			pOnResWharf_BuyTicket->dwMsgType  = ON_RESPONSE_WHARF_BUYTICKET;
			pOnResWharf_BuyTicket->dwResponse = DWORD(pParam1);

			if(pOnResWharf_BuyTicket->dwResponse == ON_RET_OK)
			{
				pOnResWharf_BuyTicket->NowMoney          = *((__int64*)pParam2);

				pOnResWharf_BuyTicket->siTicketKind      = stTicketInfo.siTicketKind;
			}

			siLength = sizeof(OnResWharf_BuyTicket);
		}
		break;
	case ON_RESPONSE_WHARF_EXIT:
		{
			// pParam1 : 성공여부
			OnResWharf_Exit*		pOnResWharf_Exit;

			pOnResWharf_Exit             = (OnResWharf_Exit*)cLocalBuffer;

			pOnResWharf_Exit->dwMsgType  = ON_RESPONSE_WHARF_EXIT;
			pOnResWharf_Exit->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResWharf_Exit);
		}
		break;
	case ON_RESPONSE_WHARFWAITROOM_ENTER:
		{
			// pParam1 : 성공여부
			// Ticket 정보를 Player의 Data에서 검사한다.
			OnResWharfWaitRoom_Enter*		pOnResWharfWaitRoom_Enter;

			pOnResWharfWaitRoom_Enter			  = (OnResWharfWaitRoom_Enter*)cLocalBuffer;
			
			pOnResWharfWaitRoom_Enter->dwMsgType  = ON_RESPONSE_WHARFWAITROOM_ENTER;
			pOnResWharfWaitRoom_Enter->dwResponse = DWORD(pParam1);

			pOnResWharfWaitRoom_Enter->siTicketKind      = stTicketInfo.siTicketKind;

			siLength = sizeof(OnResWharfWaitRoom_Enter);
		}
		break;
	case ON_RESPONSE_WHARFWAITROOM_EXIT:
		{
			// pParam1 : 성공여부
			OnResWharfWaitRoom_Exit*		pOnResWharfWaitRoom_Exit;

			pOnResWharfWaitRoom_Exit             = (OnResWharfWaitRoom_Exit*)cLocalBuffer;

			pOnResWharfWaitRoom_Exit->dwMsgType  = ON_RESPONSE_WHARFWAITROOM_EXIT;
			pOnResWharfWaitRoom_Exit->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResWharfWaitRoom_Exit);
		}
		break;
	case ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP:
		{
			// pParam1 : 성공여부
			OnResWharfWaitRoom_MoveInShip*		pOnResWharfWaitRoom_MoveInShip;

			pOnResWharfWaitRoom_MoveInShip             = (OnResWharfWaitRoom_MoveInShip*)cLocalBuffer;

			pOnResWharfWaitRoom_MoveInShip->dwMsgType  = ON_RESPONSE_WHARFWAITROOM_MOVEINSHIP;
			pOnResWharfWaitRoom_MoveInShip->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResWharfWaitRoom_MoveInShip);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_MAIN_ENTER:
		{
			// pParam1 : 성공여부
			OnResEnterGovernment*		pOnResEnterGovernment;

			pOnResEnterGovernment = (OnResEnterGovernment*)cLocalBuffer;

			pOnResEnterGovernment->dwMsgType  = ON_RESPONSE_GOVERNMENT_MAIN_ENTER;
			pOnResEnterGovernment->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResEnterGovernment);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_MAIN_LEAVE:
		{
			// pParam1 : 성공여부
			OnResLeaveGovernment*		pOnResLeaveGovernment;

			pOnResLeaveGovernment = (OnResLeaveGovernment*)cLocalBuffer;

			pOnResLeaveGovernment->dwMsgType  = ON_RESPONSE_GOVERNMENT_MAIN_LEAVE;
			pOnResLeaveGovernment->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResLeaveGovernment);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER:
		{
			// pParam1 : 성공여부
			// pParam2 : 투자 Type
			// pParam3 : 최소 투자 Percent
			// pParam4 : 최대 투자 Percent
			OnResGovernmentEnterInvestment*			pOnResGovernmentEnterInvestment;

			pOnResGovernmentEnterInvestment = (OnResGovernmentEnterInvestment*)cLocalBuffer;

			pOnResGovernmentEnterInvestment->dwMsgType  = ON_RESPONSE_GOVERNMENT_INVESTMENT_ENTER;
			pOnResGovernmentEnterInvestment->dwResponse = DWORD(pParam1);
			pOnResGovernmentEnterInvestment->m_Type     = 0;
			if(pOnResGovernmentEnterInvestment->dwResponse == ON_RET_OK)
			{
				pOnResGovernmentEnterInvestment->m_Type                 = (SI16)(pParam2);
				pOnResGovernmentEnterInvestment->m_InvestmentMinPercent = (SI32)(pParam3);
				pOnResGovernmentEnterInvestment->m_InvestmentMaxPercent = (SI32)(pParam4);
			}

			siLength = sizeof(OnResGovernmentEnterInvestment);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE:
		{
			// pParam1 : 성공여부
			OnResGovernmentLeaveInvestment*			pOnResGovernmentLeaveInvestment;

			pOnResGovernmentLeaveInvestment = (OnResGovernmentLeaveInvestment*)cLocalBuffer;

			pOnResGovernmentLeaveInvestment->dwMsgType  = ON_RESPONSE_GOVERNMENT_INVESTMENT_LEAVE;
			pOnResGovernmentLeaveInvestment->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResGovernmentLeaveInvestment);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST:
		{
			// pParam1 : 최고 투자자 ID
			// pParam2 : List Type(0xffff 이면 실패다.)
			// pParam3 : 마을 수익
			// pParam4 : List 수
			// pParam5 : List
			SI32										TempCount;
			OnResGovernmentGetInvestmentList*			pOnResGovernmentGetInvestmentList;
			stVillageInvestorData*						pstVillageInvestorData;
			OnInvestmentInfo*							pOnInvestmentInfo;
			BYTE*										pTempBuffer;
			SI32										TempMySlot;

			pOnResGovernmentGetInvestmentList = (OnResGovernmentGetInvestmentList*)cLocalBuffer;
			ZeroMemory(pOnResGovernmentGetInvestmentList, sizeof(OnResGovernmentGetInvestmentList));
			pOnResGovernmentGetInvestmentList->dwMsgType  = ON_RESPONSE_GOVERNMENT_INVESTMENT_GETLIST;	

			if((DWORD)pParam2 != 0xffff)
			{
				pTempBuffer = (BYTE*)((BYTE*)pOnResGovernmentGetInvestmentList + sizeof(OnResGovernmentGetInvestmentList));

				if(pParam1) strcpy((char*)pOnResGovernmentGetInvestmentList->m_MaxInvestmentID, (char*)pParam1);
				pOnResGovernmentGetInvestmentList->dwResponse            = ON_RET_OK;
				pOnResGovernmentGetInvestmentList->m_Type                = (SI16)((DWORD)pParam2 & 0xffff);
				pOnResGovernmentGetInvestmentList->m_GainingsMoney       = *((__int64*)pParam3);
				pOnResGovernmentGetInvestmentList->m_InvestmentListCount = (SI16)(pParam4);
				pOnResGovernmentGetInvestmentList->m_fGetOut             = (BYTE)(((DWORD)pParam2 & 0xff0000) >> 16);
				pOnResGovernmentGetInvestmentList->m_GetOutTex           = (BYTE)(((DWORD)pParam2 & 0xff000000) >> 24);

				// 뒤의 List
				pstVillageInvestorData = (stVillageInvestorData*)pParam5;
				pOnInvestmentInfo      = (OnInvestmentInfo*)pTempBuffer;
				TempMySlot             = -1;
				for(TempCount = 0; TempCount < pOnResGovernmentGetInvestmentList->m_InvestmentListCount; TempCount++)
				{				
					pOnInvestmentInfo[TempCount].m_InvestmentType     = (UI08)pstVillageInvestorData[TempCount].m_InvestorType;
					CopyMemory(pOnInvestmentInfo[TempCount].m_InvestmentName, pstVillageInvestorData[TempCount].m_InvestorID, 21);

					pOnInvestmentInfo[TempCount].m_InvestmentPercent    = pstVillageInvestorData[TempCount].m_Percent;
					pOnInvestmentInfo[TempCount].m_InvestmentMoney      = pstVillageInvestorData[TempCount].m_Money;
					pOnInvestmentInfo[TempCount].m_InvestmentUpdateTime = pstVillageInvestorData[TempCount].m_UpdateTime;

					if(TempMySlot == -1)
					{
						if(pOnInvestmentInfo[TempCount].m_InvestmentType == 0)
						{
							if(strncmp((char*)pOnInvestmentInfo[TempCount].m_InvestmentName, (char*)szID, ON_ID_LENGTH) == 0)
							{
								TempMySlot = TempCount;
							}
						}
					}
				}

				if(TempMySlot == -1)
				{
					pOnResGovernmentGetInvestmentList->m_MyInvestmentCanTime = 0;
				}
				else
				{
					pOnResGovernmentGetInvestmentList->m_MyInvestmentCanTime = MakeOneMonthAfter(pOnInvestmentInfo[TempMySlot].m_InvestmentUpdateTime);
				}

				siLength = sizeof(OnResGovernmentGetInvestmentList) + sizeof(OnInvestmentInfo) * pOnResGovernmentGetInvestmentList->m_InvestmentListCount;
			}
			else
			{
				pOnResGovernmentGetInvestmentList->dwResponse = ON_RET_NO;
				siLength = sizeof(OnResGovernmentGetInvestmentList);
			}
		}
		break;
	case ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT:
		{
			// pParam1 : 성공여부
			OnResGovernmentSetInvestment*			pOnResGovernmentSetInvestment;

			pOnResGovernmentSetInvestment = (OnResGovernmentSetInvestment*)cLocalBuffer;

			pOnResGovernmentSetInvestment->dwMsgType  = ON_RESPONSE_GOVERNMENT_INVESTMENT_SETINVESTMENT;
			pOnResGovernmentSetInvestment->dwResponse = DWORD(pParam1);

			siLength = sizeof(OnResGovernmentSetInvestment);
		}
		break;
	case ON_RESPONSE_GOVERNMENT_VILLAGEINFO:
		{
			// 관청, 마을 정보 얻기 응답
			OnResGovernmentVillageInfo*				TempOnResGovernmentVillageInfo;

			TempOnResGovernmentVillageInfo = (OnResGovernmentVillageInfo*)cLocalBuffer;
			TempOnResGovernmentVillageInfo->dwMsgType                      = ON_RESPONSE_GOVERNMENT_VILLAGEINFO;
			strcpy((char*)TempOnResGovernmentVillageInfo->m_VillageInfo_Descript, (char*)pParam1);
			TempOnResGovernmentVillageInfo->m_VillageInfo_InOutPlayerCount = (SI32)pParam2;

			TempOnResGovernmentVillageInfo->CurDefencePower					=	*( ( VDEF* )pParam3 );
			TempOnResGovernmentVillageInfo->MaxDefencePower					=	*( ( VDEF* )pParam4 );

			siLength = sizeof(OnResGovernmentVillageInfo);
		}
		break;
	case ON_RESPONSE_PROFIT_STATISTICS:
	case ON_RESPONSE_AUDIT_PROFIT_STATISTICS:
		{
			OnResProfitStatistics					*pProfitStatistics;
			stCapital								*pCapital;
			stCapital								*pTemp;

			pProfitStatistics				=	(OnResProfitStatistics*)	cLocalBuffer;
			pProfitStatistics->dwMsgType	=	siMsg;				// ON_RESPONSE_PROFIT_STATISTICS or ON_RESPONSE_AUDIT_PROFIT_STATISTICS
			pCapital						=	(stCapital*) pParam1;
			for(i=0; i<ON_STATISTICS_YEAR;i++)
			{
				if(pCapital[i].Year==0)
					break;
			}
			pProfitStatistics->siNum		=	i;
			pTemp							=	(stCapital*)((CHAR*)pProfitStatistics + sizeof(OnResProfitStatistics));

			memcpy(pTemp, pCapital, sizeof(stCapital)*pProfitStatistics->siNum);

			siLength	=	sizeof(OnResProfitStatistics) + ( sizeof(stCapital)*pProfitStatistics->siNum );
		}
		break;

	case ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT:
		{
			// 관청, 투자한돈 가지고 나가기 응답
			// pParam1 은 성공여부
			// pParam2 은 반환하는 돈
			// pParam3 은 세금
			OnResGovernmentInvestmentGetOut*				TempOnResGovernmentInvestmentGetOut;

			TempOnResGovernmentInvestmentGetOut            = (OnResGovernmentInvestmentGetOut*)cLocalBuffer;
			TempOnResGovernmentInvestmentGetOut->dwMsgType = ON_RESPONSE_GOVERNMENT_INVESTMENT_GETOUT;
			TempOnResGovernmentInvestmentGetOut->dwSuccess = (BYTE)((DWORD)pParam1);

			if(pParam2)
			{
				CopyMemory(&TempOnResGovernmentInvestmentGetOut->GetOutMoney, pParam2, sizeof(MONEY));
			}
			if(pParam3)
			{
				CopyMemory(&TempOnResGovernmentInvestmentGetOut->Tex,         pParam3, sizeof(MONEY));
			}

			siLength = sizeof(OnResGovernmentInvestmentGetOut);
		}
		break;
		
		// 아이템 교환(Trade) 관련.
	case ON_RECEIVE_ASK_TRADE:
		{		
			OnReceiveAskTradeMsg				*pReceiveAskTradeMsg;

			pReceiveAskTradeMsg				=	(OnReceiveAskTradeMsg*)	cLocalBuffer;
			pReceiveAskTradeMsg->dwMsgType	=	ON_RECEIVE_ASK_TRADE;
			pReceiveAskTradeMsg->UserID		=	UI16(pParam1);			
		
			ZeroMemory(pReceiveAskTradeMsg->szID, sizeof(pReceiveAskTradeMsg->szID));
			strncpy(pReceiveAskTradeMsg->szID, (CHAR*)pParam2, ON_ID_LENGTH);

			siLength	=	sizeof(OnReceiveAskTradeMsg);
		}
		break;
	case ON_RESPONSE_TRADE:
		{
			OnResAskTradeMsg					*pResAskTradeMsg;

			pResAskTradeMsg					=	(OnResAskTradeMsg*) cLocalBuffer;
			pResAskTradeMsg->dwMsgType		=	ON_RESPONSE_TRADE;
			pResAskTradeMsg->dwResponse		=	DWORD(pParam1);			

			siLength						=	sizeof(OnResAskTradeMsg);
		}
		break;	
	case ON_CHANGED_MONEY:
		{
			OnResMoneyInfo						*pResMoneyInfo;
			pResMoneyInfo						=	(OnResMoneyInfo*)	cLocalBuffer;
			pResMoneyInfo->dwMsgType			=	ON_CHANGED_MONEY;

			pResMoneyInfo->dwResponse			=	DWORD(pParam1);
			pResMoneyInfo->Money				=	this->siMoney;

			siLength							=	sizeof(OnResMoneyInfo);
		}
		break;
	case ON_TEXT_MSG:
		{
			OnTextMsg				*pTextMsg;
			CHAR					*pTextOut;

			pTextMsg					=	(OnTextMsg*)cLocalBuffer;
			pTextMsg->dwMsgType			=	ON_TEXT_MSG;
			pTextMsg->dwResponse		=	DWORD(pParam1);
			pTextMsg->siStringLength	=	strlen((CHAR*)pParam2);

			pTextOut	=	(CHAR*)((CHAR*)pTextMsg + sizeof(OnTextMsg));

			strncpy(pTextOut, (CHAR*)pParam2, pTextMsg->siStringLength);

			siLength	=	sizeof(OnTextMsg) + pTextMsg->siStringLength;
		}
		break;	
	case ON_RESPONSE_INC_ABILITY_BYBONUS:
		{
			OnResIncAbilityByBonusMsg	*pIncAbilityByBonusMsg;

			pIncAbilityByBonusMsg				=	(OnResIncAbilityByBonusMsg*)cLocalBuffer;
			pIncAbilityByBonusMsg->dwMsgType	=	ON_RESPONSE_INC_ABILITY_BYBONUS;
			pIncAbilityByBonusMsg->dwResponse	=	DWORD(pParam1);
			
			siLength							=	sizeof(OnResIncAbilityByBonusMsg);
		}
		break;
	case ON_RESPONSE_GUILD_CREATE_COST:
		{
			OnResponseGuildCreateCost	*pGuildCreateCostMsg;

			pGuildCreateCostMsg					=	(OnResponseGuildCreateCost*)cLocalBuffer;
			pGuildCreateCostMsg->dwMsgType		=	ON_RESPONSE_GUILD_CREATE_COST;
			pGuildCreateCostMsg->mnGuildCreateCost	=	*((MONEY*)pParam1);

			siLength							=	sizeof(OnResponseGuildCreateCost);
		}
		break;
	case ON_RESPONSE_CREATE_GUILD:
		{
			OnResCreateGuildMsg		*pCreateGuildMsg;

			pCreateGuildMsg						=	(OnResCreateGuildMsg*)cLocalBuffer;
			pCreateGuildMsg->dwMsgType			=	ON_RESPONSE_CREATE_GUILD;
			pCreateGuildMsg->dwResponse			=	DWORD(pParam1);

			siLength							=	sizeof(OnResCreateGuildMsg);
		}
		break;
	case ON_RESPONSE_DELETE_GUILD:
		{
			OnResDeleteGuildMsg			*pDeleteGuildMsg;

			pDeleteGuildMsg						=	(OnResDeleteGuildMsg*)cLocalBuffer;
			pDeleteGuildMsg->dwMsgType			=	ON_RESPONSE_DELETE_GUILD;
			pDeleteGuildMsg->dwResponse			=	DWORD(pParam1);

			siLength							=	sizeof(OnResDeleteGuildMsg);
		}
		break;
	case ON_RESPONSE_GUILD_LIST:
		{
			OnResGuildListMsg			*pGuildListMsg;
			cltGuild					*pclMyGuild, *pclOtherGuild;
			strGuildInfo				*pstGuildInfo = NULL;
			SI32						siRelation;

			pGuildListMsg						=	(OnResGuildListMsg*)cLocalBuffer;
			pGuildListMsg->dwMsgType			=	ON_RESPONSE_GUILD_LIST;
			pstGuildInfo						=	(strGuildInfo*)((CHAR*)pGuildListMsg + sizeof(OnResGuildListMsg));

			// 나의 상단을 구한다.
			if(uiGuildID != 0)
				pclMyGuild	=	pMyWorld->clIGuild.GetGuildByID(uiGuildID);
			else			
				pclMyGuild	=	NULL;
			
			pGuildListMsg->uiGuildNum			=	0;
			for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
			{
				if((pclOtherGuild = pMyWorld->clIGuild.GetGuildByIndex(i)) != NULL)
				{
					ZeroMemory(&pstGuildInfo[pGuildListMsg->uiGuildNum], sizeof(strGuildInfo));
					
					strncpy(pstGuildInfo[pGuildListMsg->uiGuildNum].szGuildID, pclOtherGuild->GetGuildName(), ON_GUILDNAME_LENGTH);				
					strncpy(pstGuildInfo[pGuildListMsg->uiGuildNum].szDAIID, pclOtherGuild->GetDAIID(), ON_ID_LENGTH);

					pstGuildInfo[pGuildListMsg->uiGuildNum].uiOwnVillageNum		 =	pclOtherGuild->GetOwnerVillageNum();
					pstGuildInfo[pGuildListMsg->uiGuildNum].uiGuildMemberNum	 =	pclOtherGuild->GetGuildMemberNum();					
					pstGuildInfo[pGuildListMsg->uiGuildNum].uiMaxGuildMembersNum =	ON_GUILD_MIN_MEMBER_NUM;
					pstGuildInfo[pGuildListMsg->uiGuildNum].siFlag				 =	pclOtherGuild->GetFlag();

					// 만약 내가 어느 상단에도 가입해 있지 않다면 상단의 관계를 줄 필요가 없다.
					if(pclMyGuild != NULL)
					{	// 내 상단에서의 상대방 상단과의 관계
						siRelation = pclMyGuild->GetRelation(pclOtherGuild->GetArrayIndex());
						pstGuildInfo[pGuildListMsg->uiGuildNum].siRelation = pMyWorld->clIGuild.RelationDefineToResponseDefine(siRelation);
						
						strncpy( pstGuildInfo[pGuildListMsg->uiGuildNum].szGuildFriendlyMessage, 
							     pclMyGuild->GetFrieldlyMessage(pclOtherGuild->GetArrayIndex()),
								 81 );
						
						// 상대방 상단이 내 상단과의 관계
						siRelation = pclOtherGuild->GetRelation(pclMyGuild->GetArrayIndex());
						pstGuildInfo[pGuildListMsg->uiGuildNum].siRelationOfOpponent = pMyWorld->clIGuild.RelationDefineToResponseDefine(siRelation);
					}
					else
					{
						pstGuildInfo[pGuildListMsg->uiGuildNum].siRelation				=	0;
						pstGuildInfo[pGuildListMsg->uiGuildNum].siRelationOfOpponent	=	0;
					}

					pGuildListMsg->uiGuildNum++;
				}			
			}			

			siLength							=	sizeof(OnResGuildListMsg) + (sizeof(strGuildInfo) * pGuildListMsg->uiGuildNum);
		}
		break;
	case ON_RESPONSE_JOIN_LEAVE_GUILD:
		{
			OnResJoinLeaveGuildMsg			*pJoinLeaveGuildMsg;			

			pJoinLeaveGuildMsg					=	(OnResJoinLeaveGuildMsg*)cLocalBuffer;
			pJoinLeaveGuildMsg->dwMsgType		=	ON_RESPONSE_JOIN_LEAVE_GUILD;
			pJoinLeaveGuildMsg->dwResponse		=	DWORD(pParam1);

			siLength							=	sizeof(OnResJoinLeaveGuildMsg);
		}
		break;
	case ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST:
		{
			OnResGuildWaitingJoinMembersListMsg		*pGWJMLMsg;			
			strCandidateInfo						*pCandidateInfo;
			DWORD									dwReqPage;
			cltGuild								*pclGuild;

			pGWJMLMsg					=	(OnResGuildWaitingJoinMembersListMsg*)cLocalBuffer;
			pGWJMLMsg->dwMsgType		=	ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST;
			pGWJMLMsg->dwResponse		=	DWORD(pParam1);		
			pclGuild					=	(cltGuild*)pParam2;
			dwReqPage					=	DWORD(pParam3);
			
						
			switch(pGWJMLMsg->dwResponse)
			{
			case ON_RET_OK:
				// 총 페이지수를 다시 보내준다.
				pGWJMLMsg->dwTotalCandidateListPageNum		=	pclGuild->GetCMTotalPageNum();			

				pCandidateInfo						=	(strCandidateInfo*)((CHAR*)pGWJMLMsg + sizeof(OnResGuildWaitingJoinMembersListMsg));			
				
				pGWJMLMsg->uiCandidateMembersNum		=	pclGuild->GetCMList(dwReqPage, pCandidateInfo);								
				pGWJMLMsg->dwNowCandidateListPageNum	=	dwReqPage;
				
				siLength			=	sizeof(OnResGuildWaitingJoinMembersListMsg)	+
										(pGWJMLMsg->uiCandidateMembersNum * sizeof(strCandidateInfo));
				break;
			case ON_RET_NO:
				siLength			=	sizeof(OnResGuildWaitingJoinMembersListMsg);					
				break;
			}			
		}		
		break;
	case ON_RESPONSE_GUILD_MEMBERS_LIST:
		{
			OnResGuildMembersListMsg			*pGuildMembersListMsg;
			strGuildMemberInfo					*pGuildMemberInfo;
			DWORD								dwReqPage;
			cltGuild							*pclGuild;

			pGuildMembersListMsg				=	(OnResGuildMembersListMsg*)cLocalBuffer;
			pGuildMembersListMsg->dwMsgType		=	ON_RESPONSE_GUILD_MEMBERS_LIST;
			pGuildMembersListMsg->dwResponse	=	DWORD(pParam1);		
			pclGuild							=	(cltGuild*)pParam2;
			dwReqPage							=	DWORD(pParam3);
			
			
			// 총 페이지수를 다시 보내준다.
			pGuildMembersListMsg->dwTotalMemberListPageNum		=	pclGuild->GetRMTotalPageNum();

			// 페이지수가 현재 총 페이지를 능가할 경우
			// 더 작은 페이지를 주게 해준다.
			dwReqPage											=	min(dwReqPage, pGuildMembersListMsg->dwTotalMemberListPageNum);

			switch(pGuildMembersListMsg->dwResponse)
			{
			case ON_RET_OK:
				pGuildMemberInfo								=	(strGuildMemberInfo*)((CHAR*)pGuildMembersListMsg + sizeof(OnResGuildMembersListMsg));
			
				//pGuildMembersListMsg->uiMaxGuildMemebersNum		=	pclGuild->GetMaxGuildMemebersNum();
				pGuildMembersListMsg->uiMaxGuildMemebersNum		=	ON_GUILD_MIN_MEMBER_NUM;
				pGuildMembersListMsg->uiGuildMemebersNum		=	pclGuild->GetRMList(dwReqPage, pGuildMemberInfo);				
				pGuildMembersListMsg->dwNowMemberListPageNum	=	dwReqPage;
				
				siLength			=	sizeof(OnResGuildMembersListMsg)	+
										(pGuildMembersListMsg->uiGuildMemebersNum * sizeof(strGuildMemberInfo));
				break;
			case ON_RET_NO:
				siLength			=	sizeof(OnResGuildMembersListMsg);					
				break;
			}			
		}
		break;
	case ON_RESPONSE_JOINGUILD_OKNO:
		{
			OnResJoinGuildOkNoMsg		*pJoinGuildOkNoMsg;

			pJoinGuildOkNoMsg				=	(OnResJoinGuildOkNoMsg*)cLocalBuffer;
			pJoinGuildOkNoMsg->dwMsgType	=	ON_RESPONSE_JOINGUILD_OKNO;
			pJoinGuildOkNoMsg->dwResponse	=	DWORD(pParam1);

			siLength						=	sizeof(OnResJoinGuildOkNoMsg);
		}
		break;

	case ON_RELOAD_WEAR_ITEM:
		{
			OnReLoadWearItem			*pOnReLoadWearItem;
			BYTE						*pTempBuffer;

			pOnReLoadWearItem				=	(OnReLoadWearItem*)cLocalBuffer;

			pOnReLoadWearItem->dwMsgType		=	ON_RELOAD_WEAR_ITEM;
			pOnReLoadWearItem->siItemCount		=	SI16(pParam2);		

			pTempBuffer	= (BYTE*)( (BYTE*)pOnReLoadWearItem + sizeof(OnReLoadWearItem) );
			CopyMemory(pTempBuffer,	(BYTE*)pParam1,	sizeof(OnTimerItemInfo)*pOnReLoadWearItem->siItemCount);

			siLength	=	sizeof(OnReLoadWearItem) + ( sizeof(OnTimerItemInfo) * pOnReLoadWearItem->siItemCount);
		}
		break;		
	case ON_RESPONSE_AUDIT_GUILD_INFO:
		{
			// pParam1 : 상단원 수(-1 : 상단 정보 변경 없음
			//					   -2 : 상단원이 아님)
			// pParam2 : 로그인 한 상단원 수
			// pParam3 : 사환을 제외한 상단원 수
			// pParam4 : 상단원 정보			
			OnRespAuditGuildMemberInfoMsg		*pAuditGuildMemberList;
			BYTE								*pTempBuffer;
			
			pAuditGuildMemberList					=	(OnRespAuditGuildMemberInfoMsg*)cLocalBuffer;
			pAuditGuildMemberList->dwMsgType		=	ON_RESPONSE_AUDIT_GUILD_INFO;
		
			pAuditGuildMemberList->siNowGuildMemberCount		= SI32(pParam1);
			pAuditGuildMemberList->siLoginedGuildMemberCount	= SI32(pParam2);
			pAuditGuildMemberList->siSHNExceptMemberCount		= SI32(pParam3);

			if(!(pAuditGuildMemberList->siSHNExceptMemberCount == -1 || 
				 pAuditGuildMemberList->siSHNExceptMemberCount == -2))
			{				
				pTempBuffer	= (BYTE*)( (BYTE*)pAuditGuildMemberList + sizeof(OnRespAuditGuildMemberInfoMsg) );
				CopyMemory( pTempBuffer, (BYTE*)pParam4, sizeof(strAuditGuildMemberInfo)*pAuditGuildMemberList->siSHNExceptMemberCount );
				siLength	=	sizeof(OnRespAuditGuildMemberInfoMsg) + ( sizeof(strAuditGuildMemberInfo) * pAuditGuildMemberList->siSHNExceptMemberCount );
			}
			else
			{
				siLength	=	sizeof(OnRespAuditGuildMemberInfoMsg);
			}
		}
		break;
	case ON_RESPONSE_GUILD_DISMISSAL:
		{
			OnResDismissalMsg			*pDismissalMsg;

			pDismissalMsg					=	(OnResDismissalMsg*)cLocalBuffer;
			pDismissalMsg->dwMsgType		=	ON_RESPONSE_GUILD_DISMISSAL;
			pDismissalMsg->dwResponse		=	DWORD(pParam1);

			siLength						=	sizeof(OnResDismissalMsg);
		}
		break;
	case ON_RESPONSE_GUILD_PROMOTION:
		{
			OnResponseGuildPromotion	*pPromotionMsg;

			pPromotionMsg					=	(OnResponseGuildPromotion*)cLocalBuffer;
			pPromotionMsg->dwMsgType		=	ON_RESPONSE_GUILD_PROMOTION;
			pPromotionMsg->dwResponse		=	DWORD(pParam1);

			siLength						=	sizeof(OnResponseGuildPromotion);
		}
		break;
	case ON_RESPONSE_GUILD_DEMOTION:
		{
			OnResponseGuildDemotion		*pDemotionMsg;

			pDemotionMsg					=	(OnResponseGuildDemotion*)cLocalBuffer;
			pDemotionMsg->dwMsgType			=	ON_RESPONSE_GUILD_DEMOTION;
			pDemotionMsg->dwResponse		=	DWORD(pParam1);

			siLength						=	sizeof(OnResponseGuildDemotion);
		}
		break;
	case ON_GUILD_MSG:
		{
			OnGuildMsg					*pGuildMsg;		
			SI32						*psiClass;

			pGuildMsg						=	(OnGuildMsg*)cLocalBuffer;
			pGuildMsg->dwMsgType			=	ON_GUILD_MSG;
			pGuildMsg->dwResponse			=	DWORD(pParam1);

			siLength						=	sizeof(OnGuildMsg);

			switch(pGuildMsg->dwResponse)
			{
			case ON_RET_GLD_DELETEGLD:
			case ON_RET_GLD_DISMISSAL:
				// 상단이 삭제되었다.				
				break;
			case ON_RET_GLD_CHANGECLASS:			// 상단에서 나의 직급이 변경되었다.
				
				psiClass	=	(SI32*)((CHAR*)pGuildMsg + sizeof(OnGuildMsg));
				*psiClass	=	SI32(pParam2);

				siLength	+=	sizeof(SI32);
				break;
			}
		}
		break;
	case ON_HUNGRY:
		{
			OnHugryMsg					*pHugryMsg;
			OnHugryChangeHealthMsg	*pDecreaseHealthMsg;
			OnHugryChangeHPMsg		*pHugryDecreaseHPMsg;			
			strHungryFollowerInfo		*pstHFI;

			pHugryMsg				=	(OnHugryMsg*)cLocalBuffer;
			pHugryMsg->dwMsgType	=	ON_HUNGRY;
			pHugryMsg->dwResponse	=	DWORD(pParam1);

			switch(pHugryMsg->dwResponse)
			{
			case ON_RET_HUR_CHANGE_HEALTH:
				pDecreaseHealthMsg				=	(OnHugryChangeHealthMsg*)pHugryMsg;
				pDecreaseHealthMsg->siHealth	=	SI32(pParam2);

				siLength						=	sizeof(OnHugryChangeHealthMsg);
				break;
			case ON_RET_HUR_CHANGE_HP:
				pHugryDecreaseHPMsg				=	(OnHugryChangeHPMsg*)pHugryMsg;
				pstHFI							=	(strHungryFollowerInfo*)((CHAR*)pHugryDecreaseHPMsg + sizeof(OnHugryChangeHPMsg));
				
				pHugryDecreaseHPMsg->uiFollowerNum	=	UI08(pParam2);
				memcpy(pstHFI, pParam3, sizeof(strHungryFollowerInfo) * pHugryDecreaseHPMsg->uiFollowerNum);
				
				siLength						=	sizeof(OnHugryChangeHPMsg) + (sizeof(strHungryFollowerInfo) * pHugryDecreaseHPMsg->uiFollowerNum);
				break;
			}
		}
		break;
	case ON_CHANGED_PLAYER_DEAD_PENALTY:
		{
			OnResDeadPenalty*			   pOnResDeadPenalty;

			pOnResDeadPenalty				=   (OnResDeadPenalty*)cLocalBuffer;
			pOnResDeadPenalty->dwMsgType	=	ON_CHANGED_PLAYER_DEAD_PENALTY;

			siLength						=	sizeof(OnResDeadPenalty);
		}
		break;
	case ON_CHANGED_WAR_END:
		{
			OnChangedWarEnd*				pOnChangedWarEnd;
			SOPlayer						*pPlayer;

			pPlayer							=	(SOPlayer*)	pParam2;

			pOnChangedWarEnd				=	(OnChangedWarEnd*)cLocalBuffer;
			pOnChangedWarEnd->dwMsgType		=	ON_CHANGED_WAR_END;
			pOnChangedWarEnd->uiKind		=	UI16(pParam1);
			pOnChangedWarEnd->uiPlayerID	=	pPlayer->uiAccount;
											
			siLength						=	sizeof(OnChangedWarEnd);
		}	
		break;
	case ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK:
		{
			OnResVillageAttackTypeCheck*   pOnResVillageAttackTypeCheck;

			pOnResVillageAttackTypeCheck				=	(OnResVillageAttackTypeCheck*)cLocalBuffer;
			pOnResVillageAttackTypeCheck->dwMsgType		=	ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK;
			pOnResVillageAttackTypeCheck->dwResponse	=	DWORD(pParam1);
			pOnResVillageAttackTypeCheck->uiVillageCode =   UI16(pParam2);

			siLength	=	sizeof(OnResVillageAttackTypeCheck);
		}
		break;

	case ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK:
		{
			OnResVillagePlayerOilAttack*	pOnResVillagePlayerOilAttack;

			pOnResVillagePlayerOilAttack							=   (OnResVillagePlayerOilAttack*)cLocalBuffer;
			pOnResVillagePlayerOilAttack->dwMsgType					=	ON_RESPONSE_VILLAGE_PLAYER_OIL_ATTACK;
			pOnResVillagePlayerOilAttack->uiVillageCode				=	UI16(pParam1);
			pOnResVillagePlayerOilAttack->dwResponse				=	DWORD(pParam2);
			pOnResVillagePlayerOilAttack->mnMoney					=	MONEY(pParam3);
			pOnResVillagePlayerOilAttack->siLimitTime				=	SI32(pParam4);
			pOnResVillagePlayerOilAttack->uiUniqueID				=	UI16(pParam5);

			siLength	=	sizeof(OnResVillagePlayerOilAttack);

		}
		break;
	
	case ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK:
		{
			OnResVillagePlayerStoneAttack*	pOnResVillagePlayerStoneAttack;

			pOnResVillagePlayerStoneAttack							=	(OnResVillagePlayerStoneAttack*)cLocalBuffer;
			pOnResVillagePlayerStoneAttack->dwMsgType				=	ON_RESPONSE_VILLAGE_PLAYER_STONE_ATTACK;
			pOnResVillagePlayerStoneAttack->uiVillageCode			=	UI16(pParam1);
			pOnResVillagePlayerStoneAttack->dwResponse				=	DWORD(pParam2);
			pOnResVillagePlayerStoneAttack->mnMoney					=	MONEY(pParam3);
			pOnResVillagePlayerStoneAttack->siLimitTime				=	SI32(pParam4);
			pOnResVillagePlayerStoneAttack->uiUniqueID				=	UI16(pParam5);

			siLength	=	sizeof(OnResVillagePlayerStoneAttack);
			
		}
		break;
	case ON_RESPONSE_VILLAGE_DEFENCE:
		{
			OnResPlayerVillageDefenceUp*	pOnResPlayerVillageDefenceUp;

			pOnResPlayerVillageDefenceUp							=	(OnResPlayerVillageDefenceUp*)cLocalBuffer;
			pOnResPlayerVillageDefenceUp->dwMsgType					=	ON_RESPONSE_VILLAGE_DEFENCE;
			pOnResPlayerVillageDefenceUp->dwResponse				=	DWORD(pParam1);
			pOnResPlayerVillageDefenceUp->uiVillageCode				=	UI16(pParam2);
			pOnResPlayerVillageDefenceUp->mnMoney					=	MONEY(pParam3);
			pOnResPlayerVillageDefenceUp->vdDefenceUp				=	VDEF(pParam4);
			pOnResPlayerVillageDefenceUp->siLimitTime				=	SI32(pParam5);
			pOnResPlayerVillageDefenceUp->uiUniqueID				=	UI16(pParam6);

			siLength     =	sizeof(OnResPlayerVillageDefenceUp);
		}
		break;
	case ON_CHANGED_VILLAGE_ATTACK_TYPE:
		{
			OnChangedVillageAttackType*		pOnChangedVillageAttackType;

			pOnChangedVillageAttackType								=	(OnChangedVillageAttackType*)cLocalBuffer;
			pOnChangedVillageAttackType->dwMsgType					=	ON_CHANGED_VILLAGE_ATTACK_TYPE;
			pOnChangedVillageAttackType->uiVillageCode				=	UI16(pParam1);
			pOnChangedVillageAttackType->siAttackType				=	SI16(pParam2);

			siLength	=	sizeof(OnChangedVillageAttackType);

		}
		break;

	case ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT:
		{
			OnResGetSupplyAtGuildUnit*	pOnResGetSupplyAtGuildUnit;

			pOnResGetSupplyAtGuildUnit								=	(OnResGetSupplyAtGuildUnit*)cLocalBuffer;
			pOnResGetSupplyAtGuildUnit->dwMsgType					=	ON_RESPONSE_GET_SUPPLY_AT_GUILD_UNIT;

			pOnResGetSupplyAtGuildUnit->uiUniqueID                  = uiAccount;
			pOnResGetSupplyAtGuildUnit->siHeAmount                  = SI16(pParam1);
			pOnResGetSupplyAtGuildUnit->siNowMP                     = SI16(pParam2);
			pOnResGetSupplyAtGuildUnit->uiTagetID                   = UI16(pParam3);
			pOnResGetSupplyAtGuildUnit->siHP                        = SI16(pParam4);

/*			pOnResGetSupplyAtGuildUnit->uiUniqueID					=	UI16(pParam1);
			pOnResGetSupplyAtGuildUnit->siSupply					=	SI16(pParam2);
			pOnResGetSupplyAtGuildUnit->uiTargetID					=	UI16(pParam3);
			pOnResGetSupplyAtGuildUnit->siSupplyCharge				=	SI16(pParam4);
*/
			siLength	=	sizeof(OnResGetSupplyAtGuildUnit);
		}
		break;
	case ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY:
		{
			OnResLongAttackVillageRdyMsg*	pOnResLongAttackVillageRdyMsg;

			pOnResLongAttackVillageRdyMsg					=	(OnResLongAttackVillageRdyMsg*)cLocalBuffer;
			pOnResLongAttackVillageRdyMsg->dwMsgType		=	ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_READY;

			pOnResLongAttackVillageRdyMsg->dwResponse       =   DWORD(pParam1);
			pOnResLongAttackVillageRdyMsg->uiAttackCharID	=	UI16(pParam2);
			pOnResLongAttackVillageRdyMsg->uiVillageCode	=	UI16(pParam3);
			pOnResLongAttackVillageRdyMsg->uiKind			=	UI16(pParam4);
			pOnResLongAttackVillageRdyMsg->dwTime           =   DWORD(pParam5);
			pOnResLongAttackVillageRdyMsg->bIsLong          =   BYTE(pParam6);

			siLength								=   sizeof(OnResLongAttackVillageRdyMsg);
		}
		break;            

	case ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION:
		{
			OnResLongAttackVillageActionMsg*	pOnResLongAttackVillageActionMsg;
			SOVillage							*pVillage;

			pVillage													=	(SOVillage*)pParam2;

			pOnResLongAttackVillageActionMsg							=	(OnResLongAttackVillageActionMsg*)cLocalBuffer;
			pOnResLongAttackVillageActionMsg->dwMsgType					=	ON_RESPONSE_LONGDISTANCE_VILLAGEATTACK_ACTION;
			pOnResLongAttackVillageActionMsg->uiAttackCharID			=	UI16(pParam1);
			pOnResLongAttackVillageActionMsg->siVillageDefenceDamage	=	SI16(pParam3);
			pOnResLongAttackVillageActionMsg->siDefenceSoldierDamage	=	SI16(pParam4);
			pOnResLongAttackVillageActionMsg->vdDefence					=   pVillage->vdDefence;
//			pOnResLongAttackVillageActionMsg->siDefenceSoldier			=	pVillage->GetDefenceSoldier();
			pOnResLongAttackVillageActionMsg->uiVillageCode				=	pVillage->uiVillageCode;
			pOnResLongAttackVillageActionMsg->siPlayerDamage			=	SI16(pParam5);
			pOnResLongAttackVillageActionMsg->siAttackType				=	SI16(pParam6);

			siLength								=   sizeof(OnResLongAttackVillageActionMsg);

		}
		break;

	case ON_RESPONSE_NORMAL_CHAR:
		{
			OnResNormalChar*		    pOnResNormalChar;
			
			pOnResNormalChar		=	(OnResNormalChar*)cLocalBuffer;
			pOnResNormalChar->dwMsgType	=	ON_RESPONSE_NORMAL_CHAR;
			pOnResNormalChar->uiUniqueID = UI16(pParam2);
			pOnResNormalChar->uiCharCode = UI16(pParam1);

			siLength								=   sizeof(OnResNormalChar);
		}
		break;
	case ON_RESPONSE_FIELDATTACK_READY:
		{
			OnResFieldAttackRdyMsg*		pOnResFieldAttackRdyMsg;

			pOnResFieldAttackRdyMsg					=	(OnResFieldAttackRdyMsg*)cLocalBuffer;
			pOnResFieldAttackRdyMsg->dwMsgType		=	ON_RESPONSE_FIELDATTACK_READY;

			pOnResFieldAttackRdyMsg->dwResponse		=   DWORD(pParam1);
			pOnResFieldAttackRdyMsg->uiAttackCharID	=	UI16(pParam2);
			pOnResFieldAttackRdyMsg->uiEnemyCharID	=	UI16(pParam3);
			pOnResFieldAttackRdyMsg->uiKind			=	UI16(pParam4);
			pOnResFieldAttackRdyMsg->dwTime         =   DWORD(pParam5);
			pOnResFieldAttackRdyMsg->bIsLong        =   BYTE(pParam6);
			
			siLength								=   sizeof(OnResFieldAttackRdyMsg);
		}
		break;

	case ON_RESPONSE_FIELDATTACK_ACTION:
		{
			OnResFieldAttackActionMsg*	pOnResFieldAttackActionMsg;
				
			pOnResFieldAttackActionMsg				=	(OnResFieldAttackActionMsg*)cLocalBuffer;
			pOnResFieldAttackActionMsg->dwMsgType	=	ON_RESPONSE_FIELDATTACK_ACTION;
			pOnResFieldAttackActionMsg->dwResponse	=	DWORD(pParam5);
			pOnResFieldAttackActionMsg->uiShooterID	=	UI16(pParam1);
			pOnResFieldAttackActionMsg->siDamage	=	SI16(pParam2);
			pOnResFieldAttackActionMsg->bAlive		=	BOOL(pParam3);
			pOnResFieldAttackActionMsg->uiEnemyID	=	UI16(pParam4);
			pOnResFieldAttackActionMsg->siAttackType=   SI16(pParam6);

			siLength								=	sizeof(OnResFieldAttackActionMsg);
		}
		break;

	case ON_RESPONSE_FIELDATTACK_DEAD:
		{
			OnResFieldAttackDeadMsg*    pOnResFieldAttackDeadMsg;

			pOnResFieldAttackDeadMsg				=	(OnResFieldAttackDeadMsg*)cLocalBuffer;
			pOnResFieldAttackDeadMsg->dwMsgType		=	ON_RESPONSE_FIELDATTACK_DEAD;
			pOnResFieldAttackDeadMsg->uiDeadCharID	=	UI16(pParam1);
			pOnResFieldAttackDeadMsg->uiShoorterID	=	UI16(pParam2);

			siLength								=	sizeof(OnResFieldAttackDeadMsg);
		}
		break;

	case ON_RESPONSE_ATTACK_VILLAGE:
		{
			OnResAttackVillageMsg		*pAttackVillageMsg;
			SOVillage					*pVillage;

			pVillage									=	(SOVillage*)pParam2;

			pAttackVillageMsg							=	(OnResAttackVillageMsg*)cLocalBuffer;
			pAttackVillageMsg->dwMsgType				=	ON_RESPONSE_ATTACK_VILLAGE;
			pAttackVillageMsg->uiCharID					=	uiAccount;
			pAttackVillageMsg->uiKind					=	UI16(pParam1);
			pAttackVillageMsg->uiVillageCode			=	pVillage->uiVillageCode;
			pAttackVillageMsg->vdDefence				=	pVillage->vdDefence;
			pAttackVillageMsg->siVillageDefenceDamage	=	SI16(pParam3);
			pAttackVillageMsg->siAttackType				=	SI16(pParam4);
			pAttackVillageMsg->bIsLong                  =   BYTE(pParam5);

			siLength								=	sizeof(OnResAttackVillageMsg);
		}
		break;
	case ON_RESPONSE_WAR_VILLAGE_DEFENCE:
		{
			OnResWarVillageDefence*			pOnResWarVillageDefence;

			pOnResWarVillageDefence   		=	(OnResWarVillageDefence*)cLocalBuffer;

			pOnResWarVillageDefence->dwMsgType			=	ON_RESPONSE_WAR_VILLAGE_DEFENCE;
			pOnResWarVillageDefence->dwResponse			=	DWORD(pParam1);
			pOnResWarVillageDefence->siLimitTime		=	SI32(pParam2);
			pOnResWarVillageDefence->uiUniqueID			=	UI16(pParam3);

			siLength								=	sizeof(OnResWarVillageDefence);


		}
		break;
		
		//수성 인터페이스에서 나간다.
	case ON_RESPONSE_VILLAGE_DEFFNCE_STOP:
		{
			OnResWarVillageIn*				pOnResWarVillageIn;

			pOnResWarVillageIn				=	(OnResWarVillageIn*)cLocalBuffer;
			pOnResWarVillageIn->dwMsgType	=	ON_RESPONSE_VILLAGE_DEFFNCE_STOP;
			pOnResWarVillageIn->uiUniqueID	=	uiAccount;
			pOnResWarVillageIn->dwResponse  =   (DWORD)(pParam1);
			siLength						=   sizeof(OnResWarVillageIn);
		}
		break;
		
	case ON_RESPONSE_CHANGE_WAR:
		{
			OnResChangeUnit          *pOnResChangeUnit;

			pOnResChangeUnit = (OnResChangeUnit*)cLocalBuffer;

			pOnResChangeUnit->dwMsgType  = ON_RESPONSE_CHANGE_WAR;
			pOnResChangeUnit->dwResPonse = DWORD(pParam6);
			pOnResChangeUnit->uiUniqueID = uiAccount;
			pOnResChangeUnit->siHP       = SI16(pParam3);
			pOnResChangeUnit->siMP       = SI16(pParam4);
			pOnResChangeUnit->siKind     = SI16(pParam1);
			pOnResChangeUnit->bQuantity  = BYTE(pParam2);
			pOnResChangeUnit->siMoney    = SI32(pParam5);

			siLength					 = sizeof(OnResChangeUnit);
		}
		break;
		
	case ON_RESPONSE_CHARGE_MP:
		{
			OnResChargeMP        *pOnResChargeMP;
			pOnResChargeMP        = (OnResChargeMP*)cLocalBuffer;

			pOnResChargeMP->dwMsgType  = ON_RESPONSE_CHARGE_MP;
			pOnResChargeMP->dwResponse = DWORD(pParam1);
			pOnResChargeMP->siNowMP    = SI16(pParam2);

			siLength                   = sizeof(OnResChargeMP);
			
		}
		break;
		
	case ON_RESPONSE_NOT_WAR:
		{
			OnMsg                 *pOnMsg;
			pOnMsg                = (OnMsg*)cLocalBuffer;

			pOnMsg->dwMsgType     = ON_RESPONSE_NOT_WAR;
			siLength              = sizeof(OnMsg);
		}
		break;

	case ON_RESPONE_WAR_TIME:
		{
			OnMsg                 *pOnMsg;
			pOnMsg                = (OnMsg*)cLocalBuffer;

			pOnMsg->dwMsgType     = ON_RESPONE_WAR_TIME;
			siLength              = sizeof(OnMsg);
		}
		break;
		
	case ON_ASK_GIVEUP:
		{
			OnAskGiveUp                 *pOnAskGiveUp;
			pOnAskGiveUp                = (OnAskGiveUp*)cLocalBuffer;

			pOnAskGiveUp->dwMsgType     = ON_ASK_GIVEUP;
			pOnAskGiveUp->uiVillageCode = UI16(pParam1);
			pOnAskGiveUp->mnMoney       = MONEY(pParam2); 

			siLength                    = sizeof(OnAskGiveUp);
		}
		break;

	case ON_RESPONE_GIVEUP:
		{
			OnResGiveUp           *pOnMsg;
			pOnMsg                = (OnResGiveUp*)cLocalBuffer;

			pOnMsg->dwMsgType     = ON_RESPONE_GIVEUP;
			pOnMsg->dwResponse    = 0;

			siLength              = sizeof(OnResGiveUp);
		}
		break;
/*	case ON_RESPONSE_WAR_VILLAGE_IN:
		{
			OnResWarVillageIn*				pOnResWarVillageIn;
			OnRecvVillageInfo*				pVillageInfo;
			SOVillage*						pVillage	=	NULL;

			pVillage					=	(SOVillage*)pParam1;

			pOnResWarVillageIn				=	(OnResWarVillageIn*)cLocalBuffer;

			pOnResWarVillageIn->dwMsgType	=	ON_RESPONSE_WAR_VILLAGE_IN;
			pOnResWarVillageIn->uiUniqueID	=	uiAccount;

			switch(siSendInfo)
			{
				case SO_SFM_ONLYNEARCHAR:
				{
					// 오직 내 주위에게만 보내는 메시지이다.
					// 마을의 정보는 보낼 필요가 없다.
					siLength						=	sizeof(OnResWarVillageIn);			
				}
				break;
				case SO_SFM_ONLYME:
				{
					pVillageInfo					=	(OnRecvVillageInfo*)((CHAR*)pOnResWarVillageIn + sizeof(OnResWarVillageIn));
					pVillage->GetVillageInfo(this, pVillageInfo);
					siLength						=	sizeof(OnResWarVillageIn) + sizeof(OnRecvVillageInfo);
				}
				break;
			}
		}
		break;
		*/

	case ON_RESPONSE_INCREASE_GUILD_UNIT:
		{
			OnResIncreaseGuildUnit*		pOnResIncreaseGuildUnit;

			pOnResIncreaseGuildUnit					=	(OnResIncreaseGuildUnit*)cLocalBuffer;
			pOnResIncreaseGuildUnit->dwMsgType		=	ON_RESPONSE_INCREASE_GUILD_UNIT;
			pOnResIncreaseGuildUnit->siKind			=	SI16(pParam1);
			pOnResIncreaseGuildUnit->siTotalCount	=	SI16(pParam2);
			pOnResIncreaseGuildUnit->siSupply		=	SI16(pParam3);
			pOnResIncreaseGuildUnit->mnMoney		=	MONEY(pParam4);
			pOnResIncreaseGuildUnit->dwResponse		=	DWORD(pParam5);

			siLength								=	sizeof(OnResIncreaseGuildUnit);

		}
		break;
	case ON_RESPONSE_DECREASE_GUILD_UNIT:
		{
			OnResDecreaseGuildUnit*		pOnResDecreaseGuildUnit;
			MyItemData*					pAddItemData = NULL;

			pOnResDecreaseGuildUnit					=	(OnResDecreaseGuildUnit*)cLocalBuffer;
			pOnResDecreaseGuildUnit->dwMsgType		=	ON_RESPONSE_DECREASE_GUILD_UNIT;
			pOnResDecreaseGuildUnit->dwResponse		=	DWORD(pParam1);


			switch(pOnResDecreaseGuildUnit->dwResponse)
			{
			case ON_RET_OK:
				pOnResDecreaseGuildUnit->siKind			=	DWORD(pParam2);
				pOnResDecreaseGuildUnit->siTotalCount	=	DWORD(pParam3);

				pAddItemData							=   (MyItemData*)(pParam4);
				pOnResDecreaseGuildUnit->siMercenarySlot=   SI16(pParam5);
				pOnResDecreaseGuildUnit->siInventoryPos	=	pAddItemData->siPosInInventory;
				pOnResDecreaseGuildUnit->siItemID		=	pAddItemData->uiID;
				pOnResDecreaseGuildUnit->siQuantity		=	pAddItemData->uiQuantity;

				siLength								=	sizeof(OnResDecreaseGuildUnit);
					
				break;
			case ON_RET_NO:
				siLength								=	sizeof(OnResDecreaseGuildUnit);
				break;
			}
		}
		break;
	case ON_RESPONSE_LIST_GUILD_UNIT:
		{
			OnResGetGuildUnitList*				 pOnResGetGuildUnitList;
			cltGuild*							 pclGuild;

			pclGuild							=	(cltGuild*)pParam1;

			pOnResGetGuildUnitList				=	(OnResGetGuildUnitList*)cLocalBuffer;
			pOnResGetGuildUnitList->dwMsgType	=	ON_RESPONSE_LIST_GUILD_UNIT;

			for(SI16 i = 0 ; i < ON_MAX_GUILD_UNIT; i++)
				pOnResGetGuildUnitList->GuildCount[i] = pclGuild->m_GuildUnit.GetGuildUnit(i);

			pOnResGetGuildUnitList->uiPopulation =   (UI32)pParam2;

			siLength								=	sizeof(OnResGetGuildUnitList);
		}
		break;
	case ON_RESPONSE_INCREASE_GUILD_SUPPLY:
		{
			OnResIncreaseGuildSupply*	pOnResIncreaseGuildSupply;
				
			pOnResIncreaseGuildSupply				=	(OnResIncreaseGuildSupply*)cLocalBuffer;
			pOnResIncreaseGuildSupply->dwMsgType	=	ON_RESPONSE_INCREASE_GUILD_SUPPLY;
			pOnResIncreaseGuildSupply->dwResponse	=	DWORD(pParam1);

			switch(pOnResIncreaseGuildSupply->dwResponse)
			{
			case ON_RET_OK:
				pOnResIncreaseGuildSupply->uiMoney			=	MONEY(pParam2);
				pOnResIncreaseGuildSupply->siSupplyQuantity	=	SI32(pParam3);

				siLength								=	sizeof(OnResIncreaseGuildSupply);
				break;
			case ON_RET_NO:
				siLength								=	sizeof(OnResIncreaseGuildSupply);
				break;
			}
		}
		break;
	case ON_RESPONSE_DECREASE_GUILD_SUPPLY:
		{
			OnResDecreaseGuildSupply*	pOnResDecreaseGuildSupply;
			pOnResDecreaseGuildSupply				=	(OnResDecreaseGuildSupply*)cLocalBuffer;
			pOnResDecreaseGuildSupply->dwMsgType	=	ON_RESPONSE_DECREASE_GUILD_SUPPLY;
			pOnResDecreaseGuildSupply->dwResponse	=	DWORD(pParam1);

			switch(pOnResDecreaseGuildSupply->dwResponse)
			{
			case ON_RET_OK:
				pOnResDecreaseGuildSupply->siSupplyQuantity	=	SI32(pParam2);	
				siLength								=	sizeof(OnResDecreaseGuildSupply);
				break;
			case ON_RET_NO:
				siLength								=	sizeof(OnResDecreaseGuildSupply);
				break;
			}
		}
		break;
	case ON_RESPONSE_LIST_GUILD_SUPPLY:
		{
			OnResGetGuildSupplyList*	pOnResGetGuildSupplyList;

			pOnResGetGuildSupplyList				=	(OnResGetGuildSupplyList*)cLocalBuffer;
			pOnResGetGuildSupplyList->dwMsgType		=	ON_RESPONSE_LIST_GUILD_SUPPLY;
			pOnResGetGuildSupplyList->siGuildSupply	=   SI32(pParam2);

			siLength								=	sizeof(OnResGetGuildSupplyList);

		}
		
		break;
	case ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER:
		{
			OnResVillageDefenceSoldier* pOnResVillageDefenceSoldier;
			
			pOnResVillageDefenceSoldier						= (OnResVillageDefenceSoldier*)cLocalBuffer;
			pOnResVillageDefenceSoldier->dwMsgType			= ON_RESPONSE_INCREASE_VILLAGE_DEFENCE_SOLDIER;
			pOnResVillageDefenceSoldier->dwResponse			= DWORD(pParam1);
			pOnResVillageDefenceSoldier->siDefenceSoldier	= SI16(pParam2);
			pOnResVillageDefenceSoldier->siSupply			= SI32(pParam3);	
			pOnResVillageDefenceSoldier->mnMoney			= MONEY(pParam4);

			siLength										= sizeof(OnResVillageDefenceSoldier);
		}
		break;
	case ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER:
		{
			OnResGetVillageDefenceSoldier* pOnResGetVillageDefenceSoldier;
			pOnResGetVillageDefenceSoldier					= (OnResGetVillageDefenceSoldier*)cLocalBuffer;
			pOnResGetVillageDefenceSoldier->dwMsgType		= ON_RESPONSE_GET_VILLAGE_DEFENCE_SOLDIER;
			pOnResGetVillageDefenceSoldier->siDefenceSoldier= SI32(pParam1);	

			siLength										= sizeof(OnResGetVillageDefenceSoldier);

		}
		break;
	case ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT:
		{
			OnResVillageAttackResult*		pOnResVillageAttackResult;
			OnGuildResult*					pGuildResult;
			cltGuildResult*					pcltGuildResult;
			SI16							i = 0 ,j = 0;

			pOnResVillageAttackResult				    	= (OnResVillageAttackResult*)cLocalBuffer;
			pOnResVillageAttackResult->dwMsgType			= ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT;
			pOnResVillageAttackResult->dwResponse			= DWORD(pParam1);
			pOnResVillageAttackResult->siVillageCount		= SI16(pParam2);
			pcltGuildResult									= (cltGuildResult*)pParam3;
			

			//pGuildResult									= (OnGuildResult*)((CHAR*)pOnResVillageAttackResult + sizeof(OnResVillageAttackResult));
			//CopyMemory(pGuildResult, pParam3, sizeof(OnGuildResult) * pOnResVillageAttackResult->siVillageCount);

			
			switch(pOnResVillageAttackResult->dwResponse)
			{
				case ON_RET_OK:
					pGuildResult = (OnGuildResult*)((CHAR*)pOnResVillageAttackResult + sizeof(OnResVillageAttackResult));
					ZeroMemory(pGuildResult,sizeof(OnGuildResult) * pOnResVillageAttackResult->siVillageCount);
					for(i = 0;i < ON_MAX_GUILD_RESULT;i++)
					{
						if(pcltGuildResult[i].GetVillageCode() != 0)
						{
							if(pcltGuildResult[i].szCharName[0] == NULL)
							{
								pGuildResult[j].siVillageCode = pcltGuildResult[i].GetVillageCode();
								pGuildResult[j].siPercent	  = pcltGuildResult[i].GetPercent();
								j++;
							}
						}
					}
					//CopyMemory(pGuildResult, pParam3, sizeof(OnGuildResult) * pOnResVillageAttackResult->siVillageCount);
					siLength	 =	sizeof(OnResVillageAttackResult) + (sizeof(OnGuildResult) * pOnResVillageAttackResult->siVillageCount);
					break;
				case ON_RET_NO:
					siLength	 =	sizeof(OnResVillageAttackResult);
					break;
			}
		}
		break;	
	case ON_RESPONSE_GUILD_CHOICE_PLAYER:
		{
			OnResGuildChoicePlayer*			pOnResGuildChoicePlayer;

			pOnResGuildChoicePlayer							=	(OnResGuildChoicePlayer*)cLocalBuffer;
			pOnResGuildChoicePlayer->dwMsgType				=	ON_RESPONSE_GUILD_CHOICE_PLAYER;
			pOnResGuildChoicePlayer->dwResponse				=	DWORD(pParam1);
			pOnResGuildChoicePlayer->siVillageCode			=	SI16(pParam2);
			strcpy(pOnResGuildChoicePlayer->szUserName,(char*)pParam3);

			siLength										=	sizeof(OnResGuildChoicePlayer);
		}
		break;
	case ON_RESPONSE_VILLAGE_INVESTMENT_POWER:
		{
			OnResInvestmentPower*			pOnResInvestmentPower;
		
			pOnResInvestmentPower				=	(OnResInvestmentPower*)cLocalBuffer;
			pOnResInvestmentPower->dwMsgType	=	ON_RESPONSE_VILLAGE_INVESTMENT_POWER;
			pOnResInvestmentPower->dwResponse	=	DWORD(pParam1);
			pOnResInvestmentPower->siPercent	=	SI16(pParam2);

			siLength							=	sizeof(OnResInvestmentPower);
		}
		break;
	case ON_GETOUTSHIP:
		{
			OnGetOutShipMsg*		pGetOutShipMsg;
			SOVillage*				pVillage;

			// 배에서 내린다.
			pGetOutShipMsg	=	(OnGetOutShipMsg*)cLocalBuffer;

			pVillage		=	(SOVillage*)pParam1;

			pGetOutShipMsg->dwMsgType			=	ON_GETOUTSHIP;
			pGetOutShipMsg->uiVillageCode		=	pVillage->uiVillageCode;
			pVillage->GetVillageInfo(this, &pGetOutShipMsg->VillageInfo);
			
			siLength							=	sizeof(OnGetOutShipMsg);
		}
		break;
	case ON_RESPONSE_CHANGE_GUILD_FLAG:
		{
			OnResGuildChangeFlag	*pGuildChangeFlag;

			// 깃발을 변경한다.
			pGuildChangeFlag	=	(OnResGuildChangeFlag*)cLocalBuffer;

			pGuildChangeFlag->dwMsgType		=	ON_RESPONSE_CHANGE_GUILD_FLAG;
			pGuildChangeFlag->dwResponse	=	DWORD(pParam1);
			pGuildChangeFlag->siFlag		=	SI32(pParam2);

			siLength						=	sizeof(OnResGuildChangeFlag);
		}
		break;
	case ON_RESPONSE_USED_FLAG_LIST:
		{
			OnResUsedFlagList		*pUsedFlagList;
			strFlagInfo				*pstFlagInfo;
			UI32					uiGuildID;

			pUsedFlagList		=	(OnResUsedFlagList*)cLocalBuffer;
			pstFlagInfo			=	(strFlagInfo*)((CHAR*)pUsedFlagList + sizeof(OnResUsedFlagList));

			uiGuildID						=	UI32(pParam1);

			pUsedFlagList->dwMsgType		=	ON_RESPONSE_USED_FLAG_LIST;
			pUsedFlagList->uiGuildMemberNum	=	pMyWorld->clIGuild.GetGuildMemberNum(uiGuildID);

			

			pMyWorld->clIGuild.GetUsedFlagList(&pUsedFlagList->siUsedFlagNum, pstFlagInfo);

			siLength			=	sizeof(OnResUsedFlagList) + (pUsedFlagList->siUsedFlagNum * sizeof(strFlagInfo));
		}
		break;
	case ON_CHANGED_VILLAGE_DEFENCE:
		{
			OnChangeVillageDefenceMsg	*pChangeVillageDefenceMsg;

			pChangeVillageDefenceMsg	=	(OnChangeVillageDefenceMsg*)cLocalBuffer;
			
			pChangeVillageDefenceMsg->dwMsgType			=	ON_CHANGED_VILLAGE_DEFENCE;
			pChangeVillageDefenceMsg->uiVillageCode		=	UI16(pParam1);			
			pChangeVillageDefenceMsg->vdChangedDefence	=	*((VDEF*)pParam2);
			
			siLength		=	sizeof(OnChangeVillageDefenceMsg);

			// 메시지를 받을 사람을 정의한다.
			siSendCharNum	=	SI32(pParam3);
			memcpy(uiCharUniIDs, pParam4, sizeof(UI16) * siSendCharNum);
			
			// 나도 이 메시지를 받아야 한다.
			bSendMe			=	TRUE;
		}
		break;
	case ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT:
		{
			// pParam1 : 성공여부
			// pParam2 : 현재 방어력
			// pParam3 : 최대 방어력
			// pParam4 : 방어력 당 Money
			// pParam5 : 최대 Money
			OnResMaxVillageDefenceInvestment*	pOnResMaxVillageDefenceInvestment;

			pOnResMaxVillageDefenceInvestment = (OnResMaxVillageDefenceInvestment*)cLocalBuffer;

			pOnResMaxVillageDefenceInvestment->dwMsgType   = ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT;
			pOnResMaxVillageDefenceInvestment->dwResponse  = DWORD(pParam1);
			pOnResMaxVillageDefenceInvestment->NowDef      = *((VDEF*)pParam2);
			pOnResMaxVillageDefenceInvestment->MaxDef      = *((VDEF*)pParam3);
			pOnResMaxVillageDefenceInvestment->DefPerMoney = *((MONEY*)pParam4);
			pOnResMaxVillageDefenceInvestment->MaxMoney    = *((MONEY*)pParam5);
			
			siLength		=	sizeof(OnResMaxVillageDefenceInvestment);
		}
		break;
	case ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT:
		{
			// pParam1 : 성공여부
			// pParam2 : 현재 방어력
			OnResSetVillageDefenceInvestment*		lpOnResSetVillageDefenceInvestment;

			lpOnResSetVillageDefenceInvestment = (OnResSetVillageDefenceInvestment*)cLocalBuffer;

			lpOnResSetVillageDefenceInvestment->dwMsgType   = ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT;
			lpOnResSetVillageDefenceInvestment->dwResponse  = DWORD(pParam1);
			if(pParam2) lpOnResSetVillageDefenceInvestment->NowDef      = *((VDEF*)pParam2);
			
			siLength		=	sizeof(OnResSetVillageDefenceInvestment);
		}
		break;
	case ON_RESPONSE_PI_INMINIMAP:
		{
			OnResponseMsg			*pResponseMsg;
			OnResPIInMiniMapOFMsg	*pPIInMiniMapOFMsg;
			OnResPIInMiniMapIVMsg	*pPIInMiniMapIVMsg;
			SOPlayer				*pPlayer;
			SOVillage				*pVillage;

			pResponseMsg	=	(OnResponseMsg*)cLocalBuffer;

			pResponseMsg->dwMsgType		=	ON_RESPONSE_PI_INMINIMAP;
			pResponseMsg->dwResponse	=	DWORD(pParam1);

			pPlayer						=	(SOPlayer*)pParam2;
			pVillage					=	(SOVillage*)pParam3;

			switch(pResponseMsg->dwResponse)
			{
			case ON_RET_ON_FIELD:
			case ON_RET_IN_BATTLE:
				pPIInMiniMapOFMsg			=	(OnResPIInMiniMapOFMsg*)pResponseMsg;

				pPIInMiniMapOFMsg->uiKind	=	pPlayer->uiKind;
				pPIInMiniMapOFMsg->uiMapID	=	pPlayer->uiMapID;
				pPIInMiniMapOFMsg->uiX		=	UI16(pPlayer->siX);
				pPIInMiniMapOFMsg->uiY		=	UI16(pPlayer->siY);

				siLength					=	sizeof(OnResPIInMiniMapOFMsg);
				break;
			case ON_RET_IN_VILLAGE:
				pPIInMiniMapIVMsg			=	(OnResPIInMiniMapIVMsg*)pResponseMsg;

				pPIInMiniMapIVMsg->uiKind			=	pPlayer->uiKind;
				pPIInMiniMapIVMsg->uiVillageCode	=	pVillage->uiVillageCode;

				siLength					=	sizeof(OnResPIInMiniMapIVMsg);
				break;			
			default:
				siLength					=	sizeof(OnResponseMsg);
				break;
			}
		}
		break;
	case ON_RESPONSE_GUILD_DETAIL_INFO:
		{
			OnResGuildDetailInfoMsg		*pGuildDetailInfoMsg;
			cltGuild					*pclGuild;						
			OnGuildVillageInfo			*pGuildVillageInfo;


			pGuildDetailInfoMsg				=	( OnResGuildDetailInfoMsg* )cLocalBuffer;			

			pGuildDetailInfoMsg->dwMsgType			=	ON_RESPONSE_GUILD_DETAIL_INFO;
			pGuildDetailInfoMsg->dwResponse			=	DWORD( pParam1 );
			pclGuild								=	( cltGuild* )pParam2;

			switch( pGuildDetailInfoMsg->dwResponse )
			{
			case ON_RET_OK:
				// puiVillageCode의 포인터를 설정한다.
				pGuildVillageInfo					=	( OnGuildVillageInfo* )( ( CHAR* )pGuildDetailInfoMsg + sizeof( OnResGuildDetailInfoMsg ) );	
				
				pclGuild->GetOwnerGuildVillageInfo( &pGuildDetailInfoMsg->uiOwnVillageNum, pGuildVillageInfo );

				siLength						=	sizeof( OnResGuildDetailInfoMsg ) + ( sizeof( OnGuildVillageInfo ) * pGuildDetailInfoMsg->uiOwnVillageNum );	
				break;
			case ON_RET_NO:
				siLength						=	sizeof( OnResGuildDetailInfoMsg );	
				break;
			}		
		}
		break;	
	case ON_RESPONSE_PM_OPEN:	
		{
			OnResPMOpenMsg		*pPMOpenMsg;

			pPMOpenMsg				=	(OnResPMOpenMsg*)cLocalBuffer;
			pPMOpenMsg->dwMsgType	=	ON_RESPONSE_PM_OPEN;
			pPMOpenMsg->dwResponse	=	DWORD(pParam1);
			pPMOpenMsg->uiAccount	=	uiAccount;

			siLength				=	sizeof(OnResPMOpenMsg);
		}
		break;
	case ON_RESPONSE_PM_CLOSE:	
		{
			OnResPMCloseMsg		*pPMCloseMsg;

			pPMCloseMsg				=	(OnResPMCloseMsg*)cLocalBuffer;
			pPMCloseMsg->dwMsgType	=	ON_RESPONSE_PM_CLOSE;
			pPMCloseMsg->dwResponse	=	DWORD(pParam1);
			pPMCloseMsg->uiAccount	=	uiAccount;

			siLength				=	sizeof(OnResPMCloseMsg);
		}
		break;
	case ON_PM_CHANGE_BOOTH_ITEM:
		{
			OnChangeBoothItemMsg	*pChangeBoothItemMsg;

			pChangeBoothItemMsg								=	(OnChangeBoothItemMsg*)cLocalBuffer;
			pChangeBoothItemMsg->dwMsgType					=	ON_PM_CHANGE_BOOTH_ITEM;
			pChangeBoothItemMsg->uiAccount					=	UI16(pParam1);
			pChangeBoothItemMsg->BaseBoothItem.uiBoothSlot	=	UI08(pParam2);
			pChangeBoothItemMsg->BaseBoothItem.uiProductID		=	UI16(pParam3);
			pChangeBoothItemMsg->BaseBoothItem.siType		=	SI08(pParam4);

			siLength	=	sizeof(OnChangeBoothItemMsg);
		}
		break;
	case ON_RESPONSE_PM_CHANGEITEM:
		{
			OnResChangeBoothItemMsg		*pChangeBoothItemMsg;

			pChangeBoothItemMsg				=	(OnResChangeBoothItemMsg*)cLocalBuffer;
			pChangeBoothItemMsg->dwMsgType	=	ON_RESPONSE_PM_CHANGEITEM;
			pChangeBoothItemMsg->dwResponse	=	DWORD(pParam1);

			siLength	=	sizeof(OnResChangeBoothItemMsg);
		}
		break;
	case ON_RESPONSE_PM_ITEMINFO:
		{
			OnResPMItemInfoMsg			*pPMItemInfoMsg;
			OnBoothItemInfo				*pBoothItemInfo;
			SI08						siItemNum;
			SOPlayer					*pPrivateMarketPlayer;

			pPMItemInfoMsg				=	(OnResPMItemInfoMsg*)cLocalBuffer;
			pPMItemInfoMsg->dwMsgType	=	ON_RESPONSE_PM_ITEMINFO;
			pPMItemInfoMsg->dwResponse	=	DWORD(pParam1);
			pPMItemInfoMsg->uiItemNum	=	0;			
			CHAR						cFollowerItemBuf[8192];			// 모든 용병들의 소지 아이템 정보가 BoothItemInfo 뒤에 따라붙게 된다.
			MyItemData					*pFollowerItems;
			UI16						numFollowerItems	=	0;
			SI32						siBoothInfoSize;

			pPrivateMarketPlayer		=	(SOPlayer*)pParam2;

			switch(pPMItemInfoMsg->dwResponse)
			{
			case ON_RET_OK:
				pBoothItemInfo			=	(OnBoothItemInfo*)(((CHAR*)pPMItemInfoMsg) + sizeof(OnResPMItemInfoMsg));
				siBoothInfoSize			=	pPrivateMarketPlayer->clPrivateMarket.GetItems( (OnBoothItemInfo*)pBoothItemInfo,
																							&siItemNum,
																							pPrivateMarketPlayer,
																							(MyItemData*)cFollowerItemBuf,
																							numFollowerItems );
				pFollowerItems		=	(MyItemData*)( (CHAR*)pPMItemInfoMsg + sizeof(OnResPMItemInfoMsg) + siBoothInfoSize );
				memcpy( pFollowerItems, cFollowerItemBuf, sizeof(MyItemData) * numFollowerItems );		// 용병들의 소지 아이템 정보 채우기

				siLength			=	sizeof(OnResPMItemInfoMsg) + siBoothInfoSize + sizeof(MyItemData) * numFollowerItems;
				
				pPMItemInfoMsg->uiItemNum	=	siItemNum;
				break;
			case ON_RET_NO:
				siLength				=	sizeof(OnResPMItemInfoMsg);
				break;
			}			
		}
		break;
	case ON_RESPONSE_PM_BUYITEM:
		{
			OnResPMBuyItemMsg	*pPMBuyItemMsg;

			pPMBuyItemMsg				=	(OnResPMBuyItemMsg*)cLocalBuffer;
			pPMBuyItemMsg->dwMsgType	=	ON_RESPONSE_PM_BUYITEM;			
			pPMBuyItemMsg->dwResponse	=	DWORD(pParam1);						
			pPMBuyItemMsg->uiBoothSlot	=	UI08(pParam2);
			pPMBuyItemMsg->uiQuantity	=	UI16(pParam3);			
			pPMBuyItemMsg->NewMoney		=	siMoney;

			siLength					=	sizeof(OnResPMBuyItemMsg);
		}
		break;

	case ON_RESPONSE_PM_BUYFOLLOWER:
		{
			OnCompleteTradeMsg					*pCompleteTradeMsg;
			OnFollowerInfo						*pFollowerInfoIn, *pFollowerInfoOut;
			DWORD								dwFollowerDataSize;				

			pCompleteTradeMsg										=	( OnCompleteTradeMsg* )cLocalBuffer;
			pCompleteTradeMsg->dwMsgType							=	ON_RESPONSE_PM_BUYFOLLOWER;									

			dwFollowerDataSize										=	DWORD( pParam1 );

			if ( dwFollowerDataSize == 10) 
			{
				pCompleteTradeMsg->uiTradeFollowerNum = 0;
				siLength			=	sizeof( OnCompleteTradeMsg );
			}
			else
			{
				pFollowerInfoIn											=	( OnFollowerInfo* )pParam2;
				pCompleteTradeMsg->uiTradeFollowerNum					=	UI16( pParam3 );

				pFollowerInfoOut										=	( OnFollowerInfo* )( ( CHAR* )pCompleteTradeMsg + sizeof( OnCompleteTradeMsg ) );
				memcpy( pFollowerInfoOut, pFollowerInfoIn, dwFollowerDataSize );

				siLength												=	sizeof( OnCompleteTradeMsg ) + dwFollowerDataSize;
			}
			break;			
		}

	case ON_RESPONSE_ACCEPT_TRADE:
		{		
			OnResAcceptTradeMsg		*pAcceptTradeMsg;
			OnResAcceptTradeOkMsg	*pAcceptTradeOkMsg;

			pAcceptTradeMsg					=	(OnResAcceptTradeMsg*)cLocalBuffer;
			pAcceptTradeMsg->dwMsgType		=	ON_RESPONSE_ACCEPT_TRADE;
			pAcceptTradeMsg->dwResponse		=	DWORD(pParam1);

			// Ok 일 경우에만 IP를 가지고 온다.
			switch( pAcceptTradeMsg->dwResponse )
			{
			case ON_RET_OK:
				pAcceptTradeOkMsg					=	( OnResAcceptTradeOkMsg* )cLocalBuffer;	
				
				// 상단 레벨
				pAcceptTradeOkMsg->siOppLevel		=	SI32( pParam2 );

				// 상단 이름.
				ZeroMemory( pAcceptTradeOkMsg->szOppGuildID, sizeof( pAcceptTradeOkMsg->szOppGuildID ) );
				strncpy( pAcceptTradeOkMsg->szOppGuildID, ( CHAR* )pParam3, ON_GUILDNAME_LENGTH );
			
				// 상단 직급.
				pAcceptTradeOkMsg->siOppGuildClass	=	SI32( pParam4 );

				
				siLength						=	sizeof( OnResAcceptTradeOkMsg );
				break;

			default:
				siLength						=	sizeof( OnResAcceptTradeMsg );
				break;
			}
			
			
		}		
		break;
	case ON_RESPONSE_CANCEL_TRADE:
		{
			OnResCancelTradeMsg					*pCancelTradeMsg;
			pCancelTradeMsg					=	(OnResCancelTradeMsg*)	cLocalBuffer;
			pCancelTradeMsg->dwMsgType		=	ON_RESPONSE_CANCEL_TRADE;
			pCancelTradeMsg->dwResponse		=	DWORD(pParam1);

			siLength	=	sizeof(OnResCancelTradeMsg);
		}
		break;
	case ON_RESPONSE_FINISH_CHOOSE_TRADEITEM:
		{
			OnResFinishChooseTradeItemMsg		*pFinishChooseTradeItemMsg;

			pFinishChooseTradeItemMsg							=	(OnResFinishChooseTradeItemMsg*)cLocalBuffer;
			pFinishChooseTradeItemMsg->dwMsgType				=	ON_RESPONSE_FINISH_CHOOSE_TRADEITEM;
			pFinishChooseTradeItemMsg->dwResponse				=	DWORD(pParam1);
			pFinishChooseTradeItemMsg->bIsMe					=	UI08(pParam2);
			pFinishChooseTradeItemMsg->bFinishChooseItem		=	UI08(pParam3);

			siLength	=	sizeof(OnResFinishChooseTradeItemMsg);
		}
		break;
	case ON_RESPONSE_DECIDE_TRADE:
		{
			OnResDecideTradeMsg					*pDecideTradeMsg;

			pDecideTradeMsg										=	(OnResDecideTradeMsg*)cLocalBuffer;
			pDecideTradeMsg->dwMsgType							=	ON_RESPONSE_DECIDE_TRADE;
			pDecideTradeMsg->dwResponse							=	DWORD(pParam1);			
			pDecideTradeMsg->bIsMe								=   BOOL(pParam2);
			siLength	=	sizeof(OnResDecideTradeMsg);			
		}
		break;
	case ON_RESPONSE_CHANGE_TRADE_GOODS:
		{		
			OnResChangeTradeGoods				*pChangeTradeGoods;

			pChangeTradeGoods									=	(OnResChangeTradeGoods*)cLocalBuffer;
			pChangeTradeGoods->dwMsgType						=	ON_RESPONSE_CHANGE_TRADE_GOODS;
			pChangeTradeGoods->dwResponse						=	DWORD(pParam1);

			siLength	=	sizeof(OnResChangeTradeGoods);
		}
		break;
	case ON_CHANGE_TRADE_GOODS:
		{
			OnChangeTradeGoods					*pChangeTradeGoods;
			TradeGoodsKind						*pTempTradeGoodsKind;			
			OnFollowerInfo						*pTradeFollowerInfoOut, *pTradeFollowerInfoIn;
			OnIndustrialComplexTradeInfo		*pIndustrialComplexTradeInfoOut, *pIndustrialComplexTradeInfoIn;
			OnReqChangeTradeGoods				*pReqGoods	=	(OnReqChangeTradeGoods*)pParam1;

			pChangeTradeGoods									=	(OnChangeTradeGoods*)cLocalBuffer;
			pChangeTradeGoods->dwMsgType						=	ON_CHANGE_TRADE_GOODS;
			pChangeTradeGoods->uiSlot							=	pReqGoods->uiSlot;

			pTempTradeGoodsKind									=	(TradeGoodsKind*)&pReqGoods->Kind;
			pChangeTradeGoods->Kind								=	*pTempTradeGoodsKind;

			pChangeTradeGoods->lParam1							=	LPARAM(pReqGoods->lParam1);
			pChangeTradeGoods->lParam2							=	LPARAM(pReqGoods->lParam2);

			pTradeFollowerInfoIn								=	(OnFollowerInfo*)pParam2;
			pIndustrialComplexTradeInfoIn						=	(OnIndustrialComplexTradeInfo*)pParam4;

			switch( pChangeTradeGoods->Kind )
			{
			case TRADE_GOODS_FOLLOWER:
				pTradeFollowerInfoOut				=	(OnFollowerInfo*)((CHAR*)pChangeTradeGoods + sizeof(OnChangeTradeGoods));		
				memcpy(pTradeFollowerInfoOut, pTradeFollowerInfoIn, (SI32)pParam3);		// pParam3: dwFollowerDataSize
				
				siLength	=	sizeof(OnChangeTradeGoods) + (SI32)pParam3;

				break;

			case TRADE_GOODS_REALESTATE:
				pIndustrialComplexTradeInfoOut		=	( OnIndustrialComplexTradeInfo* )((CHAR*)pChangeTradeGoods + sizeof(OnChangeTradeGoods));		
				memcpy( pIndustrialComplexTradeInfoOut, pIndustrialComplexTradeInfoIn, sizeof( OnIndustrialComplexTradeInfo ) );

				siLength	=	sizeof( OnChangeTradeGoods ) + sizeof( OnIndustrialComplexTradeInfo );

				break;

			default:
				siLength	=	sizeof(OnChangeTradeGoods);
				break;
			}			
		}
		break;
	case ON_COMPLETE_TRADE:
		{
			
			OnCompleteTradeMsg					*pCompleteTradeMsg;
			OnFollowerInfo						*pFollowerInfoIn, *pFollowerInfoOut;
			DWORD								dwFollowerDataSize;			
			UI16								*puiTradeItemIn, *puiTradeItemOut;
			DWORD								dwTradeItemDataSize;

			pCompleteTradeMsg										=	( OnCompleteTradeMsg* )cLocalBuffer;
			pCompleteTradeMsg->dwMsgType							=	ON_COMPLETE_TRADE;									

			dwFollowerDataSize										=	DWORD( pParam1 );
			pFollowerInfoIn											=	( OnFollowerInfo* )pParam2;
			pCompleteTradeMsg->uiTradeFollowerNum					=	UI16( pParam3 );			

			dwTradeItemDataSize										=	DWORD( pParam4 );
			puiTradeItemIn											=	( UI16* )pParam5;
			pCompleteTradeMsg->uiTradeItemNum						=	UI16( pParam6 );			
		
			pFollowerInfoOut										=	( OnFollowerInfo* )( ( CHAR* )pCompleteTradeMsg + sizeof( OnCompleteTradeMsg ) );
			memcpy( pFollowerInfoOut, pFollowerInfoIn, dwFollowerDataSize );				

			puiTradeItemOut											=	( UI16* )( ( CHAR* )pFollowerInfoOut + dwFollowerDataSize );
			memcpy( puiTradeItemOut, puiTradeItemIn, dwTradeItemDataSize );							

			siLength												=	sizeof( OnCompleteTradeMsg ) + dwFollowerDataSize + dwTradeItemDataSize;
			break;			
		}
		break;
	case ON_RESPONSE_QUEST_SELECTNPC:
		{
			// pParam1 : 상태
			// pParam2 : Quest Code
			// pParam3 : End Quest시 Quest 댓가
			OnResQuestSelectNPC*			lpOnResQuestSelectNPC;
			OnResQuestSelectNPC_EndQuest*	lpOnResQuestSelectNPC_EndQuest;
			BYTE*							lpBuffer;
			MONEY*							lpDelMoney;

			lpOnResQuestSelectNPC = (OnResQuestSelectNPC*)cLocalBuffer;
			lpOnResQuestSelectNPC->dwMsgType   = ON_RESPONSE_QUEST_SELECTNPC;
			lpOnResQuestSelectNPC->dwResponse  = DWORD(pParam1);
			lpOnResQuestSelectNPC->m_QuestCode = SI16(pParam2);
			siLength	=	sizeof(OnResQuestSelectNPC);
			switch(DWORD(pParam1))
			{
				case ON_RET_QUEST_SELECTNPC_ENDQUEST:
					lpOnResQuestSelectNPC_EndQuest = (OnResQuestSelectNPC_EndQuest*)cLocalBuffer;
					lpBuffer                       = (BYTE*)lpOnResQuestSelectNPC_EndQuest + sizeof(OnResQuestSelectNPC_EndQuest);
					CopyMemory(lpOnResQuestSelectNPC_EndQuest->m_QuestPay, pParam3, sizeof(SQuestPayData) * 5);
					siLength	=	sizeof(OnResQuestSelectNPC_EndQuest);

					// 지워지는 돈정보
					lpDelMoney = (MONEY*)pParam6;
					lpOnResQuestSelectNPC_EndQuest->m_DeleteMoney = *lpDelMoney;

					// 지워지는 아이템 정보들
					lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount = SI16(pParam4);
					if(lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount)
					{
						CopyMemory(lpBuffer, pParam5, sizeof(MercenaryItemData) * lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount);
						siLength	+=	sizeof(MercenaryItemData) * lpOnResQuestSelectNPC_EndQuest->m_DeleteItemCount;
					}

					break;
			}
		}
		break;
	case ON_RESPONSE_QUEST_ACCEPTQUEST:
		{
			// pParam1 : 상태
			// pParam2 : Quest Code
			OnResQuestAcceptQuest*				lpOnResQuestAcceptQuest;

			lpOnResQuestAcceptQuest = (OnResQuestAcceptQuest*)cLocalBuffer;
			lpOnResQuestAcceptQuest->dwMsgType   = ON_RESPONSE_QUEST_ACCEPTQUEST;
			lpOnResQuestAcceptQuest->dwResponse  = DWORD(pParam1);
			lpOnResQuestAcceptQuest->m_QuestCode = SI16(pParam2);
			siLength	=	sizeof(OnResQuestAcceptQuest);
		}
		break;
	case ON_RESPONSE_QUEST_CANCELQUEST:
		{
			// pParam1 : 상태
			// pParam2 : Quest Code
			OnResQuestCancelQuest*				lpOnResQuestCancelQuest;

			lpOnResQuestCancelQuest = (OnResQuestCancelQuest*)cLocalBuffer;
			lpOnResQuestCancelQuest->dwMsgType   = ON_RESPONSE_QUEST_CANCELQUEST;
			lpOnResQuestCancelQuest->dwResponse  = DWORD(pParam1);
			lpOnResQuestCancelQuest->m_QuestCode = SI16(pParam2);
			siLength	=	sizeof(OnResQuestCancelQuest);
		}
		break;
	case ON_RESPONSE_QUEST_GETPLAYQUEST:
		{
			// pParam2 : Playing Quest Count
			// pParam3 : Completed Quest Count
			// pParam4 : Playing Quest Data
			// pParam5 : Completed Quest Data
			OnResQuestGetPlayQuest*				lpOnResQuestGetPlayQuest;
			BYTE*								lpBuffer;

			lpOnResQuestGetPlayQuest = (OnResQuestGetPlayQuest*)cLocalBuffer;
			lpOnResQuestGetPlayQuest->dwMsgType                 = ON_RESPONSE_QUEST_GETPLAYQUEST;
			lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount   = SI16(pParam2);
			lpOnResQuestGetPlayQuest->m_CompletedQuestListCount = SI16(pParam3);

			lpBuffer = (BYTE*)(cLocalBuffer + sizeof(OnResQuestGetPlayQuest));
			CopyMemory(lpBuffer, pParam4, sizeof(SPlayingQuest) * lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount);
			lpBuffer = (BYTE*)(lpBuffer + (sizeof(SPlayingQuest) * lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount));
			CopyMemory(lpBuffer, pParam5, sizeof(SI16) * lpOnResQuestGetPlayQuest->m_CompletedQuestListCount);

			siLength	=	sizeof(OnResQuestGetPlayQuest) + (sizeof(SPlayingQuest) * lpOnResQuestGetPlayQuest->m_PlayingQuestCodeCount) + (sizeof(SI16) * lpOnResQuestGetPlayQuest->m_CompletedQuestListCount);
		}
		break;
	
	case ON_RESPONSE_QUEST_GETPLAYQUESTDATA:
		{
			// pParam1 : Playing Quest Data
			OnResQuestGetPlayQuestData*			lpOnResQuestGetPlayQuestData;

			lpOnResQuestGetPlayQuestData = (OnResQuestGetPlayQuestData*)cLocalBuffer;
			lpOnResQuestGetPlayQuestData->dwMsgType   = ON_RESPONSE_QUEST_GETPLAYQUESTDATA;
			CopyMemory(&lpOnResQuestGetPlayQuestData->m_PlayingQuestData, pParam1, sizeof(SPlayingQuest));
			siLength	=	sizeof(OnResQuestGetPlayQuestData);
		}
		break;

	case ON_RESPONSE_QUEST_ALLCLEARQUEST:
		{
			OnResQuestAllClearQuest*			lpOnResQuestAllClearQuest;

			lpOnResQuestAllClearQuest            = (OnResQuestAllClearQuest*)cLocalBuffer;
			lpOnResQuestAllClearQuest->dwMsgType = ON_RESPONSE_QUEST_ALLCLEARQUEST;

			siLength = sizeof(OnResQuestAllClearQuest);
		}
		break;
		
	case ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO : 
		{
			OnDokdoQuestCompletedUserInfo* lpOnResQuestCompletedUserInfo;

			lpOnResQuestCompletedUserInfo = (OnDokdoQuestCompletedUserInfo*)cLocalBuffer;

			strncpy(lpOnResQuestCompletedUserInfo->szID, (char*)pParam1, 
				ON_ID_LENGTH+1);
			lpOnResQuestCompletedUserInfo->uiQuestCompletedUserCount = UI32(pParam2);			
			lpOnResQuestCompletedUserInfo->dwMsgType = ON_RESPONSE_QUEST_DOKDO_COMPLETED_USER_INFO;

			siLength = sizeof(OnDokdoQuestCompletedUserInfo);
		}
		break;

	case ON_RESPONSE_LEAVE_STRUCTURE:
		{

			OnResLeaveStructureMsg				*pLeaveStructureMsg;
			DWORD								dwResponse;

			dwResponse						=	DWORD( pParam1 );
				
			pLeaveStructureMsg				=	( OnResLeaveStructureMsg* )cLocalBuffer;
			pLeaveStructureMsg->dwMsgType	=	ON_RESPONSE_LEAVE_STRUCTURE;
			pLeaveStructureMsg->dwResponse	=	dwResponse;

			siLength	=	sizeof( OnResLeaveStructureMsg );

		}

		break;

	case ON_RESPONSE_LEAVE_FACTORY:
		{

			OnResLeaveFactoryMsg				*pLeaveFactoryMsg;
			DWORD								dwResponse;

			dwResponse						=	DWORD( pParam1 );
				
			pLeaveFactoryMsg				=	( OnResLeaveFactoryMsg* )cLocalBuffer;
			pLeaveFactoryMsg->dwMsgType		=	ON_RESPONSE_LEAVE_FACTORY;
			pLeaveFactoryMsg->dwResponse	=	dwResponse;

			siLength	=	sizeof( OnResLeaveFactoryMsg );

		}	

		break;

	case ON_CHANGE_WEATHER:
		{
			OnChangeWeather*				pOnChangeWeather;

			pOnChangeWeather = (OnChangeWeather*)cLocalBuffer;

			pOnChangeWeather->dwMsgType = ON_CHANGE_WEATHER;
			pOnChangeWeather->fWeather  = BYTE(pParam1);

			siLength	=	sizeof( OnChangeWeather );
		}
		break;

	case ON_MAKE_THUNDER:
		{
			OnMakeThunder*					pOnMakeThunder;

			pOnMakeThunder = (OnMakeThunder*)cLocalBuffer;

			pOnMakeThunder->dwMsgType	= ON_MAKE_THUNDER;
			pOnMakeThunder->uiAccount	=	(UI16)pParam1;	

			siLength	=	sizeof( OnMakeThunder );
		}
		break;

	case ON_FRIEND_LOGGEDINOUT:
		{
			OnFriendLoggedInOut				*pMsg;

			pMsg	=	(OnFriendLoggedInOut*)cLocalBuffer;
			pMsg->dwMsgType		=	ON_FRIEND_LOGGEDINOUT;
			pMsg->bState		=	(BOOL)pParam1;
			strncpy( pMsg->szID, (CHAR*)pParam2, ON_ID_LENGTH );

			siLength	=	sizeof( OnFriendLoggedInOut );
		}
		break;

	case ON_FARMWORK_COMPLETED:
		{
			OnFarmWorkCompletedMsg		*pFarmWorkCompletedMsg;

			siSendCharNum						=	 SI32( pParam1 );
			memcpy( uiCharUniIDs, ( UI16* )pParam2, siSendCharNum * sizeof( UI16 ) );


			pFarmWorkCompletedMsg				=	( OnFarmWorkCompletedMsg* )cLocalBuffer;
			pFarmWorkCompletedMsg->dwMsgType	=	ON_FARMWORK_COMPLETED;

			siLength		=	sizeof( OnFarmWorkCompletedMsg );		
		}
		break;	

	case ON_RESPONSE_VMERCENARY_CHANGE:
		{
			OnResponse_VMercenary_Change*		lpOnResponse_VMercenary_Change;

			lpOnResponse_VMercenary_Change = (OnResponse_VMercenary_Change*)cLocalBuffer;
			lpOnResponse_VMercenary_Change->dwMsgType  = ON_RESPONSE_VMERCENARY_CHANGE;
			lpOnResponse_VMercenary_Change->uiUniqueID = uiAccount;
			lpOnResponse_VMercenary_Change->siKind     = SI32(pParam1);

			siLength = sizeof(OnResponse_VMercenary_Change);
		}
		break;

	case ON_RESPONSE_VMERCENARY_CHANGEMAIN:
		{
			OnResponse_VMercenary_ChangeMain*		lpOnResponse_VMercenary_ChangeMain;

			lpOnResponse_VMercenary_ChangeMain = (OnResponse_VMercenary_ChangeMain*)cLocalBuffer;
			lpOnResponse_VMercenary_ChangeMain->dwMsgType  = ON_RESPONSE_VMERCENARY_CHANGEMAIN;
			lpOnResponse_VMercenary_ChangeMain->siSlot     = SI32(pParam1);

			siLength = sizeof(OnResponse_VMercenary_ChangeMain);
		}
		break;
	case ON_RESPONSE_SPENDITEM:
		{
			OnResSpendItemMsg *		pOnResSpendItem;

			pOnResSpendItem = (OnResSpendItemMsg *)cLocalBuffer;
			pOnResSpendItem->dwMsgType  = ON_RESPONSE_SPENDITEM;
			pOnResSpendItem->dwResponse = DWORD(pParam1) ;
			

			siLength = sizeof(OnResSpendItemMsg);
		}
		break;
	case ON_ADD_FOLLOWER:
		{
			OnAddFollower		*pOnAddFollower;

			pOnAddFollower	=	(OnAddFollower*)cLocalBuffer;
			pOnAddFollower->dwMsgType=	ON_ADD_FOLLOWER;

			CopyMemory( &pOnAddFollower->FollowerInfo, pParam1, sizeof(OnFollowerInfo) );

			siLength		=	sizeof(OnAddFollower);
		}
		break;
	case ON_REQUEST_RUN_PROGRAM_LIST:
		{
			OnReqRunProgramListMsg		*pRunProgramListMsg;

			pRunProgramListMsg				=	( OnReqRunProgramListMsg* )cLocalBuffer;
			pRunProgramListMsg->dwMsgType	=	ON_REQUEST_RUN_PROGRAM_LIST;
			pRunProgramListMsg->dwReason	=	DWORD(pParam1);

			siLength = sizeof( OnReqRunProgramListMsg );
		}
		break;
	
	case ON_MYVILLAGE_ATTACKED:
		{
			OnMyVillageAttackedMsg		*pMyVillageAttackedMsg;

			pMyVillageAttackedMsg				=	( OnMyVillageAttackedMsg* )cLocalBuffer;

			pMyVillageAttackedMsg->dwMsgType		=	ON_MYVILLAGE_ATTACKED;
			pMyVillageAttackedMsg->uiVillageCode	=	UI16( pParam1 );

			siLength = sizeof( OnMyVillageAttackedMsg );
		}

		break;

	case ON_EVENT_USERRANKINGS:
		{
			OnEventUserRankings			*pEventUserRanking;
			UI16						*pRankingData;

			pEventUserRanking					=	( OnEventUserRankings* )cLocalBuffer;
			
			pEventUserRanking->dwMsgType			=	ON_EVENT_USERRANKINGS;
			pEventUserRanking->uiPlayerNum			=	UI16( pParam1 );
			
			pRankingData						=	( UI16* ) ( ( CHAR* ) pEventUserRanking + sizeof( OnEventUserRankings ) );

			memcpy( pRankingData, pParam2, sizeof( UI16 ) * pEventUserRanking->uiPlayerNum * 2 );

			siLength							=	sizeof( OnEventUserRankings ) + ( sizeof( UI16 ) * pEventUserRanking->uiPlayerNum * 2 );
		}
		break;
		
	case ON_RESPONSE_PARTY: // ON_REQUEST_PARTY 에 대한 응답
		{
			OnResponseMsg				*pResponseMsg;
			
			pResponseMsg	=	( OnResponseMsg	* )cLocalBuffer;
			
			pResponseMsg->dwMsgType		=	ON_RESPONSE_PARTY;
			pResponseMsg->dwResponse	=	DWORD( pParam1 );
			
			siLength		=	sizeof( OnResponseMsg );			
			
		}
		break;
		
		
	case ON_RECEIVE_ASK_PARTY: // ON_REQUEST_PARTY 를 타켓 클라이언트에게 전송
		{
			//파티 신청 받은 클라이언트가 받는 메세지.			
			OnReceiveAskPartyMsg* pOnReceiveAskPartyMsg = (OnReceiveAskPartyMsg*) cLocalBuffer;
			
			pOnReceiveAskPartyMsg->dwMsgType = ON_RECEIVE_ASK_PARTY;
			pOnReceiveAskPartyMsg->UserID	 = (UI16)pParam1; // 파티 신청한 사람 uiAccount
	
			ZeroMemory( pOnReceiveAskPartyMsg->szID, sizeof( pOnReceiveAskPartyMsg->szID ) );
			strncpy( pOnReceiveAskPartyMsg->szID, ( CHAR* )pParam2, ON_ID_LENGTH );
			
			siLength  = sizeof( OnReceiveAskPartyMsg );
		}
		break;
		
	case ON_RESPONSE_ACCEPT_PARTY:
		{
			OnResAcceptPartyMsg		*pResAcceptPartyMsg;
			OnPartyInfo				*pPartyInfo;

			pResAcceptPartyMsg	=	( OnResAcceptPartyMsg* )cLocalBuffer;

			pResAcceptPartyMsg->dwMsgType	=	ON_RESPONSE_ACCEPT_PARTY;
			pResAcceptPartyMsg->dwResponse	=	( DWORD )pParam1;

			if( pParam5 != NULL )
			{
				ZeroMemory( pResAcceptPartyMsg->szID, sizeof( pResAcceptPartyMsg->szID ) );
				strncpy( pResAcceptPartyMsg->szID, ( CHAR* )pParam5, ON_ID_LENGTH );	
			}

			switch( pResAcceptPartyMsg->dwResponse )
			{
			case ON_RET_OK:
				pResAcceptPartyMsg->siNum	=	SI32( pParam2 );

				siSendCharNum	=	pResAcceptPartyMsg->siNum;				
				memcpy( uiCharUniIDs, ( UI16* )pParam3, siSendCharNum * sizeof( UI16 ) );

				pPartyInfo		=	( OnPartyInfo* ) ( ( CHAR* )pResAcceptPartyMsg + sizeof( OnResAcceptPartyMsg ) );
				memcpy( pPartyInfo, ( OnPartyInfo* )pParam4, sizeof( OnPartyInfo ) * pResAcceptPartyMsg->siNum );				

				siLength		=	sizeof( OnResAcceptPartyMsg ) + ( sizeof( OnPartyInfo ) * pResAcceptPartyMsg->siNum );

				break;

			case ON_RET_NO:
				siLength	=	sizeof( OnResAcceptPartyMsg );
				break;
			}


		}
		break;
		
	case ON_CANCEL_REQUESTED_PARTY:
		{
			OnMsg	*pOnMsg	=	( OnMsg* )cLocalBuffer;
			
			pOnMsg->dwMsgType	=	ON_CANCEL_REQUESTED_PARTY;
			
			siLength			=	sizeof( OnMsg );
		}
		
		break;
		
	case ON_PARTY_DELETED:
		{
			OnMsg	*pOnMsg	=	( OnMsg* )cLocalBuffer;
			
			pOnMsg->dwMsgType	=	ON_PARTY_DELETED;
			
			siLength			=	sizeof( OnMsg );
			
		}
		break;		

	case ON_RESPONSE_LEAVE_PARTY:
		{
			OnResLeavePartyMsg			*pLeavePartyMsg;
			OnPartyInfo					*pPartyInfo;
			
			pLeavePartyMsg	=	( OnResLeavePartyMsg* )cLocalBuffer;
			
			pLeavePartyMsg->dwMsgType		=	ON_RESPONSE_LEAVE_PARTY;
			pLeavePartyMsg->dwResponse		=	DWORD( pParam1 );			
			
			if( pParam2 != NULL )
			{
				ZeroMemory( pLeavePartyMsg->szLeavedPlayerID, sizeof( pLeavePartyMsg->szLeavedPlayerID ) );
				strncpy( pLeavePartyMsg->szLeavedPlayerID, ( CHAR* )pParam2, ON_ID_LENGTH );				
			}

			pLeavePartyMsg->siMembersNum	=	DWORD( pParam3 );

			switch( siSendInfo )
			{
			case SO_SFM_CUSTOM:
				siSendCharNum	=	pLeavePartyMsg->siMembersNum;
				memcpy( uiCharUniIDs, ( UI16* )pParam4, siSendCharNum * sizeof( UI16 ) );

				pPartyInfo		=	( OnPartyInfo* ) ( ( CHAR* )pLeavePartyMsg + sizeof( OnResLeavePartyMsg ) );
				memcpy( pPartyInfo, ( OnPartyInfo* )pParam5, sizeof( OnPartyInfo ) * pLeavePartyMsg->siMembersNum );

				siLength		=	sizeof( OnResLeavePartyMsg ) + ( sizeof( OnPartyInfo ) * pLeavePartyMsg->siMembersNum );

				break;

			default:

				siLength		=	sizeof( OnResLeavePartyMsg );

				break;
			}
			
		}		

		break;

	case ON_RESPONSE_CANCEL_REQUESTING_PARTY:
		{
			OnResponseMsg				*pResponseMsg;
			
			pResponseMsg	=	( OnResponseMsg	* )cLocalBuffer;
			
			pResponseMsg->dwMsgType		=	ON_RESPONSE_CANCEL_REQUESTING_PARTY;
			pResponseMsg->dwResponse	=	DWORD( pParam1 );
			
			siLength		=	sizeof( OnResponseMsg );
		}
		break;

	case ON_CHECK_GAMEGUARD:
		{
			OnCheckGameGuardMsg		*pCheckGameGuard;

			pCheckGameGuard = (OnCheckGameGuardMsg *)cLocalBuffer;

			pCheckGameGuard->dwMsgType = ON_CHECK_GAMEGUARD;
			pCheckGameGuard->dwGameGuardKey = DWORD(pParam1);

			siLength		=	sizeof( OnCheckGameGuardMsg );
		}		
		break;
	case ON_RESOPNSE_REAL_TIME:
		{	
			OnResRealTime			*pOnResRealTime;

			pOnResRealTime	=  (OnResRealTime *)cLocalBuffer;

			pOnResRealTime->dwMsgType	=	ON_RESOPNSE_REAL_TIME;
			pOnResRealTime->uiYear		=   UI16(pParam1);
			pOnResRealTime->uiMon		=   UI16(pParam2);
			pOnResRealTime->uiDay		=   UI16(pParam3);

			siLength		=	sizeof(OnResRealTime);
		}
		break;

	case ON_RESPONSE_CHECK_GAMEGUARD:
		{
			OnResponseMsg		*pOnResponseMsg;

			pOnResponseMsg = (OnResponseMsg *)cLocalBuffer;

			pOnResponseMsg->dwMsgType = ON_RESPONSE_CHECK_GAMEGUARD;
			pOnResponseMsg->dwResponse = DWORD(pParam1);

			siLength		=	sizeof( OnResponseMsg );
		}
		break;
		
	default:
		bRet	=	FALSE;
		break;
	}
	
	//	siSendInfo		: CATEGORY(보내는 방법)  , , , , uiCharUniIDs, bSendMe);
	//	cLocalBuffer	: 보내는 소켓 버퍼 
	//	siLength		: 보내는 소켓의 길이
	//  siSendCharNum	: 반복해서 보내느 횟수
	//  uiCharUniIDs	: 받아 보는 대상( 배열 )
	//  bSendMe			: 본인한테도 보내는지 플래그 
	if(bRet == TRUE)
		SendMsg( siSendInfo, cLocalBuffer, siLength, siSendCharNum, uiCharUniIDs, bSendMe);
	
	return bRet;
}




