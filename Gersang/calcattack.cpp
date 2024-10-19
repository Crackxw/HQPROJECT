// 공격의 각종 속성에 따라 적절하게 피해를 입히는 함수를 구현한다. 
#include <GSL.h>

//#include <main.h>
#include <clGame.h>

#include "maindata.h"
#include <etc.h>
#include "calcattack.h"
#include "music.h"
#include <mynet-game.h>
#include <OnlineBattleMsg.h>
#include <CharOrg.h>

#include <charinterface.h>
#include <MapInterface.h>
#include <smoke.h>
#include <setchar.h>
#include "OnlineGeneral-Parser.h"

extern _clGame* pGame;

extern	cltCharInterface		clifChar;							// 캐릭터 관련 인터페이스 
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 
extern	_MainData				GD;									// 게임의 주데이터 
extern	_Char					CR[];								// 캐릭터의 포인터 
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 
extern	SI32					UserPlayerNumber;					// 사용자 관련 
extern	_MyNetGame				MyNet;
extern _OnlineBattleMsgQue OBMQue;
extern OnlineBattleInfo OBInfo;
extern	CItem					*g_pItem;

#include <yPutGetResource.h>

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb
#include "..\EnumData\Enum_DefenceAtb.h"	// DefenceAtb

SHORT HitListIndex=0;
SHORT HitList[MAX_HIT_LIST_NUMBER]; 


SI32 AttackAtbInfo[ATTACKATB_MAX];

// 데미지 속성과 방어 속성의 관계 
static SI32 RelationDamageAndDefence[]=
{
	// 데미지 속성.   DEFENCEATB_HUMAN	_ARMOUR _HORSE _AIRPLANE _OILARMOUR _FLOAT _ANIMAL _GHOST _SOFT 
	HITATB_SWORD,		100,			80,		80,		0,			80,		50,		150,	80,		50,			
	HITATB_RANGE,		100,			70,		40,		100,		100,	120,	80,		70,		70,
	HITATB_FIREARROW,	100,			70,		40,		200,		100,	120,	80,		70,		70,
	HITATB_MAGIC,		150,			100,	120,	0,			80,		100,	90,		150,	100,	
	HITATB_FIRE,		150,			50,		150,	0,			200,	50,		130,	30,		100,	
	HITATB_SKILL,		110,			80,		90,		50,			100,	100,	150,	100,	110,		
	-1
};

// 공격 속성에 따른 데미지 속성
static SI32 DamageAtbByAttackAtb[]=
{
	ATTACKATB_SWORD,				HITATB_SWORD, 
	ATTACKATB_BIGSWORD,				HITATB_SWORD,	
	ATTACKATB_DUEOKSINI,			HITATB_SWORD,
	ATTACKATB_TOAD,					HITATB_SWORD,
	ATTACKATB_BUTTERFLY,			HITATB_SWORD,
	ATTACKATB_GUN,					HITATB_RANGE,
	ATTACKATB_MACHINEGUN,			HITATB_RANGE,
	ATTACKATB_STRONGGUN,			HITATB_RANGE,
	ATTACKATB_CANNON,				HITATB_RANGE,
	ATTACKATB_THROWBOMB,			HITATB_RANGE,
	ATTACKATB_ARROW,				HITATB_RANGE,
	ATTACKATB_FIREARROW,			HITATB_FIREARROW,
	ATTACKATB_POISONARROW,			HITATB_RANGE,
	ATTACKATB_FURHEADDEMON,			HITATB_RANGE,
	ATTACKATB_SNOWWOMAN,			HITATB_RANGE,
	ATTACKATB_GOLDDRAGONELECTRON,	HITATB_RANGE,
	ATTACKATB_ONGHWA,				HITATB_RANGE,
	ATTACKATB_GHOSTCAT,				HITATB_RANGE,
	ATTACKATB_DOLL1,				HITATB_RANGE,
	ATTACKATB_DOLL2,				HITATB_RANGE,
	ATTACKATB_JUSTICEK,				HITATB_RANGE,
	ATTACKATB_GATPHAPOISON,			HITATB_RANGE,	
	ATTACKATB_WIDOW,				HITATB_RANGE,
	ATTACKATB_THROWSPEAR,			HITATB_RANGE,
	ATTACKATB_MAGIC,				HITATB_MAGIC, 
	ATTACKATB_EARTHQUAKE,			HITATB_MAGIC,
	ATTACKATB_ELECTRON,				HITATB_MAGIC, 
	ATTACKATB_TORNADO,				HITATB_MAGIC,
	ATTACKATB_FIRE,					HITATB_FIRE,
	ATTACKATB_MAGIC5J,				HITATB_SKILL, 
	ATTACKATB_ELEPHANT,				HITATB_SKILL,
	ATTACKATB_MAGIC8K,				HITATB_SKILL,
	ATTACKATB_SELFEXP,				HITATB_SKILL,
	ATTACKATB_WAVE,					HITATB_SKILL,
	ATTACKATB_SCORPION,				HITATB_RANGE,
	ATTACKATB_CACTUS,				HITATB_RANGE,
	ATTACKATB_SWORD,				HITATB_SWORD, 
	ATTACKATB_EVIL,					HITATB_RANGE,
	ATTACKATB_JAPANGHOST,			HITATB_RANGE,
	ATTACKATB_KUKET,				HITATB_RANGE,
	ATTACKATB_AUTA,					HITATB_RANGE,
	ATTACKATB_SUB1,					HITATB_RANGE,
	ATTACKATB_SUB2,					HITATB_RANGE,
	ATTACKATB_FIREATTACK,			HITATB_RANGE,
	ATTACKATB_RUNNINGFIRE,			HITATB_RANGE,
	ATTACKATB_ENERGYPA,				HITATB_RANGE,

	// actdoll (2004/03/06 4:54) : 3월 패치 추가자료
	ATTACKATB_PYTHON,				HITATB_MAGIC,	// 이무기 
	ATTACKATB_JINEMPEROR,			HITATB_MAGIC,	// 진시황의혼

	// robypark (2004/5/19 17:29) : 장수 2차 전직 관련 추가
	// 110	조선-거북차, 이순신 2차 전직
	ATTACKATB_TURTLE_VEHICLE,					HITATB_FIRE,
	// 111	조선-뇌전차, 허준 2차 전직
	ATTACKATB_THUNDER_VEHICLE,					HITATB_MAGIC,
	// 112	일본-지진차, 와키자카 2차 전직
	ATTACKATB_QUAKE_VEHICLE,					HITATB_MAGIC,
	// 113	일본-흑룡차, 세이메이 2차 전직
	ATTACKATB_BLACK_DRAGON_VEHICLE,				HITATB_RANGE,
	// 114	중국-발석거, 이령 2차 전직
	ATTACKATB_CATAPULT,							HITATB_RANGE,
	// 115	중국-불랑기포차, 순비연 2차 전직
	ATTACKATB_FARANGI_VEHICLE,					HITATB_RANGE,
	// 116	대만-화룡차, 손유창 2차 전직
	ATTACKATB_FIRE_DRAGON_VEHICLE,				HITATB_RANGE,
	// 117	대만-봉황비조, 장선화 2차 전직
	ATTACKATB_CHINESEPHOENIX,					HITATB_RANGE,

	ATTACKATB_CATDOLL,							HITATB_RANGE,
	ATTACKATB_OLDFOXDOLL,						HITATB_RANGE,
	ATTACKATB_RACCOONDOLL,						HITATB_RANGE,
	ATTACKATB_NINJADOLL,						HITATB_RANGE,
	ATTACKATB_SNOWWOMANDOOL,					HITATB_RANGE,
	ATTACKATB_YANGGUIBIDOLL,					HITATB_RANGE,
	ATTACKATB_JIJANGDOLL,						HITATB_RANGE,
	ATTACKATB_SNAKEWOMANDOLL,					HITATB_RANGE,

	// robypark (2004/6/4 13:6)
	// 126	불랑기포차 앙천대소 공격
	ATTACKATB_ROAR_OF_LAUGHTER,					HITATB_RANGE,

	// robypark 2004/6/10 19:4
	// 흑룡차 천지독살, 냉기공격
	ATTACKATB_BLACK_DRAGON_ICE,					HITATB_RANGE,

	-1
};



void InitAttackAtb()
{
	AttackAtbInfo[ATTACKATB_SWORD				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_TYPE1;
	AttackAtbInfo[ATTACKATB_STRONGGUN			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_AIRMISSILE			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_SAMMISSILE			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_CANNON2C			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_NEEDLE				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_CANNON				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_GUN					]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_MACHINEGUN			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_GATPHAPOISON		]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_POISON;
	AttackAtbInfo[ATTACKATB_MAGIC				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_MAGIC2				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_MAGIC3				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_FIRECAR				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_SHIPCANNON			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_FIRE				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_HEAVYGUN			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_FIREARROW			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR ;
	AttackAtbInfo[ATTACKATB_ARROW				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_SELFEXP				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_LIGHTNING			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_LONG_SPEAR			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_EARTHQUAKE			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_FIREATTACK			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_RUNNINGFIRE			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_PULL;
	AttackAtbInfo[ATTACKATB_ENERGYPA			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 ;
	AttackAtbInfo[ATTACKATB_BIGSWORD			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_TYPE1;
	AttackAtbInfo[ATTACKATB_ELECTRON			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_THROWBOMB			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_MAGICBALL			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;

	AttackAtbInfo[ATTACKATB_BOMB				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_TORNADO				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_KIMBOMB				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_MAGIC5J				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_DART				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_MAGIC8K				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CANNONJ				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE1;

	AttackAtbInfo[ATTACKATB_FIRETANK			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_COLLTANK			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CANNON2J			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_POISONARROW			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE | ATTACKATB_ATB_POISON;
	AttackAtbInfo[ATTACKATB_FIRETANKARROW		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_COLLTANKJARROW		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_GENERAL4KFIRE		]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_GENERAL4J			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_GENERAL3C			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CANNONKARROW		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_FLAMETANKJARROW		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CANNONC				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_AXE					]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_SWORDENERGY			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_ELEPHANT			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_ANTIARMOUR			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_CHASEFIRE			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_AIRATTACKUPARROW	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_TOWERCANNON			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_UPGRADETOWERCANNON	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_DUEOKSINI			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_TYPE1|ATTACKATB_ATB_PULL;
	AttackAtbInfo[ATTACKATB_TOAD				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_TYPE1;
	AttackAtbInfo[ATTACKATB_FURHEADDEMON		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE |ATTACKATB_ATB_PALSY;
	AttackAtbInfo[ATTACKATB_ICE					]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_MINRANGE |ATTACKATB_ATB_PALSY;
	AttackAtbInfo[ATTACKATB_MAGIC4T				]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_SNOWWOMAN			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_GOLDDRAGONELECTRON	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_ONGHWA				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_GHOSTCAT			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_DOLL1				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_DOLL2				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_JUSTICEK			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;
	AttackAtbInfo[ATTACKATB_BUTTERFLY			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE1;
	AttackAtbInfo[ATTACKATB_WIDOW				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE1;
	AttackAtbInfo[ATTACKATB_WAVE				]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_LEECH				]	= ATTACKATB_ATB_PHYSICS  | ATTACKATB_ATB_TYPE1 |ATTACKATB_ATB_VAMPIRE;
	AttackAtbInfo[ATTACKATB_SEAHORSE			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_THROWSPEAR			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_CACTUS				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_SCORPION			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_AIR | ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_CLAW				]	= ATTACKATB_ATB_PHYSICS  | ATTACKATB_ATB_TYPE1 |ATTACKATB_ATB_VAMPIRE;
	AttackAtbInfo[ATTACKATB_JAPANGHOST			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2| ATTACKATB_ATB_MINRANGE;
	AttackAtbInfo[ATTACKATB_KUKET				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CATDOLL				]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_OLDFOXDOLL			]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_RACCOONDOLL		    ]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_NINJADOLL			]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_SNOWWOMANDOOL		]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_YANGGUIBIDOLL		]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_JIJANGDOLL			]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_SNAKEWOMANDOLL		]   = ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_PAPERDOLL			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_NEEDLEATTACK		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_CLOUDWIND			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_STONEMAGIC			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_GODSPEAR			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_PULL | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_FLOWERRAIN			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;
	AttackAtbInfo[ATTACKATB_SOULCHANGE			]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_MAGICCATTACK		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_SUNBIYOUN			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;

//----------------------------------------------------------------------------
// actdoll (2004/03/06 4:56) : 추가 캐릭터 - 유명계/진시황릉 관련 START
//----------------------------------------------------------------------------

	AttackAtbInfo[ATTACKATB_PYTHON				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_JINEMPEROR			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;

//----------------------------------------------------------------------------
// actdoll (2004/03/06 4:56) : 추가 캐릭터 - 유명계/진시황릉 관련 END
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// damduk : 추가 캐릭터 - 수호부 관련
//----------------------------------------------------------------------------
	AttackAtbInfo[ATTACKATB_GUARDIAN_PIG		]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;

//----------------------------------------------------------------------------
// robypark (2004/05/18 11:05) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	// 110	조선-거북차, 이순신 2차 전직
	AttackAtbInfo[ATTACKATB_TURTLE_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// 111	조선-뇌전차, 허준 2차 전직
	AttackAtbInfo[ATTACKATB_THUNDER_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;

	// 112	일본-지진차, 와키자카 2차 전직
	AttackAtbInfo[ATTACKATB_QUAKE_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// 113	일본-흑룡차, 세이메이 2차 전직
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_VEHICLE]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_PALSY | ATTACKATB_ATB_POISON;

	// 114	중국-발석거, 이령 2차 전직
	AttackAtbInfo[ATTACKATB_CATAPULT			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// 115	중국-불랑기포차, 순비연 2차 전직
	AttackAtbInfo[ATTACKATB_FARANGI_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_MINRANGE;

	// 116	대만-화룡차, 손유창 2차 전직
	AttackAtbInfo[ATTACKATB_FIRE_DRAGON_VEHICLE	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// 117	대만-봉황비조, 장선화 2차 전직
	AttackAtbInfo[ATTACKATB_CHINESEPHOENIX		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// robypark (2004/6/4 13:6)
	// 앙천대소 공격, 불랑기포차
	AttackAtbInfo[ATTACKATB_ROAR_OF_LAUGHTER	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// robypark 2004/6/10 19:4
	// 흑룡차 천지독살, 냉기공격
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_ICE	]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_PALSY;

	// robypark 2004/6/11 10:49
	// 흑룡차 천지독살, 독공격
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_POISON	]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_POISON;
}


SHORT GetRealApower(SHORT apower, SHORT attackatb, DWORD defenceatb)
{
	SI32 pos	=	0;	

	// 공격 속성에 따른 데미지 속성을 찾는다.
	SI32 index	= 0;
	SI32 data	= 0;
	SI32 damageatb = 0;

	while(1)
	{
		data = DamageAtbByAttackAtb[index*2];
		if(data == -1)
		{
			damageatb = HITATB_SWORD;
			break;
		}

		if(data == attackatb)
		{
			damageatb = DamageAtbByAttackAtb[index*2 + 1];
		}
	}

	// 데미지 속성과 방어 속성을 고려하여 실제 공격력을 구한다.
	index = 0;
	while(1)
	{
		pos = index * (DEFENCEATB_MAX+1);
		data = RelationDamageAndDefence[pos];

		if(data == damageatb)
		{
			data = RelationDamageAndDefence[pos + defenceatb];
			break;
		}
		else if(data == -1)
		{
			data = 100;
			break;
		}
	}

	// 실제 공격력을 계산한다. 
	return (apower + (apower * data) / 100);	// -1: atb 값은 1부터 시작하므로
}

void InitHitList()
{
	HitListIndex=0;
}

BOOL IsInHitList(SHORT id)
{
	SHORT i;

	for(i=0;i<HitListIndex;i++)
	{
		if(HitList[i]==id)return TRUE;
	}

	return FALSE;
}


void PushHitList(SHORT id)
{
	if(HitListIndex>=MAX_HIT_LIST_NUMBER)return ;

	HitList[HitListIndex]=id;
	HitListIndex++;
}
 

// 공격속성, 공격력, 공격대상을 고려하여 실제 피해를 계산한다. 
SHORT CalculateHitPoint(SHORT attackatb, SHORT power, _NewID attackid)
{

	SHORT validpower=0;
	SHORT assistdpower=0;
	SI32 damageresist = 0;
	SI32 magicresist = 0;

	// 공격 목표가 이미 유효하지 않으면 계산 할 수 없다.
	if(IsSee(attackid.ID)==FALSE || attackid.IsSame(clifChar.GetID(attackid.ID))==FALSE)return 0;

	// 상대가 포복중이고 
	// 포복중에 방어력향상이 있다면, 
	if(clifChar.GetLowMoveSwitch(attackid.ID) == TRUE
		&& (AttackAtbInfo[attackatb] &  ATTACKATB_ATB_LOWASSISTDPOWER))
	{
		// 30%의 방어력 향상 효과. 
		power = power * 70 / 100;
	}
	

	// 공격 속성이 물리적일때. 
	if(AttackAtbInfo[attackatb] & ATTACKATB_ATB_PHYSICS)
	{
		
		damageresist = CR[attackid.ID].m_ParmeterBox.GetTotalDamageResist();
		
		return max(1, power * (100 - min(110, damageresist)* 8 / 10) / 100);
	}
	// 공격 속성이 마법적일때. 
	else if(AttackAtbInfo[attackatb] & ATTACKATB_ATB_MAGIC)
	{
		SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[attackid.ID].m_cltCharStatus.GetKind()));
		//2차 전직 장수라면.
		//if(siCharType == 3)
		//	return 0;
		//else
		{
			//2차 전직 장수가 아니면.
			magicresist = CR[attackid.ID].m_ParmeterBox.GetTotalMagicResist();
			return max(1, power * (100 - min(110, magicresist)* 8 / 10) / 100);
		}
	}
	else
	{
		clGrp.Error("fdf8927","fd983hfff:%d", attackatb);
	}


	return validpower;
}

// 공격의 여러 속성을 고려하여 적의 체력을 저하시킨다. 
SHORT CalculateHit(_NewID myid, SHORT myplayernumber, SHORT attackatb, SHORT range, SHORT area, SHORT power, _NewID attackid, SHORT dx, SHORT dy, SI32 hitrate)
{

	SHORT i, j;
	SHORT length=0;
	SHORT damage=0;
	SHORT realpower;
	SHORT id, temp_id;

	// 누군가가 공격하고 있다는 사실을 알린다.
	if(IsAlive(myid) && IsAlive(attackid))
		clifChar.SetBeAttackedSwitch(attackid.ID, myid.ID, myplayernumber);

	// 명중률을 근거로 타격 여부를 결정한다.
	if(GD.Random() % 100 > (UI32)hitrate)
		return 0;

	// 화살인 경우에는 맞았을 때 효과음 연주. 
	if(attackatb == ATTACKATB_ARROW)
	{
		if(IsAlive(attackid))
		{
			PushEffect(EFFECT_ARROWHIT, CR[attackid.ID].GetX(), CR[attackid.ID].GetY());
		}
	}
	// 독침인 경우 맞았을 때 중독시킨다. 
//	if(attackatb == ATTACKATB_POISONARROW)
	if (AttackAtbInfo[attackatb] & ATTACKATB_ATB_POISON)
	{
		if(IsAlive(attackid))
		{
			CR[attackid.ID].m_cltPoisonEffect.Set(TRUE, 200, power/3);

			// robypark 2004/8/9 13:37
			// 독 공격을 한 캐릭터 ID 설정
			CR[attackid.ID].m_cltPoisonEffect.SetPoisonAttacker(myid);
		}
	}

	// 수호부(돼지)의 공격을 받았을 경우 리턴.(임시코드)
	if(CR[attackid.ID].m_bPigDefence)	return 0;

	if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE1 )
	{
		// 목표가 무효하면 리턴
        if(IsSee(attackid.ID)==FALSE)return 0;

		// 목표에서의 오차를 구한다. 
		id=0;
		for(i=-1;i<=1;i++)
			for(j=-1;j<=1;j++)
			{
				// 범위안의 유닛을 찾는다.
				temp_id = clifMap.GetIDMap(dx+j, dy+i);

				// 유닛이 유효하고 공격대상과 일치하면, 
				if(IsSee(temp_id) && attackid.IsSame(clifChar.GetID(temp_id)) == TRUE)
				{
					id=temp_id;
				}
			}

			// 적이 유효하면, 
			if(id)
			{
				HitNOperateAfterHit(myid, clifChar.GetID(id), myplayernumber, attackatb, power);
				return 0;
			}
	}

	else if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE2 )
	{
		// 목표가 무효하면 리턴, 
        if(IsSee(attackid.ID)==FALSE)return 0;

		// 동맹이 아니어야 데미지를 줄 수 있다.(내가 상대방에게 우호적인 상태가 아니어야 ..)
		else if(IsFriendToByPlayerNumber(myplayernumber, CR[attackid.ID].GetPlayerNumber()) == FALSE)
		{
			HitNOperateAfterHit(myid, attackid, myplayernumber, attackatb, power);
		}
		
		return 0;
	}

	else if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE3 )
	{
		if(range<=0)range=1;

		// 가격한 목표의 목록을 초기화한다. 
		InitHitList();


		for(i = -range;i <= range;i++)
			for(j =-range ;j <= range;j++)
			{
				
				if(clifMap.IsInMapArea(dx+j, dy+i)==TRUE)
				{
					
					if(area==AREA_LAND)
					{
						id = clifMap.GetIDMap(dx+j, dy+i);
					}
					else
					{
						id = attackid.ID;
					}
					
					
					// 이미 처리한 캐릭터인지 확인한다. 
					if(IsSee(id) && IsInHitList(id) == FALSE)
					{
						
						length= max(abs(i), abs(j));
						realpower=power-power*(length*100/(range+1))/100;
						
						// 만약 공격 목표가 맞다면, 
						// 데미지를 충분하게 준다. 
						if(id == attackid.ID)
						{
							realpower=power;
						}
						if(attackatb == ATTACKATB_LONG_SPEAR)
						{
							realpower = power;
						}
						/////////////////////////////여기여기여기여기여기.......
						HitNOperateAfterHit(myid, clifChar.GetID(id), myplayernumber, attackatb, realpower);
						
						// 처리한 캐릭터의 목록에 추가한다. 
						PushHitList(id);
					}
				}
			}
	} 
	else
	{
		clGrp.Error("1084", "calcattack.cpp attackatb:%d kind:%d", attackatb, clifChar.GetKind(myid.ID));
	}
	return 0;
}

// 적에 대한 공격과 공격에 사후 조치를 취한다. 
BOOL HitNOperateAfterHit(_NewID myid, _NewID enemyid, SHORT myplayernumber, SHORT attackatb, SHORT power)
{
	// 공격자가 피공격자에 우호적이면 데미지를 입히지 않는다. 
	if(IsFriendToByPlayerNumber(myplayernumber, CR[enemyid.ID].GetPlayerNumber()) == TRUE)
	{
		return TRUE;
	}
//	int a = TransKindOnlineToImjin(CR[myid.ID].m_cltCharStatus.GetKind());

	
	if(IsAlive(myid))
	{
		if(CR[myid.ID].m_bGuardianEquip == true)
		{
			if(CR[myid.ID].m_siGuardianCondition == ON_GUARDIAN_GAME)
			{
				if((TransKindImjinToOnline(CR[myid.ID].m_cltCharStatus.GetKind()) & 255) == 'C')
				{
					if(CR[myid.ID].m_bUseGuardian == false && CR[myid.ID].m_siCount == 0)
					{
						CR[myid.ID].m_bUseGuardian   = true;
						CR[myid.ID].m_dwGuardianTime = timeGetTime();
						CR[myid.ID].m_siCount++;

						CR[myid.ID].m_siAttackX_ForGuardianDragon = CR[enemyid.ID].GetX();
						CR[myid.ID].m_siAttackY_ForGuardianDragon = CR[enemyid.ID].GetY();

						CR[myid.ID].m_GuardianAttackID		 = enemyid;
					}
				}
			}
		}
		else if(CR[enemyid.ID].m_bGuardianEquip == true)
		{
			if(CR[enemyid.ID].m_siGuardianCondition == ON_GUARDIAN_GAME)
			{
				if((TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()) & 255) == 'C')
				{
					if(CR[enemyid.ID].m_bUseGuardian == false && CR[enemyid.ID].m_siCount == 0)
					{
						CR[enemyid.ID].m_bUseGuardian   = true;
						CR[enemyid.ID].m_dwGuardianTime = timeGetTime();
						CR[enemyid.ID].m_siCount++;

						CR[enemyid.ID].m_siAttackX_ForGuardianDragon = CR[myid.ID].GetX();
						CR[enemyid.ID].m_siAttackY_ForGuardianDragon = CR[myid.ID].GetY();

						CR[enemyid.ID].m_GuardianAttackID			 = myid;
					}
				}
			}
		}
	}

//	if((uiCharID  & 255) == 'C')

	// 2차 전직 일경우 수정.
	// 공격에 따른 타격치를 구한다. 
	SHORT damage = CalculateHitPoint(attackatb, power, enemyid);

	// 적이 귀신탄에 걸려있는 경우
	if(CR[enemyid.ID].GhostTime)
		damage = damage*2;

	// myid가 분신인 경우 분신의 주인이 공격자가 된다. 
	// 분신의 전투결과가 주인의 전투결과로 반영이 된다. 
	if(IsAlive(myid) && CR[myid.ID].HostID.IsEmpty()==FALSE)
	{
		myid = CR[myid.ID].HostID;
	}

	// 맞았을 때 그래픽적인 효과
	if(IsAlive(enemyid) == TRUE)
	{
		CR[enemyid.ID].SetNewSelectedSwitch(TRUE);
		CR[enemyid.ID].SetBeAttackedDrawSwitch(TRUE);
		CR[enemyid.ID].SetBeAttackedDrawStep(0);
		
		// 뒤로 밀리는 속성이면 뒤로 민다. 
		if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_PULL)
		{
			// 피격된 캐릭터의 종류를 얻어온다. 2차 장수인지 검사하기위하여 얻어온다.
			SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));

			// robypark 2004/6/11 13:43
			// 2차 장수는 뒤로 밀리지 않는다.
			if (siCharType != 3)
			{
				// 공격자가 유효하면, 
				// robypark 2004/6/10 10:39
				// 불랑기포차가 변신한 앙천대소일 경우에는 뒤로 밀리지 않는다.
				if ((CR[enemyid.ID].m_cltCharStatus.GetKind() != KIND_ON_ROAR_OF_LAUGHTER)
					&& (IsAlive(myid) == TRUE))
				{
					// 때린자와 맞은자의 방향을 토대로 밀려야 하는 곳을 구한다. 
					SI32 dir = DecideDirection8(CR[myid.ID].GetX(), CR[myid.ID].GetY(), CR[enemyid.ID].GetX(), CR[enemyid.ID].GetY());
					CR[enemyid.ID].DeleteID();
					CR[enemyid.ID].Pull(dir);
					CR[enemyid.ID].SetID();
				}
			}
		}
		
		// 마비 되는 속성이면 마비 시킨다. 
		if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_PALSY)
		{
			// 공격자가 유효하면, 
			if(IsAlive(myid) == TRUE)
			{
				// 랜덤하게  마비 시킨다. 
				if(IsAlive(enemyid.ID) &&
						IsFriendToByPlayerNumber(myplayernumber, CR[enemyid.ID].GetPlayerNumber() ) == FALSE
						&& CR[enemyid.ID].IsAtb(ATB_PATIENT)	== TRUE)
				{
						// 20% 의 확률로...
						if(GD.Random() % 100 < 20)
						{
							// robypark 2004-6-3 16:15
							// 천지독살 냉기에 의한 마비, 흑룡차 기본 공격에 의한 마비시
							// 다른 마비효과 이미지를 사용한다.
							if (attackatb == ATTACKATB_BLACK_DRAGON_VEHICLE)
								CR[enemyid.ID].SetHalt(HALT_REASON_FOG_OF_ICE, 150);
							// robypark 2004/6/10 19:4
							// 흑룡차 천지독살, 냉기공격
							else if(ATTACKATB_BLACK_DRAGON_ICE == attackatb)
								CR[enemyid.ID].SetHalt(HALT_REASON_FOG_OF_ICE, 150);
							else
								CR[enemyid.ID].SetHalt(HALT_REASON_ICE, 150);

							OrderStop(enemyid, BY_PLAYER);
						}
				}
			}
		}

	}


	// 체력 흡수 속성이 있으면, 
	if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_VAMPIRE)
	{

		if(IsAlive(myid)==TRUE)
		{
			CR[myid.ID].SetHealDraw(TRUE);
			
			// 상대방 에너지를 흡수하여 자신의 에너지로 만든다. 
			CR[myid.ID].IncreaseLife(damage/3); 
		}
	}


	// 사용자 캐릭터가 당한 것이거나 
	// 몬스터가 당한 경우이고 임시 호스트인 경우, 
	SI32 victimplayernumber = CR[enemyid.ID].GetPlayerNumber();

	if( victimplayernumber== UserPlayerNumber
	|| (ISMONSTER(MyNet.PlayersdpId.dpId[victimplayernumber]) && MyNet.IsHost()) )
	{}
	else return TRUE;

	
	// 유닛의 체력을 줄여준다.
	// enemyid.ID캐릭터가 죽었다면.
	if(clifChar.ReduceLife(enemyid.ID, damage, 10) == FALSE)
	{
		SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));
		if(siCharType != 3)
		{
			if(CR[enemyid.ID].m_bUseGuardian == TRUE)
				CR[enemyid.ID].GuardianBreakless(CR[enemyid.ID].m_siGuardianKind);
			// 캐릭터 정보의 변경을 통보한다. 
			if(OBInfo.IsSingleMode == TRUE)
			{
				// 서버에 내가 누군가를 죽였음을 통보한다. 
				if(IsAlive(myid))
				{
					// 무도장이 아니어야 한다. 
					if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
							MyNet.PlayersdpId.dpId[ CR[myid.ID].GetPlayerNumber()],		CR[myid.ID].m_siOnlineID, 
							MyNet.PlayersdpId.dpId[ CR[enemyid.ID].GetPlayerNumber()],	CR[enemyid.ID].m_siOnlineID);
						
						// 킬러에게 합당한 경험치등을 제공한다. 
						pGame->pBattle->ExpOperate(myid.ID, enemyid.ID);

					
					}
				}
#ifdef _SET_SERVER_MESSAGE
				// robypark 2004/8/9 14:46
				// 공격한 캐릭터가 죽었을 경우 몬스터가 찌꺼기로 남지 않고 삭제하기
				else
				{
					// 중독되어 죽는 캐릭터일 경우, 공격한 캐릭터가 사망한 경우 몬스터가 찌꺼기로 남지 않도록 삭제하기
					if (CR[enemyid.ID].m_cltPoisonEffect.GetPoisonSwitch())
					{
						// 무도장이 아니어야 한다. 
						if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
						{
							OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
								MyNet.PlayersdpId.dpId[ CR[myid.ID].GetPlayerNumber()],		CR[myid.ID].m_siOnlineID, 
								MyNet.PlayersdpId.dpId[ CR[enemyid.ID].GetPlayerNumber()],	CR[enemyid.ID].m_siOnlineID);
						}
					}
				}
#endif
			}
			else
			{
				pGame->pBattle->PushUpdateCharInfo(enemyid.ID, myid.ID);
			}
		}

		//
		//2차 전직 장수가 죽었을 때 일반 장수를 부른다.
		//GeneralHeader
		/*
		GeneralHeader* pGeneralHeader = pGame->pOnlineWorld->pGeneralParserEx->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));
		if(pGeneralHeader != NULL)
		{	
			//kind를 기억한다.
			_ImportantParameta ip;
			ZeroMemory(&ip, sizeof(_ImportantParameta));
			//UI16 uiKind = TransKindImjinToOnline(KIND_ON_GUARDIAN_CHICK);

			//CR[myid.ID].m_ParmeterBox.IP.GetMana()

			UI16 uikind;

			uikind = TransKindOnlineToImjin(pGeneralHeader->siGeneralBaseCharacterCode);
			
			ip.SetStr(  CR[enemyid.ID].m_ParmeterBox.IP.GetStr());
			ip.SetDex(  CR[enemyid.ID].m_ParmeterBox.IP.GetDex());
			ip.SetVit(  CR[enemyid.ID].m_ParmeterBox.IP.GetVit());
			ip.SetInt(  CR[enemyid.ID].m_ParmeterBox.IP.GetInt());
			ip.SetAC(   CR[enemyid.ID].m_ParmeterBox.IP.GetAC());
			ip.SetLife( CR[enemyid.ID].m_ParmeterBox.IP.GetMaxLifeByIP()/10);
			ip.SetMana( CR[enemyid.ID].m_ParmeterBox.IP.GetMaxManaByIP()/10);
			ip.SetLevel(CR[enemyid.ID].m_ParmeterBox.IP.GetLevel());

			//SI32 siPrevKind = CR[enemyid.ID].m_cltCharStatus.GetKind();
			SetChar(uikind, enemyid.ID, CR[enemyid.ID].GetX(), CR[enemyid.ID].GetY(), CR[enemyid.ID].Direction, 
				100, CR[enemyid.ID].GetPlayerNumber(), &ip, NULL, CR[enemyid.ID].OnlineID);
		}*/

		// 유닛이 사망하면 사망에 따른 조치를 취한다. 
		KillOperation(myplayernumber, enemyid);
		

		// 화염 공격으로 사망하면 불을 붙여준다.
		// 치료가능한 유닛만이고, 
		// 공중 유니트는 안된다.
		if(attackatb==ATTACKATB_FIRE 
		&& clifChar.IsAtb(enemyid.ID, ATB_PATIENT)
		&& clifChar.IsAtb(enemyid.ID, ATB_FLY)==FALSE)
		{
		   SHORT smokeid=FindEmptySmoke();

		   if(smokeid)
		   {
			   PushEffect(EFFECT_FIRE, clifChar.GetX(enemyid.ID), clifChar.GetY(enemyid.ID));
		       SetSmoke(KIND_SMOKE_MANFIRE, smokeid, clifChar.GetCenterX(enemyid.ID), clifChar.GetCenterY(enemyid.ID), AREA_LAND);
		   }
		}

	}
	// 죽지 않았다면 공격 받는 중이라는 것을 알린다. 
	else
	{
		clifChar.SetBeAttackedSwitch(enemyid.ID, myid.ID, myplayernumber);
	}


	return TRUE;			
}

	

// 전투 결과 경험치등을 반영한다. 
void _clBattle::ExpOperate(SI32 killerid, SI32 vitimid)
{
	SI32 victimplayer = CR[vitimid].GetPlayerNumber();
	SI32 victimkind;
	_ParametaBox TempPB;

	if(CR[vitimid].m_siChickPrevKind > 0)
	{
		victimkind = CR[vitimid].m_siChickPrevKind;

		_ImportantParameta ip;
		ZeroMemory(&ip, sizeof(_ImportantParameta));
		UI16 siKind = TransKindImjinToOnline(victimkind);
		ip.SetStr(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetStr());
		ip.SetDex(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetDex());
		ip.SetVit(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetVit());
		ip.SetInt(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetInt());
		ip.SetLife(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetLife());
		ip.SetMana(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetMana());
		ip.SetAC(pGame->pOnlineWorld->pOnlineCharParser->GetCharHeaderID(siKind)->Parameta.GetAC());
		ip.SetLevel(1);

		TempPB.SetImportantParameta(ip);
		MyWeapons mw;
		mw.Init();
		CItem *pItem = NULL;
		TempPB.Action(pItem, mw);
	}
	else
	{
		victimkind = CR[vitimid].m_cltCharStatus.GetKind();
		TempPB = CR[vitimid].m_ParmeterBox;
	}

	// 죽은 놈이 NPC여야만 경험치를 올린다. (사람들끼리 서로 짜고 죽이는 것을 막기 위해)
	if(ISMONSTER(MyNet.PlayersdpId.dpId[victimplayer]) == TRUE)
	{
		SetExp(TempPB.GetMyExp() * KI[victimkind].GetExpVaryPercent() / 100);
		
		// 내 전투력을 구한다. 
//		SI32 mybattlepower = max(1, CR[killerid].m_ParmeterBox.GetMaxBattlePower());
		SI32 mybattlepower = max(1, TempPB.GetMaxBattlePower());
		
		// 적의 전투력을 구한다. 
		SI32 enemybattlepower = max(1, TempPB.GetMaxBattlePower());
		
		// 내전투력보다 적의 전투력이 낮으면 경험치를 그만큼 적게 준다. 
		
		// 적 전력이 내 전력의 얼마의 비율이 되는지 계산한다. (%)
		SI32 rate = (enemybattlepower * 100) / mybattlepower;
		
		// 전력비율이 60%이하면, 경험치를 줄인다. 
		if(rate < 60)
		{
			SetExp((SI16)max(1, GetExp() - ((GetExp() * (60-rate)) / 100)));
		}
		
		// 캐릭터의 경험치를 올리고 필요한 조치(서버에 전송 등..)를 취한다. 
		CR[killerid].IncreaseExp(GetExp(), TRUE, TRUE);
		
		// 사용자면 돈을 얻게 해준다. 
		if(CR[killerid].GetPlayerNumber() == UserPlayerNumber
			&& ISPLAYER	(MyNet.PlayersdpId.dpId[CR[killerid].GetPlayerNumber()]) == TRUE
			&& ISMONSTER(MyNet.PlayersdpId.dpId[CR[vitimid].GetPlayerNumber()]) == TRUE) 							
		{

			// 자원을 획득했다는 사실을 알려준다. 
			SetGetResource(CR[killerid].m_cltCharStatus.GetCenterX(), CR[killerid].m_cltCharStatus.GetCenterY(), 
				GetExp(), 0, 
				0, 0, 0, 0, CR[killerid].GetName());
		}
		
		// 주변에 있는 아군의 경험치도 올려준다. (10%)
		SetExp( (SI32)GetExp()/10);
		
		if(GetExp())
		{
			for(SI32 i = 0;i < (MAX_MULTI_PLAYER-1);i++)
			{
				// 동맹인 아군은 모두 영향을 받는다. 
				if(IsFriendToByPlayerNumber(CR[killerid].GetPlayerNumber(), i) == TRUE)
				{
					if(CR[killerid].m_ParmeterBox.IP.CheckCheckSum() == TRUE)
					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(
							MyNet.PlayersdpId.dpId[i], (SI32)GetExp()) ;
					}
				}
			}
		}
	}
}

// 각 공격 속성에 대한 데미지를 구한다. 
SI32 CalcDamage(SI32 attackatb, SI32 para1, SI32 para2, SI32 para3, SI32 para4)
{
	SI32 mylevel;
	SI32 myapower;
	SI32 kindatb;
	SI32 damage;
	SI32 magicapplyrate;

	myapower		= para1;
	magicapplyrate	= para2;
	mylevel			= para3;

	switch(attackatb)
	{
		case SPECIAL_EARTHQUAKE:			// 지진술
			{
				kindatb			= para3;
				
				// 장수이면, 
				if(kindatb & KINDATB_GENERAL)	damage = (20 + 4 * min(100, myapower/2)) * 15 / 10;
				else							damage = (20 + 2 * min(100, myapower/2)) * 15 / 10;
			}
			break;
		case SPECIAL_LIGHTNING:				// 번개술
			damage = myapower + (4 * mylevel);
			break;
		case SPECIAL_TORNADO:				// 풍백술
		case SPECIAL_YELLOWSAND:
			damage = myapower*3/5;
			break;
		case SPECIAL_MAGIC4T:				// 원심분쇄격(흑룡대차륜)
			damage = myapower*1/2;
			break;
		case SPECIAL_KIMBOMB:				// 폭뢰격
			damage = myapower*2;
			break;
		case SPECIAL_SPECIALATTACK:			// 일격필살
			damage = myapower*4;
			break;
		case SPECIAL_GROUNDEXP:				// 연옥술
			damage = myapower/2;
			break;
		case SPECIAL_MAGIC5J:				// 만월도끼춤
			damage = myapower*2/3;
			break;
		case SPECIAL_FIREARROW:				// 불화살
			damage = myapower;
			break;
		case SPECIAL_FIREARROW2:			// 마법불화살
			damage = myapower*3/2;
			break;
		case SPECIAL_MAGIC8K:				// 쐐기찌르기
			damage = myapower*3/2;
			break;
		case SPECIAL_MAGIC4C:				// 기공신포
			damage = myapower*7/2;
			break;
		case SPECIAL_MAGIC6C:				// 포박술
			damage = 150 + min(150, myapower);
			break;
		case SPECIAL_ABSORBLIFE:			// 흡수술(para1 = enemy life)
			damage = max(1, myapower / 4);
			break;
		case SPECIAL_HIDE:					// 은신술
			damage = myapower;
			break;
		case SPECIAL_GHOST:					// 귀신탄
			damage = myapower;
			break;
		case SPECIAL_DARKFIRE:				// 암흑화염폭풍
			damage = myapower*2;
			break;
		case SPECIAL_FIREATTACK:			// 풍쇄탄
			damage = myapower*3;
			break;
		case SPECIAL_RUNNINGFIRE:			// 연발포탄
			damage = myapower*5/4;
			break;
		case SPECIAL_ENERGYPA:				// 장풍
			damage = myapower*3/2;	
			break;
		case SPECIAL_FIREENERGYPA:			// 열화장풍(비격황룡권)
			damage = myapower*1/12;
			break;
		case SPECIAL_DOUBLE_SWORD:			// 쌍검난무
			damage = myapower*2/5;
			break;
		case SPECIAL_LONG_SPEAR:			// 장창소환
			damage = 1000 + (myapower + mylevel)*3;	
			break;
		case SPECIAL_ICE_BARRIER:			// 빙석술
			damage = myapower;
			break;
		case SPECIAL_FLOWER_NEEDLE_RAIN:	// 만천화우
			damage = myapower*2/3;
			break;
		case SPECIAL_SOUL_CHANGE:			// 빙의술
			damage = 0;
			break;
		case SPECIAL_STONE_MAGIC:			// 석괴술
			damage = myapower*7/2;
			break;
		case SPECIAL_GOD_SPEAR:				// 신창노도(일격필살)
			damage = myapower*4;
			break;
		case SPECIAL_CLOUD_WIND:			// 풍모술
			damage = myapower;
			break;
		case SPECIAL_MANABURN:				// 마나번.
			damage = 0;
			break;

	//----------------------------------------------------------------------------
	// robypark (2004/05/18 13:14) : 추가 기술 - 장수 2차 전직 특수 기술
	//----------------------------------------------------------------------------
		case SPECIAL_FIRE_ROUND_DANCE:			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
			damage = myapower * 0.2;			// 초당 데미지 myapower * 0.2 * 8, 16프레임 중 50%만 데미지 적용
			break;
		case SPECIAL_NOVA:						// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
			damage = myapower * 9.5;
			break;
		case SPECIAL_FURY_OF_VOLCANO:			// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
			damage = myapower * 0.2;			// 초당 데미지  myapower * 0.2 * 8, 16프레임 중 50%만 데미지 적용
			break;
		case SPECIAL_POISONING_HEAVENS:			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
			damage = myapower * 5.25;
			break;
		case SPECIAL_BOMB_OF_HEAVEN:			// 승천포(중국-발석거, 순비연 2차 전직 기술)
			damage = myapower * 12;
			break;
		case SPECIAL_SIEGE_MODE:				// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
			damage = myapower * 9;
			break;
		case SPECIAL_ONSLAUGHT_FIREBALL:		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
			damage = myapower * 8.5;
			break;
		case SPECIAL_FIRERAIN_OF_GOD:			// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
			damage = myapower * 1;
			break;
		default:
			damage = 0;
			clGrp.Error("fd30972", "fd397:%d", attackatb);
			break;
	}
	magicapplyrate = magicapplyrate * 100 / 70;
	return damage * magicapplyrate / 100;
}

// 공격속성과 캐릭터 속성에 따른 공격력 수정. 
// 공격자의 원공격력, 공격자의 공격속성, 방어자의 속성을 근거로 공격자의 원공격력을 수정한다. 
SI32 CalcApowerByAtb(SI32 apower, SI32 attackatb, SI32 atb)
{
	if(attackatb == ATTACKATB_FIRE)
	{
		if(atb & ATB_ICE)
		{
			apower = (apower * 12) / 10;
		}
	}

	return apower;
}