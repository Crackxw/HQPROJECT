#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

/*
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	 주점에서 나간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::LeaveInn(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;	

	if(pstINNData)
	{
		// 대화방에서 나갈 수 있다.
		pPlayer->SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));		

		bSuccess	=	TRUE;
	}

	return bSuccess;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점의 대화방으로 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::EnterWaitRoom(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;		

	if(pstINNData)
	{
		// 대기실에 포함시킨다.
		pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);		

		// 현재 열려있는 대화방의 리스트를 보여준다.
		pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));				

		// 주점 안으로 들어갈 수 있다.
		bSuccess	=	TRUE;	
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	플레이어가 주점 채팅방에 참여했다.
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::JoinChatRoomInInn(UI16 uiChatRoomID, SOPlayer *pPlayer)
{
	SI32		i;
	BOOL		bSuccess = FALSE;	
	UI16		uiPlayers[SOVILLAGE_MAX_INN_CHAT_NUM];
	UI16		uiPlayerCounter;	
	SI32		siSearchedIndex;

	if(pstINNData)
	{	
		// 들어오려는 방이 우선 유효한 방인지 알아온다.
		if(IsValidChatRoomIDInInn(uiChatRoomID) == FALSE)	return FALSE;

		// 그리고 들어갈 수 있는 공간이 있는지 살펴본다.
		siSearchedIndex	=	SOVILLAGE_MAX_INN_CHAT_NUM;
		for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
		{
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount == 0)
			{
				siSearchedIndex	=	i;			
				break;
			}			
		}
		
		if(siSearchedIndex < SOVILLAGE_MAX_INN_CHAT_NUM)
		{
			// 성공적으로 참여했다.				
			// 정렬한다.
			uiPlayerCounter	=	0;
			for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
			{
				if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
				{
					// 대신 해당 플레이어가 실제로 존재하는지 알아온다.
					// 실제로 존재하는 캐릭터만 보내준다.
					if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
					{
						uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
						uiPlayerCounter++;
					}					
				}					
			}		
			
			// 나에게 메시지를 보내준다.			
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
				LPVOID(ON_RET_INN_OK), LPVOID(uiPlayerCounter),
				LPVOID(uiPlayers),
				LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].dwUniqueID));			
			
			// 다른 플레이어에게도 알려준다.(나를 포함하기전에 보낸다.)
			if(uiPlayerCounter != 0)
				pPlayer->SendFieldMsg(ON_VIL_INN_JOIN_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));						
			
			// 마지막으로 나를 배열에 추가한다.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].uiChatRoomJoinedPlayerAccount	=	pPlayer->uiAccount;
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].siDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
			
			// 현재 이방에 참여한 총 사용자의 수를 증가시켜준다.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter + 1;

			// 대기실 명단에서 나를 제거한다.
			pstINNData->WaitRoomUserManager.DeleteType(pPlayer->uiAccount);						
			
			// 현재 이 방이 생성되는 방인지 판단해서
			// 생성되는 방이라면 아래 메시지를 보내지 않는다.
			// 방 생성 메시지는 다른 곳에서 보내준다.
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum > 1)
			{
				// 방의 플레이어수가 변경된 정보를 대기실의 사용자에게 보내준다.
				// 대기실에 있는 사용자에게 특정 방에 몇명의 사용자가 있는지 알려주는것이다.
				pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
					LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
					LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
			}


			// 플레어에게 어느 방에 들어갔다고 설정해준다.
			pPlayer->stAccountInfo.uiChatRoomIDInInn	=	uiChatRoomID;

			bSuccess	=	TRUE;	
		}		
		else
		{
			printf("더 이상 빈 방이 없습니다.\n");
		}
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	플레이어가 주점 채팅방에서 나갔다.
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::LeaveChatRoomInInn(SOPlayer *pPlayer, BOOL bDisconnect)
{
	SI32	i;	
	UI32	uiPlayerCounter;
	UI16	uiPlayers[SOVILLAGE_MAX_INN_CHAT_NUM];	
	UI16	uiChatRoomID;	

	uiChatRoomID	=	pPlayer->stAccountInfo.uiChatRoomIDInInn;

	if(DelAccountInInn(uiChatRoomID, pPlayer->uiAccount) == TRUE)	
	{
		// 플레이어가 주점에서 나갔다고 설정한다.
		pPlayer->stAccountInfo.uiChatRoomIDInInn	=	0;

		// 나가는데 성공했다.		

		// 다른 플레이어에게도 이 플레이어가 나갔다고 알려준다.
		// 정렬한다.
		uiPlayerCounter	=	0;
		for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
		{
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
			{
				// 대신 해당 플레이어가 실제로 존재하는지 알아온다.
				// 실제로 존재하는 캐릭터만 보내준다.
				if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
				{
					uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
					uiPlayerCounter++;
				}					
			}					
		}


		if(uiPlayerCounter != 0)
			pPlayer->SendFieldMsg(ON_VIL_INN_LEAVE_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));			

		// 현재 이방에 참여한 총 사용자의 수를 다시 받는다.
		pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter;		

		// Disconnect가 아니라면 나에게 메시지를 보내준다.
		if(bDisconnect == FALSE)
		{
			// 현재 대기실에 있는 방의 대한 정보를 다시 보내준다.
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));
		}

		// 만약 방에 사람이 비었다면 대기실에 방이 없어졌다고 하고
		// 대기실에 있는 사용자에게 변경된 정보를 보내준다.
		if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum == 0)
		{
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
								LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
								LPVOID(ON_VWR_DELETEROOM), LPVOID(uiChatRoomID));			
		}
		else
		{
			// 방이 없어지지 않았을 경우에는 방에 들어있는 플레이어의 수를 보내준다.
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
									LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
									LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
		}		

		if(bDisconnect == FALSE)
		{
			// Disconnect가 아니라면 대기실에 나를 추가한다.
			pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);			
		}

		return TRUE;
	}
	else
	{
		printf("이방에 들어와 있는 캐릭터가 아님\n");
		return FALSE;
	}
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	주점에서 ID를 제거한다.
//----------------------------------------------------------------------------------------------------------------
BOOL	SOVillage::DelAccountInInn(UI16 uiChatRoomID, UI16 uiAccount)
{
	SI32	i;
	UI32	uiChatRoomIndex;
	
	if(pstINNData)
	{		
		uiChatRoomIndex		=	CHATROOMIDTOINDEX(uiChatRoomID);

		for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
		{
			if(pstINNData->ChatRoom[uiChatRoomIndex].CRJPI[i].uiChatRoomJoinedPlayerAccount == uiAccount)
			{
				pstINNData->ChatRoom[uiChatRoomIndex].CRJPI[i].uiChatRoomJoinedPlayerAccount	=	0;
				return TRUE;			
			}
		}				
	}
	

	return FALSE;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	대화방을 만든다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::CreateChatRoomInInn(SOPlayer *pPlayer, CHAR *pszChatRoomName)
{
	BOOL	bSuccess = FALSE;	
	UINT	uiChatRoomIndex;

	if(pstINNData)
	{
		// 현재 플레이어의 상태가 주점 대기실에 있어야 한다.		
		if(pPlayer->stAccountInfo.uiStatusInVillage == INVILLAGE_STATUS_INN && pPlayer->stAccountInfo.uiChatRoomIDInInn == 0)
		{
			// i는 ID 이므로 
			for(uiChatRoomIndex = 0; uiChatRoomIndex < ON_MAX_CHATROOM_NUM; uiChatRoomIndex++)
			{
				// 비어있는 방을 고른다.
				if(pstINNData->ChatRoom[uiChatRoomIndex].uiJoinedPlayerNum == 0)									
					break;				
			}

			// 비어있는 방을 찾았을 경우에만.
			if(uiChatRoomIndex < ON_MAX_CHATROOM_NUM)
			{
				// 방의 이름을 설정해준다.
				ZeroMemory(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, sizeof(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));			
				strcpy(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, pszChatRoomName);				
				
				// 방의 ID를 설정한다.
				pstINNData->ChatRoom[uiChatRoomIndex].dwUniqueID	=	clWorld->IncChatRoomInVillCounter();

				// 비어있는 방을 골랐으면은 방을 만들려는 플레이어를 방안에 넣는다.
				bSuccess	=	JoinChatRoomInInn(CHATROOMINDEXTOID(uiChatRoomIndex), pPlayer);						

				if(bSuccess == TRUE)
				{
					// 그리고 다른 사용자에게 방을 만들었다고 알려준다.				
					pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
										LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
										LPVOID(ON_VWR_CREATEROOM), 
										LPVOID(CHATROOMINDEXTOID(uiChatRoomIndex)),									// 0은 에러체크를 위해서 남겨둔다.
										LPVOID(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));					
				}
				else
				{
					printf("방 생성 실패\n");
				}
			}
			else
			{
				printf("CreateChatRoomInInn :: 비어있는 방이 없습니다\n");
			}
		}		
	}

	return bSuccess;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시판에 들어간다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::EnterNoticeBoard(SOPlayer *pPlayer)
{
	// 게시판에는 무조건 들어간다.
	return pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물을 등록한다
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::RegistNotice(SOPlayer *pPlayer, CHAR *pszTitle, CHAR *pszNotice)
{	
	DBIKRegistryNoticeInfo		RegistryNoticeInfo;
	SI32						siNewIndex;
	
	// DB에 등록한다.
	RegistryNoticeInfo.uiVillageCode	=	uiVillageCode;
	
	ZeroMemory(RegistryNoticeInfo.szID, sizeof(RegistryNoticeInfo.szID));
	if(pPlayer)
		strncpy(RegistryNoticeInfo.szID, pPlayer->szID, ON_ID_LENGTH);
	else
		strncpy(RegistryNoticeInfo.szID, "운영자", ON_ID_LENGTH);

	RegistryNoticeInfo.pszTitle			=	pszTitle;
	RegistryNoticeInfo.pszNotice		=	pszNotice;

	ZeroMemory(RegistryNoticeInfo.szDate, sizeof(RegistryNoticeInfo.szDate));
	wsprintf(RegistryNoticeInfo.szDate, "%04d%02d%02d", clWorld->CurrentTime.Year, clWorld->CurrentTime.Month, clWorld->CurrentTime.Day);	
	
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_REGISTNOTICE, LPVOID(&RegistryNoticeInfo), LPVOID(&siNewIndex));

	// 글을 추가한다.(글 번호를 받아야 한다.)
	pstINNData->NoticeNumber.Add(siNewIndex);
	
	// 페이지 수를 재 설정한다.
	pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

	if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
		pstINNData->uiTotalPageNum++;

	if( pPlayer )
		return pPlayer->SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	게시물을 수정한다.
//	수정 일자	:	2002-04-15 오후 3:06:05 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::ModifyNotice(SOPlayer *pPlayer, DWORD dwIndex, CHAR *pszTitle, CHAR *pszContent)
{
	DBIKModifyNoticeInfo	ModifyNotice;
	BOOL					bModifySuccess;

	// 게시물을 수정한다.

	ZeroMemory(&ModifyNotice, sizeof(ModifyNotice));
	ModifyNotice.uiVillageCode		=	uiVillageCode;
	ModifyNotice.uiNoticeIndex		=	UI16(dwIndex);	
	ModifyNotice.pszTitle			=	pszTitle;
	ModifyNotice.pszNotice			=	pszContent;	

	ZeroMemory(ModifyNotice.szDate, sizeof(ModifyNotice.szDate));
	sprintf(ModifyNotice.szDate, "%04d%02d%02d", clWorld->CurrentTime.Year, clWorld->CurrentTime.Month, clWorld->CurrentTime.Day);	

	if(clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_MODIFYNOTICE, LPVOID(&ModifyNotice), LPVOID(&bModifySuccess)) == TRUE)
	{
		if(bModifySuccess == TRUE)
			return	pPlayer->SendFieldMsg(ON_RESPONSE_MODIFY_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));
	}
	

	return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	게시물을 삭제한다.
//	수정 일자	:	2002-04-30 오전 10:41:06 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::DeleteNotice(SOPlayer *pPlayer, UI16 uiIndex)
{
	DBIKDeleteNotice	DeleteNotice;
	BOOL				bDBSuccess, bSuccess;	
	

	// 만약 주점이 없다면 그냥 리턴한다.
	if(pstINNData == NULL)	return FALSE;

	// 게시물을 삭제한다.
	ZeroMemory(&DeleteNotice, sizeof(DeleteNotice));
	DeleteNotice.uiVillageCode	=	uiVillageCode;
	DeleteNotice.uiNoticeIndex	=	uiIndex;
	strncpy(DeleteNotice.szWroteID, pPlayer->szID, ON_ID_LENGTH);

	bDBSuccess	=	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_DELETENOTICE, LPVOID(&DeleteNotice), LPVOID(&bSuccess));

	if((bSuccess == TRUE) && (bDBSuccess == TRUE))
	{
		// 글을 삭제한다.(글 번호를 받아야 한다.)
		pstINNData->NoticeNumber.DeleteType(uiIndex);		

		// 페이지 수를 재 설정한다.
		pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

		if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
			pstINNData->uiTotalPageNum++;

		pPlayer->SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));		
	}	

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물이 리스트를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::SendNoticesList(SOPlayer *pPlayer, UI16 uiPage)
{	
	DBIKGetNoticesHeaderInfoIn				GetNoticesHeaderInfoIn;
	DBIKGetNoticesHeaderInfoOut				GetNoticesHeaderInfoOut;	
	
	GetNoticesHeaderInfoIn.uiVillageCode			=	uiVillageCode;
	GetNoticesHeaderInfoIn.puiNoticesIndexArray		=	pstINNData->NoticeNumber.m_Data;
	GetNoticesHeaderInfoIn.siTopIndex				=	(pstINNData->NoticeNumber.m_NowUseDataCount - (ON_SEND_NOTICESNUM_PER_ONCE * (uiPage - 1))) - 1;
	
	// siTopIndex가 유효하지 않으면 List를 보내지 않는다.
	if((GetNoticesHeaderInfoIn.siTopIndex >= 0) && (GetNoticesHeaderInfoIn.siTopIndex < pstINNData->NoticeNumber.m_NowUseDataCount))
	{
		clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETNOTICESHEADER, 
										LPVOID(&GetNoticesHeaderInfoIn), 
										LPVOID(&GetNoticesHeaderInfoOut));

		return pPlayer->SendFieldMsg(ON_RESPONSE_NOTICEBOARD_NOTICELIST, SO_SFM_ONLYME, 
										LPVOID(ON_RET_INN_OK),
										LPVOID(pstINNData->uiTotalPageNum), 
										LPVOID(GetNoticesHeaderInfoOut.uiNoticesNum), 
										LPVOID(GetNoticesHeaderInfoOut.NoticeHeaderInfo));
	}
	else
	{
		printf("####### SOVillage::SendNoticesList ####### 잘못된 인덱스가 들어왔습니다. : [%d] <= [%d]", 
							GetNoticesHeaderInfoIn.siTopIndex, pstINNData->NoticeNumber.m_NowUseDataCount);
		return FALSE;
	}
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	게시물의 내용물을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::SendNoticeContent(SOPlayer *pPlayer, UI16 uiNoticeID)
{
	DBIKGetNoticeContentIn		GetNoticeContentIn;
	//IKNoticeReadCount			NoticeReadCount;
	CHAR						szNoticeContentBuffer[10240];

	GetNoticeContentIn.uiVillageCode	=	uiVillageCode;
	GetNoticeContentIn.uiNoticeID		=	uiNoticeID;

	ZeroMemory(szNoticeContentBuffer, sizeof(szNoticeContentBuffer));
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GETNOTICECONTENT,
									LPVOID(&GetNoticeContentIn),
									LPVOID(szNoticeContentBuffer));

	return pPlayer->SendFieldMsg(ON_RESPONSE_VIEW_NOTICE, SO_SFM_ONLYME, 
									LPVOID(ON_RET_INN_OK),
									LPVOID(strlen(szNoticeContentBuffer)), 
									LPVOID(szNoticeContentBuffer));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	주점의 채팅 ID가 유효한지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOVillage::IsValidChatRoomIDInInn(UI16 uiChatRoomID)
{
	if((uiChatRoomID > 0) && (uiChatRoomID <= ON_MAX_CHATROOM_NUM))
		return TRUE;
	else
		return FALSE;
}
*/