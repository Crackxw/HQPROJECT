//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 메인문
//	File Name		: Main.cpp
//	Birth Date		: 2004. 01. 01.
//	Creator			: 거 상 팀
//	Editer			: 김태곤, 양정모, 이윤석, 백상현, 이준석, 정진욱, 홍성훈, 조현준
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		거상의 메인문입니다.
//
//===================================================================================================

#ifndef __MAIN_H__
#define __MAIN_H__

//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: 정의 및 선언
//-----------------------------------------------------------------------------
#pragma warning ( disable : 4786 )

//--------------------------------------------------------------------------
// 각 국가별 언어 참조 코드(폴더 설정시 사용) - 서비스 시작된 순서대로 작성
//--------------------------------------------------------------------------
typedef enum
{
	ELANGAGECODE_NONE		= 0		,// 사용하지 않음
		
	ELANGAGECODE_KOREAN				,// 한국
	ELANGAGECODE_TAIWAN				,// 대만
	ELANGAGECODE_JAPAN				,// 일본
	ELANGAGECODE_HONGKONG			,// 홍콩
	ELANGAGECODE_CHINA				,// 중국
	ELANGAGECODE_INDONESIA			,// 인도네시아

	ELANGAGECODE_MAX
} ELanguageCode;

//--------------------------------------------------------------------------
// 각 국가별 인식코드 - 각 국가별 전화번호 코드 - 서비스 시작된 순서대로 작성
//--------------------------------------------------------------------------
extern	const	int		DF_CODE_NATION_IDENTITY_KOREA;		// 한국
extern	const	int		DF_CODE_NATION_IDENTITY_TAIWAN;		// 대만
extern	const	int		DF_CODE_NATION_IDENTITY_JAPAN;		// 일본
extern	const	int		DF_CODE_NATION_IDENTITY_HONGKONG;	// 홍콩
extern	const	int		DF_CODE_NATION_IDENTITY_CHINA;		// 중국
extern	const	int		DF_CODE_NATION_IDENTITY_INDONESIA;	// 인도네시아

//--------------------------------------------------------------------------
// 각 언어별 코드페이지
//--------------------------------------------------------------------------
extern	const	int		DF_CODE_CODEPAGE_ENGLISH;			// 영문(기본)
extern	const	int		DF_CODE_CODEPAGE_KOREAN;			// 한국어 
extern	const	int		DF_CODE_CODEPAGE_JAPANESE;			// 일본어
extern	const	int		DF_CODE_CODEPAGE_CHINESE_SIMP;		// 중국어(중국, 싱가포르)
extern	const	int		DF_CODE_CODEPAGE_CHINESE_TRAD;		// 중국어(대만, 홍콩)

//--------------------------------------------------------------------------
// 각 국가별 언어 참조 코드(폴더 설정시 사용) - 서비스 시작된 순서대로 작성
//--------------------------------------------------------------------------
extern	char*	const	DF_TEXT_SERVICE_NATION_LIST[ELANGAGECODE_MAX];


// 메인화면 라이센스 텍스트
extern	const	char*	DF_TEXT_COPYRIGHT_WITH_GAMANIA;			// 조이온 감마니아 카피라이트 (한국/대만/일본/홍콩/중국)
extern	const	char*	DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;		// 조이온 볼레넷 카피라이트 (인도네시아)

// 임진록2+ 조선의 반격
#define _IMJIN2PLUS	      					1	

#define _ONLINE							1	// 거상을 포함하는가


#define CLASSNAME "GersangClient"

#ifdef _ENGLISH   // 영어 버전이면, 
#define NAME  "Gersang"
#define TITLE "Gersang"
#else             // 한글 버전이면,  
#define NAME  "거상"
#define TITLE "거상"
#define _COMPANYLOGO				1   // 유통회사 로고를 넣을까 말까?
#endif



#define _MODE800							1

// 매직 스테이션
#define REG_MAGICSTATION						1


#define _RANDOMSEED								1	// 지도가 랜덤하게 생성된다. 주석 풀것

//#define _SHOWMAPCOORDINATE					1	// 지도에 좌표를 표시한다.   
//#define _MYDEBUG

//#define _DEMO									1
#ifdef _DEMO
	//#define TYPE_YONSEI						1	// 연세 할리우드 디지털 교육용 프로그램(일 때만 주석 푼다)
	/* <연세 할리우드 디지털 교육용 프로그램>
  	1. 메인화면교체
	2. 혼자하기와 여럿이 하기만 된다. 
	3. hqnet은 지원하지 않는다. 
	4. 최종 제작진 이름을 바꿔놓는다(배경그림도 바꾼다. 한판 끝나면 무조건 보이게 한다).
	5. 시디는 인식하지 않고, 하드에 copy하는 것만으로 실행할 수 있어야 한다.
	6. 모든 나라 다 가능해야 한다
	*/
#endif

//#define _QUICKMODE							1   // 빠른 게임 진행...

//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: 필요한 헤더 화일(윈도우 제공)
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <graphics16.h>
#include <ddraw.h>
#include <dsound.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#include <assert.h>
#include <io.h>
#include <wingdi.h>
#include <math.h>
#include <commctrl.h>
#include "GSCDefine.h"


#define WINTER_TEXT_COLOR RGB(250, 250, 250)						// 겨울용 글자색 
#define NORMAL_TEXT_COLOR RGB(250, 250, 250)//RGB(180, 180, 180)	// 겨울외의 글자색 
#define COLOR_MENU_TEXT   (RGB( 163, 145, 122 ))

#define RED_TEXT_COLOR   (RGB( 250, 0, 0 ))							// 붉은색 텍스트 

#define PLAYER0_TEAM_COLOR  RGB(11, 11, 254)
#define PLAYER1_TEAM_COLOR  RGB(160, 0, 0)
#define PLAYER2_TEAM_COLOR  RGB(135, 0, 135)
#define PLAYER3_TEAM_COLOR  RGB(0, 135, 135)
#define PLAYER4_TEAM_COLOR  RGB(161, 161, 0)
#define PLAYER5_TEAM_COLOR  RGB(0, 135, 0)
#define PLAYER6_TEAM_COLOR  RGB(161, 104, 0)
#define PLAYER7_TEAM_COLOR  RGB(161, 161, 161)

// 충돌 속성 
#define BLOCK_NONE								1
#define BLOCK_DEEPWATER							2
#define BLOCK_SHALLOWWATER						4
#define BLOCK_CASTLE							8
#define BLOCK_WALL								16
#define BLOCK_NORMAL							32
#define BLOCK_TREE								64
#define BLOCK_LAND								128
#define BLOCK_NEWBLK							256
#define	BLOCK_ICEWALL							512	
#define BLOCK_RUIN								1024	

// 256부터 시작된다.
#define BLOCK_AIRCHAR							256			// 공중용 캐릭터 
#define BLOCK_AIRMOVE							512
	
// 0x1000부터 시작된다. 
#define BLOCK_CHAR								0x1000      // 캐릭터의 속성 
#define BLOCK_MOVE								0x2000      // 이동용 캐릭터의 속성

///////////////////////////////////////////////////////////////

// 땅바닥에 붙는 불의 수명을 정의 
enum FireLifeTime {GROUNDFIRE_LIFETIME_LATE = 2, GROUNDFIRE_LIFETIME_EARLY = 4, GROUNDFIRE_LIFETIME = 6};

#define NOOBJECT								0
#define OBJECT1									1
	
#define FILE_NAME_SIZE							256
#define USER_FILE_NAME_SIZE						60           // 임의게임에서 사용자 지도 이름의 길이

#define MAX_MERCHANT_SOLDIER_NUMBER				12			// 용병을 포함한 플레이어 1인당 캐릭터 수.  
	
#define MAX_CHAR_NUMBER							200			// 게임에 등장할 수 있는 최대의 캐릭터 갯수 
															 // 건물과 캐릭터 포함함.
#define MAX_SELECTED_CHAR_NUMBER				12			// 한번에 선택될 수 있는 최대 캐릭터 갯수 
#define MAX_MULTI_ORDER_NUMBER					10			// 임시로 저장할 수 있는 최대 멀티 명령의 갯수 


#define MAX_MENU_NUMBER						12   // 캐릭터가 가질 수 있는 최대 명령의 갯수 

#define MAX_ORDER_QUE							50
#define MAX_BULLET_NUMBER						100
#define MAX_SMOKE_NUMBER						300
#define MAX_CLOUD_NUMBER						40

#define MAX_NEED_HELP_DELAY						10
#define MAX_FIRE_DELAY							5
#define MAX_SQRT_RANGE							1000        
#define SQRT_MAX_SIZE							20

#define MAX_MAP_XSIZE						100
#define MAX_MAP_YSIZE						100
#define MAX_GENERAL_NUMBER_PER_COUNTRY		8		// 국가당 최대 장수의 수 

#define MAX_DISCOVER_DELAY						100
#define MAX_SHIELD_DELAY						350		// 방어막의 내구성 
#define MIN_SHIELD_DELAY						100		// 용병의 쉴드.
#define MAX_CHAT_MESSAGE_SIZE					50		// 채팅 메시지의 최대 사이즈 
#define MAX_GAME_BUTTON							30		// 한번에 나올 수 있는 최대 게임 버튼의 수. 

#define PATRIOT_BAR_WIDTH						510
#define PATRIOT_BAR_HEIGHT						12

#define MAX_BRIEFING_TEXT_XSIZE			357  // 브리핑에서 텍스트를 미리 찍어보는 공간의 가로 사이즈 
#define MAX_BRIEFING_TEXT_YSIZE			248  // 브리핑에서 텍스트를 미리 찍어보는 공간의 세로 사이즈

#define NEW_SELECTED_DELAY						6
#define SMALL_BLACK_CHECK_DELAY					6

#define MAX_MULTI_PLAYER						8   // 최대 사용자 개수 (군주)
#define MAX_BASE_NUMBER							8	//  가질 수 있는 최대 기지의 숫자 
#define MAX_CASLTE_SPR_NUMBER					5   // 성에 사용될 스프라트의 갯수 

#define MAX_WAIT_STEP_NUMBER					16	// 캐릭터가 가질 수 있는 최대 정지 모션 프레임의 수 
#define MAX_WAIT_TYPE							2   // 대기시 취하는 애니메이션의 최대 수 .

#define MAX_MOVE_STEP_NUMBER					10	// 캐릭터가 가질 수 있는 최대 이동 모션 프레임의 수 

#define GAMECOLOR_RED						69
#define GAMECOLOR_YELLOW					70
#define GAMECOLOR_GREEN						68

#define MAX_ANIMATION_PER_UNIT					15		// 유닛당 가질 수 있는 애니메이션의 총 갯수. 

#define DOUBLE_CLICK_DELAY						300
 
#define MIN_VALID_FRAME_NUMBER					1500	// 멀티게임에서 드랍시 패배가 유효하기 시작한 프레임....
#define MULTI_GAME_DIVIDER_FACTOR				160		// 멀티 게임에서 점수는 이 숫자로 나눈다. 
#define MAX_SCORE_PER_GAME						15		// 게임당 얻을 수 있는 최고 점수 
#define DEFEAT_HANDICAP							3		// 패배시에는 점수를 이 숫자만큼 나눈다. 

#ifdef _QUICKMODE
	#define DEFAULT_MULTI_PLAY_DELAY_TIME		35		// 게임의 기본 지연 시간 (프레임당 시간)
#else
	#define DEFAULT_MULTI_PLAY_DELAY_TIME		50		// 게임의 기본 지연 시간 (프레임당 시간)	
#endif

#define	RETREAT_PERMIT_CLOCK				20000		// 전투시 도망가면 안되는 시간(clock) 

// 판넬의 높이 
#define PANNEL_YSIZE						120

// 게임 방식 
#define GAME_MODE_NORMAL						0		// 적건물과 건물 생산 가능 캐릭터 섬멸시 승리 
#define GAME_MODE_FLAG							1		// 적의 깃발을 빼앗아 아군 본영으로 가져오면 승리 
#define GAME_MODE_TIMELIMIT						2		// 시간 제한 게임 
#define GAME_MODE_HERO							3		// 장수들간의 일기토. 
#define GAME_MODE_ONLINEBATTLE					4		// 온라인 전투. 

// 지도 타일의 유형 
#define MAP_TYPE_NORMAL							0
#define MAP_TYPE_SNOW							1
#define MAP_TYPE_BROWN							2
#define MAP_TYPE_GREEN							3
#define MAP_TYPE_DARK							4
#define MAP_TYPE_VOLCANO						5
#define MAP_TYPE_BRIGHT							6

#define HILL_NORMAL        						0
#define HILL_SLOPE         						1
#define HILL_PLANE         						2
			
enum etArea{
	AREA_LAND	=	1, 
	AREA_AIR	=	2		
};


						
// 방향 정의 						
#define SOUTH              						1
#define SW                 						2
#define WEST               						4
#define NW										8
#define NORTH               					16
#define NE                  					32
#define EAST                					64
#define SE                  					128 
						
#define SSW										1000						
#define WWS 									1001						
#define WWN 									1002						
#define NNW 									1003						
#define NNE 									1004						
#define EEN 									1005						
#define EES 									1006
#define SSE 									1007

#define ATTACKAREA_LAND							1
#define ATTACKAREA_AIR							2 

#define ATTACK_MODE_CHAR						1
#define ATTACK_MODE_GROUND						2


//----------------------------------------------------
//    1번째 속성 (움직임 등 물리적인 특징.) 
//    게임중 상황에 따라 속성을 없애기도, 추가하기도 할 수 있다. 
//    만약 고정 불변의 속성이어야 한다면 KINDATB_ 로 분류할 것. 
//----------------------------------------------------
// 윈형의 blackcheck, 시야의 영향을 받지 않는다. 
#define ATB_CIRCLEBALCKCHECK					0x00000002         
#define ATB_ATTACK								0x00000004	// 공격 속성
#define ATB_ATTACKGROUND						0x00000008	// 지상 공격 속성
#define ATB_FLY									0x00000010	// 비행 캐릭터 
#define ATB_SHIP								0x00000020	// 선박
#define ATB_ABSANIWAIT							0x00000080	// 현재 동작에 관계없이 일정한 애니지속 
#define ATB_ATTACKAIR							0x00000400	// 공중 공격 능력
#define ATB_TRANSPARENT							0x00000800	// 투명하게 찍는다. 
#define ATB_HORSE								0x00001000  // 말을 탄 캐릭터인가?

// robypark 2005/1/6 18:11
#define ATB_EFFECT_VILLAGECOUNTERATTACK_OIL		0x00002000	// 마을 반격 - 기름 공격을 받는다. 즉, 이펙트를 그려야 한다. ex) 공성 충차
#define ATB_EFFECT_VILLAGECOUNTERATTACK_STONE	0x00004000	// 마을 반격 - 돌 공격을 받는다. 즉, 이펙트를 그려야 한다. ex) 공성 검병

// robypark 2005/1/8 11:1
#define ATB_STORE_AND_SUPPLY_SUPPLYGOODS		0x00008000	// 공성전 보급품을 저장 및 다른 공성전 유닛에게 공급할 수 있다는 속성. ex) 공성전 보급수레

#define ATB_PATIENT								0x00010000	// 치료가능한 캐릭터 
#define ATB_REPAIR								0x00020000	// 수리 가능한 캐릭터 
#define ATB_SHADOW								0x00200000	// 그림자를 갖는가?
#define ATB_LOWMOVE								0x00400000	// 포복 이동이 가능하다.
#define ATB_LOWATTACK							0x00800000	// 포복 공격이 가능하다.
#define ATB_MINRANGE							0x01000000	// 접근전 불가 
#define ATB_STRAIGHTSIGHT						0x02000000	// 원거리 시야 
#define ATB_ICE									0x04000000	// 얼음속성. 
#define ATB_SELFEXP								0x08000000	// 자폭 속성
#define ATB_CAPTURE								0x20000000	// 점령 능력
#define ATB_BETRAY								0x40000000	// 현혹 능력

//----------------------------------------------------
// robypark 2004/10/29 14:34
//    1번째 속성 (움직임 등 물리적인 특징.) 
//----------------------------------------------------
#define ATB_ATTACK_VILLAGE						0x80000000	// robypark 2004/10/29 14:34 - 마을 공격 가능한 공성전 캐릭터인지를 나타내는 속성값

//----------------------------------------------------
//    2번째 속성 
//----------------------------------------------------
#define ATB2_HEAL								0x40		// 치료가능한 캐릭터인가?
#define ATB2_MANA								0x80		// 마나 회복을 한다.
#define ATB2_WARP								0x1000		// 공간이동이 가능한 캐릭터인가?
#define ATB2_HIDE								0x2000
#define ATB2_WARPBASE							0x4000		// 닌자들이 워프해가는 본부 역할의 유닛인가?
#define ATB2_STEALEXP							0x10000		// 경험치 빼앗는 능력
#define ATB2_SAMMISSILE							0x100000	// 대공 미사일을 가지는가
#define ATB2_GUNSMOKE							0x200000	// 총알 발사 연기
#define ATB2_SAMC								0x400000	// 천궁의 효과음
#define ATB2_CANNON2KFIRE						0x800000	// 대장군포의 효과음
#define ATB2_BETRAY								0x2000000   // 현혹 능력
#define ATB2_BETRAY2							0x10000000  // 현혹술
#define ATB2_STEALTECH							0x20000000  // 기술 훔치기
#define ATB2_INITTRAINSPOT						0x40000000	// 처음부터 집결지가 설정되어 있음(건물 남쪽)
#define ATB2_MINE								0x80000000	// 지뢰를 매설할 수 있는 속성

//------------------------------------------------------------------------------------------------
//    3번째 속성 : 마법 관련 속성. 
//------------------------------------------------------------------------------------------------
#define ATB3_ENEMYCLON							0x00000001	// 적을 복제하는 속성
#define ATB3_MAGIC5J							0x00000002	// 만월도끼춤. 
#define ATB3_MAGIC8K							0x00000004	// 쐐기지르기
#define ATB3_MAGIC4								0x00000008  // 기공신포
#define ATB3_FIREARROW							0x00000010  // 불화살 
#define ATB3_FIREARROW2							0x00000020  // 마법 불화살 
#define ATB3_SEAHORSECLON						0x00000040  // 해마 복제 
#define ATB3_CLON								0x00000080  // 일반 분신
#define ATB3_KIMBOMB							0x00000100	// 폭뢰격을 사용할 수 있는 속성. 
#define ATB3_SPECIALATTACK						0x00000200	// 일격필살.
#define ATB3_MAGIC6C							0x00000400	// 포박술
#define ATB3_TORNADO							0x00000800  // 풍백술
#define ATB3_EARTHQUAKE							0x00001000  // 지진술
#define ATB3_LIGHTNING							0x00002000	// 번개술
#define ATB3_ABSORBLIFE							0x00004000  // 흡수술. (적의 체력을 빼앗는다. ) 
#define ATB3_SHIELDMAGIC						0x00008000	// 방어막 마법을 구사할 수 있는가?
#define ATB3_EXTINGUISH							0x00010000	// 불을 끄는 속성
#define ATB3_GROUNDEXP							0x00020000	// 연옥술
#define ATB3_CLON2								0x00040000  // 식귀,아귀 분신. 
#define ATB3_MAGICTEST							0x00080000  // 테스트 마법.
#define ATB3_MAGIC_RECALL_PANDA					0x00100000  // 팬더 소환.
#define ATB3_MAGIC_RECALL_MONKEY				0x00200000  // 황룡 소환.
#define ATB3_MINE								0x00400000	// 지뢰 매설.
#define ATB3_POISONBULLET						0x00800000  // 독탄. 
#define ATB3_MAGIC4T							0x01000000  // 원심분쇄격(흑룡대차륜)
#define ATB3_YELLOWSAND							0x02000000  // 황사.
#define ATB3_MAGIC_RECALL_AMAKUSA				0x04000000  // 요술사 소환.
#define ATB3_ICE								0x08000000	// 이사의 사념 주변 얼리는 공격.
#define ATB3_SOUL								0x10000000	// 왕거한 야수의혼.
#define	ATB3_GHOST								0x20000000	// 페르난데스 귀신탄.
#define ATB3_DARKFIRE							0x40000000	// 임평후 암흑화염 폭풍.
#define ATB3_MAGIC_RECALL_TIGER					0x80000000	// 비호 졸개 호랑이 소환.

#define ATB4_IVORY								0x00000001  // 축융2차  상아폭격 
#define ATB4_BLUEDRAGON							0x00000002	// 동방불패  비격청룡권 
#define ATB4_RUNNINGFIRE						0x00000004	// 이령     연발포탄 
#define ATB4_ENERGYPA							0x00000008  // 무도가	장풍
#define ATB4_SNOWATTACK							0x00000010  // 수도승   설공신포
#define ATB4_FIREATTACK							0x00000020  // 이령     화공신포
#define ATB4_FIREENERGYPA						0x00000040	// 동방불패 열화장풍(비격황룡권).

#define ATB4_DOUBLE_SWORD						0x00000080	// 고니시 쌍검난무
#define ATB4_LONG_SPEAR							0x00000100	// 손유창 장창소환
#define ATB4_ICE_BARRIER						0x00000200	// 순비연 빙석술
#define ATB4_FLOWER_NEEDLE_RAIN					0x00000400	// 허준 만천화우
#define ATB4_SOUL_CHANGE						0x00000800	// 세이메이 빙의술
#define ATB4_STONE_MAGIC						0x00001000	// 장선화 석괴술
#define ATB4_GOD_SPEAR							0x00002000	// 곽후 신창노도(일격필살)
#define ATB4_CLOUD_WIND							0x00004000	// 염력사 풍모술
#define ATB4_SHIELD								0x00008000	// 쉴드
#define ATB4_MANABURN							0x00010000	// 마나번.
#define ATB4_MAGIC_RABBIT						0x00040000	// 토끼 소환(수호부)
#define ATB4_MAGIC_RECALL_DAMDUK				0x00100000	// 공통소환

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:55) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define ATB4_FIRE_ROUND_DANCE					0x00200000		// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
#define ATB4_NOVA								0x00400000		// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
#define ATB4_FURY_OF_VOLCANO					0x00800000		// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
#define ATB4_POISONING_HEAVENS					0x01000000		// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
#define ATB4_BOMB_OF_HEAVEN						0x02000000		// 승천포(중국-발석거, 순비연 2차 전직 기술)
#define ATB4_SIEGE_MODE							0x04000000		// 앙천대소(중국-불랑기포차, 이령 2차 전직 기술)
#define ATB4_ONSLAUGHT_FIREBALL					0x08000000		// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
#define ATB4_FIRERAIN_OF_GOD					0x10000000		// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

// robypark 2004/6/4 14:6
#define ATB4_SIEGE_MODE_UNDO					0x20000000		// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령 2차 전직 기술)


//------------------------------------------------------------------------------------------------
//    죽음에 관련된 속성
//------------------------------------------------------------------------------------------------
#define DYINGATB_SHIP							1
#define DYINGATB_BUILDING						2			// 건물의 폭파 
#define DYINGATB_IMMEDIATE						4			// 잔해를 남기지 않고 죽음  
#define DYINGATB_EXP							8			// 폭발로 사라져 죽음    
#define DYINGATB_NOCARCASS						16			// 시체를 남기지 않음 
		

//------------------------------------------------------------------------------------------------
// 데미지 속성 
//------------------------------------------------------------------------------------------------
#define HITATB_SWORD							1
#define HITATB_RANGE							2
#define HITATB_FIREARROW						3
#define HITATB_MAGIC							4
#define HITATB_FIRE								5
#define HITATB_SKILL							6

//------------------------------------------------------------------------------------------------
// 공격 속성		
//------------------------------------------------------------------------------------------------
/*
typedef enum	ENUM_ATTACKATB
{
	ATTACKATB_SWORD							1
	ATTACKATB_STRONGGUN						2
	ATTACKATB_AIRMISSILE					3
	ATTACKATB_SAMMISSILE					4
	ATTACKATB_CANNON2C						5			// 발석거 
	ATTACKATB_NEEDLE						6			// 침
	ATTACKATB_CANNON						7
	ATTACKATB_GUN							8
	ATTACKATB_MAGIC							9
	ATTACKATB_MAGIC2						10			// 조선 승병의 공격용 마법 
	ATTACKATB_MAGIC3						11			// 일본 무녀의 공격용 마법 

	ATTACKATB_FIRECAR						13
	ATTACKATB_SHIPCANNON					14
	ATTACKATB_FIRE							15
	ATTACKATB_HEAVYGUN						16
	ATTACKATB_FIREARROW						17			// 불화살 
	ATTACKATB_ARROW							18
	ATTACKATB_SELFEXP						19
	ATTACKATB_LIGHTNING						20			// 번개술
	ATTACKATB_EARTHQUAKE					21			// 지진술
	ATTACKATB_BIGSWORD						22			// 큰검
	ATTACKATB_ELECTRON						23
	ATTACKATB_THROWBOMB						24			// 던지는 폭탄 
	ATTACKATB_MAGICBALL						25			// 심유경 마법 공격 
	
	ATTACKATB_BOMB							27
	ATTACKATB_TORNADO						28			// 풍백술
	ATTACKATB_KIMBOMB           		    29			// 폭뢰격
	ATTACKATB_MAGIC5J           		    30			// 우기다의 만월도끼춤
	ATTACKATB_DART							31			// 닌자의 수리검
	ATTACKATB_MAGIC8K						32			// 김시민의 쐐기 지르기 
	ATTACKATB_CANNONJ						33          // 파쇄차 공격. 
	ATTACKATB_FIRETANK						34
	ATTACKATB_COLLTANK						35
	ATTACKATB_CANNON2J						36			// 발석거 
	ATTACKATB_POISONARROW					37			// 독침 
	ATTACKATB_FIRETANKARROW					38			// 백제 쇠뇌의 공격 
	ATTACKATB_COLLTANKJARROW				39			// 백제 충차의 공격 
	ATTACKATB_GENERAL4KFIRE					40			// 도림의 공격 
	ATTACKATB_GENERAL4J						41			// 고흥의 공격 
	ATTACKATB_GENERAL3C						42			// 원효의 공격 
	ATTACKATB_CANNONKARROW					43			// 마름쇠차 공격 
	ATTACKATB_FLAMETANKJARROW				44
	ATTACKATB_CANNONC						45			// 신라 전호피차 공격 
	ATTACKATB_AXE							46			// 도끼 공격
	ATTACKATB_SWORDENERGY					47			// 견훤의 발도검기 속성
	ATTACKATB_ELEPHANT						48			// 코끼리 (임2)
	ATTACKATB_ANTIARMOUR					49			// 대메카닉 (임2)
	ATTACKATB_CHASEFIRE						50			// 고흥의 화폭술 속성 (천신)
	ATTACKATB_AIRATTACKUPARROW				51			// 비전 연구
	ATTACKATB_TOWERCANNON					52			// 망루용 화포
	ATTACKATB_UPGRADETOWERCANNON			53			// 업그레이드 망루용 화표
	ATTACKATB_GATPHAPOISON					54			// 온라인 - 갓파의 독액 던지기
	ATTACKATB_DUEOKSINI						55          // 두억시니 공격 속성 - 맞으면 뒤로 밀림. 
	ATTACKATB_TOAD							56          // 독두꺼비

	ATTACKATB_FURHEADDEMON                  60			// 털가면. 
	ATTACKATB_SNOWWOMAN		                61			// 설녀. 
	ATTACKATB_GOLDDRAGONELECTRON            62			// 금룡 공격. 
	ATTACKATB_ONGHWA						63			// 온천 원숭이 해골.  
	ATTACKATB_GHOSTCAT						64			// 귀묘 음파 공격. 
	ATTACKATB_DOLL1							65			// 무명 인형 공격. 
	ATTACKATB_DOLL2							66			// 하카다 인형 공격. 
	ATTACKATB_JUSTICEK						67			// 조선 유생 공격
	ATTACKATB_BUTTERFLY						68			// 독나방 공격 
	ATTACKATB_WIDOW							69			// 거미요괴 공격 
	ATTACKATB_WAVE							70			// 바다거북이 파동공격. 
	ATTACKATB_FIREUSINGMANA					71			// 불공격(마법 사용)
	ATTACKATB_LEECH							72			// 왕거머리. 
	ATTACKATB_SEAHORSE						73			// 해마물쏘기. 
	ATTACKATB_MACHINEGUN					74			// 연발 사격 
	ATTACKATB_THROWSPEAR					75			// 던지는창 (원주민)  
	ATTACKATB_SCORPION						76			// 독전갈 독. 
	ATTACKATB_CACTUS						77			// 괴물선인장 가시. 
	ATTACKATB_CLAW							78			// 조도의 공격속성. 
	ATTACKATB_MAGICIAN						79			// 요술사 마법공격.  
	ATTACKATB_BIGTREE						80			// 거대수 나뭇가지 공격. 
	ATTACKATB_COBRA							81			// 코브라 독 공격.
	ATTACKATB_VOLCANOFLAME					82			// 화산의불꽃.   불덩이.
	ATTACKATB_EVIL							83			// 이사의 사념.
	ATTACKATB_ICE							84			// 이사의 사념 특수기술.
	ATTACKATB_MAGIC4T						85			// 징키스칸혼 특수 기술.
	ATTACKATB_JAPANGHOST					86			// 일본귀신 특수 기술.
	ATTACKATB_KUKET							87			// 쿠켓
	ATTACKATB_AUTA							88			// 아우타 공격.
	ATTACKATB_SUB1							89			// 진성여왕 화살공격 1
	ATTACKATB_SUB2							90			// 진성여왕 화살공격 2
	ATTACKATB_FIREATTACK					91			// 화공신포.
	ATTACKATB_RUNNINGFIRE					92			// 연발
	ATTACKATB_ENERGYPA						93			// 장풍.
	ATTACKATB_LONG_SPEAR					94			// 손유창 장창소환.
	ATTACKATB_PAPERDOLL						95			// 음양사 종이인형
	ATTACKATB_NEEDLEATTACK					96			// 침공격
	ATTACKATB_CLOUDWIND						97			// 풍모술
	ATTACKATB_STONEMAGIC					98			// 석괴술
	ATTACKATB_GODSPEAR						99			// 신창노도
	ATTACKATB_FLOWERRAIN					100			// 만천화우
	ATTACKATB_SOULCHANGE					101			// 빙의술
	ATTACKATB_MAGICCATTACK					102			// 도술사 공격
	ATTACKATB_SUNBIYOUN						103			// 순비연 공격
	ATTACKATB_SNOW							104			// 눈덩이

	// actdoll (2004/03/06 5:00) : 3월 패치관련 추가
	ATTACKATB_PYTHON						105			// 이무기 마법공격	// actdoll (2004/03/06 4:08) : 추가
	ATTACKATB_JINEMPEROR					106			// 진시황의혼 마법공격	// actdoll (2004/03/06 4:08) : 추가
	ATTACKATB_GUARDIAN_SNAKE				107			// 수호부(뱀)
	//#define ATTACKATB_RABBIT						105			// 

	// <주의> 변경시 char-attack, calcattack의 RealApower도 수정해야 함, 끊김없이 순차적이어야 함.

	MAX_ATTACKATB_NUMBER					110
};


//------------------------------------------------------------------------------------------------
// 방어 속성		
//------------------------------------------------------------------------------------------------
#define DEFENCEATB_HUMAN    					1			
#define DEFENCEATB_ARMOUR    					2			
#define DEFENCEATB_HORSE    					3			
#define DEFENCEATB_AIRPLANE   					4			
#define DEFENCEATB_OILARMOUR   					5			
#define DEFENCEATB_FLOAT	 					6			
#define DEFENCEATB_ANIMAL    					7			
#define DEFENCEATB_GHOST	   					8			
#define DEFENCEATB_SOFT		   					9			

// <주의> MAX_DEFENCEATB_NUM 정확히 설정, calcattack에 RealApower 설정할 것
#define MAX_DEFENCEATB_NUM						9	
*/

//-----------------------------------------------------------------------------
// 필살기 종류
#define SPECIAL_MAGIC5J				     		1			// 우기다의 만월도끼춤
#define SPECIAL_MAGIC8J							2			// 아사코의 환영술
#define SPECIAL_MAGIC4C							3			// 이순신의 기공신포
#define SPECIAL_MAGIC6C							4			// 송응창의 포박술		
#define SPECIAL_AMBUSH							5			// 매복
#define SPECIAL_DETECTMINE						6			// 의병의 지뢰탐지
#define SPECIAL_EXTINGUISH						7			// 불끄기
#define SPECIAL_KIMBOMB							8			// 폭뢰격 
#define SPECIAL_GROUNDEXP						9			// 연옥술
#define SPECIAL_TORNADO							10			// 풍백술
#define SPECIAL_MAGIC4T							11			// 원심분쇄격(흑룡대차륜)
#define SPECIAL_MAGIC2T							12			// 독무술
#define SPECIAL_EARTHQUAKE						13			// 지진술
#define SPECIAL_LIGHTNING						14			// 번개술
#define SPECIAL_SPECIALATTACK					15			// 일격필살
#define SPECIAL_ABSORBLIFE						16			// 흡수술
#define SPECIAL_FIREARROW						17			// 불화살
#define SPECIAL_FIREARROW2						18			// 마법불화살
#define SPECIAL_MAGIC8K							19			// 쐐기찌르기
#define SPECIAL_HIDE							20			// 은신술
#define SPECIAL_SNOWMAGIC						21			// 설녀 눈폭풍
#define SPECIAL_BIGTREE							22			// 거대수 특수공격
#define SPECIAL_MAGICTEST						23			// 테스트 
#define SPECIAL_MINE							24			// 지뢰
#define SPECIAL_YELLOWSAND						25			// 황사
#define SPECIAL_ICE								26			// 이사의 사념 특수공격
#define SPECIAL_SOUL							27			// 왕거한 야수의혼
#define SPECIAL_GHOST							28			// 페르난데스 귀신탄
#define SPECIAL_DARKFIRE						29			// 임평후 암흑화염폭풍
#define SPECIAL_IVORY							30			// 상아폭격 
#define SPECIAL_BLUEDRAGON						31			// 비격청룡도
#define SPECIAL_RUNNINGFIRE						32			// 연발포탄
#define SPECIAL_ENERGYPA						33			// 장풍
#define SPECIAL_SNOWATTACK						34			// 설공신포
#define SPECIAL_FIREATTACK						35			// 화공신포
#define SPECIAL_FIREENERGYPA					36			// 열화장풍(비격황룡권)
#define SPECIAL_DOUBLE_SWORD					37			// 쌍검난무
#define SPECIAL_LONG_SPEAR						38			// 장창소환
#define SPECIAL_ICE_BARRIER						39			// 빙석술
#define SPECIAL_FLOWER_NEEDLE_RAIN				40			// 만천화우
#define SPECIAL_SOUL_CHANGE						41			// 빙의술
#define SPECIAL_STONE_MAGIC						42			// 석괴술
#define SPECIAL_GOD_SPEAR						43			// 신창노도(일격필살)
#define SPECIAL_CLOUD_WIND						44			// 풍모술
#define SPECIAL_MANABURN						45			// 마나번.

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:25) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define SPECIAL_FIRE_ROUND_DANCE				46			// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
#define SPECIAL_NOVA							47			// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
#define SPECIAL_FURY_OF_VOLCANO					48			// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
#define SPECIAL_POISONING_HEAVENS				49			// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
#define SPECIAL_BOMB_OF_HEAVEN					50			// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
#define SPECIAL_SIEGE_MODE						51			// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
#define SPECIAL_ONSLAUGHT_FIREBALL				52			// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
#define SPECIAL_FIRERAIN_OF_GOD					53			// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

// robypark 2004/6/4 14:7
#define SPECIAL_SIEGE_MODE_UNDO					54			// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령<=순비연 2차 전직 기술)

#define MAX_MAGIC_NUMBER						55

		
//------------------------------------------------------------------------------------------------
#define MENUMODE_MOVE       					1
#define MENUMODE_ATTACK     					2
#define MENUMODE_ATTACKAIR  					4
#define MENUMODE_RIDEIN1    					8
#define MENUMODE_RIDEIN2    					16
#define MENUMODE_RIDEIN3    					32
#define MENUMODE_HELPBUILD  					64
#define MENUMODE_HEAL							128
#define MENUMODE_HARVESTCROP					256
#define MENUMODE_HARVESTTREE					512
#define MENUMODE_GOINTOHQ						1024
#define MENUMODE_EXTINGUISH						2048
#define MENUMODE_MANA							4096


#define WS_DECIDE_LENGTH_TO_DEST		 	1			// 목표 까지의 거리 계산 
#define WS_STRONGFIRE           		 	2
#define WS_FIRE                  			3
#define WS_GOTO_TARGET           			4
#define WS_GOTO_OPPOSITE         			5
#define WS_GOTO_BASE             			6
#define WS_SET_GOTO_BASE         			7
#define WS_HARVEST_CROP          			8
#define WS_DECIDE_LENGTH_TO_BASE 			9
#define WS_PUT_RESOURCE          			10
#define WS_MAKE                  			11
#define WS_INCREASEMAKEPERCENT   			12
#define WS_APPEAR                			13
#define WS_WAIT                  			14
#define WS_CAPTURE               			15
#define WS_DECIDE_LENGTH_TO_SRC  			16
#define WS_GOTO_SRC              			17
#define WS_HEAL                  			18
#define WS_RIDEOUT               			19
#define WS_SELFEXP               			20
#define WS_MINE                  			21
#define WS_INITINGROUND            			22
#define WS_INGROUND	              			23
#define WS_INITOUTGROUND	           		24
#define WS_OUTGROUND	              		25
#define WS_ATTACK							26
			
// 게임 상태 변수 		
		
#define GS_INITSTAGEGAME         		1 
#define GS_GAME                  			3
#define GS_INITSETGAME           			4
#define GS_SETGAME               			5
#define GS_INITSETMULTI          			6 
#define GS_SETMULTI              			7
#define GS_INITINITMENU          			8
#define GS_INITMENU              			9			// 게임 초기 메뉴 
#define GS_INITQUIT              			10
#define GS_QUIT                  			11
#define GS_INITSETSESSION        			12
#define GS_SETSESSION            			13
#define GS_INITCHOOSENATION      			14
#define GS_CHOOSENATION          			15
#define GS_INITPREBRIEFING       			16
#define GS_INITKOREAPREBRIEFING  			17
#define GS_INITJAPANPREBRIEFING  			18
#define GS_INITCHINAPREBRIEFING  			19
#define GS_PREBRIEFING           			20
#define GS_INITGAMEMENU          			22
#define GS_GAMEMENU              			23
#define GS_INITWIN               			24
#define GS_WIN                   			25
#define GS_INITLOSE              			26
#define GS_LOSE                  			27
#define GS_INITRESULT            			28
#define GS_RESULT                			29
#define GS_INITOPENING           			30
#define GS_OPENING               			31
#define GS_INITRESTART           			32			// 게임 다시하기위한 초기화 
#define GS_RESTART               			33 
#define GS_INITLOADGAMEINITMENU  			34
#define GS_LOADGAMEINITMENU      			35
//#define GS_INITHQLOGO            			40
//#define GS_HQLOGO                			50
//#define GS_INITSAMSUNGLOGO       			60
//#define GS_SAMSUNGLOGO           			70
#define GS_INITSTAFF             			80
#define GS_STAFF                 			90
//#define GS_INITENDING           			100
//#define GS_ENDING               			110
#define GS_INITREPLAYINLOSE     			120
#define GS_REPLAYINLOSE         			130
#define GS_INITFADEOUT          			140
#define GS_FADEOUT              			150
#define GS_INITCDMESSAGE        			160
#define GS_CDMESSAGE            			170
#define GS_INITLOGO             			180
#define GS_LOGO                 			190
#define GS_INITBATTLE						195	
#define GS_INITLAUNCHERCDCHECK  			200
#define GS_LAUNCHERCDCHECK      			210
#define GS_INITDPLAUNCHNOTCONNECTED			220
#define GS_DPLAUNCHNOTCONNECTED				230
#define	GS_INITONLINECONNECT				500
#define	GS_ONLINECONNECT					501
#define	GS_INITONLINELOGIN					502
#define	GS_ONLINELOGIN						503
#define GS_INITONLINEWORLD					504
#define GS_ONLINEWORLD						505
#define GS_INITONLINEBATTLEGAME				506
//-------------------------------------------------------------


#define ITEM_STOP        				1 
#define ITEM_MOVE        				2
#define ITEM_ATTACK      				3
#define ITEM_ATTACKGROUND  				4
#define ITEM_HARVEST     				5
#define ITEM_EXTINGUISH					6	// 불끄기
#define ITEM_BUILD       				8
#define ITEM_SELL        				9
#define ITEM_REPAIR      				10
#define ITEM_BUILD1						11
#define ITEM_BUILD2						12 
#define ITEM_INITMENU					13   // 건설 1 이나 건설 2를 취소한다.  
#define ITEM_SELLMENU					14   // 해체 명령 모드를 설정한다.
#define ITEM_INITGMENU					15
#define ITEM_BUILDCANCEL				16   // 건설을 중단한다. 
#define ITEM_TRAINSPOT					17   // 집결지 설정 
#define ITEM_RIDEOUT					18
#define ITEM_CAPTUREBUILD				19
#define ITEM_PATROL						20
#define ITEM_CONVOY						21
#define ITEM_HOLD						22
#define ITEM_BETRAY						23
#define ITEM_TAKEIN						25
#define ITEM_SELFEXP					26
#define ITEM_WARP						27
#define ITEM_AUTOMAGIC					33
#define ITEM_NOAUTOMAGIC				34
#define ITEM_GENERALFIRST				35
#define ITEM_NOGENERALFIRST				36
#define ITEM_MINE						37
#define ITEM_AIRMAGIC					38
#define ITEM_KIMBOMB					39
#define ITEM_MAGIC5J					40
#define ITEM_FARMERTOBULL				41
#define ITEM_SHOWSTORAGE				42
#define ITEM_USEPOTION					43
#define ITEM_AMBUSH						44
#define ITEM_DETECTMINE					45

#define ITEM_MAGIC_EARTHQUAKE			50
#define ITEM_MAGIC_TORNADO				52
#define ITEM_MAGIC_ABSORB				53
#define ITEM_MAGIC_MAGIC8J				54
#define ITEM_MAGIC_LIGHTNING			55
#define ITEM_MAGIC_SHIELD				56
#define ITEM_MAGIC_CLON					57
#define ITEM_MAGIC_STEALTECH			58
#define ITEM_MAGIC_RAINMAKE				59
#define ITEM_MAGIC_USEBUDA				60
#define ITEM_MAGIC_STEALEXP				61
#define ITEM_MAGIC_HEAL					62
#define ITEM_MAGIC_MASSHEAL				63
#define ITEM_MAGIC_GROUNDEXP			64
#define ITEM_MAGIC_MAGIC4C				65
#define ITEM_MAGIC_MAGIC6C				66
#define ITEM_MAGIC_MAGIC4T				67
#define ITEM_MAGIC_MAGIC2T				68
////////////////////////////////////////////
#define ITEM_MAGIC_MAGICTEST			69
////////////////////////////////////////////
#define ITEM_MAGIC_RECALL_PANDA			70
#define ITEM_MAGIC_RECALL_MONKEY		71
#define ITEM_MAGIC_ICE					72
#define	ITEM_MAGIC_SOUL					73
#define ITEM_MAGIC_GHOST				74
#define ITEM_MAGIC_DARKFIRE				75	
#define	ITEM_MAGIC_YELLOWSAND			76
#define ITEM_MAGIC_IVORY				77
#define ITEM_MAGIC_BLUEDRAGON			78
#define ITEM_MAGIC_RUNNINGFIRE			79
#define ITEM_MAGIC_ENERGYPA				80
#define ITEM_MAGIC_SNOWATTACK			81
#define ITEM_MAGIC_FIREATTACK			82
#define ITEM_MAGIC_FIREENERGYPA			83
#define ITEM_MAGIC_MANA					84
#define ITEM_MAGIC_DOUBLE_SWORD			85			// 쌍검난무
#define ITEM_MAGIC_LONG_SPEAR			86			// 장창소환
#define ITEM_MAGIC_ICE_BARRIER			87			// 빙석술
#define ITEM_MAGIC_FLOWER_NEEDLE_RAIN	88			// 만천화우
#define ITEM_MAGIC_SOUL_CHANGE			89			// 빙의술
#define ITEM_MAGIC_STONE_MAGIC			90			// 석괴술
#define ITEM_MAGIC_MANABURN				91

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:19) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
#define ITEM_MAGIC_FIRE_ROUND_DANCE				92	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
#define ITEM_MAGIC_NOVA							93	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
#define ITEM_MAGIC_FURY_OF_VOLCANO				94	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
#define ITEM_MAGIC_POISONING_HEAVENS			95	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
#define ITEM_MAGIC_BOMB_OF_HEAVEN				96	// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
#define ITEM_MAGIC_SIEGE_MODE					97	// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
#define ITEM_MAGIC_ONSLAUGHT_FIREBALL			98	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
#define ITEM_MAGIC_FIRERAIN_OF_GOD				99	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 

// robypark 2004/6/4 14:8
#define ITEM_MAGIC_SIEGE_MODE_UNDO				100	// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령<=순비연 2차 전직 기술)

#define MAX_ITEM_NUMBER					150


// Enum - KindInfo
//#include "..\EnumData\Enum_KI_Character.h"
/*		
// (주의) kind 추가할 때 맨 마지막에 추가할 것, KIND_ON_MAX 최대개수 이상으로 설정.
// actdoll (2004/02/26 10:46) : define으로 정의된 값들을 enum값으로 변경했다.
typedef enum	ENUM_KIND_ON_CHARACTER
{
	KIND_ON_INVALID				= -1,				//			// 잘못된 값

	KIND_PLAYERSTART			= 1,				//1			// 플레이어 시작 위치  

	// 운영자 
	KIND_MANAGER1,									//2			// 운영자 1

	// 주인공
	KIND_ON_KEON,									//3			// 강건
	KIND_ON_MYUNGHWA,								//4			// 이명화
	KIND_ON_RYO,									//5			// 시마다 료
	KIND_ON_HANAHINE,								//6			// 겐노 하나히네
	KIND_ON_SORYUNG,	 							//7			// 왕소룡 
	KIND_ON_MIRYUNG,								//8			// 노수연
	KIND_ON_DANG,									//9			// 당호엽(명)  
	KIND_ON_SUYOUN,									//10		// 노수연(명)

	// 조선 용병 
	KIND_ON_HORSEARCHERK		= 15,				//15		// 기마궁사
	KIND_ON_JUSTICEK,								//16		// 유생 
	KIND_ON_BUDAK,									//17		// 조선 파계승
	KIND_ON_SWORDK,									//18		// 창잡이 
	KIND_ON_ARCHERK,								//19		// 궁수
	KIND_ON_BUTCHER,								//20		// 칼잡이
	KIND_ON_PORTERK,								//21		// 조선짐꾼

	KIND_ON_HORSEARCHERUPK,							//22		// 기마궁사(Upgrade)
	KIND_ON_JUSTICEUPK,								//23		// 유생(Upgrade)
	KIND_ON_BUDAUPK,								//24		// 조선 파계승(Upgrade)
	KIND_ON_SWORDUPK,								//25		// 창잡이 (Upgrade)
	KIND_ON_ARCHERUPK,								//26		// 궁수(Upgrade)

	KIND_GENERAL1K,									//27		// 조선 권율
	KIND_GENERAL2K,									//28		// 조선 이순신
	KIND_GENERAL3K,									//29		// 조선 유성룡
	KIND_GENERAL4K,									//30		// 조선 사명대사
	KIND_GENERAL5K,									//31		// 조선 곽재우
	KIND_GENERAL6K,									//32		// 조선 허준
	KIND_GENERAL7K,									//33		// 조선 김덕령
	KIND_GENERAL8K,									//34		// 조선 김시민
	KIND_CANNON2K,									//35		// 조선 대장군포

	// 일본 용병 
	KIND_ON_SWORDJ				= 40,				//40		// 낭인
	KIND_ON_GUNJ,									//41		// 사수
	KIND_ON_NINJAJ,									//42		// 일본 닌자
	KIND_ON_HORSESWORDJ,							//43		// 늑대낭인
	KIND_ON_ADVBUDAJ,								//44		// 일본 퇴마사 
	KIND_ON_DSWORD,									//45		// 기마무사
	KIND_ON_PORTERJ,								//46		// 일본짐꾼

	KIND_ON_SWORDUPJ,								//47		// 검술낭인(Upgrade)
	KIND_ON_GUNUPJ,									//48		// 사수(Upgrade)
	KIND_ON_NINJAUPJ,								//49		// 닌자(Upgrade)
	KIND_ON_HORSESWORDUPJ,							//50		// 늑대낭인 (upgrade)
	KIND_ON_ADVBUDAUPJ,								//51		// 일본 퇴마사 (Upgrade)

	KIND_GENERAL1J,									//52		// 일본 고니시
	KIND_GENERAL2J,									//53		// 일본 가토오
	KIND_GENERAL3J,									//54		// 일본 와키자카
	KIND_GENERAL4J,									//55		// 일본 세이쇼오
	KIND_GENERAL5J,									//56		// 일본 우기다
	KIND_GENERAL6J,									//57		// 일본 도쿠가와
	KIND_GENERAL7J,									//58		// 일본 미츠나리
	KIND_GENERAL8J,									//59		// 일본 아사코

	// 대만 용병 
	KIND_ON_BUDAT				= 63,				//63		// 대만주술사
	KIND_ON_AMAZONT,								//64		// 대만 여전사
	KIND_ON_AXET,									//65		// 대만 도끼거한 
	KIND_ON_SWORDT,									//66		// 대만 봉술가
	KIND_ON_GUNT,									//67		// 대만 서양총수
	KIND_ON_PORTERT,								//68		// 대만짐꾼
	KIND_ON_BEASTWARRIOR,							//69		// 야수전사

	KIND_ON_BUDAUPT,								//70		// 고급주술사 
	KIND_ON_AMAZONUPT, 								//71		// 고급여전사 
	KIND_ON_AXEUPT,									//72		// 고급도끼거한 
	KIND_ON_SWORDUPT,								//73		// 고급봉술가 
	KIND_ON_GUNUPT,									//74		// 고급서양총수 

	KIND_ON_GENERAL1T,								//75		// 왕거한 
	KIND_ON_GENERAL2T,								//76		// 페르난데스 
	KIND_ON_GENERAL3T,								//77		// 임평후
	KIND_ON_GENERAL4T,								//78		// 유영복 
	KIND_ON_GENERAL5T,								//79		// 조세림 
	KIND_ON_GENERAL6T,								//80		// 손유창

	// 중국 용병
	KIND_ON_FAN_C				= 85,				//85		// 모험가
	KIND_ON_BUDAK_C,								//86		// 수도승
	KIND_ON_MUDOGA_C,								//87		// 무도가
	KIND_ON_SWORD_C,								//88		// 큰칼무사
	KIND_ON_FIREGUN_C,								//89		// 화포수
	KIND_ON_MAGIC_C,								//90		// 도술사
	KIND_ON_PORTER_C,								//91		// 중국짐꾼

	KIND_ON_FANUP_C,								//92		// 모험가
	KIND_ON_BUDAKUP_C,								//93		// 수도승
	KIND_ON_MUDOGAUP_C,								//94		// 무도가
	KIND_ON_SWORDUP_C,								//95		// 큰칼무사
	KIND_ON_FIREGUNUP_C,							//96		// 화포수

	KIND_ON_GENERAL1_C,								//97		// 동방불패
	KIND_ON_GENERAL2_C,								//98		// 이령
	KIND_ON_GENERAL3_C,								//99		// 제갈공명
	KIND_ON_GENERAL4_C,								//100		// 조승훈
	KIND_ON_GENERAL5_C,								//101		// 축융	
	KIND_ON_GENERAL6_C,								//102		// 순비연

	// 몬스터 
	KIND_ON_HARUBANG			= 107,				//107		// 돌하루방. 
	KIND_ON_CRAB,									//108	    // 바다게. 
	KIND_ON_LEECH,									//109		// 거머리 
	KIND_ON_SEAHORSE,								//110		// 해마 
	KIND_WHITETIGER,								//111		// 백호 
	KIND_GHOSTCAT,									//112		// 귀묘 
	KIND_TUTTLE,									//113		// 바다거북이
	KIND_BUTTERFLY,									//114		// 독나방
	KIND_WIDOW,										//115		// 거미요괴
	KIND_INSECT,									//116		// 사마귀
	KIND_WORRIORGHOST,								//117		// 무관의혼
	KIND_SINGERMGHOST,								//118		// 신검의혼
	KIND_MAMMOTH,									//119		// 눈코끼리
	KIND_WILDBOAR,									//120		// 멧돼지
	KIND_ON_THIEF,									//121		// 도굴꾼 
	KIND_ON_SEAKING,								//122		// 해저왕 
	KIND_ON_SWORDPIRATE,							//123		// 온라인 - 왜구
	KIND_ON_GUNPIRATE,								//124		// 온라인 - 조총왜구
	KIND_ON_SNOWWOMAN,								//125		// 온라인 - 설녀
	KIND_ON_EATERDEMON,								//126     // 온라인 - 식귀
	KIND_ON_WEIRDPLANT,								//127     // 온라인 - 요수목
	KIND_ON_HUNGRYDEMON,							//128     // 온라인 - 아귀
	KIND_ON_TIGER,									//129     // 온라인 - 호랑이
	KIND_ON_DOGDEMON,								//130     // 온라인 - 견신
	KIND_ON_ONGHWA,									//131     // 온라인 - 옹화
	KIND_ON_BANDIT1,								//132     // 온라인 - 산적1(소)
	KIND_ON_BANDIT2,								//133     // 온라인 - 산적2(중)
	KIND_ON_BANDIT3,								//134     // 온라인 - 산적3(대, 쇠뭉치)
	KIND_ON_BANDIT4,								//135		// 온라인 - 산적3(대, 활)
	KIND_ON_GACKDAGUI,								//136		// 온라인 - 각다귀
	KIND_ON_GATPHA,									//137		// 온라인 - 갓파
	KIND_ON_BOGY,									//138		// 온라인 - 도깨비
	KIND_ON_FOX,									//139		// 온라인 - 여우
	KIND_ON_STARFISH,								//140		// 온라인 - 불가사리
	KIND_ON_SHIP1,									//141		// 온라인 - 배1
	KIND_ON_ONI,									//142     // 온라인 - 오니
	KIND_ON_SNOWDEMON,								//143     // 온라인 - 설인
	KIND_ON_FURHEADDEMON,							//144     // 온라인 - 털가면
	KIND_ON_FIREDRAGON,								//145     // 온라인 - 화룡
	KIND_ON_GOLDDRAGON,								//146		// 온라인 - 금용
	KIND_ON_BAT,									//147		// 온라인 - 박쥐
	KIND_ON_TOAD,									//148		// 온라인 - 독두꺼비
	KIND_ON_CENTIPEDE,								//149		// 온라인 - 독지네
	KIND_ON_DUEOKSINI,								//150		// 온라인 - 두억시니
	KIND_ON_GANGSI,									//151		// 온라인 - 강시
	KIND_ON_RACCOONDOG,								//152		// 온라인 - 너구리
	KIND_ON_DEER,									//153		// 온라인 - 사슴
	KIND_ON_BEAR,									//154		// 반달곰 
	KIND_ON_HORSESPEAR,								//155		// 기마창수 
	KIND_ON_CAT,									//156		// 대만고양이
	KIND_ON_DUCK,									//157		// 대만오리
	KIND_ON_HORNET,									//158		// 대만왕벌
	KIND_ON_MBANDIT1,								//159		// 마적두목 
	KIND_ON_MBANDIT2,								//160		// 망치마적 
	KIND_ON_MBANDIT3,								//161		// 총수마적 
	KIND_ON_CHAWOO,									//162		// 차우차우 
	KIND_ON_PIRATES,								//163		// 해적 
	KIND_ON_PANDER,									//164		// 팬더 
	KIND_ON_SNAKE,									//165		// 구렁이 
	KIND_ON_CROCODILE,								//166		// 식인악어 
	KIND_ON_HOOK,									//167		// 갈고리해적 
	KIND_ON_SMALLGANG,								//168		// 졸개마적
	KIND_ON_HORSEMAN,								//169		// 마두인
	KIND_ON_BABARIAN,								//170		// 원주민무당 
	KIND_ON_BABARIANSHAMAN,							//171		// 원주민 
	KIND_ON_BADDEER,								//172		// 타락루돌프 
	KIND_ON_CACTUS,									//173		// 괴물선인장
	KIND_ON_SCORPION,								//174		// 독전갈 
	KIND_ON_SKYNINE,								//175		// 천구
	KIND_ON_ICETREE,								//176		// 얼음나무
	KIND_ON_SNOWLIZARD,								//177		// 눈도마뱀
	KIND_ON_MADBULL,								//178		// 광우
	KIND_ON_BIGTREE,								//179		// 거대수
	KIND_ON_MAGICIAN,								//180		// 요술사 
	KIND_ON_BADMERCHANT,							//181		// 악덕상인 
	KIND_ON_RUNNERHAND,								//182		// 괴물덩굴손.
	KIND_ON_SANDGOD,								//183		// 모래의신.
	KIND_ON_SEAENVOY,								//184		// 바다의사신.
	KIND_ON_VIPER,									//185		// 살무사.
	KIND_ON_AMAKUSA,								//186		// 아마쿠사.
	KIND_ON_LAVAMONSTER,							//187		// 용암괴물
	KIND_ON_EVIL,									//188		// 이사의 사념.
	KIND_ON_BIRD,									//189		// 인면조
	KIND_ON_WEASEL,									//190		// 족제비
	KIND_ON_COBRA,									//191		// 코브라.
	KIND_ON_SOUL,									//192		// 징기스칸의 혼.
	KIND_ON_VOLCANOFLAME,							//193		// 화산의 불꽃
	KIND_ON_REDMADBULL,								//194		// 붉은광우.
	KIND_ON_MAGICOFFICER,							//195		// 요술사령.
	KIND_ON_CAPBEAR,								//196		// 대장반달곰.
	KIND_ON_KOREANGHOST,							//197		// 처녀귀신.
	KIND_ON_SKELETON,								//198		// 해골.
	KIND_ON_SKELETONSOLDIER,						//199		// 해골병사.
	KIND_ON_PIRATECAPTAIN,							//200		// 해적선장
	KIND_ON_TAIWANGHOST,							//201		// 대만귀신.
	KIND_ON_JAPANGHOST,								//202		// 일본귀신.
	KIND_ON_AUTA,									//203		// 아우타.
	KIND_ON_JUCK,									//204     // 적고적.
	KIND_ON_FEMALESHAMAN,							//205		// 조선무녀.
	KIND_ON_QUEEN,									//206		// 진성여왕.
	KIND_ON_DONKEY,									//207		// 당나귀.
	KIND_ON_AGILETIGER,								//208		// 비호.
	KIND_ON_PAWNTIGER,								//209		// 졸개호랑이.
	KIND_ON_OWL,									//210		// 올빼미.
	KIND_ON_LEOPARD,								//211		// 표범.
	KIND_ON_HUNSWARRIOR,							//212		// 흉노전사
	KIND_ON_HUNSHORSE,								//213		// 흉노기마병
	KIND_ON_HUNSHORSEARCHER,						//214		// 흉노기마궁수
	KIND_ON_GATEKEEPER,								//215		// 문지기
	KIND_ON_CLAYFIGURE,								//216		// 기마병마용
	KIND_ON_PHOENIX,								//217		// 불사조
	KIND_ON_JINEMPEROR,								//218		// 진시황의혼
	KIND_ON_MEDICALMAN,								//219		// 의술사.
	KIND_ON_HEOJUN,									//220		// 허준.
	KIND_ON_COSMIC,									//221		// 음양사.
	KIND_ON_SEIMEI,									//222		// 세이메이.
	KIND_ON_YUMRUCK,								//223		// 염력사.
	KIND_ON_JANGSUNHWA,								//224		// 장선화.
	KIND_ON_LONGSPEAR,								//225		// 장창무사.
	KIND_ON_KOAKHU,									//226		// 곽후.	
	KIND_ON_SNOWMAN1,								//227		// 눈사람1.
	KIND_ON_SNOWMAN2,								//228		// 눈사람2.
	KIND_ON_SANTA1,									//229		// 가짜산타1
	KIND_ON_SANTA2,									//230		// 가짜산타2
	KIND_ON_SANTA3,									//231		// 가짜산타3
	KIND_ON_MOTHER_DEER1,							//232		// 엄마루돌이1
	KIND_ON_MOTHER_DEER2,							//233		// 엄마루돌이2
	KIND_ON_MOTHER_DEER3,							//234		// 엄마루돌이3
	KIND_ON_MOTHER_DEER4,							//235		// 엄마루돌이4
	KIND_ON_SANTA4,									//236		// 가짜산타 4
	KIND_ON_SANTA_TW,								//237		// 대만 가짜 산타.
	KIND_ON_MOTHER_DEER_TW,							//238		// 대만 가짜 루돌이.
	KIND_ON_SHIP2,									//239		// 온라인 - 배2		actdoll (031230)
	KIND_ON_SHIP3,									//240		// 온라인 - 배3		actdoll (031230)
	KIND_ON_SHIP4,									//241		// 온라인 - 배4		actdoll (031230)
	KIND_ON_SHIP5,									//242		// 온라인 - 배5		actdoll (031230)
	KIND_ON_SHIP6,									//243		// 온라인 - 배6		actdoll (031230)
	KIND_ON_SHIP7,									//244		// 온라인 - 배7		actdoll (031230)
	KIND_ON_SHIP8,									//245		// 온라인 - 배8		actdoll (031230)
	KIND_ON_FLYDRAGON,								//246		// 익룡
	KIND_ON_RAPTOR,									//247		// 랩터
	KIND_ON_TIRANO,									//248		// 티라노
	KIND_ON_MONKEY,									//249		// 소환원숭이.

	// actdoll (2004/02/26 11:12) : 추가된 몬스터 - 유명계 맵용
	KIND_ON_HELLKING,								//250		// 염라대왕
	KIND_ON_HELLMESSENGER,							//251		// 저승사자
	KIND_ON_GHOSTSAMURAI,							//252		// 유령 사무라이
	KIND_ON_PYTHON,									//253		// 이무기
	KIND_ON_GHOSTELEPHANT,							//254		// 유령 코끼리
	KING_ON_GHOSTSEAKING,							//255		// 유령 해저왕
	KIND_ON_GHOSTPIRATECAPTAIN,						//256		// 유령 해적선장
	KIND_ON_GHOSTSKELETON,							//257		// 유령 해골
	KIND_ON_GHOSTSKELETONSOLDIER,					//258		// 유령 해골병사
	KIND_ON_GHOSTFEMALESHAMAN,						//269		// 유령 무녀
	KIND_ON_GHOSTVOLCANOFLAME,						//260		// 유령 불꽃

	KIND_ON_GUARDIAN_MOUSE,							//261		// 수호부(쥐)
	KIND_ON_GUARDIAN_CAW1,							//262		// 수호부(소1)
	KIND_ON_GUARDIAN_CAW2,							//263		// 수호부(소2)
	KIND_ON_GUARDIAN_CAW3,							//264		// 수호부(소3)

	KIND_ON_GUARDIAN_CHICK,							//265		// 병아리.
	KIND_ON_GUARDIAN_PIG,							//266		// 돼지.
	KIND_ON_GUARDIAN_SNAKE,							//267		// 뱀.
	KIND_ON_GUARDIAN_MONKEY,						//268		// 원숭이.


	KIND_ON_MAX										//			// 총 캐릭터의 종류 갯수(실제 갯수는 아니다)
};
*/
// 게임에 사용되는 버튼의 유형. 
#define BUTTON_TYPE_GENERAL_MOVETO  		6	
#define BUTTON_TYPE_CREW			  		7	
#define BUTTON_TYPE_GAMEMENU				10
#define BUTTON_TYPE_ON_REGISTER				11
#define BUTTON_TYPE_ON_ALLY					13
#define BUTTON_TYPE_ON_ALLY_NUM				14
#define BUTTON_TYPE_ON_ALLY_WINDOW			15

//---------------------------------------------------------------------
//  에러 관련 
//--------------------------------------------------------------------- 
#define ERROR_MAGIC8J_NOAPOWER				1
#define ERROR_BETRAY_HEALTH					2


// 곡물 종류 						
						
// 쌀						
#define CROP_TYPE_RICE     				1
#define CROP_RICE0         				0
#define CROP_RICE1         				1
#define CROP_RICE2         				2
#define CROP_RICE3         				3

// 감자 
#define CROP_TYPE_POTATO   				2
#define CROP_POTATO0       				0
#define CROP_POTATO1       				1
#define CROP_POTATO2       				2
						
// 나무 						
#define CROP_TYPE_TREE     				3
#define CROP_TREE0         				0
#define CROP_TREE1         				1
#define CROP_TREE2         				2
#define CROP_TREE3         				3
#define CROP_TREE4         				4
#define CROP_TREE5         				5
#define CROP_TREE6         				6
#define CROP_TREE7         				7
#define CROP_TREE8         				8
#define CROP_TREE9         				9
#define CROP_TREE10         			10
#define CROP_TREE11         			11
#define CROP_TREE12         			12

// 폐허 지역 
#define CROP_TYPE_RUIN0							4      // 건물 잔해 
#define CROP_TYPE_RUIN1							5      // 건물 잔해 
#define CROP_TYPE_RUIN2    						6      // 메카닉 파괴잔해 
#define CROP_TYPE_DOLL1    						7      // 장승 
#define CROP_TYPE_TOMB1    						8      // 무덤 
#define CROP_TYPE_BROKENTREE1 					9		// 부러진 거목
#define CROP_TYPE_ODDTREE1  					10		// 성황당 
#define CROP_TYPE_WELL1   						11      // 우물 
#define CROP_TYPE_FLAGK1  						12      //조선 깃발 
#define CROP_TYPE_FLAGC1  						13      // 명나라 깃발
#define CROP_TYPE_FLAGJ1  						14      // 일본 깃발
#define CROP_TYPE_TEMPLE  						16      // 화계사
#define CROP_TYPE_VOLCANO 						20		// 분화구 
#define CROP_TYPE_SIGN1   						30		// 표지판 
#define CROP_TYPE_STONE1  						50      // 거석 
#define CROP_TYPE_STONE2  						51      // 거석2
#define CROP_TYPE_TUTTLE1 						61      // 거북 비석
#define CROP_TYPE_AD11							70      // 삼성 이정표 
#define CROP_TYPE_AD12							80      // 삼성 바닥
#define CROP_TYPE_AD13							90      // 삼성 전화기 

#define CROP_TYPE_CASTLETOWER					200		// KBS 역사 스페셜용 - 성곽 망루
#define CROP_TYPE_DESTROYEDCASTLE				201		// KBS 역사 스페셜용 - 무너진 성곽
#define CROP_TYPE_CASTLEGATE					202		// KBS 역사 스페셜용 - 성문
#define CROP_TYPE_CASTLETILE					203		// KBS 역사 스페셜용 - 성곽타일
#define CROP_TYPE_CASTLELARGEGATE				204		// KBS 역사 스페셜용 - 큰 성문


#define CROP_TYPE_RUIN0_XSIZE  3
#define CROP_TYPE_RUIN0_YSIZE  3
#define CROP_TYPE_RUIN1_XSIZE  2
#define CROP_TYPE_RUIN1_YSIZE  2
#define CROP_TYPE_RUIN2_XSIZE  2
#define CROP_TYPE_RUIN2_YSIZE  2
#define CROP_TYPE_DOLL1_XSIZE  1
#define CROP_TYPE_DOLL1_YSIZE  1
#define CROP_TYPE_TOMB1_XSIZE  2
#define CROP_TYPE_TOMB1_YSIZE  2
#define CROP_TYPE_BROKENTREE1_XSIZE 2
#define CROP_TYPE_BROKENTREE1_YSIZE 2
#define CROP_TYPE_ODDTREE1_XSIZE 2
#define CROP_TYPE_ODDTREE1_YSIZE 				2
#define CROP_TYPE_WELL1_XSIZE    				1
#define CROP_TYPE_WELL1_YSIZE    				1
#define CROP_TYPE_TUTTLE1_XSIZE  				2
#define CROP_TYPE_TUTTLE1_YSIZE  				2
#define CROP_TYPE_FLAGK1_XSIZE   				1
#define CROP_TYPE_FLAGK1_YSIZE   				1
#define CROP_TYPE_FLAGC1_XSIZE   				1
#define CROP_TYPE_FLAGC1_YSIZE   				1
#define CROP_TYPE_FLAGJ1_XSIZE   				1
#define CROP_TYPE_FLAGJ1_YSIZE   				1
#define CROP_TYPE_TEMPLE_XSIZE   				3
#define CROP_TYPE_TEMPLE_YSIZE   				3
#define CROP_TYPE_SIGN1_XSIZE    				1
#define CROP_TYPE_SIGN1_YSIZE    				1
#define CROP_TYPE_STONE1_XSIZE   				1
#define CROP_TYPE_STONE1_YSIZE   				1
#define CROP_TYPE_STONE2_XSIZE   				3
#define CROP_TYPE_STONE2_YSIZE   				3
#define CROP_TYPE_AD11_XSIZE     				3
#define CROP_TYPE_AD11_YSIZE					2
#define CROP_TYPE_AD12_XSIZE     				3
#define CROP_TYPE_AD12_YSIZE     				2
#define CROP_TYPE_AD13_XSIZE     				2
#define CROP_TYPE_AD13_YSIZE     				2
#define CROP_TYPE_CASTLETOWER_XSIZE				4
#define CROP_TYPE_CASTLETOWER_YSIZE				4
#define CROP_TYPE_DESTROYEDCASTLE_XSIZE			4
#define CROP_TYPE_DESTROYEDCASTLE_YSIZE			4
#define CROP_TYPE_CASTLEGATE_XSIZE				5
#define CROP_TYPE_CASTLEGATE_YSIZE				5
#define CROP_TYPE_CASTLETILE_XSIZE				2
#define CROP_TYPE_CASTLETILE_YSIZE				2
#define CROP_TYPE_CASTLELARGEGATE_XSIZE			9
#define CROP_TYPE_CASTLELARGEGATE_YSIZE			9


// 대나무 
#define CROP_TYPE_BAMBOO    		   100
#define CROP_BAMBOO0         			 0
#define CROP_BAMBOO1         			 1
#define CROP_BAMBOO2         			 2
#define CROP_BAMBOO3         			 3
#define CROP_BAMBOO4         			 4

#define MAX_TREE_MOUNT     170
#define MAX_BAMBOO_MOUNT    40
#define MAX_RICE_MOUNT      70
#define MAX_POTATO_MOUNT    25

#define	WM_USER_SYSRETURN	(WM_USER + 1)
#define	WM_USER_RETURN		(WM_APP + 2)

class _NewID {
public:
	SHORT ID;
	unsigned short Unique;

	void Init(){ID=0; Unique=0;}
	BOOL IsEmpty(){ if(ID==0 && Unique==0)return TRUE; return FALSE;}
	BOOL IsSame(_NewID id){ if(id.ID==ID && id.Unique==Unique)return TRUE; return FALSE;}


};

extern char			g_AutoID[256];
extern char			g_AutoIDPW[256];
extern SI16			g_AutoServerCode;

extern BOOL			g_fTestVersion;		// 하룡 테섭용인가?
extern BOOL			g_bSkipIntro;		// 인트로를 스킵할 것인가?
extern char			REG_PATH[256];

// 디버그용 경로
extern	char		DF_PATH_DEBUG_OUTSIDE_HQTEXT[MAX_PATH];			// (디버그용) 외부 HQ Text 참조경로

void				ShowErrorMsgBox( int iErrorCode, ... );


#endif//__MAIN_H__
