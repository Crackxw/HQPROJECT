#ifndef _CHEAT_H
#define _CHEAT_H   1

#include "order.h"

//////////////////////////////////////////
// ġƮŰ �ڵ�                          //
//////////////////////////////////////////
#ifdef _ENGLISH
	#define SEE_CHEAT_KEY_CODE        "see"									// �������� ��� �÷��̾��� �Ȱ��� �� �ȴ´�. 
	#define SPEED_CHEAT_KEY_CODE      "speed"								// ���� �ӵ��� ������ ���ش�. 
	#define MONEY_CHEAT_KEY_CODE      "money"								// ���� �÷��ش�.
	#define WIN_CHEAT_KEY_CODE        "win"									// �¸� 
	#define GOD_CHEAT_KEY_CODE        "god"									// ���� 
	#define UPGRADE_CHEAT_KEY_CODE    "hup"									// ��� ���׷��̵带 �Ϸ��Ѵ�. 
	#define LEVEL_CHEAT_KEY_CODE      "hlevel"								// ����� ������ ��� ���δ�. 
	#define MERCHANT_CHEAT_KEY_CODE   "hmerchant"							// ������ �ҷ��´�. 
	#define GOODS_CHEAT_KEY_CODE      "hgoods"								// ��ǰ�� ��� ä���. 
	#define LOSE_CHEAT_KEY_CODE       "hlose"								// �й� 
	#define MAP_CHEAT_KEY_CODE        "hmap"								// ���� ���� 
	#define STATUS_CHEAT_KEY_CODE     "hstatus"								// ĳ������ ������ ǥ�����ش�.  
	#define ATTACKINFO_CHEAT_KEY_CODE "hattack"								// ��ǻ���� ���� ���� 
	#define RAIN_CHEAT_KEY_CODE       "hrain"								// �� ���� �����.
	#define WAVEMUSIC_CHEAT_KEY_CODE  "wavemusic"							// ���� �� ��������� wav�� �����Ѵ�.
	#ifdef _IMJIN2PLUS
		#define JUSTICEK_CHEAT_KEY_CODE   "hjustice"						// ���� ������ �Ǻ� 12������ �ɴ´�.
	#endif
#endif

#ifndef _ENGLISH
	#ifdef _IMJIN2PLUS
		#define SEE_CHEAT_KEY_CODE			"¸�ϰ��ض㳯���ƿ´ܴ�"				// �������� ��� �÷��̾��� �Ȱ��� �� �ȴ´�. 
		#define SPEED_CHEAT_KEY_CODE		"���࿭��"							  // ���� �ӵ��� ������ ���ش�. 
		#define MONEY_CHEAT_KEY_CODE		"�߻�ƺ���"						  // ���� �÷��ش�.
		#define WIN_CHEAT_KEY_CODE			"���ݽô�"							  // �¸� 
		#define GOD_CHEAT_KEY_CODE			"�ǹ���û��"						  // ���� 
		#define RAIN_CHEAT_KEY_CODE			"�񳻸��¿�����"					  // �� ���� �����.
		#define GOODS_CHEAT_KEY_CODE		"���״뿡�Ը�ε帮��"				   // ��ǰ�� ��� ä���. 
		#define LEVEL_CHEAT_KEY_CODE		"�������ݼ���"					  // ����� ������ ��� ���δ�. 
		#define JUSTICEK_CHEAT_KEY_CODE		"���ѵ�������"						  // ���� ������ �Ǻ� 12������ �ɴ´�.
	#else
		#define SEE_CHEAT_KEY_CODE			"�������ʴ�����"					  // �������� ��� �÷��̾��� �Ȱ��� �� �ȴ´�. 
		#define SPEED_CHEAT_KEY_CODE		"�Ѿ�ź�糪��"						  // ���� �ӵ��� ������ ���ش�. 
		#define MONEY_CHEAT_KEY_CODE		"��������Ƣ���"					  // ���� �÷��ش�.
		#define WIN_CHEAT_KEY_CODE			"�̺��ٴ��������¾���"				   // �¸� 
		#define GOD_CHEAT_KEY_CODE			"�����������;���"				      // ���� 
		#define RAIN_CHEAT_KEY_CODE			"����³��Ǽ�äȭ"				      // �� ���� �����.
		#define GOODS_CHEAT_KEY_CODE		"�����ҽ��ݻ��"					  // ��ǰ�� ��� ä���. 
		#define LEVEL_CHEAT_KEY_CODE		"������°����"					      // ����� ������ ��� ���δ�. 
	#endif

	#define UPGRADE_CHEAT_KEY_CODE			"hup"								// ��� ���׷��̵带 �Ϸ��Ѵ�. 
	#define MERCHANT_CHEAT_KEY_CODE			"hmerchant"							// ������ �ҷ��´�. 
	#define LOSE_CHEAT_KEY_CODE				"hlose"								// �й� 
	#define MAP_CHEAT_KEY_CODE				"hmap"								// ���� ���� 
	#define STATUS_CHEAT_KEY_CODE			"hstatus"							// ĳ������ ������ ǥ�����ش�.  
	#define ATTACKINFO_CHEAT_KEY_CODE		"hattack"							// ��ǻ���� ���� ���� 
	#define WAVEMUSIC_CHEAT_KEY_CODE		"wavemusic"							// ���� �� ��������� wav�� �����Ѵ�.
#endif

// �Ż� �׽�Ʈ�� ġƮŰ
//#define TEST_ONLINE_CHEAT_KEY				"xxx�Ż�"	// �ּ�ó���� ��
#ifdef TEST_ONLINE_CHEAT_KEY
	#define TEST_ARMOR_CHEAT_KEY_CODE		"����"
	#define TEST_WEAPON_CHEAT_KEY_CODE		"����"
	#define TEST_HELMET_CHEAT_KEY_CODE		"����"
	#define TEST_CHAR_CHEAT_KEY_CODE		"ĳ����"
	#define TEST_CONTINUE_CHEAT_KEY_CODE	"���"
#endif

//////////////////////////////////////////


class _Cheat 
{
public:
	
	int SeeCheatKeySwitch;
	SHORT CheatSeeSwitch;

	int StatusCheatKeySwitch;
	SHORT CheatStatusSwitch;

	int SpeedCheatKeySwitch;
	SHORT CheatSpeedSwitch;

	int MoneyCheatKeySwitch;

	int ArmorCheatKeySwitch;
	int HelmetCheatKeySwitch;
	int WeaponCheatKeySwitch;
	int CharCheatKeySwitch;
	int ContinueCheatKeySwitch;
	
	int RainCheatKeySwitch;
	SHORT CheatRainSwitch;

	int UpgradeCheatKeySwitch;
	SHORT CheatUpgradeSwitch;

	int LevelCheatKeySwitch;
	SHORT CheatLevelSwitch;

	int MerchantCheatKeySwitch;
	SHORT CheatMerchantSwitch;

	int GoodsCheatKeySwitch;
	SHORT CheatGoodsSwitch;

	int WinCheatKeySwitch;
	SHORT CheatWinSwitch;

	int LoseCheatKeySwitch;
	SHORT CheatLoseSwitch;

	int AttackInfoCheatKeySwitch;
	SHORT CheatAttackInfoSwitch;

	int GodCheatKeySwitch;
	SHORT CheatGodSwitch;

	int MapCheatKeySwitch;
	SHORT CheatMapSwitch;

	int WavemusicCheatKeySwitch;
	SHORT CheatWavemusicSwitch;

	BOOL JusticeKCheatKeySwitch;

	BOOL OnlineCheatKeySwitch;
	BOOL CheatOnlineSwitch;

    int MagicPowerCheatKeySwitch;
    int TimeCheatKeySwitch;
    int HurryCheatKeySwitch;
    int SkillCheatKeySwitch;
	int NoLoseCheatKeySwitch;
	int ShieldCheatKeySwitch;

	void Init();
	void CheatOrder();
	void IsCheatKeyCodeInput(char* text);
	void CheatOperation(_OrderData order);
};

void MakeItSee();

// �������� ���� ȸ�������ش�. 
void RecoverMagic();
// ������ �ִ�� �÷��ش�.
void FullLevel();
// ����  �÷��ش�.
void IncreaseMoney();
void WinStage();
void TimeCheat();
void Hurry();
// ���� ����� �ְ�� �÷��ش�.
void Skill();
// �й� ���ϱ� 
void NoLose();
// ���� ��� ���ֱ� 
void Shield();

#endif