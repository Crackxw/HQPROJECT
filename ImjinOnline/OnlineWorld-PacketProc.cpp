
#include <GSL.h>
#include	<Mouse.h>
#include	<clGame.h>
#include	<windows.h>

#include	"..\Gersang\music.h"
#include	<SoundManager.h>
#include	"..\Gersang\directx.h"
#include	"..\Gersang\Message.h"

#include	"OnlineVersion.h"
#include	"OnlineWorld.h"
#include	"OnlineImjin2I.h"
#include	<DebugTool.h>
#include	"OnlineETC.h"
#include	<KindInfo.h>

#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include	"OnlineHelp.h"
#include	"OnlineConfig.h"
#include	"OnlineMegaText.h"
#include	"OnlineNumberControl.h"
#include	"OnlineTradeBook.h"
#include	"OnlineVillageSave.h"
#include	"OnlineProductStructure.h"
#include	"OnlineTrade.h"

#include	"OnlineImjin2I.h"
#include	"OnlineWorld.h"
#include	"OnlineGameMsg.h"
#include	"OnlineTradeBook.h"
#include	"OnlineTrade.h"
#include	"OnlineNoticeList.h"
#include	"..\Gersang\SOCmdStruct.h"
#include	"OnlineClient.h"
#include	"OnlineVillage.h"
#include	"OnlineFieldChat.h"
#include	"OnlineMiniMap.h"
#include	<DebugTool.h>
#include	"OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineVillageManager.h"
#include	"OnlineCharUI.h"
#include	"VillageAttackInfo.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineBooth.h"
#include	"OnlineMessenger.h"
#include	"OnlineQuest-Parser.h"
#include	"OnlineQuestPlay.h"
#include	"clItemInfoWindow.h"
#include	"OnlineFieldArea.h"
#include	"_SkillDefine.h"
#include	"OnlineSkillParser.h"
#include	"_BuriedItemDefine.h"
#include    "_AuctionDefine.h"
#include    "OnlineFieldAuction.h"
#include	"tlhelp32.h"
#include	"OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include    "OnlinePannel.h"
#include	"OnlineMsgProc.h"

extern _clGame* pGame;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���� ���� �޽����� ó���Ѵ�.
UI16	cltOnlineWorld::ReceiveMessageProc(UI16 uiReceivedMessage)
{
	OnlineMsgProc::MsgProcessFnc Fnc = pOnlineMsgProc->GetFnc(uiReceivedMessage);
	if( Fnc )
	{
		return (pOnlineMsgProc->*Fnc)(pOnlineClient->GetMsgBuffer(uiReceivedMessage));
	}
	else
		pOnlineClient->GetMsgBuffer(uiReceivedMessage);
	return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Chatting������ ���� ���� �޽����� ó���Ѵ�.	
UI16	cltOnlineWorld::ReceiveMessageProcChatting(HQPacket* lpPacket)
{
	UI16		cmd;
	UI32		guid;

	char		strID[32];
	char		buf[256];
	ZeroMemory(strID, sizeof(strID));
	ZeroMemory(buf, sizeof(buf));

	if(lpPacket->ReadValue(&cmd, 2) == FALSE) return 0;	

	switch(cmd)	
	{
		case REPORT_LOGIN:
			{
				pOnlineClientChatting->m_fLogInSucess = TRUE;
				
				if(pOnlineVillage == NULL)
					pOnlineClientChatting->MessageSend_MoveSell(pOnlineMap->m_MapIndex, m_CharPosition_X, m_CharPosition_Y);
				else
					pOnlineClientChatting->MessageSend_NotIsMap();
			}
			break;

		case CHATMESSAGE:
			lpPacket->ReadValue(&guid, 4);
			lpPacket->ReadString(strID, 32);
			lpPacket->ReadString(buf, 256);

			if((siGamePollStatus == ON_GAMEPOLL_FIELD))
			{
				UI16		UniqueID;

				// ���� ��ȿ�� ������ΰ�. ����.
				if(guid != 0)
				{
					UniqueID = pIOnlineChar->GetIPToUniqueID(guid);
					if(UniqueID != 0)
					{
						if(pIOnlineChar->IsExistUniqueID(UniqueID) == TRUE)
						{
							// ����ڰ� ä���� �ߴ�.
							if(!strcmp(strID, pOnlineText->Get(8000520)))
								pIOnlineChar->SetSpeech(UniqueID, buf, TRUE, strID);
							else
		 						pIOnlineChar->SetSpeech(UniqueID, buf, FALSE, strID);

							// �ŷ����� ���� �ŷ������ ��ȭ�� ǥ���Ѵ�.
							if(pOnlineTrade->IsActive())
							{
								if(pOnlineTrade->FriendInfo.szCharName[0] != 0)
								{
									if(!strcmp(strID, pOnlineTrade->FriendInfo.szCharName))
										pOnlinePannel->AddChatID(strID, buf, CHATTINGMODE_NORMAL);
								}
							}
							// �ŷ����� �ƴҶ�
							else
								pOnlinePannel->AddChatID(strID, buf, CHATTINGMODE_NORMAL);
						}
						else
						{
							// ��ȿ�� ����ڰ� �ƴ϶�� �� ������� ������ �䱸�Ѵ�.
							//RPI.Add(UniqueID);
						}
					}
				}
				else
				{
					clGrp.Error("111", "GUID Error, %s, %s", strID, buf);
				}
			}
			break;

		case WHISPER:
			{
				lpPacket->ReadString(strID, 32);
				lpPacket->ReadString(buf, 256);

				// ���� �ָ���ȭ���̸�
				if(m_fChattingRoom)
				{
					OnVilInnChatRoomChatting		TempOnVilInnChatRoomChatting;

					ZeroMemory(&TempOnVilInnChatRoomChatting, sizeof(OnVilInnChatRoomChatting));
					TempOnVilInnChatRoomChatting.dwMsgType = ON_VIL_INN_CHATTING;
					TempOnVilInnChatRoomChatting.dwIP      = 0;
					strcpy((char*)TempOnVilInnChatRoomChatting.m_ID,       strID);
					strcpy((char*)TempOnVilInnChatRoomChatting.m_Chatting, buf);

					pOnlineClient->AddMsgBuffer(ON_VIL_INN_CHATTING, &TempOnVilInnChatRoomChatting, sizeof(OnVilInnChatRoomChatting));
				}
				else
				{
					BOOL	bRefusal = FALSE;
					char	TempID[ON_ID_LENGTH+1];
					ZeroMemory(TempID, ON_ID_LENGTH+1);
					strcpy(TempID, strID);

					// ģ����� ����Ʈ �ڽ��� �ӼӸ� ���� ���̵� ���
					ZeroMemory(pOnlineTradeBook->m_pFriendList->m_szFriendName, ON_ID_LENGTH+1);
					strncpy(pOnlineTradeBook->m_pFriendList->m_szFriendName, strID, ON_ID_LENGTH);


					//pOnlineTradeBook->m_pFriendList->m_pFriendEditBox->SetString(pOnlineTradeBook->m_pFriendList->m_szFriendName);
					OnlineEditControlEx	*pOecEx;
					pOecEx			= Handler_OnlineEditControlEx::GetControl( pOnlineTradeBook->GetHOECFriend());
					pOecEx->SetString(pOnlineTradeBook->m_pFriendList->m_szFriendName);

					// �ǳ� �Ӹ���� ����Ʈ�� �߰�
					pOnlinePannel->AddWhisperList(strID);

					// ���Űźλ��¶�� ���
					if(pMyData->m_BadFriendList.GetFriend(TempID) != NULL)
						bRefusal = TRUE;

					// ����Ʈ���� ���� �̸��� ��ã�´ٸ�
					if(!bRefusal && pOnlineMessenger->m_bWhisperOn)
					{
						if(!pOnlineMessenger->IsAction())
						{
							pOnlineMessenger->SetAction(TRUE);
							if(IsVillage())		pOnlineMessenger->SetMini(TRUE);
						}

						pOnlineMessenger->AddChat(strID, buf);
						pOnlineMessenger->SetStatus(MESSENGER_RECEIVE);
					}
				}
			}
			break;

		case REPORT_WHISPER:
			{
				if(m_fChattingRoom == FALSE)
				{
					lpPacket->ReadValue(&cmd, 2);

					switch(cmd)
					{
						case 0:		// ����
							SetMessage(pOnlineText->Get(ON_TEXT_DATA_EARCHATTINGOK));
							break;

						case 1:		// ����
							SetMessage(pOnlineText->Get(ON_TEXT_DATA_EARCHATTINGFAILE));
							break;
					}
				}
			}
			break;
			
		case CHATMESSAGE_ROOM:
			{
				if(m_fChattingRoom)
				{
					OnVilInnChatRoomChatting		TempOnVilInnChatRoomChatting;
					
					lpPacket->ReadValue(&guid, 4);
					lpPacket->ReadString(strID, 32);
					lpPacket->ReadString(buf, 256);
					
					ZeroMemory(&TempOnVilInnChatRoomChatting, sizeof(OnVilInnChatRoomChatting));
					TempOnVilInnChatRoomChatting.dwMsgType = ON_VIL_INN_CHATTING;
					TempOnVilInnChatRoomChatting.dwIP      = guid;
					strcpy((char*)TempOnVilInnChatRoomChatting.m_ID,       strID);
					strcpy((char*)TempOnVilInnChatRoomChatting.m_Chatting, buf);
					
					pOnlineClient->AddMsgBuffer(ON_VIL_INN_CHATTING, &TempOnVilInnChatRoomChatting, sizeof(OnVilInnChatRoomChatting));
				}
			}
			break;

	}

	return	0;
}

