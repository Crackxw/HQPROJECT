//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: 임진록 온라인 [거상] - KindInfo 수열값 (캐릭터 ID)		
//	File Name		: Enum_KI_Character.cpp
//	Birth Date		: 2004. 03. 18.	
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		거상에서 사용되는 게임용 캐릭터인덱스를 수열화 시킨 데이터이며, 이 내용은 프로그램 컴파일에 첨부됩니다.
//		캐릭터를 추가할 때에는 특별한 경우를 제외하고 끝부분과 KIND_ON_MAX 사이에 추가하시기 바랍니다.				
//						
//===================================================================================================						

#include <GSL.h>

#include <string.h>
#include "Enum_KI_Character.h"

CLASS_KIND_ON_CHARACTER::DATA	CLASS_KIND_ON_CHARACTER::m_Data[KIND_ON_MAX]	= {0,};	// 스태틱 변수 초기화
CLASS_KIND_ON_CHARACTER			g_clKICharacter;										// 초기화 가동용 임시 전역변수

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: 해당 수열 인덱스와 거기에 할당될 실제 이름을 배치시킨다.
//		index		- 수열값
//		pszStr		- 해당 수열값의 이름
//-----------------------------------------------------------------------------
void	CLASS_KIND_ON_CHARACTER::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= KIND_ON_FIRST || index >= KIND_ON_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		pszStr		- 찾을 수열값의 이름
//		return		- 해당 수열명에 할당된 수열값. 범위가 수열을 넘겼다면 KIND_ON_INVALID를 리턴
//-----------------------------------------------------------------------------
ENUM_KIND_ON_CHARACTER	CLASS_KIND_ON_CHARACTER::GetData( char *pszStr )
{
	if( !pszStr )		return KIND_ON_INVALID;
	for( register int i=KIND_ON_FIRST; i<KIND_ON_MAX; i++ )	
	{
		if( strcmp( m_Data[i].m_pszString, pszStr ) == 0 ) return (ENUM_KIND_ON_CHARACTER)i;
	}
	return KIND_ON_INVALID;
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		enIndex		- 찾을 수열값
//		return		- 해당 수열값의 수열명. 없으면 NULL을 리턴. 배당이 안됐다면 스트링은 ""
//-----------------------------------------------------------------------------
const char* CLASS_KIND_ON_CHARACTER::GetData( ENUM_KIND_ON_CHARACTER enIndex )
{
	if( enIndex <= KIND_ON_FIRST || enIndex >= KIND_ON_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_KIND_ON_CHARACTER()
// Code: actdoll (2004-03-18)
// Desc: 소멸자
//-----------------------------------------------------------------------------
CLASS_KIND_ON_CHARACTER::~CLASS_KIND_ON_CHARACTER()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_KIND_ON_CHARACTER()
// Code: actdoll (2004-03-18)
// Desc: 생성자(아주중요!!!)
//-----------------------------------------------------------------------------
CLASS_KIND_ON_CHARACTER::CLASS_KIND_ON_CHARACTER()
{
	memset( m_Data, 0, (sizeof(DATA) * KIND_ON_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// 수열이 추가될 경우 이 부분 맨 아래 (//EOF앞쪽)에 추가시킨다.
	
	SET_PROTOCOL( KIND_PLAYERSTART					)	//1		식별자(구분값)
		
	// 운영자 					
	SET_PROTOCOL( KIND_MANAGER1						)	//2		운영자1
	
	// 주인공					
	SET_PROTOCOL( KIND_ON_KEON						)	//3		조철희
	SET_PROTOCOL( KIND_ON_MYUNGHWA					)	//4		이명화
	SET_PROTOCOL( KIND_ON_RYO						)	//5		시마다 료
	SET_PROTOCOL( KIND_ON_HANAHINE					)	//6		겐노 하나히네
	SET_PROTOCOL( KIND_ON_SORYUNG					)	//7		왕소룡
	SET_PROTOCOL( KIND_ON_MIRYUNG					)	//8		왕미령
	SET_PROTOCOL( KIND_ON_DANG						)	//9		당호엽
	SET_PROTOCOL( KIND_ON_SUYOUN					)	//10	노수연
	
	// 조선 용병 					
	SET_PROTOCOL( KIND_ON_HORSEARCHERK				)	//15	기마궁수
	SET_PROTOCOL( KIND_ON_JUSTICEK					)	//16	유생 
	SET_PROTOCOL( KIND_ON_BUDAK						)	//17	파계승
	SET_PROTOCOL( KIND_ON_SWORDK					)	//18	창잡이 
	SET_PROTOCOL( KIND_ON_ARCHERK					)	//19	활잡이
	SET_PROTOCOL( KIND_ON_BUTCHER					)	//20	칼잡이
	SET_PROTOCOL( KIND_ON_PORTERK					)	//21	조선짐꾼
	
	SET_PROTOCOL( KIND_ON_HORSEARCHERUPK			)	//22	고급기마궁수
	SET_PROTOCOL( KIND_ON_JUSTICEUPK				)	//23	고급유생
	SET_PROTOCOL( KIND_ON_BUDAUPK					)	//24	고급파계승
	SET_PROTOCOL( KIND_ON_SWORDUPK					)	//25	고급창잡이
	SET_PROTOCOL( KIND_ON_ARCHERUPK					)	//26	고급활잡이
	
	SET_PROTOCOL( KIND_GENERAL1K					)	//27	권율
	SET_PROTOCOL( KIND_GENERAL2K					)	//28	이순신
	SET_PROTOCOL( KIND_GENERAL3K					)	//29	유성룡
	SET_PROTOCOL( KIND_GENERAL4K					)	//30	사명대사
	SET_PROTOCOL( KIND_GENERAL5K					)	//31	신립
	SET_PROTOCOL( KIND_GENERAL6K					)	//32	허준(안씀.아래다른정보가있음)
	SET_PROTOCOL( KIND_GENERAL7K					)	//33	김덕령(안씀)
	SET_PROTOCOL( KIND_GENERAL8K					)	//34	김시민
	SET_PROTOCOL( KIND_CANNON2K						)	//35	대장군포(안씀)
	
	// 일본 용병 					
	SET_PROTOCOL( KIND_ON_SWORDJ					)	//40	검술낭인
	SET_PROTOCOL( KIND_ON_GUNJ						)	//41	조총낭인
	SET_PROTOCOL( KIND_ON_NINJAJ					)	//42	닌자
	SET_PROTOCOL( KIND_ON_HORSESWORDJ				)	//43	늑대낭인
	SET_PROTOCOL( KIND_ON_ADVBUDAJ					)	//44	퇴마사 
	SET_PROTOCOL( KIND_ON_DSWORD					)	//45	기마무사
	SET_PROTOCOL( KIND_ON_PORTERJ					)	//46	일본짐꾼
	
	SET_PROTOCOL( KIND_ON_SWORDUPJ					)	//47	고급검술낭인
	SET_PROTOCOL( KIND_ON_GUNUPJ					)	//48	고급조총낭인
	SET_PROTOCOL( KIND_ON_NINJAUPJ					)	//49	고급닌자
	SET_PROTOCOL( KIND_ON_HORSESWORDUPJ				)	//50	고급늑대낭인
	SET_PROTOCOL( KIND_ON_ADVBUDAUPJ				)	//51	고급퇴마사 
	
	SET_PROTOCOL( KIND_GENERAL1J					)	//52	고니시
	SET_PROTOCOL( KIND_GENERAL2J					)	//53	가토오
	SET_PROTOCOL( KIND_GENERAL3J					)	//54	와키자카
	SET_PROTOCOL( KIND_GENERAL4J					)	//55	세이쇼오
	SET_PROTOCOL( KIND_GENERAL5J					)	//56	우기다(안씀)
	SET_PROTOCOL( KIND_GENERAL6J					)	//57	도쿠가와
	SET_PROTOCOL( KIND_GENERAL7J					)	//58	미츠나리(안씀)
	SET_PROTOCOL( KIND_GENERAL8J					)	//59	아사코
	
	// 대만 용병 					
	SET_PROTOCOL( KIND_ON_BUDAT						)	//63	주술사
	SET_PROTOCOL( KIND_ON_AMAZONT					)	//64	여전사
	SET_PROTOCOL( KIND_ON_AXET						)	//65	도끼거한 
	SET_PROTOCOL( KIND_ON_SWORDT					)	//66	봉술가
	SET_PROTOCOL( KIND_ON_GUNT						)	//67	서양총수
	SET_PROTOCOL( KIND_ON_PORTERT					)	//68	대만짐꾼
	SET_PROTOCOL( KIND_ON_BEASTWARRIOR				)	//69	야수전사
	
	SET_PROTOCOL( KIND_ON_BUDAUPT					)	//70	고급주술사
	SET_PROTOCOL( KIND_ON_AMAZONUPT					)	//71	고급여전사
	SET_PROTOCOL( KIND_ON_AXEUPT					)	//72	고급도끼거한 
	SET_PROTOCOL( KIND_ON_SWORDUPT					)	//73	고급봉술가
	SET_PROTOCOL( KIND_ON_GUNUPT					)	//74	고급서양총수
	
	SET_PROTOCOL( KIND_ON_GENERAL1T					)	//75	왕거한 
	SET_PROTOCOL( KIND_ON_GENERAL2T					)	//76	페르난데스
	SET_PROTOCOL( KIND_ON_GENERAL3T					)	//77	임평후
	SET_PROTOCOL( KIND_ON_GENERAL4T					)	//78	유영복
	SET_PROTOCOL( KIND_ON_GENERAL5T					)	//79	조세림
	SET_PROTOCOL( KIND_ON_GENERAL6T					)	//80	손유창
	
	// 중국 용병					
	SET_PROTOCOL( KIND_ON_FAN_C						)	//85	모험가
	SET_PROTOCOL( KIND_ON_BUDAK_C					)	//86	수도승
	SET_PROTOCOL( KIND_ON_MUDOGA_C					)	//87	무도가
	SET_PROTOCOL( KIND_ON_SWORD_C					)	//88	큰칼무사
	SET_PROTOCOL( KIND_ON_FIREGUN_C					)	//89	화포수
	SET_PROTOCOL( KIND_ON_MAGIC_C					)	//90	도술사
	SET_PROTOCOL( KIND_ON_PORTER_C					)	//91	중국짐꾼
	
	SET_PROTOCOL( KIND_ON_FANUP_C					)	//92	고급모험가
	SET_PROTOCOL( KIND_ON_BUDAKUP_C					)	//93	고급수도승
	SET_PROTOCOL( KIND_ON_MUDOGAUP_C				)	//94	고급무도가
	SET_PROTOCOL( KIND_ON_SWORDUP_C					)	//95	고급큰칼무사
	SET_PROTOCOL( KIND_ON_FIREGUNUP_C				)	//96	고급화포수
	
	SET_PROTOCOL( KIND_ON_GENERAL1_C				)	//97	동방불패
	SET_PROTOCOL( KIND_ON_GENERAL2_C				)	//98	이령
	SET_PROTOCOL( KIND_ON_GENERAL3_C				)	//99	제갈공명
	SET_PROTOCOL( KIND_ON_GENERAL4_C				)	//100	조승훈
	SET_PROTOCOL( KIND_ON_GENERAL5_C				)	//101	축융
	SET_PROTOCOL( KIND_ON_GENERAL6_C				)	//102	순비연
	
	// 몬스터 					
	SET_PROTOCOL( KIND_ON_HARUBANG					)	//107	돌하루방
	SET_PROTOCOL( KIND_ON_CRAB						)	//108	바다게
	SET_PROTOCOL( KIND_ON_LEECH						)	//109	왕거머리
	SET_PROTOCOL( KIND_ON_SEAHORSE					)	//110	해마 
	SET_PROTOCOL( KIND_WHITETIGER					)	//111	백호 
	SET_PROTOCOL( KIND_GHOSTCAT						)	//112	귀묘 
	SET_PROTOCOL( KIND_TUTTLE						)	//113	바다거북이
	SET_PROTOCOL( KIND_BUTTERFLY					)	//114	독나방
	SET_PROTOCOL( KIND_WIDOW						)	//115	거미요괴
	SET_PROTOCOL( KIND_INSECT						)	//116	사마귀
	SET_PROTOCOL( KIND_WORRIORGHOST					)	//117	무관의혼
	SET_PROTOCOL( KIND_SINGERMGHOST					)	//118	신검의혼
	SET_PROTOCOL( KIND_MAMMOTH						)	//119	눈코끼리
	SET_PROTOCOL( KIND_WILDBOAR						)	//120	멧돼지
	SET_PROTOCOL( KIND_ON_THIEF						)	//121	도굴꾼 
	SET_PROTOCOL( KIND_ON_SEAKING					)	//122	해저왕 
	SET_PROTOCOL( KIND_ON_SWORDPIRATE				)	//123	왜구
	SET_PROTOCOL( KIND_ON_GUNPIRATE					)	//124	조총왜구
	SET_PROTOCOL( KIND_ON_SNOWWOMAN					)	//125	설녀
	SET_PROTOCOL( KIND_ON_EATERDEMON				)	//126   식귀
	SET_PROTOCOL( KIND_ON_WEIRDPLANT				)	//127	요수목
	SET_PROTOCOL( KIND_ON_HUNGRYDEMON				)	//128	아귀
	SET_PROTOCOL( KIND_ON_TIGER						)	//129	호랑이
	SET_PROTOCOL( KIND_ON_DOGDEMON					)	//130	견신
	SET_PROTOCOL( KIND_ON_ONGHWA					)	//131	온천원숭이
	SET_PROTOCOL( KIND_ON_BANDIT1					)	//132	꼬마산적
	SET_PROTOCOL( KIND_ON_BANDIT2					)	//133	산적
	SET_PROTOCOL( KIND_ON_BANDIT3					)	//134	철퇴산적
	SET_PROTOCOL( KIND_ON_BANDIT4					)	//135	궁수산적
	SET_PROTOCOL( KIND_ON_GACKDAGUI					)	//136	각다귀
	SET_PROTOCOL( KIND_ON_GATPHA					)	//137	갓파
	SET_PROTOCOL( KIND_ON_BOGY						)	//138	도깨비
	SET_PROTOCOL( KIND_ON_FOX						)	//139	구미호
	SET_PROTOCOL( KIND_ON_STARFISH					)	//140	불가사리
	SET_PROTOCOL( KIND_ON_SHIP1						)	//141	배1
	SET_PROTOCOL( KIND_ON_ONI						)	//142	오니
	SET_PROTOCOL( KIND_ON_SNOWDEMON					)	//143	설인
	SET_PROTOCOL( KIND_ON_FURHEADDEMON				)	//144	털가면
	SET_PROTOCOL( KIND_ON_FIREDRAGON				)	//145	화룡
	SET_PROTOCOL( KIND_ON_GOLDDRAGON				)	//146	황룡
	SET_PROTOCOL( KIND_ON_BAT						)	//147	박쥐
	SET_PROTOCOL( KIND_ON_TOAD						)	//148	두꺼비
	SET_PROTOCOL( KIND_ON_CENTIPEDE					)	//149	독지네
	SET_PROTOCOL( KIND_ON_DUEOKSINI					)	//150	두억시니
	SET_PROTOCOL( KIND_ON_GANGSI					)	//151	강시
	SET_PROTOCOL( KIND_ON_RACCOONDOG				)	//152	너구리
	SET_PROTOCOL( KIND_ON_DEER						)	//153	사슴
	SET_PROTOCOL( KIND_ON_BEAR						)	//154	반달곰 
	SET_PROTOCOL( KIND_ON_HORSESPEAR				)	//155	기마창수 
	SET_PROTOCOL( KIND_ON_CAT						)	//156	도둑고양이
	SET_PROTOCOL( KIND_ON_DUCK						)	//157	대만오리
	SET_PROTOCOL( KIND_ON_HORNET					)	//158	대만왕벌
	SET_PROTOCOL( KIND_ON_MBANDIT1					)	//159	마적두목 
	SET_PROTOCOL( KIND_ON_MBANDIT2					)	//160	망치마적 
	SET_PROTOCOL( KIND_ON_MBANDIT3					)	//161	총수마적 
	SET_PROTOCOL( KIND_ON_CHAWOO					)	//162	차우차우 
	SET_PROTOCOL( KIND_ON_PIRATES					)	//163	해적 
	SET_PROTOCOL( KIND_ON_PANDER					)	//164	팬더 
	SET_PROTOCOL( KIND_ON_SNAKE						)	//165	구렁이 
	SET_PROTOCOL( KIND_ON_CROCODILE					)	//166	식인악어 
	SET_PROTOCOL( KIND_ON_HOOK						)	//167	갈고리해적 
	SET_PROTOCOL( KIND_ON_SMALLGANG					)	//168	졸개마적
	SET_PROTOCOL( KIND_ON_HORSEMAN					)	//169	마두인
	SET_PROTOCOL( KIND_ON_BABARIAN					)	//170	원주민
	SET_PROTOCOL( KIND_ON_BABARIANSHAMAN			)	//171	원주민무당
	SET_PROTOCOL( KIND_ON_BADDEER					)	//172	루돌이
	SET_PROTOCOL( KIND_ON_CACTUS					)	//173	꼬마선인장
	SET_PROTOCOL( KIND_ON_SCORPION					)	//174	독전갈 
	SET_PROTOCOL( KIND_ON_SKYNINE					)	//175	천구
	SET_PROTOCOL( KIND_ON_ICETREE					)	//176	얼음나무
	SET_PROTOCOL( KIND_ON_SNOWLIZARD				)	//177	눈도마뱀
	SET_PROTOCOL( KIND_ON_MADBULL					)	//178	광우
	SET_PROTOCOL( KIND_ON_BIGTREE					)	//179	거대수
	SET_PROTOCOL( KIND_ON_MAGICIAN					)	//180	요술사 
	SET_PROTOCOL( KIND_ON_BADMERCHANT				)	//181	악덕상인 
	SET_PROTOCOL( KIND_ON_RUNNERHAND				)	//182	괴물덩굴손
	SET_PROTOCOL( KIND_ON_SANDGOD					)	//183	모래의신
	SET_PROTOCOL( KIND_ON_SEAENVOY					)	//184	바다의사신
	SET_PROTOCOL( KIND_ON_VIPER						)	//185	살무사
	SET_PROTOCOL( KIND_ON_AMAKUSA					)	//186	아마쿠사
	SET_PROTOCOL( KIND_ON_LAVAMONSTER				)	//187	용암괴물
	SET_PROTOCOL( KIND_ON_EVIL						)	//188	이사의 사념
	SET_PROTOCOL( KIND_ON_BIRD						)	//189	인면조
	SET_PROTOCOL( KIND_ON_WEASEL					)	//190	족제비
	SET_PROTOCOL( KIND_ON_COBRA						)	//191	코브라
	SET_PROTOCOL( KIND_ON_SOUL						)	//192	징기스칸의 혼
	SET_PROTOCOL( KIND_ON_VOLCANOFLAME				)	//193	화산의 불꽃
	SET_PROTOCOL( KIND_ON_REDMADBULL				)	//194	엄마광우
	SET_PROTOCOL( KIND_ON_MAGICOFFICER				)	//195	요술사령
	SET_PROTOCOL( KIND_ON_CAPBEAR					)	//196	대장반달곰
	SET_PROTOCOL( KIND_ON_KOREANGHOST				)	//197	처녀귀신
	SET_PROTOCOL( KIND_ON_SKELETON					)	//198	해골
	SET_PROTOCOL( KIND_ON_SKELETONSOLDIER			)	//199	해골병사
	SET_PROTOCOL( KIND_ON_PIRATECAPTAIN				)	//200	해적선장
	SET_PROTOCOL( KIND_ON_TAIWANGHOST				)	//201	대만귀신
	SET_PROTOCOL( KIND_ON_JAPANGHOST				)	//202	일본귀신
	SET_PROTOCOL( KIND_ON_AUTA						)	//203	아우타
	SET_PROTOCOL( KIND_ON_JUCK						)	//204	적고적
	SET_PROTOCOL( KIND_ON_FEMALESHAMAN				)	//205	무녀
	SET_PROTOCOL( KIND_ON_QUEEN						)	//206	진성여왕
	SET_PROTOCOL( KIND_ON_DONKEY					)	//207	당나귀
	SET_PROTOCOL( KIND_ON_AGILETIGER				)	//208	비호
	SET_PROTOCOL( KIND_ON_PAWNTIGER					)	//209	졸개호랑이
	SET_PROTOCOL( KIND_ON_OWL						)	//210	올빼미
	SET_PROTOCOL( KIND_ON_LEOPARD					)	//211	표범
	SET_PROTOCOL( KIND_ON_HUNSWARRIOR				)	//212	흉노전사
	SET_PROTOCOL( KIND_ON_HUNSHORSE					)	//213	흉노기마병
	SET_PROTOCOL( KIND_ON_HUNSHORSEARCHER			)	//214	흉노기궁병
	SET_PROTOCOL( KIND_ON_GATEKEEPER				)	//215	문지기
	SET_PROTOCOL( KIND_ON_CLAYFIGURE				)	//216	기마병마용
	SET_PROTOCOL( KIND_ON_PHOENIX					)	//217	불사조
	SET_PROTOCOL( KIND_ON_JINEMPEROR				)	//218	진시황의혼
	SET_PROTOCOL( KIND_ON_MEDICALMAN				)	//219	의술사
	SET_PROTOCOL( KIND_ON_HEOJUN					)	//220	허준
	SET_PROTOCOL( KIND_ON_COSMIC					)	//221	음양사
	SET_PROTOCOL( KIND_ON_SEIMEI					)	//222	세이메이
	SET_PROTOCOL( KIND_ON_YUMRUCK					)	//223	염력사
	SET_PROTOCOL( KIND_ON_JANGSUNHWA				)	//224	장선화
	SET_PROTOCOL( KIND_ON_LONGSPEAR					)	//225	장창무사
	SET_PROTOCOL( KIND_ON_KOAKHU					)	//226	곽후
	SET_PROTOCOL( KIND_ON_SNOWMAN1					)	//227	눈사람1
	SET_PROTOCOL( KIND_ON_SNOWMAN2					)	//228	눈사람2
	SET_PROTOCOL( KIND_ON_SANTA1					)	//229	가짜산타1
	SET_PROTOCOL( KIND_ON_SANTA2					)	//230	가짜산타2
	SET_PROTOCOL( KIND_ON_SANTA3					)	//231	가짜산타3
	SET_PROTOCOL( KIND_ON_MOTHER_DEER1				)	//232	엄마루돌이1
	SET_PROTOCOL( KIND_ON_MOTHER_DEER2				)	//233	엄마루돌이2
	SET_PROTOCOL( KIND_ON_MOTHER_DEER3				)	//234	엄마루돌이3
	SET_PROTOCOL( KIND_ON_MOTHER_DEER4				)	//235	엄마루돌이4
	SET_PROTOCOL( KIND_ON_SANTA4					)	//236	가짜산타 4
	SET_PROTOCOL( KIND_ON_SANTA_TW					)	//237	대만가짜산타
	SET_PROTOCOL( KIND_ON_MOTHER_DEER_TW			)	//238	대만엄마루돌이
	SET_PROTOCOL( KIND_ON_SHIP2						)	//239	배2
	SET_PROTOCOL( KIND_ON_SHIP3						)	//240	배3
	SET_PROTOCOL( KIND_ON_SHIP4						)	//241	배4
	SET_PROTOCOL( KIND_ON_SHIP5						)	//242	배5
	SET_PROTOCOL( KIND_ON_SHIP6						)	//243	배6
	SET_PROTOCOL( KIND_ON_SHIP7						)	//244	배7
	SET_PROTOCOL( KIND_ON_SHIP8						)	//245	배8
	SET_PROTOCOL( KIND_ON_FLYDRAGON					)	//246	익룡
	SET_PROTOCOL( KIND_ON_RAPTOR					)	//247	랩터
	SET_PROTOCOL( KIND_ON_TIRANO					)	//248	티라노사우르스
	SET_PROTOCOL( KIND_ON_MONKEY					)	//249	원숭이
	
	// actdoll (2004/02/26 11:12) : 추가된 몬스터 - 유명계 맵용					
	SET_PROTOCOL( KIND_ON_HELLKING					)	//250	염라대왕
	SET_PROTOCOL( KIND_ON_HELLMESSENGER				)	//251	저승사자
	SET_PROTOCOL( KIND_ON_GHOSTSAMURAI				)	//252	유령 사무라이
	SET_PROTOCOL( KIND_ON_PYTHON					)	//253	이무기
	SET_PROTOCOL( KIND_ON_GHOSTELEPHANT				)	//254	유령코끼리
	SET_PROTOCOL( KING_ON_GHOSTSEAKING				)	//255	유령해저왕
	SET_PROTOCOL( KIND_ON_GHOSTPIRATECAPTAIN		)	//256	유령해적선장
	SET_PROTOCOL( KIND_ON_GHOSTSKELETON				)	//257	유령해골
	SET_PROTOCOL( KIND_ON_GHOSTSKELETONSOLDIER		)	//258	유령해골병사
	SET_PROTOCOL( KIND_ON_GHOSTFEMALESHAMAN			)	//269	유령무녀
	SET_PROTOCOL( KIND_ON_GHOSTVOLCANOFLAME			)	//260	유령불꽃
	
	// 수호부 추가캐릭터			
	SET_PROTOCOL( KIND_ON_GUARDIAN_MOUSE			)	//261	수호부용(쥐)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW1				)	//262	수호부용(소1)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW2				)	//263	수호부용(소2)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW3				)	//264	수호부용(소3)
	
	SET_PROTOCOL( KIND_ON_GUARDIAN_CHICK			)	//265	수호부용(병아리)
	SET_PROTOCOL( KIND_ON_GUARDIAN_PIG				)	//266	수호부용(돼지)
	SET_PROTOCOL( KIND_ON_GUARDIAN_SNAKE			)	//267	수호부용(뱀)
	SET_PROTOCOL( KIND_ON_GUARDIAN_MONKEY			)	//268	수호부용(원숭이)
	SET_PROTOCOL( KIND_ON_JAP_INVADERS				)	//269	침략왜구
		
//	장수 2차 전직(2004-5-11, robypark)
	SET_PROTOCOL( KIND_ON_TURTLE_VEHICLE			)	// 270	U102	조선-거북차
	SET_PROTOCOL( KIND_ON_THUNDER_VEHICLE			)	// 271	U103	조선-뇌전차
	SET_PROTOCOL( KIND_ON_QUAKE_VEHICLE				)	// 272	U104	일본-지진차
	SET_PROTOCOL( KIND_ON_BLACK_DRAGON_VEHICLE		)	// 273	U105	일본-흑룡차
	SET_PROTOCOL( KIND_ON_CATAPULT					)	// 274	U106	중국-발석거
	SET_PROTOCOL( KIND_ON_FARANGI_VEHICLE			)	// 275	U107	중국-불랑기포차
	SET_PROTOCOL( KIND_ON_FIRE_DRAGON_VEHICLE		)	// 276	U108	대만-화룡차
	SET_PROTOCOL( KIND_ON_CHINESEPHOENIX			)	// 277	U109	대만-봉황비조

// 2차 장수 스킬 추가 관련
	// robypark 2004/6/4 20:29
	// 불랑기포차 기술 앙천대소를 위한 캐릭터
	SET_PROTOCOL( KIND_ON_ROAR_OF_LAUGHTER			)	// 278

//#ifdef _SET_CHARACTER_LIST
	// robypark 2004/7/22 16:18
	// 중국용병 - 도술사(레벨 20이상이 되면 고급용병이미지로 변경된다.)
	SET_PROTOCOL( KIND_ON_MAGIC_C_UP				)	// 279(U110)

	// robypark 2004/8/26 18:58
	// 대만 용병 - 야수전사 => 고급 야수전사
	SET_PROTOCOL( KIND_ON_BEASTWARRIOR_UP			)	// 280(U111)
	// 대만 용병 - 염력사 => 고급 염력사
	SET_PROTOCOL( KIND_ON_YUMRUCK_UP				)	// 281(U112)
	// 일본 용병 - 음양사 => 고급 음양사
	SET_PROTOCOL( KIND_ON_COSMIC_UP					)	// 282(U113)
	// 조선 용병 - 의술사 => 고급 의술사
	SET_PROTOCOL( KIND_ON_MEDICALMAN_UP				)	// 283(U114)
	// 조선 용병 - 칼잡이 => 고급 칼잡이
	SET_PROTOCOL( KIND_ON_BUTCHER_UP				)	// 284(U115)
	// 중국 용병 - 장창무사 => 고급 장창무사
	SET_PROTOCOL( KIND_ON_LONGSPEAR_UP				)	// 285(U116)
//#endif

	// robypark 2004/9/17 13:35
	// 붉은 눈 해저인
	SET_PROTOCOL( KIND_ON_RED_EYE_SEABEDMAN			)	// 286(M147)

	// 가짜 흉노전사
	SET_PROTOCOL( KIND_ON_HUNSWARRIOR_FAKE			)	// 287(M148)

	// 시작 /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 16:27
	// 공성전 관련 캐릭터 추가
	// 공성전 - 검병
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SWORDMAN		)	// 288
	// 공성전 - 창병
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SPEARMAN		)	// 289
	// 공성전 - 기마병
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_HORSEMAN		)	// 290
	// 공성전 - 궁병
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_ARCHERMAN	)	// 291
	// 공성전 - 충차
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SIEGE_TOWER	)	// 292
	// 공성전 - 발석거
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_CATAPULT		)	// 293
	// 공성전 - 보급 수레
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SUPPLY_WAGON	)	// 294
	// 끝 /////////////////////////////////////////////////////////////////

	// 시작 /////////////////////////////////////////////////////////////////
	// robypark 2004/11/4 17:0
	// 문화 유적 이벤트 몬스터
	// 저팔계 삼계탕, M149
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE1			)	// 295
	// 저팔계 장금복, M150
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE2			)	// 296
	// 끝 /////////////////////////////////////////////////////////////////

	// 공성전 - 수비병, U121
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_GARRISON_ARMY)	// 297

	// robypark 2004/11/22 18:6
	// 저팔계 무영풍체험판, M151
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE3			)	// 298

	// robypark 2005/1/14 17:49
	// 공성전 피난민 이미지활용을 위한 캐릭터 추가
	SET_PROTOCOL( KIND_ON_REFUGEE_KEON				)	// 299, 피난민 조철희
	SET_PROTOCOL( KIND_ON_REFUGEE_MYUNGHWA			)	// 300, 피난민 이명화
	SET_PROTOCOL( KIND_ON_REFUGEE_RYO				)	// 301, 피난민 시마다료
	SET_PROTOCOL( KIND_ON_REFUGEE_HANAHINE			)	// 302, 피난민 겐노하나히네
	SET_PROTOCOL( KIND_ON_REFUGEE_SORYUNG			)	// 303, 피난민 왕소룡
	SET_PROTOCOL( KIND_ON_REFUGEE_MIRYUNG			)	// 304, 피난민 왕미령
	SET_PROTOCOL( KIND_ON_REFUGEE_DANG				)	// 305, 피난민 당호엽
	SET_PROTOCOL( KIND_ON_REFUGEE_SUYOUN			)	// 306, 피난민 노수연

// EOF
#undef	SET_PROTOCOL		
}
