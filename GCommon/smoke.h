#ifndef _SMOKE_H
#define _SMOKE_H

// 연기의 애니메이션 속도의 상수.
#define	SMOKE_DELAY_1	1
#define SMOKE_DELAY_2	2
#define SMOKE_DELAY_3	3
#define	SMOKE_DELAY_4	4
#define	SMOKE_DELAY_5	5
#define	SMOKE_DELAY_10	10

// 그려질 때 모드
#define SMOKE_TRANSPARENT_MODE_NORMAL     1
#define SMOKE_TRANSPARENT_MODE_OLDTRANS   2
#define SMOKE_TRANSPARENT_MODE_HALFTRANS  3

// 효과 종류 상수 선언
#define KIND_SMOKE_EXP0						1    // 포탄 폭발 
#define KIND_SMOKE_EXP1						2
#define KIND_SMOKE_BIGEXP2  				3
#define KIND_SMOKE_EXP2     				4
#define KIND_SMOKE_EXP3     				5
#define KIND_SMOKE_EXP4     				6
#define KIND_SMOKE_EXP5     				7
#define KIND_SMOKE_EXP6     				8
#define KIND_SMOKE_EXP7     				9
#define KIND_SMOKE_EXP8     				10
#define KIND_SMOKE_EXP9     				11
#define KIND_SMOKE_EXP10    				12
#define KIND_SMOKE_EXP11    				13
#define KIND_SMOKE_EXP12    				14
#define KIND_SMOKE_EXP13    				15
#define KIND_SMOKE_EXP14    				16
#define KIND_SMOKE_EXP15    				17
#define KIND_SMOKE_SMALLEXP2 				18
#define KIND_SMOKE_GUNSMOKE 				19
#define KIND_SMOKE_GUNHIT   				20 
#define KIND_SMOKE_SMOKE0   				21
#define KIND_SMOKE_SMOKE1   				22
#define KIND_SMOKE_SMOKE2   				23
#define KIND_SMOKE_SMOKE3   				24
#define KIND_SMOKE_SMOKE10   				25
#define KIND_SMOKE_RISINGSMOKE0  			26
#define KIND_SMOKE_RISINGSMOKE1  			27 
#define KIND_SMOKE_FIRE      				28
#define KIND_SMOKE_BIGEXP    				29
#define KIND_SMOKE_MANFIRE   				30
#define KIND_SMOKE_SINK      				31
#define KIND_SMOKE_WATEREXP  				32
#define KIND_SMOKE_EARTHQUAKE 				33
#define KIND_SMOKE_LIGHTNING  				34
#define KIND_SMOKE_BUILDINGEXP				35
#define KIND_SMOKE_LIGHTEXP1				36
#define KIND_SMOKE_LIGHTEXP2				37
#define KIND_SMOKE_LIGHTEXP3				38
#define KIND_SMOKE_GROUNDEXP1				39
#define KIND_SMOKE_GROUNDEXP2				40
#define KIND_SMOKE_GROUNDEXP3				41
#define KIND_SMOKE_TORNADO					42
#define KIND_SMOKE_KIMBOMB1					43
#define KIND_SMOKE_KIMBOMB2					44
#define KIND_SMOKE_KIMBOMB3					45
#define KIND_SMOKE_MAGIC8K_EXP			    46
#define KIND_SMOKE_MAGIC6C1					47
#define KIND_SMOKE_MAGIC6C2					48
#define KIND_SMOKE_MAGIC6C3					49
#define KIND_SMOKE_MAGIC4C					50
#define KIND_SMOKE_MAGIC4CEXP				51
#define KIND_SMOKE_SNOWMAGIC				52
#define KIND_SMOKE_SNOWMAGICEXP				53
#define KIND_SMOKE_ADVTOWERKEXP1			54
#define KIND_SMOKE_ADVTOWERKEXP2			55
#define KIND_SMOKE_GROUNDFIRESMOKE			56
#define KIND_SMOKE_ARROWTRAIL				57
#define KIND_SMOKE_SPECIAL					58
#define KIND_SMOKE_WATEREFFECT				59
#define KIND_SMOKE_DUST						60
#define KIND_SMOKE_DETECTMINEEFFECT			61
#define KIND_SMOKE_CLONAPPEAR				62
#define KIND_SMOKE_CLONDIE					63
#define KIND_SMOKE_GOLDDRAGONELECTRON		64
#define KIND_SMOKE_GOLDDRAGONELECTRONEXP	65
#define KIND_SMOKE_GHOSTCATEXP				66
#define KIND_SMOKE_WAVEEFFECT				67
#define KIND_SMOKE_BIGTREE					68
#define KIND_SMOKE_MAGIC4T					69
#define KIND_SMOKE_YELLOWSAND				70
#define KIND_SMOKE_DARKFIRE					71
#define KIND_SMOKE_GHOST					72
#define KIND_SMOKE_PANDA					73
#define KIND_SMOKE_SMALL_TORNADO			74
#define KIND_SMOKE_RUNNINGFIRE				75
#define KIND_SMOKE_FIREATTACK				76
#define KIND_SMOKE_DOUBLE_SWORD				77			// 고니시 쌍검난무
#define KIND_SMOKE_LONG_SPEAR				78			// 손유창 장창소환
#define KIND_SMOKE_ICE_BARRIER				79			// 순비연 빙석술
#define KIND_SMOKE_LONG_SPEAR_HIT			80			// 손유창 장창소환
#define KIND_SMOKE_PAPERDOLL_HIT			81			// 음양사 종이인형 히트

#define KIND_SMOKE_FLOWER_NEEDLE_RAIN1		82			// 만천화우1
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN2		83			// 만천화우2
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN3		84			// 만천화우3
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN4		85			// 만천화우4
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN5		86			// 만천화우5
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN6		87			// 만천화우6
#define KIND_SMOKE_SOUL_CHANGE				88			// 빙의술
#define KIND_SMOKE_SOUL_CHANGE_HIT			89			// 빙의술 히트
#define KIND_SMOKE_STONE_MAGIC_STONE		90			// 석괴술 돌
#define KIND_SMOKE_STONE_MAGIC_SMOKE		91			// 석괴술 연기
#define KIND_SMOKE_GOD_SPEAR				92			// 신창노도(일격필살)
#define KIND_SMOKE_CLOUD_WIND_HIT			93			// 풍모술 히트
#define KIND_FIREENERGYPA_HIT1				94			// 용장풍 히트1
#define KIND_FIREENERGYPA_HIT2				95			// 용장풍 히트2
#define KIND_SMOKE_JINEMPEROR_HIT			96			// actdoll (2004/03/06 9:58) : 유명계/진시황릉 진시황의혼 히트
#define KIND_SMOKE_GUARDIAN_CAW				97			// 소 3단 변신 스모크.
#define KIND_SMOKE_GUARDIAN_TIGER			98			// 호랑이 히트 스모크.
#define KIND_SMOKE_GUARDIAN_CHICK			99


//----------------------------------------------------------------------------
// robypark (2004/05/18 11:04) : 추가 기술의 SMOKE - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT	100			// 화룡출수 히트 효과(대만-화룡차, 손유창 2차 전직)
#define KIND_SMOKE_POISON_ICE_BULLET_HIT	101			// 독, 냉기 공격 히트 효과(일본-흑룡차, 세이메이 2차 전직)
#define KIND_SMOKE_EARTHQUAKE_WAVE			102			// 일본-지진차, 공격 지진 웨이프 이펙트
#define KIND_SMOKE_THUNDER_BOLT_HIT			103			// 굉뇌 번개 히트 효과(조선-뇌전차, 허준 2차 전직 기술)
#define KIND_SMOKE_FOG_OF_ICE_HIT			104			// 천지독살 냉기 히트 효과(일본-흑룡차)
#define KIND_SMOKE_FOG_OF_POISON 			105			// 천지독살 독안개 효과(일본-흑룡차, 세이메이 2차 전직 기술)
#define KIND_SMOKE_FOG_OF_ICE				106			// 천지독살 냉기 효과(일본-흑룡차, 세이메이 2차 전직 기술)
#define KIND_SMOKE_MUSHROOM_CLOUD			107			// 승천포 기술 사용시 핵폭발 버섯구름 스모크(중국-발석거, 순비연<=이령 2차 전직 기술)
#define KIND_SMOKE_HEAD_OF_DRAGON			108			// 강강수월래 용 회전 이펙트(조선-거북차)
#define KIND_SMOKE_FIRE_ROUND_DANCE_FLOOR	109			// 강강수월래 바닥 이펙트(조선-거북차)
#define KIND_SMOKE_FIRERAIN_OF_GOD			110			// 천벌화시 불비(대만-봉황비조)
#define KIND_SMOKE_CREATE_VOLCANO			111			// 격노염폭 화산 생성(일본-지진차)
#define KIND_SMOKE_VOLCANO_HIT				112			// 격노염폭 화산탄 히트(일본 지진차)

// robypark 2004-6-3 15:43
// 승천포 작은 폭발 연기
#define KIND_SMOKE_BOMB_SMOKE1				113
#define KIND_SMOKE_BOMB_SMOKE2				114
#define KIND_SMOKE_BOMB_SMOKE3				115

// robypark 2004/6/4 19:21
// 불랑기포차 기술 앙천대소 포탄 폭발 smoke
#define KIND_SMOKE_ROAR_OF_LAUGHTER_HIT		116	

// robypark 2004/6/18 11:54
// 지진차 격노염폭 기술 사용시 생성되는 화산 위에 덧그릴 먼지(연기)
#define KIND_SMOKE_CREATE_VOLCANO_DUST		117

// robypark 2004/11/15 15:38
// 공성전 전투 중에 공격 히트 효과
#define KIND_SMOKE_SIEGEWARFARE_IN_FIGHT	118

// robypark 2004/11/24 17:34
// 공성전 발석거 돌 히트 효과
#define KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT	119

// robypark 2004/12/6 11:27
// 공성전 궁병 화살 히트 효과
#define KIND_SMOKE_SIEGEWARFARE_ARROW_HIT	120

#define MAX_SMOKE_KIND_NUMBER				128


#define ORDER_SMOKE_EXP     1

// 새로운 Smoke추가는 HQ안에 들어가는 InfoSmoke.txt에 추가해주며, 기본적인 정보를 얻어온다.

// 효과 출력 클래스 smoke.cpp에 있던 코드를 이동
class _SmokeKindInfo
{
	public:
		void Init(CHAR *file, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI32 siyindent);

		// robypark 2004/9/30 18:12
		// Smoke 정보를 HQ에서 InfoSmoke.txt에서 얻어와 사용한다.
		void Init(char *pszKindID);

		SHORT	m_ssFile, m_ssStartFont;
		SHORT	m_ssMoveFrameNumber;
		SHORT	m_ssMaxRepeatNumber;
		SHORT	m_ssRisingSwitch;
		SI32	m_siTransparentMode;
		SI32	m_siStepDelay;
		SI32	m_siYIndent;


};

class _Smoke
{
public:
	SHORT m_ssID;
	SHORT m_ssKind;
	SHORT m_ssAnimation;
	SHORT m_ssStep;
	SHORT m_ssFile, m_ssFont;
	SI32  m_siStepDelay;			// 애니메이션의 속도.
	char m_scArea;                        // 공중에 있는가 아니면 지상에 있는가?
	SHORT  m_ssCurrentOrder;               // 현재 캐릭터가 하고 있는 작업 
	DWORD m_uiLastChangedFrame;           // 애니메이션이 변경된 프레임 

	SHORT m_ssX, m_ssY;

	char m_scDrawSwitch;                   // 그려줄 것인가 말것인가?
	char m_scReverseSwitch;                // 뒤집어서 찍을 것인가?
	SHORT  m_ssDrawXsize,  m_ssDrawYsize;       // 그림의 가로 세로 사이즈 
	SHORT  m_ssDrawStartX, m_ssDrawStartY;      // 그림이 그려지는 시작 위치 
	SHORT m_ssCenterX, m_ssCenterY;
	SI32  m_ssTransparentMode;          // 광원효과를 넣을 것인가 ?

	SHORT m_ssPlayerNumber;
	SHORT m_ssApower;
	SI16  m_siApower;

	// robypark 2004-5-24, 16:37 지진차의 지진 공격 효과를 위하여 추가
	SI08	m_bFloorSmoke;		// 해당 Smoke의 출력이 바닥(Floor), 즉 오브젝트 전에 그려야 할 경우 TRUE를 설정한다.

	// robypark 2004-6-3 15:54
	// 지진차 격노염폭 기술 사용시 생성되는 화산을 위한 플래그
	// smoke 애니메이션이 끝났는가? 를 나타내는 플래그
	// value: true = 애니메이션이 끝났다.
	//		: false = 애니메이션이 끝나지 않았다.
	SI08	m_bIsDoneAnimation;

	// 이동관련 
	char m_scMoveFrameNumber;
 
	SHORT m_ssRealX, m_ssRealY;
	SHORT m_ssStartFont;

	SHORT m_ssMaxRepeatNumber;
	SHORT m_ssRepeatNumber;

	SHORT m_ssRisingSwitch;

	_NewID m_AttackID; // 피공격자 
	_NewID m_AttackerID; // 공격자. 
	float  m_siSpeed;

	_Smoke();
	void Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, char area);
	BOOL Action();
	BOOL Draw();
    void SetInfo();
	// 찍을 위치를 계산한다. 
    void CalculateRealXY();
	void CalculateRealXYNormal();

	void CalculateDrawStartXY();

	void DecideFont();
	void DecideFontNormal();

};

// Smoke를 설정한다.
// SI16 siKind[in]: SMOKE 종류
// SI16 id[in]: Smoke ID
// SI16 x[in]: Smoke를 생성할 위치(x)
// SI16 y[in]: Smoke를 생성할 위치(y)
// SI08 area[in]:
// SI16 siPlayerNumber[in]: Smoke를 생성한 플레이어 번호(default -1)
//
// robypark 2005-5-24, 16:37 지진차의 지진 공격 효과를 위하여 추가
// SI08 bFloorSmoke[in]: 해당 Smoke의 출력이 바닥(Floor), 즉 오브젝트 전에 그려야 할 경우 TRUE를 설정한다.
//					(default FALSE)
SHORT SetSmoke(SI16 siKind, SI16 id, SI16 x, SI16 y, SI08 area, SI16 siPlayerNumber = -1, SI08 bFloorSmoke = FALSE);
void DeleteSmoke();
void DeleteSmoke(SHORT id);
SHORT FindEmptySmoke();
BOOL IsLiveSmoke(SHORT id);
void SetSmokeRealXY(SHORT id, SHORT realx, SHORT realy);

// robypark 2004-6-3 15:54
// 지진차 격노염폭 기술 사용시 생성되는 화산을 위한 플래그 얻기
// smoke 애니메이션이 끝났는가? 를 나타내는 플래그 얻기
// return value: true = 애니메이션이 끝났다.
//			   : false = 애니메이션이 끝나지 않았다.
SI08 GetSmoke_IsDoneAnimation(SI32 siID);

void InitSmokeKindInfo();

// 2001.11.12 hojae_appned
BOOL Smoke_Action(SI32 i);
void Smoke_CalculateDrawStartXY(SI32 i);
void Smoke_Draw(SI32 i);
void Smoke_PlayerNumber(SHORT smokeid, SHORT PlayerNumber);
void Smoke_Apower(SHORT smokeid, SHORT level);
void Smoke_AttackID(SHORT smokeid, _NewID id);
void Smoke_AttackerID(SHORT smokeid, _NewID id);
void Smoke_Fwrite(FILE* fp);
void Smoke_Read(FILE* fp);
void Smoke_SetYIndent(SI16 kind,SI32 YIndent);
void SetApower(SI16 smokeid,SI16 siApower);

// robypark (2004-5-24, 17:02)
// 바닥(Floor) Smoke일 경우 그려주는 순서를 구분하기 위하여 _Smoke내 m_bFloorSmoke변수를 
// 이용하기 위하여 추가되었다.
extern _Smoke  g_Smoke[MAX_SMOKE_NUMBER];   // 연기용 데이터 

extern _SmokeKindInfo g_SKI[MAX_SMOKE_KIND_NUMBER];

#endif









