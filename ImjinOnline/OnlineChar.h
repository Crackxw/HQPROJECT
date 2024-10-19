/************************************************************************************************
	파일명 : OnlineChar.cpp
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_CHAR_H
#define _ONLINE_CHAR_H

#include <graphics16.h>
#include "GSCDefine.h"
#include "OnlineCommonStruct.h"


#define IMOTICON_WAITTIME		5000						// 이모티콘이 떠 있는 시간
#define	MAX_ONLINE_CHAR			400
#define MAX_ONLINE_DEFAULT_CHAR 12

#define	MAX_ORDER_BUFFER_NUM	50

// 캐릭터 명령들
#define	ON_ORDER_NONE			0		// 명령이 없다.
#define	ON_ORDER_WAIT			1		// 정지 명령.
#define	ON_ORDER_MOVE			2		// 이동 명령.
#define	ON_ORDER_ATTACK			3		// 공격 명령.
#define	ON_ORDER_USEOBJECT		4		// 오브젝트를 사용한다.
#define	ON_ORDER_PICKUPITEM		5		// 아이템 줍기 명령.

#define	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE		6	// 마을 공격하기
#define	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION	7	// 마을 공격하기 동작 들어감

#define	ON_ORDER_NPC			8		// NPC

#define ON_ORDER_FIELDATTACK_READY		9				// 공성 사람대 사람간.
#define ON_ORDER_FIELDATTACK_ACTION		10
#define ON_ORDER_FIELDATTACK_DEAD		11

#define ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY		12  //공성 원거리에서 마을 공격.
#define ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION		13	//

// robypark 2004/11/9 17:2
#define ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON	14	// 공성전 보급 수레로부터 보급품 요청명령

// robypark 2004/11/9 17:2
#define ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE		15	// 공성전 보급 수레가 마을에 보급 요청명령

// robypark 2005/1/25 18:52
#define ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL			16	// 공성전 보급 수레가 다른 유닛 치료 요청

// robypark 2005/1/25 18:52
#define ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION	17	// 공성전 보급 수레가 다른 유닛 치료 행동

// robypark 2005/1/27 18:49
// 마을 수성인터페이스로 들어가겠다
#define ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE	18

// 캐릭터 이동 상수.
#define	ON_MOVE_DONE			1		// 이동이 종료되었다.
#define	ON_MOVE_MOVING			2		// 계속 이동중이다.

#define	ON_CHAR_4D_PF_NEED		1		// 4방향 길찾기가 필요하다.
#define	ON_CHAR_4D_PF_NO_NEED	2		// 4방향 길찾기가 필요하지않다.
#define	ON_CHAR_4D_PF_STOP		3		// 정지해야 한다.

#define	ON_CHAR_WAPR_DISTANCE	5		// 5 타일이 서버와 차이가 나면 워프를 한다.

// robypark 2004/11/9 17:17
// 공성전 유닛과 보급수레와의 보급이 가능한 거리
#define VALID_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON				6

// robypark 2005/1/25 19:59
// 공성전 유닛과 보급수레와의 치료가 가능한 거리
#define VALID_HEAL_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON		3

// robypark 2004/12/6 10:23
// 공성전 유닛이 보급품 관련 효과 설정 상수. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
#define EFFECT_CONSTANT_SUPPLYGOODS_FILLED			1
#define EFFECT_CONSTANT_SUPPLYGOODS_RANOUT			2

// 공성전 변신 이펙트 설정 상수
#define EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM		1	// 공성전 유닛으로 처음 변하는 경우
#define EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM	2	// 공성전 유닛인 상태에서 변신하는 경우

// robypark 2004/11/11 14:0
// 공성전 유닛일 때 아군, 적군에 관한 상수 선언
#define SIEGEWARFARE_FORCE_FRIENDLY				0		// 아군
#define SIEGEWARFARE_FORCE_ENEMY				1		// 적군
#define SIEGEWARFARE_FORCE_UNRELATED			2		// 무관한. 적대 상단원이 아니며 같은 상단원이 아니다.

// robypark 2005/1/27 13:19
#define VDI_ANIMATION_TYPE_REPAIR_HAMMER		1		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 망치질
#define VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER	2		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 망루 수리
#define VDI_ANIMATION_TYPE_REPAIR_SAW			3		// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 톱질

struct	OnOrder
{
	SI32	siOrder;
	SI32	siOrderParam1;
	SI32	siOrderParam2;
};

class	cltOnlineChar
{
private:
	BOOL	m_fShow;
	SI32	siID;					// 배열상의 ID(즉 인덱스이다.)
	UI16	uiUniqueID;				// 이 캐릭터만의 ID.

	BOOL	m_bAttackMode;

	int		m_nScore;
	int		m_nRank;
	BOOL	m_bInvalidBattle;

	UI08	m_BattleFlag;					// Battle Flag
	DWORD	m_BattleFlagStartTickCount;		// Battle Flag Start Tick Count
	SI32	m_BattleFlagStartCheckCount;	// Battle Flag Start Check Count

	UI08	m_FindTargetFlag;		// Find Target Flag

	SI32	m_siDirection;		
	SI32	siClass;
	SI32	siKind;
	UI16	uiEquip[WEAR_ITEM_NUM];
	float	fRanking;
	SI32	siFightPower;	

	SI08	sifDataType;								// Data 상항, 0 : 데이터가 모두 있음, 1 : Detail Data가 없음, 2 : Detail Data 대기중

	// 캐릭터의 명령.
	OnOrder	PrevOrder;									// 이전 프레임의 명령.
	OnOrder	CurrentOrder;								// 현재 명령
	OnOrder	OrdersBuffer[MAX_ORDER_BUFFER_NUM];			// 다음 프레임에서 수행해야 하는 명령들.
	SI16	siOrdersBufferCount;						// 명령 버퍼의 카운트.

	// 캐릭터의 한 프레임 이전 좌표.
	SI16	siPrevX, siPrevY;

	// 캐릭터의 이전 좌표.
	SI16	siPrevX1, siPrevY1;

	// 캐릭터의 좌표.
	SI16	m_siX, m_siY;
	SI16	m_siRX, m_siRY;

	// 이동해야 할 목적지의 중간 타일 좌표
	SI16	m_siMDX, m_siMDY;
	SI16	m_siMX, m_siMY;
	SI16	m_siMOX, m_siMOY;
	SI16	m_siMoveSpeed;
	SI16	m_siMovedPercent;
	SI32	m_siMoveableTileInfo;

	// 이동해야 할 타일 좌표.	
	SI16	m_siDX, m_siDY;
	SI16	m_siNX, m_siNY;

	// robypark 2004/12/17 16:52
	// 이동하려는 좌표를 이전에 전송하였던 값을 저장하고 있는 변수
	SI16	m_siPrevSendedNX, m_siPrevSendedNY;

	// robypark 2004/12/16 10:42
	// 캐릭터의 좌표를 전송하였는지를 나타내는 플래그
	BOOL	m_bIsSendedPlayerPos;

	cltOnlineWorld	*pMyOnlineWorld;		// Online World

	// 캐릭터 애니메이션 화일의 대한 정보.	
	SI32	siAniActionCode;
	SI32	siAniDirection;
	
	// 현재 이동중인 정보.	
	SI32							siMovingDirection;
	SI16							siMovingX;						// 이동중인 X 좌표.
	SI16							siMovingY;						// 이동중인 Y 좌표.

	// 현재 캐릭터의 상태
	BOOL							bBusySwitch;

	SI16							siPutSpeechCX, siPutSpeechY;	// 대사를 찍을 좌표.(X는 중심 좌표이여서 CX, Y는 텍스트의 하단 좌표이다.)
	CHAR							szSpeech[1024];					// 대사.
	SI32							siSpeechSettingFrame;			// 스피치를 설정한 프레임(일정 프레임 이후에 없어지게 하기 위해서이다.
	BOOL							siSpeechSettingfNotDel;			// 스피치가 사라지는지 Flag

	SI16							siPutNameCX, siPutNameY;		// 이름을 찍을 좌표.
	SI16							siYetPutNameCX, siYetPutNameCY;	// 예전에 이름이 찍힌 위치
	CHAR							szCharName[ON_ID_LENGTH+1];					// 이름
	DWORD							dwIP;							// IP

	CHAR							m_szGuild[ON_GUILDNAME_LENGTH + 1];	// 길드 이름(빈칸이면 길드 미가입)
	SI32							m_siGuildInGrade;				// 길드내에서 직위
	SI32							m_siGuildFlag;					// 길드 깃발
	UI08							m_uiIsHostilityGuild;			// 적대 관계 길드 Flag

	RECT							rcRealImage;					// 캐릭터의 Real Image.	

	BOOL							bMovingSwitch;					// 현재 이동중인지의 변수.
	UI16							MoveFlag;						// 이동 Flag

	//---------------------------------
	// 임진록2 방식의 애니메이션시 필요한 변수
	SI16							m_siImKind;

	SI32							m_siImFile;
	SI32							m_siFont;						// 조합형 캐릭터는 캐릭터 애니메이션 현재 방향의 첫번째 폰트 번호(한 방향당 폰트 수 * DirectionNo)
	SI16							m_siXSize;
	SI16							m_siYSize;
	SI16							m_siAniStep;					// 애니메이션 현재 단계
	SI16							m_siAniDelayer;					// 애니메이션 딜레이어
	BOOL							m_bReverse;
	SI16							m_siDir;						// 캐릭터의 현재 방향에 맞는 폰트번호를 계산할 때 필요한 변수

	SI16							m_siCurrAni;					// 현재 애니메이션
	SI16 							m_siWait0CharAnimationIndex;	// 대기
	SI16 							m_siMoveCharAnimationIndex;		// 이동
	SI16 							m_siAttackCharAnimationIndex;	// 공격
	SI16 							m_siMagicCharAnimationIndex;	// 마법 시전

	//---------------------------------
	// 조합형 캐릭터 관련(Hero)
	//SI16							HeroID;							// 몇번째 주인공 캐릭터인가
	SI16							m_siHeroAniAction;				// Animation 그림을 찍는 Order상태
	//SI16							PreAniAction;
	SI16							m_siHeroBaseWClass;				// 무기에 따라 행동할 수 있는 동작 CLASS
	SI16							m_siHeroWeaponEnum;				// 캐릭터 애니메이션 속도
		
	CHAR							m_cDefaultBody[5];				// 캐릭터의 디폴트 몸(예: KMN)
	
	SI16							m_siWeaponDrawID;				// (평화/전투 모드)  현재 장착하고 있는 무기 ID

	//
	//---------------------------------

	// 이모티콘
	DWORD							m_Imoticon_StartCount;			// 이모티콘 시작 시간
	UI16							m_Imoticon_Type;				// 이모티콘 Type

	// 캐릭터가 선택되었음을 알려주는 변수.
	BOOL	NewSelectedFlag;
	SI32	NewSelectedDelay;

	// 현재 장비한 무기
	UI16							m_EquitWeapon;

	// 개인상점
	SellItemInfo					m_SellItemInfo[ON_MAX_BOOTH_ITEM];
	BOOL							m_bOpenBooth;

	//==================================================================================
	// 따라다니는 용병
	SI32							m_MercenaryKind;

	SI16							m_VMercenary_NowX;
	SI16							m_VMercenary_NowY;
	SI16							m_VMercenary_NextX;
	SI16							m_VMercenary_NextY;
	SI16							m_VMercenary_MovedPercent;
	SI16							m_VMercenary_Dir;

	SI16							m_VMercenary_TargetX;
	SI16							m_VMercenary_TargetY;

	SI16 							m_VMercenary_siWait0CharAnimationIndex;
	SI16 							m_VMercenary_siMoveCharAnimationIndex;
	SI16 							m_VMercenary_siAttackCharAnimationIndex;
	SI32 							m_VMercenary_siAniStep;
	SI32 							m_VMercenary_siAniDelayer;
	SI32 							m_VMercenary_siCurrAni;
	SI32 							m_VMercenary_siFont;
	SI32 							m_VMercenary_bReverse;

	SI32							m_siTempImFile;

	//==================================================================================
	// Effect
	SI32							m_ChangeEffectSex;
	SI32							m_ChangeEffectFrame;
	BOOL							m_bFieldAttackAni;

	// robypark 2004/11/2 13:29
	SI32							m_siSiegeWarfareTransformType;	// 공성전 유닛으로 변신 중인지를 나타내는 플래그.  이미 공성전 유닛인 상태에서 인지도 구분
	SI32							m_siEffectTransformFrame;		// 공성전 유닛 변신 프레임

	// robypark 2004/12/2 19:5
	SI32							m_siEffectSupplyGoodsType;		// 공성전 유닛이 보급품 관련 효과. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	SI32							m_siEffectSupplyGoodsFrame;		// 공성전 유닛 보급품 관련 애니메이션 프레임

	// robypark 2004/11/24 18:36
	BOOL							m_bSiegeWarfareEvasion;		// 공성전 유닛 견고한 방어 이펙트 사용여부
	SI32							m_siEffectEvasionFrame;		// 공성전 유닛 견고한 방어 이펙트 프레임

	BOOL							m_bSiegeWarfareDeathblow;	// 공성전 유닛 치명적 공격 이펙트 사용여부
	SI32							m_siEffectDeathblowFrame;	// 공성전 유닛 치명적 공격 이펙트 프레임

	// robypark 2004/12/10 11:38
	SI32							m_siEffectVillageConterAttackType;		// 공성전 마을 반격 타입
	SI32							m_siEffectVillageConterAttackFrame;		// 공성전 마을 반격 애니메이션 프레임

	// robypark 2004/12/16 15:2
	BOOL							m_bIsSiegeWarfareUnit;					// 공성전 유닛인지를 나타내는 플래그

	// robypark 2004/12/17 17:17
	BOOL							m_bIsInVillageDefenceInterface;			// 캐릭터가 현재 수성 인터페이스 상에 있는가?
	SI08							m_scInVillageDefenceInterfaceAniType;	// 캐릭터가 현재 수성 인터페이스 상에 있는 경우 애니메이션 타입

	// robypark 2005/1/17 11:19
	BOOL							m_bIsRefugee;			// 피난민 캐릭터인가?
	SI32							m_siRefugeeKind;		// 피난민 캐릭터 Kind

	// robypark 2005/1/27 11:54
	SI08							m_scForceRelation;		// 이 캐릭터와 사용자와의 관계

	// robypark 2005/2/17 21:44
	// 마우스 커서 아래 캐릭터인지를 설정한다.
	BOOL							m_bCursorHoverCharacter;

private:	
	// linus
	SI16							m_siFieldHp;									// 공성전 유닛의 현재 체력
	// robypark 2004/10/27 15:4
	SI16							m_siFieldHp_Maximum;							// 공성전 유닛의 최대 체력
	SI16							m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;	// 공성전 유닛 보급품 현재량
	SI16							m_siSiegeWarfareUnit_SupplyGoodsAmount;			// 공성전 유닛 보급품 최대량
	DWORD							m_dwSiegeWarfareUnit_LastAttackDelay;			// 공성전 유닛 공격 지연시간
	BOOL							m_bCaptain;
//	SI16							m_siSupplyCharge;								// 보급수레가 가지고 있는 보급품양

private:
	// robypark 2004/11/12 22:4
	SI16							m_siPositionGaugeX;			// 공성전 유닛일 때 보여지는 체력바 위치(X)
	SI16							m_siPositionGaugeY;			// 공성전 유닛일 때 보여지는 체력바 위치(Y)

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치
	SI16							m_siDrawCenterX;
	SI16							m_siDrawCenterY;

	// robypark 2005/1/27 11:1
	SI32							m_siAnimationResourceID_Blue;			// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	SI32							m_siAnimationResourceID_Red;			// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때

	// robypark 2005/2/15 16:32
	SI08							m_scMaxMoveActionSkip;					// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과
	SI08							m_scCurrentMoveActionSkip;				// 이동 처리 함수 Move() 딜레이	현재. 프레임 skip 효과

private:	
	VOID	DecideFont();		// 이번 프레임에서 캐릭터가 찍어야 할 애니메이션의 정보를 구한다.	
	SI32	Move();				// 캐릭터를 이동 시킨다.	
	VOID	GetMoveRatio(SI32 startx, SI32 starty, SI32 endx, SI32 endy, R64 movepercent, SI16 &rx, SI16 &ry);	// 캐릭터가 이동했을때의 변위를 얻어온다.		
	BOOL	UpdateAnimation(BOOL bForce, BOOL* fEndFrame, BOOL* fSound);		// 애니메이션을 업데이트 한다.(현재 명령과 현재 방향에 의존한다.)	

	BOOL	UpdateAnimationForRefugee(BOOL bForce, BOOL* fEndFrame, BOOL* fSound);		// 피난민 캐릭터를 위한 애니메이션을 업데이트 한다.(현재 명령과 현재 방향에 의존한다.)	
	
	BOOL	GetOrder(OnOrder *pOrder);			// 명령 버퍼에서 명령들을 얻어온다.	
	BOOL	IsOrderInBuffer();					// 버퍼에 명령이 들어가 있는지 본다.	
	VOID	OverHead();							// 캐릭터의 오버헤드.	

	VOID	SetMovingDirection(SI32 siDirection);	// 현재 이동중인 방향을 설정한다.
	BOOL	IsSendMovingDirection();				// 서버에게 이동중인 방향을 보내야 하는때인지를 알아온다.	

	BOOL	IsMyChar();								// 사용자의 캐릭터인가?

	BOOL	IsSpeech();								// 지금 캐릭터가 말을 하고 있는가?

	// 명령에 의한 각 함수들.
	BOOL	PreAddOrder(OnOrder *pOrder);																				// 명령을 현 명령 버퍼에 넣기전에 넣을 수 있는지 살펴본다.
	BOOL	PreProcessorOrder(OnOrder *pOrder);																			// 명령을 실지로 수행하기전에 적절한지 미리 살펴본다.

	SI32	IsNeed4DirectPathFind(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2);											// 4방향 길찾기가 필요한가?
	BOOL	GetScreenRealRect(LPRECT pRect);																			// 스크린상의 RealRect를 구한다.
	SI32	GetActionCode();																							// Action Code를 얻어온다.

	VOID	DrawHero(SI32 siDrawMode, SI32 siStartX, SI32 siStartY,  UI08 *ctable, SI32 siCenterX=-1, SI32 siCenterY=-1, SI16 WaterP=-1);	// 조합형 캐릭터를 해당 모드에 따라 그린다.
	
public:	
	cltOnlineChar();			// 생성자.
	~cltOnlineChar();			// 소멸자.

	BOOL	Init(UI16 uiUniqueID, SI32 siID, CHAR *pszID, UI16 uiCharID, SI16 siX, SI16 siY, DWORD IP, float Ranking, SI32 FightPower, cltOnlineWorld *pWorld,UI16* lpuiEquipWeapon);		// 초기화.	
	BOOL	SetEquipItem(UI16* lpuiEquipWeapon);																			// 장착 Item 지정
	VOID	Free();																											// 캐릭터를 없앤다.

protected:
	VOID	DrawRefugee(SI16 WaterP);																						// 피난민 드로우.		
	VOID	DrawChar(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);							// 캐릭터만 Draw(내부에서 쓰는 함수)
	VOID	DrawCharRefugee(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);						// 피난민 캐릭터만 Draw(내부에서 쓰는 함수)
	VOID	DrawCharByVMercenary(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);				// 따라다니는 용병만 Draw(내부에서 쓰는 함수)

public:
	VOID	Draw(SI16 WaterP);																								// 드로우.		
	VOID	DrawTransparent(SI16 WaterP);																					// 반투명으로 그린다.
	VOID	Action();																										// 액션.	
	VOID	DrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface);																		// 대사를 찍어준다.
	VOID	DrawName(HDC hdc);																								// 이름을 찍어준다.	
	VOID	DrawGuildFlag(SI16 ScreenX = -1000, SI16 ScreenY = -1000);															// 캐릭터 이름 앞에 Guild 깃발을 찍는다.(만약 좌표가 -1이면 ID 찍는 부분 앞에 찍는다.)
	VOID	GetPos(SI16 *psiX, SI16 *psiY);																					// 타일 좌표를 얻어온다.	
	VOID	GetRPos(SI16 *psiX, SI16 *psiY);																				// 타일의 변위 좌표를 얻어온다.	
	VOID	GetDPos(SI16 *psiX, SI16 *psiY);																				// 현재 이동중인 목표 위치를 얻어온다.	
	VOID	GetNPos(SI16 *psiX, SI16 *psiY);																				// 현재 이동중인 다음 위치를 얻어온다.	
	VOID	GetMovePersent(SI16 *ps);																						// 현재 이동중인 다음 위치의 비율을 얻어온다.	
	VOID	ClearOrder();						// 캐릭터의 모든 명령을 지운다.
	BOOL	AddOrder(SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2);													// 캐릭터 명령을 버퍼에 넣는다.	
	BOOL	AddOrder(OnOrder Order);																						// 캐릭터 명령을 버퍼에 넣는다.	
	VOID	SetID(CHAR *pszID);																								// 캐릭터의 ID(문자열)을 설정한다.
	UI16	GetUniqueID()			{ return uiUniqueID; }																	// 캐릭터의 UniqueID를 얻어온다.
	BOOL	IsInit()				{ return  uiUniqueID != 0 ? TRUE : FALSE; }												// 초기화가 되었는지 알아온다.

	VOID	SetMove(SI16 siDX, SI16 siDY);																					// 이동해야 할 좌표를 정해준다.	
	VOID	SetMoveByDirection(SI32 siDirection);																			// 캐릭터를 그 방향을 한 타일 이동 시킨다.
	SI32	GetMovingDirection();																							// 현재 이동중인 방향을 구한다.	
	VOID	SetWalkingFlag(UI16 Flag);				// 캐릭터를 이동 Flag

	VOID	InitSpeech();																									// 대사를 초기화 한다.
	VOID	ActionSpeech();																									// 대사 처리,
	VOID	SetSpeech(CHAR *pString, BOOL fNotDel, CHAR* pID);																// 이번에 말할 대사를 넣어준다.		
	BOOL	CheckSpeech();																									// 대사가 떠있는지 검사

	CHAR*	GetCharName();																									// 사용자의 이름을 얻어온다.
	DWORD	GetIP();																										// 사용자의 IP를 얻어온다.

	SI32	GetFightPower(void);																							// 캐릭터의 전투력을 얻는다.
	BOOL	SetFightPower(SI32 FightPower);																					// 캐릭터의 전투력을 지정한다.

	BOOL	IsPointToChar(LPPOINT pptMouse, SI32 *psiDistance);																// 현재 마우스가 가리키는 캐릭터는지 얻어온다.	

	BOOL	IsWarp(SI16 siWarpX, SI16 siWarpY,SI16 sidistance);																				// 워프를 해야하는 상황인가?
	VOID	Warp(SI16 siWarpX, SI16 siWarpY);																				// 워프를 한다.
	BOOL	IsMoving();																										// 현재 캐릭터가 이동중인가?

	OnOrder	GetCurrentOrder();																								// 현재 수행중인 명령을 리턴한다.
	SI16	GetClass();																										// 캐릭터의 클래스를 얻어온다.

	VOID	ChangeDirection(SI32 siDirection);	// 캐릭터의 방향을 바꾼다.	

	POINT	SetID(SI32 siBaseX, SI32 siBaseY);																				// BaseX, BaseY를 기준으로 ID를 심을 새로운 좌표를 찾는다.
	VOID	DelID();																										// 맵에서 자신을 삭제한다.

	VOID	SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo);					// 플레이어의 정보를 설정한다.
	VOID	SetPlayerInfo(OnCharPlayerDetailInfo* lpPlayerInfo);															// 플레이어의 정보를 설정한다.
	BOOL	SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound=FALSE);								// 캐릭터에게 아이템을 장착시킨다.
	SI16	GetWeaponDrawID()	{ return m_siWeaponDrawID; };																// (평화/전투 모드) 현재 장착하고 있는 무기 ID를 얻어온다.
	VOID	SetWeaponDrawID(SI16 siWeaponDrawID)	{	m_siWeaponDrawID = siWeaponDrawID;	};								// (평화/전투 모드) 현재 장착하고 있는 무기 ID를 설정한다.

	VOID	SetBattleFlag(UI08 Flag);																						// Set Battle Flag
	UI08	GetBattleFlag(void);																							// Get Battle Flag
	VOID	SetImoticon(UI16 uiImoticon);																					// Imoticon 출력

	VOID	SetFindTargetFlag(UI08 Flag);																					// Set Battle Flag
	SI32	GetKind()					{	return siKind;	};

	// 클라이언트에서 사용되는 캐릭터 인덱스 얻기
	SI32	GetImjinKind()				{	return m_siImKind;	};

	VOID	SetGuildData(char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag);											// 길드 Data 지정
	char*	GetGuildName(void);																								// 길드 Name 얻기
	SI32	GetGuildInGrade(void);																							// 길드 직급 얻기
	SI32	GetGuildFlag(void);																								// 길드 깃발 얻기

	BOOL	SetIsHostilityGuild(UI08 uiIsHostilityGuild);																	// 적대 길드인지 검사
	UI08	GetIsHostilityGuild(void);																						// 적대 길드인지 검사

	void	SetNewSelectedFlag(BOOL flag);
	BOOL	GetNewSelectedFlag();

	BOOL	Show(BOOL fShow);
	BOOL	GetShow(void);

	UI32	GetIP(UI16 uiUniqueID);																							// Get IP

	SellItemInfo	*GetBoothItem(UI08 uiSlot)							{ return	&m_SellItemInfo[uiSlot]; };				// 개인상점 아이템 정보 얻기
	VOID			SetBoothItem(UI08 uiSlot, SellItemInfo ItemInfo)	{ m_SellItemInfo[uiSlot] = ItemInfo; };				// 개인상점 아이템 정보 지정
	BOOL			IsBoothOpen()										{ return	m_bOpenBooth; };						// 개인상점 개설 여부 얻기
	VOID			SetBoothOpen(BOOL bOpenBooth);																			// 개인상점 개설 여부 지정

	SI08	GetDataType(void);
	void	SetDataType(SI08 fDataType);

	BOOL	SetMercenary(SI32 siKind);
	SI32	GetMercenary(void);

	void	ActionByVMercenary(void);
	void	DrawByVMercenary(SI16 WaterP);

	void	GetNPosByVMercenary(SI16 *psiX, SI16 *psiY);
	void	GetMovePersentByVMercenary(SI16 *ps);

	void	GetEquip(UI16* lpuiEquip);
	void	SetChangeEffect(void);

	void	SetAttackMode(BOOL b)	{ m_bAttackMode = b; }
	BOOL	IsAttackMode()			{ return m_bAttackMode; }

	void	SetExpoEvent(int nScore, int nRank, BOOL b) { m_nScore = nScore, m_nRank = nRank; m_bInvalidBattle = b; }

	void	SetScore(int nScore){ m_nScore = nScore; }
	void	SetRank(int nRank)	{ m_nRank = nRank; }
	int		GetScore()			{ return m_nScore; }
	int		GetRank()			{ return m_nRank; }
	BOOL	IsInvalidBattle()	{ return m_bInvalidBattle; }
	BOOL	ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis);
	
	// actdoll (2004/07/20 14:21) : 필드 상 이동속도 변경시킬 수 있는 함수
	void	SetMoveSpeed( SI16 siMoveSpeed )		{ m_siMoveSpeed = siMoveSpeed; }
	SI16	GetMoveSpeed( void )					{ return m_siMoveSpeed; }

public:
	// linus
	void	SetSiegeWarfare_Hp(SI16	siFieldHp);	// 공성전 유닛 현재 체력치 설정
	void	SetSiegeWarfare_Damage(SI16 siDamage);		// 공성전 유닛의 체력치 줄이기
	SI16	GetSiegeWarfare_Hp();						// 공성전 유닛의 현재 체력치 얻기
	BOOL	IsFieldAlive();

	// 시작 /////////////////////////////////////////
	// robypark 2004/10/27 15:5
	// 공성전 유닛 최대 체력치 설정
	void	SetSiegeWarfare_MaximumHP(SI16 siMaximumHP);
	// 공성전 유닛 최대 체력치 얻기
	SI16	GetSiegeWarfare_MaximumHP(void);
	// 공성전 유닛 보급품 현재량 설정
	void	SetSiegeWarfare_CurrentSupplyGoodsAmount(SI32 siCurrentSupplyGoodsAmount);
	// 공성전 유닛 보급품 현재량 얻기
	SI32	GetSiegeWarfare_CurrentSupplyGoodsAmount(void);
	// 공성전 유닛 보급품 최대량 설정
	void	SetSiegeWarfare_SupplyGoodsAmount(SI32 siSupplyGoodsAmount);
	// 공성전 유닛 보급품 최대량 얻기
	SI32	GetSiegeWarfare_SupplyGoodsAmount(void);

	// 공성전 유닛의 공격 지연 시간 설정
	void	SetSiegeWarfareUnit_LastAttackDelay(DWORD dwLastAttackDelay);
	// 공성전 유닛의 경과된 공격 지연 시간 설정
	void	SetSiegeWarfareUnit_PastAttackDelay(DWORD dwPastAttackDelay);
	// 공성전 유닛의 공격 지연 시간 얻기
	DWORD	GetSiegeWarfareUnit_LastAttackDelay(void);
	// 끝 /////////////////////////////////////////

public:
	// robypark 2004/11/2 17:55
	// 공성전 유닛으로 변신 효과 사용
	void	SetSiegeWarfare_TransformEffect(SI32 siSiegeWarfareTransformType);

	// robypark 2004/11/2 18:0
	// 공성전 유닛 보급품 자동 소모(1%)
	void	WasteSiegeWarfareSupplyGoods(void);

	// robypark 2004/12/10 11:41
	// 공성전 유닛이 마을로 부터 공격받는 효과 설정
	void	SetSiegeWarfare_VillageCountAttack(SI32 siEffectVillageConterAttackType);

	// robypark 2004/12/2 19:2
	// 공성전 유닛이 보급품 관련 효과 설정. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	void	SetSiegeWarfare_EffectSupplyGoods(SI32 siEffectSupplyGoodsType);

	// robypark 2004/11/24 18:31
	// 공성전 견고한 방어 효과 사용
	void	SetSiegeWarfare_Evasion(void);

	// robypark 2004/11/24 18:31
	// 공성전 치명적 공격 효과 사용
	void	SetSiegeWarfare_DeathBlow(void);

	void	SetCaptain(BOOL	bCaptain);
	BOOL	GetCaptain();

//	void	SetSupplyCharge(SI16 siSupply);
//	SI16	GetSupplyCharge();

public:
	// robypark 2004/11/12 21:15
	// 공성전 유닛의  체력 및 보급품 수치(%) 그리기
	void	DrawSiegeWarfareUnit_StatusBar(void);

	// robypark 2004/12/16 14:33
	// 공성전 유닛인가?
	BOOL	IsSiegeWarfareUnit(void);

	// robypark 2004/12/17 17:20
	// 사용자가 현재 수성 인터페이스 상에 있는가?
	BOOL	IsInVillageDefenceInterface(void);

	// robypark 2004/12/17 17:20
	// 캐릭터가 현재 수성 인터페이스 상에 있는가? 설정
	void	SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface);

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치 얻기
	void	GetCharacterDrawCenterPos(SI16 &siCenterX, SI16 &siCenterY);

	// robypark 2005/2/17 21:44
	// 마우스 커서 아래 캐릭터인지를 설정한다.
	void	SetCursorHoverCharacter(BOOL bHover);

protected:
	// robypark 2004/11/4 16:5
	// 공성전 유닛 바닥 이펙트 그리기
	// [IN] SI08 scForceRelation : 사용자 캐릭터와 그리려는 캐릭터의 관계
	//								SIEGEWARFARE_FORCE_FRIENDLY(0)	- 같은 상단원이다.
	//								SIEGEWARFARE_FORCE_ENEMY(1)		- 적대 상단원이다.
	void	DrawSiegeWarfareForceEffect(SI08 scForceRelation, SI32 siCenterX, SI32 siCenterY);
};		

#endif
