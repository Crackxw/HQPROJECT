#ifndef	_GSCDEFINE_HEADER
#define	_GSCDEFINE_HEADER

typedef			__int64		MONEY;		//	DB�� ��,����ϴ°� ���� �̰� ���� �� ��. �����  %I64d
typedef			MONEY		VDEF;		// Village Defence

// ID �ο� (�� 65535���� ID�� �Ҵ� �Ǿ� �ִ�.)
// ���������� �÷��̾� ����. (1 ~ 32767)
// actdoll (2005/02/23 23:17) : ������ �� �ִ� �÷��̾� ī��Ʈ�� �ִ밪
//
//	!!!!!!!!!!! ���� !!!!!!!!!!
//
//	�÷��̾ �Ҵ�Ǵ� ���۸� �ٿ� �۾� �ӵ��� ������ �ϱ� ���� �ַ� ���� �۾��ڰ� ���� �ǵ帮�� �κ��̴�.
//	�ϴ� �ӽ� �������� �������� ���������� ���� �����ÿ��� ������ MAX_PLAYER_NUM ���� �ݵ�� ��ġ�ؾ� �Ѵ�.
//	���� ���� Ʋ���� �� ��� �߸��ϸ� ġ������ ����(Ŭ���̾�Ʈ ĳ���Ͱ� �α��� �� �� ȭ���� �˰� ���ϴ� ���� ����)�� �߻��Ѵ�.
//	(ĳ���� �ʱ�ȭ�� (RESPONSE_LOGIN) �κп��� �߸��� ������ �Ѿ���� �Ѵ�.)
//	�ݵ�� ���������� ���� ��Ȳ�� �߻��� ���� ������ Ŭ���̾�Ʈ�� �� ���� ��ġ�ؾ� �Ѵ�.
//
// "Define.h" ���� ON_MAX_CONNECTION �� ���� ������ ������ �־�� �Ѵ�.
#ifdef	_IMJINONLINESERVER		
//	#define	MAX_PLAYER_NUM				20//32768			// ���� �κ��̹Ƿ� ���� �۾��ڵ��� ���⸸ �ǵ帱 ��! 
    #define	MAX_PLAYER_NUM				6000
#else//!_IMJINONLINESERVER
	#define	MAX_PLAYER_NUM				6000//32768			// Ŭ���̾�Ʈ �κ�. Ŭ���̾�Ʈ �۾��ڰ� ���� �ǵ帱 ���� ����. �������� ����!
#endif//_IMJINONLINESERVER

#define	PLAYER_START_ACCOUNT		1

// ���������� �� ����.(32768 ~ 33767) 
#define	MAX_SHIP_NUM		1000
#define	SHIP_START_ACCOUNT		(UI32(32768))


// ������ ���� �ڵ� CHAR ��
#define NATION_CODE_KOREA			'K'
#define NATION_CODE_CHINA			'C'
#define NATION_CODE_JAPAN			'J'
#define	NATION_CODE_TAIWAN			'T'

// ���������� ���� ����. (33768 ~ 65535)
#define	MAX_MONSTER_NUM			(UI32(31768))
#define	MONSTER_START_ACCOUNT	(UI32(33768))

// Account�� �̿��Ͽ� Kind�� ���´�.
#define	ISPLAYER(x)				(((x >= PLAYER_START_ACCOUNT) && (x <= (PLAYER_START_ACCOUNT + MAX_PLAYER_NUM - 1))) ? TRUE : FALSE)
#define	ISSHIP(x)				(((x >= SHIP_START_ACCOUNT) && (x <= (SHIP_START_ACCOUNT + MAX_SHIP_NUM - 1))) ? TRUE : FALSE)
#define	ISMONSTER(x)			(((x >= MONSTER_START_ACCOUNT) && (x <= (MONSTER_START_ACCOUNT + MAX_MONSTER_NUM - 1))) ? TRUE : FALSE)

// ������ 0�� ������ �� ������ Error����
#define	UI32_ERROR				0xffffffff
#define	UI16_ERROR				0xffff
#define UI08_ERROR				0xff


// �ִ� ������
//#define ON_MAX_MONEY				200000000		// 2��
#define ON_MAX_MONEY				500000000		// 5��
// �ִ� �����
//#define ON_MAX_SAVING				5000000000
#define ON_MAX_SAVING				10000000000		// 100�� 

// �� ĳ���Ͱ� ���� �� �ִ� �ִ� ������ ��.
#define	ON_MAX_FOLLOWER_NUM			12

// ĳ������ �ʱ� ���ʽ�
#define	ON_CHARACTER_INITBONUS		10		//	ĳ������ �ʱ� ���ʽ� ��

// �� ��� �ִ� ���� �� �ִ� �������� ��.
#define	ON_MYDATA_MAX_ITEM_NUM		10		// ��ǰ �� �������� ���� ��ġ�̴�.

// ����� ���
enum	UserGrade{ON_USERGRADE_NORMAL, ON_USERGRADE_ADMIN1, ON_USERGRADE_ADMIN2, ON_USERGRADE_ADMIN3, ON_USERGRADE_ADMIN4, ON_USERGRADE_ADMIN5};

// ���� ����.
#define	ON_EAST			1
#define	ON_WEST			2
#define	ON_NORTH		4
#define	ON_SOUTH		8

// ���忡�� �Ĵ� �ִ� �������� ���� ����.
//#define	ON_MAX_MARKETITEM		20

// ����. 
#define	ON_COUNTRY_NONE		0
#define	ON_COUNTRY_KOREA	1
#define	ON_COUNTRY_JAPAN	2
#define	ON_COUNTRY_CHINA	3
#define	ON_COUNTRY_TAIWAN	4
#define ON_COUNTRY_ALL		5

// �� ĳ���Ͱ� ���� �� �ִ� �ִ� ���� �� 
#define	ON_MAX_BANKBOOK_NUM		20

// �� ĳ���Ͱ� ���� �� �ִ� �ִ� �ε��� �� (�ε�������*���λ� �ִ�ε����)
#define ON_MAX_AUDIT_OWN		4

// �㺸�� �ñ� �� �ִ� �������� ��
#define MAX_COLLATERAL_NUM		10


// ��ο� ����Ǵ� ���������� �ִ� ����.
#define ON_MAX_AUDIT_VILLAGE_NUM	100

// �� -> ���� �ſ뵵
#define	GETTRADECREDITBYMONEY(x)	(SI32((x) / 500))	//	(UI32(sqrt(x / 100)))
#define GETMAXTRADECREDIT(x)		(SI32((x) / 50))	//�Ϸ翡 ���� �� �ִ� �ſ뵵 : ���� �ſ뵵�� 1.5%
#define ON_MAX_DAY_CREDIT				1000

#define	ON_MAX_CHATROOM_STRING_LENGTH	128			// ��ȭ���� �ִ� �� �̸��� ����	
#define	ON_MAX_CHATROOM_NUM				50			// ������ �ִ� ��ȭ���� ��.
#define	SOVILLAGE_MAX_INN_CHAT_NUM		20			// ���� ��ȭ�濡 ���� �� �ִ� �ְ� �ο�
#define	ON_MAX_WAITROOM_PLAYER_NUM		100			// ���� ���ǿ� ���� �� �ִ� �ְ� �ο�
//#define ON_MAX_PLANT_PLAYER_NUM			20			// ����ǹ��� ���� �� �ִ� �ְ� �ο�
#define	ON_MAX_INDUSTRIAL_COMPLEX_PLAYER_NUM	100		// ��� ������ �� �� �ִ� �ְ� �ο� ��.

// �Խù� ����
#define	ON_MAX_NOTICES					2000		// �� ������ �ִ� 1000���� �Խù��� ���� �� �ִ�.(���߿��� ���� �÷��� �Ѵ�.)
#define	ON_MAX_TITLE_STRING_LENGTH		50			// �Խù��� �ִ� ���� ���� (50
#define	ON_SEND_NOTICESNUM_PER_ONCE		10			// �ѹ��� �ִ� ��� ����Ʈ�� �� �� �ֳ�?

// ������ ������
#define WEAR_ITEM_NUM					9

#define ON_WEAR_HELMET_ITEM				0
#define	ON_WEAR_WEAPON_ITEM				1
#define	ON_WEAR_ARMOR_ITEM				2
#define	ON_WEAR_BELT_ITEM				3
#define	ON_WEAR_GLOVE_ITEM				4
#define	ON_WEAR_SHOES_ITEM				5
#define	ON_WEAR_RRING_ITEM				6
#define	ON_WEAR_LRING_ITEM				7
#define ON_WEAR_GUARDIAN_ITEM			8
#define ON_WEAR_NO						200

#define ON_MAX_GUILD_UNIT				8
#define ON_MAX_GUILD_UNIT_COUNT         200  //�������� ������ ����

// ���� �ǹ� ����
enum { ON_PLANT_FARM, ON_PLANT_RANCH, ON_PLANT_FACTORY, ON_PLANT_ARMOR, ON_PLANT_WEAPON, ON_PLANT_MINE, ON_PLANT_LUMBERING, PLANT_CODE_NUM };

// ���忡 ���õ� define
//#define		ON_VILLAGE_FARM_NUM		20				//	�� ������ ���� ����
#define		ON_MAX_INDUSTRIAL_COMPLEX_NUM	20			// �� ������ �ִ� ���� ������ ��.

#define		ON_FARM_SLOT_INIT		4				//	ó�� ���� ����.
#define		ON_MAX_FACTORY_NUM		6			//	��� ������ �ִ� �ִ� ������ ��.
// �ܰ躰 ������ ������ �� �ִ� ������ (Faciliy.txt)
#define		ON_FARM_LEV1_PRODUCT	10
#define		ON_FARM_LEV2_PRODUCT	20
#define		ON_FARM_LEV3_PRODUCT	PRODUCT_CODE_NUM
// �ܰ躰 ���׷��̵� �ǹ�(Faciliy.txt)
#define		ON_FARM_LEV1_UPGRADE		0		//	�ü�.
#define		ON_FARM_LEV2_UPGRADE		2
#define		ON_FARM_LEV3_UPGRADE		5
#define		ON_PLANT_SLOTUPGRADE_CODE	0XFF18
#define		ON_PLANT_EXPANDSLOT_COST	10000

#define		MAX_UPGRADESLOT_NUM		3		//3�ܰ���� ���׷��̵� ����.
#define		PRODUCT_CODE_NUM		40
#define		UPGRADE_CODE_NUM		5
#define		FACILITY_CODE_NUM		15

#define		RESIST_KIND_NUM			2		//0:Beast, 1:Bird
#define		ON_RESIST_BEAST			0
#define		ON_RESIST_BIRD			1

#define		ON_FORCE_TAKE_DAY		190		// �Ͼ��ϸ� ������ ������ ���Ѵ� ����
#define		ON_MAX_STORAGEITEM		10		//	�̰� �ٲٸ� Ŭ����... sovilllage.h ȭ���� â�� ũ�Ⱑ �ٲ�

// ���� ����Ÿ�� �������� Ȯ��.
#define		VALIDPLANT(x)			(((x <= PLANT_CODE_NUM) && (x >= 0)) ? TRUE : FALSE)
#define		VALIDPART(x)			(((x < ON_VILLAGE_FARM_NUM) && (x >= 0)) ? TRUE : FALSE)
#define		VALIDSLOT(x)			(((x <= ON_FARM_SLOT_NUM) && (x >= 0)) ? TRUE : FALSE)

#define MAX_DESCRIPT_LENGTH				256			// ���� �ȳ��� ����
#define	MAX_BATTLE_NUM					2000		// �� ���� �������� ���ÿ� ������ �� �ִ� ��
//#define	MAX_BATTLE_NUM					20		// �� ���� �������� ���ÿ� ������ �� �ִ� ��
#define	ON_BATTLE_MAX_PLAYERNUM			7			// �ѹ��� ������ ���� �� �ִ� �ִ� �÷��̾��(���� ����)

#define MAX_GUILDDISCOUNT				50			// ������ �ڽ��� ���Ե� ��� �϶� ���εǴ� Percent

#define	ON_MAX_SHIPNAME_STRING_LENGTH	32			// �� �̸��� �ִ� ����.

#define	ON_MAX_WAITING_BATTLE_TIME		4000		// ���� ������ �ϱ� ���Ͽ� ����ϴ� �ð�

#define	GETTIMEGAP(x, y)				(((y) >= (x)) ? ((y) - (x)) : (0xffffffff - (x) + (y)))

#define	MAX_TRADE_WINDOW				8			//	2*4 ����â�� ��ϼ�
#define	MAX_TRADE_FOLLOWER				8			//	����â���� ��ȯ�� �� �ִ� �ִ� �뺴�� 32/4
#define ON_FORBIDDEN_CHAR				"\'\""		//	DB�� �������� ���Ͼ� �ϴ� ����.
#define ON_FORBIDDEN_NAME				"\'\" \\"	//	�̸��� ����� �� ���� ����.

// ĳ���� �ɷ�ġ ���� ���� ���.
enum	AbilityKindType{ON_ABILITY_KIND_STR, ON_ABILITY_KIND_DEX, ON_ABILITY_KIND_VIT, ON_ABILITY_KIND_INT};	// �ɷ�ġ ����.
// ���� �Ÿ� ���� Define
enum	BoothID{BOOTH_ID_0, BOOTH_ID_1, BOOTH_ID_2, BOOTH_ID_3, BOOTH_ID_4, ON_MAX_BOOTH_ITEM};
#define	ON_MAX_BOOTH_NOTE				30			//	���Ǵ뿡�� ������ ����



// ��ܿ� ���õ� Define
#define	SO_TOTAL_GUILD_NUM				400			// �ִ� ��� ��.



#define	ON_GUILDNAME_LENGTH						10			// ����� �̸�.
#define	ON_GUILD_MIN_MEMBER_NUM					100			// ��ܿ��� �ּ� ��ܿ��� ��.
#define	ON_GUILD_MAX_MEMBER_NUM					500			// ��ܿ��� �ִ� ��ܿ��� ��.(���߿� �����Ǿ�� �� �κ��̴�.)
#define	ON_GUILD_MEMBERNUM_PER_PAGE				5			// �� �������� ��ܿ��� ��
#define	ON_GUILD_MAX_CANDIDATE_NUM				100			// ��ܿ��� �ִ� ��ܿ��� ��.(���߿� �����Ǿ�� �� �κ��̴�.)
#define	ON_GUILD_ADD_MEMBER_NUM_PER_ONEVILLAGE	10			// ��ܿ� ���� �ϳ� �߰� �ɶ����� 5���� ��ܿ����� �ִ� ��ܿ��� ��������.

//#define ON_GUILD_COST_CREATE_GUILD              2000000     // ��� �������(�ʿ������. clIGuild::GetGuildCreateCost() ����)
#define ON_GUILD_COST_CHANGE_FLAG				100000		// ��� ��� ���� ���



// �ִ� ����� ��ġ
#define ON_MAX_HEALTH					500
#define	ON_SATIETY_CONSUME_PER_DAY		5			// �Ϸ�� 5�� ���� ��ġ�� �Һ�ȴ�.
#define	ON_SATIETY_CONSUME_PER_BATTLE	1			// �����Ҵ븶�� 1�� ���� ��ġ�� �Һ�ȴ�.

// ���� ���� ����
#define ON_MAX_PROFIT_SORT				7

#define	ON_PROFIT_TRADE					0
#define	ON_PROFIT_BANK					1
#define	ON_PROFIT_EMPLOY				2
#define	ON_PROFIT_CURE					3
#define	ON_PROFIT_TICKET				4
#define	ON_PROFIT_INN					5
#define	ON_PROFIT_PLANT					6

#define	ON_STATISTICS_YEAR				10			// ����� ��踦 ���� ���� ���ΰ�?

#define	ON_MAXGENERAL_LOW				3			// ������ �ִ� �� �����
#define ON_MAXGENERAL_HIGH				5			// ���� ������ ��� -> 5���� �ְ�
#define ON_MAXMONSTER					2			// ������ �ִ� �� ���ͼ� 

#define ON_QUSETDATA_MAXCOMLETEDQUEST	512			// �ִ� �Ϸ��Ҽ� �ִ� Quest�� 
#define ON_QUSETDATA_MAXPLAYINGQUEST	8			// �ִ� �����Ҽ� �ִ� Quest�� 
#define ON_QUSETDATA_TEXT				8			// ��ȭ���� ������ �ִ� �ִ��

#define ON_QUSETDATA_MAXQUESTREQCONDITIONAL		2	// Quest �ʿ����� �ִ��
#define ON_QUSETDATA_MAXQUESTSKIPCONDITIONAL	2	// Quest Skip���� �ִ��
#define ON_QUSETDATA_MAXQUESTENDREQOBJECT		2	// Quest ������� �ִ��
#define ON_QUSETDATA_MAXQUESTPAY				5	// Quest ����� ���� �ִ��


#define ON_BANK_DEPOSIT_TYPE			1			// ����(���������, ����)
#define ON_BANK_LOAN_TYPE				3			// ����(�ſ�, �㺸)

#define	ON_MARKET_MAXQUANTITY			100			// �������� �ѹ��� ��� �ȼ��ִ� �ִ����

enum	TradeGoodsKind{ TRADE_GOODS_NONE, TRADE_GOODS_ITEM, TRADE_GOODS_FOLLOWER, TRADE_GOODS_REALESTATE, TRADE_GOODS_MONEY, TRADE_GOODS_TRADE_BOOK };

enum	EPlayerStructerInPos{STRUCTERPOS_NONE, STRUCTERPOS_WHARFWAITTINGROOM};

enum	FactoryStatus { FACTORY_STATUS_NOTDEVELOPMENT,		// ���� ������ ���ߵ����ʾҴ�.
						FACTORY_STATUS_N0TCONSTRUCT,		// ������ �Ǽ����� �ʾҴ�.
						FACTORY_STATUS_CONSTRUCTING_LV1,	// ������ �Ǽ����̴�. ( 1�ܰ� )
						FACTORY_STATUS_CONSTRUCTING_LV2,	// ������ �Ǽ����̴�. ( 2�ܰ� )
						FACTORY_STATUS_CONSTRUCTING_LV3,	// ������ �Ǽ����̴�. ( 3�ܰ� )
						FACTORY_STATUS_PRODUCE };			// ���忡�� �������̴�.						

typedef enum
{
	WEATHER_NONE
	, WEATHER_RAIN
	, WEATHER_SNOW
} SWeatherType;


#endif

