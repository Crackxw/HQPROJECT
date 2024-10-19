//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifndef SOVILLAGE_H
#define SOVILLAGE_H
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define	SOVILLAGE_MAX_MAKRET_PLAYER_NUM				100			// 시장에 들어올 수 있는 최고 인원.
#define	SOVILLAGE_MAX_WHARF_PLAYER_NUM				512			// 부두에 들어올 수 있는 최고 인원.
#define	SOVILLAGE_MAX_WHARFWAITROOM_PLAYER_NUM		512			// 부두대기실에 들어올 수 있는 최고 인원.
#define SOVILLAGE_MAX_TICKETCOUNT					24			// 한 마을 부두에서 팔수있는 최대 Ticket 수

// 16비트 형이니까 2바이트
enum	
{	
	INVILLAGE_STATUS_NONE, 
	INVILLAGE_STATUS_MARKET, 
	INVILLAGE_STATUS_WHARF, 
	INVILLAGE_STATUS_BANK,
	INVILLAGE_STATUS_BARRACK,
	INVILLAGE_STATUS_GBARRACK,
	INVILLAGE_STATUS_MBARRACK,
	INVILLAGE_STATUS_FACTORY,								// 일하는 생산 시설물이다.(농장, 공장, 목장...)
	INVILLAGE_STATUS_HOSPITAL,
	INVILLAGE_STATUS_GOVERNMENT
};

#define ON_VILLAGE_ATTACK_OIL				1
#define ON_VILLAGE_ATTACK_STONE				2
#define	ON_VILLAGE_ATTACK_DEFENCE			3
#define ON_VILLAGE_ATTACK_NONE				4

/*
#define	INVILLAGE_STATUS_NONE				0X0001			// 아무 곳에도 없다.
#define	INVILLAGE_STATUS_MARKET				0X0000			// 시장에 있다.
#define	INVILLAGE_STATUS_INN				0X0000			// 주점 안에 있다.
#define INVILLAGE_STATUS_AUCTION			0X0000			// 경매소에 있다.
#define	INVILLAGE_STATUS_FARM				0X8000			// 농장에 있다.(뒷 바이트는 농장번호)
#define	INVILLAGE_STATUS_RANCH				0X8100			// 목장에 있다.(뒷 바이트는 목장번호)
#define	INVILLAGE_STATUS_FACTORY			0X8200			// 공장에 있다.(뒷 바이트는 공장번호)
#define	INVILLAGE_STATUS_ARMOR				0X8300			// 광산에 있다.(뒷 바이트는 광산번호)
#define	INVILLAGE_STATUS_WEAPON				0X8400			// 무기장에 있다.(뒷 바이트는 광산번호)
#define	INVILLAGE_STATUS_MINE				0X8500			// 광산에 있다.(뒷 바이트는 광산번호)
*/

#define	CHATROOMINDEXTOID(x)				(x + 1)		// 채팅방에 인덱스를 ID로 바꿔준다.
#define	CHATROOMIDTOINDEX(x)				(x - 1)		// 채팅방에 ID를 인덱스로 바꿔준다.

#include "SOCommonHeader.h"

#include "SOUtility.h"
#include "OnlineItem.h"
#include "SOPlayer.h"
#include "OnlineVillage-Parser.h"
#include "OnlineTicket-Parser.h"
#include "OnlineGeneral-Parser.h"
#include "OnlineCommonStruct.h"
#include "Stack.h"
#include "OnlineGameMsg.h"
#include "GSCDefine.h"
#include "IProfit.h"
#include "SOPlant.h"
#include "VillageAttackInfo.h"
#include "SODBStructure.h"

#include "SOVillage_MarketStructure.h"
#include "SOVillage_IndustrialComplexsStructure.h"
#include "SOVillage_BarrackStructure.h"
#include "SOVillage_HospitalStructure.h"
#include "SOVillage_WharfStructure.h"
#include "SOVillage_GovernmentStructure.h"
#include "SOVillage_BankStructure.h"

class	SOPlayer;

//-------------------------------------------------------------------

/*
struct	stBankData
{
	float		fInterest;												//	이율.
	MONEY		siBankCapital;											//	마을의 금융 자본
	MONEY		siTradeCapital;											//	마을의 무역 자본
	UI32		uiAvailableAcc;											//	사용가능한 계좌 종류
};
*/
struct	ChatRoomJoinedPlayerInfo
{	
	UI16	uiChatRoomJoinedPlayerAccount;	
	SI32	siDBAccount;
};	

struct	stChatRoom
{		
	CHAR						szChatRoomName[ON_MAX_CHATROOM_STRING_LENGTH];			// 방 이름
	UI32						uiJoinedPlayerNum;										// 참여한 플레이어의 수.(반드시 1 이상이여야 한다)	
	ChatRoomJoinedPlayerInfo	CRJPI[SOVILLAGE_MAX_INN_CHAT_NUM];						// 참여한 플레이어들의 대한 정보.	
	DWORD						dwUniqueID;												// 채팅방의 고유한 ID
};

struct	stNoticeHeader
{
	UI16		uiIndex;
	CHAR		szTitle[ON_MAX_TITLE_STRING_LENGTH + 1];
	CHAR		szID[ON_ID_LENGTH + 1];
	CHAR		szDate[10];
	DWORD		dwReadCount;
};

struct	stINNData
{	
	UI32							uiTotalChatRoomNum;										// 총 대화방의 수.
	stChatRoom						ChatRoom[ON_MAX_CHATROOM_NUM];							// 대화방.	
	SStack<UI16>					WaitRoomUserManager;									// 대기실에 있는 사용자들을 관리 하는 객체.

	UI32							uiTotalPageNum;											// 총 페이지의 수.
	SStack<UI16>					NoticeNumber;											// 게시물의 번호를 가지고 있다.
	//stNoticeHeader				NoticesHeader[ON_MAX_NOTICES];							// 전체 게시물.
};

struct	stAttackGuild
{
	UI32							uiGuildID;
	SI32							siDamage;
};


struct stBestInvestment
{
	DWORD    dwTime;                 //최고 투자자가된 날짜
	CHAR     szID[ON_ID_LENGTH + 1]; //최투의 아이디
};

class SOVillage
{
public:
	// 생성자
	SOVillage();
	// 소멸자
	~SOVillage();
public:	
public:
	static	SOWorld					*clWorld;							// World Class
	static	clVillageAttackInfo		clVAInfo;							// VillageAttackInfo	

	UI16		uiMapID;												// 이 마을이 있는 맵의 id

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 마을 인구수 관련 변수들
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	UI16		uiVillageCode;											// 마을 코드
	UI32		uiPopulation;											// 마을의 인구.
	UI32		uiMaxPopulation;										// 마을의 최대 인구.
	UI32		m_uiLimitMinPopulation;									// 마을이 가질수 있는 최소인구수.
	UI32		m_uiLimitMaxPopulation;									// 마을이 가질수 있는 최대인구수.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	SI32		siVillageInOutPlayerYetYear;							// 작년 마을 출입자 수
	SI32		siVillageInOutPlayerNowYear;							// 올해 마을 출입자 수

	CHAR		cNation;												// 조선:K  define이름:NATION_CODE_KOREA
	SI16		siVillageX;												// 마을의 위치
	SI16		siVillageY;												// 마을의 위치
	UI08		uiVillageSize;											// 마을의 크기
	UI32		uiVillageName;											// 마을의 이름 코드.
	CHAR		szDescription[MAX_DESCRIPT_LENGTH];						// 마을 안내문.
	UI32		uiGuildID;												// Guild ID
	SI32		siGuildFlag;											// 깃발 ID.
	
	stCapital		VillageCapital[ON_STATISTICS_YEAR];					// 10 년동안의 연 마을 수익
	stCapital		*CurrentCapital;									// 올해의 마을 수익		
	
	SI32		m_siNextMarketDay;										// 다음 장날

	SOVillage_Market				*m_pMarket;								// 시전.
	SOVillage_IndustrialComplexs	*m_pIndustrialComplexs;					// 산업 단지
	SOVillage_Bank					*m_pBank;								// 전장
	SOVillage_Barrack				*m_Barrack;								// 훈련소
	SOVillage_Hospital				*m_Hospital;							// 의원
	SOVillage_Wharf					*m_Wharf;								// 부두
	SOVillage_Government			*m_Government;							// 관청	
	

	//stBankData					*pstBankData;
	stINNData						*pstINNData;
	//stFarmGroup					*pFarmGroup;

	SI32						siStructureKinds[ON_VILLAGE_MAX_BUILDING];											// 건물의 종류

	CHAR			szHNSID[ON_ID_LENGTH + 1];							// '행수'의 이름.

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 마을 공성 관련 변수들 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VDEF			vdDefence, vdMaxDefence, vdDefenceInDB;				// 마을의 방어력
//	SI32			m_siDefenceSoldier,m_siDefenceSoldierInDB;			// 수비병 , 디비에 저장하기 위해 기록되는 수비병수 Temp값.
//	SI32			m_siMaxDefenceSoldier;								// 마을이 가질수 있는 Max수비병 수치.

	//
	stAttackGuild	m_stAttackGuild[ SO_TOTAL_GUILD_NUM ];				// 적 상단의 공격에 의해서 받은 데미지

	UI32			m_uiLoadQuantity;									// 이 마을에서 공성병 소집으로 차출할수 있는 인구수.
	SI16			m_siVillageAttackType;								// 마을의 공격 타입.
	DWORD			m_dwPrevAttackdelay;								// 공격 타입 제한 시간 체크.(시간 저장)
	DWORD			m_dwPrevHitTime;									// 공격 적용 시간.(시간 저장)
	DWORD			m_dwSkillDelayTime;									// 기술 적용시간 쿨타임 체크
	BOOL			m_bAttackTypeDelay;									// 마을 AttackType을 바꿀수 있는 가에 대한 플래그.
	BOOL			m_bVillageAttack;									
	DWORD			m_dwVillageDamageUpdateTime;

	BOOL			m_bVillageInLimit;									// 수성을 위해서.마을 출입이 가능한가.
//	char			m_szCharName[ON_ID_LENGTH + 1];						// 이름 저장.
	char			m_szCharName[100][ON_ID_LENGTH + 1];				// 이름 저장.

	DWORD			dwLastAOVillageBeAttacked;							// 마을이 공격을 당했을 시 마지막으로 공지 사항으로 보냈을때의 시간	

	BOOL            m_bIsFall;
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


private:
	BOOL		InitStructure( VillageHeader *pVillageHeader );			// 마을에 있는 건물들을 초기화 한다.
	VOID		InitDamageAttackedByGuild();							// 적 상단의 공격에 의해서 받은 데미지를 초기화 한다.
	VOID		AddDamageAttackedByGuild( UI32 uilocal_GuildID, SI32 siDamage );
	VOID		RewardToGuildForDestroyVillage(MONEY mnMoney);
	SI32		GetAttackGuildSlot( UI32 uilocal_GuildID );				// 마을을 공격한 상단의 슬롯을 얻어온다.

public:
	// 초기화.
	BOOL		Init(VillageHeader *pVillageHeader);					// 마을을 초기화 한다.(DB로 부터 현 상황까지 받아온다.)

	UI16		GetMapID();												// 마을이 있는 맵의 id를 얻어온다.

	// 시전 관련 함수들.
	DWORD		BuyItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory );		// 시장에서 물건을 산다.
	DWORD		SellItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity );							// 시장에 물건을 판다.	
	SI32		FindItem(UI16 uiItemCode);											// 물건을 갖고있는지 찾는다.	
	BOOL		GetItemPrice(UI16 uiItemCode, MONEY *pmnBuy, MONEY *pmnSell);		// 아이템을 사고 팔때의 가격을 얻어온다. 일반 가격만 얻어온다
	SI32		GetTotalValue(UI16 uiTempItemCode, UI16 uiTempQuantity);			// 수량에 의한 아이템의 실제 가격을 구한다.
	MONEY		GetRealItemsPrice(UI16 uiID, UI16 uiItemQuantity, BOOL bBuy, SI32 siGuildRelation);		// 아이템의 물가를 고려한 실제 가격을 얻어온다.
	UI32		GetItemQuantity(UI16 uiID);											// 아이템의 수량을 얻어온다.	
	BOOL		IsSaleItem(UI16 uiID);												// 현재 이 마을에서 취급하는 아이템인가?
	VOID		IncreaseItem( UI16 uiID, UI16 uiQuantity );							// 아이템을 증가시킨다.
	VOID		DecreaseItem(UI16 uiID, UI16 uiQuantity);							// 아이템을 감소시킨다.
	VOID		ProcessProduceReduceItem();											// 아이템을 생산 및 소비한다	
	VOID		SOVillage::InitMarketDay();											// 장날을 초기화한다.
	SI32		GetNextMarketDay();													// 다가오는 장날을 얻어온다.
	VOID		SetNextMarketDay();													// 다음 장날을 설정한다.

	VOID		PlayerLogOut(SOPlayer *pPlayer);						// 플레이어가 마을에서 로그 아웃 했을 경우 호출해 준다.
	VOID		GetDescription();
	VOID		GetNationCode();
	BOOL		SetNationCode(CHAR NationCode);

	// Poll 함수
	VOID		Poll();															// Polling

	MONEY		IncreaseCapital(MONEY siIncreaseMoney, SI32	nOption, BOOL bSendToDB = TRUE );
	VOID		DecreaseCapital(MONEY siDecreaseMoney, SI32	nOption, BOOL bSendToDB = TRUE );
	VOID		CalculateBankCapital();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//마을 인구수 관련 함수.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	UI32		GetPopulation();										// 인구수를 얻어온다.
	BOOL		GetDBPopulation();
	void		SetDBPopulation();
	void		DecreasePopulation(UI32 uiDecreasePopulation);
	void		IncreasePopulation(UI32	uiIncreasePopulation);
	BOOL		CheckDecreasePopulation(UI32 uiDecreasePopulation);
	BOOL		LimitDecreasePopulation(UI32 uiDecreasePopulation);
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━



	// 은행안에서의 함수들.
	SI32		OpenSavingAccount(SOPlayer *pPlayer, OnReqBankOpenSavingAccountMsg *pReqOpenSavingAccountMsg,OnAccountNumber *pAccountNum);		// 계좌 개설을 요청한다.
	BOOL		BankDeposit(SOPlayer *pPlayer, OnReqBankDepositMsg *pReqBankDepositMsg);							// 계좌에 돈을 저금한다.
	BOOL		BankDrawing(SOPlayer *pPlayer, OnReqBankDrawingMsg *pReqBankDrawingMsg);							// 계좌에 돈을 출금한다.
	BOOL		UpdateBankInfo();																					// 은행정보를 DB에서 업데이트한다.(시동시 & 매일)
	BOOL		GetVillageCapital();
	VOID		SendBankInfo( SOPlayer *pPlayer );																	// 플레이어에게 전장의 정보를 보내준다.

	// 농장 안에서의 함수들.
	stFarmData*	GetOwnFactory( SI32 siPlayerDBAccount, UI08 uiPlayerDBSlot );
	SI32		GetFarmData();
	BOOL		MakeFarmList();
	BOOL		BuyFarm(SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID);
	BOOL		SellFarmByForce(UI08 uiFarmID, MONEY *pMoney);
	BOOL		EnterPlant(SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID);
	BOOL		LeavePlant(SOPlayer *pPlayer, BOOL bDisconnect = FALSE);
	BOOL		PlantWorkAble( SOPlayer *pPlayer );
	BOOL		ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay );
	BOOL		ExpandFarmSlot(SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID);
	BOOL		IsFarmOwner(SOPlayer *pPlayer);
	BOOL		IsFarmOwner(SOPlayer *pPlayer, stFarmData* pFarmData);
	BOOL		DestroyPlantSlot(SOPlayer *pPlayer, UI08 uiSlotID);	
	BOOL		SetFarmEquipment(SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, UI08 uiFarmSlot, UI08 uiEquipID);
	BOOL		DestroyFarmEquipment(UI08 uiSlot, UI08 uiEquipID, UI08 uiNum = 1);
	DWORD		UpgradeSlot(SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay);
	DWORD		ProductItem( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID, UI08 uiSlot, UI16 uiItem, UI32 Quantity, UI32 uiPay);
	UI32		FarmWork( SOPlayer *pPlayer, UI08 uiSlot, UI32 uiLabor, MONEY* uiPay );
	UI32		GetQuantityByLabor(UI16 uiItem, UI32	&uiLabor);
	UI32		GetLaborByQuantity(UI16 uiItem, UI32	uiQuantity);
	UI32		GetRemainLabor(stFarmSlot *pFarmSlot);
	VOID		SetTotalRemainLabor(stFarmData *pFarmData);
	BOOL		ChangePay(SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay);
	DWORD		CancelPlan(SOPlayer *pPlayer, UI08 uiSlot);
	BOOL		IsRunnig(stFarmSlot	*pFarmSlot);
	BOOL		EnterIndustrialComplexFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// 산업 단지의 공장에 들어간다.
	BOOL		LeaveIndustrialComplexFactory( SOPlayer *pPlayer );										// 산업 단지의 공장에서 나간다.
	BOOL		IsIndustrialComplexOwner( SOPlayer *pPlayer );											// 플레이어가 산업 단지를 가지고 있는지 알아온다.
	SOVillage_IndustrialComplex  *GetIndustrialComplexOwner( SOPlayer *pPlayer );                       // 플레이어가 가지고있는 산업단지를 얻어온다.
	BOOL		CancelWork( SOPlayer *pPlayer, SI32 siFactoryID );										// 현재 작업중인 공장에서 작업중인것을 취소한다.

	// 창고 안에서의 함수들	
	BOOL		CheckMaterial( UI08 uiFarmID, UI16 uiItem, UI32 Quantity );
	BOOL		SetStorageItem( UI08 uiPartID, UI08 uiSlot, UI16 uiItemCode, SI16 uiItemQnt ,UI08 uiItemIndex = UI08_ERROR);
	BOOL		StorageInputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt );
	BOOL		StorageOutputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt );
	UI08		GetStorageItemIndex(stStorage *pStorage,UI16 uiItemCode);
	UI08		DestroyStorage(SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiPartID, UI08 uiSlot);
	UI08		CheckStorageItemExist(stStorage *pStorage,UI16 uiItemCode, UI16	uiItemQnt=1);
	UI08		GetStorageEmptySlot(stStorage *pStorage,UI16 uiItemCode);
	BOOL		CheckStorageSourceItem( stFarmData *pFarmData, UI16 uiItemCode, UI16 uiItemQnt );
	BOOL		CheckStorageTool( UI08 uiPlantID, UI08 uiPartID, UI16 uiItemCode );
	BOOL		SpendMaterial( UI08 uiFarmID, UI16 uiItem, UI32 Quantity );
	UI08		GetEmptyStorage(UI08 uiPlantID, UI08 uiFarmID,UI16 uiItemCode, UI08 &uiLoc, UI16 &uiQnt);
/*	
	// 주점 안에서의 함수들.
	BOOL		IsValidChatRoomIDInInn(UI16 uiChatRoomID);												// 주점의 채팅 ID가 유효한지 알아온다.
	BOOL		EnterInn(SOPlayer *pPlayer);															// 주점 안으로 들어간다.
	BOOL		LeaveInn(SOPlayer *pPlayer);															// 주점에서 나간다.
	BOOL		EnterWaitRoom(SOPlayer *pPlayer);														// 플레이어가 대기실 안으로 들어간다.
	BOOL		JoinChatRoomInInn(UI16 uiChatRoomID, SOPlayer *pPlayer);								// 플레이어가 주점 채팅방에 참여했다.
	BOOL		LeaveChatRoomInInn(SOPlayer *pPlayer, BOOL bDisconnect = FALSE);						// 플레이어가 주점 채팅방에서 나갔다.
	BOOL		DelAccountInInn(UI16 uiChatRoomID, UI16 uiAccount);										// 대화방에서 ID를 제거한다.
	BOOL		CreateChatRoomInInn(SOPlayer *pPlayer, CHAR *pszChatRoomName);							// 대화방을 만든다.	
	BOOL		EnterNoticeBoard(SOPlayer *pPlayer);													// 게시판에 들어간다.
	BOOL		RegistNotice(SOPlayer *pPlayer, CHAR *pszTitle, CHAR *pszNotice);						// 게시물을 등록한다
	BOOL		DeleteNotice(SOPlayer *pPlayer, UI16 uiIndex);											// 게시물을 삭제한다.
	BOOL		ModifyNotice(SOPlayer *pPlayer, DWORD dwIndex, CHAR *pszTitle, CHAR *pszContent);		// 게시물을 수정한다.
	BOOL		SendNoticesList(SOPlayer *pPlayer, UI16 uiPageID);										// 게시물의 리스트를 얻어온다.
	BOOL		SendNoticeContent(SOPlayer *pPlayer, UI16 uiNoticeID);									// 게시물의 내용물을 얻어온다.	
	BOOL		EnterInnWorket(SOPlayer *pPlayer);														// 짐꾼고용에 들어간다.
	BOOL		BuyInnWorket(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);								// 짐꾼고용에서 짐꾼을 고용한다.
*/

	CHAR*		GetVillageName();																		// 마을의 이름을 얻어온다.
	void		GetVillageInfo(SOPlayer *pPlayer);														// 마을 정보 검사
	DWORD		GetVillageInfo(SOPlayer *pPlayer, OnRecvVillageInfo *pVillageInfo);						// 마을의 정보를 얻어간다.

	SI32		GetVillageState();										// 국가 코드를 준다.(상단이 없으면, 현재 국가상태로 있으면, 상단 상태로)	
	BOOL		EnterVillage(SOPlayer *pPlayer);						// 마을에 들어간다.
	VOID		DismissalHNS();											// 현재의 행수를 해고한다.		
public:		

	SI32		GetVillageInOutPlayer(void);							// 마을의 출입자 수를 계산한다.
	void		PrepareVillageInOutPlayer(void);						// 마을의 출입자 수를 계산한다.

	// 상단 관련 함수들.,
	UI32		GetGuildNID();												// 이 마을에 있는 길드의 숫자 ID를 얻어온다.
	CHAR*		GetGuildSID();												// 이 마을에 있는 길드의 문자 ID를 얻어온다.
	VOID		SetOwnerGuild(UI32 uiguildid, BOOL bInit = FALSE);			// 이 마을이 소속되는 상단을 설정한다.
	BOOL		SetHNSID(CHAR *pszHNSID);					                // 이 마을에 새로운 '행수'를 지정한다.	
	SI32		GetHighestGuildClass(CHAR *pszID);							// 이 마을에서 이 캐릭터의 최고 직급을 얻어온다.
	BOOL		DeleteGuildClass(CHAR *pszName);							// 이 마을에서 이 캐릭터가 가지고 있는 직급을 초기화 한다.
	VOID		DeleteGuild();												// 이 마을의 길드가 삭제되었다.

	VOID		SaveDefenceToDB();											// 마을의 방어력을 DB에 저장한다.
//	VOID		SaveSoldierToDB();											// 마을 수비병 수를 저장.

	VOID		ChangeGuildFlag(SI32 siFlag);								// 깃발을 변경한다.
	SI32		GetGuildFlag();												// 깃발을 얻어간다
	BOOL		IsVestInvestmentorAndGuildMember(const CHAR* pszName);		// 해당 캐릭터가 최고 투자자이며 길드원인지를 검사한다.

	BOOL		EnterStructure( SOPlayer *pPlayer, SI32 siStructureKind, LPARAM lParam1, LPARAM lParam2 );	// 마을의 건물에 들어간다.
	BOOL		LeaveStructure( SOPlayer *pPlayer );														// 마을의 건물에서 나간다.

	BOOL		DoesStructureExist(SI32 siStructureKind);					// 해당 건물이 이 마을 안에 있는가?
	BOOL		DoesHospitalExist();										// 마을에 은행이 있는지 검사한다.
	BOOL		DoesMarketExist();											// 마을에 시전이 있는지 검사한다.
	BOOL		DoesFactoryExist();											// 마을에 생산 시설물이 있는지 검사한다.
	BOOL		DoesBankExist();											// 마을에 전장이 있는지 검사한다.

	VOID		GetInfo( OnGuildVillageInfo *pGuildVillageInfo );			// 마을의 정보를 얻어온다.

	static	VOID	InitStaticVariable(SOWorld *pWorld);					// 정적 변수를 초기화 한다.
    
	//마을의 최고 투자자를 찾아서 상단에 가입되어있지 않으면 최투에서 뺀다.
	VOID        CheckBestInvestment();                                      // 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 마을 공성 관련 함수들 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	VOID		EnterAttackVillageTimeZone();								// 마을 공성이 시작되었다.
	VOID		LeaveAttackVillageTimeZone();								// 마을 공성이 시작되었다.

//	void		IncreaseDefenceSoldier(SI32 siDefenceSoldier);
//	void		DecreaseDefenceSoldier(SI32 siDefenceSoldier);
//	SI32		GetDefenceSoldier();
	
	void		SetVillageAttackType(SI16 siAttackType);
	void		InitializeVillageAttackType();
	SI16		GetVillageAttackType();
	SI16		SendVillageToPlayerAtAttackType(SI16 siAttackType);
	BOOL		GetVillageAttackTypeDelayCheck();

	BOOL        AttackDamage(SOPlayer *pPlayer, SI16 siAttackType, BOOL IsLong);

//	BOOL		AttackedByPlayer( SOPlayer *pPlayer, SI16 siAttackType);
//	BOOL		LongDistanceAttack( SOPlayer *pPlayer, SI16 siAttackType);

	VOID		RepairVillageDefence( VDEF vdDefence, BOOL bSendToDB = TRUE );		// 마을을 수리한다.

	VDEF		GetVillageDefence();										// 방어력을 얻어온다.
	VOID		IncreaseVillageDefence(VDEF IncreaseDefence);				// 방어력을 증가 시킨다.
	VOID		DecreaseVillageDefence(VDEF DecreaseDefence);				// 방어력을 뺀다.
	BOOL		AutoRestorationDefence(UI08 uiIncreasePercent);				// 마을의 방어력을 올려준다.
	VOID		AutoRestorationVillageDefence(BOOL bSendToDB);

	int         FindCharName(char *szName);
	int         GetEmptySlot();
	int         DeletCharName(char *szName);
	int         SetCharName(char *szName);

	BOOL        CheckGuild();
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#endif
