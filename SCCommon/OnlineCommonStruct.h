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


// ������ Ŭ���̾�Ʈ�� ���������� ����ϴ� ����ü.
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


// �� ĳ���Ͱ� �κ��丮���� ������ �ִ� ������.

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
	UI16		uiItemCode;												// �������ڵ�
	UI16		uiQuantity;												// ����
} STORAGE_ITEM;

struct	MyItemData
{
	SI16	siPosInInventory;					// �κ��丮�������� ��ǥ.
	UI16	uiID;								// ID
	UI16	uiQuantity;							// ����
};

struct TradeFollower							//	����â�� �÷� ������.
{
	UI16	Code;
	SI16	siTradePos;							//  Ʈ���̵� â������ ��ǥ. ������ �ʿ� ����.
};

struct TradeItem								//	����â�� �÷� ������.
{

	UI16	uiFollower;
	UI16	uiItemCode;
	SI16	siTradePos;							// Ʈ���̵� â������ ��ǥ.
	SI32	siQuantity;							//	����
};

// ĳ������ �⺻ ����.
struct	OnCharBaseInfo
{
	UI16	uiAccount;							// ĳ������ Account
	SI16	siX;								// siX
	SI16	siY;								// siY
	UI16	uiKind;								// ĳ������ ����.
};

struct	OnCharMonsterInfo : public OnCharBaseInfo	// ������ ����
{	
	SI32	siFightingPower;						// ���ݷ�.
};

struct	OnCharPlayerInfo : public OnCharBaseInfo	// �÷��̾��� ����.
{		
	SI32	siFightingPower;						// ���ݷ�.

	SI32	siDBAccount;	
	UI16	uiEquipWeapon[WEAR_ITEM_NUM];	  	    // 0:����,1:����,2:����,3:��ȣ��		
	SI08	siPrivateMarketItemNum;					// ���� ���� ����� ���, �������� ����
	SI32	siGuildInGrade;							// ��峻�� ����.
	SI08	siIsOverWeight;							// ���� �ʰ��ΰ�?
	SI32	siVMercenaryKind;						// ����ٴϴ� �뺴 Kind
	BOOL	bAttackMode;							// ��������ΰ�?

	SI16	siFieldHp;								// ������ ������ ü��.
	BOOL	bCaptain;								// �����ΰ� �ƴѰ�.
	SI16	siSupplyCharge;
	BOOL	bVillageInLimit;
};

struct	OnCharShipInfo : public OnCharBaseInfo		// ���� ����.
{
};

struct	OnCharPlayerDetailInfo
{
	CHAR	szID[ON_ID_LENGTH];						// ID
	CHAR	szGuild[ON_GUILDNAME_LENGTH];			// Guild ID
	SI32	siGuildFlag;							// ��� ���.
	UI08	uiIsHostilityGuild;						// ���� �������� ����ΰ�?	
	SI32	siEventScore;							// �̺�Ʈ ����.
	SI32	siEventRanking;							// �̺�Ʈ ��ŷ.
};


/*
// ���� ĳ������ ����.
struct	OnlineReceivedPlayerInfo
{	
	UI16	uiPlayer;
	SI16	siX;
	SI16	siY;
	UI16	uiKind;	
	SI32	siFightingPower;						// ���ݷ�	
};

// ����� ĳ������ ���
struct	OnlineReceivedHeroInfo
{
	UI16	uiPlayer;
	SI16	siX;
	SI16	siY;
	UI16	uiKind;		
	SI32	siFightingPower;				// ���ݷ�	
	
	SI32	siDBAccount;
	SI32	siGuildInGrade;
	UI16	uiEquipWeapon[3];				// 0:����,1:����,2:����
	CHAR	szID[ON_ID_LENGTH];
	CHAR	szGuild[ON_GUILDNAME_LENGTH];	
	SI32	siGuildFlag;					// ��� ���.
	
	UI08	uiIsHostilityGuild;				// ���� �������� ����ΰ�?
	SI08	siPrivateMarketItemNum;			// ���� ���� ����� ���, �������� ����
};
*/

// �ʵ忡 ������ �������� ����.
struct	OnlineReceiveFieldItemInfo
{
	UI16	uiItemID;
	UI16	uiQuantity;
	SI16	siX;
	SI16	siY;
}; 
/*
// �� �÷��̾�� 3���� ĳ���͸� Ű�� �� �ִ� �� ĳ������ ����ü.(�α��ν� �α��� ������ ���� �޴´�.)
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

// �α��� ���� ���ӽÿ� �α��� ������ ���� �޴� �޽���.
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
// ���� ���� ���ӽÿ� ���� ������ ���� �޴� �޽���.
struct	OnlineReceivedMyData
{	
	MONEY						uiMoney;

	UI16						uiPlayerNum;
	UI16						uiItemNum;
	UI16						uiFieldItemNum;

	OnlineReceivedMyItemInfo	*pItemInfo;				// �� �������� ����.
	OnCharBaseInfo				*pCharBaseInfo;			// �� ������ ĳ������ ����.
	OnlineReceiveFieldItemInfo	*pFieldItemInfo;		// �ʵ忡 ������ �������� ����.
};
*/

// ��ǥ�� �����ų� ������ ����ϴ� �޽���.
struct	OnlinePosition
{
	SI32	siX;
	SI32	siY;
};

// �������� �����ų� ������ ����ϴ� ����ü.
struct	OnlineReceivedItemInfo
{
	UI16	uiID;	
	UI16	uiQuantity;
	MONEY	BasePrice;						// �������� ���� ����
};

// ������ �ϴ� ĳ���͵��� ���� ����.
struct	OnBattlePlayerInfo
{	
	CHAR	szPlayerName[ON_ID_LENGTH];	// �÷��̾��� �̸�.
	UI16	uiPlayersUniqueID;			// �÷��̾��� UniqueID	
	UI16	uiSlot;						// Slot��ȣ.
	UI08	uiCountry;					// ����.	
	UI16	uiKind[10];					// Kind.		
};

// ����, ����, ��ȭ�濡�� ä���� �ϴ� ĳ���͵��� ����.
struct	OnVilINNChatPlayerInfo
{
	CHAR	szName[ON_ID_LENGTH];		// �̸�.
	SI32	siDBAccount;				// DBAccont
	UI16	uiPlayerUniqueID;			// Player�� UniqueID
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

	// ĳ������ �ɷ�ġ.	
	SI32	siExp;						// ����ġ.
	SI32	siLevel;					//	����
	UI16	siStr;						// ü��.
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

	//	������ ������ 
	//UI16	uiWearIte[WEAR_ITEM_NUM];
	OnWearItemInfo	stWearItemInfo[WEAR_ITEM_NUM];


	CHAR	Name[ON_ID_LENGTH];			//	�뺴 �̸�
	
	// �ڿ� ���� ���߿� �ֵ��� ����.
	//UI08	uiBlockingRate;				// BlockingRate

	// �ڿ��� ������(MyItemData)�� uiItemNum �� ��ŭ �ִ�.
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

	// ĳ������ �ɷ�ġ.	
	SI32	siExp;						// ����ġ.
	SI32	siLevel;					//	����
	UI16	siStr;						// ü��.
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

	CHAR	Name[ON_ID_LENGTH];			//	�뺴 �̸�
};

struct	OnBuyItemPosInfo
{	
	UI16	uiPosInInventory;			// �������� ��ġ.
	UI16	uiQuantity;					// �������� ����.
};

// OnSellItemPosInfo�� OnBuyItemPosInfo�� ����.
typedef	struct	OnBuyItemPosInfo	OnSellItemPosInfo;

struct SOnlineTime
{
	int			Year;								// �⵵
	int			Month;								// ��
	int			Day;								// ��
	int			Hour;								// ��
};

struct	OnRealWorldTime
{
	UI32	uiYear		: 12;						// �⵵
	UI32	uiMonth		: 4;						// ��
	UI32	uiDay		: 5;						// ��
	UI32	uiHour		: 5;						// ��
	UI32	uiMinute	: 6;						// ��
};

// ������ ����.
struct	OnRecvVillageInfo
{
	UI32		uiPopulation;
	CHAR		szGuildID[ON_GUILDNAME_LENGTH + 1];

	CHAR		m_MaxInvestmentID[21];			// �ְ� ������ ID
	VDEF		vdVillageDefence;				// ������ ����.	
	VDEF		vdVillageMaxDefence;			// ������ �ִ� ����.
	
	DWORD		dwGuildRelation;				// ����� ����.

	CHAR		cNationCode;					// ���� �ڵ�.
	SI32		siGuildFlag;					// ���.

	SI32		siDefenceSoldier;
	SI32		siMaxDefenceSoldier;
	SI16		siAttackType;
};


// ���¹�ȣ ((3)uiVillageCode - (4)uiAccountKind - (7)(uiUserDBAccount(4) - uiUserDBAccount(3)) - (1)uiCharSlot - (1)uiAccountIndex)
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
	SI32		siID;								// ��� ������ ID
	CHAR		szName[ ON_ID_LENGTH + 1 ];			// ��� ������ ����.
	UI32		m_uiTotalAmountOfWork;				// ��� ������ ��ü �۾���.
	MONEY       m_moMaxPay;                         // ��� ������ �ִ� �ӱ� 
};

/*
struct	OnFarmList
{	
	UI08			uiAvailSlot;					//����ü� ���Լ�
//	CHAR			szFarmName[ON_ID_LENGTH+1];		//���� �̸� 
	CHAR			szOwner[ON_ID_LENGTH+1];		//������ �̸�.
	UI32			uiRemainLabor;					//���� ��

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
	UI08			SlotIndex;						// �� ��° �����ΰ�?
	UI08			uiLevel;						// �� ������ ����.	
	CHAR			StructCode[4];					// �ǹ� �ڵ�	
	SI16			siPay;							//	�ӱ�
	UI32			uiLabor;						// �뵿��.
};

// ������ ��ȭ���� ���� ����.
struct	OnInnChatRoomInfo
{
	UI16		uiChatRoomID;
	UI08		uiJoinedPlayerNum;
	UI08		uiChatRoomNameStringLength;	
};

// �Խ����� �Խù��� ���� ����.
struct	OnInnNBNoticeInfo
{
	UI16		uiIndex;								// �� ��ȣ.
	UI08		uiWroteIDStringLength;					// �۾����� �̸� ����
	UI08		uiTitleStringLength;					// �� ������ ���ڿ� ����
	CHAR		szDate[6];								// ��¥.
	DWORD		dwReadCount;							// ���� Ƚ��
};

// ���� ����
struct	OnInnBuyWorkerInfo
{
	SI32		siCode;									// ���� ID
	SI32		siLevel;								// ���� ��� Level
	SI16		siKind;									// ���� Kind
	UI32		uiCharNameAddOption;					// ���� Name Add Option
	MONEY		siMoney;								// ���� ����
	SI32		siLiftPower;							// ��� �ִ� ����
};

// �뺴 ����
struct	OnInnBuySolderInfo
{
	SI32			siCode;								// �뺴 ID
	SI32			siLevel;							// ���Խ� �ʿ��� Level
	SI32			siCredit;							// ���Խ� �ʿ��� �ſ뵵
	UI32			uiCharNameAddOption;				// �뺴 Name Add Option
	MONEY			siMoney;							// �뺴 ����

	OnFollowerInfo	SolderInfo;							// �뺴 �ɷ�
};

// �뺴 ����
struct	OnDismissalSolderInfo
{
	SI32			siSlot;								// Slot

	MONEY			siMoney;							// �ذ�� ��� ��
};


// ���� �뺴 ����
struct	OnInnBuyMonsterInfo
{
	SI32			siCode;								// ���� �뺴 ID
	SI32			siLevel;							// ���Խ� �ʿ��� Level
	SI32			siCredit;							// ���Խ� �ʿ��� �ſ뵵
	UI32			uiCharNameAddOption;				// �뺴 Name Add Option
	MONEY			siMoney;							// �뺴 ����
	SI32			siSealingBeadNum;					// �ʿ��� ������ ���� ����

	OnFollowerInfo	SolderInfo;							// �뺴 �ɷ�
};

struct OnChangeJobSolderInfo
{
	SI32			siSlot;								// Slot

	SI32			siJobList[4];						// �ٲܼ� �ִ� Job(0�̸� ���°��̴�.)
	UI08			siJobListFlag[4];					// �ٲܼ� �ִ��� Flag
	SI32			siJobListChangeLevel[4];			// �ٲܶ� �ʿ��� Level
	SI32			siBonus;							// ������ ���ʽ��� ������ �ִ� ��ġ
};

// Ticket ����
struct	OnTicketInfo
{
	SI32			m_siTicketKind;						// Ticket Kind
};

// ���� ����
typedef struct
{
	UI08			m_InvestmentType;					// �������� ����(0 : Player, 1 : ���, 2 : ����)
	UI08			m_InvestmentName[21];				// �������� �̸�

	float			m_InvestmentPercent;				// ���� ����
	MONEY			m_InvestmentMoney;					// ���� �ݾ�
	int				m_InvestmentUpdateTime;				// ���� �ð�
} OnInvestmentInfo;


// ��Ƽ�� ����.
struct OnPartyInfo
{
	BOOL			bIsPartyLeader;						// ��Ƽ���ΰ�?
	SI16			siLevel;							// ĳ������ ����
	CHAR			Name[ ON_ID_LENGTH + 1 ];			// ĳ������ �̸�

};


// ��� �ε���
struct	OnAuditOwnInfo	
{
	UI08			PlantID;							//	�ε��� ���� 0:���� ....
	UI16			uiVillageCode;						//	�ε����� �ִ� ����
};



struct	strGuildInfo									// ����� ����
{
	CHAR			szGuildID[ON_GUILDNAME_LENGTH];		// ����� �̸�.
	CHAR			szDAIID[ON_ID_LENGTH];				// ����� �̸�.
	UI16			uiOwnVillageNum;					// �����ϰ� �ִ� ������ ��.
	UI32			uiGuildMemberNum;					// ��� ��� ��.
	SI32			siRelation;							// ���� ����� ���� �츮 ����� ����
	SI32			siRelationOfOpponent;				// �� ����� ���� ���� ����� ����
	UI32			uiMaxGuildMembersNum;				// �ִ� ������ �� �ִ� ��ܿ��� ��
	SI32			siFlag;								// ����� ���.
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
	UI32				uiGuildID;						// ��� ID
	SI32				siFlag;							// ����ϴ� ���.
};

struct stCapital
{
	SI16		Year;
	union
	{
		MONEY	Profit[ON_MAX_PROFIT_SORT];						// �迭�� �ٷ궧.
		struct
		{
			MONEY	TradeProfit;						// ��������(����)
			MONEY	BankProfit;							// �������(����)
			MONEY	EmployProfit;						// �뺴����(�Ʒü�)
			MONEY	CureProfit;							// ġ�����(�ǿ�)
			MONEY	TicketProfit;						// Ƽ�ϼ���(�ױ�)
			MONEY	InnProfit;							// ��������(�ָ�)
			MONEY	PlantProfit;						// ����ǹ�����(����,����,����)
		};
	};
};


struct  strGuildCommonMemberInfo
{
	SI32    siAccount;
	UI08    uiSlot;

	SI32	siClassInGuild;							// ����

};

struct	strCandidateInfo
{
	// �������� ����.
	CHAR	szName[ON_ID_LENGTH + 1];				// �̸�	
	MONEY	mnPropery;								// ���
	SI32	siLevel;								// ����	
	UI32	uiTradeCredit;							// ���� �ſ뵵
	SI32    siAccount;
	UI08    uiSlot;

};

struct	strGuildMemberInfo
{

	// �������� ����.
	CHAR	szName[ON_ID_LENGTH + 1];				// �̸�	
	MONEY	mnPropery;								// ���
	SI32	siLevel;								// ����	
	UI32	uiTradeCredit;							// ���� �ſ뵵
	SI32    siAccount;
	UI08    uiSlot;

	SI32	siClassInGuild;							// ����
};

struct	strAuditGuildMemberInfo			//��� ��� ���.
{
	CHAR	szName[ON_ID_LENGTH + 1];				// �̸�
	SI32	siClassInGuild;							// ����
	BOOL	bLogin;
};

// �뺴 �Ѹ��� ���� ����
struct	OnImjinBattleFollowerInfo
{
	_ImportantParameta	Iparameta;							// �� ĳ���͵��� �߿� �Ķ��Ÿ ����. 
	CHAR				szPlayerName[ON_ID_LENGTH + 1];	// ĳ���� �̸�.
	MyWeapons			MW;									// �� ĳ���͵��� �κ��丮 ��Ȳ ����. 
	UI16				uiKind;								// ĳ���͵�.
	UI16				uiItemNum;
	UI16				uiSlotID;							// ������ ID

	// �ڿ��� �������� ����(MyItemData)�� �� �ִ�.
};

// �÷��̾� �� ������ ����
struct	OnBSPlayerInfo
{	
	SI32		siPlayersUniqueID;					// ĳ���� ID.	
	SI32		GuildCode;							// ��� �ڵ� 		
	MONEY		mnMoney;							// ��
	SI32		siDBAccount;						// DB Accont
	UI16		uiFollowerNum;						// ���ϵ��� ��
	UI08		uiDBSlot;							// DB Slot

	// �ڿ��� ���ϵ��� ����(OnImjinBattleFollowerInfo)�� �� ����´�.
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

// ���� ����
struct	OnBaseBoothItem
{
	SI08		siType;
	UI08		uiBoothSlot;	
	UI16		uiProductID;			//�������̶�� �����۹�ȣ�� �� ���̰�, �뺴�̸� �뺴 ����(�𸶻�, �·�)�� �ȴ�.
};

// �̰� ���������� �� �Ѥ�
struct	OnBoothItem
{
	UI08						uiFollowerID;
	UI16						uiItemID;
	UI16						uiInvenPos;
	MONEY						siPrice;
	CHAR						Desc[ON_MAX_BOOTH_NOTE+1];
};

// ���� ���� ����
struct	OnBoothItemInfo
{
	SI08					siType;							// ���ǿ� �÷����� ǰ�񱸺�(������, �뺴)
	UI08					uiBoothSlot;					// �����κ� ���Թ�ȣ
	MONEY					siPrice;						// ����
	CHAR					Note[ON_MAX_BOOTH_NOTE];		// ����
	
	ItemSlot				Item;							// ������ ����
	OnFollowerInfo			Follower;						// �뺴 ����
};		// �ڿ� �뺴���� ���� ������ ������ Follower.uiItemNum��ŭ ���󰣴�.(���� �������� OnFollowerInfo�� �̹� ���ԵǾ� ����)

// ���� ������, �뺴 �ø���
struct	OnBoothItemForSell
{
	UI08					uiTradeFollowerID;				// �뺴�� ���ǿ� �ø�
	
	UI16					uiItemID;						// �������� �ø�
	UI08					uiFollowerID;

	UI08					uiBoothSlot;					// �����κ� ���Թ�ȣ
	MONEY					Price;
	CHAR					Desc[ON_MAX_BOOTH_NOTE];	
};

// ���� Ŭ���̾�Ʈ��
struct	SellItemInfo
{
	SI08			siType;									// ���ǿ� �÷����� ǰ�񱸺�(������, �뺴)
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
	SI32				m_QuestPayEventItemID;		//DB���̺� �ִ� ������ ���̵�.
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

	SI32			m_QuestObject[ON_QUSETDATA_MAXQUESTENDREQOBJECT];	// Quest ������ �޼��� ��ġ
} SPlayingQuest;

struct	SQuestHeader;

typedef struct
{
	SI16			m_siCompletedQuest[ON_QUSETDATA_MAXCOMLETEDQUEST];			// �Ϸ�� Quest��
	SQuestHeader*	m_siCompletedQuestHeader[ON_QUSETDATA_MAXCOMLETEDQUEST];	// �Ϸ�� Quest�� Header

	SPlayingQuest	m_sPlayingQuest[ON_QUSETDATA_MAXPLAYINGQUEST];				// �������� Quest
	SQuestHeader*	m_sPlayingQuestHeader[ON_QUSETDATA_MAXPLAYINGQUEST];		// �������� Quest Header

	SI32			m_QuestData_BookCount;										// ��� ��(�� �����ʹ� SelectNPC�޼��� �ö��� ������ �ȴ�.)
} SPlayerQuest;

struct	OnTradeFollowerInfo							// �����Ϸ��� �뺴�� ����.
{
	SI32	siLevel;								// ����
	SI32	siExp;									// ����ġ		
	SI32	siSTR;									// ��
	SI32	siDEX;									// ��ø��
	SI32	siVIT;									// �����
	SI32	siINT;									// ����
	UI16	uiKind;									// ����
};

struct	MercenaryItemData
{
	SI08	siSlot;									// �뺴 Slot
	SI08	siFlag;									// �������� ���� ����������, �κ��丮�� ���������� �˻�(0 : �κ��丮 ������, 1 : ���� ������)
	union
	{
		MyItemData		siInventoryItemInfo;
		SI16			siWearItemInfo;
	};
};

// ���� ������ �α��� ���� ���� ����� �޽���.
struct	OnGameServerLogInMsgData
{	
	UserGrade					USGrade;						// �÷��̾��� ����.
	MONEY						siMoney;						// �÷��̾��� ��.
	MONEY                       moSaving;                       // �����
	MONEY						YearProfit;						// ���� ����.	(���� ����)
	MONEY						SoldFacilityPrice;				// �ü� ���� �ȷ������� ����. (�ǹ� ����)
	UI32						uiTradeCredit;					// ���� �ſ뵵.
	UI16						uiMyCharUniqueID;				// �� ĳ������ UniqueID
	UI16						uiMapID;						// Map ID
	SOnlineTime					Time;							// ���� ������ �ð�.
	OnRealWorldTime				RealTime;						// ���� ������ �ð�.
	CHAR						szID[ON_ID_LENGTH + 1];			// �ڽ��� ID
	CHAR						szGuildID[ON_GUILDNAME_LENGTH];	// ����� ID
	SI32						siGuildClass;
	SI32						siHealth;						// ����� ��ġ.
	SI32						siFlag;							// ���
	SI08						siVMercenaySlot;				// ����ٴϴ� �뺴 Slot
	SI08						siMaxGeneral;					// ��� ������ �ִ� �����

	SI32						siEventScore;					// �̺�Ʈ ����.
	SI32						siEventRanking;					// �̺�Ʈ ��ŷ.

	UI16						uiFollowerNum;					// ���ϵ��� ��.
	UI16						uiPlayerNum;					// �� ������ �÷��̾� ��.	
	UI16						uiFieldItemNum;					// �� ������ ���� ������ ������ ��. 
	UI32						uiDokdoQuestCompletedUserCount; // ���� ����Ʈ�� �Ϸ��� ���� ��

	BOOL						bCompareWearItem;				// �ð��� �������� �α����� ���� ���� �������� �׷��� �ʳ�.
	BOOL						bEvent;							// �̺�Ʈ ���ΰ�.(������ ���� ���Ϸ� �̺�Ʈ ������ ������ �˻��Ѵ�.).

	SI16						m_siFieldHp;
	SI32						m_siSupplyCharge;
	UI16						m_uiPrevKind;
	DWORD						m_dwDeadSec;
	BOOL						m_bCaptainUnit;

	BOOL						m_bGuildWarDay;					// �����ϴ� ���̴�.


	//  �÷��̾�, ���� ������ ������, ���ϵ��� ����(������ ����)�� ���� ������ �ִ�.
};

struct	OnGuildVillageInfo
{
	UI16		uiVillageCode;									// VillageCode
	CHAR		szBestInvestorName[ ON_ID_LENGTH + 1 ];			// �ְ� �������� �̸�.
	MONEY		mnBestInvestorMoney;							// �ְ� �������� ��.
	VDEF		vdVillageDefencePower;							// ���� ����.	
};

struct	OnIndustrialComplexTradeInfo
{
	UI16		uiVillageCode;
	UI16		uiIndustrialComplexID;
	CHAR		cFactoryKind[ ON_MAX_FACTORY_NUM ][ 4 ];
};

//������ Upgrade �ϴ� ��� �� ����ü�� ���ؼ� ������� ��ȯ
//2003. 3. 31 �̻�� �߰�
struct OnItemUpgradeResultInfo
{
	SI16 siResult;													// enum { ITEM_UPGRADE_BROKEN = -1, ITEM_UPGRADE_FAIL, ITEM_UPGRADE_SUCCESS }; 
	UI16 uiItemID;
};

// ����� ���� ���� ����Ʈ�� �� �� ������ ����
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

// ����� ģ�� ����Ʈ�� �� �� ģ���� ����
typedef struct strFriendInfo	OnAuditFriendInfo;

#endif
