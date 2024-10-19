#ifndef _MERCHANT_H
#define _MERCHANT_H

#define MERCHANT_INTERVAL    1000

// 3���� ������ �����Ѵ�. 
#define MAX_MERCHANT_TYPE	   3
#define MERCHANT_TYPE_KOREA    0
#define MERCHANT_TYPE_JAPAN	   1
#define MERCHANT_TYPE_CHINA    2

#define MAX_GOODS_KIND		50
#define GOODS_FOOD			0
#define GOODS_TREE			1
#define GOODS_MINE			2        // �������� ���� (��)
#define GOODS_RAINMAKER		3        // ������� �ֹ� (����, �Ϻ�)
#define GOODS_SAMARROW		4        // ��� ȭ��(�Ϻ� ����)
#define GOODS_ELEPHANT		5        // �ڳ��� ���� (��)
#define GOODS_AIRMAGIC		6        // ����ź (����) 
#define GOODS_BULL			7        // Ȳ��   (����)
#define GOODS_DRAGONBALL	8		 // ������   (��)
#define GOODS_GINSENG		9        // �λ�   (����, �Ϻ�, ��)
#define GOODS_BOW1			10       // ���
#define GOODS_BOW2			11       // ö�±� 
#define GOODS_RIFLE1		12       // �����
#define GOODS_GUN1			13       // �巡�����̽�
#define GOODS_CANNON1		14		 // ö����
#define GOODS_SWORD1		15       // ûȫ��
#define GOODS_SWORD2		16       // ��������
#define GOODS_SWORD3		17       // ��糪���� ��
#define GOODS_SWORD4		18       // �пյ�
#define GOODS_SWORD5		19       // ĥ����
#define GOODS_SWORD6		20       // �ε���հ�
#define GOODS_SWORD7		21       // ���ð�
#define GOODS_SPEAR1		22       // ��õȭ��
#define GOODS_SPEAR2		23       // ��öâ
#define GOODS_SPEAR3		24       // ���Ȼ��
#define GOODS_AXE1			25       // �����ε�
#define GOODS_BOOK1			26       // �����﷫
#define GOODS_BOOK2			27       // ���ں���
#define GOODS_BOOK3			28       // ȭŸ��å
#define GOODS_NECKLACE1		29       // �޸��� ����
#define GOODS_ARMOUR1		30       // ���
#define GOODS_ARMOUR2		31       // ��⸰����
#define GOODS_ARMOUR3		32       // ��������
#define GOODS_ARMOUR4		33       // ����հ���
#define GOODS_ARMOUR5		34       // ī�ɹ����� ����
#define GOODS_BELT1			35       // ���丶�� ����
#define GOODS_BELT2			36       // ȭ��õ���� �㸮��
#define GOODS_MED1			37       // �鼳���
#define GOODS_MED2			38       // �����
#define GOODS_MED3			39       // �����
#define GOODS_MED4			40       // ������
#define GOODS_MED5			41       // ��ȭź
#define GOODS_STICK1		42       // ���������
#define GOODS_STICK2		43		 // ���ȱ͸���
#define GOODS_MACE1			44       // ��ö����
#define GOODS_CLAW1			45       // �氥����

class _GoodsEffect{
public:
	// �ʿ� ���� 
	SI32 siNeedKind;			// ��ǰ ������ ���� - �ʿ� ����. 

	// % ����. 
	SI32 siApower;              // (%)
	SI32 siAttackRange;			// (%)
	SI32 siDpower;				// (%)
	SI32 siStealHealth;         // (%) ���� ü���� ���Ѿ� ���� ����. 
	SI32 siGeneralEffectApower;	// (%) ��� ȿ��(���ݷ� ���)
	SI32 siGeneralEffectDpower;	// (%) ��� ȿ��(���� ���)
	SI32 siMagicpower;			// (%) ������ ���
	SI32 siMoveSpeed;			
	SI32 siHealth;	

	
	// ���� ��ġ ����. 
	SI32 siMagicPowerAbs;		
	SI32 siHealthRecoverAbs;	// ü���� ȸ���Ǵ� �ֱ�(������ ����)


	// ����ġ 
	BOOL bRecoverMagicPowerSwitch ;
	BOOL bStrongAttackSwitch ;			// �ϰ��ʻ� ���� ����. 
	BOOL bKimBombSwitch;				// ���ڰ� ��� ���� ����. 
	BOOL bMagic5jSwitch;				// ���������� ��� ���� ����. 
	BOOL bCallDragonSwitch;				// ��õ�� ��ȯ. 
	BOOL bMagic8kSwitch;				// ���� ������ .
	BOOL bGeneralEffectFireResistSwitch; // ���ȿ�� (�ҿ����� ����)
	BOOL bClonSwitch;						// �нż��� ����ϴ���...
	BOOL bRecoverHealthSwitch;				// ü�� ȸ�� . 
	BOOL bExtinguishSwitch;				// �ֺ��� ���� ����. 

	// 

	void Init(){ZeroMemory(this, sizeof(_GoodsEffect)); }
};

class _Goods{
public:
	SHORT	Money, Tree;
	SI32	siBuyUnit;			// �Ǹ� ���� 

	SI32    siFont;
	SI32    siPercent;

	char	Text[32];
	DWORD	Property;

	_GoodsEffect GE;

	void Init(SHORT money, SHORT tree, SI32 sipercent, SI32 sifont, SI32 sibuyunit, char* text, SI32 property, _GoodsEffect *ge);

};




class _PlayerMerchant{
public:
	SI32 PlayerNumber;

	SHORT MerchantSwitch;        // ������ ��򰡿� �����ϴ°�?
	SI32  MerchantType ;         // ������ ���� (��������, �Ϻ�����, ����ε�...)
	DWORD MerchantFrame;         // ������ �����ϱ� ������ ������ 

	SHORT Goods[MAX_GOODS_KIND];             // ��ǰ�� ����   

	// ��ǰ�� �غ��Ѵ�. 
    void ReadyGoods();
	void Action();
	void MoveToPlayer();
	// �� ��ǰ�� ������ ������ �ʿ䰡 �ִ��� Ȯ���Ѵ�. 
	BOOL CanBuyGoods(SI32 sigood);

};
// ���� Ŭ���� 
class _Merchant {
public:
	_PlayerMerchant PM[MAX_MULTI_PLAYER];

	void Init();

	void Action();
};

BOOL GetItemHelp(SI32 goods, char* helpstring);

void GoodsInit();

#endif