#include "SOMain.h"
#include "SODatabase.h"
#include "OnlineEconomy.h"
#include "SOPlayer.h"
#include "SOVillage_BankStructure.h"

SODatabase		*SOVillage_Bank::m_pDataBase;
OnlineEconomy	*SOVillage_Bank::m_pOnlineEconomy;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.		
//	熱薑 橾濠	:	2002-10-17 螃�� 1:07:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Bank::InitStaticVariable( SODatabase *pDataBase, OnlineEconomy *pOnlineEconomy )
{
	m_pDataBase			=	pDataBase;
	m_pOnlineEconomy	=	pOnlineEconomy;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-10-18 螃�� 5:01:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Bank::Init( UI16 uiVillageCode )
{
	DBIKBankInfo	BankInfo;
		
	SOVillage_BaseStructure::Init( uiVillageCode, 1000 );	
	m_uiVillageCode		=	uiVillageCode;

	BankInfo.uiVillageCode = uiVillageCode;

	// DB縑憮 瞪濰曖 薑爾蒂 橢橫螞棻.
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_BANKINFO, LPVOID( &BankInfo ), NULL ) == FALSE)			return FALSE;

	
	m_fInterest			=	BankInfo.siInterest;
	m_mnBankCapital		=	BankInfo.siBankCapital;
	m_mnTradeCapital	=	BankInfo.siTradeCapital;
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-10-10 螃�� 4:24:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Bank::SOVillage_Bank()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-10-10 螃�� 4:24:30 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_Bank::~SOVillage_Bank()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪濰曖 薑爾蒂 爾頂遽棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 4:24:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_Bank::SendBankInfo( SOPlayer *pPlayer )
{
	// 檜徽, 擎ч 濤擋, 頂 絲擊 爾頂遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID( SI16 ( m_fInterest ) ), LPVOID( &m_mnBankCapital ) );		
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鱔濰擊 偃撲и棻.
//	熱薑 橾濠	:	2002-10-10 螃�� 4:28:57 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage_Bank::OpenSavingAccount( SOPlayer *pPlayer, UI16 uiKind, MONEY mnDeposit )
{
	DBIKOpenSavingAccountInfo		DBOpenSavingAccountInfo;
	DBIKRetOpenSavingAccount		DBRetOpenSavingAccount;
	EconomyHeader					*pEconomyHeader;
	OnAccountNumber					acnAccountNumber;

	ZeroMemory( &acnAccountNumber, sizeof( acnAccountNumber ) );

	// 啗謝曖 薑爾蒂 橢橫螞棻.
	if( ( pEconomyHeader = m_pOnlineEconomy->GetHeader( uiKind - 1 ) ) == NULL )	return	ON_RET_BANK_NO;

	// DB縑 鱔濰擊 偃撲и棻.	
	DBOpenSavingAccountInfo.uiVillageCode				=	m_uiVillageCode;	
	DBOpenSavingAccountInfo.uiCharacterDBAccount		=	pPlayer->stAccountInfo.siDBAccount;
	DBOpenSavingAccountInfo.uiCharacterSlot				=	pPlayer->stAccountInfo.uiSlot;
	DBOpenSavingAccountInfo.uiAccountKind				=	uiKind;

	// 檜濠蒂 撲薑и棻.
	if( pEconomyHeader->fFixedInterest != 0 )		DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFixedInterest );
	else											DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFlexInterest + m_fInterest );

	// 渠轎 鱔濰檜塭賊 蟾晦 殮旎擋擊 - 衛鑑棻.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	DBOpenSavingAccountInfo.siFirstDepositMoney	=	-mnDeposit;
	else												DBOpenSavingAccountInfo.siFirstDepositMoney	=	mnDeposit;
	
	
	DBRetOpenSavingAccount.pAccountNumber				=	&acnAccountNumber;	
	DBRetOpenSavingAccount.siErrorRet					=	ON_RET_BANK_OK;

	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_OPENSAVINGACCOUNT, LPVOID( &DBOpenSavingAccountInfo ), LPVOID( &DBRetOpenSavingAccount ) ) == FALSE )	
		return	ON_RET_BANK_NO;
	
	if( DBRetOpenSavingAccount.siErrorRet != ON_RET_BANK_OK )	return	DBRetOpenSavingAccount.siErrorRet;


	// 撩奢瞳戲煎 啗謝蒂 虜菟歷棻.
	// 餌辨濠曖 絲擊 隸陛 塽 馬模 衛鑑棻.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	pPlayer->IncreaseMoney( mnDeposit, FALSE );		// 渠轎擊 嫡懊戲棲 模雖旎擊 隸陛 衛麵 遽棻.
	else												pPlayer->DecreaseMoney( mnDeposit, FALSE );		// 瞪濰縑 蜆啣戲棲 模雖旎擊 馬模 衛麵 遽棻.

	// 餌辨濠縑啪 撩奢ц棻堅 詭衛雖蒂 爾頂遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &acnAccountNumber ) );

	return	ON_RET_OK;

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	殮旎и棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 3:18:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Bank::Deposit( SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{	
	MONEY						mnBalance;						//鱔濰曖 絲.

	// 擎ч縑 殮旎и棻.
	mnBalance			=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DEPOSIT, LPVOID( pAccountNumber ), LPVOID( &mnBalance ) ) == FALSE )		return	FALSE;
	
	// ⑷營 頂絲擊 梗堅 葆擊 絲擊 棺萼棻.
	if( pPlayer->DecreaseMoney( mnMoney, FALSE ) == FALSE )																return	FALSE;
	//IncreaseCapital(MONEY( pReqBankDepositMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);
	
	// 唸婁蒂 爾頂遽棻.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &mnBalance ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	轎旎и棻.
//	熱薑 橾濠	:	2002-10-18 螃�� 3:24:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_Bank::Drawing(SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{
	MONEY						siBalance;		

	// 擎ч縑憮 轎旎и棻.
	siBalance	=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DRAWING, LPVOID( pAccountNumber ), LPVOID( &siBalance ) ) == FALSE)	return	FALSE;
		
	// 絲擊 撩奢瞳戲煎 轎旎ц棻. 
	// 頂 絲擊 渦ж堅 葆擊 絲擊 還檣棻.
	pPlayer->IncreaseMoney( mnMoney, FALSE );
	//DecreaseBankCapital(MONEY(pReqBankDrawingMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);

	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &siBalance ), LPVOID( &pPlayer->siMoney ) );
	
	return	TRUE;
}
