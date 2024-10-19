#ifndef _ORDER_H
#define _ORDER_H      1

#include <main.h>

#define ORDERATB_NORMAL					1
#define ORDERATB_CHEAT					2
#define ORDERATB_BUILD					4      // �ǹ��� ���� ����̴�. 
#define ORDERATB_TRAIN					8      // ���縦 �����ϴ� ����̴�. 
#define ORDERATB_MAGIC					16
#define ORDERATB_UPGRADE				32
#define ORDERATB_UPDATEMENU				64
#define ORDERATB_GUPDATEMENU			128
#define ORDERATB_ONLY					256
#define ORDERATB_GOODS					512


#define ORDER_WAIT    					1
#define ORDER_STOP    					2 
#define ORDER_MOVE    					3		// �ܼ��̵� 
#define ORDER_ATTACK  					4		// �ܼ� ���� 
#define ORDER_ATTACKGROUND  			5		// ���� ���� 
#define ORDER_DIE     					6
#define ORDER_DEAD    					7
#define ORDER_HARVEST					8
#define ORDER_EXTINGUISH				9		// �Ҳ���
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
#define ORDER_CAPTUREBUILD 				34		// ���� �ǹ��� �����Ѵ�. 
#define ORDER_PATROL     				35
#define ORDER_CONVOY     				36
#define ORDER_HEAL       				37
#define ORDER_MASSHEAL   				38
#define ORDER_HOLD       				39
#define ORDER_BETRAY     				40
#define ORDER_TAKEIN     				41		// �¿�� 
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
#define ORDER_GOINTOHQ      			74		// ����� ������ �ִ´�. 
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

// �ڵ尪 ����. 
//-------------------------------------------------
#define ORDER_QUIT						86			// ��Ʈ��ũ ������ ����ڰ� ������ �˷��ش�. 
#define ORDER_GETITEM					87			// ��Ʈ��ũ ������ ����ǰ�� ������� �˷��ش�.
//-------------------------------------------------
#define ORDER_ENEMYCLON		 			88			// ���� �����Ѵ�.
#define ORDER_UPDATECHAR				89			// ĳ������ ������ �ٸ� ����ڿ��� ������Ʈ ��Ű�� ����̴�. 
#define ORDER_MAGIC4T					90
#define ORDER_MAGIC2T					91
///////////////////////////////////////////////
#define ORDER_MAGICTEST					92
///////////////////////////////////////////////
#define ORDER_RECALL_PANDA				93			//�Ҵ� ��ȯ ����
#define ORDER_RECALL_MONKEY				94			//Ȳ�� ��ȯ ����
#define ORDER_RECALL_AMAKUSA			95			//�����,õ�� ��ȯ ����
#define ORDER_ICE						96
#define ORDER_SOUL						97			//�߼��� ȥ
#define ORDER_GHOST						98			//�ͽ�ź
#define ORDER_DARKFIRE					99			//����ȭ����ǳ
#define ORDER_IVORY						100			//�������
#define ORDER_BLUEDRAGON				101			//���û�浵
#define ORDER_RUNNINGFIRE				102			//������ź
#define ORDER_ENERGYPA					103			//��ǳ
#define ORDER_SNOWATTACK				104			//��������
#define ORDER_FIREATTACK				105			//ȭ������
#define ORDER_FIREENERGYPA				106			//��ȭ��ǳ(���Ȳ���)
#define	ORDER_RECALL_TIGER				107			//����ȣ���̼�ȯ
#define ORDER_MANA						108
#define ORDER_DOUBLE_SWORD				109			// ��Ͻ� �ְ˳���
#define ORDER_LONG_SPEAR				110			// ����â ��â��ȯ
#define ORDER_ICE_BARRIER				111			// ���� ������

#define ORDER_FLOWER_NEEDLE_RAIN		112			// ��õȭ��
#define ORDER_SOUL_CHANGE				113			// ���Ǽ�
#define ORDER_STONE_MAGIC				114			// ������
#define ORDER_GOD_SPEAR					115			// ��â�뵵
#define ORDER_CLOUD_WIND				116			// ǳ���
#define ORDER_MANABURN					117			// ������.
#define ORDER_RECALL_GUARDIAN_MOUSE		118			// ��ȣ��(��)
#define ORDER_GUARDIAN_RABBIT			119			// ��ȣ��(�䳢)
#define ORDER_RECALL_GUARDIAN_CAW		120			// ��ȣ��(��)
#define ORDER_GUARDIAN_DRAGON			121			// ��ȣ��(��)
#define ORDER_GUARDIAN_TIGER			122			// ��ȣ��(Ÿ�̰�)
#define ORDER_GUARDIAN_HORSE			123			// ��ȣ��(��)
#define ORDER_GUARDIAN_SHEEP			124			// ��ȣ��(��)

#define ORDER_RECALL_DAMDUK				130			// ��������� ��ȯ


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
// robypark (2004/05/17 15:31) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define ORDER_FIRE_ROUND_DANCE			173		// ����������(����-�ź���, �̼��� 2�� ���� ���)
#define ORDER_NOVA						174		// ����(����-������, ���� 2�� ���� ���)
#define ORDER_FURY_OF_VOLCANO			175		// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
#define ORDER_POISONING_HEAVENS			176		// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define ORDER_BOMB_OF_HEAVEN			177		// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
#define ORDER_SIEGE_MODE				178		// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
#define ORDER_ONSLAUGHT_FIREBALL		179		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
#define ORDER_FIRERAIN_OF_GOD			180		// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
#define ORDER_DIE_GENERALEX				181

// robypark 2004/6/4 14:2
#define ORDER_SIEGE_MODE_UNDO			182		// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)

#define MAX_ORDER_NUMBER				200

		
#define BY_PLAYER     0		
#define BY_COMPUTER   1

// ��Ʈ ���� 
#define ORDERINFO_NORETREAT     1   // ��� �����߿��� �������� �ʴ´�. 
#define ORDERINFO_SAMEPRIORITY  2   // ���� ��� �켱������ ����� �������϶����� ��ȿ�ϴ�. 
#define ORDERINFO_LOWPRIORITY   4   // ���� ��� �켱������ ����� �������϶����� ��ȿ�ϴ�.

// ��� �켱���� 
#define ORDERPRIORITY_0         0
#define ORDERPRIORITY_1         1
#define ORDERPRIORITY_2         2
#define ORDERPRIORITY_3         3
#define ORDERPRIORITY_4         4
#define ORDERPRIORITY_5         5
#define ORDERPRIORITY_6         6
#define ORDERPRIORITY_7         7
#define ORDERPRIORITY_LAST      8

// _OrderData �� �ٽ� ���� 
class _OrderDataInfo{
public:
	SHORT Order;               // �����ؾ� �� ��� �ڵ� 
	char HostMode;                // ����� ���� ��ü (�÷��̾� �Ǵ� ��ǻ�� )
	char ClearSwitch;           // ����� ���޹��� ĳ������ ������ ���۵��� ������ΰ�?
	                            //  TRUE : ���� ����� �����.        -> ��� ����� �����Ѵ�. 
	                            // FALSE : ���� ����� ������ �ʴ´�. ->  ����� ���� ���ۿ� �ִ´�.  

	int Para1;
	_NewID Para2;        // ��ɿ� ���� ���� �Ķ��Ÿ 

};

// ����� ��ü 
// ��ǻ�Ͱ� ������ �����ϴ� ����� ������ 
class _OrderData {
public:
    _OrderDataInfo ODI;

	SI08 PlayerNumber;                  // ����� ������ �÷��̾� ��ȣ 
	SI08 CharNumber;                    // ����� �����ؾ� �� ĳ������ ���� 
	UI08 IDData[MAX_SELECTED_CHAR_NUMBER];  // ����� �����ؾ� �� ĳ������ ���̵�  (f:unique fff:id)
};

class _Order {
public:
	SHORT Atb;
	SHORT Object;
	int Para1, Para2;

	int Info;
	SHORT Priority;      // ��ɼ����� �켱���� 

	SI32 siNeedKind;	// ������ �Ǵ� ����. 	


	// ����� ���� ������ �����Ѵ�. 
    void SetOrder(SHORT atb, SHORT object, int info, SHORT priority, int para1, int para2);
	// ����� �Ӽ��� ���Ѵ�. 
    BOOL IsAtb(SHORT atb);
	int GetPara1();
	int GetPara2();


};

void OrderInit();


// ����� orderbuffer�� �־� ���´�. 
BOOL MakeOrder(SHORT player_number, SHORT order, int para1, _NewID para2, BOOL clearswitch);
BOOL PushOrder(_OrderData order);
BOOL PopOrder();

// �÷��̾��� ����� �����Ѵ�. 
BOOL LetOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch);

BOOL DoOrder(SHORT order, _NewID id, int para1, _NewID para2, char host, BOOL clearswitch);

void OrderStop(_NewID id, int hostmode);
void OrderHold(_NewID id, int hostmode);
void OrderBuildToCar(_NewID id, int hostmode);
// ĳ���͸� �̵���Ų��. 
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
BOOL OrderReCallDamduk(_NewID id, char host);		//��ȯ ����
BOOL OrderReCallPanda(_NewID id, char host);		//�Ҵ� ��ȯ.
BOOL OrderReCallMonkey(_NewID id,char host);		//Ȳ�� ��ȯ.
BOOL OrderReCallAmakusa(_NewID id,char host);		//����� ��ȯ.
BOOL OrderReCallTiger(_NewID id,char host);			//����ȣ���� ��ȯ.
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

// �Ӽ��� �Ķ��Ÿ�� ������ �ش� ����� ã�´�.
SHORT FindOrder(SHORT atb, SHORT para1,  SHORT para2);
BOOL UpdateCharInfo(_OrderData *ptemporder);

//----------------------------------------------------------------------------
// robypark (2004/05/17 15:42) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
// ����������(����-�ź���, �̼��� 2�� ���� ���)
BOOL OrderFIRE_ROUND_DANCE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// ����(����-������, ���� 2�� ���� ���)
BOOL OrderNOVA( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
BOOL OrderFURY_OF_VOLCANO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
BOOL OrderPOISONING_HEAVENS( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
BOOL OrderBOMB_OF_HEAVEN( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
BOOL OrderSIEGE_MODE( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
BOOL OrderONSLAUGHT_FIREBALL( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
BOOL OrderFIRERAIN_OF_GOD( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

// robypark 2004/6/4 14:1
// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
BOOL OrderSIEGE_MODE_UNDO( _NewID id, SHORT x, SHORT y, _NewID attackid, char host );

#endif
