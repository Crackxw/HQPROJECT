#ifndef	_ONLINE_SHIPKI_HEADER
#define _ONLINE_SHIPKI_HEADER

class	OnlineShipKI
{
private:
	SI32	siID;					// ���� ID
	SI32	siName;
	SI32	siMaxCustomer;			// �ִ� Ż �� �ִ� �ο�
	SI32	siMoveSpeed;			// �̵� �ӵ�.
public:
	OnlineShipKI();					// ������.
	~OnlineShipKI();				// �Ҹ���.

	VOID	Init(SI32 siid, SI32 siname, SI32 simaxcustomer, SI32 simovespeed);			// �ʱ�ȭ�� �Ѵ�.
	SI32	GetID();																	// ID�� ���´�.
	SI32	GetName();																	// Name Code�� ���´�.
	SI32	GetMaxCustomer();															// �ִ� Ż �� �ִ� �ο��� ���´�.
	SI32	GetMoveSpeed();																// �̵� �ӵ��� ���´�.
};

#endif
