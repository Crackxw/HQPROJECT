#ifndef _ORDER_H
#define _ORDER_H      1

#include <main.h>

#define ORDERATB_NORMAL					1
#define ORDERATB_CHEAT					2
#define ORDERATB_BUILD					4      // 건물을 짓는 명령이다. 
#define ORDERATB_TRAIN					8      // 병사를 생산하는 명령이다. 
#define ORDERATB_MAGIC					16
#define ORDERATB_UPGRADE				32
#define ORDERATB_UPDATEMENU				64
#define ORDERATB_GUPDATEMENU			128
#define ORDERATB_ONLY					256
#define ORDERATB_GOODS					512


#define ORDER_WAIT    					1
#define ORDER_STOP    					2 
#define ORDER_MOVE    					3		// 단순이동 
#define ORDER_ATTACK  					4		// 단순 공격 
#define ORDER_ATTACKGROUND  			5		// 지상 공격 
#define ORDER_DIE     					6
#define ORDER_DEAD    					7
#define ORDER_HARVEST					8
#define ORDER_EXTINGUISH				9		// 불끄기
#define ORDER_BUILD   					11
#define ORDER_MAKE    					12
#define ORDER_SELL    					13
#define ORDER_SELLMENU 					14
#define ORDER_TRAIN   					15
#define ORDER_REPAIR  					16
#define ORDER_BUILD1  					17
#define ORDER_BUILD2  					18
#define ORDER_INITMENU					19
#define ORDER_INITGMENU					20
#define ORDER_TRAINSPOT  				21
#define ORDER_CARCASS      				22
#define ORDER_RIDEOUT      				23
#define ORDER_RIDEOUTONLY0  			24
#define ORDER_RIDEOUTONLY1  			25
#define ORDER_RIDEOUTONLY2  			26
#define ORDER_RIDEOUTONLY3  			27
#define ORDER_RIDEOUTONLY4  			28
#define ORDER_RIDEOUTONLY5  			29
#define ORDER_RIDEOUTONLY6  			30
#define ORDER_RIDEOUTONLY7  			31
#define ORDER_RIDE     					32
#define ORDER_HELPBUILD  				33
#define ORDER_CAPTUREBUILD 				34		// 적의 건물을 점령한다. 
#define ORDER_PATROL     				35
#define ORDER_CONVOY     				36
#define ORDER_HEAL       				37
#define ORDER_MASSHEAL   				38
#define ORDER_HOLD       				39
#define ORDER_BETRAY     				40
#define ORDER_TAKEIN     				41		// 태우기 
#define ORDER_CHINA_BARRACK 			42
#define ORDER_CHINA_HEAVYMAKER 			43
#define ORDER_MOVERIDEOUT  				44
#define ORDER_SELFEXP      				45
#define ORDER_EARTHQUAKE   				46
#define ORDER_YELLOWSAND				47
#define ORDER_TORNADO      				48
#define ORDER_UPGRADE      				49
#define ORDER_ABSORB       				50
#define ORDER_MAGIC8J	   				51
#define ORDER_LIGHTNING    				52
#define ORDER_SHIELD       				53
#define ORDER_CLON         				54
#define ORDER_WARP         				55
#define ORDER_SELLCROP     				56
#define ORDER_SELLTREE     				57
#define ORDER_NEWMENU_MARKET			58
#define ORDER_STEALTECH					59
#define ORDER_RAINMAKE     				60
#define ORDER_USEBUDA      				61
#define ORDER_AIRMAGIC     				62
#define ORDER_KIMBOMB      				63
#define ORDER_MAGIC5J      				64
#define ORDER_STEALEXP     				65
#define ORDER_AUTOMAGIC    				66
#define ORDER_NOAUTOMAGIC  				67
#define ORDER_GENERALFIRST 				68
#define ORDER_NOGENERALFIRST  			69
#define ORDER_MINE          			70
#define ORDER_GOODS         			71
#define ORDER_FARMERTOBULL  			72
#define ORDER_MOVENRIDEOUTNATTACK		73
#define ORDER_GOINTOHQ      			74		// 깃발을 본영에 넣는다. 
#define ORDER_GROUNDEXP     			75
#define ORDER_MAGIC4C	    			76
#define ORDER_MAGIC6C	    			77
#define ORDER_BUILDTOCAR    			78
#define ORDER_SHOWSTORAGE				79
#define ORDER_USEPOTION					80
#define ORDER_AMBUSH					81
#define ORDER_SELLITEM					82
#define ORDER_DETECTMINE				83
#define ORDER_REQUESTALLY				84
#define ORDER_ALLY						85

// 코드값 고정. 
//-------------------------------------------------
#define ORDER_QUIT						86			// 네트워크 게임중 사용자가 나감을 알려준다. 
#define ORDER_GETITEM					87			// 네트워크 게임중 전리품을 얻었음을 알려준다.
//-------------------------------------------------
#define ORDER_ENEMYCLON		 			88			// 적을 복제한다.
#define ORDER_UPDATECHAR				89			// 캐릭터의 정보를 다른 사용자에게 업데이트 시키는 명령이다. 
#define ORDER_MAGIC4T					90
#define ORDER_MAGIC2T					91
///////////////////////////////////////////////
#define ORDER_MAGICTEST					92
///////////////////////////////////////////////
#define ORDER_RECALL_PANDA				93			//팬더 소환 마법
#define ORDER_RECALL_MONKEY				94			//황룡 소환 마법
#define ORDER_RECALL_AMAKUSA			95			//요술사,천구 소환 마법
#define ORDER_ICE						96
#define ORDER_SOUL						97			//야수의 혼
#define ORDER_GHOST						98			//귀신탄
#define ORDER_DARKFIRE					99			//암흑화염폭풍
#define ORDER_IVORY						100			//상아폭격
#define ORDER_BLUEDRAGON				101			//비격청룡도
#define ORDER_RUNNINGFIRE				102			//연발포탄
#define ORDER_ENERGYPA					103			//장풍
#define ORDER_SNOWATTACK				104			//설공신포
#define ORDER_FIREATTACK				105			//화공신포
#define ORDER_FIREENERGYPA				106			//열화장풍(비격황룡권)
#define	ORDER_RECALL_TIGER				107			//졸개호랑이소환
#define ORDER_MANA						108
#define ORDER_DOUBLE_SWORD				109			// 고니시 쌍검난무
#define ORDER_LONG_SPEAR				110			// 손유창 장창소환
#define ORDER_ICE_BARRIER				111			// 순비연 빙석술

#define ORDER_FLOWER_NEEDLE_RAIN		112			// 만천화우
#define ORDER_SOUL_CHANGE				113			// 빙의술
#define ORDER_STONE_MAGIC				114			// 석괴술
#define ORDER_GOD_SPEAR					115			// 신창노도
#define ORDER_CLOUD_WIND				116			// 풍모술
#define ORDER_MANABURN					117			// 마나번.
#define ORDER_RECALL_GUARDIAN_MOUSE		118			// 수호부(쥐)
#define ORDER_GUARDIAN_RABBIT			119			// 수호부(토끼)
#define ORDER_RECALL_GUARDIAN_CAW		120			// 수호부(소)
#define ORDER_GUARDIAN_DRAGON			121			// 수호부(용)
#define ORDER_GUARDIAN_TIGER			122			// 수호부(타이거)
#define ORDER_GUARDIAN_HORSE			123			// 수호부(말)
#define ORDER_GUARDIAN_SHEEP			124			// 수호부(양)

#define ORDER_RECALL_DAMDUK				130			// 모든종류의 소환


#define ORDER_WEAPONOFF					150
#define ORDER_WEAPONON					151
#define ORDER_ARMOUROFF					152
#define ORDER_ARMOURON					153
#define ORDER_ETCOFF					154
#define ORDER_ETCON						155
#define ORDER_POTIONON					156
#define ORDER_POTIONOFF					157
#define ORDER_CHEAT_SEE					158	
#define ORDER_CHEAT_STATUS				159	
#define ORDER_CHEAT_SPEED				160
#define ORDER_CHEAT_MONEY				161
#define ORDER_CHEAT_RAIN				162
#define ORDER_CHEAT_UPGRADE				163
#define ORDER_CHEAT_LEVEL				164
#define ORDER_CHEAT_MERCHANT			165
#define ORDER_CHEAT_GOODS				166
#define ORDER_CHEAT_WIN					167
#define ORDER_CHEAT_LOSE				168 
#define ORDER_CHEAT_ATTACKINFO			169 
#define ORDER_CHEAT_GOD					170 
#define ORDER_CHEAT_MAP					171 
#define ORDER_CHEAT_JUSTICEK			172 

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:31) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define ORDER_FIRE_ROUND_DANCE			173		// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
#define ORDER_NOVA						174		// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
#define ORDER_FURY_OF_VOLCANO			175		// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
#define ORDER_POISONING_HEAVENS			176		// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
#define ORDER_BOMB_OF_HEAVEN			177		// 승천포(중국-발석거, 순비연 2차 전직 기술)
#define ORDER_SIEGE_MODE				178		// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
#define ORDER_ONSLAUGHT_FIREBALL		179		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
#define ORDER_FIRERAIN_OF_GOD			180		// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
#define ORDER_DIE_GENERALEX				181

// robypark 2004/6/4 14:2
#define ORDER_SIEGE_MODE_UNDO			182		// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령 2차 전직 기술)

#define MAX_ORDER_NUMBER				200

		
#define BY_PLAYER     0		
#define BY_COMPUTER   1

// 비트 단위 
#define ORDERINFO_NORETREAT     1   // 명령 수행중에는 후퇴하지 않는다. 
#define ORDERINFO_SAMEPRIORITY  2   // 같은 명령 우선순위의 명령이 실행중일때에도 유효하다. 
#define ORDERINFO_LOWPRIORITY   4   // 같은 명령 우선순위의 명령이 실행중일때에는 무효하다.

// 명령 우선순위 
#define ORDERPRIORITY_0         0
#define ORDERPRIORITY_1         1
#define ORDERPRIORITY_2         2
#define ORDERPRIORITY_3         3
#define ORDERPRIORITY_4         4
#define ORDERPRIORITY_5         5
#define ORDERPRIORITY_6         6
#define ORDERPRIORITY_7         7
#define ORDERPRIORITY_LAST      8

// _OrderData 의 핵심 정보 
class _OrderDataInfo{
public:
	SHORT Order;               // 수행해야 할 명령 코드 
	char HostMode;                // 명령을 내린 주체 (플레이어 또는 컴퓨터 )
	char ClearSwitch;           // 명령을 전달받은 캐릭터의 예약명령 버퍼들을 지울것인가?
	                            //  TRUE : 예약 명령을 지운다.        -> 즉시 명령을 실행한다. 
	                            // FALSE : 예약 명령을 지우지 않는다. ->  명령을 예약 버퍼에 넣는다.  

	int Para1;
	_NewID Para2;        // 명령에 딸려 가는 파라메타 

};

// 명령의 실체 
// 컴퓨터가 실제로 전송하는 명령의 데이터 
class _OrderData {
public:
    _OrderDataInfo ODI;

	SI08 PlayerNumber;                  // 명령을 수행할 플레이어 번호 
	SI08 CharNumber;                    // 명령을 수행해야 할 캐릭터의 갯수 
	UI08 IDData[MAX_SELECTED_CHAR_NUMBER];  // 명령을 수행해야 할 캐릭터의 아이디  (f:unique fff:id)
};

class _Order {
public:
	SHORT Atb;
	SHORT Object;
	int Para1, Para2;

	int Info;
	SHORT Priority;      // 명령수행의 우선순위 

	SI32 siNeedKind;	// 전제가 되는 유닛. 	


	// 명령의 여러 성질을 설정한다. 
    void SetOrder(SHORT atb, SHORT object, int info, SHORT priority, int para1, int para2);
	// 명령의 속성을 구한다. 
    BOOL IsAtb(SHORT atb);
	int GetPara1();
	int GetPara2();


};

void OrderInit();


// 명령을 orderbuffer에 넣어 놓는다. 
BOOL MakeOrder(SHORT player_number, SHORT order, int para1, _NewID para2, BOOL clearswitch);
BOOL PushOrder(_OrderData order);
BOOL PopOrder();

// 플레이어의 명령을 전달한다. 
BOOL LetOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch);

BOOL DoOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch);

void OrderStop(_NewID id, int hostmode);
void OrderHold(_NewID id, int hostmode);
void OrderBuildToCar(_NewID id, int hostmode);
// 캐릭터를 이동시킨다. 
void OrderMove(_NewID id, int x, int y, int hostmode);
BOOL OrderAttack(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderCapture(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderBetray(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderAbsorb(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMagic8j(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderStealTech(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderEarthQuake(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderTornado(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderYellowSand(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderSoul(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderGhost(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderDarkFire(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderLightning(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderHarvest(_NewID id, SHORT x, SHORT y, char host);
BOOL OrderExtinguish(_NewID id, SHORT x, SHORT y, char host);
BOOL OrderUseBuda(_NewID id, char host);
BOOL OrderHeal(_NewID id, _NewID targetid, char host);
BOOL OrderMana(_NewID id, _NewID targetid, char host);
BOOL OrderShield(_NewID id, _NewID targetid, char host);
BOOL OrderRepair(_NewID id, char host);
BOOL OrderSell(_NewID id, char host);
BOOL OrderClon(_NewID id, char host);
BOOL OrderGuardianRabbit(_NewID id,char host);
BOOL OrderRecallGuardianMouse(_NewID id,char host);
BOOL OrderRecallGuardianCaw(_NewID id,char host,SHORT kind);
BOOL OrderGuardianDragon(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderGuardianTiger(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderGuardianHorse(_NewID id,char host);
BOOL OrderGuardianSheep(_NewID id,char host);
BOOL OrderReCallDamduk(_NewID id, char host);		//소환 공통
BOOL OrderReCallPanda(_NewID id, char host);		//팬더 소환.
BOOL OrderReCallMonkey(_NewID id,char host);		//황룡 소환.
BOOL OrderReCallAmakusa(_NewID id,char host);		//요술사 소환.
BOOL OrderReCallTiger(_NewID id,char host);			//졸개호랑이 소환.
BOOL OrderEnemyClon(_NewID newid, _NewID enemynewid, char host);
BOOL OrderTakeIn(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMoveRideOut(_NewID id, int x, int y, char host);
BOOL OrderSelfExp(_NewID id, SHORT x, SHORT y, char host);
BOOL OrderRide(_NewID id, SHORT x, SHORT y, _NewID vehicle, char host);
BOOL OrderRideOut(_NewID id, char host);
BOOL OrderConvoy(_NewID id, _NewID targetid, char host);
BOOL OrderStealExp(_NewID id, char host);
BOOL OrderMine(_NewID id, SHORT x, SHORT y, char host);
BOOL OrderAirMagic(_NewID id, char host);
BOOL OrderKimBomb(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMagic5j(_NewID id, char host);
BOOL OrderWarp(_NewID id, char host);
BOOL OrderFarmerToBull(_NewID id, char host);
BOOL OrderMoveNRideOutNAttack(_NewID, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderRainMake(_NewID id, char host);
BOOL OrderGroundExp(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMagic4c(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMagicTest(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderMagic6c(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderIce(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderLongSpear(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);
BOOL OrderManaburn(_NewID id, SHORT x, SHORT y, _NewID attackid, char host);

BOOL OrderUpgrade(_NewID id, SHORT upgradekind, char host);
BOOL OrderTrain(_NewID id, SHORT kind, char host);
BOOL OrderBuild(_NewID id, SHORT kind, SHORT x, SHORT y, char host);
BOOL OrderIceBarrier( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );
BOOL OrderStoneMagic( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 속성과 파라메타를 가지고 해당 명령을 찾는다.
SHORT FindOrder(SHORT atb, SHORT para1,  SHORT para2);
BOOL UpdateCharInfo(_OrderData *ptemporder);

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:42) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
BOOL OrderFIRE_ROUND_DANCE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
BOOL OrderNOVA( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
BOOL OrderFURY_OF_VOLCANO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
BOOL OrderPOISONING_HEAVENS( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 승천포(중국-발석거, 순비연 2차 전직 기술)
BOOL OrderBOMB_OF_HEAVEN( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
BOOL OrderSIEGE_MODE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
BOOL OrderONSLAUGHT_FIREBALL( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
BOOL OrderFIRERAIN_OF_GOD( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// robypark 2004/6/4 14:1
// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령 2차 전직 기술)
BOOL OrderSIEGE_MODE_UNDO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

#endif
