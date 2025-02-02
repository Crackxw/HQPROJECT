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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 薑爾
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
typedef struct
{
	char			id			[LOGIN::ID_MAX_LENGTH];
	char			password	[LOGIN::PW_MAX_LENGTH];

	char			IP[16];							//	ip輿模(char ⑽)
	DWORD			dwIP;							//	DWORD⑽.
	SI32			siDBAccount;					//	Ы溯檜橫蒂 掘碟ж朝 等檜攪漆檜蝶鼻曖 嶸橾и 廓��
	UI08			uiSlot;							//	馨葛攪曖 蝸煜廓��.
	UI16			uiStatusInVillage;				// 葆擊 寰縑憮 菟橫陛 氈朝 勒僭.
	//UI16			uiChatRoomIDInInn;				// 葆擊, 輿薄曖 渠�食瞈� 霤罹ж堅 氈朝 寞曖 檣策蝶.

	SOBattle		*pBattle;						// ⑷營 霤陛ж堅氈朝 瞪癱寞.
	SOVillage		*pVillage;
	stFarmData		*pStructure;					// ⑷營 橾ж堅 氈朝 勒僭.

} ACCOUNT_INFO;



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 寡ル 薑爾.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
typedef struct
{
	SI32		siTicketKind;					// Ticket Kind
} TICKET_INFO;



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// Ы溯檜橫曖 濠骯 薑爾.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
typedef struct
{
	MONEY		siTotalProperty;				//	識營骯
	MONEY		siMoneyProperty;				//	⑷旎
	MONEY		siSaveProperty;					//	瞪濰 濤堅 (+)高
	MONEY		siLoanProperty;					//	瞪濰 渠轎 (-)高
	MONEY		siItemProperty;					//	嬴檜蠱擊 �粉糒� 絲
	UI32		uiRanking;						//	濠褐曖 楨韁
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
	// 牖熱 啪歜縑憮曖 等檜顫.
public:

	///////////////////////////////////////////
	// 議葛攪 蝸煜 薑爾
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

	// --  煎斜檣 婦溼 
	void		ProcessLogin();
	LoginInfo  *logininfo;
	BOOL		bSuccessLogin;								// 煎斜檣擊 撩奢瞳戲煎 ц朝陛?
	BOOL		bProcessLogin;
		
	UserGrade	USGrade;									// 褐辨 蛔晝.
	MONEY		siMoney;									// 絲
	
	MONEY		investProfit;								// 癱濠 熱櫛 
	MONEY		additionProfit;								// 睡陛 熱櫛 (勒僭 つ陽)
		
	UI32		uiTradeCredit;								// 鼠羲 褐辨紫
	SI32		siDayCredit;								// ж瑞縑 螃艇 褐辨紫
	
	UI32		uiGuildID;									// 鼻欽曖 ID
	SI32		siClassInGuild;								// 望萄頂縑憮曖 霜晝
	SI32		siFlag;										// 梓嫦	

	SI32		siNearestHospitalVillageIndexInDungeon;		// 湍瞪縑憮 陛濰 陛梱遴 曖錳擊 陛雖堅 氈朝 葆擊曖 檣策蝶
	
	DWORD		dwPrevSavePosTime;							// 檜瞪縑 頂 嬪纂蒂 DB縑 盪濰и 衛除
	DWORD		dwHeartbeatTime[ON_HEARTBEAT_SAVE_NUM];		// Ы溯檜橫陛 髦嬴氈朝雖 匐餌й 衛除


	cltPrivateMarket	clPrivateMarket;					// 偃檣 衛濰
	cltTrade			clTrade;							// 掖羲

	CSpeedHackChecker	m_SpeedChecker;
	BOOL				m_bIsSpeedHackUser;		
	
	SI08		m_siMaxGeneral;								// 堅辨 陛棟и 濰熱曖 熱
	BOOL		m_bCompareWearItem;

	SI16		m_siTimerItemUseCount;

// gguo
// -----------------------------------------------------------------------------------------------------

private:
	SI32                m_siChallengeMode;                          // 紫瞪 108啗欽 賅萄.
	ItemBankList		m_ItemBankList;

	// GameGuard婦溼
	DWORD				m_lastGameGuardCheckTime;
	CCSAuth				*m_pCSAuth;
	BOOL				m_bCheckPass;

	DWORD				m_dwCondLaborTime;							// か薑 褻勒擊 虜褶ж朝 檜瞪 喻翕 衛除

public:
	MONEY				m_MoneySaving;                              // 盪蹴п場擎 絲.

// kojimodo 
//------------------------------------------------------------------------------------------------------
private:
	int					m_satiety;									// ん虜馬
	int					m_orgSatiety;								// 錳獄 ん虜馬 
	
	WORD				m_latestSavedMapID;							// 譆斬縑 盪濰и MapID
	POINT				m_latestSavedPos;							// 譆斬縑 盪濰и 嬪纂 
	DWORD				m_latestSavedPosTime;						// 譆斬縑 嬪纂蒂 盪濰и 衛除 

	CSkillMgr*			m_pSkillMgr;								// 蝶鑒 婦葬濠 


	CAuctionMarketProc* m_pAuctionMarketProc;						// 唳衙模 ぬ韃 籀葬 贗楚蝶 
	DWORD				m_tempBiddingPrice;							// 唳衙縑憮 歜衛煎 殮雞陛問擊 爾婦ж朝 滲熱 
	BOOL				bGuildInfoChanged;							// 望萄 薑爾 滲唳ц朝雖?
	

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
	BOOL		Trade( SOPlayer *pTargetPlayer );			// 掖羲醞檣 ヶ跡縑 渠п憮 剪楚蒂 ж塭堅 憲溥遽棻.

public:	
	SOPlayer();												// 儅撩濠	
	~SOPlayer();											// 模資濠

public:
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// Time 鼻鷓 滲熱 
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	struct tm			*pNewTime;
	time_t				stClock;

public:
	SOServer	*clServer;		

public:
	// Packing縑 婦溼脹 幗ぷ諦 望檜
	char		pmsg[30000];
	char		*cMsg;							
	SI16		siMsgLength;							// 詭撮雖曖 觼晦

	JXPacket	local_packet;
public:
	SI32		Write(const VOID *, SI32 Length);
	SI32		ReadFromBattle(SI32 siLength);
	SI32		WriteToBattle(const VOID *, SI32 siLength);
	

public:
	EquipItemInfo	a;
	// Ы溯檜橫曖 鼻鷓蒂 釭顫魚棻. (嘐蕾樓檣陛?, 蕾樓醞檣陛?, 渠晦褒檣陛?, 渠�食磍帡�?, 啪歜醞檣陛?)
	UI08			uiState;

	// 勒僭寰縑憮 濠褐曖 嬪纂
	UI32			uiStructerInPos;

	// Ы溯檜橫曖 ル 薑爾
	TICKET_INFO		stTicketInfo;

	// Quest 薑爾
	SPlayerQuest	stQuestInfo;

	// Ы溯檜橫曖 蕾樓 薑爾
	ACCOUNT_INFO	stAccountInfo;	
	// Ы溯檜橫曖 濠骯 薑爾 
	PROPERTY_INFO	stPropertyData;

	SI08			siVMercenarySlot;				// 評塭棻棲朝 睡ж Slot

	// 褐辨 渠轎 и紫
	UI32			uiMaxLoan;

	SI32			uiBoardShipAccount;				// ⑷營 顫堅 氈朝 寡曖 Account

	XPlayer			*m_pXPlayer;					// 薯諫 蹺陛..

	UI16			m_uiRecvItemNum;

	BOOL			m_bDayChange;					

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	//衛除薯 嬴檜蠱 婦溼 滲熱.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	DWORD			m_dwUserhour;
	DWORD			m_dwUserMin;
	DWORD			m_dwUserSec;

	OnTimerItemInfo		m_stTimerItemInfo[12][9];
	OnTimerItemInfo		m_pTimerItemInfo[12];		// 橾欽 寡翮煎.-_-臏.


	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 奢撩 議葛攪縑 婦溼 滲熱.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	SI16			m_siFieldHp;					// 奢撩瞪衛 �僱磈炴� Hp.
	SI16            m_siFieldMaxHP;                 // 奢撩瞪衛 �僱磈炴� 譆渠 hp 

	SI32            m_siFieldMP;                    // 奢撩瞪衛 �僱磈炴� MP
	SI32            m_siFieldMaxMP;                 // 奢撩瞪衛 �僱磈炴� 譆渠MP

//	SI32			m_siSupply;						// 奢撩瞪衛 �僱磈炴� 爾晝.
//	SI32			m_siSupplyCharge;				// 奢撩瞪 醱瞪 啪檜雖. (爾晝熱溯虜 橢擊熱 氈棻.)
	BOOL			m_bFieldAlive;					// 奢撩瞪衛 髦懊釭 避歷釭.避戲賊 в萄縑 斜渠煎.5碟除 奢撩嶸棉戲煎 滲褐碳陛.
	UI16			m_uiPrevKind;					// 奢撩嶸棉戲煎 滲ж晦瞪曖 議葛攪 Kind
	BOOL			m_bCaptainUnit;					// 渠濰嶸棉檣陛.

	DWORD			m_dwDeadTime;					// 奢撩衛 避擎 衛除 晦煙. 煎斜 嬴醒 ц擊 唳辦 
	DWORD			m_dwDeadSec;					// 避歷擊陽 ぬ割じ煎 嫡擎 衛除.

	BOOL			m_bInvestmentPower;
	BOOL			m_bFieldUnitUpdate;
	BOOL			m_bPlayerVillageInLimit;
	SI16            m_siVilligeCode;                // 熱撩衛 熱撩擊 ж堅 氈朝 葆擊囀萄
	
	BOOL            m_bIsFirstDefence;              // 撩 寞橫溘擊 籀擠 螢溜釭.
	DWORD           m_dwDefenceTime;                // 撩寞橫溘擊 螢萼衛除...
	DWORD           m_dwChargeTime;                 // 奢撩衛 爾晝熱溯曖 葆釭陛 �蛹僱� 衛除.
	DWORD           m_dwMessageTime;                //
	BOOL            m_bIsWarUnit;                   //
	DWORD           m_dwAttackDelay;                // 奢問 裁溯檜 衛除
	SI16            m_siKillNum;                    // 避檣 熱 
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 啻お錶觼 婦溼л熱
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
public:
	// 議葛攪 蟾晦��
	VOID	Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID);					
 
	// Ы溯檜橫曖 模鰍 蟾晦��
	VOID	Init();

	// 僥濠翮 ip蒂 DWORD ⑽衝戲煎 夥脯遽棻.
	DWORD	ChangeIPStr(char* lpIP);

	// Ы溯檜橫曖 陝謙 滲熱 蟾晦��
	VOID	InitEnv();

	// Ы溯檜橫諦 婦溼и 賅萇 翱骯 熱ч
	VOID	Poll(VOID);
	
	// 賅萇 詭撮雖蒂 籀葬и棻.
	VOID	ProcessMessage(VOID);
	BOOL	ProcessMessageInField(VOID);
	BOOL	ProcessMessageInVillage(VOID);
	BOOL	ProcessMessageInBattle(VOID);
	BOOL	ProcessMessageCommonly(VOID);

	// 煎斜嬴醒 и棻.
	VOID	LogOut(VOID);
		
	// Ы溯檜橫蒂 儅撩и棻.
	VOID	Create( SOServer *clTempServer );					// Ы溯檜橫蒂 儅撩и棻.

public:
	// 煎斜檣 籀葬
	VOID	RecvLogIn(char *pMsg);
	VOID	SendLogIn(BOOL bSuccess);


public:
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 啪歜 等檜顫 籀葬.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// Ы溯檜橫縑啪 詭衛雖蒂 爾魚棻.
	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);	

	// 絲擊 爾魚棻.
	VOID		SendMoney(DWORD dwKind);
	VOID		GiveBonusMoney(MONEY Bonus, DWORD dwKind, BOOL bSendToDB = TRUE);		// 絲擊 遽棻.

	// 雖羲鬚餌 蹂羶擊 嫡懊棻 
	void	recvProbeArea();

	// 檜賅じ夔 嫡懊棻.
	VOID	RecvImoticon();
	 
	// Ы溯檜橫曖 薑爾蒂 爾魚棻.
	// Ы溯檜橫曖 嬪纂蒂 撲薑и棻.
	VOID	RecvPosition();
	
	// か薑 Ы溯檜橫曖 嬪纂蒂 蹂羶и棻.
	// か薑 Ы溯檜橫曖 嬪纂蒂 爾魚棻.
	// か薑 Ы溯檜橫曖 薑爾蒂 蹂羶и棻
	VOID	RecvPlayerInfo();

	VOID	RecvCharDetailInfo();								// 議葛攪曖 鼻撮 薑爾蒂 蹂羶и棻朝 詭衛雖蒂 嫡懊棻.
	
	// 嬴檜蠱擊 模綠ц棻.
	VOID	RecvSpendItem();
	BOOL	RecvWarp();											// 錶Щ и棻朝 詭衛雖蒂 嫡懊棻.			
	BOOL	Warp(UI16 uiMapID, SI32 siX, SI32 siY);				// 錶Щ蒂 ж堅 輿嬪曖 等檜顫蒂 嫡朝棻.

//	VOID	RecvDeleteTimerItem();

	// 葆擊縑 菟橫除棻.
	VOID	RecvEnterVillage();
	BOOL	EnterVillage( BOOL bIsVillageField = FALSE );		// 葆擊縑 菟橫除棻
	VOID	LeaveVillage( BOOL bIsVillageField = FALSE );		// 葆擊縑憮 集陪棻.

	VOID	RecvEnterStructure();								// 葆擊縑憮 か薑 勒僭縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvLeaveStructure();								// 葆擊縑憮 か薑 勒僭縑憮 釭除棻朝 詭衛雖蒂 嫡懊棻.

	VOID	RecvEnterFactory();									// 葆擊 骯機 欽雖曖 奢濰縑 菟橫除棻.
	VOID	RecvLeaveFactory();									// 葆擊 骯機 欽雖曖 奢濰縑憮 釭除棻.
	

	// в萄鼻縑 Ы溯檜橫陛 嬪纂й 熱 氈朝 謝ル蒂 晦遽薄擊 醞褕戲煎 瓊朝棻.
	// 瞪癱蒂 蹂羶и棻.
	VOID	RecvBattle(VOID);
	VOID	SendDeadPlayerToVillageHospital();					// 避擎 議葛攪蒂 葆擊 曖錳縑 爾魚棻.
	VOID	NotifyFriendsOfLoginState( BOOL bLogin );			// 釭蒂 耀掘煎 蛔煙и 餌塋菟縑啪 煎斜檣ц擠擊 憲萼棻.

	VOID	RecvSetAttackMode();									// 瞪癱賅萄 / ゎ�飛藒� 撲薑

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// Common
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID	RecvHeartBeat();
	// Ы溯檜橫模嶸曖 嬴檜蠱葬蝶お蒂 蹂羶и棻.
	// Ы溯檜橫模嶸曖 嬴檜蠱曖 羹觼撰擊 蹂羶и棻.
	
	// 嬴檜蠱擊 檜翕 衛鑑棻.
	VOID	RecvShareItem();
	// 嬴檜蠱擊 幗萼棻.
	VOID	RecvThrowItem();
	// 嬴檜蠱擊 鄹朝棻.
	VOID	RecvPickUpItem();
	
	
	VOID	RecvWearItem();
	VOID	RecvStripItem();
	
	BOOL	GetWearItemInfo(UI08 uiFollowerID);
	BOOL	StripItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	WearItem(	UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	BOOL	SpendItem( UI08 uiFollowerID, UI16 uiItemID, UI16 uiPosInventory, UI16 uiQuantity = 1);
	BOOL	DisAppearItem( UI08 uiFollowerID, UI08 uiWearLoc, UI16 uiItemID, SI16 uiPosInventory);
	// Ы溯檜橫模嶸曖 嬴檜蠱葬蝶お曖 ぬ韃擊 虜萇棻. (ぬ韃曖 望檜蒂 葬欐и棻)
	// Ы溯檜橫模嶸曖 嬴檜蠱菟曖 п渡葆擊縑憮曖 僭陛 ぬ韃擊 虜萇棻. (ぬ韃曖 望檜蒂 葬欐и棻)
	// 嬴檜蠱曖 羹觼撰擊 羹觼и棻.
	// 羹觼撰高擊 虜萇棻.	

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// GameGuard婦溼
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID	CheckGameGuard(VOID);
	VOID	RecvRequestCheckGameGuard(VOID);
	VOID	RecvUserInfoGameGuard(VOID);

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 盪濠 剪葬縑憮曖 籀葬
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID	RecvChargeBooth();
	VOID	RecvLeaveBooth();
	VOID	RecvDisplayBooth();
	VOID	RecvSetBoothItem();
	VOID	RecvCancelBoothItem();
	VOID	RecvBoothInfo();
	VOID	RecvBuyBoothItem();

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 葆擊寰縑憮曖 籀葬
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

	// 輿虞縑憮 嬴檜蠱 葬蝶お, take out
	VOID	RecvItemList();
	VOID	RecvTakeOutItem();
	VOID	RecvSaveItemIntoItemBank();


	// 葆擊縑憮 集陪棻.
	VOID RecvLeaveVillage();
	// Ы溯檜橫曖 模雖旎擊 滲唳и棻.
	

	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	// 葆擊曖 衛濰縑憮曖 籀葬.
	//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID RecvEnterMarket();					// 衛濰縑 菟橫除棻.	
	VOID RecvLeaveMarket();					// 衛濰縑憮 釭除棻.
	// 鼻薄縑憮 嬴檜蠱曖 葬蝶お蒂 蹂掘и棻.
	VOID RecvListItem();
	// 嬴檜蠱擊 骯棻.
	VOID RecvBuyItem();
	// 嬴檜蠱擊 っ棻.
	VOID RecvSellItem();

	VOID SendReloadWearItem();
	// Ы溯檜橫陛 模雖ж堅氈朝 嬴檜蠱菟曖 ⑷營 葆擊縑憮曖 僭陛蒂 掘и棻.

	// п渡 嬴檜蠱擊 髦 熱 氈朝雖 匐餌и棻.

	// Ы溯檜橫陛 嬴檜蠱擊 模雖ж堅氈朝雖 瓊朝棻.
	// 嬴檜蠱擊 瓊懊戲賊 寡翮縑憮曖 寡翮廓�ㄧ� 跤瓊戲賊 -1擊 葬欐и棻.
	// Ы溯檜橫曖 嬴檜蠱擊 蛔煙и棻.
	// Ы溯檜橫曖 嬴檜蠱擊 餉薯и棻.
	// Ы溯檜橫曖 嬴檜蠱擊 隸陛衛鑑棻. (嬴檜蠱檜 晦襄縑 氈棻賊 隸陛蒂, 橈棻賊 儅撩擊)
	// п渡嬴檜蠱曖 Ы溯檜橫陛 模嶸ж堅氈朝 偎熱蒂 橢朝棻.
	// Ы溯檜橫曖 嬴檜蠱擊 馬模衛鑑棻. (嬴檜蠱檜 晦襄縑 氈棻賊 馬模蒂, 0檜腎賊 餉薯蒂)
	// 葆擊寰縑憮曖 Ы溯檜橫曖 鼻鷓
	
	/*
	VOID	RecvEnterInn();							// 輿薄縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvLeaveInn();							// 輿薄縑憮 釭除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvEnterWaitRoom();					// 輿薄曖 渠晦褒縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvCreateChatRoomInInn();				// 輿薄縑憮 渠�食磍� 虜萇棻.
	VOID	RecvJoinInnChatRoom();					// 輿薄 渠�食瞈� 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvLeaveInnChatRoom();					// 輿薄 渠�食瞈□� 釭除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvEnterNoticeBoard();					// 輿薄 啪衛っ縑 菟橫除棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvRegistNotice();						// 啪衛僭擊 蛔煙и棻朝 詭衛雖蒂 嫡懊棻.
	VOID	RecvDeleteNotice();						// 啪衛僭擊 餉薯и棻.
	VOID	RecvNoticesList();						// 啪衛僭曖 葬蝶お蒂 橢橫螞棻.
	VOID	RecvViewNotice();						// 啪衛僭擊 獄棻.	
	VOID	RecvModifyNotice();						// 啪衛僭擊 熱薑и棻.
	VOID	RecvInnWorkerEnter();					// 颶羞 堅辨塢縑 菟橫除棻.
	VOID	RecvInnWorkerBuy();						// 颶羞擊 骯棻.
	*/

	VOID	RecvBarrackGetSolderList();				// 辨煽 List蒂 蹂羶и棻.
	VOID	RecvBarrackBuySolder();					// 辨煽擊 堅辨и棻.
	VOID	RecvBarrackGetDismissalSolderList();	// п堅й 辨煽 List蒂 蹂羶и棻.
	VOID	RecvBarrackDismissalSolder();			// 辨煽擊 п堅и棻.

	VOID	RecvGBarrackGetSolderList();			// 濰熱辨煽 List蒂 蹂羶и棻.
	VOID	RecvGBarrackBuySolder();				// 濰熱辨煽擊 堅辨и棻.
	VOID	RecvGBarrackGetChangeJobSolderList();	// 瞪霜й 濰熱辨煽 List蒂 蹂羶и棻.
	VOID	RecvGBarrackChangeJobSolder();			// 濰熱辨煽擊 瞪霜и棻.
	VOID	RecvGBarrackGetDismissalSolderList();	// п堅й 濰熱辨煽 List蒂 蹂羶и棻.
	VOID	RecvGBarrackDismissalSolder();			// 濰熱辨煽擊 п堅и棻.

	VOID    RecvGBarrackGetGeneralExList();			// 2離 濰熱 List 蹂羶
	VOID	RecvGBarrackBuyGeneralEx();				// 2離 濰熱 堅辨и棻.
	VOID	RecvGBarrackGetChangeGeneralExList();	// 2離 濰熱 瞪霜 List 蹂羶
	VOID	RecvGBarrackChangeGeneralEx();			// 2離 濰熱 瞪霜и棻.
	VOID	RecvGBarrackGetDismissalGeneralExList();// 2離 濰熱 п堅 List 蹂羶.
	VOID	RecvGBarrackDismissalGeneralEx();		// 2離 濰熱 п堅и棻.

	VOID	RecvMBarrackGetMonsterList();			// 跨蝶攪 List蒂 蹂羶и棻.
	VOID	RecvMBarrackBuyMonster();				// 跨蝶攪蒂 堅辨и棻.
	VOID	RecvMBarrackGetDismissMonsterList();	// п堅й 跨蝶攪 List蒂 蹂羶и棻.
	VOID	RecvMBarrackDismissMonster();			// 跨蝶攪蒂 п堅и棻.

	VOID	RecvHospitalGetCureList();				// 辨煽 纂猿List蒂 蹂羶и棻.
	VOID	RecvHospitalCureSolder();				// 辨煽擊 纂猿и棻.
	VOID	RecvHospitalCureAllSolder();			// 賅萇 辨煽擊 纂猿и棻.

	VOID	RecvWharfEnter();						// 睡舒縑 菟橫馬
	VOID	RecvWharfBuyTicket();					// ル蒂 鳴
	VOID	RecvWharfExit();						// 睡舒蒂 釭馬
	VOID	RecvWharfWaitRoomEnter();				// 睡舒 渠晦褒縑 菟橫馬
	VOID	RecvWharfWaitRoomExit();				// 睡舒 渠晦褒擊 釭馬
	VOID	WharfGetDB();							// DB縑憮 Ticket 薑爾蒂 橢橫褥

	// 輿衝 籀葬
	VOID	RecvGovernmentEnter();
	VOID	RecvGovernmentLeave();
	VOID	RecvGovernmentInvestmentEnter();
	VOID	RecvGovernmentInvestmentLeave();
	VOID	RecvGovernmentInvestmentGetList();
	VOID	RecvGovernmentInvestmentSetInvestment();
	VOID	RecvGovernmentVillageInfo();				// 婦羶, 葆擊薑爾 橢晦
	VOID	RecvProfitStatistics();
	VOID	RecvAuditProfitStatistics();
	VOID	RecvReqMaxVillageDefenceInvestment();
	VOID	RecvReqSetVillageDefenceInvestment();
	VOID	RecvGovernmentInvestmentGetOut();			// 婦羶, 癱濠и絲 陛雖堅 釭陛晦

	VOID	RecvGuildChangeFlag();						// 鼻欽曖 梓嫦擊 滲唳и棻.

	// 瞪癱蒂 部頂堅 棻衛 螞塭檣戲煎 給嬴除棻.
	VOID	RecvReturnOnline();
	// 10蟾葆棻 Heartbeat蒂 爾魚棻.
	VOID	SendHeartbeat();
	
	BOOL			CheckWeight(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity);
	BOOL			IsOverWeight();																	// 鼠剪錶憮 務晦 ��萇 鼻鷓檣陛?

	BOOL			EnoughMoney(MONEY siEnoughMoney);												// 絲擊 乾堅 氈朝雖 �挫恉挬�.
	VOID			ChangeMoney(MONEY siChangeMoney, BOOL bSendToDB = TRUE);						// 絲曖 擋熱蒂 夥羞棻.
	MONEY			IncreaseMoney(MONEY siIncreaseMoney, BOOL bSendToDB = TRUE);					// 絲擊 隸陛衛鑑棻.
	BOOL			DecreaseMoney(MONEY uiDecreaseMoney, BOOL bSendToDB = TRUE);					// 絲擊 賓棻.
	MONEY			DecreaseMoneyReturnMoney(MONEY uiDecreaseMoney);					// 絲擊 賓棻.

	BOOL            SaveMoneyAtBank(MONEY siMoney);                                                 // 擎ч縑憮 盪旎и棻.
	BOOL            WithdrawMoneyAtBank(MONEY siMoney);                                             // 擎ч縑憮 檣轎и棻.
	MONEY           ModifySaving(MONEY siMoney);                                                    //  
	MONEY           GetSaving();

	MONEY			GetMoney();																		// 絲擊 橢橫螞棻.
	MONEY			GetMaxMoneyToGet();																// 陛韓 熱 氈朝 譆渠絲擊 橢橫螞棻.

	BOOL			IncreaseBonus(UI08 uiFollowerID, SI32 siBonus);									// 爾傘蝶蒂 隸陛衛麵遽棻.
	BOOL			IncreaseCredit(SI32	siCredit);													// 褐辨紫蒂 隸陛衛麵遽棻.

	UI32			IncreaseTradeCredit( UI32 uiIncreaseCredit, BOOL bSendToDB = TRUE );			// 鼠羲 褐辨紫蒂 螢萼棻.
	VOID			DecreaseTradeCredit( UI32 uiDecreaseCredit, BOOL bSendToDB = TRUE );			// 鼠羲 褐辨紫蒂 馬模衛鑑棻.
	UI32			GetTradeCredit();																// 鼠羲 褐辨紫蒂 橢橫螞棻.
	SI32			GetGiveCredit(MONEY TradeMoney);												// 滲翕腎朝 褐辨紫蒂 掘и棻.
	BOOL			CheckForbiddenName(char *szName);
	SI32			GetTradeGrade();																// 褐辨 蛔晝擊 橢橫螞棻.

	VOID			RecvReadDescript();											// 葆擊 寰頂僥擊 嫡朝棻.
	VOID			RecvWriteDescript();										// 葆擊 寰頂僥擊 噹棻.

	VOID			RecvVillageNation();

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Quest縑 婦溼脹 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID			RecvReqQuestSelectNPC();									// NPC蒂 摹鷗ц擊陽
	VOID			RecvReqQuestAcceptQuest();									// Quest蒂 熱塊ц擊陽
	VOID			RecvReqQuestCancelQuest();									// 霞ч醞檣 Quest蒂 鏃模ц擊陽
	VOID			RecvReqQuestGetPlayQuest();									// 霞ч醞檣 Quest List 蹂羶

	SI16			Quest_FindPlayQuestNPC(SI16 NPCCode);						// 檜 NPC縑 婦溼脹橫 Playж堅 氈朝 Quest蒂 瓊朝棻.
	BOOL			Quest_CheckCompleteQuest(SI16 QuestSlot);					// 檜 Quest陛 諫猿腆熱 氈朝雖 匐餌
	VOID			Quest_EndQuest(SI16 QuestSlot);								// Quest蒂 謙猿и棻.(DB縑 盪濰ж堅, 爾鼻擊 嫡戲貊, Client煎 詭撮雖 瞪歎梱雖 и棻.)
	SI16			Quest_FindNextQuest(SI16 NPCCode);							// 檜 NPC縑啪憮 霞чй熱 氈朝 棻擠 Quest蒂 瓊朝棻.
	BOOL			Quest_CheckPlayQuestAbility(SI16 QuestCode);				// Quest蒂 Startй熱 氈朝雖 匐餌
	BOOL			Quest_CheckSkipQuestAbility(SI16 QuestCode);				// Quest蒂 Skipй熱 氈朝雖 匐餌
	SI16			Quest_FindLastPlayQuest(SI16 NPCCode);						// 檜 NPC縑啪憮 葆雖虞戲煎 Playи Quest匐餌
	BOOL			Quest_FindCompletedQuest(SI16 QuestCode);					// 蕨瞪縑 и瞳檜 氈朝 Quest檣雖 匐餌
	SI16			Quest_FindEmptyQuest(void);									// 綴 Quest Slot擊 瓊擠
	SI16			Quest_FindQuest(SI16 QuestCode);							// 霞ч醞檣 Quest蒂 瓊擠
	VOID			Quest_AllClear(void);										// 賅萇 Quest 蟾晦��, 梃莠ж啪
	VOID			Quest_CheckEventItem(SI16 siQuestSlot);			
	VOID			Quest_SetEventItem(SI16 siEventItemID,SI16 QuestSlot);

	VOID			Quest_AddQuestObject_KillMonster(SI32 MonsterCode, SI32 Count);		// 避檣 跨蝶攪 Data
	VOID			Quest_AddQuestObject_Work(SI32 siWork);								// 濠褐檜 и橾
	VOID			Quest_AddQuestObject_PortalUse(SI32 siNum);
	VOID			Quest_AddQuestObject_Npc(SI32 siNpc);
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	璽堅縑 婦溼脹 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID			RecvStorageInfo();					//	璽堅縑 氈朝 嬴檜蠱擊 爾頂遽棻.
	VOID			RecvStorageInputItem();				//	嬴檜蠱 厥晦
	VOID			RecvStorageOutputItem();			//	嬴檜蠱 疏頂晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	堯濰縑 婦溼脹 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	stFarmData*		GetOwnFactory();					// Ы溯檜橫陛 模嶸ж堅 氈朝 衛撲僭擊 橢橫螞棻.
	VOID			RecvFarmInfo();						//	堯濰 薑爾
	VOID			RecvFarmBuy();						//	堯濰 掘殮
	VOID			RecvFarmSell();						//	堯濰 つ晦
	VOID			RecvFarmSlotInfo();					//	蝸煜 薑爾
	VOID			RecvFarmUpgradeSlot();				//	蝸煜 機斜溯檜萄
	VOID			RecvFarmBuildStruct();				//	勒僭 勒撲
	VOID			RecvFarmExpandSlot();				//	蝸煜 �挫�
	VOID			RecvFarmDestroyStruct();			//	勒僭 繹剪
	VOID			RecvFarmSetEquipment();				//	衛撲僭 撲纂
	VOID			RecvFarmItemProduct();				//	嬴檜蠱 儅骯
	VOID			RecvFarmWorkStart();
	VOID			RecvFarmWork();						//	橾ж晦
	VOID			RecvFarmWorkHack();

	BOOL			CheckLaborHack(UI08 uiType, SI08 siStep, UI32 uiQuantity);		//欽啗縑 蜃朝 橾檣雖 �挫�
	BOOL			CheckLaborHack1(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack2(SI08 siStep, UI32 uiQuantity);
	BOOL			CheckLaborHack3(SI08 siStep, UI32 uiQuantity);
	UI32			SumOfWorkQuantity(SI08& siStep, UI32& uiQuantity);		//葬醴衛粽煎 橾榆 羹觼
	VOID			RecvFarmWorkInfo();					//	奢濰曖 薑爾蒂 蹂羶ц棻.
	VOID			RecvPlantChangPay();
	VOID			RecvPlantCancelPlan();
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰睡縑 婦溼脹 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	BOOL			GetTotalProperty(PROPERTY_INFO &PropertyInfo);			//	濠褐曖 識 濠骯擊 橢朝棻.
	MONEY			CalculateItemValue(MONEY &siItemMoney);					//	濠褐曖 嬴檜蠱曖 陛纂蒂 掘и棻.

	BOOL			GetAuditAccountList();									// 濰睡曖 啗謝跡煙 橢朝棻.
	BOOL			GetAuditOwnList();										// 濰睡曖 睡翕骯模嶸 橢朝棻.
	BOOL			GetAuditVillageInfo();									// 濰睡曖 葆擊薑爾 橢朝棻.
	BOOL			GetAuditStatus();										// 濰睡曖 鼠濰薑爾 橢朝棻.
	VOID			RecvAuditSetName();										//	濰睡縑憮 辨煽 檜葷擊 殮溘и棻.

	VOID			RecvAuditTotalInfo();									// 濰睡曖 識營骯 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditAccountList();									// 濰睡曖 啗謝跡煙 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditOwnList();										// 濰睡曖 睡翕骯模嶸 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditVillageList();									// 濰睡 葆擊 list 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditFriendList();									// 濰睡 耀掘 list 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditFriendSetDenial();								// 濰睡 耀掘蒂 熱褐剪睡/п薯 и棻朝 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditFriendAddOne();								// 濰睡 耀掘蒂 蹺陛и棻.
	VOID			RecvAuditFriendRemoveSome();							// 濰睡縑憮 耀掘蒂 餉薯и棻朝 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditFriendRemoveAll();								// 濰睡縑憮 耀掘 瞪羹蒂 餉薯и棻朝 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditVillageInfo();									// 濰睡曖 и 葆擊薑爾 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditStatus();										// 濰睡曖 鼠濰薑爾 蹂掘蒂 嫡懊棻.
	VOID			RecvAuditFollowerStatus();								// 濰睡曖 и 鼠濰縑 渠и 薑爾 蹂掘蒂 嫡懊棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鱔濰縑 婦溼脹 籀葬
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	
//--------------------------------------------------------------------------------------------
// 葆擊曖 瞪濰縑憮曖 籀葬.
//--------------------------------------------------------------------------------------------
	
	VOID			RecvBankbookList();																// 鱔濰 葬蝶お蒂 殖塭朝 蹂掘蒂 嫡懊棻.
	VOID			RecvBankbookListCount();														// 鱔濰 葬蝶お曖 熱蒂 殖塭朝 蹂掘蒂 嫡懊棻.
	VOID			RecvBankInfo();																	// 擎ч曖 薑爾蒂 橢橫螞棻.
	VOID			RecvBankDeposit();																// 擎ч縑 蕨旎擊 и棻.
	VOID			RecvBankDrawing();																// 擎ч縑憮 轎旎擊 и棻.	
	VOID			RecvOpenSavingAccount();														// 擎ч 啗謝蒂 虜萇棻朝 詭衛雖蒂 嫡懊棻.
	VOID			RecvBankCollateral();
	VOID			RecvBankCollateralList();
	VOID			RecvBankTakeCollateral();
	VOID			RecvMaxCreditLoan();

	VOID            RecvBankSave();
	VOID            RecvBankWithdraw();

	SI32			GetBankBootNum();																				// 鱔濰曖 識 偃熱蒂 橢橫螞棻.
	DWORD			GetInfo(OnCharPlayerInfo *pPlayerInfo);															// 議葛攪曖 薑爾蒂 橢橫螞棻.	
	DWORD			GetDetailInfo(OnCharPlayerDetailInfo *pDetailInfo, UI32 uiRefGuildID, SI32 siRefGuildInClass, DWORD siRefDBAccount );	// 議葛攪曖 鼻撮 薑爾蒂 橢橫螞棻.

	VOID			RecvReqGuildCreateCost();									// 鼻欽 儅撩縑 в蹂и 旎擋擊 蹂羶ж朝 詭衛雖蒂 嫡懊棻.
	VOID			RecvCreateGuild();											// 鼻欽擊 儅撩и棻朝 詭衛雖蒂 嫡懊棻.
	VOID			RecvDeleteGuild();											// 鼻欽擊 餉薯и棻朝 詭衛雖蒂 嫡懊棻
	VOID			RecvJoinLeaveGuild();										// 鼻欽縑 霤罹ж剪釭 集陪棻.
	VOID			RecvReqGuildWaitingJoinMembersList();						// 鼻欽縑 霤罹ж晦蒂 晦棻葬朝 餌辨濠菟曖 葬蝶お蒂 橢橫螞棻.
	VOID			RecvJoinGuildOkNo();										// 鼻欽縑 霤罹蒂 ъ塊 塽 剪睡и棻.
	VOID			RecvGuildDetailInfo();										// 鼻欽曖 渠и 鼻撮 薑爾蒂 橢橫螞棻.	
	VOID			RecvReqGuildMembersList();									// 鼻欽錳 菟曖 葬蝶お蒂 橢橫螞棻.
	VOID			RecvGuildDismissal();										// 鼻欽錳擊 п堅и棻.
	VOID			RecvReqGuildPromotion();									// 鼻欽錳擊 か薑 霜晝戲煎 歜貲и棻.
	VOID			RecvReqGuildDemotion();										// 鼻欽錳擊 か薑 霜晝縑憮 п歜и棻.
	VOID			RecvGuildList();											// 望萄曖 葬蝶お蒂 橢橫螞棻.
	VOID			SetGuild(UI32 uiguildid);									// 鼻欽擊 撲薑и棻.	
	VOID			SetClassInGuild(SI32 siclassinguild);						// 霜晝擊 撲薑и棻.
	SI32			GetClassInGuild();											// 霜晝擊 橢橫螞棻.
	VOID			RecvReqUsedFlagList();										// 餌辨脹 梓嫦曖 葬蝶お蒂 橢橫除棻.
	VOID			RecvReqPIInMinimap();										// 嘐棲裘縑憮 Ы溯檜橫曖 薑爾蒂 蹂掘и棻.



	VOID			ChangeGuildFlag(SI32 siFlag);								// 鼻欽 梓嫦擊 滲唳и棻.
	VOID			ChangeSatiety(SI32 siValue);								// ん虜馬擊 滲唳и棻.

//--------------------------------------------------------------------------------------------
// 裘擊 檜翕й 陽曖 籀葬
//--------------------------------------------------------------------------------------------	
	VOID			RecvMoveMap();												// 裘擊 檜翕ж朝 詭衛雖蒂 嫡懊棻.
	BOOL			ChangeMap(UI16 uimapid, SI16 six, SI16 siy);				// 裘擊 滲唳и棻.
	VOID			SetNearestHospitalVillageIndexInDungeon(SI32 siVillageIndex);	// 湍瞪戲煎 菟橫陛晦瞪縑 避橫憮 湍瞪 殮掘縑憮 薯橾 陛梱遴 避橫憮 葆擊曖 檣策蝶蒂 盪濰и棻.
	
	VOID			BoardShip(SI32 uiBoardShipAccount);							// 寡縑 驕棻.
	VOID			ArriveVillageByShip(SOVillage *pVillage);					// 寡蒂 顫堅 葆擊縑 紫雜ж艘棻.	

//--------------------------------------------------------------------------------------------
// 遴艙濠 婦溼 詭衛雖 籀葬
//--------------------------------------------------------------------------------------------	
	VOID			RecvCommand();												// 遴艙濠 貲滄 詭衛雖蒂 嫡懊棻.


//--------------------------------------------------------------------------------------------
// 爾傘蝶蒂 檜辨ж罹 棟溘纂蒂 螢葬朝 詭衛雖 籀葬
//--------------------------------------------------------------------------------------------	
	VOID			RecvIncAbilityByBonus();									// 爾傘蝶蒂 餌辨ж罹 棟溘纂蒂 螢萼棻朝 詭衛雖蒂 嫡懊棻.

//--------------------------------------------------------------------------------------------
// 瞪癱 謙猿 詭衛雖
//--------------------------------------------------------------------------------------------	
	VOID			PrepareBattle();											// 瞪癱蒂 衛濛и棻.
	VOID			EndBattle();												// 瞪癱蒂 謙猿и棻.

	VOID			AllCharacterDeleteLifePoint(SI32 DeleteLife);				// 賅萇 Character曖 Life蒂 賓棻.


//--------------------------------------------------------------------------------------------
// 評塭棻棲朝 辨煽
//--------------------------------------------------------------------------------------------
	VOID			RecvVMercenaryChange(SI08 siSlot);							// 評塭棻棲朝 辨煽 Change
	VOID			UpdateVMercenary(void);										// 評塭棻棲朝 辨煽 Change


//--------------------------------------------------------------------------------------------
// 喻薄鼻 詭衛雖
//--------------------------------------------------------------------------------------------	
	VOID			RecvPMOpen();												// 喻薄鼻擊 翱棻.
	VOID			RecvPMClose();												// 喻薄鼻擊 殘朝棻.
	VOID			RecvPMBuyItem();											// 喻薄鼻縑啪 嬴檜蠱擊 骯棻.
	VOID			RecvPMReqItemInfo();										// 喻薄鼻檜 陛雖堅 氈朝 嬴檜蠱曖 薑爾蒂 蹂掘и棻.
	VOID			RecvPMChangeItem();											// 喻薄鼻檜 陛雖堅 氈朝 嬴檜蠱擊 滲唳и棻.		

	
	BOOL			CanBuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money);													// 偃檣 鼻薄縑憮 嬴檜蠱擊 髦 熱 氈朝陛?		
	BOOL			BuyPMItem(UI08 uiBoothSlot, UI16 uiItemQuantity, MONEY Money, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *pMercenarySlot);		// 偃檣 鼻薄縑憮 嬴檜蠱擊 骯棻.

	
	VOID			RecvAskTrade();															// Trade蒂 ж濠堅 蹂羶и棻. 
	VOID			RecvAcceptTrade();														// Trade蒂 ж濠堅 蹂羶и匙縑 渠и 鼻渠寞曖 擬港擊 嫡懊棻.
	VOID			RecvFinishChooseTradeItem();											// Trade衛 嬴檜蠱 摹鷗擊 賅舒 部麻棻.
	VOID			RecvDecideTrade();														// Trade衛 掖羲擊 и棻朝 匙擊 唸薑и棻.
	VOID			RecvChangeTradeGoods();													// Trade衛 掖羲醞檣 嬴檜蠱擊 熱薑и棻.
	VOID			RecvCancelTrade();														// Trade衛 ⑷營 霞ч醞檣 Trade蒂 鏃模и棻.
	VOID			RecvCompleteTradeBook();												// Trade衛 濰睡 剪楚陛 諫猿腎歷棻.
	VOID            RecvRequestPersonalInfo();
	BOOL			OnUseDonkeyItem( UI08 uiFollowerID, UI16 uiItemID );					// 渡釭敝 嬴檜蠱 餌辨衛曖 籀葬

	VOID			RecvRunProgramList();
	
	BOOL			CanTrade();																// ⑷營 掖羲檜 陛棟и陛?
	BOOL			IsTrading();															// ⑷營 棻艇 Ы溯檜橫諦 掖羲醞檣雖 憲嬴螞棻.	
	BOOL			IsAcceptedTrading(UI16 uiAccount = 0);									// ⑷營 棻艇 Ы溯檜橫陛 掖羲擊 熱塊п憮 掖羲擊 ж堅 氈朝雖 憲嬴螞棻.
	BOOL			AcceptedTrade( UI32 uiOppGuildID, SI32 siOppClassInGuild, SI32 siOppLevel );												// 掖羲擊 熱辨ц棻.
	BOOL			NotAcceptedTrade();														// 掖羲擊 熱辨ж雖 彊懊棻.
	BOOL			CancelTrade();															// ⑷營 霞ч醞檣 掖羲擊 鏃模и棻.
	BOOL			IsWaitingAcceptTrade(UI16 uiAccount = 0);								// 鼻渠寞檜 掖羲擊 蝓釩ж朝 匙擊 渠晦ж朝雖 憲嬴螞棻.
	BOOL			ChooseTradeItem();														// 掖羲й 嬴檜蠱擊 摹鷗ц棻.
	BOOL			CancelChooseTradeItem();												// 掖羲й 嬴檜蠱擊 摹鷗ц湍 匙擊 鏃模и棻.
	BOOL			IsChoseTradeItem(UI16 uiAccount = 0);									// 掖羲й 嬴檜蠱擊 摹鷗ц朝雖 憲嬴螞棻.
	BOOL			DecideTrade();															// 摹鷗ц湍 嬴檜蠱菟擊 陛雖堅 掖羲й 匙擊 唸薑и棻.
	BOOL			CancelDecideTrading();													// 掖羲ж溥堅 ц湍 匙擊 鏃模и棻.	
	BOOL			IsDecideTrading(UI16 uiAccount = 0);									// 掖羲擊 ж溥堅 唸薑擊 ц朝雖 憲嬴螞棻.	
	BOOL			AddTradeGoods(UI08 uiSlotID, TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2,
								OnFollowerInfo *pTradeFollowerInfo, DWORD *pdwFollowerDataSize, OnIndustrialComplexTradeInfo *pIndustrialComplexTradeInfo );	// 掖羲 僭ヶ擊 蹺陛и棻.
	BOOL			CanReceiveGoods(TradeGoodsKind Kind, LPARAM lParam1, LPARAM lParam2);	// 鼻渠寞檜 輿溥堅 ж朝 僭ヶ菟擊 賅舒 嫡擊 熱 氈朝雖 羹觼и棻.
	VOID			TradeGoods( SOPlayer *pTargetPlayer, DWORD *pdwFollowerDataSize, OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum, 
							  DWORD *pdwTradeItemDataSize, UI16 puiTradeItem[ ON_MYDATA_MAX_ITEM_NUM * 2 ], UI16 *puiTradeItemNum, CHAR *pszDBTradeArg );		// 鼻渠寞婁 掖羲擊 и棻.
	DWORD			TradeFollower(SOPlayer *pTradePlayer, UI16 uiFollowerID, OnFollowerInfo *pFollowerInfo, UI08 *puiOppFollowerID);	// pTradePlayer縑啪 uiFollowerID曖 辨煽擊 遽棻.
	VOID			CloseTrade();															// 掖羲璽擊 殘朝棻.
	BOOL			IsValidTradeGoods();													// 頂陛 掖羲ж溥朝 ヶ跡檜 嬴霜 嶸�褲捘� 匐餌и棻
	BOOL			CanAddFollower( UI16 uiFollowerNum );									// 頂陛 辨煽擊 蹺陛й 熱 氈朝雖 憲嬴螞棻.

	BOOL			HaveGeneral();															// 濰熱蒂 陛雖堅 氈朝雖 憲嬴螞棻.
	BOOL			IsGeneral(UI08 uiFollowerSlot);											// 濰熱檣雖 憲嬴螞棻.
	BOOL			IsFieldAttackUnit();

	DWORD			GetIPInNumeric();														// Ы溯檜橫曖 璋濠煎 檜瑞橫霞 IP蒂 嫡嬴螞棻.

	SI16			GetMaxFollowerNum();													// Ы溯檜橫陛 陛韓 熱 氈朝 譆渠 辨煽曖 熱.(輿檣奢 んл)
	BOOL			IsMonster( UI08 uiFollowerSlot );										// か薑 蝸煜檜 跨蝶攪檣雖 匐餌и棻.
	SI32			GetMonsterNum();														// ⑷營 爾嶸ж堅 氈朝 跨蝶攪熱蒂 橢橫螞棻.
	BOOL			IsAdminPlayer();														// Ы溯檜橫陛 遴艙濠檣雖 憲嬴螞棻.

	bool			expUp(int followerIndex, int value);									// 唳я纂蒂 隸陛 衛鑑棻 

	VOID			RecvRequestVillageInfo();												// か薑 葆擊曖 薑爾蒂 蹂羶и棻.

	BOOL			GiveMoney( MONEY *pMoney, SOPlayer *pOppPlayer );						// 鼻渠寞縑啪 絲擊 遽棻.

	BOOL			IsInVillage();															// ⑷營 葆擊 寰縑 氈朝雖 憲嬴螞棻.
	BOOL			IsEnteredStructure();													// ⑷營 葆擊曖 勒僭 寰縑 菟橫鬲朝雖 匐餌и棻.

	BOOL			MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory );	// 嬴檜蠱擊 檜翕 衛鑑棻.


	// 嬴檜蠱 掘殮 婦溼 

	void			SendItemCount();

	// 渠濰濰檜 婦溼

	VOID            RecvRefineItem();

	// 紫瞪 108啗欽賅萄.

	SI32            GetChallengeLevel();                        // 檜 Ы溯檜橫陛 賃廓簞 溯漣梱雖 鬲朝雖 憲嬴螞棻.
	VOID            SetChallengeLevel(SI32 siChallengeLevel);   // 溯漣擊 撲薑и棻.

	// 2003.10.7 蹺陛脾, 薑撩遴

	BOOL			ProcessPartyMessage(VOID);

	BOOL			CanJoinParty();
	BOOL			CreateParty();	
	BOOL			LeaveParty();
	UI16			GetPartyId();		
	BOOL			JoinParty( UI16 uiPartyID );					// だじ縑 菟橫除棻.
	VOID			PartyDeleted();									// だじ陛 餉薯腎歷棻.

	BOOL			PartyChat( CHAR *pszChatMsg );					// だじ錳菟縑啪 瓣た 詭衛雖蒂 爾魚棻.

	VOID			CloseRequestingJoinParty();									// 釭縑啪 だじ陛 菟橫螞剪縑 渠п憮釭 頂陛 だじ 褐羶и剪縑 渠п憮 剪睡 塽 鏃模и棻.
	BOOL			AcceptRequestingJoinParty();								// 釭縑啪 菟橫螞 だじ 褐羶擊 ъ塊ц棻.
	BOOL			RejectRequestingJoinParty();								// 釭縑啪 菟橫螞 だじ 褐羶擊 剪睡и棻.
	BOOL			CancelRequestingParty( BOOL bProcessByRequest = FALSE);		// だじ 褐羶 ж艘湍匙擊 鏃模и棻.

	BOOL			IsPartyLeader();
	BOOL			IsRequestingJoinParty( UI16 uiAccount = 0 );	
	BOOL			IsPartyMember();
	BOOL			IsReceivedRequestJoinParty();
	
	BOOL			CanRequestJoinParty();							// 頂陛 鼻渠寞縑啪 だじ蒂 蹂羶й 熱 氈朝雖 憲嬴螞棻.
	BOOL			CanReceiveRequestJoinParty();					// ⑷營 だじ 蹂羶擊 嫡擊 熱 氈朝雖 憲嬴螞棻.
	VOID			RequestJoinParty( UI16 uiAccount );				// п渡 嶸盪縑啪 だじ縑 陛殮 蹂羶й 匙擊 蹂羶ц棻.
	VOID			ReceiveRequestJoinParty( UI16 uiAccount );		// п渡 嶸盪縑啪 だじ縑 陛殮 蹂羶й 匙擊 嫡懊棻.
	VOID			SetPartyLeader();								// だじ濰檜 腎歷棻.	

	// 2003.10.7 蹺陛脾, 薑撩遴
	// party message 籀葬 み暮 

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
	VOID			DeleteTimerItem();											// 衛除薯 嬴檜蠱菟 餉薯и棻.

	SI16			GetMDay(SI16 siMon);
	SI16			GetComPareDate(SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16 siEndYear,SI16 siEndMon,SI16 siEndDay);
	VOID			GetEndDate(SI16 siItemTime, SI16 siNowYear, SI16 siNowMon, SI16 siNowDay,SI16& siEndYear,SI16& siEndMon,SI16& siEndDay);

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢撩 婦溼 議葛攪縑 婦и 詭撮雖 籀葬 л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
	VOID			RecvFieldAttackReady();										// Ы溯檜橫 奢問渠晦
	VOID			RecvFieldAttackAction();									// Ы溯檜橫 識憲 蜃朝棻.			
	VOID			RecvFieldAttackDead();										// Ы溯檜橫陛 避朝棻.

	VOID			RecvLongDistanceAttackVillageReady();						// Ы溯檜橫陛 葆擊擊 陽萼棻.渠晦(錳剪葬)
	VOID			RecvLongDistanceAttackVillageAction();						// Ы溯檜橫陛 葆擊擊 陽萼棻.奢問(錳剪葬)

	VOID			RecvAttackVillage();										// 葆擊擊 奢問и棻朝 詭衛雖蒂 嫡懊棻.(斬剪葬)

	VOID			RecvReqWarVillageDefence();									// в萄 -> 熱撩 檣攪む檜蝶
	VOID			RecvReqWarVillageIn();										// 熱撩 檣攪む檜蝶 -> 葆擊

	VOID			RecvPlayerVillageDefenceUp();								// 熱撩 葆擊曖 寞橫溘擊 螢萼棻.
	VOID            RecvReqWarVillageDefenceStop();                             // 熱撩 檣攪む檜蝶蒂 謙猿и棻.

	VOID			RecvIncreaseGuildUnit();									// 鼻欽曖 奢撩嶸棉擊 隸陛.(儅骯)
	VOID			RecvDecreaseGuildUnit();									// 鼻欽曖 奢撩嶸棉擊 馬模.(離轎)	
	VOID			RecvGetGuildUnitList();										// 鼻欽曖 奢撩嶸棉曖 葬蝶お蒂 嫡嬴螞棻.

	VOID            RecvHeal();                                                 // 爾晝熱溯陛 嶸棉擊 ��葭п遽渡.
	VOID            RecvChargeMP();                                             // 爾晝熱溯曖 葆釭陛 �蛹僱Ц�棻.

	VOID			RecvGuildSetRelation();										// 望萄除曖 婦啪蒂 撲薑и棻朝 詭衛雖蒂 嫡懊棻.

	VOID			RecvFriendlyMessageOffer();									// �俸�薯曖 詭撮雖蒂 爾魚棻.
	VOID			RecvFriendlyMessageAccept();								// �俸�薯曖 詭撮雖蒂 嫡嬴菟檣棻.
	VOID			RecvFreidnlyMessageCancel();								// �俸�薯曖 詭撮雖蒂 壅朝棻.

	VOID			RecvVillageAttackResult();									// 奢撩縑 渠и 唸婁.
	VOID			RecvChoicePlayer();											// 葆擊葬蝶お縑憮 Ы溯檜橫蒂 摹鷗и棻.
	VOID			RecvInvestmentPower();

	VOID			RecvWarWarp();												// 奢撩瞪醞 錶Щ蒂 и棻.
	VOID			RecvNormalChar();											// 濠褐曖 奢撩 議葛擊 橾奩 議葛戲煎 虜萇棻.

	VOID            RecvChangeUnit();                                           // 奢撩嶸棉戲煎 滲褐蹂羶
	VOID            RecvGiveUp();                                               // 瞪癱ん晦蒂 ж艘棻. 

	

	
//	VOID			RecvVillageAttackTypeCheck();								// 葆擊 奢問顫殮擊 夥羚熱 氈朝雖 羹觼и棻.
//	VOID			RecvVillagePlayerOilAttack();								// 熱撩 葆擊曖 嶸棉 奢問寞徹(晦葷)
//	VOID			RecvVillagePlayerStoneAttack();								// 熱撩 葆擊曖 嶸棉 奢問寞徹(給)

//	VOID			RecvIncreaseGuildSupply();									// 渠寞檜釭 渠ч熱陛 葆擊 爾晝ヶ曖 熱蒂 隸陛衛鑑棻.
//	VOID			RecvDecreaseGuildSupply();									// 爾晝熱溯陛 爾晝ヶ擊 陛螳除棻.
//	VOID			RecvGetGuildSupplyList();
//	VOID			RecvGetSupplyAtGuildUnit();									// 爾晝熱溯縑啪 爾晝ヶ擊 瓣遴棻.

//	VOID			RecvIncreaseVillageDefenceSoldier();						// 熱綠煽曖 熱蒂 隸陛 衛鑑棻.
//	VOID			RecvGetVillageDefenceSoldier();								// 熱綠煽曖 熱蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 奢撩 婦溼 議葛攪縑 婦и л熱 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//	SI32			GetFieldSupplyCharge();							   //爾晝熱溯陛 陛雖堅 氈朝 爾晝翼擊 橢橫螞棻.
//	void			SetFieldSupplyCharge(SI32 siFieldSupplyCharge);	   //爾晝熱溯縑啪 爾晝ヶ擊 厥朝棻.(葆擊->爾晝熱溯)
//	void			DecreaseSupplyCharge(SI32 siDecreaseSupplyCharge); //爾晝熱溯曖 爾晝ヶ擊 陛螳除棻.
//	void			InitFieldSupplyCharge(SI32 siFieldSuppyCharge);

	void		    SetPrevKind(UI16 uiPrevKind);
	UI16			GetPrevKind();

	void		    SetCaptainUnit(BOOL	bCaptain);
	BOOL			GetCaptainUnit();
	
	BOOL			IsFieldAliveWarUnit();

	VOID			SendGuildItemDeleteMessage();
	VOID			DeleteFiledUnit();											// 10衛縑 奢撩嶸棉擊 餉薯и棻.



	BOOL            FieldAttackDead();
	UI16            ChangeUnit(UI16 uiCode, UI16 uiQuantity);//奢撩嶸棉戲煎 滲褐ж棻...
	VOID            ChangeNormalChar();
	
	UI16            GetDir(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY);
	UI16            GetSpTargetIDOFSpear(UI16 uiDir, SI32 siSX, SI32 siSY, UI16 *puiIDs); //璽煽檣唳辦 蝶Ы溯蓮 等嘐雖蒂 殮朝 議葛攪 嬴檜蛤蒂 掘и棻
    SI32            GetSpTargetIDOFSiege(SI32 siSX, SI32 siSY, SI32 siEX, SI32 siEY, UI16 *puiIDs); //嫦戮剪檣唳辦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

// write л熱縑憮 雖羲 滲熱煎 噙歜..
private:
	UI16    usPacketSize;

	UI16	usSize;
	UI32	uiSize;
	
	char	cCheckSum;

	UI32	uiCmd;

//  2003.20.07 だじ 衛蝶蠱 蹺陛�� - 薑撩遴 
	
	party_state_t	m_PartyState;

};	

#endif	