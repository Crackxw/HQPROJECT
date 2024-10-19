#ifndef	SO_SHIP_HEADER
#define SO_SHIP_HEADER

#include "Stack.h"
#include "SOBaseChar.h"

#define	SO_SHIP_MAX_CUSTOMER		500		// 한 배의 최대 탈 수 있는 인원

class	cltSOShip : public BaseChar
{
private:		
	SI32		siDX, siDY;					// 길찾기에서 사용하기 위한 DX, DY

	UI16		uiDepartureVillageCode;		// 출발하는 마을의 코드
	UI16		uiDestinationVillageCode;	// 도착 하는 마을의 코드
	
	POINT		ptDeparture, ptDepartureWarp, ptDestinationWarp, ptDestination;	// 출발, 워프, 목적지 워프 지점, 도착 지점

	SI32		siMovePathID;				// 현재 배가 이동하는 경로.
	UI16		uiMaxMovePointNum;			// 최대 이동하는 경로의 수.
	UI16		uiMovePointCounter;			// 이동하는 경로에서 어떤 지점 까지 왔는가?	

	DWORD		dwMoveDelayTime;			// 한번 이동 시키고 대기해야 하는 시간.
	DWORD		dwPrevMoveTime;				// 이전에 이동한 시간.			
	DWORD		dwRealPrevMoveTime;			// 이전 프레임에 실제로 이동한 시간.
	
	SStack<UI16>	CustomerID;									// 현재 승선한 캐릭터의 ID
	SStack<SI32>	CustomerDBAccount;							// 현재 승선한 캐릭터의 DB Account
public:
	cltSOShip();			// 생성자.
	~cltSOShip();			// 소멸자

	BOOL	Init(UI16 uiaccount, UI16 uimapid, UI16 uishipid, UI32 uideparturevillagecode, UI32 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray);		// 초기화.	
	VOID	Free();																												// 해제한다.
	
	BOOL	Poll();																												// Poll
	VOID	Arrive();										// 목적지에 도착하였을 때에 호출되는 함수.
	VOID	LogOutPlayer(UI16 uiAccount, SI32 siDBAccount);	// 배를 탄 상태에서 사용자가 로그아웃을 했다.
	VOID	GetInfo(OnCharShipInfo *pShipInfo);				// 배의 정보를 얻어온다.
	UI16	GetCustomers(UI16 *puiAccounts);				// 승선한 사람들의 정보를 얻어온다.
	
	// 실질적으로 메시지를 Send 한다.(배에서는 주로 배에 탑승한 사람에게만 보내준다.)	
	virtual	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);
};

#endif
