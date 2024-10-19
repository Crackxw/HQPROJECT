#include "SOMain.h"
#include "SODatabase.h"
#include "OnlineEconomy.h"
#include "SOPlayer.h"
#include "SOVillage_BankStructure.h"

SODatabase		*SOVillage_Bank::m_pDataBase;
OnlineEconomy	*SOVillage_Bank::m_pOnlineEconomy;

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Á¤Àû º¯¼ö¸¦ ÃÊ±âÈ­ ÇÑ´Ù.		
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-17 ¿ÀÈÄ 1:07:08 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOVillage_Bank::InitStaticVariable( SODatabase *pDataBase, OnlineEconomy *pOnlineEconomy )
{
	m_pDataBase			=	pDataBase;
	m_pOnlineEconomy	=	pOnlineEconomy;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÃÊ±âÈ­.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 5:01:10 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage_Bank::Init( UI16 uiVillageCode )
{
	DBIKBankInfo	BankInfo;
		
	SOVillage_BaseStructure::Init( uiVillageCode, 1000 );	
	m_uiVillageCode		=	uiVillageCode;

	BankInfo.uiVillageCode = uiVillageCode;

	// DB¿¡¼­ ÀüÀåÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_BANKINFO, LPVOID( &BankInfo ), NULL ) == FALSE)			return FALSE;

	
	m_fInterest			=	BankInfo.siInterest;
	m_mnBankCapital		=	BankInfo.siBankCapital;
	m_mnTradeCapital	=	BankInfo.siTradeCapital;
	
	return	TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	»ý¼ºÀÚ.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 4:24:22 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SOVillage_Bank::SOVillage_Bank()
{

}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¼Ò¸êÀÚ.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 4:24:30 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SOVillage_Bank::~SOVillage_Bank()
{
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀüÀåÀÇ Á¤º¸¸¦ º¸³»ÁØ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 4:24:13 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOVillage_Bank::SendBankInfo( SOPlayer *pPlayer )
{
	// ÀÌÀ², ÀºÇà ÀÜ¾×, ³» µ·À» º¸³»ÁØ´Ù.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID( SI16 ( m_fInterest ) ), LPVOID( &m_mnBankCapital ) );		
	
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÅëÀåÀ» °³¼³ÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-10 ¿ÀÈÄ 4:28:57 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	SOVillage_Bank::OpenSavingAccount( SOPlayer *pPlayer, UI16 uiKind, MONEY mnDeposit )
{
	DBIKOpenSavingAccountInfo		DBOpenSavingAccountInfo;
	DBIKRetOpenSavingAccount		DBRetOpenSavingAccount;
	EconomyHeader					*pEconomyHeader;
	OnAccountNumber					acnAccountNumber;

	ZeroMemory( &acnAccountNumber, sizeof( acnAccountNumber ) );

	// °èÁÂÀÇ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
	if( ( pEconomyHeader = m_pOnlineEconomy->GetHeader( uiKind - 1 ) ) == NULL )	return	ON_RET_BANK_NO;

	// DB¿¡ ÅëÀåÀ» °³¼³ÇÑ´Ù.	
	DBOpenSavingAccountInfo.uiVillageCode				=	m_uiVillageCode;	
	DBOpenSavingAccountInfo.uiCharacterDBAccount		=	pPlayer->stAccountInfo.siDBAccount;
	DBOpenSavingAccountInfo.uiCharacterSlot				=	pPlayer->stAccountInfo.uiSlot;
	DBOpenSavingAccountInfo.uiAccountKind				=	uiKind;

	// ÀÌÀÚ¸¦ ¼³Á¤ÇÑ´Ù.
	if( pEconomyHeader->fFixedInterest != 0 )		DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFixedInterest );
	else											DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFlexInterest + m_fInterest );

	// ´ëÃâ ÅëÀåÀÌ¶ó¸é ÃÊ±â ÀÔ±Ý¾×À» - ½ÃÅ²´Ù.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	DBOpenSavingAccountInfo.siFirstDepositMoney	=	-mnDeposit;
	else												DBOpenSavingAccountInfo.siFirstDepositMoney	=	mnDeposit;
	
	
	DBRetOpenSavingAccount.pAccountNumber				=	&acnAccountNumber;	
	DBRetOpenSavingAccount.siErrorRet					=	ON_RET_BANK_OK;

	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_OPENSAVINGACCOUNT, LPVOID( &DBOpenSavingAccountInfo ), LPVOID( &DBRetOpenSavingAccount ) ) == FALSE )	
		return	ON_RET_BANK_NO;
	
	if( DBRetOpenSavingAccount.siErrorRet != ON_RET_BANK_OK )	return	DBRetOpenSavingAccount.siErrorRet;


	// ¼º°øÀûÀ¸·Î °èÁÂ¸¦ ¸¸µé¾ú´Ù.
	// »ç¿ëÀÚÀÇ µ·À» Áõ°¡ ¹× °¨¼Ò ½ÃÅ²´Ù.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	pPlayer->IncreaseMoney( mnDeposit, FALSE );		// ´ëÃâÀ» ¹Þ¾ÒÀ¸´Ï ¼ÒÁö±ÝÀ» Áõ°¡ ½ÃÄÑ ÁØ´Ù.
	else												pPlayer->DecreaseMoney( mnDeposit, FALSE );		// ÀüÀå¿¡ ¸Ã°åÀ¸´Ï ¼ÒÁö±ÝÀ» °¨¼Ò ½ÃÄÑ ÁØ´Ù.

	// »ç¿ëÀÚ¿¡°Ô ¼º°øÇß´Ù°í ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &acnAccountNumber ) );

	return	ON_RET_OK;

}



//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀÔ±ÝÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 3:18:17 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage_Bank::Deposit( SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{	
	MONEY						mnBalance;						//ÅëÀåÀÇ µ·.

	// ÀºÇà¿¡ ÀÔ±ÝÇÑ´Ù.
	mnBalance			=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DEPOSIT, LPVOID( pAccountNumber ), LPVOID( &mnBalance ) ) == FALSE )		return	FALSE;
	
	// ÇöÀç ³»µ·À» ±ð°í ¸¶À» µ·À» ´Ã¸°´Ù.
	if( pPlayer->DecreaseMoney( mnMoney, FALSE ) == FALSE )																return	FALSE;
	//IncreaseCapital(MONEY( pReqBankDepositMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);
	
	// °á°ú¸¦ º¸³»ÁØ´Ù.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &mnBalance ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Ãâ±ÝÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-10-18 ¿ÀÈÄ 3:24:18 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOVillage_Bank::Drawing(SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{
	MONEY						siBalance;		

	// ÀºÇà¿¡¼­ Ãâ±ÝÇÑ´Ù.
	siBalance	=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DRAWING, LPVOID( pAccountNumber ), LPVOID( &siBalance ) ) == FALSE)	return	FALSE;
		
	// µ·À» ¼º°øÀûÀ¸·Î Ãâ±ÝÇß´Ù. 
	// ³» µ·À» ´õÇÏ°í ¸¶À» µ·À» ÁÙÀÎ´Ù.
	pPlayer->IncreaseMoney( mnMoney, FALSE );
	//DecreaseBankCapital(MONEY(pReqBankDrawingMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);

	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &siBalance ), LPVOID( &pPlayer->siMoney ) );
	
	return	TRUE;
}
