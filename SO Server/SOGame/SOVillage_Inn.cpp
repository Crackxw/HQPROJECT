#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 輿薄縑憮 釭除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::LeaveInn(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;	

	if(pstINNData)
	{
		// 渠�食瞈□� 釭陞 熱 氈棻.
		pPlayer->SendFieldMsg(ON_RESPONSE_LEAVE_INN, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));		

		bSuccess	=	TRUE;
	}

	return bSuccess;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄曖 渠�食磍虞� 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterWaitRoom(SOPlayer *pPlayer)
{
	BOOL	bSuccess = FALSE;		

	if(pstINNData)
	{
		// 渠晦褒縑 んл衛鑑棻.
		pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);		

		// ⑷營 翮溥氈朝 渠�食磍� 葬蝶お蒂 爾罹遽棻.
		pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_VWR, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));				

		// 輿薄 寰戲煎 菟橫陞 熱 氈棻.
		bSuccess	=	TRUE;	
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Ы溯檜橫陛 輿薄 瓣た寞縑 霤罹ц棻.
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
		// 菟橫螃溥朝 寞檜 辦摹 嶸�褲� 寞檣雖 憲嬴螞棻.
		if(IsValidChatRoomIDInInn(uiChatRoomID) == FALSE)	return FALSE;

		// 斜葬堅 菟橫陞 熱 氈朝 奢除檜 氈朝雖 髦よ獄棻.
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
			// 撩奢瞳戲煎 霤罹ц棻.				
			// 薑溺и棻.
			uiPlayerCounter	=	0;
			for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
			{
				if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
				{
					// 渠褐 п渡 Ы溯檜橫陛 褒薯煎 襄營ж朝雖 憲嬴螞棻.
					// 褒薯煎 襄營ж朝 議葛攪虜 爾頂遽棻.
					if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
					{
						uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
						uiPlayerCounter++;
					}					
				}					
			}		
			
			// 釭縑啪 詭衛雖蒂 爾頂遽棻.			
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_JOIN_CHAT_ROOM, SO_SFM_ONLYME,
				LPVOID(ON_RET_INN_OK), LPVOID(uiPlayerCounter),
				LPVOID(uiPlayers),
				LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].dwUniqueID));			
			
			// 棻艇 Ы溯檜橫縑啪紫 憲溥遽棻.(釭蒂 んлж晦瞪縑 爾魚棻.)
			if(uiPlayerCounter != 0)
				pPlayer->SendFieldMsg(ON_VIL_INN_JOIN_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));						
			
			// 葆雖虞戲煎 釭蒂 寡翮縑 蹺陛и棻.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].uiChatRoomJoinedPlayerAccount	=	pPlayer->uiAccount;
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[siSearchedIndex].siDBAccount					=	pPlayer->stAccountInfo.siDBAccount;
			
			// ⑷營 檜寞縑 霤罹и 識 餌辨濠曖 熱蒂 隸陛衛麵遽棻.
			pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter + 1;

			// 渠晦褒 貲欽縑憮 釭蒂 薯剪и棻.
			pstINNData->WaitRoomUserManager.DeleteType(pPlayer->uiAccount);						
			
			// ⑷營 檜 寞檜 儅撩腎朝 寞檣雖 っ欽п憮
			// 儅撩腎朝 寞檜塭賊 嬴楚 詭衛雖蒂 爾頂雖 彊朝棻.
			// 寞 儅撩 詭衛雖朝 棻艇 夠縑憮 爾頂遽棻.
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum > 1)
			{
				// 寞曖 Ы溯檜橫熱陛 滲唳脹 薑爾蒂 渠晦褒曖 餌辨濠縑啪 爾頂遽棻.
				// 渠晦褒縑 氈朝 餌辨濠縑啪 か薑 寞縑 賃貲曖 餌辨濠陛 氈朝雖 憲溥輿朝匙檜棻.
				pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
					LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
					LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
			}


			// Ы溯橫縑啪 橫替 寞縑 菟橫鬲棻堅 撲薑п遽棻.
			pPlayer->stAccountInfo.uiChatRoomIDInInn	=	uiChatRoomID;

			bSuccess	=	TRUE;	
		}		
		else
		{
			printf("渦 檜鼻 綴 寞檜 橈蝗棲棻.\n");
		}
	}

	return bSuccess;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Ы溯檜橫陛 輿薄 瓣た寞縑憮 釭鬲棻.
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
		// Ы溯檜橫陛 輿薄縑憮 釭鬲棻堅 撲薑и棻.
		pPlayer->stAccountInfo.uiChatRoomIDInInn	=	0;

		// 釭陛朝等 撩奢ц棻.		

		// 棻艇 Ы溯檜橫縑啪紫 檜 Ы溯檜橫陛 釭鬲棻堅 憲溥遽棻.
		// 薑溺и棻.
		uiPlayerCounter	=	0;
		for(i = 0; i < SOVILLAGE_MAX_INN_CHAT_NUM; i++)
		{
			if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount != 0)
			{
				// 渠褐 п渡 Ы溯檜橫陛 褒薯煎 襄營ж朝雖 憲嬴螞棻.
				// 褒薯煎 襄營ж朝 議葛攪虜 爾頂遽棻.
				if(clWorld->pPlayerOnAccount[pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount] != NULL)
				{
					uiPlayers[uiPlayerCounter] = pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].CRJPI[i].uiChatRoomJoinedPlayerAccount;
					uiPlayerCounter++;
				}					
			}					
		}


		if(uiPlayerCounter != 0)
			pPlayer->SendFieldMsg(ON_VIL_INN_LEAVE_PLAYER, SO_SFM_CUSTOM, LPVOID(uiPlayerCounter), LPVOID(uiPlayers));			

		// ⑷營 檜寞縑 霤罹и 識 餌辨濠曖 熱蒂 棻衛 嫡朝棻.
		pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum = uiPlayerCounter;		

		// Disconnect陛 嬴棲塭賊 釭縑啪 詭衛雖蒂 爾頂遽棻.
		if(bDisconnect == FALSE)
		{
			// ⑷營 渠晦褒縑 氈朝 寞曖 渠и 薑爾蒂 棻衛 爾頂遽棻.
			pPlayer->SendFieldMsg(ON_RESPONSE_VIL_INN_LEAVE_CHAT_ROOM, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->ChatRoom));
		}

		// 虜擒 寞縑 餌塋檜 綠歷棻賊 渠晦褒縑 寞檜 橈橫螺棻堅 ж堅
		// 渠晦褒縑 氈朝 餌辨濠縑啪 滲唳脹 薑爾蒂 爾頂遽棻.
		if(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum == 0)
		{
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
								LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
								LPVOID(ON_VWR_DELETEROOM), LPVOID(uiChatRoomID));			
		}
		else
		{
			// 寞檜 橈橫雖雖 彊懊擊 唳辦縑朝 寞縑 菟橫氈朝 Ы溯檜橫曖 熱蒂 爾頂遽棻.
			pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
									LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
									LPVOID(ON_VWR_CHANGEPLAYERNUM), LPVOID(uiChatRoomID), LPVOID(pstINNData->ChatRoom[CHATROOMIDTOINDEX(uiChatRoomID)].uiJoinedPlayerNum));
		}		

		if(bDisconnect == FALSE)
		{
			// Disconnect陛 嬴棲塭賊 渠晦褒縑 釭蒂 蹺陛и棻.
			pstINNData->WaitRoomUserManager.Add(pPlayer->uiAccount);			
		}

		return TRUE;
	}
	else
	{
		printf("檜寞縑 菟橫諦 氈朝 議葛攪陛 嬴椒\n");
		return FALSE;
	}
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	輿薄縑憮 ID蒂 薯剪и棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	渠�食磍� 虜萇棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::CreateChatRoomInInn(SOPlayer *pPlayer, CHAR *pszChatRoomName)
{
	BOOL	bSuccess = FALSE;	
	UINT	uiChatRoomIndex;

	if(pstINNData)
	{
		// ⑷營 Ы溯檜橫曖 鼻鷓陛 輿薄 渠晦褒縑 氈橫撿 и棻.		
		if(pPlayer->stAccountInfo.uiStatusInVillage == INVILLAGE_STATUS_INN && pPlayer->stAccountInfo.uiChatRoomIDInInn == 0)
		{
			// i朝 ID 檜嘎煎 
			for(uiChatRoomIndex = 0; uiChatRoomIndex < ON_MAX_CHATROOM_NUM; uiChatRoomIndex++)
			{
				// 綠橫氈朝 寞擊 堅艇棻.
				if(pstINNData->ChatRoom[uiChatRoomIndex].uiJoinedPlayerNum == 0)									
					break;				
			}

			// 綠橫氈朝 寞擊 瓊懊擊 唳辦縑虜.
			if(uiChatRoomIndex < ON_MAX_CHATROOM_NUM)
			{
				// 寞曖 檜葷擊 撲薑п遽棻.
				ZeroMemory(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, sizeof(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));			
				strcpy(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName, pszChatRoomName);				
				
				// 寞曖 ID蒂 撲薑и棻.
				pstINNData->ChatRoom[uiChatRoomIndex].dwUniqueID	=	clWorld->IncChatRoomInVillCounter();

				// 綠橫氈朝 寞擊 埤嫉戲賊擎 寞擊 虜菟溥朝 Ы溯檜橫蒂 寞寰縑 厥朝棻.
				bSuccess	=	JoinChatRoomInInn(CHATROOMINDEXTOID(uiChatRoomIndex), pPlayer);						

				if(bSuccess == TRUE)
				{
					// 斜葬堅 棻艇 餌辨濠縑啪 寞擊 虜菟歷棻堅 憲溥遽棻.				
					pPlayer->SendFieldMsg(ON_WAITROOM_CHANGE_INFO, SO_SFM_CUSTOM, 
										LPVOID(pstINNData->WaitRoomUserManager.m_NowUseDataCount), LPVOID(pstINNData->WaitRoomUserManager.m_Data),
										LPVOID(ON_VWR_CREATEROOM), 
										LPVOID(CHATROOMINDEXTOID(uiChatRoomIndex)),									// 0擎 縑楝羹觼蒂 嬪п憮 陴啖菩棻.
										LPVOID(pstINNData->ChatRoom[uiChatRoomIndex].szChatRoomName));					
				}
				else
				{
					printf("寞 儅撩 褒ぬ\n");
				}
			}
			else
			{
				printf("CreateChatRoomInInn :: 綠橫氈朝 寞檜 橈蝗棲棻\n");
			}
		}		
	}

	return bSuccess;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛っ縑 菟橫除棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterNoticeBoard(SOPlayer *pPlayer)
{
	// 啪衛っ縑朝 鼠褻勒 菟橫除棻.
	return pPlayer->SendFieldMsg(ON_RESPONSE_ENTER_NOTICEBOARD, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭擊 蛔煙и棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::RegistNotice(SOPlayer *pPlayer, CHAR *pszTitle, CHAR *pszNotice)
{	
	DBIKRegistryNoticeInfo		RegistryNoticeInfo;
	SI32						siNewIndex;
	
	// DB縑 蛔煙и棻.
	RegistryNoticeInfo.uiVillageCode	=	uiVillageCode;
	
	ZeroMemory(RegistryNoticeInfo.szID, sizeof(RegistryNoticeInfo.szID));
	if(pPlayer)
		strncpy(RegistryNoticeInfo.szID, pPlayer->szID, ON_ID_LENGTH);
	else
		strncpy(RegistryNoticeInfo.szID, "遴艙濠", ON_ID_LENGTH);

	RegistryNoticeInfo.pszTitle			=	pszTitle;
	RegistryNoticeInfo.pszNotice		=	pszNotice;

	ZeroMemory(RegistryNoticeInfo.szDate, sizeof(RegistryNoticeInfo.szDate));
	wsprintf(RegistryNoticeInfo.szDate, "%04d%02d%02d", clWorld->CurrentTime.Year, clWorld->CurrentTime.Month, clWorld->CurrentTime.Day);	
	
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_REGISTNOTICE, LPVOID(&RegistryNoticeInfo), LPVOID(&siNewIndex));

	// 旋擊 蹺陛и棻.(旋 廓�ㄧ� 嫡嬴撿 и棻.)
	pstINNData->NoticeNumber.Add(siNewIndex);
	
	// む檜雖 熱蒂 營 撲薑и棻.
	pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

	if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
		pstINNData->uiTotalPageNum++;

	if( pPlayer )
		return pPlayer->SendFieldMsg(ON_RESPONSE_REGIST_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK));
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	啪衛僭擊 熱薑и棻.
//	熱薑 橾濠	:	2002-04-15 螃�� 3:06:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::ModifyNotice(SOPlayer *pPlayer, DWORD dwIndex, CHAR *pszTitle, CHAR *pszContent)
{
	DBIKModifyNoticeInfo	ModifyNotice;
	BOOL					bModifySuccess;

	// 啪衛僭擊 熱薑и棻.

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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	啪衛僭擊 餉薯и棻.
//	熱薑 橾濠	:	2002-04-30 螃瞪 10:41:06 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DeleteNotice(SOPlayer *pPlayer, UI16 uiIndex)
{
	DBIKDeleteNotice	DeleteNotice;
	BOOL				bDBSuccess, bSuccess;	
	

	// 虜擒 輿薄檜 橈棻賊 斜傖 葬欐и棻.
	if(pstINNData == NULL)	return FALSE;

	// 啪衛僭擊 餉薯и棻.
	ZeroMemory(&DeleteNotice, sizeof(DeleteNotice));
	DeleteNotice.uiVillageCode	=	uiVillageCode;
	DeleteNotice.uiNoticeIndex	=	uiIndex;
	strncpy(DeleteNotice.szWroteID, pPlayer->szID, ON_ID_LENGTH);

	bDBSuccess	=	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_DELETENOTICE, LPVOID(&DeleteNotice), LPVOID(&bSuccess));

	if((bSuccess == TRUE) && (bDBSuccess == TRUE))
	{
		// 旋擊 餉薯и棻.(旋 廓�ㄧ� 嫡嬴撿 и棻.)
		pstINNData->NoticeNumber.DeleteType(uiIndex);		

		// む檜雖 熱蒂 營 撲薑и棻.
		pstINNData->uiTotalPageNum		=	pstINNData->NoticeNumber.m_NowUseDataCount / ON_SEND_NOTICESNUM_PER_ONCE;

		if(pstINNData->NoticeNumber.m_NowUseDataCount % ON_SEND_NOTICESNUM_PER_ONCE != 0)
			pstINNData->uiTotalPageNum++;

		pPlayer->SendFieldMsg(ON_RESPONSE_DELETE_NOTICE, SO_SFM_ONLYME, LPVOID(ON_RET_INN_OK), LPVOID(pstINNData->uiTotalPageNum));		
	}	

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭檜 葬蝶お蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::SendNoticesList(SOPlayer *pPlayer, UI16 uiPage)
{	
	DBIKGetNoticesHeaderInfoIn				GetNoticesHeaderInfoIn;
	DBIKGetNoticesHeaderInfoOut				GetNoticesHeaderInfoOut;	
	
	GetNoticesHeaderInfoIn.uiVillageCode			=	uiVillageCode;
	GetNoticesHeaderInfoIn.puiNoticesIndexArray		=	pstINNData->NoticeNumber.m_Data;
	GetNoticesHeaderInfoIn.siTopIndex				=	(pstINNData->NoticeNumber.m_NowUseDataCount - (ON_SEND_NOTICESNUM_PER_ONCE * (uiPage - 1))) - 1;
	
	// siTopIndex陛 嶸�褲狫� 彊戲賊 List蒂 爾頂雖 彊朝棻.
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
		printf("####### SOVillage::SendNoticesList ####### 澀跤脹 檣策蝶陛 菟橫諮蝗棲棻. : [%d] <= [%d]", 
							GetNoticesHeaderInfoIn.siTopIndex, pstINNData->NoticeNumber.m_NowUseDataCount);
		return FALSE;
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啪衛僭曖 頂辨僭擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿薄曖 瓣た ID陛 嶸�褲捘� 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::IsValidChatRoomIDInInn(UI16 uiChatRoomID)
{
	if((uiChatRoomID > 0) && (uiChatRoomID <= ON_MAX_CHATROOM_NUM))
		return TRUE;
	else
		return FALSE;
}
*/