#ifndef	_VILLAGE_BANK_HEADER
#define	_VILLAGE_BANK_HEADER

#include "SOCommonHeader.h"
#include "SOVillage_BaseStructure.h"

class	SOPlayer;
class	SODatabase;
class	OnlineEconomy;

class	SOVillage_Bank : public SOVillage_BaseStructure
{
private:
	
	static		SODatabase		*m_pDataBase;							
	static		OnlineEconomy	*m_pOnlineEconomy;						

	UI16		m_uiVillageCode;										// 마을의 코드.

	FLOAT		m_fInterest;											// 이율.
	MONEY		m_mnBankCapital;										// 마을의 금융 자본
	MONEY		m_mnTradeCapital;										// 마을의 무역 자본	



public:

	SOVillage_Bank();													// 생성자.
	~SOVillage_Bank();													// 소멸자.

	BOOL			Init( UI16 uiVillageCode );															// 초기화.
	VOID			SendBankInfo( SOPlayer *pPlayer );													// 전장의 정보를 보내준다.
	SI32			OpenSavingAccount( SOPlayer *pPlayer, UI16 uiKind, MONEY mnDeposit );				// 통장을 개설한다.
	
	BOOL			Deposit( SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney );		// 입금한다.
	BOOL			Drawing(SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney );		// 출금한다.

	static	VOID	InitStaticVariable( SODatabase *pDataBase, OnlineEconomy *pOnlineEconomy );			// 정적 변수를 초기화 한다.		
};


#endif

