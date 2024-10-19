#ifndef _ONLINE_WORLD_H
#define _ONLINE_WORLD_H


#define SERVERINFO_TICKCOUNT					30000
#define MAX_NIGHT								8
#define	BENCH_MAXNUM							128;

#define	CPF		33				// 한 프레임당 클럭수는 40이다.
//#define	CPF		1			// 한 프레임당 클럭수는 40이다.

// 피격 대상이 마을인지, 캐릭터인지를 나타내는 상수
#define TARGET_TYPE_CHARACTER			1			// 캐릭터
#define TARGET_TYPE_VILLAGE				2			// 마을

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

// 공성전 마을 반격 타입 상수값 선언
#define ON_VILLAGE_ATTACK_OIL				1		// 기름
#define ON_VILLAGE_ATTACK_STONE				2		// 돌
#define	ON_VILLAGE_ATTACK_DEFENCE			3		// 방어력 향상
#define ON_VILLAGE_ATTACK_NONE				4		// 기본 반격

// robypark 2004/11/1 16:53
// 공성전 관련 범위 값으로 실제 적용되는 범위/2한 값
#define SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE	2		// 대장 효과 범위(5 X 5)
#define SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE	10		// 변신 불가능한 적대 마을과의 거리(범위) (21 X 21)

#define SIEGEWARFARE_WASTESUPPLYGOODSTIME			1800	// 공성전 유닛의 보급품 자동 소모 딜레이 시간(1초 == 1000)

// robypark 2005/1/8 11:43
// 공성전 유닛을 마우스 커서로 가리키고 있는 캐릭터의
// 풍선 도움말(체력, 보급품 게이지) 표시할 때 좌표값 및 % 저장 구조체
typedef struct _SSiegeWarfareUnitGaugeInfo
{
	bool		bDrawGaugeHealth;				// 체력 게이지를 그리는가?
	SI16		siDrawGaugeHealthPct;			// 남은 체력 %(10% 단위이며 * 10된 정수값)
	SI08		siDrawGaugeHealthPosX;			// 체력 바 위치
	SI08		siDrawGaugeHealthPosY;			// 체력 바 위치

	bool		bDrawGaugeSupplyGoods;			// 보급품 게이지를 그리는가?
	SI16		siDrawGaugeSupplyGoodsPct;		// 남은 보급품 %(10% 단위이며 * 10된 정수값)

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

// 일주일에 할 수 있는 공성전 수
#define MAX_SIEGE_DAY			7

// robypark 2004/11/24 16:49
// 공성전 관련 옵션 구조체
typedef struct _SOptionSiegeWarfare
{
	SI08		scSiegeDay[MAX_SIEGE_DAY];

	// 공성전 시작 시간(24시간 형태 시간)
	SI16		siSIEGEWARFARETIME_BEGIN;
	// 공성전 종료 시간(24시간 형태 시간)
	SI16		siSIEGEWARFARETIME_END;

	// 폭탄투자 권한 처리 시작시간
	SI16		siSIEGEWARFARETIME_AllInInvestment_Begin;
	// 폭탄투자 권한 처리 종료시간
	SI16		siSIEGEWARFARETIME_AllInInvestment_End;

	// 공성전 관련 범위 값으로 실제 적용되는 범위/2한 값
	SI16		siSIEGEWARFARE_IsinAreaGeneralEffect_Range;		// 대장 효과 범위(5 X 5)
	SI16		siSIEGEWARFARE_IsinAreaHostileVillage_Range;	// 변신 불가능한 적대 마을과의 거리(범위) (21 X 21)
	SI32		siMedicRechargeManaCoolTime;					// 보급수레 마나 10% 자동 회복 시간(초)

	SI32		siVillageCounterAttack_DefenceUp_Cost;		// 마을 반격 방어력 향상 비용
	SI32		siVillageCounterAttack_DefenceUp_Amount;	// 마을 반격 방어력 향상 되는 수치
	SI32		siVillageCounterAttack_DefenceUp_CoolTime;	// 마을 반격 방어력 향상 쿨 타임(초)

	// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 시작 시간
	SI16		siSiegeWarfareTime_RecruitArmy_Begin;
	// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 종료 시간
	SI16		siSiegeWarfareTime_RecruitArmy_End;

	// 사용자들이 상단 가입이 불가능한 시간 - 시작
	SI16		siGuildDisabledTimeToJoin_Begin;
	// 사용자들이 상단 가입이 불가능한 시간 - 종료
	SI16		siGuildDisabledTimeToJoin_End;

	// 사용자들이 상단에서 탈퇴가 불가능한 시간 - 시작(대방이 상단원 해고도 불가능하다)
	SI16		siGuildDisabledTimeToWithdraw_Begin;
	// 사용자들이 상단에서 탈퇴가 불가능한 시간 - 종료(대방이 상단원 해고도 불가능하다)
	SI16		siGuildDisabledTimeToWithdraw_End;

	// 공성전 옵션 값들 기본값으로 설정 - 대장 효과 범위
	void SetDefaultSiegeWarfareOption_IsinAreaGeneralEffect_Range(void)
	{
		siSIEGEWARFARE_IsinAreaGeneralEffect_Range	= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE;		// 대장 효과 범위(5 X 5)
	}

	// 공성전 옵션 값들 기본값으로 설정 - 변신 불가능한 적대 마을과의 거리
	void SetDefaultSiegeWarfareOption_IsinAreaHostileVillage_Range(void)
	{
		siSIEGEWARFARE_IsinAreaHostileVillage_Range	= SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE;		// 변신 불가능한 적대 마을과의 거리(범위) (21 X 21)
	}

	_SOptionSiegeWarfare()
	{
		for (SI32 i = 0; i < MAX_SIEGE_DAY; i++)
			scSiegeDay[i] = -1;

		siSIEGEWARFARETIME_BEGIN					= 20;
		siSIEGEWARFARETIME_END						= 22;

		// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 시작 시간
		siSiegeWarfareTime_RecruitArmy_Begin		= 20;
		// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 불가능 종료 시간
		siSiegeWarfareTime_RecruitArmy_End			= 23;

		// 사용자들이 상단 가입이 불가능한 시간 - 시작
		siGuildDisabledTimeToJoin_Begin				= 17;
		// 사용자들이 상단 가입이 불가능한 시간 - 종료
		siGuildDisabledTimeToJoin_End				= 24;

		// 사용자들이 상단에서 탈퇴가 불가능한 시간 - 시작(대방이 상단원 해고도 불가능하다)
		siGuildDisabledTimeToWithdraw_Begin			= 17;
		// 사용자들이 상단에서 탈퇴가 불가능한 시간 - 종료(대방이 상단원 해고도 불가능하다)
		siGuildDisabledTimeToWithdraw_End			= 24;

		siSIEGEWARFARETIME_AllInInvestment_Begin	= 22;
		siSIEGEWARFARETIME_AllInInvestment_End		= 24;

		// 공성전 관련 범위 값으로 실제 적용되는 범위/2한 값
		siSIEGEWARFARE_IsinAreaGeneralEffect_Range	= SIEGEWARFARE_ISINAREAGENERALEFFECT_RANGE;		// 대장 효과 범위(5 X 5)
		siSIEGEWARFARE_IsinAreaHostileVillage_Range = SIEGEWARFARE_ISINAREAHOSTILEVILLAGE_RANGE;	// 변신 불가능한 적대 마을과의 거리(범위) (21 X 21)
	}

} SOptionSiegeWarfare, *LPSOptionSiegeWarfare;

// 공성 중 워프 경고 메시지 딜레이
#define WARNINGMSGINSIEGEWARFARE_DELAY			300000		// 5분 1초는 1000

// robypark 2005/1/29 13:17
// 공성전 시간에 전투 중인 마을 근처에서는 일반 유닛은 초보자 수련장 워프 경고
typedef struct _SWarningMsgInSiegeWarfare
{
	DWORD			m_dwLeftTime;		// 5분 마다 출력해주는데, 남은 시간

	_SWarningMsgInSiegeWarfare()
	{
		m_dwLeftTime = 0;
	}

} SWarningMsgInSiegeWarfare, *LPSWarningMsgInSiegeWarfare;

// 마나 자동 증가 시간
#define RECOVERY_MANA_DELAY				10000

// robypark 2005/1/29 13:56
// 보급수레(메딕)일 경우 10초마다 마나가 10%씩 회복된다.
typedef struct _SRecoveryMana
{
	bool			m_bSendedRequest;		// 서버에 10% 마나 증가를 요청했나?

	DWORD			m_dwLeftTime;			// 10초 마다 10% 마나 증가인데 남은 시간

	_SRecoveryMana()
	{
		m_bSendedRequest = FALSE;
		m_dwLeftTime	 = RECOVERY_MANA_DELAY;
	}

} SRecoveryMana, *LPSRecoveryMana;

// robypark 2004/11/13 13:36
// 사용자가 공성전 유닛이며(공성 유닛 상태 정보 보이기 버튼 사용가능한 경우),
#define ON_GAMEOPTION_SIEGEWARFAREUNIT								2
// 현재 공성 유닛 상태 정보 보이기
#define ON_GAMEOPTION_SHOWINFORMATION_ABOUTSIEGEWARFAREUNIT			4


// 공성전 유닛 리스트 배열 인덱스
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SWORDMAN		0		// 공성 검병
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SPEARMAN		1		// 공성 창병
#define ARRAY_INDEX_SIEGEWARFAREUNIT_HORSEMAN		2		// 공성 기마병
#define ARRAY_INDEX_SIEGEWARFAREUNIT_ARCHERMAN		3		// 공성 궁병
#define ARRAY_INDEX_SIEGEWARFAREUNIT_DEFENCESOLIDER	4		// 공성 수비병
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SIEGETOWER		5		// 공성 충차
#define ARRAY_INDEX_SIEGEWARFAREUNIT_CATAPULT		6		// 공성 발석거
#define ARRAY_INDEX_SIEGEWARFAREUNIT_SUPPLYWAGON	7		// 공성 보급수레

// robypark 2005/1/17 10:53
// 공성전 중에 피난민 관련 처리를 위한 상수 선언
#define SIEGE_TRANSFORM_MODE_NONE					0		// 공성전 피난민을 위하여 아무런 작업도 안했다.
#define SIEGE_TRANSFORM_MODE_REFUGEE				1		// 공성전 시간일 때, 모든 주인공 유닛을 피난민으로 만들었다. SIEGE_TRANSFORM_MODE_HERO => SIEGE_TRANSFORM_MODE_REFUGEE
#define SIEGE_TRANSFORM_MODE_HERO					2		// 공성전 시간이 아닐 때 모든 피난민 유닛을 원래대로 수정했다. SIEGE_TRANSFORM_MODE_REFUGEE => SIEGE_TRANSFORM_MODE_HERO

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

#define	ON_GAMEPOLL_FIELD						1
#define	ON_GAMEPOLL_VILLAGE						2	
#define	ON_GAMEPOLL_VILLAGEDEFENCEINTERFACE		3	// 수성하기 인터페이스 관련 처리 중...

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

#include "IOnlineObject.h"			// OnlineWorld가 넘어가야 함.
#include "IOnlineObjectKI.h"
#include <OnlineVillageDescriptList.h>
#include <OnlineFieldEffect.h>		// OnlineWorld가 넘어가야 함.
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

//준석이형 추가부분 
//class	JSWndMgr;
//class	JSWnd;

// actdoll (2004/01/13 22:28) : 디버그용 클래스 추가 선언
class	CDebugDisplay;

struct	MyCharacterInfo;
struct	SAVEFILE_SORT;


// 수성하기 인터페이스 관련 정보 구조체
typedef struct _SVillageDefenceInterface
{
	// 수성 인터페이스 기능 사용할 때 주어지는 딜레이
	SI32			m_siCurrentDelayTimeUsingVillageAttackType;
	SI32			m_siMaxDelayTimeUsingVillageAttackType;

	UI16			m_uiVillageCode;			// 마을 코드

	_SVillageDefenceInterface()
	{
		m_siCurrentDelayTimeUsingVillageAttackType	= 0;
		m_siMaxDelayTimeUsingVillageAttackType		= 0;
		m_uiVillageCode								= 0;
	}

	// 경과된 지연시간 설정
	void SetPastDelayTimeUsingVillageAttackType(DWORD siPastDelayTime)
	{
		m_siCurrentDelayTimeUsingVillageAttackType -= siPastDelayTime;

		if (m_siCurrentDelayTimeUsingVillageAttackType < 0)
			m_siCurrentDelayTimeUsingVillageAttackType = 0;
	}

	// 남은 지연 시간 설정
	void SetDelayTimeUsingVillageAttackType(SI32 siDelayTime)
	{
		m_siCurrentDelayTimeUsingVillageAttackType = m_siMaxDelayTimeUsingVillageAttackType = siDelayTime;
	}

} SVillageDefenceInterface, *LPSVillageDefenceInterface;

// robypark 2005/1/12 10:33
// 자신이 공성전 유닛일 때 공격 모드 상수 값
#define SIEGEWARFARE_ATTACKMODE_NONE		0		// 공격 중인 타겟이 없다.
#define SIEGEWARFARE_ATTACKMODE_CHARACTER	1		// 공격 중인 타겟은 다른 공성 유닛이다.
#define SIEGEWARFARE_ATTACKMODE_VILLAGE		2		// 공격 중인 타겟은 적 마을이다.

// robypark 2005/1/12 10:28
typedef struct _SSiegeWarfare_AttackMode
{
	SI08		scAttackMode;		// 자신이 공성전 유닛일 때 공격 중인 타겟이 무엇인가를 나타내는 값
									// SIEGEWARFARE_ATTACKMODE_NONE, SIEGEWARFARE_ATTACKMODE_CHARACTER, SIEGEWARFARE_ATTACKMODE_VILLAGE

	SI32		siAutoAttackTimes;	// 자동 공격 횟수 제한하기

	// 공격 대상 ID
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
	DWORD					dwRequestGameDataTime;								// 게임 데이타를 요구한 시간
};

struct OnlineNumPad
{
	OnlineNumberControl		*pNumberControl;									// 숫자판	
	__int64					siCount;											// 리턴 되는 수
	SI32					fDisable;											// Prepare시 꺼지는 경우를 처리하기 위해 필요함
};

struct OnlineBench
{
	UI16					m_uiBoothItemID[5];									// 좌판위에 있는 아이템 아디
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
	BOOL				m_bIsInEventDay;			// 현재 이벤트 기간인지를 나타내는 플래그

	// robypark 2005/1/8 11:18
	SI32				m_siRestTimeAutoSpendSupplyGoods;	// 보급품 자동 소모 남은 지연시간

	// robypark 2005/1/8 11:43
	// 공성전 유닛을 마우스 커서로 가리키고 있는 캐릭터의
	// 풍선 도움말(체력, 보급품 게이지) 표시할 때 좌표값 및 % 저장 구조체
	SSiegeWarfareUnitGaugeInfo		m_sSiegeWarfareUnitGaugeInfo;

	// robypark 2005/1/14 17:11
	// 주인공 캐릭터들의 정보 관리자
	CFieldHeroInfoMgr	*m_pFieldHeroInfoMgr;

	// robypark 2005/1/17 10:53
	// 공성전 중에 피난민 관련 처리를 위한
	SI08				m_scSiegeTransformMode;

	// robypark 2005/1/29 13:25
	// 공성전 중 워프 될 수 있다는 경고 메시지 처리 변수
	SWarningMsgInSiegeWarfare		m_sWarningMsgInSiegeWarfare;

	// 보급수레(메딕)일 경우 10초마다 마나가 10%씩 회복된다.
	SRecoveryMana					m_sRecoveryMana;

	// robypark 2005/1/31 12:55
	// 공성 유닛 관리자
	CSiegeUnitTableMgr				*m_pSiegeUnitTableMgr;

public:	
	SI32				g_PingPerUserCount[2];
//	SConnectServerInfo	*m_LogInInfo_Data;
	BOOL				bNotice;

	BOOL				fLeftMemoryUpSwitch;		// 마우스 Up Flag 기억
	BOOL				fRightMemoryUpSwitch;		// 마우스 Up Flag 기억

	BOOL				fLeftUpSwitch;				// 마우스 Up Flag
	BOOL				fRightUpSwitch;				// 마우스 Up Flag
	BOOL				fLeftOneSwitch;				// 마우스 One Flag
	BOOL				fRightOneSwitch;			// 마우스 One Flag
	BOOL				fVillageLeftOneSwitch;		// 마을용 마우스 One Flag
	BOOL				fVillageRightOneSwitch;		// 마을용 마우스 One Flag
	BOOL				bPay;						// 보수
	BOOL				bCharMove;					// 캐릭터 이동하나 마나
	BOOL				bReturnKeyDown;				// 엔터키 눌렸나 체크
	
	__int64				dwMapTileDrawTime;			// Map Tile Draw Time
	SI32				siMapTileDrawTimeAddTime;	// Map Tile Draw Add Time
	__int64				siMapTileDrawTimeAddCount;	// Map Tile Draw Add Count


	DWORD				m_WaitTickCount;			// Logout WaitDelay TickCount
	DWORD				m_ImageFreeTimer;

// 재미로..
	int                 m_siUpdateMethod;           
	int                 m_siUpdateCounter;

private:	
	BOOL				fYetLeftPressSwitch;		// 이전에 마우스 Press Flag
	BOOL				fYetRightPressSwitch;		// 이전에 마우스 Press Flag

	float				fYetLight;					// 현재 밤일때 어두어질 비율

	SI16				siOnlineMajorVersion;		// 온라인 버전(메이저)
	SI16				siOnlineMinorVersion;		// 온라인 버전(마이너)
	CHAR				szOnlineVersion[32];		// 문자열 온라인 버젼.

	UI32				uiTotalFrame;					// 전체 프레임 수.
	DWORD				dwCurrentFrameStartTime;		// 현재 프레임의 시작된 시간	
	DWORD				dwCurrentFrameAddTime;			// 현재 프레임의 남은 시간	
	SI16				dwCurrentFrameSkippingCount;	// 현재 프레임 스킵핑수
	SI32				siFPS;							// 초당 프레임 수

	SI32				siMainPollStatus;			// 게임 외부에서의 폴 상태.
	SI32				siGamePollStatus;			// 게임 내부에서의 폴 상태.
	
	SI32				siLogOutStatus;				// 로그아웃 할때의 상태.
	SI32				siNextMenuAfterLogOut;		// 로그아웃하고 그 후에 이동할 메뉴.
	SI32				siNextPollAfterLogOut;		// 로그아웃하고 그 후에 이동할 메뉴.
	BOOL				bWillGoGameServer;			// 게임 서버로 이동하는가?
	BOOL				bExitOnlineGame;			// 게임이 종료 되는가.
	RECT				rcPannel;					// 판넬.
	BOOL				ShortKey;					// 단축키
	BOOL				bFirstLogIn;
	BOOL				bServerSelect;
	SI16				siTalkDealy;
	SI16				m_siNoField;
	POINT				BenchMousePosition;			// 좌판안에 있을때 마우스 클릭위치

	BOOL				m_iMinPlaySoundTick;

	DWORD				m_GameTickCount;			// Game TickCount
	DWORD				m_GameTime;					// Game Time
	DWORD				m_dwGameGuardCheckTick;

	DWORD				m_HackGameTickCount;		// Hack Game TickCount
	DWORD				m_HackGameTime;				// Hack Game Time
	SI32				m_TickTimeDiff;

	SI08				m_siNationCode;

	DWORD				m_LastNowUserConnectMessageTickCount;			// 마지막으로 현재 접속한 User정보 메세지를 받은 시간
	SI32				m_LastNowUserConnectMessageNowUser;				// 마지막으로 현재 접속한 User정보 메세지를 받은 중에서 현재 접속자 수
	SI32				m_LastNowUserConnectMessageErrorOutPutCount;	// 마지막으로 현재 접속한 User정보 메세지를 받은 중에서 에러가 날경우 에러음 출력수

//	UI32				m_uiDokdoQuestCompleteUserCount;				// actdoll (2004/03/30 11:01) : = 이벤트 = 독도퀘스트 끝낸 유저수

	OnlineLogInMgr					*pOnlineLogInMgr;					// 로그인 관련 메니저
	OnlineLoadingData				*pOnlineLoadingData;				// 초기 로딩

	OnlineReturnOnline				*pOnlineReturnOnline;				// 전투를 종료하고 Online으로 복귀한다.
	OnlineBench						*pOnlineBench;						// 좌판


//	CMacroCheckProcess				*m_pMacroProcess;					// 매크로 사용자 프로세서

	// robypark 2004/11/24 16:51
	SOptionSiegeWarfare				m_SOptionSiegeWarfare;				// 공성전 시간 등의 옵션 정보 구조체
	
public:	
	OnlineVillageInfoData			*pOnlineVillageInfoData;			// 마을 정보

	UI16					BackgroundMusic;					// 배경 음악의 아이디

//	BOOL					m_LanguageMode;						// 언어모드
	ELanguageCode			m_LanguageCode;

	UserGrade				m_GMFlag;							// 운영자 Flag
	BOOL					m_InfoFlag;							// 정보 Flag
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
	BYTE					PaletteNight[MAX_NIGHT][768];		// Palette 밤, 낯에 관련된 Palette

	// robypark 2005/1/12 10:55
	// 공성전 공격 모드 설정 변수
	SSiegeWarfare_AttackMode	m_sSiegeWarfare_AttackMode;

	CWordSearchTree			m_SlanderFilter;			// 욕설필터

	OnlineGameOption*		m_GameOption;				// Game Option

	OnlineTicketParser*		pTicketParser;				// Ticket Parser
	OnlineGeneralParser*	pGeneralParser;				// General Parser
	OnlineGeneralParser*    pGeneralParserEx;			// General ParserEx
	OnlineQuestParser*		pQuestParser;				// Quest Parser
	OnlineLevelMarkParser*	pLevelMarkParser;			// Level Mark Parser

	POINT					YetMousePosition;			// 지난번 마우스 위치 
	COnlineTimer			*pTimer;					// Timer
	BOOL					fFirstGetTime;				// 처음으로 서버에서 시간을 받았는지 검사하는 Flag
	OnRealWorldTime			m_RealTime;					// 실제 시간

	BOOL					bPeace;						// 평화 모드 인가?	
	SI32					m_VillageAttackDelay;		// Village Attack Delay

	OnlineMercenaryParser*	pOnlineMercenaryParser;		// 고용할수 있는 용병수 Data
	OnlineEconomy			*pOnlineEconomy;			// 통장에 관한 Data
	IOnlineCharKI			*pIOnlineCharKI;			// 캐릭터 종류의 인터페이스.
	IOnlineChar				*pIOnlineChar;				// 캐릭터 인터페이스
	IOnlineObjectKI			*pIOnlineObjectKI;			// 오브젝트 종류의 인터페이스	
	IOnlineObject			*pIOnlineObject;			// 오브젝트의 인터페이스.	
	IOnlineShipKI			*pIOnlineShipKI;			// 배 종류의 인터페이스.
	OnlineMap				*pOnlineMap;				// 맵 객체
	OnlineMapSound			*m_pOnlineMapSound;
	OnlineCharParser		*pOnlineCharParser;

	BOOL					m_fChattingRoom;
	OnlineVillage			*pOnlineVillage;			// 마을 객체.
	SI16					m_VillageInStruct;

	OnlineVillageDescriptList*	pOnlineVillageDescriptList;	// 마을 설명 List
	OnlineNoticeList*		pOnlineNoticeList;			// 공지사항 List
	OnlineFieldEffect*		pOnlineFieldEffect;			// 필드상 Effect 처리
	
	cltOnlineFindPath		*pclFindPath;				// 길찾기 인공지능 객체

	MyData					*pMyData;					// 나의 데이타들.

	OnlineImoticon			*pOnlineImoticon;			// 이모티콘
	

	OnlineClient			*pOnlineClient;					// 온라인 클라이언트 객체.
	HQClientSocket			*pOnlineClientChatting;			// 온라인 Chatting 클라이언트 객체.
	OnlineResource			*pOnlineResource;				// 온라인 리소스 객체.

	OnlineCharUI			*pOnlineCharUI;				// 나의 캐릭터 조작을 위한 객체.

	OnlineInventory			*pOnlineInventory;			// 인벤토리 창.
	OnReqThrowItemMsg		OnlineTempOnReqThrowItemMsg;
	OnReqPickUpItemMsg		OnlineTempOnReqPickUpItemMsg;

	OnlineMiniMap			*pOnlineMiniMap;			// 미니맵

	OnlineFonts				*pOnlineFonts;				// 온라인에서 사용하는 폰트들을 모아 놓은 객체.
	OnlineText				*pOnlineText;				// 온라인에서 사용하는 모든 텍스트를 모아 놓은 객체.
	CItem					*pItemsInfo;				// 온라인에서 사용하는 모든 아이템을 모아 놓은 클래스.
	OnlineKeyboard			*pOnlineKeyboard;			// 키보드를 처리하는 오브젝트.	

	LPDIRECTDRAWSURFACE7	pOnlineDirectDraw;			// 온라인에서 사용하는 다이렉트드로우 객체.	

  
	OnlineConfig				*pConfig;				// Config 파일 관리 클래스	
	OnlineMegaText				*pOnlineMegaText;		// Text 관리,	
//	OnlineCharAni				*pOnlineCharAni;		// 모든 캐릭터들의 애니메이션 정보를 저장하고 있는 객체.
	OnlineCharActionCode		*pOnlineCharActionCode;	// 캐릭터의 문자열 액션 코드를 숫자 액션 코드로 변환해 주는 객체.

	IOnlineMouse				*pIOnlineMouse;			// 온라인에서 사용하는 모든 마우스를 관리하는 객체.
	OnlineFieldArea				*pOnlineFieldArea;		// 필드 지역을 관리하는 클래스.	
	OnlineVillageManager		*pOnlineVillageManager;	// 마을 관리 클래스.

	OnlinePannel				*pOnlinePannel;			// 온라인 판넬.
	OnlineHelp					*pOnlineHelp;			// 도움말 출력

	OnlineMousePointInfo		*pMousePointInfo;		// 마우스가 가리키고 있는 것의 대한 정보.
	OnlineMousePoint			*pOnlineMousePoint;		// Mouse Point
	OnRPI						RPI;					// RequeestPlayerInfo
	OnlineNumPad				*pOnlineNumPad;			// 숫자판
	OnlineMsgBox				*pOnlineMsgBox;				// 메세지 박스
	OnlineMsgBox				*pOnlineMsgBoxPay;			// 메세지 박스(값 지불) 마을 투자 수익 결과, 생산시설 팔려나가는 경우, 상단 해체, 상단 내 직급 조절, 등의 메시지 박스로 사용된다.
	OnlineMsgBox				*pOnlineMsgBoxUsePortal;	// 메세지 박스(포탈을 쓰겠냐?)
	OnlineMsgBox				*pOnlineMsgBoxError;		// 메세지 박스(에러)
	OnlineMsgBox				*pOnlineLoginMsgBox;		// 로그인메세지 박스.
	SI32						UsePortalType;			// Portal 사용 확인시 적용돼는 Portal
	OnlineTradeBook				*pOnlineTradeBook;		// 장부
	OnlineVillageSave			*pOnlineVillageSave;	// 장부에서 마을에 관련 파일을 읽고 쓴다.
	OnlineBuilding				*pOnlineBuilding;		// 생산 시설 관련 파서

	CHAR						m_TempBuffer[4096];		// 공용으로 쓰기 위한 버퍼.
	SI16						NextMapIndex;			// 현재 이동할려는 Portal
	CCharFileLoadManager		*pCharFileLoadManager;	// 캐릭터 데이터
	OnlineTrade					*pOnlineTrade;			// 거래
	BOOL						bFindTrade;				// 장부 검색
	SAVEFILE_SORT				*pTempSearch;
	SI16						siTempSearchNum;
	OnlineBooth					*pOnlineBooth;			// 좌판
	BOOL						bTempFlag;
	SI16						m_siMsgBoxStatus;

	cltIGuildClass				m_cltIGuildClass;		// 길드 직급명
	OnlineTip					*pOnlineTip;			// TIP 출력
	OnlineQuestPlay				*pOnlineQuestPlay;		// Quest Play 출력
	OnlineParty					*pOnlineParty;
	OnlineMessenger				*pOnlineMessenger;
	OnlineDungeonParser			*pOnlineDungeon;		// 특수지형 파서
	cltIProfit					*pIProfit;
	_Char						*pChar;
	OnlineTradeParser			*pOnlineTradeParser;
	OnlineSkillParser			*pOnlineSkillParser;
	OnlineSetting				*pOnlineSetting;		// 환경설정
	CBuriedItemParser			*pCBuriedItemParser;

//	OnlineBanner				*pOnlineBanner;

	OnlineMsgProc				*pOnlineMsgProc;

	CMemoryProtected*			pMemoryProtected;

	// 경매
	OnlineReception             *pOnlineReception;
	OnlineFieldAuction          *pOnlineFieldAuction;

	OnlineSmith					*pOnlineSmith;			// 대장장이
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

	SI16						m_CharPosition_X;		// 아이템 탐사 요청시 캐릭위치 기억
	SI16						m_CharPosition_Y;

	// actdoll (2004/04/02 15:28) : = 이벤트 = 독도 퀘스트 관련
//	BOOL						m_bEventDokdoQuestPlay;	// 독도 퀘스트를 진행중인가?
//	RECT						m_rtIconDokdoQuest;		// 독도 퀘스트 아이콘의 영역
//	XSPR						m_Spr_Icon_DokdoQuest;	// 독도 퀘스트용 아이콘 이미지

	BOOL						m_bReceivedItem;		// 도착한 아이템이 있는지...
	RECT						m_rtReceivedItem;		// 도착한 아이템 이미지영역
	XSPR						m_ReceivedItemAniSpr;
	SI16						m_siAniNoField;

	BOOL						m_bItemDraging;			// 받은 아이템 드래깅중
	DWORD						m_dwDelay;
	BOOL						m_bReceivdeItemShow;	// 깜빡거림 표현하기위한 변수
	SI16						m_siReceivedItemCount;
	SI16						m_siInnStatus;			// 주막에서의 상태(받기모드:1, 보내기모드:2)
	BOOL						m_bMiniGameOn;			// 현재 미니게임 중인지
	BOOL						m_bGuildOn;				// 상단 채팅채널을 열고닫음.

	CHAR						m_szProgramList[200][255];
	UI16						m_uiProgramNum;
	CHAR						szURLBuffer[128];
	char						m_szCharTextList[10][256];

	BOOL						m_bFollowerSelLock;		// 용병선택시 단축키 1~10번 사용가능한지 아닌지
	BOOL						m_bEvent;
	BOOL						m_bQuestState;

	SI16						m_siFieldHp;

	OnlineNPCTalk				*m_pNpcTalk;			// NPC 자동 말하기
	OnlineChatMacro				*m_pChatMacro;			// 체팅 메크로
	OnlineQuestInterface		*m_pQuestInterface;		// 퀘스트 인터페이스
	OnlineMercenary				*m_pMercenary;			// 용병창

	// actdoll (031229)			추가데이터
	OnlinePortrait				*m_pPortrait;			// 초상화 컨트롤러

	// robypark 2004/10/25 13:26
	// 공성전 전투 효과 관련 변수 추가
	COnlineFieldEffect_SiegeWarfare_BulletMgr		m_SiegeWarfare_BulletMgr;		// 공성전 총알
	COnlineFieldEffect_SiegeWarfare_SmokeMgr		m_SiegeWarfare_SmokeMgr;		// 공성전 효과(Smoke)

	// robypark 2004/11/13 12:24
	// 공성전 유닛일 경우 공성전 유닛의 사용자 이름, 체력, 보급품, 등을 볼 수 있는지를 나타내는 플래그
	SI08						m_scIsShowInformationAboutSiegeWarfareUnit;

	// robypark 2004/12/10 18:22
	// 수성하기 인터페이스 관련 변수
	SVillageDefenceInterface	m_sVillageDefenceInterface;

public:	
	cltOnlineWorld();								// 생성자.
	~cltOnlineWorld();								// 소멸자.
  
	BOOL	Init();									// 초기화.	
	VOID	Initialize();							// 초기화.	
	VOID	DrawTextInLoading(CHAR *pszString);		// 로딩중에 텍스트를 출력한다.
	VOID	Free();									// 해제. --> 온라인상에서 쓰는 모든 것을 해제하는 함수
	UI16	Poll();

	VOID	ScrollLoading( UI32 uiLoadPercent );
	
	// actdoll (2004/11/15 18:43) : 인트로 화면 추가
	void	IntroPoll				( void );
	void	IntroInit				( void );
	void	IntroFree				( void );
	void	IntroPoll_FadeIn_Ready	( void );
	void	IntroPoll_FadeIn		( void );
	void	IntroPoll_Draw			( void );
	void	IntroPoll_FadeOut_Ready	( void );
	void	IntroPoll_FadeOut		( void );
	void	IntroPoll_Finish		( void );

	// 로그인 메니저
	VOID	InitLogInMgr();
	UI16	LogInMgrPoll();
	VOID	DrawLogInMgr();
	VOID	FreeLogInMgr();
	BOOL	RecvPWData();

	// 약관동의, 공지사항
	VOID	InitAgreeNotice(BOOL blicense);
	SI08	AgreeNotice();
	VOID	DrawAgreeNotice();
	VOID	FreeAgreeNotice();

	// 서버선택
	VOID	InitSelectServer();
	SI08	SelectServer();
	VOID	DrawSelectServer();
	VOID	FreeSelectServer();
	VOID	ScrollAction();

	// 계정 로그인
	VOID	InitAccountLogIn();
	SI08	AccountLogIn();
	VOID	DrawAccountLogIn();
	VOID	FreeAccountLogIn();

	// 캐릭터 선택
	VOID	InitSelectCharacter();
	SI08	SelectCharacter();	
	VOID	DrawSelectCharacter();
	VOID	FreeSelectCharacter();

	// 캐릭터 생성
	VOID	InitCreateCharacter();
	SI08	CreateCharacter();	
	VOID	DrawCreateCharacter();
	VOID	FreeCreateCharacter();
  
	// 게임 시작
	VOID	InitGameServerLogIn();
	SI08	GameServerLogIn();						
	VOID	DrawGameServerLogIn();
	VOID	FreeGameServerLogIn();

	/////////////////////////////////////
	
	VOID	InitGamePoll();							// 메인 Poll() 초기화
	UI16	GamePoll();								// 메인 Poll()	

	UI16	FieldPoll(UI16 uiClientRet);				 					// 캐릭터가 필드상에 있을때 호출하는 함수.
	VOID	FieldShortCutKey();												// 필드 상에서의 단축키를 처리한다.
	VOID	FieldDraw();													// 필드에서 화면 출력부분을 처리한다.
	VOID	FieldDrawPannel();												// 필드에서 판넬을 출력한다.		
	VOID	InitFieldDrawPannel();					// 판넬을 초기화 한다.
 
	UI16	VillagePoll(UI16 uiClientRet);			// 캐릭터가 마을상에 있을때 호출하는 함수.
	UI16	MapEditotrPoll();						// Map Eidtor Poll 함수.			
	VOID	InitReceiveGoOutVillage();				// 마을을 나가기 위한 초기화
	UI16	ReceiveGoOutVillage();					// 마을을 나가기 위한 작업
	VOID	InitConnectServerSelect();				// 접속할 서버 고르기.(초기화)

	// 공성전 수성하기 인터페이스 관련 OnlineWorld-VillageDefenceInterface.cpp (캐릭터가 필드에 있는 것처럼 처리한다. 단, 필드상황을 볼 수만 있다.)
	UI16	VillageDefenceInterfacePoll(UI16 uiClientRet);					// 수성하기 인터페이스 메인 폴
	VOID	VillageDefenceInterfaceDraw(VOID);								// 수성하기 인터페이스 및 필드 그리기
	VOID	InitVillageDefenceInterface(VOID);								// 수성하기 인터페이스 초기화
	VOID	FreeVillageDefenceInterface(VOID);								// 수성하기 인터페이스 해제
	UI16	OnResponseEnterVillageDefenceInterface(BYTE *pMsg);		// ON_RESPONSE_WAR_VILLAGE_DEFENCE, 수성하기 인터페이스로 들어가기 요청에 대한 응답 처리
//	UI16	OnResponseEnterVillageFromVillageDefenceInterface(BYTE *pMsg);		// ON_RESPONSE_WAR_VILLAGE_IN, 수성하기 인터페이스에서 마을로 들어가기 요청에 대한 응답 처리
	
	// 수성하기 인터페이스에 나온다. 필드로
	// ON_RESPONSE_VILLAGE_DEFFNCE_STOP
	UI16	OnResponseLeaveVillageDefenceInterface(BYTE *pMsg);

	UI16	OnResponseVullageAttackTypeCheck(BYTE *pMsg);	// ON_RESPONSE_VULLAGE_ATTACK_TYPE_CHECK, 수성하기 인터페이스에서 공격 속성 등의 초기화 정보 얻기

			       
	SI32	GetDirection(CHAR *pszDirection);														// 문자열로 방향을 가지고 숫자로 되어 있는 방향을 얻어온다.
	SI32	GetDirection8(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2);								// 방향을 구한다.
	UI32	GetTotalFrame();																		// 프레임수를 구해온다.

	VOID	UpdateScreen(LPDIRECTDRAWSURFACE7 pSurface = NULL, BOOL bSourceColorKey = FALSE,BOOL bForceRedraw = FALSE);		// 서피스를 프라이머리 서피스에 갱신 시킨다.	

	VOID	GetLatitudeLongitude(SI32 siX, SI32 siY, SI32 *psiLongitude, SI32 *psiLatitude, SI32 *psiLongitudeDegree, SI32 *psiLatitudeDegree);		// 타일 좌표를 사용하여 위도와 경도를 얻어온다.	
	UI16	ReceiveMessageProc(UI16 uiReceivedMessage);												// 서버로 부터 받은 메시지를 처리한다.	
	UI16	ReceiveMessageProcChatting(HQPacket* lpPacket);											// Chatting서버로 부터 받은 메시지를 처리한다.	

	VOID	ShowSystemCursor(BOOL bShow);															// 시스템 마우스를 보이게 해준다.
	VOID	SetLogOut(SI32 siNextMenu, SI32 siNextPoll, BOOL bGoGameServer = FALSE);				// 로그아웃을 하고 다음으로 이동할 메뉴를 설정한다.
	
	VOID	FreeForGotoImjinGame();																	// 임진록2 Game 으로 가기 위하여 해제해야 할 것등을 해제한다.	

	VOID	InitReturnOnline();																		// 임진록2 Game에서 임진록 온라인으로 가기 위해서 해제했던 메모리를 다시 할당 받는다.
	UI16	ReturnOnline();																			// 임진록2에서 온라인으로 다시 돌아간다.
	VOID	FreeReturnOnline();																		

	VOID	DrawVersion(HDC hdc, SI32 siX, SI32 siY);												// 버젼을 화면에 찍어준다.
	
	SI16	DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);		// 숫자를 출력해준다.
	VOID	DrawNumber(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );									// 숫자를 출력해준다.
	SI16	DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);		// 숫자를 붉게 출력해준다.
	VOID	DrawNumberRed(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// 숫자를 붉게 출력해준다.
	SI16	DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, SI16 startfont, SI32 number, SI16 option);	// 숫자를 푸르게 출력해준다.
	VOID	DrawNumberGreen(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// 숫자를 푸르게 출력해준다.
	VOID	DrawNumberBlue(SI16 startx, SI16 starty, XSPR *pXspr, CHAR *pNumber );								// 숫자를 파랗게 출력해준다.

	VOID	ProcessMyCharOrder();																	// 현재 내 캐릭터의 명령을 처리한다.
	VOID	ExitOnline();																			// 게임 종료

	UI16	GetSoundID();																			// 현재 배경음악의 아이디를 리턴
	VOID	SetPeace(BOOL bFlag);																	// 평화 모드 -> 전투 모드
	BOOL	IsPeace()							{	return bPeace;	};								// 평화 모드 인가?

	VOID	FindTrade();																			// 장부에서 최고 투자 상품을 구해 준다.
	BOOL	IsFindNow();																			// 장부가 최고 투자 상품을 찾는 중인가?
	VOID	SetShortKey( BOOL bFlag );																// 단축키
	BOOL	GetShortKey();
	VOID	FindTradeBook();																		// 장부

	SI32	GetVillageStateCode(UI16 VillageCode);													// 마을의 국가 코드를 얻기
	SI32	GetVillageGuildFlag(UI16 VillageCode);													// 마을의 길드 깃발을 얻기
	float	GetVillageDefence(UI16 VillageCode);													// 마을의 방어력을 얻는다.
	BOOL	GetVillageEnemyGuild(UI16 VillageCode);													// 적대마을인지 검사
	BOOL	GetVillageWarDayGuild(UI16 VillageCode);												// 오늘 공성하는 마을인지 검사 robypark 2004/12/22 11:50
	VDEF	GetVillageDefenceNowDefence(UI16 VillageCode);											// 마을의 현재 방어력을 얻는다.
	VDEF	GetVillageDefenceMaxDefence(UI16 VillageCode);											// 마을의 최대 방어력을 얻는다.
	SI32	GetVillageCurrentGarrisonCount(UI16 VillageCode);										// 마을에 주둔한 현재 수비병 부대 수 얻기
	SI32	GetVillageMaximumGarrisonCount(UI16 VillageCode);										// 마을에 주둔한 최대 수비병 부대 수 얻기
	SI16	GetvillageCounterAttackType(UI16 VillageCode);											// 마을의 반격 공격 형태 얻기. 일반, 돌, 기름
	BOOL	SendHeartBeat(BOOL first=FALSE);

	// actdoll (2004/01/29 12:53) : 캐릭터 이름의 문자열이 정상인지 체크, CheckID() -> IsValidCharacterNameString()으로 바뀜
//	BOOL	CheckID( CHAR *pID );																	// 사용가능한 아디인가?
	BOOL	IsValidCharacterNameString( char* pszCharName );										// 캐릭터의 이름 문자열이 사용 가능한 형태인가?
	VOID	PlayItemEffectSound( SI16 siGroupCode,SI16 siSound );									// 아이템 사용 & 장착시 효과음 연주
	VOID	PlayFoodEatEffectSound(SI16 siSound );


	void	CheckNowConnectUser(SI32 MaxUser, SI32 NowUser);										// Check Now Connect User
	void	CheckNowConnectUserMessage(void);														// Check Now Connect User Message
	void	ErrorGameServer(void);																	// Error Game Server

	SI32	GetWarFieldMap(void);																	// Get War Field Map
	BOOL	CheckPKFieldMap(void);																	// 현재 FieldMap이 PK가 가능한지 검사

	BOOL	IsVillage(void);																		// 마을안인지 Flag
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
	
	BOOL	IsNight();																				// 현재 밤인가?

	// actdoll (2004/03/30 11:19) : = 이벤트 = 독도 퀘스트 완료자 수
//	void	SetDokdoQuestCompleteUserCount( UI32 uiCount )	{	m_uiDokdoQuestCompleteUserCount = uiCount;	}
//	UI32	GetDokdoQuestCompleteUserCount( void )			{	return m_uiDokdoQuestCompleteUserCount;		}

public:
	// robypark 2004/9/9 15:30
	// 현재 이벤트 기간인지를 얻어온다.
	BOOL	IsInEventDay(void);

	// robypark 2004/9/9 20:56
	// 이벤트 기간, 국가에 따른 이벤트 퀘스트를 주는 NPC 비활성화
	VOID	DisableEventNPC(void);

public:
	// robypark 2004/10/27 13:59
	// 공성전 유닛이 공격이 가능한지 검사 함수
	// UI16 uiAttackerUniqueID: 공격자의 UniqueID
	// UI16 uiTargetUniqueID: 피격자의 UniqueID
	BOOL	SiegeWarfare_CanAttackUnit(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID);

	// robypark 2005/1/8 13:16
	// 공성전 유닛이 공격이 가능한지 검사 함수
	// UI16 uiTargetUniqueID: 피격자의 UniqueID
	BOOL	SiegeWarfare_CheckCanAttackUnit(UI16 uiTargetUniqueID);

	// robypark 2005/1/12 11:3
	// 공성전 유닛이 공격이 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
	// UI16 uiTargetUniqueID: 피격자의 UniqueID
	SI16	SiegeWarfare_CheckCursorCanAttackUnit(UI16 uiTargetUniqueID);

	// robypark 2004/10/28 16:21
	// 공성전 유닛이 공격이 마을을 공격 가능한지 검사
	// [IN]  UI16 uiAttackerUniqueID: 공격자의 UniqueID
	// [IN]  UI32 siObjectID		: 공격 대상 오브젝트(마을) ID
	// [OUT] BOOL &bRangeAttacker	: 공격자가 원거리 공격 유닛인가?
	BOOL	SiegeWarfare_CanAttackVillage(UI16 uiAttackerUniqueID, SI32 siObjectID, BOOL &bRangeAttacker);

	// robypark 2005/1/8 13:16
	// 공성전 유닛이 공격이 마을을 공격 가능한지 검사
	BOOL	SiegeWarfare_CheckCanAttackVillage(void);

	// robypark 2005/1/12 11:3
	// 공성전 유닛이 공격이 마을을 공격 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
	SI16	SiegeWarfare_CheckCursorCanAttackVillage(SI32 siObjectID);

public:
	// robypark 2004/10/28 17:29
	// 마을이 캐릭터 소속 상단의 마을인지 검사
	// [IN] UI16 VillageCode: 마을 코드
	BOOL	GetVillageOurGuild(UI16 VillageCode);

public:
	// robypark 2004/11/1 16:20
	// 자신의 주변에 대장 유닛이 있는지 검사
	// return value : 공성전 유닛의 공격 타입.
	//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - 대장효과(공격력 업)에 영향을 받는다.
	//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - 대장 공격이다.
	//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - 일반 형태의 공격을 한다.
	//				-1									   - 유효하지 않은 공격이다.
	SI16 SiegeWarfare_IsInAreaGeneralEffect(void);

	// robypark 2004/11/8 16:23
	// 특정 캐릭터의 주변에 대장 유닛이 있는지 검사
	// [IN] UI16 uiUniqueID: 비교하려는 캐릭터 Unique ID
	// return value : 공성전 유닛의 공격 타입.
	//				ON_ATTACK_TYPE_NEARCAPTAIN	(DWORD(0)) - 대장효과(공격력 업)에 영향을 받는다.
	//				ON_ATTACK_TYPE_CAPTAIN		(DWORD(1)) - 대장 공격이다.
	//				ON_ATTACK_TYPE_NORMAL		(DWORD(2)) - 일반 형태의 공격을 한다.
	//				-1									   - 유효하지 않은 공격이다.
	SI16 SiegeWarfare_IsInAreaGeneralEffect(UI16 uiUniqueID);

	// robypark 2004/11/1 16:22
	// 공성전 유닛으로 변신하고자 할 때 자신 주변에 적대상단 마을의 범위 안에 있는지 검사
	// Return Value: TRUE  - 적 상단 마을 범위 안에 캐릭터가 존재하거나 본 맵이 아니다.
	//				 FALSE - 변신 가능한 곳에 캐릭터가 존재한다.
	BOOL	SiegeWarfare_IsInAreaDisabledTransform(void);

	// robypark 2005/1/13 21:39
	// 자신이 주변의 적대상단 마을의 범위 안에 있는지 검사
	// Return Value: TRUE  - 적 상단 마을 범위 안에 캐릭터가 존재.
	//				 FALSE - 근처에 적대 상단이 없는 곳에 캐릭터가 존재한다.
	BOOL	SiegeWarfare_IsInAreaHostileVillage(void);

	// robypark 2005/1/13 21:39
	// 자신이 주변의 아군상단 마을의 범위 안에 있는지 검사
	// Return Value: TRUE  - 아군 상단 마을 범위 안에 캐릭터가 존재.
	//				 FALSE - 근처에 아군 상단이 없는 곳에 캐릭터가 존재한다.
	BOOL	SiegeWarfare_IsInAreaOurVillage(void);

	// robypark 2005/1/13 21:45
	// 공성에 방해되는 일반 유저들(피난민)을 각국 초보자 수련장으로 워프시킬 수 있는지 검사
	BOOL	SiegeWarfare_CheckAbledWarfRefugee(UI16 uiDestUniqueID);

	// robypark 2004/11/2 15:0
	// 공성전 유닛 공격 속성에 따라 효과 사용하기
	void	SiegeWarfare_CreateAttackEffectCharacter(UI16 uiAttackerUniqueID, UI16 uiTargetUniqueID, bool bNeedToSendActionMsg = false);

	// robypark 2004/11/3 14:5
	// 공성전 유닛 공격 속성에 따라 효과 사용하기
	// 목표물의 위치에 생성
	void	SiegeWarfare_CreateAttackEffectVillage(UI16 uiAttackerUniqueID, SI16 ssTargetX, SI16 ssTargetY, UI16 uiVillageCode = 0);

	// robypark 2004/11/8 15:38
	// 공성전 유닛인지를 검사한다.
	BOOL	IsSiegeWarfareUnit(SI32 siImjinKindIdx);

	// robypark 2004/11/13 12:24
	// 공성전 유닛일 경우 공성전 유닛의 사용자 이름, 체력, 보급품, 등을 볼 수 있는지를 나타내는 플래그
	SI08	GetIsShowInformationAboutSiegeWarfareUnit(void)	{ return m_scIsShowInformationAboutSiegeWarfareUnit;	}
	void	SetIsShowInformationAboutSiegeWarfareUnit(SI08 scIsShowInformationAboutSiegeWarfareUnit);

	// robypark 2004/12/10 18:22
	// 수성하기 인터페이스 관련 변수
	LPSVillageDefenceInterface	GetVillageDefenceInterfaceInfo(void)	{ return &m_sVillageDefenceInterface; }

	// robypark 2004/11/24 16:52
	// 공성전 시간 등의 옵션 정보 구조체 얻기
	LPSOptionSiegeWarfare GetSOptionSiegeWarfare(void)	{ return &m_SOptionSiegeWarfare; }

	// robypark 2004/11/24 16:52
	// 공성전 시간 등의 옵션 파일 HQ에서 읽기
	BOOL LoadSOptionSiegeWarfare(char *pszServiceNation, SI16 siFindServiceType, char *pszFileName);

	// robypark 2004/11/13 16:49
	// 공성전 유닛일 때 마우스 타입 얻기
	// 다른 공성 유닛을 가리키는 경우

	// robypark 2004/11/13 16:49
	// 공성전 유닛일 때 마우스 타입 얻기
	// 마을을 가리키는 경우

	// robypark 2004/12/1 10:11
	// 사용자의 용병(주인공 포함) 인벤토리에 공성전 소집권 아이템이 있는지 검사한다.
	BOOL IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(void);

	// robypark 2005/1/8 16:26
	// 해당 슬롯의 용병의 인벤토리에 공성전 소집권 아이템이 있는지 검사한다.
	BOOL IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(UI08	uiFollowerSlot);

	// robypark 2004/12/2 17:31
	// 주인공 및 용병 인벤토리에 있는 모든 공성전 소집권 아이템을 삭제한다.
	void DeleteItemInInventoryAboutTransformationOfSiegeWarfareUnit(void);

	/////////////////////////////////////////////////
	// robypark 2004/12/27 10:46
	// 공성전 시간인지를 검사한다.
	BOOL	IsInTimeSiegeWarfare(void);

	// 공성전 대방이 소집권 아이템 변환(생산) 시간인지를 검사
	BOOL	IsInTimeRecruitArmyItem(void);

	// 공성전 대방이 소집권 아이템을 상단원에게 나줘 줄 수 있는 시간인지를 검사
	BOOL	IsInTimeGiveArmyItem(void);

	// 공성전 준비를 위하여 대방, 대행수, 행수가 상단 소속 마을에서 병력모집 가능 시간인지 검사
	BOOL	IsInTimeDisabledRecruitArmyInGovernment(void);

	// 사용자들이 상단 가입이 불가능한 시간인지를 검사
	BOOL	IsInTimeDisabledTimeToJoin(void);

	// 사용자들이 상단에서 탈퇴가 불가능한 시간(대방이 상단원 해고도 불가능하다) 인지 검사
	BOOL	IsInTimeDisabledTimeToWithdraw(void);

	// 폭탄투자 권한 처리 시간인지 검사
	BOOL	IsInTimeAllInInvestment(void);

	// robypark 2005/1/31 16:53
	// 공성하는 날인지 검사
	BOOL	IsInDaySiegeWarfare(void);

	// 사용자와 같은 상단의 상단원인지를 검사
	BOOL	IsMemberOfSameGuild(UI16 uiDestUniqueID);

	// 사용자와 같은 상단의 상단원인지를 검사
	// char *pszGuildName:	상대방 상단 명
	// SI32 siClassInGuild: 상대방 상단내 직급
	BOOL	IsMemberOfSameGuild(char *pszGuildName, SI32 siClassInGuild);

	// 캐릭터 A와 캐릭터B가 같은 상단의 상단원인지를 검사
	BOOL	IsMemberOfSameGuild(UI16 uiUniqueID, UI16 uiDestUniqueID);
	/////////////////////////////////////////////////

	// robypark 2005/1/8 11:18
	// 보급품 자동 소모 남은 지연시간 얻기
	SI32	GetRestTimeAutoSpendSupplyGoods(void);

	// robypark 2005/1/8 11:18
	// 보급품 자동 소모 남은 지연시간 설정
	void	SetRestTimeAutoSpendSupplyGoods(SI32 siRestTimeAutoSpendSupplyGoods);

	// robypark 2005/1/12 19:30
	// 공성전 공격 모드 설정 초기화
	void	CancelSiegeWarfare_AttackMode(void);

	// robypark 2005/1/14 17:11
	// 주인공 캐릭터들의 정보 관리자 얻기
	CFieldHeroInfoMgr	*GetFieldHeroInfoMgr(void)	{	return m_pFieldHeroInfoMgr;	}

	// robypark 2005/1/31 12:55
	// 공성 유닛 관리자
	CSiegeUnitTableMgr	*GetSiegeUnitTableMgr(void)
	{
		return m_pSiegeUnitTableMgr;
	}

	// robypark 2005/1/25 18:18
	// 다른 공성전 유닛을 치료 가능한가?
	BOOL	SiegeWarfare_CanHeal(UI16 uiUniqueID);

	// robypark 2005/2/15 15:30
	// 공성전 유닛이 치료가 가능한지 검사하여 마우스 커서 설정을 위한 마우스 타입 얻기
	// UI16 uiTargetUniqueID: 피격자의 UniqueID
	SI16	SiegeWarfare_CheckCursorCanHeal(UI16 uiUniqueID);

	// robypark 2005/1/26 18:22
	// 필드에 있을 때 마우스 커서 아래 오브젝트들에 관한 처리(Mouse Move)
	void	ProcessFieldMouseMove(BOOL bIsSiegeWarfareUnit, POINT ptMouse);

	// robypark 2005/1/26 18:22
	// 필드에 있을 때 마우스 커서 왼쪽 버튼 클릭 처리
	void	ProcessFieldMouseLeftBtnClick(BOOL bIsSiegeWarfareUnit, SI32 siCursorX, SI32 siCursorY);

	// robypark 2005/1/26 18:22
	// 필드에 있을 때 마우스 커서 오른쪽 버튼 클릭 처리
	void	ProcessFieldMouseRightBtnClick(BOOL bIsSiegeWarfareUnit);

	// 보급수레(메딕)일 경우 10초마다 마나가 10%씩 회복 정보 구조체 얻기
	LPSRecoveryMana		GetRecoveryManaInfo(void)
	{
		return &m_sRecoveryMana;
	}

	LPSWarningMsgInSiegeWarfare GetWarningMsgInSiegeWarfare()
	{
		return &m_sWarningMsgInSiegeWarfare;
	}
	// H2-S-05.02.04 //
	// 유저 어카운트로 캐릭터 명을 구한다.
	char*	OnlineCharName(UI16 nAccount);
	// H2-E-05.02.04 //

};
 

BOOL	IsOnlineWorld();																			// 현재 온라인 게임중인가?
BOOL	OnlineSendHeartBeat();																		// 게임이 살아 있다는 것을 게임 서버에 보내는 함수. 
CHAR	*GetItemName( UI16 uiItemID );																// 전투중 아이템 이름을 알아 온다.
OnlineNoticeList* GetOnlineNoticeListPointer();														// 온라인 상단 메시지관련 클래스의 포인터를 얻어온다.
BOOL	ManContactvillage(int VillageX, int VillageY, int VillageWidth, int VillageXHeight, int CharX, int CharY, int distance);
SI32	GetWarFieldMap(void);																									

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd,LPARAM lParam);
#endif 
