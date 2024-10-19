#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"
#include "SODefine.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	°èÁÂ °³¼³À» ¿äÃ»ÇÑ´Ù.(¸¶À»ÀÇ µ·ÀÌ ´Ã¾î³ª´Â °Í Ãß°¡ ¿Ï·á)
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 3:45:08 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	SOVillage::OpenSavingAccount(SOPlayer *pPlayer, OnReqBankOpenSavingAccountMsg *pReqOpenSavingAccountMsg, OnAccountNumber *pAccountNum)
{
	// ¸¶À»¿¡ ÀºÇàÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->OpenSavingAccount( pPlayer, pReqOpenSavingAccountMsg->uiKind, pReqOpenSavingAccountMsg->siFirstDepositMoney );
		

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	°èÁÂ¿¡ µ·À» Àú±ÝÇÑ´Ù. (¸¶À»ÀÇ µ·ÀÌ ´Ã¾î³ª´Â °Í Ãß°¡ ¿Ï·á)
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 4:29:52 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::BankDeposit(SOPlayer *pPlayer, OnReqBankDepositMsg *pReqBankDepositMsg)
{
	// ¸¶À»¿¡ ÀºÇàÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Deposit( pPlayer, &pReqBankDepositMsg->AccountNumber, pReqBankDepositMsg->siMoney );

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	°èÁÂ¿¡¼­ µ·À» Ã£´Â´Ù. (¸¶À»ÀÇ µ·ÀÌ ÁÙ¾îµå´Â °Í Ãß°¡ ¿Ï·á)
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 4:31:31 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage::BankDrawing( SOPlayer *pPlayer, OnReqBankDrawingMsg *pReqBankDrawingMsg )
{
	// ¸¶À»¿¡ ÀºÇàÀÌ ÀÖ´ÂÁö °Ë»çÇÑ´Ù.
	if( DoesBankExist() == FALSE )							return	ON_RET_BANK_NO;

	return	m_pBank->Drawing( pPlayer, &pReqBankDrawingMsg->AccountNumber, pReqBankDrawingMsg->siMoney );

}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¸¶À»ÀÇ ÀÚº»À» Áõ°¡½ÃÅ²´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-11-13 ¿ÀÈÄ 5:36:57 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// DB¿¡ ¼³Á¤ÇÑ´Ù.
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¸¶À»ÀÇ ÀÚº»À» »«´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-11-13 ¿ÀÈÄ 5:37:48 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOVillage::DecreaseCapital( MONEY siDecreaseMoney, SI32 nOption, BOOL bSendToDB )
{
	DBIKVillageProfit				VillageProfit;

	if(CurrentCapital->Profit[nOption] > siDecreaseMoney)
		CurrentCapital->Profit[nOption]		-=	siDecreaseMoney;
	else
		CurrentCapital->Profit[nOption]		=	0;

	// DB¿¡ ¼³Á¤ÇÑ´Ù.
	if( bSendToDB == TRUE )
	{
		VillageProfit.VillageCode		=	uiVillageCode;
		VillageProfit.Year				=	SI16(clWorld->CurrentTime.Year);
		VillageProfit.Profit			=	CurrentCapital->Profit[nOption];
		VillageProfit.Option			=	nOption;	
		
		clWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_SAVECAPITAL, &VillageProfit, sizeof(DBIKVillageProfit));
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀÛ¼ºÀÚ 	:	ÀÌ ¼º Àç 
//	¼³¸í	:	¸¶À»ÀÇ ±ÝÀ¶ ÀÚº»À» DB¿¡¼­ °è»êÇÏµµ·Ï ÇÑ´Ù.(ÀüÃ¼ ±ÝÀ¶ ÀÚº»ÀÇ ³â5% - ¸Å´Þ ½ÇÇà)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOVillage::CalculateBankCapital()
{	
	// DB¿¡ ¼³Á¤ÇÑ´Ù.
	clWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_BANKCAPITAL_CALCULATE, this, NULL);
}

/*
-- °æ¸Å DataBase --
¹°°Ç¾ÆÀÌÅÛ, ¹°°Ç°³¼ö, ¸¶À»id, Ã³À½°¡°Ý, °æ¸Å¶§±îÁö³²Àº³¯(°æ¸Å³¯)
*/

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÇÃ·¹ÀÌ¾î¿¡°Ô ÀüÀåÀÇ Á¤º¸¸¦ º¸³»ÁØ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 5:21:57 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOVillage::SendBankInfo( SOPlayer *pPlayer )
{
	if( DoesBankExist() == FALSE )								return;

	m_pBank->SendBankInfo( pPlayer );
}
