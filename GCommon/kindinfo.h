#ifndef _KINDINFO_H
#define _KINDINFO_H  

#include <CharAnimation.h>

// #define PRINT_CHARACTER_INF
class	cltOnlineWorld;

//-------------------------------------------------------------------------------------------------
// �Ӽ� ���� 
// ���� ������ ���ֿ��� ������ ��ġ�� �Ӽ�. 
// ������ ����� �� ���� �Ӽ��̴�. (�ʱ�ȭ�� �ѹ� �����ϸ� ����� �Ұ�.!!!) 
//-------------------------------------------------------------------------------------------------
#define KINDATB_WATER							0x00000002			// ���� ĳ���� 
#define KINDATB_FLY								0x00000004			// ���� ĳ���� 
#define KINDATB_GENERAL							0x00000008			// ��� ĳ���� 
#define KINDATB_SWIM							0x00000100			// ���� ���� 
#define KINDATB_GEFFECTHIDE						0x00000200			// ��� ȿ�� - ���� ���� 
#define KINDATB_GEFFECTAPOWER					0x00000400			// ��� ȿ�� - ���ݷ� ���� 
#define KINDATB_GEFFECTDPOWER					0x00000800			// ��� ȿ�� - ���� ���� 
#define KINDATB_GEFFECTHEALTH					0x00001000			// ��� ȿ�� - ü�� ȸ��  
#define KINDATB_GEFFECTGUN						0x00002000			// ��� ȿ�� - �ѽ�� ������ ���� ��� 
#define KINDATB_AMBUSH							0x00004000			// �ź�����
#define KINDATB_AIRMAGIC						0x00010000			// ����ź ��� ����
#define KINDATB_SHOWORIGINALTEAMNUMBER			0x00020000			// ���� �÷��̾� ��ȣ�� ǥ���ϴ°� ?
#define KINDATB_DETECTMINE						0x00040000			// ���� ���� ����
#define KINDATB_HERO							0x00080000			// ���ΰ� ĳ����(������)

#define KINDATB_SIEGE_WARFARE_UNIT				0x00100000			// robypark 2004/10/18 16:33: ������ ĳ���������� ��Ÿ���� �Ӽ� ��

//-----------------------------------------------------------------------------
// Name: ST_KI_SUBDATA
// Code: actdoll (2004-03-06)
// Desc: KindInfo �ʱ�ȭ�� ����� ���� ������ ����ü
//-----------------------------------------------------------------------------
struct ST_KI_SUBDATA 
{
	// KindInfo ���� ������ �ɼǵ�
	SI32	m_siOptionType_01;
	SI32	m_siOptionType_02;
	SI32	m_siOptionType_03;

	// KindInfo ���� ������ �ɼǵ鿡 ����� ��ġ ù��°
	SI32	m_siOptionValue1_01;
	SI32	m_siOptionValue1_02;
	SI32	m_siOptionValue1_03;

	// KindInfo ���� ������ �ɼǵ鿡 ����� ��ġ �ι�°
	SI32	m_siOptionValue2_01;
	SI32	m_siOptionValue2_02;
	SI32	m_siOptionValue2_03;
}; 

//-------------------------------------------------------------------------------------------------
// ĳ���Ϳ� �ǹ��� ���� ������ �����Ѵ�.
//-------------------------------------------------------------------------------------------------
class _KindInfo 
{
protected:
	// robypark 2004-5-24, 14:59: �������� �Ұ����� ĳ������ �ּ� ��� ���������� ���� ������ �߰�
	char					m_nMinAttackRange[2];		// ĳ���Ͱ� ���� ������ �ּҰŸ�(�⺻ ������ 1)

public:
	// robbypark 2004-5-24, 14:59: �������� �Ұ����� ĳ������ �ּ� ��� ���������� ���� ������ �߰�
	// ĳ���Ͱ� ���� ������ �ּҰŸ� ���
	// SI32 siAttackAtb[in]: �� ��° ���� �Ӽ��ΰ�? (0 or 1)
	// return value: �ش� ���� �Ӽ��� ���� ������ �ּҰŸ�
	char GetMinAttackRange(SI32 siAttackAtb)
	{
		return m_nMinAttackRange[siAttackAtb];
	}

	// robbypark 2004-5-24, 14:59: �������� �Ұ����� ĳ������ �ּ� ��� ���������� ���� ������ �߰�
	// ĳ���Ͱ� ���� ������ �ּҰŸ� ����
	// SI32 siAttackAtb[in]: �� ��° ���� �Ӽ��ΰ�? (0 or 1)
	// char nMinAttackRange[in]: ���� ������ �ּҰŸ� ��
	// return value: �ش� ���� �Ӽ��� ���� ������ �ּҰŸ�
	void SetMinAttackRange(SI32 siAttackAtb, char nMinAttackRange)
	{
		m_nMinAttackRange[siAttackAtb] = nMinAttackRange;
	}

private:
	SHORT 					m_ssOnlinePortraitFont;							// �Ż󿡼� �Ϲݿ� �ʻ�ȭ�� �̹��� ��Ʈ, ���� �߿��� ����. 
	SHORT 					m_ssOnlineMercenaryFont;						// �Ż󿡼� �뺴�� �ʻ�ȭ�� �̹��� ��Ʈ 
//	SHORT 					MaxApower;
	SI32					m_siExpVaryPercent;								// ����ġ ����ġ. 
	SI32 					m_siAttackRange1, m_siAttackRange2;				// ����1 �����Ÿ�, ����2 �����Ÿ�
	DWORD 					m_uiAttackAtb1;									// ���� �Ӽ� 1
	DWORD 					m_uiAttackAtb2;									// ���� �Ӽ� 2
	SI32 					m_siAttackDelay1;								// ���� 1 �����ð�
	SI32 					m_siAttackDelay2;								// ���� 2 �����ð�
	SI08					m_scMagicApplyRate;								// ���� �����(���� Damage ��꿡 ����ȴ�)
	
public:
	DWORD 					m_uiUnique;										// �������� �����Ǿ������� �˼� �ִ� ����ũ ��.
	SI32					m_siServerID;									// �������� ���Ǵ� �� ĳ������ ���� ID

	SI32					m_siAttackEffect1;								// ����1�� ȿ����
	SI32					m_siAttackEffect2;								// ����2�� ȿ����
	
	SHORT 					m_ssBottomIndent;        						// �׸� ��Ʈ�� �ٴڿ��� ���� �󸶳� ������ �ִ°�?
	SHORT 					m_ssIDXsize, m_ssIDYsize;    					// ĳ���Ͱ� �������� Ÿ�Ͽ��� �����ϴ� Ÿ���� ����. ����, ����.

	SHORT					m_ssXsizePercent, m_ssYsizePercent;				//  ���� �߿� ĳ���� ���� �ڽ� ũ��. 
	int						m_siKindAtb;									// ���ΰ�(������), ���, ���� ĳ���������� ��Ÿ���� �Ӽ�
	SHORT 					m_ssNation;             						// ��������� ĳ�����ΰ�?
	SHORT 					m_ssMoveSpeed;									// ĳ���� �̵��ӵ�(����)
						
	SI32					m_siSearchRange;								// ĳ���� �þ�(����)
	SHORT 					m_ssMaxFireRepeatNumber, m_ssFireRepeatDelay;
	SHORT 					m_ssWorkDelay, m_ssDeadDelay, m_ssTowerDelay, m_ssChangeDirectionDelay, m_ssAttackGiveUpDelay;
	DWORD 					m_uiAtb;										// ���� �Ӽ� Ÿ�� 1
	DWORD 					m_uiAtb2;										// ���� �Ӽ� Ÿ�� 2
	DWORD 					m_uiAtb3;										// ���� �Ӽ� Ÿ�� 3
	DWORD					m_uiAtb4;										// ���� �Ӽ� Ÿ�� 4
	DWORD					m_uiAtb5;										// ���� �Ӽ� Ÿ�� 5 robypark 2004/10/1 17:6
	DWORD 					m_uiDyingAtb;									// ����� �Ӽ�(��ü ����, ��)
	DWORD 					m_uiDefenceAtb;									// �� Ÿ��(2004/10/5 ���ȵ�)
	SHORT 					m_ssVibrateSwitch;								// ���� ���� ȿ�� ����ġ
	SHORT					m_ssPriority_Attack_Type;						// robypark 2004/10/18 12:10 - �� ���� ���� �Ӽ��� ���� ��� �ΰ����� ó������ ��

	ST_KI_SUBDATA			m_SubData;			// actdoll (2004/03/06 3:19) : ���� �����Ͱ� �ʿ��ϹǷ� �߰��Ѵ�.
		
	SI32					m_siTryAttackEffect;	// �ʵ忡�� ������ �� ȿ����. 
	SI32 					m_siSelectEffect;		// ȿ����. ĳ���� ����
	SI32 					m_siDyingEffect;		// ȿ����. ĳ���� ���
	SI32 					m_siMoveEffect;			// ȿ����. ĳ���� �̵�. �̵����
	SI32 					m_siAttackEffect;		// ȿ����. ĳ���� ����

	char 				   *m_pszName;				// ĳ���� ��
	char 				   *m_pszIDName; 

	SHORT					m_ssHeroID;				// ���ΰ�(������) ĳ���� ID
	SHORT 					m_ssDefaultArmor;		// ���ΰ�(������) ĳ���� �⺻ ��
	SHORT 					m_ssDefaultHelmet;		// ���ΰ�(������) ĳ���� �⺻ ���
	SHORT 					m_ssDefaultWeapon;		// ���ΰ�(������) ĳ���� �⺻ ����


	SI32					m_siCharAnimationInfo[MAX_ANIMATION_PER_UNIT];	// ĳ���� �ִϸ��̼� ����
	cltCharAnimation		m_clCharAnimation[MAX_ANIMATION_PER_UNIT];		// ĳ���� �ִϸ��̼� ������

#ifdef PRINT_CHARACTER_INF
	char					m_nameCode[200];
	char					m_aniFileName[MAX_ANIMATION_PER_UNIT][200];

	void					PrintInf(void);
#endif

#ifndef _USE_HASHCODE_EFFECTSOUND
	void					Init(char*namecode,
								int kindatb, 
								SHORT movespeed, 
								SI32 expvarypercent, 
								SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
								SI32 attackdelay1, SI32 attackdelay2,
								SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
								SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD dyingatb, 
								SI32 attackatb, SI32 attackatb2,
								SI32 attackeffect1, SI32 attackeffect2,
								DWORD defenceatb,
								SHORT vibrateswitch, 
								SI32 tryattackeffect, SI32 selecteffect, SI32 dyingeffect, SI32 moveeffect, SI32 attackeffect, 
								char* name, char *idname, 
								SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);
#else
	void					Init(char*namecode,
								int kindatb, 
								SHORT movespeed, 
								SI32 expvarypercent, 
								SI32 attackrange1, SI32 attackrange2, SHORT searchrange, 
								SI32 attackdelay1, SI32 attackdelay2,
								SHORT maxfirerepeatnumber, SHORT firerepeatdelay,
								SHORT workdelay, SHORT deaddelay, SHORT towerdelay, SHORT changedirectiondelay, SHORT attackgiveupdelay, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD dyingatb, 
								SI32 attackatb, SI32 attackatb2,
								const char* attackeffect1, 
								const char* attackeffect2,
								DWORD defenceatb,
								SHORT vibrateswitch, 
								const char* tryattackeffect, 
								const char* selecteffect, 
								const char* dyingeffect, 
								const char* moveeffect, 
								const char* attackeffect, 
								char* name, char *idname, 
								SHORT heroid, SHORT defaultArmor, SHORT defaultHelmet, SHORT defaultWeapon, 
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);
#endif
	// robypark 2004/9/23 14:50
	// ĳ������ �� �� ������ HQ�� CharInfo.txt���Ͽ��� �о������ �����Ѵ�.
	void					Init(char*namecode,
								int kindatb, 
								DWORD atb, 
								DWORD atb2,
								DWORD atb3,
								DWORD atb4,
								DWORD atb5,
								DWORD dyingatb, 
								SI32 attackatb1,
								SI32 attackatb2,
								DWORD defenceatb,
								char* name,
								char *idname, 
								SHORT defaultarmor = -1,
								SHORT defaulthelmet = -1,
								SHORT defaultweapon = -1,
								SHORT heroid				= 0,
								ST_KI_SUBDATA	*pSubData	= NULL,
								SI08 magicapplyrate			= 70);

	void					SetFile(SI32 Ani, BOOL DirSwitch, char* File,SI32 Font,SI32 Frame, SI32 Instant, SI32 Delay, SI32 atb=0);

	DWORD					GetUnique( void );


	BOOL					HaveUpgradeType(SHORT upgrade);

	DWORD					Add( void );

	SI32					GetAttackRange(SI32 currentattack);
	SI32					GetAttackAtb(SI32 currentattack);
	SI32					GetAttackDelay(SI32 currentattack);

	int						IsKindAtb(int atb)
	{
	   if( m_siKindAtb & atb )	return TRUE;
	   return FALSE;
	}

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 18:19
	// ĳ���� �Ӽ� �˻� �Լ� �߰�
	// ĳ���� �Ӽ�1�� �ش� �÷��� �Ӽ� �����ϴ��� �˻�
	int						IsAttribute1(DWORD dwAttribute)
	{
		if (m_uiAtb & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// ĳ���� �Ӽ�2�� �ش� �÷��� �Ӽ� �����ϴ��� �˻�
	int						IsAttribute2(DWORD dwAttribute)
	{
		if (m_uiAtb2 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// ĳ���� �Ӽ�3�� �ش� �÷��� �Ӽ� �����ϴ��� �˻�
	int						IsAttribute3(DWORD dwAttribute)
	{
		if (m_uiAtb3 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// ĳ���� �Ӽ�4�� �ش� �÷��� �Ӽ� �����ϴ��� �˻�
	int						IsAttribute4(DWORD dwAttribute)
	{
		if (m_uiAtb4 & dwAttribute)
			return TRUE;

		return FALSE;
	}

	// ĳ���� �Ӽ�5�� �ش� �÷��� �Ӽ� �����ϴ��� �˻�
	int						IsAttribute5(DWORD dwAttribute)
	{
		if (m_uiAtb5 & dwAttribute)
			return TRUE;

		return FALSE;
	}
	// �� /////////////////////////////////////////////////////////////////

	// robypark 2004/11/3 10:35 ĳ���� ���� �Ӽ�(���� ���) ȿ���� ���
	SI32	GetWeaponAttackEffect	( SI32 siAttackIdx )	
	{
		switch (siAttackIdx)
		{
			case 0:
				return m_siAttackEffect1;
			case 1:
				return m_siAttackEffect2;
			default:
				break;
		}

		return 0;
	}

	SHORT					GetMakeDelay			( void );

	SHORT					GetOnlinePortraitFont	( void )			{ return m_ssOnlinePortraitFont; }
	SHORT					GetOnlineMercenaryFont	( void )			{ return m_ssOnlineMercenaryFont; }

	SI32					GetExpVaryPercent		( void )			{ return m_siExpVaryPercent; }
	SI08					GetMagicApplyRate		( void )			{ return m_scMagicApplyRate; }
	void					SetMagicApplyRate		( SI08 magicrate )	{ m_scMagicApplyRate = magicrate; }

	// �������� �߽� ��ǥ�� �������� ĳ���͸� ����ش�. 
	void Draw(UI16 kind, SI32 animation, UI16 Frame, SI16 X, SI16 Y, cltOnlineWorld *pWorld, SI32 armourcode=0, SI32 helmetcode=0, SI32 weaponcode=0, SI08 siSlot=0);

	// robypark 2004/10/18 12:10
	SHORT					GetPriority_Attack_Type	( void )			{ return m_ssPriority_Attack_Type; }
	void					SetPriority_Attack_Type ( SHORT ssPriority_Attack_Type )	{ m_ssPriority_Attack_Type = ssPriority_Attack_Type; }

	// robypark 2004/11/3 18:5
	// ĳ���� �ִϸ��̼� ���� �ε��� ���
	SI32					GetCharAnimationInfoIdx(SI32 siAnimationIdx)
	{
		SI32 siAnimatonInfoIdx = 1;

		for (siAnimatonInfoIdx = 1; siAnimatonInfoIdx < MAX_ANIMATION_PER_UNIT; siAnimatonInfoIdx++ )
		{
			if( m_siCharAnimationInfo[siAnimatonInfoIdx] == siAnimationIdx)
			{
				return siAnimatonInfoIdx;
			}
		}

		return 0;
	}

	// robypark 2004/11/3 18:9
	// ĳ���� �ִϸ��̼� �߿� Ư�� ����(�Լ� ȣ��) �߻� ���� ��Ʈ ��ȣ ���
	SI32					GetCharAniamtionInstant(SI32 siAnimationIdx)
	{
		return m_clCharAnimation[GetCharAnimationInfoIdx(siAnimationIdx)].GetAnimationInstant();
	}
};

BOOL	InitKindInfo( void );
SI32	TransKindOnlineToImjin( SI32 kind, BOOL bShowErrorMsg = true );
SI32	TransKindImjinToOnline( SI32 kind, BOOL bShowErrorMsg = true );
char	*GetKindName(SHORT kind, SHORT pnumber );

//Ư�� ĳ���Ϳ� ���ؼ� ���ݽõ��� �� ȿ������ �����Ѵ�. 
BOOL PlayTryAttackEffect(SI32 onlinekind);
//Ư�� ĳ���͸� �ʵ�� ���̰� �� �� ȿ������ �����Ѵ�. 
BOOL PlayShowPetEffect(SI32 onlinekind);

// ���� �Ӽ��� ���� ���� ���� �ð��� ���´�. 
SI32 DecideAttackDelayByAttackAtb(SI32 attackatb);

// kindinfo���� ��ŷ ���θ� �˻��Ѵ�. 
BOOL CheckKindInfo( void );

#endif