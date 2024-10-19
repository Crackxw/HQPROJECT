#ifndef	SO_SHIP_HEADER
#define SO_SHIP_HEADER

#include "Stack.h"
#include "SOBaseChar.h"

#define	SO_SHIP_MAX_CUSTOMER		500		// �� ���� �ִ� Ż �� �ִ� �ο�

class	cltSOShip : public BaseChar
{
private:		
	SI32		siDX, siDY;					// ��ã�⿡�� ����ϱ� ���� DX, DY

	UI16		uiDepartureVillageCode;		// ����ϴ� ������ �ڵ�
	UI16		uiDestinationVillageCode;	// ���� �ϴ� ������ �ڵ�
	
	POINT		ptDeparture, ptDepartureWarp, ptDestinationWarp, ptDestination;	// ���, ����, ������ ���� ����, ���� ����

	SI32		siMovePathID;				// ���� �谡 �̵��ϴ� ���.
	UI16		uiMaxMovePointNum;			// �ִ� �̵��ϴ� ����� ��.
	UI16		uiMovePointCounter;			// �̵��ϴ� ��ο��� � ���� ���� �Դ°�?	

	DWORD		dwMoveDelayTime;			// �ѹ� �̵� ��Ű�� ����ؾ� �ϴ� �ð�.
	DWORD		dwPrevMoveTime;				// ������ �̵��� �ð�.			
	DWORD		dwRealPrevMoveTime;			// ���� �����ӿ� ������ �̵��� �ð�.
	
	SStack<UI16>	CustomerID;									// ���� �¼��� ĳ������ ID
	SStack<SI32>	CustomerDBAccount;							// ���� �¼��� ĳ������ DB Account
public:
	cltSOShip();			// ������.
	~cltSOShip();			// �Ҹ���

	BOOL	Init(UI16 uiaccount, UI16 uimapid, UI16 uishipid, UI32 uideparturevillagecode, UI32 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray);		// �ʱ�ȭ.	
	VOID	Free();																												// �����Ѵ�.
	
	BOOL	Poll();																												// Poll
	VOID	Arrive();										// �������� �����Ͽ��� ���� ȣ��Ǵ� �Լ�.
	VOID	LogOutPlayer(UI16 uiAccount, SI32 siDBAccount);	// �踦 ź ���¿��� ����ڰ� �α׾ƿ��� �ߴ�.
	VOID	GetInfo(OnCharShipInfo *pShipInfo);				// ���� ������ ���´�.
	UI16	GetCustomers(UI16 *puiAccounts);				// �¼��� ������� ������ ���´�.
	
	// ���������� �޽����� Send �Ѵ�.(�迡���� �ַ� �迡 ž���� ������Ը� �����ش�.)	
	virtual	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);
};

#endif
