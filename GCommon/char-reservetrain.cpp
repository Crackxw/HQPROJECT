
#include <main.h>

#ifdef _IMJIN2
	#include "..\Gersang\order.h"
	#include <ridedata.h>
	#include "..\Gersang\reservetrain.h"
	#include "..\Gersang\char.h"
#else 
	#include "..\õ���� ��ȭ\char.h"
#endif

////////////////////////////////////////////////////////////////////////////
///    ���� ���� ���� 
////////////////////////////////////////////////////////////////////////////

BOOL cltCharOrg::PopReserveTrain()
{
	_OrderDataInfo order;

	// ���ۿ� ����Ǿ� �ִ� ����� �����´�. 
	BOOL status=ReserveTrainBuffer.PopReserveTrain(order);

	if(status==TRUE)
	{
	   PushOrder(order, FALSE, 20);
	}

	return status;
}

BOOL cltCharOrg::PopReserveTrainGeneral()
{
	_OrderDataInfo order;

	BOOL status=ReserveTrainBuffer.PopReserveTrainGeneral(order);

	if(status==TRUE)
	{
	   PushOrder(order, FALSE, 30);
	}

	return status;
}

