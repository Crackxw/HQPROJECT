#include <GSL.h>

#include <main.h>

#ifdef _IMJIN2
	#include "maindata.h"
#endif

#include <map.h>
#include <CharOrg.h>
#include <OnlineWorld.h>

extern	_MainData			GD;									// 게임의 주데이터 
extern	_Map 				Map;



// 장수 훈련을 메뉴에 추가할 수 있는지 여부를 알려준다. 
BOOL cltCharOrg::CanPushGeneralMakeMenu(SHORT kind)
{
	// 현재 작업이 해당 훈련이고  
	if(CurrentOrder==ORDER_TRAIN && Para1==kind)
	{
		return FALSE;
	}

	// 이미 장수가 게임에 등장한다면, 
	if(GD.CharNumber[GetPlayerNumber()][kind])return FALSE;

	return TRUE;
}


BOOL cltCharOrg::UpdateMenu(SHORT order)
{

	for(SHORT i=0;i<MAX_MENU_NUMBER;i++)
	{
		MenuItem[i]=0;
	}

	UpdateMenuNormal(order);

	return TRUE;

}




// 메뉴를 업데이트한다. 
BOOL cltCharOrg::UpdateMenuNormal(SHORT order)
{

	UpdateMenuOrder=order;

	MenuNumber=0;
	PushMenu(0, ITEM_STOP);
	PushMenu(1, ITEM_MOVE);
	
	// 공격 속성을 갖고 있어야만 공격 메뉴가 작동한다. 
	if(IsAtb(ATB_ATTACK|ATB_ATTACKAIR))
		PushMenu(2, ITEM_ATTACK);

	PushMenu(3, ITEM_HOLD);


	SI32 index= 4;

	if(IsAtb3(ATB3_LIGHTNING ) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_LIGHTNING);
		index++;
	}

	if(IsAtb3(ATB3_EARTHQUAKE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_EARTHQUAKE);
		index++;
	}

	// 폭뢰격. 
	if(IsAtb3(ATB3_KIMBOMB) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_KIMBOMB);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC5J) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC5J);
		index++;
	}

	// 기공신포 .
	if(IsAtb3(ATB3_MAGIC4 ) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC4C);
		index++;
	}

	// 포박술. 
	if(IsAtb3(ATB3_MAGIC6C) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC6C);
		index++;
	}

	//이사의 사념 특수 공격.
	if(IsAtb3(ATB3_ICE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_ICE);
		index++;
	}

	// 풍백술. 
	if(IsAtb3(ATB3_TORNADO) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_TORNADO);
		index++;
	}
	// 황사.
	if(IsAtb3(ATB3_YELLOWSAND) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_YELLOWSAND);
		index++;
	}

	// 원심분쇄격(흑룡대차륜)
	if(IsAtb3(ATB3_MAGIC4T) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC4T);
		index++;
	}
	// 야수의 혼
	if(IsAtb3(ATB3_SOUL) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index, ITEM_MAGIC_SOUL);
		index++;
	}

	//귀신탄.
	if(IsAtb3(ATB3_GHOST) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_GHOST);
	}

	//암흑화염폭풍.
	if(IsAtb3(ATB3_DARKFIRE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_DARKFIRE);
		index++;
	}

	if(IsAtb3(ATB3_YELLOWSAND) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_YELLOWSAND);
	}

	//상아폭격
	if(IsAtb4(ATB4_IVORY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_IVORY);
		index++;
	}

	//비격청룡도
	if(IsAtb4(ATB4_BLUEDRAGON) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_BLUEDRAGON);
		index++;
	}

	//연발포탄
	if(IsAtb4(ATB4_RUNNINGFIRE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RUNNINGFIRE);
		index++;
	}

	//장풍
	if(IsAtb4(ATB4_ENERGYPA)	== TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_ENERGYPA);
		index++;
	}

	//설공신포
	if(IsAtb4(ATB4_SNOWATTACK) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_SNOWATTACK);
		index++;
	}
	
	//화공신포
	if(IsAtb4(ATB4_FIREATTACK) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FIREATTACK);
		index++;
	}
	//열화장풍(비격황룡권).
	if(IsAtb4(ATB4_FIREENERGYPA) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FIREENERGYPA);
		index++;
	}
	

	// 분신술. 
	// 분신은 사용할 수 없다.(HostID.IsEmpty() == TRUE :분신이 아니다.)
	if(IsAtb3(ATB3_CLON) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_CLON);
		index++;
	}

	// 분신술. 
	// 요술사의 식귀,아귀 소환
	// 아마쿠사가 불러낸 요술사는 식귀,아귀를 소환할수 있다.
	if(IsAtb3(ATB3_CLON2) == TRUE )
	{
		PushMenu(index, ITEM_MAGIC_CLON);
		index++;
	}


/*	// 흡수술. 
	// 분신은 사용할 수 없다.(HostID.IsEmpty() == TRUE :분신이 아니다.)
	if(IsAtb3(ATB3_ABSORBLIFE) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_ABSORB);
		index++;
	}
	*/
	// 방어막. 
	if(IsAtb3(ATB3_SHIELDMAGIC) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_SHIELD);
		index++;
	}
	
	// 소화술 
	// 분신은 사용할 수 없다.(HostID.IsEmpty() == TRUE :분신이 아니다.)
	if(IsAtb3(ATB3_EXTINGUISH) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_EXTINGUISH);
		index++;
	}


	// 연옥술 
	// 분신은 사용할 수 없다.(HostID.IsEmpty() == TRUE :분신이 아니다.)
	if(IsAtb3(ATB3_GROUNDEXP) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_GROUNDEXP);
		index++;
	}

	// 독무술
	if(IsAtb3(ATB3_POISONBULLET) == TRUE && HostID.IsEmpty() == TRUE	)
	{
		PushMenu(index, ITEM_MAGIC_MAGIC2T);
		index++;
	}
	//마법 테스트 
	//////////////////////////////////////////////////////////////////////
	if(IsAtb3(ATB3_MAGICTEST) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MAGICTEST);
		index++;
	}
	//////////////////////////////////////////////////////////////////////
	if(IsAtb3(ATB3_MAGIC_RECALL_PANDA) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_PANDA);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_MONKEY);
		index++;
	}

	if(IsAtb3(ATB3_MAGIC_RECALL_MONKEY) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_RECALL_MONKEY);
		index++;
	}
	if(IsAtb3(ATB3_MINE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MINE);
		index++;
	}
	//마나번.
	/*
	if(IsAtb4(ATB4_MANABURN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MANABURN);
		index++;
	}
	*/
	// 쌍검난무
	if(IsAtb4(ATB4_DOUBLE_SWORD) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_DOUBLE_SWORD);
		index++;
	}
	// 장창소환
	if(IsAtb4(ATB4_LONG_SPEAR) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_LONG_SPEAR);
		index++;
	}		
	// 빙석술
	if(IsAtb4(ATB4_ICE_BARRIER) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_ICE_BARRIER);
		index++;
	}
	// 만천화우
	if(IsAtb4(ATB4_FLOWER_NEEDLE_RAIN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_FLOWER_NEEDLE_RAIN);
		index++;
	}
	// 빙의술
	if(IsAtb4(ATB4_SOUL_CHANGE) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_SOUL_CHANGE);
		index++;
	}		
	// 석괴술
	if(IsAtb4(ATB4_STONE_MAGIC) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_STONE_MAGIC);
		index++;
	}
	//마나번
	if(IsAtb4(ATB4_MANABURN) == TRUE && HostID.IsEmpty() == TRUE)
	{
		PushMenu(index,ITEM_MAGIC_MANABURN);
		index++;
	}

//----------------------------------------------------------------------------
// robypark (2004/05/17 14:55) : 추가 기술 - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	// 강강수월래(조선-거북차, 이순신 2차 전직 기술)
	if ((IsAtb4(ATB4_FIRE_ROUND_DANCE) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FIRE_ROUND_DANCE);
		index++;
	}

	// 굉뇌(조선-뇌전차, 허준 2차 전직 기술)
	if ((IsAtb4(ATB4_NOVA) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_NOVA);
		index++;
	}

	// 격노염폭(일본-지진차, 와키자카 2차 전직 기술)
	if ((IsAtb4(ATB4_FURY_OF_VOLCANO) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FURY_OF_VOLCANO);
		index++;
	}

	// 천지독살(일본-흑룡차, 세이메이 2차 전직 기술)
	if ((IsAtb4(ATB4_POISONING_HEAVENS) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_POISONING_HEAVENS);
		index++;
	}

	// 승천포(중국-발석거, 순비연<=이령 2차 전직 기술)
	if ((IsAtb4(ATB4_BOMB_OF_HEAVEN) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_BOMB_OF_HEAVEN);
		index++;
	}

	// 앙천대소(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	if ((IsAtb4(ATB4_SIEGE_MODE) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_SIEGE_MODE);
		index++;
	}

	// 앙천대소=>불랑기포차 변신(중국-불랑기포차, 이령<=순비연 2차 전직 기술)
	if ((IsAtb4(ATB4_SIEGE_MODE_UNDO) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_SIEGE_MODE_UNDO);
		index++;
	}

	// 맹격화룡파(대만-화룡차, 손유창 2차 전직 기술)
	if ((IsAtb4(ATB4_ONSLAUGHT_FIREBALL) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_ONSLAUGHT_FIREBALL);
		index++;
	}

	// 천벌화시(대만-봉황비조, 장선화 2차 전직 기술) 
	if ((IsAtb4(ATB4_FIRERAIN_OF_GOD) == TRUE) && (HostID.IsEmpty() == TRUE))
	{
		PushMenu(index, ITEM_MAGIC_FIRERAIN_OF_GOD);
		index++;
	}

	return TRUE;
}


