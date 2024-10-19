/************************************************************************************************
	ÆÄÀÏ¸í : OnlineCharUI.cpp
	´ã´çÀÚ : ÀÌÀ±¼®

	ÀÚ½ÅÀÇ Character¿¡°Ô ¸í·ÉÀ» ³»¸²
************************************************************************************************/
#include <GSL.h>
#include <OnlineWorld.h>
#include <OnlineCharUI.h>
#include "OnlineMyData.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	»ý¼ºÀÚ.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineCharUI::OnlineCharUI()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¼Ò¸êÀÚ.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineCharUI::~OnlineCharUI()
{
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÃÊ±âÈ­.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Init(cltOnlineWorld *pOnlineWorld)
{
	OnlineCharUI::pOnlineWorld		=	pOnlineWorld;
	siPrevSendOrderFrame			=	-15;
	Init();
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÃÊ±âÈ­.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Init()
{
	Order.siOrder			=	ON_ORDER_NONE;		
	bAction					=	TRUE;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÇØÁ¦	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Free()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Æú¸µ ÇÔ¼ö.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::Poll()
{
	if(bAction == FALSE)								return;

	// Æ¯Á¤ ÇÁ·¹ÀÓ °£°ÝÀ¸·Î ½ÇÁ¦ ³ªÀÇ Ä³¸¯ÅÍ¿¡°Ô ¿©±â¿¡ ÀÖ´Â ¸í·ÉÀ» º¸³»ÁØ´Ù.
	// ÇöÀç ¸í·ÉÀÌ ÀÖÀ¸¸é º¸³»ÁØ´Ù.
	if(Order.siOrder != ON_ORDER_NONE)
	{
		// 5 ÇÁ·¹ÀÓ¸¶´Ù ÇÑ¹ø¾¿ º¸³»ÁØ´Ù.
		if(SI32(pOnlineWorld->GetTotalFrame()) > siPrevSendOrderFrame + 5)
		{			
			if(pOnlineWorld->pIOnlineChar->AddOrder(pOnlineWorld->pMyData->GetMyCharUniqueID(), Order) == TRUE)
			{
				siPrevSendOrderFrame	=	pOnlineWorld->GetTotalFrame();

				// ±×¸®°í Order ¹öÆÛ¸¦ ÃÊ±âÈ­ ½ÃÅ²´Ù.
				Init();			
			}
		}
	}		
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÌ Å¬·¡½º¿¡ ÀÛµ¿ ¿©ºÎ¸¦ ¼³Á¤ÇÑ´Ù.,	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::SetAction(BOOL baction)
{
	Init();

	bAction		=		baction;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	ÀÌµ¿ÇÏµµ·Ï ¼³Á¤ÇÑ´Ù.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineCharUI::SetMove(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_MOVE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	°ø°Ý.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¿ÀºêÁ§Æ®¸¦ »ç¿ëÇÑ´Ù.(¸¶À»Àº µé¾î°£´Ù, ÀÌÁ¤Ç¥¸¦ º»´Ù)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetUseObject(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_USEOBJECT;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

// robypark 2005/1/27 18:49
// ¸¶À» ¼ö¼ºÀÎÅÍÆäÀÌ½º·Î µé¾î°¡°Ú´Ù°í Ã³¸®
VOID	OnlineCharUI::SetUseObject_VillageDefence(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶À» °ø°Ý(±Ù°Å¸®)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetShortDistanceAttackVillage(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸¶À» °ø°Ý(¿ø°Å¸®)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetLongDistanceAttackVillage(SI32 siX, SI32 siY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY;
	Order.siOrderParam1		=	siX;
	Order.siOrderParam2		=	siY;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¾ÆÀÌÅÛÀ» ÁÝ´Â´Ù
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetPickUpItem(SI32 siX, SI32 siY, UI32 uiItemID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_PICKUPITEM;
	Order.siOrderParam1		=	MAKELPARAM(siX, siY);
	Order.siOrderParam2		=	uiItemID;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	NPC 
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetUseNPC(SI32 siX, SI32 siY, SI16 siNPCCode)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_NPC;
	Order.siOrderParam1		=	MAKELPARAM(siX, siY);
	Order.siOrderParam2		=	siNPCCode;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// robypark 2004/11/9 17:15
// ¼³¸í		:	°ø¼ºÀü º¸±Þ ¼ö·¹¿¡ °ø¼ºÀü À¯´ÖÀÌ º¸±ÞÇ° ¿äÃ»
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetRequestSupplyGoodsFromSupplyWagon(UI16 uiSupplyWagonUniqueID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON;
	Order.siOrderParam1		=	uiSupplyWagonUniqueID;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// robypark 2004/11/9 18:20
// ¼³¸í		:	°ø¼ºÀü º¸±Þ ¼ö·¹°¡ ¸¶À»¿¡ º¸±ÞÇ° ¿äÃ»
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	OnlineCharUI::SetRequestSupplyGoodsFromVillage(SI16 uiVillagePosX, SI16 uiVillagePosY)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE;
	Order.siOrderParam1		=	uiVillagePosX;
	Order.siOrderParam2		=	uiVillagePosY;
}

// robypark 2005/1/25 18:53
// º¸±Þ ¼ö·¹°¡ ´Ù¸¥ °ø¼ºÀü À¯´Ö Ä¡·á ¸í·É
VOID	OnlineCharUI::SetOrderHeal(UI16 uiDestUniqueID)
{
	if(bAction == FALSE)	return;

	Order.siOrder			=	ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL;
	Order.siOrderParam1		=	uiDestUniqueID;
}
