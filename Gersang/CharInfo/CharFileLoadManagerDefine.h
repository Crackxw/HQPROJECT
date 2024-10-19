//=============================================================================
//	FileName	:	CharFileLoadManagerDefine.h
//	History		:	2001/10/23(�ڿ���) - Create
//	Discription	:	ĳ���� ���� ������ ������ Define��
//=============================================================================

#ifndef _CHARFILELOADMANAGERDEFINE_H_
#define _CHARFILELOADMANAGERDEFINE_H_

//-----------------------------------------------------------------------------
// �ִ� ��������Ʈ ����
//-----------------------------------------------------------------------------
#define MAX_XSPR				500			// ���ӿ����� 300������ �ϴ� ����Ѵ�.
 
//-----------------------------------------------------------------------------
// ACTION
// ����� ���ΰ��� ĳ������ Ŭ������ �����ϹǷ� ���� Action ������ Set�� �����ؾ��Ѵ�.
//-----------------------------------------------------------------------------

// ���ΰ��� �ൿ����  
#define HERO_ACT_A1				0			// ����
#define HERO_ACT_N1				1			// ���ڸ�
#define HERO_ACT_WL				2			// �ȱ�
#define HERO_ACT_DT				3			// �ױ�

#define MAX_HEROACTIONS			4	


//-----------------------------------------------------------------------------
// WEAPONCLASS
//-----------------------------------------------------------------------------
// ���⿡ ���� �ൿ�� �� �ִ� ���� CLASS
#define HERO_CLASS_1HS			0			// �Ѽ� ����
#define HERO_CLASS_2HS			1			// �μ� ����
#define HERO_CLASS_2HT			2			//???
#define HERO_CLASS_THW			3			//???
#define HERO_CLASS_BOW			4			// Ȱ
#define HERO_CLASS_HTH			5			// �Ǽ�
#define HERO_CLASS_RIF			6			// ��
#define HERO_CLASS_SP1			7			// �⺻ ����
#define HERO_CLASS_SP2			8			// Ư�� ����
#define HERO_CLASS_SP3			9			// Ư�� ����
#define HERO_CLASS_SP4			10			// Ư�� ����
#define HERO_CLASS_SP5			11			// Ư�� ����

#define MAX_HEROWEAPONCLASS		12			// ���Ⱑ �ൿ�Ҽ��ִ� ����

//-----------------------------------------------------------------------------
// BODYPART
//-----------------------------------------------------------------------------
// �ϳ��� ĳ���͸� �����ϱ� ���� PART
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

#define MAX_HEROBODYPART		13			// �ϳ��� ĳ���͸� �����ϱ� ���� PART0

//=================================
// ĳ������ Animation Speed
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
#define MAX_CFGHEADER			9		// Config ���� ��� �κ��� �� ���� ��

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
#define MAX_HERO_KIND_NUM		6		// �� ������ ���ΰ� ĳ���� ���� ��

#define MAX_HEROID_NUM			100		//mmm �Ѳ����� �����ϴ� �ִ� ���ΰ� ������ ĳ���� ��

//============================================================================
// EXEL INFORMATION
//============================================================================

#define MAX_LINESTR				3072	// Excel ������ ������ ���ڰ���

//------------
// info
#define MAX_ITEMINFO			100
#endif

