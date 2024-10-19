#ifndef	_IONLINE_SHIPKI_HEADER
#define _IONLINE_SHIPKI_HEADER

#include "OnlineShipKI.h"

class	IOnlineShipKI
{
private:
	OnlineShipKI		*pOnlineShipKI;
	SI32				siTotalShipNum;

private:
	SI32				FindIndex(SI32 siID);							// ID를 사용하여 Index를 찾는다.
public:
	IOnlineShipKI();													// 생성자.
	~IOnlineShipKI();													// 소멸자.

	BOOL			Init();														// 초기화를 한다.
	VOID			Free();														// 메모리를 해제한다.
	OnlineShipKI*	GetInfoByIndex(SI32 siIndex);								// Index를 사용하여 OnlineShipKI 포인터를 얻어온다.
	SI32			GetName(SI32 siID);											// Name Code를 얻어온다.
	SI32			GetMaxCustomer(SI32 siID);									// 최대 탈 수 있는 인원을 얻어온다.
	SI32			GetMoveSpeed(SI32 siID);									// 이동 속도를 얻어온다.
	BOOL			IsInit();													// 초기화를 했는지 알아온다.
	SI32			GetTotalShipNum();											// 총 배의 수를 얻어온다.
};


#endif