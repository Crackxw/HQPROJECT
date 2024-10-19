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
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_MyNetGame			MyNet;
extern	_Cheat 				Cheat;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	cltMapInterface		clifMap;							// �ʰ��� �������̽� 
extern	_NewID 				ZeroID;
extern _Char   CR[];       // ĳ������ ������ 
extern SI32	UserPlayerNumber;
extern	_Text 				Text;								// �ؽ�Ʈ 

void OrderOperateGetItem(_OrderData order);
void OrderOperateUpdateChar(_OrderData order);
void OrderOperateAlly(_OrderData order);
void OrderOperateQuit(_OrderData order);

// ����� ���� ������ �����Ѵ�. 
void _Order::SetOrder(SHORT atb, SHORT object, int info, SHORT priority, int para1, int para2)
{
	Atb=atb;
	Object=object;
	Info=info;
	Priority=priority;
	Para1=para1;
	Para2=para2;
}

// ����� �Ӽ��� ���Ѵ�. 
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

	// ���� ��� 
	Order[ORDER_AUTOMAGIC]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_NOAUTOMAGIC]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_GENERALFIRST]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_NOGENERALFIRST]		.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ALLY]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_QUIT]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);

	// ���� 
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

	// ��� ��Ż�� ���. 
	Order[ORDER_WEAPONOFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_WEAPONON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ARMOUROFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ARMOURON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ETCOFF]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_ETCON]				.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_POTIONON]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);
	Order[ORDER_POTIONOFF]			.SetOrder(ORDERATB_NORMAL,      NOOBJECT,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4,0, 0);

	// ġƮŰ ��� 
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
// robypark (2004/05/17 15:31) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	Order[ORDER_FIRE_ROUND_DANCE]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIRE_ROUND_DANCE,	0);	// ����������(����-�ź���, �̼��� 2�� ���� ���)
	Order[ORDER_NOVA]					 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_NOVA,				0);	// ����(����-������, ���� 2�� ���� ���)
	Order[ORDER_FURY_OF_VOLCANO]		 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FURY_OF_VOLCANO,	0);	// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
	Order[ORDER_POISONING_HEAVENS]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_POISONING_HEAVENS,	0);	// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
	Order[ORDER_BOMB_OF_HEAVEN]			 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_BOMB_OF_HEAVEN,		0);	// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
	Order[ORDER_SIEGE_MODE]				 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SIEGE_MODE,			0);	// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
	Order[ORDER_ONSLAUGHT_FIREBALL]		 .SetOrder(ORDERATB_MAGIC, OBJECT1,  ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_ONSLAUGHT_FIREBALL,	0);	// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
	Order[ORDER_FIRERAIN_OF_GOD]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_FIRERAIN_OF_GOD,	0);	// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 

	// robypark 2004/6/4 14:5
	Order[ORDER_SIEGE_MODE_UNDO]		 .SetOrder(ORDERATB_MAGIC, NOOBJECT, ORDERINFO_SAMEPRIORITY,ORDERPRIORITY_4, MAGICPOWER_SIEGE_MODE_UNDO,	0);	// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
}


// ����� orderbuffer�� �־� ���´�. 
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

	// ��� �÷��̾�� ����� �ش�. 
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
	// Ư�� �÷����Ը� ����� �ش�. 
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

	// �¶��� �������� ����ڵ��� �߿� ���� ������ �����, 
	MyNet.UpdatePlayerInfoToServer();

	MyNet.WaitForMessageFrom();

	for(i=0;i<MAX_MULTI_PLAYER;i++)
		for(j=0;j<MAX_ORDER_QUE;j++)
		{
			if(GD.OrderQue[i][j].ODI.Order == 0) continue;
			
			// ����ǰ ȹ�� ����̸�, 
			if(GD.OrderQue[i][j].ODI.Order == ORDER_GETITEM)
			{
				OrderOperateGetItem(GD.OrderQue[i][j]);
			}
			// ĳ���� ���� ������Ʈ ����̸�, 
			else if(GD.OrderQue[i][j].ODI.Order == ORDER_UPDATECHAR)
			{
				OrderOperateUpdateChar(GD.OrderQue[i][j]);
			}
			// ġƮ ����� ó�� 
			else if(Order[GD.OrderQue[i][j].ODI.Order].Atb==ORDERATB_CHEAT)
			{
				Cheat.CheatOperation(GD.OrderQue[i][j]);
			}
			// �ڵ� ���� ��� ���� 
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_AUTOMAGIC)
			{						
				GD.Player[i].AutoMagicSwitch=TRUE;
				//Config.AutoMagicSwitch=TRUE;
				//Config.Save();
			}
			// �ڵ� ���� ��� ���� 
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_NOAUTOMAGIC)
			{
				GD.Player[i].AutoMagicSwitch=FALSE;
				//Config.AutoMagicSwitch=FALSE;
				//Config.Save();
			}
			// ���� ��û/����  
			else if(GD.OrderQue[i][j].ODI.Order==ORDER_ALLY)
			{
				OrderOperateAlly(GD.OrderQue[i][j]);
			}
			// ����ڰ� ������ ���������ٴ� �޽�����, 
			else if(GD.OrderQue[i][j].ODI.Order == ORDER_QUIT)
			{
				OrderOperateQuit(GD.OrderQue[i][j]);
			}
			// �Ϲ� ĳ���� ����� ó�� 
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


// �÷��̾��� ����� �����Ѵ�. 
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


	// ����� �ִ� ��� ó���ؾ� �ϴ� ����� �ٷ� ó���Ѵ�. 
	if(clearswitch==TRUE)
	{

	   clifChar.PushOrder(id.ID, odi, TRUE, 100);
	   
	   // ���� ����� ���ش�. 
	   // ������ ������ �������� ������ �ʴ´�. 
	   if(order==ORDER_TRAINSPOT)
	   {
	   }
	   else
	   {
	     clifChar.ClearReserveOrder(id.ID);
	   }
    }
	// ��Ƽ ����� ���ۿ� ���� �Ŀ� ���� ���� ó���ϰ� �Ѵ�. 
	else
	{
	   clifChar.PushReserveOrder(id.ID, odi);
	}

	return TRUE;

}

// ��ǻ�Ͱ� �ڽſ��� ����� �����Ѵ�. 
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


// ĳ���͸� �̵���Ų��. 
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

// ��ȯ ����
BOOL OrderReCallDamduk(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_DAMDUK, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//�Ҵ� ��ȯ.
/////////////////////////////////////////////////////////////////////
BOOL OrderReCallPanda(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_PANDA, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//��ȣ�� (��) 
BOOL OrderRecallGuardianMouse(_NewID id, char host)
{
	DoOrder(ORDER_RECALL_GUARDIAN_MOUSE, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//��ȣ�� (�䳢) 
BOOL OrderGuardianRabbit(_NewID id, char host)
{
	DoOrder(ORDER_GUARDIAN_RABBIT, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//��ȣ�� (��)
BOOL OrderRecallGuardianCaw(_NewID id, char host,SHORT kind)
{
	DoOrder(ORDER_RECALL_GUARDIAN_CAW, id, kind, ZeroID, host, FALSE);
	return TRUE;
}

//��ȣ�� (��)
BOOL OrderGuardianDragon(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_GUARDIAN_DRAGON, id, 0, attackid, host, FALSE);
	return TRUE;
}

//��ȣ�� (ȣ����)
BOOL OrderGuardianTiger(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_GUARDIAN_TIGER, id, 0, attackid, host, FALSE);
	return TRUE;
}

//��ȣ�� (��)
BOOL OrderGuardianHorse(_NewID id,char host)
{
	DoOrder(ORDER_GUARDIAN_HORSE, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//��ȣ�� (��)
BOOL OrderGuardianSheep(_NewID id,char host)
{
	DoOrder(ORDER_GUARDIAN_SHEEP, id, 0, ZeroID, host, FALSE);
	return TRUE;
}

//Ȳ�� ��ȯ.
/////////////////////////////////////////////////////////////////////
BOOL OrderReCallMonkey(_NewID id,char host)
{
	DoOrder(ORDER_RECALL_MONKEY, id, 0, ZeroID, host, FALSE);
	return TRUE;
}
//����� ��ȯ.
BOOL OrderReCallAmakusa(_NewID id,char host)
{
	DoOrder(ORDER_RECALL_AMAKUSA, id, 0, ZeroID, host, FALSE);
	return TRUE;
}	

//���� ȣ���� ��ȯ.
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


// �̵���Ű�� ���� ��Ų�� ���� ��Ų��.
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

//���� �׽�Ʈ 
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

//�̻��� ��� Ư�� ���.
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

// ������
BOOL OrderIceBarrier(_NewID id, SHORT x, SHORT y, _NewID attackid, char host)
{
	DoOrder(ORDER_ICE_BARRIER, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// ��ȸ��
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
		// �Ǽ�����߿� �ش��ϴ� ���������� 
		if(Order[i].IsAtb(ORDERATB_BUILD) && Order[i].GetPara1()==kind )
		{
			DoOrder(i, id, x|(y<<16), ZeroID, host, FALSE);
			return TRUE;
		}
	}

	return TRUE;
}


// �Ӽ��� �Ķ��Ÿ�� ������ �ش� ����� ã�´�.
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

// ĳ���� ������ ������Ʈ �� �� �ִ� ĳ�����ΰ�?
BOOL _clBattle::IsProperCharForUpdate(SI32 playernumber)
{
	// ȥ���ϱ⿡���� ������Ʈ ���� �ʴ´�. 
	//if(OBInfo.IsSingleMode == TRUE)return FALSE;

	// ����ڰ� �����ϴ� ĳ�����̰ų�, 
	// ȣ��Ʈ ���� �����ΰ��, 
	if(playernumber == UserPlayerNumber
	|| (ISMONSTER(MyNet.PlayersdpId.dpId[playernumber])  && MyNet.IsHost()) )
		return TRUE;

	return FALSE;
}

SI32 _clBattle::FindUpdateCharInfoChar()
{
	// �����͸� ������ �� ĳ������ ���̵� ���Ѵ�. 
	SI32 updateid = 0;
	

	for(SI32 i = LastUpdateCharInfo + 1; i < MAX_CHAR_NUMBER; i++)
	{
		SI32 pnumber = CR[i].GetPlayerNumber();
		if(GD.CharInfo[i])
		{

			if( pnumber== UserPlayerNumber
			|| (ISMONSTER(MyNet.PlayersdpId.dpId[pnumber]) &&  MyNet.IsHost()))
			{
				// ĳ������ �������� �����Ѵ�. 
				CR[i].SetUpdateInfo();

				// ����� ������ ���ٸ� ������Ʈ ���� �ʴ´�. 
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
	
	// ĳ���� ���̵� ������. 
	ptemporder->IDData[0]			= CR[updateid].m_NewID.ID;
	// ĳ���� ������ ������. 
	
	UI16 Kind = CR[updateid].m_cltCharStatus.GetKind();
	//ptemporder->IDData[1]			= CR[updateid].m_cltCharStatus.GetKind();
	memcpy(&ptemporder->IDData[1], &Kind, sizeof(UI16));

	// ĳ���� ü���� ������. 
	UI16 life = CR[updateid].m_ParmeterBox.IP.GetLife();
	memcpy(&ptemporder->IDData[3], &life, sizeof(UI16));

	// ĳ���� ������ ������. 
	UI16 mana = CR[updateid].m_ParmeterBox.IP.GetMana();
	memcpy(&ptemporder->IDData[5], &mana, sizeof(UI16));

	// ĳ���� ��ġ�� ������. 
	ptemporder->IDData[7]			= CR[updateid].GetX();
	ptemporder->IDData[8]			= CR[updateid].GetY();
	
	// �� ĳ���͸� ����ĳ����(killer) ���̵� ������. 
	ptemporder->IDData[9]			= killerid;

	return TRUE;
}

void _clBattle::PushUpdateCharInfo(SI32 id, SI32 killerid)
{

	SI32 i;

	if(UpdateCharInfoDataIndex >= (MAX_UPDATRE_CHAR_INFO_NUMBER-1))return ;

	_OrderData temporder;
	
	// �̹� �ش� ĳ������ ����� ����Ǿ� �ִ� ���¶�� �ٽ� �� �����Ѵ�. 
	for(i = 0; i < UpdateCharInfoDataIndex;i++)
	{
		memcpy(&temporder, UpdateCharInfoData[i], sizeof(_OrderData));

		// ���� ĳ������ ������ ��� �ִ�. 
		if(temporder.IDData[0] == id)
		{
			if(UpdateCharInfo(id,  killerid, &temporder) == TRUE)
			{
				// ���� �� ������. 
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


	// �ش� ĳ������ ���̵� ã�´�. 
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
	
	// �ش� ĳ������ ������ ������Ʈ �Ѵ�.
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

	// ĳ���Ͱ� ��ȿ�ϰ� 
	if(GD.CharInfo[id])
	{
		// ���� ������ ĳ�����̸�, 
		if(CR[id].m_cltCharStatus.GetKind() == kind)
		{
			// ų���� ���� �����ϴ� ĳ���Ͷ��, 
			if(GD.CharInfo[killerid])
			{
				if(CR[killerid].GetPlayerNumber() == UserPlayerNumber
					|| ( ISMONSTER(MyNet.PlayersdpId.dpId[CR[killerid].GetPlayerNumber()] ) &&  MyNet.IsHost()))
				{
					
					// ������ ���� �������� �׿����� �뺸�Ѵ�. 

					// �������� �ƴϾ�� �Ѵ�. 
					if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
					{
						OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
							MyNet.PlayersdpId.dpId[ CR[killerid].GetPlayerNumber()],	CR[killerid].m_siOnlineID, 
							MyNet.PlayersdpId.dpId[ CR[id].GetPlayerNumber()],		CR[id].m_siOnlineID);
						
						// ų������ �մ��� ����ġ���� �����Ѵ�. 
						pGame->pBattle->ExpOperate(killerid, id);
					}
				}
				
			}

			//������Ʈ �ؾ��� ĳ���Ͱ� ���� ���� ĳ���Ͷ�� ������Ʈ ���� �ʴ´�. 
			// ������Ʈ �ϰ� �Ǹ� 2������ �����ϴ¼�. 
			if(CR[id].GetPlayerNumber() == UserPlayerNumber
			|| ( ISMONSTER(MyNet.PlayersdpId.dpId[CR[id].GetPlayerNumber()] ) &&  MyNet.IsHost()))
			{
			}
			else
			{

				CR[id].m_ParmeterBox.IP.SetLife(life);
				CR[id].m_ParmeterBox.IP.SetMana(mana);
				
				SI32 length = max( abs(CR[id].m_ssX - posx), abs(CR[id].m_ssY - posy));
				
				// �Ÿ��� ���� ������ ������ �����Ѵ�. 
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
		// �ٸ� ������ ĳ���Ͷ��, ���δ�.(�ٽ� ����� ���ؼ�..)
		else
		{
			CR[id].m_ParmeterBox.IP.SetLife(0);
		}
	}
	// ĳ���Ͱ� ��ȿ���� ������, �����. 
	else
	{
		// �¸��� �й赵 �ƴ� ��쿡��, 
		// �¸��� �й��� ��쿡�� ĳ���Ͱ� ������� ������...
		// �̹� ��ȿȭ�� ������� ��쿡�� ĳ���͸� ������ �ȵȴ�. 
		if(GD.VictoryDelay == 0 && GD.DefeatDelay == 0 && MyNet.PlayersdpId.dpId[playernumber])
		{
			// ���� ���� �����ϴ� ĳ���ʹ� �ٽ� ����� �ȵȴ�. 
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
				
				// ĳ���͸� �����Ѵ�. 
				SetChar(kind, id, posx, posy, SOUTH, 100, playernumber, &ip, NULL, -1);

			}
		}

	}

}


void OrderOperateAlly(_OrderData order)
{
	// i�� pnumber���� ���� ���� �����ϱ�
	
	SI32 i			= order.PlayerNumber;
	SI32 pnumber	= order.ODI.Para1;
	SI32 k, m;
	char buffer[128];

	
	// ������ ����.
	if(GD.OnlineAllyNumSwitch[i][pnumber] == TRUE)
	{
		GD.OnlineAllyNumSwitch[i][pnumber] = FALSE;
		
		// actdoll (2004/09/17 11:22) : ���� ���� �޽���
		wsprintf(buffer, Text.Msg_About_Destroy_Alliance.Get(), MyNet.PlayersName.Name[i], MyNet.PlayersName.Name[pnumber]);
		SetMessage(buffer);
		
		// EnemyBit�� �����Ѵ�. 
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
	// ������ �δ´�. 
	else
	{
		
		GD.OnlineAllyNumSwitch[i][pnumber] = TRUE;
		
		// actdoll (2004/09/17 11:22) : ���� ��û �޽���
		wsprintf(buffer, Text.Msg_About_Request_Alliance.Get(), MyNet.PlayersName.Name[i], MyNet.PlayersName.Name[pnumber]);
		SetMessage(buffer);
		
		// ������ �ξ��ִ� �Լ��� ȣ���Ѵ�. 
		// ��밡 �̹� ������ ������ ��û�� ���¿��� �Ѵ�. 
		if(GD.OnlineAllyNumSwitch[pnumber][i] == TRUE)
		{
			
			// EnemyBit�� �����Ѵ�. 
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

	DWriteStringToFile("��������");
	SI32 pnumber = order.PlayerNumber;

	// �������� ����ڰ� ���� ȣ��Ʈ���ٸ�, 
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
// robypark (2004/05/17 15:42) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
// ����������(����-�ź���, �̼��� 2�� ���� ���)
BOOL OrderFIRE_ROUND_DANCE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FIRE_ROUND_DANCE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// ����(����-������, ���� 2�� ���� ���)
BOOL OrderNOVA( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_NOVA, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
BOOL OrderFURY_OF_VOLCANO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FURY_OF_VOLCANO, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
BOOL OrderPOISONING_HEAVENS( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_POISONING_HEAVENS, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
BOOL OrderBOMB_OF_HEAVEN( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_BOMB_OF_HEAVEN, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
BOOL OrderSIEGE_MODE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_SIEGE_MODE, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
BOOL OrderONSLAUGHT_FIREBALL( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_ONSLAUGHT_FIREBALL, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
BOOL OrderFIRERAIN_OF_GOD( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_FIRERAIN_OF_GOD, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}

// robypark 2004/6/4 14:3
// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
BOOL OrderSIEGE_MODE_UNDO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host )
{
	DoOrder(ORDER_SIEGE_MODE_UNDO, id, (x|(y<<16)), attackid, host, FALSE);
	return TRUE;
}
