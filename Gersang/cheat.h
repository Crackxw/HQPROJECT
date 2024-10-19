#ifndef _CHEAT_H
#define _CHEAT_H   1

#include "order.h"

//////////////////////////////////////////
// 치트키 코드                          //
//////////////////////////////////////////
#ifdef _ENGLISH
	#define SEE_CHEAT_KEY_CODE        "see"									// 게임중인 모든 플레이어의 안개를 다 걷는다. 
	#define SPEED_CHEAT_KEY_CODE      "speed"								// 생산 속도를 빠르게 해준다. 
	#define MONEY_CHEAT_KEY_CODE      "money"								// 돈을 올려준다.
	#define WIN_CHEAT_KEY_CODE        "win"									// 승리 
	#define GOD_CHEAT_KEY_CODE        "god"									// 무적 
	#define UPGRADE_CHEAT_KEY_CODE    "hup"									// 모든 업그레이드를 완료한다. 
	#define LEVEL_CHEAT_KEY_CODE      "hlevel"								// 장수의 레벨을 모두 높인다. 
	#define MERCHANT_CHEAT_KEY_CODE   "hmerchant"							// 상인을 불러온다. 
	#define GOODS_CHEAT_KEY_CODE      "hgoods"								// 물품을 모두 채운다. 
	#define LOSE_CHEAT_KEY_CODE       "hlose"								// 패배 
	#define MAP_CHEAT_KEY_CODE        "hmap"								// 지도 켑춰 
	#define STATUS_CHEAT_KEY_CODE     "hstatus"								// 캐릭터의 정보를 표시해준다.  
	#define ATTACKINFO_CHEAT_KEY_CODE "hattack"								// 컴퓨터의 공격 정보 
	#define RAIN_CHEAT_KEY_CODE       "hrain"								// 비가 오게 만든다.
	#define WAVEMUSIC_CHEAT_KEY_CODE  "wavemusic"							// 게임 중 배경음악을 wav로 연주한다.
	#ifdef _IMJIN2PLUS
		#define JUSTICEK_CHEAT_KEY_CODE   "hjustice"						// 본영 주위에 의병 12마리를 심는다.
	#endif
#endif

#ifndef _ENGLISH
	#ifdef _IMJIN2PLUS
		#define SEE_CHEAT_KEY_CODE			"쨍하고해뜰날돌아온단다"				// 게임중인 모든 플레이어의 안개를 다 걷는다. 
		#define SPEED_CHEAT_KEY_CODE		"남행열차"							  // 생산 속도를 빠르게 해준다. 
		#define MONEY_CHEAT_KEY_CODE		"잘살아보세"						  // 돈을 올려준다.
		#define WIN_CHEAT_KEY_CODE			"감격시대"							  // 승리 
		#define GOD_CHEAT_KEY_CODE			"맨발의청춘"						  // 무적 
		#define RAIN_CHEAT_KEY_CODE			"비내리는영동교"					  // 비가 오게 만든다.
		#define GOODS_CHEAT_KEY_CODE		"나그대에게모두드리리"				   // 물품을 모두 채운다. 
		#define LEVEL_CHEAT_KEY_CODE		"굳세어라금순아"					  // 장수의 레벨을 모두 높인다. 
		#define JUSTICEK_CHEAT_KEY_CODE		"대한독립만세"						  // 본영 주위에 의병 12마리를 심는다.
	#else
		#define SEE_CHEAT_KEY_CODE			"보이지않는위험"					  // 게임중인 모든 플레이어의 안개를 다 걷는다. 
		#define SPEED_CHEAT_KEY_CODE		"총알탄사나이"						  // 생산 속도를 빠르게 해준다. 
		#define MONEY_CHEAT_KEY_CODE		"돈을갖고튀어라"					  // 돈을 올려준다.
		#define WIN_CHEAT_KEY_CODE			"이보다더좋을수는없다"				   // 승리 
		#define GOD_CHEAT_KEY_CODE			"인정사정볼것없다"				      // 무적 
		#define RAIN_CHEAT_KEY_CODE			"비오는날의수채화"				      // 비가 오게 만든다.
		#define GOODS_CHEAT_KEY_CODE		"주유소습격사건"					  // 물품을 모두 채운다. 
		#define LEVEL_CHEAT_KEY_CODE		"열세번째전사"					      // 장수의 레벨을 모두 높인다. 
	#endif

	#define UPGRADE_CHEAT_KEY_CODE			"hup"								// 모든 업그레이드를 완료한다. 
	#define MERCHANT_CHEAT_KEY_CODE			"hmerchant"							// 상인을 불러온다. 
	#define LOSE_CHEAT_KEY_CODE				"hlose"								// 패배 
	#define MAP_CHEAT_KEY_CODE				"hmap"								// 지도 켑춰 
	#define STATUS_CHEAT_KEY_CODE			"hstatus"							// 캐릭터의 정보를 표시해준다.  
	#define ATTACKINFO_CHEAT_KEY_CODE		"hattack"							// 컴퓨터의 공격 정보 
	#define WAVEMUSIC_CHEAT_KEY_CODE		"wavemusic"							// 게임 중 배경음악을 wav로 연주한다.
#endif

// 거상 테스트용 치트키
//#define TEST_ONLINE_CHEAT_KEY				"xxx거상"	// 주석처리할 것
#ifdef TEST_ONLINE_CHEAT_KEY
	#define TEST_ARMOR_CHEAT_KEY_CODE		"갑옷"
	#define TEST_WEAPON_CHEAT_KEY_CODE		"무기"
	#define TEST_HELMET_CHEAT_KEY_CODE		"투구"
	#define TEST_CHAR_CHEAT_KEY_CODE		"캐릭터"
	#define TEST_CONTINUE_CHEAT_KEY_CODE	"계속"
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

// 마법력을 전부 회복시켜준다. 
void RecoverMagic();
// 레벨을 최대로 올려준다.
void FullLevel();
// 돈을  올려준다.
void IncreaseMoney();
void WinStage();
void TimeCheat();
void Hurry();
// 각종 기술을 최고로 올려준다.
void Skill();
// 패배 안하기 
void NoLose();
// 마법 결계 없애기 
void Shield();

#endif