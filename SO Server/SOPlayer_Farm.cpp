#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "OnlineCommonStruct.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾ �����ϰ� �ִ� �ü����� ���´�.
//	���� ����	:	2002-10-04 ���� 7:38:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
stFarmData*	SOPlayer::GetOwnFactory()
{
//	return pMyWorld->GetOwnFactory( stAccountInfo.siDBAccount, stAccountInfo.uiSlot );
	return	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ ������ ���´�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ������ �����Ѵ�.
//	���� ����	:	2002-10-08 ���� 8:29:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID SOPlayer::RecvFarmBuy()
{
	OnReqFarmBuyMsg			*pReqFarmBuyMsg;
	BOOL					bResult = FALSE;
	
	pReqFarmBuyMsg	=		(OnReqFarmBuyMsg*)cMsg;

	// �̹� ������ ��� ������ �ִ��� �˻��Ѵ�.
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
//	����	:	���� �ǹ��� �Ⱦ� ������.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���������� ���´�.(���忡 ���鼭 ���� ������ �����ٶ�...)
//	���� ����	:	2002-10-10 ���� 2:06:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ǹ��� ���´�.
//	���� ����	:	2002-09-23 ���� 3:13:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//	����	:	������ ������ Ȯ���Ѵ�.
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
//	����	:	������ ������ Ȯ���Ѵ�.
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
//	����	:	���� �ǹ��� �����Ѵ�.
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
//	����	:	�ü����� ��ġ �Ѵ�.(�ǹ� ���׷��̵�)
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
//	����	:	�������� �����Ѵ�.(��ȹ�� �����.)
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���� ���� �Ѵ�.
//	���� ����	:	2002-10-11 ���� 4:13:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �����ƴ�.
//	���� ����	:	2002-07-22 ���� 2:00:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
		// ����Ʈ�� ���� ���� �˷��ش�.
		Quest_AddQuestObject_Work( pFarmWorkMsg->siLabor );
	}


END:
	if( bSuccess == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_FARM_WORK, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�۾��� ��ŷ ����͸� �α�
//	���� ����	:	2003-07-21 ���� 11:54 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvFarmWorkHack()
{	
	OnFarmWorkHack*		pRecvMsg;

	pRecvMsg	=	(OnFarmWorkHack*)cMsg;

//	pMyWorld->clServer->WriteInfo( "..\\RecvFarmWorkHack.txt", "DBAccount(%d)  szID(%s)  uiType(%d)  siStep(%d)  siPoint1(%d)  siPoint2(%d)",
//									stAccountInfo.siDBAccount, szID, pRecvMsg->uiType, pRecvMsg->siStep, pRecvMsg->siPoint1, pRecvMsg->siPoint2 );
	writeInfoToFile( "RecvFarmWorkHack.txt", "DBAccount(%d)  szID(%s)  uiType(%d)  siStep(%d)  siPoint1(%d)  siPoint2(%d)",
									stAccountInfo.siDBAccount, szID, pRecvMsg->uiType, pRecvMsg->siStep, pRecvMsg->siPoint1, pRecvMsg->siPoint2 );

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�۾����� üũ�Ͽ� �����̸� TRUE, ��ŷ�� ����� ������ FALSE.. �ϵ��ڵ��� ���� -_-;
//	���� ����	:	2003-07-25 ���� 6:35 - �����, 2004�� 11�� 13�� �߰� ��ĥ �Ӱ���~
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ī�� ¦ ���߱� �Ϸ� üũ..
//	���� ����	:	2003-07-25 ���� 6:36 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack1( SI08 siStep, UI32 uiQuantity )
{
	const UI32	uiCard[17]	=	{ 0, 12, 12, 16, 16, 18, 18, 24, 24, 30, 30, 24, 24, 32, 32, 40, 40 };	// 16�ܰ�

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 3 * uiCard[i] / 2;
	}
	UI32	uiMaxQuantity	=	uiPoint / 30;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �Ϸ� üũ..
//	���� ����	:	2003-07-25 ���� 6:36 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack2( SI08 siStep, UI32 uiQuantity )
{
	const UI32 uiCard[19] = { 0, 12, 12, 18, 18, 20, 20, 24, 24, 30, 30, 40, 40, 42, 42, 48, 48, 56, 56 };	// 18�ܰ�

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 4 * uiCard[i] / 3;
	}
	UI32	uiMaxQuantity	=	uiPoint / 75;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ޱ� �Ϸ� üũ.. �ϵ� �ڵ��� ���� 2ź -_-;;
//	���� ����	:	2003-07-25 ���� 6:40 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack3( SI08 siStep, UI32 uiQuantity )
{
	const UI32	uiNumFruit[11]	=	{ 0, 50, 50, 60, 60, 70, 70, 80, 80, 90, 90 };	// 10�ܰ�

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ī�� ¦ ���߱� �Ϸ� üũ..
//	���� ����	:	2003-07-25 ���� 6:36 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack1( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD1 17
	const UI32	uiCard[MAX_CARD1]	=	{ 0, 12, 12, 16, 16, 18, 18, 24, 24, 30, 30, 24, 24, 32, 32, 40, 40 };	// 16�ܰ�

	if(siStep >= MAX_CARD1) return(FALSE);

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 3 * uiCard[i] / 2;
	}
	UI32	uiMaxQuantity	=	uiPoint / 30;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �Ϸ� üũ..
//	���� ����	:	2003-07-25 ���� 6:36 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack2( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD2 19
	const UI32 uiCard[MAX_CARD2] = { 0, 12, 12, 18, 18, 20, 20, 24, 24, 30, 30, 40, 40, 42, 42, 48, 48, 56, 56 };	// 18�ܰ�

	if(siStep >= MAX_CARD2) return(FALSE);

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += uiCard[i] * 4 * uiCard[i] / 3;
	}
	UI32	uiMaxQuantity	=	uiPoint / 75;

	if( uiQuantity > uiMaxQuantity )	return FALSE;	// Hacked
	else								return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �ޱ� �Ϸ� üũ.. �ϵ� �ڵ��� ���� 2ź -_-;;
//	���� ����	:	2003-07-25 ���� 6:40 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL SOPlayer::CheckLaborHack3( SI08 siStep, UI32 uiQuantity )
{
#define MAX_CARD3 11
	const UI32	uiNumFruit[MAX_CARD3]	=	{ 0, 50, 50, 60, 60, 70, 70, 80, 80, 90, 90 };	// 10�ܰ�

	if(siStep >= MAX_CARD3) return(FALSE);

	// siStep �ܰ������ ���� ����Ʈ.. 0�ܰ�� 0
	UI32 uiPoint = 0;
	for( SI32 i = 1; i <= siStep; i++ )
	{
		uiPoint += (i + 1) * uiNumFruit[i] + uiNumFruit[i] * (uiNumFruit[i] + 1 ) / 6;
	}
	UI32 uiMaxQuantity = uiPoint / 50;

	if( uiQuantity > uiMaxQuantity )	return FALSE;		// Hacked
	else								return TRUE;
}




//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ ��û�ߴ�.
//	���� ����	:	2002-10-08 ���� 2:15:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	â�� �ִ� �������� ������.
//	���� ����	:	2002-10-08 ���� 3:13:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
//	����	:	â�� �ִ� �������� �ִ´�.
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

	// �ش� �뺴�� �������� ������ �ִ��� �˻��Ѵ�.
	if( IFollower.HasItem( pStorageInputItemMsg->uiFollowerID, pStorageInputItemMsg->uiItemCode, UI16( pStorageInputItemMsg->uiQunatity ) ) == FALSE )	goto	END;

	InvenSlot = IFollower.GetItemIndex(pStorageInputItemMsg->uiFollowerID, pStorageInputItemMsg->uiItemCode);

	

	if( stAccountInfo.pVillage->StorageInputItem( this, pStorageInputItemMsg->uiItemCode, pStorageInputItemMsg->uiQunatity ) == FALSE )						goto	END;

	// �־��� �ڵ����!
	// db query�� �������� �������� ����� ������ ���´�.
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
	
	// �ش� �뺴�� �������� ����.
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
//	����	:	â�� �������� ������.
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

	// �ش� �뺴���� �������� ���� �� �ִ��� �˻��Ѵ�.
	if( IFollower.CanAddItem( pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode, UI16( pStorageOutputItemMsg->uiQunatity ) ) == FALSE )	goto	END;

	

	// �־��� �ڵ� �� ����!
	// �̹����� �������� ���������� �̸� ��ġ�� Ȯ���Ѵ�.
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

	// �ش� �뺴�� �������� ���Ѵ�.
	AddItemNoQuery( pStorageOutputItemMsg->uiFollowerID, pStorageOutputItemMsg->uiItemCode, pStorageOutputItemMsg->uiQunatity );

	// �������� �ְ��� �� ��ġ�� Ȯ���Ѵ�.
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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ӱ��� �ٲ۴�.
//	���� ����	:	2002-07-24 ���� 11:37:15 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	
	// �ӱ��� �ٲٸ鼭 ��� ������ �����ش�.
	if( bSuccess == FALSE )
	{
		//SendFieldMsg(ON_RESPONSE_PLANT_CHANGE_PAY, SO_SFM_ONLYME, LPVOID(dwResult),LPVOID(&siMoney));
		SendFieldMsg(ON_RESPONSE_PLANT_CHANGE_PAY, SO_SFM_ONLYME, LPVOID( ON_RET_NO ) );
	}

	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�۾��� ����Ѵ�.
//	���� ����	:	2002-10-12 ���� 2:36:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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


