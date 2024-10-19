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

	UI16		m_uiVillageCode;										// ������ �ڵ�.

	FLOAT		m_fInterest;											// ����.
	MONEY		m_mnBankCapital;										// ������ ���� �ں�
	MONEY		m_mnTradeCapital;										// ������ ���� �ں�	



public:

	SOVillage_Bank();													// ������.
	~SOVillage_Bank();													// �Ҹ���.

	BOOL			Init( UI16 uiVillageCode );															// �ʱ�ȭ.
	VOID			SendBankInfo( SOPlayer *pPlayer );													// ������ ������ �����ش�.
	SI32			OpenSavingAccount( SOPlayer *pPlayer, UI16 uiKind, MONEY mnDeposit );				// ������ �����Ѵ�.
	
	BOOL			Deposit( SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney );		// �Ա��Ѵ�.
	BOOL			Drawing(SOPlayer *pPlayer, OnAccountNumber *pAccountNumber, MONEY mnMoney );		// ����Ѵ�.

	static	VOID	InitStaticVariable( SODatabase *pDataBase, OnlineEconomy *pOnlineEconomy );			// ���� ������ �ʱ�ȭ �Ѵ�.		
};


#endif

