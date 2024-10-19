#ifndef _MAGIC_H
#define _MAGIC_H


// ������ �ʿ��� ������
#define MAGICPOWER_BETRAY     				60		// ��Ȥ�� �ʿ��� ������ 
#define MAGICPOWER_ABSORB     				40		// ����� �ʿ��� ������ 
#define MAGICPOWER_MAGIC8J     				50		// ȯ������ �ʿ��� ������ 
#define MAGICPOWER_EARTHQUAKE 				40		// ������ �ʿ��� ������ 
#define MAGICPOWER_TORNADO    				100		// ǳ����� �ʿ��� ������ 
#define MAGICPOWER_LIGHTNING  				40		// ������ �ʿ��� ������ 
#define MAGICPOWER_PROTECTOR  				50		// ��ȣ���� �ʿ��� ������ 
#define MAGICPOWER_STEALTECH  				90		// ��� ��ġ�⿡ �ʿ��� ������ 
#define MAGICPOWER_RAINMAKE   				140		// ����� 
#define MAGICPOWER_WARP       				30		// �����̵� 
#define MAGICPOWER_HEAL       				20		// ġ�� 
#define MAGICPOWER_MANA						20		// ���� ġ��.
#define MAGICPOWER_STEALEXP   				40
#define MAGICPOWER_CLON       				80		// �ڽ��� �����ϴ� ������
#define MAGICPOWER_ENEMYCLON       			40 		// ���� �����ϴ� ������
#define MAGICPOWER_SHIELD					80		// ���� ���ִµ� �Ҹ�Ǵ� ������
#define MAGICPOWER_GROUNDEXP				100		// ������ ����µ� �ʿ��� ������ 
#define MAGICPOWER_MAGIC4C					50      // �������. 
#define MAGICPOWER_MAGIC6C					100		// ���ڼ� ����µ� �ʿ��� ������ 
#define MAGICPOWER_KIMBOMB					100		// ����� ���ڰ� ����µ� �ʿ��� ������ 
#define MAGICPOWER_MAGIC5J					50		// ���������� ����µ� �ʿ��� ������ 
#define MAGICPOWER_DETECTMINE				40  
#define MAGICPOWER_DRAGON					200     // ��õ�� ��ȯ�� �Ҹ�Ǵ� ����.   
#define MAGICPOWER_TOADPOISON				20      /// ���β��� �ߵ� ��Ű�µ� �ʿ��� ����. 
#define MAGICPOWER_JUSTICEK					2       /// ������ ���� �����Ű�µ� �ʿ��� ����. 
#define MAGICPOWER_WIDOW					10      /// �Ź̿䱫 �Ź��� ����. 
#define MAGICPOWER_WAVE						20		// �ٴٰź����� �ĵ� ���� . 
#define MAGICPOWER_FIRE						10		// ȭ�� ���ݿ� �Ҹ�Ǵ� ����. 
#define MAGICPOWER_MAGIC8K					2		// ���������� 
#define MAGICPOWER_FIREARROW2				2		// ������ȭ��
#define MAGICPOWER_EXTINGUISH				10		// ��ȭ�� �ʿ��� ����. 
#define MAGICPOWER_MAGIC4T    				100		// ���ɺм��(��������)�� �ʿ��� ������ 
#define MAGICPOWER_MAGIC2T    				100		//�������� �ʿ��� ������ 
#define MAGICPOWER_RECALL_PANDA				300     //�Ҵ� ��ȯ�� �ʿ��� ������
#define MAGICPOWER_RECALL_MONKEY			100     //������ ��ȯ�� �ʿ��� ������ 
#define MAGICPOWER_RECALL_AMAKUSA			10		//����� ��ȯ�� �ʿ��� ������.
#define MAGICPOWER_MINE						2		//���� �ż�.
#define MAGICPOWER_YELLOWSAND				100		//���ǽ��� Ȳ�� ����.
#define MAGICPOWER_ICE						100	    // �̻��� ��� Ư�����.
#define MAGICPOWER_BIGTREE					50		// �Ŵ�� Ư������. 
#define MAGICPOWER_SOUL						80		// �հ��� �߼��� ȥ.
#define MAGICPOWER_GHOST					30		// �丣������ �ͽ�ź.
#define MAGICPOWER_DARKFIRE					100		// ������ ����ȭ�� ��ǳ.
#define MAGICPOWER_IVORY					30		// �������.
#define MAGICPOWER_BLUEDRAGON				30		// ȭ������.
#define MAGICPOWER_RUNNINGFIRE				150		// ������ź.
#define MAGICPOWER_ENERGYPA					20		// ��ǳ.
#define MAGICPOWER_SNOWATTACK				30		//
#define MAGICPOWER_FIREATTACK				50		// ȭ������.
#define MAGICPOWER_FIREENERGYPA				100		// ��ȭ��ǳ(���Ȳ���).
#define MAGICPOWER_RECALL_TIGER				50		// ��ȣ�� ���� ȣ���� ��ȯ.

#define MAGICPOWER_DOUBLE_SWORD				100		// �ְ˳���
#define MAGICPOWER_LONG_SPEAR				300		// ��â��ȯ
#define MAGICPOWER_ICE_BARRIER				50		// ������
#define MAGICPOWER_FLOWER_NEEDLE_RAIN		100		// ��õȭ��
#define MAGICPOWER_SOUL_CHANGE				100		// ���Ǽ�
#define MAGICPOWER_STONE_MAGIC				60		// ������
#define MAGICPOWER_GOD_SPEAR				40		// ��â�뵵
#define MAGICPOWER_CLOUD_WIND				20		// ǳ���
#define MAGICPOWER_MANABURN					0		// ������


//----------------------------------------------------------------------------
// robypark (2004/05/17 13:57) : �߰� ��� �Ҹ� ������- ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
#define MAGICPOWER_FIRE_ROUND_DANCE			200		// ����������(����-�ź���, �̼��� 2�� ���� ���)
#define MAGICPOWER_NOVA						250		// ����(����-������, ���� 2�� ���� ���)
#define MAGICPOWER_FURY_OF_VOLCANO			200		// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
#define MAGICPOWER_POISONING_HEAVENS		250		// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
#define MAGICPOWER_BOMB_OF_HEAVEN			200		// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
#define MAGICPOWER_SIEGE_MODE				200		// ��õ���(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)
#define MAGICPOWER_ONSLAUGHT_FIREBALL		200		// �Ͱ�ȭ����(�븸-ȭ����, ����â 2�� ���� ���)
#define MAGICPOWER_FIRERAIN_OF_GOD			225		// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) => �����Һ� 250=>225 robypark 2004/11/2 11:34

// robypark 2004/6/4 14:12
#define MAGICPOWER_SIEGE_MODE_UNDO			0		// ��õ���=>�Ҷ������� ����(�߱�-�Ҷ�������, �̷�<=���� 2�� ���� ���)

// robypark 2004/11/11 11:17
#define MAGICPOWER_SIEGE_MODE_FIRE_MISSILE	25		// �߱� �Ҷ������� ��õ����� �� ���� �̻��� �߻� �Ҹ� ������

////////////////////////////////////////////////////////////////////////////
#define MAGICPOWER_MAGICTEST				1       //�׽�Ʈ ������ �ʿ��� ������.
////////////////////////////////////////////////////////////////////////////


class _Magic {
public:
	SI32 m_siMagicKind;
	SI32 m_siX, m_siY;
	_NewID m_MagicID;
	SI32 m_siMagicPlayerNumber;
	SI32 m_siArea;
	SI32 m_siApower;
	_NewID m_AttackID;

	SI32 m_siStep;
	SI32 m_siHitRate;

	SI32 m_siSmokeID;

	_Magic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	virtual SI32 Action();
};


class _KimBomb : public _Magic{
public:

	_KimBomb(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _Magic5j : public _Magic{
public:

	_Magic5j(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32	Action();
	_NewID	FindNearEnemy(SI32 six, SI32 siy);
};


class _MagicExtinguish: public _Magic{
public:

	_MagicExtinguish(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, _NewID AttackID);
	SI32 Action();
};

class _MagicDetectMine: public _Magic{
public:

	_MagicDetectMine(SI32 simagickind, SI32 siX, SI32 siY, SI32 siplayernumber);
	SI32 Action();
};

class _Magic6c : public _Magic{
public:

	_Magic6c(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// �������. 
class _Magic4c : public _Magic{
public:

	_Magic4c(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//�������.
class _MagicIvory : public _Magic{
public:
	_MagicIvory(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicBlueDragon : public _Magic{

public:
	_MagicBlueDragon(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicFireAttack : public _Magic{

public: 
	_MagicFireAttack(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

class _MagicRunningFire : public _Magic{

public:
	_MagicRunningFire(SI32 siMagicKind,SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();

	SI16	m_siCount;
};

class _MagicEnergyPa : public _Magic{

public:
	_MagicEnergyPa(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();

};

class _MagicFireEnergyPa : public _Magic{

public:
	_MagicFireEnergyPa(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action(); 


};

class _MagicIce : public _Magic{
public:
	_MagicIce(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//���� �׽�Ʈ.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class _MagicTest : public _Magic{
public:
	_MagicTest(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ����ǳ. 
class _SnowMagic : public _Magic{
public:

	_SnowMagic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// ������. 
class _Magic2T : public _Magic{
public:

	_Magic2T(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

// �Ŵ�� Ư������. 
class _BigTree : public _Magic{
public:

	_BigTree(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//�ְ˳���.
class _MagicDoubleSword : public _Magic{
public:
	_MagicDoubleSword(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//������.
class _MagicManaBurn : public _Magic{
public:
	_MagicManaBurn(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//��â��ȯ.
class _MagicLongSpear : public _Magic{
public:
	_MagicLongSpear(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//������.
class _MagicIceBarrier : public _Magic{
public:
	SI32 m_siBlockX[9], m_siBlockY[9];

	_MagicIceBarrier(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//��õȭ��
class _MagicFlowerNeedleRain : public _Magic{
public:
	SI32 m_siSmokesID[4];

	_MagicFlowerNeedleRain(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//���Ǽ�
class _MagicSoulChange : public _Magic{
public:
	_MagicSoulChange(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//������
class _MagicStoneMagic : public _Magic{
public:
	_MagicStoneMagic(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//��â�뵵
class _MagicGodSpear : public _Magic{
public:
	_MagicGodSpear(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//ǳ���
class _MagicCloudWind : public _Magic{
public:
	_MagicCloudWind(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
	SI32 Action();
};

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:17) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
// ����������(����-�ź���, �̼��� 2�� ���� ���)
class _Magic_FIRE_ROUND_DANCE : public _Magic
{
	public:
		// ������
		_Magic_FIRE_ROUND_DANCE(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// ����(���) ���� ����
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;		// ���������� ��� ���� ���� �ð�
		SI32				m_siFloorSmokeID;	// ���������� �ٴ� Smoke ID
};

// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
class _Magic_FURY_OF_VOLCANO : public _Magic
{
	public:
		// ������
		_Magic_FURY_OF_VOLCANO(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// ����(���) ���� ����
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;			// �ݳ뿰�� ��� ���� ���� �ð�
};

// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
class _Magic_POISONING_HEAVENS : public _Magic
{
	public:
		// ������
		_Magic_POISONING_HEAVENS(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// ����(���) ���� ����
		SI32 Action();
};

// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
class _Magic_BOMB_OF_HEAVEN : public _Magic
{
	public:
		// ������
		_Magic_BOMB_OF_HEAVEN(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// ����(���) ���� ����
		SI32 Action();
};

// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
class _Magic_FIRERAIN_OF_GOD : public _Magic
{
	public:
		// ������
		_Magic_FIRERAIN_OF_GOD(SI32 siMagicKind, SI32 siX, SI32 siY, _NewID NewID, \
									SI32 siPlayerNumber, SI32 siArea, SI32 siApower, \
									SI32 siHitRate, _NewID AttackID);

		// ����(���) ���� ����
		SI32 Action();

	protected:
		UI32				m_uiBeginTime;		// õ��ȭ�� ��� ���� ���� �ð�
};



void MagicAction();
void DeleteMagic();
SI32 FindEmptyMagic();


void SetKimBomb(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagic5j(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicExtinguish(SI32 siX, SI32 siY);
void SetMagic6c(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagic4c(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIvory(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicBlueDragon(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFireAttack(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicRunningFire(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIce(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetSnowMagic(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicMine(SI32 siX, SI32 siY);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMagicTest(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMagic2T(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicDetectMine(SI32 siX, SI32 siY, SI32 playernumber);
void SetBigTree(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicEnergyPa(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFireEnergyPa(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicDoubleSword(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicManaBurn(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicLongSpear(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicIceBarrier(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicFlowerNeedleRain(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicSoulChange(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicStoneMagic(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicGodSpear(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);
void SetMagicCloudWind(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:17) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
// ����������(����-�ź���, �̼��� 2�� ���� ���)
void SetMagic_FIRE_ROUND_DANCE(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// �ݳ뿰��(�Ϻ�-������, ��Ű��ī 2�� ���� ���)
void SetMagic_FURY_OF_VOLCANO(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// õ������(�Ϻ�-�����, ���̸��� 2�� ���� ���)
void SetMagic_POISONING_HEAVENS(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// ��õ��(�߱�-�߼���, ����<=�̷� 2�� ���� ���)
void SetMagic_BOMB_OF_HEAVEN(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

// õ��ȭ��(�븸-��Ȳ����, �弱ȭ 2�� ���� ���) 
void SetMagic_FIRERAIN_OF_GOD(SI32 siX, SI32 siY, _NewID NewID, SI32 siPlayerNumber, SI32 siArea, SI32 siApower, SI32 siHitRate, _NewID AttackID);

#endif
