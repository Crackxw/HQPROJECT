#include "SOMain.h"
#include "SODatabase.h"
#include "OnlineEconomy.h"
#include "SOPlayer.h"
#include "SOVillage_BankStructure.h"

SODatabase		*SOVillage_Bank::m_pDataBase;
OnlineEconomy	*SOVillage_Bank::m_pOnlineEconomy;

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʱ�ȭ �Ѵ�.		
//	���� ����	:	2002-10-17 ���� 1:07:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Bank::InitStaticVariable( SODatabase *pDataBase, OnlineEconomy *pOnlineEconomy )
{
	m_pDataBase			=	pDataBase;
	m_pOnlineEconomy	=	pOnlineEconomy;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-10-18 ���� 5:01:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Bank::Init( UI16 uiVillageCode )
{
	DBIKBankInfo	BankInfo;
		
	SOVillage_BaseStructure::Init( uiVillageCode, 1000 );	
	m_uiVillageCode		=	uiVillageCode;

	BankInfo.uiVillageCode = uiVillageCode;

	// DB���� ������ ������ ���´�.
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_BANKINFO, LPVOID( &BankInfo ), NULL ) == FALSE)			return FALSE;

	
	m_fInterest			=	BankInfo.siInterest;
	m_mnBankCapital		=	BankInfo.siBankCapital;
	m_mnTradeCapital	=	BankInfo.siTradeCapital;
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-10-10 ���� 4:24:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Bank::SOVillage_Bank()
{

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-10-10 ���� 4:24:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SOVillage_Bank::~SOVillage_Bank()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ������ �����ش�.
//	���� ����	:	2002-10-10 ���� 4:24:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOVillage_Bank::SendBankInfo( SOPlayer *pPlayer )
{
	// ����, ���� �ܾ�, �� ���� �����ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_INFO, SO_SFM_ONLYME, LPVOID( SI16 ( m_fInterest ) ), LPVOID( &m_mnBankCapital ) );		
	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �����Ѵ�.
//	���� ����	:	2002-10-10 ���� 4:28:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	SOVillage_Bank::OpenSavingAccount( SOPlayer *pPlayer, UI16 uiKind, MONEY mnDeposit )
{
	DBIKOpenSavingAccountInfo		DBOpenSavingAccountInfo;
	DBIKRetOpenSavingAccount		DBRetOpenSavingAccount;
	EconomyHeader					*pEconomyHeader;
	OnAccountNumber					acnAccountNumber;

	ZeroMemory( &acnAccountNumber, sizeof( acnAccountNumber ) );

	// ������ ������ ���´�.
	if( ( pEconomyHeader = m_pOnlineEconomy->GetHeader( uiKind - 1 ) ) == NULL )	return	ON_RET_BANK_NO;

	// DB�� ������ �����Ѵ�.	
	DBOpenSavingAccountInfo.uiVillageCode				=	m_uiVillageCode;	
	DBOpenSavingAccountInfo.uiCharacterDBAccount		=	pPlayer->stAccountInfo.siDBAccount;
	DBOpenSavingAccountInfo.uiCharacterSlot				=	pPlayer->stAccountInfo.uiSlot;
	DBOpenSavingAccountInfo.uiAccountKind				=	uiKind;

	// ���ڸ� �����Ѵ�.
	if( pEconomyHeader->fFixedInterest != 0 )		DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFixedInterest );
	else											DBOpenSavingAccountInfo.siInterest	=	SI16( pEconomyHeader->fFlexInterest + m_fInterest );

	// ���� �����̶�� �ʱ� �Աݾ��� - ��Ų��.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	DBOpenSavingAccountInfo.siFirstDepositMoney	=	-mnDeposit;
	else												DBOpenSavingAccountInfo.siFirstDepositMoney	=	mnDeposit;
	
	
	DBRetOpenSavingAccount.pAccountNumber				=	&acnAccountNumber;	
	DBRetOpenSavingAccount.siErrorRet					=	ON_RET_BANK_OK;

	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_OPENSAVINGACCOUNT, LPVOID( &DBOpenSavingAccountInfo ), LPVOID( &DBRetOpenSavingAccount ) ) == FALSE )	
		return	ON_RET_BANK_NO;
	
	if( DBRetOpenSavingAccount.siErrorRet != ON_RET_BANK_OK )	return	DBRetOpenSavingAccount.siErrorRet;


	// ���������� ���¸� �������.
	// ������� ���� ���� �� ���� ��Ų��.
	if( pEconomyHeader->siType == ON_BANK_LOAN_TYPE )	pPlayer->IncreaseMoney( mnDeposit, FALSE );		// ������ �޾����� �������� ���� ���� �ش�.
	else												pPlayer->DecreaseMoney( mnDeposit, FALSE );		// ���忡 �ð����� �������� ���� ���� �ش�.

	// ����ڿ��� �����ߴٰ� �޽����� �����ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_OPENSAVINGACCOUNT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &acnAccountNumber ) );

	return	ON_RET_OK;

}



//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ա��Ѵ�.
//	���� ����	:	2002-10-18 ���� 3:18:17 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Bank::Deposit( SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{	
	MONEY						mnBalance;						//������ ��.

	// ���࿡ �Ա��Ѵ�.
	mnBalance			=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DEPOSIT, LPVOID( pAccountNumber ), LPVOID( &mnBalance ) ) == FALSE )		return	FALSE;
	
	// ���� ������ ��� ���� ���� �ø���.
	if( pPlayer->DecreaseMoney( mnMoney, FALSE ) == FALSE )																return	FALSE;
	//IncreaseCapital(MONEY( pReqBankDepositMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);
	
	// ����� �����ش�.
	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DEPOSIT, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &mnBalance ), LPVOID( &pPlayer->siMoney ) );	

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����Ѵ�.
//	���� ����	:	2002-10-18 ���� 3:24:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOVillage_Bank::Drawing(SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney )
{
	MONEY						siBalance;		

	// ���࿡�� ����Ѵ�.
	siBalance	=	mnMoney;
	if( m_pDataBase->GetDBInfo( SO_DB_INFOKIND_DRAWING, LPVOID( pAccountNumber ), LPVOID( &siBalance ) ) == FALSE)	return	FALSE;
		
	// ���� ���������� ����ߴ�. 
	// �� ���� ���ϰ� ���� ���� ���δ�.
	pPlayer->IncreaseMoney( mnMoney, FALSE );
	//DecreaseBankCapital(MONEY(pReqBankDrawingMsg->siMoney*ON_CAPITAL_RATE_BANK),ON_PROFIT_BANK);

	pPlayer->SendFieldMsg( ON_RESPONSE_BANK_DRAWING, SO_SFM_ONLYME, LPVOID( ON_RET_BANK_OK ), LPVOID( &siBalance ), LPVOID( &pPlayer->siMoney ) );
	
	return	TRUE;
}
