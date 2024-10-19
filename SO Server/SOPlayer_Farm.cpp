#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "OnlineCommonStruct.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÇÃ·¹ÀÌ¾î°¡ ¼ÒÀ¯ÇÏ°í ÀÖ´Â ½Ã¼³¹°À» ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-04 ¿ÀÈÄ 7:38:43 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
stFarmData*	SOPlayer::GetOwnFactory()
{
//	return pMyWorld->GetOwnFactory( stAccountInfo.siDBAccount, stAccountInfo.uiSlot );
	return	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	³óÀåÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvFarmInfo()
{
	BOOL	bSuccess = FALSE;

	if( IsInVillage() == FALSE )									goto	END;

	if( stAccountInfo.pVillage->DoesFactoryExist() == FALSE )		goto	END;
	
	bSuccess	=	stAccountInfo.pVillage->m_pIndustrialComplexs->SendIndustrialComplexList( this );					
	

END:
	
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_FARM_INFO, SO_SFM_ONLYME, LPVOID( -1 ) );
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ê¾÷ ´ÜÁö¸¦ ±¸ÀÔÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-08 ¿ÀÈÄ 8:29:14 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOPlayer::RecvFarmBuy()
{
	OnReqFarmBuyMsg			*pReqFarmBuyMsg;
	BOOL					bResult = FALSE;
	
	pReqFarmBuyMsg	=		(OnReqFarmBuyMsg*)cMsg;

	// ÀÌ¹Ì ±¸ÀÔÇÑ »ê¾÷ ´ÜÁö°¡ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( pMyWorld->IsIndustrialComplexOwner( this ) == TRUE )				goto	END;

	if( IsInVillage() == FALSE )											goto	END;
		
	bResult	=	stAccountInfo.pVillage->m_pIndustrialComplexs->BuyIndustrialComplex( this, pReqFarmBuyMsg->uiFarmIndex);
	
END:

	if( bResult == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID(ON_RET_NO) );
	}

	/*
	if(stAccountInfo.pVillage )
	{
		bResult	=	stAccountInfo.pVillage->BuyFarm( this, pReqFarmBuyMsg->uiPlantID, pReqFarmBuyMsg->uiFarmIndex);
	}
	if( bResult )
		SendFieldMsg( ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID(ON_RET_OK) );
	else
	*/	
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	»ý»ê °Ç¹°À» ÆÈ¾Æ ¹ö¸°´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID SOPlayer::RecvFarmSell()
{
	OnReqFarmBuyMsg			*pReqFarmBuyMsg;
	BOOL					bResult = FALSE;
	
	pReqFarmBuyMsg	=		(OnReqFarmBuyMsg*)cMsg;

	if(stAccountInfo.pVillage != NULL)
	{
		if( stAccountInfo.pVillage->DoesFactoryExist() == TRUE )
		{
			bResult	=	stAccountInfo.pVillage->m_pIndustrialComplexs->SellFactory( this, pReqFarmBuyMsg->uiFarmIndex );
		}

/*
		if(pReqFarmBuyMsg->uiFarmIndex >=0 && pReqFarmBuyMsg->uiFarmIndex <ON_VILLAGE_FARM_NUM )
		{
			bResult	=	stAccountInfo.pVillage->BuyFarm( this ,pReqFarmBuyMsg->uiPlantID, pReqFarmBuyMsg->uiFarmIndex);
			if( bResult	==	TRUE )
			{
				SendFieldMsg(ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
			}
		}
		*/
	}
	if( bResult == FALSE )
		SendFieldMsg(ON_RESPONSE_FARM_BUY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	³óÀåÀÇ ½½·ÔÁ¤º¸¸¦ ¾ò¾î¿Â´Ù.(³óÀå¿¡ µé¾î°¡¸é¼­ ³óÀå Àü°æÀ» º¸¿©ÁÙ¶§...)
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 2:06:16 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID SOPlayer::RecvFarmSlotInfo()
{
	OnReqFarmSlotInfoMsg				*pFarmSlotinfoMsg;
	BOOL								bSuccess  = FALSE;

	pFarmSlotinfoMsg			=	(OnReqFarmSlotInfoMsg*)cMsg;
	if(stAccountInfo.pVillage)
	{

		bSuccess	=	stAccountInfo.pVillage->EnterStructure( this, ON_VIL_ST_KIND_FARM, 0, pFarmSlotinfoMsg->uiFarmIndex);
			
	}

	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_FARM_SLOTINFO, SO_SFM_ONLYME, LPVOID( -1 ) );
	}

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	³óÀåÀÇ °Ç¹°À» Áþ´Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-23 ¿ÀÈÄ 3:13:52 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvFarmBuildStruct()
{	
	OnReqFarmBuildStructMsg				*pFarmBuildStructMsg;
	BOOL								bSuccess = FALSE;	

	if( stAccountInfo.pVillage == NULL )	goto	END;

	pFarmBuildStructMsg		=	( OnReqFarmBuildStructMsg* ) cMsg;
	//dwResult				=	stAccountInfo.pVillage->BuildFarmStruct( this, pFarmBuildStructMsg->uiSlotIndex, 
	//																	pFarmBuildStructMsg->Code, pFarmBuildStructMsg->siPay );
	bSuccess				=	stAccountInfo.pVillage->ConstructFactory( this, pFarmBuildStructMsg->uiSlotIndex, pFarmBuildStructMsg->Code, pFarmBuildStructMsg->siPay );

END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_FARM_BUILDSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}

}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	³óÀåÀÇ ½½·ÔÀ» È®ÀåÇÑ´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvFarmUpgradeSlot()
{
	OnReqFarmUpgradeSlotMsg			*pUpgradeSlotMsg;
	BOOL							bSuccess = FALSE;	
	
	pUpgradeSlotMsg				=	(OnReqFarmUpgradeSlotMsg*)	cMsg;

	if( IsInVillage() == FALSE )		goto	END;

	bSuccess = stAccountInfo.pVillage->UpgradeSlot( this, pUpgradeSlotMsg->uiSlotIndex, pUpgradeSlotMsg->siPay );

END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_FARM_UPGRADESLOT, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}

}
//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	³óÀåÀÇ ½½·ÔÀ» È®ÀåÇÑ´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvFarmExpandSlot()
{
	OnReqFarmExpandSlotMsg			*pFarmExpandSlotMsg;
	BOOL							bSuccess  = FALSE;
	
	pFarmExpandSlotMsg		=		(OnReqFarmExpandSlotMsg*)cMsg;		

	if( IsInVillage() == TRUE )
	{	
		bSuccess	= stAccountInfo.pVillage->ExpandFarmSlot( this, pFarmExpandSlotMsg->uiPlantID, pFarmExpandSlotMsg->uiFarmIndex );
	}

	if( bSuccess == FALSE )
	{

		SendFieldMsg(ON_RESPONSE_FARM_EXPANDSLOT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
		
	}

}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	³óÀå °Ç¹°À» Á¦°ÅÇÑ´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvFarmDestroyStruct()
{
	OnReqFarmDestroyStructMsg		*pFarmDestroyStructMsg;
	BOOL							bSuccess;
	
	pFarmDestroyStructMsg			=	(OnReqFarmDestroyStructMsg*)	cMsg;

	if( IsInVillage() == FALSE )			goto	END;
	
	bSuccess	=	stAccountInfo.pVillage->DestroyPlantSlot( this, pFarmDestroyStructMsg->uiSlotIndex );

		/*if(stAccountInfo.pVillage->DestroyPlantSlot(this, pFarmDestroyStructMsg->uiSlotIndex) == TRUE)
		{
			SendFieldMsg(ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
			return;
		}
		*/

END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_FARM_DESTROYSTRUCT, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}

}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	½Ã¼³¹°À» ¼³Ä¡ ÇÑ´Ù.(°Ç¹° ¾÷±×·¹ÀÌµå)
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvFarmSetEquipment()
{
	OnReqFarmSetEquipmentMsg		*pFarmSetEquipmentMsg;
	
	if(stAccountInfo.pVillage != NULL)
	{
		pFarmSetEquipmentMsg		=	(OnReqFarmSetEquipmentMsg*) cMsg;
//		if(stAccountInfo.pVillage->SetFarmEquipment(this, pFarmSetEquipmentMsg->uiPlantID,
//		pFarmSetEquipmentMsg->uiFarmIndex, pFarmSetEquipmentMsg->uiSlotIndex,pFarmSetEquipmentMsg->uiUpIndex) == TRUE)
		{
			SendFieldMsg(ON_RESPONSE_FARM_SETEQUIPMENT, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
			return;
		}
	}
	SendFieldMsg(ON_RESPONSE_FARM_SETEQUIPMENT, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
};


//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	¾ÆÀÌÅÛÀ» »ý»êÇÑ´Ù.(°èÈ¹À» ¼¼¿î´Ù.)
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvFarmItemProduct()
{
	OnReqFarmItemProductMsg			*pFarmItemProductMsg;
	DWORD							dwResponse = ON_RET_NO;

	pFarmItemProductMsg		=	(OnReqFarmItemProductMsg*)cMsg;

	if( IsInVillage() == FALSE )			goto	END;	

	dwResponse		=	stAccountInfo.pVillage->ProductItem( this, pFarmItemProductMsg->uiPlantID, pFarmItemProductMsg->uiFarmIndex,
															pFarmItemProductMsg->uiSlotIndex, pFarmItemProductMsg->uiItemCode, 
															pFarmItemProductMsg->uiQuantity,pFarmItemProductMsg->uiPay);

END:

	if( dwResponse != ON_RET_OK )
	{
		SendFieldMsg( ON_RESPONSE_FARM_ITEMPRODUCT, SO_SFM_ONLYME, LPVOID( dwResponse ) );
	}	

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ý»êÀ» À§ÇÑ ÀÏÀ» ÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-11 ¿ÀÈÄ 4:13:44 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvFarmWorkStart()
{
	OnReqFarmWorkStartMsg			*pFarmWorkStartMsg;
	BOOL							bSuccess = FALSE;	

	if(stAccountInfo.pVillage != NULL)
	{
		pFarmWorkStartMsg		=	(OnReqFarmWorkStartMsg*) cMsg;
		
		//bSuccess				= stAccountInfo.pVillage->PlantWorkAble( this, pFarmWorkStartMsg->uiSlotIndex );
		bSuccess				= stAccountInfo.pVillage->PlantWorkAble( this );
	}

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_FARM_WORKSTART, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÏÀ» ³¡¸¶ÃÆ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-07-22 ¿ÀÈÄ 2:00:14 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvFarmWork()
{
	OnReqFarmWorkMsg				*pFarmWorkMsg;
	stFarmSlot						*pFarmSlot = NULL;	
	
	BOOL							bSuccess	=	FALSE;

	pFarmWorkMsg		=	(OnReqFarmWorkMsg*) cMsg;

	if( IsInVillage() == FALSE )
	{
		goto	END;
	}
	
	if( stAccountInfo.pVillage->DoesFactoryExist() == FALSE )
	{
		goto	END;
	}

	bSuccess	=	stAccountInfo.pVillage->m_pIndustrialComplexs->FinishWork( this, pFarmWorkMsg );

	if( bSuccess == TRUE )
	{
		// Äù½ºÆ®¿¡ ÀÏÇÑ ¾çÀ» ¾Ë·ÁÁØ´Ù.
		Quest_AddQuestObject_Work( pFarmWorkMsg->siLabor );
	}


END:
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_FARM_WORK, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÛ¾÷·® ÇØÅ· ¸ð´ÏÅÍ¸µ ·Î±×
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-21 ¿ÀÀü 11:54 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvFarmWorkHack()
{	
	OnFarmWorkHack*		pRecvMsg;

	pRecvMsg	=	(OnFarmWorkHack*)cMsg;

//	pMyWorld->clServer->WriteInfo( "..\\RecvFarmWorkHack.txt", "DBAccount(%d)  szID(%s)  uiType(%d)  siStep(%d)  siPoint1(%d)  siPoint2(%d)",
//									stAccountInfo.siDBAccount, szID, pRecvMsg->uiType, pRecvMsg->siStep, pRecvMsg->siPoint1, pRecvMsg->siPoint2 );
	writeInfoToFile( "RecvFarmWorkHack.txt", "DBAccount(%d)  szID(%s)  uiType(%d)  siStep(%d)  siPoint1(%d)  siPoint2(%d)",
									stAccountInfo.siDBAccount, szID, pRecvMsg->uiType, pRecvMsg->siStep, pRecvMsg->siPoint1, pRecvMsg->siPoint2 );

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÛ¾÷·®À» Ã¼Å©ÇÏ¿© Á¤»óÀÌ¸é TRUE, ÇØÅ·ÀÇ ¿ì·Á°¡ ÀÖÀ¸¸é FALSE.. ÇÏµåÄÚµùÀÇ Áø¼ö -_-;
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:35 - ¹é»óÇö, 2004³â 11¿ù 13ÀÏ Ãß°¡ ¶±Ä¥ ÀÓ°úÀå~
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack( UI08 uiType, SI08 siStep, UI32 uiQuantity )
{
	if(siStep > 10)
	{
		if(pMyWorld->dwtimeGetTime > m_dwCondLaborTime)
		{
			if( (pMyWorld->dwtimeGetTime - m_dwCondLaborTime) < 1000 * 60 )
			{
				m_dwCondLaborTime = 0;
				return(FALSE);
			}
		}
		m_dwCondLaborTime = pMyWorld->dwtimeGetTime;
	}

	switch( uiType )
	{
	case MINIGAME_TYPE_1:
		return CheckLaborHack1( siStep, uiQuantity );		break;
	case MINIGAME_TYPE_2:
		return CheckLaborHack2( siStep, uiQuantity );		break;
	case MINIGAME_TYPE_3:
		return CheckLaborHack3( siStep, uiQuantity );		break;
	default:
		return FALSE;										break;
	}
}

/*
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Ä«µå Â¦ ¸ÂÃß±â ÀÏ·® Ã¼Å©..
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:36 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack1( SI08 siStep, UI32 uiQuantity )
{
	const UI32	uiCard[17]	=	{ 0, 12, 12, 16, 16, 18, 18, 24, 24, 30, 30, 24, 24, 32, 32, 40, 40 };	// 16´Ü°è

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 3 * uiCard[i] / 2;
	}
	UI32	uiMaxQuantity	=	uiPoint / 30;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¸¶ÀÛ ÀÏ·® Ã¼Å©..
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:36 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack2( SI08 siStep, UI32 uiQuantity )
{
	const UI32 uiCard[19] = { 0, 12, 12, 18, 18, 20, 20, 24, 24, 30, 30, 40, 40, 42, 42, 48, 48, 56, 56 };	// 18´Ü°è

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 4 * uiCard[i] / 3;
	}
	UI32	uiMaxQuantity	=	uiPoint / 75;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	°úÀÏ ¹Þ±â ÀÏ·® Ã¼Å©.. ÇÏµå ÄÚµùÀÇ Áø¼ö 2Åº -_-;;
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:40 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack3( SI08 siStep, UI32 uiQuantity )
{
	const UI32	uiNumFruit[11]	=	{ 0, 50, 50, 60, 60, 70, 70, 80, 80, 90, 90 };	// 10´Ü°è

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += (i + 1) * uiNumFruit[i] + uiNumFruit[i] * (uiNumFruit[i] + 1 ) / 6;
	}
	UI32 uiMaxQuantity = uiPoint / 50;

	if( uiQuantity > uiMaxQuantity )	return FALSE;		// Hacked
	else								return TRUE;
}
*/


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Ä«µå Â¦ ¸ÂÃß±â ÀÏ·® Ã¼Å©..
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:36 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack1( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD1 17
	const UI32	uiCard[MAX_CARD1]	=	{ 0, 12, 12, 16, 16, 18, 18, 24, 24, 30, 30, 24, 24, 32, 32, 40, 40 };	// 16´Ü°è

	if(siStep >= MAX_CARD1) return(FALSE);

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 3 * uiCard[i] / 2;
	}
	UI32	uiMaxQuantity	=	uiPoint / 30;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¸¶ÀÛ ÀÏ·® Ã¼Å©..
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:36 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack2( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD2 19
	const UI32 uiCard[MAX_CARD2] = { 0, 12, 12, 18, 18, 20, 20, 24, 24, 30, 30, 40, 40, 42, 42, 48, 48, 56, 56 };	// 18´Ü°è

	if(siStep >= MAX_CARD2) return(FALSE);

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 4 * uiCard[i] / 3;
	}
	UI32	uiMaxQuantity	=	uiPoint / 75;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	°úÀÏ ¹Þ±â ÀÏ·® Ã¼Å©.. ÇÏµå ÄÚµùÀÇ Áø¼ö 2Åº -_-;;
//	¼öÁ¤ ÀÏÀÚ	:	2003-07-25 ¿ÀÈÄ 6:40 - ¹é»óÇö
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL SOPlayer::CheckLaborHack3( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD3 11
	const UI32	uiNumFruit[MAX_CARD3]	=	{ 0, 50, 50, 60, 60, 70, 70, 80, 80, 90, 90 };	// 10´Ü°è

	if(siStep >= MAX_CARD3) return(FALSE);

	// siStep ´Ü°è±îÁöÀÇ ´©Àû Æ÷ÀÎÆ®.. 0´Ü°è´Â 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += (i + 1) * uiNumFruit[i] + uiNumFruit[i] * (uiNumFruit[i] + 1 ) / 6;
	}
	UI32 uiMaxQuantity = uiPoint / 50;

	if( uiQuantity > uiMaxQuantity )	return FALSE;		// Hacked
	else								return TRUE;
}




//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	°øÀåÀÇ Á¤º¸¸¦ ¿äÃ»Çß´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-08 ¿ÀÈÄ 2:15:04 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvFarmWorkInfo()
{
	OnReqFarmWorkInfoMsg		*pFarmWorkInfoMsg;
	BOOL						bSuccess = FALSE;
	
	pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
	
	if( IsInVillage() == FALSE )										goto	END;
	
	bSuccess	=	stAccountInfo.pVillage->EnterIndustrialComplexFactory( this, SI32( pFarmWorkInfoMsg->uiSlotIndex ) );
		//bSuccess	=	stAccountInfo.pVillage->m_pIndustrialComplexs->SendFactoryInfo( this, pFarmWorkInfoMsg->uiSlotIndex );

			/*
		if(stAccountInfo.pStructure)
		{
			pFarmWorkInfoMsg		=	(OnReqFarmWorkInfoMsg*) cMsg;
			pFarmSlot	= stAccountInfo.pStructure->GetFarmSlot(pFarmWorkInfoMsg->uiSlotIndex);
			if(pFarmSlot)
			{
				if( pFarmSlot->bOnSchedule() )
				{
					SendFieldMsg(ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pFarmSlot));
					return;
				}
				else if( pFarmSlot->bStore )
				{
					if( stAccountInfo.pVillage->IsFarmOwner( this ) )
					{
						SendFieldMsg(ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK),LPVOID(pFarmSlot));
						return;
					}
				}
			}
		}
	
	}
	*/
END:

	if( bSuccess == FALSE )
	{
		SendFieldMsg(ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));
	}

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Ã¢°í¿¡ ÀÖ´Â ¾ÆÀÌÅÛÀ» º¸³½´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-08 ¿ÀÈÄ 3:13:24 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvStorageInfo()
{
	OnReqStorageInfoMsg				*pStorageInfoMsg;
	BOOL							bSuccess = FALSE;

	pStorageInfoMsg				=	(OnReqStorageInfoMsg*)	cMsg;		

	if( IsInVillage() == FALSE )									goto	END;

	bSuccess	=	stAccountInfo.pVillage->EnterIndustrialComplexFactory( this, SI32( pStorageInfoMsg->uiSlotIndex ) );
		/*
		if(stAccountInfo.pStructure)
		{
			pStorageInfoMsg				=	(OnReqStorageInfoMsg*)	cMsg;
			pFarmSlot	=	stAccountInfo.pStructure->GetFarmSlot(pStorageInfoMsg->uiSlotIndex);
			if(pFarmSlot)
			{
				if(pFarmSlot->bStore)
				{
					if( pFarmSlot->bOnSchedule() )
					{
						SendFieldMsg( ON_RESPONSE_FARM_WORKINFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK),LPVOID(pFarmSlot));
						return;
					}
					else
					{
						if(stAccountInfo.pVillage->IsFarmOwner(this))
						{
							SendFieldMsg( ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_OK), LPVOID(pFarmSlot));
							return;
						}
					}
				}
			}
		}
		*/
	
END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg(ON_RESPONSE_STORAGE_INFO, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

	}

}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Ã¢°í¿¡ ÀÖ´Â ¾ÆÀÌÅÛÀ» ³Ö´Â´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvStorageInputItem()
{
	OnReqStorageInputItemMsg				*pStorageInputItemMsg;
	BOOL									bSuccess = FALSE;

	SI32 FarmID = -1;
	SI32 FactoryID = -1;
	SI32 FactorySlot = -1;

	SI32 InvenSlot;
	

	if( IsInVillage() == FALSE )			goto	END;		

	pStorageInputItemMsg		=	(OnReqStorageInputItemMsg*) cMsg;

	// ÇØ´ç ¿ëº´ÀÌ ¾ÆÀÌÅÛÀ» °¡Áö°í ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( IFollower.HasItem( pStorageInputItemMsg->uiFollowerID, pStorageInputItemMsg->uiItemCode, UI16( pStorageInputItemMsg->uiQunatity ) ) == FALSE )	goto	END;

	InvenSlot = IFollower.GetItemIndex(pStorageInputItemMsg->uiFollowerID, pStorageInputItemMsg->uiItemCode);

	

	if( stAccountInfo.pVillage->StorageInputItem( this, pStorageInputItemMsg->uiItemCode, pStorageInputItemMsg->uiQunatity ) == FALSE )						goto	END;

	// ÃÖ¾ÇÀÇ ÄÚµåµîÀå!
	// db query¿¡ ¾²±âÀ§ÇØ ¾ÆÀÌÅÛÀÌ ÀúÀåµÈ ½½·ÔÀ» ¾ò¾î¿Â´Ù.
	FarmID = stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplexIndex(this);

	if ( FarmID != -1) {
		FactoryID = stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplex(this)->GetEnteredFactoryIndex(this);

		if (FactoryID != -1) {
			stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplex(this)->GetEnteredFactory(this)->GetItem(pStorageInputItemMsg->uiItemCode,&FactorySlot);
			if (FactorySlot == -1)
			{
				goto END;
			}

		}
		else
		{
			goto END;
		}
	}
	else
	{
		goto END;
	}
	
	// ÇØ´ç ¿ëº´ÀÇ ¾ÆÀÌÅÛÀ» »«´Ù.
	DelItemNoQuery( pStorageInputItemMsg->uiFollowerID, pStorageInputItemMsg->uiItemCode, pStorageInputItemMsg->uiQunatity );		

	bSuccess	=	TRUE;

	


END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_STORAGE_INPUTITEM, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}
	else
	{

		DBIKStorageInputItem  info;
		info.DBAccount = this->stAccountInfo.siDBAccount;
		info.uiSlot = this->stAccountInfo.uiSlot;		

		info.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
		info.uiFarmID = FarmID;
		info.uiFactoryID = FactoryID;

		info.uiItemCode = pStorageInputItemMsg->uiItemCode;
		info.uiItemQnt = pStorageInputItemMsg->uiQunatity;
		info.uiMercenary = pStorageInputItemMsg->uiFollowerID;
		info.uiMercenaryItemPosition = InvenSlot;

		info.uiStoreItemPosition = FactorySlot;

		
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_STORAGE_INPUT_ITEM,&info,sizeof(info));


		SendFieldMsg( ON_RESPONSE_STORAGE_INPUTITEM, SO_SFM_ONLYME, LPVOID( ON_RET_OK ) );
	}
}

//----------------------------------------------------------------------------------------------------------------
//	¼³¸í	:	Ã¢°í¼­ ¾ÆÀÌÅÛÀ» ²¨³½´Ù.
//----------------------------------------------------------------------------------------------------------------
VOID	SOPlayer::RecvStorageOutputItem()
{
	OnReqStorageOutputItemMsg				*pStorageOutputItemMsg;
	UI32									uiTemp=0;
	BOOL									bSuccess = FALSE;

	SI32 FarmID = -1;
	SI32 FactoryID = -1;
	SI32 FactorySlot = -1;

	SI32 InvenSlot;
	

	if( IsInVillage() == FALSE )			goto	END;		
	
	pStorageOutputItemMsg		=	(OnReqStorageOutputItemMsg*) cMsg;

	// ÇØ´ç ¿ëº´¿¡°Ô ¾ÆÀÌÅÛÀ» ³ÖÀ» ¼ö ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( IFollower.CanAddItem( pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode, UI16( pStorageOutputItemMsg->uiQunatity ) ) == FALSE )	goto	END;

	

	// ÃÖ¾ÇÀÇ ÄÚµå ¶Ç µîÀå!
	// ÀÌ¹ø¿¡´Â ¾ÆÀÌÅÛÀ» ²¨³»±âÀü¿¡ ¹Ì¸® À§Ä¡¸¦ È®ÀÎÇÑ´Ù.
	FarmID = stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplexIndex(this);

	if (FarmID != -1) {
		FactoryID = stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplex(this)->GetEnteredFactoryIndex(this);
		if (FactoryID != -1) {
			stAccountInfo.pVillage->m_pIndustrialComplexs->GetEnteredIndustrialComplex(this)->GetEnteredFactory(this)->GetItem(pStorageOutputItemMsg->uiItemCode,&FactorySlot);
			if (FactorySlot == -1)
			{
				goto END;
			}
		}
		else
		{
			goto	END;
		}
	}
	else
	{
		goto	END;

	}
	
	

	if( stAccountInfo.pVillage->StorageOutputItem( this, pStorageOutputItemMsg->uiItemCode, pStorageOutputItemMsg->uiQunatity ) == FALSE )						goto	END;

	// ÇØ´ç ¿ëº´ÀÇ ¾ÆÀÌÅÛÀ» ´õÇÑ´Ù.
	AddItemNoQuery( pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode, pStorageOutputItemMsg->uiQunatity );

	// ¾ÆÀÌÅÛÀ» ³Ö°í³ª¼­ µé¾î°£ À§Ä¡¸¦ È®ÀÎÇÑ´Ù.
	InvenSlot = IFollower.GetItemIndex(pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode);
	

	bSuccess	=	TRUE;
		
/*		if(stAccountInfo.pVillage->StorageOutputItem( pStorageOutputItemMsg->uiPlantID,pStorageOutputItemMsg->uiPartID,
			pStorageOutputItemMsg->uiSlotIndex,pStorageOutputItemMsg->uiItemCode, pStorageOutputItemMsg->uiQunatity) != 0)
		{			
			AddItem(pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode, pStorageOutputItemMsg->uiQunatity,pStorageOutputItemMsg->siPosInInventory);
			SendFieldMsg(ON_RESPONSE_STORAGE_OUTPUTITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK));
			return;
		}
		*/
	
END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg(ON_RESPONSE_STORAGE_OUTPUTITEM, SO_SFM_ONLYME, LPVOID(ON_RET_NO));

	}
	else
	{

		DBIKStorageOutputItem  info;

		info.DBAccount = stAccountInfo.siDBAccount;
		info.uiSlot = stAccountInfo.uiSlot;		

		info.uiVillageCode = stAccountInfo.pVillage->uiVillageCode;
		info.uiFarmID = FarmID;
		info.uiFactoryID = FactoryID;

		info.uiItemCode = pStorageOutputItemMsg->uiItemCode;
		info.uiItemQnt = pStorageOutputItemMsg->uiQunatity;
		info.uiMercenary = pStorageOutputItemMsg->uiFollowerID;
		info.uiMercenaryItemPosition = InvenSlot;

		info.uiStoreItemPosition = FactorySlot;

		
		pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_STORAGE_OUTPUT_ITEM,&info,sizeof(info));

		SendFieldMsg(ON_RESPONSE_STORAGE_OUTPUTITEM, SO_SFM_ONLYME, LPVOID(ON_RET_OK));

	}
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÓ±ÝÀ» ¹Ù²Û´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-07-24 ¿ÀÀü 11:37:15 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvPlantChangPay()
{
	OnReqPlantChangePayMsg					*pPlantChangePayMsg;
	DWORD									dwResult= ON_RET_NO;
	BOOL									bSuccess = FALSE;
	
	if(stAccountInfo.pVillage != NULL)
	{
		pPlantChangePayMsg		=	(OnReqPlantChangePayMsg*) cMsg;
	
		bSuccess				=	stAccountInfo.pVillage->ChangePay( this, pPlantChangePayMsg->uiSlotIndex, pPlantChangePayMsg->siPay );		

		//dwResult				=	stAccountInfo.pVillage->ChangePay(this, pPlantChangePayMsg->uiSlotIndex, pPlantChangePayMsg->siPay);		
	}
	
	// ÀÓ±ÝÀ» ¹Ù²Ù¸é¼­ ¾ò´Â ÀÌÀÍÀ» º¸³»ÁØ´Ù.
	if( bSuccess == FALSE )
	{
		//SendFieldMsg(ON_RESPONSE_PLANT_CHANGE_PAY, SO_SFM_ONLYME, LPVOID(dwResult),LPVOID(&siMoney));
		SendFieldMsg(ON_RESPONSE_PLANT_CHANGE_PAY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

	
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÛ¾÷À» Ãë¼ÒÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-12 ¿ÀÈÄ 2:36:44 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvPlantCancelPlan()
{
	OnReqPlantCancelPlanMsg					*pPlantChangePayMsg;
	BOOL									bSuccess = FALSE;	
	
	pPlantChangePayMsg			=	(OnReqPlantCancelPlanMsg*) cMsg;

	if( IsInVillage() == FALSE )									goto	END;
	
	bSuccess		=	stAccountInfo.pVillage->CancelWork( this, SI32( pPlantChangePayMsg->uiSlotIndex ) );

END:

	if( bSuccess == FALSE )
	{

		SendFieldMsg( ON_RESPONSE_PLANT_CANCEL_PLAN, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );

	}
}


