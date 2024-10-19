/************************************************************************************************
	파일명 : OnlineChar-Order.cpp
	담당자 : 이윤석

	현재 Game에서 존재하는 Player의 정보를 관리한다.
	이 부분은 Player가 명령을 내릴때 처리하는 부분(Network으로 오는 명령 포함)
************************************************************************************************/
#include <GSL.h>
#include <OnlineWorld.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	 캐릭터의 모든 명령을 지운다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::ClearOrder()
{
	SI32	i;

	for(i = 0; i < MAX_ORDER_BUFFER_NUM; i++)
	{
		OrdersBuffer[i].siOrder			=	ON_ORDER_NONE;
		OrdersBuffer[i].siOrderParam1		=	0;
		OrdersBuffer[i].siOrderParam2		=	0;
	}

	siOrdersBufferCount	=	0;
}


//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 명령을 버퍼에 넣는다.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::AddOrder(SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2)
{
	OnOrder		Order;

	Order.siOrder			=	siOrder;
	Order.siOrderParam1		=	siOrderParam1;
	Order.siOrderParam2		=	siOrderParam2;

	return AddOrder(Order);	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 명령을 버퍼에 넣는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::AddOrder(OnOrder Order)
{
	if(PreAddOrder(&Order) == FALSE)
		return FALSE;

	if(siOrdersBufferCount < MAX_ORDER_BUFFER_NUM)
	{
		memcpy(&OrdersBuffer[siOrdersBufferCount], &Order, sizeof(OnOrder));		
		siOrdersBufferCount++;

		return TRUE;
	}
	else
	{	
		// 메시지 버퍼가 꽉 찼으면 앞에 있는 것을 삭제하고 하나씩 앞으로 땡긴다.
		memmove(&OrdersBuffer[0], &OrdersBuffer[1], sizeof(OnOrder) * (MAX_ORDER_BUFFER_NUM - 1));

		memcpy(&OrdersBuffer[MAX_ORDER_BUFFER_NUM - 1], &Order, sizeof(OnOrder));				
		return TRUE;
	}

}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	명령 버퍼에서 명령들을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::GetOrder(OnOrder *pOrder)
{
	SI32	i;

	if(siOrdersBufferCount > 0)
	{
		// 명령은 첫번째 것만 가져간다.
		pOrder->siOrder				=	OrdersBuffer[0].siOrder;
		pOrder->siOrderParam1		=	OrdersBuffer[0].siOrderParam1;
		pOrder->siOrderParam2		=	OrdersBuffer[0].siOrderParam2;		

		// 명령을 하나씩 뒤로 옮긴다.
		for(i = 0; i < siOrdersBufferCount - 1; i++)
			memcpy(&OrdersBuffer[i], &OrdersBuffer[i + 1], sizeof(OnOrder));		

		siOrdersBufferCount--;

		return TRUE;
	}
	else
	{
		clGrp.Error("Get Order Failed", "");
		return FALSE;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	버퍼에 명령이 들어가 있는지 본다.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsOrderInBuffer()
{
	if(siOrdersBufferCount > 0)
		return TRUE;
	else
		return FALSE;	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	명령을 현 명령 버퍼에 넣기전에 넣을 수 있는지 살펴본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::PreAddOrder(OnOrder *pOrder)
{		
	// 현재 수행하는 명령과 같다면 넘긴다.
	if(memcmp(pOrder, &CurrentOrder, sizeof(OnOrder)) == 0)
		return FALSE;

	if(siOrdersBufferCount > 0)
	{
		// 바로 앞에 명령이 이동이라면 앞에 명령을 이 명령으로 대체한다.
		if(OrdersBuffer[siOrdersBufferCount - 1].siOrder == pOrder->siOrder)
		{
			OrdersBuffer[siOrdersBufferCount - 1].siOrderParam1	=	pOrder->siOrderParam1;
			OrdersBuffer[siOrdersBufferCount - 1].siOrderParam2	=	pOrder->siOrderParam2;
			return FALSE;
		}	
	}		
	
	return TRUE;
}
