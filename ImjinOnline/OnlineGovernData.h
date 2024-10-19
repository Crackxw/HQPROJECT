#ifndef	_ONLINE_GOVERNMENT_DATA_HEADER
#define _ONLINE_GOVERNMENT_DATA_HEADER

#define	VILLAGE_DESCRIPT_NOTE			254		// ���� �ȳ��� ����
#define	INVESTOR_MAX_LINE				5		// ������ ��� �ִ����

#define	GUILDMAKE_MINLEVEL				30		// ����� ����� �ּ� �ʿ� Level
#define	GUILDMAKE_MINCREDIT				20		// ����� ����� �ּ� �ʿ� �ſ뵵

#define GUILD_FLAG_COST					100000	// ��ܱ�� ���� ���
#define	GUILD_FLAG_MIN_NEED_MEMBER		60		// ��ܱ�� ������ �ʿ��� �ּ� ��ܿ���
#define GUILD_FLAG_XCOUNT				6		// ��ܱ�� ���̴� X���� 
#define GUILD_FLAG_YCOUNT				1		// ��ܱ�� ���̴� Y���� 

#define	GUILD_LIST_MAX_LINE				8		// ��ܸ���Ʈ �ִ����
#define	PAGE_LIST_MAX_LINE				5		// ����, ��ܿ� ����Ʈ �ִ����

#define OFFERPEACE_MESSAGE_LENGTH		80		// ȭģ���� �޽��� ����(NULL���� ����)

enum
{   
	ON_SUB_VILLAGEINFO_INIT = 0,
	ON_SUB_VILLAGEINFO_POLL,
	ON_SUB_VILLAGEINFO_SETDEFENCE,
	ON_SUB_VILLAGEINFO_WAIT,
	ON_SUB_VILLAGEINFO_EXIT,

	// robypark 2004/11/15 14:52
	// ������ ��û ���� ���� �� �� �޴� �߰�
	ON_SUB_VILLAGEINFO_TO_DEFENCE_MENU,			// �����ϱ�
	ON_SUB_VILLAGEINFO_RECRUIT_ARMY,			// ���� ����
	ON_SUB_VILLAGEINFO_PRODUCE_SUPPLYGOODS,		// ����ǰ ����
	ON_SUB_VILLAGEINFO_DECLARE_OCCUPATION		// ���� ����
};

// ������ ��û �������� �߿� ����, ���� �������� ���� �޴� ó���� ����
enum
{   
	ON_SUB_VILLAGEINFO_SUB_INIT = 0,
	ON_SUB_VILLAGEINFO_SUB_POLL,
	ON_SUB_VILLAGEINFO_SUB_WAIT,
	ON_SUB_VILLAGEINFO_SUB_EXIT,
	ON_SUB_VILLAGEINFO_SUB_RECRUIT_FIELD_ARMY,		// �ʵ� ���� ������ ������ ���� ����
	ON_SUB_VILLAGEINFO_SUB_RECRUIT_GARRISON_ARMY,	// ���� �ֵб� ����
	ON_SUB_VILLAGEINFO_SUB_PRODUCE_SUPPLYGOODS		// ���� ����ǰ ����
};

enum
{   
	ON_SUB_INVEST_INIT = 0,
	ON_SUB_INVEST_POLL,
	ON_SUB_INVEST_WAIT,
	ON_SUB_INVEST_SETINVEST,
	ON_SUB_INVEST_GETOUT,
	ON_SUB_INVEST_EXIT,
	ON_SUB_INVEST_SETINVEST_ALLIN,	// ��ź ���� ���� �������̽� Ȱ��ȭ�� ���, ������ ��ź ���� �޽��� ����
};

enum
{   
	ON_SUB_GUILD_INIT = 0,
	ON_SUB_GUILD_POLL,
	ON_SUB_GUILD_WAIT,
	ON_SUB_GUILD_OPEN,
	ON_SUB_GUILD_WAITING_LIST,
	ON_SUB_GUILD_FLAG,
	ON_SUB_GUILD_JOIN_LEAVE,
	ON_SUB_GUILD_CLOSE,
	ON_SUB_GUILD_MEMBER_LIST,
	ON_SUB_GUILD_DIFF_GUILD,
	ON_SUB_GUILD_EXIT
};

enum
{   
	ON_SUB_SUB_GUILD_INIT = 0,
	ON_SUB_SUB_GUILD_POLL,
	ON_SUB_SUB_GUILD_RELATION,
	ON_SUB_SUB_GUILD_VILLAGELIST,
	ON_SUB_SUB_GUILD_WAIT,
	ON_SUB_SUB_GUILD_WAIT2,
	ON_SUB_SUB_GUILD_EXIT,
	ON_SUB_SUB_GUILD_OFFERPEACE,			// ȭģ�����ϱ� �޴� ó��
	ON_SUB_SUB_GUILD_SHOW_OFFEREDPEACE,		// ȭģ������ ���� �޽��� ���� ó��
};

// robypark 2004/11/25 11:10
// ȭģ���� ���� �������̽� ó��
enum
{
	ON_SUB_SUB_GUILD_OFFERPEACE_INIT = 0,	// �ʱ�ȭ
	ON_SUB_SUB_GUILD_OFFERPEACE_POLL,		// ���� ó��
	ON_SUB_SUB_GUILD_OFFERPEACE_WAIT,		// ���
	ON_SUB_SUB_GUILD_OFFERPEACE_EXIT,		// ����
};

// ��û���� ������ ���¸����� �� �����Ϸ��� ���� ���� ����ü
struct SRECRUIT_ArmyInformation_InGovern
{
	SI32			m_siArmyKind;				// �����Ϸ��� ĳ������ Imperium.txt���� �о���� ������ ���� �ε���
//	SI32			m_siArmyUnitCount;			// �����Ϸ��� ���� �δ� ��
	SI32			m_siArmyUnitCost;			// �����Ϸ��� �� �δ��� �� ����� ���. �� �˺� �� �δ�� 100�̹Ƿ� �� �δ��� ����� �α����� ���� ���̴�.
	SI32			m_siArmyUnitSupplyGoods;	// �����Ϸ��� �� �δ��� ����ǰ ���
	SI32			m_siArmyUnitPopulation;		// �����Ϸ��� �� �δ��� �α���
};

// ��û���� ������ ���¸����� �� �������̽� ���� ����ü
struct SRECRUIT_Army_InGovern
{
	// ���¸��� �������̽� ��� �̹���
	XSPR				m_MainImage;

	// ���¸��� ��ư �̹���
	XSPR				m_sprBtnRecruit;

	// ���� ������ �� ���Ǵ� ������ ���� ��ư
	_Button				m_btnSiegeWarfare_Swordman;		// ������ �˺� ���� ��ư
	_Button				m_btnSiegeWarfare_Spearman;		// ������ â�� ���� ��ư
	_Button				m_btnSiegeWarfare_Horseman;		// ������ �⸶�� ���� ��ư
	_Button				m_btnSiegeWarfare_Archerman;	// ������ �ú� ���� ��ư
	_Button				m_btnSiegeWarfare_Siege_tower;	// ������ ���� ���� ��ư
	_Button				m_btnSiegeWarfare_Catapult;		// ������ �߼��� ���� ��ư
	_Button				m_btnSiegeWarfare_Supply_wagon;	// ������ ���޼��� ���� ��ư
	_Button				m_btnSiegeWarfare_Garrison;		// ������ ����(�ֵб�) ���� ��ư

	// ��û���� ������ ���¸����� �� �����Ϸ��� ���� ���� ����ü
	SRECRUIT_ArmyInformation_InGovern	m_sArmyInformation_InGovern;

	SI32				m_siGuildCount[ON_MAX_GUILD_UNIT];	// ������ ������ ����� ������ ������ ���ֵ� ����Ʈ.
	UI32				m_siRecruitablePopulation;			// ���� ������ �α���
	BOOL				m_bDownloadedUnitCount;				// ������ ���� ����Ʈ�� �ٿ�ε��Ͽ���.

	SI32				m_siDefenceSoldier;					// ���� ��
	BOOL				m_bDownloadedDefenceSoldier;		// ������ ���� ���� �ٿ�ε��Ͽ���.
};

// ���� ����ǰ ���� �������̽� ����ü
struct SProduceSupplyGoods
{
	// ����ǰ ���� �������̽� ��� �̹���
	XSPR				m_MainImage;

	// ����ǰ ���� ��ư �̹���
	XSPR				m_sprMiniButton;

	_Button				m_btnProduceSupplyGoods;			// ����ǰ ���� ��ư
	SI16				m_siCostSupplyGoods;				// ����ǰ �ϳ��� ������ 125��

	SProduceSupplyGoods()
	{
		m_siCostSupplyGoods = 125;
	}
};

// �������� ���� ����ü
struct VillageInfo
{
	UI08					m_uiStatus, m_uiSubStatus;
	XSPR					m_MainImage, m_DescOkImage, m_ButImage;
	_Button					m_BDescOk, m_BDefence, m_btnBack/*�ڷ�, ���� �޴��� ���ư���*/;

	// robypark 2004/11/15 13:50
	// ����Ǵ� �������� ���� ��û -���� ���� - �߰� ��ư
	_Button					m_btnToDefenceMenu;				// �����ϱ� ��ư
	_Button					m_btnRecruitArmy;				// ���¸��� ��ư
	_Button					m_btnProduceSupplyGoods;		// ����ǰ ���� ��ư
	_Button					m_btnDeclareOccupation;			// ���ɼ��� ��ư

	// ���� ���� ���� ����ü ����
	SRECRUIT_Army_InGovern	m_sRECRUIT_Army_InGovern;

	// ���� ����ǰ ���� �������̽� ����ü ����
	SProduceSupplyGoods		m_sProduceSupplyGoods;

	UI32					m_uiPopulation;					// �α�
	SI32					m_siInOutPlayer;				// ���� ���������� ��
	VDEF					m_CurDefPower, m_MaxDefPower;	// ����
	MONEY					m_MoneyPerDef, m_MaxMoney;		// ���� 1�� Money, �ִ���º����ڱ�
	CHAR					m_szStruct[256];				// ���νü�

	//===========================================================================================
	// actdoll (2004/05/04 16:33) : [OECEX]
//	OnlineEditControl*		m_pEditBox;
//	RECT					m_rcDescRect;					// �����ȳ��� ����
	HOEC					m_hOecNotice;					// ���� �ȳ���

	SI32					m_siSupplyGoodsAmount;				// ��� ����ǰ ��

	VillageInfo()
	{
		m_uiPopulation = m_siInOutPlayer = m_CurDefPower = m_MaxDefPower = 0;
		ZeroMemory(m_szStruct, sizeof(m_szStruct));
//		SetRect(&m_rcDescRect, 0, 0, 0, 0);
	}
};

// ���� ���� ����ü
struct InvestmentInfo
{
	UI08				m_uiStatus;
	XSPR				m_MainImage;

	SI08				m_siTalk;								// ��û��� ����
	MONEY				m_TotInvestMoney;						// �� ���ڱ�
	MONEY				m_TotCapital;							// �� ����

	XSPR				m_InvestButImage;						// ���ڹ�ư �̹���
	_Button				m_BGetOut, m_BInvest;					// ���ڱ� ����, ����
	_Button				m_BYearUp, m_BYearDown;					// ���ͳ⵵

	// ��ũ�Ѱ���
	SI16				m_siScroll, m_siScrollPos;
	RECT				m_rtScroll;								// ��ũ�� ����
	XSPR				m_ScrBackImage, m_ScrBarImage;			// ��ũ�ѹ� �̹���
	XSPR				m_UpImage, m_DownImage;					// ���ٿ� ��ư �̹���
	_Button				m_BScrollUp, m_BScrollDown;				// ��ũ�� ��ư


	// ������ ���, ����
	OnInvestmentInfo	*m_pInvestList;							// ������ ���
	SI08				m_siInvestNum;							// ������ ��
	MONEY				m_MyInvestMoney;						// ���� ������ �ݾ�
	SI32				m_siMyInvestCanTime;					// ���� ������ �� �ִ� ��¥
	SI32				m_siMyInvestMaxPersent;					// ���� ������ �� �ִ� �ִ� �ۼ�Ʈ
	SI32				m_siMyInvestMinPersent;					// ���� ������ �� �ִ� �ּ� �ۼ�Ʈ
	MONEY				m_InvestAfterMoney;						// ���� �� ������
	MONEY				m_InvestAfterMoney_InBank;				// ���� �� ���� ���ݾ�

	// ���ڱ� ����
	BYTE				m_bGetOut;								// ���ڱ� ȸ�� Flag
	BYTE				m_GetOutTexPersent;						// ���ڱ� ȸ���� ���� Percent

	// ���� ���� ��Ȳ
	stCapital			*m_pCapitalList;						// �ü����� ����Ʈ
	SI08				m_siCapitalNum;							// �ü����� ����Ʈ ����(�⵵��)
	SI08				m_siCurCapital;							// ���� ������ �ü�����

	SI16				m_siAllInInvestmentPercent;				// ����ڰ� ���� �ִ� ������ ��ź ���� ������ % �� ���� ����

	InvestmentInfo()
	{
		m_siTalk = m_TotInvestMoney = m_TotCapital  = m_siScroll = m_siScrollPos = m_MyInvestMoney = m_siMyInvestCanTime
		= m_siMyInvestMaxPersent = m_siMyInvestMinPersent = m_bGetOut = m_GetOutTexPersent = m_siCapitalNum = m_siCurCapital = 0;
		m_pInvestList = NULL;
		m_pCapitalList = NULL;
		m_siAllInInvestmentPercent = 0;	
	}
};

struct SInterfaceOfferPeace
{
	HOEC				m_hOecMessage;	// ȭģ���� �޽��� OnlineEditControl
	_Button				m_btnSendMessage;	// ȭģ���� �޽��� ����
	_Button				m_btnCancel;		// ȭģ���� �޽��� �ۼ� ���

	RECT				m_rtEditBox;	// ȭģ���� �޽��� �ڽ��� �ڽ� ũ��

	SInterfaceOfferPeace()
	{
		m_hOecMessage = NULL;
	}
};

// ��ܰ��� ����ü��
struct GuildInfo
{
	UI08				m_uiStatus, m_uiSubStatus, m_uiSubStatus_OfferPeace/*ȭģ���� �޴� ó�� ����*/;
	XSPR				m_MainImage, m_ButImage;
	_Button				m_BOpen, m_BMemberList, m_BFlag, m_BJoinLeave, m_BClose, m_BAppoint, m_BDiffGuild;
	_Button				m_BOk, m_BCancel, m_BBack, m_BDHS, m_BHND;

	SI16				m_siCurPage, m_siMaxPage;

	// ��ũ�� ����
	SI16				m_siScroll, m_siScrollPos;
	RECT				m_rtScroll;								// ��ũ�� ����
	XSPR				m_ScrBackImage, m_ScrBarImage;			// ��ũ�ѹ� �̹���
	XSPR				m_UpImage, m_DownImage;					// ���ٿ� ��ư �̹���
	_Button				m_BScrollUp, m_BScrollDown;				// ��ũ�� ��ư
	SI16				m_siSelect;

	// ��ܼ���
	MONEY				m_GuildOpenMoney;
	//OnlineEditControl*	m_pEditBox;
	HOEC					m_hOecEditBox;

	// ����� ����Ʈ
	strCandidateInfo	*m_pWaitingList;
	UI16				m_uiWaitingNum;

	// ��ܿ� ����Ʈ
	strGuildMemberInfo	*m_pMemberList;
	UI16				m_uiMemberNum;

	// �������
	SI32				m_siFlagIndex[MAX_ONLINE_FLAG_FILE];
	SI16				m_siFlagCount;

	// �ٸ����
	strGuildInfo		*m_pGuildList;							// �ٸ���� ����Ʈ
	UI32				m_uiGuildNum;							// �ٸ���� ����
	XSPR				m_RelationImage;						// �����̹���
	UI16				m_uiRelation;							// �Ϲ�, ����, ���� ���輳��

	// robypark 2004/11/19 17:32
	_Button				m_btnOfferPeace;						// ȭģ���� ��ư
	SInterfaceOfferPeace	m_sInterfaceOfferPeace;				// ȭģ���� �޴� ���� ó��
	XSPR				m_sprOfferedPeace;						// ȭģ���Ǹ� ���� ��� ǥ�õǴ� ������ �̹���

	// ��������Ʈ
	OnGuildVillageInfo	*m_pVillageList;						// ��������Ʈ
	UI16				m_uiVillageNum;

	GuildInfo()
	{
		m_siCurPage = m_siMaxPage = m_GuildOpenMoney = m_siFlagCount = m_siScroll = m_siScrollPos = 
		m_siSelect = m_uiGuildNum = m_uiVillageNum = m_uiWaitingNum = m_uiMemberNum = 0;
		SetRect(&m_rtScroll, 0, 0, 0, 0);
		m_pGuildList = NULL;
		m_pVillageList = NULL;
		m_pWaitingList = NULL;
		m_pMemberList = NULL;
	}
};

#endif