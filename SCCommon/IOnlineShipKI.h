#ifndef	_IONLINE_SHIPKI_HEADER
#define _IONLINE_SHIPKI_HEADER

#include "OnlineShipKI.h"

class	IOnlineShipKI
{
private:
	OnlineShipKI		*pOnlineShipKI;
	SI32				siTotalShipNum;

private:
	SI32				FindIndex(SI32 siID);							// ID�� ����Ͽ� Index�� ã�´�.
public:
	IOnlineShipKI();													// ������.
	~IOnlineShipKI();													// �Ҹ���.

	BOOL			Init();														// �ʱ�ȭ�� �Ѵ�.
	VOID			Free();														// �޸𸮸� �����Ѵ�.
	OnlineShipKI*	GetInfoByIndex(SI32 siIndex);								// Index�� ����Ͽ� OnlineShipKI �����͸� ���´�.
	SI32			GetName(SI32 siID);											// Name Code�� ���´�.
	SI32			GetMaxCustomer(SI32 siID);									// �ִ� Ż �� �ִ� �ο��� ���´�.
	SI32			GetMoveSpeed(SI32 siID);									// �̵� �ӵ��� ���´�.
	BOOL			IsInit();													// �ʱ�ȭ�� �ߴ��� �˾ƿ´�.
	SI32			GetTotalShipNum();											// �� ���� ���� ���´�.
};


#endif