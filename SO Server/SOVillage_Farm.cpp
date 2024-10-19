#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅骯 勒僭曖 蟾晦 薑爾蒂 橢橫螞棻. (憮幗 衛翕衛..)
//	熱薑 橾濠	:	2002-09-23 螃�� 6:04:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::GetFarmData()
{
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_FARM_GETINFO, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;	// 勒撲 勒僭擊 檗朝棻..	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_FARM_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )		return FALSE;	// 璽堅 等檜顫蒂 檗朝棻..(璽堅 纔檜綰)	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_STORAGE_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;
	
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	橾ж晦 幗が擊 揚毓擊陽 橾ж晦陛 陛棟и雖 爾頂遽棻.
//	熱薑 橾濠	:	2002-09-23 螃�� 6:31:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::PlantWorkAble( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )														return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.
	
	return	m_pIndustrialComplexs->BeginWork( pPlayer );					
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	饜雖蒂 偃繫и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 6:59:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::ExpandFarmSlot( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )															return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.

	if( m_pIndustrialComplexs->Reclamation( pPlayer, SI32( uiFarmID ), &mnCapital ) == FALSE )	return	FALSE;		// 饜雖蒂 偃繫и棻.



	// Ы溯檜橫, 蝸煜,葆擊 ID,奢濰 ID,儅骯衛撲熱櫛,

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢濰擊 勒撲и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 9:55:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.

	if( m_pIndustrialComplexs->ConstructFactory( pPlayer, SI32( siFactoryID ), cFactoryCode, mnPay, &mnCapital ) == FALSE )	return	FALSE;		// 饜雖蒂 偃繫и棻.


	IncreaseCapital( mnCapital, ON_PROFIT_PLANT , FALSE);	// db縑朝 憲葬雖彊朝棻

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蝸煜縑 勒僭擊 繹剪и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::DestroyPlantSlot(SOPlayer *pPlayer, UI08 uiSlotID)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.
	
	if( m_pIndustrialComplexs->DestroyFactory( pPlayer, SI32( uiSlotID ), &mnCapital ) == TRUE )
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
	
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蝸煜擊 機斜溯檜萄 и棻. (機斜溯檜萄 啗�嘛� 撮遴棻.)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage::UpgradeSlot(SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																				return	FALSE;	// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.	
	
	if( m_pIndustrialComplexs->UpgradeFactory( pPlayer, SI32( uiSlot ), MONEY( siPay ), &mnCapital ) == FALSE )		return	FALSE;

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);

	

	return	TRUE;

	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯и棻.(蟾晦 啗��)
//	熱薑 橾濠	:	2002-08-17 螃�� 4:22:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	SOVillage::ProductItem(SOPlayer *pPlayer, UI08	uiPlantID, UI08 uiFarmID, UI08 uiSlot, UI16 uiItem, UI32 Quantity, UI32 uiPay)
{	
	MONEY	mnCapital;
	DWORD	dwResoponse;

	if( DoesFactoryExist() == FALSE )																										return	FALSE;	// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.	
	
	dwResoponse	=	m_pIndustrialComplexs->ProduceItem( pPlayer, SI32( uiSlot ), uiItem, Quantity, MONEY( uiPay ) , cNation, &mnCapital );

	if( dwResoponse	== ON_RET_OK )
	{
		IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);
	}
	
	return	dwResoponse;

	
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	璽堅縑 營猿陛 氈朝雖 �挫恉挬�.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
					if( pFarmSlot->pFacility && pFarmSlot->bStore )   // 璽堅塭賊...
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 儅骯и棻.(儅骯脹 璋濠蒂 葬欐и棻.)
//	熱薑 橾濠	:	2002-09-23 螃�� 7:14:04 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	if( pPlayer == NULL)													return	0;			// 嶸�褲� Ы溯檜橫檣雖 匐餌и棻.
	if( pFarmGroup == NULL)													return	0;			// 儅骯 衛撲僭檜 氈朝雖 匐餌и棻.
	if( ( pFarm = pPlayer->stAccountInfo.pStructure ) == NULL )				return	0;			// Ы溯檜橫陛 儅骯 衛撲僭 寰縑 菟橫陛 氈朝雖 匐餌и棻.
	if( ( pFarmSlot	= pFarm->GetFarmSlot( uiSlot ) ) == NULL )				return	0;			// Ы溯檜橫陛 濛機и 勒僭檜 嶸�褲捘� 匐餌и棻.
	if( pFarmSlot->pFacility == NULL )										return	0;			// ?????
	if( pFarmSlot->bWorkable ==	FALSE )										return	0;			// п渡 勒僭檜 濛機擊 й 熱 氈朝 鼻鷓檣雖 匐餌и棻;

	// 論檣 喻翕榆 =  陴擎 喻翕 + 蹺陛脹 喻翕
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
		// 機斜溯檜萄 濛機

		if( uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor				=	pFarmSlot->uiRemainLabor;
			TempPay					=	ResultLabor * pFarmSlot->siPay;
			if( pFarmSlot->bStore )						
			{	
				// 璽堅橾 唳辦				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_STORAGE_LEVELUP, &SetFarmProduct, sizeof(SetFarmProduct) );

				pFarmSlot->Storage.Init();
			}
			else
			{	
				// 橾奩勒僭橾 唳辦				
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
				// 璽堅橾 唳辦				
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
		// 嬴檜蠱 儅骯 濛機

		uiItem		=	pFarmSlot->ProductItem;
		StgSlot		=	GetEmptyStorage(SetFarmProduct.uiPlantID,SetFarmProduct.uiFarmID,uiItem,StgLoc, StgQnt);
		if(StgSlot == UI08_ERROR)		return 0L;
		
		if(  uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor	=	pFarmSlot->uiRemainLabor;
			Quantity	=	pFarmSlot->Structure.GetRemainNum();	//	霤褻滲熱: 陴擎 uiLabor陛 釭螞棻.
			SetFarmProduct.Labor		=	0;
			TempPay		=	ResultLabor * pFarmSlot->siPay;
			
			// 橾奩勒僭橾 唳辦			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORKEND, &SetFarmProduct, sizeof(SetFarmProduct) );

			pFarmSlot->Structure.Init();
			pFarmSlot->EndProduct();
			pFarm->FarmInfo.uiRemainLabor	-=	ResultLabor;
		}
		else
		{
			ResultLabor	=	uiLabor + pFarmSlot->uiLabor;
			Quantity	=	GetQuantityByLabor(uiItem, ResultLabor);	//	霤褻滲熱: 陴擎 uiLabor陛 釭螞棻.
			SetFarmProduct.Labor		=	ResultLabor;
			// 嬴檜蠱擊 儅骯й 唳辦
			SetFarmProduct.Quantity		=	pFarmSlot->Structure.uiNum + Quantity;
			TempPay						=	uiLabor * pFarmSlot->siPay;
			
			// DB 機等檜お			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORK, &SetFarmProduct, sizeof(SetFarmProduct));

			// 憮幗 機等檜お
			pFarmSlot->uiLabor			=	ResultLabor;
			pFarmSlot->Structure.uiNum	=	SetFarmProduct.Quantity;
			pFarmSlot->uiRemainLabor		-=	uiLabor;
			pFarm->FarmInfo.uiRemainLabor	-=	uiLabor;
		}
		// 嬴檜蠱 陛問 滲翕擊 嬪п 儅骯榆擊 ITEM Table縑 盪濰и棻.
		DBIKItemNum			ItemNum;
		ItemNum.ItemID		=	uiItem;
		ItemNum.Num			=	Quantity;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_ITEMPRODUCTSET, &ItemNum, sizeof(ItemNum) );
		
		if( SetStorageItem(SetFarmProduct.uiFarmID, StgSlot, uiItem, (StgQnt+Quantity), StgLoc) == FALSE )
			return 0;
	}
	
	// 賅萇 橾ж朝 濛機橾 唳辦...
	pFarm->siWorkDay = 0;
	memcpy(uiPay, &TempPay, sizeof(MONEY));
	pPlayer->IncreaseMoney(TempPay);
	
	return Quantity;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	喻翕榆擊 嬴檜蠱 儅骯й 熱 氈朝  偃熱煎 夥羞棻. 陴擎 喻翕檜 釭螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//			printf("喻翕溘[%u]戲煎 嬴檜蠱[%u]偃曖 嬴檜夔擊 虜菟 熱 氈棻.\n",uiLabor,uiQuantity);
			uiLabor = uiLabor % (ItemCompose->m_siLabor);
		}
	}
	return uiQuantity;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱 偃熱蒂 喻翕榆戲煎 夥羞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//			printf("喻翕溘[%u]戲煎 嬴檜蠱[%u]偃曖 嬴檜夔擊 虜菟 熱 氈棻.\n",uiLabor,uiQuantity);
		}
	}
	return uiLabor;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	喻翕榆擊 嬴檜蠱 儅骯й 熱 氈朝  偃熱煎 夥羞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	璽堅縑憮 嬴檜蠱擊 疏魚棻.(鼠啪 堅溥 寰л)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::StorageOutputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.

	return	m_pIndustrialComplexs->DeleteItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	璽堅縑 嬴檜蠱擊 餵橫 厥朝棻.(鼠啪 堅溥 寰л) - 餵橫 厥擎 偃熱蒂 葬欐
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::StorageInputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{

	if( DoesFactoryExist() == FALSE )													return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.

	return	m_pIndustrialComplexs->AddItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	歜旎擊 滲唳и棻.
//	熱薑 橾濠	:	2002-07-24 螃瞪 11:37:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::ChangePay( SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )														return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.

	if( m_pIndustrialComplexs->ChangeFactoryPay( pPlayer, MONEY( siPay ), &mnCapital ) == TRUE )
	{	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}


}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑 菟橫除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:34:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::EnterIndustrialComplexFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.	
		
	return	m_pIndustrialComplexs->EnterFactory( pPlayer, siFactoryID );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	骯機 欽雖曖 奢濰縑憮 釭除棻.
//	熱薑 橾濠	:	2002-10-11 螃�� 4:45:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::LeaveIndustrialComplexFactory( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.			

	return	m_pIndustrialComplexs->LeaveFactory( pPlayer );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 骯機 欽雖蒂 陛雖堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 1:23:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::IsIndustrialComplexOwner( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.				

	if( m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer ) == NULL )				return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 骯機 欽雖蒂 陛雖堅 氈朝雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-10-15 螃�� 1:23:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_IndustrialComplex  *SOVillage::GetIndustrialComplexOwner( SOPlayer *pPlayer )
{

	if ( pPlayer == NULL) return NULL;
	if( DoesFactoryExist() == FALSE )													return	NULL;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.				

	return m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer );

	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 濛機醞檣 奢濰縑憮 濛機醞檣匙擊 鏃模и棻.
//	熱薑 橾濠	:	2002-10-16 螃瞪 11:28:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																return	FALSE;		// 檜 葆擊縑 勒僭檜 氈朝雖 匐餌и棻.				

	if( m_pIndustrialComplexs->CancelWork( pPlayer, siFactoryID, &mnCapital ) == TRUE )
	{
	
	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}

	
}