#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"
#include "SODefine.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啗謝 偃撲擊 蹂羶и棻.(葆擊曖 絲檜 棺橫釭朝 匙 蹺陛 諫猿)
//	熱薑 橾濠	:	2002-10-18 螃�� 3:45:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage::OpenSavingAccount(SOPlayer *pPlayer, OnReqBankOpenSavingAccountMsg *pReqOpenSavingAccountMsg, OnAccountNumber *pAccountNum)
{
	// 葆擊縑 擎ч檜 氈朝雖 匐餌и棻.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->OpenSavingAccount( pPlayer, pReqOpenSavingAccountMsg->uiKind, pReqOpenSavingAccountMsg->siFirstDepositMoney );
		

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	啗謝縑 絲擊 盪旎и棻. (葆擊曖 絲檜 棺橫釭朝 匙 蹺陛 諫猿)
//	熱薑 橾濠	:	2002-10-18 螃�� 4:29:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::BankDeposit(SOPlayer *pPlayer, OnReqBankDepositMsg *pReqBankDepositMsg)
{
	// 葆擊縑 擎ч檜 氈朝雖 匐餌и棻.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Deposit( pPlayer, &pReqBankDepositMsg->AccountNumber, pReqBankDepositMsg->siMoney );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	啗謝縑憮 絲擊 瓊朝棻. (葆擊曖 絲檜 還橫萄朝 匙 蹺陛 諫猿)
//	熱薑 橾濠	:	2002-10-18 螃�� 4:31:31 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage::BankDrawing( SOPlayer *pPlayer, OnReqBankDrawingMsg *pReqBankDrawingMsg )
{
	// 葆擊縑 擎ч檜 氈朝雖 匐餌и棻.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Drawing( pPlayer, &pReqBankDrawingMsg->AccountNumber, pReqBankDrawingMsg->siMoney );

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 濠獄擊 隸陛衛鑑棻.
//	熱薑 橾濠	:	2002-11-13 螃�� 5:36:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// DB縑 撲薑и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	葆擊曖 濠獄擊 賓棻.
//	熱薑 橾濠	:	2002-11-13 螃�� 5:37:48 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::DecreaseCapital( MONEY siDecreaseMoney, SI32 nOption, BOOL bSendToDB )
{
	DBIKVillageProfit				VillageProfit;

	if(CurrentCapital->Profit[nOption] > siDecreaseMoney)
		CurrentCapital->Profit[nOption]		-=	siDecreaseMoney;
	else
		CurrentCapital->Profit[nOption]		=	0;

	// DB縑 撲薑и棻.
	if( bSendToDB == TRUE )
	{
		VillageProfit.VillageCode		=	uiVillageCode;
		VillageProfit.Year				=	SI16(clWorld->CurrentTime.Year);
		VillageProfit.Profit			=	CurrentCapital->Profit[nOption];
		VillageProfit.Option			=	nOption;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVECAPITAL, &VillageProfit, sizeof(DBIKVillageProfit));
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濛撩濠 	:	檜 撩 營 
//	撲貲	:	葆擊曖 旎懦 濠獄擊 DB縑憮 啗骯ж紫煙 и棻.(瞪羹 旎懦 濠獄曖 喇5% - 衙殖 褒ч)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::CalculateBankCapital()
{	
	// DB縑 撲薑и棻.
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKCAPITAL_CALCULATE, this, NULL);
}

/*
-- 唳衙 DataBase --
僭勒嬴檜蠱, 僭勒偃熱, 葆擊id, 籀擠陛問, 唳衙陽梱雖陴擎陳(唳衙陳)
*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫縑啪 瞪濰曖 薑爾蒂 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 5:21:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage::SendBankInfo( SOPlayer *pPlayer )
{
	if( DoesBankExist() == FALSE )								return;

	m_pBank->SendBankInfo( pPlayer );
}
