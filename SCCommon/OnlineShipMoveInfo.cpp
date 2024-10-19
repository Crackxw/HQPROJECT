#include <GSL.h>

#include "OnlineShipMoveInfo.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltOnlineSMI::cltOnlineSMI()
{
	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltOnlineSMI::~cltOnlineSMI()
{

}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineSMI::Init()
{
	clShipMovePointParser.Load("Online\\ShipInfo\\ShipMovePoint.txt");
	clShipMovePathParser.Load("Online\\ShipInfo\\ShipMovePass.txt");
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineSMI::Free()
{
	clShipMovePointParser.Free();
	clShipMovePathParser.Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineSMI::GetPoint(SI32 siPathID, SI32 siPointCount, SI32 *psiX, SI32 *psiY)
{
	strShipMovePath		*pstShipMovePath;
	POINT				*pstShipMovePoint;

	pstShipMovePath	=	clShipMovePathParser.GetPath(siPathID);

	if(pstShipMovePath != NULL)
	{
		pstShipMovePoint	=	clShipMovePointParser.GetPoint();
		
		if(clShipMovePointParser.GetPointNum() > siPointCount)
		{
			*psiX				=	pstShipMovePoint[pstShipMovePath->puiShipMovePointArray[siPointCount]].x;
			*psiY				=	pstShipMovePoint[pstShipMovePath->puiShipMovePointArray[siPointCount]].y;
			return TRUE;
		}
		else
			return FALSE;		
	}
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� �濡 ��� ����Ʈ�� �ִ��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineSMI::GetPathPointNum(SI32 siPathID)
{
	strShipMovePath		*pstShipMovePath;

	pstShipMovePath	=	clShipMovePathParser.GetPath(siPathID);

	if(pstShipMovePath != NULL)
	{
		return pstShipMovePath->siNum;
	}
	else
		return 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�谡 �̵��� �������� ���´�.
//	���� ����	:	2002-06-20 ���� 6:11:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineSMI::GetMovePoints(SI32 siPathID, POINT *pptDeparture, POINT *pptDepartureWarp, POINT *pptDestinationWarp, POINT *pptDestination)
{
	strShipMovePath		*pstShipMovePath;

	pstShipMovePath	=	clShipMovePathParser.GetPath(siPathID);
	if(pstShipMovePath == NULL)	return	FALSE;

	*pptDeparture			=	pstShipMovePath->ptDeparture;
	*pptDepartureWarp		=	pstShipMovePath->ptDepartureWarp;
	*pptDestinationWarp		=	pstShipMovePath->ptDestinationWarp;
	*pptDestination			=	pstShipMovePath->ptDestination;

	return	TRUE;
}