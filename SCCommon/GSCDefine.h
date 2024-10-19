#ifndef	_GSCDEFINE_HEADER
#define	_GSCDEFINE_HEADER

typedef			__int64		MONEY;		//	DB로 입,출력하는건 아직 이거 쓰지 말 것. 출력은  %I64d
typedef			MONEY		VDEF;		// Village Defence

// ID 부여 (총 65535개의 ID가 할당 되어 있다.)
// 서버에서의 플레이어 관련. (1 ~ 32767)
// actdoll (2005/02/23 23:17) : 수용할 수 있는 플레이어 카운트의 최대값
//
//	!!!!!!!!!!! 주의 !!!!!!!!!!
//
//	플레이어에 할당되는 버퍼를 줄여 작업 속도를 빠르게 하기 위해 주로 서버 작업자가 많이 건드리는 부분이다.
//	일단 임시 방편으로 디파인을 나눠놨으나 최종 배포시에는 양쪽의 MAX_PLAYER_NUM 값이 반드시 일치해야 한다.
//	양쪽 값이 틀리게 될 경우 잘못하면 치명적인 오류(클라이언트 캐릭터가 로그인 한 후 화면이 검게 변하는 등의 버그)가 발생한다.
//	(캐릭터 초기화시 (RESPONSE_LOGIN) 부분에서 잘못된 정보가 넘어오곤 한다.)
//	반드시 최종적으로 배포 상황이 발생할 때는 서버와 클라이언트의 두 값이 일치해야 한다.
//
// "Define.h" 에서 ON_MAX_CONNECTION 도 같은 값으로 설정해 주어야 한다.
#ifdef	_IMJINONLINESERVER		
//	#define	MAX_PLAYER_NUM				20//32768			// 서버 부분이므로 서버 작업자들은 여기만 건드릴 것! 
    #define	MAX_PLAYER_NUM				6000
#else//!_IMJINONLINESERVER
	#define	MAX_PLAYER_NUM				6000//32768			// 클라이언트 부분. 클라이언트 작업자가 거의 건드릴 일이 없다. 조작하지 말것!
#endif//_IMJINONLINESERVER

#define	PLAYER_START_ACCOUNT		1

// 서버에서의 배 관련.(32768 ~ 33767) 
#define	MAX_SHIP_NUM		1000
#define	SHIP_START_ACCOUNT		(UI32(32768))


// 마을의 국가 코드 CHAR 형
#define NATION_CODE_KOREA			'K'
#define NATION_CODE_CHINA			'C'
#define NATION_CODE_JAPAN			'J'
#define	NATION_CODE_TAIWAN			'T'

// 서버에서의 몬스터 관련. (33768 ~ 65535)
#define	MAX_MONSTER_NUM			(UI32(31768))
#define	MONSTER_START_ACCOUNT	(UI32(33768))

// Account를 이용하여 Kind를 얻어온다.
#define	ISPLAYER(x)				(((x >= PLAYER_START_ACCOUNT) && (x <= (PLAYER_START_ACCOUNT + MAX_PLAYER_NUM - 1))) ? TRUE : FALSE)
#define	ISSHIP(x)				(((x >= SHIP_START_ACCOUNT) && (x <= (SHIP_START_ACCOUNT + MAX_SHIP_NUM - 1))) ? TRUE : FALSE)
#define	ISMONSTER(x)			(((x >= MONSTER_START_ACCOUNT) && (x <= (MONSTER_START_ACCOUNT + MAX_MONSTER_NUM - 1))) ? TRUE : FALSE)

// 에러시 0을 리턴할 수 없을때 Error정의
#define	UI32_ERROR				0xffffffff
#define	UI16_ERROR				0xffff
#define UI08_ERROR				0xff


// 최대 소지금
//#define ON_MAX_MONEY				200000000		// 2억
#define ON_MAX_MONEY				500000000		// 5억
// 최대 저축액
//#define ON_MAX_SAVING				5000000000
#define ON_MAX_SAVING				10000000000		// 100억 

// 한 캐릭터가 가질 수 있는 최대 부하의 수.
#define	ON_MAX_FOLLOWER_NUM			12

// 캐릭터의 초기 보너스
#define	ON_CHARACTER_INITBONUS		10		//	캐릭터의 초기 보너스 값

// 한 명당 최대 가질 수 있는 아이템의 수.
#define	ON_MYDATA_MAX_ITEM_NUM		10		// 물품 및 아이템을 합한 수치이다.

// 사용자 등급
enum	UserGrade{ON_USERGRADE_NORMAL, ON_USERGRADE_ADMIN1, ON_USERGRADE_ADMIN2, ON_USERGRADE_ADMIN3, ON_USERGRADE_ADMIN4, ON_USERGRADE_ADMIN5};

// 방향 설정.
#define	ON_EAST			1
#define	ON_WEST			2
#define	ON_NORTH		4
#define	ON_SOUTH		8

// 시장에서 파는 최대 아이템의 종류 개수.
//#define	ON_MAX_MARKETITEM		20

// 국가. 
#define	ON_COUNTRY_NONE		0
#define	ON_COUNTRY_KOREA	1
#define	ON_COUNTRY_JAPAN	2
#define	ON_COUNTRY_CHINA	3
#define	ON_COUNTRY_TAIWAN	4
#define ON_COUNTRY_ALL		5

// 한 캐릭터가 가질 수 있는 최대 통장 수 
#define	ON_MAX_BANKBOOK_NUM		20

// 한 캐릭터가 가질 수 있는 최대 부동산 수 (부동산종류*개인상 최대부동산수)
#define ON_MAX_AUDIT_OWN		4

// 담보로 맡길 수 있는 아이템의 수
#define MAX_COLLATERAL_NUM		10


// 장부에 저장되는 마을정보의 최대 개수.
#define ON_MAX_AUDIT_VILLAGE_NUM	100

// 돈 -> 무역 신용도
#define	GETTRADECREDITBYMONEY(x)	(SI32((x) / 500))	//	(UI32(sqrt(x / 100)))
#define GETMAXTRADECREDIT(x)		(SI32((x) / 50))	//하루에 오를 수 있는 신용도 : 전날 신용도의 1.5%
#define ON_MAX_DAY_CREDIT				1000

#define	ON_MAX_CHATROOM_STRING_LENGTH	128			// 대화방의 최대 방 이름이 길이	
#define	ON_MAX_CHATROOM_NUM				50			// 주점이 최대 대화방의 수.
#define	SOVILLAGE_MAX_INN_CHAT_NUM		20			// 주점 대화방에 들어올 수 있는 최고 인원
#define	ON_MAX_WAITROOM_PLAYER_NUM		100			// 주점 대기실에 들어올 수 있는 최고 인원
//#define ON_MAX_PLANT_PLAYER_NUM			20			// 생산건물에 들어올 수 있는 최고 인원
#define	ON_MAX_INDUSTRIAL_COMPLEX_PLAYER_NUM	100		// 산업 단지에 들어갈 수 있는 최고 인원 수.

// 게시물 정보
#define	ON_MAX_NOTICES					2000		// 한 마을에 최대 1000개의 게시물이 있을 수 있다.(나중에는 좀더 늘려야 한다.)
#define	ON_MAX_TITLE_STRING_LENGTH		50			// 게시물의 최대 제목 길이 (50
#define	ON_SEND_NOTICESNUM_PER_ONCE		10			// 한번에 최대 몇개의 리스트가 갈 수 있나?

// 착용한 아이템
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
#define ON_MAX_GUILD_UNIT_COUNT         200  //공성전용 유닛의 개수

// 생산 건물 정보
enum { ON_PLANT_FARM, ON_PLANT_RANCH, ON_PLANT_FACTORY, ON_PLANT_ARMOR, ON_PLANT_WEAPON, ON_PLANT_MINE, ON_PLANT_LUMBERING, PLANT_CODE_NUM };

// 농장에 관련된 define
//#define		ON_VILLAGE_FARM_NUM		20				//	한 마을의 농장 개수
#define		ON_MAX_INDUSTRIAL_COMPLEX_NUM	20			// 한 마을의 최대 생산 단지의 수.

#define		ON_FARM_SLOT_INIT		4				//	처음 슬롯 갯수.
#define		ON_MAX_FACTORY_NUM		6			//	산업 단지에 있는 최대 공장의 수.
// 단계별 아이템 생산할 수 있는 아이템 (Faciliy.txt)
#define		ON_FARM_LEV1_PRODUCT	10
#define		ON_FARM_LEV2_PRODUCT	20
#define		ON_FARM_LEV3_PRODUCT	PRODUCT_CODE_NUM
// 단계별 업그레이드 건물(Faciliy.txt)
#define		ON_FARM_LEV1_UPGRADE		0		//	시설.
#define		ON_FARM_LEV2_UPGRADE		2
#define		ON_FARM_LEV3_UPGRADE		5
#define		ON_PLANT_SLOTUPGRADE_CODE	0XFF18
#define		ON_PLANT_EXPANDSLOT_COST	10000

#define		MAX_UPGRADESLOT_NUM		3		//3단계까지 업그레이드 가능.
#define		PRODUCT_CODE_NUM		40
#define		UPGRADE_CODE_NUM		5
#define		FACILITY_CODE_NUM		15

#define		RESIST_KIND_NUM			2		//0:Beast, 1:Bird
#define		ON_RESIST_BEAST			0
#define		ON_RESIST_BIRD			1

#define		ON_FORCE_TAKE_DAY		190		// 일안하면 강제로 농장을 빼앗는 날자
#define		ON_MAX_STORAGEITEM		10		//	이거 바꾸면 클난다... sovilllage.h 화일의 창고 크기가 바뀜

// 농장 데이타가 유용한지 확인.
#define		VALIDPLANT(x)			(((x <= PLANT_CODE_NUM) && (x >= 0)) ? TRUE : FALSE)
#define		VALIDPART(x)			(((x < ON_VILLAGE_FARM_NUM) && (x >= 0)) ? TRUE : FALSE)
#define		VALIDSLOT(x)			(((x <= ON_FARM_SLOT_NUM) && (x >= 0)) ? TRUE : FALSE)

#define MAX_DESCRIPT_LENGTH				256			// 마을 안내문 길이
#define	MAX_BATTLE_NUM					2000		// 한 게임 서버에서 동시에 전투할 수 있는 수
//#define	MAX_BATTLE_NUM					20		// 한 게임 서버에서 동시에 전투할 수 있는 수
#define	ON_BATTLE_MAX_PLAYERNUM			7			// 한번에 전투에 임할 수 있는 최대 플레이어수(몬스터 포함)

#define MAX_GUILDDISCOUNT				50			// 마을이 자신이 가입된 길드 일때 할인되는 Percent

#define	ON_MAX_SHIPNAME_STRING_LENGTH	32			// 배 이름의 최대 길이.

#define	ON_MAX_WAITING_BATTLE_TIME		4000		// 실제 전투를 하기 위하여 대기하는 시간

#define	GETTIMEGAP(x, y)				(((y) >= (x)) ? ((y) - (x)) : (0xffffffff - (x) + (y)))

#define	MAX_TRADE_WINDOW				8			//	2*4 교역창의 블록수
#define	MAX_TRADE_FOLLOWER				8			//	교역창에서 교환할 수 있는 최대 용병수 32/4
#define ON_FORBIDDEN_CHAR				"\'\""		//	DB에 저장하지 말하야 하는 글자.
#define ON_FORBIDDEN_NAME				"\'\" \\"	//	이름에 사용할 수 없는 글자.

// 캐릭터 능력치 조정 관련 상수.
enum	AbilityKindType{ON_ABILITY_KIND_STR, ON_ABILITY_KIND_DEX, ON_ABILITY_KIND_VIT, ON_ABILITY_KIND_INT};	// 능력치 종류.
// 저자 거리 관련 Define
enum	BoothID{BOOTH_ID_0, BOOTH_ID_1, BOOTH_ID_2, BOOTH_ID_3, BOOTH_ID_4, ON_MAX_BOOTH_ITEM};
#define	ON_MAX_BOOTH_NOTE				30			//	좌판대에서 아이템 설명



// 상단에 관련된 Define
#define	SO_TOTAL_GUILD_NUM				400			// 최대 길드 수.



#define	ON_GUILDNAME_LENGTH						10			// 상단의 이름.
#define	ON_GUILD_MIN_MEMBER_NUM					100			// 상단에서 최소 상단원의 수.
#define	ON_GUILD_MAX_MEMBER_NUM					500			// 상단에서 최대 상단원의 수.(나중에 삭제되어야 할 부분이다.)
#define	ON_GUILD_MEMBERNUM_PER_PAGE				5			// 한 페이지당 상단원의 수
#define	ON_GUILD_MAX_CANDIDATE_NUM				100			// 상단에서 최대 상단원의 수.(나중에 삭제되어야 할 부분이다.)
#define	ON_GUILD_ADD_MEMBER_NUM_PER_ONEVILLAGE	10			// 상단에 마을 하나 추가 될때마다 5명의 상단원수가 최대 상단원에 더해진다.

//#define ON_GUILD_COST_CREATE_GUILD              2000000     // 길드 생성비용(필요없어짐. clIGuild::GetGuildCreateCost() 참조)
#define ON_GUILD_COST_CHANGE_FLAG				100000		// 상단 깃발 변경 비용



// 최대 배고픔 수치
#define ON_MAX_HEALTH					500
#define	ON_SATIETY_CONSUME_PER_DAY		5			// 하루당 5의 포만 수치가 소비된다.
#define	ON_SATIETY_CONSUME_PER_BATTLE	1			// 전투할대마다 1의 포만 수치가 소비된다.

// 마을 수익 관련
#define ON_MAX_PROFIT_SORT				7

#define	ON_PROFIT_TRADE					0
#define	ON_PROFIT_BANK					1
#define	ON_PROFIT_EMPLOY				2
#define	ON_PROFIT_CURE					3
#define	ON_PROFIT_TICKET				4
#define	ON_PROFIT_INN					5
#define	ON_PROFIT_PLANT					6

#define	ON_STATISTICS_YEAR				10			// 몇년의 통계를 갖고 있을 것인가?

#define	ON_MAXGENERAL_LOW				3			// 가질수 있는 총 장수수
#define ON_MAXGENERAL_HIGH				5			// 손자 병법서 사용 -> 5명이 최고
#define ON_MAXMONSTER					2			// 가질수 있는 총 몬스터수 

#define ON_QUSETDATA_MAXCOMLETEDQUEST	512			// 최대 완료할수 있는 Quest수 
#define ON_QUSETDATA_MAXPLAYINGQUEST	8			// 최대 진행할수 있는 Quest수 
#define ON_QUSETDATA_TEXT				8			// 대화에서 가질수 있는 최대수

#define ON_QUSETDATA_MAXQUESTREQCONDITIONAL		2	// Quest 필요조건 최대수
#define ON_QUSETDATA_MAXQUESTSKIPCONDITIONAL	2	// Quest Skip조건 최대수
#define ON_QUSETDATA_MAXQUESTENDREQOBJECT		2	// Quest 종료목적 최대수
#define ON_QUSETDATA_MAXQUESTPAY				5	// Quest 종료시 보상 최대수


#define ON_BANK_DEPOSIT_TYPE			1			// 예금(자유저축식, 정기)
#define ON_BANK_LOAN_TYPE				3			// 대출(신용, 담보)

#define	ON_MARKET_MAXQUANTITY			100			// 상점에서 한번에 사고 팔수있는 최대수량

enum	TradeGoodsKind{ TRADE_GOODS_NONE, TRADE_GOODS_ITEM, TRADE_GOODS_FOLLOWER, TRADE_GOODS_REALESTATE, TRADE_GOODS_MONEY, TRADE_GOODS_TRADE_BOOK };

enum	EPlayerStructerInPos{STRUCTERPOS_NONE, STRUCTERPOS_WHARFWAITTINGROOM};

enum	FactoryStatus { FACTORY_STATUS_NOTDEVELOPMENT,		// 아직 토지가 개발되지않았다.
						FACTORY_STATUS_N0TCONSTRUCT,		// 공장이 건설되지 않았다.
						FACTORY_STATUS_CONSTRUCTING_LV1,	// 공장을 건설중이다. ( 1단계 )
						FACTORY_STATUS_CONSTRUCTING_LV2,	// 공장을 건설중이다. ( 2단계 )
						FACTORY_STATUS_CONSTRUCTING_LV3,	// 공장을 건설중이다. ( 3단계 )
						FACTORY_STATUS_PRODUCE };			// 공장에서 생산중이다.						

typedef enum
{
	WEATHER_NONE
	, WEATHER_RAIN
	, WEATHER_SNOW
} SWeatherType;


#endif

