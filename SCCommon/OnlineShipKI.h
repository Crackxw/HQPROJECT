#ifndef	_ONLINE_SHIPKI_HEADER
#define _ONLINE_SHIPKI_HEADER

class	OnlineShipKI
{
private:
	SI32	siID;					// 배의 ID
	SI32	siName;
	SI32	siMaxCustomer;			// 최대 탈 수 있는 인원
	SI32	siMoveSpeed;			// 이동 속도.
public:
	OnlineShipKI();					// 생성자.
	~OnlineShipKI();				// 소멸자.

	VOID	Init(SI32 siid, SI32 siname, SI32 simaxcustomer, SI32 simovespeed);			// 초기화를 한다.
	SI32	GetID();																	// ID를 얻어온다.
	SI32	GetName();																	// Name Code를 얻어온다.
	SI32	GetMaxCustomer();															// 최대 탈 수 있는 인원을 얻어온다.
	SI32	GetMoveSpeed();																// 이동 속도를 얻어온다.
};

#endif
