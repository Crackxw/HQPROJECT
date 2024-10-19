#ifndef	SO_ISOSHIP_HEADER
#define SO_ISOSHIP_HEADER

#include "GSCDefine.h"
#include "SOShip.h"

class	ISOShip
{
private:
	cltSOShip	clSOShip[MAX_SHIP_NUM];			// 배.	

	UI32		uiLastShipAccount;				// 마지막으로 부여 받았던 Account
private:
	UI16		FindAccount();				// 계정을 찾는다.	

public:
	ISOShip();			// 생성자
	~ISOShip();			// 소멸자.

	VOID		Init();			// 초기화.
	VOID		Free();			// 할당 받았던 메모리를 해제한다.
	BOOL		Init(UI16 uimapid, UI16 uishipid, UI16 uideparturevillagecode, UI16 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray);		// 초기화.		
	VOID		Poll();			// Poll 함수.
	VOID		LogOutPlayer(UI16 uiShipAccount, UI16 uiPlayerAccount, SI32 siDBAccount);	// 플레이어가 배를 타고 있는 상태에서 로그 아웃을 했다.
	VOID		GetInfo(UI16 uiShipAccount, OnCharShipInfo *pShipInfo);						// 배의 정보를 얻어온다.
	BOOL		IsValidID(UI16 uiAccount);													// 유효한 ID 인가?
	UI08		DidSendMoveInfo(UI16 uiAccount);											// 이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가 알아온다.
	UI16		GetCustomers(UI16 uiAccount, UI16 *puiAccounts);							// 승선한 사람들의 정보를 얻어온다.

	cltSOShip*	GetShip(UI16 uiAccount);													// 배를 얻어온다.
};

#endif
