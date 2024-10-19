//����������������������������������������������������������������������������
#ifndef SOVILLAGE_H
#define SOVILLAGE_H
//����������������������������������������������������������������������������

#define	SOVILLAGE_MAX_MAKRET_PLAYER_NUM				100			// ���忡 ���� �� �ִ� �ְ� �ο�.
#define	SOVILLAGE_MAX_WHARF_PLAYER_NUM				512			// �εο� ���� �� �ִ� �ְ� �ο�.
#define	SOVILLAGE_MAX_WHARFWAITROOM_PLAYER_NUM		512			// �εδ��ǿ� ���� �� �ִ� �ְ� �ο�.
#define SOVILLAGE_MAX_TICKETCOUNT					24			// �� ���� �εο��� �ȼ��ִ� �ִ� Ticket ��

// 16��Ʈ ���̴ϱ� 2����Ʈ
enum	
{	
	INVILLAGE_STATUS_NONE, 
	INVILLAGE_STATUS_MARKET, 
	INVILLAGE_STATUS_WHARF, 
	INVILLAGE_STATUS_BANK,
	INVILLAGE_STATUS_BARRACK,
	INVILLAGE_STATUS_GBARRACK,
	INVILLAGE_STATUS_MBARRACK,
	INVILLAGE_STATUS_FACTORY,								// ���ϴ� ���� �ü����̴�.(����, ����, ����...)
	INVILLAGE_STATUS_HOSPITAL,
	INVILLAGE_STATUS_GOVERNMENT
};

#define ON_VILLAGE_ATTACK_OIL				1
#define ON_VILLAGE_ATTACK_STONE				2
#define	ON_VILLAGE_ATTACK_DEFENCE			3
#define ON_VILLAGE_ATTACK_NONE				4

/*
#define	INVILLAGE_STATUS_NONE				0X0001			// �ƹ� ������ ����.
#define	INVILLAGE_STATUS_MARKET				0X0000			// ���忡 �ִ�.
#define	INVILLAGE_STATUS_INN				0X0000			// ���� �ȿ� �ִ�.
#define INVILLAGE_STATUS_AUCTION			0X0000			// ��żҿ� �ִ�.
#define	INVILLAGE_STATUS_FARM				0X8000			// ���忡 �ִ�.(�� ����Ʈ�� �����ȣ)
#define	INVILLAGE_STATUS_RANCH				0X8100			// ���忡 �ִ�.(�� ����Ʈ�� �����ȣ)
#define	INVILLAGE_STATUS_FACTORY			0X8200			// ���忡 �ִ�.(�� ����Ʈ�� �����ȣ)
#define	INVILLAGE_STATUS_ARMOR				0X8300			// ���꿡 �ִ�.(�� ����Ʈ�� �����ȣ)
#define	INVILLAGE_STATUS_WEAPON				0X8400			// �����忡 �ִ�.(�� ����Ʈ�� �����ȣ)
#define	INVILLAGE_STATUS_MINE				0X8500			// ���꿡 �ִ�.(�� ����Ʈ�� �����ȣ)
*/

#define	CHATROOMINDEXTOID(x)				(x + 1)		// ä�ù濡 �ε����� ID�� �ٲ��ش�.
#define	CHATROOMIDTOINDEX(x)				(x - 1)		// ä�ù濡 ID�� �ε����� �ٲ��ش�.

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
	float		fInterest;												//	����.
	MONEY		siBankCapital;											//	������ ���� �ں�
	MONEY		siTradeCapital;											//	������ ���� �ں�
	UI32		uiAvailableAcc;											//	��밡���� ���� ����
};
*/
struct	ChatRoomJoinedPlayerInfo
{	
	UI16	uiChatRoomJoinedPlayerAccount;	
	SI32	siDBAccount;
};	

struct	stChatRoom
{		
	CHAR						szChatRoomName[ON_MAX_CHATROOM_STRING_LENGTH];			// �� �̸�
	UI32						uiJoinedPlayerNum;										// ������ �÷��̾��� ��.(�ݵ�� 1 �̻��̿��� �Ѵ�)	
	ChatRoomJoinedPlayerInfo	CRJPI[SOVILLAGE_MAX_INN_CHAT_NUM];						// ������ �÷��̾���� ���� ����.	
	DWORD						dwUniqueID;												// ä�ù��� ������ ID
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
	UI32							uiTotalChatRoomNum;										// �� ��ȭ���� ��.
	stChatRoom						ChatRoom[ON_MAX_CHATROOM_NUM];							// ��ȭ��.	
	SStack<UI16>					WaitRoomUserManager;									// ���ǿ� �ִ� ����ڵ��� ���� �ϴ� ��ü.

	UI32							uiTotalPageNum;											// �� �������� ��.
	SStack<UI16>					NoticeNumber;											// �Խù��� ��ȣ�� ������ �ִ�.
	//stNoticeHeader				NoticesHeader[ON_MAX_NOTICES];							// ��ü �Խù�.
};

struct	stAttackGuild
{
	UI32							uiGuildID;
	SI32							siDamage;
};


struct stBestInvestment
{
	DWORD    dwTime;                 //�ְ� �����ڰ��� ��¥
	CHAR     szID[ON_ID_LENGTH + 1]; //������ ���̵�
};

class SOVillage
{
public:
	// ������
	SOVillage();
	// �Ҹ���
	~SOVillage();
public:	
public:
	static	SOWorld					*clWorld;							// World Class
	static	clVillageAttackInfo		clVAInfo;							// VillageAttackInfo	

	UI16		uiMapID;												// �� ������ �ִ� ���� id

	//����������������������������������������������������������������������������������������������������
	// ���� �α��� ���� ������
	//����������������������������������������������������������������������������������������������������
	UI16		uiVillageCode;											// ���� �ڵ�
	UI32		uiPopulation;											// ������ �α�.
	UI32		uiMaxPopulation;										// ������ �ִ� �α�.
	UI32		m_uiLimitMinPopulation;									// ������ ������ �ִ� �ּ��α���.
	UI32		m_uiLimitMaxPopulation;									// ������ ������ �ִ� �ִ��α���.
	//����������������������������������������������������������������������������������������������������

	SI32		siVillageInOutPlayerYetYear;							// �۳� ���� ������ ��
	SI32		siVillageInOutPlayerNowYear;							// ���� ���� ������ ��

	CHAR		cNation;												// ����:K  define�̸�:NATION_CODE_KOREA
	SI16		siVillageX;												// ������ ��ġ
	SI16		siVillageY;												// ������ ��ġ
	UI08		uiVillageSize;											// ������ ũ��
	UI32		uiVillageName;											// ������ �̸� �ڵ�.
	CHAR		szDescription[MAX_DESCRIPT_LENGTH];						// ���� �ȳ���.
	UI32		uiGuildID;												// Guild ID
	SI32		siGuildFlag;											// ��� ID.
	
	stCapital		VillageCapital[ON_STATISTICS_YEAR];					// 10 �⵿���� �� ���� ����
	stCapital		*CurrentCapital;									// ������ ���� ����		
	
	SI32		m_siNextMarketDay;										// ���� �峯

	SOVillage_Market				*m_pMarket;								// ����.
	SOVillage_IndustrialComplexs	*m_pIndustrialComplexs;					// ��� ����
	SOVillage_Bank					*m_pBank;								// ����
	SOVillage_Barrack				*m_Barrack;								// �Ʒü�
	SOVillage_Hospital				*m_Hospital;							// �ǿ�
	SOVillage_Wharf					*m_Wharf;								// �ε�
	SOVillage_Government			*m_Government;							// ��û	
	

	//stBankData					*pstBankData;
	stINNData						*pstINNData;
	//stFarmGroup					*pFarmGroup;

	SI32						siStructureKinds[ON_VILLAGE_MAX_BUILDING];											// �ǹ��� ����

	CHAR			szHNSID[ON_ID_LENGTH + 1];							// '���'�� �̸�.

	//����������������������������������������������������������������������������������������������������
	// ���� ���� ���� ������ 
	//����������������������������������������������������������������������������������������������������
	VDEF			vdDefence, vdMaxDefence, vdDefenceInDB;				// ������ ����
//	SI32			m_siDefenceSoldier,m_siDefenceSoldierInDB;			// ���� , ��� �����ϱ� ���� ��ϵǴ� ���񺴼� Temp��.
//	SI32			m_siMaxDefenceSoldier;								// ������ ������ �ִ� Max���� ��ġ.

	//
	stAttackGuild	m_stAttackGuild[ SO_TOTAL_GUILD_NUM ];				// �� ����� ���ݿ� ���ؼ� ���� ������

	UI32			m_uiLoadQuantity;									// �� �������� ������ �������� �����Ҽ� �ִ� �α���.
	SI16			m_siVillageAttackType;								// ������ ���� Ÿ��.
	DWORD			m_dwPrevAttackdelay;								// ���� Ÿ�� ���� �ð� üũ.(�ð� ����)
	DWORD			m_dwPrevHitTime;									// ���� ���� �ð�.(�ð� ����)
	DWORD			m_dwSkillDelayTime;									// ��� ����ð� ��Ÿ�� üũ
	BOOL			m_bAttackTypeDelay;									// ���� AttackType�� �ٲܼ� �ִ� ���� ���� �÷���.
	BOOL			m_bVillageAttack;									
	DWORD			m_dwVillageDamageUpdateTime;

	BOOL			m_bVillageInLimit;									// ������ ���ؼ�.���� ������ �����Ѱ�.
//	char			m_szCharName[ON_ID_LENGTH + 1];						// �̸� ����.
	char			m_szCharName[100][ON_ID_LENGTH + 1];				// �̸� ����.

	DWORD			dwLastAOVillageBeAttacked;							// ������ ������ ������ �� ���������� ���� �������� ���������� �ð�	

	BOOL            m_bIsFall;
	//����������������������������������������������������������������������������������������������������


private:
	BOOL		InitStructure( VillageHeader *pVillageHeader );			// ������ �ִ� �ǹ����� �ʱ�ȭ �Ѵ�.
	VOID		InitDamageAttackedByGuild();							// �� ����� ���ݿ� ���ؼ� ���� �������� �ʱ�ȭ �Ѵ�.
	VOID		AddDamageAttackedByGuild( UI32 uilocal_GuildID, SI32 siDamage );
	VOID		RewardToGuildForDestroyVillage(MONEY mnMoney);
	SI32		GetAttackGuildSlot( UI32 uilocal_GuildID );				// ������ ������ ����� ������ ���´�.

public:
	// �ʱ�ȭ.
	BOOL		Init(VillageHeader *pVillageHeader);					// ������ �ʱ�ȭ �Ѵ�.(DB�� ���� �� ��Ȳ���� �޾ƿ´�.)

	UI16		GetMapID();												// ������ �ִ� ���� id�� ���´�.

	// ���� ���� �Լ���.
	DWORD		BuyItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory );		// ���忡�� ������ ���.
	DWORD		SellItemInMarket( SOPlayer *pPlayer, UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity );							// ���忡 ������ �Ǵ�.	
	SI32		FindItem(UI16 uiItemCode);											// ������ �����ִ��� ã�´�.	
	BOOL		GetItemPrice(UI16 uiItemCode, MONEY *pmnBuy, MONEY *pmnSell);		// �������� ��� �ȶ��� ������ ���´�. �Ϲ� ���ݸ� ���´�
	SI32		GetTotalValue(UI16 uiTempItemCode, UI16 uiTempQuantity);			// ������ ���� �������� ���� ������ ���Ѵ�.
	MONEY		GetRealItemsPrice(UI16 uiID, UI16 uiItemQuantity, BOOL bBuy, SI32 siGuildRelation);		// �������� ������ ����� ���� ������ ���´�.
	UI32		GetItemQuantity(UI16 uiID);											// �������� ������ ���´�.	
	BOOL		IsSaleItem(UI16 uiID);												// ���� �� �������� ����ϴ� �������ΰ�?
	VOID		IncreaseItem( UI16 uiID, UI16 uiQuantity );							// �������� ������Ų��.
	VOID		DecreaseItem(UI16 uiID, UI16 uiQuantity);							// �������� ���ҽ�Ų��.
	VOID		ProcessProduceReduceItem();											// �������� ���� �� �Һ��Ѵ�	
	VOID		SOVillage::InitMarketDay();											// �峯�� �ʱ�ȭ�Ѵ�.
	SI32		GetNextMarketDay();													// �ٰ����� �峯�� ���´�.
	VOID		SetNextMarketDay();													// ���� �峯�� �����Ѵ�.

	VOID		PlayerLogOut(SOPlayer *pPlayer);						// �÷��̾ �������� �α� �ƿ� ���� ��� ȣ���� �ش�.
	VOID		GetDescription();
	VOID		GetNationCode();
	BOOL		SetNationCode(CHAR NationCode);

	// Poll �Լ�
	VOID		Poll();															// Polling

	MONEY		IncreaseCapital(MONEY siIncreaseMoney, SI32	nOption, BOOL bSendToDB = TRUE );
	VOID		DecreaseCapital(MONEY siDecreaseMoney, SI32	nOption, BOOL bSendToDB = TRUE );
	VOID		CalculateBankCapital();

	//����������������������������������������������������������������������������������������������������
	//���� �α��� ���� �Լ�.
	//����������������������������������������������������������������������������������������������������
	UI32		GetPopulation();										// �α����� ���´�.
	BOOL		GetDBPopulation();
	void		SetDBPopulation();
	void		DecreasePopulation(UI32 uiDecreasePopulation);
	void		IncreasePopulation(UI32	uiIncreasePopulation);
	BOOL		CheckDecreasePopulation(UI32 uiDecreasePopulation);
	BOOL		LimitDecreasePopulation(UI32 uiDecreasePopulation);
	//����������������������������������������������������������������������������������������������������



	// ����ȿ����� �Լ���.
	SI32		OpenSavingAccount(SOPlayer *pPlayer, OnReqBankOpenSavingAccountMsg *pReqOpenSavingAccountMsg,OnAccountNumber *pAccountNum);		// ���� ������ ��û�Ѵ�.
	BOOL		BankDeposit(SOPlayer *pPlayer, OnReqBankDepositMsg *pReqBankDepositMsg);							// ���¿� ���� �����Ѵ�.
	BOOL		BankDrawing(SOPlayer *pPlayer, OnReqBankDrawingMsg *pReqBankDrawingMsg);							// ���¿� ���� ����Ѵ�.
	BOOL		UpdateBankInfo();																					// ���������� DB���� ������Ʈ�Ѵ�.(�õ��� & ����)
	BOOL		GetVillageCapital();
	VOID		SendBankInfo( SOPlayer *pPlayer );																	// �÷��̾�� ������ ������ �����ش�.

	// ���� �ȿ����� �Լ���.
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
	BOOL		EnterIndustrialComplexFactory( SOPlayer *pPlayer, SI32 siFactoryID );					// ��� ������ ���忡 ����.
	BOOL		LeaveIndustrialComplexFactory( SOPlayer *pPlayer );										// ��� ������ ���忡�� ������.
	BOOL		IsIndustrialComplexOwner( SOPlayer *pPlayer );											// �÷��̾ ��� ������ ������ �ִ��� �˾ƿ´�.
	SOVillage_IndustrialComplex  *GetIndustrialComplexOwner( SOPlayer *pPlayer );                       // �÷��̾ �������ִ� ��������� ���´�.
	BOOL		CancelWork( SOPlayer *pPlayer, SI32 siFactoryID );										// ���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.

	// â�� �ȿ����� �Լ���	
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
	// ���� �ȿ����� �Լ���.
	BOOL		IsValidChatRoomIDInInn(UI16 uiChatRoomID);												// ������ ä�� ID�� ��ȿ���� �˾ƿ´�.
	BOOL		EnterInn(SOPlayer *pPlayer);															// ���� ������ ����.
	BOOL		LeaveInn(SOPlayer *pPlayer);															// �������� ������.
	BOOL		EnterWaitRoom(SOPlayer *pPlayer);														// �÷��̾ ���� ������ ����.
	BOOL		JoinChatRoomInInn(UI16 uiChatRoomID, SOPlayer *pPlayer);								// �÷��̾ ���� ä�ù濡 �����ߴ�.
	BOOL		LeaveChatRoomInInn(SOPlayer *pPlayer, BOOL bDisconnect = FALSE);						// �÷��̾ ���� ä�ù濡�� ������.
	BOOL		DelAccountInInn(UI16 uiChatRoomID, UI16 uiAccount);										// ��ȭ�濡�� ID�� �����Ѵ�.
	BOOL		CreateChatRoomInInn(SOPlayer *pPlayer, CHAR *pszChatRoomName);							// ��ȭ���� �����.	
	BOOL		EnterNoticeBoard(SOPlayer *pPlayer);													// �Խ��ǿ� ����.
	BOOL		RegistNotice(SOPlayer *pPlayer, CHAR *pszTitle, CHAR *pszNotice);						// �Խù��� ����Ѵ�
	BOOL		DeleteNotice(SOPlayer *pPlayer, UI16 uiIndex);											// �Խù��� �����Ѵ�.
	BOOL		ModifyNotice(SOPlayer *pPlayer, DWORD dwIndex, CHAR *pszTitle, CHAR *pszContent);		// �Խù��� �����Ѵ�.
	BOOL		SendNoticesList(SOPlayer *pPlayer, UI16 uiPageID);										// �Խù��� ����Ʈ�� ���´�.
	BOOL		SendNoticeContent(SOPlayer *pPlayer, UI16 uiNoticeID);									// �Խù��� ���빰�� ���´�.	
	BOOL		EnterInnWorket(SOPlayer *pPlayer);														// ���۰�뿡 ����.
	BOOL		BuyInnWorket(SOPlayer *pPlayer, SI32 Code, BYTE* lpName);								// ���۰�뿡�� ������ ����Ѵ�.
*/

	CHAR*		GetVillageName();																		// ������ �̸��� ���´�.
	void		GetVillageInfo(SOPlayer *pPlayer);														// ���� ���� �˻�
	DWORD		GetVillageInfo(SOPlayer *pPlayer, OnRecvVillageInfo *pVillageInfo);						// ������ ������ ����.

	SI32		GetVillageState();										// ���� �ڵ带 �ش�.(����� ������, ���� �������·� ������, ��� ���·�)	
	BOOL		EnterVillage(SOPlayer *pPlayer);						// ������ ����.
	VOID		DismissalHNS();											// ������ ����� �ذ��Ѵ�.		
public:		

	SI32		GetVillageInOutPlayer(void);							// ������ ������ ���� ����Ѵ�.
	void		PrepareVillageInOutPlayer(void);						// ������ ������ ���� ����Ѵ�.

	// ��� ���� �Լ���.,
	UI32		GetGuildNID();												// �� ������ �ִ� ����� ���� ID�� ���´�.
	CHAR*		GetGuildSID();												// �� ������ �ִ� ����� ���� ID�� ���´�.
	VOID		SetOwnerGuild(UI32 uiguildid, BOOL bInit = FALSE);			// �� ������ �ҼӵǴ� ����� �����Ѵ�.
	BOOL		SetHNSID(CHAR *pszHNSID);					                // �� ������ ���ο� '���'�� �����Ѵ�.	
	SI32		GetHighestGuildClass(CHAR *pszID);							// �� �������� �� ĳ������ �ְ� ������ ���´�.
	BOOL		DeleteGuildClass(CHAR *pszName);							// �� �������� �� ĳ���Ͱ� ������ �ִ� ������ �ʱ�ȭ �Ѵ�.
	VOID		DeleteGuild();												// �� ������ ��尡 �����Ǿ���.

	VOID		SaveDefenceToDB();											// ������ ������ DB�� �����Ѵ�.
//	VOID		SaveSoldierToDB();											// ���� ���� ���� ����.

	VOID		ChangeGuildFlag(SI32 siFlag);								// ����� �����Ѵ�.
	SI32		GetGuildFlag();												// ����� ����
	BOOL		IsVestInvestmentorAndGuildMember(const CHAR* pszName);		// �ش� ĳ���Ͱ� �ְ� �������̸� ���������� �˻��Ѵ�.

	BOOL		EnterStructure( SOPlayer *pPlayer, SI32 siStructureKind, LPARAM lParam1, LPARAM lParam2 );	// ������ �ǹ��� ����.
	BOOL		LeaveStructure( SOPlayer *pPlayer );														// ������ �ǹ����� ������.

	BOOL		DoesStructureExist(SI32 siStructureKind);					// �ش� �ǹ��� �� ���� �ȿ� �ִ°�?
	BOOL		DoesHospitalExist();										// ������ ������ �ִ��� �˻��Ѵ�.
	BOOL		DoesMarketExist();											// ������ ������ �ִ��� �˻��Ѵ�.
	BOOL		DoesFactoryExist();											// ������ ���� �ü����� �ִ��� �˻��Ѵ�.
	BOOL		DoesBankExist();											// ������ ������ �ִ��� �˻��Ѵ�.

	VOID		GetInfo( OnGuildVillageInfo *pGuildVillageInfo );			// ������ ������ ���´�.

	static	VOID	InitStaticVariable(SOWorld *pWorld);					// ���� ������ �ʱ�ȭ �Ѵ�.
    
	//������ �ְ� �����ڸ� ã�Ƽ� ��ܿ� ���ԵǾ����� ������ �������� ����.
	VOID        CheckBestInvestment();                                      // 
	//����������������������������������������������������������������������������������������������������
	// ���� ���� ���� �Լ��� 
	//����������������������������������������������������������������������������������������������������
	VOID		EnterAttackVillageTimeZone();								// ���� ������ ���۵Ǿ���.
	VOID		LeaveAttackVillageTimeZone();								// ���� ������ ���۵Ǿ���.

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

	VOID		RepairVillageDefence( VDEF vdDefence, BOOL bSendToDB = TRUE );		// ������ �����Ѵ�.

	VDEF		GetVillageDefence();										// ������ ���´�.
	VOID		IncreaseVillageDefence(VDEF IncreaseDefence);				// ������ ���� ��Ų��.
	VOID		DecreaseVillageDefence(VDEF DecreaseDefence);				// ������ ����.
	BOOL		AutoRestorationDefence(UI08 uiIncreasePercent);				// ������ ������ �÷��ش�.
	VOID		AutoRestorationVillageDefence(BOOL bSendToDB);

	int         FindCharName(char *szName);
	int         GetEmptySlot();
	int         DeletCharName(char *szName);
	int         SetCharName(char *szName);

	BOOL        CheckGuild();
	//����������������������������������������������������������������������������������������������������

};
//����������������������������������������������������������������������������
#endif
