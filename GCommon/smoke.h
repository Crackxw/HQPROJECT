#ifndef _SMOKE_H
#define _SMOKE_H

// ������ �ִϸ��̼� �ӵ��� ���.
#define	SMOKE_DELAY_1	1
#define SMOKE_DELAY_2	2
#define SMOKE_DELAY_3	3
#define	SMOKE_DELAY_4	4
#define	SMOKE_DELAY_5	5
#define	SMOKE_DELAY_10	10

// �׷��� �� ���
#define SMOKE_TRANSPARENT_MODE_NORMAL     1
#define SMOKE_TRANSPARENT_MODE_OLDTRANS   2
#define SMOKE_TRANSPARENT_MODE_HALFTRANS  3

// ȿ�� ���� ��� ����
#define KIND_SMOKE_EXP0						1    // ��ź ���� 
#define KIND_SMOKE_EXP1						2
#define KIND_SMOKE_BIGEXP2  				3
#define KIND_SMOKE_EXP2     				4
#define KIND_SMOKE_EXP3     				5
#define KIND_SMOKE_EXP4     				6
#define KIND_SMOKE_EXP5     				7
#define KIND_SMOKE_EXP6     				8
#define KIND_SMOKE_EXP7     				9
#define KIND_SMOKE_EXP8     				10
#define KIND_SMOKE_EXP9     				11
#define KIND_SMOKE_EXP10    				12
#define KIND_SMOKE_EXP11    				13
#define KIND_SMOKE_EXP12    				14
#define KIND_SMOKE_EXP13    				15
#define KIND_SMOKE_EXP14    				16
#define KIND_SMOKE_EXP15    				17
#define KIND_SMOKE_SMALLEXP2 				18
#define KIND_SMOKE_GUNSMOKE 				19
#define KIND_SMOKE_GUNHIT   				20 
#define KIND_SMOKE_SMOKE0   				21
#define KIND_SMOKE_SMOKE1   				22
#define KIND_SMOKE_SMOKE2   				23
#define KIND_SMOKE_SMOKE3   				24
#define KIND_SMOKE_SMOKE10   				25
#define KIND_SMOKE_RISINGSMOKE0  			26
#define KIND_SMOKE_RISINGSMOKE1  			27 
#define KIND_SMOKE_FIRE      				28
#define KIND_SMOKE_BIGEXP    				29
#define KIND_SMOKE_MANFIRE   				30
#define KIND_SMOKE_SINK      				31
#define KIND_SMOKE_WATEREXP  				32
#define KIND_SMOKE_EARTHQUAKE 				33
#define KIND_SMOKE_LIGHTNING  				34
#define KIND_SMOKE_BUILDINGEXP				35
#define KIND_SMOKE_LIGHTEXP1				36
#define KIND_SMOKE_LIGHTEXP2				37
#define KIND_SMOKE_LIGHTEXP3				38
#define KIND_SMOKE_GROUNDEXP1				39
#define KIND_SMOKE_GROUNDEXP2				40
#define KIND_SMOKE_GROUNDEXP3				41
#define KIND_SMOKE_TORNADO					42
#define KIND_SMOKE_KIMBOMB1					43
#define KIND_SMOKE_KIMBOMB2					44
#define KIND_SMOKE_KIMBOMB3					45
#define KIND_SMOKE_MAGIC8K_EXP			    46
#define KIND_SMOKE_MAGIC6C1					47
#define KIND_SMOKE_MAGIC6C2					48
#define KIND_SMOKE_MAGIC6C3					49
#define KIND_SMOKE_MAGIC4C					50
#define KIND_SMOKE_MAGIC4CEXP				51
#define KIND_SMOKE_SNOWMAGIC				52
#define KIND_SMOKE_SNOWMAGICEXP				53
#define KIND_SMOKE_ADVTOWERKEXP1			54
#define KIND_SMOKE_ADVTOWERKEXP2			55
#define KIND_SMOKE_GROUNDFIRESMOKE			56
#define KIND_SMOKE_ARROWTRAIL				57
#define KIND_SMOKE_SPECIAL					58
#define KIND_SMOKE_WATEREFFECT				59
#define KIND_SMOKE_DUST						60
#define KIND_SMOKE_DETECTMINEEFFECT			61
#define KIND_SMOKE_CLONAPPEAR				62
#define KIND_SMOKE_CLONDIE					63
#define KIND_SMOKE_GOLDDRAGONELECTRON		64
#define KIND_SMOKE_GOLDDRAGONELECTRONEXP	65
#define KIND_SMOKE_GHOSTCATEXP				66
#define KIND_SMOKE_WAVEEFFECT				67
#define KIND_SMOKE_BIGTREE					68
#define KIND_SMOKE_MAGIC4T					69
#define KIND_SMOKE_YELLOWSAND				70
#define KIND_SMOKE_DARKFIRE					71
#define KIND_SMOKE_GHOST					72
#define KIND_SMOKE_PANDA					73
#define KIND_SMOKE_SMALL_TORNADO			74
#define KIND_SMOKE_RUNNINGFIRE				75
#define KIND_SMOKE_FIREATTACK				76
#define KIND_SMOKE_DOUBLE_SWORD				77			// ��Ͻ� �ְ˳���
#define KIND_SMOKE_LONG_SPEAR				78			// ����â ��â��ȯ
#define KIND_SMOKE_ICE_BARRIER				79			// ���� ������
#define KIND_SMOKE_LONG_SPEAR_HIT			80			// ����â ��â��ȯ
#define KIND_SMOKE_PAPERDOLL_HIT			81			// ����� �������� ��Ʈ

#define KIND_SMOKE_FLOWER_NEEDLE_RAIN1		82			// ��õȭ��1
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN2		83			// ��õȭ��2
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN3		84			// ��õȭ��3
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN4		85			// ��õȭ��4
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN5		86			// ��õȭ��5
#define KIND_SMOKE_FLOWER_NEEDLE_RAIN6		87			// ��õȭ��6
#define KIND_SMOKE_SOUL_CHANGE				88			// ���Ǽ�
#define KIND_SMOKE_SOUL_CHANGE_HIT			89			// ���Ǽ� ��Ʈ
#define KIND_SMOKE_STONE_MAGIC_STONE		90			// ������ ��
#define KIND_SMOKE_STONE_MAGIC_SMOKE		91			// ������ ����
#define KIND_SMOKE_GOD_SPEAR				92			// ��â�뵵(�ϰ��ʻ�)
#define KIND_SMOKE_CLOUD_WIND_HIT			93			// ǳ��� ��Ʈ
#define KIND_FIREENERGYPA_HIT1				94			// ����ǳ ��Ʈ1
#define KIND_FIREENERGYPA_HIT2				95			// ����ǳ ��Ʈ2
#define KIND_SMOKE_JINEMPEROR_HIT			96			// actdoll (2004/03/06 9:58) : �����/����Ȳ�� ����Ȳ��ȥ ��Ʈ
#define KIND_SMOKE_GUARDIAN_CAW				97			// �� 3�� ���� ����ũ.
#define KIND_SMOKE_GUARDIAN_TIGER			98			// ȣ���� ��Ʈ ����ũ.
#define KIND_SMOKE_GUARDIAN_CHICK			99


//----------------------------------------------------------------------------
// robypark (2004/05/18 11:04) : �߰� ����� SMOKE - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT	100			// ȭ����� ��Ʈ ȿ��(�븸-ȭ����, ����â 2�� ����)
#define KIND_SMOKE_POISON_ICE_BULLET_HIT	101			// ��, �ñ� ���� ��Ʈ ȿ��(�Ϻ�-�����, ���̸��� 2�� ����)
#define KIND_SMOKE_EARTHQUAKE_WAVE			102			// �Ϻ�-������, ���� ���� ������ ����Ʈ
#define KIND_SMOKE_THUNDER_BOLT_HIT			103			// ���� ���� ��Ʈ ȿ��(����-������, ���� 2�� ���� ���)
#define KIND_SMOKE_FOG_OF_ICE_HIT			104			// õ������ �ñ� ��Ʈ ȿ��(�Ϻ�-�����)
#define KIND_SMOKE_FOG_OF_POISON 			105			// õ������ ���Ȱ� ȿ��(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define KIND_SMOKE_FOG_OF_ICE				106			// õ������ �ñ� ȿ��(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define KIND_SMOKE_MUSHROOM_CLOUD			107			// ��õ�� ��� ���� ������ �������� ����ũ(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
#define KIND_SMOKE_HEAD_OF_DRAGON			108			// ���������� �� ȸ�� ����Ʈ(����-�ź���)
#define KIND_SMOKE_FIRE_ROUND_DANCE_FLOOR	109			// ���������� �ٴ� ����Ʈ(����-�ź���)
#define KIND_SMOKE_FIRERAIN_OF_GOD			110			// õ��ȭ�� �Һ�(�븸-��Ȳ����)
#define KIND_SMOKE_CREATE_VOLCANO			111			// �ݳ뿰�� ȭ�� ����(�Ϻ�-������)
#define KIND_SMOKE_VOLCANO_HIT				112			// �ݳ뿰�� ȭ��ź ��Ʈ(�Ϻ� ������)

// robypark 2004-6-3 15:43
// ��õ�� ���� ���� ����
#define KIND_SMOKE_BOMB_SMOKE1				113
#define KIND_SMOKE_BOMB_SMOKE2				114
#define KIND_SMOKE_BOMB_SMOKE3				115

// robypark 2004/6/4 19:21
// �Ҷ������� ��� ��õ��� ��ź ���� smoke
#define KIND_SMOKE_ROAR_OF_LAUGHTER_HIT		116	

// robypark 2004/6/18 11:54
// ������ �ݳ뿰�� ��� ���� �����Ǵ� ȭ�� ���� ���׸� ����(����)
#define KIND_SMOKE_CREATE_VOLCANO_DUST		117

// robypark 2004/11/15 15:38
// ������ ���� �߿� ���� ��Ʈ ȿ��
#define KIND_SMOKE_SIEGEWARFARE_IN_FIGHT	118

// robypark 2004/11/24 17:34
// ������ �߼��� �� ��Ʈ ȿ��
#define KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT	119

// robypark 2004/12/6 11:27
// ������ �ú� ȭ�� ��Ʈ ȿ��
#define KIND_SMOKE_SIEGEWARFARE_ARROW_HIT	120

#define MAX_SMOKE_KIND_NUMBER				128


#define ORDER_SMOKE_EXP     1

// ���ο� Smoke�߰��� HQ�ȿ� ���� InfoSmoke.txt�� �߰����ָ�, �⺻���� ������ ���´�.

// ȿ�� ��� Ŭ���� smoke.cpp�� �ִ� �ڵ带 �̵�
class _SmokeKindInfo
{
	public:
		void Init(CHAR *file, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI32 siyindent);

		// robypark 2004/9/30 18:12
		// Smoke ������ HQ���� InfoSmoke.txt���� ���� ����Ѵ�.
		void Init(char *pszKindID);

		SHORT	m_ssFile, m_ssStartFont;
		SHORT	m_ssMoveFrameNumber;
		SHORT	m_ssMaxRepeatNumber;
		SHORT	m_ssRisingSwitch;
		SI32	m_siTransparentMode;
		SI32	m_siStepDelay;
		SI32	m_siYIndent;


};

class _Smoke
{
public:
	SHORT m_ssID;
	SHORT m_ssKind;
	SHORT m_ssAnimation;
	SHORT m_ssStep;
	SHORT m_ssFile, m_ssFont;
	SI32  m_siStepDelay;			// �ִϸ��̼��� �ӵ�.
	char m_scArea;                        // ���߿� �ִ°� �ƴϸ� ���� �ִ°�?
	SHORT  m_ssCurrentOrder;               // ���� ĳ���Ͱ� �ϰ� �ִ� �۾� 
	DWORD m_uiLastChangedFrame;           // �ִϸ��̼��� ����� ������ 

	SHORT m_ssX, m_ssY;

	char m_scDrawSwitch;                   // �׷��� ���ΰ� �����ΰ�?
	char m_scReverseSwitch;                // ����� ���� ���ΰ�?
	SHORT  m_ssDrawXsize,  m_ssDrawYsize;       // �׸��� ���� ���� ������ 
	SHORT  m_ssDrawStartX, m_ssDrawStartY;      // �׸��� �׷����� ���� ��ġ 
	SHORT m_ssCenterX, m_ssCenterY;
	SI32  m_ssTransparentMode;          // ����ȿ���� ���� ���ΰ� ?

	SHORT m_ssPlayerNumber;
	SHORT m_ssApower;
	SI16  m_siApower;

	// robypark 2004-5-24, 16:37 �������� ���� ���� ȿ���� ���Ͽ� �߰�
	SI08	m_bFloorSmoke;		// �ش� Smoke�� ����� �ٴ�(Floor), �� ������Ʈ ���� �׷��� �� ��� TRUE�� �����Ѵ�.

	// robypark 2004-6-3 15:54
	// ������ �ݳ뿰�� ��� ���� �����Ǵ� ȭ���� ���� �÷���
	// smoke �ִϸ��̼��� �����°�? �� ��Ÿ���� �÷���
	// value: true = �ִϸ��̼��� ������.
	//		: false = �ִϸ��̼��� ������ �ʾҴ�.
	SI08	m_bIsDoneAnimation;

	// �̵����� 
	char m_scMoveFrameNumber;
 
	SHORT m_ssRealX, m_ssRealY;
	SHORT m_ssStartFont;

	SHORT m_ssMaxRepeatNumber;
	SHORT m_ssRepeatNumber;

	SHORT m_ssRisingSwitch;

	_NewID m_AttackID; // �ǰ����� 
	_NewID m_AttackerID; // ������. 
	float  m_siSpeed;

	_Smoke();
	void Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, char area);
	BOOL Action();
	BOOL Draw();
    void SetInfo();
	// ���� ��ġ�� ����Ѵ�. 
    void CalculateRealXY();
	void CalculateRealXYNormal();

	void CalculateDrawStartXY();

	void DecideFont();
	void DecideFontNormal();

};

// Smoke�� �����Ѵ�.
// SI16 siKind[in]: SMOKE ����
// SI16 id[in]: Smoke ID
// SI16 x[in]: Smoke�� ������ ��ġ(x)
// SI16 y[in]: Smoke�� ������ ��ġ(y)
// SI08 area[in]:
// SI16 siPlayerNumber[in]: Smoke�� ������ �÷��̾� ��ȣ(default -1)
//
// robypark 2005-5-24, 16:37 �������� ���� ���� ȿ���� ���Ͽ� �߰�
// SI08 bFloorSmoke[in]: �ش� Smoke�� ����� �ٴ�(Floor), �� ������Ʈ ���� �׷��� �� ��� TRUE�� �����Ѵ�.
//					(default FALSE)
SHORT SetSmoke(SI16 siKind, SI16 id, SI16 x, SI16 y, SI08 area, SI16 siPlayerNumber = -1, SI08 bFloorSmoke = FALSE);
void DeleteSmoke();
void DeleteSmoke(SHORT id);
SHORT FindEmptySmoke();
BOOL IsLiveSmoke(SHORT id);
void SetSmokeRealXY(SHORT id, SHORT realx, SHORT realy);

// robypark 2004-6-3 15:54
// ������ �ݳ뿰�� ��� ���� �����Ǵ� ȭ���� ���� �÷��� ���
// smoke �ִϸ��̼��� �����°�? �� ��Ÿ���� �÷��� ���
// return value: true = �ִϸ��̼��� ������.
//			   : false = �ִϸ��̼��� ������ �ʾҴ�.
SI08 GetSmoke_IsDoneAnimation(SI32 siID);

void InitSmokeKindInfo();

// 2001.11.12 hojae_appned
BOOL Smoke_Action(SI32 i);
void Smoke_CalculateDrawStartXY(SI32 i);
void Smoke_Draw(SI32 i);
void Smoke_PlayerNumber(SHORT smokeid, SHORT PlayerNumber);
void Smoke_Apower(SHORT smokeid, SHORT level);
void Smoke_AttackID(SHORT smokeid, _NewID id);
void Smoke_AttackerID(SHORT smokeid, _NewID id);
void Smoke_Fwrite(FILE* fp);
void Smoke_Read(FILE* fp);
void Smoke_SetYIndent(SI16 kind,SI32 YIndent);
void SetApower(SI16 smokeid,SI16 siApower);

// robypark (2004-5-24, 17:02)
// �ٴ�(Floor) Smoke�� ��� �׷��ִ� ������ �����ϱ� ���Ͽ� _Smoke�� m_bFloorSmoke������ 
// �̿��ϱ� ���Ͽ� �߰��Ǿ���.
extern _Smoke  g_Smoke[MAX_SMOKE_NUMBER];   // ����� ������ 

extern _SmokeKindInfo g_SKI[MAX_SMOKE_KIND_NUMBER];

#endif









