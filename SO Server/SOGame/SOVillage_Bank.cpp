#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"
#include "SODefine.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ��û�Ѵ�.(������ ���� �þ�� �� �߰� �Ϸ�)
//	���� ����	:	2002-10-18 ���� 3:45:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage::OpenSavingAccount(SOPlayer *pPlayer, OnReqBankOpenSavingAccountMsg *pReqOpenSavingAccountMsg, OnAccountNumber *pAccountNum)
{
	// ������ ������ �ִ��� �˻��Ѵ�.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->OpenSavingAccount( pPlayer, pReqOpenSavingAccountMsg->uiKind, pReqOpenSavingAccountMsg->siFirstDepositMoney );
		

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���¿� ���� �����Ѵ�. (������ ���� �þ�� �� �߰� �Ϸ�)
//	���� ����	:	2002-10-18 ���� 4:29:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::BankDeposit(SOPlayer *pPlayer, OnReqBankDepositMsg *pReqBankDepositMsg)
{
	// ������ ������ �ִ��� �˻��Ѵ�.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Deposit( pPlayer, &pReqBankDepositMsg->AccountNumber, pReqBankDepositMsg->siMoney );

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���¿��� ���� ã�´�. (������ ���� �پ��� �� �߰� �Ϸ�)
//	���� ����	:	2002-10-18 ���� 4:31:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage::BankDrawing( SOPlayer *pPlayer, OnReqBankDrawingMsg *pReqBankDrawingMsg )
{
	// ������ ������ �ִ��� �˻��Ѵ�.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Drawing( pPlayer, &pReqBankDrawingMsg->AccountNumber, pReqBankDrawingMsg->siMoney );

}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ں��� ������Ų��.
//	���� ����	:	2002-11-13 ���� 5:36:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
MONEY	SOVillage::IncreaseCapital( MONEY siIncreaseMoney, SI32 nOption, BOOL bSendToDB )
{
	DBIKVillageProfit				VillageProfit;
	MONEY							mnPrevCapital;	

	mnPrevCapital	=	CurrentCapital->Profit[ nOption ];

	if( ( ON_MAX_MONEY - CurrentCapital->Profit[ nOption ] ) >=  siIncreaseMoney)
	{
		CurrentCapital->Profit[ nOption ]		+=	siIncreaseMoney;		
	}
	else
	{
		CurrentCapital->Profit[ nOption ]		=	ON_MAX_MONEY;
		
	}

	// DB�� �����Ѵ�.
	if( bSendToDB == TRUE )
	{
		VillageProfit.VillageCode		=	uiVillageCode;
		VillageProfit.Year				=	SI16(clWorld->CurrentTime.Year);
		VillageProfit.Profit			=	CurrentCapital->Profit[nOption];
		VillageProfit.Option			=	nOption;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVECAPITAL, &VillageProfit, sizeof(DBIKVillageProfit));
	}

	return	MONEY( CurrentCapital->Profit[ nOption ] - mnPrevCapital );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �ں��� ����.
//	���� ����	:	2002-11-13 ���� 5:37:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::DecreaseCapital( MONEY siDecreaseMoney, SI32 nOption, BOOL bSendToDB )
{
	DBIKVillageProfit				VillageProfit;

	if(CurrentCapital->Profit[nOption] > siDecreaseMoney)
		CurrentCapital->Profit[nOption]		-=	siDecreaseMoney;
	else
		CurrentCapital->Profit[nOption]		=	0;

	// DB�� �����Ѵ�.
	if( bSendToDB == TRUE )
	{
		VillageProfit.VillageCode		=	uiVillageCode;
		VillageProfit.Year				=	SI16(clWorld->CurrentTime.Year);
		VillageProfit.Profit			=	CurrentCapital->Profit[nOption];
		VillageProfit.Option			=	nOption;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVECAPITAL, &VillageProfit, sizeof(DBIKVillageProfit));
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	�ۼ��� 	:	�� �� �� 
//	����	:	������ ���� �ں��� DB���� ����ϵ��� �Ѵ�.(��ü ���� �ں��� ��5% - �Ŵ� ����)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::CalculateBankCapital()
{	
	// DB�� �����Ѵ�.
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKCAPITAL_CALCULATE, this, NULL);
}

/*
-- ��� DataBase --
���Ǿ�����, ���ǰ���, ����id, ó������, ��Ŷ�����������(��ų�)
*/

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�÷��̾�� ������ ������ �����ش�.
//	���� ����	:	2002-10-18 ���� 5:21:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage::SendBankInfo( SOPlayer *pPlayer )
{
	if( DoesBankExist() == FALSE )								return;

	m_pBank->SendBankInfo( pPlayer );
}
