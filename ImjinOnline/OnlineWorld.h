#ifndef _ONLINE_WORLD_H
#define _ONLINE_WORLD_H


#define SERVERINFO_TICKCOUNT					30000
#define MAX_NIGHT								8
#define	BENCH_MAXNUM							128;

#define	CPF		33				// �� �����Ӵ� Ŭ������ 40�̴�.
//#define	CPF		1			// �� �����Ӵ� Ŭ������ 40�̴�.

// �ǰ� ����� ��������, ĳ���������� ��Ÿ���� ���
#define TARGET_TYPE_CHARACTER			1			// ĳ����
#define TARGET_TYPE_VILLAGE				2			// ����

enum
{   
	ON_MAINPOLL_LOGINMGR				= 0,
	ON_MAINPOLL_INITGAME,
	ON_MAINPOLL_GAME,
	ON_MAINPOLL_FORCED_LOGOUT,
	ON_MAINPOLL_INITRECEIVEGOOUTVILLAGE,
	ON_MAINPOLL_RECEIVEGOOUTVILLAGE,
	ON_MAINPOLL_INITGOTOIMJINGAME,
	ON_MAINPOLL_GOTOIMJINGAME,				
	ON_MAINPOLL_INITRETURNONLINE,			
	ON_MAINPOLL_RETURNONLINE,
	ON_MAINPOLL_ENTERBATTLEEFFECT
};

// ������ ���� �ݰ� Ÿ�� ����� ����
#define ON_VILLAGE_ATTACK_OIL				1		// �⸧
#define ON_VILLAGE_ATTACK_STONE				2		// ��
#define	ON_VILLAGE_ATTACK_DEFENCE			3		// ���� ���
#define ON_VILLAGE_ATTACK_NONE				4		// �⺻ �ݰ�

// robypark 2004/11/1 16:53
// ������ ���� ���� ������ ���� ����Ǵ� ����/2�� ��
#define SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE	2		// ���� ȿ�� ����(5 X 5)
#define SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE	10		// ���� �Ұ����� ���� �������� �Ÿ�(����) (21 X 21)

#define SIEGEWARFARE_WASTESUPPLYGOODSTIME			1800	// ������ ������ ����ǰ �ڵ� �Ҹ� ������ �ð�(1�� == 1000)

// robypark 2005/1/8 11:43
// ������ ������ ���콺 Ŀ���� ����Ű�� �ִ� ĳ������
// ǳ�� ����(ü��, ����ǰ ������) ǥ���� �� ��ǥ�� �� % ���� ����ü
typedef struct _SSiegeWarfareUnitGaugeInfo
{
	bool		bDrawGaugeHealth;				// ü�� �������� �׸��°�?
	SI16		siDrawGaugeHealthPct;			// ���� ü�� %(10% �����̸� * 10�� ������)
	SI08		siDrawGaugeHealthPosX;			// ü�� �� ��ġ
	SI08		siDrawGaugeHealthPosY;			// ü�� �� ��ġ

	bool		bDrawGaugeSupplyGoods;			// ����ǰ �������� �׸��°�?
	SI16		siDrawGaugeSupplyGoodsPct;		// ���� ����ǰ %(10% �����̸� * 10�� ������)

	_SSiegeWarfareUnitGaugeInfo()
	{
		bDrawGaugeHealth			=	false;
		siDrawGaugeHealthPct		=	0;
		siDrawGaugeHealthPosX		=	0;
		siDrawGaugeHealthPosY		=	0;

		bDrawGaugeSupplyGoods		=	false;
		siDrawGaugeSupplyGoodsPct	=	0;
	}

} SSiegeWarfareUnitGaugeInfo, *LPSSiegeWarfareUnitGaugeInfo;

// �����Ͽ� �� �� �ִ� ������ ��
#define MAX_SIEGE_DAY			7

// robypark 2004/11/24 16:49
// ������ ���� �ɼ� ����ü
typedef struct _SOptionSiegeWarfare
{
	SI08		scSiegeDay[MAX_SIEGE_DAY];

	// ������ ���� �ð�(24�ð� ���� �ð�)
	SI16		siSIEGEWARFARETIME_BEGIN;
	// ������ ���� �ð�(24�ð� ���� �ð�)
	SI16		siSIEGEWARFARETIME_END;

	// ��ź���� ���� ó�� ���۽ð�
	SI16		siSIEGEWARFARETIME_AllInInvestment_Begin;
	// ��ź���� ���� ó�� ����ð�
	SI16		siSIEGEWARFARETIME_AllInInvestment_End;

	// ������ ���� ���� ������ ���� ����Ǵ� ����/2�� ��
	SI16		siSIEGEWARFARE_IsinAreaGeneralEffect_Range;		// ���� ȿ�� ����(5 X 5)
	SI16		siSIEGEWARFARE_IsinAreaHostileVillage_Range;	// ���� �Ұ����� ���� �������� �Ÿ�(����) (21 X 21)
	SI32		siMedicRechargeManaCoolTime;					// ���޼��� ���� 10% �ڵ� ȸ�� �ð�(��)

	SI32		siVillageCounterAttack_DefenceUp_Cost;		// ���� �ݰ� ���� ��� ���
	SI32		siVillageCounterAttack_DefenceUp_Amount;	// ���� �ݰ� ���� ��� �Ǵ� ��ġ
	SI32		siVillageCounterAttack_DefenceUp_CoolTime;	// ���� �ݰ� ���� ��� �� Ÿ��(��)

	// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� ���� �ð�
	SI16		siSiegeWarfareTime_RecruitArmy_Begin;
	// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� ���� �ð�
	SI16		siSiegeWarfareTime_RecruitArmy_End;

	// ����ڵ��� ��� ������ �Ұ����� �ð� - ����
	SI16		siGuildDisabledTimeToJoin_Begin;
	// ����ڵ��� ��� ������ �Ұ����� �ð� - ����
	SI16		siGuildDisabledTimeToJoin_End;

	// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð� - ����(����� ��ܿ� �ذ� �Ұ����ϴ�)
	SI16		siGuildDisabledTimeToWithdraw_Begin;
	// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð� - ����(����� ��ܿ� �ذ� �Ұ����ϴ�)
	SI16		siGuildDisabledTimeToWithdraw_End;

	// ������ �ɼ� ���� �⺻������ ���� - ���� ȿ�� ����
	void SetDefaultSiegeWarfareOption_IsinAreaGeneralEffect_Range(void)
	{
		siSIEGEWARFARE_IsinAreaGeneralEffect_Range	= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE;		// ���� ȿ�� ����(5 X 5)
	}

	// ������ �ɼ� ���� �⺻������ ���� - ���� �Ұ����� ���� �������� �Ÿ�
	void SetDefaultSiegeWarfareOption_IsinAreaHostileVillage_Range(void)
	{
		siSIEGEWARFARE_IsinAreaHostileVillage_Range	= SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE;		// ���� �Ұ����� ���� �������� �Ÿ�(����) (21 X 21)
	}

	_SOptionSiegeWarfare()
	{
		for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
			scSiegeDay[i] = -1;

		siSIEGEWARFARETIME_BEGIN					= 20;
		siSIEGEWARFARETIME_END						= 22;

		// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� ���� �ð�
		siSiegeWarfareTime_RecruitArmy_Begin		= 20;
		// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� �Ұ��� ���� �ð�
		siSiegeWarfareTime_RecruitArmy_End			= 23;

		// ����ڵ��� ��� ������ �Ұ����� �ð� - ����
		siGuildDisabledTimeToJoin_Begin				= 17;
		// ����ڵ��� ��� ������ �Ұ����� �ð� - ����
		siGuildDisabledTimeToJoin_End				= 24;

		// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð� - ����(����� ��ܿ� �ذ� �Ұ����ϴ�)
		siGuildDisabledTimeToWithdraw_Begin			= 17;
		// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð� - ����(����� ��ܿ� �ذ� �Ұ����ϴ�)
		siGuildDisabledTimeToWithdraw_End			= 24;

		siSIEGEWARFARETIME_AllInInvestment_Begin	= 22;
		siSIEGEWARFARETIME_AllInInvestment_End		= 24;

		// ������ ���� ���� ������ ���� ����Ǵ� ����/2�� ��
		siSIEGEWARFARE_IsinAreaGeneralEffect_Range	= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE;		// ���� ȿ�� ����(5 X 5)
		siSIEGEWARFARE_IsinAreaHostileVillage_Range = SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE;	// ���� �Ұ����� ���� �������� �Ÿ�(����) (21 X 21)
	}

} SOptionSiegeWarfare, *LPSOptionSiegeWarfare;

// ���� �� ���� ��� �޽��� ������
#define WARNINGMSGINSIEGEWARFARE_DELAY			300000		// 5�� 1�ʴ� 1000

// robypark 2005/1/29 13:17
// ������ �ð��� ���� ���� ���� ��ó������ �Ϲ� ������ �ʺ��� ������ ���� ���
typedef struct _SWarningMsgInSiegeWarfare
{
	DWORD			m_dwLeftTime;		// 5�� ���� ������ִµ�, ���� �ð�

	_SWarningMsgInSiegeWarfare()
	{
		m_dwLeftTime = 0;
	}

} SWarningMsgInSiegeWarfare, *LPSWarningMsgInSiegeWarfare;

// ���� �ڵ� ���� �ð�
#define RECOVERY_MANA_DELAY				10000

// robypark 2005/1/29 13:56
// ���޼���(�޵�)�� ��� 10�ʸ��� ������ 10%�� ȸ���ȴ�.
typedef struct _SRecoveryMana
{
	bool			m_bSendedRequest;		// ������ 10% ���� ������ ��û�߳�?

	DWORD			m_dwLeftTime;			// 10�� ���� 10% ���� �����ε� ���� �ð�

	_SRecoveryMana()
	{
		m_bSendedRequest = FALSE;
		m_dwLeftTime	 = RECOVERY_MANA_DELAY;
	}

} SRecoveryMana, *LPSRecoveryMana;

// robypark 2004/11/13 13:36
// ����ڰ� ������ �����̸�(���� ���� ���� ���� ���̱� ��ư ��밡���� ���),
#define ON_GAMEOPTION_SIEGEWARFAREUNIT								2
// ���� ���� ���� ���� ���� ���̱�
#define ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT			4


// ������ ���� ����Ʈ �迭 �ε���
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN		0		// ���� �˺�
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN		1		// ���� â��
#define ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN		2		// ���� �⸶��
#define ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN		3		// ���� �ú�
#define ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER	4		// ���� ����
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER		5		// ���� ����
#define ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT		6		// ���� �߼���
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON	7		// ���� ���޼���

// robypark 2005/1/17 10:53
// ������ �߿� �ǳ��� ���� ó���� ���� ��� ����
#define SIEGE_TRANSFORM_MODE_NONE					0		// ������ �ǳ����� ���Ͽ� �ƹ��� �۾��� ���ߴ�.
#define SIEGE_TRANSFORM_MODE_REFUGEE				1		// ������ �ð��� ��, ��� ���ΰ� ������ �ǳ������� �������. SIEGE_TRANSFORM_MODE_HERO => SIEGE_TRANSFORM_MODE_REFUGEE
#define SIEGE_TRANSFORM_MODE_HERO					2		// ������ �ð��� �ƴ� �� ��� �ǳ��� ������ ������� �����ߴ�. SIEGE_TRANSFORM_MODE_REFUGEE => SIEGE_TRANSFORM_MODE_HERO

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

#define	ON_GAMEPOLL_FIELD						1
#define	ON_GAMEPOLL_VILLAGE						2	
#define	ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE		3	// �����ϱ� �������̽� ���� ó�� ��...

#define ON_BUY_CREDIT							5

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4018 )


// #include <crtdbg.h>
#include "charorg.h"
#include "OnlineResource.h"
#include "IOnlineCharKI.h"
#include "IOnlineShipKI.h"
#include "IOnlineChar.h"
#include <OnlineFindPath.h>
#include "IOnlineMouse.h"
#include "OnlineMousePointInfo.h"
#include "OnlineMousePoint.h"

#include "IOnlineObject.h"			// OnlineWorld�� �Ѿ�� ��.
#include "IOnlineObjectKI.h"
#include <OnlineVillageDescriptList.h>
#include <OnlineFieldEffect.h>		// OnlineWorld�� �Ѿ�� ��.
#include <Button.h>
#include "OnlineClient.h"			
#include "OnlineRPI.h"
#include "GuildClassParser.h"
#include "IGuildClass.h"
#include "slanderfilter.h"
#include "ReadIP.h"
#include <DDraw.h>
#include "OnlineMemoryProtected.h"
#include "LoginData.h"
#include "OnlineGameOption.h"
#include "macrocheckprocess.h"
#include "OnlineFieldEffect_SiegeWarfare_BulletMgr.h"
#include "OnlineFieldEffect_SiegeWarfare_SmokeMgr.h"

#define		GETSCREENSURFACE(pOnlineWorld)	((pOnlineWorld)->pOnlineDirectDraw)

class	OnlinePortrait;
class	OnlineMercenary;
class	OnlineQuestInterface;
class	OnlineChatMacro;
class   OnlineNPCTalk;
class	OnlineMsgProc;
class	OnlineBooth;
class	HQClientSocket;
class	HQPacket;
class	cltIProfit;
class	OnlineTip;
class   OnlineParty;
class	OnlineHelp;
class   OnlineConfig;
class	OnlineMegaText;
class	OnlineNumberControl;
class	OnlineTradeBook;
class	OnlineVillageSave;
class	OnlineBuilding;
class	OnlineItemList;
class	OnlineTrade;
class	OnlineDungeonParser;
class	OnlineTicketParser;
class   OnlineFonts;
class   OnlinePannel;
class   OnlineNoticeList;
class   OnlineEditControl;
class	OnlineKeyboard;
class	MyData;
class	OnlineVillage;
class	VillageStructManager;
class	OnlineEconomy;
class	OnlineVillageBaseStructure;
class   OnlineChatList;
class	CItem;
class	OnlineText;
class   OnlineMercenaryParser;
class	OnlineFieldChat;
class	OnlineMiniMap;
class	OnlineMsgBox;
class	OnlineMap;
class	OnlineMapSound;
class	OnlineFieldArea;
class	OnlineInventory;
class	OnlinePaletteManager;
class	OnlineImoticon;
class	OnlineVillageManager;
class	OnlineCharActionCode;
class	OnlineIME;
class	OnlineCharUI;
class	CCharFileLoadManager;
class   OnlineNotice;
class	OnlineTradeParser;
class	OnlineSkillParser;
class	OnlineGeneralParser;
class	OnlineMessenger;
class	OnlineQuestParser;
class	OnlineQuestPlay;
class	CWordSearchTree;
class	OnlineSetting;
class	CBuriedItemParser;
class   OnlineFieldAuction;
class   OnlineReception;
class	OnlineSmith;
class	OnlineSmithParser;
class	OnlineLevelMarkParser;
class	MemMgr;
class   CMacroCheckProcess;

class	CFieldHeroInfoMgr;

class	CSiegeUnitTableMgr;

//�ؼ����� �߰��κ� 
//class	JSWndMgr;
//class	JSWnd;

// actdoll (2004/01/13 22:28) : ����׿� Ŭ���� �߰� ����
class	CDebugDisplay;

struct	MyCharacterInfo;
struct	SAVEFILE_SORT;


// �����ϱ� �������̽� ���� ���� ����ü
typedef struct _SVillageDefenceInterface
{
	// ���� �������̽� ��� ����� �� �־����� ������
	SI32			m_siCurrentDelayTimeUsingVillageAttackType;
	SI32			m_siMaxDelayTimeUsingVillageAttackType;

	UI16			m_uiVillageCode;			// ���� �ڵ�

	_SVillageDefenceInterface()
	{
		m_siCurrentDelayTimeUsingVillageAttackType	= 0;
		m_siMaxDelayTimeUsingVillageAttackType		= 0;
		m_uiVillageCode								= 0;
	}

	// ����� �����ð� ����
	void SetPastDelayTimeUsingVillageAttackType(DWORD siPastDelayTime)
	{
		m_siCurrentDelayTimeUsingVillageAttackType -= siPastDelayTime;

		if (m_siCurrentDelayTimeUsingVillageAttackType < 0)
			m_siCurrentDelayTimeUsingVillageAttackType = 0;
	}

	// ���� ���� �ð� ����
	void SetDelayTimeUsingVillageAttackType(SI32 siDelayTime)
	{
		m_siCurrentDelayTimeUsingVillageAttackType = m_siMaxDelayTimeUsingVillageAttackType = siDelayTime;
	}

} SVillageDefenceInterface, *LPSVillageDefenceInterface;

// robypark 2005/1/12 10:33
// �ڽ��� ������ ������ �� ���� ��� ��� ��
#define SIEGEWARFARE_ATTACKMODE_NONE		0		// ���� ���� Ÿ���� ����.
#define SIEGEWARFARE_ATTACKMODE_CHARACTER	1		// ���� ���� Ÿ���� �ٸ� ���� �����̴�.
#define SIEGEWARFARE_ATTACKMODE_VILLAGE		2		// ���� ���� Ÿ���� �� �����̴�.

// robypark 2005/1/12 10:28
typedef struct _SSiegeWarfare_AttackMode
{
	SI08		scAttackMode;		// �ڽ��� ������ ������ �� ���� ���� Ÿ���� �����ΰ��� ��Ÿ���� ��
									// SIEGEWARFARE_ATTACKMODE_NONE, SIEGEWARFARE_ATTACKMODE_CHARACTER, SIEGEWARFARE_ATTACKMODE_VILLAGE

	SI32		siAutoAttackTimes;	// �ڵ� ���� Ƚ�� �����ϱ�

	// ���� ��� ID
	union
	{
		UI32	uiVillageAttackVillageCode;	// Village Attack Village Code
		UI16	uiCharacterAttackUniqueID;	// Character Attack Unique ID
	};

} SSiegeWarfare_AttackMode, *LPSSiegeWarfare_AttackMode;

//-----------------------------------------------------------------------
BOOL	InitOnlineWorld();
UI16	OnlineWorld();


struct	OnlineReturnOnline
{
	DWORD					dwRequestGameDataTime;								// ���� ����Ÿ�� �䱸�� �ð�
};

struct OnlineNumPad
{
	OnlineNumberControl		*pNumberControl;									// ������	
	__int64					siCount;											// ���� �Ǵ� ��
	SI32					fDisable;											// Prepare�� ������ ��츦 ó���ϱ� ���� �ʿ���
};

struct OnlineBench
{
	UI16					m_uiBoothItemID[5];									// �������� �ִ� ������ �Ƶ�
};

struct OnlineVillageInfoData
{
	UI16					m_uiVillageCode;
	SI16					m_siFrame;
	OnRecvVillageInfo		m_VillageInfo;

	SI16					m_siVillageSeriesWait;

	VillageHeader*			m_lpVillageHeader;
};

class	cltOnlineWorld
{
private:
	// robypark 2004/9/9 15:30
	BOOL				m_bIsInEventDay;			// ���� �̺�Ʈ �Ⱓ������ ��Ÿ���� �÷���

	// robypark 2005/1/8 11:18
	SI32				m_siRestTimeAutoSpendSupplyGoods;	// ����ǰ �ڵ� �Ҹ� ���� �����ð�

	// robypark 2005/1/8 11:43
	// ������ ������ ���콺 Ŀ���� ����Ű�� �ִ� ĳ������
	// ǳ�� ����(ü��, ����ǰ ������) ǥ���� �� ��ǥ�� �� % ���� ����ü
	SSiegeWarfareUnitGaugeInfo		m_sSiegeWarfareUnitGaugeInfo;

	// robypark 2005/1/14 17:11
	// ���ΰ� ĳ���͵��� ���� ������
	CFieldHeroInfoMgr	*m_pFieldHeroInfoMgr;

	// robypark 2005/1/17 10:53
	// ������ �߿� �ǳ��� ���� ó���� ����
	SI08				m_scSiegeTransformMode;

	// robypark 2005/1/29 13:25
	// ������ �� ���� �� �� �ִٴ� ��� �޽��� ó�� ����
	SWarningMsgInSiegeWarfare		m_sWarningMsgInSiegeWarfare;

	// ���޼���(�޵�)�� ��� 10�ʸ��� ������ 10%�� ȸ���ȴ�.
	SRecoveryMana					m_sRecoveryMana;

	// robypark 2005/1/31 12:55
	// ���� ���� ������
	CSiegeUnitTableMgr				*m_pSiegeUnitTableMgr;

public:	
	SI32				g_PingPerUserCount[2];
//	SConnectServerInfo	*m_LogInInfo_Data;
	BOOL				bNotice;

	BOOL				fLeftMemoryUpSwitch;		// ���콺 Up Flag ���
	BOOL				fRightMemoryUpSwitch;		// ���콺 Up Flag ���

	BOOL				fLeftUpSwitch;				// ���콺 Up Flag
	BOOL				fRightUpSwitch;				// ���콺 Up Flag
	BOOL				fLeftOneSwitch;				// ���콺 One Flag
	BOOL				fRightOneSwitch;			// ���콺 One Flag
	BOOL				fVillageLeftOneSwitch;		// ������ ���콺 One Flag
	BOOL				fVillageRightOneSwitch;		// ������ ���콺 One Flag
	BOOL				bPay;						// ����
	BOOL				bCharMove;					// ĳ���� �̵��ϳ� ����
	BOOL				bReturnKeyDown;				// ����Ű ���ȳ� üũ
	
	__int64				dwMapTileDrawTime;			// Map Tile Draw Time
	SI32				siMapTileDrawTimeAddTime;	// Map Tile Draw Add Time
	__int64				siMapTileDrawTimeAddCount;	// Map Tile Draw Add Count


	DWORD				m_WaitTickCount;			// Logout WaitDelay TickCount
	DWORD				m_ImageFreeTimer;

// ��̷�..
	int                 m_siUpdateMethod;           
	int                 m_siUpdateCounter;

private:	
	BOOL				fYetLeftPressSwitch;		// ������ ���콺 Press Flag
	BOOL				fYetRightPressSwitch;		// ������ ���콺 Press Flag

	float				fYetLight;					// ���� ���϶� ��ξ��� ����

	SI16				siOnlineMajorVersion;		// �¶��� ����(������)
	SI16				siOnlineMinorVersion;		// �¶��� ����(���̳�)
	CHAR				szOnlineVersion[32];		// ���ڿ� �¶��� ����.

	UI32				uiTotalFrame;					// ��ü ������ ��.
	DWORD				dwCurrentFrameStartTime;		// ���� �������� ���۵� �ð�	
	DWORD				dwCurrentFrameAddTime;			// ���� �������� ���� �ð�	
	SI16				dwCurrentFrameSkippingCount;	// ���� ������ ��ŵ�μ�
	SI32				siFPS;							// �ʴ� ������ ��

	SI32				siMainPollStatus;			// ���� �ܺο����� �� ����.
	SI32				siGamePollStatus;			// ���� ���ο����� �� ����.
	
	SI32				siLogOutStatus;				// �α׾ƿ� �Ҷ��� ����.
	SI32				siNextMenuAfterLogOut;		// �α׾ƿ��ϰ� �� �Ŀ� �̵��� �޴�.
	SI32				siNextPollAfterLogOut;		// �α׾ƿ��ϰ� �� �Ŀ� �̵��� �޴�.
	BOOL				bWillGoGameServer;			// ���� ������ �̵��ϴ°�?
	BOOL				bExitOnlineGame;			// ������ ���� �Ǵ°�.
	RECT				rcPannel;					// �ǳ�.
	BOOL				ShortKey;					// ����Ű
	BOOL				bFirstLogIn;
	BOOL				bServerSelect;
	SI16				siTalkDealy;
	SI16				m_siNoField;
	POINT				BenchMousePosition;			// ���Ǿȿ� ������ ���콺 Ŭ����ġ

	BOOL				m_iMinPlaySoundTick;

	DWORD				m_GameTickCount;			// Game TickCount
	DWORD				m_GameTime;					// Game Time
	DWORD				m_dwGameGuardCheckTick;

	DWORD				m_HackGameTickCount;		// Hack Game TickCount
	DWORD				m_HackGameTime;				// Hack Game Time
	SI32				m_TickTimeDiff;

	SI08				m_siNationCode;

	DWORD				m_LastNowUserConnectMessageTickCount;			// ���������� ���� ������ User���� �޼����� ���� �ð�
	SI32				m_LastNowUserConnectMessageNowUser;				// ���������� ���� ������ User���� �޼����� ���� �߿��� ���� ������ ��
	SI32				m_LastNowUserConnectMessageErrorOutPutCount;	// ���������� ���� ������ User���� �޼����� ���� �߿��� ������ ����� ������ ��¼�

//	UI32				m_uiDokdoQuestCompleteUserCount;				// actdoll (2004/03/30 11:01) : = �̺�Ʈ = ��������Ʈ ���� ������

	OnlineLogInMgr					*pOnlineLogInMgr;					// �α��� ���� �޴���
	OnlineLoadingData				*pOnlineLoadingData;				// �ʱ� �ε�

	OnlineReturnOnline				*pOnlineReturnOnline;				// ������ �����ϰ� Online���� �����Ѵ�.
	OnlineBench						*pOnlineBench;						// ����


//	CMacroCheckProcess				*m_pMacroProcess;					// ��ũ�� ����� ���μ���

	// robypark 2004/11/24 16:51
	SOptionSiegeWarfare				m_SOptionSiegeWarfare;				// ������ �ð� ���� �ɼ� ���� ����ü
	
public:	
	OnlineVillageInfoData			*pOnlineVillageInfoData;			// ���� ����

	UI16					BackgroundMusic;					// ��� ������ ���̵�

//	BOOL					m_LanguageMode;						// �����
	ELanguageCode			m_LanguageCode;

	UserGrade				m_GMFlag;							// ��� Flag
	BOOL					m_InfoFlag;							// ���� Flag
	BOOL					m_ShowMessagBoxCharInfoFlag;
	OnlinePaletteManager*	pPaletteManager;					// Palette Manager
	SI16					PaletteDataRed;						// Palette Data Red
	SI16					PaletteDataWhiteRed;				// Palette Data White Red
	SI16					PaletteDataWhiteBlue;				// Palette Data White Blue
	SI16					PaletteDataYellow;					// 
	SI16					PaletteDataGreen;					// Palette Data Green
	SI16					PaletteDataBlack;					// Palette Data Black
	COLORREF				PaletteWhiteColor;					// Palette White Color
	SI16					PaletteWhiteColorIndex;				// Palette White Color Index
	BYTE					PaletteNight[MAX_NIGHT][768];		// Palette ��, ���� ���õ� Palette

	// robypark 2005/1/12 10:55
	// ������ ���� ��� ���� ����
	SSiegeWarfare_AttackMode	m_sSiegeWarfare_AttackMode;

	CWordSearchTree			m_SlanderFilter;			// �弳����

	OnlineGameOption*		m_GameOption;				// Game Option

	OnlineTicketParser*		pTicketParser;				// Ticket Parser
	OnlineGeneralParser*	pGeneralParser;				// General Parser
	OnlineGeneralParser*    pGeneralParserEx;			// General ParserEx
	OnlineQuestParser*		pQuestParser;				// Quest Parser
	OnlineLevelMarkParser*	pLevelMarkParser;			// Level Mark Parser

	POINT					YetMousePosition;			// ������ ���콺 ��ġ 
	COnlineTimer			*pTimer;					// Timer
	BOOL					fFirstGetTime;				// ó������ �������� �ð��� �޾Ҵ��� �˻��ϴ� Flag
	OnRealWorldTime			m_RealTime;					// ���� �ð�

	BOOL					bPeace;						// ��ȭ ��� �ΰ�?	
	SI32					m_VillageAttackDelay;		// Village Attack Delay

	OnlineMercenaryParser*	pOnlineMercenaryParser;		// ����Ҽ� �ִ� �뺴�� Data
	OnlineEconomy			*pOnlineEconomy;			// ���忡 ���� Data
	IOnlineCharKI			*pIOnlineCharKI;			// ĳ���� ������ �������̽�.
	IOnlineChar				*pIOnlineChar;				// ĳ���� �������̽�
	IOnlineObjectKI			*pIOnlineObjectKI;			// ������Ʈ ������ �������̽�	
	IOnlineObject			*pIOnlineObject;			// ������Ʈ�� �������̽�.	
	IOnlineShipKI			*pIOnlineShipKI;			// �� ������ �������̽�.
	OnlineMap				*pOnlineMap;				// �� ��ü
	OnlineMapSound			*m_pOnlineMapSound;
	OnlineCharParser		*pOnlineCharParser;

	BOOL					m_fChattingRoom;
	OnlineVillage			*pOnlineVillage;			// ���� ��ü.
	SI16					m_VillageInStruct;

	OnlineVillageDescriptList*	pOnlineVillageDescriptList;	// ���� ���� List
	OnlineNoticeList*		pOnlineNoticeList;			// �������� List
	OnlineFieldEffect*		pOnlineFieldEffect;			// �ʵ�� Effect ó��
	
	cltOnlineFindPath		*pclFindPath;				// ��ã�� �ΰ����� ��ü

	MyData					*pMyData;					// ���� ����Ÿ��.

	OnlineImoticon			*pOnlineImoticon;			// �̸�Ƽ��
	

	OnlineClient			*pOnlineClient;					// �¶��� Ŭ���̾�Ʈ ��ü.
	HQClientSocket			*pOnlineClientChatting;			// �¶��� Chatting Ŭ���̾�Ʈ ��ü.
	OnlineResource			*pOnlineResource;				// �¶��� ���ҽ� ��ü.

	OnlineCharUI			*pOnlineCharUI;				// ���� ĳ���� ������ ���� ��ü.

	OnlineInventory			*pOnlineInventory;			// �κ��丮 â.
	OnReqThrowItemMsg		OnlineTempOnReqThrowItemMsg;
	OnReqPickUpItemMsg		OnlineTempOnReqPickUpItemMsg;

	OnlineMiniMap			*pOnlineMiniMap;			// �̴ϸ�

	OnlineFonts				*pOnlineFonts;				// �¶��ο��� ����ϴ� ��Ʈ���� ��� ���� ��ü.
	OnlineText				*pOnlineText;				// �¶��ο��� ����ϴ� ��� �ؽ�Ʈ�� ��� ���� ��ü.
	CItem					*pItemsInfo;				// �¶��ο��� ����ϴ� ��� �������� ��� ���� Ŭ����.
	OnlineKeyboard			*pOnlineKeyboard;			// Ű���带 ó���ϴ� ������Ʈ.	

	LPDIRECTDRAWSURFACE7	pOnlineDirectDraw;			// �¶��ο��� ����ϴ� ���̷�Ʈ��ο� ��ü.	

  
	OnlineConfig				*pConfig;				// Config ���� ���� Ŭ����	
	OnlineMegaText				*pOnlineMegaText;		// Text ����,	
//	OnlineCharAni				*pOnlineCharAni;		// ��� ĳ���͵��� �ִϸ��̼� ������ �����ϰ� �ִ� ��ü.
	OnlineCharActionCode		*pOnlineCharActionCode;	// ĳ������ ���ڿ� �׼� �ڵ带 ���� �׼� �ڵ�� ��ȯ�� �ִ� ��ü.

	IOnlineMouse				*pIOnlineMouse;			// �¶��ο��� ����ϴ� ��� ���콺�� �����ϴ� ��ü.
	OnlineFieldArea				*pOnlineFieldArea;		// �ʵ� ������ �����ϴ� Ŭ����.	
	OnlineVillageManager		*pOnlineVillageManager;	// ���� ���� Ŭ����.

	OnlinePannel				*pOnlinePannel;			// �¶��� �ǳ�.
	OnlineHelp					*pOnlineHelp;			// ���� ���

	OnlineMousePointInfo		*pMousePointInfo;		// ���콺�� ����Ű�� �ִ� ���� ���� ����.
	OnlineMousePoint			*pOnlineMousePoint;		// Mouse Point
	OnRPI						RPI;					// RequeestPlayerInfo
	OnlineNumPad				*pOnlineNumPad;			// ������
	OnlineMsgBox				*pOnlineMsgBox;				// �޼��� �ڽ�
	OnlineMsgBox				*pOnlineMsgBoxPay;			// �޼��� �ڽ�(�� ����) ���� ���� ���� ���, ����ü� �ȷ������� ���, ��� ��ü, ��� �� ���� ����, ���� �޽��� �ڽ��� ���ȴ�.
	OnlineMsgBox				*pOnlineMsgBoxUsePortal;	// �޼��� �ڽ�(��Ż�� ���ڳ�?)
	OnlineMsgBox				*pOnlineMsgBoxError;		// �޼��� �ڽ�(����)
	OnlineMsgBox				*pOnlineLoginMsgBox;		// �α��θ޼��� �ڽ�.
	SI32						UsePortalType;			// Portal ��� Ȯ�ν� ����Ŵ� Portal
	OnlineTradeBook				*pOnlineTradeBook;		// ���
	OnlineVillageSave			*pOnlineVillageSave;	// ��ο��� ������ ���� ������ �а� ����.
	OnlineBuilding				*pOnlineBuilding;		// ���� �ü� ���� �ļ�

	CHAR						m_TempBuffer[4096];		// �������� ���� ���� ����.
	SI16						NextMapIndex;			// ���� �̵��ҷ��� Portal
	CCharFileLoadManager		*pCharFileLoadManager;	// ĳ���� ������
	OnlineTrade					*pOnlineTrade;			// �ŷ�
	BOOL						bFindTrade;				// ��� �˻�
	SAVEFILE_SORT				*pTempSearch;
	SI16						siTempSearchNum;
	OnlineBooth					*pOnlineBooth;			// ����
	BOOL						bTempFlag;
	SI16						m_siMsgBoxStatus;

	cltIGuildClass				m_cltIGuildClass;		// ��� ���޸�
	OnlineTip					*pOnlineTip;			// TIP ���
	OnlineQuestPlay				*pOnlineQuestPlay;		// Quest Play ���
	OnlineParty					*pOnlineParty;
	OnlineMessenger				*pOnlineMessenger;
	OnlineDungeonParser			*pOnlineDungeon;		// Ư������ �ļ�
	cltIProfit					*pIProfit;
	_Char						*pChar;
	OnlineTradeParser			*pOnlineTradeParser;
	OnlineSkillParser			*pOnlineSkillParser;
	OnlineSetting				*pOnlineSetting;		// ȯ�漳��
	CBuriedItemParser			*pCBuriedItemParser;

//	OnlineBanner				*pOnlineBanner;

	OnlineMsgProc				*pOnlineMsgProc;

	CMemoryProtected*			pMemoryProtected;

	// ���
	OnlineReception             *pOnlineReception;
	OnlineFieldAuction          *pOnlineFieldAuction;

	OnlineSmith					*pOnlineSmith;			// ��������
	OnlineSmithParser			*pOnlineSmithParser;

	MemMgr						*pOnlineMemMgr;
//	JSWndMgr					*pJSWndMgr;
//	JSWnd						*pJSTestWnd;
//	HMODULE						m_IME_DLLHandle;

	SI32						m_SelectCharacterIndex;
	SI16						m_SelectCharacterNameDrawX;
	SI16						m_SelectCharacterNameDrawY;

	BOOL						m_BattleAfterMiniMapFlag;
	BOOL						m_bForcedLogout;

	char						m_MemoUser[32];
	SI16						m_siServiceType;

	SI16						m_CharPosition_X;		// ������ Ž�� ��û�� ĳ����ġ ���
	SI16						m_CharPosition_Y;

	// actdoll (2004/04/02 15:28) : = �̺�Ʈ = ���� ����Ʈ ����
//	BOOL						m_bEventDokdoQuestPlay;	// ���� ����Ʈ�� �������ΰ�?
//	RECT						m_rtIconDokdoQuest;		// ���� ����Ʈ �������� ����
//	XSPR						m_Spr_Icon_DokdoQuest;	// ���� ����Ʈ�� ������ �̹���

	BOOL						m_bReceivedItem;		// ������ �������� �ִ���...
	RECT						m_rtReceivedItem;		// ������ ������ �̹�������
	XSPR						m_ReceivedItemAniSpr;
	SI16						m_siAniNoField;

	BOOL						m_bItemDraging;			// ���� ������ �巡����
	DWORD						m_dwDelay;
	BOOL						m_bReceivdeItemShow;	// �����Ÿ� ǥ���ϱ����� ����
	SI16						m_siReceivedItemCount;
	SI16						m_siInnStatus;			// �ָ������� ����(�ޱ���:1, ��������:2)
	BOOL						m_bMiniGameOn;			// ���� �̴ϰ��� ������
	BOOL						m_bGuildOn;				// ��� ä��ä���� �������.

	CHAR						m_szProgramList[200][255];
	UI16						m_uiProgramNum;
	CHAR						szURLBuffer[128];
	char						m_szCharTextList[10][256];

	BOOL						m_bFollowerSelLock;		// �뺴���ý� ����Ű 1~10�� ��밡������ �ƴ���
	BOOL						m_bEvent;
	BOOL						m_bQuestState;

	SI16						m_siFieldHp;

	OnlineNPCTalk				*m_pNpcTalk;			// NPC �ڵ� ���ϱ�
	OnlineChatMacro				*m_pChatMacro;			// ü�� ��ũ��
	OnlineQuestInterface		*m_pQuestInterface;		// ����Ʈ �������̽�
	OnlineMercenary				*m_pMercenary;			// �뺴â

	// actdoll (031229)			�߰�������
	OnlinePortrait				*m_pPortrait;			// �ʻ�ȭ ��Ʈ�ѷ�

	// robypark 2004/10/25 13:26
	// ������ ���� ȿ�� ���� ���� �߰�
	COnlineFieldEffect_SiegeWarfare_BulletMgr		m_SiegeWarfare_BulletMgr;		// ������ �Ѿ�
	COnlineFieldEffect_SiegeWarfare_SmokeMgr		m_SiegeWarfare_SmokeMgr;		// ������ ȿ��(Smoke)

	// robypark 2004/11/13 12:24
	// ������ ������ ��� ������ ������ ����� �̸�, ü��, ����ǰ, ���� �� �� �ִ����� ��Ÿ���� �÷���
	SI08						m_scIsShowInformationAboutSiegeWarfareUnit;

	// robypark 2004/12/10 18:22
	// �����ϱ� �������̽� ���� ����
	SVillageDefenceInterface	m_sVillageDefenceInterface;

public:	
	cltOnlineWorld();								// ������.
	~cltOnlineWorld();								// �Ҹ���.
  
	BOOL	Init();									// �ʱ�ȭ.	
	VOID	Initialize();							// �ʱ�ȭ.	
	VOID	DrawTextInLoading(CHAR *pszString);		// �ε��߿� �ؽ�Ʈ�� ����Ѵ�.
	VOID	Free();									// ����. --> �¶��λ󿡼� ���� ��� ���� �����ϴ� �Լ�
	UI16	Poll();

	VOID	ScrollLoading( UI32 uiLoadPercent );
	
	// actdoll (2004/11/15 18:43) : ��Ʈ�� ȭ�� �߰�
	void	IntroPoll				( void );
	void	IntroInit				( void );
	void	IntroFree				( void );
	void	IntroPoll_FadeIn_Ready	( void );
	void	IntroPoll_FadeIn		( void );
	void	IntroPoll_Draw			( void );
	void	IntroPoll_FadeOut_Ready	( void );
	void	IntroPoll_FadeOut		( void );
	void	IntroPoll_Finish		( void );

	// �α��� �޴���
	VOID	InitLogInMgr();
	UI16	LogInMgrPoll();
	VOID	DrawLogInMgr();
	VOID	FreeLogInMgr();
	BOOL	RecvPWData();

	// �������, ��������
	VOID	InitAgreeNotice(BOOL blicense);
	SI08	AgreeNotice();
	VOID	DrawAgreeNotice();
	VOID	FreeAgreeNotice();

	// ��������
	VOID	InitSelectServer();
	SI08	SelectServer();
	VOID	DrawSelectServer();
	VOID	FreeSelectServer();
	VOID	ScrollAction();

	// ���� �α���
	VOID	InitAccountLogIn();
	SI08	AccountLogIn();
	VOID	DrawAccountLogIn();
	VOID	FreeAccountLogIn();

	// ĳ���� ����
	VOID	InitSelectCharacter();
	SI08	SelectCharacter();	
	VOID	DrawSelectCharacter();
	VOID	FreeSelectCharacter();

	// ĳ���� ����
	VOID	InitCreateCharacter();
	SI08	CreateCharacter();	
	VOID	DrawCreateCharacter();
	VOID	FreeCreateCharacter();
  
	// ���� ����
	VOID	InitGameServerLogIn();
	SI08	GameServerLogIn();						
	VOID	DrawGameServerLogIn();
	VOID	FreeGameServerLogIn();

	/////////////////////////////////////
	
	VOID	InitGamePoll();							// ���� Poll() �ʱ�ȭ
	UI16	GamePoll();								// ���� Poll()	

	UI16	FieldPoll(UI16 uiClientRet);				 					// ĳ���Ͱ� �ʵ�� ������ ȣ���ϴ� �Լ�.
	VOID	FieldShortCutKey();												// �ʵ� �󿡼��� ����Ű�� ó���Ѵ�.
	VOID	FieldDraw();													// �ʵ忡�� ȭ�� ��ºκ��� ó���Ѵ�.
	VOID	FieldDrawPannel();												// �ʵ忡�� �ǳ��� ����Ѵ�.		
	VOID	InitFieldDrawPannel();					// �ǳ��� �ʱ�ȭ �Ѵ�.
 
	UI16	VillagePoll(UI16 uiClientRet);			// ĳ���Ͱ� ������ ������ ȣ���ϴ� �Լ�.
	UI16	MapEditotrPoll();						// Map Eidtor Poll �Լ�.			
	VOID	InitReceiveGoOutVillage();				// ������ ������ ���� �ʱ�ȭ
	UI16	ReceiveGoOutVillage();					// ������ ������ ���� �۾�
	VOID	InitConnectServerSelect();				// ������ ���� ����.(�ʱ�ȭ)

	// ������ �����ϱ� �������̽� ���� OnlineWorld-VillageDefenceInterface.cpp (ĳ���Ͱ� �ʵ忡 �ִ� ��ó�� ó���Ѵ�. ��, �ʵ��Ȳ�� �� ���� �ִ�.)
	UI16	VillageDefenceInterfacePoll(UI16 uiClientRet);					// �����ϱ� �������̽� ���� ��
	VOID	VillageDefenceInterfaceDraw(VOID);								// �����ϱ� �������̽� �� �ʵ� �׸���
	VOID	InitVillageDefenceInterface(VOID);								// �����ϱ� �������̽� �ʱ�ȭ
	VOID	FreeVillageDefenceInterface(VOID);								// �����ϱ� �������̽� ����
	UI16	OnResponseEnterVillageDefenceInterface(BYTE *pMsg);		// ON_RESPONSE_WAR_VILLAGE_DEFENCE, �����ϱ� �������̽��� ���� ��û�� ���� ���� ó��
//	UI16	OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg);		// ON_RESPONSE_WAR_VILLAGE_IN, �����ϱ� �������̽����� ������ ���� ��û�� ���� ���� ó��
	
	// �����ϱ� �������̽��� ���´�. �ʵ��
	// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
	UI16	OnResponseLeaveVillageDefenceInterface(BYTE *pMsg);

	UI16	OnResponseVullageAttackTypeCheck(BYTE *pMsg);	// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK, �����ϱ� �������̽����� ���� �Ӽ� ���� �ʱ�ȭ ���� ���

			       
	SI32	GetDirection(CHAR *pszDirection);														// ���ڿ��� ������ ������ ���ڷ� �Ǿ� �ִ� ������ ���´�.
	SI32	GetDirection8(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2);								// ������ ���Ѵ�.
	UI32	GetTotalFrame();																		// �����Ӽ��� ���ؿ´�.

	VOID	UpdateScreen(LPDIRECTDRAWSURFACE7 pSurface = NULL, BOOL bSourceColorKey = FALSE,BOOL bForceRedraw = FALSE);		// ���ǽ��� �����̸Ӹ� ���ǽ��� ���� ��Ų��.	

	VOID	GetLatitudeLongitude(SI32 siX, SI32 siY, SI32 *psiLongitude, SI32 *psiLatitude, SI32 *psiLongitudeDegree, SI32 *psiLatitudeDegree);		// Ÿ�� ��ǥ�� ����Ͽ� ������ �浵�� ���´�.	
	UI16	ReceiveMessageProc(UI16 uiReceivedMessage);												// ������ ���� ���� �޽����� ó���Ѵ�.	
	UI16	ReceiveMessageProcChatting(HQPacket* lpPacket);											// Chatting������ ���� ���� �޽����� ó���Ѵ�.	

	VOID	ShowSystemCursor(BOOL bShow);															// �ý��� ���콺�� ���̰� ���ش�.
	VOID	SetLogOut(SI32 siNextMenu, SI32 siNextPoll, BOOL bGoGameServer = FALSE);				// �α׾ƿ��� �ϰ� �������� �̵��� �޴��� �����Ѵ�.
	
	VOID	FreeForGotoImjinGame();																	// ������2 Game ���� ���� ���Ͽ� �����ؾ� �� �͵��� �����Ѵ�.	

	VOID	InitReturnOnline();																		// ������2 Game���� ������ �¶������� ���� ���ؼ� �����ߴ� �޸𸮸� �ٽ� �Ҵ� �޴´�.
	UI16	ReturnOnline();																			// ������2���� �¶������� �ٽ� ���ư���.
	VOID	FreeReturnOnline();																		

	VOID	DrawVersion(HDC hdc, SI32 siX, SI32 siY);												// ������ ȭ�鿡 ����ش�.
	
	SI16	DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);		// ���ڸ� ������ش�.
	VOID	DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );									// ���ڸ� ������ش�.
	SI16	DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);		// ���ڸ� �Ӱ� ������ش�.
	VOID	DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// ���ڸ� �Ӱ� ������ش�.
	SI16	DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);	// ���ڸ� Ǫ���� ������ش�.
	VOID	DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// ���ڸ� Ǫ���� ������ش�.
	VOID	DrawNumberBlue(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// ���ڸ� �Ķ��� ������ش�.

	VOID	ProcessMyCharOrder();																	// ���� �� ĳ������ ����� ó���Ѵ�.
	VOID	ExitOnline();																			// ���� ����

	UI16	GetSoundID();																			// ���� ��������� ���̵� ����
	VOID	SetPeace(BOOL bFlag);																	// ��ȭ ��� -> ���� ���
	BOOL	IsPeace()							{	return bPeace;	};								// ��ȭ ��� �ΰ�?

	VOID	FindTrade();																			// ��ο��� �ְ� ���� ��ǰ�� ���� �ش�.
	BOOL	IsFindNow();																			// ��ΰ� �ְ� ���� ��ǰ�� ã�� ���ΰ�?
	VOID	SetShortKey( BOOL bFlag );																// ����Ű
	BOOL	GetShortKey();
	VOID	FindTradeBook();																		// ���

	SI32	GetVillageStateCode(UI16 VillageCode);													// ������ ���� �ڵ带 ���
	SI32	GetVillageGuildFlag(UI16 VillageCode);													// ������ ��� ����� ���
	float	GetVillageDefence(UI16 VillageCode);													// ������ ������ ��´�.
	BOOL	GetVillageEnemyGuild(UI16 VillageCode);													// ���븶������ �˻�
	BOOL	GetVillageWarDayGuild(UI16 VillageCode);												// ���� �����ϴ� �������� �˻� robypark 2004/12/22 11:50
	VDEF	GetVillageDefenceNowDefence(UI16 VillageCode);											// ������ ���� ������ ��´�.
	VDEF	GetVillageDefenceMaxDefence(UI16 VillageCode);											// ������ �ִ� ������ ��´�.
	SI32	GetVillageCurrentGarrisonCount(UI16 VillageCode);										// ������ �ֵ��� ���� ���� �δ� �� ���
	SI32	GetVillageMaximumGarrisonCount(UI16 VillageCode);										// ������ �ֵ��� �ִ� ���� �δ� �� ���
	SI16	GetvillageCounterAttackType(UI16 VillageCode);											// ������ �ݰ� ���� ���� ���. �Ϲ�, ��, �⸧
	BOOL	SendHeartBeat(BOOL first=FALSE);

	// actdoll (2004/01/29 12:53) : ĳ���� �̸��� ���ڿ��� �������� üũ, CheckID() -> IsValidCharacterNameString()���� �ٲ�
//	BOOL	CheckID( CHAR *pID );																	// ��밡���� �Ƶ��ΰ�?
	BOOL	IsValidCharacterNameString( char* pszCharName );										// ĳ������ �̸� ���ڿ��� ��� ������ �����ΰ�?
	VOID	PlayItemEffectSound( SI16 siGroupCode,SI16 siSound );									// ������ ��� & ������ ȿ���� ����
	VOID	PlayFoodEatEffectSound(SI16 siSound );


	void	CheckNowConnectUser(SI32 MaxUser, SI32 NowUser);										// Check Now Connect User
	void	CheckNowConnectUserMessage(void);														// Check Now Connect User Message
	void	ErrorGameServer(void);																	// Error Game Server

	SI32	GetWarFieldMap(void);																	// Get War Field Map
	BOOL	CheckPKFieldMap(void);																	// ���� FieldMap�� PK�� �������� �˻�

	BOOL	IsVillage(void);																		// ���������� Flag
	VOID	SetMainPollStatus(SI32 siStatus) { siMainPollStatus = siStatus; }
	char*   ReadText(char* filename);

	SI32	GetGamePollStatus()				{ return siGamePollStatus; }
	VOID	SetGamePollStatus( SI32 s )		{ siGamePollStatus = s; }
	SI16	GetVillageInStruct()			{ return m_VillageInStruct; }

	VOID	GotoField();
	VOID	GotoVillage();

	UI16	ReturnOnline(BYTE *pMsg);	
	UI16	LeaveVillage(BYTE *pMsg);

	SI08	CheckHeroZone(void);
	VOID	SetBackGroundMusic(SI08 siTempNation, BOOL bForce = FALSE);


	VOID	ToggleAttackMode(UI16 uiAccount, BOOL bAttackMode);

	void	SetQuestIconAction(BOOL state)		{ m_bQuestState = state; }	
	BOOL	GetQuestIconAction()				{ return m_bQuestState;}
	UI32    DevideByLine(CHAR *pContent, SI32 siMaxStrLen);

	BOOL	IsRunningProcess(char *szPorocessName);

	BOOL	WeaponWearCheck(UI16 uiCharID, SI16 siWeaponType);
	
	BOOL	IsNight();																				// ���� ���ΰ�?

	// actdoll (2004/03/30 11:19) : = �̺�Ʈ = ���� ����Ʈ �Ϸ��� ��
//	void	SetDokdoQuestCompleteUserCount( UI32 uiCount )	{	m_uiDokdoQuestCompleteUserCount = uiCount;	}
//	UI32	GetDokdoQuestCompleteUserCount( void )			{	return m_uiDokdoQuestCompleteUserCount;		}

public:
	// robypark 2004/9/9 15:30
	// ���� �̺�Ʈ �Ⱓ������ ���´�.
	BOOL	IsInEventDay(void);

	// robypark 2004/9/9 20:56
	// �̺�Ʈ �Ⱓ, ������ ���� �̺�Ʈ ����Ʈ�� �ִ� NPC ��Ȱ��ȭ
	VOID	DisableEventNPC(void);

public:
	// robypark 2004/10/27 13:59
	// ������ ������ ������ �������� �˻� �Լ�
	// UI16 uiAttackerUniqueID: �������� UniqueID
	// UI16 uiTargetUniqueID: �ǰ����� UniqueID
	BOOL	SiegeWarfare_CanAttackUnit(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID);

	// robypark 2005/1/8 13:16
	// ������ ������ ������ �������� �˻� �Լ�
	// UI16 uiTargetUniqueID: �ǰ����� UniqueID
	BOOL	SiegeWarfare_CheckCanAttackUnit(UI16 uiTargetUniqueID);

	// robypark 2005/1/12 11:3
	// ������ ������ ������ �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
	// UI16 uiTargetUniqueID: �ǰ����� UniqueID
	SI16	SiegeWarfare_CheckCursorCanAttackUnit(UI16 uiTargetUniqueID);

	// robypark 2004/10/28 16:21
	// ������ ������ ������ ������ ���� �������� �˻�
	// [IN]  UI16 uiAttackerUniqueID: �������� UniqueID
	// [IN]  UI32 siObjectID		: ���� ��� ������Ʈ(����) ID
	// [OUT] BOOL &bRangeAttacker	: �����ڰ� ���Ÿ� ���� �����ΰ�?
	BOOL	SiegeWarfare_CanAttackVillage(UI16 uiAttackerUniqueID, SI32 siObjectID, BOOL &bRangeAttacker);

	// robypark 2005/1/8 13:16
	// ������ ������ ������ ������ ���� �������� �˻�
	BOOL	SiegeWarfare_CheckCanAttackVillage(void);

	// robypark 2005/1/12 11:3
	// ������ ������ ������ ������ ���� �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
	SI16	SiegeWarfare_CheckCursorCanAttackVillage(SI32 siObjectID);

public:
	// robypark 2004/10/28 17:29
	// ������ ĳ���� �Ҽ� ����� �������� �˻�
	// [IN] UI16 VillageCode: ���� �ڵ�
	BOOL	GetVillageOurGuild(UI16 VillageCode);

public:
	// robypark 2004/11/1 16:20
	// �ڽ��� �ֺ��� ���� ������ �ִ��� �˻�
	// return value : ������ ������ ���� Ÿ��.
	//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ����ȿ��(���ݷ� ��)�� ������ �޴´�.
	//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ���� �����̴�.
	//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - �Ϲ� ������ ������ �Ѵ�.
	//				-1									   - ��ȿ���� ���� �����̴�.
	SI16 SiegeWarfare_IsInAreaGeneralEffect(void);

	// robypark 2004/11/8 16:23
	// Ư�� ĳ������ �ֺ��� ���� ������ �ִ��� �˻�
	// [IN] UI16 uiUniqueID: ���Ϸ��� ĳ���� Unique ID
	// return value : ������ ������ ���� Ÿ��.
	//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - ����ȿ��(���ݷ� ��)�� ������ �޴´�.
	//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - ���� �����̴�.
	//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - �Ϲ� ������ ������ �Ѵ�.
	//				-1									   - ��ȿ���� ���� �����̴�.
	SI16 SiegeWarfare_IsInAreaGeneralEffect(UI16 uiUniqueID);

	// robypark 2004/11/1 16:22
	// ������ �������� �����ϰ��� �� �� �ڽ� �ֺ��� ������ ������ ���� �ȿ� �ִ��� �˻�
	// Return Value: TRUE  - �� ��� ���� ���� �ȿ� ĳ���Ͱ� �����ϰų� �� ���� �ƴϴ�.
	//				 FALSE - ���� ������ ���� ĳ���Ͱ� �����Ѵ�.
	BOOL	SiegeWarfare_IsInAreaDisabledTransform(void);

	// robypark 2005/1/13 21:39
	// �ڽ��� �ֺ��� ������ ������ ���� �ȿ� �ִ��� �˻�
	// Return Value: TRUE  - �� ��� ���� ���� �ȿ� ĳ���Ͱ� ����.
	//				 FALSE - ��ó�� ���� ����� ���� ���� ĳ���Ͱ� �����Ѵ�.
	BOOL	SiegeWarfare_IsInAreaHostileVillage(void);

	// robypark 2005/1/13 21:39
	// �ڽ��� �ֺ��� �Ʊ���� ������ ���� �ȿ� �ִ��� �˻�
	// Return Value: TRUE  - �Ʊ� ��� ���� ���� �ȿ� ĳ���Ͱ� ����.
	//				 FALSE - ��ó�� �Ʊ� ����� ���� ���� ĳ���Ͱ� �����Ѵ�.
	BOOL	SiegeWarfare_IsInAreaOurVillage(void);

	// robypark 2005/1/13 21:45
	// ������ ���صǴ� �Ϲ� ������(�ǳ���)�� ���� �ʺ��� ���������� ������ų �� �ִ��� �˻�
	BOOL	SiegeWarfare_CheckAbledWarfRefugee(UI16 uiDestUniqueID);

	// robypark 2004/11/2 15:0
	// ������ ���� ���� �Ӽ��� ���� ȿ�� ����ϱ�
	void	SiegeWarfare_CreateAttackEffectCharacter(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID, bool bNeedToSendActionMsg = false);

	// robypark 2004/11/3 14:5
	// ������ ���� ���� �Ӽ��� ���� ȿ�� ����ϱ�
	// ��ǥ���� ��ġ�� ����
	void	SiegeWarfare_CreateAttackEffectVillage(UI16 uiAttackerUniqueID, SI16 ssTargetX, SI16 ssTargetY, UI16 uiVillageCode = 0);

	// robypark 2004/11/8 15:38
	// ������ ���������� �˻��Ѵ�.
	BOOL	IsSiegeWarfareUnit(SI32 siImjinKindIdx);

	// robypark 2004/11/13 12:24
	// ������ ������ ��� ������ ������ ����� �̸�, ü��, ����ǰ, ���� �� �� �ִ����� ��Ÿ���� �÷���
	SI08	GetIsShowInformationAboutSiegeWarfareUnit(void)	{ return m_scIsShowInformationAboutSiegeWarfareUnit;	}
	void	SetIsShowInformationAboutSiegeWarfareUnit(SI08 scIsShowInformationAboutSiegeWarfareUnit);

	// robypark 2004/12/10 18:22
	// �����ϱ� �������̽� ���� ����
	LPSVillageDefenceInterface	GetVillageDefenceInterfaceInfo(void)	{ return &m_sVillageDefenceInterface; }

	// robypark 2004/11/24 16:52
	// ������ �ð� ���� �ɼ� ���� ����ü ���
	LPSOptionSiegeWarfare GetSOptionSiegeWarfare(void)	{ return &m_SOptionSiegeWarfare; }

	// robypark 2004/11/24 16:52
	// ������ �ð� ���� �ɼ� ���� HQ���� �б�
	BOOL LoadSOptionSiegeWarfare(char *pszServiceNation, SI16 siFindServiceType, char *pszFileName);

	// robypark 2004/11/13 16:49
	// ������ ������ �� ���콺 Ÿ�� ���
	// �ٸ� ���� ������ ����Ű�� ���

	// robypark 2004/11/13 16:49
	// ������ ������ �� ���콺 Ÿ�� ���
	// ������ ����Ű�� ���

	// robypark 2004/12/1 10:11
	// ������� �뺴(���ΰ� ����) �κ��丮�� ������ ������ �������� �ִ��� �˻��Ѵ�.
	BOOL IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(void);

	// robypark 2005/1/8 16:26
	// �ش� ������ �뺴�� �κ��丮�� ������ ������ �������� �ִ��� �˻��Ѵ�.
	BOOL IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(UI08	uiFollowerSlot);

	// robypark 2004/12/2 17:31
	// ���ΰ� �� �뺴 �κ��丮�� �ִ� ��� ������ ������ �������� �����Ѵ�.
	void DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit(void);

	/////////////////////////////////////////////////
	// robypark 2004/12/27 10:46
	// ������ �ð������� �˻��Ѵ�.
	BOOL	IsInTimeSiegeWarfare(void);

	// ������ ����� ������ ������ ��ȯ(����) �ð������� �˻�
	BOOL	IsInTimeRecruitArmyItem(void);

	// ������ ����� ������ �������� ��ܿ����� ���� �� �� �ִ� �ð������� �˻�
	BOOL	IsInTimeGiveArmyItem(void);

	// ������ �غ� ���Ͽ� ���, �����, ����� ��� �Ҽ� �������� ���¸��� ���� �ð����� �˻�
	BOOL	IsInTimeDisabledRecruitArmyInGovernment(void);

	// ����ڵ��� ��� ������ �Ұ����� �ð������� �˻�
	BOOL	IsInTimeDisabledTimeToJoin(void);

	// ����ڵ��� ��ܿ��� Ż�� �Ұ����� �ð�(����� ��ܿ� �ذ� �Ұ����ϴ�) ���� �˻�
	BOOL	IsInTimeDisabledTimeToWithdraw(void);

	// ��ź���� ���� ó�� �ð����� �˻�
	BOOL	IsInTimeAllInInvestment(void);

	// robypark 2005/1/31 16:53
	// �����ϴ� ������ �˻�
	BOOL	IsInDaySiegeWarfare(void);

	// ����ڿ� ���� ����� ��ܿ������� �˻�
	BOOL	IsMemberOfSameGuild(UI16 uiDestUniqueID);

	// ����ڿ� ���� ����� ��ܿ������� �˻�
	// char *pszGuildName:	���� ��� ��
	// SI32 siClassInGuild: ���� ��ܳ� ����
	BOOL	IsMemberOfSameGuild(char *pszGuildName, SI32 siClassInGuild);

	// ĳ���� A�� ĳ����B�� ���� ����� ��ܿ������� �˻�
	BOOL	IsMemberOfSameGuild(UI16 uiUniqueID, UI16 uiDestUniqueID);
	/////////////////////////////////////////////////

	// robypark 2005/1/8 11:18
	// ����ǰ �ڵ� �Ҹ� ���� �����ð� ���
	SI32	GetRestTimeAutoSpendSupplyGoods(void);

	// robypark 2005/1/8 11:18
	// ����ǰ �ڵ� �Ҹ� ���� �����ð� ����
	void	SetRestTimeAutoSpendSupplyGoods(SI32 siRestTimeAutoSpendSupplyGoods);

	// robypark 2005/1/12 19:30
	// ������ ���� ��� ���� �ʱ�ȭ
	void	CancelSiegeWarfare_AttackMode(void);

	// robypark 2005/1/14 17:11
	// ���ΰ� ĳ���͵��� ���� ������ ���
	CFieldHeroInfoMgr	*GetFieldHeroInfoMgr(void)	{	return m_pFieldHeroInfoMgr;	}

	// robypark 2005/1/31 12:55
	// ���� ���� ������
	CSiegeUnitTableMgr	*GetSiegeUnitTableMgr(void)
	{
		return m_pSiegeUnitTableMgr;
	}

	// robypark 2005/1/25 18:18
	// �ٸ� ������ ������ ġ�� �����Ѱ�?
	BOOL	SiegeWarfare_CanHeal(UI16 uiUniqueID);

	// robypark 2005/2/15 15:30
	// ������ ������ ġ�ᰡ �������� �˻��Ͽ� ���콺 Ŀ�� ������ ���� ���콺 Ÿ�� ���
	// UI16 uiTargetUniqueID: �ǰ����� UniqueID
	SI16	SiegeWarfare_CheckCursorCanHeal(UI16 uiUniqueID);

	// robypark 2005/1/26 18:22
	// �ʵ忡 ���� �� ���콺 Ŀ�� �Ʒ� ������Ʈ�鿡 ���� ó��(Mouse Move)
	void	ProcessFieldMouseMove(BOOL bIsSiegeWarfareUnit, POINT ptMouse);

	// robypark 2005/1/26 18:22
	// �ʵ忡 ���� �� ���콺 Ŀ�� ���� ��ư Ŭ�� ó��
	void	ProcessFieldMouseLeftBtnClick(BOOL bIsSiegeWarfareUnit, SI32 siCursorX, SI32 siCursorY);

	// robypark 2005/1/26 18:22
	// �ʵ忡 ���� �� ���콺 Ŀ�� ������ ��ư Ŭ�� ó��
	void	ProcessFieldMouseRightBtnClick(BOOL bIsSiegeWarfareUnit);

	// ���޼���(�޵�)�� ��� 10�ʸ��� ������ 10%�� ȸ�� ���� ����ü ���
	LPSRecoveryMana		GetRecoveryManaInfo(void)
	{
		return &m_sRecoveryMana;
	}

	LPSWarningMsgInSiegeWarfare GetWarningMsgInSiegeWarfare()
	{
		return &m_sWarningMsgInSiegeWarfare;
	}
	// H2-S-05.02.04 //
	// ���� ��ī��Ʈ�� ĳ���� ���� ���Ѵ�.
	char*	OnlineCharName(UI16 nAccount);
	// H2-E-05.02.04 //

};
 

BOOL	IsOnlineWorld();																			// ���� �¶��� �������ΰ�?
BOOL	OnlineSendHeartBeat();																		// ������ ��� �ִٴ� ���� ���� ������ ������ �Լ�. 
CHAR	*GetItemName( UI16 uiItemID );																// ������ ������ �̸��� �˾� �´�.
OnlineNoticeList* GetOnlineNoticeListPointer();														// �¶��� ��� �޽������� Ŭ������ �����͸� ���´�.
BOOL	ManContactvillage(int VillageX, int VillageY, int VillageWidth, int VillageXHeight, int CharX, int CharY, int distance);
SI32	GetWarFieldMap(void);																									

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd,LPARAM lParam);
#endif 
