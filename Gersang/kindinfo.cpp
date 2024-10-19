//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 캐릭터별 각종 정보 적재 클래스
//	File Name		: KindInfo.cpp
//	Birth Date		: 2003. 12. 30.
//	Creator			: 거 상 팀
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		클라이언트에서 사용되는 모든 캐릭터에 대한 초상 및 구획 등을 초기화하는 클래스입니다.
//		여기에서 해당 초상 및 게임 정보가 로딩되므로 매우 중요한 클래스입니다.
//		기존 코드에서는 하드 코딩을 통해 정보를 늘여가는 방식을 사용하고 있습니다만,
//		로딩에 필요한 주요 정보를 자료 파일로 빼내 소스의 간결성 및 자료의 정규화(Normalization)를
//		추구할 수 있도록 작업을 진행할 계획입니다.
//
//===================================================================================================

#include <GSL.h>

//#include <main.h>
#include "OnlineWorld.h"
#include <player.h>
#include "music.h"
#include <OnlineImjin2i.h>
#include <Etc.h>

#include "text.h"
#include <CharAnimation.h>
#include "charanimation.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include "CharInfo\CharFileLoadManager.h"
#include "bindJXFile.h"
#include <kindinfo.h>

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include "..\EnumData\Enum_AttackAtb.h"			// AttackAtb
#include "..\EnumData\Enum_DefenceAtb.h"		// DefenceAtb

// robypark 2004/9/23 16:4
// 온라인공용 파서 사용
#include "../SCCommon/OnlineCommonParser.h"

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2003-12-30)
// Desc: 정의들
//-----------------------------------------------------------------------------

extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	_Text 				Text;								// 텍스트 
extern	OnlineBattleInfo	OBInfo;
extern	char 					SavePath[];

#define WORKDELAY(x)			(x) 

// 공격력 정의 
#define APOWER(x)				(x)

// 방어력 정의 
#define DPOWER(x)				(x)

// 이동속도 정의 
#define MOVESPEED(x)			(x)

// 공격 범위 
#define ATTACKRANGE(x)			(x)

// 공격 지연 
#define ATTACKDELAY(x)			(x)

#define PERCENT(x)				(x)

#define EXPVARYPERCENT(x)		(x)

#define BOTTOM(x)				(x)

// 캐릭터 코드를 실제 DB에 저장될 때 사용되는 캐릭터 번호로 바꿔준다.
#define		CONVERT_CHARCODE_TO_DBCHARNO( character, number )					( number << 8 | character )
#define		CONVERT_DBCHARNO_TO_CHARCODE( value, getcharacter, getnumber )		( getcharacter = value & 0xff; getnumber = value >> 8 )


// 캐릭터 정보용 체크섬 - 자료 추가시 기존 정보로 실행하여 나오는 에러값으로 이 부분을 수정해 줄 것.
const	DWORD		DF_VALUE_BU		= 6066;				// 크랙방지 체크섬1
const	DWORD		DF_VALUE_CS		= 4924048;			// 크랙방지 체크섬2, main.cpp의 DF_VALUE_CS와는 다른 값으로,
														// 캐릭터들에 관한 체크섬이다.

// actdoll (2004/03/18 21:29) : 임시 데이터 변수
SI32	g_siDBCharNo[KIND_ON_MAX] = {0,};

#include <clGame.h>
extern _clGame*				pGame;

/*
// 온라인과 전투 상의 코드 변환 규칙. 
// 예)전투상의 캐릭터 코드인 KIND_ON_KEON은 온라인 상에서는 67이 된다. 
// CharCode.txt 파일에서 읽어들인다.
const	SI32 TransKindData[]=
{
// Online Code : Battle Code
	60+7,		KIND_ON_KEON,			// 67
	300+23,		KIND_ON_MYUNGHWA,		// 323
	9000+27,	KIND_ON_HANAHINE,		// 9000+27
	8000+771,	KIND_ON_RYO,			// 8000+771
	10000+307,	KIND_ON_SORYUNG,		// 10000+307
	10000+563,	KIND_ON_MIRYUNG,		// 10000+563
	80+5,		KIND_ON_PORTERK,		// 80+5
	300+41,		KIND_ON_SWORDK,			// 300+41
	12000+117,	KIND_ON_SWORDUPK,		// 12000+117
	500+97,		KIND_ON_ARCHERK,		// 500+97
	11000+349,	KIND_ON_ARCHERUPK,		// 11000+349
	800+53,		KIND_ON_BUDAK,			// 800+53
	12000+373,	KIND_ON_BUDAUPK,		// 12000+373
	1000+109,	KIND_CANNON2K,			// 1000+109
	1000+365,	KIND_ON_HORSEARCHERK,	// 1000+365
	11000+605,	KIND_ON_HORSEARCHERUPK, // 11000+605
	2000+133,	KIND_ON_JUSTICEK,		// 2000+133
	11000+861,	KIND_ON_JUSTICEUPK,		// 11000+861
	2000+389,	KIND_ON_SWORDJ,			// 2000+389
	12000+629,	KIND_ON_SWORDUPJ,		// 12000+629
	2000+645,	KIND_ON_GUNJ,			// 2000+645
	12000+885,	KIND_ON_GUNUPJ,			// 12000+885
	2000+901,	KIND_ON_HORSESWORDJ,	// 2000+901
	13000+141,	KIND_ON_HORSESWORDUPJ,	// 13000+141
	3000+157,	KIND_ON_ADVBUDAJ,		// 3000+157
	13000+397,	KIND_ON_ADVBUDAUPJ,		// 13000+397
	3000+413,	KIND_ON_NINJAJ,
	13000+653,	KIND_ON_NINJAUPJ,
	70+7,		KIND_ON_HUNGRYDEMON,
	800+45,		KIND_ON_EATERDEMON,
	300+33,		KIND_ON_TIGER,
	500+89,		KIND_ON_WEIRDPLANT,
	1300+57,	KIND_ON_DOGDEMON,
	1100+1,		KIND_ON_ONGHWA,
	1600+13,	KIND_ON_BANDIT1,
	1800+69,	KIND_ON_BANDIT2,
	2100+25,	KIND_ON_BANDIT3,
	4600+85,	KIND_ON_BANDIT4,
	2600+37,	KIND_ON_GACKDAGUI,
	2300+81,	KIND_ON_GATPHA,
	2800+93,	KIND_ON_BOGY,
	3100+49,	KIND_ON_FOX,
	3400+5,		KIND_ON_STARFISH,
	15189,		KIND_ON_SWORDT,
	3600+61,	KIND_ON_ONI,
	4100+73,	KIND_ON_SNOWDEMON,
	4400+29,	KIND_ON_FURHEADDEMON,
	3900+17,	KIND_ON_FIREDRAGON,
	4900+41,	KIND_ON_GOLDDRAGON,
	5100+97,	KIND_ON_BAT,
	5400+53,	KIND_ON_TOAD,
	5700+9,		KIND_ON_CENTIPEDE,
	5900+65,	KIND_ON_DUEOKSINI,
	6400+77,	KIND_ON_RACCOONDOG,
	6200+21,	KIND_ON_DEER,
	6700+33,	KIND_ON_SWORDPIRATE,
	6900+89,	KIND_ON_GUNPIRATE,
	6200+29,	KIND_ON_PORTERJ,
	7200+45,	KIND_ON_SNOWWOMAN,
	7500+1,		KIND_ON_GANGSI,
	7700+57,	KIND_ON_HARUBANG,
	8000+13,	KIND_ON_CRAB,
	8200+69,	KIND_GHOSTCAT,
	23000+107,	KIND_MANAGER1,
	8500+25,	KIND_WHITETIGER,
	9000+37,	KIND_BUTTERFLY,
	9200+93,	KIND_WIDOW,
	8700+81,	KIND_TUTTLE,
	9500+49,	KIND_INSECT,
	10000+61,	KIND_WORRIORGHOST,
	10000+317,	KIND_SINGERMGHOST,
	9800+5,		KIND_MAMMOTH,
	10000+573,	KIND_WILDBOAR,
	11000+93,	KIND_GENERAL4J,
	9500+57,	KIND_GENERAL6J,
	8200+77,	KIND_GENERAL5K,
	9000+45,	KIND_GENERAL4K,
	8500+33,	KIND_GENERAL8K,
	11000+85,	KIND_ON_SEAHORSE,
	10000+829,	KIND_ON_LEECH,
	11000+341,	KIND_ON_THIEF,
	11000+597,	KIND_ON_SEAKING,
	80+3,		KIND_ON_SHIP1,
	300+39,		KIND_ON_SHIP2,
	500+95,		KIND_ON_SHIP3,
	800+51,		KIND_ON_SHIP4,
	1100+7,		KIND_ON_SHIP5,
	1300+63,	KIND_ON_SHIP6,
	1600+19,	KIND_ON_SHIP7,
	1800+75,	KIND_ON_SHIP8,
	11000+853,  KIND_ON_BEAR,
	10000+325,	KIND_GENERAL2J,
	10000+69,	KIND_GENERAL3J,
	10000+837,	KIND_GENERAL8J,
	9300+1,		KIND_GENERAL3K,
	7700+65,	KIND_GENERAL2K,
	12000+877,	KIND_ON_HORNET,
	12000+621,	KIND_ON_CAT,
	12000+365,  KIND_ON_DUCK,
	13000+909,	KIND_ON_PORTERT,
	14000+677,	KIND_ON_BUDAT,
	14000+421,	KIND_ON_GUNT,
	14000+933,	KIND_ON_AMAZONT,
	14000+165,	KIND_ON_AXET,
	13000+133,	KIND_ON_BADMERCHANT,
	15000+957,	KIND_ON_SWORDUPT,
	15000+701,	KIND_ON_BUDAUPT,
	15000+445,	KIND_ON_GUNUPT,
	16000+469,	KIND_ON_AXEUPT,
	16000+213,	KIND_ON_AMAZONUPT,
	13000+645,	KIND_ON_MBANDIT2,
	13000+901,	KIND_ON_MBANDIT3,
	14000+925,	KIND_ON_CHAWOO,
	14000+669,	KIND_ON_PIRATES,
	13000+389,	KIND_ON_MBANDIT1,
	15000+437,	KIND_ON_PANDER,
	15000+949,	KIND_ON_CROCODILE,
	15000+693,	KIND_ON_SNAKE,
	16000+205,	KIND_ON_HOOK,
	12000+109,	KIND_ON_HORSESPEAR,
	14000+413,	KIND_ON_BABARIANSHAMAN,
	14000+157,	KIND_ON_BABARIAN, 
	12000+867,	KIND_ON_DANG,
	13000+123,	KIND_ON_SUYOUN,
	16000+461,	KIND_ON_HORSEMAN,
	17000+229,	KIND_ON_BADDEER, 
	16000+973,	KIND_ON_SCORPION, 
	15000+181,  KIND_ON_CACTUS, 
	16000+717,  KIND_ON_SMALLGANG,
	18000 +5,	KIND_ON_GENERAL1T, 
	18000+261,	KIND_ON_GENERAL4T,
	17000+485,  KIND_ON_SKYNINE,
	17000+997,  KIND_ON_ICETREE,
	18000+253,	KIND_ON_SNOWLIZARD,
	18000+509,	KIND_ON_MADBULL, 
	18000+765,	KIND_ON_BIGTREE,
	17000+741,	KIND_ON_MAGICIAN, 
	18000+517,  KIND_ON_GENERAL2T,
	18000+773,  KIND_ON_GENERAL3T, 
	19000+29,	KIND_ON_GENERAL5T,
	6000+485,	KIND_ON_PORTER_C,
	17000+237,	KIND_ON_FAN_C,
	17000+493,	KIND_ON_BUDAK_C,
	16000+981,	KIND_ON_MUDOGA_C,
	16000+725,	KIND_ON_SWORD_C,
	17000+749,	KIND_ON_FIREGUN_C,
	19000+797,  KIND_ON_FANUP_C,
	20000+53,   KIND_ON_BUDAKUP_C,
	19000+541,  KIND_ON_MUDOGAUP_C,
	19000+285,	KIND_ON_SWORDUP_C,
	20000+309,  KIND_ON_FIREGUNUP_C,
	20000+565,	KIND_ON_GENERAL4_C,			//조승훈
	20000+821,	KIND_ON_GENERAL2_C,			//이령
	21000+77,	KIND_ON_GENERAL1_C,			//동방불패
	21000+333,  KIND_ON_GENERAL5_C,			//축융
	21000+589,  KIND_ON_GENERAL3_C,			//제갈공명 
	19000+21,	KIND_ON_RUNNERHAND,			//괴물덩굴손
	19000+277,  KIND_ON_SANDGOD,			//모래의 신
	19000+533,	KIND_ON_SEAENVOY,			//바다의 사신
	19000+789,	KIND_ON_VIPER,				//살모사.
	20000+45,   KIND_ON_AMAKUSA,			//아마쿠사.
	20000+301,  KIND_ON_LAVAMONSTER,		//용암괴물.
	20000+557,  KIND_ON_EVIL,				//이사의 사념.
	20000+813,	KIND_ON_BIRD,				//인면조.
	21000+69,	KIND_ON_WEASEL,				//족제비.
	21000+581,	KIND_ON_COBRA,				//코브라.
	21000+325,	KIND_ON_SOUL,				//징기스칸의 혼.
	21000+837,	KIND_ON_VOLCANOFLAME,		//화산의 불꽃.
	22000+93,   KIND_ON_REDMADBULL,			//붉은광우.			
	22000+349,	KIND_ON_MAGICOFFICER,		//요술사령.
	22000+605,  KIND_ON_CAPBEAR,			//대장반달곰.
	22000+861,  KIND_ON_KOREANGHOST,		//처녀귀신.
	23000+117,  KIND_ON_SKELETON,			//해골.
	23000+373,  KIND_ON_SKELETONSOLDIER,	//해골병사.
	23000+629,  KIND_ON_PIRATECAPTAIN,		//해적선장.
	23000+885,	KIND_ON_TAIWANGHOST,		//대만귀신(흑백무상).
	24000+141,	KIND_ON_JAPANGHOST,			//일본귀신(우산귀신).
	24000+397,	KIND_ON_JUCK,				//적고적.
	24000+653,	KIND_ON_FEMALESHAMAN,		//조선무녀.
	24000+909,  KIND_ON_QUEEN,				//진성여왕.
	25000+165,  KIND_ON_AUTA,               //아우타.
	21000+845,  KIND_ON_DONKEY,				//당나귀.
	22000+101,  KIND_ON_BUTCHER,			//칼잡이.
	22000+357,  KIND_GENERAL1K,				//권율.
	22000+613,  KIND_ON_DSWORD,				//기마전사.
	22000+869,  KIND_GENERAL1J,				//고니시.
	23000+637,	KIND_ON_BEASTWARRIOR,		//야수전사.
	23000+893,	KIND_ON_GENERAL6T,			//손유창.
	23000+125,  KIND_ON_MAGIC_C,			//도술사.
	23000+381,  KIND_ON_GENERAL6_C,			//순비연.
	25000+421,	KIND_ON_AGILETIGER,			//비호.
	25000+677,	KIND_ON_PAWNTIGER,			//졸개호랑이.
	27000+213,	KIND_ON_OWL,				//올빼미
	27000+469,	KIND_ON_LEOPARD,			//표범
	28000+237,	KIND_ON_HUNSWARRIOR,		//흉노전사
	27000+981,	KIND_ON_HUNSHORSE,			//흉노기마병
	27000+725,	KIND_ON_HUNSHORSEARCHER,	//흉노기마궁수.
	25000+933,	KIND_ON_GATEKEEPER,			//문지기
	26000+445,	KIND_ON_CLAYFIGURE,			//기마병마용
	26000+701,	KIND_ON_PHOENIX,			//불사조
	26000+957,	KIND_ON_JINEMPEROR,			//진시황의혼
	24000+149,	KIND_ON_MEDICALMAN,			//의술사.
	24000+405,	KIND_ON_HEOJUN,				//허준.
	24000+661,	KIND_ON_COSMIC,				//음양사.
	24000+917,  KIND_ON_SEIMEI,				//세이메이.
	25000+173,  KIND_ON_YUMRUCK,			//염력사.
	25000+429,  KIND_ON_JANGSUNHWA,			//장선화.
	25000+685,  KIND_ON_LONGSPEAR,			//장창무사.
	25000+941,	KIND_ON_KOAKHU,				//곽후.	 
	28000+493,  KIND_ON_SNOWMAN1,			//눈사람1
	28000+749,  KIND_ON_SNOWMAN2,           //눈사람2
	29000+5,	KIND_ON_SANTA1,				//가짜산타1
	29000+261,  KIND_ON_SANTA2,				//가짜산타2
	29000+517,  KIND_ON_SANTA3,				//가짜산타3
	29000+773,  KIND_ON_MOTHER_DEER1,		//엄마루돌이1
	30000+29,	KIND_ON_MOTHER_DEER2,		//엄마루돌이2
	30000+285,	KIND_ON_MOTHER_DEER3,		//엄마루돌이3
	30000+541,	KIND_ON_MOTHER_DEER4,		//엄마루돌이4
	30000+797,	KIND_ON_SANTA4,				//가짜산타4
	31000+53,	KIND_ON_SANTA_TW,			//대만 가짜 산타.
	31000+309,	KIND_ON_MOTHER_DEER_TW,		//대만 가짜 루돌이.
	31000+565,  KIND_ON_FLYDRAGON,			//익룡
	31000+821,	KIND_ON_RAPTOR,				//랩터
	32000+77,   KIND_ON_TIRANO,				//티라노	
	32000+333,	KIND_ON_MONKEY,				//소환원숭이.

	// actdoll (2004/02/26 11:12) : 추가된 몬스터 - 유명계 맵용
	32589,		KIND_ON_HELLKING,								//250		// 염라대왕
	32845,		KIND_ON_HELLMESSENGER,							//251		// 저승사자
	33101,		KIND_ON_GHOSTSAMURAI,							//252		// 유령 사무라이
	33357,		KIND_ON_PYTHON,									//253		// 이무기
	33613,		KIND_ON_GHOSTELEPHANT,							//254		// 유령 코끼리
	33869,		KING_ON_GHOSTSEAKING,							//255		// 유령 해저왕
	34125,		KIND_ON_GHOSTPIRATECAPTAIN,						//256		// 유령 해적선장
	34381,		KIND_ON_GHOSTSKELETON,							//257		// 유령 해골
	34637,		KIND_ON_GHOSTSKELETONSOLDIER,					//258		// 유령 해골병사
	34893,		KIND_ON_GHOSTFEMALESHAMAN,						//259		// 유령 무녀
	35149,		KIND_ON_GHOSTVOLCANOFLAME,						//260		// 유령 불꽃

	35405,		KIND_ON_GUARDIAN_MOUSE,							//261		// 수호부(쥐)
	35661,		KIND_ON_GUARDIAN_CAW1,							//262		// 수호부(소1)
	35917,		KIND_ON_GUARDIAN_CAW2,							//263		// 수호부(소2)
	36173,		KIND_ON_GUARDIAN_CAW3,							//264		// 수호부(소3)

	36429,		KIND_ON_GUARDIAN_CHICK,							//265		// 수호부(병아리)
	36685,		KIND_ON_GUARDIAN_PIG,							//266		// 수호부(돼지)
	36941,		KIND_ON_GUARDIAN_SNAKE,							//267		// 수호부(뱀)
	37197,		KIND_ON_GUARDIAN_MONKEY,						//268		// 수호부(원숭이)

    37453,		KIND_ON_JAP_INVADERS,							//269		// 침략왜구(독도)

//	장수 2차 전직(2004-5-11, robypark)
	26197,		KIND_ON_TURTLE_VEHICLE,							// 270	U102	조선-거북차
	26453,		KIND_ON_THUNDER_VEHICLE,						// 271	U103	조선-뇌전차
	26709,		KIND_ON_QUAKE_VEHICLE,							// 272	U104	일본-지진차
	26965,		KIND_ON_BLACK_DRAGON_VEHICLE,					// 273	U105	일본-흑룡차
	27221,		KIND_ON_CATAPULT,								// 274	U106	중국-발석거
	27477,		KIND_ON_FARANGI_VEHICLE,						// 275	U107	중국-불랑기포차
	27733,		KIND_ON_FIRE_DRAGON_VEHICLE,					// 276	U108	대만-화룡차
	27989,		KIND_ON_CHINESEPHOENIX,							// 277	U109	대만-봉황비조

	-1
};
*/
#ifdef _USE_HASHCODE_EFFECTSOUND
#define ENUM_TO_STRING(x) #x
#endif

// robypark 2004/9/23 14:50
// 캐릭터의 몇 몇 정보를 HQ의 CharInfo.txt파일에서 읽어오도록 수정한다.
void _KindInfo::Init(char *namecode,	// 캐릭터 ID
					int kindatb,		// 캐릭터 속성(주인공(조합용 캐릭터), 장수, 일반(0))
					DWORD atb,			// 캐릭터 속성1
					DWORD atb2,			// 캐릭터 속성2 Skill
					DWORD atb3,			// 캐릭터 속성3 Skill
					DWORD atb4,			// 캐릭터 속성4 Skill
					DWORD atb5,			// 캐릭터 속성5 Skill
					DWORD dyingatb,		// 사망 속성
					SI32 attackatb1,	// 공격 속성1
					SI32 attackatb2,	// 공격 속성2
					DWORD defenceatb,	// 방어구 타입
					char* name,			// 전투 중에 마우스 밑에 있는 캐릭터 이름
					char *idname,		// 사용되지 않는다.
					SHORT defaultarmor/* = -1*/,	// 기본 방어구
					SHORT defaulthelmet/* = -1*/,	// 기본 헬멧
					SHORT defaultweapon/* = -1*/,	// 기본 무기
					SHORT heroid/*= 0*/,		// 주인공 조합캐릭터 ID. 그 외에는 0
					ST_KI_SUBDATA	*pSubData/*	= NULL*/,
					SI08 magicapplyrate/*= 70*/)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharInfo.txt";		// 경로를 HQ/Online/CharInfo/로 변경. robypark 2004/10/26 17:52

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "KindInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		SI32	siIndex, siNameCode, siPortraitInventory, siPortraitMercenary, siDrawBottomX, siPossessTileX, siPossessTileY;
		SI32	siSelectBoxWidth, siSelectBoxHeight, siMoveSpeed, siExperience_Vary_Percent;
		SI32	siAttack_range_1, siAttack_delay_1, siMin_attack_range_1;
		SI32	siAttack_range_2, siAttack_delay_2, siMin_attack_range_2;
		SI32	siSearch_range, siMax_fire_repeat_number, siFire_repeat_delay, siWork_delay, siDead_delay, siTower_delay, siChange_direction_delay, siAttack_give_up_delay;
		SI32	siVibrate_switch, siPriority_Attack_Type;

		char	pszCharacterID[48], pszNation[32], pszTemp[128];
		char	pszEFFECT_SND_ATTACK_1[128], pszEFFECT_SND_ATTACK_2[128];
		char	pszEFFECT_SND_TRY_ATTACK[128], pszEFFECT_SND_SELECT[128], pszEFFECT_SND_DYING[128], pszEFFECT_SND_MOVE[128], pszEFFECT_SND_ATTACK[128];

		siSelectBoxWidth = siSelectBoxHeight = siAttack_give_up_delay = 0;

		ocp.GetValue( siIndex,					iLineNo, iColNo );				// Index(Meaningless)
		ocp.GetValue( pszCharacterID,	47,		iLineNo, iColNo );		// Character ID

		//----------------------------------------------------------------------------
		// 일단 여기까지 찾고 정보가 일치하지 않으면 다음 라인 값을 추출.
		if ( stricmp(pszCharacterID, namecode) != 0)
		{
			continue;
		}
		// 넘어왔다면 찾은 것이다.
		//----------------------------------------------------------------------------

		ocp.GetValue( pszTemp,					127,	iLineNo, iColNo );		// 캐릭터 명.
		ocp.GetValue( siNameCode,						iLineNo, iColNo );		// 캐릭터 이름 코드. OnlineText를 이용한다. 단, 이 함수 호출시 넘어오는 캐릭터명(문자열)을 우선시 한다. robypark 2004/12/21 22:00
		ocp.GetValue( siPortraitInventory,				iLineNo, iColNo );		// 캐릭터 초상 인벤토리용
		ocp.GetValue( siPortraitMercenary,				iLineNo, iColNo );		// 캐릭터 초상 용병창용
		ocp.GetValue( siDrawBottomX,					iLineNo, iColNo );		// 캐릭터 그려지는 위치
		ocp.GetValue( siPossessTileX,					iLineNo, iColNo );		// 전투 맵에서 캐릭터가 차지하는 타일 수
		ocp.GetValue( siPossessTileY,					iLineNo, iColNo );		// 전투 맵에서 캐릭터가 차지하는 타일 수
		ocp.GetValue( siSelectBoxWidth,					iLineNo, iColNo );		// 전투 중에 캐릭터 선택 박스 Width
		ocp.GetValue( siSelectBoxHeight,				iLineNo, iColNo );		// 전투 중에 캐릭터 선택 박스 Height
		ocp.GetValue( pszNation,				 31,	iLineNo, iColNo );		// 캐릭터 국가명
		ocp.GetValue( siMoveSpeed,						iLineNo, iColNo );		// 캐릭터 이동속도
		ocp.GetValue( siExperience_Vary_Percent,		iLineNo, iColNo );		// 전투 맵에서 캐릭터가 차지하는 타일 수
		ocp.GetValue( siAttack_range_1,					iLineNo, iColNo );		// 공격1 사정거리
		ocp.GetValue( siAttack_delay_1,					iLineNo, iColNo );		// 공격1 공격 지연시간
		ocp.GetValue( siMin_attack_range_1,				iLineNo, iColNo );		// 공격1 최소 사정거리
		ocp.GetValue( pszEFFECT_SND_ATTACK_1,	127,	iLineNo, iColNo );		// 공격1 효과음
		ocp.GetValue( siAttack_range_2,					iLineNo, iColNo );		// 공격1 사정거리
		ocp.GetValue( siAttack_delay_2,					iLineNo, iColNo );		// 공격1 공격 지연시간
		ocp.GetValue( siMin_attack_range_2,				iLineNo, iColNo );		// 공격1 최소 사정거리
		ocp.GetValue( pszEFFECT_SND_ATTACK_2,	127,	iLineNo, iColNo );		// 공격1 효과음

		ocp.GetValue( siSearch_range,					iLineNo, iColNo );		// 시야
		ocp.GetValue( siMax_fire_repeat_number,			iLineNo, iColNo );		// 최대 연속 발사 총알 개수
		ocp.GetValue( siFire_repeat_delay,				iLineNo, iColNo );		// 연속 발사 딜레이
		ocp.GetValue( siWork_delay,						iLineNo, iColNo );		// 행동 지연시간
		ocp.GetValue( siDead_delay,						iLineNo, iColNo );		// 사망 지연시간
		ocp.GetValue( siTower_delay,					iLineNo, iColNo );		// ???
		ocp.GetValue( siChange_direction_delay,			iLineNo, iColNo );		// 방향 전환 지연시간
		ocp.GetValue( siAttack_give_up_delay,			iLineNo, iColNo );		// 공격포기 딜레이
		ocp.GetValue( pszEFFECT_SND_TRY_ATTACK,	127,	iLineNo, iColNo );		// 캐릭터 공격시도 효과음
		ocp.GetValue( pszEFFECT_SND_SELECT,		127,	iLineNo, iColNo );		// 캐릭터 선택 효과음
		ocp.GetValue( pszEFFECT_SND_DYING,		127,	iLineNo, iColNo );		// 캐릭터 사망 효과음
		ocp.GetValue( pszEFFECT_SND_MOVE,		127,	iLineNo, iColNo );		// 캐릭터 이동명령 효과음
		ocp.GetValue( pszEFFECT_SND_ATTACK,		127,	iLineNo, iColNo );		// 캐릭터 공격 효과음
		ocp.GetValue( siVibrate_switch,					iLineNo, iColNo );		// 공중 유닛일 경우 흔들림 효과 여부
		ocp.GetValue( siPriority_Attack_Type,			iLineNo, iColNo );		// robypark 2004/10/18 12:14 - 두 가지 공격속성을 가진 경우 인공지능 사용값

		fclose( fp );
		ocp.Release();

		m_ssOnlinePortraitFont	= siPortraitInventory;
		m_ssOnlineMercenaryFont	= siPortraitMercenary;
		m_ssBottomIndent		= siDrawBottomX;		// 전투 중 캐릭터 선택 상자의 바닥 위치, 캐릭터가 그려지는 좌측 상단의 y좌표
		m_ssIDXsize				= siPossessTileX;		// 전투 맵에서 차지하는 타일 수
		m_ssIDYsize				= siPossessTileY;		// 전투 맵에서 차지하는 타일 수
		m_ssXsizePercent		= siSelectBoxWidth;		// 전투 중 캐릭터 선택 상자의 가로 크기
		m_ssYsizePercent		= siSelectBoxHeight;	// 전투 중 캐릭터 선택 상자의 세로 크기
		
		if(strcmp(pszNation, "KOREA") == 0)
			m_ssNation					= COUNTRY_KOREA;
		else if(strcmp(pszNation, "JAPAN") == 0)
			m_ssNation					= COUNTRY_JAPAN;
		else if(strcmp(pszNation, "TAIWAN") == 0)
			m_ssNation					= COUNTRY_TAIWAN;
		else if(strcmp(pszNation, "CHINA") == 0)
			m_ssNation					= COUNTRY_CHINA;
		else if(strcmp(pszNation, "NEUTRAL") == 0)
			m_ssNation					= COUNTRY_NEUTRAL;
		else
			clGrp.Error("Invalid Country:KindInfo", "%s", pszNation);

		m_siKindAtb					= kindatb;

		m_ssMoveSpeed				= siMoveSpeed;

		m_siExpVaryPercent		= siExperience_Vary_Percent;
	
		m_siAttackRange1		= siAttack_range_1;

		m_siAttackRange2		= siAttack_range_2;
	
		m_siSearchRange			= siSearch_range;
	
		m_siAttackDelay1		= siAttack_delay_1;

		m_siAttackDelay2		= siAttack_delay_2;
	
		m_ssMaxFireRepeatNumber		= siMax_fire_repeat_number;
	
		m_ssFireRepeatDelay			= siFire_repeat_delay;
	
		m_ssWorkDelay				= siWork_delay;
	
		m_ssDeadDelay				= siDead_delay;
	
		m_ssTowerDelay				= siTower_delay;
	 
		m_ssChangeDirectionDelay	= siChange_direction_delay;
	
		m_ssAttackGiveUpDelay		= siAttack_give_up_delay;
	
		m_uiAtb					= atb;
	
		m_uiAtb2				= atb2;
	
		m_uiAtb3				= atb3;

		m_uiAtb4				= atb4;

		m_uiAtb5				= atb5;

		m_uiDyingAtb			= dyingatb;
	
		m_uiAttackAtb1			= attackatb1;

		m_uiAttackAtb2			= attackatb2;

		if (strcmp(pszEFFECT_SND_ATTACK_1, "0") == 0)
		{
			m_siAttackEffect1			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siAttackEffect1			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_ATTACK_1);
		}

		if (strcmp(pszEFFECT_SND_ATTACK_2, "0") == 0)
		{
			m_siAttackEffect2			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siAttackEffect2			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_ATTACK_2);
		}

		m_uiDefenceAtb				= defenceatb;
	
		m_ssVibrateSwitch			= siVibrate_switch;

		// robypark 2004/10/18 12:14
		// 두 가지 공격 속성 가진 캐릭터 인공지능 관련 값
		m_ssPriority_Attack_Type	= siPriority_Attack_Type;
	
		if (strcmp(pszEFFECT_SND_TRY_ATTACK, "0") == 0)
		{
			m_siTryAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siTryAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_TRY_ATTACK);
		}
		if (strcmp(pszEFFECT_SND_SELECT, "0") == 0)
		{
			m_siSelectEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siSelectEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_SELECT);
		}

		if (strcmp(pszEFFECT_SND_DYING, "0") == 0)
		{
			m_siDyingEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siDyingEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_DYING);
		}

		if (strcmp(pszEFFECT_SND_MOVE, "0") == 0)
		{
			m_siMoveEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siMoveEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_MOVE);
		}

		if (strcmp(pszEFFECT_SND_ATTACK, "0") == 0)
		{
			m_siAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(NULL);
		}
		else
		{
			m_siAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(pszEFFECT_SND_ATTACK);
		}
	
		// robypark 2004/12/21 22:10
		// 캐릭터명을 GameText.dat가 아니라 OnlineText.dat를 같이 이용하도록 수정.
		// 어차피 중복되는 데이터이다.
		// 함수 인자로 이름 변수 값(name)이 NULL이 아니라면 그를 설정한다.
		if (NULL == name)
		{
			// CharacterList.txt의 Name Code와 같은 값을 이용한다.
			m_pszName					= pGame->pOnlineWorld->pOnlineText->Get(siNameCode);
		}
		else	// GameText.dat의 캐릭터 명을 이용한다.
			m_pszName					= name;

		m_pszIDName					= idname;
	
		m_ssHeroID					= heroid;
		m_ssDefaultArmor			= defaultarmor;
		m_ssDefaultHelmet			= defaulthelmet;
		m_ssDefaultWeapon			= defaultweapon;

		// actdoll (2004/03/06 3:21) : 보조 데이터 추가
		if( !pSubData )			ZeroMemory( &m_SubData, sizeof(ST_KI_SUBDATA) );
		else					CopyMemory( &m_SubData, pSubData, sizeof(ST_KI_SUBDATA) );

		m_scMagicApplyRate		= magicapplyrate;

		m_nMinAttackRange[0] = siMin_attack_range_1;		// 캐릭터가 공격 가능한 최소거리(기본 값으로 1) 첫 번째 공격 속성
		m_nMinAttackRange[1] = siMin_attack_range_2;		// 캐릭터가 공격 가능한 최소거리(기본 값으로 1) 두 번째 공격 속성
	
		m_uiUnique					= GetUnique();
		return;
	}

	fclose( fp );
	ocp.Release();

#ifdef _DEBUG
	clGrp.Error("Error: KindInfo.cpp Init()", "KindInfo [%s] : Not registered.", namecode);
#else
	clGrp.Error("Character Information Setting Error", "거상 클라이언트의 변조가 의심됩니다. 설치된 거상 클라이언트를 확인해주세요.\nGersang(Kyosyoden) Client doubt modified. Please Check your client.");
#endif
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-30)
// Desc: 해당 캐릭터별 정보를 적재하는 부분이다.
//		추후 수정할 것이 많은 부분이다.
//		namecode										- 이 캐릭터에 주어진 KIND_OF의 인덱스
//		kindatb											- ?
//		movespeed										- 이동 속도
//		expvarypercent									- EXP 변화량 퍼센트
//		attackrange1, attackrange2						- 공격 범위
//		searchrange										- 검색 범위
//		attackdelay1, attackdelay2						- 공격 간격 시간
//		maxfirerepeatnumber, firerepeatdelay			- 최대 화염 재생 횟수 및 시간
//		workdelay, deaddelay, towerdelay				- 딜레이들? 
//		changedirectiondelay, attackgiveupdelay			- 딜레이들?
//		atb, atb2, atb3, atb4							- 캐릭터의 속성들
//		dyingatb										- 사망시 캐릭터의 속성들
//		attackatb1, attackatb2							- 공격 속성
//		defenceatb										- 방어 속성
//		vibrateswitch									- 진동(?)
//		tryattackeffect, selecteffect, dyingeffect,	moveeffect, attackeffect	- 사운드
//		( 공격 명령 대기중일 때, 캐릭터 선택시, 죽을 때, 이동 명령시, 공격 결정시 )
//		name											- 이 캐릭터의 명칭
//		idname											- 안쓰는 것으로 판단됨
//		heroid, defaultArmor, defaultHelmet, defaultWeapon	- 안쓰는 것들 같다.
//		pSubData										- 보조적으로 필요한 데이터들을 받는다. 기본값은 NULL이다
//		magicapplyrate									- 70으로 설정되어 있다.
//		
//-----------------------------------------------------------------------------

#ifndef _USE_HASHCODE_EFFECTSOUND
void _KindInfo::Init(	char *namecode,
						int kindatb,
						SHORT movespeed,
						SI32  expvarypercent, 
						SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
						SI32 attackdelay1,SI32 attackdelay2,
						SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
						SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
						DWORD atb, DWORD atb2, DWORD atb3, DWORD atb4,
						DWORD dyingatb, 
						SI32 attackatb1,SI32 attackatb2,
						SI32 attackeffect1, SI32 attackeffect2,
						DWORD defenceatb, 
						SHORT vibrateswitch, 
						SI32 tryattackeffect, SI32 selecteffect, SI32 dyingeffect, SI32 moveeffect, SI32 attackeffect, 
						char* name, char* idname, 
						SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
						ST_KI_SUBDATA	*pSubData,
						SI08 magicapplyrate )
#else
void _KindInfo::Init(	char *namecode,
						int kindatb,
						SHORT movespeed,
						SI32  expvarypercent, 
						SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
						SI32 attackdelay1,SI32 attackdelay2,
						SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
						SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
						DWORD atb, DWORD atb2, DWORD atb3, DWORD atb4,
						DWORD dyingatb, 
						SI32 attackatb1,SI32 attackatb2,
						const char* attackeffect1,
						const char* attackeffect2,
						DWORD defenceatb, 
						SHORT vibrateswitch, 
						const char* tryattackeffect, 
						const char* selecteffect, 
						const char* dyingeffect, 
						const char* moveeffect, 
						const char* attackeffect, 
						char* name, char* idname, 
						SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
						ST_KI_SUBDATA	*pSubData,
						SI08 magicapplyrate )
#endif
{

	CHAR	buffer[1024] ;
	CHAR	code[128] = "";
	CHAR	temp[128];
	CHAR	nation[24];
	FILE* fp; 

	SI32	gameID, portrait, mercenary, bottom, idxsize, idysize;
	SI32	xpercent, ypercent; 

	// savepath를 고려하여 화일이름을 보정한다. 
//	GetFileNamePath(, SavePath, buffer);
	if ( (fp = BindJXFile::GetInstance()->GetFile( "CharInfo.txt" )) == NULL )
	{
		clGrp.Error("File Open Error", "CharInfo" );
		return ;
	}
	
	BOOL ExitSwitch = FALSE;

	// 해당 파일에서 줄을 하나씩 얻어온다.
	while( fgets(buffer, 1024, fp) != NULL)
	{
		// actdoll : 해당 버퍼에 스킵 플래그가 있을 경우 일단 리턴이다.
		if( buffer[0] == '/' || buffer[0] == '\n' || buffer[0] == '\r' )	continue;

		// actdoll (2003/12/30 18:38) : 
		//	게임에서 쓰는 ID / 캐릭터 정의명(NOUSE) / 실제이름(NOUSE) / 초상폰트-일반 / 초상폰트-용병 / bottom / xsize / ysize / xpercent / ypercent / nation
		sscanf(buffer, "%d %s %s %d %d %d %d %d %d %d %s", 
			&gameID, code, temp, &portrait, &mercenary, &bottom, &idxsize, &idysize, &xpercent, &ypercent, nation);

//		gameID		= CLASS_KIND_ON_CHARACTER::GetData( code );
//		if( gameID != KIND_ON_INVALID )
		if(strcmp(code, namecode) == 0)
		{
			ExitSwitch				= TRUE;
			m_ssOnlinePortraitFont	= portrait;
			m_ssOnlineMercenaryFont	= mercenary;	// actdoll (2003/12/30 18:47) : 자료 추가 - 용병 초상 데이터가 증설됐다.
			m_ssBottomIndent			= bottom;		// 전투 중 캐릭터 선택 상자의 바닥 위치, 캐릭터가 그려지는 좌측 상단의 y좌표
			m_ssIDXsize					= idxsize;
			m_ssIDYsize					= idysize;
			m_ssXsizePercent			= xpercent;		// 전투 중 캐릭터 선택 상자의 가로 크기
			m_ssYsizePercent			= ypercent;		// 전투 중 캐릭터 선택 상자의 세로 크기
			
			if(strcmp(nation, "KOREA") == 0)
				m_ssNation					= COUNTRY_KOREA;
			else if(strcmp(nation, "JAPAN") == 0)
				m_ssNation					= COUNTRY_JAPAN;
			else if(strcmp(nation, "TAIWAN") == 0)
				m_ssNation					= COUNTRY_TAIWAN;
			else if(strcmp(nation, "CHINA") == 0)
				m_ssNation					= COUNTRY_CHINA;
			else if(strcmp(nation, "NEUTRAL") == 0)
				m_ssNation					= COUNTRY_NEUTRAL;
			else
				clGrp.Error("Invalid Country:KindInfo", "%s", nation);


			break;
		}
	}

	fclose(fp);

	if(ExitSwitch == FALSE)
	{
		clGrp.Error("fds3732fdsfd", "%s", namecode);
	}
	
	m_siKindAtb					= kindatb;

	m_ssMoveSpeed				= movespeed;

	m_siExpVaryPercent		= expvarypercent;
	
	m_siAttackRange1		= attackrange1;

	m_siAttackRange2		= attackrange2;
	
	m_siSearchRange				= searchrange;
	
	m_siAttackDelay1		= attackdelay1;

	m_siAttackDelay2		= attackdelay2;
	
	m_ssMaxFireRepeatNumber		= maxfirerepeatnumber;
	
	m_ssFireRepeatDelay			= firerepeatdelay;
	
	m_ssWorkDelay				= workdelay;
	
	m_ssDeadDelay				= deaddelay;
	
	m_ssTowerDelay				= towerdelay;
	 
	m_ssChangeDirectionDelay	= changedirectiondelay;
	
	m_ssAttackGiveUpDelay		= attackgiveupdelay;
	
	m_uiAtb					= atb;
	
	m_uiAtb2				= atb2;
	
	m_uiAtb3				= atb3;

	m_uiAtb4				= atb4;

	m_uiDyingAtb				= dyingatb;
	
	m_uiAttackAtb1			= attackatb1;

	m_uiAttackAtb2			= attackatb2;

#ifndef _USE_HASHCODE_EFFECTSOUND
	m_siAttackEffect1			= attackeffect1;
	m_siAttackEffect2			= attackeffect2;
#else
	m_siAttackEffect1			= GetEffectDataMgr()->GetMyHashTable()->Find(attackeffect1);
	m_siAttackEffect2			= GetEffectDataMgr()->GetMyHashTable()->Find(attackeffect2);
#endif

	m_uiDefenceAtb				= defenceatb;
	
	m_ssVibrateSwitch			= vibrateswitch;
	
#ifndef _USE_HASHCODE_EFFECTSOUND
	m_siTryAttackEffect			= tryattackeffect;
	m_siSelectEffect			= selecteffect;
	m_siDyingEffect				= dyingeffect;
	m_siMoveEffect				= moveeffect;
	m_siAttackEffect			= attackeffect;
#else
	m_siTryAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(tryattackeffect);
	m_siSelectEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(selecteffect);
	m_siDyingEffect				= GetEffectDataMgr()->GetMyHashTable()->Find(dyingeffect);	
	m_siMoveEffect				= GetEffectDataMgr()->GetMyHashTable()->Find(moveeffect);	
	m_siAttackEffect			= GetEffectDataMgr()->GetMyHashTable()->Find(attackeffect);	
#endif
	
	m_pszName					= name;
	m_pszIDName					= idname;
	
	m_ssHeroID					= heroid;
	m_ssDefaultArmor			= defaultArmor;
	m_ssDefaultHelmet			= defaultHelmet;
	m_ssDefaultWeapon			= defaultWeapon;

	// actdoll (2004/03/06 3:21) : 보조 데이터 추가
	if( !pSubData )			ZeroMemory( &m_SubData, sizeof(ST_KI_SUBDATA) );
	else					CopyMemory( &m_SubData, pSubData, sizeof(ST_KI_SUBDATA) );

	m_scMagicApplyRate		= magicapplyrate;

	// robypark 2004-5-24, 15:11: 접근전이 불가능한 캐릭터의 최소 사격 가능지점에 관한 데이터 추가
	m_nMinAttackRange[0] = 1;		// 캐릭터가 공격 가능한 최소거리(기본 값으로 1) 첫 번째 공격 속성
	m_nMinAttackRange[1] = 1;		// 캐릭터가 공격 가능한 최소거리(기본 값으로 1) 두 번째 공격 속성
	
	m_uiUnique					= GetUnique();
}


void _KindInfo::SetFile(SI32 Ani, BOOL DirSwitch, char* File, SI32 Font, SI32 Frame, SI32 Instant, SI32 Delay, SI32 atb)
{
	SI32 i;
	
	for( i=1; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		if( m_siCharAnimationInfo[i] == 0 )
		{
			m_siCharAnimationInfo[i] = Ani;
			m_clCharAnimation[i].Set(DirSwitch, Frame, File, Font, Instant, Delay, atb);
			break;
		}
	}
}


SI32 _KindInfo::GetAttackRange(SI32 currentattack)
{
	switch(currentattack)
	{
	case 0: return m_siAttackRange1; break;
	default: return m_siAttackRange2; break;
	}
	return 0;
}

SI32 _KindInfo::GetAttackAtb(SI32 currentattack)
{
	switch(currentattack)
	{
	case 0: return m_uiAttackAtb1; break;
	default: return m_uiAttackAtb2; break;
	}
	return 0;
}

SI32 _KindInfo::GetAttackDelay(SI32 currentattack)
{
	switch(currentattack)
	{
	case 0: return m_siAttackDelay1; break;
	default: return m_siAttackDelay2; break;
	}
	return 0;
}


DWORD _KindInfo::GetUnique()
{
	DWORD unique = 0;
	
	unique += m_ssOnlinePortraitFont;

	unique += m_ssOnlineMercenaryFont;
	
	unique += m_ssBottomIndent;
	
	unique += m_ssIDXsize;
	
	unique += m_ssIDYsize;
	
	unique += m_ssXsizePercent;
	
	unique += m_ssYsizePercent;
	
	unique += m_ssNation;
	
	unique += m_siKindAtb;
	
	unique += m_ssMoveSpeed;

	unique += m_siExpVaryPercent;
	
	unique += m_siAttackRange1;

	unique += m_siAttackRange2;
	
	unique += m_siSearchRange;
	
	unique += m_siAttackDelay1;

	unique += m_siAttackDelay2;
	
	unique += m_ssMaxFireRepeatNumber;
	
	unique += m_ssFireRepeatDelay;
	
	unique += m_ssWorkDelay;
	
	unique += m_ssDeadDelay;
	
	unique += m_ssTowerDelay;
	
	unique += m_ssChangeDirectionDelay;
	
	unique += m_ssAttackGiveUpDelay;
	
	unique += m_uiAtb;
	
	unique += m_uiAtb2;
	
	unique += m_uiAtb3;

	unique += m_uiAtb4;

	unique += m_uiAtb5;	// robypark 2004/10/1 17:30 추가 기술을 위한 추가

	unique += m_uiDyingAtb;
	
	unique += m_uiAttackAtb1;

	unique += m_uiAttackAtb2;

	unique += m_siAttackEffect1;

	unique += m_siAttackEffect2;
	
	unique += m_uiDefenceAtb;
	
	unique += m_ssVibrateSwitch;
	
	unique += m_siTryAttackEffect;

	unique += m_siSelectEffect;
	
	unique += m_siDyingEffect;
	
	unique += m_siMoveEffect;
	
	unique += m_siAttackEffect;
	
	unique += m_scMagicApplyRate;

	// robypark 2004-5-24, 15:12: 접근전이 불가능한 캐릭터의 최소 사격 가능지점에 관한 데이터 추가
	unique += m_nMinAttackRange[0];
	unique += m_nMinAttackRange[1];

	// robypark 2004/10/18 12:27
	// 우선 공격 속성에 관한 인공지능 관련 값
	unique += m_ssPriority_Attack_Type;

	return unique;
}


// 종류별로 중심 좌표를 기준으로 캐릭터를 찍어준다. 
void _KindInfo::Draw(UI16 kind, SI32 animation, UI16 Frame, SI16 X, SI16 Y, cltOnlineWorld *pWorld, SI32 armourcode, SI32 helmetcode, SI32 weaponcode, SI08 siSlot)
{
	SI32	file, font, frame, delay, xsize, ysize, ani, x, y;
	BOOL	bExist;

	SI32	i;

	if( IsKindAtb(KINDATB_HERO) )
	{
		SI32	siCountA, siFileID, siCharFont;
		SI32	siBodyPart, siTotalPiece;
		SI32	aniAction, wclass, weaponEnum;
		CHAR	defaultBody[5];								// 캐릭터의 디폴트 몸(예: KMN)
		
		aniAction			= animation;
		
		wclass				= HERO_CLASS_HTH;
		weaponEnum			= CHAR_NORMALSPEED;
		
		//-----------------------------
		// 디폴트 몸 코드를 무엇으로 할 것인가 결정
		switch( m_ssNation )
		{
		case COUNTRY_KOREA:		strcpy(defaultBody, "KD01");		break;
		case COUNTRY_JAPAN:		strcpy(defaultBody, "JD01");		break;
		case COUNTRY_CHINA:		strcpy(defaultBody, "CD01");		break;
		case COUNTRY_TAIWAN:	strcpy(defaultBody, "TD01");		break;
		default:				clGrp.Error("No Default Body.", "%d", m_ssNation); break;
		}
		
		// 장착 정보 셋팅
		if(armourcode == 0)
		{
			armourcode = m_ssDefaultArmor;
		}
		if(helmetcode == 0)
		{
			helmetcode = m_ssDefaultHelmet;
		}
		if(weaponcode == 0)
		{
			weaponcode = m_ssDefaultWeapon;
		}
		//pWorld->pCharFileLoadManager->SetAllChange(armourcode, helmetcode, weaponcode, HeroID, 0, defaultBody);
		
		
		frame	= pWorld->pCharFileLoadManager->m_HeroConfig[m_ssHeroID].siFontCount[aniAction][wclass];
		// 그 방향의 첫 폰트 번호
		font	= ((Frame/10) % frame);
		
		// 화면에 찍을 Body의 갯수를 얻어온다.
		siTotalPiece	= pWorld->pCharFileLoadManager->m_HeroConfig[m_ssHeroID].siTotalPiece[aniAction][wclass];
		
		bExist = FALSE;
		for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
		{
			// 찍을 Bodypart를 얻어온다.
			siBodyPart = pWorld->pCharFileLoadManager->m_HeroConfig[KI[kind].m_ssHeroID].siPartDrawSort[aniAction][wclass][0][siCountA];
			if( pWorld->pCharFileLoadManager->m_HeroCurrEquip[siSlot].bPart[aniAction][wclass][siBodyPart] == TRUE )
			{
				siCharFont		=  pWorld->pCharFileLoadManager->m_HeroConfig[m_ssHeroID].siPartFrame[aniAction][wclass][weaponEnum][font];
				siFileID		=  pWorld->pCharFileLoadManager->m_HeroCurrEquip[siSlot].siFileID[aniAction][siBodyPart];
				
				if( siFileID <= 0 )	continue;
				if(pWorld->pCharFileLoadManager->m_XSpr[siFileID].Image == NULL)	continue;
				if(!pWorld->pCharFileLoadManager->m_XSpr[siFileID].bLoadComplete)	continue;
				
				xsize = pWorld->pCharFileLoadManager->m_XSpr[siFileID].Header.Xsize;
				ysize = pWorld->pCharFileLoadManager->m_XSpr[siFileID].Header.Ysize;
				
				if( bExist == FALSE )
				{
					x = X-xsize/2;

					SI32 realysize = (m_ssYsizePercent * ysize)/ 100;
					
					SI32 _ysize = realysize/2 + m_ssBottomIndent;

					y = Y-(ysize-_ysize);

					bExist = TRUE;
				}
				
				clGrp.PutSpriteT(x, y, xsize, ysize,
					&pWorld->pCharFileLoadManager->m_XSpr[siFileID].Image[pWorld->pCharFileLoadManager->m_XSpr[siFileID].Header.Start[siCharFont]]);
			}
		} //for
	}
	else
	{
		
		ani	= 1;
		for( i=1; i<MAX_ANIMATION_PER_UNIT; i++ )
		{
			if( m_siCharAnimationInfo[i] == ANI_WAIT0 )
			{
				ani = i;
				break;
			}
		}
		// 캐릭터에 필요한 파일이 없다면 불러온다.
		file = m_clCharAnimation[ani].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
				LoadImgFile(file);
		}
		
		font = m_clCharAnimation[ani].siFont;
		frame = m_clCharAnimation[ani].GetFrameNumber();
		delay = m_clCharAnimation[ani].siDelay;
		
		xsize = GetFileXsize(file);
		ysize = GetFileYsize(file);
		
		SI32 realysize = (m_ssYsizePercent * ysize)/ 100;
		
		SI32 _ysize = realysize/2 + m_ssBottomIndent;
		
		
		if(delay != 0)	Frame /= (delay+3);
		font += (Frame % frame);
		
		Draw_PutSpriteT(X-xsize/2, Y-(ysize-_ysize), xsize, ysize, file, font);
	}
}


//-----------------------------------------------------------------------------
// Name: Parse_CharCode()
// Code: actdoll (2003-12-30)
// Desc: CharCode파일을 통해서 코드 정보를 적재한다.
//-----------------------------------------------------------------------------
BOOL	Parse_CharCode()
{
	////////////////////////////////////////////////////////////
	// robypark 2004/10/2 11:22
	// HQ의 CharCode.txt에서 데이터를 얻어와 사용.
	// 새로운 캐릭터를 추가할 때에 서버인덱스 값을 설정할 때 사용.
	// InitKindInfo() 함수에서 초기화해 준다.
	// 기존과 동일하나 온라인 공용 파서를 이용하도록 수정.
	////////////////////////////////////////////////////////////

	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharCode.txt";		// 경로를 HQ/Online/CharInfo/로 변경. robypark 2004/10/26 17:52

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Character Code Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		SI32	siCharGameCode, siDBIndexNum;
		char	pszEnumCharacterID[48], pszServerCharacterID[48], pszNation[32], pszTemp[128];

		ocp.GetValue( siCharGameCode,				iLineNo, iColNo );		// Index(Meaningless)
		ocp.GetValue( pszEnumCharacterID,	47,		iLineNo, iColNo );		// Enum(Constant) Character ID
		ocp.GetValue( pszServerCharacterID,	47,		iLineNo, iColNo );		// Server Character ID
		ocp.GetValue( pszTemp,				127,	iLineNo, iColNo );		// 캐릭터 명.
		ocp.GetValue( pszNation,			31,		iLineNo, iColNo );		// Nation

		// 인덱스 코드의 스트링을 비교한다.
		siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszEnumCharacterID );
		if( siCharGameCode == KIND_ON_INVALID )
		{
			fclose( fp );
			ocp.Release();
			clGrp.Error( "Parse_CharCode Error", "%s", pszEnumCharacterID );
			return FALSE;
		}

		char chDBIndexChar = pszServerCharacterID[0];

		// 알파벳 문자가 아닌 경우 검사
		if (isalpha(chDBIndexChar) == 0) 
		{
			// DB코드 조합
			if( chDBIndexChar == '@' )// 이 코드가 박혀있다면 이놈은 DB정보에 없는 놈이다.
			{
				KI[siCharGameCode].m_siServerID	= 0;
			}
			else
			{
				fclose( fp );
				ocp.Release();
				clGrp.Error( "Parse_CharCode Error", "%s[%s]", pszEnumCharacterID,  pszServerCharacterID);
				return FALSE;
			}
		}
		else
		{
			// 서버 캐릭터 인덱스로 변환
			char *szDBIndexNum	= pszServerCharacterID + 1;
			siDBIndexNum		= atoi(szDBIndexNum);

			chDBIndexChar					= toupper( chDBIndexChar );		// 일단 소문자일지도 모르니 대문자로 확인사살
			KI[siCharGameCode].m_siServerID	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
		}

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;

/*
	FILE	*fp;
	// 파싱할 파일을 얻어온다.
	if ( (fp = BindJXFile::GetInstance()->GetFile( "CharCode.txt" )) == NULL )
	{
		clGrp.Error( "File Open Error", "CharCode" );
		return FALSE;
	}

	// 해당 파일에서 줄을 하나씩 얻어온다.
	char	buffer[1024];
	while( fgets(buffer, 1024, fp) != NULL)
	{
		// actdoll : 해당 버퍼에 스킵 플래그가 있을 경우 일단 리턴이다.
		if( buffer[0] == '\t' )	// 이건 해당 줄에 버퍼는 있으나 글씨는 아무것도 없을때를 체크
		{
			if( buffer[1] == '/' || buffer[1] == '\n' || buffer[1] == '\r' || buffer[1] == '\t' )	continue;
		}
		else					// 이건 줄바꿈이나 주석을 체크
		{
			if( buffer[0] == '/' || buffer[0] == '\n' || buffer[0] == '\r' )						continue;
		}

		// actdoll (2003/12/30 18:38) : 원하는 정보를 얻어온다.
		SI32	siCharGameCode, siDBIndexNum;
		char	pszCharCodeStr[128], pszCharNameStr[128], pszNation[32];
		char	chDBIndexChar;
		
		//	해당 캐릭터에 정의된 게임 인덱스	/ 캐릭터 코드명	/ 해당 캐릭터에 / 정의된 DB 인덱스	/ 캐릭터 이름(사용안함)	/ 국적
		sscanf(buffer, "%d %s %c%d %s %s", 
			&siCharGameCode, pszCharCodeStr, &chDBIndexChar, &siDBIndexNum, pszCharNameStr, pszNation );

		// 인덱스 코드의 스트링을 비교한다.
		siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszCharCodeStr );
		if( siCharGameCode == KIND_ON_INVALID )
		{
			clGrp.Error( "Parse_CharCode Error", "%s",pszCharCodeStr );
			return FALSE;
		}

		// DB코드 조합
		if( chDBIndexChar == '@' )// 이 코드가 박혀있다면 이놈은 DB정보에 없는 놈이다.
		{
			KI[siCharGameCode].m_siServerID	= 0;
//			g_siDBCharNo[siCharGameCode]	= 0;
		}
		else
		{
			chDBIndexChar					= toupper( chDBIndexChar );		// 일단 소문자일지도 모르니 대문자로 확인사살
			KI[siCharGameCode].m_siServerID	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
//			g_siDBCharNo[siCharGameCode]	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
		}
	}

	fclose(fp);
	
	return TRUE;
*/
}

// robypark 2004/10/5 14:26
// 캐릭터의 애니메이션 정보 파일을 파싱하여 설정한다..
// HQ의 CharAniInfo.txt파일에서 얻어와 설정한다.
// 온라인 공용 파서를 이용.
// InitKindInfo() 함수에서 호출하여 사용된다.
BOOL Parse_CharacterAnimationInformation(void)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharAniInfo.txt";

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );		// 경로를 HQ/Online/CharInfo/로 변경. robypark 2004/10/26 17:52

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Character Animation Information Setting Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		char	pszCharacterKindID[64],	pszCharacterName[128], pszResourceIndex[128];
		SI32	siAnimationIndex, siIsMirroring, siStartFontNumber, 
				siFontNumberEachDirection, siInstant, siDelay, siAttribute;

		ocp.GetValue( pszCharacterKindID,		63,		iLineNo, iColNo );		// Enum(Constant) Character ID
		ocp.GetValue( pszCharacterName,			127,	iLineNo, iColNo );		// 캐릭터 명(사용안함)
		ocp.GetValue( siAnimationIndex,					iLineNo, iColNo );		// 애니메이션 타입 인덱스
		ocp.GetValue( siIsMirroring,					iLineNo, iColNo );		// 이미지를 미러링 복사하여 사용하는가
		ocp.GetValue( pszResourceIndex,			127,	iLineNo, iColNo );		// 스프라이트 이미지 리소스 아이디(인덱스)
		ocp.GetValue( siStartFontNumber,				iLineNo, iColNo );		// 애니메이션 시작폰트 번호
		ocp.GetValue( siFontNumberEachDirection,		iLineNo, iColNo );		// 한 방향에 해당되는 캐릭터 애니메이션 폰트 수
		ocp.GetValue( siInstant,						iLineNo, iColNo );		// 캐릭터 동작 시점 폰트 번호
		ocp.GetValue( siDelay,							iLineNo, iColNo );		// 애니메이션 딜레이
		ocp.GetValue( siAttribute,						iLineNo, iColNo );		// 애니메이션 속성

		// 인덱스 코드의 스트링을 실제 사용가능한 인덱스(숫자) 값을 얻어 온다.
		SI32 siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszCharacterKindID );

		if( siCharGameCode == KIND_ON_INVALID )
		{
			fclose( fp );
			ocp.Release();
			clGrp.Error( "Parse_CharacterAnimationInformation Error", "Line:%d Kind:%s", iLineNo, pszCharacterKindID );
			return FALSE;
		}

		// 애니메이션 설정
		KI[siCharGameCode].SetFile(siAnimationIndex, siIsMirroring, pszResourceIndex, siStartFontNumber, siFontNumberEachDirection, siInstant, siDelay, siAttribute);

		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

BOOL InitKindInfo()
{
	ST_KI_SUBDATA	subData;

	// 캐릭터 코드 정보 파싱
	Parse_CharCode();

	//////////////////////////////////////////////////////////////////
	// robypark 2004/10/1 3:40
	// HQ의 CharInfo.txt에서 데이터를 얻어와 사용하도록 수정.
	// 온라인 공용 파서를 이용하여 작성되었다.
	// 새로운 캐릭터를 추가할 때에는 Enum_KI_Character.h에서 
	// 해당 캐릭터 종류에 관한 Kind 상수값을 선언하여주며, 
	// Enum_KI_Character.cpp에 역시 추가해 준다. InitSmokeKindInfo()
	// 함수에서 초기화 부분을 추가해 준다. 또한 CharInfo.txt에
	// 도 추가해준다. CharCode.txt에도 캐릭터 아이디를 설정해주어야
	// 한다.
	//////////////////////////////////////////////////////////////////

	// 식별자 
	KI[KIND_PLAYERSTART].Init   ("KIND_PLAYERSTART",
		0,		// KindAtb
		0,		// Atb
		0,		// Atb2
		0,		// Atb3
		0,		// Atb4
		0,		// Atb5
		0,		// Dying Atb
		0, 0,	// Attack Atb
		0,		// Defence type
		Text.PlayerStart.Get(), NULL);


	// 바다게
	KI[KIND_ON_CRAB].Init      ("KIND_ON_CRAB",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_SHIELDMAGIC,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 귀묘 
	KI[KIND_GHOSTCAT].Init      ("KIND_GHOSTCAT",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_GHOSTCAT,0, 
		DEFENCEATB_ANIMAL,
		NULL , NULL);

	// 조선 유생
	KI[KIND_ON_JUSTICEK].Init       ("KIND_ON_JUSTICEK",
		/*KINDATB_AMBUSH|*/KINDATB_DETECTMINE, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		0,
		0, 
		0,
		0,
		ATTACKATB_JUSTICEK,0,  
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 고급유생(Upgrade)
	KI[KIND_ON_JUSTICEUPK].Init       ("KIND_ON_JUSTICEUPK",
		/*KINDATB_AMBUSH|*/KINDATB_DETECTMINE, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		0, 
		0,
		0,
		0, 
		ATTACKATB_JUSTICEK,0,  
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 기마궁수
	KI[KIND_ON_HORSEARCHERK].Init ("KIND_ON_HORSEARCHERK",
		0, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_HORSE|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 고급기마궁수
	KI[KIND_ON_HORSEARCHERUPK].Init ("KIND_ON_HORSEARCHERUPK",
		0, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_HORSE|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 대만 봉술가. 
	KI[KIND_ON_SWORDT].Init       ("KIND_ON_SWORDT",
		0,
		ATB_ATTACK|ATB_PATIENT,	
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//주술사
	KI[KIND_ON_BUDAT].Init        ("KIND_ON_BUDAT",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		ATB2_HEAL, 
		0, 
		0,
		0,
		0,
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);


	// 일본 퇴마사.
	KI[KIND_ON_ADVBUDAJ].Init     ("KIND_ON_ADVBUDAJ",
		0,  
		ATB_ATTACK|ATB_PATIENT/*|ATB_BETRAY*/,
		ATB2_HEAL, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 고급퇴마사.(Upgrade)
	KI[KIND_ON_ADVBUDAUPJ].Init     ("KIND_ON_ADVBUDAUPJ",
		0,  
		ATB_ATTACK|ATB_PATIENT/*|ATB_BETRAY*/,
		ATB2_HEAL, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 대만 서양총수
	KI[KIND_ON_GUNT].Init         ("KIND_ON_GUNT",
		0,  
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 늑대낭인 
	KI[KIND_ON_HORSESWORDJ].Init  ("KIND_ON_HORSESWORDJ",
		0,  
		ATB_ATTACK|ATB_HORSE|ATB_PATIENT,	
		0, 
		0,
		0,
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 고급늑대낭인 (Upgrade)
	KI[KIND_ON_HORSESWORDUPJ].Init  ("KIND_ON_HORSESWORDUPJ",
		0,  
		ATB_ATTACK|ATB_HORSE|ATB_PATIENT,	
		0, 
		0,
		0,
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 닌자
	KI[KIND_ON_NINJAJ].Init       ("KIND_ON_NINJAJ",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		ATB2_HIDE|ATB2_WARP,  
		0,
		0, 
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 고급닌자(Upgrade)
	KI[KIND_ON_NINJAUPJ].Init       ("KIND_ON_NINJAUPJ",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		ATB2_HIDE|ATB2_WARP, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
	
	// 대만 여전사.
	KI[KIND_ON_AMAZONT].Init       ("KIND_ON_AMAZONT",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_CAPTURE,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_DART,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 대만 도끼거한
	KI[KIND_ON_AXET].Init       ("KIND_ON_AXET",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
	
	//고급주술사
	KI[KIND_ON_BUDAUPT].Init        ("KIND_ON_BUDAUPT",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		ATB2_HEAL, 
		0, 
		0,
		0,
		0,
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
	
	// 조선 대장군포
	KI[KIND_CANNON2K].Init     ("KIND_CANNON2K",
		0, 
		ATB_REPAIR|ATB_ATTACKGROUND|ATB_MINRANGE,
		ATB2_CANNON2KFIRE, 
		0,
		0, 
		0,
		DYINGATB_EXP,
		ATTACKATB_CANNON2C,0, 
		DEFENCEATB_ARMOUR,
		NULL , NULL);

	// 고급도끼거한
	KI[KIND_ON_AXEUPT].Init       ("KIND_ON_AXEUPT",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 고급봉술가. 
	KI[KIND_ON_SWORDUPT].Init       ("KIND_ON_SWORDUPT",
		0,
		ATB_ATTACK|ATB_PATIENT,	
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 악덕상인 
	KI[KIND_ON_BADMERCHANT].Init         ("KIND_ON_BADMERCHANT",
		0, 
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 일본짐꾼
	KI[KIND_ON_PORTERJ].Init      ("KIND_ON_PORTERJ",
		0, 
		ATB_PATIENT,
		0,
		0, 
		0, 
		0,
		0,
		0,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
	
	// 대만짐꾼
	KI[KIND_ON_PORTERT].Init      ("KIND_ON_PORTERT",
		0,  
		ATB_PATIENT,
		0,
		0, 
		0,
		0,
		0,
		0,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
	
	// 대만 고급여전사 
	KI[KIND_ON_AMAZONUPT].Init       ("KIND_ON_AMAZONUPT",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_CAPTURE,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_DART,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 마적두목. 
	KI[KIND_ON_MBANDIT1].Init       ("KIND_ON_MBANDIT1",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 망치마적
	KI[KIND_ON_MBANDIT2].Init       ("KIND_ON_MBANDIT2",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 총수마적 
	KI[KIND_ON_MBANDIT3].Init         ("KIND_ON_MBANDIT3",
		0,  
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 차우차우 
	KI[KIND_ON_CHAWOO].Init       ("KIND_ON_CHAWOO",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 해적 
	KI[KIND_ON_PIRATES].Init       ("KIND_ON_PIRATES",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 장수 
	// 조선 이순신 
	KI[KIND_GENERAL2K].Init ("KIND_GENERAL2K",
		KINDATB_GENERAL/*|KINDATB_AIRMAGIC*/, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		0, 
		ATB3_MAGIC4,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General2KIDName.Get());
	
	// 조선 유성룡 
	KI[KIND_GENERAL3K].Init ("KIND_GENERAL3K",
		KINDATB_GENERAL/*|KINDATB_AIRMAGIC*/, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		0/*ATB2_BETRAY*/, 
		ATB3_MAGIC6C,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_JUSTICEK,0, 
		DEFENCEATB_HUMAN,
		NULL , Text.General3KIDName.Get());

	//조선 사명대사 
	KI[KIND_GENERAL4K].Init ("KIND_GENERAL4K",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		ATB2_HEAL, 
		ATB3_LIGHTNING,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2/*ATTACKATB_ELECTRON*/,ATTACKATB_MAGIC2, 
		DEFENCEATB_HUMAN,
		NULL , Text.General4KIDName.Get());
	
	// 조선 신립  
	KI[KIND_GENERAL5K].Init ("KIND_GENERAL5K",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,	
		0, 
		ATB3_FIREARROW2|ATB3_EXTINGUISH|ATB3_SPECIALATTACK,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HORSE,
		NULL, Text.General5KIDName.Get());

	// 조선 김덕령 
	KI[KIND_GENERAL7K].Init ("KIND_GENERAL7K",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_PATIENT|ATB_ATTACK,
		0, 
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		Text.General7K.Get(), Text.General7KIDName.Get());
/*	
	KI[KIND_GENERAL7K].SetFile(ANI_WAIT0,	TRUE,	"FILE_CHAR_GENERALK7",	40, 8,  0, 5);
	KI[KIND_GENERAL7K].SetFile(ANI_MOVE,	TRUE,	"FILE_CHAR_GENERALK7",	88, 8,  0, 1);
	KI[KIND_GENERAL7K].SetFile(ANI_DYING,  FALSE,	"FILE_CHAR_GENERALK7",	80, 8,  7, 2);
	KI[KIND_GENERAL7K].SetFile(ANI_ATTACK, TRUE,	"FILE_CHAR_GENERALK7",	0,	8,	7, 1);
*/

	// 조선 김시민  
	KI[KIND_GENERAL8K].Init ("KIND_GENERAL8K",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_PATIENT|ATB_ATTACK,
		0,
		ATB3_MAGIC8K|ATB3_GROUNDEXP,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General8KIDName.Get());

	// 일본 가토
	KI[KIND_GENERAL2J].Init ("KIND_GENERAL2J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_KIMBOMB,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General2JIDName.Get());
	
	// 일본 와키자카 
	KI[KIND_GENERAL3J].Init ("KIND_GENERAL3J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		ATB2_GUNSMOKE, 
		ATB3_SPECIALATTACK,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_GUN, ATTACKATB_MACHINEGUN, 
		DEFENCEATB_HUMAN,
		NULL , Text.General3JIDName.Get());
	
	// 일본 세이쇼오(조선의 반격)
	KI[KIND_GENERAL4J].Init ("KIND_GENERAL4J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		ATB2_HEAL, 
		ATB3_EARTHQUAKE,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2, ATTACKATB_ELECTRON/*이 공격 속성은 작동해서는 않되며 작동되지 않는다. 즉, 아래의 애니메이션에 ANI_ATTACK2가 존재해서는 안된다. 이 속성은 단지 세이쇼오의 AI힐링 속도를 빠르게 하기 위하여 사용된다.*/, 
		DEFENCEATB_HUMAN,
		NULL, Text.General4JIDName.Get());
	
	// 일본 우기다 
	KI[KIND_GENERAL5J].Init ("KIND_GENERAL5J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		Text.General5J.Get() , Text.General5JIDName.Get());
/*	
	KI[KIND_GENERAL5J].SetFile(ANI_WAIT0,	TRUE,	"FILE_CHAR_GENERALJ5",	0,	10,  0, 5);
	KI[KIND_GENERAL5J].SetFile(ANI_MOVE,	TRUE,	"FILE_CHAR_GENERALJ5",	50,	8,  0, 1);
	KI[KIND_GENERAL5J].SetFile(ANI_DYING,  FALSE,	"FILE_CHAR_GENERALJ5",	90,	8,  7, 2);
	KI[KIND_GENERAL5J].SetFile(ANI_ATTACK, TRUE,	"FILE_CHAR_GENERALJ5",	99, 11, 7, 1);
*/	
	
	// 일본 도쿠가와 이에야스
	KI[KIND_GENERAL6J].Init ("KIND_GENERAL6J",
		KINDATB_GENERAL, 
		ATB_PATIENT|ATB_ATTACK,
		0, 
		ATB3_TORNADO,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,ATTACKATB_BIGSWORD, 
		DEFENCEATB_HUMAN,
		NULL, Text.General6JIDName.Get());
	
	// 일본 이시다 미츠나리 
	KI[KIND_GENERAL7J].Init ("KIND_GENERAL7J",
		KINDATB_GENERAL, 
		ATB_PATIENT|ATB_ATTACK|ATB_ATTACKAIR,
		0, 
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HUMAN,
		Text.General7J.Get(), Text.General7JIDName.Get());
/*	
	KI[KIND_GENERAL7J].SetFile(ANI_WAIT0,	TRUE,	"FILE_CHAR_GENERALJ7",	45,	9,  0, 5);
	KI[KIND_GENERAL7J].SetFile(ANI_MOVE,	TRUE,	"FILE_CHAR_GENERALJ7",	99,	9,  0, 1);
	KI[KIND_GENERAL7J].SetFile(ANI_DYING,  FALSE,	"FILE_CHAR_GENERALJ7",	90,	9,  8, 2);
	KI[KIND_GENERAL7J].SetFile(ANI_ATTACK, TRUE,	"FILE_CHAR_GENERALJ7",	0,	9,	7, 1);
*/	
	
	// 일본 아사코 
	KI[KIND_GENERAL8J].Init ("KIND_GENERAL8J",
		KINDATB_GENERAL, 
		ATB_PATIENT|ATB_ATTACK|ATB_CIRCLEBALCKCHECK,
		0/*ATB2_HIDE|ATB2_WARPBASE|ATB2_BETRAY2*/, 
		ATB3_CLON,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General8JIDName.Get());

	//---------------------------------
	// 온라인 캐릭터
	// 온라인 - 조철희.
	KI[KIND_ON_KEON].Init       ("KIND_ON_KEON",
		KINDATB_HERO, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 1, 1, 59, HERO_KM);
	
	// 온라인 - 이명화
	KI[KIND_ON_MYUNGHWA].Init       ("KIND_ON_MYUNGHWA",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 1, 1, 59, HERO_KW);

	// 온라인 - 시마다 료
	KI[KIND_ON_RYO].Init       ("KIND_ON_RYO",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 2, 2, 59, HERO_JM);

	// 온라인 - 겐노 하나히네
	KI[KIND_ON_HANAHINE].Init       ("KIND_ON_HANAHINE",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 2, 2, 60, HERO_JW);
	
	// 온라인 - 왕소룡
	KI[KIND_ON_SORYUNG].Init       ("KIND_ON_SORYUNG",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 48, 25, 59, HERO_TM);
	
	// 온라인 - 왕미령
	KI[KIND_ON_MIRYUNG].Init       ("KIND_ON_MIRYUNG",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 48, 25, 59, HERO_TW);

	// 온라인 - 당호엽 
	KI[KIND_ON_DANG].Init       ("KIND_ON_DANG",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 3, 3, 59, HERO_CM);

	// 온라인 - 노수연 
	KI[KIND_ON_SUYOUN].Init       ("KIND_ON_SUYOUN",
		KINDATB_HERO,
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL, 3, 3, 59, HERO_CW);

	// 온라인 - 식귀
	KI[KIND_ON_EATERDEMON].Init       ("KIND_ON_EATERDEMON",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_SOFT,
		NULL, NULL);
	
	// 온라인 - 요수목
	KI[KIND_ON_WEIRDPLANT].Init       ("KIND_ON_WEIRDPLANT",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_SHIELDMAGIC,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_OILARMOUR,
		NULL, NULL);
	
	// 온라인 - 아귀
	KI[KIND_ON_HUNGRYDEMON].Init       ("KIND_ON_HUNGRYDEMON",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_HEAL,
		ATB3_ABSORBLIFE,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_OILARMOUR,
		NULL, NULL);
	
	// 온라인 - 호랑이
	KI[KIND_ON_TIGER].Init       ("KIND_ON_TIGER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 백호
	KI[KIND_WHITETIGER].Init       ("KIND_WHITETIGER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 견신
	KI[KIND_ON_DOGDEMON].Init       ("KIND_ON_DOGDEMON",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 온천원숭이
	KI[KIND_ON_ONGHWA].Init       ("KIND_ON_ONGHWA",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0, 
		ATTACKATB_ONGHWA, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 꼬마산적
	KI[KIND_ON_BANDIT1].Init       ("KIND_ON_BANDIT1",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 산적
	KI[KIND_ON_BANDIT2].Init       ("KIND_ON_BANDIT2",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 온라인 - 철퇴산적
	KI[KIND_ON_BANDIT3].Init       ("KIND_ON_BANDIT3",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 온라인 - 궁수산적
	KI[KIND_ON_BANDIT4].Init       ("KIND_ON_BANDIT4",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_ARROW, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 짐꾼
	KI[KIND_ON_PORTERK].Init      ("KIND_ON_PORTERK",
		0,  
		ATB_PATIENT,
		0,
		0,
		0,
		0, 
		0, 
		0,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 온라인 - 각다귀
	KI[KIND_ON_GACKDAGUI].Init       ("KIND_ON_GACKDAGUI",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_OILARMOUR,
		NULL, NULL);

	// 온라인 - 갓파
	KI[KIND_ON_GATPHA].Init       ("KIND_ON_GATPHA",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE|ATB_LOWATTACK, 
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_GATPHAPOISON, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 도깨비
	KI[KIND_ON_BOGY].Init       ("KIND_ON_BOGY",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_ENEMYCLON, 
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 돌하루방
	KI[KIND_ON_HARUBANG].Init       ("KIND_ON_HARUBANG",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);
	
	// 온라인 - 구미호
	KI[KIND_ON_FOX].Init       ("KIND_ON_FOX",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_CLON,
		0, 
		0,
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 불가사리
	KI[KIND_ON_STARFISH].Init       ("KIND_ON_STARFISH",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_DUEOKSINI, ATTACKATB_FIREUSINGMANA, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 배1
	KI[KIND_ON_SHIP1].Init       ("KIND_ON_SHIP1",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0, 
		0,
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP1].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP1].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/	
	
	// 온라인 - 오니
	KI[KIND_ON_ONI].Init       ("KIND_ON_ONI",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
		
	// 온라인 - 설인
	KI[KIND_ON_SNOWDEMON].Init       ("KIND_ON_SNOWDEMON",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_ICE, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
	
	// 온라인 - 털가면
	KI[KIND_ON_FURHEADDEMON].Init       ("KIND_ON_FURHEADDEMON",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_ICE, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_FURHEADDEMON, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);
		
	// 온라인 - 화룡
	KI[KIND_ON_FIREDRAGON].Init       ("KIND_ON_FIREDRAGON",
		KINDATB_FLY, 
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT, 
		0,
		0, 
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_FIRE, 0, 
		DEFENCEATB_AIRPLANE,
		NULL, NULL);
	
	// 온라인 - 황룡
	KI[KIND_ON_GOLDDRAGON].Init       ("KIND_ON_GOLDDRAGON",
		KINDATB_FLY, 
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT, 
		0, 
		ATB3_GROUNDEXP,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_GOLDDRAGONELECTRON, 0, 
		DEFENCEATB_AIRPLANE,
		NULL, NULL);
	
	// 온라인 - 박쥐
	KI[KIND_ON_BAT].Init       ("KIND_ON_BAT",
		KINDATB_FLY, 
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT, 
		0,
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_AIRPLANE,
		NULL, NULL);
		
	// 온라인 - 독두꺼비
	KI[KIND_ON_TOAD].Init       ("KIND_ON_TOAD",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_TOAD, 0, 
		DEFENCEATB_SOFT,
		NULL, NULL);
		
	// 온라인 - 독지네
	KI[KIND_ON_CENTIPEDE].Init       ("KIND_ON_CENTIPEDE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 두억시니
	KI[KIND_ON_DUEOKSINI].Init       ("KIND_ON_DUEOKSINI",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_DUEOKSINI,0,  
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 START
//----------------------------------------------------------------------------

	// 온라인 - 강시 - 완성
	KI[KIND_ON_GANGSI].Init       ("KIND_ON_GANGSI",
		0, 
		ATB_ATTACK | ATB_PATIENT, 
		0,
		ATB3_CLON,
		0, 
		0,
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 END
//----------------------------------------------------------------------------

	// 온라인 - 너구리
	KI[KIND_ON_RACCOONDOG].Init       ("KIND_ON_RACCOONDOG",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 사슴
	KI[KIND_ON_DEER].Init       ("KIND_ON_DEER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 반달곰 
	KI[KIND_ON_BEAR].Init       ("KIND_ON_BEAR",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 대장반달곰 
	KI[KIND_ON_CAPBEAR].Init       ("KIND_ON_CAPBEAR",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 조선 창잡이
	KI[KIND_ON_SWORDK].Init       ("KIND_ON_SWORDK",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 조선 고급창잡이(Upgrade)
	KI[KIND_ON_SWORDUPK].Init       ("KIND_ON_SWORDUPK",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 조선 활잡이
	KI[KIND_ON_ARCHERK].Init      ("KIND_ON_ARCHERK",
		0,
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 조선 고급활잡이(Upgrade)
	KI[KIND_ON_ARCHERUPK].Init      ("KIND_ON_ARCHERUPK",
		0,
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_ARROW,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 일본 검술낭인
	KI[KIND_ON_SWORDJ].Init       ("KIND_ON_SWORDJ",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 일본 고급검술낭인(Upgrade)
	KI[KIND_ON_SWORDUPJ].Init       ("KIND_ON_SWORDUPJ",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 일본 조총낭인
	KI[KIND_ON_GUNJ].Init         ("KIND_ON_GUNJ",
		0,

		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE/*mmm |ATB_LOWATTAC*/|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 일본 고급조총낭인(Upgrade)
	KI[KIND_ON_GUNUPJ].Init         ("KIND_ON_GUNUPJ",
		0,
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE/*mmm |ATB_LOWATTAC*/|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 왜구
	KI[KIND_ON_SWORDPIRATE].Init       ("KIND_ON_SWORDPIRATE",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_LOWMOVE,
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 온라인 - 조총왜구
	KI[KIND_ON_GUNPIRATE].Init         ("KIND_ON_GUNPIRATE",
		0,
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE|ATB_LOWATTACK|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);
		
	// 온라인 - 설녀
	KI[KIND_ON_SNOWWOMAN].Init       ("KIND_ON_SNOWWOMAN",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE|ATB_ICE, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SNOWWOMAN, 0, 
		DEFENCEATB_FLOAT,
		NULL, NULL);

	// 온라인 - 조선 파계승
	KI[KIND_ON_BUDAK].Init        ("KIND_ON_BUDAK",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		ATB2_HEAL, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 조선 고급파계승(Upgrade)
	KI[KIND_ON_BUDAUPK].Init        ("KIND_ON_BUDAUPK",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		ATB2_HEAL, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 운영자1
	KI[KIND_MANAGER1].Init        ("KIND_MANAGER1",
		0,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		ATB2_HEAL, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 바다거북이
	KI[KIND_TUTTLE].Init       ("KIND_TUTTLE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_WAVE, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 독나방
	KI[KIND_BUTTERFLY].Init       ("KIND_BUTTERFLY",
		KINDATB_FLY, 
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT|ATB_MINRANGE, 
		0,
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BUTTERFLY,0,  
		DEFENCEATB_AIRPLANE,
		NULL, NULL);

	// 거미요괴
	KI[KIND_WIDOW].Init       ("KIND_WIDOW",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, ATTACKATB_WIDOW, 
		DEFENCEATB_SOFT,
		NULL, NULL);

	// 사마귀 
	KI[KIND_INSECT].Init       ("KIND_INSECT",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_SOFT,
		NULL, NULL);

	// 무관의혼 
	KI[KIND_WORRIORGHOST].Init       ("KIND_WORRIORGHOST",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_WARP, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, ATTACKATB_BIGSWORD, 
		DEFENCEATB_GHOST,
		NULL, NULL);
	
	// 신검의혼 
	KI[KIND_SINGERMGHOST].Init       ("KIND_SINGERMGHOST",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_WARP, 
		ATB3_KIMBOMB,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, ATTACKATB_KIMBOMB, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 눈코끼리
	KI[KIND_MAMMOTH].Init       ("KIND_MAMMOTH",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_ICE, 
		0, 
		ATB3_EARTHQUAKE,
		0, 
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 멧돼지
	KI[KIND_WILDBOAR].Init       ("KIND_WILDBOAR",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 온라인 - 왕거머리 
	KI[KIND_ON_LEECH].Init        ("KIND_ON_LEECH",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD,ATTACKATB_LEECH, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 해마 
	KI[KIND_ON_SEAHORSE].Init        ("KIND_ON_SEAHORSE",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SEAHORSE,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 도굴꾼. 
	KI[KIND_ON_THIEF].Init       ("KIND_ON_THIEF",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_HEAL, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 온라인 - 해저왕
	KI[KIND_ON_SEAKING].Init        ("KIND_ON_SEAKING",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_SEAHORSECLON,
		0, 
		0,
		0, 
		ATTACKATB_SWORD,0 , 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 기마창수 
	KI[KIND_ON_HORSESPEAR].Init  ("KIND_ON_HORSESPEAR",
		0,  
		ATB_ATTACK|ATB_HORSE|ATB_PATIENT,	
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HORSE,
		NULL , NULL);

	// 도둑고양이
	KI[KIND_ON_CAT].Init  ("KIND_ON_CAT",
		0,  
		ATB_ATTACK|ATB_HORSE|ATB_PATIENT,	
		0, 
		0, 
		0,
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL , NULL);

	// 대만오리
	KI[KIND_ON_DUCK].Init  ("KIND_ON_DUCK",
		0,  
		ATB_ATTACK|ATB_HORSE|ATB_PATIENT,	
		0, 
		0, 
		0,
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL , NULL);

	// 대만왕벌 
	KI[KIND_ON_HORNET].Init       ("KIND_ON_HORNET",
		KINDATB_FLY, 
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT, 
		0,
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 대만 고급서양총수
	KI[KIND_ON_GUNUPT].Init         ("KIND_ON_GUNUPT",
		0,  
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 팬더   
	KI[KIND_ON_PANDER].Init       ("KIND_ON_PANDER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 구렁이    
	KI[KIND_ON_SNAKE].Init       ("KIND_ON_SNAKE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 식인악어
	KI[KIND_ON_CROCODILE].Init       ("KIND_ON_CROCODILE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 갈고리해적 
	KI[KIND_ON_HOOK].Init       ("KIND_ON_HOOK",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 졸개마적 
	KI[KIND_ON_SMALLGANG].Init       ("KIND_ON_SMALLGANG",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 마두인 
	KI[KIND_ON_HORSEMAN].Init       ("KIND_ON_HORSEMAN",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0, 
		0,
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 원주민무당 
	KI[KIND_ON_BABARIANSHAMAN].Init       ("KIND_ON_BABARIANSHAMAN",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_HEAL, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_POISONARROW, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 원주민
	KI[KIND_ON_BABARIAN].Init       ("KIND_ON_BABARIAN",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, ATTACKATB_THROWSPEAR, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 타락루돌프 
	KI[KIND_ON_BADDEER].Init       ("KIND_ON_BADDEER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 괴물선인장. 
	KI[KIND_ON_CACTUS].Init       ("KIND_ON_CACTUS",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_CACTUS, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 독전갈
	KI[KIND_ON_SCORPION].Init       ("KIND_ON_SCORPION",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_SWORD, ATTACKATB_SCORPION, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 대만 왕거한
	KI[KIND_ON_GENERAL1T].Init       ("KIND_ON_GENERAL1T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_SOUL|ATB3_MAGIC5J,
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 대만 유영복
	KI[KIND_ON_GENERAL4T].Init       ("KIND_ON_GENERAL4T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_MAGIC4T, // 흑룡대차륜
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 대만 페르난데스 
	KI[KIND_ON_GENERAL2T].Init       ("KIND_ON_GENERAL2T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE|ATB_ATTACKAIR,
		0, 
		ATB3_POISONBULLET|ATB3_GHOST,
		0,
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 대만 조세림 
	KI[KIND_ON_GENERAL5T].Init       ("KIND_ON_GENERAL5T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_MAGIC_RECALL_PANDA,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_DART,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 대만 임평후
	KI[KIND_ON_GENERAL3T].Init       ("KIND_ON_GENERAL3T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE|ATB_ATTACKAIR,
		ATB2_HEAL, 
		ATB3_DARKFIRE,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 천구. 
	KI[KIND_ON_SKYNINE].Init       ("KIND_ON_SKYNINE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_LIGHTNING,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_ELECTRON, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 얼음나무. 
	KI[KIND_ON_ICETREE].Init       ("KIND_ON_ICETREE",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_ICE, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_FURHEADDEMON, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 눈도마뱀 
	KI[KIND_ON_SNOWLIZARD].Init       ("KIND_ON_SNOWLIZARD",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_ICE, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 광우 
	KI[KIND_ON_MADBULL].Init       ("KIND_ON_MADBULL",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//붉은광우.
	KI[KIND_ON_REDMADBULL].Init("KIND_ON_REDMADBULL",
		0,
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_DUEOKSINI, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 거대수 
	KI[KIND_ON_BIGTREE].Init       ("KIND_ON_BIGTREE",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_SWORD, ATTACKATB_BIGTREE, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 요술사 
	KI[KIND_ON_MAGICIAN].Init       ("KIND_ON_MAGICIAN",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_CLON2,
		0,
		0, 
		0, 
		ATTACKATB_MAGICIAN, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//요술사령.
	KI[KIND_ON_MAGICOFFICER].Init	("KIND_ON_MAGICOFFICER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_CLON2,
		0,
		0, 
		0, 
		ATTACKATB_MAGICIAN, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//중국 용병
	KI[KIND_ON_SWORD_C].Init       ("KIND_ON_SWORD_C",							//큰칼무사.
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_PORTER_C].Init       ("KIND_ON_PORTER_C",							//중국 짐꾼.
		0, 
		ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		0, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_FAN_C].Init       ("KIND_ON_FAN_C",							//모험가.
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_JUSTICEK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_BUDAK_C].Init       ("KIND_ON_BUDAK_C",							//수도승.
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_HEAL, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_MAGIC2, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_MUDOGA_C].Init       ("KIND_ON_MUDOGA_C",							//무도가
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_ENERGYPA,
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_SWORD, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_FIREGUN_C].Init       ("KIND_ON_FIREGUN_C",							//화포수
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_CANNON, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//고급 큰칼무사.
	KI[KIND_ON_SWORDUP_C].Init       ("KIND_ON_SWORDUP_C",							//큰칼무사.
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0, 
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//고급 모험가.
	KI[KIND_ON_FANUP_C].Init       ("KIND_ON_FANUP_C",							//모험가.
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_JUSTICEK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//고급 수도승 
	KI[KIND_ON_BUDAKUP_C].Init       ("KIND_ON_BUDAKUP_C",							//수도승.
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_HEAL, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_MAGIC2, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//고급 무도가.
	KI[KIND_ON_MUDOGAUP_C].Init       ("KIND_ON_MUDOGAUP_C",							//무도가
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		ATB4_ENERGYPA,
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_SWORD, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 고급 화포수
	KI[KIND_ON_FIREGUNUP_C].Init       ("KIND_ON_FIREGUNUP_C",							
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_CANNON, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//중국 장수 추가
	//동방불패(중국장수) 워프 테스트.
	KI[KIND_ON_GENERAL1_C].Init ("KIND_ON_GENERAL1_C",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT, 
		ATB2_WARP,
		0,
		ATB4_FIREENERGYPA,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,ATTACKATB_BIGSWORD, 
		DEFENCEATB_HUMAN,
		NULL, Text.General1CIDName.Get());

	//이령(중국장수) //지뢰테스트
	KI[KIND_ON_GENERAL2_C].Init ("KIND_ON_GENERAL2_C",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		0,
		0,
		ATB4_FIREATTACK,
		0, 
		0,
		ATTACKATB_CANNON,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General2CIDName.Get());

	//제갈공명(중국장수)
	KI[KIND_ON_GENERAL3_C].Init ("KIND_ON_GENERAL3_C",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		ATB2_HEAL,
		ATB3_LIGHTNING, //ATB3_SHIELDMAGIC
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General3CIDName.Get());

	//조승훈(중국장수)
	KI[KIND_ON_GENERAL4_C].Init ("KIND_ON_GENERAL4_C",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		0,
		ATB3_KIMBOMB,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General4CIDName.Get());

	//축융 (중국장수)
	KI[KIND_ON_GENERAL5_C].Init ("KIND_ON_GENERAL5_C",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT, 
		0,
		ATB3_MAGIC6C,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_JUSTICEK,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General5CIDName.Get());

	////////////////////////////////////////////////////////////////////////////////////////
	//몬스터
	////////////////////////////////////////////////////////////////////////////////////////

	//괴물 덩굴손.
	KI[KIND_ON_RUNNERHAND].Init ("KIND_ON_RUNNERHAND",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//모래의 신.
	KI[KIND_ON_SANDGOD].Init ("KIND_ON_SANDGOD",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		ATB3_YELLOWSAND,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);
	
	//바다의 사신.
	KI[KIND_ON_SEAENVOY].Init ("KIND_ON_SEAENVOY",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_DUEOKSINI,ATTACKATB_BIGSWORD, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//살모사.
	KI[KIND_ON_VIPER].Init ("KIND_ON_VIPER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//아마쿠사.
	KI[KIND_ON_AMAKUSA].Init ("KIND_ON_AMAKUSA",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		ATB3_MAGIC_RECALL_AMAKUSA,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_EVIL,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	//용암괴물
	KI[KIND_ON_LAVAMONSTER].Init ("KIND_ON_LAVAMONSTER",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_VOLCANOFLAME,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);


//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 진시황릉 관련 START
//----------------------------------------------------------------------------

	//이사의 사념.
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_GATEKEEPER;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 1800;
	KI[KIND_ON_EVIL].Init ("KIND_ON_EVIL",
		0, 
		ATB_ATTACK | ATB_PATIENT | ATB_MINRANGE, 
		0, 
		0,
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_EVIL, 0,
		DEFENCEATB_GHOST,
		NULL, NULL, -1, -1, -1, 0, &subData );
//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 진시황릉 관련 END
//----------------------------------------------------------------------------


	//인면조
	KI[KIND_ON_BIRD].Init ("KIND_ON_BIRD",
		KINDATB_FLY,
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR, 
		0,
		ATB3_TORNADO,//풍백술.
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//족제비.
	KI[KIND_ON_WEASEL].Init ("KIND_ON_WEASEL",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0,
		0, 
		ATTACKATB_SWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//징키스칸의 혼.
	KI[KIND_ON_SOUL].Init ("KIND_ON_SOUL",
		0, 
		ATB_ATTACK|ATB_PATIENT,
		0,
		ATB3_MAGIC4T,		//원심분쇄격(흑룡대차륜)
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,ATTACKATB_MAGIC4T, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//코브라.
	KI[KIND_ON_COBRA].Init ("KIND_ON_COBRA",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0,
		0,
		0,
		0,
		0, 
		ATTACKATB_TOAD,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//화산의불꽃
	KI[KIND_ON_VOLCANOFLAME].Init ("KIND_ON_VOLCANOFLAME",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		ATB3_KIMBOMB,//폭뢰격.
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_VOLCANOFLAME,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// 처녀귀신.
	KI[KIND_ON_KOREANGHOST].Init  ("KIND_ON_KOREANGHOST",
		0,  
		ATB_ATTACK|ATB_PATIENT,	
		0, 
		0, 
		0,
		0,
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//해골
	KI[KIND_ON_SKELETON].Init  ("KIND_ON_SKELETON",
		0,  
		ATB_ATTACK|ATB_PATIENT,	
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_SWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//해골병사.
	KI[KIND_ON_SKELETONSOLDIER].Init  ("KIND_ON_SKELETONSOLDIER",
		0,  
		ATB_ATTACK|ATB_PATIENT,	
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 해적선장.
	KI[KIND_ON_PIRATECAPTAIN].Init  ("KIND_ON_PIRATECAPTAIN",
		0,  
		ATB_ATTACK | ATB_ATTACKAIR | ATB_PATIENT | ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_GUN,ATTACKATB_CANNON, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//대만귀신.
	KI[KIND_ON_TAIWANGHOST].Init  ("KIND_ON_TAIWANGHOST",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_YELLOWSAND,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//일본귀신.
	KI[KIND_ON_JAPANGHOST].Init  ("KIND_ON_JAPANGHOST",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_JAPANGHOST,0,
		DEFENCEATB_GHOST,
		NULL, NULL);

	//아우타.
	KI[KIND_ON_AUTA].Init  ("KIND_ON_AUTA",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		ATB3_YELLOWSAND,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_AUTA,0,
		DEFENCEATB_GHOST,
		NULL, NULL);

	//적고적
	KI[KIND_ON_JUCK].Init  ("KIND_ON_JUCK",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD,0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//조선무녀
	KI[KIND_ON_FEMALESHAMAN].Init  ("KIND_ON_FEMALESHAMAN",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD,0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//진성여왕.
	KI[KIND_ON_QUEEN].Init  ("KIND_ON_QUEEN",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		ATB3_GROUNDEXP,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SUB1, ATTACKATB_SUB2, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//당나귀
	KI[KIND_ON_DONKEY].Init      ("KIND_ON_DONKEY",
		0,  
		ATB_PATIENT,
		0,
		0,
		0,
		0, 
		0, 
		0,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 칼잡이
	KI[KIND_ON_BUTCHER].Init       ("KIND_ON_BUTCHER",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 조선 용병 - 고급 칼잡이
	KI[KIND_ON_BUTCHER_UP].Init       ("KIND_ON_BUTCHER_UP",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 권율 
	KI[KIND_GENERAL1K].Init ("KIND_GENERAL1K",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		ATB3_KIMBOMB,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General1KIDName.Get());

	// 기마무사
	KI[KIND_ON_DSWORD].Init       ("KIND_ON_DSWORD",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 고니시
	KI[KIND_GENERAL1J].Init ("KIND_GENERAL1J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_DOUBLE_SWORD,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General1JIDName.Get());

	// 야수전사
	KI[KIND_ON_BEASTWARRIOR].Init       ("KIND_ON_BEASTWARRIOR",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 대만 용병 - 고급 야수전사
	KI[KIND_ON_BEASTWARRIOR_UP].Init       ("KIND_ON_BEASTWARRIOR_UP",
		0, 
		ATB_ATTACK|ATB_PATIENT, 
		0, 
		0,
		0,
		0, 
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 손유창
	KI[KIND_ON_GENERAL6T].Init ("KIND_ON_GENERAL6T",
		KINDATB_GENERAL|KINDATB_AIRMAGIC, 
		ATB_PATIENT|ATB_ATTACK,
		0,
		0,
		ATB4_LONG_SPEAR,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, Text.General8KIDName.Get());

	// 도술사
	KI[KIND_ON_MAGIC_C].Init        ("KIND_ON_MAGIC_C",
		0,
		ATB_ATTACK|ATB_PATIENT,
		ATB2_MANA, 
		0,
		0,
		0,
		0, 
		ATTACKATB_MAGICCATTACK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// robypark 2004/7/22 16:21
	// 중국 용병 - 고급 도술사
	KI[KIND_ON_MAGIC_C_UP].Init        ("KIND_ON_MAGIC_C_UP",
		0,
		ATB_ATTACK|ATB_PATIENT,
		ATB2_MANA, 
		0,
		0,
		0,
		0, 
		ATTACKATB_MAGICCATTACK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 순비연
	KI[KIND_ON_GENERAL6_C].Init       ("KIND_ON_GENERAL6_C",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		ATB2_MANA, 
		0,
		ATB4_ICE_BARRIER,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SUNBIYOUN, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//비호
	KI[KIND_ON_AGILETIGER].Init       ("KIND_ON_AGILETIGER",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_TORNADO | ATB3_MAGIC4 | ATB3_MAGIC_RECALL_TIGER,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//졸개호랑이.
	KI[KIND_ON_PAWNTIGER].Init       ("KIND_ON_PAWNTIGER",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//올빼미
	KI[KIND_ON_OWL].Init       ("KIND_ON_OWL",
		0,  
//		ATB_ATTACK|ATB_PATIENT,
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_STRAIGHTSIGHT, 
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//표범
	KI[KIND_ON_LEOPARD].Init       ("KIND_ON_LEOPARD",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//흉노전사
	KI[KIND_ON_HUNSWARRIOR].Init       ("KIND_ON_HUNSWARRIOR",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	// 흉노기마병
	KI[KIND_ON_HUNSHORSE].Init       ("KIND_ON_HUNSHORSE",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HORSE,
		NULL, NULL);

	// 흉노기마궁수
	KI[KIND_ON_HUNSHORSEARCHER].Init       ("KIND_ON_HUNSHORSEARCHER",
		0,  
		ATB_ATTACK|ATB_ATTACKAIR|ATB_HORSE|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_ARROW, 0, 
		DEFENCEATB_HORSE,
		NULL, NULL);


//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 진시황릉 관련 START
//----------------------------------------------------------------------------

	// 문지기
	KI[KIND_ON_GATEKEEPER].Init       ("KIND_ON_GATEKEEPER",
		0, 
		ATB_ATTACK | ATB_PATIENT, 
		0, 
		0,
		ATB3_MAGIC4T, // 흑룡대차륜
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_MAGIC4T, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 기마병마용
	KI[KIND_ON_CLAYFIGURE].Init       ("KIND_ON_CLAYFIGURE",
		0,  
		ATB_ATTACK | ATB_PATIENT,
		0, 
		0,
		ATB4_STONE_MAGIC,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HORSE,
		NULL, NULL);

	// 불사조
	KI[KIND_ON_PHOENIX].Init ("KIND_ON_PHOENIX",
		KINDATB_FLY,
		ATB_ATTACK | ATB_ATTACKAIR | ATB_FLY | ATB_SHADOW | ATB_PATIENT | ATB_STRAIGHTSIGHT, 
		0,
		ATB3_GROUNDEXP,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_AIRPLANE,
		NULL, NULL);

	// 진시황의혼
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_CLAYFIGURE;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 400;
	KI[KIND_ON_JINEMPEROR].Init ("KIND_ON_JINEMPEROR",
		0, 
		ATB_ATTACK | ATB_PATIENT | ATB_MINRANGE, 
		0, 
		ATB3_MAGIC6C,
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_JINEMPEROR, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL, -1, -1, -1, 0, &subData );

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 진시황릉 관련 END
//----------------------------------------------------------------------------


	//의술사
	KI[KIND_ON_MEDICALMAN].Init       ("KIND_ON_MEDICALMAN",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_NEEDLEATTACK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 조선 용병 - 고급 의술사
	KI[KIND_ON_MEDICALMAN_UP].Init       ("KIND_ON_MEDICALMAN_UP",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0, 
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_NEEDLEATTACK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//허준
	KI[KIND_ON_HEOJUN].Init       ("KIND_ON_HEOJUN", 
		KINDATB_GENERAL|KINDATB_AIRMAGIC,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		ATB4_FLOWER_NEEDLE_RAIN,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_NEEDLEATTACK, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//음양사
	KI[KIND_ON_COSMIC].Init       ("KIND_ON_COSMIC", 
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0, 
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_PAPERDOLL, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 일본 용병 - 고급 음양사
	KI[KIND_ON_COSMIC_UP].Init       ("KIND_ON_COSMIC_UP", 
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_PAPERDOLL, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//세이메이
	KI[KIND_ON_SEIMEI].Init       ("KIND_ON_SEIMEI", 
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_SOUL_CHANGE,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SOULCHANGE/*세이메이 기본 미사일 공격*//*ATTACKATB_BIGSWORD*/, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//염력사
	KI[KIND_ON_YUMRUCK].Init       ("KIND_ON_YUMRUCK",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		ATB4_CLOUD_WIND,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_CLOUDWIND, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 대만 용병 - 고급 염력사
	KI[KIND_ON_YUMRUCK_UP].Init       ("KIND_ON_YUMRUCK_UP",
		0,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		ATB4_CLOUD_WIND,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_CLOUDWIND, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//장선화
	KI[KIND_ON_JANGSUNHWA].Init       ("KIND_ON_JANGSUNHWA",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		ATB4_STONE_MAGIC,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_CLOUDWIND, 0,  
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//장창무사
	KI[KIND_ON_LONGSPEAR].Init       ("KIND_ON_LONGSPEAR", 
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 중국 용병 - 고급 장창무사
	KI[KIND_ON_LONGSPEAR_UP].Init       ("KIND_ON_LONGSPEAR_UP", 
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//곽후.
	KI[KIND_ON_KOAKHU].Init       ("KIND_ON_KOAKHU", 
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_MAGIC8K,
		ATB4_GOD_SPEAR,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SANTA1].Init       ("KIND_ON_SANTA1",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SANTA2].Init       ("KIND_ON_SANTA2",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SANTA3].Init       ("KIND_ON_SANTA3",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SANTA4].Init       ("KIND_ON_SANTA4",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SANTA_TW].Init       ("KIND_ON_SANTA_TW",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SNOWMAN1].Init	("KIND_ON_SNOWMAN1",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SNOW, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_SNOWMAN2].Init	("KIND_ON_SNOWMAN2",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SNOW, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_MOTHER_DEER1].Init	("KIND_ON_MOTHER_DEER1", 
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	KI[KIND_ON_MOTHER_DEER2].Init	("KIND_ON_MOTHER_DEER2",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	KI[KIND_ON_MOTHER_DEER3].Init	("KIND_ON_MOTHER_DEER3",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	KI[KIND_ON_MOTHER_DEER4].Init	("KIND_ON_MOTHER_DEER4",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	KI[KIND_ON_MOTHER_DEER_TW].Init	("KIND_ON_MOTHER_DEER_TW",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	KI[KIND_ON_RAPTOR].Init	("KIND_ON_RAPTOR",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//티라노
	KI[KIND_ON_TIRANO].Init	("KIND_ON_TIRANO",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//익룡
	KI[KIND_ON_FLYDRAGON].Init	("KIND_ON_FLYDRAGON",
		KINDATB_FLY,
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BUTTERFLY, 0, 
		DEFENCEATB_AIRPLANE,
		NULL, NULL);

	//소환원숭이.
	KI[KIND_ON_MONKEY].Init	("KIND_ON_MONKEY",
		KINDATB_FLY,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// actdoll (031230)		온라인 - 배2
	KI[KIND_ON_SHIP2].Init       ("KIND_ON_SHIP2",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0,
		0, 
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP2].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP2].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배3
	KI[KIND_ON_SHIP3].Init       ("KIND_ON_SHIP3",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0, 
		0,
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP3].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP3].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배4
	KI[KIND_ON_SHIP4].Init       ("KIND_ON_SHIP4",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0, 
		0,
		0,
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP4].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP4].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배5
	KI[KIND_ON_SHIP5].Init       ("KIND_ON_SHIP5",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0,
		0, 
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP5].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP5].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배6
	KI[KIND_ON_SHIP6].Init       ("KIND_ON_SHIP6",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0, 
		0,
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP6].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP6].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배7
	KI[KIND_ON_SHIP7].Init       ("KIND_ON_SHIP7",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0, 
		0,
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP7].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP7].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/
	// actdoll (031230)		온라인 - 배8
	KI[KIND_ON_SHIP8].Init       ("KIND_ON_SHIP8",
		KINDATB_WATER, 
		ATB_REPAIR|ATB_SHIP,
		0, 
		0,
		0,
		0, 
		DYINGATB_SHIP|DYINGATB_IMMEDIATE,
		0,0, 
		DEFENCEATB_ARMOUR,
		Text.TShipK.Get() , NULL);
/*	
	KI[KIND_ON_SHIP8].SetFile(ANI_WAIT0,	TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 5);
	KI[KIND_ON_SHIP8].SetFile(ANI_MOVE,		TRUE,	"FILE_ON_CHAR_SHIP1", 0, 10,  0, 2);
*/



//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 START
//----------------------------------------------------------------------------

	// 염라대왕 - 완성
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_HELLMESSENGER;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 1000;
	KI[KIND_ON_HELLKING].Init	("KIND_ON_HELLKING",
		0,
		ATB_ATTACK | ATB_PATIENT,									// 캐릭터 물리행동 속성
		0,															// 캐릭터 마법행동 속성(대상을 오른쪽 버튼 클릭 시)
		ATB3_MAGIC6C | ATB3_GROUNDEXP,								// 캐릭터 특수기술 1
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD, ATTACKATB_MAGIC2,						// 공격 속성(근접, 마법)
		DEFENCEATB_GHOST,
		NULL, NULL, -1, -1, -1, 0, &subData);

	// 저승사자 - 완성
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_GANGSI;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 60;	// robypark 2004/9/2 15:19 강시소환에 필요한 마나량 수정(400 => 60)
	KI[KIND_ON_HELLMESSENGER].Init	("KIND_ON_HELLMESSENGER",
		0,
		ATB_ATTACK | ATB_PATIENT | ATB_MINRANGE,
		0, 
		ATB3_EARTHQUAKE,
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2, 0,
		DEFENCEATB_GHOST,
		NULL, NULL, -1, -1, -1, 0, &subData );

	// 유령 사무라이 - 완성
	KI[KIND_ON_GHOSTSAMURAI].Init	("KIND_ON_GHOSTSAMURAI",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0, 
		ATB3_KIMBOMB,
		0,
		0,
		0, 
		ATTACKATB_SWORD, ATTACKATB_KIMBOMB,
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 이무기 - 완성
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_VIPER;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 400;
	KI[KIND_ON_PYTHON].Init	("KIND_ON_PYTHON",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0, 
		ATB3_DARKFIRE,
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		0,
		ATTACKATB_BIGSWORD, ATTACKATB_PYTHON, 
		DEFENCEATB_GHOST,
		NULL, NULL, -1, -1, -1, 0, &subData );

	// 유령 코끼리 - 완성
	KI[KIND_ON_GHOSTELEPHANT].Init	("KIND_ON_GHOSTELEPHANT",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0, 
		ATB3_EARTHQUAKE,
		0,
		0, 
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 해저왕 - 완성
	KI[KING_ON_GHOSTSEAKING].Init	("KING_ON_GHOSTSEAKING",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0, 
		ATB3_SEAHORSECLON,
		0, 
		0,
		0,
		ATTACKATB_SWORD,0 , 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 해적선장 - 완성
	KI[KIND_ON_GHOSTPIRATECAPTAIN].Init	("KIND_ON_GHOSTPIRATECAPTAIN",
		0,
		ATB_ATTACK | ATB_ATTACKAIR | ATB_PATIENT | ATB_MINRANGE,
		0, 
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_GUN, 0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 해골 - 완성
	KI[KIND_ON_GHOSTSKELETON].Init	("KIND_ON_GHOSTSKELETON",
		0,
		ATB_ATTACK | ATB_PATIENT,	
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 해골병사 - 완성
	KI[KIND_ON_GHOSTSKELETONSOLDIER].Init	("KIND_ON_GHOSTSKELETONSOLDIER",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 무녀 - 완성
	KI[KIND_ON_GHOSTFEMALESHAMAN].Init	("KIND_ON_GHOSTFEMALESHAMAN",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0,
		0, 
		ATTACKATB_BIGSWORD,0,
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 유령 불꽃 - 완성
	KI[KIND_ON_GHOSTVOLCANOFLAME].Init	("KIND_ON_GHOSTVOLCANOFLAME",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_KIMBOMB,//폭뢰격.
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_VOLCANOFLAME, ATTACKATB_KIMBOMB, 
		DEFENCEATB_GHOST,
		NULL, NULL);
//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : 추가 캐릭터 - 유명계 관련 END
//----------------------------------------------------------------------------

	// 수호부(쥐)
	KI[KIND_ON_GUARDIAN_MOUSE].Init	("KIND_ON_GUARDIAN_MOUSE",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0,
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 수호부(소1)
	KI[KIND_ON_GUARDIAN_CAW1].Init	("KIND_ON_GUARDIAN_CAW1",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 수호부(소2)
	KI[KIND_ON_GUARDIAN_CAW2].Init	("KIND_ON_GUARDIAN_CAW2",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 수호부(소3
	KI[KIND_ON_GUARDIAN_CAW3].Init	("KIND_ON_GUARDIAN_CAW3",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);
	////////////////////////////////////////////////////////////////////////////////////////
	//수호부 6종류에 대한 캐릭터 (임시).

	KI[KIND_ON_GUARDIAN_CHICK].Init	("KIND_ON_GUARDIAN_CHICK",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	KI[KIND_ON_GUARDIAN_PIG].Init	("KIND_ON_GUARDIAN_PIG",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_GUARDIAN_PIG, 0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	KI[KIND_ON_GUARDIAN_SNAKE].Init	("KIND_ON_GUARDIAN_SNAKE",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,//ATB4_MANABURN,
		0, 
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_GUARDIAN_SNAKE,0,//ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	KI[KIND_ON_GUARDIAN_MONKEY].Init	("KIND_ON_GUARDIAN_MONKEY",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0, 
		0,
		0,//DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,0, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	// 온라인 - 침략왜구 2004-5-7
	// 조총왜구와 동일하며 이름만 다름
	KI[KIND_ON_JAP_INVADERS].Init         ("KIND_ON_JAP_INVADERS",
		0,
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT|ATB_LOWMOVE|ATB_LOWATTACK|ATB_MINRANGE,
		0, 
		0,
		0, 
		0,
		0, 
		ATTACKATB_GUN,0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
//----------------------------------------------------------------------------
// robypark (2004/05/11 13:15) : 추가 캐릭터 - 장수 2차 전직
//----------------------------------------------------------------------------
	// 온라인 - 거북차
	// 조선-이순신 2차 전직
	KI[KIND_ON_TURTLE_VEHICLE].Init         ("KIND_ON_TURTLE_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_FIRE_ROUND_DANCE/*강강수월래*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_TURTLE_VEHICLE/*거북차 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 뇌전차
	// 조선-허준 2차 전직
	KI[KIND_ON_THUNDER_VEHICLE].Init         ("KIND_ON_THUNDER_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_NOVA/*굉뇌*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_THUNDER_VEHICLE/*뇌전차 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 지진차
	// 일본-와키자카 2차 전직
	KI[KIND_ON_QUAKE_VEHICLE].Init         ("KIND_ON_QUAKE_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_FURY_OF_VOLCANO/*격노염폭*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_QUAKE_VEHICLE/*지진차 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 흑룡차
	// 일본-세이메이 2차 전직
	KI[KIND_ON_BLACK_DRAGON_VEHICLE].Init         ("KIND_ON_BLACK_DRAGON_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_POISONING_HEAVENS/*천지독살*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BLACK_DRAGON_VEHICLE/*흑룡차 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 발석거
	// 중국-순비연<=이령 2차 전직
	KI[KIND_ON_CATAPULT].Init         ("KIND_ON_CATAPULT",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_BOMB_OF_HEAVEN/*승천포*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_CATAPULT/*발석거 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 불랑기포차
	// 중국-이령<=순비연 2차 전직
	KI[KIND_ON_FARANGI_VEHICLE].Init         ("KIND_ON_FARANGI_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_SIEGE_MODE/*앙천대소*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_FARANGI_VEHICLE/*불랑기 공격*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);
	
	// 온라인 - 화룡차
	// 대만-손유창 2차 전직
	KI[KIND_ON_FIRE_DRAGON_VEHICLE].Init         ("KIND_ON_FIRE_DRAGON_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_ONSLAUGHT_FIREBALL/*맹격화룡파*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD/*큰검*/, ATTACKATB_FIRE_DRAGON_VEHICLE/*화룡차 공격*/,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 온라인 - 봉황비조
	// 대만-장선화 2차 전직
	KI[KIND_ON_CHINESEPHOENIX].Init         ("KIND_ON_CHINESEPHOENIX",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_SHADOW,
		0, 
		0,
		ATB4_FIRERAIN_OF_GOD/*천벌화시*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_CHINESEPHOENIX/*봉황비조 공격*/, 0,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 2차 장수 스킬 추가 관련
	// robypark 2004/6/4 20:28
	// 온라인 - 불랑기포차가 변신한 앙천대소
	// 중국-이령 2차 전직 기술 앙천대소
	// 불랑기포차와 동일하나 기술, 사정거리, 공격속도, 시야, 그림일부만 다르다.
	// CharaterList.txt파일에는 존재하지 않는다. 다만 CharCode.txt, CharInfo.txt파일에만 
	// 초상화, 박스크기, 등의 정보만 저장되어있다. CharCode.txt내에 KIND_ON_ROAR_OF_LAUGHTER에
	// 관한내용, 즉 캐릭터아이디는 U107로 불랑기포차의 그것과 동일하다.
	// CharInfo.txt파일 내 KIND_ON_ROAR_OF_LAUGHTER의 내용도 불랑기포차의 그것과 동일하다. 단지,
	// BottomIndent, XsizePercent, YsizePercent만 다르다.
	KI[KIND_ON_ROAR_OF_LAUGHTER].Init         ("KIND_ON_ROAR_OF_LAUGHTER",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_SIEGE_MODE_UNDO/*앙천대소=>불랑기포차 변신*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_ROAR_OF_LAUGHTER/*앙천대소 공격*/, 0,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// robypark 2004/9/17 13:32
	// 붉은 눈 해저인
	KI[KIND_ON_RED_EYE_SEABEDMAN].Init         ("KIND_ON_RED_EYE_SEABEDMAN",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// robypark 2004/9/17 13:32
	// 가짜 흉노전사
	// 흉노 전사와 이름만 다를 뿐 모든 설정은 동일하다.
	KI[KIND_ON_HUNSWARRIOR_FAKE].Init       ("KIND_ON_HUNSWARRIOR_FAKE",
		0,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 시작 /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 16:40
	// 공성전 관련 캐릭터 추가
	// 다음의 속성들은 공성전 유닛일 경우에 해석되는 것을 말한다.
	// 공성전 캐릭터 속성 설명: KINDATB_SIEGE_WARFARE_UNIT - 공성전 유닛을 나타내는 속성
	//							ATB_ATTACK - 공성전 유닛을 공격할 수 있다는 속성. 이 속성이 없으면 충차나 보급수레처럼 공성전 유닛 공격은 불가능하다.
	//							ATB_PATIENT - 보급품을 사용 (자동)소모한다는 속성. 보급수레처럼 보급품을 나눠 줄 수 있는 유닛에게서 보급품을 받을 수 있다는 의미로 사용된다.
	//							ATB_ATTACK_VILLAGE - 마을을 공격할 수 있다는 속성. 이 속성이 없으면 창병이나 보급수레처럼 마을에 대한 공격이 불가능하다.
	//							- 공성전 캐릭터의 공격 속성 값은 전투에서 적용되는 공격속성과 동일하게 적용받지 않는다.
	//							단지, 공성전 중에 이펙트 연출을 위하여 구분하기 위한 값이다.
	//							ATB_EFFECT_VILLAGECOUNTERATTACK_OIL	- 마을 반격 - 기름 공격을 받는다. 즉, 이펙트를 그려야 한다. ex) 공성 충차
	//							ATB_EFFECT_VILLAGECOUNTERATTACK_STONE - 마을 반격 - 돌 공격을 받는다. 즉, 이펙트를 그려야 한다. ex) 공성 검병
	//							ATB_STORE_AND_SUPPLY_SUPPLYGOODS - 공성전 보급품을 저장 및 다른 공성전 유닛에게 공급할 수 있다는 속성. ex) 공성전 보급수레
	//							ATTACKATB_SWORD - 근접 공격 공성전 유닛의 공격 이펙트 사용
	//							ATTACKATB_ARROW - 공성전 궁병 화살 공격
	//							ATTACKATB_UPGRADETOWERCANNON - 공성전 충차 마을 공격 이펙트 사용
	//							ATTACKATB_CATAPULT - 공성전 발석거 돌 공격
	// 공성전 - 검병
	KI[KIND_ON_SIEGEWARFARE_SWORDMAN].Init(	"KIND_ON_SIEGEWARFARE_SWORDMAN",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_ATTACK|ATB_PATIENT | ATB_ATTACK_VILLAGE | ATB_EFFECT_VILLAGECOUNTERATTACK_STONE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 공성전 - 창병
	KI[KIND_ON_SIEGEWARFARE_SPEARMAN].Init(	"KIND_ON_SIEGEWARFARE_SPEARMAN",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_ATTACK|ATB_PATIENT | ATB_ATTACK_VILLAGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_LONG_SPEAR, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 공성전 - 궁병
	KI[KIND_ON_SIEGEWARFARE_ARCHERMAN].Init(	"KIND_ON_SIEGEWARFARE_ARCHERMAN",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE| ATB_ATTACK_VILLAGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_ARROW, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 공성전 - 기마병
	KI[KIND_ON_SIEGEWARFARE_HORSEMAN].Init(	"KIND_ON_SIEGEWARFARE_HORSEMAN",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_ATTACK|ATB_PATIENT | ATB_ATTACK_VILLAGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_SWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// 공성전 - 충차
	KI[KIND_ON_SIEGEWARFARE_SIEGE_TOWER].Init(	"KIND_ON_SIEGEWARFARE_SIEGE_TOWER",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_PATIENT | ATB_ATTACK_VILLAGE | ATB_EFFECT_VILLAGECOUNTERATTACK_OIL,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_UPGRADETOWERCANNON, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 공성전 - 발석거
	KI[KIND_ON_SIEGEWARFARE_CATAPULT].Init(	"KIND_ON_SIEGEWARFARE_CATAPULT",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE| ATB_ATTACK_VILLAGE,
		0, 
		0,
		0,
		0, 
		0,
		ATTACKATB_CATAPULT, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 공성전 - 보급 수레
	KI[KIND_ON_SIEGEWARFARE_SUPPLY_WAGON].Init(	"KIND_ON_SIEGEWARFARE_SUPPLY_WAGON",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_PATIENT | ATB_STORE_AND_SUPPLY_SUPPLYGOODS/*마나를 가지고 있다*/,
		ATB2_HEAL/*다른 공성전 유닛 치료 가능*/, 
		0,
		0,
		0, 
		0,
		0, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	// 끝 /////////////////////////////////////////////////////////////////

	// 시작 ///////////////////////////////////////////////////////////////
	// robypark 2004/11/17:8
	// 11월 문화 유적 탐사 이벤트 몬스터 추가
	// 저팔계 삼계탕
	KI[KIND_ON_JEOPALGE_TYPE1].Init( "KIND_ON_JEOPALGE_TYPE1",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// 저팔계 장금복
	KI[KIND_ON_JEOPALGE_TYPE2].Init( "KIND_ON_JEOPALGE_TYPE2",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);
	// 끝 /////////////////////////////////////////////////////////////////

	// 저팔계 장금복
	KI[KIND_ON_JEOPALGE_TYPE3].Init( "KIND_ON_JEOPALGE_TYPE3",
		0,
		ATB_ATTACK | ATB_PATIENT,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//---------------------------------
	// 온라인 피난민 캐릭터
	// 온라인 - 피난민 조철희.
	KI[KIND_ON_REFUGEE_KEON].Init( "KIND_ON_REFUGEE_KEON",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_REFUGEE_KEON].m_ssBottomIndent = 15;
	
	// 온라인 - 피난민 이명화
	KI[KIND_ON_REFUGEE_MYUNGHWA].Init       ("KIND_ON_REFUGEE_MYUNGHWA",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_REFUGEE_MYUNGHWA].m_ssBottomIndent = 15;

	// 온라인 - 피난민 시마다 료
	KI[KIND_ON_REFUGEE_RYO].Init       ("KIND_ON_REFUGEE_RYO",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_REFUGEE_RYO].m_ssBottomIndent = 15;

	// 온라인 - 피난민 겐노 하나히네
	KI[KIND_ON_REFUGEE_HANAHINE].Init       ("KIND_ON_REFUGEE_HANAHINE",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	KI[KIND_ON_REFUGEE_HANAHINE].m_ssBottomIndent = 15;

	// 온라인 - 피난민 왕소룡
	KI[KIND_ON_REFUGEE_SORYUNG].Init       ("KIND_ON_REFUGEE_SORYUNG",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	KI[KIND_ON_REFUGEE_SORYUNG].m_ssBottomIndent = 15;

	// 온라인 - 왕미령
	KI[KIND_ON_REFUGEE_MIRYUNG].Init       ("KIND_ON_REFUGEE_MIRYUNG",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_REFUGEE_MIRYUNG].m_ssBottomIndent = 15;

	// 온라인 - 피난민 당호엽 
	KI[KIND_ON_REFUGEE_DANG].Init       ("KIND_ON_REFUGEE_DANG",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);

	KI[KIND_ON_REFUGEE_DANG].m_ssBottomIndent = 15;

	// 온라인 - 피난민 노수연 
	KI[KIND_ON_REFUGEE_SUYOUN].Init       ("KIND_ON_REFUGEE_SUYOUN",
		0,
		0,
		0,
		0,
		0,
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		0, 0,
		DEFENCEATB_HUMAN,
		NULL, NULL);
	
	KI[KIND_ON_REFUGEE_SUYOUN].m_ssBottomIndent = 15;

	//////////////////////////////////////////////////////
	// robypark 2004/10/5 14:47
	// 캐릭터들의 애니메이션 설정
	Parse_CharacterAnimationInformation();

	//////////////////////////////////////////////////////
	// 크래킹 방지를 위한 처리
	if(CheckKindInfo() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: TransKindOnlineToImjin()
// Code: actdoll (2003-12-30)
// Desc: 서버용 캐릭터 인덱스를 게임용 캐릭터 인덱스로 바꿔준다.
//		kind			- 서버용 캐릭터 인덱스 번호
//		bShowErrorMsg	- 에러메시지를 보여주려면 true, 아니라면 false
//		return			- 존재한다면 게임용 캐릭터 인덱스, 아니라면 -1
//-----------------------------------------------------------------------------
SI32	TransKindOnlineToImjin( SI32 kind, BOOL bShowErrorMsg )
{
	register	SI32	i;
	for(i=0; i<KIND_ON_MAX; i++)
	{
		if( KI[i].m_siServerID == kind )		return i;
	}

	// 에러 메시지를 쓴다는 것은 확인용이므로 에러를 보여주고 임시로 사슴값으로 리턴
	if( bShowErrorMsg )		
	{
		clGrp.Error("No Char:TKOTI", "[%d]", kind);
		return KIND_ON_DEER;
	}

	return KIND_ON_INVALID;
/*
	SI32 onlinekind, battlekind;
	SI32 index = 0;

	while(1)
	{
		onlinekind = TransKindData[index*2];
		if(onlinekind == -1)break;

		battlekind = TransKindData[index*2 + 1];

		if(kind == onlinekind)
		{
			return battlekind;
		}
		index++;
	}

	// 에러 메시지를 쓴다는 것은 확인용이므로 에러를 보여주고 임시로 사슴값으로 리턴
	if( bShowErrorMsg )		
	{
		clGrp.Error("No Char:TransKindOnlineToImjin", "[%d]", kind);
		return KIND_ON_DEER;
	}

	// 에러 메시지를 안쓴다는 것은 검색용으로 찾겠다는 뜻임
	return -1;
*/
}

//-----------------------------------------------------------------------------
// Name: TransKindImjinToOnline()
// Code: actdoll (2003-12-30)
// Desc: 게임용 캐릭터 인덱스를 서버용 캐릭터 인덱스로 바꿔준다.
//		kind			- 게임용 캐릭터 인덱스 번호
//		bShowErrorMsg	- 에러메시지를 보여주려면 true, 아니라면 false
//		return			- 존재한다면 서버용 캐릭터 인덱스, 아니라면 0
//-----------------------------------------------------------------------------
SI32	TransKindImjinToOnline( SI32 kind, BOOL bShowErrorMsg )
{
	BOOL	bError	= false;
	if( kind <= KIND_ON_INVALID || kind >= KIND_ON_MAX )	bError	= true;	// 범위 오버
	if( !bError && KI[kind].m_siServerID == 0 )				bError	= true;	// 해당 캐릭터가 존재 안함

	if( bError )
	{
		// 에러 메시지를 쓴다는 것은 확인용이므로 에러를 보여주고 임시로 사슴값으로 리턴
		if( bShowErrorMsg )		
		{
			clGrp.Error("No Char:TKITO", "[%d]", kind);
			return KI[KIND_ON_DEER].m_siServerID;
		}
		return 0;
	}

	return KI[kind].m_siServerID;
/*
	SI32 onlinekind, battlekind;
	SI32 index = 0;

	while(1)
	{
		onlinekind = TransKindData[index*2];
		if(onlinekind == -1)break;

		battlekind = TransKindData[index*2 + 1];

		if(kind == battlekind)
		{
			return onlinekind;
		}
		index++;
	}

	// 에러 메시지를 쓴다는 것은 확인용이므로 에러를 보여주고 임시로 사슴값으로 리턴
	if( bShowErrorMsg )
	{
		clGrp.Error("No Char2", "[%d]", kind);
		return 6221;
	}

	// 에러 메시지를 안쓴다는 것은 검색용으로 찾겠다는 뜻임
	return -1;
*/
}

//특정 캐릭터에 대해서 공격시도할 때 효과음을 연주한다. 
BOOL PlayTryAttackEffect(SI32 onlinekind)
{
	SI32 battlekind = TransKindOnlineToImjin(onlinekind);

	if(KI[battlekind].m_siTryAttackEffect)
	{
#ifndef _USE_HASHCODE_EFFECTSOUND
		PushEffect(KI[battlekind].m_siTryAttackEffect, 0, 0);
#else
		HashCodePushEffect(KI[battlekind].m_siTryAttackEffect, 0, 0);
#endif
	}

	return TRUE;

}

//특정 캐릭터를 필드상에 보이게 할 때 효과음을 연주한다. 
BOOL PlayShowPetEffect(SI32 onlinekind)
{
	SI32 battlekind = TransKindOnlineToImjin(onlinekind);

	if(KI[battlekind].m_siSelectEffect)
	{
#ifndef _USE_HASHCODE_EFFECTSOUND
		PushEffect(KI[battlekind].m_siSelectEffect, 0, 0);
#else
		HashCodePushEffect(KI[battlekind].m_siSelectEffect, 0, 0);
#endif
	}
 
	return TRUE;

}

// 공격 속성에 따라 공격 지연 시간을 얻어온다. 
SI32 DecideAttackDelayByAttackAtb(SI32 attackatb)
{

	switch(attackatb)
	{
	case ATTACKATB_GUN: // 총 
		return 10;
		break;

	case ATTACKATB_ARROW:  // 활    
		return 4;
		break;

	case ATTACKATB_BIGSWORD:	// 칼 
		return 0;
		break;

	case ATTACKATB_SWORD:		// 창 
		return 2;
		break;

	case ATTACKATB_DART:		// 표창.
		return 2;
		break;

	case ATTACKATB_DOLL1:		// 무명인형 
	case ATTACKATB_DOLL2:		// 하카다인형 
	case ATTACKATB_KUKET:		// 쿠켓
	case ATTACKATB_SNOW:
		return 2;
		break;

	case ATTACKATB_JUSTICEK:	// 부채. 
		return 5;
		break;
	}

	return 0;

}


//-----------------------------------------------------------------------------
// Name: CheckKindInfo()
// Code: actdoll (2003-12-30)
// Desc: 이 정보 파일이 해킹되었는지를 검사한다.
//-----------------------------------------------------------------------------
BOOL CheckKindInfo()
{
	// 캐릭터 정보의 합계
	register	SI32	i;
	DWORD				unique	= 0;
	SI32				sum		= 0;

// robypark 2004/10/2 11:55
#ifdef _DEBUG
	// 디버그 모드에서 캐릭터 체크 섬을 프로그램 시작할 때 한번만 검사하도록 수정.
	static bool bOneTimer = false;

	if (false == bOneTimer)
	{
		bOneTimer = true;
	}
	else
		return TRUE;
#endif	// _DEBUG
   
	// 유니크 값을 구한다. 
	for( i=0; i<KIND_ON_MAX; i++ )	unique += KI[i].m_uiUnique;
	
	// 치트된 것이라면, 
	if((unique % 10000) != DF_VALUE_BU)
	{
		// 경고 메시지 
		clGrp.Error("Warning", " %s [%d]", Text.CheatedGame.Get(), unique%10000);

// actdoll (2004/02/27 17:27) : 디버그모드시에는 표기만 해주고 진행을 한다.
#ifndef	_DEBUG
		return FALSE;
#endif//_DEBUG
	}

	// TransKindData[] 값에 조작이 없었는지 확인한다. 
	for( i=0; i<KIND_ON_MAX; i++ )	sum += KI[i].m_siServerID;

	// TransKindData[] 값에 조작이 있다면 에러!
	if( sum != DF_VALUE_CS )
	{
		clGrp.Error( "fds937h", "fd9334h:%d", sum );
// actdoll (2004/02/27 17:27) : 디버그모드시에는 표기만 해주고 진행을 한다.
#ifndef	_DEBUG
		return FALSE;
#endif//_DEBUG
	}

	return TRUE;
}
