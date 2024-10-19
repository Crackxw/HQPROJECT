#ifndef	SO_ISOSHIP_HEADER
#define SO_ISOSHIP_HEADER

#include "GSCDefine.h"
#include "SOShip.h"

class	ISOShip
{
private:
	cltSOShip	clSOShip[MAX_SHIP_NUM];			// ��.	

	UI32		uiLastShipAccount;				// ���������� �ο� �޾Ҵ� Account
private:
	UI16		FindAccount();				// ������ ã�´�.	

public:
	ISOShip();			// ������
	~ISOShip();			// �Ҹ���.

	VOID		Init();			// �ʱ�ȭ.
	VOID		Free();			// �Ҵ� �޾Ҵ� �޸𸮸� �����Ѵ�.
	BOOL		Init(UI16 uimapid, UI16 uishipid, UI16 uideparturevillagecode, UI16 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray);		// �ʱ�ȭ.		
	VOID		Poll();			// Poll �Լ�.
	VOID		LogOutPlayer(UI16 uiShipAccount, UI16 uiPlayerAccount, SI32 siDBAccount);	// �÷��̾ �踦 Ÿ�� �ִ� ���¿��� �α� �ƿ��� �ߴ�.
	VOID		GetInfo(UI16 uiShipAccount, OnCharShipInfo *pShipInfo);						// ���� ������ ���´�.
	BOOL		IsValidID(UI16 uiAccount);													// ��ȿ�� ID �ΰ�?
	UI08		DidSendMoveInfo(UI16 uiAccount);											// �̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°� �˾ƿ´�.
	UI16		GetCustomers(UI16 uiAccount, UI16 *puiAccounts);							// �¼��� ������� ������ ���´�.

	cltSOShip*	GetShip(UI16 uiAccount);													// �踦 ���´�.
};

#endif
