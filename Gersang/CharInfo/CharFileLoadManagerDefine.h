//=============================================================================
//	FileName	:	CharFileLoadManagerDefine.h
//	History		:	2001/10/23(박연숙) - Create
//	Discription	:	캐릭터 파일 관리에 관련한 Define들
//=============================================================================

#ifndef _CHARFILELOADMANAGERDEFINE_H_
#define _CHARFILELOADMANAGERDEFINE_H_

//-----------------------------------------------------------------------------
// 최대 스프라이트 갯수
//-----------------------------------------------------------------------------
#define MAX_XSPR				500			// 게임에서는 300정도만 일단 사용한다.
 
//-----------------------------------------------------------------------------
// ACTION
// 현재는 주인공과 캐릭터의 클래스가 동일하므로 같은 Action 순서와 Set를 구성해야한다.
//-----------------------------------------------------------------------------

// 주인공의 행동동작  
#define HERO_ACT_A1				0			// 공격
#define HERO_ACT_N1				1			// 제자리
#define HERO_ACT_WL				2			// 걷기
#define HERO_ACT_DT				3			// 죽기

#define MAX_HEROACTIONS			4	


//-----------------------------------------------------------------------------
// WEAPONCLASS
//-----------------------------------------------------------------------------
// 무기에 따라 행동할 수 있는 동작 CLASS
#define HERO_CLASS_1HS			0			// 한손 스윙
#define HERO_CLASS_2HS			1			// 두손 스윙
#define HERO_CLASS_2HT			2			//???
#define HERO_CLASS_THW			3			//???
#define HERO_CLASS_BOW			4			// 활
#define HERO_CLASS_HTH			5			// 맨손
#define HERO_CLASS_RIF			6			// 총
#define HERO_CLASS_SP1			7			// 기본 동작
#define HERO_CLASS_SP2			8			// 특수 동작
#define HERO_CLASS_SP3			9			// 특수 동작
#define HERO_CLASS_SP4			10			// 특수 동작
#define HERO_CLASS_SP5			11			// 특수 동작

#define MAX_HEROWEAPONCLASS		12			// 무기가 행동할수있는 동작

//-----------------------------------------------------------------------------
// BODYPART
//-----------------------------------------------------------------------------
// 하나의 캐릭터를 조합하기 위한 PART
#define HERO_BODY_LG			0
#define HERO_BODY_TR			1
#define HERO_BODY_HD			2
#define HERO_BODY_RA			3
#define HERO_BODY_LA			4
#define HERO_BODY_RH			5
#define HERO_BODY_LH			6
#define HERO_BODY_HE			7
#define HERO_BODY_RF			8
#define HERO_BODY_LF			9
#define HERO_BODY_S1			10
#define HERO_BODY_S2			11
#define HERO_BODY_S3			12

#define MAX_HEROBODYPART		13			// 하나의 캐릭터를 조합하기 위한 PART0

//=================================
// 캐릭터의 Animation Speed
//=================================
#define CHAR_SLOWSPEED			0
#define CHAR_NORMALSPEED		1
#define CHAR_FASTSPEED			2
#define CHAR_VERYFASTSPEED		3

#define MAX_ATTACKSPEEDTYPE		4
//=================================


//-----------------------------------------------------------------------------
// CONFIG
//-----------------------------------------------------------------------------
#define MAX_CFGHEADER			9		// Config 파일 헤더 부분의 총 라인 수

#define MAX_NAMECODESTR			5
#define MAX_PARTCODESTR			7

#define MAX_BODYDIR				16		
#define MAX_BODYFRAME			60

#define MAX_FULLPATHNAME		30


//-----------------------------------------------------------------------------
// MAX HERO IN MULTIPLAY
//-----------------------------------------------------------------------------
#define HERO_KM					1
#define HERO_JM					2
#define HERO_CM					3
#define HERO_KW					4
#define HERO_JW					5
#define HERO_CW					6
#define MAX_HERO_KIND_NUM		6		// 총 조합형 주인공 캐릭터 종류 수

#define MAX_HEROID_NUM			100		//mmm 한꺼번에 등장하는 최대 주인공 조합형 캐릭터 수

//============================================================================
// EXEL INFORMATION
//============================================================================

#define MAX_LINESTR				3072	// Excel 파일의 라인의 문자갯수

//------------
// info
#define MAX_ITEMINFO			100
#endif

