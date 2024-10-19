//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: 임진록 온라인 [거상] - AttackAtb 수열값
//	File Name		: Enum_AttackAtb.cpp
//	Birth Date		: 2004. 03. 29.	
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		거상에서 사용되는 수열값 데이터이며, 이 내용은 프로그램 컴파일에 첨부됩니다.
//		수열을 추가할 때에는 특별한 경우를 제외하고 끝부분과 MAX값 사이에 추가하시기 바랍니다.				
//						
//===================================================================================================						

#include <GSL.h>

#include <string.h>
#include "Enum_AttackAtb.h"

CLASS_ATTACKATB::DATA	CLASS_ATTACKATB::m_Data[ATTACKATB_MAX]	= {0,};	// 스태틱 변수 초기화
CLASS_ATTACKATB			g_clAttackAtb;									// 초기화 가동용 임시 전역변수

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: 해당 수열 인덱스와 거기에 할당될 실제 이름을 배치시킨다.
//		index		- 수열값
//		pszStr		- 해당 수열값의 이름
//-----------------------------------------------------------------------------
void	CLASS_ATTACKATB::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= ATTACKATB_FIRST || index >= ATTACKATB_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		pszStr		- 찾을 수열값의 이름
//		return		- 해당 수열명에 할당된 수열값. 범위가 수열을 넘겼다면 ATTACKATB_INVALID를 리턴
//-----------------------------------------------------------------------------
ENUM_ATTACKATB	CLASS_ATTACKATB::GetData( char *pszStr )
{
	if( !pszStr )		return ATTACKATB_INVALID;
	for( register int i=ATTACKATB_FIRST; i<ATTACKATB_MAX; i++ )	
	{
		if( strcmp( m_Data[i].m_pszString, pszStr ) == 0 ) return (ENUM_ATTACKATB)i;
	}
	return ATTACKATB_INVALID;
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		enIndex		- 찾을 수열값
//		return		- 해당 수열값의 수열명. 없으면 NULL을 리턴. 배당이 안됐다면 스트링은 ""
//-----------------------------------------------------------------------------
const char* CLASS_ATTACKATB::GetData( ENUM_ATTACKATB enIndex )
{
	if( enIndex <= ATTACKATB_FIRST || enIndex >= ATTACKATB_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_ATTACKATB()
// Code: actdoll (2004-03-18)
// Desc: 소멸자
//-----------------------------------------------------------------------------
CLASS_ATTACKATB::~CLASS_ATTACKATB()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_ATTACKATB()
// Code: actdoll (2004-03-18)
// Desc: 생성자(아주중요!!!)
//-----------------------------------------------------------------------------
CLASS_ATTACKATB::CLASS_ATTACKATB()
{
	memset( m_Data, 0, (sizeof(DATA) * ATTACKATB_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// 수열이 추가될 경우 이 부분 맨 아래 (//EOF앞쪽)에 추가시킨다.
	
	SET_PROTOCOL( ATTACKATB_SWORD					)	//1		
	SET_PROTOCOL( ATTACKATB_STRONGGUN				)	//2		
	SET_PROTOCOL( ATTACKATB_AIRMISSILE				)	//3		
	SET_PROTOCOL( ATTACKATB_SAMMISSILE				)	//4		
	SET_PROTOCOL( ATTACKATB_CANNON2C				)	//5		발석거 
	SET_PROTOCOL( ATTACKATB_NEEDLE					)	//6		침
	SET_PROTOCOL( ATTACKATB_CANNON					)	//7		
	SET_PROTOCOL( ATTACKATB_GUN						)	//8		
	SET_PROTOCOL( ATTACKATB_MAGIC					)	//9		
	SET_PROTOCOL( ATTACKATB_MAGIC2					)	//10	조선 승병의 공격용 마법 
	SET_PROTOCOL( ATTACKATB_MAGIC3					)	//11	일본 무녀의 공격용 마법 
	
	SET_PROTOCOL( ATTACKATB_FIRECAR					)	//13
	SET_PROTOCOL( ATTACKATB_SHIPCANNON				)	//14
	SET_PROTOCOL( ATTACKATB_FIRE					)	//15
	SET_PROTOCOL( ATTACKATB_HEAVYGUN				)	//16
	SET_PROTOCOL( ATTACKATB_FIREARROW				)	//17	불화살 
	SET_PROTOCOL( ATTACKATB_ARROW					)	//18
	SET_PROTOCOL( ATTACKATB_SELFEXP					)	//19
	SET_PROTOCOL( ATTACKATB_LIGHTNING				)	//20	번개술
	SET_PROTOCOL( ATTACKATB_EARTHQUAKE				)	//21	지진술
	SET_PROTOCOL( ATTACKATB_BIGSWORD				)	//22	큰검
	SET_PROTOCOL( ATTACKATB_ELECTRON				)	//23
	SET_PROTOCOL( ATTACKATB_THROWBOMB				)	//24	던지는 폭탄 
	SET_PROTOCOL( ATTACKATB_MAGICBALL				)	//25	심유경 마법 공격 
	
	SET_PROTOCOL( ATTACKATB_BOMB					)	//27
	SET_PROTOCOL( ATTACKATB_TORNADO					)	//28	풍백술
	SET_PROTOCOL( ATTACKATB_KIMBOMB           		)	//29	폭뢰격
	SET_PROTOCOL( ATTACKATB_MAGIC5J           		)	//30	우기다의 만월도끼춤
	SET_PROTOCOL( ATTACKATB_DART					)	//31	닌자의 수리검
	SET_PROTOCOL( ATTACKATB_MAGIC8K					)	//32	김시민의 쐐기 지르기 
	SET_PROTOCOL( ATTACKATB_CANNONJ					)	//33	파쇄차 공격. 
	SET_PROTOCOL( ATTACKATB_FIRETANK				)	//34
	SET_PROTOCOL( ATTACKATB_COLLTANK				)	//35
	SET_PROTOCOL( ATTACKATB_CANNON2J				)	//36	발석거 
	SET_PROTOCOL( ATTACKATB_POISONARROW				)	//37	독침 
	SET_PROTOCOL( ATTACKATB_FIRETANKARROW			)	//38	백제 쇠뇌의 공격 
	SET_PROTOCOL( ATTACKATB_COLLTANKJARROW			)	//39	백제 충차의 공격 
	SET_PROTOCOL( ATTACKATB_GENERAL4KFIRE			)	//40	도림의 공격 
	SET_PROTOCOL( ATTACKATB_GENERAL4J				)	//41	고흥의 공격 
	SET_PROTOCOL( ATTACKATB_GENERAL3C				)	//42	원효의 공격 
	SET_PROTOCOL( ATTACKATB_CANNONKARROW			)	//43	마름쇠차 공격 
	SET_PROTOCOL( ATTACKATB_FLAMETANKJARROW			)	//44
	SET_PROTOCOL( ATTACKATB_CANNONC					)	//45	신라 전호피차 공격 
	SET_PROTOCOL( ATTACKATB_AXE						)	//46	도끼 공격
	SET_PROTOCOL( ATTACKATB_SWORDENERGY				)	//47	견훤의 발도검기 속성
	SET_PROTOCOL( ATTACKATB_ELEPHANT				)	//48	코끼리 (임2)
	SET_PROTOCOL( ATTACKATB_ANTIARMOUR				)	//49	대메카닉 (임2)
	SET_PROTOCOL( ATTACKATB_CHASEFIRE				)	//50	고흥의 화폭술 속성 (천신)
	SET_PROTOCOL( ATTACKATB_AIRATTACKUPARROW		)	//51	비전 연구
	SET_PROTOCOL( ATTACKATB_TOWERCANNON				)	//52	망루용 화포
	SET_PROTOCOL( ATTACKATB_UPGRADETOWERCANNON		)	//53	업그레이드 망루용 화표
	SET_PROTOCOL( ATTACKATB_GATPHAPOISON			)	//54	온라인 - 갓파의 독액 던지기
	SET_PROTOCOL( ATTACKATB_DUEOKSINI				)	//55	두억시니 공격 속성 - 맞으면 뒤로 밀림. 
	SET_PROTOCOL( ATTACKATB_TOAD					)	//56	독두꺼비
	
	SET_PROTOCOL( ATTACKATB_FURHEADDEMON			)	//60	털가면. 
	SET_PROTOCOL( ATTACKATB_SNOWWOMAN				)	//61	설녀. 
	SET_PROTOCOL( ATTACKATB_GOLDDRAGONELECTRON		)	//62	금룡 공격. 
	SET_PROTOCOL( ATTACKATB_ONGHWA					)	//63	온천 원숭이 해골.  
	SET_PROTOCOL( ATTACKATB_GHOSTCAT				)	//64	귀묘 음파 공격. 
	SET_PROTOCOL( ATTACKATB_DOLL1					)	//65	무명 인형 공격. 
	SET_PROTOCOL( ATTACKATB_DOLL2					)	//66	하카다 인형 공격. 
	SET_PROTOCOL( ATTACKATB_JUSTICEK				)	//67	조선 유생 공격
	SET_PROTOCOL( ATTACKATB_BUTTERFLY				)	//68	독나방 공격 
	SET_PROTOCOL( ATTACKATB_WIDOW					)	//69	거미요괴 공격 
	SET_PROTOCOL( ATTACKATB_WAVE					)	//70	바다거북이 파동공격. 
	SET_PROTOCOL( ATTACKATB_FIREUSINGMANA			)	//71	불공격(마법 사용)
	SET_PROTOCOL( ATTACKATB_LEECH					)	//72	왕거머리. 
	SET_PROTOCOL( ATTACKATB_SEAHORSE				)	//73	해마물쏘기. 
	SET_PROTOCOL( ATTACKATB_MACHINEGUN				)	//74	연발 사격 
	SET_PROTOCOL( ATTACKATB_THROWSPEAR				)	//75	던지는창 (원주민)  
	SET_PROTOCOL( ATTACKATB_SCORPION				)	//76	독전갈 독. 
	SET_PROTOCOL( ATTACKATB_CACTUS					)	//77	괴물선인장 가시. 
	SET_PROTOCOL( ATTACKATB_CLAW					)	//78	조도의 공격속성. 
	SET_PROTOCOL( ATTACKATB_MAGICIAN				)	//79	요술사 마법공격.  
	SET_PROTOCOL( ATTACKATB_BIGTREE					)	//80	거대수 나뭇가지 공격. 
	SET_PROTOCOL( ATTACKATB_COBRA					)	//81	코브라 독 공격.
	SET_PROTOCOL( ATTACKATB_VOLCANOFLAME			)	//82	화산의불꽃.   불덩이.
	SET_PROTOCOL( ATTACKATB_EVIL					)	//83	이사의 사념.
	SET_PROTOCOL( ATTACKATB_ICE						)	//84	이사의 사념 특수기술.
	SET_PROTOCOL( ATTACKATB_MAGIC4T					)	//85	징키스칸혼 특수 기술.
	SET_PROTOCOL( ATTACKATB_JAPANGHOST				)	//86	일본귀신 특수 기술.
	SET_PROTOCOL( ATTACKATB_KUKET					)	//87	쿠켓
	SET_PROTOCOL( ATTACKATB_AUTA					)	//88	아우타 공격.
	SET_PROTOCOL( ATTACKATB_SUB1					)	//89	진성여왕 화살공격 1
	SET_PROTOCOL( ATTACKATB_SUB2					)	//90	진성여왕 화살공격 2
	SET_PROTOCOL( ATTACKATB_FIREATTACK				)	//91	화공신포.
	SET_PROTOCOL( ATTACKATB_RUNNINGFIRE				)	//92	연발
	SET_PROTOCOL( ATTACKATB_ENERGYPA				)	//93	장풍.
	SET_PROTOCOL( ATTACKATB_LONG_SPEAR				)	//94	손유창 장창소환.
	SET_PROTOCOL( ATTACKATB_PAPERDOLL				)	//95	음양사 종이인형
	SET_PROTOCOL( ATTACKATB_NEEDLEATTACK			)	//96	침공격
	SET_PROTOCOL( ATTACKATB_CLOUDWIND				)	//97	풍모술
	SET_PROTOCOL( ATTACKATB_STONEMAGIC				)	//98	석괴술
	SET_PROTOCOL( ATTACKATB_GODSPEAR				)	//99	신창노도
	SET_PROTOCOL( ATTACKATB_FLOWERRAIN				)	//100	만천화우
	SET_PROTOCOL( ATTACKATB_SOULCHANGE				)	//101	빙의술
	SET_PROTOCOL( ATTACKATB_MAGICCATTACK			)	//102	도술사 공격
	SET_PROTOCOL( ATTACKATB_SUNBIYOUN				)	//103	순비연 공격
	SET_PROTOCOL( ATTACKATB_SNOW					)	//104	눈덩이

	// actdoll (2004/03/06 5:00) : 3월 패치관련 추가
	SET_PROTOCOL( ATTACKATB_PYTHON					)	//105	이무기 마법공격	// actdoll (2004/03/06 4:08) : 추가
	SET_PROTOCOL( ATTACKATB_JINEMPEROR				)	//106	진시황의혼 마법공격	// actdoll (2004/03/06 4:08) : 추가
	SET_PROTOCOL( ATTACKATB_GUARDIAN_SNAKE			)	//107	수호부(뱀)
	SET_PROTOCOL( ATTACKATB_GUARDIAN_PIG			)	//108	수호부(돼지)
	SET_PROTOCOL( ATTACKATB_GUARDIAN_CHICKEN		)	//109	수호부(닭)

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:48) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	SET_PROTOCOL( ATTACKATB_TURTLE_VEHICLE			)	// 110	조선-거북차, 이순신 2차 전직
	SET_PROTOCOL( ATTACKATB_THUNDER_VEHICLE			)	// 111	조선-뇌전차, 허준 2차 전직
	SET_PROTOCOL( ATTACKATB_QUAKE_VEHICLE			)	// 112	일본-지진차, 와키자카 2차 전직
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_VEHICLE	)	// 113	일본-흑룡차, 세이메이 2차 전직
	SET_PROTOCOL( ATTACKATB_CATAPULT				)	// 114	중국-발석거, 순비연<=이령 2차 전직
	SET_PROTOCOL( ATTACKATB_FARANGI_VEHICLE			)	// 115	중국-불랑기포차, 이령<=순비연 2차 전직
	SET_PROTOCOL( ATTACKATB_FIRE_DRAGON_VEHICLE		)	// 116	대만-화룡차, 손유창 2차 전직
	SET_PROTOCOL( ATTACKATB_CHINESEPHOENIX			)	// 117	대만-봉황비조, 장선화 2차 전직

	SET_PROTOCOL( ATTACKATB_CATDOLL					)	// 118  고양이 인형.
	SET_PROTOCOL( ATTACKATB_OLDFOXDOLL				)	// 119  구미호 인형.
	SET_PROTOCOL( ATTACKATB_RACCOONDOLL				)   // 120	너구리 인형.
	SET_PROTOCOL( ATTACKATB_NINJADOLL				)   // 121  닌자 인형.
	SET_PROTOCOL( ATTACKATB_SNOWWOMANDOOL			)   // 122  설녀 인형.
	SET_PROTOCOL( ATTACKATB_YANGGUIBIDOLL			)	// 123  양귀비 인형.
	SET_PROTOCOL( ATTACKATB_JIJANGDOLL				)	// 124  지장보살 인형.
	SET_PROTOCOL( ATTACKATB_SNAKEWOMANDOLL			)	// 125  사녀 인형.

	// robypark (2004/6/4 13:6)
	// 앙천대소 공격
	SET_PROTOCOL( ATTACKATB_ROAR_OF_LAUGHTER		)	// 126	불랑기포차 앙천대소 공격

	// robypark 2004/6/10 19:4
	// 흑룡차 천지독살, 냉기공격
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_ICE		)	// 127

	// robypark 2004/6/11 10:49
	// 흑룡차 천지독살, 독공격
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_POISON		)	// 128

	// robypark 2004/7/14 16:41
	// 에비스 인형 공격
	SET_PROTOCOL( ATTACKATB_DOLL_EVIS				)	// 129

	// robypark 2005/2/2 17:27
	// 달마 인형 공격
	SET_PROTOCOL( ATTACKATB_DOLL_DHARMA				)	// 130

// EOF
#undef	SET_PROTOCOL		
}
