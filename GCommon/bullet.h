//==================================================================================================================================================
//	FileName	:	bullet.h
//	Discription	:	총알 관련
//	History		:	2002/02/16(박연숙) - Modify
//==================================================================================================================================================

#ifndef _BULLET_H
#define _BULLET_H

class	cltOnlineWorld;

#define MAX_SHOOT_RANGE							200     

//------------------------------------------------------------------------------------------------
// 총알의 작동 
//------------------------------------------------------------------------------------------------
#define ORDER_BULLET_MOVE						1
#define ORDER_BULLET_EXP     					2
#define ORDER_BULLET_BLOCK   					3
#define ORDER_BULLET_RETURN  					4
#define ORDER_BULLET_SEARCH  					5
#define ORDER_BULLET_CHASE   					6
#define ORDER_BULLET_WAITEXP 					7
#define ORDER_BULLET_DARKFIRE 					8
#define ORDER_BULLET_FIREENERGY					9
#define ORDER_BULLET_WAITEXP4					10

//------------------------------------------------------------------------------------------------
// 총알의 작동 추가 : robypark(2004/5/19, 12:31)
//------------------------------------------------------------------------------------------------
// 피어싱(관통)되는 총알 작동(맵 끝 까지 진행)
// 총알 이동 중 총알이 있는 곳의 적들 유닛에 데미지를 준다.
// 맹격화룡파를 위한 총알 이동
#define ORDER_BULLET_PIERCE						11

// 직선으로 맵의 한 끝점으로 이동하는 총알 작동
// 총알 이동 중 총알이 있는 곳의 적들 유닛에 데미지를 준다.
// 데미지를 준 이후 총알은 소멸한다.
// 굉뇌(조선-뇌전차))를 위한 총알 이동
#define ORDER_BULLET_STRAIGHT					12

// 탄착되었을 때 특수기술을 생성한다.
// 승천포(중국-발석거)를 위한 작동
#define ORDER_BULLET_EVENT_IMPACT				13

//------------------------------------------------------------------------------------------------
// 총알의 애니메이션 동작. 
//------------------------------------------------------------------------------------------------
#define ANI_BULLET_MOVE							1
#define ANI_BULLET_WAIT							2




//------------------------------------------------------------------------------------------------
// 총알 종류 
//------------------------------------------------------------------------------------------------
#define KIND_BULLET_AIRMISSILE       			1		// 공중용 미사일 
#define KIND_BULLET_SAMMISSILE       			2		// 천궁 미사일 
#define KIND_BULLET_ARROW            			3		// 일반 화살
#define KIND_BULLET_FIREARROW        			4		// 불화살
#define KIND_BULLET_SHIPCANNON       			5		// 함포 
#define KIND_BULLET_THROWBOMB        			6		// 던지는 폭탄 
#define KIND_BULLET_FIRE              			7		// 화염 방사가의 불 
#define KIND_BULLET_BOMB              			8		// 폭탄
#define KIND_BULLET_FIRETANKARROW     			9		// 화차의 신기전
#define KIND_BULLET_HEAVYGUN          			10		// 기관포 
#define KIND_BULLET_MAGICBALL1        			11		// 심유경 마법 공격 
#define KIND_BULLET_MAGICBALL2        			12		// 심유경 마법 공격 
#define KIND_BULLET_BUDAFIRE          			13		// 승병의 마법 공격 
#define KIND_BULLET_BUDAFIRE2         			14		// 조선 승병의 마법 공격 
#define KIND_BULLET_BUDAFIRE3         			15		// 일본 무녀의 마법 공격 
#define KIND_BULLET_CANNON            			16 
#define KIND_BULLET_AIRMAGIC          			17

// robypark 2004/7/14 16:44
#define KIND_BULLET_DOLL_EVIS					18		// 에비스 인형 공격 총알

#define KIND_BULLET_GUN               			19
#define KIND_BULLET_GUNPOISON              		20
#define KIND_BULLET_STRONGGUN         			21
#define KIND_BULLET_MAGIC6C			  			22
#define KIND_BULLET_FRAGMENT1         			23
#define KIND_BULLET_FRAGMENT2         			24
#define KIND_BULLET_FRAGMENT3         			25
#define KIND_BULLET_NEEDLE            			26
#define KIND_BULLET_GROUNDEXP         			27		// 연옥술 
#define KIND_BULLET_DART			  			28		// 일본 닌자의 수리검 
#define KIND_BULLET_MAGIC8K			  			29		// 조선 김시민의 쐐기지르기
#define KIND_BULLET_BURNARROW		  			30  
#define KIND_BULLET_MAGIC5J			  			31		// 우기다의 만월도끼춤  
#define KIND_BULLET_VOLCANO						32		// 화산탄 
#define KIND_BULLET_UPGRADETOWERCANNON			33		// 업그레이드된 화포 망루의 포탄
#define KIND_BULLET_AIRATTACKUPARROW			34		// 비전연구
#define KIND_BULLET_GATPHAPOISON				35		// 갓파의 독약 던지기
#define KIND_BULLET_FURHEADDEMON       			36		// 털가면의 얼음 
#define	KIND_BULLET_GOLDDRAGONELECTRON			37      // 금룡 전기. 
#define	KIND_BULLET_ONGHWA						38      // 온천 원숭이 해골.  
#define	KIND_BULLET_GHOSTCAT					39		// 귀묘 음파 공격. 
#define	KIND_BULLET_DOLL1						40		// 무명 인형. 
#define	KIND_BULLET_DOLL2						41		// 하카다 인형. 
#define KIND_BULLET_JUSTICEK        			42		// 조선 유생 공격
#define KIND_BULLET_BUTTERFLY        			43		// 독나방
#define KIND_BULLET_WIDOW	        			44		// 거미요괴.
#define KIND_BULLET_SEAHORSE	       			45		// 해마 물쏘기.
#define KIND_BULLET_POISONARROW	       			46		// 독침      (원주민무당) 
#define KIND_BULLET_THROWSPEAR	       			47		// 던지는 창 (원주민) 
#define KIND_BULLET_SCORPIONBULLET				48		// 독전갈 독. 
#define KIND_BULLET_CACTUSBULLET				49		// 괴물선인장 가시. 
#define KIND_BULLET_MAGICIANBULLET				50		// 요술사 장풍.
#define KIND_BULLET_VOLCANOFLAME				51		// 화산의 불꽃.
#define KIND_BULLET_EVIL						52		// 이사의 사념,아사쿠마(장풍)
#define KIND_BULLET_ICE							53		// 이사의 사념 얼음 공격.
#define KIND_BULLET_DARKFIRE					54		// 임평후의 암흑화염폭풍
#define KIND_BULLET_GHOST						55		// 페르난데스의 귀신탄
#define KIND_BULLET_JAPANGHOST					56		// 일본귀신의 우산살공격
#define KIND_BULLET_KUKET						57		// 쿠켓
#define KIND_BULLET_AUTA						58		// 아우타 공격.
#define KIND_BULLET_QUEENSUB1					59		// 진성여왕 화살공격1
#define KIND_BULLET_QUEENSUB2					60		// 진성여왕 화살공격2
#define KIND_BULLET_FIREATTACK					61		// 이령 화공신포
#define KIND_BULLET_RUNNINGFIRE					62		// 이령 연발폭탄
#define KIND_BULLET_ENERGYPA					63		// 장풍
#define KIND_BULLET_FIREENERGYPA				64		// 열화장풍(비격황룡권)
#define KIND_BULLET_FIREENERGYPASTART			65		// 열화장풍(비격황룡권) 시작
#define KIND_BULLET_PAPERDOLL					66		// 음양사 종이인형
#define KIND_BULLET_NEEDLEATTACK				67		// 침공격(의술사, 허준)
#define KIND_BULLET_CLOUDWIND					68		// 염력사 풍모술
#define KIND_BULLET_GODSPEAR					69		// 곽후 신창노도
#define KIND_BULLET_SOULCHANGE					70		// 빙의술
#define KIND_BULLET_MAGICCATTACK				71		// 도술사 공격
#define KIND_BULLET_SUNBIYOUNATTACK				72		// 순비연 공격
#define KIND_BULLET_SNOW						73		// 눈 

// actdoll (2004/03/06 4:38) : 유명계/진시황릉 관련 추가
#define KIND_BULLET_PYTHON						74		// 이무기 마법공격	// actdoll (2004/03/06 4:38) : 추가
#define KIND_BULLET_JINEMPEROR					75		// 이무기 마법공격	// actdoll (2004/03/06 4:38) : 추가

#define KIND_BULLET_GUARDIAN_TIGER				76		// 수호부(호랑이)
#define KIND_BULLET_GUARDIAN_RABBIT				77		// 수호부(토끼)

#define KIND_BULLET_GUARDIAN_CHICK				78		// 수호부(닭)

//----------------------------------------------------------------------------
// robypark (2004/05/18 14:53) : 추가 캐릭터 사용 bullet - 장수 2차 전직 사용 bullet
//----------------------------------------------------------------------------
#define KIND_BULLET_TURTLE_FIREBALL				79		// 조선-거북차; 파이어 볼
#define KIND_BULLET_THUNDER_BOLT				80		// 조선-뇌전차; 번개 볼트
#define KIND_BULLET_POISON_ICE_BULLET			81		// 일본-흑룡차; 독, 얼음 공격
#define KIND_BULLET_CATAPULT_FIRE_STONE			82		// 중국-발석거; 불붙은 돌덩이
#define KIND_BULLET_FARANGI_BULLET				83		// 중국-불랑기포차; 불랑기포탄
#define KIND_BULLET_FIRE_DRAGON_MISSILE			84		// 대만-화룡차; 화룡출수
#define KIND_BULLET_CHINESEPHOENIX_FEATHER		85		// 대만-봉황비조; 봉황비조 깃털 미사일

#define KIND_BULLET_CATDOLL						86
#define KIND_BULLET_OLDFOXDOLL					87
#define	KIND_BULLET_RACCOONDOLL					88
#define KIND_BULLET_NINJADOLL					89
#define KIND_BULLET_SNOWWOMANDOLL				90
#define KIND_BULLET_YANGGUIBIDOLL				91
#define KIND_BULLET_JIJANGDOLL					92
#define KIND_BULLET_SNAKEWOMANDOLL				93

// robypark 2004-6-1 14:12
#define KIND_BULLET_BOMB_OF_HEAVEN				94	// 발석거, 승천포 불 붙은 포탄
#define KIND_BULLET_VOLCANO_BULLET				95	// 지진차, 격노염폭 화산탄

// robypark 2004-6-3 15:39
#define KIND_BULLET_ONSLAUGHT_FIREBALL			96	// 대만-화룡차; 맹격화룡파 포탄

// robypark 2004-6-4 19:12
#define KIND_BULLET_ROAR_OF_LAUGHTER_BOMB		97	// 불랑기포차 기술 앙천대소 총알
#define KIND_BULLET_FRAGMENT4					98	// 불랑기포차 기술 앙천대소 공격시 파편

// robypark 2004/10/22 11:2
// 공성전 필드 총알
#define KIND_BULLET_SIEGEWARFARE_ARROW			99	// 공성전 - 궁병 화살
#define KIND_BULLET_SIEGEWARFARE_STONE			100	// 공성전 - 발석거 돌

// robypark 2005/2/2 17:27
#define KIND_BULLET_DOLL_DHARMA					101 // 달마 인형 공격 총알

#define MAX_BULLET_KIND_NUMBER       			128


// 탄 비행 궤적 			
#define COURSE_DIRECT							1		// 직사화기 
#define COURSE_SLOPE							2		// 포물선 화기 

#define MAX_ICE_TYPE							4		// 얼음 마법의 종류

#define MAX_HIT_ID								25		// 5*5 사이즈에 준하여 , 
														// 공격에 의하여 피해를 입는 캐릭터의 스택 사이즈 

//--------------------------------------------------------------------------------------------------------------------
// 총알 속성 
#define BULLETATB_HITNORMAL					0x00000001 
#define BULLETATB_RANGE1					0x00000002 
#define BULLETATB_WATER						0x00000004
#define BULLETATB_ARROW						0x00000008
#define BULLETATB_NEEDLE					0x00000010
#define BULLETATB_GUN						0x00000020
#define BULLETATB_STRONGGUN					0x00000040
#define BULLETATB_MAGIC1					0x00000080
#define BULLETATB_MAGIC2					0x00000100
#define BULLETATB_MAGIC3					0x00000200
#define BULLETATB_FIRE						0x00000400
#define BULLETATB_AIRMISSILE				0x00000800
#define BULLETATB_RUNNINGFIRE				0x00001000   // 연박폭탄.
#define BULLETATB_FIREENERGY				0x00002000
#define BULLETATB_CANNON					0x00004000
#define BULLETATB_CANNON2C	  				0x00008000
#define BULLETATB_MAGIC6C	  				0x00010000
#define BULLETATB_SOULCHANGE				0x00020000
#define BULLETATB_FIREARROW					0x00040000
#define BULLETATB_MAKEFIRE					0x00080000
#define BULLETATB_FIREATTACK				0x00100000 //  화공신포.
#define BULLETATB_UPGRADETOWERCANNON		0x00200000
#define BULLETATB_AIRATTACKUPARROW			0x00400000
#define BULLETATB_FURHEADDEMON				0x00800000
#define BULLETATB_GOLDDRAGONELECTRON		0x01000000
#define BULLETATB_ONGHWA					0x02000000
#define BULLETATB_HALT						0x04000000
#define BULLETATB_FRAGMENT					0x08000000
#define BULLETATB_BUTTERFLY					0x10000000
#define BULLETATB_WIDOW						0x20000000
#define BULLETATB_SEAHORSE					0x40000000
#define BULLETATB_POISONARROW				0x80000000

//#define BULLET_SPEED_ARROW	30		
//#define BULLET_SPEED_GUN	40		

// 총알에 따르는 연기의 속성. 
#define BULLETSMOKEATB_SMOKE_EXP0					0x00000001
#define BULLETSMOKEATB_SMOKE_EXP6					0x00000002
#define BULLETSMOKEATB_SMOKE_EXP7					0x00000004
#define BULLETSMOKEATB_SMOKE_EXP9					0x00000008
#define BULLETSMOKEATB_SMOKE_EXP10					0x00000010
#define BULLETSMOKEATB_SMOKE_ADVTOWERKEXP1			0x00000020
#define BULLETSMOKEATB_SMOKE_EXP12					0x00000040
#define BULLETSMOKEATB_SMOKE_EXP14					0x00000080
#define BULLETSMOKEATB_SMOKE_EXP15					0x00000100
#define BULLETSMOKEATB_SMOKE_GHOSTCATEXP			0x00000200
#define BULLETSMOKEATB_SMOKE_MAGIC8K_EXP			0x00000400
#define BULLETSMOKEATB_SMOKE_GOLDDRAGONELECTRONEXP	0x00000800
#define BULLETSMOKEATB_TRAIL_ARROWTRAIL				0x00001000
#define BULLETSMOKEATB_SMOKE_3						0x00002000
#define BULLETSMOKEATB_SMOKE_GHOST					0x00004000
#define BULLETSMOKEATB_SMOKE_PAPERDOLL				0x00008000
#define BULLETSMOKEATB_SMOKE_SOULCHANGE				0x00010000
#define BULLETSMOKEATB_SMOKE_JINEMPEROR				0x00020000			// actdoll (2004/03/06 9:54) : 유명계/진시황릉 진시황의혼
#define	BULLETSMOKEATB_SMOKE_GUARDIAN_RABBIT		0x00040000
#define BULLETSMOKEATB_SMOKE_GAURDIAN_TIGER			0x00080000
#define BULLETSMOKEATB_SMOKE_GUARDIAN_CHICK			0x00100000

//----------------------------------------------------------------------------
// robypark (2004/05/18 15:40) : 추가 캐릭터 사용 bullet smoke - 장수 2차 전직 사용 bullet smoke
//----------------------------------------------------------------------------
#define BULLETSMOKEATB_SMOKE_TURTLE_FIREBALL		0x00200000		// 조선-거북차; 파이어 볼
#define BULLETSMOKEATB_SMOKE_THUNDER_BOLT			0x00400000		// 조선-뇌전차; 번개 볼트
#define BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET		0x00800000		// 일본-흑룡차; 독, 얼음 공격
#define BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE	0x01000000		// 중국-발석거; 불붙은 돌덩이
#define BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE	0x02000000		// 대만-화룡차; 화룡출수
#define BULLETSMOKEATB_SMOKE_CHINESEPHOENIX_FEATHER	0x04000000		// 대만-봉황비조; 봉황비조 깃털 미사일
#define BULLETSMOKEATB_SMOKE_VOLCANO_BULLET			0x08000000		// 일본-지진차; 격노염폭 시전시 화산탄 히트 효과

// robypark 2004/6/4 19:17
#define BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB	0x10000000		// 중국-불랑기포차 기술 앙천대소 폭발효과 사용

// robypark 2004/11/24 17:40
#define BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT	0x20000000	// 공성전 발석거 돌 히트 효과 사용

// robypark 2004/12/6 13:38
#define BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT				0x40000000	// 공성전 궁병 화살 히트 효과 사용
//--------------------------------------------------------------------------------------------------------------------

// 새로운 Bullet추가는 HQ안에 들어가는 InfoBullet.txt에 추가해주며, 기본적인 정보를 얻어온다.

// bullet.cpp에서 선언된 Class를 bullet.h로 소스 이동
// 총알 종류 정보 클래스
class _BulletKindInfo
{
public:
	// robypark 2004/9/30 18:28
	// Smoke 정보를 HQ에서 InfoBullet.txt에서 얻어와 사용한다.
	void Init(char *pszKindID, SI32 BulletAtb, SI32 BulletSmokeAtb);

	SHORT m_ssStepWidth;
	SHORT m_ssFile, m_ssFont;
	SI32  m_siMoveFrameNumber;
	
	// 방향마다 다른 그림들이 존재하는가(TRUE), 한가지 그림만 존재하는가 ?(FALSE)
	BOOL m_bDirSwitch;                          

	SHORT m_ssTailSwitch, m_ssChaseSwitch, m_ssCurveSwitch, m_ssTransparentSwitch;
	SHORT m_ssHeightSwitch;  // 고저차에 의한 공격력 차이 

	SI32 m_siAtb;
	SI32 m_siSmokeAtb;
	SI32 m_siHitEffectSoundIdx;			// 총알 명중 효과음. Hash Table 이용한 평가 값

	SI32 m_siDelay;
	
};

class _Bullet
{
public:
	SHORT m_ssAnimation;									// 어떤 종류의 애니메이션을 수행하는가?
	SHORT m_ssDrawSwitch;									// 그려줄 것인가 말것인가?
	SHORT m_ssReverseSwitch;                // 뒤집어서 찍을 것인가?
	SHORT m_ssTransparentSwitch;
	SHORT m_ssHeightSwitch;
	SHORT  m_ssDrawStartX, m_ssDrawStartY;      // 그림이 그려지는 시작 위치 
	SHORT  m_ssDrawXsize,  m_ssDrawYsize;       // 그림의 가로 세로 사이즈 
	SHORT m_ssFile, m_ssFont;
	SHORT m_ssAniStep;
	char m_scRealImageXPercent, m_scRealImageYPercent;  // 실제 이미지가 가로 세로로 각 몇 %를 점하는가?
	SHORT m_ssRealImageStartX, m_ssRealImageEndX;        // 실제 이미지가 점하는 가로 세로 좌표 
	SHORT m_ssRealImageStartY, m_ssRealImageEndY; 

	SHORT  m_ssCurrentOrder;               // 현재 캐릭터가 하고 있는 작업 

	SHORT  m_ssID;                          // 캐릭터의 식별 번호 
	SHORT m_ssKind;                        // 어떤 종류의 obj인가?
	SHORT m_ssCenterX, m_ssCenterY;

	SHORT m_ssStepWidth;                    // 이동 단위

	char m_scArea;                        // 공중에 있는가 아니면 지상에 있는가?
	int m_siTargetDirection;               // 목표의 방위 
	int m_siTempDirection;                // 16방위로 존재하는가?

	SHORT  m_ssX, m_ssY;                        // obj의 중심 위치 

	// 이동관련 
	char m_scMoveFrameNumber;
    SHORT m_ssMoveFile[9], m_ssMoveFont[9];

	SHORT m_ssRealX, m_ssRealY;
	SHORT m_ssRealDX, m_ssRealDY;
	SHORT m_ssMapSX, m_ssMapSY;
	SHORT m_ssMapDX, m_ssMapDY;
	SHORT m_ssOriginalRealDX, m_ssOriginalRealDY;

	SHORT m_ssCurveBulletRealX, m_ssCurveBulletRealY;
	SHORT m_ssOldCurveBulletRealX, m_ssOldCurveBulletRealY;
	SHORT m_ssCurveBulletDrawX, m_ssCurveBulletDrawY;
	SHORT m_ssCurveSwitch;

	SHORT m_ssCurveRate;					// 얼마나 곡선을 이루는가?

	_NewID m_AttackID;                    // 공격할 대상 아이디 
	SHORT m_ssApower;

	_NewID m_ShooterID;
	SHORT m_ssShooterPlayer;

	SHORT m_ssMoveStep;
	SHORT m_ssMaxMoveStep;

	DWORD m_uiSkillTime;

	SHORT m_ssType;
    SHORT m_ssHitIDIndex;
    _NewID m_HitID[MAX_HIT_ID];

	SHORT m_ssTailSwitch;					// 꼬리를 만들것인지 여부를 알려주는 변수 
	SHORT m_ssChaseSwitch;					// 추적할 것인가 말것인가?
	SHORT m_ssChaseRepeatNumber;
	SHORT m_ssVaryX, m_ssVaryY;


	SHORT m_ssCourseX[MAX_SHOOT_RANGE],m_ssCourseY[MAX_SHOOT_RANGE];

	SHORT m_ssBallSize;

	SI32 m_ssHitRate;						// 총알이 명중할 확률. 

	// robypark (2004/6/12 12:27
	// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
	// 설정해준다.
	bool	m_bLayerAboveVolcano;

	// robypark (2004/5/21 18:44) 화룡출수, 발석거 불 붙은 돌 애니메이션 보간을 위한 추가
	//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	//		따라 프레임 스킵이 필요하다.
	// 이동 단계에 따른 프레임 번호를 저장하고 있다.
	SHORT		m_snFrameIndex[MAX_SHOOT_RANGE];

	// robypark (2004/5/27 18:49)
	// 불랑기포차의 3발 포탄의 시차 발사를 위한 변수
	SHORT		m_snCurrentDelay;	// 현재 지체된 시간
	SHORT		m_snMaxDelay;		// 지체할 시간

	protected:
		// robypark 2004/6/4 19:25
		// 불랑기포차 기술 앙천대소 폭탄 폭발시 파편을 생성해준다.
		void CreateFragment(void);

	public:
		_Bullet();
	  void Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooter_id, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetidir, SHORT attack_area);

	// robypark (2004/5/21 18:44) 화룡출수 애니메이션 보간을 위한 추가
	//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	//		따라 프레임 스킵이 필요하다.
	//  이동 단계에 따른 애니메이션 프레임을 설정한다.
	void CreateFrameIndex_4FireDragonMissile(void);

	// robypark (2004/5/27 19:9)
	// 불랑기포차의 3발 포탄 시차 적용
	void SetLuanchDelay(SHORT snMaxDelay);

	// robypark (2004/6/12 12:27
	// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
	// 설정해준다.
	void SetLayerAboveVolcano(bool bAbove = true);

      // 한번 공격한 캐릭터를 다시 공격하지 않기 위해서.
      // 스택을 초기화한다. 
      void InitPushID();
	  void PushHitID(_NewID id);
	  BOOL IsAlreadyPushID(_NewID id);
	  // 찍을 위치를 계산한다.
	  void CalculateDrawStartXY();

      void CalculateRealXY();
      // 찍어야 할 그림을 선정한다. 
      void DecideFont();
	  void DecideFontMove();
	  void DecideFontMoveNormal();
	  void DecideFontMoveFireTankArrow();

	  void SetMoveImage16(SHORT file, SHORT font, SHORT frame);
	  void SetMoveImage(SHORT ani, SHORT file, SHORT font);

	  void GetCourse();
	  void GetCourseCurve(SHORT startstep);
	  void GetCourseCurve();
	  BOOL Draw();
	  BOOL Action();
	  BOOL Hit();
      BOOL HitNormal();

	  BOOL HitFireArrow();
	  BOOL HitNeedle();
	  BOOL HitGun();
	  BOOL HitStrongGun();
	  BOOL HitAirMissile();
	  BOOL HitAttackerC();
	  BOOL HitSamMissile();
	  BOOL HitAirMagic();
	  BOOL HitCannon();    
	  BOOL HitHeavyGun();  
	  BOOL HitBomb();      
	  BOOL HitFire();      
	  BOOL HitShipCannon();
	  BOOL HitThrowBomb(); 
	  BOOL HitFireTankArrow();
	  BOOL HitMagicBall(); 
	  BOOL HitFragment1(); 

	  void SetFileNFont();

	  int GetArea(){return m_scArea;}
	  void SetInfo();
	  void DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	  void DecideDirectionFireTankArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	  void DecideDirectionNormal(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
  
};


SHORT FindEmptyBullet();
void DeleteBullet();
void DeleteBullet(SHORT id);

SHORT SetBullet(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooterid, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetdir, char attack_area);

void InitBulletKindInfo();

// robypark 2004/6/12 12:38
extern _Bullet g_Bullet[MAX_BULLET_NUMBER]; // 총알용 테이터 

// 총알 정보 변수 전역 변수 선언
extern _BulletKindInfo g_BKI[MAX_BULLET_KIND_NUMBER];

//#endif

// robypark (2004/5/27 19:11)
// 불랑기포차 3발사 시차 발사 위한 함수
void Bullet_SetLuanchDelay(int i, SHORT snMaxDelay);

BOOL Bullet_Action(int i);
// 2001.11.12	hojae_append
BOOL Bullet_Action(int i);
void Bullet_CalculateDrawStartXY(int i);
void Bullet_Draw(int i);
void Bullet_DrawSwitch(SHORT bulletid, BOOL TorF);
void Bullet_CurrentOrder(SHORT bulletid, SHORT DEF);
void Bullet_Fwrite(FILE* fp);
void Bullet_Read(FILE* fp);

// robypark (2004/6/12 12:27
// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
// 설정해준다.
void SetLayerAboveVolcano(SI32 siBulletID, bool bAbove = true);

#endif
///////////////////////////////////////////////