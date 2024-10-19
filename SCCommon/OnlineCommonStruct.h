#ifndef	_ONLINE_COMMONSTRUCT_H
#define _ONLINE_COMMONSTRUCT_H

#include "GSCDefine.h"
#include "Defines.h"
#include <parametabox.h>
#include <OnlineMyWeapons.h>
#include <OnlineImjin2I.h>


enum { LIST_COLOR_NOT, LIST_COLOR_RED, LIST_COLOR_PINK };
enum { LIST_NOT = -1, LIST_ITEM, LIST_FOLLOWER, LIST_PROPERTY , LIST_TRADEBOOK};
enum { ITEM_UPGRADE_BROKEN, ITEM_UPGRADE_FAIL, ITEM_UPGRADE_SUCCESS, ITEM_UPGRADE_IMPOSSIBLE }; 
enum { MAGIC_STONE_BLUE, MAGIC_STONE_RED, MAGIC_STONE_BLACK };
enum { RUN_PROG_LIST_SPEEDHACK, RUN_PROG_LIST_LABORHACK };


// 서버와 클라이언트가 공통적으로 사용하는 구조체.
struct REPORT_INITBATTLE
{
	DWORD						command;
	DWORD						subcommand;		
	
	WORD						randomSeed;				
	DWORD						numPlayer;		
	MONEY						userMoney;				
	BOOL						isSingleMode;
	
	OnImjinBattlePlayerInfo		OIBPI[8];
};


// 한 캐릭터가 인벤토리내에 가지고 있는 아이템.

struct	ITEMDATA
{
	SI08	Slot;
	SI32	Qnt;
	SI16	ItemID;
};

struct InventoryInput
{
	SI16	siPosInInventory;
	UI32	uiQunatity;
};

typedef struct
{
	UI16		uiItemCode;												// 아이템코드
	UI16		uiQuantity;												// 수량
} STORAGE_ITEM;

struct	MyItemData
{
	SI16	siPosInInventory;					// 인벤토리내에서의 좌표.
	UI16	uiID;								// ID
	UI16	uiQuantity;							// 수량
};

struct TradeFollower							//	무역창에 올려 놓을때.
{
	UI16	Code;
	SI16	siTradePos;							//  트레이드 창에서의 좌표. 개수는 필요 없다.
};

struct TradeItem								//	무역창에 올려 놓을때.
{

	UI16	uiFollower;
	UI16	uiItemCode;
	SI16	siTradePos;							// 트레이드 창에서의 좌표.
	SI32	siQuantity;							//	개수
};

// 캐릭터의 기본 정보.
struct	OnCharBaseInfo
{
	UI16	uiAccount;							// 캐릭터의 Account
	SI16	siX;								// siX
	SI16	siY;								// siY
	UI16	uiKind;								// 캐릭터의 종류.
};

struct	OnCharMonsterInfo : public OnCharBaseInfo	// 몬스터의 정보
{	
	SI32	siFightingPower;						// 공격력.
};

struct	OnCharPlayerInfo : public OnCharBaseInfo	// 플레이어의 정보.
{		
	SI32	siFightingPower;						// 공격력.

	SI32	siDBAccount;	
	UI16	uiEquipWeapon[WEAR_ITEM_NUM];	  	    // 0:투구,1:무기,2:갑옷,3:수호부		
	SI08	siPrivateMarketItemNum;					// 개인 상점 모드일 경우, 아이템의 개수
	SI32	siGuildInGrade;							// 길드내의 직급.
	SI08	siIsOverWeight;							// 무게 초과인가?
	SI32	siVMercenaryKind;						// 따라다니는 용병 Kind
	BOOL	bAttackMode;							// 전투모드인가?

	SI16	siFieldHp;								// 공성전 유닛의 체력.
	BOOL	bCaptain;								// 대장인가 아닌가.
	SI16	siSupplyCharge;
	BOOL	bVillageInLimit;
};

struct	OnCharShipInfo : public OnCharBaseInfo		// 배의 정보.
{
};

struct	OnCharPlayerDetailInfo
{
	CHAR	szID[ON_ID_LENGTH];						// ID
	CHAR	szGuild[ON_GUILDNAME_LENGTH];			// Guild ID
	SI32	siGuildFlag;							// 상단 깃발.
	UI08	uiIsHostilityGuild;						// 나와 적대적인 길드인가?	
	SI32	siEventScore;							// 이벤트 점수.
	SI32	siEventRanking;							// 이벤트 랭킹.
};


/*
// 각각 캐릭터의 정보.
struct	OnlineReceivedPlayerInfo
{	
	UI16	uiPlayer;
	SI16	siX;
	SI16	siY;
	UI16	uiKind;	
	SI32	siFightingPower;						// 공격력	
};

// 사용자 캐릭터일 경우
struct	OnlineReceivedHeroInfo
{
	UI16	uiPlayer;
	SI16	siX;
	SI16	siY;
	UI16	uiKind;		
	SI32	siFightingPower;				// 공격력	
	
	SI32	siDBAccount;
	SI32	siGuildInGrade;
	UI16	uiEquipWeapon[3];				// 0:투구,1:무기,2:갑옷
	CHAR	szID[ON_ID_LENGTH];
	CHAR	szGuild[ON_GUILDNAME_LENGTH];	
	SI32	siGuildFlag;					// 상단 깃발.
	
	UI08	uiIsHostilityGuild;				// 나와 적대적인 길드인가?
	SI08	siPrivateMarketItemNum;			// 개인 상점 모드일 경우, 아이템의 개수
};
*/

// 필드에 떨어진 아이템의 정보.
struct	OnlineReceiveFieldItemInfo
{
	UI16	uiItemID;
	UI16	uiQuantity;
	SI16	siX;
	SI16	siY;
}; 
/*
// 한 플레이어당 3명의 캐릭터를 키울 수 있는 각 캐릭터의 구조체.(로그인시 로그인 서버로 부터 받는다.)
struct	OnlineMyCharacterInfo
{
	UI08	uiSlot;
	CHAR	szID[ON_ID_LENGTH];
	SI16	siX;
	SI16	siY;
	UI16	uiKind;	
	SI16	MapIndex;
	SI16	Helmet;
	SI16	Weapon;
	SI16	Armor;
};

// 로그인 서버 접속시에 로그인 서버로 부터 받는 메시지.
struct	OnlineLogInServerLogIn
{		
	UI08					GameServerIP[4];
	CHAR					szPlayerGuid[36];
	UI08					uiPrevSelectCharacterSlot;
	UI08					uiCharacterNum;	
	OnlineMyCharacterInfo	*pCharacterInfos;
};
*/
typedef	MyItemData		OnlineReceivedMyItemInfo;

/*
// 게임 서버 접속시에 게임 서버로 부터 받는 메시지.
struct	OnlineReceivedMyData
{	
	MONEY						uiMoney;

	UI16						uiPlayerNum;
	UI16						uiItemNum;
	UI16						uiFieldItemNum;

	OnlineReceivedMyItemInfo	*pItemInfo;				// 내 아이템이 정보.
	OnCharBaseInfo				*pCharBaseInfo;			// 내 주위의 캐릭터의 정보.
	OnlineReceiveFieldItemInfo	*pFieldItemInfo;		// 필드에 떨어진 아이템의 정보.
};
*/

// 좌표를 보내거나 받을때 사용하는 메시지.
struct	OnlinePosition
{
	SI32	siX;
	SI32	siY;
};

// 아이템을 보내거나 받을때 사용하는 구조체.
struct	OnlineReceivedItemInfo
{
	UI16	uiID;	
	UI16	uiQuantity;
	MONEY	BasePrice;						// 아이템의 기준 가격
};

// 전투를 하는 캐릭터들의 대한 정보.
struct	OnBattlePlayerInfo
{	
	CHAR	szPlayerName[ON_ID_LENGTH];	// 플레이어의 이름.
	UI16	uiPlayersUniqueID;			// 플레이어의 UniqueID	
	UI16	uiSlot;						// Slot번호.
	UI08	uiCountry;					// 국가.	
	UI16	uiKind[10];					// Kind.		
};

// 마을, 주점, 대화방에서 채팅을 하는 캐릭터들의 정보.
struct	OnVilINNChatPlayerInfo
{
	CHAR	szName[ON_ID_LENGTH];		// 이름.
	SI32	siDBAccount;				// DBAccont
	UI16	uiPlayerUniqueID;			// Player의 UniqueID
};

struct OnWearItemInfo
{
	UI16	uiItemID;
	SI16	siLimitTime;
};

struct	OnFollowerInfo
{
	UI08	uiSlot;						// Slot
	UI16	uiKind;
	UI16	uiItemNum;

	// 캐릭터의 능력치.	
	SI32	siExp;						// 경험치.
	SI32	siLevel;					//	레벨
	UI16	siStr;						// 체력.
	UI16	siDex;						// Dex
	UI16	siVit;						// Vit
	UI16	siInt;						// Int
	
	SI32	siLife;						// Life
	SI32	siMana;						// Mana
	SI16	siBonus;					// Bonus

	SI32	siLiftConstant;				// LiftConstant

	SI16	siEquipMinDamage;			// Equip Min Damage
	SI16	siEquipMaxDamage;			// Equip Max Damage

	SI16	siAC;						// AC
	SI16	siDamageRegist;				// DamageRegist
	SI16	siMagicRegist;				// MagicRegist	

	//	장착한 아이템 
	//UI16	uiWearIte[WEAR_ITEM_NUM];
	OnWearItemInfo	stWearItemInfo[WEAR_ITEM_NUM];


	CHAR	Name[ON_ID_LENGTH];			//	용병 이름
	
	// 뒤에 것은 나중에 넣도록 하자.
	//UI08	uiBlockingRate;				// BlockingRate

	// 뒤에는 아이템(MyItemData)이 uiItemNum 수 만큼 있다.
};

struct OnTimerItemInfo
{
	SI16	siMercenarySlot;
	SI16	siLoc;
	SI16	siItemID;
	SI16	siRestTime;
	BOOL	bDeleteTimerItem;
};


struct	OnChangeJobFollowerInfo
{
	UI08	uiSlot;						// Slot
	UI16	uiKind;

	// 캐릭터의 능력치.	
	SI32	siExp;						// 경험치.
	SI32	siLevel;					//	레벨
	UI16	siStr;						// 체력.
	UI16	siDex;						// Dex
	UI16	siVit;						// Vit
	UI16	siInt;						// Int
	
	SI32	siLife;						// Life
	SI32	siMana;						// Mana
	SI16	siBonus;					// Bonus

	SI32	siLiftConstant;				// LiftConstant

	SI16	siEquipMinDamage;			// Equip Min Damage
	SI16	siEquipMaxDamage;			// Equip Max Damage

	SI16	siAC;						// AC
	SI16	siDamageRegist;				// DamageRegist
	SI16	siMagicRegist;				// MagicRegist	

	CHAR	Name[ON_ID_LENGTH];			//	용병 이름
};

struct	OnBuyItemPosInfo
{	
	UI16	uiPosInInventory;			// 아이템의 위치.
	UI16	uiQuantity;					// 아이템이 수량.
};

// OnSellItemPosInfo는 OnBuyItemPosInfo와 같다.
typedef	struct	OnBuyItemPosInfo	OnSellItemPosInfo;

struct SOnlineTime
{
	int			Year;								// 년도
	int			Month;								// 월
	int			Day;								// 일
	int			Hour;								// 시
};

struct	OnRealWorldTime
{
	UI32	uiYear		: 12;						// 년도
	UI32	uiMonth		: 4;						// 월
	UI32	uiDay		: 5;						// 일
	UI32	uiHour		: 5;						// 시
	UI32	uiMinute	: 6;						// 분
};

// 마을의 정보.
struct	OnRecvVillageInfo
{
	UI32		uiPopulation;
	CHAR		szGuildID[ON_GUILDNAME_LENGTH + 1];

	CHAR		m_MaxInvestmentID[21];			// 최고 투자자 ID
	VDEF		vdVillageDefence;				// 마을의 방어력.	
	VDEF		vdVillageMaxDefence;			// 마을의 최대 방어력.
	
	DWORD		dwGuildRelation;				// 길드의 관계.

	CHAR		cNationCode;					// 국가 코드.
	SI32		siGuildFlag;					// 깃발.

	SI32		siDefenceSoldier;
	SI32		siMaxDefenceSoldier;
	SI16		siAttackType;
};


// 계좌번호 ((3)uiVillageCode - (4)uiAccountKind - (7)(uiUserDBAccount(4) - uiUserDBAccount(3)) - (1)uiCharSlot - (1)uiAccountIndex)
struct	OnAccountNumber
{
	UI32		uiVillageCode : 10;
	UI32		uiAccountKind : 14;
	UI32		uiUserDBAccount : 20;
	UI32		uiCharSlot		: 4;
	UI32		uiAccountIndex  : 4;
};

struct	OnIndustrialComplexInfo
{	
	SI32		siID;								// 산업 단지의 ID
	CHAR		szName[ ON_ID_LENGTH + 1 ];			// 산업 단지의 주인.
	UI32		m_uiTotalAmountOfWork;				// 산업 단지의 전체 작업량.
	MONEY       m_moMaxPay;                         // 산업 단지의 최대 임금 
};

/*
struct	OnFarmList
{	
	UI08			uiAvailSlot;					//생산시설 슬롯수
//	CHAR			szFarmName[ON_ID_LENGTH+1];		//농장 이름 
	CHAR			szOwner[ON_ID_LENGTH+1];		//농장주 이름.
	UI32			uiRemainLabor;					//남은 일

	VOID Init()
	{
		uiAvailSlot				=	ON_FARM_SLOT_INIT;
//		ZeroMemory( szFarmName, sizeof(szFarmName) );
		ZeroMemory( szOwner, sizeof(szOwner) );
		uiRemainLabor			=	0;
	}
};
*/
struct	OnFarmSlotList
{
	UI08			SlotIndex;						// 몇 번째 슬롯인가?
	UI08			uiLevel;						// 현 공장의 레벨.	
	CHAR			StructCode[4];					// 건물 코드	
	SI16			siPay;							//	임금
	UI32			uiLabor;						// 노동량.
};

// 주점의 대화방의 대한 정보.
struct	OnInnChatRoomInfo
{
	UI16		uiChatRoomID;
	UI08		uiJoinedPlayerNum;
	UI08		uiChatRoomNameStringLength;	
};

// 게시판의 게시물의 대한 정보.
struct	OnInnNBNoticeInfo
{
	UI16		uiIndex;								// 글 번호.
	UI08		uiWroteIDStringLength;					// 글쓴이의 이름 길이
	UI08		uiTitleStringLength;					// 그 제목의 문자열 길이
	CHAR		szDate[6];								// 날짜.
	DWORD		dwReadCount;							// 읽은 횟수
};

// 짐꾼 정보
struct	OnInnBuyWorkerInfo
{
	SI32		siCode;									// 짐꾼 ID
	SI32		siLevel;								// 짐꾼 고용 Level
	SI16		siKind;									// 짐꾼 Kind
	UI32		uiCharNameAddOption;					// 짐꾼 Name Add Option
	MONEY		siMoney;								// 짐꾼 가격
	SI32		siLiftPower;							// 들수 있는 무게
};

// 용병 정보
struct	OnInnBuySolderInfo
{
	SI32			siCode;								// 용병 ID
	SI32			siLevel;							// 구입시 필요한 Level
	SI32			siCredit;							// 구입시 필요한 신용도
	UI32			uiCharNameAddOption;				// 용병 Name Add Option
	MONEY			siMoney;							// 용병 가격

	OnFollowerInfo	SolderInfo;							// 용병 능력
};

// 용병 정보
struct	OnDismissalSolderInfo
{
	SI32			siSlot;								// Slot

	MONEY			siMoney;							// 해고시 얻는 돈
};


// 몬스터 용병 정보
struct	OnInnBuyMonsterInfo
{
	SI32			siCode;								// 몬스터 용병 ID
	SI32			siLevel;							// 구입시 필요한 Level
	SI32			siCredit;							// 구입시 필요한 신용도
	UI32			uiCharNameAddOption;				// 용병 Name Add Option
	MONEY			siMoney;							// 용병 가격
	SI32			siSealingBeadNum;					// 필요한 봉인의 구슬 개수

	OnFollowerInfo	SolderInfo;							// 용병 능력
};

struct OnChangeJobSolderInfo
{
	SI32			siSlot;								// Slot

	SI32			siJobList[4];						// 바꿀수 있는 Job(0이면 없는것이다.)
	UI08			siJobListFlag[4];					// 바꿀수 있는지 Flag
	SI32			siJobListChangeLevel[4];			// 바꿀때 필요한 Level
	SI32			siBonus;							// 전직시 보너스로 얻을수 있는 수치
};

// Ticket 정보
struct	OnTicketInfo
{
	SI32			m_siTicketKind;						// Ticket Kind
};

// 투자 정보
typedef struct
{
	UI08			m_InvestmentType;					// 투자자의 종류(0 : Player, 1 : 길드, 2 : 국가)
	UI08			m_InvestmentName[21];				// 투자자의 이름

	float			m_InvestmentPercent;				// 투자 비율
	MONEY			m_InvestmentMoney;					// 투자 금액
	int				m_InvestmentUpdateTime;				// 투자 시간
} OnInvestmentInfo;


// 파티원 정보.
struct OnPartyInfo
{
	BOOL			bIsPartyLeader;						// 파티장인가?
	SI16			siLevel;							// 캐릭터의 레벨
	CHAR			Name[ ON_ID_LENGTH + 1 ];			// 캐릭터의 이름

};


// 장부 부동산
struct	OnAuditOwnInfo	
{
	UI08			PlantID;							//	부동산 종류 0:농장 ....
	UI16			uiVillageCode;						//	부동산이 있는 마을
};



struct	strGuildInfo									// 상단의 정보
{
	CHAR			szGuildID[ON_GUILDNAME_LENGTH];		// 상단의 이름.
	CHAR			szDAIID[ON_ID_LENGTH];				// 대방의 이름.
	UI16			uiOwnVillageNum;					// 소유하고 있는 마을의 수.
	UI32			uiGuildMemberNum;					// 길드 멤버 수.
	SI32			siRelation;							// 상대방 상단의 대한 우리 상단의 관계
	SI32			siRelationOfOpponent;				// 내 상단의 대한 상대방 상단의 관계
	UI32			uiMaxGuildMembersNum;				// 최대 가입할 수 있는 상단원의 수
	SI32			siFlag;								// 상단의 깃발.
	CHAR			szGuildFriendlyMessage[81];
};


struct	strHungryFollowerInfo
{
	UI08			uiFollowerID;
	SI32			siLife;
};

struct  strCureSolder
{
	UI08						dwCureSolderSlot;			// Follower Slot
	SI32						siLife;						// Life
	SI32						siMana;						// Mana
};

struct	strFlagInfo
{
	UI32				uiGuildID;						// 상단 ID
	SI32				siFlag;							// 사용하는 깃발.
};

struct stCapital
{
	SI16		Year;
	union
	{
		MONEY	Profit[ON_MAX_PROFIT_SORT];						// 배열로 다룰때.
		struct
		{
			MONEY	TradeProfit;						// 무역수익(시전)
			MONEY	BankProfit;							// 전장수익(은행)
			MONEY	EmployProfit;						// 용병수익(훈련소)
			MONEY	CureProfit;							// 치료수익(의원)
			MONEY	TicketProfit;						// 티켓수익(항구)
			MONEY	InnProfit;							// 여관수익(주막)
			MONEY	PlantProfit;						// 생산건물수익(농장,공장,목장)
		};
	};
};


struct  strGuildCommonMemberInfo
{
	SI32    siAccount;
	UI08    uiSlot;

	SI32	siClassInGuild;							// 직급

};

struct	strCandidateInfo
{
	// 공통적인 정보.
	CHAR	szName[ON_ID_LENGTH + 1];				// 이름	
	MONEY	mnPropery;								// 재산
	SI32	siLevel;								// 레벨	
	UI32	uiTradeCredit;							// 무역 신용도
	SI32    siAccount;
	UI08    uiSlot;

};

struct	strGuildMemberInfo
{

	// 공통적인 정보.
	CHAR	szName[ON_ID_LENGTH + 1];				// 이름	
	MONEY	mnPropery;								// 재산
	SI32	siLevel;								// 레벨	
	UI32	uiTradeCredit;							// 무역 신용도
	SI32    siAccount;
	UI08    uiSlot;

	SI32	siClassInGuild;							// 직급
};

struct	strAuditGuildMemberInfo			//장부 길드 멤버.
{
	CHAR	szName[ON_ID_LENGTH + 1];				// 이름
	SI32	siClassInGuild;							// 직급
	BOOL	bLogin;
};

// 용병 한명의 대한 정보
struct	OnImjinBattleFollowerInfo
{
	_ImportantParameta	Iparameta;							// 각 캐릭터들의 중요 파라메타 정보. 
	CHAR				szPlayerName[ON_ID_LENGTH + 1];	// 캐릭터 이름.
	MyWeapons			MW;									// 각 캐릭터들의 인벤토리 상황 정보. 
	UI16				uiKind;								// 캐릭터들.
	UI16				uiItemNum;
	UI16				uiSlotID;							// 슬롯의 ID

	// 뒤에는 아이템의 정보(MyItemData)가 들어가 있다.
};

// 플레이어 및 몬스터의 정보
struct	OnBSPlayerInfo
{	
	SI32		siPlayersUniqueID;					// 캐릭터 ID.	
	SI32		GuildCode;							// 상단 코드 		
	MONEY		mnMoney;							// 돈
	SI32		siDBAccount;						// DB Accont
	UI16		uiFollowerNum;						// 부하들의 수
	UI08		uiDBSlot;							// DB Slot

	// 뒤에는 부하들의 정보(OnImjinBattleFollowerInfo)가 뒤 따라온다.
};

struct FollowerSlot
{
	UI16 uiKind;
	SI16 siLevel;
	SI16 siDex;
	SI16 siInt;
	SI16 siStr;
	SI16 siVit;
	UI08 uiSlot;
};

struct ItemSlot
{
	UI08	uiFollowerID;
	UI16	uiItemID;
	UI16	uiItemCount;
};

// 좌판 관련
struct	OnBaseBoothItem
{
	SI08		siType;
	UI08		uiBoothSlot;	
	UI16		uiProductID;			//아이템이라면 아이템번호가 될 것이고, 용병이면 용병 종류(퇴마사, 승려)가 된다.
};

// 이건 지워버려야 함 ㅡㅡ
struct	OnBoothItem
{
	UI08						uiFollowerID;
	UI16						uiItemID;
	UI16						uiInvenPos;
	MONEY						siPrice;
	CHAR						Desc[ON_MAX_BOOTH_NOTE+1];
};

// 좌판 정보 보기
struct	OnBoothItemInfo
{
	SI08					siType;							// 좌판에 올려놓는 품목구분(아이템, 용병)
	UI08					uiBoothSlot;					// 좌판인벤 슬롯번호
	MONEY					siPrice;						// 가격
	CHAR					Note[ON_MAX_BOOTH_NOTE];		// 설명
	
	ItemSlot				Item;							// 아이템 정보
	OnFollowerInfo			Follower;						// 용병 정보
};		// 뒤에 용병들의 소지 아이템 정보가 Follower.uiItemNum만큼 따라간다.(장착 아이템은 OnFollowerInfo에 이미 포함되어 있음)

// 좌판 아이템, 용병 올리기
struct	OnBoothItemForSell
{
	UI08					uiTradeFollowerID;				// 용병을 좌판에 올림
	
	UI16					uiItemID;						// 아이템을 올림
	UI08					uiFollowerID;

	UI08					uiBoothSlot;					// 좌판인벤 슬롯번호
	MONEY					Price;
	CHAR					Desc[ON_MAX_BOOTH_NOTE];	
};

// 좌판 클라이언트용
struct	SellItemInfo
{
	SI08			siType;									// 좌판에 올려놓는 품목구분(아이템, 용병)
	MONEY			siItemPrice;
	CHAR			szItemDesc[ON_MAX_BOOTH_NOTE+1];

	ItemSlot		Item;
	OnFollowerInfo	Follower;
};

enum	EQuestPayType
{
	QUESTPAYTYPE_NONE     = 0
	, QUESTPAYTYPE_MONEY  = 1
	, QUESTPAYTYPE_CREDIT = 2
	, QUESTPAYTYPE_EXP    = 3
	, QUESTPAYTYPE_ITEM   = 4
	, QUESTPAYTYPE_EVENT_ITEM  = 5
	, QUESTPAYTYPE_EVENT_GAMEITEM = 6
};

enum	EQuestConditionalType
{
	QUESTCONDITIONALTYPE_NONE                     = 0
	, QUESTCONDITIONALTYPE_CREDIT                 = 1
	, QUESTCONDITIONALTYPE_LEVEL                  = 2
	, QUESTCONDITIONALTYPE_BATTLE                 = 3
	, QUESTCONDITIONALTYPE_COMPLETIONQUEST        = 4
	, QUESTCONDITIONALTYPE_ITEM                   = 5
};

enum	EQuestObjectType
{
	QUESTOBJECTTYPE_NONE        = 0
	, QUESTOBJECTTYPE_ITEM      = 1
	, QUESTOBJECTTYPE_WORK      = 2
	, QUESTOBJECTTYPE_BANKBOOK  = 3
	, QUESTOBJECTTYPE_BOOK      = 4
	, QUESTOBJECTTYPE_MERCENARY = 5
	, QUESTOBJECTTYPE_MONSTER   = 6
	, QUESTOBJECTTYPE_SATIETY   = 7
	, QUESTOBJECTTYPE_LEVEL     = 8
	, QUESTOBJECTTYPE_QUEST     = 9
	, QUESTOBJECTTYPE_MONEY	    = 10
	, QUESTOBJECTTYPE_HEALTH	= 11
	, QUESTOBJECTTYPE_PORTAL	= 12
	, QUESTOBJECTTYPE_NPC		= 13
};

struct	SQuestConditional
{
	EQuestConditionalType		m_QuestConditionalType;
	SI32						m_QuestConditionalCode;
	SI32						m_QuestConditionalQuantity;
};

struct	SQuestPay
{
	EQuestPayType		m_QuestPayType;
	SI32				m_QuestPayCode;
	SI32				m_QuestPayQuantity;
	SI32				m_QuestPayEventItemID;		//DB테이블에 있는 아이템 아이디.
};

struct	SSQuestPayData_Money
{
	MONEY				m_AddMoney;
};

struct	SSQuestPayData_Credit
{
	SI32				m_AddCredit;
};

struct	SSQuestPayData_EXP
{
	SI32				m_AddHeroEXP;
};

struct	SSQuestPayData_Item
{
	SI32				m_FollowerSlot;

	MyItemData			m_AddItemData;
};

struct SSQuestPayData_Event
{
	char				szEventName[256];
};

struct	SQuestPayData
{
	EQuestPayType		m_QuestPayType;

	union
	{
		SSQuestPayData_Money	m_QuestPayData_Money;
		SSQuestPayData_Credit	m_QuestPayData_Credit;
		SSQuestPayData_EXP		m_QuestPayData_EXP;
		SSQuestPayData_Item		m_QuestPayData_Item;
		SSQuestPayData_Event	m_QuestPayData_Event;
	};
};

struct	SQuestObject
{
	EQuestObjectType	m_QuestObjectType;
	SI32				m_QuestObjectCode;
	SI32				m_QuestObjectQuantity;
//	SI32				m_QuestItemCollect;
};


typedef struct
{
	SI16			m_QuestCode;										// Quest Code

	SI32			m_QuestObject[ON_QUSETDATA_MAXQUESTENDREQOBJECT];	// Quest 목적들 달성한 수치
} SPlayingQuest;

struct	SQuestHeader;

typedef struct
{
	SI16			m_siCompletedQuest[ON_QUSETDATA_MAXCOMLETEDQUEST];			// 완료된 Quest들
	SQuestHeader*	m_siCompletedQuestHeader[ON_QUSETDATA_MAXCOMLETEDQUEST];	// 완료된 Quest들 Header

	SPlayingQuest	m_sPlayingQuest[ON_QUSETDATA_MAXPLAYINGQUEST];				// 진행중인 Quest
	SQuestHeader*	m_sPlayingQuestHeader[ON_QUSETDATA_MAXPLAYINGQUEST];		// 진행중인 Quest Header

	SI32			m_QuestData_BookCount;										// 장부 수(이 데이터는 SelectNPC메세지 올때만 갱신이 된다.)
} SPlayerQuest;

struct	OnTradeFollowerInfo							// 교역하려는 용병의 정보.
{
	SI32	siLevel;								// 레벨
	SI32	siExp;									// 경험치		
	SI32	siSTR;									// 힘
	SI32	siDEX;									// 민첩성
	SI32	siVIT;									// 생명력
	SI32	siINT;									// 지력
	UI16	uiKind;									// 종족
};

struct	MercenaryItemData
{
	SI08	siSlot;									// 용병 Slot
	SI08	siFlag;									// 아이템이 장비된 아이템인지, 인벤토리의 아이템인지 검사(0 : 인벤토리 아이템, 1 : 장비된 아이템)
	union
	{
		MyItemData		siInventoryItemInfo;
		SI16			siWearItemInfo;
	};
};

// 게임 서버에 로그인 성공 했을 경우의 메시지.
struct	OnGameServerLogInMsgData
{	
	UserGrade					USGrade;						// 플레이어의 직급.
	MONEY						siMoney;						// 플레이어의 돈.
	MONEY                       moSaving;                       // 저축액
	MONEY						YearProfit;						// 연간 수익.	(투자 이익)
	MONEY						SoldFacilityPrice;				// 시설 물이 팔렸을때의 가격. (건물 이익)
	UI32						uiTradeCredit;					// 무역 신용도.
	UI16						uiMyCharUniqueID;				// 내 캐릭터의 UniqueID
	UI16						uiMapID;						// Map ID
	SOnlineTime					Time;							// 현재 서버의 시간.
	OnRealWorldTime				RealTime;						// 현재 세계의 시간.
	CHAR						szID[ON_ID_LENGTH + 1];			// 자신의 ID
	CHAR						szGuildID[ON_GUILDNAME_LENGTH];	// 길드의 ID
	SI32						siGuildClass;
	SI32						siHealth;						// 배고픔 수치.
	SI32						siFlag;							// 깃발
	SI08						siVMercenaySlot;				// 따라다니는 용병 Slot
	SI08						siMaxGeneral;					// 고용 가능한 최대 장수수

	SI32						siEventScore;					// 이벤트 점수.
	SI32						siEventRanking;					// 이벤트 랭킹.

	UI16						uiFollowerNum;					// 부하들의 수.
	UI16						uiPlayerNum;					// 내 주위의 플레이어 수.	
	UI16						uiFieldItemNum;					// 내 주위의 땅에 떨어진 아이템 수. 
	UI32						uiDokdoQuestCompletedUserCount; // 독도 퀘스트를 완료한 유저 수

	BOOL						bCompareWearItem;				// 시간제 아이템이 로그인을 안한 사이 벗겨졌나 그렇지 않나.
	BOOL						bEvent;							// 이벤트 중인가.(서버의 셋팅 파일로 이벤트 진행의 유무를 검사한다.).

	SI16						m_siFieldHp;
	SI32						m_siSupplyCharge;
	UI16						m_uiPrevKind;
	DWORD						m_dwDeadSec;
	BOOL						m_bCaptainUnit;

	BOOL						m_bGuildWarDay;					// 공성하는 날이다.


	//  플레이어, 땅에 떨어진 아이템, 부하들의 정보(아이템 포함)이 대한 정보가 있다.
};

struct	OnGuildVillageInfo
{
	UI16		uiVillageCode;									// VillageCode
	CHAR		szBestInvestorName[ ON_ID_LENGTH + 1 ];			// 최고 투자자의 이름.
	MONEY		mnBestInvestorMoney;							// 최고 투자자의 돈.
	VDEF		vdVillageDefencePower;							// 마을 방어력.	
};

struct	OnIndustrialComplexTradeInfo
{
	UI16		uiVillageCode;
	UI16		uiIndustrialComplexID;
	CHAR		cFactoryKind[ ON_MAX_FACTORY_NUM ][ 4 ];
};

//아이템 Upgrade 하는 경우 이 구조체를 통해서 결과값을 반환
//2003. 3. 31 이상민 추가
struct OnItemUpgradeResultInfo
{
	SI16 siResult;													// enum { ITEM_UPGRADE_BROKEN = -1, ITEM_UPGRADE_FAIL, ITEM_UPGRADE_SUCCESS }; 
	UI16 uiItemID;
};

// 장부의 투자 마을 리스트에 들어갈 한 마을의 정보
struct OnAuditVillageInfo
{
	UI16		uiVillageCode;
	SI32		siLastInvestedTime;
	R32			fPercent;
	MONEY		mnInvestedMoney;
};

struct OnVillageAttackPlayerList
{
	UI16	uiPlayerID;
};

struct OnGuildResult
{
	SI16 siVillageCode;
	SI16 siPercent;
};

// 장부의 친구 리스트에 들어갈 한 친구의 정보
typedef struct strFriendInfo	OnAuditFriendInfo;

#endif
