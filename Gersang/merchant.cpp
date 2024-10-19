#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
	#include <etc.h>
	#include "music.h"
#endif

#include <charinterface.h>
#include <Map.h>
#include "text.h"
#include "yprg.h"
#include <kindinfo.h>
#include <DebugTool.h>	// hojae_ddd

#define BUYUNIT(x)		(x)

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Goods 				GoodsInfo[];
extern	int 				GeneralList[];						// 국가별 장수의 리스트 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_Text 				Text;								// 텍스트 
extern	_Briefing 			Briefing;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 

static SI32 MerchantGoodsKorea[] = {
	GOODS_GINSENG,	GOODS_ARMOUR1, GOODS_MINE,	 GOODS_SAMARROW, GOODS_AIRMAGIC, GOODS_BOW1,		
	GOODS_SWORD2,	GOODS_SWORD5, GOODS_SPEAR2,	 GOODS_MED1, 	 GOODS_MED5, 
	GOODS_ARMOUR3,  GOODS_ARMOUR4,	GOODS_STICK1,   -1
};
 
static SI32 MerchantGoodsJapan[] = {
	GOODS_GINSENG,	GOODS_ARMOUR1,  GOODS_MINE,		GOODS_SAMARROW, GOODS_AIRMAGIC, GOODS_RIFLE1,
	GOODS_GUN1,		GOODS_CANNON1,	GOODS_SWORD3, 	GOODS_SWORD4,	GOODS_AXE1,
	GOODS_MED2,		GOODS_MED5,		GOODS_STICK2,	GOODS_MACE1, 	GOODS_CLAW1,
	GOODS_ARMOUR4,  GOODS_ARMOUR5, -1
};

static SI32 MerchantGoodsChina[] = {
	GOODS_GINSENG,	GOODS_ARMOUR1,  GOODS_MINE,		GOODS_SAMARROW, GOODS_AIRMAGIC, GOODS_BOW2,
	GOODS_SWORD1,	GOODS_SWORD6,	GOODS_SWORD7,	GOODS_SPEAR1,	GOODS_BOOK1,
	GOODS_BOOK2,	GOODS_BOOK3,	GOODS_NECKLACE1,GOODS_ARMOUR2,	GOODS_ARMOUR4,
	GOODS_BELT1,	GOODS_BELT2,	GOODS_MED3,		GOODS_MED4,		GOODS_MED5,
	GOODS_SPEAR3,	-1
};

	
void _Goods::Init(SHORT money, SHORT tree, SI32 sipercent, SI32 sifont, SI32 sibuyunit, char* text, SI32 property, _GoodsEffect *ge)
{
		Money=money;
		Tree=tree;
		siPercent = sipercent;
		siFont = sifont;
		siBuyUnit = sibuyunit;

		strcpy(Text,text);
		Property = property;

		memcpy(&GE, ge, sizeof(_GoodsEffect));
}


void _Merchant::Init()
{
	SHORT i;
	

	ZeroMemory(this, sizeof(_Merchant));

	for(i = 0;i<MAX_MULTI_PLAYER;i++)
	{
		PM[i].PlayerNumber = i;
	}

	
}

// 물품을 준비한다. 
void _PlayerMerchant::ReadyGoods()
{
	SHORT i;
	
	// 초기화한다. 
	for(i=0;i<MAX_GOODS_KIND;i++)
	{
		Goods[i]=0;             // 상품의 수량   
	}
	
	// 20000프레임 이후에나.. 
	if(GD.Frame>20000)
		Goods[GOODS_RAINMAKER]=((GD.Random() % 3) ? 0 : 1);
	
	if(GD.Player[PlayerNumber].PI.Country== COUNTRY_CHINA)
		Goods[GOODS_ELEPHANT] = ((GD.Random() % 2 == 0) ? 0 : GD.Random() % 3 + 1 );
	
	
#ifndef _IMJIN2PLUS
	Goods[GOODS_DRAGONBALL] = ((GD.Random() % 4 == 0) ? 0 : GD.Random() % 2 + 1 );
	Goods[GOODS_GINSENG] = GD.Random() % 2;
	if(GD.Player[PlayerNumber].PI.Country== COUNTRY_KOREA)
		Goods[GOODS_AIRMAGIC] = 10;
	if(GD.Player[PlayerNumber].PI.Country== COUNTRY_JAPAN)
		Goods[GOODS_SAMARROW] = GD.Random() % 10;
	if(GD.Player[PlayerNumber].PI.Country== COUNTRY_CHINA)
		Goods[GOODS_MINE]=GD.Random()%5;             // 상품의 수량   
#endif

	if(GD.Player[PlayerNumber].PI.Country == COUNTRY_KOREA)
		Goods[GOODS_BULL]    = GD.Random() % 3;

#ifdef _IMJIN2PLUS

	if(GD.Player[PlayerNumber].bStorageSwitch == TRUE)
	{
		int* pMerchantGoods = NULL; 

		// 상인별로 다른 아이템을 가져온다. 
		switch(MerchantType)
		{
		case MERCHANT_TYPE_KOREA:
			pMerchantGoods = MerchantGoodsKorea;
			break;

		case MERCHANT_TYPE_JAPAN:
			pMerchantGoods = MerchantGoodsJapan;
			break;

		default:
			pMerchantGoods = MerchantGoodsChina;
			break;
		}

		SI32 index = 0;
		UI32 percent = 0;
		SI32 goods = 0; 
		while(1)
		{
			goods = pMerchantGoods [index];
			if(goods == -1)break;

			percent = GoodsInfo[goods].siPercent;

			if(CanBuyGoods(goods)== TRUE)
			{
				Goods[goods]	= ((GD.Random() % 100) < percent  ? 1:0);
			}

			index++;
		}

	}
#endif

}

// 이 제품을 상인이 가져올 필요가 있는지 확인한다. 
BOOL _PlayerMerchant::CanBuyGoods(SI32 sigoods)
{
	SI32 sicountry= GD.Player[PlayerNumber].PI.Country;
	SI32 generalkind;
	SI32 atb = GoodsInfo[sigoods].Property ;


	// 일반 아이템인 경우,
	if( atb & GOODSATB_NORMAL)
	{
		// 조선  전용이고 내가 조선이 아니면, 
		if((atb & GOODSATB_KOREA) && sicountry != COUNTRY_KOREA)
		{
			return FALSE;
		}
		// 일본 전용이고 내가 일본이 아니면, 
		if((atb & GOODSATB_JAPAN) && sicountry != COUNTRY_JAPAN)
		{
			return FALSE;
		}
		// 명 전용이고 내가 명이 아니면, 
		if((atb & GOODSATB_CHINA) && sicountry != COUNTRY_CHINA)
		{
			return FALSE;
		}

		// 조반 전용 아이템이고 조반이 아니면,. 
#ifndef _IMJIN2PLUS
		if(atb & GOODSATB_PLUS)
		{
			return FALSE;
		}
#endif

		// 특정 유닛이 필요한 경우. 
		if(GoodsInfo[sigoods].GE.siNeedKind)
		{
			// 해당 유닛이 없다면 
			if(GD.Player[PlayerNumber].KindSwitch[GoodsInfo[sigoods].GE.siNeedKind] == FALSE)return FALSE;
		}

		return TRUE;
	}
	else
	{
		// 장수가 장비하는  아이템인 경우. 
		SI32 index = 0;
		while(1)
		{
			generalkind = GeneralList[index * 5 + 0];
			if(generalkind  == 0)break;
			if(GD.Player[PlayerNumber].GeneralSwitch[generalkind] == TRUE)
			{
				if(KI[generalkind].Nation == sicountry)
				{
					if(KI[generalkind].GoodsAtb &  atb )
					{
						return TRUE;
					}
					
				}
			}
			
			index++;
		}
	}

	return FALSE;
}
void _PlayerMerchant::Action()
{

	// 상인이 배치된지 시간이 지난 것이라면, 
	if( (GD.Frame-MerchantFrame) < MERCHANT_INTERVAL ) return ;

	if(GD.Player[PlayerNumber].UpgradeSwitch[UPGRADE_MARKET] == FALSE)return ;
	
	if(MerchantSwitch == TRUE)
	{
		
	}
	else
	{
		if(GD.Random() % 3)
		{
			MerchantFrame=GD.Frame;
			return ;
		}

		// 시장이 준비되지 않으면 상인은 가지 않는다. 
		if((GD.Player[PlayerNumber].KindSwitch[KIND_MILLK]==FALSE
			&& GD.Player[PlayerNumber].KindSwitch[KIND_MILLJ]==FALSE
			&& GD.Player[PlayerNumber].KindSwitch[KIND_MILLC]==FALSE))
		{
			// 상인을 없앤다. 
			MerchantFrame=GD.Frame;
			return ;
		}
	}

	// 플레이어를 옮긴 프레임을 저장한다. 
	MerchantFrame=GD.Frame;

	MoveToPlayer();

}


void _PlayerMerchant::MoveToPlayer( )
{

	SI32	i;
	SI32	MerchantKind;
	CHAR    *MerchantIDName;

	if(MerchantSwitch == TRUE)
	{
		MerchantSwitch  = FALSE;
	}
	else
	{
		MerchantSwitch  = TRUE;

		// 상인의 종류를 설정한다. 
		MerchantType = GD.Random() % MAX_MERCHANT_TYPE;
	}


	if(MerchantSwitch == TRUE)
	{
		// 물품을 준비한다. 
		ReadyGoods();
		
		// 컴퓨터 플레이어에게 간 경우라면, 
		if(GD.Player[PlayerNumber].PI.Host==BY_COMPUTER)
		{
			// 컴퓨터는 상인이 오자마자 물품을 공급받는다. 
			for(i=0;i<MAX_GOODS_KIND;i++)
			{
				GD.Player[PlayerNumber].Goods[i]+= Goods[i];             // 상품의 수량   
			}
		}
		

		if(PlayerNumber == UserPlayerNumber)
		{
			// 상인이 도착했음을 알려준다. 
#ifdef _IMJIN2PLUS
			char filename[FILE_NAME_SIZE];
			char text[128];
			
			switch(MerchantType)
			{
			case MERCHANT_TYPE_KOREA:
				MerchantKind = -1;
				MerchantIDName = Text.MerchantKoreaIDName.Get();
				break;
			case MERCHANT_TYPE_JAPAN:
				MerchantKind = -2;
				MerchantIDName = Text.MerchantJapanIDName.Get();
				break;
			default:
				MerchantKind = -3;
				MerchantIDName = Text.MerchantChinaIDName.Get();
				break;
			}
			
			if(GD.Talk.FindProperText(MerchantKind, TALKINDEX_MERCHANT_ARRIVE, text, filename)==TRUE)
			{
				Briefing.Play(Y_PORTRAIT_PUT_LEFT_UP, text, 255, MerchantIDName, filename);
			}
#else		
			// 메시지를 표시한다 
			SetMessage2(Text.MerchantComeMessage.Get());
			PushEffect(EFFECT_MERCHANTCOME, 0, 0);
#endif
			
			_NewID newid=FindKindOrAtb(0, ATB_ADVMARKET);
			if(IsSee(newid))
			{
				GD.SetBattleField(clifChar.GetX(newid.ID), clifChar.GetY(newid.ID));
			}
		}
	}
	else
	{

		if(PlayerNumber == UserPlayerNumber)
		{
			// 메시지를 표시한다. 
			SetMessage2(Text.MerchantLeaveMessage.Get());
			PushEffect(EFFECT_MERCHANTLEAVE, 0, 0);
		}
	}
}

void _Merchant::Action()
{
	SI32 i;

	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		PM[i].Action();
	}
}



void GoodsInit()
{

	_GoodsEffect GE;


	GE.Init();
	GoodsInfo[GOODS_TREE].Init		(0,		0,		0, 2, BUYUNIT(1), Text.Tree.Get(), GOODSATB_NORMAL, &GE);
	
	GE.Init();
	GoodsInfo[GOODS_FOOD].Init		(0,		0,		0, 1, BUYUNIT(1), Text.Crop.Get(), GOODSATB_NORMAL, &GE);

	GE.Init();
	GE.siNeedKind = KIND_GENERAL5C;
	GoodsInfo[GOODS_MINE].Init		(1000,	1000,   60, 0, BUYUNIT(5), Text.Mine.Get(), GOODSATB_NORMAL|GOODSATB_CHINA, &GE);

	GE.Init();
	GoodsInfo[GOODS_RAINMAKER].Init	(500,	500,	0, 0, BUYUNIT(1), Text.RainMaker.Get(), GOODSATB_NORMAL, &GE);

	GE.Init();
	GoodsInfo[GOODS_SAMARROW].Init	(1000,	1000,   60, 0, BUYUNIT(10), Text.SamArrow.Get(), GOODSATB_NORMAL|GOODSATB_JAPAN, &GE);

	GE.Init();
	GoodsInfo[GOODS_ELEPHANT].Init	(1000,	1000,	0, 0, BUYUNIT(1), Text.Elephant.Get(), GOODSATB_NORMAL, &GE);

	GE.Init();
	GoodsInfo[GOODS_AIRMAGIC].Init	(1200,	1200,  100, 0, BUYUNIT(10), Text.AirMagic.Get(), GOODSATB_NORMAL|GOODSATB_KOREA, &GE);

	GE.Init();
	GoodsInfo[GOODS_BULL].Init		(500,	500,	0, 0, BUYUNIT(1), Text.Bull.Get(), GOODSATB_NORMAL, &GE);

	GE.Init();
	GoodsInfo[GOODS_DRAGONBALL].Init(1000,	1000,	0, 0, BUYUNIT(1), Text.DragonBall.Get(), GOODSATB_NORMAL, &GE);

	GE.Init();
	GoodsInfo[GOODS_GINSENG].Init	(800,	800,	50, 0, BUYUNIT(1), Text.GinSeng.Get(), GOODSATB_NORMAL, &GE);

	// 대궁
	GE.Init();
	GE.siApower = 20;
	GE.siAttackRange = 40;
	GoodsInfo[GOODS_BOW1].Init		(1000,	1000,	60, 7, BUYUNIT(1), Text.Bow1.Get(),		GOODSATB_WEAPON|GOODSATB_BOW|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 철력궁
	GE.Init();
	GE.siApower = 30;
	GE.siAttackRange = 30;
	GE.bStrongAttackSwitch = TRUE;
	GoodsInfo[GOODS_BOW2].Init		(1500,	1500,	40, 22, BUYUNIT(1), Text.Bow2.Get(),		GOODSATB_WEAPON|GOODSATB_BOW|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 용쇄총
	GE.Init();
	GE.siApower = 30;
	GE.siAttackRange = 40;
	GoodsInfo[GOODS_RIFLE1].Init	(1000,	1000,	70, 16, BUYUNIT(1), Text.Rifle1.Get(),	GOODSATB_WEAPON|GOODSATB_RIFLE|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 드래군파이슨
	GE.Init();
	GE.siApower = 30;
	GE.siAttackRange = 50;
	GoodsInfo[GOODS_GUN1].Init		(1000,	1000,	90, 9, BUYUNIT(1), Text.Gun1.Get(),		GOODSATB_WEAPON|GOODSATB_GUN|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 철릉포
	GE.Init();
	GE.siApower = 50;
	GE.siAttackRange = 70;
	GoodsInfo[GOODS_CANNON1].Init	(1500,	1500,	40, 23, BUYUNIT(1), Text.Cannon1.Get(),	GOODSATB_WEAPON|GOODSATB_CANNON|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 청홍검
	GE.Init();
	GE.siApower = 100;
	GE.bStrongAttackSwitch = TRUE;
	GoodsInfo[GOODS_SWORD1].Init	(1500,	1500,	30, 24, BUYUNIT(1), Text.Sword1.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 폭마혈도
	GE.Init();
	GE.siApower = 60;
	GE.siMagicPowerAbs = 100;
	GE.bKimBombSwitch = TRUE;
	GoodsInfo[GOODS_SWORD2].Init	(2000,	2000,	60, 27, BUYUNIT(1), Text.Sword2.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 쿠사나기의 검
	GE.Init();
	GE.siApower = 45;
	GoodsInfo[GOODS_SWORD3].Init	(1000,	1000,	90, 26, BUYUNIT(1), Text.Sword3.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 패왕도
	GE.Init();
	GE.siApower = 50;
	GE.siStealHealth = 5;
	GoodsInfo[GOODS_SWORD4].Init	(1200,	1200,	60, 31, BUYUNIT(1), Text.Sword4.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 칠성검
	GE.Init();
	GE.siApower = 40;
	GE.siStealHealth = 10;
	GoodsInfo[GOODS_SWORD5].Init	(1200,	1200,	50, 25, BUYUNIT(1), Text.Sword5.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 부동명왕검
	GE.Init();
	GE.siApower = 65;
	GoodsInfo[GOODS_SWORD6].Init	(1200,	1200,	50, 13, BUYUNIT(1), Text.Sword6.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);
	
	// 용비늘검
	GE.Init();
	GE.siApower = 55;
	GE.siMagicPowerAbs = 200;
	GE.bCallDragonSwitch = TRUE;
	GoodsInfo[GOODS_SWORD7].Init	(5000,	5000,	30, 15, BUYUNIT(1), Text.Sword7.Get(),	GOODSATB_WEAPON|GOODSATB_SWORD|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 방천화극
	GE.Init();
	GE.siApower = 35;
	GE.siGeneralEffectApower = 40;
	GoodsInfo[GOODS_SPEAR1].Init	(2000,	2000,	70, 10, BUYUNIT(1), Text.Spear1.Get(),	GOODSATB_WEAPON|GOODSATB_SPEAR|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 흑철창
	GE.Init();
	GE.siApower = 40;
	GE.siAttackRange = 500;
	GE.bMagic8kSwitch = TRUE;
	GoodsInfo[GOODS_SPEAR2].Init	(2000,	2000,	40, 30, BUYUNIT(1), Text.Spear2.Get(),	GOODSATB_WEAPON|GOODSATB_SPEAR|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 장팔사모
	GE.Init();
	GE.siApower = 50;
	GoodsInfo[GOODS_SPEAR3].Init	(1200,	1200,	80, 34, BUYUNIT(1), Text.Spear3.Get(),	GOODSATB_WEAPON|GOODSATB_SPEAR|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 참마부도
	GE.Init();
	GE.siApower = 35;
	GE.siMagicPowerAbs = 100;
	GE.bMagic5jSwitch = TRUE;
	GoodsInfo[GOODS_AXE1].Init		(2000,	2000,	100, 21, BUYUNIT(1), Text.Axe1.Get(),	GOODSATB_WEAPON|GOODSATB_AXE|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 육도삼략
	GE.Init();
	GE.siMagicpower = 35;
	GoodsInfo[GOODS_BOOK1].Init		(1000,	1000,	60, 19, BUYUNIT(1), Text.Book1.Get(),	GOODSATB_ETC|GOODSATB_BOOK|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 손자병법
	GE.Init();
	GE.siMagicpower = 40;
	GoodsInfo[GOODS_BOOK2].Init		(1200,	1200,	30, 14, BUYUNIT(1), Text.Book2.Get(),	GOODSATB_ETC|GOODSATB_BOOK|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 화타의 책
	GE.Init();
	GE.siMagicpower = 30;
	GE.siHealthRecoverAbs = 3;
	GoodsInfo[GOODS_BOOK3].Init		(3000,	3000,	20, 29, BUYUNIT(1), Text.Book3.Get(),	GOODSATB_ETC|GOODSATB_BOOK|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 달마의 염주
	GE.Init();
	GE.siMagicpower = 100;
	GoodsInfo[GOODS_NECKLACE1].Init	(1500,	1500,	20, 6, BUYUNIT(1), Text.Necklace1.Get(),GOODSATB_ETC|GOODSATB_NECKLACE|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 등갑옷
	GE.Init();
	//GoodsInfo[GOODS_ARMOUR1].Init	(300,	300,	60, 8, BUYUNIT(3), Text.Armour1.Get(),	GOODSATB_ARMOUR|GOODSATB_CHINA|GOODSATB_NORMAL|GOODSATB_PLUS, &GE);
	GoodsInfo[GOODS_ARMOUR1].Init	(300,	300,	100, 8, BUYUNIT(3), Text.Armour1.Get(),	GOODSATB_CHINA|GOODSATB_NORMAL|GOODSATB_PLUS, &GE);	// hojae_ddd

	// 용기린갑옷
	GE.Init();
	GE.siDpower = 35;
	GE.siGeneralEffectDpower = 40;
	GoodsInfo[GOODS_ARMOUR2].Init	(2500,	2500,	50, 5, BUYUNIT(1), Text.Armour2.Get(),	GOODSATB_ARMOUR|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 현무갑옷
	GE.Init();
	GE.siDpower = 50;
	GoodsInfo[GOODS_ARMOUR3].Init	(1000,	1000,	60, 4, BUYUNIT(1), Text.Armour3.Get(),	GOODSATB_ARMOUR|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 수룡왕갑옷
	GE.Init();
	GE.siDpower = 40;
	GE.bGeneralEffectFireResistSwitch = TRUE;
	GoodsInfo[GOODS_ARMOUR4].Init	(2000,	2000,	30, 41, BUYUNIT(1), Text.Armour4.Get(),	GOODSATB_ARMOUR|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);
	
	// 카케무샤의 투구
	GE.Init();
	GE.siDpower = 25;
	GE.bClonSwitch = TRUE;
	GoodsInfo[GOODS_ARMOUR5].Init	(4000,	4000,	60, 40, BUYUNIT(1), Text.Armour5.Get(),	GOODSATB_ARMOUR|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 적토마의 갈기
	GE.Init();
	GE.siMoveSpeed	= 30;
	GE.siHealth		= 50;
	GoodsInfo[GOODS_BELT1].Init		(1800,	1800,	60, 20, BUYUNIT(1), Text.Belt1.Get(),	GOODSATB_ETC|GOODSATB_BELT|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 화룡천마의 허리띠
	GE.Init();
	GE.siDpower = 30;
	GE.siMoveSpeed = 40;
	GoodsInfo[GOODS_BELT2].Init		(2000,	2000,	40, 28, BUYUNIT(1), Text.Belt2.Get(),	GOODSATB_ETC|GOODSATB_BELT|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 백설산삼
	GE.Init();
	GE.siHealth = 50;
	GoodsInfo[GOODS_MED1].Init		(2000,	2000,	40, 12, BUYUNIT(1), Text.Med1.Get(),	GOODSATB_POTION|GOODSATB_MED|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 백사탕
	GE.Init();
	GE.bRecoverMagicPowerSwitch = TRUE;
	GoodsInfo[GOODS_MED2].Init		(500,	500,	100, 11, BUYUNIT(1), Text.Med2.Get(),	GOODSATB_POTION|GOODSATB_MED|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 용왕탕
	GE.Init();
	GE.siMagicpower = 20;
	GoodsInfo[GOODS_MED3].Init		(700,	700,	30, 17, BUYUNIT(1), Text.Med3.Get(),	GOODSATB_POTION|GOODSATB_MED|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 웅족탕
	GE.Init();
	GE.bRecoverHealthSwitch = TRUE;
	GoodsInfo[GOODS_MED4].Init		(1000,	1000,	30, 18, BUYUNIT(1), Text.Med4.Get(),	GOODSATB_POTION|GOODSATB_MED|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 진화탄
	GE.Init();
	GE.bExtinguishSwitch = TRUE;
	GoodsInfo[GOODS_MED5].Init		(300,	300,	60, 32, BUYUNIT(1), Text.Med5.Get(),	GOODSATB_POTION|GOODSATB_MED|GOODSATB_MERCHANT_KOREA|GOODSATB_MERCHANT_JAPAN|GOODSATB_MERCHANT_CHINA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 나찰신장봉
	GE.Init();
	GE.siApower = 20;
	GE.siMagicpower = 30;
	GoodsInfo[GOODS_STICK1].Init	(1000,	1000,	40, 35, BUYUNIT(1), Text.Stick1.Get(),	GOODSATB_WEAPON|GOODSATB_STICK|GOODSATB_MERCHANT_KOREA|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 백팔귀마봉
	GE.Init();
	GE.siApower = 10;
	GE.siMagicpower = 40;
	GoodsInfo[GOODS_STICK2].Init	(1500,	1500,	50, 39, BUYUNIT(1), Text.Stick2.Get(),	GOODSATB_WEAPON|GOODSATB_STICK|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 참철살퇴
	GE.Init();
	GE.siApower = 40;
	GoodsInfo[GOODS_MACE1].Init		(1000,	1000,	70, 37, BUYUNIT(1), Text.Mace1.Get(),	GOODSATB_WEAPON|GOODSATB_MACE|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);

	// 흑갈조도
	GE.Init();
	GE.siApower = 30;
	GE.siMagicpower = 10;
	GoodsInfo[GOODS_CLAW1].Init		(1200,	1200,	50, 36, BUYUNIT(1), Text.Claw1.Get(),	GOODSATB_WEAPON|GOODSATB_CLAW|GOODSATB_MERCHANT_JAPAN|GOODSATB_LIST|GOODSATB_PLUS, &GE);
}



BOOL GetItemHelp(SI32 goods, char* helpstring)
{
		
	char temp[128]= "";


	if(helpstring == NULL)return FALSE;

	strcpy(helpstring, "");

	if(GoodsInfo[goods].Property & GOODSATB_POTION)
	{
		wsprintf(temp, "[소모품] ");
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siHealth)
	{
		wsprintf(temp, "[체력 향상:%d%%] ", GoodsInfo[goods].GE.siHealth);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siApower)
	{
		wsprintf(temp, "[공격력 향상:%d%%] ", GoodsInfo[goods].GE.siApower);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siDpower)
	{
		wsprintf(temp, "[방어력 향상:%d%%] ", GoodsInfo[goods].GE.siDpower);
		strcat(helpstring, temp);
	}
	
	
	if(GoodsInfo[goods].GE.siMagicpower)
	{
		wsprintf(temp, "[마법력 향상:%d%%] ", GoodsInfo[goods].GE.siMagicpower);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siMoveSpeed)
	{
		wsprintf(temp, "[이동속도 향상:%d%%] ", GoodsInfo[goods].GE.siMoveSpeed);
		strcat(helpstring, temp);
	}
	
	
	if(GoodsInfo[goods].GE.siAttackRange)
	{
		wsprintf(temp, "[공격거리 향상:%d%%] ", GoodsInfo[goods].GE.siAttackRange);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siStealHealth)
	{
		wsprintf(temp, "[적 체력 흡수:%d%%] ", GoodsInfo[goods].GE.siStealHealth);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siGeneralEffectApower)
	{
		wsprintf(temp, "[부하 공격력 향상:%d%%] ", GoodsInfo[goods].GE.siGeneralEffectApower);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siGeneralEffectDpower)
	{
		wsprintf(temp, "[부하 방어력 향상:%d%%] ", GoodsInfo[goods].GE.siGeneralEffectDpower);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.siHealthRecoverAbs)
	{
		wsprintf(temp, "[자동체력 회복:%d%%] ", GoodsInfo[goods].GE.siHealthRecoverAbs);
		strcat(helpstring, temp);
	}
	
	if(GoodsInfo[goods].GE.bRecoverMagicPowerSwitch)
	{
		wsprintf(temp, "[마법력 최대 회복] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bRecoverHealthSwitch)
	{
		wsprintf(temp, "[체력 최대 회복] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bStrongAttackSwitch)
	{
		wsprintf(temp, "[일격 필살] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bKimBombSwitch)
	{
		wsprintf(temp, "[폭뢰격 사용] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bCallDragonSwitch)
	{
		wsprintf(temp, "[승천용 소환] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bMagic8kSwitch)
	{
		wsprintf(temp, "[쐐기지르기 사용] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bMagic5jSwitch)
	{
		wsprintf(temp, "[만월도끼춤 사용] ");
		strcat(helpstring, temp);
	}


	if(GoodsInfo[goods].GE.bGeneralEffectFireResistSwitch)
	{
		wsprintf(temp, "[부하 화염 저항 능력] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bClonSwitch)
	{
		wsprintf(temp, "[분신 소환] ");
		strcat(helpstring, temp);
	}

	if(GoodsInfo[goods].GE.bExtinguishSwitch)
	{
		wsprintf(temp, "[주변의 불을 끈다.] ");
		strcat(helpstring, temp);
	}
	 

	if(strcmp(temp, "")== 0)
	{
		return FALSE;
	}

	return TRUE;
}

