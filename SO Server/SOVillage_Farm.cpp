#include "SOMain.h"
#include "SODefine.h"
#include "SOWorld.h"
#include "SOVillage.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ǹ��� �ʱ� ������ ���´�. (���� �õ���..)
//	���� ����	:	2002-09-23 ���� 6:04:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::GetFarmData()
{
	if( clWorld->clDatabase->GetDBInfo( SO_DB_INFOKIND_FARM_GETINFO, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;	// �Ǽ� �ǹ��� �д´�..	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_FARM_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )		return FALSE;	// â�� ����Ÿ�� �д´�..(â�� ���̺�)	
	if( clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_STORAGE_GETDATA, LPVOID( uiVillageCode ), LPVOID( m_pIndustrialComplexs ) ) == FALSE )	return FALSE;
	
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ϱ� ��ư�� �������� ���ϱⰡ �������� �����ش�.
//	���� ����	:	2002-09-23 ���� 6:31:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::PlantWorkAble( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )														return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.
	
	return	m_pIndustrialComplexs->BeginWork( pPlayer );					
}




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ��ô�Ѵ�.
//	���� ����	:	2002-10-10 ���� 6:59:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::ExpandFarmSlot( SOPlayer *pPlayer, UI08 uiPlantID, UI08 uiFarmID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )															return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.

	if( m_pIndustrialComplexs->Reclamation( pPlayer, SI32( uiFarmID ), &mnCapital ) == FALSE )	return	FALSE;		// ������ ��ô�Ѵ�.



	// �÷��̾�, ����,���� ID,���� ID,����ü�����,

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);	

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �Ǽ��Ѵ�.
//	���� ����	:	2002-10-10 ���� 9:55:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::ConstructFactory( SOPlayer *pPlayer, SI32 siFactoryID, CHAR cFactoryCode[ 4 ], MONEY mnPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.

	if( m_pIndustrialComplexs->ConstructFactory( pPlayer, SI32( siFactoryID ), cFactoryCode, mnPay, &mnCapital ) == FALSE )	return	FALSE;		// ������ ��ô�Ѵ�.


	IncreaseCapital( mnCapital, ON_PROFIT_PLANT , FALSE);	// db���� �˸����ʴ´�

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���Կ� �ǹ��� ö���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::DestroyPlantSlot(SOPlayer *pPlayer, UI08 uiSlotID)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																						return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.
	
	if( m_pIndustrialComplexs->DestroyFactory( pPlayer, SI32( uiSlotID ), &mnCapital ) == TRUE )
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
	
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���׷��̵� �Ѵ�. (���׷��̵� ��ȹ�� �����.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage::UpgradeSlot(SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay)
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																				return	FALSE;	// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.	
	
	if( m_pIndustrialComplexs->UpgradeFactory( pPlayer, SI32( uiSlot ), MONEY( siPay ), &mnCapital ) == FALSE )		return	FALSE;

	IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);

	

	return	TRUE;

	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.(�ʱ� ��ȹ)
//	���� ����	:	2002-08-17 ���� 4:22:54 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	SOVillage::ProductItem(SOPlayer *pPlayer, UI08	uiPlantID, UI08 uiFarmID, UI08 uiSlot, UI16 uiItem, UI32 Quantity, UI32 uiPay)
{	
	MONEY	mnCapital;
	DWORD	dwResoponse;

	if( DoesFactoryExist() == FALSE )																										return	FALSE;	// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.	
	
	dwResoponse	=	m_pIndustrialComplexs->ProduceItem( pPlayer, SI32( uiSlot ), uiItem, Quantity, MONEY( uiPay ) , cNation, &mnCapital );

	if( dwResoponse	== ON_RET_OK )
	{
		IncreaseCapital( mnCapital, ON_PROFIT_PLANT ,FALSE);
	}
	
	return	dwResoponse;

	
}
/*
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	â�� ��ᰡ �ִ��� Ȯ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
					if( pFarmSlot->pFacility && pFarmSlot->bStore )   // â����...
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.(����� ���ڸ� �����Ѵ�.)
//	���� ����	:	2002-09-23 ���� 7:14:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	if( pPlayer == NULL)													return	0;			// ��ȿ�� �÷��̾����� �˻��Ѵ�.
	if( pFarmGroup == NULL)													return	0;			// ���� �ü����� �ִ��� �˻��Ѵ�.
	if( ( pFarm = pPlayer->stAccountInfo.pStructure ) == NULL )				return	0;			// �÷��̾ ���� �ü��� �ȿ� �� �ִ��� �˻��Ѵ�.
	if( ( pFarmSlot	= pFarm->GetFarmSlot( uiSlot ) ) == NULL )				return	0;			// �÷��̾ �۾��� �ǹ��� ��ȿ���� �˻��Ѵ�.
	if( pFarmSlot->pFacility == NULL )										return	0;			// ?????
	if( pFarmSlot->bWorkable ==	FALSE )										return	0;			// �ش� �ǹ��� �۾��� �� �� �ִ� �������� �˻��Ѵ�;

	// ���� �뵿�� =  ���� �뵿 + �߰��� �뵿
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
		// ���׷��̵� �۾�

		if( uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor				=	pFarmSlot->uiRemainLabor;
			TempPay					=	ResultLabor * pFarmSlot->siPay;
			if( pFarmSlot->bStore )						
			{	
				// â���� ���				
				clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_STORAGE_LEVELUP, &SetFarmProduct, sizeof(SetFarmProduct) );

				pFarmSlot->Storage.Init();
			}
			else
			{	
				// �Ϲݰǹ��� ���				
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
				// â���� ���				
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
		// ������ ���� �۾�

		uiItem		=	pFarmSlot->ProductItem;
		StgSlot		=	GetEmptyStorage(SetFarmProduct.uiPlantID,SetFarmProduct.uiFarmID,uiItem,StgLoc, StgQnt);
		if(StgSlot == UI08_ERROR)		return 0L;
		
		if(  uiLabor >= pFarmSlot->uiRemainLabor )
		{
			ResultLabor	=	pFarmSlot->uiRemainLabor;
			Quantity	=	pFarmSlot->Structure.GetRemainNum();	//	��������: ���� uiLabor�� ���´�.
			SetFarmProduct.Labor		=	0;
			TempPay		=	ResultLabor * pFarmSlot->siPay;
			
			// �Ϲݰǹ��� ���			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORKEND, &SetFarmProduct, sizeof(SetFarmProduct) );

			pFarmSlot->Structure.Init();
			pFarmSlot->EndProduct();
			pFarm->FarmInfo.uiRemainLabor	-=	ResultLabor;
		}
		else
		{
			ResultLabor	=	uiLabor + pFarmSlot->uiLabor;
			Quantity	=	GetQuantityByLabor(uiItem, ResultLabor);	//	��������: ���� uiLabor�� ���´�.
			SetFarmProduct.Labor		=	ResultLabor;
			// �������� ������ ���
			SetFarmProduct.Quantity		=	pFarmSlot->Structure.uiNum + Quantity;
			TempPay						=	uiLabor * pFarmSlot->siPay;
			
			// DB ������Ʈ			
			clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_FARM_WORK, &SetFarmProduct, sizeof(SetFarmProduct));

			// ���� ������Ʈ
			pFarmSlot->uiLabor			=	ResultLabor;
			pFarmSlot->Structure.uiNum	=	SetFarmProduct.Quantity;
			pFarmSlot->uiRemainLabor		-=	uiLabor;
			pFarm->FarmInfo.uiRemainLabor	-=	uiLabor;
		}
		// ������ ���� ������ ���� ���귮�� ITEM Table�� �����Ѵ�.
		DBIKItemNum			ItemNum;
		ItemNum.ItemID		=	uiItem;
		ItemNum.Num			=	Quantity;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_ITEMPRODUCTSET, &ItemNum, sizeof(ItemNum) );
		
		if( SetStorageItem(SetFarmProduct.uiFarmID, StgSlot, uiItem, (StgQnt+Quantity), StgLoc) == FALSE )
			return 0;
	}
	
	// ��� ���ϴ� �۾��� ���...
	pFarm->siWorkDay = 0;
	memcpy(uiPay, &TempPay, sizeof(MONEY));
	pPlayer->IncreaseMoney(TempPay);
	
	return Quantity;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뵿���� ������ ������ �� �ִ�  ������ �ٲ۴�. ���� �뵿�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//			printf("�뵿��[%u]���� ������[%u]���� �������� ���� �� �ִ�.\n",uiLabor,uiQuantity);
			uiLabor = uiLabor % (ItemCompose->m_siLabor);
		}
	}
	return uiQuantity;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ������ �뵿������ �ٲ۴�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//			printf("�뵿��[%u]���� ������[%u]���� �������� ���� �� �ִ�.\n",uiLabor,uiQuantity);
		}
	}
	return uiLabor;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�뵿���� ������ ������ �� �ִ�  ������ �ٲ۴�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	â���� �������� ������.(���� ��� ����)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::StorageOutputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.

	return	m_pIndustrialComplexs->DeleteItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	â�� �������� ���� �ִ´�.(���� ��� ����) - ���� ���� ������ ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::StorageInputItem( SOPlayer *pPlayer, UI16 uiItemCode, UI16 uiItemQnt )
{

	if( DoesFactoryExist() == FALSE )													return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.

	return	m_pIndustrialComplexs->AddItem( pPlayer, uiItemCode, uiItemQnt ,FALSE);

}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ӱ��� �����Ѵ�.
//	���� ����	:	2002-07-24 ���� 11:37:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::ChangePay( SOPlayer *pPlayer, UI08 uiSlot, SI16 siPay )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )														return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.

	if( m_pIndustrialComplexs->ChangeFactoryPay( pPlayer, MONEY( siPay ), &mnCapital ) == TRUE )
	{	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}


}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡 ����.
//	���� ����	:	2002-10-11 ���� 4:34:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::EnterIndustrialComplexFactory( SOPlayer *pPlayer, SI32 siFactoryID )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.	
		
	return	m_pIndustrialComplexs->EnterFactory( pPlayer, siFactoryID );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ ���忡�� ������.
//	���� ����	:	2002-10-11 ���� 4:45:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::LeaveIndustrialComplexFactory( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.			

	return	m_pIndustrialComplexs->LeaveFactory( pPlayer );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ��� ������ ������ �ִ��� �˾ƿ´�.
//	���� ����	:	2002-10-15 ���� 1:23:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::IsIndustrialComplexOwner( SOPlayer *pPlayer )
{
	if( DoesFactoryExist() == FALSE )													return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.				

	if( m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer ) == NULL )				return	FALSE;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ ��� ������ ������ �ִ��� �˾ƿ´�.
//	���� ����	:	2002-10-15 ���� 1:23:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_IndustrialComplex  *SOVillage::GetIndustrialComplexOwner( SOPlayer *pPlayer )
{

	if ( pPlayer == NULL) return NULL;
	if( DoesFactoryExist() == FALSE )													return	NULL;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.				

	return m_pIndustrialComplexs->GetOwnIndustrialComplex( pPlayer );

	
}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �۾����� ���忡�� �۾����ΰ��� ����Ѵ�.
//	���� ����	:	2002-10-16 ���� 11:28:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::CancelWork( SOPlayer *pPlayer, SI32 siFactoryID )
{
	MONEY	mnCapital;

	if( DoesFactoryExist() == FALSE )																return	FALSE;		// �� ������ �ǹ��� �ִ��� �˻��Ѵ�.				

	if( m_pIndustrialComplexs->CancelWork( pPlayer, siFactoryID, &mnCapital ) == TRUE )
	{
	
	
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}

	
}