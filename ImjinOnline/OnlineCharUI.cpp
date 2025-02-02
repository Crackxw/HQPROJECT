/************************************************************************************************
	だ橾貲 : OnlineCharUI.cpp
	氬渡濠 : 檜彌戮

	濠褐曖 Character縑啪 貲滄擊 頂葡
************************************************************************************************/
#include <GSL.h>
#include <OnlineWorld.h>
#include <OnlineCharUI.h>
#include "OnlineMyData.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineCharUI::OnlineCharUI()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	模資濠.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineCharUI::~OnlineCharUI()
{
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Init(cltOnlineWorld *pOnlineWorld)
{
	OnlineCharUI::pOnlineWorld		=	pOnlineWorld;
	siPrevSendOrderFrame			=	-15;
	Init();
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Init()
{
	Order.siOrder			=	ON_ORDER_NONE;		
	bAction					=	TRUE;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	п薯	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Free()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	ィ葭 л熱.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Poll()
{
	if(bAction == FALSE)								return;

	// か薑 Щ溯歜 除問戲煎 褒薯 釭曖 議葛攪縑啪 罹晦縑 氈朝 貲滄擊 爾頂遽棻.
	// ⑷營 貲滄檜 氈戲賊 爾頂遽棻.
	if(Order.siOrder != ON_ORDER_NONE)
	{
		// 5 Щ溯歜葆棻 и廓噶 爾頂遽棻.
		if(SI32(pOnlineWorld->GetTotalFrame()) > siPrevSendOrderFrame + 5)
		{			
			if(pOnlineWorld->pIOnlineChar->AddOrder(pOnlineWorld->pMyData->GetMyCharUniqueID(), Order) == TRUE)
			{
				siPrevSendOrderFrame	=	pOnlineWorld->GetTotalFrame();

				// 斜葬堅 Order 幗ぷ蒂 蟾晦�� 衛鑑棻.
				Init();			
			}
		}
	}		
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜 贗楚蝶縑 濛翕 罹睡蒂 撲薑и棻.,	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::SetAction(BOOL baction)
{
	Init();

	bAction		=		baction;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜翕ж紫煙 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::SetMove(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_MOVE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	奢問.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetAttack(UI16 uiUniqueID)
{	
	if(bAction == FALSE)	return;
	

	Order.siOrder			=	ON_ORDER_ATTACK;
	Order.siOrderParam1		=	uiUniqueID;	
}

//     
VOID	OnlineCharUI::SetFieldAttack(UI16	uiUniqueID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_FIELDATTACK_READY;
	Order.siOrderParam1		=	uiUniqueID;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	螃粽薛お蒂 餌辨и棻.(葆擊擎 菟橫除棻, 檜薑ル蒂 獄棻)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetUseObject(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_USEOBJECT;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

// robypark 2005/1/27 18:49
// 葆擊 熱撩檣攪む檜蝶煎 菟橫陛啊棻堅 籀葬
VOID	OnlineCharUI::SetUseObject_VillageDefence(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 奢問(斬剪葬)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetShortDistanceAttackVillage(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊 奢問(錳剪葬)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetLongDistanceAttackVillage(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 鄹朝棻
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetPickUpItem(SI32 siX, SI32 siY, UI32 uiItemID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_PICKUPITEM;
	Order.siOrderParam1		=	MAKELPARAM(siX, siY);
	Order.siOrderParam2		=	uiItemID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	NPC 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetUseNPC(SI32 siX, SI32 siY, SI16 siNPCCode)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_NPC;
	Order.siOrderParam1		=	MAKELPARAM(siX, siY);
	Order.siOrderParam2		=	siNPCCode;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// robypark 2004/11/9 17:15
// 撲貲		:	奢撩瞪 爾晝 熱溯縑 奢撩瞪 嶸棉檜 爾晝ヶ 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetRequestSupplyGoodsFromSupplyWagon(UI16 uiSupplyWagonUniqueID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON;
	Order.siOrderParam1		=	uiSupplyWagonUniqueID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// robypark 2004/11/9 18:20
// 撲貲		:	奢撩瞪 爾晝 熱溯陛 葆擊縑 爾晝ヶ 蹂羶
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharUI::SetRequestSupplyGoodsFromVillage(SI16 uiVillagePosX, SI16 uiVillagePosY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE;
	Order.siOrderParam1		=	uiVillagePosX;
	Order.siOrderParam2		=	uiVillagePosY;
}

// robypark 2005/1/25 18:53
// 爾晝 熱溯陛 棻艇 奢撩瞪 嶸棉 纂猿 貲滄
VOID	OnlineCharUI::SetOrderHeal(UI16 uiDestUniqueID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL;
	Order.siOrderParam1		=	uiDestUniqueID;
}
