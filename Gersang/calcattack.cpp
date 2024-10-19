// ������ ���� �Ӽ��� ���� �����ϰ� ���ظ� ������ �Լ��� �����Ѵ�. 
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

extern	cltCharInterface		clifChar;							// ĳ���� ���� �������̽� 
extern	cltMapInterface			clifMap;							// �ʰ��� �������̽� 
extern	_MainData				GD;									// ������ �ֵ����� 
extern	_Char					CR[];								// ĳ������ ������ 
extern	_KindInfo				KI[];								// ĳ���� ������ ������ 
extern	SI32					UserPlayerNumber;					// ����� ���� 
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

// ������ �Ӽ��� ��� �Ӽ��� ���� 
static SI32 RelationDamageAndDefence[]=
{
	// ������ �Ӽ�.   DEFENCEATB_HUMAN	_ARMOUR _HORSE _AIRPLANE _OILARMOUR _FLOAT _ANIMAL _GHOST _SOFT 
	HITATB_SWORD,		100,			80,		80,		0,			80,		50,		150,	80,		50,			
	HITATB_RANGE,		100,			70,		40,		100,		100,	120,	80,		70,		70,
	HITATB_FIREARROW,	100,			70,		40,		200,		100,	120,	80,		70,		70,
	HITATB_MAGIC,		150,			100,	120,	0,			80,		100,	90,		150,	100,	
	HITATB_FIRE,		150,			50,		150,	0,			200,	50,		130,	30,		100,	
	HITATB_SKILL,		110,			80,		90,		50,			100,	100,	150,	100,	110,		
	-1
};

// ���� �Ӽ��� ���� ������ �Ӽ�
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

	// actdoll (2004/03/06 4:54) : 3�� ��ġ �߰��ڷ�
	ATTACKATB_PYTHON,				HITATB_MAGIC,	// �̹��� 
	ATTACKATB_JINEMPEROR,			HITATB_MAGIC,	// ����Ȳ��ȥ

	// robypark (2004/5/19 17:29) : ��� 2�� ���� ���� �߰�
	// 110	����-�ź���, �̼��� 2�� ����
	ATTACKATB_TURTLE_VEHICLE,					HITATB_FIRE,
	// 111	����-������, ���� 2�� ����
	ATTACKATB_THUNDER_VEHICLE,					HITATB_MAGIC,
	// 112	�Ϻ�-������, ��Ű��ī 2�� ����
	ATTACKATB_QUAKE_VEHICLE,					HITATB_MAGIC,
	// 113	�Ϻ�-�����, ���̸��� 2�� ����
	ATTACKATB_BLACK_DRAGON_VEHICLE,				HITATB_RANGE,
	// 114	�߱�-�߼���, �̷� 2�� ����
	ATTACKATB_CATAPULT,							HITATB_RANGE,
	// 115	�߱�-�Ҷ�������, ���� 2�� ����
	ATTACKATB_FARANGI_VEHICLE,					HITATB_RANGE,
	// 116	�븸-ȭ����, ����â 2�� ����
	ATTACKATB_FIRE_DRAGON_VEHICLE,				HITATB_RANGE,
	// 117	�븸-��Ȳ����, �弱ȭ 2�� ����
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
	// 126	�Ҷ������� ��õ��� ����
	ATTACKATB_ROAR_OF_LAUGHTER,					HITATB_RANGE,

	// robypark 2004/6/10 19:4
	// ����� õ������, �ñ����
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
// actdoll (2004/03/06 4:56) : �߰� ĳ���� - �����/����Ȳ�� ���� START
//----------------------------------------------------------------------------

	AttackAtbInfo[ATTACKATB_PYTHON				]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;
	AttackAtbInfo[ATTACKATB_JINEMPEROR			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER;

//----------------------------------------------------------------------------
// actdoll (2004/03/06 4:56) : �߰� ĳ���� - �����/����Ȳ�� ���� END
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// damduk : �߰� ĳ���� - ��ȣ�� ����
//----------------------------------------------------------------------------
	AttackAtbInfo[ATTACKATB_GUARDIAN_PIG		]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER;

//----------------------------------------------------------------------------
// robypark (2004/05/18 11:05) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	// 110	����-�ź���, �̼��� 2�� ����
	AttackAtbInfo[ATTACKATB_TURTLE_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// 111	����-������, ���� 2�� ����
	AttackAtbInfo[ATTACKATB_THUNDER_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2;

	// 112	�Ϻ�-������, ��Ű��ī 2�� ����
	AttackAtbInfo[ATTACKATB_QUAKE_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// 113	�Ϻ�-�����, ���̸��� 2�� ����
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_VEHICLE]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_PALSY | ATTACKATB_ATB_POISON;

	// 114	�߱�-�߼���, �̷� 2�� ����
	AttackAtbInfo[ATTACKATB_CATAPULT			]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// 115	�߱�-�Ҷ�������, ���� 2�� ����
	AttackAtbInfo[ATTACKATB_FARANGI_VEHICLE		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_MINRANGE;

	// 116	�븸-ȭ����, ����â 2�� ����
	AttackAtbInfo[ATTACKATB_FIRE_DRAGON_VEHICLE	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// 117	�븸-��Ȳ����, �弱ȭ 2�� ����
	AttackAtbInfo[ATTACKATB_CHINESEPHOENIX		]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3;

	// robypark (2004/6/4 13:6)
	// ��õ��� ����, �Ҷ�������
	AttackAtbInfo[ATTACKATB_ROAR_OF_LAUGHTER	]	= ATTACKATB_ATB_PHYSICS | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE3 | ATTACKATB_ATB_MINRANGE;

	// robypark 2004/6/10 19:4
	// ����� õ������, �ñ����
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_ICE	]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_PALSY;

	// robypark 2004/6/11 10:49
	// ����� õ������, ������
	AttackAtbInfo[ATTACKATB_BLACK_DRAGON_POISON	]	= ATTACKATB_ATB_MAGIC | ATTACKATB_ATB_LOWASSISTDPOWER | ATTACKATB_ATB_TYPE2 | ATTACKATB_ATB_POISON;
}


SHORT GetRealApower(SHORT apower, SHORT attackatb, DWORD defenceatb)
{
	SI32 pos	=	0;	

	// ���� �Ӽ��� ���� ������ �Ӽ��� ã�´�.
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

	// ������ �Ӽ��� ��� �Ӽ��� ����Ͽ� ���� ���ݷ��� ���Ѵ�.
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

	// ���� ���ݷ��� ����Ѵ�. 
	return (apower + (apower * data) / 100);	// -1: atb ���� 1���� �����ϹǷ�
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
 

// ���ݼӼ�, ���ݷ�, ���ݴ���� ����Ͽ� ���� ���ظ� ����Ѵ�. 
SHORT CalculateHitPoint(SHORT attackatb, SHORT power, _NewID attackid)
{

	SHORT validpower=0;
	SHORT assistdpower=0;
	SI32 damageresist = 0;
	SI32 magicresist = 0;

	// ���� ��ǥ�� �̹� ��ȿ���� ������ ��� �� �� ����.
	if(IsSee(attackid.ID)==FALSE || attackid.IsSame(clifChar.GetID(attackid.ID))==FALSE)return 0;

	// ��밡 �������̰� 
	// �����߿� ��������� �ִٸ�, 
	if(clifChar.GetLowMoveSwitch(attackid.ID) == TRUE
		&& (AttackAtbInfo[attackatb] &  ATTACKATB_ATB_LOWASSISTDPOWER))
	{
		// 30%�� ���� ��� ȿ��. 
		power = power * 70 / 100;
	}
	

	// ���� �Ӽ��� �������϶�. 
	if(AttackAtbInfo[attackatb] & ATTACKATB_ATB_PHYSICS)
	{
		
		damageresist = CR[attackid.ID].m_ParmeterBox.GetTotalDamageResist();
		
		return max(1, power * (100 - min(110, damageresist)* 8 / 10) / 100);
	}
	// ���� �Ӽ��� �������϶�. 
	else if(AttackAtbInfo[attackatb] & ATTACKATB_ATB_MAGIC)
	{
		SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[attackid.ID].m_cltCharStatus.GetKind()));
		//2�� ���� ������.
		//if(siCharType == 3)
		//	return 0;
		//else
		{
			//2�� ���� ����� �ƴϸ�.
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

// ������ ���� �Ӽ��� ����Ͽ� ���� ü���� ���Ͻ�Ų��. 
SHORT CalculateHit(_NewID myid, SHORT myplayernumber, SHORT attackatb, SHORT range, SHORT area, SHORT power, _NewID attackid, SHORT dx, SHORT dy, SI32 hitrate)
{

	SHORT i, j;
	SHORT length=0;
	SHORT damage=0;
	SHORT realpower;
	SHORT id, temp_id;

	// �������� �����ϰ� �ִٴ� ����� �˸���.
	if(IsAlive(myid) && IsAlive(attackid))
		clifChar.SetBeAttackedSwitch(attackid.ID, myid.ID, myplayernumber);

	// ���߷��� �ٰŷ� Ÿ�� ���θ� �����Ѵ�.
	if(GD.Random() % 100 > (UI32)hitrate)
		return 0;

	// ȭ���� ��쿡�� �¾��� �� ȿ���� ����. 
	if(attackatb == ATTACKATB_ARROW)
	{
		if(IsAlive(attackid))
		{
			PushEffect(EFFECT_ARROWHIT, CR[attackid.ID].GetX(), CR[attackid.ID].GetY());
		}
	}
	// ��ħ�� ��� �¾��� �� �ߵ���Ų��. 
//	if(attackatb == ATTACKATB_POISONARROW)
	if (AttackAtbInfo[attackatb] & ATTACKATB_ATB_POISON)
	{
		if(IsAlive(attackid))
		{
			CR[attackid.ID].m_cltPoisonEffect.Set(TRUE, 200, power/3);

			// robypark 2004/8/9 13:37
			// �� ������ �� ĳ���� ID ����
			CR[attackid.ID].m_cltPoisonEffect.SetPoisonAttacker(myid);
		}
	}

	// ��ȣ��(����)�� ������ �޾��� ��� ����.(�ӽ��ڵ�)
	if(CR[attackid.ID].m_bPigDefence)	return 0;

	if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE1 )
	{
		// ��ǥ�� ��ȿ�ϸ� ����
        if(IsSee(attackid.ID)==FALSE)return 0;

		// ��ǥ������ ������ ���Ѵ�. 
		id=0;
		for(i=-1;i<=1;i++)
			for(j=-1;j<=1;j++)
			{
				// �������� ������ ã�´�.
				temp_id = clifMap.GetIDMap(dx+j, dy+i);

				// ������ ��ȿ�ϰ� ���ݴ��� ��ġ�ϸ�, 
				if(IsSee(temp_id) && attackid.IsSame(clifChar.GetID(temp_id)) == TRUE)
				{
					id=temp_id;
				}
			}

			// ���� ��ȿ�ϸ�, 
			if(id)
			{
				HitNOperateAfterHit(myid, clifChar.GetID(id), myplayernumber, attackatb, power);
				return 0;
			}
	}

	else if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE2 )
	{
		// ��ǥ�� ��ȿ�ϸ� ����, 
        if(IsSee(attackid.ID)==FALSE)return 0;

		// ������ �ƴϾ�� �������� �� �� �ִ�.(���� ���濡�� ��ȣ���� ���°� �ƴϾ�� ..)
		else if(IsFriendToByPlayerNumber(myplayernumber, CR[attackid.ID].GetPlayerNumber()) == FALSE)
		{
			HitNOperateAfterHit(myid, attackid, myplayernumber, attackatb, power);
		}
		
		return 0;
	}

	else if( AttackAtbInfo[attackatb] & ATTACKATB_ATB_TYPE3 )
	{
		if(range<=0)range=1;

		// ������ ��ǥ�� ����� �ʱ�ȭ�Ѵ�. 
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
					
					
					// �̹� ó���� ĳ�������� Ȯ���Ѵ�. 
					if(IsSee(id) && IsInHitList(id) == FALSE)
					{
						
						length= max(abs(i), abs(j));
						realpower=power-power*(length*100/(range+1))/100;
						
						// ���� ���� ��ǥ�� �´ٸ�, 
						// �������� ����ϰ� �ش�. 
						if(id == attackid.ID)
						{
							realpower=power;
						}
						if(attackatb == ATTACKATB_LONG_SPEAR)
						{
							realpower = power;
						}
						/////////////////////////////���⿩�⿩�⿩�⿩��.......
						HitNOperateAfterHit(myid, clifChar.GetID(id), myplayernumber, attackatb, realpower);
						
						// ó���� ĳ������ ��Ͽ� �߰��Ѵ�. 
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

// ���� ���� ���ݰ� ���ݿ� ���� ��ġ�� ���Ѵ�. 
BOOL HitNOperateAfterHit(_NewID myid, _NewID enemyid, SHORT myplayernumber, SHORT attackatb, SHORT power)
{
	// �����ڰ� �ǰ����ڿ� ��ȣ���̸� �������� ������ �ʴ´�. 
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

	// 2�� ���� �ϰ�� ����.
	// ���ݿ� ���� Ÿ��ġ�� ���Ѵ�. 
	SHORT damage = CalculateHitPoint(attackatb, power, enemyid);

	// ���� �ͽ�ź�� �ɷ��ִ� ���
	if(CR[enemyid.ID].GhostTime)
		damage = damage*2;

	// myid�� �н��� ��� �н��� ������ �����ڰ� �ȴ�. 
	// �н��� ��������� ������ ��������� �ݿ��� �ȴ�. 
	if(IsAlive(myid) && CR[myid.ID].HostID.IsEmpty()==FALSE)
	{
		myid = CR[myid.ID].HostID;
	}

	// �¾��� �� �׷������� ȿ��
	if(IsAlive(enemyid) == TRUE)
	{
		CR[enemyid.ID].SetNewSelectedSwitch(TRUE);
		CR[enemyid.ID].SetBeAttackedDrawSwitch(TRUE);
		CR[enemyid.ID].SetBeAttackedDrawStep(0);
		
		// �ڷ� �и��� �Ӽ��̸� �ڷ� �δ�. 
		if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_PULL)
		{
			// �ǰݵ� ĳ������ ������ ���´�. 2�� ������� �˻��ϱ����Ͽ� ���´�.
			SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));

			// robypark 2004/6/11 13:43
			// 2�� ����� �ڷ� �и��� �ʴ´�.
			if (siCharType != 3)
			{
				// �����ڰ� ��ȿ�ϸ�, 
				// robypark 2004/6/10 10:39
				// �Ҷ��������� ������ ��õ����� ��쿡�� �ڷ� �и��� �ʴ´�.
				if ((CR[enemyid.ID].m_cltCharStatus.GetKind() != KIND_ON_ROAR_OF_LAUGHTER)
					&& (IsAlive(myid) == TRUE))
				{
					// �����ڿ� �������� ������ ���� �з��� �ϴ� ���� ���Ѵ�. 
					SI32 dir = DecideDirection8(CR[myid.ID].GetX(), CR[myid.ID].GetY(), CR[enemyid.ID].GetX(), CR[enemyid.ID].GetY());
					CR[enemyid.ID].DeleteID();
					CR[enemyid.ID].Pull(dir);
					CR[enemyid.ID].SetID();
				}
			}
		}
		
		// ���� �Ǵ� �Ӽ��̸� ���� ��Ų��. 
		if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_PALSY)
		{
			// �����ڰ� ��ȿ�ϸ�, 
			if(IsAlive(myid) == TRUE)
			{
				// �����ϰ�  ���� ��Ų��. 
				if(IsAlive(enemyid.ID) &&
						IsFriendToByPlayerNumber(myplayernumber, CR[enemyid.ID].GetPlayerNumber() ) == FALSE
						&& CR[enemyid.ID].IsAtb(ATB_PATIENT)	== TRUE)
				{
						// 20% �� Ȯ����...
						if(GD.Random() % 100 < 20)
						{
							// robypark 2004-6-3 16:15
							// õ������ �ñ⿡ ���� ����, ����� �⺻ ���ݿ� ���� �����
							// �ٸ� ����ȿ�� �̹����� ����Ѵ�.
							if (attackatb == ATTACKATB_BLACK_DRAGON_VEHICLE)
								CR[enemyid.ID].SetHalt(HALT_REASON_FOG_OF_ICE, 150);
							// robypark 2004/6/10 19:4
							// ����� õ������, �ñ����
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


	// ü�� ��� �Ӽ��� ������, 
	if(AttackAtbInfo[attackatb] &  ATTACKATB_ATB_VAMPIRE)
	{

		if(IsAlive(myid)==TRUE)
		{
			CR[myid.ID].SetHealDraw(TRUE);
			
			// ���� �������� ����Ͽ� �ڽ��� �������� �����. 
			CR[myid.ID].IncreaseLife(damage/3); 
		}
	}


	// ����� ĳ���Ͱ� ���� ���̰ų� 
	// ���Ͱ� ���� ����̰� �ӽ� ȣ��Ʈ�� ���, 
	SI32 victimplayernumber = CR[enemyid.ID].GetPlayerNumber();

	if( victimplayernumber== UserPlayerNumber
	|| (ISMONSTER(MyNet.PlayersdpId.dpId[victimplayernumber]) && MyNet.IsHost()) )
	{}
	else return TRUE;

	
	// ������ ü���� �ٿ��ش�.
	// enemyid.IDĳ���Ͱ� �׾��ٸ�.
	if(clifChar.ReduceLife(enemyid.ID, damage, 10) == FALSE)
	{
		SI16 siCharType = pGame->pOnlineWorld->pIOnlineCharKI->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));
		if(siCharType != 3)
		{
			if(CR[enemyid.ID].m_bUseGuardian == TRUE)
				CR[enemyid.ID].GuardianBreakless(CR[enemyid.ID].m_siGuardianKind);
			// ĳ���� ������ ������ �뺸�Ѵ�. 
			if(OBInfo.IsSingleMode == TRUE)
			{
				// ������ ���� �������� �׿����� �뺸�Ѵ�. 
				if(IsAlive(myid))
				{
					// �������� �ƴϾ�� �Ѵ�. 
					if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
							MyNet.PlayersdpId.dpId[ CR[myid.ID].GetPlayerNumber()],		CR[myid.ID].m_siOnlineID, 
							MyNet.PlayersdpId.dpId[ CR[enemyid.ID].GetPlayerNumber()],	CR[enemyid.ID].m_siOnlineID);
						
						// ų������ �մ��� ����ġ���� �����Ѵ�. 
						pGame->pBattle->ExpOperate(myid.ID, enemyid.ID);

					
					}
				}
#ifdef _SET_SERVER_MESSAGE
				// robypark 2004/8/9 14:46
				// ������ ĳ���Ͱ� �׾��� ��� ���Ͱ� ���� ���� �ʰ� �����ϱ�
				else
				{
					// �ߵ��Ǿ� �״� ĳ������ ���, ������ ĳ���Ͱ� ����� ��� ���Ͱ� ���� ���� �ʵ��� �����ϱ�
					if (CR[enemyid.ID].m_cltPoisonEffect.GetPoisonSwitch())
					{
						// �������� �ƴϾ�� �Ѵ�. 
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
		//2�� ���� ����� �׾��� �� �Ϲ� ����� �θ���.
		//GeneralHeader
		/*
		GeneralHeader* pGeneralHeader = pGame->pOnlineWorld->pGeneralParserEx->GetGeneral(TransKindImjinToOnline(CR[enemyid.ID].m_cltCharStatus.GetKind()));
		if(pGeneralHeader != NULL)
		{	
			//kind�� ����Ѵ�.
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

		// ������ ����ϸ� ����� ���� ��ġ�� ���Ѵ�. 
		KillOperation(myplayernumber, enemyid);
		

		// ȭ�� �������� ����ϸ� ���� �ٿ��ش�.
		// ġ�ᰡ���� ���ָ��̰�, 
		// ���� ����Ʈ�� �ȵȴ�.
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
	// ���� �ʾҴٸ� ���� �޴� ���̶�� ���� �˸���. 
	else
	{
		clifChar.SetBeAttackedSwitch(enemyid.ID, myid.ID, myplayernumber);
	}


	return TRUE;			
}

	

// ���� ��� ����ġ���� �ݿ��Ѵ�. 
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

	// ���� ���� NPC���߸� ����ġ�� �ø���. (����鳢�� ���� ¥�� ���̴� ���� ���� ����)
	if(ISMONSTER(MyNet.PlayersdpId.dpId[victimplayer]) == TRUE)
	{
		SetExp(TempPB.GetMyExp() * KI[victimkind].GetExpVaryPercent() / 100);
		
		// �� �������� ���Ѵ�. 
//		SI32 mybattlepower = max(1, CR[killerid].m_ParmeterBox.GetMaxBattlePower());
		SI32 mybattlepower = max(1, TempPB.GetMaxBattlePower());
		
		// ���� �������� ���Ѵ�. 
		SI32 enemybattlepower = max(1, TempPB.GetMaxBattlePower());
		
		// �������º��� ���� �������� ������ ����ġ�� �׸�ŭ ���� �ش�. 
		
		// �� ������ �� ������ ���� ������ �Ǵ��� ����Ѵ�. (%)
		SI32 rate = (enemybattlepower * 100) / mybattlepower;
		
		// ���º����� 60%���ϸ�, ����ġ�� ���δ�. 
		if(rate < 60)
		{
			SetExp((SI16)max(1, GetExp() - ((GetExp() * (60-rate)) / 100)));
		}
		
		// ĳ������ ����ġ�� �ø��� �ʿ��� ��ġ(������ ���� ��..)�� ���Ѵ�. 
		CR[killerid].IncreaseExp(GetExp(), TRUE, TRUE);
		
		// ����ڸ� ���� ��� ���ش�. 
		if(CR[killerid].GetPlayerNumber() == UserPlayerNumber
			&& ISPLAYER	(MyNet.PlayersdpId.dpId[CR[killerid].GetPlayerNumber()]) == TRUE
			&& ISMONSTER(MyNet.PlayersdpId.dpId[CR[vitimid].GetPlayerNumber()]) == TRUE) 							
		{

			// �ڿ��� ȹ���ߴٴ� ����� �˷��ش�. 
			SetGetResource(CR[killerid].m_cltCharStatus.GetCenterX(), CR[killerid].m_cltCharStatus.GetCenterY(), 
				GetExp(), 0, 
				0, 0, 0, 0, CR[killerid].GetName());
		}
		
		// �ֺ��� �ִ� �Ʊ��� ����ġ�� �÷��ش�. (10%)
		SetExp( (SI32)GetExp()/10);
		
		if(GetExp())
		{
			for(SI32 i = 0;i < (MAX_MULTI_PLAYER-1);i++)
			{
				// ������ �Ʊ��� ��� ������ �޴´�. 
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

// �� ���� �Ӽ��� ���� �������� ���Ѵ�. 
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
		case SPECIAL_EARTHQUAKE:			// ������
			{
				kindatb			= para3;
				
				// ����̸�, 
				if(kindatb & KINDATB_GENERAL)	damage = (20 + 4 * min(100, myapower/2)) * 15 / 10;
				else							damage = (20 + 2 * min(100, myapower/2)) * 15 / 10;
			}
			break;
		case SPECIAL_LIGHTNING:				// ������
			damage = myapower + (4 * mylevel);
			break;
		case SPECIAL_TORNADO:				// ǳ���
		case SPECIAL_YELLOWSAND:
			damage = myapower*3/5;
			break;
		case SPECIAL_MAGIC4T:				// ���ɺм��(��������)
			damage = myapower*1/2;
			break;
		case SPECIAL_KIMBOMB:				// ���ڰ�
			damage = myapower*2;
			break;
		case SPECIAL_SPECIALATTACK:			// �ϰ��ʻ�
			damage = myapower*4;
			break;
		case SPECIAL_GROUNDEXP:				// ������
			damage = myapower/2;
			break;
		case SPECIAL_MAGIC5J:				// ����������
			damage = myapower*2/3;
			break;
		case SPECIAL_FIREARROW:				// ��ȭ��
			damage = myapower;
			break;
		case SPECIAL_FIREARROW2:			// ������ȭ��
			damage = myapower*3/2;
			break;
		case SPECIAL_MAGIC8K:				// �������
			damage = myapower*3/2;
			break;
		case SPECIAL_MAGIC4C:				// �������
			damage = myapower*7/2;
			break;
		case SPECIAL_MAGIC6C:				// ���ڼ�
			damage = 150 + min(150, myapower);
			break;
		case SPECIAL_ABSORBLIFE:			// �����(para1 = enemy life)
			damage = max(1, myapower / 4);
			break;
		case SPECIAL_HIDE:					// ���ż�
			damage = myapower;
			break;
		case SPECIAL_GHOST:					// �ͽ�ź
			damage = myapower;
			break;
		case SPECIAL_DARKFIRE:				// ����ȭ����ǳ
			damage = myapower*2;
			break;
		case SPECIAL_FIREATTACK:			// ǳ��ź
			damage = myapower*3;
			break;
		case SPECIAL_RUNNINGFIRE:			// ������ź
			damage = myapower*5/4;
			break;
		case SPECIAL_ENERGYPA:				// ��ǳ
			damage = myapower*3/2;	
			break;
		case SPECIAL_FIREENERGYPA:			// ��ȭ��ǳ(���Ȳ���)
			damage = myapower*1/12;
			break;
		case SPECIAL_DOUBLE_SWORD:			// �ְ˳���
			damage = myapower*2/5;
			break;
		case SPECIAL_LONG_SPEAR:			// ��â��ȯ
			damage = 1000 + (myapower + mylevel)*3;	
			break;
		case SPECIAL_ICE_BARRIER:			// ������
			damage = myapower;
			break;
		case SPECIAL_FLOWER_NEEDLE_RAIN:	// ��õȭ��
			damage = myapower*2/3;
			break;
		case SPECIAL_SOUL_CHANGE:			// ���Ǽ�
			damage = 0;
			break;
		case SPECIAL_STONE_MAGIC:			// ������
			damage = myapower*7/2;
			break;
		case SPECIAL_GOD_SPEAR:				// ��â�뵵(�ϰ��ʻ�)
			damage = myapower*4;
			break;
		case SPECIAL_CLOUD_WIND:			// ǳ���
			damage = myapower;
			break;
		case SPECIAL_MANABURN:				// ������.
			damage = 0;
			break;

	//----------------------------------------------------------------------------
	// robypark (2004/05/18 13:14) : �߰� ��� - ��� 2�� ���� Ư�� ���
	//----------------------------------------------------------------------------
		case SPECIAL_FIRE_ROUND_DANCE:			// ����������(����-�ź���, �̼��� 2�� ���� ���)
			damage = myapower * 0.2;			// �ʴ� ������ myapower * 0.2 * 8, 16������ �� 50%�� ������ ����
			break;
		case SPECIAL_NOVA:						// ����(����-������, ���� 2�� ���� ���)
			damage = myapower * 9.5;
			break;
		case SPECIAL_FURY_OF_VOLCANO:			// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
			damage = myapower * 0.2;			// �ʴ� ������  myapower * 0.2 * 8, 16������ �� 50%�� ������ ����
			break;
		case SPECIAL_POISONING_HEAVENS:			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
			damage = myapower * 5.25;
			break;
		case SPECIAL_BOMB_OF_HEAVEN:			// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
			damage = myapower * 12;
			break;
		case SPECIAL_SIEGE_MODE:				// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
			damage = myapower * 9;
			break;
		case SPECIAL_ONSLAUGHT_FIREBALL:		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
			damage = myapower * 8.5;
			break;
		case SPECIAL_FIRERAIN_OF_GOD:			// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
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

// ���ݼӼ��� ĳ���� �Ӽ��� ���� ���ݷ� ����. 
// �������� �����ݷ�, �������� ���ݼӼ�, ������� �Ӽ��� �ٰŷ� �������� �����ݷ��� �����Ѵ�. 
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