
#include <main.h>

#ifdef _IMJIN2
	#include "..\Gersang\order.h"
	#include <ridedata.h>
	#include "..\Gersang\reservetrain.h"
	#include "..\Gersang\char.h"
#else 
	#include "..\천년의 신화\char.h"
#endif

////////////////////////////////////////////////////////////////////////////
///    예약 생산 관련 
////////////////////////////////////////////////////////////////////////////

BOOL cltCharOrg::PopReserveTrain()
{
	_OrderDataInfo order;

	// 버퍼에 저장되어 있는 명령을 가져온다. 
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

