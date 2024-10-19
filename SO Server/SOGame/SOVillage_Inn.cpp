#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �������� ������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::LeaveInn(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;	

	if(pstINNData)
	{
		// ��ȭ�濡�� ���� �� �ִ�.
		pPlayer->SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));		

		bSuccess	=	TRUE;
	}

	return bSuccess;
};

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ��ȭ������ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterWaitRoom(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;		

	if(pstINNData)
	{
		// ���ǿ� ���Խ�Ų��.
		pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);		

		// ���� �����ִ� ��ȭ���� ����Ʈ�� �����ش�.
		pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));				

		// ���� ������ �� �� �ִ�.
		bSuccess	=	TRUE;	
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�÷��̾ ���� ä�ù濡 �����ߴ�.
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
		// �������� ���� �켱 ��ȿ�� ������ �˾ƿ´�.
		if(IsValidChatRoomIDInInn(uiChatRoomID) == FALSE)	return FALSE;

		// �׸��� �� �� �ִ� ������ �ִ��� ���캻��.
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
			// ���������� �����ߴ�.				
			// �����Ѵ�.
			uiPlayerCounter	=	0;
			for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
			{
				if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
				{
					// ��� �ش� �÷��̾ ������ �����ϴ��� �˾ƿ´�.
					// ������ �����ϴ� ĳ���͸� �����ش�.
					if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
					{
						uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
						uiPlayerCounter++;
					}					
				}					
			}		
			
			// ������ �޽����� �����ش�.			
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
				LPVOID(ON_RET_INN_OK), LPVOID(uiPlayerCounter),
				LPVOID(uiPlayers),
				LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].dwUniqueID));			
			
			// �ٸ� �÷��̾�Ե� �˷��ش�.(���� �����ϱ����� ������.)
			if(uiPlayerCounter != 0)
				pPlayer->SendFieldMsg(ON_VIL_INN_JOIN_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));						
			
			// ���������� ���� �迭�� �߰��Ѵ�.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].uiChatRoomJoinedPlayerAccount	=	pPlayer->uiAccount;
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].siDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
			
			// ���� �̹濡 ������ �� ������� ���� ���������ش�.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter + 1;

			// ���� ��ܿ��� ���� �����Ѵ�.
			pstINNData->WaitRoomUserManager.DeleteType(pPlayer->uiAccount);						
			
			// ���� �� ���� �����Ǵ� ������ �Ǵ��ؼ�
			// �����Ǵ� ���̶�� �Ʒ� �޽����� ������ �ʴ´�.
			// �� ���� �޽����� �ٸ� ������ �����ش�.
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum > 1)
			{
				// ���� �÷��̾���� ����� ������ ������ ����ڿ��� �����ش�.
				// ���ǿ� �ִ� ����ڿ��� Ư�� �濡 ����� ����ڰ� �ִ��� �˷��ִ°��̴�.
				pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
					LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
					LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
			}


			// �÷���� ��� �濡 ���ٰ� �������ش�.
			pPlayer->stAccountInfo.uiChatRoomIDInInn	=	uiChatRoomID;

			bSuccess	=	TRUE;	
		}		
		else
		{
			printf("�� �̻� �� ���� �����ϴ�.\n");
		}
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�÷��̾ ���� ä�ù濡�� ������.
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
		// �÷��̾ �������� �����ٰ� �����Ѵ�.
		pPlayer->stAccountInfo.uiChatRoomIDInInn	=	0;

		// �����µ� �����ߴ�.		

		// �ٸ� �÷��̾�Ե� �� �÷��̾ �����ٰ� �˷��ش�.
		// �����Ѵ�.
		uiPlayerCounter	=	0;
		for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
		{
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
			{
				// ��� �ش� �÷��̾ ������ �����ϴ��� �˾ƿ´�.
				// ������ �����ϴ� ĳ���͸� �����ش�.
				if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
				{
					uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
					uiPlayerCounter++;
				}					
			}					
		}


		if(uiPlayerCounter != 0)
			pPlayer->SendFieldMsg(ON_VIL_INN_LEAVE_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));			

		// ���� �̹濡 ������ �� ������� ���� �ٽ� �޴´�.
		pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter;		

		// Disconnect�� �ƴ϶�� ������ �޽����� �����ش�.
		if(bDisconnect == FALSE)
		{
			// ���� ���ǿ� �ִ� ���� ���� ������ �ٽ� �����ش�.
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));
		}

		// ���� �濡 ����� ����ٸ� ���ǿ� ���� �������ٰ� �ϰ�
		// ���ǿ� �ִ� ����ڿ��� ����� ������ �����ش�.
		if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum == 0)
		{
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
								LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
								LPVOID(ON_VWR_DELETEROOM), LPVOID(uiChatRoomID));			
		}
		else
		{
			// ���� �������� �ʾ��� ��쿡�� �濡 ����ִ� �÷��̾��� ���� �����ش�.
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
									LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
									LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
		}		

		if(bDisconnect == FALSE)
		{
			// Disconnect�� �ƴ϶�� ���ǿ� ���� �߰��Ѵ�.
			pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);			
		}

		return TRUE;
	}
	else
	{
		printf("�̹濡 ���� �ִ� ĳ���Ͱ� �ƴ�\n");
		return FALSE;
	}
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	�������� ID�� �����Ѵ�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȭ���� �����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::CreateChatRoomInInn(SOPlayer *pPlayer, CHAR *pszChatRoomName)
{
	BOOL	bSuccess = FALSE;	
	UINT	uiChatRoomIndex;

	if(pstINNData)
	{
		// ���� �÷��̾��� ���°� ���� ���ǿ� �־�� �Ѵ�.		
		if(pPlayer->stAccountInfo.uiStatusInVillage == INVILLAGE_STATUS_INN && pPlayer->stAccountInfo.uiChatRoomIDInInn == 0)
		{
			// i�� ID �̹Ƿ� 
			for(uiChatRoomIndex = 0; uiChatRoomIndex < ON_MAX_CHATROOM_NUM; uiChatRoomIndex++)
			{
				// ����ִ� ���� ����.
				if(pstINNData->ChatRoom[uiChatRoomIndex].uiJoinedPlayerNum == 0)									
					break;				
			}

			// ����ִ� ���� ã���� ��쿡��.
			if(uiChatRoomIndex < ON_MAX_CHATROOM_NUM)
			{
				// ���� �̸��� �������ش�.
				ZeroMemory(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, sizeof(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));			
				strcpy(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, pszChatRoomName);				
				
				// ���� ID�� �����Ѵ�.
				pstINNData->ChatRoom[uiChatRoomIndex].dwUniqueID	=	clWorld->IncChatRoomInVillCounter();

				// ����ִ� ���� ��������� ���� ������� �÷��̾ ��ȿ� �ִ´�.
				bSuccess	=	JoinChatRoomInInn(CHATROOMINDEXTOID(uiChatRoomIndex), pPlayer);						

				if(bSuccess == TRUE)
				{
					// �׸��� �ٸ� ����ڿ��� ���� ������ٰ� �˷��ش�.				
					pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
										LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
										LPVOID(ON_VWR_CREATEROOM), 
										LPVOID(CHATROOMINDEXTOID(uiChatRoomIndex)),									// 0�� ����üũ�� ���ؼ� ���ܵд�.
										LPVOID(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));					
				}
				else
				{
					printf("�� ���� ����\n");
				}
			}
			else
			{
				printf("CreateChatRoomInInn :: ����ִ� ���� �����ϴ�\n");
			}
		}		
	}

	return bSuccess;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խ��ǿ� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterNoticeBoard(SOPlayer *pPlayer)
{
	// �Խ��ǿ��� ������ ����.
	return pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ����Ѵ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::RegistNotice(SOPlayer *pPlayer, CHAR *pszTitle, CHAR *pszNotice)
{	
	DBIKRegistryNoticeInfo		RegistryNoticeInfo;
	SI32						siNewIndex;
	
	// DB�� ����Ѵ�.
	RegistryNoticeInfo.uiVillageCode	=	uiVillageCode;
	
	ZeroMemory(RegistryNoticeInfo.szID, sizeof(RegistryNoticeInfo.szID));
	if(pPlayer)
		strncpy(RegistryNoticeInfo.szID, pPlayer->szID, ON_ID_LENGTH);
	else
		strncpy(RegistryNoticeInfo.szID, "���", ON_ID_LENGTH);

	RegistryNoticeInfo.pszTitle			=	pszTitle;
	RegistryNoticeInfo.pszNotice		=	pszNotice;

	ZeroMemory(RegistryNoticeInfo.szDate, sizeof(RegistryNoticeInfo.szDate));
	wsprintf(RegistryNoticeInfo.szDate, "%04d%02d%02d", clWorld->CurrentTime.Year, clWorld->CurrentTime.Month, clWorld->CurrentTime.Day);	
	
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_REGISTNOTICE, LPVOID(&RegistryNoticeInfo), LPVOID(&siNewIndex));

	// ���� �߰��Ѵ�.(�� ��ȣ�� �޾ƾ� �Ѵ�.)
	pstINNData->NoticeNumber.Add(siNewIndex);
	
	// ������ ���� �� �����Ѵ�.
	pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

	if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
		pstINNData->uiTotalPageNum++;

	if( pPlayer )
		return pPlayer->SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Խù��� �����Ѵ�.
//	���� ����	:	2002-04-15 ���� 3:06:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::ModifyNotice(SOPlayer *pPlayer, DWORD dwIndex, CHAR *pszTitle, CHAR *pszContent)
{
	DBIKModifyNoticeInfo	ModifyNotice;
	BOOL					bModifySuccess;

	// �Խù��� �����Ѵ�.

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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Խù��� �����Ѵ�.
//	���� ����	:	2002-04-30 ���� 10:41:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DeleteNotice(SOPlayer *pPlayer, UI16 uiIndex)
{
	DBIKDeleteNotice	DeleteNotice;
	BOOL				bDBSuccess, bSuccess;	
	

	// ���� ������ ���ٸ� �׳� �����Ѵ�.
	if(pstINNData == NULL)	return FALSE;

	// �Խù��� �����Ѵ�.
	ZeroMemory(&DeleteNotice, sizeof(DeleteNotice));
	DeleteNotice.uiVillageCode	=	uiVillageCode;
	DeleteNotice.uiNoticeIndex	=	uiIndex;
	strncpy(DeleteNotice.szWroteID, pPlayer->szID, ON_ID_LENGTH);

	bDBSuccess	=	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_DELETENOTICE, LPVOID(&DeleteNotice), LPVOID(&bSuccess));

	if((bSuccess == TRUE) && (bDBSuccess == TRUE))
	{
		// ���� �����Ѵ�.(�� ��ȣ�� �޾ƾ� �Ѵ�.)
		pstINNData->NoticeNumber.DeleteType(uiIndex);		

		// ������ ���� �� �����Ѵ�.
		pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

		if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
			pstINNData->uiTotalPageNum++;

		pPlayer->SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));		
	}	

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ����Ʈ�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::SendNoticesList(SOPlayer *pPlayer, UI16 uiPage)
{	
	DBIKGetNoticesHeaderInfoIn				GetNoticesHeaderInfoIn;
	DBIKGetNoticesHeaderInfoOut				GetNoticesHeaderInfoOut;	
	
	GetNoticesHeaderInfoIn.uiVillageCode			=	uiVillageCode;
	GetNoticesHeaderInfoIn.puiNoticesIndexArray		=	pstINNData->NoticeNumber.m_Data;
	GetNoticesHeaderInfoIn.siTopIndex				=	(pstINNData->NoticeNumber.m_NowUseDataCount - (ON_SEND_NOTICESNUM_PER_ONCE * (uiPage - 1))) - 1;
	
	// siTopIndex�� ��ȿ���� ������ List�� ������ �ʴ´�.
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
		printf("####### SOVillage::SendNoticesList ####### �߸��� �ε����� ���Խ��ϴ�. : [%d] <= [%d]", 
							GetNoticesHeaderInfoIn.siTopIndex, pstINNData->NoticeNumber.m_NowUseDataCount);
		return FALSE;
	}
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Խù��� ���빰�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ä�� ID�� ��ȿ���� �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::IsValidChatRoomIDInInn(UI16 uiChatRoomID)
{
	if((uiChatRoomID > 0) && (uiChatRoomID <= ON_MAX_CHATROOM_NUM))
		return TRUE;
	else
		return FALSE;
}
*/