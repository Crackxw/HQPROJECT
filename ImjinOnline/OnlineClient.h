#ifndef	_ONLINE_CLIENT_H
#define _ONLINE_CLIENT_H

#pragma warning ( disable : 4786 )

//---------------------------------------------------------------------------
#include <GSL.h>
#include "../SCCommon/Defines.h"
#include "../SCCommon/OnlineProtocol.h"
#include "../ImjinOnline/OnlineMyData.h"
#include "../SCCommon/OnlineCommonStruct.h"
#include "GSCProtocol.h"
#include "OnlineGameMsg.h"
#include "BlackPig.h"
#include "..\\SCCommon\\OnlineClientCallback.h"

/*
#include "../LoginServer/_ProtocolClient.h"
#include "../LoginServer/_PacketClient.h"
*/

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

//---------------------------------------------------------------------------

// �ӽ÷� �޼����� �����ϰ� �ִ� ����ü �̴�
struct  OnlineTempMsg
{
	BYTE	Data[ON_MAX_IN_BUFFER];
};

// �ӽ÷� �޼����� �����ϰ� ����ü �̴� 
typedef map<DWORD, OnlineTempMsg, less<DWORD> >	MsgBuffer;


struct	OnlineReceivedMovingDirection
{
	UI16	uiPlayer;
	SI32	siDirection;
};

struct	OnlineReceivedLogOut
{
	UI16	uiPlayer;
};

struct	OnlineReceivedChatting
{
	UI16	uiPlayer;	
	CHAR	szBuffer[1024];
};

struct	OnlineReceivedGotoVillage
{
	UI16	uiPlayer;
};

struct	OnlineReceiveBattleInfo
{
	CHAR						szBattleServerIP[16];
	SI16						siBattleServerChannelNum;	

	SI32						siPlayersID[8];

};

struct	OnlineReceiveThrowItemInfo
{
	UI16						uiPlayer;
	UI16						uiItemID;
	SI16						siX;
	SI16						siY;
	UI16						uiQuantity;
};

struct	OnlineReceivePickUpItemInfo
{
	SI16						siX;
	SI16						siY;
};

class OnlineClient : public COnlineClientCallback
{
public:	
	SI32				m_RecvDataSize;

	cltOnlineWorld		*pMyOnlineWorld;	
	MsgBuffer			m_pMsgBuffer;								// �޼��� ����	

	BOOL				m_BlackPigUse;
	CBlackPig			m_BlackPig;

public:
	// ���� ����
	CHAR				szUserID[ON_ACCOUNT_LENGTH + 1];
	CHAR				szUserPW[ON_PASS_LENGTH + 1];

	char				UserName[ON_ID_LENGTH + 1];			// �̿��� �̸�						
	char				IPAddress[30];	

	// ���� ����
	fd_set				fdError;
	fd_set				fdPlayer;
	SOCKET				Client;						
	timeval				timeout;

	struct sockaddr_in	Server;
    struct hostent*		Hp;
    unsigned long		Addr;

	// ���� ����
	BOOL				ConnectOK;								// �����ߴ°�?
	BOOL				GameConnectOK;							// ������ ����� ���¿��� ��Ʋ�� ���� ���ΰ�?		
	char				cInBuffer[ON_MAX_IN_BUFFER], cOutBuffer[ON_MAX_OUT_BUFFER];
	CRITICAL_SECTION	m_csOut;
	SI32				siInBufferLen, siOutBufferLen;			// ������ ����Ÿ ����		

	char				cMsg[ON_MAX_IN_BUFFER];
	BYTE				ReMsg[ON_MAX_IN_BUFFER];
	SI16				siMsgLengh;

	// ��ȣȭ Ű ���� ����
	DWORD				m_dwRoundKey[32];
	void				SetSeedKey(DWORD *dwKey)
	{
		if(dwKey != NULL)
		{
			memcpy(m_dwRoundKey, dwKey, sizeof(DWORD) * 32);
		}
	}
	
public:
	// �ʱ�ȭ�� ó������
	OnlineClient();
	~OnlineClient();
	VOID	InitZero();
	BOOL	Init(cltOnlineWorld	*pOnlineWorld, char IpAddr[], SI32 siPort, BOOL fPassword);
	BOOL	InitSocket();	
	VOID	Close();
	
	VOID	FreeDynamicBuffer();	
	VOID	AddMsgBuffer( DWORD dwMsgType, VOID *pData, SI32 siSize );

	BYTE*	GetMsgBuffer(DWORD dwMsgType);
	UI16	GetMsgIndex();

	//========================================================================================================
	// �Ϲ� Msg �ۼ��� �Լ���
	// �۽� �Լ�.

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////
	///�α��ΰ��� �ٲ����
	BOOL	SendGameServerLogIn();											// ���Ӽ����� �α����Ѵ�.
	BOOL	SendAccountLogIn(CHAR *pszID, CHAR *pszPW);						// �α����� ������ ������.
//	BOOL	SendLogInInWeb(CHAR *pszID, CHAR *pszPW);						// �α����� ������ ������.
	BOOL	SendLogOut();													// �α׾ƿ��� ������ ������.
	BOOL	SendClientKind();												// Ŭ���̾�Ʈ�� ������ ������.
	BOOL	SendRequestWorldInfo(void);										// ��� ������� ������ ��û�Ѵ�.
	BOOL	SendRequestCharInfo();											// ĳ���� ������ ��û�Ѵ�.
	BOOL	SendWorldID(BYTE BWorldID);										// ������ ������̵� ������.
	BOOL	SendCreateNewCharacter(CHAR *pszID, SI32 uiKind, 
			UI08 uiStr, UI08 uiDex, UI08 uiVit, UI08 uiInt);				// ĳ���͸� �����Ѵٰ� ������ �˷��ش�.
	BOOL	SendDeleteCharacter(UI08 uiSlot);								// ĳ���͸� �����Ѵ�.

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////

	BOOL	SendPlayerPos();														// �� �÷��̾��� ��ġ�� ������.
	BOOL	SendPlayerPos(SI16 siXPos, SI16 siYPos, BOOL bIsSiegeWarfareUnit);		// �� �÷��̾��� ��ġ�� ������.
	BOOL	SendMovingDirection();											// ���� �̵��ؾ� �ϴ� ������ �����ش�.
	BOOL	SendMovingPos();												// ���� �̵��ؾ� �ϴ� ������ ����Ͽ� �̵��ؾ� �ϴ� ��ġ�� �����ش�.
	BOOL	SendRequestPlayerPos(UI16 uiPlayer);							// Ư�� �÷��̾��� ��ġ�� �䱸�Ѵ�.
	BOOL	SendChatting(CHAR *pChatMessage);								// ���� Ÿ������ �޽����� ������.
	BOOL	SendGotoVillage(UI16 uiGotoVillageCode);						// ���� ������ ���ٴ� �޽����� �������� �˷��ش�.
	BOOL	SendGoOutVillage();												// ������ ������.
	BOOL	SendRequestItemList();											// �������� ����Ʈ�� �޶�� �䱸�Ѵ�.
	BOOL	SendBuyItem(OnReqBuyItemMsg* lpBuyItem);						// �������� ��ٰ� ������ ��û�Ѵ�.
	BOOL	SendSellItem(UI08 uiFollower, UI16 uiID, UI16 uiItemQuantity);	// �������� �Ǵٰ� ������ ��û�Ѵ�.	
	BOOL	SendBattle(SI32 siID);											// �ٸ� �÷��̾�� ������ �Ѵٰ� �������� �˷��ش�.
	BOOL	SendItemPosInInventory(UI08 uiFollower, UI08 uiReceiverID,
			UI16 uiID, UI16 siNewPos, SI16 siQuantity);						// �κ��丮�������� �� �������� ��ġ�� �������� �˷��ش�.
	BOOL	SendThrowItem(OnReqThrowItemMsg* lpOnReqThrowItemMsg);			// �������� ������.
	BOOL	SendPickUpItem(OnReqPickUpItemMsg* lpOnlineTempOnReqPickUpItemMsg);	// ���� ������ �ִ� �������� �־���.
	BOOL	SendReturnOnline(BOOL bWin);									// �ٽ� �¶������� ���ư����� �Ѵ�.
	BOOL	SendRequestPlayerInfo(UI16 uiCharNum, UI16 *puiIDIn);			// ĳ������ ������ �޶�� �����Ѵ�.
	BOOL	SendRequestEnterMarket();										// ����, ���忡 ����.
	BOOL	SendRequestLeaveMarket();										// ����, ���忡�� ������.
	BOOL	SendRequestBankInfo(void);																			// ����, ������ ���� ��û
	BOOL	SendRequestOpenSavingAccount(UI16 Kind, MONEY SavingMoney, BOOL fSavingTime, UI32 SavingTime);		// ���� ���� ��û
	BOOL	SendRequestMySavingAccount(void);																	// ���� Player�� ���� List�� ��û�Ѵ�.
	BOOL	SendRequestSavingAccount(OnAccountNumber AccountNumber, MONEY Money);								// �Ա�
	BOOL	SendRequestDrawingAccount(OnAccountNumber AccountNumber, MONEY Money);								// ���
	BOOL	SendCollateralLoanMsg(UI16 uiMonth,UI32 uiMoney,DWORD dwItemNum,UI16 *pID,UI16 *pCount,SI16 *pPos);	// ����, ���忡�� �㺸����
	BOOL	SendCollateralListMsg();																			// ����, ���忡�� �㺸�� ������ �޴´�.
	BOOL	SendReqAuditVillageInfo();																			// ���ڱ� ����
	BOOL	SendTakeCollateral( SI16 siNum, UI08 *puiFollowerID, UI16 *pItemID, UI16 *pCount, SI16 *pPos );		// ����, ���忡�� �㺸�� ã�´�.
	BOOL	SendActionInfo();																					// ����, ���� ��żҿ��� ��� >SendRequestBankInfo();������ ��û
	BOOL	SendActionBid( UI32 uiMoney );																		// ����, ���� ��żҿ��� ������ �θ���.
	BOOL	SendActionExit();																					// ����, ���� ��żҿ��� ������.
	BOOL	SendRequestEnterInn();											// ����, ������ ����.
	BOOL	SendRequestLeaveInn();											// ����, �������� ������.
	BOOL	SendRequestEnterInnVWR();										// ����, ������ ��ȭ�Ƿ� ����.
	BOOL	SendRequestJoinVilInnChatRoom(UI16 ChattingRoomID);				// ����, ������ ��ȭ�ǿ��� ��ȭ�濡 �����Ѵ�.
	BOOL	SendRequestLeaveVilInnChatRoom();								// ����, ������ ��ȭ�ǿ��� ��ȭ�濡�� ������.
	BOOL	SendMessage(char* lpMsg, SI16 Size);							// Message ����
	BOOL	SendRequestINNCreateChattingRoom(char* lpRoomName);				// ����, ������ ��ȭ�ǿ��� ä�ù� ����
	BOOL	SendRequestEnterInnNoticeBoard(DWORD Kind);						// ����, ������ �Խ��ǿ� ��
	BOOL	SendRequestModifyNotic(DWORD dwNoticeNum, UI08 uiTitleLength, 
				UI16 uiNoticeLength, const char* pTitle, const char* pNotice);	// ���� �Խ��ǿ��� �Խù� ������ ����
	BOOL	SendRequestDeleteNotice( DWORD dwPage, UI16 uiIndex );			// ������� ���� �����.
	BOOL	SendRequestSearchNotice( UI08 uiType, char *pMsg );				// �Խ��ǿ����� �˻�
	BOOL	SendRequestNoticesListMsg( UI16 uiPageID );						// �Խ��ǿ� ���� ��û �Ѵ�.
	BOOL	SendRequestViewNotice( UI16	uiIndex );							// �Խ����� �д´�.
	BOOL	SendGotoPortal(SI16 siID);										// ���� Portal�� ���ٴ� �޽����� �������� �˷��ش�.
	BOOL	SendRequestBarrackEnter(void);									// �Ʒüҿ� ����.
	BOOL	SendRequestBarrackBuySolder(SI32 Code, BYTE* lpName);			// �Ʒüҿ��� �뺴�� ����Ѵ�.
	BOOL	SendRequestBarrackDismissalEnter(void);							// �Ʒü� �ذ� ����.
	BOOL	SendRequestBarrackDismissalSolder(SI32 Slot);					// �Ʒüҿ��� �뺴�� �ذ��Ѵ�.
	
	BOOL	SendRequestGBarrackEnter(void);									// ����缺�ҿ� ����.
	BOOL	SendRequestGBarrackBuySolder(SI32 Code, BYTE* lpName);			// ����缺�ҿ��� ����� ����Ѵ�.
	BOOL	SendRequestGBarrackChangeJobEnter(void);						// ����缺�� ������ ����.
	BOOL	SendRequestGBarrackChangeJobSolder(SI32 Slot, SI32 Kind);		// ����缺�ҿ��� ����� ����Ѵ�.
	BOOL	SendRequestGBarrackDismissalEnter(void);						// ����缺�� �ذ� ����.
	BOOL	SendRequestGBarrackDismissalSolder(SI32 Slot);					// ����缺�ҿ��� ����� �ذ��Ѵ�.

	BOOL	SendRequestGBarrackGeneralExEnter(void);						// 2�� ���� ����缺�ҿ� ����.(��ư�� ������.)
	BOOL	SendRequestGBarrackBuyGeneralEx(SI32 Code, BYTE* lpName);		// 2�� ���� ����缺�ҿ��� 2�� ���� ����� ����Ѵ�.
	BOOL	SendRequestGBarrackChangeGeneralExEnter(void);					// 2�� ���� ����缺�ҿ��� ������ ����.
	BOOL	SendRequestGBarrackBuyChangeGeneralEx(SI32 Slot, SI32 Kind);    // 2�� ���� ����缺�ҿ��� ����� ����Ѵ�.
	BOOL	SendRequestGBarrackDismissalGeneralExEnter(void);				// 2�� ���� ����缺�ҿ��� �ذ� ����.
	BOOL	SendRequestGBarrackSellDismissalGeneralEx(SI32 Slot);			// 2�� ���� ����缺�ҿ��� ����� �ذ��Ѵ�.

	BOOL	SendRequestMBarrackEnter(void);									// ���;缺�ҿ� ����.
	BOOL	SendRequestMBarrackBuySolder(SI32 Code, BYTE* lpName);			// ���;缺�ҿ��� �뺴�� ����Ѵ�.
	BOOL	SendRequestMBarrackDismissalEnter(void);						// ���;缺�� �ذ� ����.
	BOOL	SendRequestMBarrackDismissalSolder(SI32 Slot);					// ���;缺�ҿ��� �뺴�� �ذ��Ѵ�.
	BOOL	SendRequestHospitalGetCureList(void);							// �ǿ����� ġ�� �뺴�� List�� ���´�.
	BOOL	SendRequestHospitalCureSolder(UI08 Slot);						// �ǿ����� �뺴�� ġ���Ѵ�.
	BOOL	SendRequestHospitalCureAllSolder(void);							// �ǿ����� ��� �뺴�� ġ���Ѵ�.
	BOOL	SendAuditAccountList(void);										// ���, �� ���¸� �䱸�Ѵ�.	
	BOOL	SendAuditOwnListMsg(void);										// ��ο��� �ε��� ����Ʈ�� �䱸 �Ѵ�.
	BOOL	SendAuditGuildMemberList(void);									// ��ο��� ��� ����Ʈ�� ���´�.
	BOOL	SendRequestWharfEnter(void);									// �εο� ����.
	BOOL	SendRequestWharfBuyTicket(SI32 TicketKind);						// �εο��� ǥ�� ���.
	BOOL	SendRequestWharfExit(void);										// �εο��� ������.
	BOOL	SendRequestWharfWaitRoomEnter(void);							// �εδ��ǿ� ����.
	BOOL	SendRequestWharfWaitRoomExit(void);								// �εδ��ǿ��� ������.

	BOOL	SendRequestFramInfo(UI08 uiPlantID);																	// ����ü� - ���� ������ ��û�� �Ѵ�.
	BOOL	SendRequestMakeBuy(UI08 uiPlantID, SI16 siSlot);														// ����ü� - �ǹ��� ���
	BOOL	SendRequestBuildStruct( SI16 siPay, SI16 siSlot, CHAR *pBuildKind );									// ����ü� - �ǹ��� �Ǽ�
	BOOL	SendRequestSlotInfo( UI08 uiPlantID, SI16 siSlot );														// ����ü� - ������ ������ ��û�Ѵ�.
	BOOL	SendRequestSetEquipment( UI08 uiPlantID, SI16 siFarm, SI16 siSlot, UI08 uiUpIndex );					// ����ü� - �ǹ��� ���׷��̵� �Ѵ�.
	BOOL	SendRequestStorageInfo( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex );								// ����ü� - â���� ������ ��û�Ѵ�.
	BOOL	SendRequestStorageInPut( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex, UI16 uiQunatity,				// ����ü� - â���� �������� �ִ´�.
									 UI08 uiFollowerID, SI16 siPosInInventory, UI16 uiItemCode );
	BOOL	SendRequestWorkInfo( UI08 uiPlantID, SI16 siFarm, UI08 uiSlot );										// ����ü� - �ǹ��� � �������� ���尡?
	BOOL	SendRequestMakeItem( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, UI16 uiItemCode, 
								UI32 uiQuantity, MONEY uiPay );														// ����ü� - �������� ���� �Ѵٰ� 
	BOOL	SendRequestWork( UI08 uiSlotIndex, UI08 uiGameType, SI32 siLabor, SI08 siStep );														// ����ü� - �뵿�� �Ѵ�.
	BOOL	SendRequestOutPut( UI08 uiPlantID,UI08 uiPartID,UI08 uiSlotIndex,UI08 uiFollowerID,UI16 uiItemCode, 
							   UI32 uiQunatity, SI16 siPosInInventory );											// ����ü� - �������� ������ ����.									
	BOOL	SendRequesExtension( UI08 uiPlantID, UI08 uiFarmIndex );												// ����ü� - ������ Ȯ���� �Ѵ�.
	BOOL	SendRequesDestroyStrcut( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex );							// ����ü� - ���Կ� �ִ� �ǹ��� ����
	BOOL	SendRequseUpgradeSlot( UI08 uiSlot, SI16 siPay );														// ����ü� - ������ ���� �Ѵ�.
	BOOL	SendRequseFarmWorkStartMsg( UI08 uiSlot );																// ����ü� - ���ϴ� ���� ���� �ִ� �˾� ����.
	BOOL	SendRequseChangePay( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, SI16 siPay );					// ����ü� - �ӱ��� ����
	BOOL	SendRequseCancel( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex );									// ����ü� - ���� ���
	BOOL	SendRequestLeaveStructure();																			// ����ü� - �������� �����ٰ� �����ش�.
	BOOL	SendRequestLeaveFactory();	// ����ü� - ���忡�� �����ٰ� �����ش�.

	// ��ż�

	BOOL	SendRequestAuctionSellItem(void);																		
	BOOL	SendRequestAuctionSellFollower(void);																	
	BOOL	SendRequestAuctionSellArticleList(void);																
	BOOL	SendRequestAuctionSellArticleDetailInfo(void);															
	BOOL	SendRequestAuctionBidding(void);


	BOOL	SendRequestItemInfo();																		// �����ϰ� �ִ� �������� �޴´�.
	BOOL	SendRequestAttachItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup );				// �ƾ��� ����
	BOOL	SendRequestStripItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup );				// ������ ����
	BOOL	SendRequestUseItem( UI08 uiFollower, SI16 siPos, UI32 uiID, UI08 uiNum );					// ������ ���

	BOOL	SendRequestGovernment_Main_Enter(void);														// ��û ����
	BOOL	SendRequestGovernment_Main_Leave(void);														// ��û ������
	BOOL	SendRequestGovernment_Invenstment_Enter(SI16 Type);											// ��û, ���� ����
	BOOL	SendRequestGovernment_Invenstment_Leave(void);												// ��û, ���� ������
	BOOL	SendRequestGovernment_Invenstment_GetList(SI16 Type);										// ��û, ���� List���
	BOOL	SendRequestGovernment_Invenstment_SetInvestment(SI16 Type, MONEY InvestmentMoney_InPocket,
															MONEY InvestmentMoney_InBank = 0);			// ��û, ���� �ϱ�

	BOOL	SendRequestVillageDescript_Read(UI16 uiVillageCode);											// ���� �Ұ��� �б�
	BOOL	SendRequestVillageDescript_Write(char* lpText);												// ���� �Ұ��� ����
	BOOL	SendRequestVillageData(UI16 uiVillageCode);													// ���� ���� ��û

	BOOL	SendRequestImoticon(UI16 ImoticonID);														// �̸�Ƽ�� ���

	BOOL	SendRequestAskTrade( UI16 uiID );															// �ŷ� ��û
	BOOL	SendRequestTrade(BOOL bAbccept);															// �ŷ� ��û ����

	BOOL	SendRequestAskParty( UI16 uiID );															// ��Ƽ �ý��� ��û.
	BOOL	SendRequestParty(DWORD dwAccept);																// ��Ƽ �ý��� ��û ����.
	BOOL	SendRequestQuitParty();																		// ��Ƽ���� Ż��.	
	BOOL	SendRequestPartyCancel();																	// ��Ƽ�� ��û �� ����� ��Ƽ�� ���.

	BOOL	SendRequestPreTrade(UI08 uiSlot, UI16 uiID ,UI16 uiItemNum, MONEY Money, 
			UI16 uiFollower, BOOL bProperty = FALSE);													// ������ �÷� ���´�.
	BOOL	SendRequestCancelTrade();																	// �ŷ� ���
	BOOL	SendRequestConfirmTrade( BOOL bFlag );														// �ŷ� ��ư�� ����
	BOOL	SendRequestTradeSuccess(BOOL bPush);														// ���� ��ư�� ����

	BOOL	SendGMMessage(char* lpChatting);															// ��� ���

	BOOL	SendBouns( UI08 uiFollowerID, AbilityKindType uiAbility );									// ���ʽ� ��ġ 

	BOOL	SendOpenBoothMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell);					// ���Ǵ뿡 ��� ��
	BOOL	SendChangeBoothItemMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell);				// ���Ǵ뿡 ��� ��
	BOOL	SendLeaveBoothMsg();																		// ���Ǵ뿡�� ������.
	BOOL	SendInputBoothDescMsg( UI16 uiBoothID, UI08 uiNum, UI08 uiLength, CHAR *pDesc );			// ������ ������ �ʾ� �ش�.
	BOOL	SendDisplayBoothItemMsg( UI16 uiBoothID );													// ���ǿ� �ִ� �������� ��û�� �Ѵ�.
	BOOL	SendGetBoothItemDescMsg( UI16 uiBoothID, UI08 uiNum );										// ���ǿ� �ִ� ������ ��� �´�.
	BOOL	SendGetBoothItemInfo(UI16 uiAccount);														// ���ǿ� �ִ� ������ ������ ���´�.(���� Ŭ��)
	BOOL	SendBuyBoothItem(UI16 uiAccount, UI16 uiQuantity, UI08 uiSlotID, UI08 uiInvenPos, UI16 uiItemID, UI08 uiFollowerID);// ���ǹ����� �����ϴ�.

	BOOL	SendRequestGovernment_VillageInfo(void);													// ���� ������ �޶�� ��û

	BOOL	SendRequestGovernment_GetDiffGuild(void);													// �ٸ� ��������� ����
	BOOL	SendRequestGovernment_GetJoinGuild(SI32 PageNum);											// ��ܿ� �����ҷ��� �÷��̾� ������ ����
	BOOL	SendRequestGovernment_GetDismissalGuild(SI32 PageNum, UI32 uiGuildID);						// ��ܿ��� �ذ��ҷ��� �÷��̾� ������ ����
	BOOL	SendRequestGovernment_MakeGuild(char* lpName);												// ����� ����
	BOOL	SendRequestGovernment_DeleteGuild(void);													// ����� ��ü
	BOOL	SendRequestGovernment_GuildJoinLeave(void);													// ��� ����/Ż��
	BOOL	SendRequestGovernment_JoinGuildPlayer(char* lpID);											// ��� ������ �����
	BOOL	SendRequestGovernment_JoinCancelGuildPlayer(char* lpID);									// ��� ������ �ź���
	BOOL	SendRequestGovernment_DismissalGuildPlayer(char* lpID);										// ��� Ż�� ��Ŵ
	BOOL	SendRequestGovernment_SetRelation(char* lpGuildID, SI32 RelationType);						// ��� ����, ���� ����
	BOOL	SendRequestGovernment_AppointClass(char* lpID, SI32 ClassCode);								// �����, �ൿ���� ���� ������ �Ӹ��Ѵ�.
	BOOL	SendRequestGovernment_DismissClass(char* lpID);												// �����, �ൿ���� ���� ���޿��� �����Ѵ�.
	BOOL	SendRequestGovernment_GetGuildVillageList(char* lpGuildID);									// ��� ��������Ʈ�� ���´�.
	BOOL	SendRequestGovernment_GetMaxVillageDefenceInvestment(void);									// ������ ���¿� ���ډƼ� �ִ� �ִ뵷�� ���´�.
	BOOL	SendRequestGovernment_SetVillageDefenceInvestment(MONEY DefenceMoney);						// ������ ���¿� �����Ѵ�.
	BOOL	SendRequestGovernment_GetVillageProfitStatistics(void);										// ������ ���� ��踦 ���´�.

	BOOL	SendRequestGovernment_InvestmentGetOut(MONEY GetOutMoney);									// ���ڱ��� �����Ѵ�.

	BOOL	SendRequestSetRename( CHAR *pRename, UI08 uiFollower );										// �뺴�� �̸��� ����

	BOOL	SendGameTickCount( DWORD TickCount, UI16 uiType );											// ���� TickCount�� ���� 
	BOOL	SendShortDistanceAttackVillage( UI16 VillageCode, SI16 siAttackType );						// ������ �����Ѵ�.(�ٰŸ�)

	BOOL	SendLongDistanceAttackVillage( UI16 VillageCode ,SI16 siAttackCharX, SI16 siAttackCharY);	// ������ �����Ѵ�.(���Ÿ�).
	BOOL	SendLongDistanceAttackVillageAction(UI16 VillageCode, SI16 siAttackType);					// ������ �����Ѵ�.Action(���Ÿ�).

	BOOL	SendFieldAttackReady(UI16 uiUniqueID,SI16 siAttackCharX,SI16 siAttackCharY);				// ������ �ΰ��� �����Ѵٴ� �޼����� ������ ������.
	BOOL	SendFieldAttackAction(UI16	uiEnemyCharID, SI16 siAttackType);								// ������ �ΰ��� �����Ѵ�.
	BOOL	SendFieldAttackDead(UI16 uiUniqueID);														// ������ �׾���.

	// robypark 2004/12/2 10:48
	// ������ ���� �Ŀ� ������ ���� ����Ʈ�� ��û�Ѵ�.
	BOOL	SendRequest_guild_villageattack_result(void);

	// robypark 2004/12/2 13:41
	// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
	BOOL	SendRequest_guild_choice_player(UI16 uiVillageCode, char *pszCharname);

	// robypark 2004/12/2 14:26
	// ����ڰ� �� �ִ� ������ ��ź ���� ������ �ִ��� ��û�Ѵ�.
	BOOL	SendRequest_village_investment_power(UI16 uiVillageCode, char *pszCharname);

	// robypark 2004/11/9 16:53
	// ������ ���� ���� �������� ����ǰ�� ��û�Ѵ�.
	// UI16 uiUniqueID_SupplyWagon:	������ ���޼��� ������ Unique ID
	// SI16 ssNeedsSupplyGoodsAmount: �ʿ��� ����ǰ ��
	BOOL	SendSiegeWarfareSupplyGoodsFromSupplyWagon(UI16 uiUniqueID_SupplyWagon, SI16 ssNeedsSupplyGoodsAmount);

	// robypark 2004/11/19 13:34
	// ������ ���� ���� �� ��û�ϱ�
	BOOL	SendVillageGarrisonCount(SI16 siVillageCode);

	// robypark 2004/11/25 15:51
	// ������ ȭģ���� �޽��� ������
	BOOL	SendRequestGovernment_OfferPeace(CHAR *szGuildID, CHAR *szMessage);

	// robypark 2004/11/25 16:16
	// ������ ȭģ���� ���� �޽��� ������
	BOOL	SendRequestGovernment_AcceptOfferedPeace(CHAR *szGuildID);

	// robypark 2004/11/25 16:16
	// ������ ȭģ���� ���� �޽��� ������
	BOOL	SendRequestGovernment_DeclineOfferedPeace(CHAR *szGuildID);

	// robypark 2004/12/21 16:30
	// ������ �߿� �Ϲ� ���� �� ������ Ư�� ��ġ(�ʺ��� ������)�� ���� ��û�ϱ�
	// �� ��û ������ ������ ��� ��ɾ��� /w(����)�� ���� ����Ѵ�.(ON_RESPONSE_WARP)
	BOOL	SendRequestSiegeWarfareWarp(UI16 uiUniqueID);

	// robypark 2005/1/5 17:4
	// ������ ������ ���¿��� �Ϲ� �������� ���ư��ڴٰ� ������ ��û
	BOOL	SendRequestNormalCharacter(UI16 uiUniqueID);

	// ������
	BOOL	SendEnterVillageDefenceInterface(UI16 uiVillageCode);										// ������ ���� �������̽��� ���ڴٰ� ��û
//	BOOL	SendEnterVillageFromVillageDefenceInterface(UI16 uiVillageCode);							// ������ ���� �������̽� �󿡼� ������ ���ڴٰ� ��û

	// ������ ���� �������̽� �󿡼� �����ڴٰ� ��û
	BOOL	SendLeaveVillageDefenceInterface(UI16 uiVillageCode);

	BOOL	SendCanChangeVillageAttackType(UI16 VillageCode);											// ������ ���� Ÿ���� ������ �������� ��û�ϱ�
	BOOL	SendVillagePlayerOilAttack(UI16  VillageCode);												// ������ �⸧ ���� �õ� ��û
	BOOL	SendVillagePlayerStoneAttack(UI16  VillageCode);											// ������ �� ���� �õ� ��û
	BOOL	SendVillageDefenceUp(UI16 VillageCode);														// ���� ���� ��� ��û�ϱ�

	// robypark 2005/1/28 18:3
	// ������ �������� �����ϰڴٰ� ������ ��û
	BOOL	SendTransformToSiegeUnit(SI16 siChangeKindIdx);

	// robypark 2005/1/29 14:13
	// ������ ���޼����� ���� 10�ʸ��� �ڵ� ȸ�� ��û
	BOOL	SendReuqestChargeMP(void);
	
	BOOL	SendIncreaseGuildUnit(SI16 siKind,SI16 siCount, SI16 siVillageCode);						// Guild�� �������ּ��� �ø���.
	BOOL	SendDecreaseGuildUnit(SI16 siKind,SI16 siCount);											// Guild�� ���������� �����ش�.

	BOOL	SendGetGuildUnitList(UI32 uiGuildID);														// Guild�� �������� List�� ���´�.

	BOOL	SendIncreaseGuildSupply(SI32 siSupply,SI16 siVillageCode);									// ���̳� ����� ���� ���� ���� ����ǰ ���� �ø���.
	BOOL	SendDecreaseGuildSupply(SI32 siSupply,SI16 siVillageCode);									// ����ǰ�� ���޼����� ��������. 
	BOOL	SendGetGuildSupplyList(SI16 siVillageCode);													// ����� ����ǰ ���� ���´�.

	BOOL	SendIncreaseVillageDefenceSoldier(SI16 siDefenceSoldier, SI16 siVillageCode);				// ���� ������ ���� ��Ų��.

	BOOL	SendChangeGuildFlag(SI32 siGuildFlag);														// Change Guild Flag
	BOOL	SendUsedFlag(void);																			// ���Ŵ� ��� ���
	BOOL	SendRPIInMinimap(CHAR *pszName);															// �̴ϸʿ��� ã���� �ϴ� �÷��̾� �̸��� ����
	BOOL	SendAdminMsg(void);																			// �˸��޼����� ��û�Ѵ�.

	BOOL	SendBattleMsg(char *pMsg, SI32 siSize) { return Write(pMsg, siSize, TRUE); };							// ���� �޼����� ���� �ش�.

	BOOL	SendRequestQuestSelectNPC(SI16 siNPCCode, SI32 siBookCount);								// NPC ����
	BOOL	SendRequestQuestAcceptQuest(SI32 siQuestCode);												// Quest ����
	BOOL	SendRequestQuestCancelQuest(SI32 siQuestCode);												// �������� Quest ���
	BOOL	SendRequestQuestGetPlayQuestList(void);														// �������� Quest List ��û
	BOOL	SendRequestQuestGetBankBookCount(void);														// ������� ���� ����

	BOOL	SendPlayerDetailInfo(UI16 uiID);															// �÷��̾��� ������ ��û�Ѵ�.

	BOOL	SendRequestServerData(void);

//	BOOL	SendRequestUseSkill(SI16 siSkillKind, SI16 siWhatDo);										// ��ų ����� ��û�Ѵ�.
//	BOOL	SendRequestLevelUp(SI16 siSkillKind);														// ��ų����� �ø���.

	BOOL	SendRequestProbeArea();																		// ����Ž�縦 ��û�Ѵ�.
	BOOL	SendRequestVillageInfo(SI16 siVillageSeries);												// ���������� ��û�Ѵ�.

	// �ٿ� ����
	BOOL	SendRequestBankSave(MONEY SavingMoney);																	
	BOOL	SendRequestBankWithdraw(MONEY DrawMoney);													

	BOOL	SendRequestInnItemList();																	// ������ ������ ����Ʈ�� ��û�Ѵ�.(�ָ�)
	BOOL	SendMoveInnItemToInven(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity);					// ������ �������� �κ��丮�� �ִ´�.(�ָ�)
	BOOL	SendMoveInvenItemToInn(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity);					// �������� �ָ� �κ��丮�� �ִ´�.(�ָ�)

	BOOL	SendRequestGuildInfo(CHAR *pszName);														// ��������� ��û�Ѵ�.
	BOOL	SendRequestDeleteVMercenary(void);															// ����ٴϴ� �뺴�� �����Ѵ�. 
	BOOL	SendRequestRefineItem(UI16 uiFollowerSlot, UI16 uiItemID);									// ������ ������ ��û�Ѵ�.
	BOOL	SendRequestGuildCreateCost();																// ��ܰ��� ����� ��û�Ѵ�.
	BOOL	SendRequest_Investment_Gains_GetList(int code);												//���� ����.

	BOOL	SendReqFriendList();								// ģ������Ʈ ��û
	BOOL	SendReqAddFriend(char* lpID);						// ģ�� �߰�
	BOOL	SendReqDeleteFriend(char* lpID, UI16 FriendNum);	// ģ�� ����
	BOOL	SendReqDeleteAllFriend();							// ��� ģ�� ����
	BOOL	SendReqSetDenialFriend(char* lpID, BOOL denial);	// ���Űź� ����

	BOOL	SendFarmWorkHack(UI08 uiType, SI08 siStep, SI32 siPoint1, SI32 siPoint2);
	BOOL	SendAttackMode(BOOL b);								// TRUE = ������� FALSE = ��ȭ���


	BOOL	SendGameGuardUserInfo(DWORD dwType, const char* szMsg);
	BOOL	SendMacroUsingUserInfo(const char* szMacroProgName);
	BOOL	SendGameGuardKeyResult(DWORD dwKey);

	// ��ü ó�� �Լ�
	UI16	DataProc();
	UI16	CommandProc();	

	//---------------------------------------------------------------------------
	// �⺻ �Լ�
	SI16	Read(SI32 siBytesToRead);
	BOOL	Write(const VOID * cMsg, UI16 iBytes, BOOL fPW = TRUE);
	BOOL	Readable();
	BOOL	Writeable();
	BOOL	FlushOutBuffer();
	VOID	EraseInBuffer(UI16 uiBytes);
	VOID	ResetSets();

	//---------------------------------------------------------------------------
	// �ܺ� ó�� �Լ�.
	VOID	GetID(CHAR *pszID);			// �� ID�� ���´�.

	VOID	PackMessage(char *cSrcMsg, SI16 siSrcSize, char *cPackMsg, SI16 *siPackSize);		// ���� �޼����� ��Ű¡ �Ѵ�.
	bool	UnpackMessage();																	// ���� ��ŷ�� �޼����� Ǭ��.
	BOOL    SendDummyPacket(void *pPacket,int size);
};

#endif
