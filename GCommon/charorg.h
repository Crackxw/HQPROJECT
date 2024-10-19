#ifndef _CHARORG_H
#define _CHARORG_H

#include <GSL.h>

#include <CharAnimation.h>
#include "../SCCommon/ParametaBox.h"
#include "../SCCommon/OnlineMyWeapons.h"

#include "..\Gersang\order.h"

#include <Main.h>
#include <kindinfo.h>
#include "../GCommon/ElectronShock/ElectronShock.h"
#include "../GCommon/CharDraw/CharDrawInfo.h"
#include "../GCommon/PoisonEffect/PoisonEffect.h"

#define MAX_BUFFER_ORDER_NUMBER 10   // 명령을 저장해 둘 수 있는 최대 갯수 

#define ON_GUARDIAN_MOUSE			1
#define ON_GUARDIAN_CAW				2
#define ON_GUARDIAN_TIGER			3
#define ON_GUARDIAN_RABBIT			4
#define ON_GUARDIAN_DRAGON			5
#define ON_GUARDIAN_SNAKE			6
#define ON_GUARDIAN_HORSE			7
#define ON_GUARDIAN_SHEEP			8
#define ON_GUARDIAN_MONKEY			9
#define ON_GUARDIAN_CHICKEN			10
#define ON_GUARDIAN_DOG				11
#define ON_GUARDIAN_PIG				12

#define ON_GUARDIAN_GAME			1			//게임 중에 맞았을때.
#define ON_GUARDIAN_START			2			//게임이 시작되자 마자.

#define CHAR_DYING					0
#define CHAR_GENERALEX				1
#define CHAR_DEAD					2

// 캐릭터가 마비된 원인. 
enum HaltReason {	HALT_REASON_ICE = 1, 
					HALT_REASON_IVY = 2,

					// robypark 2004-6-3 16:15
					// 천지독살 냉기에 의한 마비 smoke, 흑룡차 기본 공격에 의한 마비 smoke
					HALT_REASON_FOG_OF_ICE = 3
				};

// 캐릭터 이동에 따른 리턴값. 
enum MoveInfo {	MOVEINFO_MOVING  =  0, /* 이동중인 과정에 있다. */
				MOVEINFO_MOVEDONE = 1, // 이동이 완료되었다. 
				MOVEINFO_HALFDONE = 2, // 중간 지지점에 도착했다. 
				MOVEINFO_REALMOVE = 3};// 실제로 유닛이 이동했음을 알려준다.

#define MAX_FLY_VIBRATE     8


// 캐릭터의 상태 정보. 
class cltCharStatus{
private:
	//-----------------
	// 캐릭터 종류 정보. 
	//-----------------
	SI32	m_siKind;			         // 어떤 종류의 obj인가?
	
	//-------------------
	// 플레이어 식별 정보. 
	//-------------------
	SI32	m_siPlayerNumber;                // 어느 편인가?
	SI32	m_siOriginalPlayerNumber;        // 생성될 당시에 어느 플레이어 소속이었는가?
	DWORD	m_uiChangedPlayerFrame;         // 플레이어가 변경된 프레임     

	//-------------------
	// 위치 정보 
	//-------------------
	etArea 	m_eArea;                        // 공중에 있는가 아니면 지상에 있는가?
	SHORT 	m_ssCenterX, m_ssCenterY;            // 캐릭터 이미지의 중심 좌표 (절대좌표) 


	//------------------
	// 구동관련 
	//------------------

	SI32 			m_siWorkDelay;
	SI32 			m_siWorkDelayor;

public:

	// 종류를 돌려준다.
	SI32 GetKind()const{return m_siKind;};
	void SetKind(SI32 kind){m_siKind = kind;}

	SI32 GetPlayerNumber()const{return m_siPlayerNumber;}
	void SetPlayerNumber(SI32 pnumber){m_siPlayerNumber = pnumber;}

	SI32 GetOriginalPlayerNumber()const{return m_siOriginalPlayerNumber;}
	void SetOriginalPlayerNumber(SI32 pnumber){m_siOriginalPlayerNumber = pnumber;}

	DWORD GetChangedPlayerFrame()const{return m_uiChangedPlayerFrame;}
	void SetChangedPlayerFrame(DWORD frame){m_uiChangedPlayerFrame = frame;}

	etArea GetArea()const{return m_eArea;}
	void   SetArea(etArea area){m_eArea = area;}

	void SetCenterX(SHORT centerx){m_ssCenterX = centerx;}
	SHORT GetCenterX()const{return m_ssCenterX;}

	void SetCenterY(SHORT centery){m_ssCenterY = centery;}
	SHORT GetCenterY()const{return m_ssCenterY;}

	void SetWorkDelay(SI32 workdelay){m_siWorkDelay = workdelay;}
	SI32 GetWorkDelay()const{return m_siWorkDelay;}

	void SetWorkDelayor(SI32 delayor){m_siWorkDelayor = delayor;}
	SI32 GetWorkDelayor()const{return m_siWorkDelayor;}

};


class cltCharOrg{

protected:
	SI32			m_siUpdateInfo;
	SI32			m_siOldUpdateInfo;

	char 			m_scSelectedSwitch;               // 선택되어 있는지 여부를 알려주는 변수 

	// robypark 2004/6/14 15:35
	// 죽기 전에 이 캐릭터가 선택이 되어 있었나를 나타내는 플래그
	BOOL			m_bAlreadySelectedBeforeDead;

	// 피격시 시각 효과 관련 
	BOOL			m_bBeAttackedDrawSwitch;
	SI32 			m_siBeAttackedDrawStep;

	// 공격 관련. 
	SI32			m_siWeaponAttackDelay;
	SI32 			m_siAttackDelayor;


	SI32 			m_siSearchRange;	// 시야

	char 			m_szName[32];		// 캐릭터 명

	SI08 			m_scSeeMode[MAX_MULTI_PLAYER];
	SI08 			m_scSeeStep[MAX_MULTI_PLAYER];
	SI32 			m_siDarkStep;		// Not Used
	SI32 			m_siTimerDelay;		// 이 타이머가 작동중이면 캐릭터는 시한부 인생이된다. 


public:

	// 전기 감전 관련 
	cltElectronShock m_cltElectronShock;

	// 중독 관련 
	cltPoisonEffect  m_cltPoisonEffect;

	// 캐릭터 출력과 관련된 정보. 
	cltCharDrawInfo  m_cltCharDrawInfo;

	// 캐릭터 상태 정보. 
	cltCharStatus	m_cltCharStatus;

	_ParametaBox	m_ParmeterBox;				  // 캐릭터의 행동을 제어하는 파라메타들의 클래스. 

	MyWeapons		MW;				  // 캐릭터의 인벤토리 상황 정보. 

//-------------------------------------------------

	unsigned char	ChangeDirectionDelayor, ChangeDirectionDelay;  // 방향 전환 지연 
	
	BOOL			ShallowWaterSwitch;         // 얕은 물 위에 있는가 ?
	BOOL			SwimSwitch;					// 수영중인가 ?


	DWORD			m_uiAtb, m_uiAtb2, m_uiAtb3, m_uiAtb4, m_uiAtb5;            // 객체의 속성 
	SI32			m_siAttackAtb1, m_siAttackAtb2;
	DWORD			m_uiDefenceAtb;
	SI32			DyingAtb;
	DWORD			WorkStep;					// 현재 어느 작업을 하고 있는가?

	SI32			ShieldDelay;				// 방어막이 있는가?

	BOOL			ClimbSwitch;				// 언덕을 올라가는 중인가?

	SI32			m_siCurrentAttack;			// 현재 진행중인 공격의 종류(공격이 복수로 존재할 때)
    // 범위 관련. 
	SI32 			m_siAttackRange1;			// 공격 사정거리1 
	SI32 			m_siAttackRange2;			// 공격 사정거리2 
	SHORT			m_ssPriority_Attack_Type;	// robypark 2004/10/18 12:36 - 두 가지 공격 속성이 있을 경우 인공지능 처리 관련 변수

	// 공격관련 
	_NewID 			AttackID;                   // 공격할 대상 아이디 
	char  			AttackArea;                 // 공격할 지역 
	SHORT 			AttackGiveUpDelayor;		// 공격 포기 지연시간
	SHORT 			AttackGiveUpDelay;			// 공격 포기 지연시간
	SHORT 			AttackMode;
	SHORT 			HitPointX, HitPointY;
	SHORT 			m_ssAttackX, m_ssAttackY;	// 공격 목표 위치 좌표
	SI08			m_scMagicApplyRate;			// 마법 적용률(최종 Damage 계산에 적용된다)

	char 			FireRepeatNumber;			// 몇 번 반복해서 공격했는가?
	char 			MaxFireRepeatNumber;		// 반복회수의 최대값 
	SHORT 			FireRepeatDelay;
	SHORT 			FireRepeatDelayor;

	char 			StrongAttackFrameNumber;

	UI32 			LastStrongAttackFrame;		// 마지막으로 필살기를 사용한 프레임.  

	// 귀신탄
	DWORD			GhostTime;

	// 풍모술
	DWORD			m_CloudWindFrame;

	// 은신 관련. 
	BOOL 			HideSwitch;
	DWORD 			HideFrame;
	BOOL 			bAssistHideSwitch;
	DWORD 			AssistHideFrame;

	// 위장 관련 
	BOOL 			CoverSwitch;

	//----------------------------------------------------------------
	//   애니메이션 관련 
	//----------------------------------------------------------------
	// 정지 관련 
	SHORT 			MaxWaitType;   // 이 유닛이 정지 애니메이션으로 가질 수 있는 애니의 갯수 
	SI32 			WaitType;       // 현재 이 유닛이 취하고 있는 정지 애니 
	SI32 			NextWaitType;   // 다음에 이 유닛이 취할  정지 애니 
	BOOL			WaitAnimationChangedSwitch;		// 정지 애니메이션간의 변경을 했는지 여부. 

	// 죽음관련 
	char 			DyingOperationSwitch;
	SI32 			DyingStep;
	
	SI32			DeadDelay;			// 죽은 후에 얼마나 지체하는가. 
	SI32			DeadDelayor;			// 죽은 후에 얼마나 지체하는가. 

	// 탐지관련 
	SHORT			BlackCheckX, BlackCheckY; 

	// 기어서 이동 관련 
	BOOL			LowMoveSwitch;

	// robypark 2004/6/4 13:33
	// 불랑기포차 변신 애니메이션과 관련된 변수
	SI32			m_siMagicAniStep;	// 변신 애니메이션 font인덱스

	// 공격 직전에 취하는 행동의 정보 
	char			ReadyToFireStep;

	SHORT			CurrentOrder;               				// 현재 캐릭터가 하고 있는 작업 
	SHORT			Step;                         				// 일이 진행중인 단계 
    char			HostMode;                     				// 명령을 준 것이 컴인가 플레이어인가?
	BOOL			m_bReverseSwitch;                			// 뒤집어서 찍을 것인가?

    _NewID			m_NewID;									// 캐릭터 자신의 아이디
	SHORT			CharOrderNumber;             				// 유닛이 배열에서 어느 위치에 있는가?
	SI32			m_siOnlineID;								// 온라인 캐릭터로서의일련번호. 

	SHORT			m_ssX, m_ssY;                  				// obj의 중심 위치 
	SI32			OldX, OldY;                   				// 유닛의 이전 위치. 
	SHORT			DX, DY;                       				// 목적지 
	SHORT			PX, PY;                       				// 이전에 있던 위치 
	SHORT			MOX, MOY;                     				// 이동의 기준이 되는 이전 좌표 
	SHORT			MDX, MDY;                     				// 이동의 기준이 되는 다음 좌표 

	char			m_scXsize, m_scYsize;                		// obj의 크기 
	SHORT			m_ssDrawStartX, m_ssDrawStartY;      		// 그림이 그려지는 시작 위치 
	SI32			ImageXsize, ImageYsize;       				// 액션시에 사용되는 그림의 가로 세로 사이즈 
	SHORT			RX, RY;                       				// 칸 단위로 이동하는 캐릭터의 변위 
	char			StepWidth;                    				// 이동 단위
		
	char			m_scIDXsize, m_scIDYsize;            		// 아이디 값이 표시되는 가로 세로 사이즈. 전투 맵에서 캐릭터가 차지하는 타일 개수
		
	SHORT			Direction;                     				// 방향 
	SHORT			TempDirection;                				// 16방위로 존재하는가?
	SHORT			TargetDirection;               				// 목표의 방위 
		


	//---------------------------------
	// 조합형 캐릭터 관련(Hero)
	SI16			HeroID;										// 몇번째 주인공 캐릭터인가
	SI16			DirectionNo;								// 캐릭터의 현재 방향에 맞는 폰트번호를 계산할 때 필요한 변수
	SI16			AniAction;									// Animation 그림을 찍는 Order상태
	SI16			CharBaseWClass;								// 무기에 따라 행동할 수 있는 동작 CLASS
	SI16			CharWeaponEnum;								// 캐릭터 애니메이션 속도
		
	SI16			AniStartFont;								// 캐릭터 애니메이션 현재 방향의 첫번째 폰트 번호(한 방향당 폰트 수 * DirectionNo)
	SI16			AniStep_Hero;								// 애니메이션 현재 단계
	SI16			ReserveAniStep;								// 다음에 찍을 animation font번호 

	CHAR			DefaultBody[5];								// 캐릭터의 디폴트 몸(예: KD01)


	_NewID			m_GuardianAttackID;

	SI16			m_ssWeaponID;									// 현재 장착한 무기
	SI16			m_ssPreWeaponID;								// 이전에 장착한 무기
	SI16			m_siPreGuardianID;
	SI16			m_siGuardianID;									// 수호부 ID
	SI16			m_siArmorDrawID;
	SI16			m_siPrevArmorDrawID;
	SI16			m_siHelmetDrawID;
	SI16			m_siPrevHelmetDrawID;
	SI16			m_siWeaponDrawID;
	SI16			m_siPrevWeaponDrawID;
	//
	//---------------------------------


	//-------------------------------------------------------------
	// 충돌 관련 변수. 
	//------------------------------------------------------------
	SHORT 			SelfBlock;                   				// 자체 충돌 속성 
	SHORT 			Block;                         				// 충돌하는 속성 

	char			m_scSeeSwitch;                				// 전장에 나와 있는가 없는가?
	char			m_scBusySwitch;                  			// 현재 바쁜가 바쁘지 않은가?

	int				Para1;
	_NewID			Para2;                  					// 명령의 파라메타 1, 2 


	RECT			RealRect;


	SHORT			AutoSwitch;									// 사용자의 명령을 받는가?
	SHORT			EventSwitch;								// 이벤트가 발생하는 유닛이다. 
																// 컴이 공격하지 않는다. 

	// 캐릭터 선택관련 
	clock_t			SelectedClock;								// 선택된 시간 
	char 			NewSelectedSwitch;
	char 			NewSelectedDelay;
	SI32 			Group;										// 어느 팀에 소속되어 있는가?


	// 죽음 관련
	SHORT 			CarcassDelay, CarcassDelayor;
		

	// 치료관련 
	SHORT 			HealDrawSwitch;                      // 치료 받는 모습을 그릴 것인가?
	SHORT 			HealDrawStep;                   
		
	// (거상) 레벨업 관련
	SHORT 			LevelUpDrawSwitch;                      // 치료 받는 모습을 그릴 것인가?
	SHORT 			LevelUpDrawStep;                   

	// 포박술 관련 
	SI32			m_siHaltDelay;                          // 정지하고 있어야 하는 시간. 
	SI32			m_siHaltReason;					  	  // 무엇 때문에 마비되었는가?
	SI32			m_siHaltStep;								

	//야수의 혼	
	SI32			m_siSoulDelay;						  // 야수의 혼 지속 시간.	
	bool			m_bSoulUse;							  // 야수의 혼이 사용중인가.
	SI32			m_siFontCount;						  // 야수의 혼 그래픽 폰트.
		
	// 보호막 관련 
	SHORT 			ProtectorDrawSwitch;
	SHORT 			ProtectorDrawStep;

	// 명령관련 
	SHORT 			BufferOrderIndex;                       
	SHORT 			PushOrderMode;
	_OrderDataInfo	BufferOrder;
	_OrderDataInfo	OldOrder;                              // 직전에 하던 명령을 저장할 공간 

	//수호부를 사용하는 상태인가.
	bool			m_bUseGuardian;
	SI16			m_siCount;
	DWORD			m_dwGuardianTime;

	SI16			m_siGuardianKind;
	SI16			m_siGuardianCondition;
	SI16			m_siGuardianSkillNum;

	UI16			m_uiGuardianRecallKind;
	SI16			m_siGuardianRecallNum;
	SI16			m_siGuardianNHitrate;
	SI16			m_siGuardianBHitrate;
	SI16			m_siGuardianSHitrate;
	SI16			m_siGuardianNoHaltHitrate;
	SI16			m_siGuardianAliveTime;

	bool			m_siGuardianTimeOut;
	bool			m_siGuardianNoTimeOutSkillUse;

	SI16			m_GuardianStartFile;		//수호부 캐릭터 폰트.

	SI32			targetid[50];
	SI32			targetidCount;

	//수호부로 만들어진 캐릭터인가.
	bool			m_bGuardianChar;

	//수호부를 가지고 있나.
	bool			m_bGuardianEquip;	

	//수호부 (토끼)관련.
	SHORT			MoveSpeed;		    // 이동 속도 
	float			m_MoveDelay;		// 움직이는 속도.
	float			m_AttackDelay;		// 공격하는 속도.
	
	bool			m_bRabbitAttack;	// 적캐릭터가 rabbit을 맞았는가.
	SHORT			m_RabbitShooterId;	// rabbit을 사용한 캐릭터의 아이디를 저장한다.

	//수호부 (소)관련.
	DWORD			m_dwCawTimer;		
	SHORT			m_siCawNumber;
	_NewID			m_idCaw;
	char			m_hostCaw;

	bool			m_bHorseAttack;		// 우리편 캐릭터가 Horse를 가지고 있는가.

	SHORT			m_siDeadCawX;
	SHORT			m_siDeadCawY;
	
	SHORT			m_siAttackX_ForGuardianDragon;	//적캐릭터의 X좌표. 수호부 용의 연옥술을 위한 좌표 값
	SHORT			m_siAttackY_ForGuardianDragon;	//적캐릭터의 Y좌표. 수호부 용의 연옥술을 위한 좌표 값

	bool			m_bGuardianLifeUp;			// 수호부 LifeUp
	bool			m_bMagicResist;				// 수호부 마방 Up
	bool			m_bDamageResist;			// 수호부 타저 Up

	SI16			m_siGuardianMaxLife;	// Guardian이 업데이트 할수 있는 Max Life
	SI16			m_siGuardianLife;		// Guardian이 업데이트 할수 있는  Life

	// 수호부(돼지)
	DWORD			m_dwPigAttackDelay;		// 돼지가 적을 삼키는 시간(3초)
	BOOL			m_bPigAttack;			// 수호부 공격을 행한 상태인가(돼지)
	BOOL			m_bPigDefence;			// 수호부 공격을 당한 상태인가(적군)

	bool			m_bTargetAttack;

	// 수호부(닭)
	_NewID			m_ChickenID[3];			// 병아리로 변할 몬스터 아이디
	SI32			m_siChickPrevKind;		// 병아리로 변하기 전 Kind

	// 예약 명령 
	SHORT			ReserveOrderIndex;                     // 예약버퍼에 명령이 몇개나 차 있는가?
	_OrderDataInfo	ReserveOrder[MAX_BUFFER_ORDER_NUMBER];

	_NewID 			HostID;                                // 주인이 되는 캐리터 (분신)
	_NewID 			SlaveID;									// 슬레이브의 아이디 
	_NewID 			Slave2ID;								// 슬레이브의 아이디 
			
	char  			OtherCollSwitch;                        // 자기 자리 외에 다른 공간에도 충돌 체크를 하는가?
	POINT 			OtherCollPoint;                         // 자기 자리 외에 다른 공간에 표시되는 충돌 체크의 위치 
	SHORT 			SetIDSwitch;                            // 유닛을 지도상에 설정했는지 여부를 기억한다.
			
	SHORT 			MenuItem   [MAX_MENU_NUMBER];
	SHORT 			MenuMode   [MAX_MENU_NUMBER];
			
	SHORT 			MenuNumber;
	SHORT 			UpdateMenuOrder;

	// 애니메이션 정보. 
	SI32 			Wait0CharAnimationIndex;
	SI32 			Wait1CharAnimationIndex;
	SI32 			MoveCharAnimationIndex;
	SI32 			LowMoveCharAnimationIndex;
	SI32 			DyingCharAnimationIndex;
	SI32 			AttackCharAnimationIndex;
	SI32 			Attack2CharAnimationIndex;
	SI32 			SwimCharAnimationIndex;
	SI32 			SwimWaitCharAnimationIndex;
	SI32 			AmbushCharAnimationIndex;
	SI32 			MagicCharAnimationIndex;
	SI32			Magic2CharAnimationIndex;
	SI32 			HealCharAnimationIndex;
	SI32			ManaCharAnimationIndex;
	SI32 			LowAttackCharAnimationIndex;
	SI32 			ReadyToFireCharAnimationIndex;

	SI32 			SatJEtcCharAnimationIndex;					// 가미가제 폭발 준비되었음을 알려주는 변수. 
	SI32 			MagicEtcCharAnimationIndex;				// 마법 사용 캐릭터의 특수 효과. 
	SI32 			FireHouseKEtcCharAnimationIndex;			// 조선 봉화대 화염 효과. 
	SI32 			EtcCharAnimationIndex;
	SI32 			ShipyardKEtcCharAnimationIndex;
	SI32 			TowerCEtcCharAnimationIndex;
	SI32 			AttackEtcCharAnimationIndex;
	SI32 			DyingEtcCharAnimationIndex;
	SI32 			General4kMagicEtcCharAnimationIndex;
	SI32 			General4jMagicEtcCharAnimationIndex;
	SI32 			InvalidCharAnimationIndex;
	SI32 			MakePortalCharAnimationIndex;				// 공간의 문 출구

	SI32			InGroundCharAnimationIndex;
	SI32			OutGroundCharAnimationIndex;

	// 자리사수 관련 
	SHORT			HoldSwitch;

	// 매복관련. 
	BOOL 			AmbushSwitch;
	SI32 			AmbushDelay;

	// 경계 관련
	SHORT 			GuardSwitch;               // 경계를 서는지 여부를 알려준다. 
	SHORT 			GuardX, GuardY;            // 경계를 서는 지역 

	// 피격관련 
	SHORT			AttackMePlayer;
	_NewID			AttackMeID;
	SHORT 			BeAttackedSwitch;
	SHORT 			NeedHelpSwitch;
    SHORT 			NeedHelpDelay;
	DWORD 			BeAttackedFrame;
	_NewID			PrevAttackMeID;

	_NewID			FireID;
	SHORT 			FireSwitch;
	SHORT 			FireDelay;

	SHORT 			NX, NY;       // 캐릭터의 다음 위치 
	SHORT 			MX, MY;       // 이동의 중간 기점 
	SHORT 			MoveBlockX, MoveBlockY;
			

	SHORT 			MoveBlockDelay;
	SHORT 			MoveBlockCount;

	SHORT 			FindAnotherPathSwitch;   // 다른 경로를 찾는지 여부를 설정한다. 
				
	SHORT 			MoveDelayor;
			
		

	SHORT			MoveSpeedVary;   // 여러가지 상황으로 인해 변경되는 가중치 
	
	SHORT			MovePercent;  // 몇 %를 이동하였는가? 

	SHORT 			WaitCount;
	SHORT 			Wait;


	SHORT 			SelfExpPercent;  // 자폭기능이 생길때까지의 퍼센트 
			

	SHORT 			Discover[MAX_MULTI_PLAYER];        // 보이는지 여부 
	SHORT 			DiscoverDelay[MAX_MULTI_PLAYER];   // 얼마나 보이는가?

			
	// 기타 출력관련 
	SHORT 			EtcDrawSwitch;
	SHORT 			EtcDrawType;
	SHORT 			EtcDrawStep;
			
	BOOL 			SearchForwardSwitch;	// 전방 탐색을 실시할 것인가?

	SI16			m_siReCallCount;

	BOOL			m_bChangePlayer;
			

public:
	cltCharOrg();
	void Init(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber,_ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid);

protected:
	//-------------------------------------------------------------------------
	// 매복 관련 
	BOOL			Ambush();

	//-------------------------------------------------------------------------
	// 지뢰 탐지 관련. 
	void 			DetectMine();
	BOOL 			CanDetectMine();

	void			HeroInit();								// 조합형 캐릭터 정보 초기화

public:
	
	// 속성을 구한다. 
    BOOL 			IsAtb(DWORD atb) const
	{			
		if( m_uiAtb & atb )
			return TRUE;
		return FALSE;
	}
	
	// 속성을 구한다. 
    BOOL 			IsAtb2(DWORD atb) const
	{			
		if( m_uiAtb2 & atb )
			return TRUE;
		return FALSE;
	}
	
	// 속성을 구한다. 
    BOOL 			IsAtb3(DWORD atb) const
	{			
		if( m_uiAtb3 & atb )	
			return TRUE;
		return FALSE;
	}

	// 속성을 구한다. 
	BOOL			IsAtb4(DWORD atb) const
	{
		if( m_uiAtb4 & atb ) 
			return TRUE;
		return FALSE;
	}
	
	// 속성을 구한다. 
	BOOL			IsAtb5(DWORD atb) const
	{
		if( m_uiAtb5 & atb ) 
			return TRUE;
		return FALSE;
	}

	// 모든 조건이 충족되어야 
	BOOL 			IsAtbAnd(DWORD atb) const
	{			
		if((m_uiAtb&atb)==atb)return TRUE;
		return FALSE;
	}
	// 속성을 구한다. 
     BOOL 			IsDyingAtb(SHORT atb)const
	{			
	  if(DyingAtb&atb)return TRUE;
	  return FALSE;
	}

	// 장수인지 여부를 알려준다. 
	 BOOL 			IsGeneral() const;
			
    BOOL 			IsAttackAtb(SI32 atb) const{	if(GetAttackAtb() ==atb) return TRUE;	return FALSE; };
    BOOL 			IsDefenceAtb(DWORD atb) const{	if(m_uiDefenceAtb==atb)return TRUE;	return FALSE; };

	/////////////   // 정지 관련 함수  ////////////////////////////////////
	void 			_SetWaitImage  (SHORT waittype, SHORT file, SHORT font, SHORT frame);
	void 			_SetWaitImage16(SHORT waittype, SHORT file, SHORT font, SHORT frame);
	void 			_SetWaitImage  (SHORT waittype, SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	/////////////   // 이동 관련 함수  ////////////////////////////////////
	void 			SetMoveImage(SHORT file, SHORT font, SHORT frame);
    void 			SetMoveImage16(SHORT file, SHORT font, SHORT frame);
	void 			SetMoveImage(SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	//////////////// 공격 관련 함수  /////////////////////////////////////
	void 			SetAttackImage(SHORT file, SHORT font, SHORT frame);
	void 			SetAttackImage16(SHORT file, SHORT font, SHORT frame);
	void 			SetAttackImage(SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	void 			DecideFontMove();
	void 			DecideFontWait();
	void 			DecideFontWaitNormal();
	void 			DecideFontWaitFly();

	void 			DecideFontAttackNormal();
	void 			DecideFontAttack16();

	// 공격 효과음을 얻어온다. 
	SI32 GetAttackEffect() const;

	// 공격 범위를 얻어온다. 
	SI32 GetAttackRange() const;
	// 공격 범위를 설정한다.
	void SetAttackRange(SI32 range);

	// 공격 속성을  얻어온다. 
	SI32 GetAttackAtb() const;
	// 공격 속성을 설정한다.
	void SetAttackAtb(SI32 atb);




////////////////////////////////////////////////////////////////////////////
///    예약 생산 관련 
////////////////////////////////////////////////////////////////////////////


	// 명령을 버퍼에 넣는다. 
    BOOL			PushOrder(_OrderDataInfo order, SHORT mode, SHORT debuginfo);
	// 예약 명령을 삭제한다. 
    void			ClearReserveOrder();
    // 예약 명령을 꺼내서 실행 버퍼로 옮긴다. 
    void			PopReserveOrder();
    // 명령을 예약 버퍼에 넣는다. 
    BOOL			PushReserveOrder(_OrderDataInfo order);
	// 주어진 명령이 예약 버퍼에 이미 들어 있는지 확인한다. 
    BOOL			HaveReserveOrder(SHORT order);
	// 현재 수행중인 명령이 무엇이었느냐에 따라 여러가지 복구를 시켜준다. 
    void			RecoverPrevOrder();

	BOOL			IncreaseLife(SHORT amount);
	BOOL			ReduceLife(SHORT amount, SHORT unique);

	BOOL			IncreaseMana(SHORT amount);
	BOOL			ReduceMana(SHORT amount);

	// 방향전환 관련 /////////////////////
    // 캐릭터의 방향을 전환한다. 
    BOOL			ChangeDirection(SHORT destdir);
    // 캐릭터의 방향을 전환한다. 
    BOOL			ChangeDirectionNormal(SHORT destdir);
    // 캐릭터의 방향을 전환한다. 
    BOOL			ChangeDirection16(SHORT destdir);

	// 유닛과 인접한 육지 섹터를 구한다. 
    SHORT			GetNearLandSector();

	//지금 좌표와 다음 좌표간의 백분율 비 변위를 구한다. 
    void			GetMoveRatio(int startx, int starty, int endx, int endy, int movepercent, SHORT& rx, SHORT& ry);


	// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
    BOOL			IsAbsPtInObj(SHORT x, SHORT y, SHORT& length);
	// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 
    BOOL			IsPtInObj(SHORT x, SHORT y);
    // 어떤 rect와의 교점이 존재하는가?
    BOOL			IsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
    // 어떤 rect와의 교점이 존재하는가?
    BOOL			IsAbsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2);



	void			SetSeeSwitch(SHORT mode);

	// 캐릭터가 x, y위치에 있을 수 있는가 없는가?
    BOOL			IsColl(SHORT x, SHORT y);
	// 캐릭터가 x, y위치에 있을 수 있는가 없는가?
    BOOL			IsColl(SHORT x, SHORT y, SHORT block);

	// 지도 위치 x,y에서 이루어지는 조치를 취한다. 
    void			OperateMapPosition(SHORT x, SHORT y);

	BOOL			SetSelect(int mode, int effectswitch);

	// 단축 아이콘에 있는 명령을 없앤다. 
    void			SetShortCutUnselect();

//------------------------------------------------------------------------
//  보물 관련. 
//------------------------------------------------------------------------
	void			GetTreasure(SHORT treasure);

	// 인근의 빈 자리에 보물을 내려 놓는다. 
    void			PutTreasureNear(SHORT type, SI32 amount);

	// 죽은 후 자리에 아이템을 남겨 놓는다.
	BOOL			ReleaseItem();

	// 피격시 표현 관련 함수. 
	void SetBeAttackedDrawSwitch(BOOL mode){m_bBeAttackedDrawSwitch = mode;}
	void SetBeAttackedDrawStep(SI32 step){m_siBeAttackedDrawStep = step;};

	// 메뉴를 업데이트한다. 
    BOOL			UpdateMenu(SHORT order);
	// 업그레이드 메뉴를 추가할 수 있는지 여부를 알려준다. 
    BOOL			CanPushUpgradeMenu(SHORT upgrade);
	// 장수 훈련을 메뉴에 추가할 수 있는지 여부를 알려준다. 
    BOOL			CanPushGeneralMakeMenu(SHORT kind);

	BOOL 			UpdateMenuNormal(SHORT order);


	BOOL 			SelectOperation();
	void 			PushMenu(SHORT number, int menu, SHORT upgrade=0, SHORT level=0, SHORT kind=0, SHORT needkind=0, SHORT money=0, SHORT tree=0);
	void 			PushMenuByOrder(SHORT number, int menu);


//--------------------------------------------------
//   이동 관련 함수 
//--------------------------------------------------
	// 캐릭터를 이동하게 만든다. 
    BOOL			SetMove(SHORT dx, SHORT dy);
	int 			Move(SHORT moveblockcount, SHORT range=0);
	int 			MoveNormal(SHORT moveblockcount, SHORT range);
	int 			MoveFly(SHORT moveblockcount, SHORT range);
	int				GetMovePercent(){return MovePercent;}
	
	SI32			UnderGroundMove();				// 지하로 이동하는 것 처리.


	void			CharMove();

	BOOL 			IsPreMoveOk();
	BOOL 			IsPreMoveOkSAMC();
	BOOL 			IsPreMoveOkFireTankK();

	void 			DoPreMove();
	BOOL			MoveDone();

//---------------------------------------------------
//  출력 관련 
//---------------------------------------------------

	// 포박된 상태인 경우 이 장면을 보여준다. 
    void			DrawHalt();


//-----------------------------------------------------------------------------------
//  보물 관련 
//-----------------------------------------------------------------------------------
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y);
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, BOOL diff_level_mode = TRUE);
	BOOL 			FindEmptyAreaTreasure(SHORT& x, SHORT &y);

	void			SetFileNFont();


//-----------------------------------------------------------------------------------
//  불끄기 관련 
//-----------------------------------------------------------------------------------
	BOOL 			SetExtinguish();
	BOOL			FindNearFire(int* six, int* siy);
	// 가까운 안전지대를 찾는다. 
	BOOL			FindNearFireNo(int* six, int* siy);
	// 인공지능으로 불을 끈다. 
	BOOL			ExtinguishAI();


	// 불 붙은 자리에 있을때의 대처 방안. 
	BOOL			FireOperation(int x,int y);

	//-------------------------------------------------------------------------
	// 필살기 관련

	// 특수 기술의 정보를 얻는다. 
	SI32 GetSpecialInfo(SI32 siAttackAtb, SI32 infotype);

	BOOL SpecialOperation(SI32 siSecial, _NewID id, SI32 x=0, SI32 y=0);
	BOOL CanSpecial(SI32 siSecial, _NewID id, SI32 *pReturnVal=NULL);
	BOOL SetSpecial(SI32 siSecial, _NewID id, SI32 *pReturnVal=NULL);
	BOOL Special(SI32 siSecial);
	SI32 SpecialAct(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y);

	BOOL CanSpecialAtb4(SI32 siSpecial, _NewID id);
	BOOL SetSpecialAtb4(SI32 siSpecial, _NewID id);
	BOOL SpecialAtb4(SI32 siSpecial);
	BOOL SpecialOperationAtb4(SI32 siSpecial, _NewID id, SI32 x=0, SI32 y=0);
	SI32 SpecialActAtb4(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y);

			
//-----------------------------------------------------------------------------------
// 유닛 인공지능 관련. 
//-----------------------------------------------------------------------------------
	// 시야를 처리한다
	void BlackCheck(); 
    void 			BlackCheckNormal();			// 일반 게임
	void 			BlackCheckHeroMode();			// 일기토 모드 게임
			
	// 전방 탐사 :근처에 적이 있는지 여부를 조사한다. 
    BOOL			SearchForward();

	// 일을 할 준비가 되어 있는가?
    BOOL			CanWork();

	// 적을 발견했을 경우 어떻게 대처하는지 결정한다. 
    // TRUE:적을 발견하고 적절한 조치를 취했다. 
    // FALSE:적에게 조치를 취할 수 없는 경우이다. 
    BOOL			OperateFindEnemy(SHORT x, SHORT y, _NewID id);

	BOOL			IsInRange(SHORT id, SHORT range);


	BOOL 			CanAttack(_NewID id);
    BOOL 			CanAttack(SHORT x, SHORT y);
	BOOL			CanAttackGround(SI32 six, SI32 siy)
	{	// 내 자신이 볼수 없는 상태라면, 
		if(m_scSeeSwitch==FALSE)return FALSE;
		
		return TRUE;
	}

			
	// 현재 공격중인 적과 id중에서 id를 공격하는 것이 더 좋겠는가?
    BOOL			IsGoodAttackNewEnemy(_NewID id);


	void			SetOldOrder(SHORT order, SHORT hostmode, SHORT clearswitch,
							int para1, _NewID para2);

	// 특정 방향으로 캐릭터를 밀어낸다. 
	BOOL Pull(SI32 dir);

	// robypark 2004/6/1 17:11
	// 특정 방향으로 캐릭터를 밀어낸다. 
	// SI32 dir[in]: 방향
	// SI32 siDistance[in]: 밀어낼 거리
	BOOL Pull(SI32 dir, SI32 siDistance);

//--------------------------------------------------------------------------------------------
// 애니메이션 관련. 
//--------------------------------------------------------------------------------------------
	void			SetAnimation(SI32 ani, SI32 * pdelayor = NULL);
	BOOL			DoAnimationDelay(BOOL animode, BOOL atb = 0);
	SI32			GetAniInstant();


	BOOL			FindEmptyAreaWater(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize);
	// 공중에서 빈자리를 찾는다. 
    BOOL			FindEmptyAreaAir(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize);

	//------------------------------------------
	// 조합형 캐릭터 관련
	BOOL			SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound);	// 캐릭터에게 장착된 아이템 정보를 셋팅한다.

	// 캐릭터의 경험치를 올려주고 전투 서버에 결과를 통보한다. 
	// showresultswitch: TRUE인 경우 화면에 경험치를 얻었다는 메시지를 보여준다. 
	void IncreaseExp(SI32 exp, BOOL serversendswitch, BOOL showresultswitch);

	SI32 GetApower();
	SI32 GetDpower();

	// 캐릭터의 이름을 얻어온다. 
	char* GetName() const;
	void SetName(char* name, SI32 namelength);

	// 공격 관련. 
	void SetWeaponAttackDelay(SI32 delay){m_siWeaponAttackDelay = delay;}

	SI32 GetTotalAttackDelay();
	SI32 GetTotalSearchRange()const{return m_siSearchRange;}
	void SetSearchRange(SI32 range){m_siSearchRange = range;}


	// 마비 설정 함수. 
	void SetHalt(SI32 haltmode, SI32 haltdelay);

	void SetUpdateInfo()
	{
		m_siUpdateInfo = m_ParmeterBox.IP.GetLife() + m_ParmeterBox.IP.GetMana() + (m_ssX/3) + (m_ssY/3);
	}

	void SetOldUpdateInfo()
	{
		m_siOldUpdateInfo = m_siUpdateInfo;
	}

	SI32 GetUpdateInfo()const
	{
		return m_siUpdateInfo;
	}

	SI32 GetOldUpdateInfo()const
	{
		return m_siOldUpdateInfo;
	}


	BOOL GetSeeStep(SI32 player) const;
	SI32 GetBlackByPlayerNumber(SI32 pnumber, SI32 x, SI32 y) const;
	BOOL GetSeeModeByPlayerNumber(SI32 pnumber) const;


	SI32 GetPlayerNumber()const{return m_cltCharStatus.GetPlayerNumber();}
	SI32 GetTimerDelay()const{return m_siTimerDelay;}

	void SetGuardianStartImage(SI16 m_siGuardianKind);
	SI16 GetGuardianStartImage();


//----------------------------------------------------------------------------
// robypark (2004/05/18 16:53) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	// 함수명: CreateBullet_SpecialNOVA
	// 파라미터:
	//			SI32 siX[in]: 기술을 사용하는 캐릭터 타일 위치(X)
	//			SI32 siY[in]: 기술을 사용하는 캐릭터 타일 위치(Y)
	//			_NewID OwnerID[in]: 기술을 사용하는 캐릭터 ID
	// 설명: 굉뇌(조선-뇌전차, 허준 2차 전직): 16방향으로 번개를 발사한다.
	void CreateBullet_SpecialNOVA(SI32 siX, SI32 siY, _NewID OwnerID);
	
//----------------------------------------------------------------------------
// robypark (2004/05/19 15:05) : 추가 기술 - 장수 2차 전직 특수 기술 관련,
//								 맹격화룡파를 위한 함수
//----------------------------------------------------------------------------
	// 함수명: GetDirection_ONSLAUGHT_FIREBALL
	// Parameter
	//			SI32 siX[in]: 기술을 사용하는 캐릭터 타일 위치(X)
	//			SI32 siY[in]: 기술을 사용하는 캐릭터 타일 위치(Y)
	//			SI32 siTargetX[in]: 목표 타일 위치(X)
	//			SI32 siTargetY[in]: 목표 타일 위치(Y)
	//			_NewID OwnerID[in]: 기술을 사용하는 캐릭터 ID
	// 설명: 맹격화룡파를 발사한다.
	void CreateBullet_ONSLAUGHT_FIREBALL(SI32 siX, SI32 siY, SI32 siTargetX, SI32 siTargetY, _NewID OwnerID);

	// robypark 2004-5-29 13:5 최종 수정
	// 캐릭터의 좌표와 목표물의 좌표를 입력받아 두 점이 이루는 직선과 맵의 끝 선분과의 교차점을 구해서 돌려준다.
	// SI32 x1[in]: 발사하려는 캐릭터 좌표(X)
	// SI32 y1[in]: 발사하려는 캐릭터 좌표(Y)
	// SI32 x2[in]: 목표의 좌표(X)
	// SI32 y2[in]: 목표의 좌표(Y)
	// SI32 &siTargetX[out]: 두 좌표가 이루는 직선과 맵 끝 선분과 교차점(X)
	// SI32 &siTargetY[out]: 두 좌표가 이루는 직선과 맵 끝 선분과 교차점(Y)
	void CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 x2, SI32 y2, SI32 &siTargetX, SI32 &siTargetY);

	// robypark 2004-5-29 13:15
	// 캐릭터의 좌표와 목표물의 좌표를 입력받아 두 점이 이루는 직선과 맵의 끝 선분과의 교차점을 구해서 돌려준다.
	// SI32 x1[in]: 발사하려는 캐릭터 좌표(X)
	// SI32 y1[in]: 발사하려는 캐릭터 좌표(Y)
	// SI32 siDelta_X[in]: 발사하는 캐릭터 좌표와 목표물의 위치 차이(X)
	// SI32 siDelta_Y[in]: 발사하는 캐릭터 좌표와 목표물의 위치 차이(Y)
	// float m[in]: 기울기 m
	// SI32 x2[in]: 목표의 좌표(X)
	// SI32 y2[in]: 목표의 좌표(Y)
	// SI32 &siTargetX[out]: 점(x1, y1)과 기울기 m이 이루는 직선과 맵 끝 선분과 교차점(X)
	// SI32 &siTargetY[out]: 점(x1, y1)과 기울기 m이 이루는 직선과 맵 끝 선분과 교차점(Y)
	void CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 siDelta_X, SI32 siDelta_Y, float m, SI32 &siTargetX, SI32 &siTargetY);
};

// x, y주변에 곡물이 얼마나 있는지 확인한다. 
SHORT	GetNearCropCount(SHORT x, SHORT y);





// 캐릭터의 클래스 
class _Char : public cltCharOrg
{
private:

public:

	// 전기 공격 관련 
	BOOL ElectronSwitch ;
	BOOL ElectronInfo1 ;
	BOOL ElectronInfo2 ;
	BOOL ElectronInfo3 ;
	BOOL ElectronInfo4 ;


	// 경험치 빼앗기 관련 
	SHORT StealExpDrawSwitch;
	SHORT StealExpDrawStep;                   

///////////////////////////////////////////////////////////////


	_Char();
	void CharInit(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid);

	// robypark 2004/6/9 12:32
	// 캐릭터가 변신할 때 사용
	// 이 함수는 CharInit(...)함수와 동일하다. 단지 SetInfo()함수를 호출하지 않는다.
	// SHORT CharOrderNumber: 유닛이 배열에서 어느 위치에 있는가?
	void CharInit_DISGUISE(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID);

	SHORT GetX()const{return m_ssX;}
	SHORT GetY()const{return m_ssY;}

	// 찍을 좌표를 계산한다. 
    void CalculateRealXY();
	// 찍을 좌표를 계산한다. 
    void CalculateDrawStartXY();
	void CalculateDrawStartXYInMapCaptureMode();


    void GetHitPoint(SHORT & x, SHORT &y);

	UI08 GetTeamColor()const;

	
	// 객체가  x,y에 있을때 dx, dy까지의 최단 거리 
    int  GetLengthToDest(SHORT x, SHORT y, SHORT dx, SHORT dy)
	{
	   return (dx-x)*(dx-x)+(dy-y)*(dy-y);
	}


	// id가 나의 최소 범위안에 있는지 확인한다. 
    BOOL IsInMinRange(SHORT id, SHORT minrange);

	// 명령을 새로 수여하기 전에 해야할 초기화 
    void InitPreGetOrder();



	void DecideAttackPoint(int port, int& x, int&y );



	///////////// 찍어야 할 그림을 선정한다. /////////////////////
    void DecideFont();
	void DecideFontNormal();
	void DecideFontLowMove();
	void DecideFontAttack(SI32 *file, SI32* font, BOOL* reverseswitch);
	void DecideFontAttack2(SI32 *file, SI32* font, BOOL* reverseswitch);
	void DecideFontSell();
	void DecideFontBuild();
	void DecideFontDead();

	//---------------------------------
	// 조합형 캐릭터 관련
    void DecideFontHero();



	void InitMenu(){};
	BOOL DrawShadow(SHORT darkstep);
	BOOL DrawShadowHero();			// 조합형 캐릭터(주인공)의 그림자 찍기
	

    // 필살기를 사용하여야 하는 상황인가 아닌가?
	BOOL IsStrongAttackAvailable();

	BOOL  Fire();
	//  각종 파라메타를 초기화한다. 
    BOOL ParametaAction();

	void GeneralEffect();


	BOOL CanSelfExp();
	BOOL SelfExp();
	BOOL SetSelfExp(SHORT x, SHORT y);

//////////////////////////////////////////////////////////
	// 인공지능 관련 
    BOOL AI();
/////////////////////////////////////////////////////////


	// player_number인 적에게 나를 보이게 한다. 
    void SetDiscover(SHORT player_number);

	// 적에게 공격받고 있을때 호출하는 함수 
    void SetBeAttackedSwitch(SHORT enemyid, SHORT attackmeplayer);
	// 적에게 공격받을때의 조치 
    void BeAttackedOperation();
	
	void OldOperation();

	// 캐릭터의 흔적을 남긴다 
    BOOL SetID();
	// 캐릭터의 흔적을 없앤다. 
    BOOL DeleteID();



	// 캐릭터가 생성되었음을 알린다. 
    void SetInfo();


	BOOL Action();

	// 비축된 명령을 꺼내서 수행한다. 
    BOOL GetOrderBuffer();

////////////////////////////////////////////////////////////////////////////
// 화면에 그려주기 
///////////////////////////////////////////////////////////////////////////
	// 캐릭터를 화면에 그려준다. 
	BOOL Draw();
	// 조합형 캐릭터
	BOOL DrawHero(unsigned char *darktable, SHORT shadowysize);
	void DrawEtcInChar(SHORT darkstep, unsigned char* darktable);
	
	SI32 DrawHealthBar();
	// 보호막을 그려준다.
    void DrawShield();
	// 치료력을 보여준다. 
    SI32 DrawHealBar(SI32 yindent);
	// 방어막의 내구력을 보여준다. 
    SI32 DrawShieldBar(SI32 yindent);
	// 경험치를 표시한다. 
    SI32 DrawExp(SI32 yindent);

	// 부대 번호를 표시한다. 
    void DrawTeamNumber();
	// 부대 번호를 표시한다. 
    void DrawLevel();
	void DrawSelectedBox(SHORT startx, SHORT starty, SHORT endx, SHORT endy);
	void DrawHeal();			// 치료받는 모습을 그려준다. 
	void DrawStealExp();
	void DrawProtector();

	void DrawGuardian();
	// 온라인에서 레벨업시 효과
	void DrawLevelUp();
	void DrawWangSoul();
	void DrawWangSoulBottom();
	BOOL GetSoulUse(){return m_bSoulUse;}

	// 초상화를 그린다. 
    void DrawPortrait(LPDIRECTDRAWSURFACE7 surface);
    void DrawHealth();
	// 마법력을 보여준다. 
    void DrawMagic();
	void DrawDpower();
	void DrawApower();
	void DrawCrew();
	void DrawCharInfo();		// 온라인 전투 모드에서 캐릭터 정보창을 보여준다.
	// 캐릭터 정보를 보여준다. 
    void ShowCharInfo(LPDIRECTDRAWSURFACE7 surface) ;

	// robypark 2004/10/6 15:28
	// 캐릭터의 간략한 정보를 보여준다.
	// 초상, 체력, 마법
	void ShowCharInfoSimple(LPDIRECTDRAWSURFACE7 lpSurface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:42
	// 위치, 크기 인자 사용
	// 초상화를 그린다. 
	void DrawPortrait(LPDIRECTDRAWSURFACE7 surface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:24
	// 남은 체력을 보여준다. 
	void DrawHealth(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:24
	// 마법력을 보여준다. 
	void DrawMagic(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	void SetHealDraw(SHORT mode);
	// (거상) 레벨업 효과를 그려주는가
	void SetLevelUpDraw(SHORT mode);	
	void SetProtectorDraw(SHORT mode);
	void SetStealExpDraw(SHORT mode);

	SHORT GetSelectedSwitch(){return m_scSelectedSwitch;}
	void SetNewSelectedSwitch(SHORT mode);

	/////////////////////////////////////////////////////
    ////// 건설 관련 
	BOOL Sell();
	BOOL Build();
	// 보조건물을 입력된 위치(x,y)에 따라 지을 좌표를 구한다. 
    BOOL GetBuildAddOnXY(SHORT kind, SHORT x, SHORT y, SHORT &dx, SHORT &dy);
	// kind 보조건물을 가지고 있는가?
    // 건물로서 유효해야 한다. 
    BOOL HaveAddOnBuild(SHORT kind);
	BOOL Repair();
	void BuildToCar();

	// 건물을 지을 수 있는가?
    BOOL CanMake();
	BOOL SetMake(SHORT x, SHORT y, SHORT kind);
	void Make();
	void MakeNormal();
	void MakeHQC();
	void HelpBuild();

//////////////////////////////////////////////////////////////

	// 병사 생산 관련 
	void  Train();

	////////////// 공격 관련 함수  ///////////////////////////////////////
    //캐릭터 전투 
    BOOL Attack();
	BOOL TryAttack();
	BOOL TryAttackGround();
	BOOL Escape();

	// fire직전에 확인 
    BOOL ReadyToFire();

    ////  attack_id 적을 공격하게 만드는 함수 
    BOOL SetAttack(_NewID id);
	////  지상을 공격하게 만드는 함수 
	BOOL SetAttackGround(SI32 six, SI32 siy);

    // 본격적인 전투에 들어가기 전에 처리해야 할 것을 처리한다. 
    BOOL PreAttack();

	// robypark 2004/6/4 13:33
	// 불랑기포차 변신을 위한 함수
	// 앙천대소 시즈모드 => 일반 불랑기포차
	BOOL ChangeModeFarangi(void);

	// robypark 2004/6/4 13:33
	// 불랑기포차 변신을 위한 함수
	// 일반 불랑기포차 => 앙천대소 시즈모드
	BOOL ChangeModeRoarOfLaughter(void);

	// robypark 2004/6/10 11:14
	// 앙천대소에서 즉시 불랑기포차로 변신한다.
	BOOL ChangeModeFarangi_Immediately(void);

	// robypark 2004/6/15 18:13
	// 불랑기포차에서 즉시 앙천대소로 변신한다.
	BOOL ChangeModeRoarOfLaughter_Immediately(void);

	/////////////  죽음 관련 함수 ///////////////////////////////////////
	BOOL DyingOperation();
	BOOL DyingOperationNormal();
	SI16 Dying();
	BOOL Dead();
	BOOL GeneralExDying();
	// 사망시 잃게 되는 경험치를 구하는 함수. (레벨에 따라 차등적으로 경험치를 잃게 된다.)
	SI32 GetExpReduceForDying();

	// 캐릭터 소지금
	void GetOnlineUserMoney(MONEY money);

 
    //////// 적 건물 점령에 관한 함수 /////////////////////////////////////
	// id를 점려할 수 있는가 ?
    BOOL CanCaptureBuild(_NewID id);
	BOOL SetCapture(_NewID id);
	BOOL CaptureBuild();
    // 플레이어 번호를 바꿔준다. 
    BOOL ChangePlayer(SHORT destpnumber);
	BOOL ChangePlayerMagic(SHORT destpnumber,_NewID id );

	BOOL SetBetray(_NewID id, int* preturnval);
	// id를 현혹할 수 있는가?
    BOOL CanBetray(_NewID id,  int* preturnval);
	BOOL Betray();
	BOOL BetrayOperation(_NewID id);


	BOOL SetGoIntoHQ(_NewID id);
	BOOL CanGoIntoHQ(_NewID id);
	BOOL GoIntoHQ();

///////////////////  적에너지 흡수  ///////////////////////////////////////
	BOOL SetAbsorb(_NewID id);
    // id의 체력을 흡수할 수 있는가?
    BOOL CanAbsorb(_NewID id);
	BOOL Absorb();
	BOOL AbsorbOperation(_NewID id);

///////////////  기술훔치기관련 함수  ////////////////////////////////////
    BOOL SetStealTech(_NewID id);
	// id의 기술을 훔칠 수 있는가?
    BOOL CanStealTech(_NewID id);
	BOOL StealTech();
	BOOL StealTechOperation(_NewID id);


	///// 순찰관련 함수 /////////////////////////////////////////////////////
	BOOL SetPatrol();
	BOOL Patrol();

	///// 마법 보조 관련  /////////////////////////////////////////////////
	// 주변의 마법 캐릭터로부터 마법력을 얻어온다.
    void UseBuda();
	// 마법 보조를 행할 수 있는지 여부를 알려준다. 
    BOOL CanUseBuda();

	//// 경험치 빼앗기 ////////////////////////////////////////////////////
	BOOL CanStealExp();
	void StealExp();

	//// 공중 공격 마법 ////////////////////////////////////////////////////
    void AirMagic();
	// 마법 보조를 행할 수 있는지 여부를 알려준다. 
    BOOL CanAirMagic();

    /// 치료 관련 함수 /////////////////////////////////////////////////////
	BOOL HealOperation(_NewID id);
	// 자신이 치료가능한가?
    BOOL CanHeal();
    // id를 치료할 수 있는가? 
	BOOL CanHeal(_NewID id);
	BOOL SetHeal(_NewID id);
	BOOL Heal();
	void MassHeal();
	BOOL HealAI();

	//마나 회복 관련 함수.
	BOOL ManaOperation(_NewID id);
	// 자신이 마나 치료가능한가?
    BOOL CanMana();
	// id를 마나 치료할 수 있는가? 
	BOOL CanMana(_NewID id);
	BOOL SetMana(_NewID id);
	BOOL Mana();
	void MassMana();




	////// 기우제 관련  ///////////////////////////////////////////////////
	BOOL CanRainMake();
	void RainMake();

	//////////////////////////////////
	//  지진 관련 
	//////////////////////////////////
	BOOL EarthQuakeOperation(_NewID id);
	BOOL EarthQuake();
	// 자신이 지진공격이 가능한가?
    BOOL CanEarthQuake();
	BOOL CanEarthQuake(_NewID id); 
	BOOL SetEarthQuake(_NewID id);

	//////////////////////////////////
	//  화공신포
	//////////////////////////////////
	BOOL FireAttackOperation(_NewID id);
	BOOL FireAttack();

	BOOL CanFireAttack();
	BOOL CanFireAttack(_NewID id);
	BOOL SetFireAttack(_NewID id);

	//////////////////////////////////
	//  번개 관련 
	//////////////////////////////////
	BOOL LightningOperation(_NewID id);
	BOOL Lightning();
	// 자신이 지진공격이 가능한가?
    BOOL CanLightning();
	BOOL CanLightning(_NewID id);
	BOOL SetLightning(_NewID id);

	//야수의 혼.
	BOOL Soul();
	BOOL SoulOperation(_NewID id);
	BOOL CanSoul();
	BOOL SetSoul(SI32 siSoulDelay);

	//장창소환
	BOOL LongSpear();
	BOOL LongSpearOperation(_NewID id);

	BOOL CanLongSpear();
	BOOL CanLongSpear(_NewID id);
	BOOL SetLongSpear(_NewID id);




    ///////////////////////////////////////////////////////////////////////
	// 업그레이드관련 
	//////////////////////////////////////////////////////////////////////
	void  UpgwadeFunction();


	//////////////////////////////////////////////////////////////////////
	// 자신의 분신술 관련 
	//////////////////////////////////////////////////////////////////////
	BOOL CanClon(_NewID targetnewid);
	BOOL Clon();
	//////////////////////////////////////////////////////////////////////
	// 소환 
	//////////////////////////////////////////////////////////////////////
	BOOL CanReCallDamduk();
	BOOL ReCallDamduk();		// 모든 소환 공통
	BOOL CanReCall();
	BOOL ReCall();
	BOOL ReCallPanda();
	BOOL ReCallGuardianChar();						    //	수호부 캐릭터(쥐,토끼)
	BOOL PandaRecallOperation(_NewID targetnewid);		//	수호부 캐릭터(쥐,토끼)
	BOOL RecallGuardianCaw();						    //	수호부 캐릭터(소) Order에 따라 호출되는 함수.
	BOOL StartRecallGurdianCaw(_NewID id,char host); 	//	수호부 캐릭터(소)가 시작된다.
	BOOL IsRecallGuardianCaw(){return m_siCawNumber;};	//  수호부 캐릭터가 있는가.
	BOOL DeleteRecallGuardianChar();					//  수호부 아이템에 따라 생성된 캐릭터를 삭제한다.
	BOOL DeleteRecallGuardianCaw();						//  수호부 (소) 가 죽는다.

	BOOL GetAtbSkill(SI16 id,SI16 targetid);

	BOOL AttackMagicGuardianRabbit();
	BOOL DeleteGuardianRabbit();						//
	BOOL MagicGuardianRabbit(_NewID attackID);			//  수호부 

	BOOL DeleteGuardianHorse();
	BOOL MagicGuardianHorse();

	BOOL DeleteGuardianLifeUp();					//  수호부 (양) 체력 업
	BOOL MagicGuardianLifeUp();						

	BOOL MagicGuardianMagicResistUp();					//  수호부 (개) 마저 업
	BOOL DeleteGuardianMagicResistUp();

	BOOL MagicGuardianDamageResistUp();					//  수호부 (개) 타저 업
	BOOL DeleteGuardianDamageResistUp();					
	
	BOOL DrawGuardianUse();

	void GuardianBreakless(SI16 siKind);

	
	BOOL	AttackMagicGuardianTiger();
	BOOL	AttackMagicGuardianChicken();
	BOOL    MagicGuardianChicken(_NewID attackID);

	_NewID	FindNearEnemy(SI16 siX,SI16 siY);
	
	GroundExp();
	GroundExpOperation(_NewID id,SI16 x,SI16 y);
    ///////////////////////////////////////////////////////////////////////
    //  방어막 관련 함수 
    //////////////////////////////////////////////////////////////////////
    BOOL SetShield(_NewID id);
	BOOL CanShield();
	// id에게 방어막을 줄 수 있는가? 
    BOOL CanShield(_NewID id);
	BOOL Shield();
	BOOL ShieldOperation(_NewID id);
 

	///////////////////////////////////////////////////////////////////////
    //  공간이동 관련 함수 
    BOOL SetWarp();
	BOOL CanWarp();
	BOOL Warp();


	///////////////////////////////////////////////////////////////////////
	//  지뢰 관련 
	BOOL CanMine();
	// 지뢰를 심을 수 있는가?
    BOOL	CanMine(SHORT x, SHORT y);
	BOOL	SetMine(SHORT x, SHORT y);
	BOOL	Mine();
	BOOL	MineOperation(SHORT x, SHORT y);
	BOOL	SearchForMineSpot(SI32 *px, SI32 *py);						// 지뢰를 매설할 수 있는 장소를 찾는다.

	// 전투에 따라 이벤트 마크와 전투지역설정등을 담당한다. 			 
    void	SetBattleFieldNEventMark();

	// 공격에 대한 정보를 저장한다. 다른 곳에서 현재 이 유닛이 전투중인지를 판단하는 근거..
    void	SetFireInfo(_NewID id);

	// 정지시 행동 
    BOOL	OperateWait();
	SI16	GetUpgradeMakeKind(BOOL bUpgrade);

	// damduk : 수호부(돼지)(닭)
	BOOL	ActionPig();
//	BOOL	AttackGuardianChicken();

	void FireOperation_Sword(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Dueoksini(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Toad(SI32 attackinstant, SI32 hitrate);
	void FireOperation_CannonJ(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Elephant(SI32 attackinstant, SI32 hitrate);
	void FireOperation_BigSword(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SeaKing(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Gun(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Electron(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GatphaPoison(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GhostCat(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic3(SI32 attackinstant, SI32 hitrate);
	void FireOperation_MagicBall(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Arrow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Needle(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Snowman(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FurHeadDemon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Dart(SI32 attackinstant, SI32 hitrate);
	void FireOperation_AirMissile(SI32 attackinstant, SI32 hitrate);
	void FireOperation_AttackerC(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SamMissile(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_TowerCannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_HeavyGun(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ShipCannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ThrowBomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cannon2C(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Bomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FireCar(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Fire(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FireUsingMana(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GoldDragonElectron(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Onghwa(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Doll1(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Doll2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Kuket(SI32 attackinstant, SI32 hitrate);

	//인형 무기 추가. 
	///////////////////////////////////////////////////////////////////////
	void FireOperation_CatDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_OldFoxDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_RaccoonDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_NinjaDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SnowWomanDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_YangGuiBiDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_JijangDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SnakeWomanDoll(SI32 attackinstant, SI32 hitrate);
	///////////////////////////////////////////////////////////////////////

	void FireOperation_Snow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Justicek(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Butterfly(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Widow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Wave(SI32 attackinstant, SI32 hitrate);
	void FireOperation_KimBomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Leech(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SeaHorse(SI32 attackinstant, SI32 hitrate);
	void FireOperation_PoisonArrow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ThrowSpear(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cactus(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Scorpion(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Claw(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magician(SI32 attackinstant, SI32 hitrate);
	void FireOperation_BigTree(SI32 attackinstant, SI32 hitrate);
	void FireOperation_VolcanoFlame(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Evil(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Ice(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic4T(SI32 attackinstant, SI32 hitrate);
	void FireOperation_JapanGhost(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Auta(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sub1(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sub2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_PaperDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_NeedleAttack(SI32 attackinstant, SI32 hitrate);
	void FireOperation_CloudWind(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Seimei(SI32 attackinstant, SI32 hitrate);
	void FireOperation_MagicCAttack(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sunbiyoun(SI32 attackinstant, SI32 hitrate);

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 유명계 관련 START
//----------------------------------------------------------------------------

	void	FireOperation_Python		( SI32 attackinstant, SI32 hitrate );
	void	FireOperation_JinEmperor	( SI32 attackinstant, SI32 hitrate );

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 유명계 관련 END
//----------------------------------------------------------------------------

//	void FireOperation_Rabbit(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GuardianSnake(SI32 attackinstant, SI32 hitrate);

//----------------------------------------------------------------------------
// damduk : 추가 캐릭터 - 수호부 관련
//----------------------------------------------------------------------------
	void FireOperation_GuardianPig(SI32 attackinstant, SI32 hitrate);


//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : 장수 2차 전직 관련 추가
//----------------------------------------------------------------------------
	// 조선-거북차, 이순신 2차 전직
	void FireOperation_TURTLE_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 조선-뇌전차, 허준 2차 전직
	void FireOperation_THUNDER_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 일본-지진차, 와키자카 2차 전직
	void FireOperation_QUAKE_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 일본-흑룡차, 세이메이 2차 전직
	void FireOperation_BLACK_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 중국-발석거, 이령 2차 전직
	void FireOperation_CATAPULT(SI32 attackinstant, SI32 hitrate);
	// 중국-불랑기포차, 순비연 2차 전직
	void FireOperation_FARANGI_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 대만-화룡차, 손유창 2차 전직
	void FireOperation_FIRE_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// 대만-봉황비조, 장선화 2차 전직
	void FireOperation_CHINESEPHOENIX(SI32 attackinstant, SI32 hitrate);

	// robypark (2004/6/4 13:6)
	// 앙천대소 공격, 불랑기포차, 126
	// ATTACKATB_ROAR_OF_LAUGHTER
	void FireOperation_ROAR_OF_LAUGHTER(SI32 attackinstant, SI32 hitrate);

	// robypark 2004/7/14 16:34
	// 에비스 인형 공격
	// ATTACKATB_DOLL_EVIS
	void FireOperation_DOLL_EVIS(SI32 attackinstant, SI32 hitrate);

	// robypark 2005/2/2 17:27
	// 달마 인형 공격
	// ATTACKATB_DOLL_DHARMA
	void FireOperation_DOLL_DHARMA(SI32 attackinstant, SI32 hitrate);
};



// id를 치료할 수 있는가?
// id자체의 정보만을 가지고 확인한다. 
BOOL CanHealGlobal(_NewID id);
BOOL CanManaGlobal(_NewID id);

#endif
