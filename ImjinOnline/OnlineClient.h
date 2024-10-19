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

// 임시로 메세지를 저장하고 있는 구조체 이다
struct  OnlineTempMsg
{
	BYTE	Data[ON_MAX_IN_BUFFER];
};

// 임시로 메세지를 저장하고 구조체 이다 
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
	MsgBuffer			m_pMsgBuffer;								// 메세지 버퍼	

	BOOL				m_BlackPigUse;
	CBlackPig			m_BlackPig;

public:
	// 개인 정보
	CHAR				szUserID[ON_ACCOUNT_LENGTH + 1];
	CHAR				szUserPW[ON_PASS_LENGTH + 1];

	char				UserName[ON_ID_LENGTH + 1];			// 이용자 이름						
	char				IPAddress[30];	

	// 소켓 변수
	fd_set				fdError;
	fd_set				fdPlayer;
	SOCKET				Client;						
	timeval				timeout;

	struct sockaddr_in	Server;
    struct hostent*		Hp;
    unsigned long		Addr;

	// 공용 변수
	BOOL				ConnectOK;								// 접속했는가?
	BOOL				GameConnectOK;							// 서버와 연결된 상태에서 배틀넷 게임 중인가?		
	char				cInBuffer[ON_MAX_IN_BUFFER], cOutBuffer[ON_MAX_OUT_BUFFER];
	CRITICAL_SECTION	m_csOut;
	SI32				siInBufferLen, siOutBufferLen;			// 버퍼의 데이타 길이		

	char				cMsg[ON_MAX_IN_BUFFER];
	BYTE				ReMsg[ON_MAX_IN_BUFFER];
	SI16				siMsgLengh;

	// 암호화 키 저장 버퍼
	DWORD				m_dwRoundKey[32];
	void				SetSeedKey(DWORD *dwKey)
	{
		if(dwKey != NULL)
		{
			memcpy(m_dwRoundKey, dwKey, sizeof(DWORD) * 32);
		}
	}
	
public:
	// 초기화및 처리관련
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
	// 일반 Msg 송수신 함수들
	// 송신 함수.

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////
	///로그인관련 바뀐버젼
	BOOL	SendGameServerLogIn();											// 게임서버에 로그인한다.
	BOOL	SendAccountLogIn(CHAR *pszID, CHAR *pszPW);						// 로그인의 정보를 보낸다.
//	BOOL	SendLogInInWeb(CHAR *pszID, CHAR *pszPW);						// 로그인의 정보를 보낸다.
	BOOL	SendLogOut();													// 로그아웃의 정보를 보낸다.
	BOOL	SendClientKind();												// 클라이언트의 정보를 보낸다.
	BOOL	SendRequestWorldInfo(void);										// 모든 월드들의 정보를 요청한다.
	BOOL	SendRequestCharInfo();											// 캐릭터 정보를 요청한다.
	BOOL	SendWorldID(BYTE BWorldID);										// 선택한 월드아이디를 보낸다.
	BOOL	SendCreateNewCharacter(CHAR *pszID, SI32 uiKind, 
			UI08 uiStr, UI08 uiDex, UI08 uiVit, UI08 uiInt);				// 캐릭터를 생성한다고 서버에 알려준다.
	BOOL	SendDeleteCharacter(UI08 uiSlot);								// 캐릭터를 삭제한다.

	//////////////////////////////////////////////////
	//////////////////////////////////////////////////

	BOOL	SendPlayerPos();														// 내 플레이어의 위치를 보낸다.
	BOOL	SendPlayerPos(SI16 siXPos, SI16 siYPos, BOOL bIsSiegeWarfareUnit);		// 내 플레이어의 위치를 보낸다.
	BOOL	SendMovingDirection();											// 내가 이동해야 하는 방향을 보내준다.
	BOOL	SendMovingPos();												// 내가 이동해야 하는 방향을 사용하여 이동해야 하는 위치를 보내준다.
	BOOL	SendRequestPlayerPos(UI16 uiPlayer);							// 특정 플레이어의 위치를 요구한다.
	BOOL	SendChatting(CHAR *pChatMessage);								// 내가 타이핑한 메시지를 보낸다.
	BOOL	SendGotoVillage(UI16 uiGotoVillageCode);						// 내가 마을에 들어간다는 메시지를 서버에게 알려준다.
	BOOL	SendGoOutVillage();												// 마을을 나간다.
	BOOL	SendRequestItemList();											// 아이템의 리스트를 달라고 요구한다.
	BOOL	SendBuyItem(OnReqBuyItemMsg* lpBuyItem);						// 아이템을 산다고 서버에 요청한다.
	BOOL	SendSellItem(UI08 uiFollower, UI16 uiID, UI16 uiItemQuantity);	// 아이템을 판다고 서버에 요청한다.	
	BOOL	SendBattle(SI32 siID);											// 다른 플레이어와 전투를 한다고 서버에게 알려준다.
	BOOL	SendItemPosInInventory(UI08 uiFollower, UI08 uiReceiverID,
			UI16 uiID, UI16 siNewPos, SI16 siQuantity);						// 인벤토리내에서의 내 아이템의 위치를 서버에게 알려준다.
	BOOL	SendThrowItem(OnReqThrowItemMsg* lpOnReqThrowItemMsg);			// 아이템을 버린다.
	BOOL	SendPickUpItem(OnReqPickUpItemMsg* lpOnlineTempOnReqPickUpItemMsg);	// 땅에 떨어져 있는 아이템을 주었다.
	BOOL	SendReturnOnline(BOOL bWin);									// 다시 온라인으로 돌아가려고 한다.
	BOOL	SendRequestPlayerInfo(UI16 uiCharNum, UI16 *puiIDIn);			// 캐릭터의 정보를 달라고 설정한다.
	BOOL	SendRequestEnterMarket();										// 마을, 시장에 들어간다.
	BOOL	SendRequestLeaveMarket();										// 마을, 시장에서 나간다.
	BOOL	SendRequestBankInfo(void);																			// 마을, 전장의 정보 요청
	BOOL	SendRequestOpenSavingAccount(UI16 Kind, MONEY SavingMoney, BOOL fSavingTime, UI32 SavingTime);		// 계좌 개설 요청
	BOOL	SendRequestMySavingAccount(void);																	// 현재 Player의 계좌 List를 요청한다.
	BOOL	SendRequestSavingAccount(OnAccountNumber AccountNumber, MONEY Money);								// 입금
	BOOL	SendRequestDrawingAccount(OnAccountNumber AccountNumber, MONEY Money);								// 출금
	BOOL	SendCollateralLoanMsg(UI16 uiMonth,UI32 uiMoney,DWORD dwItemNum,UI16 *pID,UI16 *pCount,SI16 *pPos);	// 마을, 전장에서 담보대출
	BOOL	SendCollateralListMsg();																			// 마을, 전장에서 담보물 리스를 받는다.
	BOOL	SendReqAuditVillageInfo();																			// 투자금 정보
	BOOL	SendTakeCollateral( SI16 siNum, UI08 *puiFollowerID, UI16 *pItemID, UI16 *pCount, SI16 *pPos );		// 마을, 전장에서 담보를 찾는다.
	BOOL	SendActionInfo();																					// 마을, 전장 경매소에서 경매 >SendRequestBankInfo();정보를 요청
	BOOL	SendActionBid( UI32 uiMoney );																		// 마을, 전장 경매소에서 가격을 부른다.
	BOOL	SendActionExit();																					// 마을, 전장 경매소에서 나간다.
	BOOL	SendRequestEnterInn();											// 마을, 주점에 들어간다.
	BOOL	SendRequestLeaveInn();											// 마을, 주점에서 나간다.
	BOOL	SendRequestEnterInnVWR();										// 마을, 주점에 대화실로 들어간다.
	BOOL	SendRequestJoinVilInnChatRoom(UI16 ChattingRoomID);				// 마을, 주점에 대화실에서 대화방에 참여한다.
	BOOL	SendRequestLeaveVilInnChatRoom();								// 마을, 주점에 대화실에서 대화방에서 나간다.
	BOOL	SendMessage(char* lpMsg, SI16 Size);							// Message 전송
	BOOL	SendRequestINNCreateChattingRoom(char* lpRoomName);				// 마을, 주점에 대화실에서 채팅방 생성
	BOOL	SendRequestEnterInnNoticeBoard(DWORD Kind);						// 마을, 주점에 게시판에 들어감
	BOOL	SendRequestModifyNotic(DWORD dwNoticeNum, UI08 uiTitleLength, 
				UI16 uiNoticeLength, const char* pTitle, const char* pNotice);	// 마을 게시판에서 게시물 내용을 변경
	BOOL	SendRequestDeleteNotice( DWORD dwPage, UI16 uiIndex );			// 계시판의 글을 지운다.
	BOOL	SendRequestSearchNotice( UI08 uiType, char *pMsg );				// 게시판에서의 검색
	BOOL	SendRequestNoticesListMsg( UI16 uiPageID );						// 게시판에 글을 요청 한다.
	BOOL	SendRequestViewNotice( UI16	uiIndex );							// 게시판을 읽는다.
	BOOL	SendGotoPortal(SI16 siID);										// 내가 Portal에 들어간다는 메시지를 서버에게 알려준다.
	BOOL	SendRequestBarrackEnter(void);									// 훈련소에 들어간다.
	BOOL	SendRequestBarrackBuySolder(SI32 Code, BYTE* lpName);			// 훈련소에서 용병을 고용한다.
	BOOL	SendRequestBarrackDismissalEnter(void);							// 훈련소 해고에 들어간다.
	BOOL	SendRequestBarrackDismissalSolder(SI32 Slot);					// 훈련소에서 용병을 해고한다.
	
	BOOL	SendRequestGBarrackEnter(void);									// 장수양성소에 들어간다.
	BOOL	SendRequestGBarrackBuySolder(SI32 Code, BYTE* lpName);			// 장수양성소에서 장수을 고용한다.
	BOOL	SendRequestGBarrackChangeJobEnter(void);						// 장수양성소 전직에 들어간다.
	BOOL	SendRequestGBarrackChangeJobSolder(SI32 Slot, SI32 Kind);		// 장수양성소에서 장수을 고용한다.
	BOOL	SendRequestGBarrackDismissalEnter(void);						// 장수양성소 해고에 들어간다.
	BOOL	SendRequestGBarrackDismissalSolder(SI32 Slot);					// 장수양성소에서 장수을 해고한다.

	BOOL	SendRequestGBarrackGeneralExEnter(void);						// 2차 전직 장수양성소에 들어간다.(버튼을 누른다.)
	BOOL	SendRequestGBarrackBuyGeneralEx(SI32 Code, BYTE* lpName);		// 2차 전직 장수양성소에서 2차 전직 장수를 고용한다.
	BOOL	SendRequestGBarrackChangeGeneralExEnter(void);					// 2차 전직 장수양성소에서 전직에 들어간다.
	BOOL	SendRequestGBarrackBuyChangeGeneralEx(SI32 Slot, SI32 Kind);    // 2차 전직 장수양성소에서 장수를 고용한다.
	BOOL	SendRequestGBarrackDismissalGeneralExEnter(void);				// 2차 전직 장수양성소에서 해고에 들어간다.
	BOOL	SendRequestGBarrackSellDismissalGeneralEx(SI32 Slot);			// 2차 전직 장수양성소에서 장수를 해고한다.

	BOOL	SendRequestMBarrackEnter(void);									// 몬스터양성소에 들어간다.
	BOOL	SendRequestMBarrackBuySolder(SI32 Code, BYTE* lpName);			// 몬스터양성소에서 용병을 고용한다.
	BOOL	SendRequestMBarrackDismissalEnter(void);						// 몬스터양성소 해고에 들어간다.
	BOOL	SendRequestMBarrackDismissalSolder(SI32 Slot);					// 몬스터양성소에서 용병을 해고한다.
	BOOL	SendRequestHospitalGetCureList(void);							// 의원에서 치료 용병의 List를 얻어온다.
	BOOL	SendRequestHospitalCureSolder(UI08 Slot);						// 의원에서 용병을 치료한다.
	BOOL	SendRequestHospitalCureAllSolder(void);							// 의원에서 모든 용병을 치료한다.
	BOOL	SendAuditAccountList(void);										// 장부, 총 계좌를 요구한다.	
	BOOL	SendAuditOwnListMsg(void);										// 장부에서 부동산 리스트를 요구 한다.
	BOOL	SendAuditGuildMemberList(void);									// 장부에서 길드 리스트를 얻어온다.
	BOOL	SendRequestWharfEnter(void);									// 부두에 들어간다.
	BOOL	SendRequestWharfBuyTicket(SI32 TicketKind);						// 부두에서 표를 산다.
	BOOL	SendRequestWharfExit(void);										// 부두에서 나간다.
	BOOL	SendRequestWharfWaitRoomEnter(void);							// 부두대기실에 들어간다.
	BOOL	SendRequestWharfWaitRoomExit(void);								// 부두대기실에서 나간다.

	BOOL	SendRequestFramInfo(UI08 uiPlantID);																	// 생산시설 - 농장 정보를 요청을 한다.
	BOOL	SendRequestMakeBuy(UI08 uiPlantID, SI16 siSlot);														// 생산시설 - 건물을 산다
	BOOL	SendRequestBuildStruct( SI16 siPay, SI16 siSlot, CHAR *pBuildKind );									// 생산시설 - 건물을 건설
	BOOL	SendRequestSlotInfo( UI08 uiPlantID, SI16 siSlot );														// 생산시설 - 슬롯의 정보를 요청한다.
	BOOL	SendRequestSetEquipment( UI08 uiPlantID, SI16 siFarm, SI16 siSlot, UI08 uiUpIndex );					// 생산시설 - 건물을 업그레이드 한다.
	BOOL	SendRequestStorageInfo( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex );								// 생산시설 - 창고의 정보를 요청한다.
	BOOL	SendRequestStorageInPut( UI08 uiPlantID, UI08 uiPartID, UI08 uiSlotIndex, UI16 uiQunatity,				// 생산시설 - 창고의 아이템을 넣는다.
									 UI08 uiFollowerID, SI16 siPosInInventory, UI16 uiItemCode );
	BOOL	SendRequestWorkInfo( UI08 uiPlantID, SI16 siFarm, UI08 uiSlot );										// 생산시설 - 건물이 어떤 아이템을 만드가?
	BOOL	SendRequestMakeItem( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, UI16 uiItemCode, 
								UI32 uiQuantity, MONEY uiPay );														// 생산시설 - 아이템을 생산 한다고 
	BOOL	SendRequestWork( UI08 uiSlotIndex, UI08 uiGameType, SI32 siLabor, SI08 siStep );														// 생산시설 - 노동을 한다.
	BOOL	SendRequestOutPut( UI08 uiPlantID,UI08 uiPartID,UI08 uiSlotIndex,UI08 uiFollowerID,UI16 uiItemCode, 
							   UI32 uiQunatity, SI16 siPosInInventory );											// 생산시설 - 아이템을 가지고 간다.									
	BOOL	SendRequesExtension( UI08 uiPlantID, UI08 uiFarmIndex );												// 생산시설 - 슬롯을 확장을 한다.
	BOOL	SendRequesDestroyStrcut( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex );							// 생산시설 - 슬롯에 있는 건물을 제거
	BOOL	SendRequseUpgradeSlot( UI08 uiSlot, SI16 siPay );														// 생산시설 - 슬롯을 업글 한다.
	BOOL	SendRequseFarmWorkStartMsg( UI08 uiSlot );																// 생산시설 - 일하는 곳에 도구 있는 알아 본다.
	BOOL	SendRequseChangePay( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex, SI16 siPay );					// 생산시설 - 임금을 변경
	BOOL	SendRequseCancel( UI08 uiPlantID, UI08 uiFarmIndex, UI08 uiSlotIndex );									// 생산시설 - 생산 취소
	BOOL	SendRequestLeaveStructure();																			// 생산시설 - 단지에서 나간다고 보내준다.
	BOOL	SendRequestLeaveFactory();	// 생산시설 - 공장에서 나간다고 보내준다.

	// 경매소

	BOOL	SendRequestAuctionSellItem(void);																		
	BOOL	SendRequestAuctionSellFollower(void);																	
	BOOL	SendRequestAuctionSellArticleList(void);																
	BOOL	SendRequestAuctionSellArticleDetailInfo(void);															
	BOOL	SendRequestAuctionBidding(void);


	BOOL	SendRequestItemInfo();																		// 착용하고 있는 아이템을 받는다.
	BOOL	SendRequestAttachItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup );				// 아아템 착용
	BOOL	SendRequestStripItem( UI08 uiFollower, SI16 siPos, UI32 uiID, SI16 siGroup );				// 아이템 벗다
	BOOL	SendRequestUseItem( UI08 uiFollower, SI16 siPos, UI32 uiID, UI08 uiNum );					// 아이템 사용

	BOOL	SendRequestGovernment_Main_Enter(void);														// 관청 들어가기
	BOOL	SendRequestGovernment_Main_Leave(void);														// 관청 나가기
	BOOL	SendRequestGovernment_Invenstment_Enter(SI16 Type);											// 관청, 투자 들어가기
	BOOL	SendRequestGovernment_Invenstment_Leave(void);												// 관청, 투자 나가기
	BOOL	SendRequestGovernment_Invenstment_GetList(SI16 Type);										// 관청, 투자 List얻기
	BOOL	SendRequestGovernment_Invenstment_SetInvestment(SI16 Type, MONEY InvestmentMoney_InPocket,
															MONEY InvestmentMoney_InBank = 0);			// 관청, 투자 하기

	BOOL	SendRequestVillageDescript_Read(UI16 uiVillageCode);											// 마을 소개문 읽기
	BOOL	SendRequestVillageDescript_Write(char* lpText);												// 마을 소개문 쓰기
	BOOL	SendRequestVillageData(UI16 uiVillageCode);													// 마을 정보 요청

	BOOL	SendRequestImoticon(UI16 ImoticonID);														// 이모티콘 사용

	BOOL	SendRequestAskTrade( UI16 uiID );															// 거래 요청
	BOOL	SendRequestTrade(BOOL bAbccept);															// 거래 요청 응답

	BOOL	SendRequestAskParty( UI16 uiID );															// 파티 시스템 요청.
	BOOL	SendRequestParty(DWORD dwAccept);																// 파티 시스템 요청 응답.
	BOOL	SendRequestQuitParty();																		// 파티에서 탈퇴.	
	BOOL	SendRequestPartyCancel();																	// 파티를 요청 한 사람이 파티를 취소.

	BOOL	SendRequestPreTrade(UI08 uiSlot, UI16 uiID ,UI16 uiItemNum, MONEY Money, 
			UI16 uiFollower, BOOL bProperty = FALSE);													// 물건을 올려 놓는다.
	BOOL	SendRequestCancelTrade();																	// 거래 취소
	BOOL	SendRequestConfirmTrade( BOOL bFlag );														// 거래 버튼를 눌름
	BOOL	SendRequestTradeSuccess(BOOL bPush);														// 수락 버튼을 눌름

	BOOL	SendGMMessage(char* lpChatting);															// 운영자 명령

	BOOL	SendBouns( UI08 uiFollowerID, AbilityKindType uiAbility );									// 보너스 수치 

	BOOL	SendOpenBoothMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell);					// 좌판대에 들어 감
	BOOL	SendChangeBoothItemMsg(UI16 uiItemNum, OnBoothItemForSell *pBoothItemForSell);				// 좌판대에 들어 감
	BOOL	SendLeaveBoothMsg();																		// 좌판대에서 나간다.
	BOOL	SendInputBoothDescMsg( UI16 uiBoothID, UI08 uiNum, UI08 uiLength, CHAR *pDesc );			// 아이템 설명을 너어 준다.
	BOOL	SendDisplayBoothItemMsg( UI16 uiBoothID );													// 좌판에 있는 아이템을 요청을 한다.
	BOOL	SendGetBoothItemDescMsg( UI16 uiBoothID, UI08 uiNum );										// 좌판에 있는 설명을 얻어 온다.
	BOOL	SendGetBoothItemInfo(UI16 uiAccount);														// 좌판에 있는 아이템 정보를 얻어온다.(좌판 클릭)
	BOOL	SendBuyBoothItem(UI16 uiAccount, UI16 uiQuantity, UI08 uiSlotID, UI08 uiInvenPos, UI16 uiItemID, UI08 uiFollowerID);// 좌판물건을 구입하다.

	BOOL	SendRequestGovernment_VillageInfo(void);													// 마을 정보를 달라고 요청

	BOOL	SendRequestGovernment_GetDiffGuild(void);													// 다른 상단정보를 얻어옴
	BOOL	SendRequestGovernment_GetJoinGuild(SI32 PageNum);											// 상단에 가입할려는 플레이어 정보를 얻어옴
	BOOL	SendRequestGovernment_GetDismissalGuild(SI32 PageNum, UI32 uiGuildID);						// 상단에서 해고할려는 플레이어 정보를 얻어옴
	BOOL	SendRequestGovernment_MakeGuild(char* lpName);												// 상단을 만듬
	BOOL	SendRequestGovernment_DeleteGuild(void);													// 상단을 해체
	BOOL	SendRequestGovernment_GuildJoinLeave(void);													// 상단 가입/탈퇴
	BOOL	SendRequestGovernment_JoinGuildPlayer(char* lpID);											// 상단 가입을 허락함
	BOOL	SendRequestGovernment_JoinCancelGuildPlayer(char* lpID);									// 상단 가입을 거부함
	BOOL	SendRequestGovernment_DismissalGuildPlayer(char* lpID);										// 상단 탈퇴를 시킴
	BOOL	SendRequestGovernment_SetRelation(char* lpGuildID, SI32 RelationType);						// 상단 동맹, 적대 설정
	BOOL	SendRequestGovernment_AppointClass(char* lpID, SI32 ClassCode);								// 대행수, 행동대장 등의 직급을 임명한다.
	BOOL	SendRequestGovernment_DismissClass(char* lpID);												// 대행수, 행동대장 등의 직급에서 해임한다.
	BOOL	SendRequestGovernment_GetGuildVillageList(char* lpGuildID);									// 상단 마을리스트를 얻어온다.
	BOOL	SendRequestGovernment_GetMaxVillageDefenceInvestment(void);									// 마을의 방어력에 투자됄수 있는 최대돈을 얻어온다.
	BOOL	SendRequestGovernment_SetVillageDefenceInvestment(MONEY DefenceMoney);						// 마을의 방어력에 투자한다.
	BOOL	SendRequestGovernment_GetVillageProfitStatistics(void);										// 마을의 수익 통계를 얻어온다.

	BOOL	SendRequestGovernment_InvestmentGetOut(MONEY GetOutMoney);									// 투자금을 인출한다.

	BOOL	SendRequestSetRename( CHAR *pRename, UI08 uiFollower );										// 용병의 이름을 변경

	BOOL	SendGameTickCount( DWORD TickCount, UI16 uiType );											// 현재 TickCount를 전송 
	BOOL	SendShortDistanceAttackVillage( UI16 VillageCode, SI16 siAttackType );						// 마을을 공격한다.(근거리)

	BOOL	SendLongDistanceAttackVillage( UI16 VillageCode ,SI16 siAttackCharX, SI16 siAttackCharY);	// 마을을 공격한다.(원거리).
	BOOL	SendLongDistanceAttackVillageAction(UI16 VillageCode, SI16 siAttackType);					// 마을을 공격한다.Action(원거리).

	BOOL	SendFieldAttackReady(UI16 uiUniqueID,SI16 siAttackCharX,SI16 siAttackCharY);				// 공성시 인간을 공격한다는 메세지를 서버로 날린다.
	BOOL	SendFieldAttackAction(UI16	uiEnemyCharID, SI16 siAttackType);								// 공성시 인간을 공격한다.
	BOOL	SendFieldAttackDead(UI16 uiUniqueID);														// 공성시 죽었다.

	// robypark 2004/12/2 10:48
	// 공성전 종료 후에 점령한 마을 리스트를 요청한다.
	BOOL	SendRequest_guild_villageattack_result(void);

	// robypark 2004/12/2 13:41
	// 공성전 종료 후에 마을에 대한 폭투 권한을 특정 캐릭터에 주기 요청
	BOOL	SendRequest_guild_choice_player(UI16 uiVillageCode, char *pszCharname);

	// robypark 2004/12/2 14:26
	// 사용자가 들어가 있는 마을에 폭탄 투자 권한이 있는지 요청한다.
	BOOL	SendRequest_village_investment_power(UI16 uiVillageCode, char *pszCharname);

	// robypark 2004/11/9 16:53
	// 공성전 유닛 보급 수레에게 보급품을 요청한다.
	// UI16 uiUniqueID_SupplyWagon:	공성전 보급수레 유닛의 Unique ID
	// SI16 ssNeedsSupplyGoodsAmount: 필요한 보급품 양
	BOOL	SendSiegeWarfareSupplyGoodsFromSupplyWagon(UI16 uiUniqueID_SupplyWagon, SI16 ssNeedsSupplyGoodsAmount);

	// robypark 2004/11/19 13:34
	// 공성전 마을 수비병 수 요청하기
	BOOL	SendVillageGarrisonCount(SI16 siVillageCode);

	// robypark 2004/11/25 15:51
	// 공성전 화친제의 메시지 보내기
	BOOL	SendRequestGovernment_OfferPeace(CHAR *szGuildID, CHAR *szMessage);

	// robypark 2004/11/25 16:16
	// 공성전 화친제의 수락 메시지 보내기
	BOOL	SendRequestGovernment_AcceptOfferedPeace(CHAR *szGuildID);

	// robypark 2004/11/25 16:16
	// 공성전 화친제의 거절 메시지 보내기
	BOOL	SendRequestGovernment_DeclineOfferedPeace(CHAR *szGuildID);

	// robypark 2004/12/21 16:30
	// 공성전 중에 일반 유닛 각 국가의 특정 위치(초보자 수련장)로 워프 요청하기
	// 이 요청 응답은 기존의 운영자 명령어인 /w(워프)의 것을 사용한다.(ON_RESPONSE_WARP)
	BOOL	SendRequestSiegeWarfareWarp(UI16 uiUniqueID);

	// robypark 2005/1/5 17:4
	// 공성전 유닛인 상태에서 일반 유닛으로 돌아가겠다고 서버에 요청
	BOOL	SendRequestNormalCharacter(UI16 uiUniqueID);

	// 공성전
	BOOL	SendEnterVillageDefenceInterface(UI16 uiVillageCode);										// 공성전 수성 인터페이스로 들어가겠다고 요청
//	BOOL	SendEnterVillageFromVillageDefenceInterface(UI16 uiVillageCode);							// 공성전 수성 인터페이스 상에서 마을에 들어가겠다고 요청

	// 공성전 수성 인터페이스 상에서 나가겠다고 요청
	BOOL	SendLeaveVillageDefenceInterface(UI16 uiVillageCode);

	BOOL	SendCanChangeVillageAttackType(UI16 VillageCode);											// 마을의 공격 타입을 변경이 가능한지 요청하기
	BOOL	SendVillagePlayerOilAttack(UI16  VillageCode);												// 마을이 기름 공격 시도 요청
	BOOL	SendVillagePlayerStoneAttack(UI16  VillageCode);											// 마을이 돌 공격 시도 요청
	BOOL	SendVillageDefenceUp(UI16 VillageCode);														// 마을 방어력 향상 요청하기

	// robypark 2005/1/28 18:3
	// 공성전 유닛으로 변신하겠다고 서버에 요청
	BOOL	SendTransformToSiegeUnit(SI16 siChangeKindIdx);

	// robypark 2005/1/29 14:13
	// 공성전 보급수레가 마나 10초마다 자동 회복 요청
	BOOL	SendReuqestChargeMP(void);
	
	BOOL	SendIncreaseGuildUnit(SI16 siKind,SI16 siCount, SI16 siVillageCode);						// Guild의 공성유닛수를 늘린다.
	BOOL	SendDecreaseGuildUnit(SI16 siKind,SI16 siCount);											// Guild의 공성유닛을 나눠준다.

	BOOL	SendGetGuildUnitList(UI32 uiGuildID);														// Guild의 공성유닛 List를 얻어온다.

	BOOL	SendIncreaseGuildSupply(SI32 siSupply,SI16 siVillageCode);									// 방이나 행수가 돈을 질러 마을 보급품 수를 늘린다.
	BOOL	SendDecreaseGuildSupply(SI32 siSupply,SI16 siVillageCode);									// 보급품을 보급수레가 가져간다. 
	BOOL	SendGetGuildSupplyList(SI16 siVillageCode);													// 길드의 보급품 양을 얻어온다.

	BOOL	SendIncreaseVillageDefenceSoldier(SI16 siDefenceSoldier, SI16 siVillageCode);				// 마을 수비병을 증가 시킨다.

	BOOL	SendChangeGuildFlag(SI32 siGuildFlag);														// Change Guild Flag
	BOOL	SendUsedFlag(void);																			// 사용돼는 깃발 얻기
	BOOL	SendRPIInMinimap(CHAR *pszName);															// 미니맵에서 찾고자 하는 플레이어 이름을 전송
	BOOL	SendAdminMsg(void);																			// 알림메세지를 요청한다.

	BOOL	SendBattleMsg(char *pMsg, SI32 siSize) { return Write(pMsg, siSize, TRUE); };							// 전투 메세지를 보내 준다.

	BOOL	SendRequestQuestSelectNPC(SI16 siNPCCode, SI32 siBookCount);								// NPC 선택
	BOOL	SendRequestQuestAcceptQuest(SI32 siQuestCode);												// Quest 수락
	BOOL	SendRequestQuestCancelQuest(SI32 siQuestCode);												// 진행중인 Quest 취소
	BOOL	SendRequestQuestGetPlayQuestList(void);														// 진행중인 Quest List 요청
	BOOL	SendRequestQuestGetBankBookCount(void);														// 은행계좌 수를 얻어옴

	BOOL	SendPlayerDetailInfo(UI16 uiID);															// 플레이어의 정보를 요청한다.

	BOOL	SendRequestServerData(void);

//	BOOL	SendRequestUseSkill(SI16 siSkillKind, SI16 siWhatDo);										// 스킬 사용을 요청한다.
//	BOOL	SendRequestLevelUp(SI16 siSkillKind);														// 시킬등급을 올린다.

	BOOL	SendRequestProbeArea();																		// 지역탐사를 요청한다.
	BOOL	SendRequestVillageInfo(SI16 siVillageSeries);												// 마을정보를 요청한다.

	// 꾸오 전장
	BOOL	SendRequestBankSave(MONEY SavingMoney);																	
	BOOL	SendRequestBankWithdraw(MONEY DrawMoney);													

	BOOL	SendRequestInnItemList();																	// 구입한 아이템 리스트를 요청한다.(주막)
	BOOL	SendMoveInnItemToInven(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity);					// 구입한 아이템을 인벤토리에 넣는다.(주막)
	BOOL	SendMoveInvenItemToInn(UI08 uiFollowerID, UI16 uiItemID, UI16 uiQuantity);					// 아이템을 주막 인벤토리에 넣는다.(주막)

	BOOL	SendRequestGuildInfo(CHAR *pszName);														// 상단정보를 요청한다.
	BOOL	SendRequestDeleteVMercenary(void);															// 따라다니는 용병을 삭제한다. 
	BOOL	SendRequestRefineItem(UI16 uiFollowerSlot, UI16 uiItemID);									// 아이템 제련을 요청한다.
	BOOL	SendRequestGuildCreateCost();																// 상단개설 비용을 요청한다.
	BOOL	SendRequest_Investment_Gains_GetList(int code);												//투자 수익.

	BOOL	SendReqFriendList();								// 친구리스트 요청
	BOOL	SendReqAddFriend(char* lpID);						// 친구 추가
	BOOL	SendReqDeleteFriend(char* lpID, UI16 FriendNum);	// 친구 삭제
	BOOL	SendReqDeleteAllFriend();							// 모든 친구 삭제
	BOOL	SendReqSetDenialFriend(char* lpID, BOOL denial);	// 수신거부 설정

	BOOL	SendFarmWorkHack(UI08 uiType, SI08 siStep, SI32 siPoint1, SI32 siPoint2);
	BOOL	SendAttackMode(BOOL b);								// TRUE = 전투모드 FALSE = 평화모드


	BOOL	SendGameGuardUserInfo(DWORD dwType, const char* szMsg);
	BOOL	SendMacroUsingUserInfo(const char* szMacroProgName);
	BOOL	SendGameGuardKeyResult(DWORD dwKey);

	// 전체 처리 함수
	UI16	DataProc();
	UI16	CommandProc();	

	//---------------------------------------------------------------------------
	// 기본 함수
	SI16	Read(SI32 siBytesToRead);
	BOOL	Write(const VOID * cMsg, UI16 iBytes, BOOL fPW = TRUE);
	BOOL	Readable();
	BOOL	Writeable();
	BOOL	FlushOutBuffer();
	VOID	EraseInBuffer(UI16 uiBytes);
	VOID	ResetSets();

	//---------------------------------------------------------------------------
	// 외부 처리 함수.
	VOID	GetID(CHAR *pszID);			// 내 ID를 얻어온다.

	VOID	PackMessage(char *cSrcMsg, SI16 siSrcSize, char *cPackMsg, SI16 *siPackSize);		// 보낼 메세지를 패키징 한다.
	bool	UnpackMessage();																	// 받은 패킹된 메세지를 푼다.
	BOOL    SendDummyPacket(void *pPacket,int size);
};

#endif
