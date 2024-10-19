#include <GSL.h>

#include <main.h>
#include <Music.h>
#include <clGame.h>
#include "clItemInfoWindow.h"

#include <OnlineBattleMsg.h>
#include "maindata.h"
#include <etc.h>
#include "..\Gersang\cheat.h"
#include "..\Gersang\text.h"

#include <charinterface.h>
#include <CharOrg.h>
#include <MapInterface.h>
#include <mynet-game.h>
#include <SetChar.h>
#include <OnlineImjin2i.h>

#include <Magic.h>

#include <DebugTool.h>

extern  _clGame* pGame;

extern _OnlineBattleMsgQue OBMQue;
extern OnlineBattleInfo OBInfo;


extern	_Order 				Order[];
extern	_MainData			GD;									// 게임의 주데이터 
extern	_MyNetGame			MyNet;
extern	_Cheat 				Cheat;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	cltMapInterface		clifMap;							// 맵관련 인터페이스 
extern	_NewID 				ZeroID;
extern _Char   CR[];       // 캐릭터의 포인터 
extern SI32	UserPlayerNumber;
extern	_Text 				Text;								// 텍스트 

void OrderOperateGetItem(_OrderData order);
void OrderOperateUpdateChar(_OrderData order);
void OrderOperateAlly(_OrderData order);
void OrderOperateQuit(_OrderData order);

// 명령의 여러 성질을 설정한다. 
void _Order::SetOrder(SHORT atb, SHORT object, int info, SHORT priority, int para1, int para2)
{
	Atb=atb;
	Object=object;
	Info=info;
	Priority=priority;
	Para1=para1;
	Para2=para2;
}

// 명령의 속성을 구한다. 
BOOL _Order::IsAtb(SHORT atb)
{
	if(Atb&atb)return TRUE;
	return FALSE;
}

int _Order::GetPara1()
{
	return Para1;
}

int _Order::GetPara2()
{
	return Para2;
}

void OrderInit()
{
	Order[ORDER_WAIT]				.SetOrder(ORDERATB_NORMAL, NOOBJECT,  0, ORDERPRIORITY_LAST, 0, 0);
	Order[ORDER_STOP]				.SetOrder(ORDERATB_NORMAL, NOOBJECT,  0, ORDERPRIORITY_0, 0, 0);
	Order[ORDER_MOVE]				.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_ATTACK]				.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_ATTACKGROUND]		.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_DIE]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_DIE_GENERALEX]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_DEAD]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_HARVEST]			.SetOrder(ORDERATB_NORMAL, OBJECT1  , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_BUILD]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_MAKE]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_SELL]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_TRAIN]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_2, 0, 0);
	Order[ORDER_REPAIR]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_BUILD1]				.SetOrder(ORDERATB_UPDATEMENU, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_BUILD2]				.SetOrder(ORDERATB_UPDATEMENU, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_INITMENU]			.SetOrder(ORDERATB_UPDATEMENU, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_SELLMENU]			.SetOrder(ORDERATB_UPDATEMENU, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_TRAINSPOT]			.SetOrder(ORDERATB_NORMAL, OBJECT1  , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_2,0, 0);
	Order[ORDER_CARCASS]			.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUT]			.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY0]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY1]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY2]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY3]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY4]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY5]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY6]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDEOUTONLY7]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_RIDE]				.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_TAKEIN]				.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_HELPBUILD]			.SetOrder(ORDERATB_NORMAL, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_CAPTUREBUILD]		.SetOrder(ORDERATB_NORMAL, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_PATROL]				.SetOrder(ORDERATB_NORMAL, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_CONVOY]				.SetOrder(ORDERATB_NORMAL, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_HOLD]				.SetOrder(ORDERATB_NORMAL, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_CHINA_BARRACK]		.SetOrder(ORDERATB_UPDATEMENU, NOOBJECT,    ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_CHINA_HEAVYMAKER]	.SetOrder(ORDERATB_UPDATEMENU,NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_NEWMENU_MARKET]		.SetOrder(ORDERATB_UPDATEMENU,NOOBJECT,ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_UPGRADE]			.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_2, 0, 0);
	Order[ORDER_GOODS]				.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_2, 0, 0);
	Order[ORDER_SELLCROP]			.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_SELLTREE]			.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_USEBUDA]			.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_MINE]				.SetOrder(ORDERATB_NORMAL, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_AIRMAGIC]			.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_FARMERTOBULL]		.SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_MOVENRIDEOUTNATTACK].SetOrder(ORDERATB_NORMAL, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_GOINTOHQ]			.SetOrder(ORDERATB_NORMAL, OBJECT1 ,  ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_BUILDTOCAR]			.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_SHOWSTORAGE]		.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_USEPOTION]			.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_AMBUSH]				.SetOrder(ORDERATB_NORMAL, NOOBJECT , ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);
	Order[ORDER_SELLITEM]			.SetOrder(ORDERATB_NORMAL, OBJECT1  , ORDERINFO_SAMEPRIORITY, ORDERPRIORITY_4, 0, 0);


    Order[ORDER_MOVERIDEOUT]		.SetOrder(ORDERATB_NORMAL, OBJECT1,  ORDERINFO_NORETREAT|ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_SELFEXP]			.SetOrder(ORDERATB_NORMAL, OBJECT1,  ORDERINFO_NORETREAT|ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);

	// 전역 명령 
	Order[ORDER_AUTOMAGIC]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_NOAUTOMAGIC]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_GENERALFIRST]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_NOGENERALFIRST]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ALLY]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_QUIT]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);

	// 마법 
	Order[ORDER_BETRAY]				.SetOrder(ORDERATB_MAGIC, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_BETRAY, 0);
	Order[ORDER_HEAL]				.SetOrder(ORDERATB_MAGIC, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_HEAL, 0);
	Order[ORDER_MANA]				.SetOrder(ORDERATB_MAGIC, OBJECT1,   ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_MANA, 0);
	Order[ORDER_MASSHEAL]			.SetOrder(ORDERATB_MAGIC, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_HEAL, 0);
	Order[ORDER_EARTHQUAKE]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_EARTHQUAKE, 0);
	Order[ORDER_TORNADO]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_TORNADO, 0);
	Order[ORDER_YELLOWSAND]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_YELLOWSAND, 0);
	Order[ORDER_SOUL]				.SetOrder(ORDERATB_MAGIC, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SOUL, 0);
	Order[ORDER_GHOST]				.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_GHOST, 0);
	Order[ORDER_DARKFIRE]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_DARKFIRE, 0);
	Order[ORDER_IVORY]				.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_IVORY, 0);
	Order[ORDER_BLUEDRAGON]			.SetOrder(ORDERATB_MAGIC, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_BLUEDRAGON, 0);
	Order[ORDER_RUNNINGFIRE]		.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_RUNNINGFIRE, 0);
	Order[ORDER_ENERGYPA]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ENERGYPA, 0);
	Order[ORDER_SNOWATTACK]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SNOWATTACK, 0);
	Order[ORDER_FIREATTACK]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIREATTACK, 0);
	Order[ORDER_FIREENERGYPA]		.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIREENERGYPA, 0);
	Order[ORDER_ABSORB]				.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ABSORB, 0);
	Order[ORDER_MAGIC8J]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC8J, 0);
	Order[ORDER_LIGHTNING]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_LIGHTNING, 0);
	Order[ORDER_SHIELD]				.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SHIELD, 0);
	Order[ORDER_DETECTMINE]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_DETECTMINE, 0);
	Order[ORDER_WARP]				.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_WARP]				.siNeedKind = KIND_GENERAL8J;
	Order[ORDER_EXTINGUISH]			.SetOrder(ORDERATB_MAGIC, NOOBJECT ,ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_EXTINGUISH, 0);
	Order[ORDER_MAGIC4T]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC4T, 0);
	Order[ORDER_MAGIC2T]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC2T, 0);
	Order[ORDER_MAGICTEST]			.SetOrder(ORDERATB_MAGIC, NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_MAGICTEST,0);
	Order[ORDER_RECALL_PANDA]		.SetOrder(ORDERATB_MAGIC, OBJECT1, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_RECALL_PANDA,0);
	Order[ORDER_RECALL_MONKEY]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_RECALL_MONKEY,0);
	Order[ORDER_RECALL_AMAKUSA]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_RECALL_AMAKUSA,0);
	Order[ORDER_RECALL_GUARDIAN_MOUSE].SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0); 
	Order[ORDER_GUARDIAN_RABBIT]	.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0);
	Order[ORDER_GUARDIAN_TIGER]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0);
	Order[ORDER_RECALL_GUARDIAN_CAW].SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0); 
	Order[ORDER_GUARDIAN_DRAGON]	.SetOrder(ORDERATB_MAGIC, OBJECT1, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0);
	Order[ORDER_GUARDIAN_HORSE]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0);
	Order[ORDER_GUARDIAN_SHEEP]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_0,0,0);
	Order[ORDER_RECALL_TIGER]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,MAGICPOWER_RECALL_TIGER,0);
	Order[ORDER_STEALTECH]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_STEALTECH, 0);
	Order[ORDER_RAINMAKE]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_RAINMAKE, 0);
	Order[ORDER_STEALEXP]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_STEALEXP, 0);
	Order[ORDER_CLON]				.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_CLON, 0);
	Order[ORDER_ENEMYCLON]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, 0, 0);
	Order[ORDER_GROUNDEXP]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_GROUNDEXP, 0);
	Order[ORDER_MAGIC4C]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC4C, 0);
	Order[ORDER_MAGIC6C]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC6C, 0);
	Order[ORDER_ICE]				.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ICE, 0);
	Order[ORDER_KIMBOMB]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_KIMBOMB, 0);
	Order[ORDER_MAGIC5J]			.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MAGIC5J, 0);
	Order[ORDER_MANABURN]			.SetOrder(ORDERATB_MAGIC, OBJECT1, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_MANABURN, 0);
	Order[ORDER_DOUBLE_SWORD]		.SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_DOUBLE_SWORD, 0);
	Order[ORDER_LONG_SPEAR]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_LONG_SPEAR, 0);
	Order[ORDER_ICE_BARRIER]		.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ICE_BARRIER, 0);

	Order[ORDER_FLOWER_NEEDLE_RAIN]	.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FLOWER_NEEDLE_RAIN, 0);
	Order[ORDER_SOUL_CHANGE]		.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SOUL_CHANGE, 0);
	Order[ORDER_STONE_MAGIC]		.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_STONE_MAGIC, 0);
	Order[ORDER_GOD_SPEAR]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_GOD_SPEAR, 0);
	Order[ORDER_CLOUD_WIND]			.SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_CLOUD_WIND, 0);

	// 장비 장탈착 명령. 
	Order[ORDER_WEAPONOFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_WEAPONON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ARMOUROFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ARMOURON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ETCOFF]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ETCON]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_POTIONON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_POTIONOFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);

	// 치트키 명령 
	Order[ORDER_CHEAT_SEE]			.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_STATUS]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_SPEED]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_MONEY]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_RAIN]			.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_UPGRADE]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,      0);
	Order[ORDER_CHEAT_LEVEL]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,     0);
	Order[ORDER_CHEAT_MERCHANT]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,       0);
	Order[ORDER_CHEAT_GOODS]		.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,      0);
	Order[ORDER_CHEAT_WIN]			.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,      0);
	Order[ORDER_CHEAT_LOSE]			.SetOrder(ORDERATB_CHEAT, NOOBJECT,  0,ORDERPRIORITY_0,0,      0);
	Order[ORDER_CHEAT_ATTACKINFO]	.SetOrder(ORDERATB_CHEAT, NOOBJECT, 0,ORDERPRIORITY_0,0,         0);
	Order[ORDER_CHEAT_GOD]			.SetOrder(ORDERATB_CHEAT, NOOBJECT, 0,ORDERPRIORITY_0,0,         0);
	Order[ORDER_CHEAT_MAP]			.SetOrder(ORDERATB_CHEAT, NOOBJECT, 0,ORDERPRIORITY_0,0,         0);
	Order[ORDER_CHEAT_JUSTICEK]		.SetOrder(ORDERATB_CHEAT, NOOBJECT, 0,ORDERPRIORITY_0,0,         0);

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:31) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	Order[ORDER_FIRE_ROUND_DANCE]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIRE_ROUND_DANCE,	0);	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	Order[ORDER_NOVA]					 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_NOVA,				0);	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	Order[ORDER_FURY_OF_VOLCANO]		 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FURY_OF_VOLCANO,	0);	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	Order[ORDER_POISONING_HEAVENS]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_POISONING_HEAVENS,	0);	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	Order[ORDER_BOMB_OF_HEAVEN]			 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_BOMB_OF_HEAVEN,		0);	// 승천포(중국-발석거, 순비연 2차 전직 기술)
	Order[ORDER_SIEGE_MODE]				 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SIEGE_MODE,			0);	// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
	Order[ORDER_ONSLAUGHT_FIREBALL]		 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ONSLAUGHT_FIREBALL,	0);	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	Order[ORDER_FIRERAIN_OF_GOD]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIRERAIN_OF_GOD,	0);	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

	// robypark 2004/6/4 14:5
	Order[ORDER_SIEGE_MODE_UNDO]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SIEGE_MODE_UNDO,	0);	// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령 2차 전직 기술)
}


// 명령을 orderbuffer에 넣어 놓는다. 
BOOL MakeOrder(SHORT player_number, SHORT order, int para1, _NewID para2, BOOL clearswitch)
{
	int i;
	int index = 0;

	_NewID selectedid[MAX_SELECTED_CHAR_NUMBER];
	_OrderData temporder;

	for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	{
		selectedid[i].Init();
	}

    for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	{
	   selectedid[i] = GD.SelectedID[i];
	}
	
	ZeroMemory(&temporder, sizeof(_OrderData));

	temporder.PlayerNumber      = player_number;
	temporder.ODI.Order         = order;
	temporder.ODI.Para1         = para1;
	temporder.ODI.Para2         = para2;
	temporder.ODI.HostMode      = BY_PLAYER;
	temporder.ODI.ClearSwitch   = clearswitch;

	index = 0;

	for(i = 0;i < MAX_SELECTED_CHAR_NUMBER;i++)
	{
		if(IsAlive(selectedid[i]) == TRUE)
		{
		   temporder.IDData[index] = selectedid[i].ID;
		   index++;
		}
	}

	temporder.CharNumber = index;

	GD.PushOrderBuffer(temporder);

	return TRUE;
}


BOOL PushOrder(_OrderData order)
{
	int i, j;
	int index=-1;
	int player_number;


	player_number=order.PlayerNumber;

	// 모든 플레이어에게 명령을 준다. 
	if(player_number==-1)
	{
		for(j=0;j<MAX_MULTI_PLAYER;j++)
		{
			index=-1;

	       for(i=0;i<MAX_ORDER_QUE;i++)
		   {
		      if(GD.OrderQue[j][i].ODI.Order==0)
			  {
			      index=i;
			      break;
			  }
		   }
	       if(index<0)return FALSE;

           GD.OrderQue[j][index].PlayerNumber=j;
	       GD.OrderQue[j][index].ODI.Order=order.ODI.Order;
	       GD.OrderQue[j][index].ODI.Para1=order.ODI.Para1;
	       GD.OrderQue[j][index].ODI.Para2=order.ODI.Para2;
	       GD.OrderQue[j][index].ODI.HostMode=BY_PLAYER;
		   GD.OrderQue[j][index].ODI.ClearSwitch=order.ODI.ClearSwitch;

	       for(i=0;i<order.CharNumber;i++)
		   {
		      GD.OrderQue[j][index].IDData[i] = order.IDData[i];
		   }
           GD.OrderQue[j][index].CharNumber=order.CharNumber;
		}
	}
	// 특정 플레어어에게만 명령을 준다. 
	else
	{
	   for(i=0;i<MAX_ORDER_QUE;i++)
	   {
		   if(GD.OrderQue[player_number][i].ODI.Order==0)
		   {
			   index=i;
			   break;
		   }
	   }
	   if(index<0)return FALSE;

       GD.OrderQue[player_number][index].PlayerNumber=player_number;
	   GD.OrderQue[player_number][index].ODI.Order=order.ODI.Order;
	   GD.OrderQue[player_number][index].ODI.Para1=order.ODI.Para1;
	   GD.OrderQue[player_number][index].ODI.Para2=order.ODI.Para2;
	   GD.OrderQue[player_number][index].ODI.HostMode=BY_PLAYER;
	   GD.OrderQue[player_number][index].ODI.ClearSwitch=order.ODI.ClearSwitch;

	   for(i=0;i<order.CharNumber;i++)
	   {
		  GD.OrderQue[player_number][index].IDData[i]=order.IDData[i];
	   }
       GD.OrderQue[player_number][index].CharNumber=order.CharNumber;
	}

   return TRUE;
}

BOOL PopOrder()
{
	int i, j, k;

	// 온라인 전투에서 사용자들의 중요 변동 사항이 생기면, 
	MyNet.UpdatePlayerInfoToServer();

	MyNet.WaitForMessageFrom();

	for(i=0;i<MAX_MULTI_PLAYER;i++)
		for(j=0;j<MAX_ORDER_QUE;j++)
		{
			if(GD.OrderQue[i][j].ODI.Order == 0) continue;
			
			// 전리품 획득 명령이면, 
			if(GD.OrderQue[i][j].ODI.Order == ORDER_GETITEM)
			{
				OrderOperateGetItem(GD.OrderQue[i][j]);
			}
			// 캐릭터 정보 업데이트 명령이면, 
			else if(GD.OrderQue[i][j].ODI.Order == ORDER_UPDATECHAR)
			{
				OrderOperateUpdateChar(GD.OrderQue[i][j]);
			}
			// 치트 명령의 처리 
			else if(Order[GD.OrderQue[i][j].ODI.Order].Atb==ORDERATB_CHEAT)
			{
				Cheat.CheatOperation(GD.OrderQue[i][j]);
			}
			// 자동 마법 명령 설정 
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_AUTOMAGIC)
			{						
				GD.Player[i].AutoMagicSwitch=TRUE;
				//Config.AutoMagicSwitch=TRUE;
				//Config.Save();
			}
			// 자동 마법 명령 해제 
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_NOAUTOMAGIC)
			{
				GD.Player[i].AutoMagicSwitch=FALSE;
				//Config.AutoMagicSwitch=FALSE;
				//Config.Save();
			}
			// 동맹 신청/해제  
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_ALLY)
			{
				OrderOperateAlly(GD.OrderQue[i][j]);
			}
			// 사용자가 게임을 빠져나간다는 메시지면, 
			else if(GD.OrderQue[i][j].ODI.Order == ORDER_QUIT)
			{
				OrderOperateQuit(GD.OrderQue[i][j]);
			}
			// 일반 캐릭터 명령의 처리 
			else
			{
				for(k=0;k<GD.OrderQue[i][j].CharNumber;k++)
				{
					LetOrder(GD.OrderQue[i][j].ODI.Order, 
						CR[ GD.OrderQue[i][j].IDData[k] ].m_NewID, 
						GD.OrderQue[i][j].ODI.Para1, 
						GD.OrderQue[i][j].ODI.Para2, 
						GD.OrderQue[i][j].ODI.HostMode,
						GD.OrderQue[i][j].ODI.ClearSwitch);
				}
			}
			GD.OrderQue[i][j].ODI.Order=0;
		}
	
	return TRUE;
}


// 플레이어의 명령을 전달한다. 
BOOL LetOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch)
{
	if(IsAlive(id.ID)==FALSE)return FALSE;

	_OrderDataInfo odi;
	ZeroMemory(&odi, sizeof(_OrderDataInfo));

	odi.Order=order;
	odi.HostMode=host;

    odi.Para1=para1;
    odi.Para2=para2;

	odi.ClearSwitch=clearswitch;


	// 명령을 주는 즉시 처리해야 하는 명령은 바로 처리한다. 
	if(clearswitch==TRUE)
	{

	   clifChar.PushOrder(id.ID, odi, TRUE, 100);
	   
	   // 예약 명령을 없앤다. 
	   // 집결지 설정은 예약명령을 없애지 않는다. 
	   if(order==ORDER_TRAINSPOT)
	   {
	   }
	   else
	   {
	     clifChar.ClearReserveOrder(id.ID);
	   }
    }
	// 멀티 명령은 버퍼에 넣은 후에 차근 차근 처리하게 한다. 
	else
	{
	   clifChar.PushReserveOrder(id.ID, odi);
	}

	return TRUE;

}

// 컴퓨터가 자신에게 명령을 전달한다. 
BOOL DoOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch)
{
	if(IsAlive(id.ID)==FALSE)return FALSE;

	_OrderDataInfo odi;
	ZeroMemory(&odi, sizeof(_OrderDataInfo));

	odi.Order=order;
	odi.HostMode=host;

    odi.Para1=para1;
    odi.Para2=para2;

	odi.ClearSwitch=clearswitch;

    clifChar.PushOrder(id.ID, odi, TRUE, 200);

	return TRUE;
}

void OrderStop(_NewID id, int hostmode)
{
	DoOrder(ORDER_STOP, id, 0, ZeroID, hostmode, FALSE);
}

void OrderHold(_NewID id, int hostmode)
{
	DoOrder(ORDER_HOLD, id, 0, ZeroID, hostmode, FALSE);
}

void OrderBuildToCar(_NewID id, int hostmode)
{
	DoOrder(ORDER_BUILDTOCAR, id, 0, ZeroID, hostmode, FALSE);
}


// 캐릭터를 이동시킨다. 
void OrderMove(_NewID id, int x, int y, int hostmode)
{
	DoOrder(ORDER_MOVE, id, (x|(y<<16)), ZeroID, hostmode, FALSE);
}

BOOL OrderAttack(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_ATTACK, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderEarthQuake(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_EARTHQUAKE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderTornado(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_TORNADO, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderYellowSand(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_YELLOWSAND, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderSoul(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_SOUL, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderGhost(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_GHOST, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderDarkFire(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_DARKFIRE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}


BOOL OrderCapture(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_CAPTUREBUILD, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderBetray(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_BETRAY, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderAbsorb(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_ABSORB, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderMagic8j(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_MAGIC8J, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderStealTech(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_STEALTECH, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderLightning(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_LIGHTNING, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}


BOOL OrderTakeIn(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_TAKEIN, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderUseBuda(_NewID id, char host)
{

	DoOrder(ORDER_USEBUDA, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderHeal(_NewID id, _NewID targetid, char host)
{

	DoOrder(ORDER_HEAL, id, 0, targetid, host, FALSE);
	return TRUE;
}

BOOL OrderMana(_NewID id, _NewID targetid, char host)
{
	DoOrder(ORDER_MANA, id, 0, targetid, host, FALSE);
	return TRUE;
}

BOOL OrderStealExp(_NewID id, char host)
{

	DoOrder(ORDER_STEALEXP, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderShield(_NewID id, _NewID targetid, char host)
{

	DoOrder(ORDER_SHIELD, id, 0, targetid, host, FALSE);
	return TRUE;
}

BOOL OrderConvoy(_NewID id, _NewID targetid, char host)
{

	DoOrder(ORDER_CONVOY, id, 0, targetid, host, FALSE);
	return TRUE;
}

BOOL OrderRepair(_NewID id, char host)
{

	DoOrder(ORDER_REPAIR, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderSell(_NewID id, char host)
{

	DoOrder(ORDER_SELL, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderClon(_NewID id, char host)
{
	DoOrder(ORDER_CLON, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderEnemyClon(_NewID newid, _NewID enemynewid, char host)
{

	DoOrder(ORDER_ENEMYCLON, newid, 0, enemynewid, host, FALSE);
	return TRUE;
}

// 소환 공통
BOOL OrderReCallDamduk(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_DAMDUK, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//팬더 소환.
/////////////////////////////////////////////////////////////////////
BOOL OrderReCallPanda(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_PANDA, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//수호부 (쥐) 
BOOL OrderRecallGuardianMouse(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_GUARDIAN_MOUSE, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//수호부 (토끼) 
BOOL OrderGuardianRabbit(_NewID id, char host)
{
	DoOrder(ORDER_GUARDIAN_RABBIT, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//수호부 (소)
BOOL OrderRecallGuardianCaw(_NewID id, char host,SHORT kind)
{
	DoOrder(ORDER_RECALL_GUARDIAN_CAW, id, kind, ZeroID, host, FALSE);
	return TRUE;
}

//수호부 (용)
BOOL OrderGuardianDragon(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_GUARDIAN_DRAGON, id, 0, attackid, host, FALSE);
	return TRUE;
}

//수호부 (호랑이)
BOOL OrderGuardianTiger(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_GUARDIAN_TIGER, id, 0, attackid, host, FALSE);
	return TRUE;
}

//수호부 (말)
BOOL OrderGuardianHorse(_NewID id,char host)
{
	DoOrder(ORDER_GUARDIAN_HORSE, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//수호부 (양)
BOOL OrderGuardianSheep(_NewID id,char host)
{
	DoOrder(ORDER_GUARDIAN_SHEEP, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//황룡 소환.
/////////////////////////////////////////////////////////////////////
BOOL OrderReCallMonkey(_NewID id,char host)
{
	DoOrder(ORDER_RECALL_MONKEY, id, 0, ZeroID, host, FALSE);
	return TRUE;
}
//요술사 소환.
BOOL OrderReCallAmakusa(_NewID id,char host)
{
	DoOrder(ORDER_RECALL_AMAKUSA, id, 0, ZeroID, host, FALSE);
	return TRUE;
}	

//졸개 호랑이 소환.
BOOL OrderReCallTiger(_NewID id,char host)
{
	DoOrder(ORDER_RECALL_TIGER, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderHarvest(_NewID id, SHORT x, SHORT y, char host)
{

	DoOrder(ORDER_HARVEST, id, (x|(y<<16)), ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderExtinguish(_NewID id, SHORT x, SHORT y, char host)
{
	DoOrder(ORDER_EXTINGUISH, id, (x|(y<<16)), ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderRide(_NewID id, SHORT x, SHORT y, _NewID vehicle, char host)
{

	DoOrder(ORDER_RIDE, id, (x|(y<<16)), vehicle, host, FALSE);
	return TRUE;
}

BOOL OrderMoveRideOut(_NewID id, int x, int y, char host)
{

	DoOrder(ORDER_MOVERIDEOUT, id, (x|(y<<16)), ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderMine(_NewID id, SHORT x, SHORT y, char host)
{
	DoOrder(ORDER_MINE, id, (x|(y<<16)), ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderRideOut(_NewID id, char host)
{
	DoOrder(ORDER_RIDEOUT, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderAirMagic(_NewID id, char host)
{
	DoOrder(ORDER_AIRMAGIC, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderKimBomb(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_KIMBOMB, id, 0, attackid, host, FALSE);
	return TRUE;
}

BOOL OrderMagic5j(_NewID id, char host)
{
	DoOrder(ORDER_MAGIC5J, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderWarp(_NewID id, char host)
{
	DoOrder(ORDER_WARP, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderFarmerToBull(_NewID id, char host)
{

	DoOrder(ORDER_FARMERTOBULL, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

BOOL OrderRainMake(_NewID id, char host)
{

	DoOrder(ORDER_RAINMAKE, id, 0, ZeroID, host, FALSE);
	return TRUE;
}


BOOL OrderSelfExp(_NewID id, SHORT x, SHORT y, char host)
{

	DoOrder(ORDER_SELFEXP, id, (x|(y<<16)), ZeroID, host, FALSE);
	return TRUE;
}


// 이동시키고 하차 시킨후 공격 시킨다.
BOOL OrderMoveNRideOutNAttack(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_MOVENRIDEOUTNATTACK, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}


BOOL OrderUpgrade(_NewID id, SHORT upgradekind, char host)
{
	
	SHORT i;

	
	for(i=0;i<MAX_ORDER_NUMBER;i++)
	{
		if(Order[i].IsAtb(ORDERATB_UPGRADE) && Order[i].GetPara1()==upgradekind)
		{
			DoOrder(i, id, 0, ZeroID, host, FALSE);
			return TRUE;
		}
	}

	return TRUE;
}

BOOL OrderGroundExp(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_GROUNDEXP, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderMagic4c(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_MAGIC4C, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

//마법 테스트 
//////////////////////////////////////////////////////////////////////////
BOOL OrderMagicTest(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_MAGICTEST, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
BOOL OrderMagic6c(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{

	DoOrder(ORDER_MAGIC6C, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

//이사의 사념 특수 기술.
BOOL OrderIce(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_ICE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderLongSpear(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_LONG_SPEAR,id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 빙석술
BOOL OrderIceBarrier(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_ICE_BARRIER, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 석회술
BOOL OrderStoneMagic(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_STONE_MAGIC, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderManaburn(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_MANABURN, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

BOOL OrderTrain(_NewID id, SHORT kind, char host)
{
	
	SHORT i;

	
	for(i=0;i<MAX_ORDER_NUMBER;i++)
	{
		if(Order[i].IsAtb(ORDERATB_TRAIN) && Order[i].GetPara1()==kind)
		{
			DoOrder(i, id, 0, ZeroID, host, FALSE);
			return TRUE;
		}
	}

	return TRUE;
}


BOOL OrderBuild(_NewID id, SHORT kind, SHORT x, SHORT y, char host)
{
	
	SHORT i;

	
	for(i=0;i<MAX_ORDER_NUMBER;i++)
	{
		// 건설명령중에 해당하는 것이있으면 
		if(Order[i].IsAtb(ORDERATB_BUILD) && Order[i].GetPara1()==kind )
		{
			DoOrder(i, id, x|(y<<16), ZeroID, host, FALSE);
			return TRUE;
		}
	}

	return TRUE;
}


// 속성과 파라메타를 가지고 해당 명령을 찾는다.
SHORT FindOrder(SHORT atb, SHORT para1, SHORT para2)
{

	SHORT i;
	SHORT order=0;

	for(i=0;i<MAX_ORDER_NUMBER;i++)
	{
		if(Order[i].Atb&atb)
		{
			if(para1>=0 && para1!=Order[i].Para1)continue;
			if(para2>=0 && para2!=Order[i].Para1)continue;

			order=i;
		}

	}

	return order;
}

// 캐릭터 정보를 업데이트 할 수 있는 캐릭터인가?
BOOL _clBattle::IsProperCharForUpdate(SI32 playernumber)
{
	// 혼자하기에서는 업데이트 하지 않는다. 
	//if(OBInfo.IsSingleMode == TRUE)return FALSE;

	// 사용자가 조작하는 캐릭터이거나, 
	// 호스트 컴의 몬스터인경우, 
	if(playernumber == UserPlayerNumber
	|| (ISMONSTER(MyNet.PlayersdpId.dpId[playernumber])  && MyNet.IsHost()) )
		return TRUE;

	return FALSE;
}

SI32 _clBattle::FindUpdateCharInfoChar()
{
	// 데이터를 보내야 할 캐릭터의 아이디를 구한다. 
	SI32 updateid = 0;
	

	for(SI32 i = LastUpdateCharInfo + 1; i < MAX_CHAR_NUMBER; i++)
	{
		SI32 pnumber = CR[i].GetPlayerNumber();
		if(GD.CharInfo[i])
		{

			if( pnumber== UserPlayerNumber
			|| (ISMONSTER(MyNet.PlayersdpId.dpId[pnumber]) &&  MyNet.IsHost()))
			{
				// 캐릭터의 정보값을 변경한다. 
				CR[i].SetUpdateInfo();

				// 변경된 정보가 없다면 업데이트 하지 않는다. 
				if(CR[i].GetUpdateInfo() != CR[i].GetOldUpdateInfo())
				{
					CR[i].SetOldUpdateInfo();

					updateid = i;
					LastUpdateCharInfo = i;


					break;
				}

			}
		}
	}
	return updateid;
}

BOOL _clBattle::UpdateCharInfo(SI32 updateid, SI32 killerid, _OrderData *ptemporder)
{

	if(CR[updateid].m_cltCharStatus.GetKind() == 29)
		int a = 0 ;
	ptemporder->ODI.Order			= ORDER_UPDATECHAR;
	ptemporder->ODI.HostMode		= BY_PLAYER;
	ptemporder->ODI.ClearSwitch		= TRUE;
	
	ptemporder->ODI.Para1				= 0;
	ptemporder->ODI.Para2				= ZeroID;
	
	ptemporder->PlayerNumber		= CR[updateid].GetPlayerNumber();   
	ptemporder->CharNumber			= MAX_SELECTED_CHAR_NUMBER;
	
	// 캐릭터 아이디를 보낸다. 
	ptemporder->IDData[0]			= CR[updateid].m_NewID.ID;
	// 캐릭터 종류를 보낸다. 
	
	UI16 Kind = CR[updateid].m_cltCharStatus.GetKind();
	//ptemporder->IDData[1]			= CR[updateid].m_cltCharStatus.GetKind();
	memcpy(&ptemporder->IDData[1], &Kind, sizeof(UI16));

	// 캐릭터 체력을 보낸다. 
	UI16 life = CR[updateid].m_ParmeterBox.IP.GetLife();
	memcpy(&ptemporder->IDData[3], &life, sizeof(UI16));

	// 캐릭터 마나를 보낸다. 
	UI16 mana = CR[updateid].m_ParmeterBox.IP.GetMana();
	memcpy(&ptemporder->IDData[5], &mana, sizeof(UI16));

	// 캐릭터 위치를 보낸다. 
	ptemporder->IDData[7]			= CR[updateid].GetX();
	ptemporder->IDData[8]			= CR[updateid].GetY();
	
	// 이 캐릭터를 죽인캐릭터(killer) 아이디를 보낸다. 
	ptemporder->IDData[9]			= killerid;

	return TRUE;
}

void _clBattle::PushUpdateCharInfo(SI32 id, SI32 killerid)
{

	SI32 i;

	if(UpdateCharInfoDataIndex >= (MAX_UPDATRE_CHAR_INFO_NUMBER-1))return ;

	_OrderData temporder;
	
	// 이미 해당 캐릭터의 명령이 저장되어 있는 상태라면 다시 재 설정한다. 
	for(i = 0; i < UpdateCharInfoDataIndex;i++)
	{
		memcpy(&temporder, UpdateCharInfoData[i], sizeof(_OrderData));

		// 같은 캐릭터의 정보가 들어 있다. 
		if(temporder.IDData[0] == id)
		{
			if(UpdateCharInfo(id,  killerid, &temporder) == TRUE)
			{
				// 덮어 써 버린다. 
				memcpy(UpdateCharInfoData[i], &temporder, sizeof(_OrderData));
				return ;
			}
		}
	}

	if(UpdateCharInfo(id,  killerid, &temporder) == TRUE)
	{
		memcpy(UpdateCharInfoData[UpdateCharInfoDataIndex], &temporder, sizeof(_OrderData));
		UpdateCharInfoDataIndex++;
		return ;
	}
}

BOOL _clBattle::PopUpdateCharInfo(_OrderData* pdata)
{

	if(UpdateCharInfoDataIndex == 0)return FALSE;

	memcpy(pdata, UpdateCharInfoData[0], sizeof(_OrderData));

	for(SI32 i = 1;i < UpdateCharInfoDataIndex;i++)
	{
		memcpy(UpdateCharInfoData[i-1], UpdateCharInfoData[i], sizeof(_OrderData));
	}
	UpdateCharInfoDataIndex--;
	return TRUE;
}


void OrderOperateGetItem(_OrderData order)
{
	SI32 i;
	char buffer[128];

	UI08 playernumber		= order.IDData[0];
	UI08 slotnumber			= order.IDData[1];
	
	UI08 lostplayernumber	= order.IDData[2];
	UI08 lostslotnumber		= order.IDData[3];

	UI16 itemcode;
	memcpy(&itemcode, &order.IDData[4], sizeof(UI16));


	// 해당 캐릭터의 아이디를 찾는다. 
	SI32 charid = 0;
	for(i=0;i<MAX_CHAR_NUMBER;i++)
	{
		if(IsAlive(i) && CR[i].GetPlayerNumber() == playernumber && CR[i].m_siOnlineID == slotnumber)
		{
			charid = i;
			break;
		}
	}

	if(charid == 0)return ;

	if(playernumber == UserPlayerNumber)
	{
		sprintf(buffer, Text.GetItemMessage.Get(), 
			CR[charid].GetName(), 
			pGame->GetItemName(itemcode));

		pGame->pItemInfoWindow->AddItemInfoWindow(60, 250, 10, itemcode, buffer);

		PushEffect(EFFECT_GETITEM, 0, 0);
	}
}

void OrderOperateUpdateChar(_OrderData order)
{
	
	// 해당 캐릭터의 정보를 업데이트 한다.
	SI32 playernumber	= order.PlayerNumber;

	UI08 id			= order.IDData[0];

	UI16 kind;
	memcpy(&kind, &order.IDData[1], sizeof(UI16));
	
	UI16 life;  		//= (UI16)order.IDData[2];
	memcpy(&life, &order.IDData[3], sizeof(UI16));

	UI16 mana;			//= (UI16)order.IDData[3];
	memcpy(&mana, &order.IDData[5], sizeof(UI16));

	UI08 posx		= order.IDData[7];
	UI08 posy		= order.IDData[8];
	UI08 killerid	= order.IDData[9];

	// 캐릭터가 유효하고 
	if(GD.CharInfo[id])
	{
		// 같은 종류의 캐릭터이면, 
		if(CR[id].m_cltCharStatus.GetKind() == kind)
		{
			// 킬러가 내가 관할하는 캐릭터라면, 
			if(GD.CharInfo[killerid])
			{
				if(CR[killerid].GetPlayerNumber() == UserPlayerNumber
					|| ( ISMONSTER(MyNet.PlayersdpId.dpId[CR[killerid].GetPlayerNumber()] ) &&  MyNet.IsHost()))
				{
					
					// 서버에 내가 누군가를 죽였음을 통보한다. 

					// 무도장이 아니어야 한다. 
					if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
							MyNet.PlayersdpId.dpId[ CR[killerid].GetPlayerNumber()],	CR[killerid].m_siOnlineID, 
							MyNet.PlayersdpId.dpId[ CR[id].GetPlayerNumber()],		CR[id].m_siOnlineID);
						
						// 킬러에게 합당한 경험치등을 제공한다. 
						pGame->pBattle->ExpOperate(killerid, id);
					}
				}
				
			}

			//업데이트 해야할 캐릭터가 내가 가진 캐릭터라면 업데이트 하지 않는다. 
			// 업데이트 하게 되면 2중으로 설정하는셈. 
			if(CR[id].GetPlayerNumber() == UserPlayerNumber
			|| ( ISMONSTER(MyNet.PlayersdpId.dpId[CR[id].GetPlayerNumber()] ) &&  MyNet.IsHost()))
			{
			}
			else
			{

				CR[id].m_ParmeterBox.IP.SetLife(life);
				CR[id].m_ParmeterBox.IP.SetMana(mana);
				
				SI32 length = max( abs(CR[id].m_ssX - posx), abs(CR[id].m_ssY - posy));
				
				// 거리가 많이 떨어져 있으면 워프한다. 
				if(length > 3)
				{
					if(CR[id].IsColl(posx, posy)==FALSE)
					{
						CR[id].DeleteID();

						CR[id].m_ssX = posx;
						CR[id].m_ssY = posy;

						CR[id].SetID();
					}
				}
				else if(length)
				{
					OrderMove(CR[id].m_NewID, posx, posy, BY_PLAYER);
				}
			}

		}
		// 다른 종류의 캐릭터라면, 죽인다.(다시 만들기 위해서..)
		else
		{
			CR[id].m_ParmeterBox.IP.SetLife(0);
		}
	}
	// 캐릭터가 유효하지 않으면, 만든다. 
	else
	{
		// 승리도 패배도 아닌 경우에만, 
		// 승리나 패배인 경우에는 캐릭터가 사라지기 때문에...
		// 이미 무효화된 사용자인 경우에는 캐릭터를 만들어서는 안된다. 
		if(GD.VictoryDelay == 0 && GD.DefeatDelay == 0 && MyNet.PlayersdpId.dpId[playernumber])
		{
			// 내가 직접 관리하는 캐릭터는 다시 만들면 안된다. 
			if(playernumber == UserPlayerNumber
				|| ( ISMONSTER(MyNet.PlayersdpId.dpId[playernumber] ) &&  MyNet.IsHost()))
			{}
			else
			{
				
				_ImportantParameta ip;
				ZeroMemory(&ip, sizeof(_ImportantParameta));
				ip.Init(0,		1,		10,				10,		
										10,				10,				10, 
										10,				10,				0,         
										0,	
										0,
										0,0,
										100);

				
				ip.SetLife(life);
				ip.SetMana(mana);
				
				// 캐릭터를 생성한다. 
				SetChar(kind, id, posx, posy, SOUTH, 100, playernumber, &ip, NULL, -1);

			}
		}

	}

}


void OrderOperateAlly(_OrderData order)
{
	// i가 pnumber와의 동맹 상태 설정하기
	
	SI32 i			= order.PlayerNumber;
	SI32 pnumber	= order.ODI.Para1;
	SI32 k, m;
	char buffer[128];

	
	// 동맹의 해제.
	if(GD.OnlineAllyNumSwitch[i][pnumber] == TRUE)
	{
		GD.OnlineAllyNumSwitch[i][pnumber] = FALSE;
		
		// actdoll (2004/09/17 11:22) : 동맹 해제 메시지
		wsprintf(buffer, Text.Msg_About_Destroy_Alliance.Get(), MyNet.PlayersName.Name[i], MyNet.PlayersName.Name[pnumber]);
		SetMessage(buffer);
		
		// EnemyBit를 설정한다. 
		for(k = 0;k < MAX_MULTI_PLAYER;k++)
		{
			GD.Player[k].PI.MyBit=(1<<GD.Player[k].PI.PlayerNumber);
			
			for(m = 0;m < MAX_MULTI_PLAYER;m++)
			{
				if(k==m)continue;
				
				if(GD.IsFriendTo(k, m) == FALSE)
				{
					GD.Player[k].PI.EnemyBit|=(1<<m);
				}
			}
		}
	}
	// 동맹을 맺는다. 
	else
	{
		
		GD.OnlineAllyNumSwitch[i][pnumber] = TRUE;
		
		// actdoll (2004/09/17 11:22) : 동맹 요청 메시지
		wsprintf(buffer, Text.Msg_About_Request_Alliance.Get(), MyNet.PlayersName.Name[i], MyNet.PlayersName.Name[pnumber]);
		SetMessage(buffer);
		
		// 동맹을 맺어주는 함수를 호출한다. 
		// 상대가 이미 나에게 동맹을 신청한 상태여야 한다. 
		if(GD.OnlineAllyNumSwitch[pnumber][i] == TRUE)
		{
			
			// EnemyBit를 설정한다. 
			for(k = 0;k < MAX_MULTI_PLAYER;k++)
			{
				GD.Player[k].PI.MyBit=(1<<GD.Player[k].PI.PlayerNumber);
				
				for(m = 0;m < MAX_MULTI_PLAYER;m++)
				{
					if(k==m)continue;
					
					if(GD.IsFriendTo(k, m) == FALSE)
					{
						GD.Player[k].PI.EnemyBit|=(1<<m);
					}
				}
			}

		}
	}
}


void OrderOperateQuit(_OrderData order)
{
	
	SI32 k;

	DWriteStringToFile("게임종료");
	SI32 pnumber = order.PlayerNumber;

	// 빠져나간 사용자가 원래 호스트였다면, 
	if(MyNet.OperateHostQuit(MyNet.PlayersdpId.dpId[ pnumber ]) == TRUE)
	{
		
	}
	
	
	for(k = 0 ;k < MAX_CHAR_NUMBER;k++)
	{
		if(IsAlive(k) && clifChar.GetPlayerNumber(k) == pnumber)
		{
			DeleteChar(k);
		}
	}

}

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:42) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
BOOL OrderFIRE_ROUND_DANCE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FIRE_ROUND_DANCE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
BOOL OrderNOVA( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_NOVA, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
BOOL OrderFURY_OF_VOLCANO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FURY_OF_VOLCANO, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
BOOL OrderPOISONING_HEAVENS( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_POISONING_HEAVENS, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 승천포(중국-발석거, 순비연 2차 전직 기술)
BOOL OrderBOMB_OF_HEAVEN( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_BOMB_OF_HEAVEN, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
BOOL OrderSIEGE_MODE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_SIEGE_MODE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
BOOL OrderONSLAUGHT_FIREBALL( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_ONSLAUGHT_FIREBALL, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
BOOL OrderFIRERAIN_OF_GOD( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FIRERAIN_OF_GOD, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// robypark 2004/6/4 14:3
// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령 2차 전직 기술)
BOOL OrderSIEGE_MODE_UNDO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_SIEGE_MODE_UNDO, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}
