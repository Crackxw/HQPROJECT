//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ���ι�
//	File Name		: Main.cpp
//	Birth Date		: 2004. 01. 01.
//	Creator			: �� �� ��
//	Editer			: ���°�, ������, ������, �����, ���ؼ�, ������, ȫ����, ������
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		�Ż��� ���ι��Դϴ�.
//
//===================================================================================================

#ifndef __MAIN_H__
#define __MAIN_H__

//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: ���� �� ����
//-----------------------------------------------------------------------------
#pragma warning ( disable : 4786 )

//--------------------------------------------------------------------------
// �� ������ ��� ���� �ڵ�(���� ������ ���) - ���� ���۵� ������� �ۼ�
//--------------------------------------------------------------------------
typedef enum
{
	ELANGAGECODE_NONE		= 0		,// ������� ����
		
	ELANGAGECODE_KOREAN				,// �ѱ�
	ELANGAGECODE_TAIWAN				,// �븸
	ELANGAGECODE_JAPAN				,// �Ϻ�
	ELANGAGECODE_HONGKONG			,// ȫ��
	ELANGAGECODE_CHINA				,// �߱�
	ELANGAGECODE_INDONESIA			,// �ε��׽þ�

	ELANGAGECODE_MAX
} ELanguageCode;

//--------------------------------------------------------------------------
// �� ������ �ν��ڵ� - �� ������ ��ȭ��ȣ �ڵ� - ���� ���۵� ������� �ۼ�
//--------------------------------------------------------------------------
extern	const	int		DF_CODE_NATION_IDENTITY_KOREA;		// �ѱ�
extern	const	int		DF_CODE_NATION_IDENTITY_TAIWAN;		// �븸
extern	const	int		DF_CODE_NATION_IDENTITY_JAPAN;		// �Ϻ�
extern	const	int		DF_CODE_NATION_IDENTITY_HONGKONG;	// ȫ��
extern	const	int		DF_CODE_NATION_IDENTITY_CHINA;		// �߱�
extern	const	int		DF_CODE_NATION_IDENTITY_INDONESIA;	// �ε��׽þ�

//--------------------------------------------------------------------------
// �� �� �ڵ�������
//--------------------------------------------------------------------------
extern	const	int		DF_CODE_CODEPAGE_ENGLISH;			// ����(�⺻)
extern	const	int		DF_CODE_CODEPAGE_KOREAN;			// �ѱ��� 
extern	const	int		DF_CODE_CODEPAGE_JAPANESE;			// �Ϻ���
extern	const	int		DF_CODE_CODEPAGE_CHINESE_SIMP;		// �߱���(�߱�, �̰�����)
extern	const	int		DF_CODE_CODEPAGE_CHINESE_TRAD;		// �߱���(�븸, ȫ��)

//--------------------------------------------------------------------------
// �� ������ ��� ���� �ڵ�(���� ������ ���) - ���� ���۵� ������� �ۼ�
//--------------------------------------------------------------------------
extern	char*	const	DF_TEXT_SERVICE_NATION_LIST[ELANGAGECODE_MAX];


// ����ȭ�� ���̼��� �ؽ�Ʈ
extern	const	char*	DF_TEXT_COPYRIGHT_WITH_GAMANIA;			// ���̿� �����Ͼ� ī�Ƕ���Ʈ (�ѱ�/�븸/�Ϻ�/ȫ��/�߱�)
extern	const	char*	DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;		// ���̿� ������ ī�Ƕ���Ʈ (�ε��׽þ�)

// ������2+ ������ �ݰ�
#define _IMJIN2PLUS	      					1	

#define _ONLINE							1	// �Ż��� �����ϴ°�


#define CLASSNAME "GersangClient"

#ifdef _ENGLISH   // ���� �����̸�, 
#define NAME  "Gersang"
#define TITLE "Gersang"
#else             // �ѱ� �����̸�,  
#define NAME  "�Ż�"
#define TITLE "�Ż�"
#define _COMPANYLOGO				1   // ����ȸ�� �ΰ� ������ ����?
#endif



#define _MODE800							1

// ���� �����̼�
#define REG_MAGICSTATION						1


#define _RANDOMSEED								1	// ������ �����ϰ� �����ȴ�. �ּ� Ǯ��

//#define _SHOWMAPCOORDINATE					1	// ������ ��ǥ�� ǥ���Ѵ�.   
//#define _MYDEBUG

//#define _DEMO									1
#ifdef _DEMO
	//#define TYPE_YONSEI						1	// ���� �Ҹ���� ������ ������ ���α׷�(�� ���� �ּ� Ǭ��)
	/* <���� �Ҹ���� ������ ������ ���α׷�>
  	1. ����ȭ�鱳ü
	2. ȥ���ϱ�� ������ �ϱ⸸ �ȴ�. 
	3. hqnet�� �������� �ʴ´�. 
	4. ���� ������ �̸��� �ٲ���´�(���׸��� �ٲ۴�. ���� ������ ������ ���̰� �Ѵ�).
	5. �õ�� �ν����� �ʰ�, �ϵ忡 copy�ϴ� �͸����� ������ �� �־�� �Ѵ�.
	6. ��� ���� �� �����ؾ� �Ѵ�
	*/
#endif

//#define _QUICKMODE							1   // ���� ���� ����...

//-----------------------------------------------------------------------------
// Name: Main
// Code: All (2004-01-01)
// Desc: �ʿ��� ��� ȭ��(������ ����)
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


#define WINTER_TEXT_COLOR RGB(250, 250, 250)						// �ܿ�� ���ڻ� 
#define NORMAL_TEXT_COLOR RGB(250, 250, 250)//RGB(180, 180, 180)	// �ܿ���� ���ڻ� 
#define COLOR_MENU_TEXT   (RGB( 163, 145, 122 ))

#define RED_TEXT_COLOR   (RGB( 250, 0, 0 ))							// ������ �ؽ�Ʈ 

#define PLAYER0_TEAM_COLOR  RGB(11, 11, 254)
#define PLAYER1_TEAM_COLOR  RGB(160, 0, 0)
#define PLAYER2_TEAM_COLOR  RGB(135, 0, 135)
#define PLAYER3_TEAM_COLOR  RGB(0, 135, 135)
#define PLAYER4_TEAM_COLOR  RGB(161, 161, 0)
#define PLAYER5_TEAM_COLOR  RGB(0, 135, 0)
#define PLAYER6_TEAM_COLOR  RGB(161, 104, 0)
#define PLAYER7_TEAM_COLOR  RGB(161, 161, 161)

// �浹 �Ӽ� 
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

// 256���� ���۵ȴ�.
#define BLOCK_AIRCHAR							256			// ���߿� ĳ���� 
#define BLOCK_AIRMOVE							512
	
// 0x1000���� ���۵ȴ�. 
#define BLOCK_CHAR								0x1000      // ĳ������ �Ӽ� 
#define BLOCK_MOVE								0x2000      // �̵��� ĳ������ �Ӽ�

///////////////////////////////////////////////////////////////

// ���ٴڿ� �ٴ� ���� ������ ���� 
enum FireLifeTime {GROUNDFIRE_LIFETIME_LATE = 2, GROUNDFIRE_LIFETIME_EARLY = 4, GROUNDFIRE_LIFETIME = 6};

#define NOOBJECT								0
#define OBJECT1									1
	
#define FILE_NAME_SIZE							256
#define USER_FILE_NAME_SIZE						60           // ���ǰ��ӿ��� ����� ���� �̸��� ����

#define MAX_MERCHANT_SOLDIER_NUMBER				12			// �뺴�� ������ �÷��̾� 1�δ� ĳ���� ��.  
	
#define MAX_CHAR_NUMBER							200			// ���ӿ� ������ �� �ִ� �ִ��� ĳ���� ���� 
															 // �ǹ��� ĳ���� ������.
#define MAX_SELECTED_CHAR_NUMBER				12			// �ѹ��� ���õ� �� �ִ� �ִ� ĳ���� ���� 
#define MAX_MULTI_ORDER_NUMBER					10			// �ӽ÷� ������ �� �ִ� �ִ� ��Ƽ ����� ���� 


#define MAX_MENU_NUMBER						12   // ĳ���Ͱ� ���� �� �ִ� �ִ� ����� ���� 

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
#define MAX_GENERAL_NUMBER_PER_COUNTRY		8		// ������ �ִ� ����� �� 

#define MAX_DISCOVER_DELAY						100
#define MAX_SHIELD_DELAY						350		// ���� ������ 
#define MIN_SHIELD_DELAY						100		// �뺴�� ����.
#define MAX_CHAT_MESSAGE_SIZE					50		// ä�� �޽����� �ִ� ������ 
#define MAX_GAME_BUTTON							30		// �ѹ��� ���� �� �ִ� �ִ� ���� ��ư�� ��. 

#define PATRIOT_BAR_WIDTH						510
#define PATRIOT_BAR_HEIGHT						12

#define MAX_BRIEFING_TEXT_XSIZE			357  // �긮�ο��� �ؽ�Ʈ�� �̸� ���� ������ ���� ������ 
#define MAX_BRIEFING_TEXT_YSIZE			248  // �긮�ο��� �ؽ�Ʈ�� �̸� ���� ������ ���� ������

#define NEW_SELECTED_DELAY						6
#define SMALL_BLACK_CHECK_DELAY					6

#define MAX_MULTI_PLAYER						8   // �ִ� ����� ���� (����)
#define MAX_BASE_NUMBER							8	//  ���� �� �ִ� �ִ� ������ ���� 
#define MAX_CASLTE_SPR_NUMBER					5   // ���� ���� ������Ʈ�� ���� 

#define MAX_WAIT_STEP_NUMBER					16	// ĳ���Ͱ� ���� �� �ִ� �ִ� ���� ��� �������� �� 
#define MAX_WAIT_TYPE							2   // ���� ���ϴ� �ִϸ��̼��� �ִ� �� .

#define MAX_MOVE_STEP_NUMBER					10	// ĳ���Ͱ� ���� �� �ִ� �ִ� �̵� ��� �������� �� 

#define GAMECOLOR_RED						69
#define GAMECOLOR_YELLOW					70
#define GAMECOLOR_GREEN						68

#define MAX_ANIMATION_PER_UNIT					15		// ���ִ� ���� �� �ִ� �ִϸ��̼��� �� ����. 

#define DOUBLE_CLICK_DELAY						300
 
#define MIN_VALID_FRAME_NUMBER					1500	// ��Ƽ���ӿ��� ����� �й谡 ��ȿ�ϱ� ������ ������....
#define MULTI_GAME_DIVIDER_FACTOR				160		// ��Ƽ ���ӿ��� ������ �� ���ڷ� ������. 
#define MAX_SCORE_PER_GAME						15		// ���Ӵ� ���� �� �ִ� �ְ� ���� 
#define DEFEAT_HANDICAP							3		// �й�ÿ��� ������ �� ���ڸ�ŭ ������. 

#ifdef _QUICKMODE
	#define DEFAULT_MULTI_PLAY_DELAY_TIME		35		// ������ �⺻ ���� �ð� (�����Ӵ� �ð�)
#else
	#define DEFAULT_MULTI_PLAY_DELAY_TIME		50		// ������ �⺻ ���� �ð� (�����Ӵ� �ð�)	
#endif

#define	RETREAT_PERMIT_CLOCK				20000		// ������ �������� �ȵǴ� �ð�(clock) 

// �ǳ��� ���� 
#define PANNEL_YSIZE						120

// ���� ��� 
#define GAME_MODE_NORMAL						0		// ���ǹ��� �ǹ� ���� ���� ĳ���� ����� �¸� 
#define GAME_MODE_FLAG							1		// ���� ����� ���Ѿ� �Ʊ� �������� �������� �¸� 
#define GAME_MODE_TIMELIMIT						2		// �ð� ���� ���� 
#define GAME_MODE_HERO							3		// ����鰣�� �ϱ���. 
#define GAME_MODE_ONLINEBATTLE					4		// �¶��� ����. 

// ���� Ÿ���� ���� 
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


						
// ���� ���� 						
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
//    1��° �Ӽ� (������ �� �������� Ư¡.) 
//    ������ ��Ȳ�� ���� �Ӽ��� ���ֱ⵵, �߰��ϱ⵵ �� �� �ִ�. 
//    ���� ���� �Һ��� �Ӽ��̾�� �Ѵٸ� KINDATB_ �� �з��� ��. 
//----------------------------------------------------
// ������ blackcheck, �þ��� ������ ���� �ʴ´�. 
#define ATB_CIRCLEBALCKCHECK					0x00000002         
#define ATB_ATTACK								0x00000004	// ���� �Ӽ�
#define ATB_ATTACKGROUND						0x00000008	// ���� ���� �Ӽ�
#define ATB_FLY									0x00000010	// ���� ĳ���� 
#define ATB_SHIP								0x00000020	// ����
#define ATB_ABSANIWAIT							0x00000080	// ���� ���ۿ� ������� ������ �ִ����� 
#define ATB_ATTACKAIR							0x00000400	// ���� ���� �ɷ�
#define ATB_TRANSPARENT							0x00000800	// �����ϰ� ��´�. 
#define ATB_HORSE								0x00001000  // ���� ź ĳ�����ΰ�?

// robypark 2005/1/6 18:11
#define ATB_EFFECT_VILLAGECOUNTERATTACK_OIL		0x00002000	// ���� �ݰ� - �⸧ ������ �޴´�. ��, ����Ʈ�� �׷��� �Ѵ�. ex) ���� ����
#define ATB_EFFECT_VILLAGECOUNTERATTACK_STONE	0x00004000	// ���� �ݰ� - �� ������ �޴´�. ��, ����Ʈ�� �׷��� �Ѵ�. ex) ���� �˺�

// robypark 2005/1/8 11:1
#define ATB_STORE_AND_SUPPLY_SUPPLYGOODS		0x00008000	// ������ ����ǰ�� ���� �� �ٸ� ������ ���ֿ��� ������ �� �ִٴ� �Ӽ�. ex) ������ ���޼���

#define ATB_PATIENT								0x00010000	// ġ�ᰡ���� ĳ���� 
#define ATB_REPAIR								0x00020000	// ���� ������ ĳ���� 
#define ATB_SHADOW								0x00200000	// �׸��ڸ� ���°�?
#define ATB_LOWMOVE								0x00400000	// ���� �̵��� �����ϴ�.
#define ATB_LOWATTACK							0x00800000	// ���� ������ �����ϴ�.
#define ATB_MINRANGE							0x01000000	// ������ �Ұ� 
#define ATB_STRAIGHTSIGHT						0x02000000	// ���Ÿ� �þ� 
#define ATB_ICE									0x04000000	// �����Ӽ�. 
#define ATB_SELFEXP								0x08000000	// ���� �Ӽ�
#define ATB_CAPTURE								0x20000000	// ���� �ɷ�
#define ATB_BETRAY								0x40000000	// ��Ȥ �ɷ�

//----------------------------------------------------
// robypark 2004/10/29 14:34
//    1��° �Ӽ� (������ �� �������� Ư¡.) 
//----------------------------------------------------
#define ATB_ATTACK_VILLAGE						0x80000000	// robypark 2004/10/29 14:34 - ���� ���� ������ ������ ĳ���������� ��Ÿ���� �Ӽ���

//----------------------------------------------------
//    2��° �Ӽ� 
//----------------------------------------------------
#define ATB2_HEAL								0x40		// ġ�ᰡ���� ĳ�����ΰ�?
#define ATB2_MANA								0x80		// ���� ȸ���� �Ѵ�.
#define ATB2_WARP								0x1000		// �����̵��� ������ ĳ�����ΰ�?
#define ATB2_HIDE								0x2000
#define ATB2_WARPBASE							0x4000		// ���ڵ��� �����ذ��� ���� ������ �����ΰ�?
#define ATB2_STEALEXP							0x10000		// ����ġ ���Ѵ� �ɷ�
#define ATB2_SAMMISSILE							0x100000	// ��� �̻����� �����°�
#define ATB2_GUNSMOKE							0x200000	// �Ѿ� �߻� ����
#define ATB2_SAMC								0x400000	// õ���� ȿ����
#define ATB2_CANNON2KFIRE						0x800000	// ���屺���� ȿ����
#define ATB2_BETRAY								0x2000000   // ��Ȥ �ɷ�
#define ATB2_BETRAY2							0x10000000  // ��Ȥ��
#define ATB2_STEALTECH							0x20000000  // ��� ��ġ��
#define ATB2_INITTRAINSPOT						0x40000000	// ó������ �������� �����Ǿ� ����(�ǹ� ����)
#define ATB2_MINE								0x80000000	// ���ڸ� �ż��� �� �ִ� �Ӽ�

//------------------------------------------------------------------------------------------------
//    3��° �Ӽ� : ���� ���� �Ӽ�. 
//------------------------------------------------------------------------------------------------
#define ATB3_ENEMYCLON							0x00000001	// ���� �����ϴ� �Ӽ�
#define ATB3_MAGIC5J							0x00000002	// ����������. 
#define ATB3_MAGIC8K							0x00000004	// ����������
#define ATB3_MAGIC4								0x00000008  // �������
#define ATB3_FIREARROW							0x00000010  // ��ȭ�� 
#define ATB3_FIREARROW2							0x00000020  // ���� ��ȭ�� 
#define ATB3_SEAHORSECLON						0x00000040  // �ظ� ���� 
#define ATB3_CLON								0x00000080  // �Ϲ� �н�
#define ATB3_KIMBOMB							0x00000100	// ���ڰ��� ����� �� �ִ� �Ӽ�. 
#define ATB3_SPECIALATTACK						0x00000200	// �ϰ��ʻ�.
#define ATB3_MAGIC6C							0x00000400	// ���ڼ�
#define ATB3_TORNADO							0x00000800  // ǳ���
#define ATB3_EARTHQUAKE							0x00001000  // ������
#define ATB3_LIGHTNING							0x00002000	// ������
#define ATB3_ABSORBLIFE							0x00004000  // �����. (���� ü���� ���Ѵ´�. ) 
#define ATB3_SHIELDMAGIC						0x00008000	// �� ������ ������ �� �ִ°�?
#define ATB3_EXTINGUISH							0x00010000	// ���� ���� �Ӽ�
#define ATB3_GROUNDEXP							0x00020000	// ������
#define ATB3_CLON2								0x00040000  // �ı�,�Ʊ� �н�. 
#define ATB3_MAGICTEST							0x00080000  // �׽�Ʈ ����.
#define ATB3_MAGIC_RECALL_PANDA					0x00100000  // �Ҵ� ��ȯ.
#define ATB3_MAGIC_RECALL_MONKEY				0x00200000  // Ȳ�� ��ȯ.
#define ATB3_MINE								0x00400000	// ���� �ż�.
#define ATB3_POISONBULLET						0x00800000  // ��ź. 
#define ATB3_MAGIC4T							0x01000000  // ���ɺм��(��������)
#define ATB3_YELLOWSAND							0x02000000  // Ȳ��.
#define ATB3_MAGIC_RECALL_AMAKUSA				0x04000000  // ����� ��ȯ.
#define ATB3_ICE								0x08000000	// �̻��� ��� �ֺ� �󸮴� ����.
#define ATB3_SOUL								0x10000000	// �հ��� �߼���ȥ.
#define	ATB3_GHOST								0x20000000	// �丣������ �ͽ�ź.
#define ATB3_DARKFIRE							0x40000000	// ������ ����ȭ�� ��ǳ.
#define ATB3_MAGIC_RECALL_TIGER					0x80000000	// ��ȣ ���� ȣ���� ��ȯ.

#define ATB4_IVORY								0x00000001  // ����2��  ������� 
#define ATB4_BLUEDRAGON							0x00000002	// �������  ���û��� 
#define ATB4_RUNNINGFIRE						0x00000004	// �̷�     ������ź 
#define ATB4_ENERGYPA							0x00000008  // ������	��ǳ
#define ATB4_SNOWATTACK							0x00000010  // ������   ��������
#define ATB4_FIREATTACK							0x00000020  // �̷�     ȭ������
#define ATB4_FIREENERGYPA						0x00000040	// ������� ��ȭ��ǳ(���Ȳ���).

#define ATB4_DOUBLE_SWORD						0x00000080	// ��Ͻ� �ְ˳���
#define ATB4_LONG_SPEAR							0x00000100	// ����â ��â��ȯ
#define ATB4_ICE_BARRIER						0x00000200	// ���� ������
#define ATB4_FLOWER_NEEDLE_RAIN					0x00000400	// ���� ��õȭ��
#define ATB4_SOUL_CHANGE						0x00000800	// ���̸��� ���Ǽ�
#define ATB4_STONE_MAGIC						0x00001000	// �弱ȭ ������
#define ATB4_GOD_SPEAR							0x00002000	// ���� ��â�뵵(�ϰ��ʻ�)
#define ATB4_CLOUD_WIND							0x00004000	// ���»� ǳ���
#define ATB4_SHIELD								0x00008000	// ����
#define ATB4_MANABURN							0x00010000	// ������.
#define ATB4_MAGIC_RABBIT						0x00040000	// �䳢 ��ȯ(��ȣ��)
#define ATB4_MAGIC_RECALL_DAMDUK				0x00100000	// �����ȯ

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:55) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define ATB4_FIRE_ROUND_DANCE					0x00200000		// ����������(����-�ź���, �̼��� 2�� ���� ���)
#define ATB4_NOVA								0x00400000		// ����(����-������, ���� 2�� ���� ���)
#define ATB4_FURY_OF_VOLCANO					0x00800000		// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
#define ATB4_POISONING_HEAVENS					0x01000000		// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define ATB4_BOMB_OF_HEAVEN						0x02000000		// ��õ��(�߱�-�߼���, ���� 2�� ���� ���)
#define ATB4_SIEGE_MODE							0x04000000		// ��õ���(�߱�-�Ҷ�������, �̷� 2�� ���� ���)
#define ATB4_ONSLAUGHT_FIREBALL					0x08000000		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
#define ATB4_FIRERAIN_OF_GOD					0x10000000		// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 

// robypark 2004/6/4 14:6
#define ATB4_SIEGE_MODE_UNDO					0x20000000		// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷� 2�� ���� ���)


//------------------------------------------------------------------------------------------------
//    ������ ���õ� �Ӽ�
//------------------------------------------------------------------------------------------------
#define DYINGATB_SHIP							1
#define DYINGATB_BUILDING						2			// �ǹ��� ���� 
#define DYINGATB_IMMEDIATE						4			// ���ظ� ������ �ʰ� ����  
#define DYINGATB_EXP							8			// ���߷� ����� ����    
#define DYINGATB_NOCARCASS						16			// ��ü�� ������ ���� 
		

//------------------------------------------------------------------------------------------------
// ������ �Ӽ� 
//------------------------------------------------------------------------------------------------
#define HITATB_SWORD							1
#define HITATB_RANGE							2
#define HITATB_FIREARROW						3
#define HITATB_MAGIC							4
#define HITATB_FIRE								5
#define HITATB_SKILL							6

//------------------------------------------------------------------------------------------------
// ���� �Ӽ�		
//------------------------------------------------------------------------------------------------
/*
typedef enum	ENUM_ATTACKATB
{
	ATTACKATB_SWORD							1
	ATTACKATB_STRONGGUN						2
	ATTACKATB_AIRMISSILE					3
	ATTACKATB_SAMMISSILE					4
	ATTACKATB_CANNON2C						5			// �߼��� 
	ATTACKATB_NEEDLE						6			// ħ
	ATTACKATB_CANNON						7
	ATTACKATB_GUN							8
	ATTACKATB_MAGIC							9
	ATTACKATB_MAGIC2						10			// ���� �º��� ���ݿ� ���� 
	ATTACKATB_MAGIC3						11			// �Ϻ� ������ ���ݿ� ���� 

	ATTACKATB_FIRECAR						13
	ATTACKATB_SHIPCANNON					14
	ATTACKATB_FIRE							15
	ATTACKATB_HEAVYGUN						16
	ATTACKATB_FIREARROW						17			// ��ȭ�� 
	ATTACKATB_ARROW							18
	ATTACKATB_SELFEXP						19
	ATTACKATB_LIGHTNING						20			// ������
	ATTACKATB_EARTHQUAKE					21			// ������
	ATTACKATB_BIGSWORD						22			// ū��
	ATTACKATB_ELECTRON						23
	ATTACKATB_THROWBOMB						24			// ������ ��ź 
	ATTACKATB_MAGICBALL						25			// ������ ���� ���� 
	
	ATTACKATB_BOMB							27
	ATTACKATB_TORNADO						28			// ǳ���
	ATTACKATB_KIMBOMB           		    29			// ���ڰ�
	ATTACKATB_MAGIC5J           		    30			// ������ ����������
	ATTACKATB_DART							31			// ������ ������
	ATTACKATB_MAGIC8K						32			// ��ù��� ���� ������ 
	ATTACKATB_CANNONJ						33          // �ļ��� ����. 
	ATTACKATB_FIRETANK						34
	ATTACKATB_COLLTANK						35
	ATTACKATB_CANNON2J						36			// �߼��� 
	ATTACKATB_POISONARROW					37			// ��ħ 
	ATTACKATB_FIRETANKARROW					38			// ���� ����� ���� 
	ATTACKATB_COLLTANKJARROW				39			// ���� ������ ���� 
	ATTACKATB_GENERAL4KFIRE					40			// ������ ���� 
	ATTACKATB_GENERAL4J						41			// ������ ���� 
	ATTACKATB_GENERAL3C						42			// ��ȿ�� ���� 
	ATTACKATB_CANNONKARROW					43			// �������� ���� 
	ATTACKATB_FLAMETANKJARROW				44
	ATTACKATB_CANNONC						45			// �Ŷ� ��ȣ���� ���� 
	ATTACKATB_AXE							46			// ���� ����
	ATTACKATB_SWORDENERGY					47			// ������ �ߵ��˱� �Ӽ�
	ATTACKATB_ELEPHANT						48			// �ڳ��� (��2)
	ATTACKATB_ANTIARMOUR					49			// ���ī�� (��2)
	ATTACKATB_CHASEFIRE						50			// ������ ȭ���� �Ӽ� (õ��)
	ATTACKATB_AIRATTACKUPARROW				51			// ���� ����
	ATTACKATB_TOWERCANNON					52			// ����� ȭ��
	ATTACKATB_UPGRADETOWERCANNON			53			// ���׷��̵� ����� ȭǥ
	ATTACKATB_GATPHAPOISON					54			// �¶��� - ������ ���� ������
	ATTACKATB_DUEOKSINI						55          // �ξ�ô� ���� �Ӽ� - ������ �ڷ� �и�. 
	ATTACKATB_TOAD							56          // ���β���

	ATTACKATB_FURHEADDEMON                  60			// �а���. 
	ATTACKATB_SNOWWOMAN		                61			// ����. 
	ATTACKATB_GOLDDRAGONELECTRON            62			// �ݷ� ����. 
	ATTACKATB_ONGHWA						63			// ��õ ������ �ذ�.  
	ATTACKATB_GHOSTCAT						64			// �͹� ���� ����. 
	ATTACKATB_DOLL1							65			// ���� ���� ����. 
	ATTACKATB_DOLL2							66			// ��ī�� ���� ����. 
	ATTACKATB_JUSTICEK						67			// ���� ���� ����
	ATTACKATB_BUTTERFLY						68			// ������ ���� 
	ATTACKATB_WIDOW							69			// �Ź̿䱫 ���� 
	ATTACKATB_WAVE							70			// �ٴٰź��� �ĵ�����. 
	ATTACKATB_FIREUSINGMANA					71			// �Ұ���(���� ���)
	ATTACKATB_LEECH							72			// �հŸӸ�. 
	ATTACKATB_SEAHORSE						73			// �ظ������. 
	ATTACKATB_MACHINEGUN					74			// ���� ��� 
	ATTACKATB_THROWSPEAR					75			// ������â (���ֹ�)  
	ATTACKATB_SCORPION						76			// ������ ��. 
	ATTACKATB_CACTUS						77			// ���������� ����. 
	ATTACKATB_CLAW							78			// ������ ���ݼӼ�. 
	ATTACKATB_MAGICIAN						79			// ����� ��������.  
	ATTACKATB_BIGTREE						80			// �Ŵ�� �������� ����. 
	ATTACKATB_COBRA							81			// �ں�� �� ����.
	ATTACKATB_VOLCANOFLAME					82			// ȭ���ǺҲ�.   �ҵ���.
	ATTACKATB_EVIL							83			// �̻��� ���.
	ATTACKATB_ICE							84			// �̻��� ��� Ư�����.
	ATTACKATB_MAGIC4T						85			// ¡Ű��ĭȥ Ư�� ���.
	ATTACKATB_JAPANGHOST					86			// �Ϻ��ͽ� Ư�� ���.
	ATTACKATB_KUKET							87			// ����
	ATTACKATB_AUTA							88			// �ƿ�Ÿ ����.
	ATTACKATB_SUB1							89			// �������� ȭ����� 1
	ATTACKATB_SUB2							90			// �������� ȭ����� 2
	ATTACKATB_FIREATTACK					91			// ȭ������.
	ATTACKATB_RUNNINGFIRE					92			// ����
	ATTACKATB_ENERGYPA						93			// ��ǳ.
	ATTACKATB_LONG_SPEAR					94			// ����â ��â��ȯ.
	ATTACKATB_PAPERDOLL						95			// ����� ��������
	ATTACKATB_NEEDLEATTACK					96			// ħ����
	ATTACKATB_CLOUDWIND						97			// ǳ���
	ATTACKATB_STONEMAGIC					98			// ������
	ATTACKATB_GODSPEAR						99			// ��â�뵵
	ATTACKATB_FLOWERRAIN					100			// ��õȭ��
	ATTACKATB_SOULCHANGE					101			// ���Ǽ�
	ATTACKATB_MAGICCATTACK					102			// ������ ����
	ATTACKATB_SUNBIYOUN						103			// ���� ����
	ATTACKATB_SNOW							104			// ������

	// actdoll (2004/03/06 5:00) : 3�� ��ġ���� �߰�
	ATTACKATB_PYTHON						105			// �̹��� ��������	// actdoll (2004/03/06 4:08) : �߰�
	ATTACKATB_JINEMPEROR					106			// ����Ȳ��ȥ ��������	// actdoll (2004/03/06 4:08) : �߰�
	ATTACKATB_GUARDIAN_SNAKE				107			// ��ȣ��(��)
	//#define ATTACKATB_RABBIT						105			// 

	// <����> ����� char-attack, calcattack�� RealApower�� �����ؾ� ��, ������� �������̾�� ��.

	MAX_ATTACKATB_NUMBER					110
};


//------------------------------------------------------------------------------------------------
// ��� �Ӽ�		
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

// <����> MAX_DEFENCEATB_NUM ��Ȯ�� ����, calcattack�� RealApower ������ ��
#define MAX_DEFENCEATB_NUM						9	
*/

//-----------------------------------------------------------------------------
// �ʻ�� ����
#define SPECIAL_MAGIC5J				     		1			// ������ ����������
#define SPECIAL_MAGIC8J							2			// �ƻ����� ȯ����
#define SPECIAL_MAGIC4C							3			// �̼����� �������
#define SPECIAL_MAGIC6C							4			// ����â�� ���ڼ�		
#define SPECIAL_AMBUSH							5			// �ź�
#define SPECIAL_DETECTMINE						6			// �Ǻ��� ����Ž��
#define SPECIAL_EXTINGUISH						7			// �Ҳ���
#define SPECIAL_KIMBOMB							8			// ���ڰ� 
#define SPECIAL_GROUNDEXP						9			// ������
#define SPECIAL_TORNADO							10			// ǳ���
#define SPECIAL_MAGIC4T							11			// ���ɺм��(��������)
#define SPECIAL_MAGIC2T							12			// ������
#define SPECIAL_EARTHQUAKE						13			// ������
#define SPECIAL_LIGHTNING						14			// ������
#define SPECIAL_SPECIALATTACK					15			// �ϰ��ʻ�
#define SPECIAL_ABSORBLIFE						16			// �����
#define SPECIAL_FIREARROW						17			// ��ȭ��
#define SPECIAL_FIREARROW2						18			// ������ȭ��
#define SPECIAL_MAGIC8K							19			// �������
#define SPECIAL_HIDE							20			// ���ż�
#define SPECIAL_SNOWMAGIC						21			// ���� ����ǳ
#define SPECIAL_BIGTREE							22			// �Ŵ�� Ư������
#define SPECIAL_MAGICTEST						23			// �׽�Ʈ 
#define SPECIAL_MINE							24			// ����
#define SPECIAL_YELLOWSAND						25			// Ȳ��
#define SPECIAL_ICE								26			// �̻��� ��� Ư������
#define SPECIAL_SOUL							27			// �հ��� �߼���ȥ
#define SPECIAL_GHOST							28			// �丣������ �ͽ�ź
#define SPECIAL_DARKFIRE						29			// ������ ����ȭ����ǳ
#define SPECIAL_IVORY							30			// ������� 
#define SPECIAL_BLUEDRAGON						31			// ���û�浵
#define SPECIAL_RUNNINGFIRE						32			// ������ź
#define SPECIAL_ENERGYPA						33			// ��ǳ
#define SPECIAL_SNOWATTACK						34			// ��������
#define SPECIAL_FIREATTACK						35			// ȭ������
#define SPECIAL_FIREENERGYPA					36			// ��ȭ��ǳ(���Ȳ���)
#define SPECIAL_DOUBLE_SWORD					37			// �ְ˳���
#define SPECIAL_LONG_SPEAR						38			// ��â��ȯ
#define SPECIAL_ICE_BARRIER						39			// ������
#define SPECIAL_FLOWER_NEEDLE_RAIN				40			// ��õȭ��
#define SPECIAL_SOUL_CHANGE						41			// ���Ǽ�
#define SPECIAL_STONE_MAGIC						42			// ������
#define SPECIAL_GOD_SPEAR						43			// ��â�뵵(�ϰ��ʻ�)
#define SPECIAL_CLOUD_WIND						44			// ǳ���
#define SPECIAL_MANABURN						45			// ������.

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:25) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define SPECIAL_FIRE_ROUND_DANCE				46			// ����������(����-�ź���, �̼��� 2�� ���� ���)
#define SPECIAL_NOVA							47			// ����(����-������, ���� 2�� ���� ���)
#define SPECIAL_FURY_OF_VOLCANO					48			// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
#define SPECIAL_POISONING_HEAVENS				49			// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define SPECIAL_BOMB_OF_HEAVEN					50			// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
#define SPECIAL_SIEGE_MODE						51			// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
#define SPECIAL_ONSLAUGHT_FIREBALL				52			// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
#define SPECIAL_FIRERAIN_OF_GOD					53			// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 

// robypark 2004/6/4 14:7
#define SPECIAL_SIEGE_MODE_UNDO					54			// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)

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


#define WS_DECIDE_LENGTH_TO_DEST		 	1			// ��ǥ ������ �Ÿ� ��� 
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
			
// ���� ���� ���� 		
		
#define GS_INITSTAGEGAME         		1 
#define GS_GAME                  			3
#define GS_INITSETGAME           			4
#define GS_SETGAME               			5
#define GS_INITSETMULTI          			6 
#define GS_SETMULTI              			7
#define GS_INITINITMENU          			8
#define GS_INITMENU              			9			// ���� �ʱ� �޴� 
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
#define GS_INITRESTART           			32			// ���� �ٽ��ϱ����� �ʱ�ȭ 
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
#define ITEM_EXTINGUISH					6	// �Ҳ���
#define ITEM_BUILD       				8
#define ITEM_SELL        				9
#define ITEM_REPAIR      				10
#define ITEM_BUILD1						11
#define ITEM_BUILD2						12 
#define ITEM_INITMENU					13   // �Ǽ� 1 �̳� �Ǽ� 2�� ����Ѵ�.  
#define ITEM_SELLMENU					14   // ��ü ��� ��带 �����Ѵ�.
#define ITEM_INITGMENU					15
#define ITEM_BUILDCANCEL				16   // �Ǽ��� �ߴ��Ѵ�. 
#define ITEM_TRAINSPOT					17   // ������ ���� 
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
#define ITEM_MAGIC_DOUBLE_SWORD			85			// �ְ˳���
#define ITEM_MAGIC_LONG_SPEAR			86			// ��â��ȯ
#define ITEM_MAGIC_ICE_BARRIER			87			// ������
#define ITEM_MAGIC_FLOWER_NEEDLE_RAIN	88			// ��õȭ��
#define ITEM_MAGIC_SOUL_CHANGE			89			// ���Ǽ�
#define ITEM_MAGIC_STONE_MAGIC			90			// ������
#define ITEM_MAGIC_MANABURN				91

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:19) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define ITEM_MAGIC_FIRE_ROUND_DANCE				92	// ����������(����-�ź���, �̼��� 2�� ���� ���)
#define ITEM_MAGIC_NOVA							93	// ����(����-������, ���� 2�� ���� ���)
#define ITEM_MAGIC_FURY_OF_VOLCANO				94	// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
#define ITEM_MAGIC_POISONING_HEAVENS			95	// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define ITEM_MAGIC_BOMB_OF_HEAVEN				96	// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
#define ITEM_MAGIC_SIEGE_MODE					97	// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
#define ITEM_MAGIC_ONSLAUGHT_FIREBALL			98	// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
#define ITEM_MAGIC_FIRERAIN_OF_GOD				99	// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 

// robypark 2004/6/4 14:8
#define ITEM_MAGIC_SIEGE_MODE_UNDO				100	// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)

#define MAX_ITEM_NUMBER					150


// Enum - KindInfo
//#include "..\EnumData\Enum_KI_Character.h"
/*		
// (����) kind �߰��� �� �� �������� �߰��� ��, KIND_ON_MAX �ִ밳�� �̻����� ����.
// actdoll (2004/02/26 10:46) : define���� ���ǵ� ������ enum������ �����ߴ�.
typedef enum	ENUM_KIND_ON_CHARACTER
{
	KIND_ON_INVALID				= -1,				//			// �߸��� ��

	KIND_PLAYERSTART			= 1,				//1			// �÷��̾� ���� ��ġ  

	// ��� 
	KIND_MANAGER1,									//2			// ��� 1

	// ���ΰ�
	KIND_ON_KEON,									//3			// ����
	KIND_ON_MYUNGHWA,								//4			// �̸�ȭ
	KIND_ON_RYO,									//5			// �ø��� ��
	KIND_ON_HANAHINE,								//6			// �ճ� �ϳ�����
	KIND_ON_SORYUNG,	 							//7			// �ռҷ� 
	KIND_ON_MIRYUNG,								//8			// �����
	KIND_ON_DANG,									//9			// ��ȣ��(��)  
	KIND_ON_SUYOUN,									//10		// �����(��)

	// ���� �뺴 
	KIND_ON_HORSEARCHERK		= 15,				//15		// �⸶�û�
	KIND_ON_JUSTICEK,								//16		// ���� 
	KIND_ON_BUDAK,									//17		// ���� �İ��
	KIND_ON_SWORDK,									//18		// â���� 
	KIND_ON_ARCHERK,								//19		// �ü�
	KIND_ON_BUTCHER,								//20		// Į����
	KIND_ON_PORTERK,								//21		// ��������

	KIND_ON_HORSEARCHERUPK,							//22		// �⸶�û�(Upgrade)
	KIND_ON_JUSTICEUPK,								//23		// ����(Upgrade)
	KIND_ON_BUDAUPK,								//24		// ���� �İ��(Upgrade)
	KIND_ON_SWORDUPK,								//25		// â���� (Upgrade)
	KIND_ON_ARCHERUPK,								//26		// �ü�(Upgrade)

	KIND_GENERAL1K,									//27		// ���� ����
	KIND_GENERAL2K,									//28		// ���� �̼���
	KIND_GENERAL3K,									//29		// ���� ������
	KIND_GENERAL4K,									//30		// ���� �����
	KIND_GENERAL5K,									//31		// ���� �����
	KIND_GENERAL6K,									//32		// ���� ����
	KIND_GENERAL7K,									//33		// ���� �����
	KIND_GENERAL8K,									//34		// ���� ��ù�
	KIND_CANNON2K,									//35		// ���� ���屺��

	// �Ϻ� �뺴 
	KIND_ON_SWORDJ				= 40,				//40		// ����
	KIND_ON_GUNJ,									//41		// ���
	KIND_ON_NINJAJ,									//42		// �Ϻ� ����
	KIND_ON_HORSESWORDJ,							//43		// ���볶��
	KIND_ON_ADVBUDAJ,								//44		// �Ϻ� �𸶻� 
	KIND_ON_DSWORD,									//45		// �⸶����
	KIND_ON_PORTERJ,								//46		// �Ϻ�����

	KIND_ON_SWORDUPJ,								//47		// �˼�����(Upgrade)
	KIND_ON_GUNUPJ,									//48		// ���(Upgrade)
	KIND_ON_NINJAUPJ,								//49		// ����(Upgrade)
	KIND_ON_HORSESWORDUPJ,							//50		// ���볶�� (upgrade)
	KIND_ON_ADVBUDAUPJ,								//51		// �Ϻ� �𸶻� (Upgrade)

	KIND_GENERAL1J,									//52		// �Ϻ� ��Ͻ�
	KIND_GENERAL2J,									//53		// �Ϻ� �����
	KIND_GENERAL3J,									//54		// �Ϻ� ��Ű��ī
	KIND_GENERAL4J,									//55		// �Ϻ� ���̼��
	KIND_GENERAL5J,									//56		// �Ϻ� ����
	KIND_GENERAL6J,									//57		// �Ϻ� ������
	KIND_GENERAL7J,									//58		// �Ϻ� ��������
	KIND_GENERAL8J,									//59		// �Ϻ� �ƻ���

	// �븸 �뺴 
	KIND_ON_BUDAT				= 63,				//63		// �븸�ּ���
	KIND_ON_AMAZONT,								//64		// �븸 ������
	KIND_ON_AXET,									//65		// �븸 �������� 
	KIND_ON_SWORDT,									//66		// �븸 ������
	KIND_ON_GUNT,									//67		// �븸 �����Ѽ�
	KIND_ON_PORTERT,								//68		// �븸����
	KIND_ON_BEASTWARRIOR,							//69		// �߼�����

	KIND_ON_BUDAUPT,								//70		// ����ּ��� 
	KIND_ON_AMAZONUPT, 								//71		// ��޿����� 
	KIND_ON_AXEUPT,									//72		// ��޵������� 
	KIND_ON_SWORDUPT,								//73		// ��޺����� 
	KIND_ON_GUNUPT,									//74		// ��޼����Ѽ� 

	KIND_ON_GENERAL1T,								//75		// �հ��� 
	KIND_ON_GENERAL2T,								//76		// �丣������ 
	KIND_ON_GENERAL3T,								//77		// ������
	KIND_ON_GENERAL4T,								//78		// ������ 
	KIND_ON_GENERAL5T,								//79		// ������ 
	KIND_ON_GENERAL6T,								//80		// ����â

	// �߱� �뺴
	KIND_ON_FAN_C				= 85,				//85		// ���谡
	KIND_ON_BUDAK_C,								//86		// ������
	KIND_ON_MUDOGA_C,								//87		// ������
	KIND_ON_SWORD_C,								//88		// ūĮ����
	KIND_ON_FIREGUN_C,								//89		// ȭ����
	KIND_ON_MAGIC_C,								//90		// ������
	KIND_ON_PORTER_C,								//91		// �߱�����

	KIND_ON_FANUP_C,								//92		// ���谡
	KIND_ON_BUDAKUP_C,								//93		// ������
	KIND_ON_MUDOGAUP_C,								//94		// ������
	KIND_ON_SWORDUP_C,								//95		// ūĮ����
	KIND_ON_FIREGUNUP_C,							//96		// ȭ����

	KIND_ON_GENERAL1_C,								//97		// �������
	KIND_ON_GENERAL2_C,								//98		// �̷�
	KIND_ON_GENERAL3_C,								//99		// ��������
	KIND_ON_GENERAL4_C,								//100		// ������
	KIND_ON_GENERAL5_C,								//101		// ����	
	KIND_ON_GENERAL6_C,								//102		// ����

	// ���� 
	KIND_ON_HARUBANG			= 107,				//107		// ���Ϸ��. 
	KIND_ON_CRAB,									//108	    // �ٴٰ�. 
	KIND_ON_LEECH,									//109		// �ŸӸ� 
	KIND_ON_SEAHORSE,								//110		// �ظ� 
	KIND_WHITETIGER,								//111		// ��ȣ 
	KIND_GHOSTCAT,									//112		// �͹� 
	KIND_TUTTLE,									//113		// �ٴٰź���
	KIND_BUTTERFLY,									//114		// ������
	KIND_WIDOW,										//115		// �Ź̿䱫
	KIND_INSECT,									//116		// �縶��
	KIND_WORRIORGHOST,								//117		// ������ȥ
	KIND_SINGERMGHOST,								//118		// �Ű���ȥ
	KIND_MAMMOTH,									//119		// ���ڳ���
	KIND_WILDBOAR,									//120		// �����
	KIND_ON_THIEF,									//121		// ������ 
	KIND_ON_SEAKING,								//122		// ������ 
	KIND_ON_SWORDPIRATE,							//123		// �¶��� - �ֱ�
	KIND_ON_GUNPIRATE,								//124		// �¶��� - ���ѿֱ�
	KIND_ON_SNOWWOMAN,								//125		// �¶��� - ����
	KIND_ON_EATERDEMON,								//126     // �¶��� - �ı�
	KIND_ON_WEIRDPLANT,								//127     // �¶��� - �����
	KIND_ON_HUNGRYDEMON,							//128     // �¶��� - �Ʊ�
	KIND_ON_TIGER,									//129     // �¶��� - ȣ����
	KIND_ON_DOGDEMON,								//130     // �¶��� - �߽�
	KIND_ON_ONGHWA,									//131     // �¶��� - ��ȭ
	KIND_ON_BANDIT1,								//132     // �¶��� - ����1(��)
	KIND_ON_BANDIT2,								//133     // �¶��� - ����2(��)
	KIND_ON_BANDIT3,								//134     // �¶��� - ����3(��, �蹶ġ)
	KIND_ON_BANDIT4,								//135		// �¶��� - ����3(��, Ȱ)
	KIND_ON_GACKDAGUI,								//136		// �¶��� - ���ٱ�
	KIND_ON_GATPHA,									//137		// �¶��� - ����
	KIND_ON_BOGY,									//138		// �¶��� - ������
	KIND_ON_FOX,									//139		// �¶��� - ����
	KIND_ON_STARFISH,								//140		// �¶��� - �Ұ��縮
	KIND_ON_SHIP1,									//141		// �¶��� - ��1
	KIND_ON_ONI,									//142     // �¶��� - ����
	KIND_ON_SNOWDEMON,								//143     // �¶��� - ����
	KIND_ON_FURHEADDEMON,							//144     // �¶��� - �а���
	KIND_ON_FIREDRAGON,								//145     // �¶��� - ȭ��
	KIND_ON_GOLDDRAGON,								//146		// �¶��� - �ݿ�
	KIND_ON_BAT,									//147		// �¶��� - ����
	KIND_ON_TOAD,									//148		// �¶��� - ���β���
	KIND_ON_CENTIPEDE,								//149		// �¶��� - ������
	KIND_ON_DUEOKSINI,								//150		// �¶��� - �ξ�ô�
	KIND_ON_GANGSI,									//151		// �¶��� - ����
	KIND_ON_RACCOONDOG,								//152		// �¶��� - �ʱ���
	KIND_ON_DEER,									//153		// �¶��� - �罿
	KIND_ON_BEAR,									//154		// �ݴް� 
	KIND_ON_HORSESPEAR,								//155		// �⸶â�� 
	KIND_ON_CAT,									//156		// �븸�����
	KIND_ON_DUCK,									//157		// �븸����
	KIND_ON_HORNET,									//158		// �븸�չ�
	KIND_ON_MBANDIT1,								//159		// �����θ� 
	KIND_ON_MBANDIT2,								//160		// ��ġ���� 
	KIND_ON_MBANDIT3,								//161		// �Ѽ����� 
	KIND_ON_CHAWOO,									//162		// �������� 
	KIND_ON_PIRATES,								//163		// ���� 
	KIND_ON_PANDER,									//164		// �Ҵ� 
	KIND_ON_SNAKE,									//165		// ������ 
	KIND_ON_CROCODILE,								//166		// ���ξǾ� 
	KIND_ON_HOOK,									//167		// �������� 
	KIND_ON_SMALLGANG,								//168		// ��������
	KIND_ON_HORSEMAN,								//169		// ������
	KIND_ON_BABARIAN,								//170		// ���ֹι��� 
	KIND_ON_BABARIANSHAMAN,							//171		// ���ֹ� 
	KIND_ON_BADDEER,								//172		// Ÿ���絹�� 
	KIND_ON_CACTUS,									//173		// ����������
	KIND_ON_SCORPION,								//174		// ������ 
	KIND_ON_SKYNINE,								//175		// õ��
	KIND_ON_ICETREE,								//176		// ��������
	KIND_ON_SNOWLIZARD,								//177		// ��������
	KIND_ON_MADBULL,								//178		// ����
	KIND_ON_BIGTREE,								//179		// �Ŵ��
	KIND_ON_MAGICIAN,								//180		// ����� 
	KIND_ON_BADMERCHANT,							//181		// �Ǵ����� 
	KIND_ON_RUNNERHAND,								//182		// ����������.
	KIND_ON_SANDGOD,								//183		// ���ǽ�.
	KIND_ON_SEAENVOY,								//184		// �ٴ��ǻ��.
	KIND_ON_VIPER,									//185		// �칫��.
	KIND_ON_AMAKUSA,								//186		// �Ƹ����.
	KIND_ON_LAVAMONSTER,							//187		// ��ϱ���
	KIND_ON_EVIL,									//188		// �̻��� ���.
	KIND_ON_BIRD,									//189		// �θ���
	KIND_ON_WEASEL,									//190		// ������
	KIND_ON_COBRA,									//191		// �ں��.
	KIND_ON_SOUL,									//192		// ¡�⽺ĭ�� ȥ.
	KIND_ON_VOLCANOFLAME,							//193		// ȭ���� �Ҳ�
	KIND_ON_REDMADBULL,								//194		// ��������.
	KIND_ON_MAGICOFFICER,							//195		// ������.
	KIND_ON_CAPBEAR,								//196		// ����ݴް�.
	KIND_ON_KOREANGHOST,							//197		// ó��ͽ�.
	KIND_ON_SKELETON,								//198		// �ذ�.
	KIND_ON_SKELETONSOLDIER,						//199		// �ذ񺴻�.
	KIND_ON_PIRATECAPTAIN,							//200		// ��������
	KIND_ON_TAIWANGHOST,							//201		// �븸�ͽ�.
	KIND_ON_JAPANGHOST,								//202		// �Ϻ��ͽ�.
	KIND_ON_AUTA,									//203		// �ƿ�Ÿ.
	KIND_ON_JUCK,									//204     // ������.
	KIND_ON_FEMALESHAMAN,							//205		// ��������.
	KIND_ON_QUEEN,									//206		// ��������.
	KIND_ON_DONKEY,									//207		// �糪��.
	KIND_ON_AGILETIGER,								//208		// ��ȣ.
	KIND_ON_PAWNTIGER,								//209		// ����ȣ����.
	KIND_ON_OWL,									//210		// �û���.
	KIND_ON_LEOPARD,								//211		// ǥ��.
	KIND_ON_HUNSWARRIOR,							//212		// �������
	KIND_ON_HUNSHORSE,								//213		// ���⸶��
	KIND_ON_HUNSHORSEARCHER,						//214		// ���⸶�ü�
	KIND_ON_GATEKEEPER,								//215		// ������
	KIND_ON_CLAYFIGURE,								//216		// �⸶������
	KIND_ON_PHOENIX,								//217		// �һ���
	KIND_ON_JINEMPEROR,								//218		// ����Ȳ��ȥ
	KIND_ON_MEDICALMAN,								//219		// �Ǽ���.
	KIND_ON_HEOJUN,									//220		// ����.
	KIND_ON_COSMIC,									//221		// �����.
	KIND_ON_SEIMEI,									//222		// ���̸���.
	KIND_ON_YUMRUCK,								//223		// ���»�.
	KIND_ON_JANGSUNHWA,								//224		// �弱ȭ.
	KIND_ON_LONGSPEAR,								//225		// ��â����.
	KIND_ON_KOAKHU,									//226		// ����.	
	KIND_ON_SNOWMAN1,								//227		// �����1.
	KIND_ON_SNOWMAN2,								//228		// �����2.
	KIND_ON_SANTA1,									//229		// ��¥��Ÿ1
	KIND_ON_SANTA2,									//230		// ��¥��Ÿ2
	KIND_ON_SANTA3,									//231		// ��¥��Ÿ3
	KIND_ON_MOTHER_DEER1,							//232		// �����絹��1
	KIND_ON_MOTHER_DEER2,							//233		// �����絹��2
	KIND_ON_MOTHER_DEER3,							//234		// �����絹��3
	KIND_ON_MOTHER_DEER4,							//235		// �����絹��4
	KIND_ON_SANTA4,									//236		// ��¥��Ÿ 4
	KIND_ON_SANTA_TW,								//237		// �븸 ��¥ ��Ÿ.
	KIND_ON_MOTHER_DEER_TW,							//238		// �븸 ��¥ �絹��.
	KIND_ON_SHIP2,									//239		// �¶��� - ��2		actdoll (031230)
	KIND_ON_SHIP3,									//240		// �¶��� - ��3		actdoll (031230)
	KIND_ON_SHIP4,									//241		// �¶��� - ��4		actdoll (031230)
	KIND_ON_SHIP5,									//242		// �¶��� - ��5		actdoll (031230)
	KIND_ON_SHIP6,									//243		// �¶��� - ��6		actdoll (031230)
	KIND_ON_SHIP7,									//244		// �¶��� - ��7		actdoll (031230)
	KIND_ON_SHIP8,									//245		// �¶��� - ��8		actdoll (031230)
	KIND_ON_FLYDRAGON,								//246		// �ͷ�
	KIND_ON_RAPTOR,									//247		// ����
	KIND_ON_TIRANO,									//248		// Ƽ���
	KIND_ON_MONKEY,									//249		// ��ȯ������.

	// actdoll (2004/02/26 11:12) : �߰��� ���� - ����� �ʿ�
	KIND_ON_HELLKING,								//250		// ������
	KIND_ON_HELLMESSENGER,							//251		// ���»���
	KIND_ON_GHOSTSAMURAI,							//252		// ���� �繫����
	KIND_ON_PYTHON,									//253		// �̹���
	KIND_ON_GHOSTELEPHANT,							//254		// ���� �ڳ���
	KING_ON_GHOSTSEAKING,							//255		// ���� ������
	KIND_ON_GHOSTPIRATECAPTAIN,						//256		// ���� ��������
	KIND_ON_GHOSTSKELETON,							//257		// ���� �ذ�
	KIND_ON_GHOSTSKELETONSOLDIER,					//258		// ���� �ذ񺴻�
	KIND_ON_GHOSTFEMALESHAMAN,						//269		// ���� ����
	KIND_ON_GHOSTVOLCANOFLAME,						//260		// ���� �Ҳ�

	KIND_ON_GUARDIAN_MOUSE,							//261		// ��ȣ��(��)
	KIND_ON_GUARDIAN_CAW1,							//262		// ��ȣ��(��1)
	KIND_ON_GUARDIAN_CAW2,							//263		// ��ȣ��(��2)
	KIND_ON_GUARDIAN_CAW3,							//264		// ��ȣ��(��3)

	KIND_ON_GUARDIAN_CHICK,							//265		// ���Ƹ�.
	KIND_ON_GUARDIAN_PIG,							//266		// ����.
	KIND_ON_GUARDIAN_SNAKE,							//267		// ��.
	KIND_ON_GUARDIAN_MONKEY,						//268		// ������.


	KIND_ON_MAX										//			// �� ĳ������ ���� ����(���� ������ �ƴϴ�)
};
*/
// ���ӿ� ���Ǵ� ��ư�� ����. 
#define BUTTON_TYPE_GENERAL_MOVETO  		6	
#define BUTTON_TYPE_CREW			  		7	
#define BUTTON_TYPE_GAMEMENU				10
#define BUTTON_TYPE_ON_REGISTER				11
#define BUTTON_TYPE_ON_ALLY					13
#define BUTTON_TYPE_ON_ALLY_NUM				14
#define BUTTON_TYPE_ON_ALLY_WINDOW			15

//---------------------------------------------------------------------
//  ���� ���� 
//--------------------------------------------------------------------- 
#define ERROR_MAGIC8J_NOAPOWER				1
#define ERROR_BETRAY_HEALTH					2


// � ���� 						
						
// ��						
#define CROP_TYPE_RICE     				1
#define CROP_RICE0         				0
#define CROP_RICE1         				1
#define CROP_RICE2         				2
#define CROP_RICE3         				3

// ���� 
#define CROP_TYPE_POTATO   				2
#define CROP_POTATO0       				0
#define CROP_POTATO1       				1
#define CROP_POTATO2       				2
						
// ���� 						
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

// ���� ���� 
#define CROP_TYPE_RUIN0							4      // �ǹ� ���� 
#define CROP_TYPE_RUIN1							5      // �ǹ� ���� 
#define CROP_TYPE_RUIN2    						6      // ��ī�� �ı����� 
#define CROP_TYPE_DOLL1    						7      // ��� 
#define CROP_TYPE_TOMB1    						8      // ���� 
#define CROP_TYPE_BROKENTREE1 					9		// �η��� �Ÿ�
#define CROP_TYPE_ODDTREE1  					10		// ��Ȳ�� 
#define CROP_TYPE_WELL1   						11      // �칰 
#define CROP_TYPE_FLAGK1  						12      //���� ��� 
#define CROP_TYPE_FLAGC1  						13      // ���� ���
#define CROP_TYPE_FLAGJ1  						14      // �Ϻ� ���
#define CROP_TYPE_TEMPLE  						16      // ȭ���
#define CROP_TYPE_VOLCANO 						20		// ��ȭ�� 
#define CROP_TYPE_SIGN1   						30		// ǥ���� 
#define CROP_TYPE_STONE1  						50      // �ż� 
#define CROP_TYPE_STONE2  						51      // �ż�2
#define CROP_TYPE_TUTTLE1 						61      // �ź� ��
#define CROP_TYPE_AD11							70      // �Ｚ ����ǥ 
#define CROP_TYPE_AD12							80      // �Ｚ �ٴ�
#define CROP_TYPE_AD13							90      // �Ｚ ��ȭ�� 

#define CROP_TYPE_CASTLETOWER					200		// KBS ���� ����ȿ� - ���� ����
#define CROP_TYPE_DESTROYEDCASTLE				201		// KBS ���� ����ȿ� - ������ ����
#define CROP_TYPE_CASTLEGATE					202		// KBS ���� ����ȿ� - ����
#define CROP_TYPE_CASTLETILE					203		// KBS ���� ����ȿ� - ����Ÿ��
#define CROP_TYPE_CASTLELARGEGATE				204		// KBS ���� ����ȿ� - ū ����


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


// �볪�� 
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

extern BOOL			g_fTestVersion;		// �Ϸ� �׼����ΰ�?
extern BOOL			g_bSkipIntro;		// ��Ʈ�θ� ��ŵ�� ���ΰ�?
extern char			REG_PATH[256];

// ����׿� ���
extern	char		DF_PATH_DEBUG_OUTSIDE_HQTEXT[MAX_PATH];			// (����׿�) �ܺ� HQ Text �������

void				ShowErrorMsgBox( int iErrorCode, ... );


#endif//__MAIN_H__
