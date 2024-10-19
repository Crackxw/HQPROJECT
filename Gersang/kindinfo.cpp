//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ĳ���ͺ� ���� ���� ���� Ŭ����
//	File Name		: KindInfo.cpp
//	Birth Date		: 2003. 12. 30.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		Ŭ���̾�Ʈ���� ���Ǵ� ��� ĳ���Ϳ� ���� �ʻ� �� ��ȹ ���� �ʱ�ȭ�ϴ� Ŭ�����Դϴ�.
//		���⿡�� �ش� �ʻ� �� ���� ������ �ε��ǹǷ� �ſ� �߿��� Ŭ�����Դϴ�.
//		���� �ڵ忡���� �ϵ� �ڵ��� ���� ������ �ÿ����� ����� ����ϰ� �ֽ��ϴٸ�,
//		�ε��� �ʿ��� �ֿ� ������ �ڷ� ���Ϸ� ���� �ҽ��� ���Ἲ �� �ڷ��� ����ȭ(Normalization)��
//		�߱��� �� �ֵ��� �۾��� ������ ��ȹ�Դϴ�.
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
// �¶��ΰ��� �ļ� ���
#include "../SCCommon/OnlineCommonParser.h"

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2003-12-30)
// Desc: ���ǵ�
//-----------------------------------------------------------------------------

extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	OnlineBattleInfo	OBInfo;
extern	char 					SavePath[];

#define WORKDELAY(x)			(x) 

// ���ݷ� ���� 
#define APOWER(x)				(x)

// ���� ���� 
#define DPOWER(x)				(x)

// �̵��ӵ� ���� 
#define MOVESPEED(x)			(x)

// ���� ���� 
#define ATTACKRANGE(x)			(x)

// ���� ���� 
#define ATTACKDELAY(x)			(x)

#define PERCENT(x)				(x)

#define EXPVARYPERCENT(x)		(x)

#define BOTTOM(x)				(x)

// ĳ���� �ڵ带 ���� DB�� ����� �� ���Ǵ� ĳ���� ��ȣ�� �ٲ��ش�.
#define		CONVERT_CHARCODE_TO_DBCHARNO( character, number )					( number << 8 | character )
#define		CONVERT_DBCHARNO_TO_CHARCODE( value, getcharacter, getnumber )		( getcharacter = value & 0xff; getnumber = value >> 8 )


// ĳ���� ������ üũ�� - �ڷ� �߰��� ���� ������ �����Ͽ� ������ ���������� �� �κ��� ������ �� ��.
const	DWORD		DF_VALUE_BU		= 6066;				// ũ������ üũ��1
const	DWORD		DF_VALUE_CS		= 4924048;			// ũ������ üũ��2, main.cpp�� DF_VALUE_CS�ʹ� �ٸ� ������,
														// ĳ���͵鿡 ���� üũ���̴�.

// actdoll (2004/03/18 21:29) : �ӽ� ������ ����
SI32	g_siDBCharNo[KIND_ON_MAX] = {0,};

#include <clGame.h>
extern _clGame*				pGame;

/*
// �¶��ΰ� ���� ���� �ڵ� ��ȯ ��Ģ. 
// ��)�������� ĳ���� �ڵ��� KIND_ON_KEON�� �¶��� �󿡼��� 67�� �ȴ�. 
// CharCode.txt ���Ͽ��� �о���δ�.
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
	20000+565,	KIND_ON_GENERAL4_C,			//������
	20000+821,	KIND_ON_GENERAL2_C,			//�̷�
	21000+77,	KIND_ON_GENERAL1_C,			//�������
	21000+333,  KIND_ON_GENERAL5_C,			//����
	21000+589,  KIND_ON_GENERAL3_C,			//�������� 
	19000+21,	KIND_ON_RUNNERHAND,			//����������
	19000+277,  KIND_ON_SANDGOD,			//���� ��
	19000+533,	KIND_ON_SEAENVOY,			//�ٴ��� ���
	19000+789,	KIND_ON_VIPER,				//����.
	20000+45,   KIND_ON_AMAKUSA,			//�Ƹ����.
	20000+301,  KIND_ON_LAVAMONSTER,		//��ϱ���.
	20000+557,  KIND_ON_EVIL,				//�̻��� ���.
	20000+813,	KIND_ON_BIRD,				//�θ���.
	21000+69,	KIND_ON_WEASEL,				//������.
	21000+581,	KIND_ON_COBRA,				//�ں��.
	21000+325,	KIND_ON_SOUL,				//¡�⽺ĭ�� ȥ.
	21000+837,	KIND_ON_VOLCANOFLAME,		//ȭ���� �Ҳ�.
	22000+93,   KIND_ON_REDMADBULL,			//��������.			
	22000+349,	KIND_ON_MAGICOFFICER,		//������.
	22000+605,  KIND_ON_CAPBEAR,			//����ݴް�.
	22000+861,  KIND_ON_KOREANGHOST,		//ó��ͽ�.
	23000+117,  KIND_ON_SKELETON,			//�ذ�.
	23000+373,  KIND_ON_SKELETONSOLDIER,	//�ذ񺴻�.
	23000+629,  KIND_ON_PIRATECAPTAIN,		//��������.
	23000+885,	KIND_ON_TAIWANGHOST,		//�븸�ͽ�(��鹫��).
	24000+141,	KIND_ON_JAPANGHOST,			//�Ϻ��ͽ�(���ͽ�).
	24000+397,	KIND_ON_JUCK,				//������.
	24000+653,	KIND_ON_FEMALESHAMAN,		//��������.
	24000+909,  KIND_ON_QUEEN,				//��������.
	25000+165,  KIND_ON_AUTA,               //�ƿ�Ÿ.
	21000+845,  KIND_ON_DONKEY,				//�糪��.
	22000+101,  KIND_ON_BUTCHER,			//Į����.
	22000+357,  KIND_GENERAL1K,				//����.
	22000+613,  KIND_ON_DSWORD,				//�⸶����.
	22000+869,  KIND_GENERAL1J,				//��Ͻ�.
	23000+637,	KIND_ON_BEASTWARRIOR,		//�߼�����.
	23000+893,	KIND_ON_GENERAL6T,			//����â.
	23000+125,  KIND_ON_MAGIC_C,			//������.
	23000+381,  KIND_ON_GENERAL6_C,			//����.
	25000+421,	KIND_ON_AGILETIGER,			//��ȣ.
	25000+677,	KIND_ON_PAWNTIGER,			//����ȣ����.
	27000+213,	KIND_ON_OWL,				//�û���
	27000+469,	KIND_ON_LEOPARD,			//ǥ��
	28000+237,	KIND_ON_HUNSWARRIOR,		//�������
	27000+981,	KIND_ON_HUNSHORSE,			//���⸶��
	27000+725,	KIND_ON_HUNSHORSEARCHER,	//���⸶�ü�.
	25000+933,	KIND_ON_GATEKEEPER,			//������
	26000+445,	KIND_ON_CLAYFIGURE,			//�⸶������
	26000+701,	KIND_ON_PHOENIX,			//�һ���
	26000+957,	KIND_ON_JINEMPEROR,			//����Ȳ��ȥ
	24000+149,	KIND_ON_MEDICALMAN,			//�Ǽ���.
	24000+405,	KIND_ON_HEOJUN,				//����.
	24000+661,	KIND_ON_COSMIC,				//�����.
	24000+917,  KIND_ON_SEIMEI,				//���̸���.
	25000+173,  KIND_ON_YUMRUCK,			//���»�.
	25000+429,  KIND_ON_JANGSUNHWA,			//�弱ȭ.
	25000+685,  KIND_ON_LONGSPEAR,			//��â����.
	25000+941,	KIND_ON_KOAKHU,				//����.	 
	28000+493,  KIND_ON_SNOWMAN1,			//�����1
	28000+749,  KIND_ON_SNOWMAN2,           //�����2
	29000+5,	KIND_ON_SANTA1,				//��¥��Ÿ1
	29000+261,  KIND_ON_SANTA2,				//��¥��Ÿ2
	29000+517,  KIND_ON_SANTA3,				//��¥��Ÿ3
	29000+773,  KIND_ON_MOTHER_DEER1,		//�����絹��1
	30000+29,	KIND_ON_MOTHER_DEER2,		//�����絹��2
	30000+285,	KIND_ON_MOTHER_DEER3,		//�����絹��3
	30000+541,	KIND_ON_MOTHER_DEER4,		//�����絹��4
	30000+797,	KIND_ON_SANTA4,				//��¥��Ÿ4
	31000+53,	KIND_ON_SANTA_TW,			//�븸 ��¥ ��Ÿ.
	31000+309,	KIND_ON_MOTHER_DEER_TW,		//�븸 ��¥ �絹��.
	31000+565,  KIND_ON_FLYDRAGON,			//�ͷ�
	31000+821,	KIND_ON_RAPTOR,				//����
	32000+77,   KIND_ON_TIRANO,				//Ƽ���	
	32000+333,	KIND_ON_MONKEY,				//��ȯ������.

	// actdoll (2004/02/26 11:12) : �߰��� ���� - ����� �ʿ�
	32589,		KIND_ON_HELLKING,								//250		// ������
	32845,		KIND_ON_HELLMESSENGER,							//251		// ���»���
	33101,		KIND_ON_GHOSTSAMURAI,							//252		// ���� �繫����
	33357,		KIND_ON_PYTHON,									//253		// �̹���
	33613,		KIND_ON_GHOSTELEPHANT,							//254		// ���� �ڳ���
	33869,		KING_ON_GHOSTSEAKING,							//255		// ���� ������
	34125,		KIND_ON_GHOSTPIRATECAPTAIN,						//256		// ���� ��������
	34381,		KIND_ON_GHOSTSKELETON,							//257		// ���� �ذ�
	34637,		KIND_ON_GHOSTSKELETONSOLDIER,					//258		// ���� �ذ񺴻�
	34893,		KIND_ON_GHOSTFEMALESHAMAN,						//259		// ���� ����
	35149,		KIND_ON_GHOSTVOLCANOFLAME,						//260		// ���� �Ҳ�

	35405,		KIND_ON_GUARDIAN_MOUSE,							//261		// ��ȣ��(��)
	35661,		KIND_ON_GUARDIAN_CAW1,							//262		// ��ȣ��(��1)
	35917,		KIND_ON_GUARDIAN_CAW2,							//263		// ��ȣ��(��2)
	36173,		KIND_ON_GUARDIAN_CAW3,							//264		// ��ȣ��(��3)

	36429,		KIND_ON_GUARDIAN_CHICK,							//265		// ��ȣ��(���Ƹ�)
	36685,		KIND_ON_GUARDIAN_PIG,							//266		// ��ȣ��(����)
	36941,		KIND_ON_GUARDIAN_SNAKE,							//267		// ��ȣ��(��)
	37197,		KIND_ON_GUARDIAN_MONKEY,						//268		// ��ȣ��(������)

    37453,		KIND_ON_JAP_INVADERS,							//269		// ħ���ֱ�(����)

//	��� 2�� ����(2004-5-11, robypark)
	26197,		KIND_ON_TURTLE_VEHICLE,							// 270	U102	����-�ź���
	26453,		KIND_ON_THUNDER_VEHICLE,						// 271	U103	����-������
	26709,		KIND_ON_QUAKE_VEHICLE,							// 272	U104	�Ϻ�-������
	26965,		KIND_ON_BLACK_DRAGON_VEHICLE,					// 273	U105	�Ϻ�-�����
	27221,		KIND_ON_CATAPULT,								// 274	U106	�߱�-�߼���
	27477,		KIND_ON_FARANGI_VEHICLE,						// 275	U107	�߱�-�Ҷ�������
	27733,		KIND_ON_FIRE_DRAGON_VEHICLE,					// 276	U108	�븸-ȭ����
	27989,		KIND_ON_CHINESEPHOENIX,							// 277	U109	�븸-��Ȳ����

	-1
};
*/
#ifdef _USE_HASHCODE_EFFECTSOUND
#define ENUM_TO_STRING(x) #x
#endif

// robypark 2004/9/23 14:50
// ĳ������ �� �� ������ HQ�� CharInfo.txt���Ͽ��� �о������ �����Ѵ�.
void _KindInfo::Init(char *namecode,	// ĳ���� ID
					int kindatb,		// ĳ���� �Ӽ�(���ΰ�(���տ� ĳ����), ���, �Ϲ�(0))
					DWORD atb,			// ĳ���� �Ӽ�1
					DWORD atb2,			// ĳ���� �Ӽ�2 Skill
					DWORD atb3,			// ĳ���� �Ӽ�3 Skill
					DWORD atb4,			// ĳ���� �Ӽ�4 Skill
					DWORD atb5,			// ĳ���� �Ӽ�5 Skill
					DWORD dyingatb,		// ��� �Ӽ�
					SI32 attackatb1,	// ���� �Ӽ�1
					SI32 attackatb2,	// ���� �Ӽ�2
					DWORD defenceatb,	// �� Ÿ��
					char* name,			// ���� �߿� ���콺 �ؿ� �ִ� ĳ���� �̸�
					char *idname,		// ������ �ʴ´�.
					SHORT defaultarmor/* = -1*/,	// �⺻ ��
					SHORT defaulthelmet/* = -1*/,	// �⺻ ���
					SHORT defaultweapon/* = -1*/,	// �⺻ ����
					SHORT heroid/*= 0*/,		// ���ΰ� ����ĳ���� ID. �� �ܿ��� 0
					ST_KI_SUBDATA	*pSubData/*	= NULL*/,
					SI08 magicapplyrate/*= 70*/)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharInfo.txt";		// ��θ� HQ/Online/CharInfo/�� ����. robypark 2004/10/26 17:52

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "KindInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
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
		// �ϴ� ������� ã�� ������ ��ġ���� ������ ���� ���� ���� ����.
		if ( stricmp(pszCharacterID, namecode) != 0)
		{
			continue;
		}
		// �Ѿ�Դٸ� ã�� ���̴�.
		//----------------------------------------------------------------------------

		ocp.GetValue( pszTemp,					127,	iLineNo, iColNo );		// ĳ���� ��.
		ocp.GetValue( siNameCode,						iLineNo, iColNo );		// ĳ���� �̸� �ڵ�. OnlineText�� �̿��Ѵ�. ��, �� �Լ� ȣ��� �Ѿ���� ĳ���͸�(���ڿ�)�� �켱�� �Ѵ�. robypark 2004/12/21 22:00
		ocp.GetValue( siPortraitInventory,				iLineNo, iColNo );		// ĳ���� �ʻ� �κ��丮��
		ocp.GetValue( siPortraitMercenary,				iLineNo, iColNo );		// ĳ���� �ʻ� �뺴â��
		ocp.GetValue( siDrawBottomX,					iLineNo, iColNo );		// ĳ���� �׷����� ��ġ
		ocp.GetValue( siPossessTileX,					iLineNo, iColNo );		// ���� �ʿ��� ĳ���Ͱ� �����ϴ� Ÿ�� ��
		ocp.GetValue( siPossessTileY,					iLineNo, iColNo );		// ���� �ʿ��� ĳ���Ͱ� �����ϴ� Ÿ�� ��
		ocp.GetValue( siSelectBoxWidth,					iLineNo, iColNo );		// ���� �߿� ĳ���� ���� �ڽ� Width
		ocp.GetValue( siSelectBoxHeight,				iLineNo, iColNo );		// ���� �߿� ĳ���� ���� �ڽ� Height
		ocp.GetValue( pszNation,				 31,	iLineNo, iColNo );		// ĳ���� ������
		ocp.GetValue( siMoveSpeed,						iLineNo, iColNo );		// ĳ���� �̵��ӵ�
		ocp.GetValue( siExperience_Vary_Percent,		iLineNo, iColNo );		// ���� �ʿ��� ĳ���Ͱ� �����ϴ� Ÿ�� ��
		ocp.GetValue( siAttack_range_1,					iLineNo, iColNo );		// ����1 �����Ÿ�
		ocp.GetValue( siAttack_delay_1,					iLineNo, iColNo );		// ����1 ���� �����ð�
		ocp.GetValue( siMin_attack_range_1,				iLineNo, iColNo );		// ����1 �ּ� �����Ÿ�
		ocp.GetValue( pszEFFECT_SND_ATTACK_1,	127,	iLineNo, iColNo );		// ����1 ȿ����
		ocp.GetValue( siAttack_range_2,					iLineNo, iColNo );		// ����1 �����Ÿ�
		ocp.GetValue( siAttack_delay_2,					iLineNo, iColNo );		// ����1 ���� �����ð�
		ocp.GetValue( siMin_attack_range_2,				iLineNo, iColNo );		// ����1 �ּ� �����Ÿ�
		ocp.GetValue( pszEFFECT_SND_ATTACK_2,	127,	iLineNo, iColNo );		// ����1 ȿ����

		ocp.GetValue( siSearch_range,					iLineNo, iColNo );		// �þ�
		ocp.GetValue( siMax_fire_repeat_number,			iLineNo, iColNo );		// �ִ� ���� �߻� �Ѿ� ����
		ocp.GetValue( siFire_repeat_delay,				iLineNo, iColNo );		// ���� �߻� ������
		ocp.GetValue( siWork_delay,						iLineNo, iColNo );		// �ൿ �����ð�
		ocp.GetValue( siDead_delay,						iLineNo, iColNo );		// ��� �����ð�
		ocp.GetValue( siTower_delay,					iLineNo, iColNo );		// ???
		ocp.GetValue( siChange_direction_delay,			iLineNo, iColNo );		// ���� ��ȯ �����ð�
		ocp.GetValue( siAttack_give_up_delay,			iLineNo, iColNo );		// �������� ������
		ocp.GetValue( pszEFFECT_SND_TRY_ATTACK,	127,	iLineNo, iColNo );		// ĳ���� ���ݽõ� ȿ����
		ocp.GetValue( pszEFFECT_SND_SELECT,		127,	iLineNo, iColNo );		// ĳ���� ���� ȿ����
		ocp.GetValue( pszEFFECT_SND_DYING,		127,	iLineNo, iColNo );		// ĳ���� ��� ȿ����
		ocp.GetValue( pszEFFECT_SND_MOVE,		127,	iLineNo, iColNo );		// ĳ���� �̵���� ȿ����
		ocp.GetValue( pszEFFECT_SND_ATTACK,		127,	iLineNo, iColNo );		// ĳ���� ���� ȿ����
		ocp.GetValue( siVibrate_switch,					iLineNo, iColNo );		// ���� ������ ��� ��鸲 ȿ�� ����
		ocp.GetValue( siPriority_Attack_Type,			iLineNo, iColNo );		// robypark 2004/10/18 12:14 - �� ���� ���ݼӼ��� ���� ��� �ΰ����� ��밪

		fclose( fp );
		ocp.Release();

		m_ssOnlinePortraitFont	= siPortraitInventory;
		m_ssOnlineMercenaryFont	= siPortraitMercenary;
		m_ssBottomIndent		= siDrawBottomX;		// ���� �� ĳ���� ���� ������ �ٴ� ��ġ, ĳ���Ͱ� �׷����� ���� ����� y��ǥ
		m_ssIDXsize				= siPossessTileX;		// ���� �ʿ��� �����ϴ� Ÿ�� ��
		m_ssIDYsize				= siPossessTileY;		// ���� �ʿ��� �����ϴ� Ÿ�� ��
		m_ssXsizePercent		= siSelectBoxWidth;		// ���� �� ĳ���� ���� ������ ���� ũ��
		m_ssYsizePercent		= siSelectBoxHeight;	// ���� �� ĳ���� ���� ������ ���� ũ��
		
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
		// �� ���� ���� �Ӽ� ���� ĳ���� �ΰ����� ���� ��
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
		// ĳ���͸��� GameText.dat�� �ƴ϶� OnlineText.dat�� ���� �̿��ϵ��� ����.
		// ������ �ߺ��Ǵ� �������̴�.
		// �Լ� ���ڷ� �̸� ���� ��(name)�� NULL�� �ƴ϶�� �׸� �����Ѵ�.
		if (NULL == name)
		{
			// CharacterList.txt�� Name Code�� ���� ���� �̿��Ѵ�.
			m_pszName					= pGame->pOnlineWorld->pOnlineText->Get(siNameCode);
		}
		else	// GameText.dat�� ĳ���� ���� �̿��Ѵ�.
			m_pszName					= name;

		m_pszIDName					= idname;
	
		m_ssHeroID					= heroid;
		m_ssDefaultArmor			= defaultarmor;
		m_ssDefaultHelmet			= defaulthelmet;
		m_ssDefaultWeapon			= defaultweapon;

		// actdoll (2004/03/06 3:21) : ���� ������ �߰�
		if( !pSubData )			ZeroMemory( &m_SubData, sizeof(ST_KI_SUBDATA) );
		else					CopyMemory( &m_SubData, pSubData, sizeof(ST_KI_SUBDATA) );

		m_scMagicApplyRate		= magicapplyrate;

		m_nMinAttackRange[0] = siMin_attack_range_1;		// ĳ���Ͱ� ���� ������ �ּҰŸ�(�⺻ ������ 1) ù ��° ���� �Ӽ�
		m_nMinAttackRange[1] = siMin_attack_range_2;		// ĳ���Ͱ� ���� ������ �ּҰŸ�(�⺻ ������ 1) �� ��° ���� �Ӽ�
	
		m_uiUnique					= GetUnique();
		return;
	}

	fclose( fp );
	ocp.Release();

#ifdef _DEBUG
	clGrp.Error("Error: KindInfo.cpp Init()", "KindInfo [%s] : Not registered.", namecode);
#else
	clGrp.Error("Character Information Setting Error", "�Ż� Ŭ���̾�Ʈ�� ������ �ǽɵ˴ϴ�. ��ġ�� �Ż� Ŭ���̾�Ʈ�� Ȯ�����ּ���.\nGersang(Kyosyoden) Client doubt modified. Please Check your client.");
#endif
}

//-----------------------------------------------------------------------------
// Name: Init()
// Code: actdoll (2003-12-30)
// Desc: �ش� ĳ���ͺ� ������ �����ϴ� �κ��̴�.
//		���� ������ ���� ���� �κ��̴�.
//		namecode										- �� ĳ���Ϳ� �־��� KIND_OF�� �ε���
//		kindatb											- ?
//		movespeed										- �̵� �ӵ�
//		expvarypercent									- EXP ��ȭ�� �ۼ�Ʈ
//		attackrange1, attackrange2						- ���� ����
//		searchrange										- �˻� ����
//		attackdelay1, attackdelay2						- ���� ���� �ð�
//		maxfirerepeatnumber, firerepeatdelay			- �ִ� ȭ�� ��� Ƚ�� �� �ð�
//		workdelay, deaddelay, towerdelay				- �����̵�? 
//		changedirectiondelay, attackgiveupdelay			- �����̵�?
//		atb, atb2, atb3, atb4							- ĳ������ �Ӽ���
//		dyingatb										- ����� ĳ������ �Ӽ���
//		attackatb1, attackatb2							- ���� �Ӽ�
//		defenceatb										- ��� �Ӽ�
//		vibrateswitch									- ����(?)
//		tryattackeffect, selecteffect, dyingeffect,	moveeffect, attackeffect	- ����
//		( ���� ��� ������� ��, ĳ���� ���ý�, ���� ��, �̵� ��ɽ�, ���� ������ )
//		name											- �� ĳ������ ��Ī
//		idname											- �Ⱦ��� ������ �Ǵܵ�
//		heroid, defaultArmor, defaultHelmet, defaultWeapon	- �Ⱦ��� �͵� ����.
//		pSubData										- ���������� �ʿ��� �����͵��� �޴´�. �⺻���� NULL�̴�
//		magicapplyrate									- 70���� �����Ǿ� �ִ�.
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

	// savepath�� ����Ͽ� ȭ���̸��� �����Ѵ�. 
//	GetFileNamePath(, SavePath, buffer);
	if ( (fp = BindJXFile::GetInstance()->GetFile( "CharInfo.txt" )) == NULL )
	{
		clGrp.Error("File Open Error", "CharInfo" );
		return ;
	}
	
	BOOL ExitSwitch = FALSE;

	// �ش� ���Ͽ��� ���� �ϳ��� ���´�.
	while( fgets(buffer, 1024, fp) != NULL)
	{
		// actdoll : �ش� ���ۿ� ��ŵ �÷��װ� ���� ��� �ϴ� �����̴�.
		if( buffer[0] == '/' || buffer[0] == '\n' || buffer[0] == '\r' )	continue;

		// actdoll (2003/12/30 18:38) : 
		//	���ӿ��� ���� ID / ĳ���� ���Ǹ�(NOUSE) / �����̸�(NOUSE) / �ʻ���Ʈ-�Ϲ� / �ʻ���Ʈ-�뺴 / bottom / xsize / ysize / xpercent / ypercent / nation
		sscanf(buffer, "%d %s %s %d %d %d %d %d %d %d %s", 
			&gameID, code, temp, &portrait, &mercenary, &bottom, &idxsize, &idysize, &xpercent, &ypercent, nation);

//		gameID		= CLASS_KIND_ON_CHARACTER::GetData( code );
//		if( gameID != KIND_ON_INVALID )
		if(strcmp(code, namecode) == 0)
		{
			ExitSwitch				= TRUE;
			m_ssOnlinePortraitFont	= portrait;
			m_ssOnlineMercenaryFont	= mercenary;	// actdoll (2003/12/30 18:47) : �ڷ� �߰� - �뺴 �ʻ� �����Ͱ� �����ƴ�.
			m_ssBottomIndent			= bottom;		// ���� �� ĳ���� ���� ������ �ٴ� ��ġ, ĳ���Ͱ� �׷����� ���� ����� y��ǥ
			m_ssIDXsize					= idxsize;
			m_ssIDYsize					= idysize;
			m_ssXsizePercent			= xpercent;		// ���� �� ĳ���� ���� ������ ���� ũ��
			m_ssYsizePercent			= ypercent;		// ���� �� ĳ���� ���� ������ ���� ũ��
			
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

	// actdoll (2004/03/06 3:21) : ���� ������ �߰�
	if( !pSubData )			ZeroMemory( &m_SubData, sizeof(ST_KI_SUBDATA) );
	else					CopyMemory( &m_SubData, pSubData, sizeof(ST_KI_SUBDATA) );

	m_scMagicApplyRate		= magicapplyrate;

	// robypark 2004-5-24, 15:11: �������� �Ұ����� ĳ������ �ּ� ��� ���������� ���� ������ �߰�
	m_nMinAttackRange[0] = 1;		// ĳ���Ͱ� ���� ������ �ּҰŸ�(�⺻ ������ 1) ù ��° ���� �Ӽ�
	m_nMinAttackRange[1] = 1;		// ĳ���Ͱ� ���� ������ �ּҰŸ�(�⺻ ������ 1) �� ��° ���� �Ӽ�
	
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

	unique += m_uiAtb5;	// robypark 2004/10/1 17:30 �߰� ����� ���� �߰�

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

	// robypark 2004-5-24, 15:12: �������� �Ұ����� ĳ������ �ּ� ��� ���������� ���� ������ �߰�
	unique += m_nMinAttackRange[0];
	unique += m_nMinAttackRange[1];

	// robypark 2004/10/18 12:27
	// �켱 ���� �Ӽ��� ���� �ΰ����� ���� ��
	unique += m_ssPriority_Attack_Type;

	return unique;
}


// �������� �߽� ��ǥ�� �������� ĳ���͸� ����ش�. 
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
		CHAR	defaultBody[5];								// ĳ������ ����Ʈ ��(��: KMN)
		
		aniAction			= animation;
		
		wclass				= HERO_CLASS_HTH;
		weaponEnum			= CHAR_NORMALSPEED;
		
		//-----------------------------
		// ����Ʈ �� �ڵ带 �������� �� ���ΰ� ����
		switch( m_ssNation )
		{
		case COUNTRY_KOREA:		strcpy(defaultBody, "KD01");		break;
		case COUNTRY_JAPAN:		strcpy(defaultBody, "JD01");		break;
		case COUNTRY_CHINA:		strcpy(defaultBody, "CD01");		break;
		case COUNTRY_TAIWAN:	strcpy(defaultBody, "TD01");		break;
		default:				clGrp.Error("No Default Body.", "%d", m_ssNation); break;
		}
		
		// ���� ���� ����
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
		// �� ������ ù ��Ʈ ��ȣ
		font	= ((Frame/10) % frame);
		
		// ȭ�鿡 ���� Body�� ������ ���´�.
		siTotalPiece	= pWorld->pCharFileLoadManager->m_HeroConfig[m_ssHeroID].siTotalPiece[aniAction][wclass];
		
		bExist = FALSE;
		for( siCountA=0; siCountA < siTotalPiece; siCountA++ )
		{
			// ���� Bodypart�� ���´�.
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
		// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
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
// Desc: CharCode������ ���ؼ� �ڵ� ������ �����Ѵ�.
//-----------------------------------------------------------------------------
BOOL	Parse_CharCode()
{
	////////////////////////////////////////////////////////////
	// robypark 2004/10/2 11:22
	// HQ�� CharCode.txt���� �����͸� ���� ���.
	// ���ο� ĳ���͸� �߰��� ���� �����ε��� ���� ������ �� ���.
	// InitKindInfo() �Լ����� �ʱ�ȭ�� �ش�.
	// ������ �����ϳ� �¶��� ���� �ļ��� �̿��ϵ��� ����.
	////////////////////////////////////////////////////////////

	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharCode.txt";		// ��θ� HQ/Online/CharInfo/�� ����. robypark 2004/10/26 17:52

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Character Code Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		SI32	siCharGameCode, siDBIndexNum;
		char	pszEnumCharacterID[48], pszServerCharacterID[48], pszNation[32], pszTemp[128];

		ocp.GetValue( siCharGameCode,				iLineNo, iColNo );		// Index(Meaningless)
		ocp.GetValue( pszEnumCharacterID,	47,		iLineNo, iColNo );		// Enum(Constant) Character ID
		ocp.GetValue( pszServerCharacterID,	47,		iLineNo, iColNo );		// Server Character ID
		ocp.GetValue( pszTemp,				127,	iLineNo, iColNo );		// ĳ���� ��.
		ocp.GetValue( pszNation,			31,		iLineNo, iColNo );		// Nation

		// �ε��� �ڵ��� ��Ʈ���� ���Ѵ�.
		siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszEnumCharacterID );
		if( siCharGameCode == KIND_ON_INVALID )
		{
			fclose( fp );
			ocp.Release();
			clGrp.Error( "Parse_CharCode Error", "%s", pszEnumCharacterID );
			return FALSE;
		}

		char chDBIndexChar = pszServerCharacterID[0];

		// ���ĺ� ���ڰ� �ƴ� ��� �˻�
		if (isalpha(chDBIndexChar) == 0) 
		{
			// DB�ڵ� ����
			if( chDBIndexChar == '@' )// �� �ڵ尡 �����ִٸ� �̳��� DB������ ���� ���̴�.
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
			// ���� ĳ���� �ε����� ��ȯ
			char *szDBIndexNum	= pszServerCharacterID + 1;
			siDBIndexNum		= atoi(szDBIndexNum);

			chDBIndexChar					= toupper( chDBIndexChar );		// �ϴ� �ҹ��������� �𸣴� �빮�ڷ� Ȯ�λ��
			KI[siCharGameCode].m_siServerID	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
		}

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;

/*
	FILE	*fp;
	// �Ľ��� ������ ���´�.
	if ( (fp = BindJXFile::GetInstance()->GetFile( "CharCode.txt" )) == NULL )
	{
		clGrp.Error( "File Open Error", "CharCode" );
		return FALSE;
	}

	// �ش� ���Ͽ��� ���� �ϳ��� ���´�.
	char	buffer[1024];
	while( fgets(buffer, 1024, fp) != NULL)
	{
		// actdoll : �ش� ���ۿ� ��ŵ �÷��װ� ���� ��� �ϴ� �����̴�.
		if( buffer[0] == '\t' )	// �̰� �ش� �ٿ� ���۴� ������ �۾��� �ƹ��͵� �������� üũ
		{
			if( buffer[1] == '/' || buffer[1] == '\n' || buffer[1] == '\r' || buffer[1] == '\t' )	continue;
		}
		else					// �̰� �ٹٲ��̳� �ּ��� üũ
		{
			if( buffer[0] == '/' || buffer[0] == '\n' || buffer[0] == '\r' )						continue;
		}

		// actdoll (2003/12/30 18:38) : ���ϴ� ������ ���´�.
		SI32	siCharGameCode, siDBIndexNum;
		char	pszCharCodeStr[128], pszCharNameStr[128], pszNation[32];
		char	chDBIndexChar;
		
		//	�ش� ĳ���Ϳ� ���ǵ� ���� �ε���	/ ĳ���� �ڵ��	/ �ش� ĳ���Ϳ� / ���ǵ� DB �ε���	/ ĳ���� �̸�(������)	/ ����
		sscanf(buffer, "%d %s %c%d %s %s", 
			&siCharGameCode, pszCharCodeStr, &chDBIndexChar, &siDBIndexNum, pszCharNameStr, pszNation );

		// �ε��� �ڵ��� ��Ʈ���� ���Ѵ�.
		siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszCharCodeStr );
		if( siCharGameCode == KIND_ON_INVALID )
		{
			clGrp.Error( "Parse_CharCode Error", "%s",pszCharCodeStr );
			return FALSE;
		}

		// DB�ڵ� ����
		if( chDBIndexChar == '@' )// �� �ڵ尡 �����ִٸ� �̳��� DB������ ���� ���̴�.
		{
			KI[siCharGameCode].m_siServerID	= 0;
//			g_siDBCharNo[siCharGameCode]	= 0;
		}
		else
		{
			chDBIndexChar					= toupper( chDBIndexChar );		// �ϴ� �ҹ��������� �𸣴� �빮�ڷ� Ȯ�λ��
			KI[siCharGameCode].m_siServerID	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
//			g_siDBCharNo[siCharGameCode]	= CONVERT_CHARCODE_TO_DBCHARNO( chDBIndexChar, siDBIndexNum );
		}
	}

	fclose(fp);
	
	return TRUE;
*/
}

// robypark 2004/10/5 14:26
// ĳ������ �ִϸ��̼� ���� ������ �Ľ��Ͽ� �����Ѵ�..
// HQ�� CharAniInfo.txt���Ͽ��� ���� �����Ѵ�.
// �¶��� ���� �ļ��� �̿�.
// InitKindInfo() �Լ����� ȣ���Ͽ� ���ȴ�.
BOOL Parse_CharacterAnimationInformation(void)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "Online\\CharInfo\\CharAniInfo.txt";

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );		// ��θ� HQ/Online/CharInfo/�� ����. robypark 2004/10/26 17:52

	if ( !fp )
		return FALSE;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Character Animation Information Setting Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		char	pszCharacterKindID[64],	pszCharacterName[128], pszResourceIndex[128];
		SI32	siAnimationIndex, siIsMirroring, siStartFontNumber, 
				siFontNumberEachDirection, siInstant, siDelay, siAttribute;

		ocp.GetValue( pszCharacterKindID,		63,		iLineNo, iColNo );		// Enum(Constant) Character ID
		ocp.GetValue( pszCharacterName,			127,	iLineNo, iColNo );		// ĳ���� ��(������)
		ocp.GetValue( siAnimationIndex,					iLineNo, iColNo );		// �ִϸ��̼� Ÿ�� �ε���
		ocp.GetValue( siIsMirroring,					iLineNo, iColNo );		// �̹����� �̷��� �����Ͽ� ����ϴ°�
		ocp.GetValue( pszResourceIndex,			127,	iLineNo, iColNo );		// ��������Ʈ �̹��� ���ҽ� ���̵�(�ε���)
		ocp.GetValue( siStartFontNumber,				iLineNo, iColNo );		// �ִϸ��̼� ������Ʈ ��ȣ
		ocp.GetValue( siFontNumberEachDirection,		iLineNo, iColNo );		// �� ���⿡ �ش�Ǵ� ĳ���� �ִϸ��̼� ��Ʈ ��
		ocp.GetValue( siInstant,						iLineNo, iColNo );		// ĳ���� ���� ���� ��Ʈ ��ȣ
		ocp.GetValue( siDelay,							iLineNo, iColNo );		// �ִϸ��̼� ������
		ocp.GetValue( siAttribute,						iLineNo, iColNo );		// �ִϸ��̼� �Ӽ�

		// �ε��� �ڵ��� ��Ʈ���� ���� ��밡���� �ε���(����) ���� ��� �´�.
		SI32 siCharGameCode	= CLASS_KIND_ON_CHARACTER::GetData( pszCharacterKindID );

		if( siCharGameCode == KIND_ON_INVALID )
		{
			fclose( fp );
			ocp.Release();
			clGrp.Error( "Parse_CharacterAnimationInformation Error", "Line:%d Kind:%s", iLineNo, pszCharacterKindID );
			return FALSE;
		}

		// �ִϸ��̼� ����
		KI[siCharGameCode].SetFile(siAnimationIndex, siIsMirroring, pszResourceIndex, siStartFontNumber, siFontNumberEachDirection, siInstant, siDelay, siAttribute);

		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
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

	// ĳ���� �ڵ� ���� �Ľ�
	Parse_CharCode();

	//////////////////////////////////////////////////////////////////
	// robypark 2004/10/1 3:40
	// HQ�� CharInfo.txt���� �����͸� ���� ����ϵ��� ����.
	// �¶��� ���� �ļ��� �̿��Ͽ� �ۼ��Ǿ���.
	// ���ο� ĳ���͸� �߰��� ������ Enum_KI_Character.h���� 
	// �ش� ĳ���� ������ ���� Kind ������� �����Ͽ��ָ�, 
	// Enum_KI_Character.cpp�� ���� �߰��� �ش�. InitSmokeKindInfo()
	// �Լ����� �ʱ�ȭ �κ��� �߰��� �ش�. ���� CharInfo.txt��
	// �� �߰����ش�. CharCode.txt���� ĳ���� ���̵� �������־��
	// �Ѵ�.
	//////////////////////////////////////////////////////////////////

	// �ĺ��� 
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


	// �ٴٰ�
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

	// �͹� 
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

	// ���� ����
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

	// �������(Upgrade)
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

	// �⸶�ü�
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

	// ��ޱ⸶�ü�
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

	// �븸 ������. 
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

	//�ּ���
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


	// �Ϻ� �𸶻�.
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

	// ����𸶻�.(Upgrade)
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

	// �븸 �����Ѽ�
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

	// ���볶�� 
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

	// ��޴��볶�� (Upgrade)
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

	// ����
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

	// ��޴���(Upgrade)
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
	
	// �븸 ������.
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

	// �븸 ��������
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
	
	//����ּ���
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
	
	// ���� ���屺��
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

	// ��޵�������
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

	// ��޺�����. 
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

	// �Ǵ����� 
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

	// �Ϻ�����
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
	
	// �븸����
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
	
	// �븸 ��޿����� 
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

	// �����θ�. 
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

	// ��ġ����
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
	
	// �Ѽ����� 
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

	// �������� 
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

	// ���� 
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
	
	// ��� 
	// ���� �̼��� 
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
	
	// ���� ������ 
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

	//���� ����� 
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
	
	// ���� �Ÿ�  
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

	// ���� ����� 
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

	// ���� ��ù�  
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

	// �Ϻ� ����
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
	
	// �Ϻ� ��Ű��ī 
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
	
	// �Ϻ� ���̼��(������ �ݰ�)
	KI[KIND_GENERAL4J].Init ("KIND_GENERAL4J",
		KINDATB_GENERAL, 
		ATB_ATTACK|ATB_ATTACKAIR|ATB_PATIENT,
		ATB2_HEAL, 
		ATB3_EARTHQUAKE,
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_MAGIC2, ATTACKATB_ELECTRON/*�� ���� �Ӽ��� �۵��ؼ��� �ʵǸ� �۵����� �ʴ´�. ��, �Ʒ��� �ִϸ��̼ǿ� ANI_ATTACK2�� �����ؼ��� �ȵȴ�. �� �Ӽ��� ���� ���̼���� AI���� �ӵ��� ������ �ϱ� ���Ͽ� ���ȴ�.*/, 
		DEFENCEATB_HUMAN,
		NULL, Text.General4JIDName.Get());
	
	// �Ϻ� ���� 
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
	
	// �Ϻ� ������ �̿��߽�
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
	
	// �Ϻ� �̽ô� �������� 
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
	
	// �Ϻ� �ƻ��� 
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
	// �¶��� ĳ����
	// �¶��� - ��ö��.
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
	
	// �¶��� - �̸�ȭ
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

	// �¶��� - �ø��� ��
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

	// �¶��� - �ճ� �ϳ�����
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
	
	// �¶��� - �ռҷ�
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
	
	// �¶��� - �չ̷�
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

	// �¶��� - ��ȣ�� 
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

	// �¶��� - ����� 
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

	// �¶��� - �ı�
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
	
	// �¶��� - �����
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
	
	// �¶��� - �Ʊ�
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
	
	// �¶��� - ȣ����
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

	// �¶��� - ��ȣ
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
	
	// �¶��� - �߽�
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
	
	// �¶��� - ��õ������
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
	
	// �¶��� - ��������
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

	// �¶��� - ����
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
	
	// �¶��� - ö�����
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
	
	// �¶��� - �ü�����
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

	// �¶��� - ����
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
	
	// �¶��� - ���ٱ�
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

	// �¶��� - ����
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
	
	// �¶��� - ������
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

	// �¶��� - ���Ϸ��
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
	
	// �¶��� - ����ȣ
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
	
	// �¶��� - �Ұ��縮
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

	// �¶��� - ��1
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
	
	// �¶��� - ����
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
		
	// �¶��� - ����
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
	
	// �¶��� - �а���
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
		
	// �¶��� - ȭ��
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
	
	// �¶��� - Ȳ��
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
	
	// �¶��� - ����
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
		
	// �¶��� - ���β���
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
		
	// �¶��� - ������
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

	// �¶��� - �ξ�ô�
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
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� START
//----------------------------------------------------------------------------

	// �¶��� - ���� - �ϼ�
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
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� END
//----------------------------------------------------------------------------

	// �¶��� - �ʱ���
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

	// �¶��� - �罿
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

	// �¶��� - �ݴް� 
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

	// �¶��� - ����ݴް� 
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

	// �¶��� - ���� â����
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

	// �¶��� - ���� ���â����(Upgrade)
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

	// �¶��� - ���� Ȱ����
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

	// �¶��� - ���� ���Ȱ����(Upgrade)
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

	// �¶��� - �Ϻ� �˼�����
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

	// �¶��� - �Ϻ� ��ް˼�����(Upgrade)
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

	// �¶��� - �Ϻ� ���ѳ���
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

	// �¶��� - �Ϻ� ������ѳ���(Upgrade)
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

	// �¶��� - �ֱ�
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

	// �¶��� - ���ѿֱ�
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
		
	// �¶��� - ����
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

	// �¶��� - ���� �İ��
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

	// �¶��� - ���� ����İ��(Upgrade)
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

	// �¶��� - ���1
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

	// �ٴٰź���
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

	// ������
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

	// �Ź̿䱫
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

	// �縶�� 
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

	// ������ȥ 
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
	
	// �Ű���ȥ 
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

	// ���ڳ���
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

	// �����
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

	// �¶��� - �հŸӸ� 
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

	// �¶��� - �ظ� 
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

	// �¶��� - ������. 
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

	// �¶��� - ������
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

	// �⸶â�� 
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

	// ���ϰ����
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

	// �븸����
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

	// �븸�չ� 
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

	// �븸 ��޼����Ѽ�
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

	// �Ҵ�   
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

	// ������    
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

	// ���ξǾ�
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

	// �������� 
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

	// �������� 
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

	// ������ 
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

	// ���ֹι��� 
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

	// ���ֹ�
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

	// Ÿ���絹�� 
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

	// ����������. 
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

	// ������
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

	// �븸 �հ���
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

	// �븸 ������
	KI[KIND_ON_GENERAL4T].Init       ("KIND_ON_GENERAL4T",
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_MAGIC4T, // ��������
		0, 
		0,
		0,
		ATTACKATB_BIGSWORD, 0, 
		DEFENCEATB_HUMAN,
		NULL , NULL);

	// �븸 �丣������ 
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

	// �븸 ������ 
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

	// �븸 ������
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

	// õ��. 
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

	// ��������. 
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

	// �������� 
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

	// ���� 
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

	//��������.
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

	// �Ŵ�� 
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

	// ����� 
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

	//������.
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

	//�߱� �뺴
	KI[KIND_ON_SWORD_C].Init       ("KIND_ON_SWORD_C",							//ūĮ����.
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

	KI[KIND_ON_PORTER_C].Init       ("KIND_ON_PORTER_C",							//�߱� ����.
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

	KI[KIND_ON_FAN_C].Init       ("KIND_ON_FAN_C",							//���谡.
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

	KI[KIND_ON_BUDAK_C].Init       ("KIND_ON_BUDAK_C",							//������.
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

	KI[KIND_ON_MUDOGA_C].Init       ("KIND_ON_MUDOGA_C",							//������
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

	KI[KIND_ON_FIREGUN_C].Init       ("KIND_ON_FIREGUN_C",							//ȭ����
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

	//��� ūĮ����.
	KI[KIND_ON_SWORDUP_C].Init       ("KIND_ON_SWORDUP_C",							//ūĮ����.
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

	//��� ���谡.
	KI[KIND_ON_FANUP_C].Init       ("KIND_ON_FANUP_C",							//���谡.
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

	//��� ������ 
	KI[KIND_ON_BUDAKUP_C].Init       ("KIND_ON_BUDAKUP_C",							//������.
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

	//��� ������.
	KI[KIND_ON_MUDOGAUP_C].Init       ("KIND_ON_MUDOGAUP_C",							//������
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

	// ��� ȭ����
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

	//�߱� ��� �߰�
	//�������(�߱����) ���� �׽�Ʈ.
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

	//�̷�(�߱����) //�����׽�Ʈ
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

	//��������(�߱����)
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

	//������(�߱����)
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

	//���� (�߱����)
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
	//����
	////////////////////////////////////////////////////////////////////////////////////////

	//���� ������.
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

	//���� ��.
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
	
	//�ٴ��� ���.
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

	//����.
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

	//�Ƹ����.
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
	
	//��ϱ���
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
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����Ȳ�� ���� START
//----------------------------------------------------------------------------

	//�̻��� ���.
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
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����Ȳ�� ���� END
//----------------------------------------------------------------------------


	//�θ���
	KI[KIND_ON_BIRD].Init ("KIND_ON_BIRD",
		KINDATB_FLY,
		ATB_FLY|ATB_SHADOW|ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR, 
		0,
		ATB3_TORNADO,//ǳ���.
		0,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SWORD,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	//������.
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

	//¡Ű��ĭ�� ȥ.
	KI[KIND_ON_SOUL].Init ("KIND_ON_SOUL",
		0, 
		ATB_ATTACK|ATB_PATIENT,
		0,
		ATB3_MAGIC4T,		//���ɺм��(��������)
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD,ATTACKATB_MAGIC4T, 
		DEFENCEATB_GHOST,
		NULL, NULL);

	//�ں��.
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

	//ȭ���ǺҲ�
	KI[KIND_ON_VOLCANOFLAME].Init ("KIND_ON_VOLCANOFLAME",
		0, 
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE, 
		0,
		ATB3_KIMBOMB,//���ڰ�.
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_VOLCANOFLAME,0, 
		DEFENCEATB_ANIMAL,
		NULL, NULL);

	// ó��ͽ�.
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

	//�ذ�
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

	//�ذ񺴻�.
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

	// ��������.
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

	//�븸�ͽ�.
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

	//�Ϻ��ͽ�.
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

	//�ƿ�Ÿ.
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

	//������
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

	//��������
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

	//��������.
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

	//�糪��
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

	// Į����
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

	// ���� �뺴 - ��� Į����
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

	// ���� 
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

	// �⸶����
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

	// ��Ͻ�
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

	// �߼�����
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

	// �븸 �뺴 - ��� �߼�����
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

	// ����â
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

	// ������
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
	// �߱� �뺴 - ��� ������
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

	// ����
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

	//��ȣ
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

	//����ȣ����.
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

	//�û���
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

	//ǥ��
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

	//�������
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
	
	// ���⸶��
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

	// ���⸶�ü�
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
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����Ȳ�� ���� START
//----------------------------------------------------------------------------

	// ������
	KI[KIND_ON_GATEKEEPER].Init       ("KIND_ON_GATEKEEPER",
		0, 
		ATB_ATTACK | ATB_PATIENT, 
		0, 
		0,
		ATB3_MAGIC4T, // ��������
		0, 
		0, 
		ATTACKATB_SWORD, ATTACKATB_MAGIC4T, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	// �⸶������
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

	// �һ���
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

	// ����Ȳ��ȥ
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
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����Ȳ�� ���� END
//----------------------------------------------------------------------------


	//�Ǽ���
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

	// ���� �뺴 - ��� �Ǽ���
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

	//����
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

	//�����
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

	// �Ϻ� �뺴 - ��� �����
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

	//���̸���
	KI[KIND_ON_SEIMEI].Init       ("KIND_ON_SEIMEI", 
		KINDATB_GENERAL,  
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_SOUL_CHANGE,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_SOULCHANGE/*���̸��� �⺻ �̻��� ����*//*ATTACKATB_BIGSWORD*/, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);

	//���»�
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

	// �븸 �뺴 - ��� ���»�
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

	//�弱ȭ
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

	//��â����
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

	// �߱� �뺴 - ��� ��â����
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

	//����.
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

	//Ƽ���
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

	//�ͷ�
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

	//��ȯ������.
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

	// actdoll (031230)		�¶��� - ��2
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
	// actdoll (031230)		�¶��� - ��3
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
	// actdoll (031230)		�¶��� - ��4
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
	// actdoll (031230)		�¶��� - ��5
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
	// actdoll (031230)		�¶��� - ��6
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
	// actdoll (031230)		�¶��� - ��7
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
	// actdoll (031230)		�¶��� - ��8
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
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� START
//----------------------------------------------------------------------------

	// ������ - �ϼ�
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_HELLMESSENGER;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 1000;
	KI[KIND_ON_HELLKING].Init	("KIND_ON_HELLKING",
		0,
		ATB_ATTACK | ATB_PATIENT,									// ĳ���� �����ൿ �Ӽ�
		0,															// ĳ���� �����ൿ �Ӽ�(����� ������ ��ư Ŭ�� ��)
		ATB3_MAGIC6C | ATB3_GROUNDEXP,								// ĳ���� Ư����� 1
		ATB4_MAGIC_RECALL_DAMDUK,
		0, 
		DYINGATB_IMMEDIATE | DYINGATB_NOCARCASS,
		ATTACKATB_BIGSWORD, ATTACKATB_MAGIC2,						// ���� �Ӽ�(����, ����)
		DEFENCEATB_GHOST,
		NULL, NULL, -1, -1, -1, 0, &subData);

	// ���»��� - �ϼ�
	ZeroMemory( &subData, sizeof(subData) );
	subData.m_siOptionType_01		= KIND_ON_GANGSI;
	subData.m_siOptionValue1_01		= 1;
	subData.m_siOptionValue2_01		= 60;	// robypark 2004/9/2 15:19 ���ü�ȯ�� �ʿ��� ������ ����(400 => 60)
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

	// ���� �繫���� - �ϼ�
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

	// �̹��� - �ϼ�
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

	// ���� �ڳ��� - �ϼ�
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

	// ���� ������ - �ϼ�
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

	// ���� �������� - �ϼ�
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

	// ���� �ذ� - �ϼ�
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

	// ���� �ذ񺴻� - �ϼ�
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

	// ���� ���� - �ϼ�
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

	// ���� �Ҳ� - �ϼ�
	KI[KIND_ON_GHOSTVOLCANOFLAME].Init	("KIND_ON_GHOSTVOLCANOFLAME",
		0,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		ATB3_KIMBOMB,//���ڰ�.
		0, 
		0,
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS,
		ATTACKATB_VOLCANOFLAME, ATTACKATB_KIMBOMB, 
		DEFENCEATB_GHOST,
		NULL, NULL);
//----------------------------------------------------------------------------
// actdoll (2004/02/26 13:33) : �߰� ĳ���� - ����� ���� END
//----------------------------------------------------------------------------

	// ��ȣ��(��)
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

	// ��ȣ��(��1)
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

	// ��ȣ��(��2)
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

	// ��ȣ��(��3
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
	//��ȣ�� 6������ ���� ĳ���� (�ӽ�).

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

	// �¶��� - ħ���ֱ� 2004-5-7
	// ���ѿֱ��� �����ϸ� �̸��� �ٸ�
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
// robypark (2004/05/11 13:15) : �߰� ĳ���� - ��� 2�� ����
//----------------------------------------------------------------------------
	// �¶��� - �ź���
	// ����-�̼��� 2�� ����
	KI[KIND_ON_TURTLE_VEHICLE].Init         ("KIND_ON_TURTLE_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_FIRE_ROUND_DANCE/*����������*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_TURTLE_VEHICLE/*�ź��� ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - ������
	// ����-���� 2�� ����
	KI[KIND_ON_THUNDER_VEHICLE].Init         ("KIND_ON_THUNDER_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_NOVA/*����*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_THUNDER_VEHICLE/*������ ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - ������
	// �Ϻ�-��Ű��ī 2�� ����
	KI[KIND_ON_QUAKE_VEHICLE].Init         ("KIND_ON_QUAKE_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_FURY_OF_VOLCANO/*�ݳ뿰��*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_QUAKE_VEHICLE/*������ ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - �����
	// �Ϻ�-���̸��� 2�� ����
	KI[KIND_ON_BLACK_DRAGON_VEHICLE].Init         ("KIND_ON_BLACK_DRAGON_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_POISONING_HEAVENS/*õ������*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BLACK_DRAGON_VEHICLE/*����� ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - �߼���
	// �߱�-����<=�̷� 2�� ����
	KI[KIND_ON_CATAPULT].Init         ("KIND_ON_CATAPULT",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_BOMB_OF_HEAVEN/*��õ��*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_CATAPULT/*�߼��� ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - �Ҷ�������
	// �߱�-�̷�<=���� 2�� ����
	KI[KIND_ON_FARANGI_VEHICLE].Init         ("KIND_ON_FARANGI_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_SIEGE_MODE/*��õ���*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_FARANGI_VEHICLE/*�Ҷ��� ����*/, 0, 
		DEFENCEATB_ARMOUR,
		NULL, NULL);
	
	// �¶��� - ȭ����
	// �븸-����â 2�� ����
	KI[KIND_ON_FIRE_DRAGON_VEHICLE].Init         ("KIND_ON_FIRE_DRAGON_VEHICLE",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT,
		0, 
		0,
		ATB4_ONSLAUGHT_FIREBALL/*�Ͱ�ȭ����*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_BIGSWORD/*ū��*/, ATTACKATB_FIRE_DRAGON_VEHICLE/*ȭ���� ����*/,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// �¶��� - ��Ȳ����
	// �븸-�弱ȭ 2�� ����
	KI[KIND_ON_CHINESEPHOENIX].Init         ("KIND_ON_CHINESEPHOENIX",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_ATTACKAIR|ATB_SHADOW,
		0, 
		0,
		ATB4_FIRERAIN_OF_GOD/*õ��ȭ��*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_CHINESEPHOENIX/*��Ȳ���� ����*/, 0,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// 2�� ��� ��ų �߰� ����
	// robypark 2004/6/4 20:28
	// �¶��� - �Ҷ��������� ������ ��õ���
	// �߱�-�̷� 2�� ���� ��� ��õ���
	// �Ҷ��������� �����ϳ� ���, �����Ÿ�, ���ݼӵ�, �þ�, �׸��Ϻθ� �ٸ���.
	// CharaterList.txt���Ͽ��� �������� �ʴ´�. �ٸ� CharCode.txt, CharInfo.txt���Ͽ��� 
	// �ʻ�ȭ, �ڽ�ũ��, ���� ������ ����Ǿ��ִ�. CharCode.txt���� KIND_ON_ROAR_OF_LAUGHTER��
	// ���ѳ���, �� ĳ���;��̵�� U107�� �Ҷ��������� �װͰ� �����ϴ�.
	// CharInfo.txt���� �� KIND_ON_ROAR_OF_LAUGHTER�� ���뵵 �Ҷ��������� �װͰ� �����ϴ�. ����,
	// BottomIndent, XsizePercent, YsizePercent�� �ٸ���.
	KI[KIND_ON_ROAR_OF_LAUGHTER].Init         ("KIND_ON_ROAR_OF_LAUGHTER",
		KINDATB_GENERAL,
		ATB_ATTACK|ATB_PATIENT|ATB_MINRANGE,
		0, 
		0,
		ATB4_SIEGE_MODE_UNDO/*��õ���=>�Ҷ������� ����*/,
		0, 
		DYINGATB_IMMEDIATE|DYINGATB_NOCARCASS, 
		ATTACKATB_ROAR_OF_LAUGHTER/*��õ��� ����*/, 0,
		DEFENCEATB_ARMOUR,
		NULL, NULL);

	// robypark 2004/9/17 13:32
	// ���� �� ������
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
	// ��¥ �������
	// ��� ����� �̸��� �ٸ� �� ��� ������ �����ϴ�.
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

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 16:40
	// ������ ���� ĳ���� �߰�
	// ������ �Ӽ����� ������ ������ ��쿡 �ؼ��Ǵ� ���� ���Ѵ�.
	// ������ ĳ���� �Ӽ� ����: KINDATB_SIEGE_WARFARE_UNIT - ������ ������ ��Ÿ���� �Ӽ�
	//							ATB_ATTACK - ������ ������ ������ �� �ִٴ� �Ӽ�. �� �Ӽ��� ������ ������ ���޼���ó�� ������ ���� ������ �Ұ����ϴ�.
	//							ATB_PATIENT - ����ǰ�� ��� (�ڵ�)�Ҹ��Ѵٴ� �Ӽ�. ���޼���ó�� ����ǰ�� ���� �� �� �ִ� ���ֿ��Լ� ����ǰ�� ���� �� �ִٴ� �ǹ̷� ���ȴ�.
	//							ATB_ATTACK_VILLAGE - ������ ������ �� �ִٴ� �Ӽ�. �� �Ӽ��� ������ â���̳� ���޼���ó�� ������ ���� ������ �Ұ����ϴ�.
	//							- ������ ĳ������ ���� �Ӽ� ���� �������� ����Ǵ� ���ݼӼ��� �����ϰ� ������� �ʴ´�.
	//							����, ������ �߿� ����Ʈ ������ ���Ͽ� �����ϱ� ���� ���̴�.
	//							ATB_EFFECT_VILLAGECOUNTERATTACK_OIL	- ���� �ݰ� - �⸧ ������ �޴´�. ��, ����Ʈ�� �׷��� �Ѵ�. ex) ���� ����
	//							ATB_EFFECT_VILLAGECOUNTERATTACK_STONE - ���� �ݰ� - �� ������ �޴´�. ��, ����Ʈ�� �׷��� �Ѵ�. ex) ���� �˺�
	//							ATB_STORE_AND_SUPPLY_SUPPLYGOODS - ������ ����ǰ�� ���� �� �ٸ� ������ ���ֿ��� ������ �� �ִٴ� �Ӽ�. ex) ������ ���޼���
	//							ATTACKATB_SWORD - ���� ���� ������ ������ ���� ����Ʈ ���
	//							ATTACKATB_ARROW - ������ �ú� ȭ�� ����
	//							ATTACKATB_UPGRADETOWERCANNON - ������ ���� ���� ���� ����Ʈ ���
	//							ATTACKATB_CATAPULT - ������ �߼��� �� ����
	// ������ - �˺�
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

	// ������ - â��
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

	// ������ - �ú�
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

	// ������ - �⸶��
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

	// ������ - ����
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

	// ������ - �߼���
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

	// ������ - ���� ����
	KI[KIND_ON_SIEGEWARFARE_SUPPLY_WAGON].Init(	"KIND_ON_SIEGEWARFARE_SUPPLY_WAGON",
		KINDATB_SIEGE_WARFARE_UNIT,  
		ATB_PATIENT | ATB_STORE_AND_SUPPLY_SUPPLYGOODS/*������ ������ �ִ�*/,
		ATB2_HEAL/*�ٸ� ������ ���� ġ�� ����*/, 
		0,
		0,
		0, 
		0,
		0, 0, 
		DEFENCEATB_HUMAN,
		NULL, NULL);
	// �� /////////////////////////////////////////////////////////////////

	// ���� ///////////////////////////////////////////////////////////////
	// robypark 2004/11/17:8
	// 11�� ��ȭ ���� Ž�� �̺�Ʈ ���� �߰�
	// ���Ȱ� �����
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

	// ���Ȱ� ��ݺ�
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
	// �� /////////////////////////////////////////////////////////////////

	// ���Ȱ� ��ݺ�
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
	// �¶��� �ǳ��� ĳ����
	// �¶��� - �ǳ��� ��ö��.
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
	
	// �¶��� - �ǳ��� �̸�ȭ
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

	// �¶��� - �ǳ��� �ø��� ��
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

	// �¶��� - �ǳ��� �ճ� �ϳ�����
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

	// �¶��� - �ǳ��� �ռҷ�
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

	// �¶��� - �չ̷�
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

	// �¶��� - �ǳ��� ��ȣ�� 
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

	// �¶��� - �ǳ��� ����� 
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
	// ĳ���͵��� �ִϸ��̼� ����
	Parse_CharacterAnimationInformation();

	//////////////////////////////////////////////////////
	// ũ��ŷ ������ ���� ó��
	if(CheckKindInfo() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: TransKindOnlineToImjin()
// Code: actdoll (2003-12-30)
// Desc: ������ ĳ���� �ε����� ���ӿ� ĳ���� �ε����� �ٲ��ش�.
//		kind			- ������ ĳ���� �ε��� ��ȣ
//		bShowErrorMsg	- �����޽����� �����ַ��� true, �ƴ϶�� false
//		return			- �����Ѵٸ� ���ӿ� ĳ���� �ε���, �ƴ϶�� -1
//-----------------------------------------------------------------------------
SI32	TransKindOnlineToImjin( SI32 kind, BOOL bShowErrorMsg )
{
	register	SI32	i;
	for(i=0; i<KIND_ON_MAX; i++)
	{
		if( KI[i].m_siServerID == kind )		return i;
	}

	// ���� �޽����� ���ٴ� ���� Ȯ�ο��̹Ƿ� ������ �����ְ� �ӽ÷� �罿������ ����
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

	// ���� �޽����� ���ٴ� ���� Ȯ�ο��̹Ƿ� ������ �����ְ� �ӽ÷� �罿������ ����
	if( bShowErrorMsg )		
	{
		clGrp.Error("No Char:TransKindOnlineToImjin", "[%d]", kind);
		return KIND_ON_DEER;
	}

	// ���� �޽����� �Ⱦ��ٴ� ���� �˻������� ã�ڴٴ� ����
	return -1;
*/
}

//-----------------------------------------------------------------------------
// Name: TransKindImjinToOnline()
// Code: actdoll (2003-12-30)
// Desc: ���ӿ� ĳ���� �ε����� ������ ĳ���� �ε����� �ٲ��ش�.
//		kind			- ���ӿ� ĳ���� �ε��� ��ȣ
//		bShowErrorMsg	- �����޽����� �����ַ��� true, �ƴ϶�� false
//		return			- �����Ѵٸ� ������ ĳ���� �ε���, �ƴ϶�� 0
//-----------------------------------------------------------------------------
SI32	TransKindImjinToOnline( SI32 kind, BOOL bShowErrorMsg )
{
	BOOL	bError	= false;
	if( kind <= KIND_ON_INVALID || kind >= KIND_ON_MAX )	bError	= true;	// ���� ����
	if( !bError && KI[kind].m_siServerID == 0 )				bError	= true;	// �ش� ĳ���Ͱ� ���� ����

	if( bError )
	{
		// ���� �޽����� ���ٴ� ���� Ȯ�ο��̹Ƿ� ������ �����ְ� �ӽ÷� �罿������ ����
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

	// ���� �޽����� ���ٴ� ���� Ȯ�ο��̹Ƿ� ������ �����ְ� �ӽ÷� �罿������ ����
	if( bShowErrorMsg )
	{
		clGrp.Error("No Char2", "[%d]", kind);
		return 6221;
	}

	// ���� �޽����� �Ⱦ��ٴ� ���� �˻������� ã�ڴٴ� ����
	return -1;
*/
}

//Ư�� ĳ���Ϳ� ���ؼ� ���ݽõ��� �� ȿ������ �����Ѵ�. 
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

//Ư�� ĳ���͸� �ʵ�� ���̰� �� �� ȿ������ �����Ѵ�. 
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

// ���� �Ӽ��� ���� ���� ���� �ð��� ���´�. 
SI32 DecideAttackDelayByAttackAtb(SI32 attackatb)
{

	switch(attackatb)
	{
	case ATTACKATB_GUN: // �� 
		return 10;
		break;

	case ATTACKATB_ARROW:  // Ȱ    
		return 4;
		break;

	case ATTACKATB_BIGSWORD:	// Į 
		return 0;
		break;

	case ATTACKATB_SWORD:		// â 
		return 2;
		break;

	case ATTACKATB_DART:		// ǥâ.
		return 2;
		break;

	case ATTACKATB_DOLL1:		// �������� 
	case ATTACKATB_DOLL2:		// ��ī������ 
	case ATTACKATB_KUKET:		// ����
	case ATTACKATB_SNOW:
		return 2;
		break;

	case ATTACKATB_JUSTICEK:	// ��ä. 
		return 5;
		break;
	}

	return 0;

}


//-----------------------------------------------------------------------------
// Name: CheckKindInfo()
// Code: actdoll (2003-12-30)
// Desc: �� ���� ������ ��ŷ�Ǿ������� �˻��Ѵ�.
//-----------------------------------------------------------------------------
BOOL CheckKindInfo()
{
	// ĳ���� ������ �հ�
	register	SI32	i;
	DWORD				unique	= 0;
	SI32				sum		= 0;

// robypark 2004/10/2 11:55
#ifdef _DEBUG
	// ����� ��忡�� ĳ���� üũ ���� ���α׷� ������ �� �ѹ��� �˻��ϵ��� ����.
	static bool bOneTimer = false;

	if (false == bOneTimer)
	{
		bOneTimer = true;
	}
	else
		return TRUE;
#endif	// _DEBUG
   
	// ����ũ ���� ���Ѵ�. 
	for( i=0; i<KIND_ON_MAX; i++ )	unique += KI[i].m_uiUnique;
	
	// ġƮ�� ���̶��, 
	if((unique % 10000) != DF_VALUE_BU)
	{
		// ��� �޽��� 
		clGrp.Error("Warning", " %s [%d]", Text.CheatedGame.Get(), unique%10000);

// actdoll (2004/02/27 17:27) : ����׸��ÿ��� ǥ�⸸ ���ְ� ������ �Ѵ�.
#ifndef	_DEBUG
		return FALSE;
#endif//_DEBUG
	}

	// TransKindData[] ���� ������ �������� Ȯ���Ѵ�. 
	for( i=0; i<KIND_ON_MAX; i++ )	sum += KI[i].m_siServerID;

	// TransKindData[] ���� ������ �ִٸ� ����!
	if( sum != DF_VALUE_CS )
	{
		clGrp.Error( "fds937h", "fd9334h:%d", sum );
// actdoll (2004/02/27 17:27) : ����׸��ÿ��� ǥ�⸸ ���ְ� ������ �Ѵ�.
#ifndef	_DEBUG
		return FALSE;
#endif//_DEBUG
	}

	return TRUE;
}
