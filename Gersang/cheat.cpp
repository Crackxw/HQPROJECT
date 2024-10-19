#include <GSL.h>

#include <main.h>

//#include <BNCa_Header.H>
#include <setchar.h>
#include "maindata.h"
#include <etc.h>
#include "..\Gersang\cheat.h"

#include <map.h>
#include <CharOrg.h>
#include "..\Gersang\text.h"
#include <charinterface.h>
#include "charanimation.h"
#include <kindinfo.h>

#include <OnlineWorld.h>
#include "GMenu.h"
#include <map.h>

extern	_NewID 				ZeroID;
extern	_MainData			GD;									// 게임의 주데이터 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Text 				Text;								// 텍스트 
extern	_Map 				Map;



void _Cheat::Init()
{
	// 안개 보이기. 
	SeeCheatKeySwitch=FALSE;
	CheatSeeSwitch=FALSE;

	// 캐릭터 정보 보여주기 
	StatusCheatKeySwitch=FALSE;
	CheatStatusSwitch=FALSE;

	// 생산 속도 빠르게 하기 
	SpeedCheatKeySwitch=FALSE;
	CheatSpeedSwitch=FALSE;

	// 돈올리기
	MoneyCheatKeySwitch=FALSE;

	ArmorCheatKeySwitch			= FALSE;
	HelmetCheatKeySwitch		= FALSE;
	WeaponCheatKeySwitch		= FALSE;
	CharCheatKeySwitch			= FALSE;
	ContinueCheatKeySwitch		= FALSE;
	
	// 비오게 만들기
	RainCheatKeySwitch=FALSE;
	CheatRainSwitch=FALSE;

	// 업그레이드 완료하기 
	UpgradeCheatKeySwitch=FALSE;
	CheatUpgradeSwitch=FALSE;

	// 레벨 높이기 
	LevelCheatKeySwitch=FALSE;
	CheatLevelSwitch=FALSE;

	// 상인 부르기 
	MerchantCheatKeySwitch=FALSE;
	CheatMerchantSwitch=FALSE;

	// 물품 채우기 
	GoodsCheatKeySwitch=FALSE;
	CheatGoodsSwitch=FALSE;

	// 승리
	WinCheatKeySwitch=FALSE;
	CheatWinSwitch=FALSE;

	// 패배 
	LoseCheatKeySwitch=FALSE;
	CheatLoseSwitch=FALSE;

	// 컴퓨터의 공격에 대한 정보 제공 치트키 
	AttackInfoCheatKeySwitch=FALSE;
	CheatAttackInfoSwitch=FALSE;

	// 무적 
	GodCheatKeySwitch=FALSE;
	CheatGodSwitch=FALSE;

	// 지도  
	MapCheatKeySwitch			= FALSE;
	CheatMapSwitch				= FALSE;

	// 의병심기 치트키
	JusticeKCheatKeySwitch		= FALSE;

	// online
	OnlineCheatKeySwitch=FALSE;
	CheatOnlineSwitch=FALSE;

	// 배경음악 연주
	WavemusicCheatKeySwitch		= FALSE;
	CheatWavemusicSwitch		= FALSE;

    MagicPowerCheatKeySwitch ;
    LevelCheatKeySwitch			= FALSE;
    TimeCheatKeySwitch			= FALSE;
    HurryCheatKeySwitch			= FALSE;
    SkillCheatKeySwitch			= FALSE;
	NoLoseCheatKeySwitch		= FALSE;
	ShieldCheatKeySwitch		= FALSE;

}


void _Cheat::CheatOrder()
{
	int cheatswitch=FALSE;

	// 멀티 게임에서 가능해야 하고 전체 사용자에게 전달하지 않고 치트키를 사용하는 당사자에게만 영향을 미쳐야 한다.
	if(WavemusicCheatKeySwitch==TRUE)
	{
		WavemusicCheatKeySwitch=FALSE;
	}

	return ;

	/*
	if(SeeCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		SeeCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_SEE, 0, ZeroID, TRUE);
	}
	if(StatusCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		StatusCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_STATUS, 0, ZeroID, TRUE);
	}
	if(SpeedCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		SpeedCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_SPEED, 0, ZeroID, TRUE);
	}
	if(MoneyCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		MoneyCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_MONEY, 0, ZeroID, TRUE);
	}

	//-----
	// 조합형 캐릭터 장착 테스트용 
	SI32        id, i;
	
	if( ArmorCheatKeySwitch )
	{
		cheatswitch=TRUE;
		for( i=0; i<GD.CharOrderIndex; i++ )
		{
			id = GD.CharOrder[i];
			if(IsAlive(id) && KI[clifChar.GetKind(id)].IsKindAtb(KINDATB_HERO))	
				CR[id].SetEquip(TRUE, ON_WEAR_ARMOR_ITEM, ArmorCheatKeySwitch, FALSE);
		}
		ArmorCheatKeySwitch=FALSE;
	}
	if( HelmetCheatKeySwitch )
	{
		cheatswitch=TRUE;
		for( i=0; i<GD.CharOrderIndex; i++ )
		{
			id = GD.CharOrder[i];
			if( IsAlive(id) && KI[clifChar.GetKind(id)].IsKindAtb(KINDATB_HERO) )
				CR[id].SetEquip(TRUE, ON_WEAR_HELMET_ITEM, HelmetCheatKeySwitch, FALSE);
		}
		HelmetCheatKeySwitch=FALSE;
	}
	if( WeaponCheatKeySwitch )
	{
		cheatswitch	= TRUE;
		for( i=0; i<GD.CharOrderIndex; i++ )
		{
			id = GD.CharOrder[i];
			if( IsAlive(id) && KI[clifChar.GetKind(id)].IsKindAtb(KINDATB_HERO) )	CR[id].SetEquip(TRUE, ON_WEAR_WEAPON_ITEM, WeaponCheatKeySwitch, FALSE);
		}
		WeaponCheatKeySwitch	= FALSE;
	}
	if( CharCheatKeySwitch )
	{
		cheatswitch	= TRUE;
		id = FindEmptyID(UserPlayerNumber);
		if( id )
		{
			SetChar(CharCheatKeySwitch, id, Map.GetVirtualMapX(), Map.GetVirtualMapY(), NORTH|WEST, 100, UserPlayerNumber);
		}
		
		CharCheatKeySwitch	= FALSE;
	}
	//
	//------

	if(RainCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		RainCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_RAIN, 0, ZeroID, TRUE);
	}
	if(UpgradeCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		UpgradeCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_UPGRADE, 0, ZeroID, TRUE);
	}
	if(LevelCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		LevelCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_LEVEL, 0, ZeroID, TRUE);
	}
	if(MerchantCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		MerchantCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_MERCHANT, 0, ZeroID, TRUE);
	}
	if(GoodsCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		GoodsCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_GOODS, 0, ZeroID, TRUE);
	}
	if(WinCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		WinCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_WIN, 0, ZeroID, TRUE);
	}
	if(LoseCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		LoseCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_LOSE, 0, ZeroID, TRUE);
	}
	if(AttackInfoCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		AttackInfoCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_ATTACKINFO, 0, ZeroID, TRUE);
	}

	if(GodCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		GodCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_GOD, 0, ZeroID, TRUE);
	}

	if(MapCheatKeySwitch==TRUE)
	{
		cheatswitch=TRUE;

		MapCheatKeySwitch=FALSE;
		MakeOrder(UserPlayerNumber, ORDER_CHEAT_MAP, 0, ZeroID, TRUE);
	}

	if( JusticeKCheatKeySwitch == TRUE )
	{
		cheatswitch=TRUE;
		
		JusticeKCheatKeySwitch=FALSE;
		MakeOrder(0, ORDER_CHEAT_JUSTICEK, 0, ZeroID, TRUE);
	}

	if(cheatswitch==TRUE)
	   SetMessage2(Text.CheatMessage.Get());
*/
}

void _Cheat::IsCheatKeyCodeInput(char* text)
{
	
	// 화면을 다 보이게 만드는 치트키 
	if(strcmp(text, SEE_CHEAT_KEY_CODE)==0)
	{
		SeeCheatKeySwitch=TRUE;
	}
	// 캐릭터 상태를 보여주는 치트키 
	if(strcmp(text, STATUS_CHEAT_KEY_CODE)==0)
	{
		StatusCheatKeySwitch=TRUE;
	}
	if(strcmp(text, SPEED_CHEAT_KEY_CODE)==0)
	{
		SpeedCheatKeySwitch=TRUE;
	}
	if(strcmp(text, MONEY_CHEAT_KEY_CODE)==0)
	{
		MoneyCheatKeySwitch=TRUE;
	}
	if(strcmp(text, RAIN_CHEAT_KEY_CODE)==0)
	{
		RainCheatKeySwitch=TRUE;
	}
	if(strcmp(text, UPGRADE_CHEAT_KEY_CODE)==0)
	{
		UpgradeCheatKeySwitch=TRUE;
	}
	if(strcmp(text, LEVEL_CHEAT_KEY_CODE)==0)
	{
		LevelCheatKeySwitch=TRUE;
	}
	
	if(strcmp(text, MERCHANT_CHEAT_KEY_CODE)==0)
	{
		MerchantCheatKeySwitch=TRUE;
	}
	
	if(strcmp(text, GOODS_CHEAT_KEY_CODE)==0)
	{
		GoodsCheatKeySwitch=TRUE;
	}
	
	if(strcmp(text, WIN_CHEAT_KEY_CODE)==0)
	{
		WinCheatKeySwitch=TRUE;
	}
	
	if(strcmp(text, LOSE_CHEAT_KEY_CODE)==0)
	{
		LoseCheatKeySwitch=TRUE;
	}
	if(strcmp(text, ATTACKINFO_CHEAT_KEY_CODE)==0)
	{
		AttackInfoCheatKeySwitch=TRUE;
	}
	if(strcmp(text, GOD_CHEAT_KEY_CODE)==0)
	{
		GodCheatKeySwitch=TRUE;
	}
	if( strcmp(text, MAP_CHEAT_KEY_CODE) == 0 )
	{
		MapCheatKeySwitch=TRUE;
	}
	if(strcmp(text, WAVEMUSIC_CHEAT_KEY_CODE)==0)
	{
		WavemusicCheatKeySwitch=TRUE;
	}
#ifdef _IMJIN2PLUS
	if( strcmp(text, JUSTICEK_CHEAT_KEY_CODE) == 0 )
	{
 		JusticeKCheatKeySwitch = TRUE;
	}
#endif

#ifdef TEST_ONLINE_CHEAT_KEY
	if( strcmp(text, TEST_CONTINUE_CHEAT_KEY_CODE) == 0 )	ContinueCheatKeySwitch	= TRUE;

	char	cTemp[1024];
	SI32	siTemp = 0;
	sscanf(text, "%s%d", cTemp, &siTemp);
	if( strcmp(cTemp, TEST_ARMOR_CHEAT_KEY_CODE) == 0 )		ArmorCheatKeySwitch		= siTemp;
	if( strcmp(cTemp, TEST_HELMET_CHEAT_KEY_CODE) == 0 )	HelmetCheatKeySwitch	= siTemp;
	if( strcmp(cTemp, TEST_WEAPON_CHEAT_KEY_CODE) == 0 )	WeaponCheatKeySwitch	= siTemp;
	if( strcmp(cTemp, TEST_CHAR_CHEAT_KEY_CODE) == 0 )		CharCheatKeySwitch		= siTemp;
#endif
	

}


void _Cheat::CheatOperation(_OrderData od)
{
	SI32	i, j, k;	

	SHORT order=od.ODI.Order;

	switch(order)
	{
	case ORDER_CHEAT_SEE:
		if(CheatSeeSwitch==FALSE)
		{
			for(k=0;k<MAX_MULTI_PLAYER;k++)
			   for(i=0;i<Map.GetYsize();i++)
		           for(j=0;j<Map.GetXsize();j++)
				   {
			           GD.BlackMap[k][j][i] = 0;
				   }

				   CheatSeeSwitch=TRUE;
				   Map.DrawForceSwitch=TRUE;
				   Map.SetMapDrawSwitch(TRUE);
		}
		else
		{
		   CheatSeeSwitch=FALSE;
		   Map.SetMapDrawSwitch(TRUE);
		}

		break;


	case ORDER_CHEAT_STATUS:
		if(CheatStatusSwitch==FALSE)
		    CheatStatusSwitch=TRUE;
		else 
			CheatStatusSwitch=FALSE;
    	break;


	case ORDER_CHEAT_SPEED:
		if(CheatSpeedSwitch==FALSE)
			CheatSpeedSwitch=TRUE;
		else 
			CheatSpeedSwitch=FALSE;

    	break;

	case ORDER_CHEAT_RAIN:
		GD.Rain.SetRainSwitch(TRUE);
    	break;


	case ORDER_CHEAT_MAP:
		Map.DrawMapEntire();
		break;

	case ORDER_CHEAT_JUSTICEK:
		break;

	default:
		break;
	}
}

