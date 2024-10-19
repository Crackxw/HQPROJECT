/************************************************************************************************
	���ϸ� : OnlineChar-Order.cpp
	����� : ������

	���� Game���� �����ϴ� Player�� ������ �����Ѵ�.
	�� �κ��� Player�� ����� ������ ó���ϴ� �κ�(Network���� ���� ��� ����)
************************************************************************************************/
#include <GSL.h>
#include <OnlineWorld.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	 ĳ������ ��� ����� �����.
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
//	����	:	ĳ���� ����� ���ۿ� �ִ´�.
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
//	����	:	ĳ���� ����� ���ۿ� �ִ´�.
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
		// �޽��� ���۰� �� á���� �տ� �ִ� ���� �����ϰ� �ϳ��� ������ �����.
		memmove(&OrdersBuffer[0], &OrdersBuffer[1], sizeof(OnOrder) * (MAX_ORDER_BUFFER_NUM - 1));

		memcpy(&OrdersBuffer[MAX_ORDER_BUFFER_NUM - 1], &Order, sizeof(OnOrder));				
		return TRUE;
	}

}

//----------------------------------------------------------------------------------------------------------------
//	����	:	��� ���ۿ��� ��ɵ��� ���´�.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::GetOrder(OnOrder *pOrder)
{
	SI32	i;

	if(siOrdersBufferCount > 0)
	{
		// ����� ù��° �͸� ��������.
		pOrder->siOrder				=	OrdersBuffer[0].siOrder;
		pOrder->siOrderParam1		=	OrdersBuffer[0].siOrderParam1;
		pOrder->siOrderParam2		=	OrdersBuffer[0].siOrderParam2;		

		// ����� �ϳ��� �ڷ� �ű��.
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
//	����	:	���ۿ� ����� �� �ִ��� ����.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsOrderInBuffer()
{
	if(siOrdersBufferCount > 0)
		return TRUE;
	else
		return FALSE;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����� �� ��� ���ۿ� �ֱ����� ���� �� �ִ��� ���캻��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::PreAddOrder(OnOrder *pOrder)
{		
	// ���� �����ϴ� ��ɰ� ���ٸ� �ѱ��.
	if(memcmp(pOrder, &CurrentOrder, sizeof(OnOrder)) == 0)
		return FALSE;

	if(siOrdersBufferCount > 0)
	{
		// �ٷ� �տ� ����� �̵��̶�� �տ� ����� �� ������� ��ü�Ѵ�.
		if(OrdersBuffer[siOrdersBufferCount - 1].siOrder == pOrder->siOrder)
		{
			OrdersBuffer[siOrdersBufferCount - 1].siOrderParam1	=	pOrder->siOrderParam1;
			OrdersBuffer[siOrdersBufferCount - 1].siOrderParam2	=	pOrder->siOrderParam2;
			return FALSE;
		}	
	}		
	
	return TRUE;
}
