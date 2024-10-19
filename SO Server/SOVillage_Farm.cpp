#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ý»ê °Ç¹°ÀÇ ÃÊ±â Á¤º¸¸¦ ¾ò¾î¿Â´Ù. (¼­¹ö ½Ãµ¿½Ã..)
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-23 ¿ÀÈÄ 6:04:35 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::GetFarmData()
{
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_FARM_GETINFO, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;	// °Ç¼³ °Ç¹°À» ÀÐ´Â´Ù..	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_FARM_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )		return FALSE;	// Ã¢°í µ¥ÀÌÅ¸¸¦ ÀÐ´Â´Ù..(Ã¢°í Å×ÀÌºí)	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_STORAGE_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;
	
	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÏÇÏ±â ¹öÆ°À» ´­·¶À»¶§ ÀÏÇÏ±â°¡ °¡´ÉÇÑÁö º¸³»ÁØ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-23 ¿ÀÈÄ 6:31:25 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::PlantWorkAble( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )														return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	
	return	m_pIndustrialComplexs->BeginWork( pPlayer );					
}




//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÅäÁö¸¦ °³Ã´ÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 6:59:34 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::ExpandFarmSlot( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )															return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.

	if( m_pIndustrialComplexs->Reclamation( pPlayer, SI32( uiFarmID ), &mnCapital ) == FALSE )	return	FALSE;		// ÅäÁö¸¦ °³Ã´ÇÑ´Ù.



	// ÇÃ·¹ÀÌ¾î, ½½·Ô,¸¶À» ID,°øÀå ID,»ý»ê½Ã¼³¼öÀÍ,

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);	

	return	TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	°øÀåÀ» °Ç¼³ÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 9:55:01 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.

	if( m_pIndustrialComplexs->ConstructFactory( pPlayer, SI32( siFactoryID ), cFactoryCode, mnPay, &mnCapital ) == FALSE )	return	FALSE;		// ÅäÁö¸¦ °³Ã´ÇÑ´Ù.


	IncreaseCapital( mnCapital, ON_PROFIT_PLANT , FALSE);	// db¿¡´Â ¾Ë¸®Áö¾Ê´Â´Ù

	return	TRUE;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	½½·Ô¿¡ °Ç¹°À» Ã¶°ÅÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::DestroyPlantSlot(SOPlayer *pPlayer, UI08 uiSlotID)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	
	if( m_pIndustrialComplexs->DestroyFactory( pPlayer, SI32( uiSlotID ), &mnCapital ) == TRUE )
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
	
	
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	½½·ÔÀ» ¾÷±×·¹ÀÌµå ÇÑ´Ù. (¾÷±×·¹ÀÌµå °èÈ¹À» ¼¼¿î´Ù.)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
DWORD	SOVillage::UpgradeSlot(SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																				return	FALSE;	// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.	
	
	if( m_pIndustrialComplexs->UpgradeFactory( pPlayer, SI32( uiSlot ), MONEY( siPay ), &mnCapital ) == FALSE )		return	FALSE;

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);

	

	return	TRUE;

	
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¾ÆÀÌÅÛÀ» »ý»êÇÑ´Ù.(ÃÊ±â °èÈ¹)
//	¼öÁ¤ ÀÏÀÚ	:	2002-08-17 ¿ÀÈÄ 4:22:54 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
DWORD	SOVillage::ProductItem(SOPlayer *pPlayer, UI08	uiPlantID, UI08 uiFarmID, UI08 uiSlot, UI16 uiItem, UI32 Quantity, UI32 uiPay)
{	
	MONEY	mnCapital;
	DWORD	dwResoponse;

	if( DoesFactoryExist() == FALSE )																										return	FALSE;	// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.	
	
	dwResoponse	=	m_pIndustrialComplexs->ProduceItem( pPlayer, SI32( uiSlot ), uiItem, Quantity, MONEY( uiPay ) , cNation, &mnCapital );

	if( dwResoponse	== ON_RET_OK )
	{
		IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);
	}
	
	return	dwResoponse;

	
}
/*
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Ã¢°í¿¡ Àç·á°¡ ÀÖ´ÂÁö È®ÀÎÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::CheckMaterial( UI08 uiFarmID, UI16 uiItem, UI32 Quantity )
{
	stFarmData					*pFarm;
	stFarmSlot					*pFarmSlot;
	UI08						Temp;

	if(pFarmGroup)
	{
		pFarm		=	pFarmGroup->GetFarm(uiFarmID);
		if(pFarm)
		{
			for(int i =0 ; i< pFarm->FarmInfo.uiAvailSlot; i++)
			{
				pFarmSlot	=	pFarm->GetFarmSlot(i);
				if(pFarmSlot)
				{
					if( pFarmSlot->pFacility && pFarmSlot->bStore )   // Ã¢°í¶ó¸é...
					{
						Temp	=	pFarmSlot->Storage.Search(uiItem);
						if( Temp != UI08_ERROR )
						{
							if( pFarmSlot->Storage.item[Temp].uiQuantity > Quantity )
								return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¾ÆÀÌÅÛÀ» »ý»êÇÑ´Ù.(»ý»êµÈ ¼ýÀÚ¸¦ ¸®ÅÏÇÑ´Ù.)
//	¼öÁ¤ ÀÏÀÚ	:	2002-09-23 ¿ÀÈÄ 7:14:04 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
UI32	SOVillage::FarmWork( SOPlayer *pPlayer, UI08 uiSlot, UI32 uiLabor, MONEY* uiPay )
{
	stFarmData				*pFarm;
	stFarmSlot				*pFarmSlot;
	DBIKSetFarmProduct		SetFarmProduct;
	UI32					Quantity=0, ResultLabor=0;
	MONEY					TempPay =0;
	UI08					StgSlot, StgLoc;
	UI16					StgQnt;
	UI16					uiItem;
	BOOL					bWorkEnd = FALSE;

	if( pPlayer == NULL)													return	0;			// À¯È¿ÇÑ ÇÃ·¹ÀÌ¾îÀÎÁö °Ë»çÇÑ´Ù.
	if( pFarmGroup == NULL)													return	0;			// »ý»ê ½Ã¼³¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( ( pFarm = pPlayer->stAccountInfo.pStructure ) == NULL )				return	0;			// ÇÃ·¹ÀÌ¾î°¡ »ý»ê ½Ã¼³¹° ¾È¿¡ µé¾î°¡ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( ( pFarmSlot	= pFarm->GetFarmSlot( uiSlot ) ) == NULL )				return	0;			// ÇÃ·¹ÀÌ¾î°¡ ÀÛ¾÷ÇÑ °Ç¹°ÀÌ À¯È¿ÇÑÁö °Ë»çÇÑ´Ù.
	if( pFarmSlot->pFacility == NULL )										return	0;			// ?????
	if( pFarmSlot->bWorkable ==	FALSE )										return	0;			// ÇØ´ç °Ç¹°ÀÌ ÀÛ¾÷À» ÇÒ ¼ö ÀÖ´Â »óÅÂÀÎÁö °Ë»çÇÑ´Ù;

	// ½×ÀÎ ³ëµ¿·® =  ³²Àº ³ëµ¿ + Ãß°¡µÈ ³ëµ¿
	ResultLabor		=	pFarmSlot->uiLabor + uiLabor;
	
	memset(&SetFarmProduct,0, sizeof(DBIKSetFarmProduct));
	SetFarmProduct.uiVillageCode	=	uiVillageCode;
	SetFarmProduct.uiPlantID		=	pFarmGroup->pPlant->DBIndex;
	SetFarmProduct.uiFarmID			=	pFarm->nIndex;
	SetFarmProduct.uiSlotID			=	pFarmSlot->nSlotIndex;
	SetFarmProduct.Level 			=	pFarmSlot->uiLevel+1;
	SetFarmProduct.uiItemID			=	pFarmSlot->ProductItem;
	
	if( pFarmSlot->ProductItem  == ON_PLANT_SLOTUPGRADE_CODE )
	{
		// ¾÷±×·¹ÀÌµå ÀÛ¾÷

		if( uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor				=	pFarmSlot->uiRemainLabor;
			TempPay					=	ResultLabor * pFarmSlot->siPay;
			if( pFarmSlot->bStore )						
			{	
				// Ã¢°íÀÏ °æ¿ì				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_STORAGE_LEVELUP, &SetFarmProduct, sizeof(SetFarmProduct) );

				pFarmSlot->Storage.Init();
			}
			else
			{	
				// ÀÏ¹Ý°Ç¹°ÀÏ °æ¿ì				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_LEVELUP, &SetFarmProduct, sizeof(SetFarmProduct) );

				pFarmSlot->Structure.Init();
			}

			pFarmSlot->LevelUp();
			pFarm->FarmInfo.uiRemainLabor	-=	ResultLabor;
		}
		else
		{
			SetFarmProduct.Labor		=	ResultLabor;
			SetFarmProduct.Quantity		=	ResultLabor;
			TempPay						=	uiLabor * pFarmSlot->siPay;
			if( pFarmSlot->bStore )
			{	
				// Ã¢°íÀÏ °æ¿ì				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_STORAGE_WORK, &SetFarmProduct, sizeof(SetFarmProduct) );
			}
			else
			{				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORK, &SetFarmProduct, sizeof(SetFarmProduct) );
			}
			pFarmSlot->uiLabor				=	ResultLabor;
			pFarmSlot->uiRemainLabor		-=	uiLabor;
			pFarm->FarmInfo.uiRemainLabor	-=	uiLabor;
		}
		
	}
	else
	{
		// ¾ÆÀÌÅÛ »ý»ê ÀÛ¾÷

		uiItem		=	pFarmSlot->ProductItem;
		StgSlot		=	GetEmptyStorage(SetFarmProduct.uiPlantID,SetFarmProduct.uiFarmID,uiItem,StgLoc, StgQnt);
		if(StgSlot == UI08_ERROR)		return 0L;
		
		if(  uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor	=	pFarmSlot->uiRemainLabor;
			Quantity	=	pFarmSlot->Structure.GetRemainNum();	//	ÂüÁ¶º¯¼ö: ³²Àº uiLabor°¡ ³ª¿Â´Ù.
			SetFarmProduct.Labor		=	0;
			TempPay		=	ResultLabor * pFarmSlot->siPay;
			
			// ÀÏ¹Ý°Ç¹°ÀÏ °æ¿ì			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORKEND, &SetFarmProduct, sizeof(SetFarmProduct) );

			pFarmSlot->Structure.Init();
			pFarmSlot->EndProduct();
			pFarm->FarmInfo.uiRemainLabor	-=	ResultLabor;
		}
		else
		{
			ResultLabor	=	uiLabor + pFarmSlot->uiLabor;
			Quantity	=	GetQuantityByLabor(uiItem, ResultLabor);	//	ÂüÁ¶º¯¼ö: ³²Àº uiLabor°¡ ³ª¿Â´Ù.
			SetFarmProduct.Labor		=	ResultLabor;
			// ¾ÆÀÌÅÛÀ» »ý»êÇÒ °æ¿ì
			SetFarmProduct.Quantity		=	pFarmSlot->Structure.uiNum + Quantity;
			TempPay						=	uiLabor * pFarmSlot->siPay;
			
			// DB ¾÷µ¥ÀÌÆ®			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORK, &SetFarmProduct, sizeof(SetFarmProduct));

			// ¼­¹ö ¾÷µ¥ÀÌÆ®
			pFarmSlot->uiLabor			=	ResultLabor;
			pFarmSlot->Structure.uiNum	=	SetFarmProduct.Quantity;
			pFarmSlot->uiRemainLabor		-=	uiLabor;
			pFarm->FarmInfo.uiRemainLabor	-=	uiLabor;
		}
		// ¾ÆÀÌÅÛ °¡°Ý º¯µ¿À» À§ÇØ »ý»ê·®À» ITEM Table¿¡ ÀúÀåÇÑ´Ù.
		DBIKItemNum			ItemNum;
		ItemNum.ItemID		=	uiItem;
		ItemNum.Num			=	Quantity;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_ITEMPRODUCTSET, &ItemNum, sizeof(ItemNum) );
		
		if( SetStorageItem(SetFarmProduct.uiFarmID, StgSlot, uiItem, (StgQnt+Quantity), StgLoc) == FALSE )
			return 0;
	}
	
	// ¸ðµç ÀÏÇÏ´Â ÀÛ¾÷ÀÏ °æ¿ì...
	pFarm->siWorkDay = 0;
	memcpy(uiPay, &TempPay, sizeof(MONEY));
	pPlayer->IncreaseMoney(TempPay);
	
	return Quantity;	
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	³ëµ¿·®À» ¾ÆÀÌÅÛ »ý»êÇÒ ¼ö ÀÖ´Â  °³¼ö·Î ¹Ù²Û´Ù. ³²Àº ³ëµ¿ÀÌ ³ª¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
UI32	SOVillage::GetQuantityByLabor(UI16 uiItem, UI32	&uiLabor)
{
	UI32			uiQuantity = 0;
	MakeItem		*ItemCompose	=	NULL;
	CItemHeader		*pItemHeader	=	NULL;

	pItemHeader	=	clWorld->clItem.GetItem(uiItem);
	if( pItemHeader )
	{
		ItemCompose = pItemHeader->pMakeItem;
		if(ItemCompose)
		{
			uiQuantity = (uiLabor / (ItemCompose->m_siLabor)) * ItemCompose->m_siProduction;
//			printf("³ëµ¿·Â[%u]À¸·Î ¾ÆÀÌÅÛ[%u]°³ÀÇ ¾ÆÀÌÄÜÀ» ¸¸µé ¼ö ÀÖ´Ù.\n",uiLabor,uiQuantity);
			uiLabor = uiLabor % (ItemCompose->m_siLabor);
		}
	}
	return uiQuantity;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¾ÆÀÌÅÛ °³¼ö¸¦ ³ëµ¿·®À¸·Î ¹Ù²Û´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
UI32	SOVillage::GetLaborByQuantity(UI16 uiItem, UI32	uiQuantity)
{
	UI32			uiLabor = 0;
	MakeItem		*ItemCompose	=	NULL;
	CItemHeader		*pItemHeader	=	NULL;

	pItemHeader	=	clWorld->clItem.GetItem(uiItem);
	if( pItemHeader )
	{
		ItemCompose = pItemHeader->pMakeItem;
		if(ItemCompose)
		{
			uiLabor = ( uiQuantity/ ItemCompose->m_siProduction) * (ItemCompose->m_siLabor) ;
			if( uiQuantity % ItemCompose->m_siProduction != 0)	uiLabor += ItemCompose->m_siLabor;
//			printf("³ëµ¿·Â[%u]À¸·Î ¾ÆÀÌÅÛ[%u]°³ÀÇ ¾ÆÀÌÄÜÀ» ¸¸µé ¼ö ÀÖ´Ù.\n",uiLabor,uiQuantity);
		}
	}
	return uiLabor;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	³ëµ¿·®À» ¾ÆÀÌÅÛ »ý»êÇÒ ¼ö ÀÖ´Â  °³¼ö·Î ¹Ù²Û´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::SetStorageItem( UI08 uiPartID, UI08 uiSlot, UI16 uiItemCode, SI16 uiItemQnt, UI08 uiItemIndex)
{
	stFarmData					*pFarm;
	stFarmSlot					*pFarmSlot;
	DBIKSetStorageItem			stSetStorageItem;

	if( uiItemQnt < 0 )			return FALSE;

	if( pFarmGroup )
	{
		pFarm		=	pFarmGroup->GetFarm(uiPartID);
		if( pFarm )
		{
			pFarmSlot	=	pFarm->GetFarmSlot(uiSlot);
			if( pFarmSlot )
			{
				if( pFarmSlot->pFacility && pFarmSlot->bStore )
				{					
					if(uiItemIndex	==	UI08_ERROR)		uiItemIndex	= pFarmSlot->Storage.GetStorageItemIndex(uiItemCode );
					if(uiItemIndex	==	UI08_ERROR)		return FALSE;
					if(uiItemQnt	==	0 )				uiItemCode	=	0;

					stSetStorageItem.uiVillageCode		=	uiVillageCode;
					stSetStorageItem.uiPlantID			=	pFarmGroup->pPlant->DBIndex;
					stSetStorageItem.uiPartID			=	pFarm->nIndex;
					stSetStorageItem.uiSlot				=	pFarmSlot->nSlotIndex;
					stSetStorageItem.uiItemIndex		=	uiItemIndex;
					stSetStorageItem.uiItemCode			=	uiItemCode;
					stSetStorageItem.uiItemQnt			=	uiItemQnt;
					
					//if( clWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_STORAGE_SETDATA, LPVOID(&stSetStorageItem), NULL) == FALSE )
					//	return FALSE;
					clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_STORAGE_SETDATA, &stSetStorageItem, sizeof(stSetStorageItem));

					pFarmSlot->Storage.item[uiItemIndex].uiItemCode =	uiItemCode;
					pFarmSlot->Storage.item[uiItemIndex].uiQuantity	=	uiItemQnt;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
*/
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Ã¢°í¿¡¼­ ¾ÆÀÌÅÛÀ» ²¨³½´Ù.(¹«°Ô °í·Á ¾ÈÇÔ)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::StorageOutputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.

	return	m_pIndustrialComplexs->DeleteItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	Ã¢°í¿¡ ¾ÆÀÌÅÛÀ» Áý¾î ³Ö´Â´Ù.(¹«°Ô °í·Á ¾ÈÇÔ) - Áý¾î ³ÖÀº °³¼ö¸¦ ¸®ÅÏ
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::StorageInputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{

	if( DoesFactoryExist() == FALSE )													return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.

	return	m_pIndustrialComplexs->AddItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÓ±ÝÀ» º¯°æÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-07-24 ¿ÀÀü 11:37:42 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::ChangePay( SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )														return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.

	if( m_pIndustrialComplexs->ChangeFactoryPay( pPlayer, MONEY( siPay ), &mnCapital ) == TRUE )
	{	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}


}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ê¾÷ ´ÜÁöÀÇ °øÀå¿¡ µé¾î°£´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-11 ¿ÀÈÄ 4:34:14 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::EnterIndustrialComplexFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.	
		
	return	m_pIndustrialComplexs->EnterFactory( pPlayer, siFactoryID );
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ê¾÷ ´ÜÁöÀÇ °øÀå¿¡¼­ ³ª°£´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-11 ¿ÀÈÄ 4:45:41 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::LeaveIndustrialComplexFactory( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.			

	return	m_pIndustrialComplexs->LeaveFactory( pPlayer );
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÇÃ·¹ÀÌ¾î°¡ »ê¾÷ ´ÜÁö¸¦ °¡Áö°í ÀÖ´ÂÁö ¾Ë¾Æ¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-15 ¿ÀÈÄ 1:23:58 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::IsIndustrialComplexOwner( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.				

	if( m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer ) == NULL )				return	FALSE;

	return	TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÇÃ·¹ÀÌ¾î°¡ »ê¾÷ ´ÜÁö¸¦ °¡Áö°í ÀÖ´ÂÁö ¾Ë¾Æ¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-15 ¿ÀÈÄ 1:23:58 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SOVillage_IndustrialComplex  *SOVillage::GetIndustrialComplexOwner( SOPlayer *pPlayer )
{

	if ( pPlayer == NULL) return NULL;
	if( DoesFactoryExist() == FALSE )													return	NULL;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.				

	return m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer );

	
}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÇöÀç ÀÛ¾÷ÁßÀÎ °øÀå¿¡¼­ ÀÛ¾÷ÁßÀÎ°ÍÀ» Ãë¼ÒÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-16 ¿ÀÀü 11:28:28 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																return	FALSE;		// ÀÌ ¸¶À»¿¡ °Ç¹°ÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.				

	if( m_pIndustrialComplexs->CancelWork( pPlayer, siFactoryID, &mnCapital ) == TRUE )
	{
	
	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}

	
}