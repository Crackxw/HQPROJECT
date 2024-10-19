#ifndef SOPLAYER_H
#define SOPLAYER_H

#include <time.h>
#include <winsock2.h>

#include "SOUtility.h"
#include "SOCommonHeader.h"
#include "SOBaseChar.h"
#include "OnlineCommonStruct.h"
#include "IOnlineFollower.h"
#include "OnlineGameMsg.h"
#include "SODefine.h"
#include "PrivateMarket.h"
#include "Trade.h"
#include "SpeedChecker.h"
#include "SOPlayer_ItemBankList.h"
#include "Common.h"
#include "../SCCommon/Defines.h"
#include "SODBStructure.h"

#include "../AuthServer/_LoginDefine.h"

struct LoginInfo;

class   CSkillMgr;

class	XPlayer;
		
class	SOBattle;
class	SOServer;
class	SOGate;
class	SOVillage;
struct	stFarmData;
class   CAuctionMarket;
#include "..\\XCommon\\JXPacket.h"
#include "./party/IPartyMgr.h"

#include "CSiegeUniteTable.h"
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾��� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
typedef struct
{
	char			id			[LOGIN::ID_MAX_LENGTH];
	char			password	[LOGIN::PW_MAX_LENGTH];

	char			IP[16];							//	ip�ּ�(char ��)
	DWORD			dwIP;							//	DWORD��.
	SI32			siDBAccount;					//	�÷��̾ �����ϴ� �����ͺ��̽����� ������ ��ȣ
	UI08			uiSlot;							//	�ɸ����� ���Թ�ȣ.
	UI16			uiStatusInVillage;				// ���� �ȿ��� �� �ִ� �ǹ�.
	//UI16			uiChatRoomIDInInn;				// ����, ������ ��ȭ�濡 �����ϰ� �ִ� ���� �ε���.

	SOBattle		*pBattle;						// ���� �����ϰ��ִ� ������.
	SOVillage		*pVillage;
	stFarmData		*pStructure;					// ���� ���ϰ� �ִ� �ǹ�.

} ACCOUNT_INFO;



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾��� ��ǥ ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
typedef struct
{
	SI32		siTicketKind;					// Ticket Kind
} TICKET_INFO;



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
// �÷��̾��� �ڻ� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
typedef struct
{
	MONEY		siTotalProperty;				//	�����
	MONEY		siMoneyProperty;				//	����
	MONEY		siSaveProperty;					//	���� �ܰ� (+)��
	MONEY		siLoanProperty;					//	���� ���� (-)��
	MONEY		siItemProperty;					//	�������� ȯ���� ��
	UI32		uiRanking;						//	�ڽ��� ��ŷ
} PROPERTY_INFO;

extern char cMsg2[ 80000 ];

//=========================================================================================
enum MAIN_STATE
{
	NONE_STATE			=   0 ,

	USER_IN_AUTH			  ,
	USER_IN_LOGIN			  ,
	USER_IN_GAME			  ,
	
	USER_MUST_LOGOUT_IN_AUTH  ,
	USER_MUST_LOGOUT_IN_LOGIN ,
	USER_MUST_LOGOUT_IN_GAME  ,

	USER_WAIT_FOR_LOGOUT

};

class CMainState
{
private:
	long			m_state;	

public:
	void			set(long state)    { InterlockedExchange(&m_state, state); }
	int				get()			   { return m_state; }
	bool			match(long state)  { return get() == state ? true : false; }
	void			logout()		   
	{ 
		switch( get() )
		{
			case USER_IN_AUTH:			
				set( USER_MUST_LOGOUT_IN_AUTH );				
				break;

			case USER_IN_LOGIN:
				set( USER_MUST_LOGOUT_IN_LOGIN );
				break;

			case USER_IN_GAME:
				set( USER_MUST_LOGOUT_IN_GAME );
				break;
		}
	}
};
//=========================================================================================

class CCSAuth;

class CAuctionMarketProc;
class CBattle;
class SOPlayer : public BaseChar
{
	// ���� ���ӿ����� ����Ÿ.
public:

	///////////////////////////////////////////
	// ĳ���� ���� ����
	///////////////////////////////////////////

	LOGIN::sCharInfo	m_charsInfo[ LOGIN::CHAR_MAX_NUM ];

	void				clearCharsInfo();
	void				setSlot( int slot );
	void				delSlot( int slot );
	int					getEmptySlot();
	void				sendCharsInfo();

	////////////////////////////////////////////


	CMainState			m_mainState;

	DWORD				m_lastTick;

	// --  �α��� ���� 
	void		ProcessLogin();
	LoginInfo  *logininfo;
	BOOL		bSuccessLogin;								// �α����� ���������� �ߴ°�?
	BOOL		bProcessLogin;
		
	UserGrade	USGrade;									// �ſ� ���.
	MONEY		siMoney;									// ��
	
	MONEY		investProfit;								// ���� ���� 
	MONEY		additionProfit;								// �ΰ� ���� (�ǹ� �ȶ�)
		
	UI32		uiTradeCredit;								// ���� �ſ뵵
	SI32		siDayCredit;								// �Ϸ翡 ���� �ſ뵵
	
	UI32		uiGuildID;									// ����� ID
	SI32		siClassInGuild;								// ��峻������ ����
	SI32		siFlag;										// ���	

	SI32		siNearestHospitalVillageIndexInDungeon;		// �������� ���� ����� �ǿ��� ������ �ִ� ������ �ε���
	
	DWORD		dwPrevSavePosTime;							// ������ �� ��ġ�� DB�� ������ �ð�
	DWORD		dwHeartbeatTime[ON_HEARTBEAT_SAVE_NUM];		// �÷��̾ ����ִ��� �˻��� �ð�


	cltPrivateMarket	clPrivateMarket;					// ���� ����
	cltTrade			clTrade;							// ����

	CSpeedHackChecker	m_SpeedChecker;
	BOOL				m_bIsSpeedHackUser;		
	
	SI08		m_siMaxGeneral;								// ��� ������ ����� ��
	BOOL		m_bCompareWearItem;

	SI16		m_siTimerItemUseCount;

// gguo
// -----------------------------------------------------------------------------------------------------

private:
	SI32                m_siChallengeMode;                          // ���� 108��� ���.
	ItemBankList		m_ItemBankList;

	// GameGuard����
	DWORD				m_lastGameGuardCheckTime;
	CCSAuth				*m_pCSAuth;
	BOOL				m_bCheckPass;

	DWORD				m_dwCondLaborTime;							// Ư�� ������ �����ϴ� ���� �뵿 �ð�

public:
	MONEY				m_MoneySaving;                              // �����س��� ��.

// kojimodo 
//------------------------------------------------------------------------------------------------------
private:
	int					m_satiety;									// ������
	int					m_orgSatiety;								// ���� ������ 
	
	WORD				m_latestSavedMapID;							// �ֱٿ� ������ MapID
	POINT				m_latestSavedPos;							// �ֱٿ� ������ ��ġ 
	DWORD				m_latestSavedPosTime;						// �ֱٿ� ��ġ�� ������ �ð� 

	CSkillMgr*			m_pSkillMgr;								// ��ų ������ 


	CAuctionMarketProc* m_pAuctionMarketProc;						// ��ż� ��Ŷ ó�� Ŭ���� 
	DWORD				m_tempBiddingPrice;							// ��ſ��� �ӽ÷� ���������� �����ϴ� ���� 
	BOOL				bGuildInfoChanged;							// ��� ���� �����ߴ���?
	

public:
	void				SetGuildInfoChanged()	{ bGuildInfoChanged = TRUE; }
	void				UnSetGuildInfoChanged()	{ bGuildInfoChanged = FALSE; }

	void				setTempBiddingPrice( DWORD price );			
	DWORD				getTempBiddingPrice();

	CAuctionMarket*		getAuctionMarket();
			
	void				saveSatietyToDB();							
	void				saveLogoutInfoToDB();
	void				saveLogoutStartToDB();
	void				saveAllFollowerDataToDB();								
	void				saveLogountWarInfoToDB();
	
	void				removeSelfInGame();
	

private:
	void				removeSelfInAuctionMarket();	
	void				removeSelfInBattle();

	void				savePosToDB();
	void				saveFollowerDataToDB( DWORD dbCmd, BYTE followerSlot );		
	
	bool				isValidGUID(char* guid);
// semarine
//------------------------------------------------------------------------------------------------------

public:
	SI32				m_numWin;
	SI32				m_numLose;
	SI32				m_siPoint;
	SI32				m_siRank;



//------------------------------------------------------------------------------------------------------


protected:
	BOOL		Trade( SOPlayer *pTargetPlayer );			// �������� ǰ�� ���ؼ� �ŷ��� �϶�� �˷��ش�.

public:	
	SOPlayer();												// ������	
	~SOPlayer();											// �Ҹ���

public:
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// Time ���� ���� 
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	struct tm			*pNewTime;
	time_t				stClock;

public:
	SOServer	*clServer;		

public:
	// Packing�� ���õ� ���ۿ� ����
	char		pmsg[30000];
	char		*cMsg;							
	SI16		siMsgLength;							// �޼����� ũ��

	JXPacket	local_packet;
public:
	SI32		Write(const VOID *, SI32 Length);
	SI32		ReadFromBattle(SI32 siLength);
	SI32		WriteToBattle(const VOID *, SI32 siLength);
	

public:
	EquipItemInfo	a;
	// �÷��̾��� ���¸� ��Ÿ����. (�������ΰ�?, �������ΰ�?, �����ΰ�?, ��ȭ���ΰ�?, �������ΰ�?)
	UI08			uiState;

	// �ǹ��ȿ��� �ڽ��� ��ġ
	UI32			uiStructerInPos;

	// �÷��̾��� ǥ ����
	TICKET_INFO		stTicketInfo;

	// Quest ����
	SPlayerQuest	stQuestInfo;

	// �÷��̾��� ���� ����
	ACCOUNT_INFO	stAccountInfo;	
	// �÷��̾��� �ڻ� ���� 
	PROPERTY_INFO	stPropertyData;

	SI08			siVMercenarySlot;				// ����ٴϴ� ���� Slot

	// �ſ� ���� �ѵ�
	UI32			uiMaxLoan;

	SI32			uiBoardShipAccount;				// ���� Ÿ�� �ִ� ���� Account

	XPlayer			*m_pXPlayer;					// ���� �߰�..

	UI16			m_uiRecvItemNum;

	BOOL			m_bDayChange;					

	//��������������������������������������������������������������������������������������������
	//�ð��� ������ ���� ����.
	//��������������������������������������������������������������������������������������������
	DWORD			m_dwUserhour;
	DWORD			m_dwUserMin;
	DWORD			m_dwUserSec;

	OnTimerItemInfo		m_stTimerItemInfo[12][9];
	OnTimerItemInfo		m_pTimerItemInfo[12];		// �ϴ� �迭��.-_-��.


	//��������������������������������������������������������������������������������������������
	// ���� ĳ���Ϳ� ���� ����.
	//��������������������������������������������������������������������������������������������
	SI16			m_siFieldHp;					// �������� ȹ���ϴ� Hp.
	SI16            m_siFieldMaxHP;                 // �������� ȹ���ϴ� �ִ� hp 

	SI32            m_siFieldMP;                    // �������� ȹ���ϴ� MP
	SI32            m_siFieldMaxMP;                 // �������� ȹ���ϴ� �ִ�MP

//	SI32			m_siSupply;						// �������� ȹ���ϴ� ����.
//	SI32			m_siSupplyCharge;				// ������ ���� ������. (���޼����� ������ �ִ�.)
	BOOL			m_bFieldAlive;					// �������� ��ҳ� �׾���.������ �ʵ忡 �״��.5�а� ������������ ���źҰ�.
	UI16			m_uiPrevKind;					// ������������ ���ϱ����� ĳ���� Kind
	BOOL			m_bCaptainUnit;					// ���������ΰ�.

	DWORD			m_dwDeadTime;					// ������ ���� �ð� ���. �α� �ƿ� ���� ��� 
	DWORD			m_dwDeadSec;					// �׾����� �г�Ƽ�� ���� �ð�.

	BOOL			m_bInvestmentPower;
	BOOL			m_bFieldUnitUpdate;
	BOOL			m_bPlayerVillageInLimit;
	SI16            m_siVilligeCode;                // ������ ������ �ϰ� �ִ� �����ڵ�
	
	BOOL            m_bIsFirstDefence;              // �� ������ ó�� �÷ȳ�.
	DWORD           m_dwDefenceTime;                // �������� �ø��ð�...
	DWORD           m_dwChargeTime;                 // ������ ���޼����� ������ ȸ���� �ð�.
	DWORD           m_dwMessageTime;                //
	BOOL            m_bIsWarUnit;                   //
	DWORD           m_dwAttackDelay;                // ���� ������ �ð�
	SI16            m_siKillNum;                    // ���� �� 
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ��Ʈ��ũ �����Լ�
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
public:
	// ĳ���� �ʱ�ȭ
	VOID	Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID);					
 
	// �÷��̾��� ���� �ʱ�ȭ
	VOID	Init();

	// ���ڿ� ip�� DWORD �������� �ٲ��ش�.
	DWORD	ChangeIPStr(char* lpIP);

	// �÷��̾��� ���� ���� �ʱ�ȭ
	VOID	InitEnv();

	// �÷��̾�� ������ ��� ���� ����
	VOID	Poll(VOID);
	
	// ��� �޼����� ó���Ѵ�.
	VOID	ProcessMessage(VOID);
	BOOL	ProcessMessageInField(VOID);
	BOOL	ProcessMessageInVillage(VOID);
	BOOL	ProcessMessageInBattle(VOID);
	BOOL	ProcessMessageCommonly(VOID);

	// �α׾ƿ� �Ѵ�.
	VOID	LogOut(VOID);
		
	// �÷��̾ �����Ѵ�.
	VOID	Create( SOServer *clTempServer );					// �÷��̾ �����Ѵ�.

public:
	// �α��� ó��
	VOID	RecvLogIn(char *pMsg);
	VOID	SendLogIn(BOOL bSuccess);


public:
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ���� ����Ÿ ó��.
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �÷��̾�� �޽����� ������.
	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);	

	// ���� ������.
	VOID		SendMoney(DWORD dwKind);
	VOID		GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB = TRUE);		// ���� �ش�.

	// ����Ž�� ��û�� �޾Ҵ� 
	void	recvProbeArea();

	// �̸�Ƽ�� �޾Ҵ�.
	VOID	RecvImoticon();
	 
	// �÷��̾��� ������ ������.
	// �÷��̾��� ��ġ�� �����Ѵ�.
	VOID	RecvPosition();
	
	// Ư�� �÷��̾��� ��ġ�� ��û�Ѵ�.
	// Ư�� �÷��̾��� ��ġ�� ������.
	// Ư�� �÷��̾��� ������ ��û�Ѵ�
	VOID	RecvPlayerInfo();

	VOID	RecvCharDetailInfo();								// ĳ������ �� ������ ��û�Ѵٴ� �޽����� �޾Ҵ�.
	
	// �������� �Һ��ߴ�.
	VOID	RecvSpendItem();
	BOOL	RecvWarp();											// ���� �Ѵٴ� �޽����� �޾Ҵ�.			
	BOOL	Warp(UI16 uiMapID, SI32 siX, SI32 siY);				// ������ �ϰ� ������ ����Ÿ�� �޴´�.

//	VOID	RecvDeleteTimerItem();

	// ������ ����.
	VOID	RecvEnterVillage();
	BOOL	EnterVillage( BOOL bIsVillageField = FALSE );		// ������ ����
	VOID	LeaveVillage( BOOL bIsVillageField = FALSE );		// �������� ������.

	VOID	RecvEnterStructure();								// �������� Ư�� �ǹ��� ���ٴ� �޽����� �޾Ҵ�.
	VOID	RecvLeaveStructure();								// �������� Ư�� �ǹ����� �����ٴ� �޽����� �޾Ҵ�.

	VOID	RecvEnterFactory();									// ���� ��� ������ ���忡 ����.
	VOID	RecvLeaveFactory();									// ���� ��� ������ ���忡�� ������.
	

	// �ʵ�� �÷��̾ ��ġ�� �� �ִ� ��ǥ�� �������� �߽����� ã�´�.
	// ������ ��û�Ѵ�.
	VOID	RecvBattle(VOID);
	VOID	SendDeadPlayerToVillageHospital();					// ���� ĳ���͸� ���� �ǿ��� ������.
	VOID	NotifyFriendsOfLoginState( BOOL bLogin );			// ���� ģ���� ����� ����鿡�� �α��������� �˸���.

	VOID	RecvSetAttackMode();									// ������� / ��ȭ��� ����

	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// Common
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID	RecvHeartBeat();
	// �÷��̾������ �����۸���Ʈ�� ��û�Ѵ�.
	// �÷��̾������ �������� üũ���� ��û�Ѵ�.
	
	// �������� �̵� ��Ų��.
	VOID	RecvShareItem();
	// �������� ������.
	VOID	RecvThrowItem();
	// �������� �ݴ´�.
	VOID	RecvPickUpItem();
	
	
	VOID	RecvWearItem();
	VOID	RecvStripItem();
	
	BOOL	GetWearItemInfo(UI08 uiFollowerID);
	BOOL	StripItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	WearItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	SpendItem( UI08 uiFollowerID, UI16 uiItemID, UI16 uiPosInventory, UI16 uiQuantity = 1);
	BOOL	DisAppearItem( UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	// �÷��̾������ �����۸���Ʈ�� ��Ŷ�� �����. (��Ŷ�� ���̸� �����Ѵ�)
	// �÷��̾������ �����۵��� �ش縶�������� ���� ��Ŷ�� �����. (��Ŷ�� ���̸� �����Ѵ�)
	// �������� üũ���� üũ�Ѵ�.
	// üũ������ �����.	

	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// GameGuard����
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID	CheckGameGuard(VOID);
	VOID	RecvRequestCheckGameGuard(VOID);
	VOID	RecvUserInfoGameGuard(VOID);

	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ���� �Ÿ������� ó��
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID	RecvChargeBooth();
	VOID	RecvLeaveBooth();
	VOID	RecvDisplayBooth();
	VOID	RecvSetBoothItem();
	VOID	RecvCancelBoothItem();
	VOID	RecvBoothInfo();
	VOID	RecvBuyBoothItem();

	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �����ȿ����� ó��
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������

	// �ָ����� ������ ����Ʈ, take out
	VOID	RecvItemList();
	VOID	RecvTakeOutItem();
	VOID	RecvSaveItemIntoItemBank();


	// �������� ������.
	VOID RecvLeaveVillage();
	// �÷��̾��� �������� �����Ѵ�.
	

	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ������ ���忡���� ó��.
	//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID RecvEnterMarket();					// ���忡 ����.	
	VOID RecvLeaveMarket();					// ���忡�� ������.
	// �������� �������� ����Ʈ�� �䱸�Ѵ�.
	VOID RecvListItem();
	// �������� ���.
	VOID RecvBuyItem();
	// �������� �Ǵ�.
	VOID RecvSellItem();

	VOID SendReloadWearItem();
	// �÷��̾ �����ϰ��ִ� �����۵��� ���� ���������� ������ ���Ѵ�.

	// �ش� �������� �� �� �ִ��� �˻��Ѵ�.

	// �÷��̾ �������� �����ϰ��ִ��� ã�´�.
	// �������� ã������ �迭������ �迭��ȣ�� ��ã���� -1�� �����Ѵ�.
	// �÷��̾��� �������� ����Ѵ�.
	// �÷��̾��� �������� �����Ѵ�.
	// �÷��̾��� �������� ������Ų��. (�������� ������ �ִٸ� ������, ���ٸ� ������)
	// �ش�������� �÷��̾ �����ϰ��ִ� ������ ��´�.
	// �÷��̾��� �������� ���ҽ�Ų��. (�������� ������ �ִٸ� ���Ҹ�, 0�̵Ǹ� ������)
	// �����ȿ����� �÷��̾��� ����
	
	/*
	VOID	RecvEnterInn();							// ������ ���ٴ� �޽����� �޾Ҵ�.
	VOID	RecvLeaveInn();							// �������� �����ٴ� �޽����� �޾Ҵ�.
	VOID	RecvEnterWaitRoom();					// ������ ���ǿ� ���ٴ� �޽����� �޾Ҵ�.
	VOID	RecvCreateChatRoomInInn();				// �������� ��ȭ���� �����.
	VOID	RecvJoinInnChatRoom();					// ���� ��ȭ�濡 ���ٴ� �޽����� �޾Ҵ�.
	VOID	RecvLeaveInnChatRoom();					// ���� ��ȭ�濡�� �����ٴ� �޽����� �޾Ҵ�.
	VOID	RecvEnterNoticeBoard();					// ���� �Խ��ǿ� ���ٴ� �޽����� �޾Ҵ�.
	VOID	RecvRegistNotice();						// �Խù��� ����Ѵٴ� �޽����� �޾Ҵ�.
	VOID	RecvDeleteNotice();						// �Խù��� �����Ѵ�.
	VOID	RecvNoticesList();						// �Խù��� ����Ʈ�� ���´�.
	VOID	RecvViewNotice();						// �Խù��� ����.	
	VOID	RecvModifyNotice();						// �Խù��� �����Ѵ�.
	VOID	RecvInnWorkerEnter();					// ���� ������ ����.
	VOID	RecvInnWorkerBuy();						// ������ ���.
	*/

	VOID	RecvBarrackGetSolderList();				// �뺴 List�� ��û�Ѵ�.
	VOID	RecvBarrackBuySolder();					// �뺴�� ����Ѵ�.
	VOID	RecvBarrackGetDismissalSolderList();	// �ذ��� �뺴 List�� ��û�Ѵ�.
	VOID	RecvBarrackDismissalSolder();			// �뺴�� �ذ��Ѵ�.

	VOID	RecvGBarrackGetSolderList();			// ����뺴 List�� ��û�Ѵ�.
	VOID	RecvGBarrackBuySolder();				// ����뺴�� ����Ѵ�.
	VOID	RecvGBarrackGetChangeJobSolderList();	// ������ ����뺴 List�� ��û�Ѵ�.
	VOID	RecvGBarrackChangeJobSolder();			// ����뺴�� �����Ѵ�.
	VOID	RecvGBarrackGetDismissalSolderList();	// �ذ��� ����뺴 List�� ��û�Ѵ�.
	VOID	RecvGBarrackDismissalSolder();			// ����뺴�� �ذ��Ѵ�.

	VOID    RecvGBarrackGetGeneralExList();			// 2�� ��� List ��û
	VOID	RecvGBarrackBuyGeneralEx();				// 2�� ��� ����Ѵ�.
	VOID	RecvGBarrackGetChangeGeneralExList();	// 2�� ��� ���� List ��û
	VOID	RecvGBarrackChangeGeneralEx();			// 2�� ��� �����Ѵ�.
	VOID	RecvGBarrackGetDismissalGeneralExList();// 2�� ��� �ذ� List ��û.
	VOID	RecvGBarrackDismissalGeneralEx();		// 2�� ��� �ذ��Ѵ�.

	VOID	RecvMBarrackGetMonsterList();			// ���� List�� ��û�Ѵ�.
	VOID	RecvMBarrackBuyMonster();				// ���͸� ����Ѵ�.
	VOID	RecvMBarrackGetDismissMonsterList();	// �ذ��� ���� List�� ��û�Ѵ�.
	VOID	RecvMBarrackDismissMonster();			// ���͸� �ذ��Ѵ�.

	VOID	RecvHospitalGetCureList();				// �뺴 ġ��List�� ��û�Ѵ�.
	VOID	RecvHospitalCureSolder();				// �뺴�� ġ���Ѵ�.
	VOID	RecvHospitalCureAllSolder();			// ��� �뺴�� ġ���Ѵ�.

	VOID	RecvWharfEnter();						// �εο� ��
	VOID	RecvWharfBuyTicket();					// ǥ�� ��
	VOID	RecvWharfExit();						// �εθ� ����
	VOID	RecvWharfWaitRoomEnter();				// �ε� ���ǿ� ��
	VOID	RecvWharfWaitRoomExit();				// �ε� ������ ����
	VOID	WharfGetDB();							// DB���� Ticket ������ ����

	// �ֽ� ó��
	VOID	RecvGovernmentEnter();
	VOID	RecvGovernmentLeave();
	VOID	RecvGovernmentInvestmentEnter();
	VOID	RecvGovernmentInvestmentLeave();
	VOID	RecvGovernmentInvestmentGetList();
	VOID	RecvGovernmentInvestmentSetInvestment();
	VOID	RecvGovernmentVillageInfo();				// ��û, �������� ���
	VOID	RecvProfitStatistics();
	VOID	RecvAuditProfitStatistics();
	VOID	RecvReqMaxVillageDefenceInvestment();
	VOID	RecvReqSetVillageDefenceInvestment();
	VOID	RecvGovernmentInvestmentGetOut();			// ��û, �����ѵ� ������ ������

	VOID	RecvGuildChangeFlag();						// ����� ����� �����Ѵ�.

	// ������ ������ �ٽ� �¶������� ���ư���.
	VOID	RecvReturnOnline();
	// 10�ʸ��� Heartbeat�� ������.
	VOID	SendHeartbeat();
	
	BOOL			CheckWeight(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity);
	BOOL			IsOverWeight();																	// ���ſ��� �ȱ� ���� �����ΰ�?

	BOOL			EnoughMoney(MONEY siEnoughMoney);												// ���� ���� �ִ��� Ȯ���Ѵ�.
	VOID			ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB = TRUE);						// ���� �׼��� �ٲ۴�.
	MONEY			IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB = TRUE);					// ���� ������Ų��.
	BOOL			DecreaseMoney(MONEY uiDecreaseMoney, BOOL bSendToDB = TRUE);					// ���� ����.
	MONEY			DecreaseMoneyReturnMoney(MONEY uiDecreaseMoney);					// ���� ����.

	BOOL            SaveMoneyAtBank(MONEY siMoney);                                                 // ���࿡�� �����Ѵ�.
	BOOL            WithdrawMoneyAtBank(MONEY siMoney);                                             // ���࿡�� �����Ѵ�.
	MONEY           ModifySaving(MONEY siMoney);                                                    //  
	MONEY           GetSaving();

	MONEY			GetMoney();																		// ���� ���´�.
	MONEY			GetMaxMoneyToGet();																// ���� �� �ִ� �ִ뵷�� ���´�.

	BOOL			IncreaseBonus(UI08 uiFollowerID, SI32 siBonus);									// ���ʽ��� ���������ش�.
	BOOL			IncreaseCredit(SI32	siCredit);													// �ſ뵵�� ���������ش�.

	UI32			IncreaseTradeCredit( UI32 uiIncreaseCredit, BOOL bSendToDB = TRUE );			// ���� �ſ뵵�� �ø���.
	VOID			DecreaseTradeCredit( UI32 uiDecreaseCredit, BOOL bSendToDB = TRUE );			// ���� �ſ뵵�� ���ҽ�Ų��.
	UI32			GetTradeCredit();																// ���� �ſ뵵�� ���´�.
	SI32			GetGiveCredit(MONEY TradeMoney);												// �����Ǵ� �ſ뵵�� ���Ѵ�.
	BOOL			CheckForbiddenName(char *szName);
	SI32			GetTradeGrade();																// �ſ� ����� ���´�.

	VOID			RecvReadDescript();											// ���� �ȳ����� �޴´�.
	VOID			RecvWriteDescript();										// ���� �ȳ����� ����.

	VOID			RecvVillageNation();

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Quest�� ���õ� ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID			RecvReqQuestSelectNPC();									// NPC�� ����������
	VOID			RecvReqQuestAcceptQuest();									// Quest�� ����������
	VOID			RecvReqQuestCancelQuest();									// �������� Quest�� ���������
	VOID			RecvReqQuestGetPlayQuest();									// �������� Quest List ��û

	SI16			Quest_FindPlayQuestNPC(SI16 NPCCode);						// �� NPC�� ���õȾ� Play�ϰ� �ִ� Quest�� ã�´�.
	BOOL			Quest_CheckCompleteQuest(SI16 QuestSlot);					// �� Quest�� �Ϸ�ɼ� �ִ��� �˻�
	VOID			Quest_EndQuest(SI16 QuestSlot);								// Quest�� �����Ѵ�.(DB�� �����ϰ�, ������ ������, Client�� �޼��� ���۱��� �Ѵ�.)
	SI16			Quest_FindNextQuest(SI16 NPCCode);							// �� NPC���Լ� �����Ҽ� �ִ� ���� Quest�� ã�´�.
	BOOL			Quest_CheckPlayQuestAbility(SI16 QuestCode);				// Quest�� Start�Ҽ� �ִ��� �˻�
	BOOL			Quest_CheckSkipQuestAbility(SI16 QuestCode);				// Quest�� Skip�Ҽ� �ִ��� �˻�
	SI16			Quest_FindLastPlayQuest(SI16 NPCCode);						// �� NPC���Լ� ���������� Play�� Quest�˻�
	BOOL			Quest_FindCompletedQuest(SI16 QuestCode);					// ������ ������ �ִ� Quest���� �˻�
	SI16			Quest_FindEmptyQuest(void);									// �� Quest Slot�� ã��
	SI16			Quest_FindQuest(SI16 QuestCode);							// �������� Quest�� ã��
	VOID			Quest_AllClear(void);										// ��� Quest �ʱ�ȭ, ����ϰ�
	VOID			Quest_CheckEventItem(SI16 siQuestSlot);			
	VOID			Quest_SetEventItem(SI16 siEventItemID,SI16 QuestSlot);

	VOID			Quest_AddQuestObject_KillMonster(SI32 MonsterCode, SI32 Count);		// ���� ���� Data
	VOID			Quest_AddQuestObject_Work(SI32 siWork);								// �ڽ��� ����
	VOID			Quest_AddQuestObject_PortalUse(SI32 siNum);
	VOID			Quest_AddQuestObject_Npc(SI32 siNpc);
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	â�� ���õ� ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID			RecvStorageInfo();					//	â�� �ִ� �������� �����ش�.
	VOID			RecvStorageInputItem();				//	������ �ֱ�
	VOID			RecvStorageOutputItem();			//	������ ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���忡 ���õ� ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	stFarmData*		GetOwnFactory();					// �÷��̾ �����ϰ� �ִ� �ü����� ���´�.
	VOID			RecvFarmInfo();						//	���� ����
	VOID			RecvFarmBuy();						//	���� ����
	VOID			RecvFarmSell();						//	���� �ȱ�
	VOID			RecvFarmSlotInfo();					//	���� ����
	VOID			RecvFarmUpgradeSlot();				//	���� ���׷��̵�
	VOID			RecvFarmBuildStruct();				//	�ǹ� �Ǽ�
	VOID			RecvFarmExpandSlot();				//	���� Ȯ��
	VOID			RecvFarmDestroyStruct();			//	�ǹ� ö��
	VOID			RecvFarmSetEquipment();				//	�ü��� ��ġ
	VOID			RecvFarmItemProduct();				//	������ ����
	VOID			RecvFarmWorkStart();
	VOID			RecvFarmWork();						//	���ϱ�
	VOID			RecvFarmWorkHack();

	BOOL			CheckLaborHack(UI08 uiType, SI08 siStep, UI32 uiQuantity);		//�ܰ迡 �´� ������ Ȯ��
	BOOL			CheckLaborHack1(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack2(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack3(SI08 siStep, UI32 uiQuantity);
	UI32			SumOfWorkQuantity(SI08& siStep, UI32& uiQuantity);		//��Ŀ�ú�� �Ϸ� üũ
	VOID			RecvFarmWorkInfo();					//	������ ������ ��û�ߴ�.
	VOID			RecvPlantChangPay();
	VOID			RecvPlantCancelPlan();
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ο� ���õ� ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	BOOL			GetTotalProperty(PROPERTY_INFO &PropertyInfo);			//	�ڽ��� �� �ڻ��� ��´�.
	MONEY			CalculateItemValue(MONEY &siItemMoney);					//	�ڽ��� �������� ��ġ�� ���Ѵ�.

	BOOL			GetAuditAccountList();									// ����� ���¸�� ��´�.
	BOOL			GetAuditOwnList();										// ����� �ε������ ��´�.
	BOOL			GetAuditVillageInfo();									// ����� �������� ��´�.
	BOOL			GetAuditStatus();										// ����� �������� ��´�.
	VOID			RecvAuditSetName();										//	��ο��� �뺴 �̸��� �Է��Ѵ�.

	VOID			RecvAuditTotalInfo();									// ����� ����� �䱸�� �޾Ҵ�.
	VOID			RecvAuditAccountList();									// ����� ���¸�� �䱸�� �޾Ҵ�.
	VOID			RecvAuditOwnList();										// ����� �ε������ �䱸�� �޾Ҵ�.
	VOID			RecvAuditVillageList();									// ��� ���� list �䱸�� �޾Ҵ�.
	VOID			RecvAuditFriendList();									// ��� ģ�� list �䱸�� �޾Ҵ�.
	VOID			RecvAuditFriendSetDenial();								// ��� ģ���� ���Űź�/���� �Ѵٴ� �䱸�� �޾Ҵ�.
	VOID			RecvAuditFriendAddOne();								// ��� ģ���� �߰��Ѵ�.
	VOID			RecvAuditFriendRemoveSome();							// ��ο��� ģ���� �����Ѵٴ� �䱸�� �޾Ҵ�.
	VOID			RecvAuditFriendRemoveAll();								// ��ο��� ģ�� ��ü�� �����Ѵٴ� �䱸�� �޾Ҵ�.
	VOID			RecvAuditVillageInfo();									// ����� �� �������� �䱸�� �޾Ҵ�.
	VOID			RecvAuditStatus();										// ����� �������� �䱸�� �޾Ҵ�.
	VOID			RecvAuditFollowerStatus();								// ����� �� ���忡 ���� ���� �䱸�� �޾Ҵ�.

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���忡 ���õ� ó��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	
//--------------------------------------------------------------------------------------------
// ������ ���忡���� ó��.
//--------------------------------------------------------------------------------------------
	
	VOID			RecvBankbookList();																// ���� ����Ʈ�� �޶�� �䱸�� �޾Ҵ�.
	VOID			RecvBankbookListCount();														// ���� ����Ʈ�� ���� �޶�� �䱸�� �޾Ҵ�.
	VOID			RecvBankInfo();																	// ������ ������ ���´�.
	VOID			RecvBankDeposit();																// ���࿡ ������ �Ѵ�.
	VOID			RecvBankDrawing();																// ���࿡�� ����� �Ѵ�.	
	VOID			RecvOpenSavingAccount();														// ���� ���¸� ����ٴ� �޽����� �޾Ҵ�.
	VOID			RecvBankCollateral();
	VOID			RecvBankCollateralList();
	VOID			RecvBankTakeCollateral();
	VOID			RecvMaxCreditLoan();

	VOID            RecvBankSave();
	VOID            RecvBankWithdraw();

	SI32			GetBankBootNum();																				// ������ �� ������ ���´�.
	DWORD			GetInfo(OnCharPlayerInfo *pPlayerInfo);															// ĳ������ ������ ���´�.	
	DWORD			GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount );	// ĳ������ �� ������ ���´�.

	VOID			RecvReqGuildCreateCost();									// ��� ������ �ʿ��� �ݾ��� ��û�ϴ� �޽����� �޾Ҵ�.
	VOID			RecvCreateGuild();											// ����� �����Ѵٴ� �޽����� �޾Ҵ�.
	VOID			RecvDeleteGuild();											// ����� �����Ѵٴ� �޽����� �޾Ҵ�
	VOID			RecvJoinLeaveGuild();										// ��ܿ� �����ϰų� ������.
	VOID			RecvReqGuildWaitingJoinMembersList();						// ��ܿ� �����ϱ⸦ ��ٸ��� ����ڵ��� ����Ʈ�� ���´�.
	VOID			RecvJoinGuildOkNo();										// ��ܿ� ������ ��� �� �ź��Ѵ�.
	VOID			RecvGuildDetailInfo();										// ����� ���� �� ������ ���´�.	
	VOID			RecvReqGuildMembersList();									// ��ܿ� ���� ����Ʈ�� ���´�.
	VOID			RecvGuildDismissal();										// ��ܿ��� �ذ��Ѵ�.
	VOID			RecvReqGuildPromotion();									// ��ܿ��� Ư�� �������� �Ӹ��Ѵ�.
	VOID			RecvReqGuildDemotion();										// ��ܿ��� Ư�� ���޿��� �����Ѵ�.
	VOID			RecvGuildList();											// ����� ����Ʈ�� ���´�.
	VOID			SetGuild(UI32 uiguildid);									// ����� �����Ѵ�.	
	VOID			SetClassInGuild(SI32 siclassinguild);						// ������ �����Ѵ�.
	SI32			GetClassInGuild();											// ������ ���´�.
	VOID			RecvReqUsedFlagList();										// ���� ����� ����Ʈ�� ����.
	VOID			RecvReqPIInMinimap();										// �̴ϸʿ��� �÷��̾��� ������ �䱸�Ѵ�.



	VOID			ChangeGuildFlag(SI32 siFlag);								// ��� ����� �����Ѵ�.
	VOID			ChangeSatiety(SI32 siValue);								// �������� �����Ѵ�.

//--------------------------------------------------------------------------------------------
// ���� �̵��� ���� ó��
//--------------------------------------------------------------------------------------------	
	VOID			RecvMoveMap();												// ���� �̵��ϴ� �޽����� �޾Ҵ�.
	BOOL			ChangeMap(UI16 uimapid, SI16 six, SI16 siy);				// ���� �����Ѵ�.
	VOID			SetNearestHospitalVillageIndexInDungeon(SI32 siVillageIndex);	// �������� �������� �׾ ���� �Ա����� ���� ����� �׾ ������ �ε����� �����Ѵ�.
	
	VOID			BoardShip(SI32 uiBoardShipAccount);							// �迡 ź��.
	VOID			ArriveVillageByShip(SOVillage *pVillage);					// �踦 Ÿ�� ������ �����Ͽ���.	

//--------------------------------------------------------------------------------------------
// ��� ���� �޽��� ó��
//--------------------------------------------------------------------------------------------	
	VOID			RecvCommand();												// ��� ��� �޽����� �޾Ҵ�.


//--------------------------------------------------------------------------------------------
// ���ʽ��� �̿��Ͽ� �ɷ�ġ�� �ø��� �޽��� ó��
//--------------------------------------------------------------------------------------------	
	VOID			RecvIncAbilityByBonus();									// ���ʽ��� ����Ͽ� �ɷ�ġ�� �ø��ٴ� �޽����� �޾Ҵ�.

//--------------------------------------------------------------------------------------------
// ���� ���� �޽���
//--------------------------------------------------------------------------------------------	
	VOID			PrepareBattle();											// ������ �����Ѵ�.
	VOID			EndBattle();												// ������ �����Ѵ�.

	VOID			AllCharacterDeleteLifePoint(SI32 DeleteLife);				// ��� Character�� Life�� ����.


//--------------------------------------------------------------------------------------------
// ����ٴϴ� �뺴
//--------------------------------------------------------------------------------------------
	VOID			RecvVMercenaryChange(SI08 siSlot);							// ����ٴϴ� �뺴 Change
	VOID			UpdateVMercenary(void);										// ����ٴϴ� �뺴 Change


//--------------------------------------------------------------------------------------------
// ������ �޽���
//--------------------------------------------------------------------------------------------	
	VOID			RecvPMOpen();												// �������� ����.
	VOID			RecvPMClose();												// �������� �ݴ´�.
	VOID			RecvPMBuyItem();											// �����󿡰� �������� ���.
	VOID			RecvPMReqItemInfo();										// �������� ������ �ִ� �������� ������ �䱸�Ѵ�.
	VOID			RecvPMChangeItem();											// �������� ������ �ִ� �������� �����Ѵ�.		

	
	BOOL			CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money);													// ���� �������� �������� �� �� �ִ°�?		
	BOOL			BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot);		// ���� �������� �������� ���.

	
	VOID			RecvAskTrade();															// Trade�� ���ڰ� ��û�Ѵ�. 
	VOID			RecvAcceptTrade();														// Trade�� ���ڰ� ��û�ѰͿ� ���� ������ ������ �޾Ҵ�.
	VOID			RecvFinishChooseTradeItem();											// Trade�� ������ ������ ��� ���´�.
	VOID			RecvDecideTrade();														// Trade�� ������ �Ѵٴ� ���� �����Ѵ�.
	VOID			RecvChangeTradeGoods();													// Trade�� �������� �������� �����Ѵ�.
	VOID			RecvCancelTrade();														// Trade�� ���� �������� Trade�� ����Ѵ�.
	VOID			RecvCompleteTradeBook();												// Trade�� ��� �ŷ��� �Ϸ�Ǿ���.
	VOID            RecvRequestPersonalInfo();
	BOOL			OnUseDonkeyItem( UI08 uiFollowerID, UI16 uiItemID );					// �糪�� ������ ������ ó��

	VOID			RecvRunProgramList();
	
	BOOL			CanTrade();																// ���� ������ �����Ѱ�?
	BOOL			IsTrading();															// ���� �ٸ� �÷��̾�� ���������� �˾ƿ´�.	
	BOOL			IsAcceptedTrading(UI16 uiAccount = 0);									// ���� �ٸ� �÷��̾ ������ �����ؼ� ������ �ϰ� �ִ��� �˾ƿ´�.
	BOOL			AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel );												// ������ �����ߴ�.
	BOOL			NotAcceptedTrade();														// ������ �������� �ʾҴ�.
	BOOL			CancelTrade();															// ���� �������� ������ ����Ѵ�.
	BOOL			IsWaitingAcceptTrade(UI16 uiAccount = 0);								// ������ ������ �³��ϴ� ���� ����ϴ��� �˾ƿ´�.
	BOOL			ChooseTradeItem();														// ������ �������� �����ߴ�.
	BOOL			CancelChooseTradeItem();												// ������ �������� �����ߴ� ���� ����Ѵ�.
	BOOL			IsChoseTradeItem(UI16 uiAccount = 0);									// ������ �������� �����ߴ��� �˾ƿ´�.
	BOOL			DecideTrade();															// �����ߴ� �����۵��� ������ ������ ���� �����Ѵ�.
	BOOL			CancelDecideTrading();													// �����Ϸ��� �ߴ� ���� ����Ѵ�.	
	BOOL			IsDecideTrading(UI16 uiAccount = 0);									// ������ �Ϸ��� ������ �ߴ��� �˾ƿ´�.	
	BOOL			AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2,
								OnFollowerInfo *pTradeFollowerInfo, DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo );	// ���� ��ǰ�� �߰��Ѵ�.
	BOOL			CanReceiveGoods(TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);	// ������ �ַ��� �ϴ� ��ǰ���� ��� ���� �� �ִ��� üũ�Ѵ�.
	VOID			TradeGoods( SOPlayer *pTargetPlayer, DWORD *pdwFollowerDataSize, OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum, 
							  DWORD *pdwTradeItemDataSize, UI16 puiTradeItem[ ON_MYDATA_MAX_ITEM_NUM * 2 ], UI16 *puiTradeItemNum, CHAR *pszDBTradeArg );		// ����� ������ �Ѵ�.
	DWORD			TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID);	// pTradePlayer���� uiFollowerID�� �뺴�� �ش�.
	VOID			CloseTrade();															// ����â�� �ݴ´�.
	BOOL			IsValidTradeGoods();													// ���� �����Ϸ��� ǰ���� ���� ��ȿ���� �˻��Ѵ�
	BOOL			CanAddFollower( UI16 uiFollowerNum );									// ���� �뺴�� �߰��� �� �ִ��� �˾ƿ´�.

	BOOL			HaveGeneral();															// ����� ������ �ִ��� �˾ƿ´�.
	BOOL			IsGeneral(UI08 uiFollowerSlot);											// ������� �˾ƿ´�.
	BOOL			IsFieldAttackUnit();

	DWORD			GetIPInNumeric();														// �÷��̾��� ���ڷ� �̷���� IP�� �޾ƿ´�.

	SI16			GetMaxFollowerNum();													// �÷��̾ ���� �� �ִ� �ִ� �뺴�� ��.(���ΰ� ����)
	BOOL			IsMonster( UI08 uiFollowerSlot );										// Ư�� ������ �������� �˻��Ѵ�.
	SI32			GetMonsterNum();														// ���� �����ϰ� �ִ� ���ͼ��� ���´�.
	BOOL			IsAdminPlayer();														// �÷��̾ ������� �˾ƿ´�.

	bool			expUp(int followerIndex, int value);									// ����ġ�� ���� ��Ų�� 

	VOID			RecvRequestVillageInfo();												// Ư�� ������ ������ ��û�Ѵ�.

	BOOL			GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer );						// ���濡�� ���� �ش�.

	BOOL			IsInVillage();															// ���� ���� �ȿ� �ִ��� �˾ƿ´�.
	BOOL			IsEnteredStructure();													// ���� ������ �ǹ� �ȿ� ������ �˻��Ѵ�.

	BOOL			MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory );	// �������� �̵� ��Ų��.


	// ������ ���� ���� 

	void			SendItemCount();

	// �������� ����

	VOID            RecvRefineItem();

	// ���� 108��ܸ��.

	SI32            GetChallengeLevel();                        // �� �÷��̾ ���° �������� ������ �˾ƿ´�.
	VOID            SetChallengeLevel(SI32 siChallengeLevel);   // ������ �����Ѵ�.

	// 2003.10.7 �߰���, ������

	BOOL			ProcessPartyMessage(VOID);

	BOOL			CanJoinParty();
	BOOL			CreateParty();	
	BOOL			LeaveParty();
	UI16			GetPartyId();		
	BOOL			JoinParty( UI16 uiPartyID );					// ��Ƽ�� ����.
	VOID			PartyDeleted();									// ��Ƽ�� �����Ǿ���.

	BOOL			PartyChat( CHAR *pszChatMsg );					// ��Ƽ���鿡�� ä�� �޽����� ������.

	VOID			CloseRequestingJoinParty();									// ������ ��Ƽ�� ���°ſ� ���ؼ��� ���� ��Ƽ ��û�Ѱſ� ���ؼ� �ź� �� ����Ѵ�.
	BOOL			AcceptRequestingJoinParty();								// ������ ���� ��Ƽ ��û�� ����ߴ�.
	BOOL			RejectRequestingJoinParty();								// ������ ���� ��Ƽ ��û�� �ź��Ѵ�.
	BOOL			CancelRequestingParty( BOOL bProcessByRequest = FALSE);		// ��Ƽ ��û �Ͽ������� ����Ѵ�.

	BOOL			IsPartyLeader();
	BOOL			IsRequestingJoinParty( UI16 uiAccount = 0 );	
	BOOL			IsPartyMember();
	BOOL			IsReceivedRequestJoinParty();
	
	BOOL			CanRequestJoinParty();							// ���� ���濡�� ��Ƽ�� ��û�� �� �ִ��� �˾ƿ´�.
	BOOL			CanReceiveRequestJoinParty();					// ���� ��Ƽ ��û�� ���� �� �ִ��� �˾ƿ´�.
	VOID			RequestJoinParty( UI16 uiAccount );				// �ش� �������� ��Ƽ�� ���� ��û�� ���� ��û�ߴ�.
	VOID			ReceiveRequestJoinParty( UI16 uiAccount );		// �ش� �������� ��Ƽ�� ���� ��û�� ���� �޾Ҵ�.
	VOID			SetPartyLeader();								// ��Ƽ���� �Ǿ���.	

	// 2003.10.7 �߰���, ������
	// party message ó�� ��� 

	VOID			RecvRequestParty();
	VOID			RecvCancelRequestingParty();
	VOID			RecvRequestAcceptParty();
	VOID			RecvRequestLeaveParty(); 
	VOID			RecvCancelRequestParty();	

	VOID			RecvGetAuditGuildInfo();
	VOID			RecvMacroUsingUserInfo();

	VOID			SetEquipItems(EquipItemInfo* ItemInfom,SI16 siMercenarySlot,SI16& siTimerItemUseCount,OnlineFollower* pFollower);
	BOOL			CheckGetTimeLimitItem();
	SI16			GetRestTime(SI32 start,SI32 end);
	BOOL			IsDeleteTimerItem(SI16 siRestTime,SI16 siMaxTime);
	SI16		    GetClientSendDeleteTimerItem(OnTimerItemInfo* pTimerItemInfo);
	VOID			DeleteTimerItem();											// �ð��� �����۵� �����Ѵ�.

	SI16			GetMDay(SI16 siMon);
	SI16			GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay);
	VOID			GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay);

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ���� ĳ���Ϳ� ���� �޼��� ó�� �Լ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	VOID			RecvFieldAttackReady();										// �÷��̾� ���ݴ��
	VOID			RecvFieldAttackAction();									// �÷��̾� �Ѿ� �´´�.			
	VOID			RecvFieldAttackDead();										// �÷��̾ �״´�.

	VOID			RecvLongDistanceAttackVillageReady();						// �÷��̾ ������ ������.���(���Ÿ�)
	VOID			RecvLongDistanceAttackVillageAction();						// �÷��̾ ������ ������.����(���Ÿ�)

	VOID			RecvAttackVillage();										// ������ �����Ѵٴ� �޽����� �޾Ҵ�.(�ٰŸ�)

	VOID			RecvReqWarVillageDefence();									// �ʵ� -> ���� �������̽�
	VOID			RecvReqWarVillageIn();										// ���� �������̽� -> ����

	VOID			RecvPlayerVillageDefenceUp();								// ���� ������ ������ �ø���.
	VOID            RecvReqWarVillageDefenceStop();                             // ���� �������̽��� �����Ѵ�.

	VOID			RecvIncreaseGuildUnit();									// ����� ���������� ����.(����)
	VOID			RecvDecreaseGuildUnit();									// ����� ���������� ����.(����)	
	VOID			RecvGetGuildUnitList();										// ����� ���������� ����Ʈ�� �޾ƿ´�.

	VOID            RecvHeal();                                                 // ���޼����� ������ �������ش�.
	VOID            RecvChargeMP();                                             // ���޼����� ������ ȸ���Ǿ���.

	VOID			RecvGuildSetRelation();										// ��尣�� ���Ը� �����Ѵٴ� �޽����� �޾Ҵ�.

	VOID			RecvFriendlyMessageOffer();									// ȭģ���� �޼����� ������.
	VOID			RecvFriendlyMessageAccept();								// ȭģ���� �޼����� �޾Ƶ��δ�.
	VOID			RecvFreidnlyMessageCancel();								// ȭģ���� �޼����� �ô´�.

	VOID			RecvVillageAttackResult();									// ������ ���� ���.
	VOID			RecvChoicePlayer();											// ��������Ʈ���� �÷��̾ �����Ѵ�.
	VOID			RecvInvestmentPower();

	VOID			RecvWarWarp();												// �������� ������ �Ѵ�.
	VOID			RecvNormalChar();											// �ڽ��� ���� ĳ���� �Ϲ� ĳ������ �����.

	VOID            RecvChangeUnit();                                           // ������������ ���ſ�û
	VOID            RecvGiveUp();                                               // �������⸦ �Ͽ���. 

	

	
//	VOID			RecvVillageAttackTypeCheck();								// ���� ����Ÿ���� �ٲܼ� �ִ��� üũ�Ѵ�.
//	VOID			RecvVillagePlayerOilAttack();								// ���� ������ ���� ���ݹ��(�⸧)
//	VOID			RecvVillagePlayerStoneAttack();								// ���� ������ ���� ���ݹ��(��)

//	VOID			RecvIncreaseGuildSupply();									// ����̳� ������� ���� ����ǰ�� ���� ������Ų��.
//	VOID			RecvDecreaseGuildSupply();									// ���޼����� ����ǰ�� ��������.
//	VOID			RecvGetGuildSupplyList();
//	VOID			RecvGetSupplyAtGuildUnit();									// ���޼������� ����ǰ�� ä���.

//	VOID			RecvIncreaseVillageDefenceSoldier();						// ������ ���� ���� ��Ų��.
//	VOID			RecvGetVillageDefenceSoldier();								// ������ ���� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���� ���� ĳ���Ϳ� ���� �Լ� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
	SI16			GetFieldHp();
	void			SetFieldHp(SI16 siFieldHp);

	SI16            GetFieldMaxHP();
	void            SetFieldMaxHP(SI16 siMaxHP);

	void			IncreaseFieldHp(SI16 siFieldHp);
	void			DecreaseFieldHp(SI16 siFieldHp);

	BOOL			IsFieldAlive();

	SI32            GetFieldMP();
	void            SetFieldMP(SI32 siMP);

	SI16            GetFieldMaxMP();
	void            SetFieldMaxMP(SI32 siMaxMP);

	void			IncreaseFieldMP(SI16 siFieldMP);
	void			DecreaseFieldMP(SI16 siFieldMP);

//	SI32			GetFieldSupply();
//	void			SetFieldSupply(SI32 siFieldSupply);
//	SI32			GetFieldSupplyCharge();							   //���޼����� ������ �ִ� �����l�� ���´�.
//	void			SetFieldSupplyCharge(SI32 siFieldSupplyCharge);	   //���޼������� ����ǰ�� �ִ´�.(����->���޼���)
//	void			DecreaseSupplyCharge(SI32 siDecreaseSupplyCharge); //���޼����� ����ǰ�� ��������.
//	void			InitFieldSupplyCharge(SI32 siFieldSuppyCharge);

	void		    SetPrevKind(UI16 uiPrevKind);
	UI16			GetPrevKind();

	void		    SetCaptainUnit(BOOL	bCaptain);
	BOOL			GetCaptainUnit();
	
	BOOL			IsFieldAliveWarUnit();

	VOID			SendGuildItemDeleteMessage();
	VOID			DeleteFiledUnit();											// 10�ÿ� ���������� �����Ѵ�.



	BOOL            FieldAttackDead();
	UI16            ChangeUnit(UI16 uiCode, UI16 uiQuantity);//������������ �����ϴ�...
	VOID            ChangeNormalChar();
	
	UI16            GetDir(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY);
	UI16            GetSpTargetIDOFSpear(UI16 uiDir, SI32 siSX, SI32 siSY, UI16 *puiIDs); //â���ΰ�� ���÷��� �������� �Դ� ĳ���� ���̵� ���Ѵ�
    SI32            GetSpTargetIDOFSiege(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *puiIDs); //�߼����ΰ��
//��������������������������������������������������������������������������������������������������������������������������������������������������������������

// write �Լ����� ���� ������ ����..
private:
	UI16    usPacketSize;

	UI16	usSize;
	UI32	uiSize;
	
	char	cCheckSum;

	UI32	uiCmd;

//  2003.20.07 ��Ƽ �ý��� �߰��� - ������ 
	
	party_state_t	m_PartyState;

};	

#endif	