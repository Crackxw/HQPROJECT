//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: 임진록 온라인 [거상] - AttackAtb 수열값
//	File Name		: Enum_AttackAtb.h	
//	Birth Date		: 2004. 03. 29.	
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		거상에서 사용되는 수열값 데이터이며, 이 내용은 프로그램 컴파일에 첨부됩니다.
//		수열을 추가할 때에는 특별한 경우를 제외하고 끝부분과 MAX값 사이에 추가하시기 바랍니다.				
//						
//===================================================================================================						

#ifndef	__ENUM_ATTACKATB_H__
#define	__ENUM_ATTACKATB_H__

//-----------------------------------------------------------------------------
// Name: ENUM_ATTACKATB
// Code: actdoll (2004-01-01)
// Desc: 
//		수열을 정의하는 글자수가 영문기준 47자를 넘어가면 안된다!!!!
//		이곳에 수열을 추가할 경우 Enum_AttackAtb.cpp의 생성자 부분에서
//		SET_PROTOCOL을 이용해 해당 수열명을 반드시 등록해야 한다. 잊지말자!!!
//-----------------------------------------------------------------------------
typedef enum	ENUM_ATTACKATB		
{						
	ATTACKATB_INVALID				= -1	,	//		에러값이다.(이 값은 죽어도 절대불변!!!)
	ATTACKATB_FIRST					= 0		,	//		안쓰는 값. 기준으로 쓰기 위한 값이다.
	ATTACKATB_NONE					= 0		,	//		세팅값. 이 값을 설정에서 쓰면 작동을 안하게끔 한다.

	ATTACKATB_SWORD					= 1		,	//1		
	ATTACKATB_STRONGGUN						,	//2		
	ATTACKATB_AIRMISSILE					,	//3
	ATTACKATB_SAMMISSILE					,	//4
	ATTACKATB_CANNON2C						,	//5		발석거 
	ATTACKATB_NEEDLE						,	//6		침
	ATTACKATB_CANNON						,	//7
	ATTACKATB_GUN							,	//8
	ATTACKATB_MAGIC							,	//9
	ATTACKATB_MAGIC2						,	//10	조선 승병의 공격용 마법 
	ATTACKATB_MAGIC3						,	//11	일본 무녀의 공격용 마법 

	ATTACKATB_FIRECAR				= 13	,	//13
	ATTACKATB_SHIPCANNON					,	//14
	ATTACKATB_FIRE							,	//15
	ATTACKATB_HEAVYGUN						,	//16
	ATTACKATB_FIREARROW						,	//17	불화살 
	ATTACKATB_ARROW							,	//18
	ATTACKATB_SELFEXP						,	//19
	ATTACKATB_LIGHTNING						,	//20	번개술
	ATTACKATB_EARTHQUAKE					,	//21	지진술
	ATTACKATB_BIGSWORD						,	//22	큰검
	ATTACKATB_ELECTRON						,	//23
	ATTACKATB_THROWBOMB						,	//24	던지는 폭탄 
	ATTACKATB_MAGICBALL						,	//25	심유경 마법 공격 

	ATTACKATB_BOMB					= 27	,	//27
	ATTACKATB_TORNADO						,	//28	풍백술
	ATTACKATB_KIMBOMB           			,	//29	폭뢰격
	ATTACKATB_MAGIC5J           			,	//30	우기다의 만월도끼춤
	ATTACKATB_DART							,	//31	닌자의 수리검
	ATTACKATB_MAGIC8K						,	//32	김시민의 쐐기 지르기 
	ATTACKATB_CANNONJ						,	//33	파쇄차 공격. 
	ATTACKATB_FIRETANK						,	//34
	ATTACKATB_COLLTANK						,	//35
	ATTACKATB_CANNON2J						,	//36	발석거 
	ATTACKATB_POISONARROW					,	//37	독침 
	ATTACKATB_FIRETANKARROW					,	//38	백제 쇠뇌의 공격 
	ATTACKATB_COLLTANKJARROW				,	//39	백제 충차의 공격 
	ATTACKATB_GENERAL4KFIRE					,	//40	도림의 공격 
	ATTACKATB_GENERAL4J						,	//41	고흥의 공격 
	ATTACKATB_GENERAL3C						,	//42	원효의 공격 
	ATTACKATB_CANNONKARROW					,	//43	마름쇠차 공격 
	ATTACKATB_FLAMETANKJARROW				,	//44
	ATTACKATB_CANNONC						,	//45	신라 전호피차 공격 
	ATTACKATB_AXE							,	//46	도끼 공격
	ATTACKATB_SWORDENERGY					,	//47	견훤의 발도검기 속성
	ATTACKATB_ELEPHANT						,	//48	코끼리 (임2)
	ATTACKATB_ANTIARMOUR					,	//49	대메카닉 (임2)
	ATTACKATB_CHASEFIRE						,	//50	고흥의 화폭술 속성 (천신)
	ATTACKATB_AIRATTACKUPARROW				,	//51	비전 연구
	ATTACKATB_TOWERCANNON					,	//52	망루용 화포
	ATTACKATB_UPGRADETOWERCANNON			,	//53	업그레이드 망루용 화표
	ATTACKATB_GATPHAPOISON					,	//54	온라인 - 갓파의 독액 던지기
	ATTACKATB_DUEOKSINI						,	//55	두억시니 공격 속성 - 맞으면 뒤로 밀림. 
	ATTACKATB_TOAD							,	//56	독두꺼비

	ATTACKATB_FURHEADDEMON			= 60	,	//60	털가면. 
	ATTACKATB_SNOWWOMAN						,	//61	설녀. 
	ATTACKATB_GOLDDRAGONELECTRON			,	//62	금룡 공격. 
	ATTACKATB_ONGHWA						,	//63	온천 원숭이 해골.  
	ATTACKATB_GHOSTCAT						,	//64	귀묘 음파 공격. 
	ATTACKATB_DOLL1							,	//65	무명 인형 공격. 
	ATTACKATB_DOLL2							,	//66	하카다 인형 공격. 
	ATTACKATB_JUSTICEK						,	//67	조선 유생 공격
	ATTACKATB_BUTTERFLY						,	//68	독나방 공격 
	ATTACKATB_WIDOW							,	//69	거미요괴 공격 
	ATTACKATB_WAVE							,	//70	바다거북이 파동공격. 
	ATTACKATB_FIREUSINGMANA					,	//71	불공격(마법 사용)
	ATTACKATB_LEECH							,	//72	왕거머리. 
	ATTACKATB_SEAHORSE						,	//73	해마물쏘기. 
	ATTACKATB_MACHINEGUN					,	//74	연발 사격 
	ATTACKATB_THROWSPEAR					,	//75	던지는창 (원주민)  
	ATTACKATB_SCORPION						,	//76	독전갈 독. 
	ATTACKATB_CACTUS						,	//77	괴물선인장 가시. 
	ATTACKATB_CLAW							,	//78	조도의 공격속성. 
	ATTACKATB_MAGICIAN						,	//79	요술사 마법공격.  
	ATTACKATB_BIGTREE						,	//80	거대수 나뭇가지 공격. 
	ATTACKATB_COBRA							,	//81	코브라 독 공격.
	ATTACKATB_VOLCANOFLAME					,	//82	화산의불꽃.   불덩이.
	ATTACKATB_EVIL							,	//83	이사의 사념.
	ATTACKATB_ICE							,	//84	이사의 사념 특수기술.
	ATTACKATB_MAGIC4T						,	//85	징키스칸혼 특수 기술.
	ATTACKATB_JAPANGHOST					,	//86	일본귀신 특수 기술.
	ATTACKATB_KUKET							,	//87	쿠켓
	ATTACKATB_AUTA							,	//88	아우타 공격.
	ATTACKATB_SUB1							,	//89	진성여왕 화살공격 1
	ATTACKATB_SUB2							,	//90	진성여왕 화살공격 2
	ATTACKATB_FIREATTACK					,	//91	화공신포.
	ATTACKATB_RUNNINGFIRE					,	//92	연발
	ATTACKATB_ENERGYPA						,	//93	장풍.
	ATTACKATB_LONG_SPEAR					,	//94	손유창 장창소환.
	ATTACKATB_PAPERDOLL						,	//95	음양사 종이인형
	ATTACKATB_NEEDLEATTACK					,	//96	침공격
	ATTACKATB_CLOUDWIND						,	//97	풍모술
	ATTACKATB_STONEMAGIC					,	//98	석괴술
	ATTACKATB_GODSPEAR						,	//99	신창노도
	ATTACKATB_FLOWERRAIN					,	//100	만천화우
	ATTACKATB_SOULCHANGE					,	//101	빙의술
	ATTACKATB_MAGICCATTACK					,	//102	도술사 공격
	ATTACKATB_SUNBIYOUN						,	//103	순비연 공격
	ATTACKATB_SNOW							,	//104	눈덩이

	// actdoll (2004/03/06 5:00) : 3월 패치관련 추가
	ATTACKATB_PYTHON						,	//105	이무기 마법공격	// actdoll (2004/03/06 4:08) : 추가
	ATTACKATB_JINEMPEROR					,	//106	진시황의혼 마법공격	// actdoll (2004/03/06 4:08) : 추가
	ATTACKATB_GUARDIAN_SNAKE				,	//107	수호부(뱀)
	ATTACKATB_GUARDIAN_PIG					,	//108	수호부(돼지)
	ATTACKATB_GUARDIAN_CHICKEN				,	//109	수호부(닭)

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:48) : 추가 기술 - 장수 2차 전직
//----------------------------------------------------------------------------
	ATTACKATB_TURTLE_VEHICLE				,	// 110	조선-거북차, 이순신 2차 전직
	ATTACKATB_THUNDER_VEHICLE				,	// 111	조선-뇌전차, 허준 2차 전직
	ATTACKATB_QUAKE_VEHICLE					,	// 112	일본-지진차, 와키자카 2차 전직
	ATTACKATB_BLACK_DRAGON_VEHICLE			,	// 113	일본-흑룡차, 세이메이 2차 전직
	ATTACKATB_CATAPULT						,	// 114	중국-발석거, 순비연<=이령 2차 전직
	ATTACKATB_FARANGI_VEHICLE				,	// 115	중국-불랑기포차, 이령<=순비연 2차 전직
	ATTACKATB_FIRE_DRAGON_VEHICLE			,	// 116	대만-화룡차, 손유창 2차 전직
	ATTACKATB_CHINESEPHOENIX				,	// 117	대만-봉황비조, 장선화 2차 전직

	ATTACKATB_CATDOLL						,
	ATTACKATB_OLDFOXDOLL					,
	ATTACKATB_RACCOONDOLL					,
	ATTACKATB_NINJADOLL						,
	ATTACKATB_SNOWWOMANDOOL					,
	ATTACKATB_YANGGUIBIDOLL					,
	ATTACKATB_JIJANGDOLL					,
	ATTACKATB_SNAKEWOMANDOLL				,

	// robypark (2004/6/4 13:6)
	// 앙천대소 공격, 불랑기포차
	ATTACKATB_ROAR_OF_LAUGHTER				,	// 126

	// robypark 2004/6/10 19:4
	// 흑룡차 천지독살, 냉기공격
	ATTACKATB_BLACK_DRAGON_ICE				,	// 127

	// robypark 2004/6/11 10:49
	// 흑룡차 천지독살, 독공격
	ATTACKATB_BLACK_DRAGON_POISON			,	// 128

	// robypark 2004/7/14 16:35
	// 에비스 인형 공격
	ATTACKATB_DOLL_EVIS						,	// 129

	// robypark 2005/2/2 17:27
	// 달마 인형 공격
	ATTACKATB_DOLL_DHARMA					,	// 130

	// <주의> 변경시 char-attack, calcattack의 RealApower도 수정해야 함, 끊김없이 순차적이어야 함.

	ATTACKATB_MAX								//		총 캐릭터의 종류 갯수(실제 갯수는 아니다. 이것 밖으로 캐릭터를 추가하면 절대 안된다!!!!)
};						

//-----------------------------------------------------------------------------
// Name: CLASS_ATTACKATB
// Code: actdoll (2004-01-01)
// Desc: 수열에 대한 스트링 글자를 관리하는 클래스다.
//-----------------------------------------------------------------------------
class CLASS_ATTACKATB
{
private:
	struct DATA
	{
		char	m_pszString[48];
	};
	static DATA	m_Data[ATTACKATB_MAX];

private:
	static	void	SetData( int index, char *pszStr );

public:
	static	ENUM_ATTACKATB	GetData		( char *pszStr );
	static	const char*		GetData		( ENUM_ATTACKATB enIndex );
	static	int				GetMaxCount	( void )	{ return ATTACKATB_MAX; }

	CLASS_ATTACKATB();
	~CLASS_ATTACKATB();
};

#endif//__ENUM_ATTACKATB_H__