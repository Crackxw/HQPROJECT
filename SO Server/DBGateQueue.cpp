#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"
#include "DBGateQueue.h"

//----------------------------
#include "SOMain.h"
#include "SOCommonHeader.h"
#include "MSSQLDB.h"
#include "SOVillage.h"
#include "IGuild.h"
#include "SODBStructure.h"
//----------------------------




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자 
//--------------------------------------------------------------------------------------------
CDBGateQueue::CDBGateQueue(int a, int b): CDBQueueProc(a, b)
{
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소멸자 
//--------------------------------------------------------------------------------------------
CDBGateQueue::~CDBGateQueue()
{
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 처리 할수 있는 쿼리 인가를 알아 본다 
//--------------------------------------------------------------------------------------------
bool CDBGateQueue::isAbleQuery(char* data)
{
	DBQUERY_HEADER* header = (DBQUERY_HEADER*)data;
		
	if( header->command > 0 && header->command < 300 ) return TRUE;
		
	return FALSE;	
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 쿼리를 처리 한다 
//--------------------------------------------------------------------------------------------
void CDBGateQueue::setDBInfo(DWORD command, char* data)
{
	BOOL	bSuccess = TRUE;
	DWORD	result	 = 0;

	//--------------------------------
	SI32		siInfoKind = command;
	LPVOID		pIn		   = data;
	LPVOID		pOut	   = &result;	
	//--------------------------------

	switch(siInfoKind)
	{


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_AUDIT_SETNAME:
		{
			DBIKSetName					*pSetName;
			SI32						siResult = 0;

			pSetName				=	(DBIKSetName*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnMercenarySetName %d, %u, %u, '%s '",
						  	 pSetName->DBAccount, pSetName->uiSlot, pSetName->uiFollowerID, pSetName->cName ) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				m_pSQL->DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&siResult);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			if(siResult != 1)
				bSuccess	=	FALSE;
		}
		break;



	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SAVEALLPARAM:
		{
			DBIKFollowerParam	*pFollowerInfo;

			pFollowerInfo	=	(DBIKFollowerParam*)pIn;

			
			if( pFollowerInfo->Mana >= 10000 )
			{
				writeInfoToFile("SO_DB_INFOKIND_SAVEALLPARAM.txt", "[ %u, %d, %d ] :: [ %d ]", 
												pFollowerInfo->DBAccount, pFollowerInfo->uiSlot, 
												pFollowerInfo->uiFollowerSlot, pFollowerInfo->Mana );
/*
				FILE	*fp;
				if( ( fp = fopen( "..\\SO_DB_INFOKIND_SAVEALLPARAM.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "[ %u, %d, %d ] :: [ %d ] \n", pFollowerInfo->DBAccount, pFollowerInfo->uiSlot, 
																pFollowerInfo->uiFollowerSlot, pFollowerInfo->Mana );
					fclose( fp );
				}
*/
			}

			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetMercenaryParam %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
								pFollowerInfo->DBAccount,
								pFollowerInfo->uiSlot,
								pFollowerInfo->uiFollowerSlot,
								pFollowerInfo->Kind,
								pFollowerInfo->Exp,
								pFollowerInfo->Level,
								pFollowerInfo->Str,
								pFollowerInfo->Dex,
								pFollowerInfo->Vit,
								pFollowerInfo->Int,
								pFollowerInfo->Life,
								pFollowerInfo->Mana,
								pFollowerInfo->AC,
								pFollowerInfo->Bonus,
								pFollowerInfo->DmResist,
								pFollowerInfo->MgResist,
								pFollowerInfo->EquipMin,
								pFollowerInfo->EquipMax) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SAVEMONEY:
		{
			DBIKSaveMoney			*pSaveMoney;

			pSaveMoney		=	(DBIKSaveMoney*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnSavePlayerMoney %d, %d, %I64d", pSaveMoney->DBAccount, pSaveMoney->uiSlot, pSaveMoney->Money) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SAVECAPITAL:
		{
			DBIKVillageProfit				*pVillageProfit;
			pVillageProfit				=	(DBIKVillageProfit*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageInterestSet %d, %d, %I64d, %u",
				pVillageProfit->VillageCode, pVillageProfit->Year, pVillageProfit->Profit, pVillageProfit->Option) == false)
			{
				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	// 연간 전장 수익을 계산하여 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BANKCAPITAL_CALCULATE:
		{
			SOVillage  *pVillage  = (SOVillage*)pIn;
			
			DBNUMERIC				TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageTradeCapitalCalculate %d", pVillage->uiVillageCode) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}
			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				m_pSQL->DBBind (1, SRCNUMERICBIND,	  0,	(BYTE *)&TempNumeric);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
				
				memcpy(&pVillage->CurrentCapital->BankProfit, TempNumeric.val,sizeof(MONEY));
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_APPENDITEM:
		{
			DBIKPlayerItemInfo	*pItemInfo;

			pItemInfo	=	(DBIKPlayerItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnAppendMercenaryItem %d, %d, %d, %d, %d, %d", 							
							pItemInfo->siDBAccount,
							pItemInfo->uiSlot,
							pItemInfo->uiFollowerID,
							pItemInfo->uiItemID,							
							pItemInfo->uiItemPosInInventory,
							pItemInfo->uiNewQuantity) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DELETEITEM:
		{
			DBIKPlayerItemInfo	*pItemInfo;

			pItemInfo	=	(DBIKPlayerItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnDeleteMercenaryItem %d, %d, %d, %d, %d, %d",
							pItemInfo->siDBAccount,
							pItemInfo->uiSlot,
							pItemInfo->uiFollowerID,
							pItemInfo->uiItemID, 
							pItemInfo->uiItemPosInInventory,
							pItemInfo->uiNewQuantity) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;	

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_UPDATEITEM:
		{
			DBIKPlayerItemInfo	*pItemInfo;

			pItemInfo	=	(DBIKPlayerItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnUpdateMercenaryItem %d, %d, %d, %d, %d, %d, %d", 
							pItemInfo->siDBAccount,
							pItemInfo->uiSlot,
							pItemInfo->uiFollowerID,
							pItemInfo->uiItemID,
							pItemInfo->uiItemPosInInventory,
							pItemInfo->uiNewQuantity,
							pItemInfo->uiNewPosInInventory) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_UPDATEVILLAGEITEM:
		{
			DBIKVillageItemInfo	*pItemInfo;

			pItemInfo	=	(DBIKVillageItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnUpdateVillageItemQuantity %d, %d, %u", 							
							pItemInfo->uiVillageCode,
							pItemInfo->siID,
							pItemInfo->uiQuantity) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SAVEPLAYERPOS:
		{
			DBIKPlayerPos				*pPlayerPos;
			pPlayerPos				=	(DBIKPlayerPos*)	pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSaveCurrentPosition %d, %d, %d, %d, %d", 
							pPlayerPos->DBAccount, pPlayerPos->uiSlot, pPlayerPos->siX, pPlayerPos->siY, pPlayerPos->MapID) == false)
			{				
				bSuccess = FALSE;
				break;
			}
			
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_APPENDFIELDITEM:
		{
			DBIKFieldItemInfo	*pFieldItemInfo;

			pFieldItemInfo		=	(DBIKFieldItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnAddItemOnMap %d, %d, %d, %d, %d", 
							pFieldItemInfo->uiMapID, pFieldItemInfo->siX, pFieldItemInfo->siY, pFieldItemInfo->uiID, pFieldItemInfo->uiQuantity) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_ITEM_APPEND:
		{
			DBIKItem		*pItemData;

			pItemData		=	(DBIKItem *)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnItemAppend %d, '%s', %d ,%d",pItemData->ItemID, pItemData->Name, pItemData->Price,pItemData->siUseDay) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DELETEFIELDITEM:
		{
			DBIKFieldItemInfo	*pFieldItemInfo;

			pFieldItemInfo		=	(DBIKFieldItemInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On3RemoveItemOnMap %d, %d, %d, %d, %d", 
							pFieldItemInfo->siAccount, pFieldItemInfo->uiSlot, pFieldItemInfo->uiMapID, pFieldItemInfo->siX, pFieldItemInfo->siY) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_ITEMPRODUCTSET:
		{
			DBIKItemNum			*pItemNum;
			pItemNum			=	(DBIKItemNum*)	pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_OnItemProductSet %d, %d", pItemNum->ItemID, pItemNum->Num) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_PLANT_CANCEL_PLAN:
		{
			DBIKFarmCancelPlan		*pCancelPlan =	(DBIKFarmCancelPlan*)	pIn;
			

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2CancelWorkOfEquipment %u, %u, %u, %d, %u, %I64d",
				pCancelPlan->uiVillageCode, pCancelPlan->uiFarmID,pCancelPlan->uiFactoryID, 
				pCancelPlan->DBAccount, pCancelPlan->uiSlot, pCancelPlan->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}

			
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_PLANT_CANCEL_STORAGE:
		{
			DBIKFarmCancelPlan		*pCancelPlan =	(DBIKFarmCancelPlan*)	pIn;
			

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2CancelWorkOfStorage %u, %u, %u, %d, %u, %I64d",
				pCancelPlan->uiVillageCode, pCancelPlan->uiFarmID,pCancelPlan->uiFactoryID, 
				pCancelPlan->DBAccount, pCancelPlan->uiSlot, pCancelPlan->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}


			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
			
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}

			
		}
		break;


	

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_WORKEND:
		{
			DBIKFarmWorkEndInfo		    *pWorkInfo = (DBIKFarmWorkEndInfo	*)pIn;
			

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2CompleteProductOfItem %u, %u, %u, %u ,%u ,%u ,%u ,%d , %u, %I64d",
							 pWorkInfo->uiVillageCode, pWorkInfo->uiFarmID,pWorkInfo->uiFactoryID,
							 pWorkInfo->siStorageID, pWorkInfo->siStoragePos, pWorkInfo->uiItemCode, pWorkInfo->uiItemQnt,
							 pWorkInfo->DBAccount,pWorkInfo->uiSlot,pWorkInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_WORK:
		{			
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2WorkByStorage %u, %u, %u, %u, %d,%u, %I64d",
						pFarmInfo->uiVillageCode,pFarmInfo->uiFarmID,pFarmInfo->uiFactoryID,pFarmInfo->Labor,
						pFarmInfo->DBAccount,pFarmInfo->uiSlot,pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{							
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_WORK:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			

			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2WorkByEquipment %u, %u, %u, %u, %d,%u ,%I64d",
						pFarmInfo->uiVillageCode,pFarmInfo->uiFarmID,pFarmInfo->uiFactoryID,pFarmInfo->Labor,
						pFarmInfo->DBAccount,pFarmInfo->uiSlot,pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}


			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_LEVELUP:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2CompleteBuildOfStorage %u, %u, %u, %u, %u, %I64d",
						pFarmInfo->uiVillageCode, pFarmInfo->uiFarmID, pFarmInfo->uiFactoryID,
						pFarmInfo->DBAccount, pFarmInfo->uiSlot, pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_LEVELUP:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;


			if(m_pSQL->SQLRun("EXEC SPHQ_On2CompleteRiseOfEquipment %u, %u, %u, %u, %u, %I64d",
						pFarmInfo->uiVillageCode, pFarmInfo->uiFarmID, pFarmInfo->uiFactoryID,
						pFarmInfo->DBAccount, pFarmInfo->uiSlot, pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}
			
			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
				
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_UPGRADESLOT:
		{
			DBIKSetUpgradeSlot		*pSetUpgradeSlot;


			pSetUpgradeSlot			=	(DBIKSetUpgradeSlot*)	pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2RiseEquipmentByProducingCenter %u, %u, %u, %I64d, %u, %I64d, %I64d ,%u,%u",
						pSetUpgradeSlot->uiVillageCode,	pSetUpgradeSlot->uiFarmID,
						pSetUpgradeSlot->uiFactoryID,pSetUpgradeSlot->moPay,
						pSetUpgradeSlot->Labor,pSetUpgradeSlot->moTax,
						pSetUpgradeSlot->moCapital,pSetUpgradeSlot->siDBAccount,
						pSetUpgradeSlot->uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_PRODUCTITEM:
		{
			DBIKSetFarmProduct		*pFarmProduct;
			SI32					siResult =0;

			pFarmProduct			=	(DBIKSetFarmProduct*)	pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2ProduceItemByEquipment %u, %u, %u, %u, %u, %I64d,%u,%I64d, %I64d ,%d,%u",
						pFarmProduct->uiVillageCode, pFarmProduct->uiFarmID,  pFarmProduct->uiFactoryID,
						pFarmProduct->uiItemID, pFarmProduct->Quantity, pFarmProduct->moPay,pFarmProduct->Labor,
						pFarmProduct->moTax, pFarmProduct->moProfit, pFarmProduct->siDBAccount,pFarmProduct->uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_SETEQUIPMENT:
		{
			DBIKFarmEquipData				*pFarmEquipData;

			pFarmEquipData					=	(DBIKFarmEquipData*)	pIn;			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetFarmEquipment %u, %u, %u, %u, %u, %u",
						pFarmEquipData->uiVillageCode, pFarmEquipData->uiPlantID, pFarmEquipData->uiFarmID,
						pFarmEquipData->uiSlotID, pFarmEquipData->uiEquipID, pFarmEquipData->uiLevel ) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_DESTROY_STORAGE:
		{
			DBIKDestroyStorage  *DS = (DBIKDestroyStorage  *)pIn;

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2DestroyOfStorage %u, %u, %u", DS->uiVillageCode,
						DS->uiFarmID, DS->uiFactoryID ) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
			
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;
	case SO_DB_INFOKIND_FARM_DESTROY_FACTORY:
		{
			DBIKDestroyFactory					*DF = (DBIKDestroyFactory	*)pIn;


			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2DestroyOfEquipment %u, %u, %u", DF->uiVillageCode,
						DF->uiFarmID, DF->uiFactoryID ) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_EXPANDSLOT:
		{
			DBIKReclamation						*pFarmData;
			pFarmData			=				(DBIKReclamation*)	pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2ReclaimByProducingCenter %u, %u, %I64d, %I64d, %d, %u",
									pFarmData->uiVillageCode, pFarmData->uiFarmID,
									pFarmData->moPay,pFarmData->moCapital,
									pFarmData->DBAccount, pFarmData->uiSlot) == false)
			{							
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_SETDATA:
		{
			DBIKSetStorageItem					*pSetStorageItem;

			pSetStorageItem					=	(DBIKSetStorageItem*)	pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetStorageData %u, %u, %u, %u, %u, %u, %u", pSetStorageItem->uiVillageCode,
					pSetStorageItem->uiPlantID, pSetStorageItem->uiPartID,pSetStorageItem->uiSlot,
				pSetStorageItem->uiItemIndex, pSetStorageItem->uiItemCode,pSetStorageItem->uiItemQnt) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;
	//-------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_DELETEITEM:
		{
			DBIKFarmSetItem					*pSetStorageItem;

			pSetStorageItem					=	(DBIKFarmSetItem*)	pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_On2DelItmeByStroage %u, %u, %u, %u, %u, %u", 
				pSetStorageItem->uiVillageCode,
				pSetStorageItem->uiFarmID, pSetStorageItem->uiFactoryID,
				pSetStorageItem->uiItemIndex, pSetStorageItem->uiItemCode,pSetStorageItem->uiItemQnt) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_BUILD:
		{
			DBIKFarmBuildStructInfo				*pBuildStructInfo;



			pBuildStructInfo = (DBIKFarmBuildStructInfo*) pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2BuildStorageByProducingCenter %u, %u, %u, %I64d, %u, %I64d, %I64d, %d ,%u",
							pBuildStructInfo->uiVillageCode, pBuildStructInfo->uiFarmID,
							pBuildStructInfo->uiFactoryID, pBuildStructInfo->moPay,pBuildStructInfo->uiWork,
							pBuildStructInfo->moTax,pBuildStructInfo->moProfit,
							pBuildStructInfo->siAccount, pBuildStructInfo->uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_BUILDSTRUCT:
		{
			DBIKFarmBuildStructInfo				*pBuildStructInfo;

			pBuildStructInfo = (DBIKFarmBuildStructInfo*) pIn;

			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2BuildEquipmentByProducingCenter %u, %u, %u, '%s',%I64d, %u, %I64d, %I64d, %d ,%u",
							pBuildStructInfo->uiVillageCode, pBuildStructInfo->uiFarmID,
							pBuildStructInfo->uiFactoryID, pBuildStructInfo->Code,pBuildStructInfo->moPay,
							pBuildStructInfo->uiWork,
							pBuildStructInfo->moTax,pBuildStructInfo->moProfit,
							pBuildStructInfo->siAccount, pBuildStructInfo->uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}

		

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;
	
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_SETREMAINDAY:
		{
			DBIKBuyFarmByForceInfo		*pUpdateWorkDay;


			pUpdateWorkDay			=	(DBIKBuyFarmByForceInfo*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnFarmSetRemainDay %u, %u, %d",
				pUpdateWorkDay->uiVillageCode, pUpdateWorkDay->uiFarmID, pUpdateWorkDay->siWorkDay) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_SELL_BYFORCE:
		{
			DBIKBuyFarmByForceInfo		*pBuyFarmInfo;


			pBuyFarmInfo			=	(DBIKBuyFarmByForceInfo*)	pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_On2SellProducingCenterByForce %u, %u, %d, %u,%I64d",
							 pBuyFarmInfo->uiVillageCode,		
							 pBuyFarmInfo->uiFarmID,	
							 pBuyFarmInfo->siAccount,	
							 pBuyFarmInfo->uiSlot,		
							 pBuyFarmInfo->mnPrice) 
							 == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
			
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_BUY:
		{
			DBIKBuyFarmInfo				*pBuyFarmInfo;
			

			pBuyFarmInfo			=	(DBIKBuyFarmInfo*)	pIn;
			

			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2BuyProducingCenterByVillage %u, %u, %d, %u, %I64d",
							pBuyFarmInfo->uiVillageCode, pBuyFarmInfo->uiFarmID, 
							pBuyFarmInfo->siAccount, pBuyFarmInfo->uiSlot, pBuyFarmInfo->moMoney) == false)							
			{				
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SETVILLAGEITEM:
		{
			SI32					siResult;
			DBIKVillageItem			*pVillageItem;
			pVillageItem		=	(DBIKVillageItem*)	pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageItemSetData %d, %d, %d, %u",
				pVillageItem->VillageCode, pVillageItem->Slot, pVillageItem->ItemID, pVillageItem->uiItemQuantity) == false)
			{
				bSuccess	=	FALSE;
				break;
			}
			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				m_pSQL->DBBind (1, INTBIND,	  0,	(BYTE *)&siResult);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			if(siResult == 0 )
				bSuccess	=	FALSE;
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_CHANGE_WEAR_INFO:
		{
			DBIKWearItem				*pWearItem;
			
			pWearItem				=	(DBIKWearItem*)	pIn;
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetWearItem %d, %u, %u, %u, %u, %u, %u, %d, %d, %d",
				pWearItem->siDBAccount, pWearItem->uiSlot, pWearItem->uiFollowerID, pWearItem->uiWearLoc, 
				pWearItem->uiItemID, pWearItem->uiQuantity, pWearItem->siItemPosInInventory, pWearItem->siLimitTime, pWearItem->siStartTime,pWearItem->siEndTime) == false)
//			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetWearItem %d, %u, %u, %u, %u, %u, %u",
//				pWearItem->siDBAccount, pWearItem->uiSlot, pWearItem->uiFollowerID, pWearItem->uiWearLoc, 
//				pWearItem->uiItemID, pWearItem->uiQuantity, pWearItem->siItemPosInInventory) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;	

	
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_TAKECOLLATERAL:
		{			
			DBIKVillageCollateralInfo				*pVillageCollateralInfo;
			SI32									*siRet = 0;   //성공0 실패0

			pVillageCollateralInfo				=	(DBIKVillageCollateralInfo*)pIn;
			siRet								=	(SI32*)pOut;

			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnDeletePawn %d, %u, %u",pVillageCollateralInfo->siAccount,pVillageCollateralInfo->uiSlot,pVillageCollateralInfo->uiVillageCode ) == false)
			{
				bSuccess = FALSE;
				break;
			}
			
			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				m_pSQL->DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)siRet);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			// 담보 정보를 지운다.
		}
		break;

	//----------------------------------------------------------------------------------------
/*	case SO_DB_INFOKIND_SETBATTLECREDIT:
		{
			
			DBIKPlayerData				*pPlayerData;
			pPlayerData			=		(DBIKPlayerData*)pIn;

			// 신용도를 얻어온다.
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetCreditCombat %d, %d, %d", pPlayerData->DBAccount, pPlayerData->uiSlot, pPlayerData->Data) == false)
			{
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;	*/

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SETTRADECREDIT:
		{
			// 전투 Credit를 얻어온다.
			DBIKPlayerCredit				*pPlayerCredit;
			pPlayerCredit			=		(DBIKPlayerCredit*)	pIn;

			// 신용도를 얻어온다.
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetCredit %d, %d, %d, %d", pPlayerCredit->DBAccount, 
				pPlayerCredit->uiSlot, pPlayerCredit->Credit, pPlayerCredit->DayCredit) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;		

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGE_SETNATION:
		{	
			DBIKVillageNation		*pVillageNation;
			SI32					siResult = 0;
			pVillageNation			=	(DBIKVillageNation*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageSetNation %d '%c'", pVillageNation->VillageCode, pVillageNation->Code) == false)
			{
				bSuccess = FALSE;
				break;
			}			
			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				m_pSQL->DBBind (1, CHARBIND,	(DBCHAR) 0,	(BYTE *)&siResult);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			if(siResult != 1)
				bSuccess	=	FALSE;
		}
		break;
	case SO_DB_INFOKIND_VILLAGE_INFO:
		{
			DBIKVillageInfo*		pVillageInfo;
			
			pVillageInfo		=	(DBIKVillageInfo*)pIn;

			SI32 siVillageid = (pVillageInfo->siX<<16) + pVillageInfo->siY;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageAppend '%s', %d, %d, %d, %d, '%c', %d",
				               pVillageInfo->szName, pVillageInfo->siX, pVillageInfo->siY, 
							   pVillageInfo->siPopulation, pVillageInfo->uiViillageCode, 
							   pVillageInfo->szNation, siVillageid)  == false)
			{
				bSuccess = FALSE;
				break;
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;
	case SO_DB_INFOKIND_VILLAGE_ITEM:
		{
			DBIKVillageItem*	pDBIKVillageItemInfo;

			pDBIKVillageItemInfo	=	(DBIKVillageItem*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageItemInsert %d, %d, %d, %d",pDBIKVillageItemInfo->VillageCode,pDBIKVillageItemInfo->Slot,
				pDBIKVillageItemInfo->ItemID,pDBIKVillageItemInfo->uiItemQuantity) == false)
			{
				bSuccess = FALSE;
				break;
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;
	case SO_DB_INFOKIND_CHARACTER_LIST:
		{
			DBIKCharacterList* pDBIKCharacterList;

			pDBIKCharacterList = (DBIKCharacterList*)pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnCharacterListAppend %d, '%s', %d, %d, %d,%d, %d, %d, %d, %d,%d, %d, %d, %d, %d,%d, %d, %d, %d",
				pDBIKCharacterList->uiID,
				pDBIKCharacterList->szChar,pDBIKCharacterList->siLv,pDBIKCharacterList->uiExp,pDBIKCharacterList->siNation,
				pDBIKCharacterList->siStr,pDBIKCharacterList->siDex,pDBIKCharacterList->siVit,pDBIKCharacterList->siInt,
				pDBIKCharacterList->siAc,pDBIKCharacterList->siDamageResist,pDBIKCharacterList->siMagicResist,pDBIKCharacterList->siMinEquip,
				pDBIKCharacterList->siMaxEquip,pDBIKCharacterList->siMoveSpeed,pDBIKCharacterList->siLife,
				pDBIKCharacterList->siMana,pDBIKCharacterList->siCost,pDBIKCharacterList->siBonus) == false)
			{
				bSuccess = FALSE;
				break;
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );

		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_WRITE_DESCRIPT:
		{
			SI32					siResult = 0;
			DBIKVillageDescript		*pVillageDescript;
			CHAR					*Descript;
			pVillageDescript		=	(DBIKVillageDescript*)	pIn;
			Descript				=	(CHAR*)pIn	+ sizeof(DBIKVillageDescript);

			if(m_pSQL->SQLRun("EXEC SPHQ_OnWriteDescript %d, '%s '", pVillageDescript->VillageCode, Descript) == false)
			{
				bSuccess = FALSE;
				break;
			}
			if(m_pSQL->DBResults() == SUCCEED)
			{				
				m_pSQL->DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&siResult);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			if(siResult != 1)
				bSuccess	=	FALSE;
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_DELETE:
		{
			SI32	uiGuildID;
			SI32	*psiSuccess;

			uiGuildID	=	*((SI32*)pIn);
			psiSuccess	=	(SI32*)pOut;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnGuildDelete %d", uiGuildID) == false)
			{
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{
				m_pSQL->DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)psiSuccess);
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	case SO_DB_INFOKIND_WAR_PROFIT:
		{
			DBIKGuildReward*	pDBIKGuildReward;
			
			pDBIKGuildReward	=	(DBIKGuildReward*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2WarProfit %d, %d, %d, %64d, %d",
				            pDBIKGuildReward->uiGuildID,
				            pDBIKGuildReward->siAccount,
							pDBIKGuildReward->uiSlot,
							pDBIKGuildReward->mnMoney,
				            pDBIKGuildReward->siIncreaseSupply);
			
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_JOIN:
		{
			DBIKGuildJoinInfo	*pGuildJoinInfo;
			

			pGuildJoinInfo	=	(DBIKGuildJoinInfo*)pIn;			
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2JoinMemberByGuild %d, %d, %d",	pGuildJoinInfo->uiGuildID, 
																			pGuildJoinInfo->siAccount, 
																			pGuildJoinInfo->uiSlot) == false)
			{
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_LEAVE:
		{
			DBIKGuildLeaveInfo	*pGuildLeaveInfo;
			

			pGuildLeaveInfo		=	(DBIKGuildLeaveInfo*)pIn;			
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2LeaveMemberByGuild %d, %d, %d, %d",	pGuildLeaveInfo->uiGuildID, 
																		pGuildLeaveInfo->siAccount,
																		pGuildLeaveInfo->uiSlot,
																		pGuildLeaveInfo->uiClass) == false)
			{
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_KICK:
		{
			DBIKGuildLeaveInfo	*pGuildLeaveInfo;
			

			pGuildLeaveInfo		=	(DBIKGuildLeaveInfo*)pIn;			
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2KickMemberByGuild %d, %d, %d, %d",	pGuildLeaveInfo->uiGuildID, 
																		pGuildLeaveInfo->siAccount,
																		pGuildLeaveInfo->uiSlot,
																		pGuildLeaveInfo->uiClass) == false)
			{
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_CHANGE_CLASS:
		{
			DBIKGuildChangeClassInfo	*pGuildChangeClassInfo;
			

			pGuildChangeClassInfo	=	(DBIKGuildChangeClassInfo*)pIn;
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2ChangeClassOfGuild %d, %d, %d, %d",	pGuildChangeClassInfo->uiGuildID, 
																			pGuildChangeClassInfo->siDBAccount,
																			pGuildChangeClassInfo->uiSlot,
																			pGuildChangeClassInfo->uiChangeClass) == false)
			{
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
		}
		break;	

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEGUILDID_SET:
		{
			DBIKGuildOwnerVillageInfo	*pGuildOwnerVillageInfo;

			pGuildOwnerVillageInfo	=	(DBIKGuildOwnerVillageInfo*)pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageOwnerSet %d, %d",	pGuildOwnerVillageInfo->uiVillageCode, pGuildOwnerVillageInfo->uiGuildID) == false)
			{
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEOWNER_DELETE:
		{
			DBIKGuildOwnerVillageInfo	*pGuildOwnerVillageInfo;
			pGuildOwnerVillageInfo	=	(DBIKGuildOwnerVillageInfo*)pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageOwnerDelete %d, %d",	pGuildOwnerVillageInfo->uiVillageCode, pGuildOwnerVillageInfo->uiGuildID) == false)
			{
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );

		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_SETRELATION:		
		{
			DBIKGuildRelation	*pGuildRelation;

			pGuildRelation	=	(DBIKGuildRelation*)pIn;
			
			switch(pGuildRelation->siRelation)
			{
			case ON_GUILD_RELATION_ENEMY:
				if(m_pSQL->SQLRun("EXEC SPHQ_OnGuildSetHostile %d, %d, %d", pGuildRelation->uiGuildID1, pGuildRelation->uiGuildID2,pGuildRelation->siTime) == false)
				{
					bSuccess = FALSE;
					break;
				}
				break;
			case ON_GUILD_RELATION_NEUTRAL:
				if(m_pSQL->SQLRun("EXEC SPHQ_OnGuildUnSetHostile %d, %d", pGuildRelation->uiGuildID1, pGuildRelation->uiGuildID2) == false)
				{					
					bSuccess = FALSE;
					break;
				}
				break;
			}		

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;	
	case SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_OFFER:
		{
			DBIKGuildFrieldlyMessageOffer*	pDBIKGuildFrieldlyMessageOffer;

			pDBIKGuildFrieldlyMessageOffer	=	(DBIKGuildFrieldlyMessageOffer*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2SetHostileMessage %d, %d, '%s'", pDBIKGuildFrieldlyMessageOffer->uiGuildID1,pDBIKGuildFrieldlyMessageOffer->uiGuildID2,pDBIKGuildFrieldlyMessageOffer->szMessage ) == false)
			{
				bSuccess = FALSE;
				break;		
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;
	case SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_ACCEPT:
		{
			DBIKGuildFriendlyMessageAccept*	pDBIKGuildFriendlyMessageAccept;

			pDBIKGuildFriendlyMessageAccept =	(DBIKGuildFriendlyMessageAccept*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnGuildUnSetHostileByMsg %d, %d", pDBIKGuildFriendlyMessageAccept->uiGuildID1,pDBIKGuildFriendlyMessageAccept->uiGuildID2 ) == false)
			{
				bSuccess = FALSE;
				break;		
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;
	case SO_DB_INFOKIND_GUILD_FRIENDLY_MESSAGE_CANCEL:
		{
			DBIKGuildFriendlyMessageCancel*	pDBIKGuildFriendlyMessageCancel;

			pDBIKGuildFriendlyMessageCancel =	(DBIKGuildFriendlyMessageCancel*)pIn; 

			if(m_pSQL->SQLRun("EXEC SPHQ_On2DeleteHostileMessage %d, %d", pDBIKGuildFriendlyMessageCancel->uiGuildID1,pDBIKGuildFriendlyMessageCancel->uiGuildID2) == false)
			{
				bSuccess = FALSE;
				break;		
			}
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEHNSNAME_SET:
		{
			DBIKVillageMaster	*pVillageMaster;
			pVillageMaster		=	(DBIKVillageMaster*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetTradeMaster %d, '%s'", pVillageMaster->VillageCode, pVillageMaster->Name) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEDEFENCE_SET:
		{
			DBIKVillageDurability		*pVillageDurability;
			pVillageDurability		=	(DBIKVillageDurability*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnVillageSetDurability %d, %I64d", pVillageDurability->VillageCode, pVillageDurability->Durability)  == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	case SO_DB_INFOKIND_DECREASE_SOLDIER:
		{
			DBIKDecreaseDefenceSoldier		*pDecreaseDefenceSoldier;
			pDecreaseDefenceSoldier		=	(DBIKDecreaseDefenceSoldier*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2DecreaseGuard %d, %d", pDecreaseDefenceSoldier->VillageCode,pDecreaseDefenceSoldier->siDefenceSoldier)  == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_FLAG:
		{
			DBIKGuildFlag		*pGuildFlag;
			pGuildFlag		=	(DBIKGuildFlag*)	pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeFlagOfGuild %d, %d, %d, %d, %I64d", pGuildFlag->GuildID, pGuildFlag->Flag , pGuildFlag->siAccount, pGuildFlag->uiSlot, pGuildFlag->moMoney)  == false)
			{
				bSuccess = FALSE;
				break;		
			}
			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_NEARESTHVCODE:
		{
			DBIKNearestHVCode	*pNearestHVCode;			

			pNearestHVCode	=	(DBIKNearestHVCode*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnCharactorNearVillageSet %d, %d, %d", pNearestHVCode->DBAccount, pNearestHVCode->uiSlot, pNearestHVCode->uiVillageCode)  == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GIVEFOLLOWER:
		{
			DBIKGiveFollower	*pGiveFollower;			

			pGiveFollower	=	(DBIKGiveFollower*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnExchangeMercenary %d, %d, %d, %d, %d, %d", 
								pGiveFollower->siFromDBAccount, pGiveFollower->uiFromSlot, pGiveFollower->uiFromFollowerID, 
								pGiveFollower->siToDBAccount, pGiveFollower->uiToSlot, pGiveFollower->uiToFollowerID) == false)
			{
				bSuccess = FALSE;
				break;		
			}
				
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BUYTICKET:
		{
			DBIKBuyTicket*		lpDBIKBuyTicket;

			lpDBIKBuyTicket = (DBIKBuyTicket*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_On2BuyTicket %d, %d, %d, %d, %I64d, %I64d"
				, lpDBIKBuyTicket->uiVillageCode
				, lpDBIKBuyTicket->DBAccount, lpDBIKBuyTicket->uiSlot
				, lpDBIKBuyTicket->siTicketKind, lpDBIKBuyTicket->siMoney
				, lpDBIKBuyTicket->siVillageProfit);
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVESTORSETINFO:
		{
			DBIKInvestorSetInfo*		lpDBIKInvestorSetInfo;

			lpDBIKInvestorSetInfo = (DBIKInvestorSetInfo*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_OnInvestorSetInfo %d, '%s ', %d, %d, '%s ', %d, %d", lpDBIKInvestorSetInfo->uiVillageCode
				, lpDBIKInvestorSetInfo->szRealBestInvestorID
				, lpDBIKInvestorSetInfo->uiRealBestInvestorType
				, lpDBIKInvestorSetInfo->uiRealBestInvestorUpdateTime
				, lpDBIKInvestorSetInfo->szNowBestInvestorID
				, lpDBIKInvestorSetInfo->uiNowBestInvestorType
				, lpDBIKInvestorSetInfo->uiNowBestInvestorUpdateTime);
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVEST_SETINVESTMENT:
		{
			DBIKInvestSetInvestment*		lpDBIKInvestSetInvestment;

			lpDBIKInvestSetInvestment = (DBIKInvestSetInvestment*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_On2InvestOfVillage %d, %d, %d, '%s ', %d, %I64d, %I64d, %d"
				, lpDBIKInvestSetInvestment->uiVillageCode
				, lpDBIKInvestSetInvestment->siInvestorDBAccount
				, lpDBIKInvestSetInvestment->uiInvestorSlot
				, lpDBIKInvestSetInvestment->szInvestorID
				, lpDBIKInvestSetInvestment->uiInvestorType
				, lpDBIKInvestSetInvestment->moInvestmentMoney
				, lpDBIKInvestSetInvestment->moInvestmentSaveMoney
				, lpDBIKInvestSetInvestment->uiInvestorUpdateTime);
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVEST_DELINVESTMENT:
		{
			DBIKInvestDelInvestment*		lpDBIKInvestDelInvestment;

			lpDBIKInvestDelInvestment = (DBIKInvestDelInvestment*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_OnDeleteInvestment %d, %d, %d", lpDBIKInvestDelInvestment->uiVillageCode, lpDBIKInvestDelInvestment->siInvestorDBAccount, lpDBIKInvestDelInvestment->uiInvestorSlot);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVEST_DELAll10PINVESTMENT:
		{
			DBIKInvestDelAll10PInvestment*		lpDBIKInvestDelAll10PInvestment;

			lpDBIKInvestDelAll10PInvestment = (DBIKInvestDelAll10PInvestment*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_OnSetInvestmentMoneyAfterFall %d,%d,%d,%d", 
				            lpDBIKInvestDelAll10PInvestment->uiVillageCode,
							lpDBIKInvestDelAll10PInvestment->siInvestorDBAccount,
							lpDBIKInvestDelAll10PInvestment->uiInvestorSlot,
							lpDBIKInvestDelAll10PInvestment->siPercent);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGESETCAPITAL:
		{
			DBIKVillageSetCapital*		lpDBIKVillageSetCapital;

			lpDBIKVillageSetCapital = (DBIKVillageSetCapital*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_OnVillageSetCapital %d, %I64d", lpDBIKVillageSetCapital->uiVillageCode, lpDBIKVillageSetCapital->moVillageCapital);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_SETMERCENARYPARAM:
		{
			DBIKSetMercenaryParam*		lpDBIKSetMercenaryParam;

			lpDBIKSetMercenaryParam = (DBIKSetMercenaryParam*)pIn;


			if( lpDBIKSetMercenaryParam->siMana >= 10000 )
			{
				writeInfoToFile("SO_DB_INFOKIND_SETMERCENARYPARAM.txt", "[ %u, %d, %d ] :: [ %d ] \n", 
								lpDBIKSetMercenaryParam->DBAccount, lpDBIKSetMercenaryParam->uiSlot, 
								lpDBIKSetMercenaryParam->siMercenarySlot, lpDBIKSetMercenaryParam->siMana );
/*
				FILE	*fp;
				if( ( fp = fopen( "..\\SO_DB_INFOKIND_SETMERCENARYPARAM.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "[ %u, %d, %d ] :: [ %d ] \n", lpDBIKSetMercenaryParam->DBAccount, lpDBIKSetMercenaryParam->uiSlot, 
																lpDBIKSetMercenaryParam->siMercenarySlot, lpDBIKSetMercenaryParam->siMana );
					fclose( fp );
				}
*/
			}
			

			m_pSQL->SQLRun("EXEC SPHQ_OnSetMercenaryParam %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
				lpDBIKSetMercenaryParam->DBAccount, lpDBIKSetMercenaryParam->uiSlot, 
				lpDBIKSetMercenaryParam->siMercenarySlot, 
				lpDBIKSetMercenaryParam->siKind,
				lpDBIKSetMercenaryParam->siCurExp,
				lpDBIKSetMercenaryParam->siLevel, 
				lpDBIKSetMercenaryParam->siSTR,
				lpDBIKSetMercenaryParam->siDEX,
				lpDBIKSetMercenaryParam->siVIT,
				lpDBIKSetMercenaryParam->siINT,
				lpDBIKSetMercenaryParam->siLife, 
				lpDBIKSetMercenaryParam->siMana, 
				lpDBIKSetMercenaryParam->siAC,
				lpDBIKSetMercenaryParam->siBonus, 
				lpDBIKSetMercenaryParam->siDamageResist,
				lpDBIKSetMercenaryParam->siMagicResist,
				lpDBIKSetMercenaryParam->siEquipMinDamage,
				lpDBIKSetMercenaryParam->siEquipMaxDamage);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_RESETTICKET:
		{
			DBIKResetTicket*		lpDBIKResetTicket;

			lpDBIKResetTicket = (DBIKResetTicket*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_On2ResetTicket %d, %d", lpDBIKResetTicket->DBAccount, lpDBIKResetTicket->uiSlot);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DELETEMERCENARY:
		{
			DBIKDeleteMercenary*		lpDBIKDeleteMercenary;


			lpDBIKDeleteMercenary = (DBIKDeleteMercenary*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2FireMercenaryByVillage %d, %d, %d, %d, %I64d, %I64d", lpDBIKDeleteMercenary->DBAccount, lpDBIKDeleteMercenary->uiSlot, lpDBIKDeleteMercenary->siDeleteMercenarySlot, lpDBIKDeleteMercenary->siVillageCode, lpDBIKDeleteMercenary->siMoney, lpDBIKDeleteMercenary->siEmployProfit);
			
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				//m_pSQL->DBBind(1, INTBIND, (DBINT) 0, (BYTE*)&Result);
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DELETEMONSTER:
		{
			DBIKDeleteMonster*		lpDBIKDeleteMonster;


			lpDBIKDeleteMonster = (DBIKDeleteMonster*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2FireMonsterByVillage %d, %d, %d, %d, %I64d, %I64d", lpDBIKDeleteMonster->DBAccount, lpDBIKDeleteMonster->uiSlot, lpDBIKDeleteMonster->siDeleteMercenarySlot, lpDBIKDeleteMonster->siVillageCode, lpDBIKDeleteMonster->siMoney, lpDBIKDeleteMonster->siEmployProfit);
			
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_QUEST_SETPLAYQUESTDATA:
		{
			DBIKQuestSetPlayQuestData*	lpDBIKQuestSetPlayQuestData;

			lpDBIKQuestSetPlayQuestData = (DBIKQuestSetPlayQuestData*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnQuestSetProgressQuest %d, %d, %d, %d, %d, %d"
				, lpDBIKQuestSetPlayQuestData->DBAccount, lpDBIKQuestSetPlayQuestData->uiSlot, lpDBIKQuestSetPlayQuestData->QuestIndex
				, lpDBIKQuestSetPlayQuestData->QuestData.m_QuestCode
				, lpDBIKQuestSetPlayQuestData->QuestData.m_QuestObject[0], lpDBIKQuestSetPlayQuestData->QuestData.m_QuestObject[1]) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_QUEST_CANCELQUEST:
		{
			DBIKQuestCancelQuest*		lpDBIKQuestCancelQuest;

			lpDBIKQuestCancelQuest = (DBIKQuestCancelQuest*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnQuestDelProgressQuest %d, %d, %d", lpDBIKQuestCancelQuest->DBAccount, lpDBIKQuestCancelQuest->uiSlot, lpDBIKQuestCancelQuest->QuestIndex) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_QUEST_COMPLETEQUEST:
		{
			DBIKQuestCompleteQuest*		lpDBIKQuestCompleteQuest;

			lpDBIKQuestCompleteQuest = (DBIKQuestCompleteQuest*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnQuestSetCompletedQuest %d, %d, %d", lpDBIKQuestCompleteQuest->DBAccount, lpDBIKQuestCompleteQuest->uiSlot, lpDBIKQuestCompleteQuest->QuestIndex) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFOKIND_QUEST_PAY_MONEY:
		{
			DBIKQuestPayMoney*		lpDBIKQuestPayMoney;

			lpDBIKQuestPayMoney = (DBIKQuestPayMoney*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeMoneyByQuest %d, %d, %d, %I64d"
				, lpDBIKQuestPayMoney->siDBAccount, lpDBIKQuestPayMoney->uiDBSlot
				, lpDBIKQuestPayMoney->uiQuestCode
				, lpDBIKQuestPayMoney->siMoney) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFO_DOKDO_QUEST_COMPLETED_SAVE :
		{
			DBQuestCompletedCount*		lpDBQuestCompletedCount;

			lpDBQuestCompletedCount = (DBQuestCompletedCount*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetQuestCount %d, %d, %d, %d"				
				, lpDBQuestCompletedCount->siDBAccount
				, lpDBQuestCompletedCount->uiDBSlot
				,lpDBQuestCompletedCount->uiQuestCode
				,lpDBQuestCompletedCount->nQuestCompletedCount) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );	
		}
		break;

	case SO_DB_INFOKIND_QUEST_PAY_ITEM:
		{
			DBIKQuestPayItem*		lpDBIKQuestPayItem;

			lpDBIKQuestPayItem = (DBIKQuestPayItem*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeItemByQuest %d, %d, %d, %d, %d, %d, %d"
				, lpDBIKQuestPayItem->siDBAccount, lpDBIKQuestPayItem->uiDBSlot
				, lpDBIKQuestPayItem->uiMercenarySlot, lpDBIKQuestPayItem->uiPosition
				, lpDBIKQuestPayItem->uiQuestCode
				, lpDBIKQuestPayItem->siItemID, lpDBIKQuestPayItem->siQuantity) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFOKIND_QUEST_PAY_CREDIT:
		{
			DBIKQuestPayCredit*		lpDBIKQuestPayCredit;

			lpDBIKQuestPayCredit = (DBIKQuestPayCredit*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeCreditByQuest %d, %d, %d, %d"
				, lpDBIKQuestPayCredit->siDBAccount, lpDBIKQuestPayCredit->uiDBSlot
				, lpDBIKQuestPayCredit->uiQuestCode
				, lpDBIKQuestPayCredit->siCredit) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	case SO_DB_INFOKIND_QUEST_PAY_EVENTITEM:
		{
			DBIKQuestPayEventItem*	lpDBIKQuestPayEvent;

			lpDBIKQuestPayEvent = (DBIKQuestPayEventItem*)pIn;
			//[09/13/04 06:35:16] EXEC SPHQ_On2TakeItemByEvent 2851151, 2, 0, 0, 22120, 8, 1, 1, 0,'2004-9-13' Fail!
			//[09/07/04 08:18:40] EXEC SPHQ_On2TakeItemByEvent 2348661, 3, 0, 0, 22120, 2, 1, 1, 0,'2004-9-7' Fail!
			//[09/13/04 19:59:15] EXEC SPHQ_On2TakeItemByEvent 1485485, 1, 0, 0, 22120, 1, 1, 1, 0,'2004-9-13' Fail!
			//[09/13/04 22:13:48] EXEC SPHQ_On2TakeItemByEvent 15719, 2, 0, 6, 22120, 6, 0, 1, 843,'2004-9-13' Fail!
			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeItemByEvent %d, %d, %d, %d, %d, %d, %d, %d, %d,'%s'"
				,lpDBIKQuestPayEvent->siDBAccount,lpDBIKQuestPayEvent->uiDBSlot,
				lpDBIKQuestPayEvent->uiMercenarySlot,lpDBIKQuestPayEvent->uiPosition,
				lpDBIKQuestPayEvent->uiQuestCode,lpDBIKQuestPayEvent->uiEventItemID,
				lpDBIKQuestPayEvent->uiDelivery,lpDBIKQuestPayEvent->uiQuantity,
				lpDBIKQuestPayEvent->uiItemID,lpDBIKQuestPayEvent->szdate) == false)
			{
				bSuccess = FALSE;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );

		}
		break;

	case SO_DB_INFOKIND_QUEST_PAY_EXP:
		{
			DBIKQuestPayEXP*		lpDBIKQuestPayEXP;

			lpDBIKQuestPayEXP = (DBIKQuestPayEXP*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2TakeExpByQuest %d, %d, %d, %d"
				, lpDBIKQuestPayEXP->siDBAccount, lpDBIKQuestPayEXP->uiDBSlot
				, lpDBIKQuestPayEXP->uiQuestCode
				, lpDBIKQuestPayEXP->siEXP) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFOKIND_QUEST_REQUEST_ITEM:
		{
			DBIKQuestRequest_Item*		lpDBIKQuestRequest_Item;

			lpDBIKQuestRequest_Item = (DBIKQuestRequest_Item*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2GiveItemByQuest %d, %d, %d, %d, %d, %d"
				, lpDBIKQuestRequest_Item->siDBAccount, lpDBIKQuestRequest_Item->uiDBSlot
				, lpDBIKQuestRequest_Item->uiMercenarySlot
				, lpDBIKQuestRequest_Item->siItemID
				, lpDBIKQuestRequest_Item->siQuantity
				, lpDBIKQuestRequest_Item->uiQuestCode) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFOKIND_QUEST_REQUEST_MONEY:
		{
			DBIKQuestRequest_Money*		lpDBIKQuestRequest_Money;

			lpDBIKQuestRequest_Money = (DBIKQuestRequest_Money*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_On2GiveMoneyByQuest %d, %d, %I64d, %d"
				, lpDBIKQuestRequest_Money->siDBAccount, lpDBIKQuestRequest_Money->uiDBSlot
				, lpDBIKQuestRequest_Money->siMoney
				, lpDBIKQuestRequest_Money->uiQuestCode) == false)
			{
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	//----------------------------------------------------------------------------------------
	//case SO_DB_INFOKIND_QUEST_
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_AUDIT_TOTAL_2:
		{
			DBIKPopertyRanking			*pPropertyRanking;
			PROPERTY_INFO				*pPropertyInfo;
			
			pPropertyRanking			=	(DBIKPopertyRanking*) pIn;
			pPropertyInfo				=	(PROPERTY_INFO*)	pOut;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetTotalProperty %d, %u, %I64d", pPropertyRanking->pPlayer->stAccountInfo.siDBAccount,
				pPropertyRanking->pPlayer->stAccountInfo.uiSlot, pPropertyRanking->siProperty) == false)
			{
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	

	
	//----------------------------------------------------------------------------------------		
	case SO_DB_INFOKIND_PLANT_CHANGE_PAY:
		{
			DBIKChangeFarmPay			*pChangePay;

			pChangePay			=	(DBIKChangeFarmPay*) pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2ChangePayOfEquipment %u, %u, %u, %I64d,%d , %u , %I64d",
							pChangePay->uiVillageCode, pChangePay->uiFarmID, pChangePay->uiFactoryID, pChangePay->moPay,pChangePay->siDBAccount, pChangePay->uiSlot,pChangePay->moDifference) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
	case SO_DB_INFOKIND_STORAGE_CHANGE_PAY:
		{
			DBIKChangeFarmPay			*pChangePay;

			pChangePay			=	(DBIKChangeFarmPay*) pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2ChangePayOfStorage %u, %u, %u, %I64d,%d , %u , %I64d",
							pChangePay->uiVillageCode, pChangePay->uiFarmID, pChangePay->uiFactoryID, pChangePay->moPay,pChangePay->siDBAccount, pChangePay->uiSlot,pChangePay->moDifference) == false)
			{				
				bSuccess = FALSE;
				break;
			}


			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_APPENDMERCENARY:
		{
			DBIKAppendMercenary*		lpDBIKAppendMercenary;

			lpDBIKAppendMercenary = (DBIKAppendMercenary*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnAppendMercenary %d, %d, %d, '%s', %d", 
						    lpDBIKAppendMercenary->DBAccount, lpDBIKAppendMercenary->uiSlot
							,lpDBIKAppendMercenary->siMercenarySlot,	lpDBIKAppendMercenary->szName, lpDBIKAppendMercenary->siMercenaryKind) == 0)
			{				
				bSuccess = FALSE;
				break;		
			}
			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_UPDATEMERCENARY:
		{
			DBIKUpdateMercenary*		lpDBIKUpdateMercenary;

			lpDBIKUpdateMercenary = (DBIKUpdateMercenary*)pIn;
			if(m_pSQL->SQLRun("EXEC SPHQ_OnUpdateMercenary %d, %d, %d, '%s', %d", 
							lpDBIKUpdateMercenary->DBAccount, lpDBIKUpdateMercenary->uiSlot
						   ,lpDBIKUpdateMercenary->siMercenarySlot,	lpDBIKUpdateMercenary->szName, lpDBIKUpdateMercenary->siMercenaryKind) == 0)
			{				
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;
	
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_TRADE :
		{					


			if(m_pSQL->SQLRun( "EXEC SPHQ_On2Trade %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_HEALATVILLAGE:
		{
			DBIKHealAtVillage*		lpDBIKHealAtVillage;

			lpDBIKHealAtVillage = (DBIKHealAtVillage*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_On2HealAtVillage %d, %d, %I64d, %d, %I64d", lpDBIKHealAtVillage->DBAccount, lpDBIKHealAtVillage->uiSlot, lpDBIKHealAtVillage->siMoney, lpDBIKHealAtVillage->uiVillageCode, lpDBIKHealAtVillage->siVillageCapital);
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;

	case SO_DB_INFOKIND_MOVEITEM:
		{
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2MoveItemByMercenary %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;
	case SO_DB_INFOKIND_SAVETOBANK:
		{
			DBIKSaveToBank *pSaveToBank;
			pSaveToBank				=	(DBIKSaveToBank*)	pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2Deposit %d, %d, %I64d, %d", pSaveToBank->DBAccount, pSaveToBank->uiSlot, pSaveToBank->moSaveMoney ,pSaveToBank->uiVillageCode ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);

		}
		break;
	case SO_DB_INFOKIND_WITHDRAWFROMBANK:
		{
			DBIKWithdrawFromBank *pWithdrawFromBank;
			pWithdrawFromBank				=	(DBIKWithdrawFromBank *)	pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2DrawOut %d, %d, %I64d, %d", pWithdrawFromBank->DBAccount, pWithdrawFromBank->uiSlot, pWithdrawFromBank->moDrawMoney ,pWithdrawFromBank->uiVillageCode ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;

	case SO_DB_KIND_BUYITEMINMARKET:
		{

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2BuyItemOfVillage %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);

		}
		break;

	case SO_DB_KIND_SELLITEMINMARKET:
		{
		
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2SellItemOfVillage %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);

		}
		
		break;

	case SO_DB_INFOKIND_DROPITEM:
		{
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2DropItem %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}

		break;

	case SO_DB_INFOKIND_PICKUPITEM:
		{
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2PickupItem %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}

		break;

	case SO_DB_INFOKIND_CREATEITEM_BYADMIN:
		{
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2CreateItemOfAdmin %s ", ( CHAR* )pIn ) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}

		break;

	case SO_DB_INFOKIND_CHANGEJOBMERCENARY:
		{
			DBIKChangeJobMercenary*		lpDBIKChangeJobMercenary;


			lpDBIKChangeJobMercenary = (DBIKChangeJobMercenary*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2PromoteMercenaryByVillage %d, %d, %d, %d, %d, %d", lpDBIKChangeJobMercenary->DBAccount, lpDBIKChangeJobMercenary->uiSlot, lpDBIKChangeJobMercenary->uiMercenarySlot, lpDBIKChangeJobMercenary->siChangeKind, lpDBIKChangeJobMercenary->siBonus, lpDBIKChangeJobMercenary->uiVillageCode);
			
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				//m_pSQL->DBBind(1, INTBIND, (DBINT) 0, (BYTE*)&Result);
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;

	case SO_DB_INFOKIND_BUYMERCENARY:
		{
			DBIKBuyMercenary*			lpDBIKBuyMercenary;


			lpDBIKBuyMercenary = (DBIKBuyMercenary*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2EmployMercenaryByVillage %d, %d, %d, %d, %d, %I64d, %I64d"
				, lpDBIKBuyMercenary->DBAccount
				, lpDBIKBuyMercenary->uiSlot
				, lpDBIKBuyMercenary->uiMercenarySlot
				, lpDBIKBuyMercenary->siKind
				, lpDBIKBuyMercenary->uiVillageCode
				, lpDBIKBuyMercenary->siMoney
				, lpDBIKBuyMercenary->siEmployProfit);
			
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				//m_pSQL->DBBind(1, INTBIND, (DBINT) 0, (BYTE*)&Result);
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_CONSUMEDONKEYITEM:
		{
			DBIKConsumeDonkeyItem		*pDBInfo;

			pDBInfo		=	(DBIKConsumeDonkeyItem*)pIn;

			m_pSQL->SQLRun( "EXEC SPHQ_On3ConsumeDonkeyItem %d, %d, %d, %d, %d, %d",
							pDBInfo->DBAccount, pDBInfo->uiSlot, pDBInfo->uiFollowerID, pDBInfo->uiItemID, pDBInfo->uiDonkeySlot, pDBInfo->siKind );

		}
		break;
	case SO_DB_INFOKIND_BUYMONSTER:
		{
			DBIKBuyMonster*			lpDBIKBuyMonster;


			lpDBIKBuyMonster = (DBIKBuyMonster*)pIn;

			m_pSQL->SQLRun("EXEC SPHQ_On2EmployMonsterByVillage %d, %d, %d, %d, %d, %I64d, %I64d, %u, %u"
				, lpDBIKBuyMonster->DBAccount
				, lpDBIKBuyMonster->uiSlot
				, lpDBIKBuyMonster->uiMercenarySlot
				, lpDBIKBuyMonster->siKind
				, lpDBIKBuyMonster->uiVillageCode
				, lpDBIKBuyMonster->siMoney
				, lpDBIKBuyMonster->siEmployProfit
				, lpDBIKBuyMonster->uiItemID
				, lpDBIKBuyMonster->siItemQuantity);
			
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				//m_pSQL->DBBind(1, INTBIND, (DBINT) 0, (BYTE*)&Result);
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_FARM_STORAGE_INPUT_ITEM:
		{
			DBIKStorageInputItem *lpDBInfo = (DBIKStorageInputItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2PutItemToStorage %u,%u,%u,%u,%u,%u,%u,%u,%u", 
				lpDBInfo->uiVillageCode,lpDBInfo->uiFarmID,lpDBInfo->uiFactoryID,
				lpDBInfo->uiStoreItemPosition,lpDBInfo->DBAccount,lpDBInfo->uiSlot,
				lpDBInfo->uiMercenary,lpDBInfo->uiItemCode,lpDBInfo->uiItemQnt) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		


			if((m_pSQL->DBResults()) == SUCCEED)
			{
			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
			

		}
		break;
	case SO_DB_INFOKIND_FARM_STORAGE_OUTPUT_ITEM:
		{
			DBIKStorageOutputItem *lpDBInfo = (DBIKStorageOutputItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2PutItemFromStorage %u,%u,%u,%u,%d,%u,%u,%u,%u,%u", 
				lpDBInfo->uiVillageCode,lpDBInfo->uiFarmID,lpDBInfo->uiFactoryID,
				lpDBInfo->uiStoreItemPosition,lpDBInfo->DBAccount,lpDBInfo->uiSlot,
				lpDBInfo->uiMercenary,lpDBInfo->uiMercenaryItemPosition,lpDBInfo->uiItemCode,lpDBInfo->uiItemQnt) == 0 )
			{	
				bSuccess = FALSE;
				break;		
			}		
	


			if((m_pSQL->DBResults()) == SUCCEED)
			{
			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}


		}
		break;


	// 아이템 뱅크에서 아이템 가져오기
	case SO_DB_INFOKIND_PUTITEM_FROMITEMBANK:
		{
			DBIKPutItemFromItemBank *lpDBInfo = (DBIKPutItemFromItemBank *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2PutItemFromItemBank %d, %d, %d, %d, %d, %d",
				lpDBInfo->DBAccount, lpDBInfo->uiSlot, lpDBInfo->uiFollowerID, lpDBInfo->uiPosition, lpDBInfo->siItemID, lpDBInfo->siQuantity ) == FALSE ) {
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;

	// 아이템 뱅크에 아이템 저장
	case SO_DB_INFOKIND_SAVEITEM_ITEMBANK:
		{
			DBIKSaveItemIntoItemBank *lpDBInfo = (DBIKSaveItemIntoItemBank *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2PutItemToItemBank %d, %d, %d, %d, %d, 100000000",
				lpDBInfo->DBAccount, lpDBInfo->uiSlot, lpDBInfo->uiFollowerID, lpDBInfo->siItemID, lpDBInfo->siQuantity ) == FALSE ) {
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{
			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;


	// 아이템 경매 추가 
	case SO_DB_INFOKIND_ADD_ITEM_AUCTION:
		{
			sDB_AddItemAuction* pAddItemAuction = (sDB_AddItemAuction*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2RegistItemByAuction %d, %d, %d, %d, %d, %d, %d, %d",
								pAddItemAuction->auctioneerDBAccount,
								pAddItemAuction->auctioneerCharslot,
								pAddItemAuction->hasItemFollowerSlot,
								pAddItemAuction->minPrice,
								pAddItemAuction->maxPrice,
								pAddItemAuction->itemID,
								pAddItemAuction->itemQuantity,
								pAddItemAuction->feePrice ) ) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;


	// 아이템 경매 낙찰 
	case SO_DB_INFOKIND_SUCCESSFULBID_ITEM_AUCTION:
		{
				
			sDB_SuccessfulBidItemAuction* pSuccessfulBidItemAuction = (sDB_SuccessfulBidItemAuction*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2SuccessfulBidItemByAuction %d, %d, %d, %d, %d, %d, %d, %d, %d",
								pSuccessfulBidItemAuction->bidderDBAccount,
								pSuccessfulBidItemAuction->bidderCharSlot,
								pSuccessfulBidItemAuction->recvItemFollowerSlot,
								pSuccessfulBidItemAuction->posInInventory,
								pSuccessfulBidItemAuction->auctioneerDBAccount,
								pSuccessfulBidItemAuction->auctioneerCharSlot,
								pSuccessfulBidItemAuction->biddingPrice,
								pSuccessfulBidItemAuction->itemID,
								pSuccessfulBidItemAuction->itemQuantity ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;

	
	// 아이템 경매 유찰 
	case SO_DB_INFOKIND_FAILBID_ITEM_AUCTION:
		{
			sDB_FailBidItemAuction* pFailBidItemAuction = (sDB_FailBidItemAuction*)pIn;
			
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2FailBidItemByAuction %d, %d",
								pFailBidItemAuction->auctioneerDBAccount,
								pFailBidItemAuction->auctioneerCharSlot ) ) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;

	// 아이템 경매 제거 
	case SO_DB_INFOKIND_DEL_ITEM_AUCTION:
		{
			sDB_DelItemAuction* pDelItemAuction = (sDB_DelItemAuction*)pIn;
			
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2DelItemByAuction %d, %d",
								pDelItemAuction->auctioneerDBAccount,
								pDelItemAuction->auctioneerCharSlot ) ) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}			
		}
		break;

	// NPC가 일한다.
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_WORKEND_BYNPC:
		{
			DBIKFarmWorkEndInfo		    *pWorkInfo = (DBIKFarmWorkEndInfo	*)pIn;
			

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2NPCCompleteProductOfItem %u, %u, %u, %u ,%u ,%u ,%u , %I64d",
							 pWorkInfo->uiVillageCode, pWorkInfo->uiFarmID,pWorkInfo->uiFactoryID,
							 pWorkInfo->siStorageID, pWorkInfo->siStoragePos, pWorkInfo->uiItemCode, pWorkInfo->uiItemQnt,
							 pWorkInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_WORK_BYNPC:
		{			
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			SI32					siResult = 0;

			
			// 처리
			if(m_pSQL->SQLRun("EXEC SPHQ_On2NPCWorkByStorage %u, %u, %u, %u, %I64d",
						pFarmInfo->uiVillageCode,pFarmInfo->uiFarmID,pFarmInfo->uiFactoryID,pFarmInfo->Labor,
						pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{							
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_WORK_BYNPC:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			SI32					siResult = 0;

			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2NPCWorkByEquipment %u, %u, %u, %u, %I64d",
						pFarmInfo->uiVillageCode,pFarmInfo->uiFarmID,pFarmInfo->uiFactoryID,pFarmInfo->Labor,
						pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}


			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
		}
		break;
	case SO_DB_INFOKIND_STORAGE_LEVELUP_BYNPC:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			SI32					siResult = 0;
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2NPCCompleteBuildOfStorage %u, %u, %u,  %I64d",
						pFarmInfo->uiVillageCode, pFarmInfo->uiFarmID, pFarmInfo->uiFactoryID,
						 pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			
			
		}
		break;

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_LEVELUP_BYNPC:
		{
			DBIKFarmWorkInfo		*pFarmInfo = (DBIKFarmWorkInfo *)pIn;
			SI32					siResult = 0;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2NPCCompleteRiseOfEquipment %u, %u, %u,  %I64d",
						pFarmInfo->uiVillageCode, pFarmInfo->uiFarmID, pFarmInfo->uiFactoryID,
						 pFarmInfo->moPay) == false)
			{								
				bSuccess = FALSE;
				break;
			}
			
			if ((m_pSQL->DBResults()) == SUCCEED)
			{
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
				
				
			}
			
		}
		break;
	case SO_DB_INFOKIND_INITMERCENARYPARAM:
		{
			DBIKInitMercenaryParamByItem * pMercenaryParam = (DBIKInitMercenaryParamByItem *)pIn;
			
			if(m_pSQL->SQLRun( "EXEC SPHQ_On2InitParamOfMercenary %d, %d , %d , %d, %d",
								pMercenaryParam->siDBAccount,
								pMercenaryParam->uiDBSlot,
								pMercenaryParam->uiMercenarySlot,
								pMercenaryParam->uiItemID,
								pMercenaryParam->siBonus) ) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;

	case SO_DB_INFOKIND_VMERCENARYSHOW:
		{
			DBIKVMercenaryShow*		pVMercenaryShow = (DBIKVMercenaryShow*)pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_On2ShowPet %d, %d, %d, %d", pVMercenaryShow->siDBAccount, pVMercenaryShow->uiDBSlot, pVMercenaryShow->uiMercenarySlot, pVMercenaryShow->uiItemID)) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;

	case SO_DB_INFOKIND_VMERCENARYDELETE:
		{
			DBIKVMercenaryHide*		pVMercenaryHide = (DBIKVMercenaryHide*)pIn;
			
			if(m_pSQL->SQLRun("EXEC SPHQ_On2HidePet %d, %d", pVMercenaryHide->siDBAccount, pVMercenaryHide->uiDBSlot)) 
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_BOOTHFOLLOWERTRADE:
		{
			DBIKBoothFollowerTrade *pTrade = (DBIKBoothFollowerTrade *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On3BusinessByMercenary %d, %u, %d, %I64d, %d, %u, %d, %I64d",
				                pTrade->siFromDBAccount,
								pTrade->uiFromDBSlot,
								pTrade->siFromMercenarySlot,
								pTrade->moFromMoney,
								pTrade->siToDBAccount,
								pTrade->uiToDBSlot,
								pTrade->siToMercenarySlot,								
								pTrade->moToMoney
								) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}



		}
		break;

	case SO_DB_INFOKIND_BOOTHTRADE:
		{

			DBIKBoothTrade *pTrade = (DBIKBoothTrade *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2Business %d, %d, %d, %I64d, %d, %d, %d, %d, %I64d, %d, %d",
				                pTrade->siFromDBAccount,
								pTrade->uiFromDBSlot,
								pTrade->uiFromMercenarySlot,
								pTrade->moFromMoney,
								pTrade->siToDBAccount,
								pTrade->uiToDBSlot,
								pTrade->uiToMercenarySlot,
								pTrade->uiToItemSlot,
								pTrade->moToMoney,
								pTrade->uiItemID,
								pTrade->uiQuantity
								) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}


		}
		break;
	case SO_DB_INFOKIND_SMITH_REFINE_SUCCESS:
		{

			DBIKRefineItem *pRefineItem = (DBIKRefineItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2SuccessGambleByOldItem %d, %d, %d,  %d, %d, %d, %I64d",
								 pRefineItem->siDBAccount,
								 pRefineItem->uiDBSlot,
								 pRefineItem->uiMercenarySlot,
								 pRefineItem->uiOldItemID,
								 pRefineItem->uiItemSlot,
								 pRefineItem->uiNewItemID,
								 pRefineItem->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}


		}
		break;
	case SO_DB_INFOKIND_SMITH_REFINE_NO_CHANGES:
		{

			DBIKRefineItem *pRefineItem = (DBIKRefineItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2MaintainGambleByOldItem %d, %d, %d,   %I64d",
								 pRefineItem->siDBAccount,
								 pRefineItem->uiDBSlot,
								 pRefineItem->uiOldItemID,
								 pRefineItem->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}


		}
		break;
	case SO_DB_INFOKIND_SMITH_REFINE_FAILED:
		{

			DBIKRefineItem *pRefineItem = (DBIKRefineItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2FailGambleByOldItem %d, %d, %d,  %d,  %I64d",
								 pRefineItem->siDBAccount,
								 pRefineItem->uiDBSlot,
								 pRefineItem->uiMercenarySlot,
								 pRefineItem->uiOldItemID,
								 pRefineItem->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}


		}
		break;
	case SO_DB_INFOKIND_CHANGESEX:
		{
			DBIKChangeSex *pDbinfo = (DBIKChangeSex *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2ChangeSexOfCharacter %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiNewCharactorCode,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}



		}
		break;
	case SO_DB_INFOKIND_CHANGENATION:
		{
			DBIKChangeNation *pDbinfo = (DBIKChangeNation *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2ChangeNationOfCharacter %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiNewCharactorCode,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;

	//공성유닛이 죽을 때.
	case SO_DB_INFOKIND_CHANGECHARACTER:
		{
			DBIKChangeCharacter	*pDbinfo	=	(DBIKChangeCharacter *)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2WarCharDie %d, %d, %d",
				               pDbinfo->siDBAccount, pDbinfo->uiDBSlot, pDbinfo->siDeadTime))
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	//공성유닛으로 변신할때.
	case SO_DB_INFOKIND_IMPERIUM_USE:
		{
			DBIKImperumUse* pDbinfo			=	(DBIKImperumUse*)pIn;
	
//			if(m_pSQL->SQLRun("EXEC SPHQ_On2ChangeWar %d, %d, %d, %d, %d, %d, %d, %d",
//			pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiNewKind,
//			pDbinfo->bCaptain, pDbinfo->siHp,pDbinfo->siSupply,pDbinfo->uiItemID,pDbinfo->uiQuantity) == 0)
			if(m_pSQL->SQLRun("EXEC SPHQ_On2ChangeWar %d, %d, %d, %d, %d, %d",
			                   pDbinfo->siDBAccount, pDbinfo->uiDBSlot, pDbinfo->uiNewKind,
			                   pDbinfo->siHp, pDbinfo->siMp, pDbinfo->bCaptain) == 0)
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_INCREASE_GUILD_UNIT:
		{
			DBIKMakeGuildUnit* pDbinfo		=	(DBIKMakeGuildUnit*)pIn;

//			if(m_pSQL->SQLRun("EXEC SPHQ_On2MakeWarItem %d, %d, %d, %d, %d, %d, %I64d, %d, %d",
//				pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiGuildID,pDbinfo->uiItemID,
//				pDbinfo->siQuantity,pDbinfo->siSupply,pDbinfo->mnMoney,pDbinfo->uiVillageCode,pDbinfo->uiPopulation) == 0)
			
			if(pDbinfo->siQuantity <= 0) 
			{
				printf("[SO_DB_INFOKIND_INCREASE_GUILD_UNIT]DBAccount:%d, Kind:%d, Count:%d\n", 
					    pDbinfo->siDBAccount, pDbinfo->uiItemID, pDbinfo->siQuantity);
				
				writeInfoToFile("WarLog.txt", "[SO_DB_INFOKIND_INCREASE_GUILD_UNIT]DBAccount:%d, Kind:%d, Count:%d\n", 
					             pDbinfo->siDBAccount, pDbinfo->uiItemID, pDbinfo->siQuantity);
				return;
			}					
			
			if(m_pSQL->SQLRun("EXEC SPHQ_On2MakeWarItem %d, %d, %d, %d, %d, %I64d, %d, %d",
				               pDbinfo->siDBAccount, pDbinfo->uiDBSlot, pDbinfo->uiGuildID, 
							   pDbinfo->uiItemID, pDbinfo->siQuantity,  pDbinfo->mnMoney, 
							   pDbinfo->uiVillageCode, pDbinfo->uiPopulation) == 0)
			{
				bSuccess = FALSE;
				break;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DECREASE_GUILD_UNIT:
		{
			DBIKDecreaseGuildUnit* pDbinfo		=	(DBIKDecreaseGuildUnit*)pIn;

//			if(m_pSQL->SQLRun("SPHQ_On2GetGiveItem %d, %d, %d, %d, %d, %d, %d",
//				pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiGuildID,pDbinfo->uiItemID,
//				pDbinfo->siQuantity, pDbinfo->uiMercenarySlot,pDbinfo->siInventoryPos) == 0)
			
			if(pDbinfo->siQuantity <= 0) 
			{
				printf("[SO_DB_INFOKIND_DECREASE_GUILD_UNIT]DBAccount:%d, Kind:%d, Count:%d\n", 
					    pDbinfo->siDBAccount, pDbinfo->uiItemID, pDbinfo->siQuantity);
				
				writeInfoToFile("WarLogError.txt", "[SO_DB_INFOKIND_DECREASE_GUILD_UNIT]DBAccount:%d, Kind:%d, Count:%d\n", 
					             pDbinfo->siDBAccount, pDbinfo->uiItemID, pDbinfo->siQuantity);
				return;
			}

	        if(m_pSQL->SQLRun("SPHQ_On2TakeWarItem %d, %d, %d, %d, %d",
				               pDbinfo->siDBAccount, pDbinfo->uiDBSlot, pDbinfo->uiGuildID,
							   pDbinfo->uiItemID, pDbinfo->siQuantity) == 0)
			{
				bSuccess = FALSE;
				break;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_INCREASE_SUPPLY:
		{
			DBIKMakeGuildSupply* pDbinfo	=	(DBIKMakeGuildSupply*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2MakeSupplyItem %d, %d, %d, %d, %I64d",
				pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiGuildID,pDbinfo->siQuantity,pDbinfo->mnMoney) == 0)
			{
				bSuccess = FALSE;
				break;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DECREASE_SUPPLY:
		{	
			DBIKDecreaseGuildSupply* pDbinfo	=	(DBIKDecreaseGuildSupply*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2GetUserSupplyItem %d, %d, %d, %d",
				pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiGuildID,pDbinfo->siQuantity) == 0)
			{
				bSuccess = FALSE;
				break;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_INCREASE_DEFENCESOLDIER:
		{
			DBIKIncreaseSoldierDefence* pDbinfo	=	(DBIKIncreaseSoldierDefence*)pIn;
			
			if(m_pSQL->SQLRun("SPHQ_On2IncreaseGuard %d, %d, %d, %d, %d, %d, %I64d, %d",
				pDbinfo->siDBAccount,pDbinfo->uiDBSlot,pDbinfo->uiGuildID,pDbinfo->siVillageCode,
				pDbinfo->siDefenceSoldier,pDbinfo->siSupply,pDbinfo->mnMoney,pDbinfo->uiPopulation) == 0)
			{
				bSuccess = FALSE;
				break;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_INCREASE_POPULATION:
		{
			DBIKSetVillageInfo* pDbinfo = (DBIKSetVillageInfo*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2IncreasePopulation %d, %d, %d",
				pDbinfo->uiVillageCode,pDbinfo->uiPopulation,pDbinfo->uiMaxPopulation) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DECREASE_POPULATION:
		{
			DBIKDecreasePopulation* pDbinfo = (DBIKDecreasePopulation*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2DecreasePopulation %d, %d",
				pDbinfo->uiVillageCode,pDbinfo->uiPopulation) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_SET_VILLAGE_DAMAGE:
		{
			DBIKVillageDamage*	pDbinfo	=	(DBIKVillageDamage*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2GiveDamage %d, %d,%d",
				pDbinfo->uiGuildID,pDbinfo->siVillageCode,pDbinfo->siDamage) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_SET_GUILD_RESULT:
		{
			DBIKGuildResult*	pDbinfo	=	(DBIKGuildResult*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2SinkVillage %d, %d, %d, '%s'",
				pDbinfo->uiGuildID,pDbinfo->siVillageCode,pDbinfo->siPercent,pDbinfo->szCharName) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_SET_DELETE_INVESTMENT:
		{
			DBIKGuildInvestVillage*		pDbinfo	=	(DBIKGuildInvestVillage*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2InvestVillage %d, %d",
				pDbinfo->uiGuildID,pDbinfo->uiVillageCode) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_SET_GUILD_WAR_END:		// 10시에 공성전이 끝나면.체크.
		{
			if(m_pSQL->SQLRun("SPHQ_On2ChangePeaceAll") == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_SET_POWER_INVESTMENT_END:
		{
			if(m_pSQL->SQLRun("SPHQ_On2DeleteWarInfo") == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DELETE_CHARACTOR_WAR:
		{
			DBIKDelteWarCharactor*	pDBIKDelteWarCharactor = (DBIKDelteWarCharactor*)pIn;

			if(m_pSQL->SQLRun("SPHQ_On2ChangePeace %d, %d",pDBIKDelteWarCharactor->siDBAccount,pDBIKDelteWarCharactor->uiDBSlot) == 0)
			{
				bSuccess = FALSE;
			}
			if((m_pSQL->DBResults()) == SUCCEED)
			{
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_REWARDTODAIFORDESTROYVILLAGE:
		{
			DBIKGuildFallVillage* pDBIKGuildFallVillage = (DBIKGuildFallVillage*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2FallVillage %d, %d, %d", pDBIKGuildFallVillage->uiVillageCode, 
				                pDBIKGuildFallVillage->uiGuildID, pDBIKGuildFallVillage->mnMoney) == 0)
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;
	
	case SO_DB_INFOKIND_UPDURABILITYOFVILLAGE:
		{
			DBIKUpDurabilityOfVillage* pDBIKUpDurabilityOfVillage = (DBIKUpDurabilityOfVillage*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2UpDurabilityOfVillage %d, %d, %d, %d, %d, %d"
				, pDBIKUpDurabilityOfVillage->uiVillageCode
				, pDBIKUpDurabilityOfVillage->siDBAccount
				, pDBIKUpDurabilityOfVillage->uiDBSlot
				, pDBIKUpDurabilityOfVillage->siUpDurability
				, pDBIKUpDurabilityOfVillage->siRate1
				, pDBIKUpDurabilityOfVillage->siRate2) == 0)
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;
	
	case SO_DB_INFOKIND_GETOUTINVERTMENT:
		{
			DBIKGetOutInvestment* pDBIKGetOutInvestment = (DBIKGetOutInvestment*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2DepositByInvest %d, %d, %d, %I64d, %I64d"
				, pDBIKGetOutInvestment->uiVillageCode
				, pDBIKGetOutInvestment->siDBAccount
				, pDBIKGetOutInvestment->uiDBSlot
				, pDBIKGetOutInvestment->GetOutInvestment
				, pDBIKGetOutInvestment->Tex) == 0)
			{	
				bSuccess = FALSE;
				break;		
			}		

			m_pSQL->DBResults();
			while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
		}
		break;
	// 상단을 생성 한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_CREATE:
		{
			DBIKGuildCreateInfo	*pGuildCreateInfo;
			

			pGuildCreateInfo	=	(DBIKGuildCreateInfo*)pIn;
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2MakeGuild %d ,%d, '%s ', %d, %d, %I64d ",	pGuildCreateInfo->uiVillageCode, pGuildCreateInfo->uiGuildID, pGuildCreateInfo->szGuildName, pGuildCreateInfo->siAccount,pGuildCreateInfo->uiSlot, pGuildCreateInfo->moMoney	) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{	
				
				while ( m_pSQL->DBNextRow() != NO_MORE_ROWS );
			}												
		}
		break;
	case SO_DB_INFOKIND_CONSUMEITEM:
		{
			DBIKConsumeItem *pDBInfo = (DBIKConsumeItem *)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2ConsumeItemByMercenary %d, %d, %d, %d",
				pDBInfo->siDBAccount,
				pDBInfo->uiSlot,
				pDBInfo->uiFollowerID,
				pDBInfo->uiItemID	) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if(m_pSQL->DBResults() == SUCCEED)
			{	
				
				while ( m_pSQL->DBNextRow() != NO_MORE_ROWS );
			}



		}
		break;
	case SO_DB_INFOKIND_CONSUMECREDITITEM:
		{
			DBIKConsumeCreditItem *pDbinfo = (DBIKConsumeCreditItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2ConsumeItemForCredit %d, %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiFollowerID,
								 pDbinfo->uiCreditUp,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_INCREASEMAXGENERAL:
		{
			DBIKIncreaseMaxGeneral *pDBInfo	=	(DBIKIncreaseMaxGeneral*)pIn;

			if( !m_pSQL->SQLRun("EXEC SPHQ_On3IncreaseMaxGeneral %d, %d, %d, %d",
								pDBInfo->siDBAccount, pDBInfo->uiDBSlot, pDBInfo->uiFollowerID, pDBInfo->uiItemID) )
			{
				bSuccess = FALSE;
				break;
			}
		}
		break;
	case SO_DB_INFOKIND_UPGRADEWEAPONSUCCESS:
		{
			DBIKUpgradeWeaponSuccess *pDbinfo = (DBIKUpgradeWeaponSuccess *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On3useMagicStone_success %d, %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiFollowerID,
								 pDbinfo->uiUpgradeItemID,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_UPGRADEWEAPONFAILURE:
		{
			DBIKUpgradeWeaponFailure *pDbinfo = (DBIKUpgradeWeaponFailure*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On3useMagicStone_failure %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiFollowerID,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_UPGRADEWEAPONBROKEN:
		{
			DBIKUpgradeWeaponBroken *pDbinfo = (DBIKUpgradeWeaponBroken*)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On3useMagicStone_broken %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiFollowerID,
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_USEPORTAL:
		{
			DBIKUsePortal *pDbinfo = (DBIKUsePortal *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2UsePortal %d, %d, %d, %I64d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->siPortalNum,
								 pDbinfo->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_GIVEMONEYBYADMIN:
		{
			DBIKGiveMoneyByAdmin *pDbinfo = (DBIKGiveMoneyByAdmin *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2GiveMoneyByAdmin %d, %d, %I64d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DIEATBATTLELOSEITEM:
		{
			DBIKDieAtBattleLoseItem *pDbinfo = (DBIKDieAtBattleLoseItem *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2LoseItemAtBattle %d, %d, %d, %d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiMercenaryID,								 
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_DIEATBATTLELOSEMONEY:
		{
			DBIKDieAtBattleLoseMoney *pDbinfo = (DBIKDieAtBattleLoseMoney *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2LoseMoneyAtBattle %d, %d, %I64d",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->moMoney								 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_GETITEMATBATTLE:
		{
			DBIKGetItemAtBattle *pDbinfo = (DBIKGetItemAtBattle *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2TakeItemAtBattle %d, %d, %d, %d, %d, %u",
								 pDbinfo->siDBAccount,
								 pDbinfo->uiDBSlot,
								 pDbinfo->uiMercenaryID,
								 pDbinfo->uiItemSlot,
								 pDbinfo->uiItemID,
								 pDbinfo->uiVictimCharKind
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_ROBITEMATBATTLE:
		{
			DBIKRobItemAtBattle *pDbinfo = (DBIKRobItemAtBattle *)pIn;

			if(m_pSQL->SQLRun( "EXEC SPHQ_On2RobItemAtBattle %d, %d, %d, %d, %d, %d, %d, %d",
								 pDbinfo->siToDBAccount,
								 pDbinfo->uiToDBSlot,
								 pDbinfo->uiToMercenaryID,
								 pDbinfo->uiToItemSlot,

								 pDbinfo->siFromDBAccount,
								 pDbinfo->uiFromDBSlot,
								 pDbinfo->uiFromMercenaryID,
								 
								 
								 pDbinfo->uiItemID							 
				                ) ) 
			{
				bSuccess = FALSE;
				break;
			}


			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_AUDIT_FRIEND_SET_DENIAL:
		{
			CHAR						szID[ON_ID_LENGTH+1];
			DBIKAuditFriendSetDenial	*pDBInfo	=	(DBIKAuditFriendSetDenial*)pIn;

			ZeroMemory( szID, sizeof(szID) );
			strncpy( szID, pDBInfo->szFriendCharID, sizeof(ON_ID_LENGTH) );
			if( m_pSQL->SQLRun( "EXEC SPHQ_On3SetFriendDenial %d, %d, '%s', %d",
								pDBInfo->siDBAccount,
								pDBInfo->uiSlot,
								pDBInfo->szFriendCharID,
								pDBInfo->bDenial ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE:
		{
			DBIKAuditFriendAddOne	*pDBInfo	=	(DBIKAuditFriendAddOne*)pIn;

			if( m_pSQL->SQLRun( "EXEC SPHQ_On3AddFriendList %d, %d, '%s'",
								pDBInfo->siDBAccount,
								pDBInfo->uiSlot,
								pDBInfo->szFriendCharID ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_SOME:
		{
			DBIKAuditFriendRemoveSome	*pDBInfo	=	(DBIKAuditFriendRemoveSome*)pIn;

			if( m_pSQL->SQLRun( "EXEC SPHQ_On3SubFriendList %d, %d, '%s'",
								pDBInfo->siDBAccount,
								pDBInfo->uiSlot,
								pDBInfo->szFriendCharID ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL:
		{
			DBIKAuditFriendRemoveAll	*pDBInfo	=	(DBIKAuditFriendRemoveAll*)pIn;

			if( m_pSQL->SQLRun( "EXEC SPHQ_On3SubFriendListAll %d, %d",
								pDBInfo->siDBAccount,
								pDBInfo->uiSlot ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;

	case SO_DB_INFOKIND_EVENT_EXPOBATTLERESULT:
		{
			DBIKEvent_ExpoBattleResult	*pDBInfo	=	( DBIKEvent_ExpoBattleResult* )pIn;

			if( m_pSQL->SQLRun( "EXEC SPHQ_On3ExpoBattleResulte %d, %d, %d, %d, %d", 
								 pDBInfo->siWinDBAccount,
								 pDBInfo->uiWinDBSlot,
								 pDBInfo->siLoseDBAccount,
								 pDBInfo->uiLoseDBSlot,
								 pDBInfo->siScore ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}

		break;
	
	case SO_DB_INFOKIND_EVENT_EXPORANKING:
		{
			DBIKEvent_ExpoRanking		*pDBInfo	=	( DBIKEvent_ExpoRanking* )pIn;

			if( m_pSQL->SQLRun( "EXEC SPHQ_On3ExpoRanking %d, 1, %d", 
								 pDBInfo->siDBAccount,								 
								 pDBInfo->siRanking ) )
			{
				bSuccess = FALSE;
				break;
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
		
	case SO_DB_INFOKIND_SET_WARGIVEUP:
		{
			DBIKWarGiveUp          *pDBInfo = (DBIKWarGiveUp*)pIn;
			
			if(pDBInfo->siFlag == 0)
			{
				if(!m_pSQL->SQLRun( "EXEC SPHQ_On2SetWarGiveup %d, NULL, NULL, NULL", pDBInfo->uiGuildID))
				{
					bSuccess = FALSE;
					break;
				}
			}
			else
			{
				if(!m_pSQL->SQLRun( "EXEC SPHQ_On2SetWarGiveup %d, %d, %I64d, %d", 
								     pDBInfo->uiGuildID, pDBInfo->uiVillageCode, pDBInfo->mnMoney, pDBInfo->siFlag))
				{
					bSuccess = FALSE;
					break;
				}
			}

			if((m_pSQL->DBResults()) == SUCCEED)
			{			
				while(m_pSQL->DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;

	default:
		return;

	}

	m_pSQL->ClearCMDBuf();
	m_pSQL->ClearRows();
}


