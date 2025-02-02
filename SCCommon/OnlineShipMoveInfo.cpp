#include <GSL.h>

#include "OnlineShipMoveInfo.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltOnlineSMI::cltOnlineSMI()
{
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltOnlineSMI::~cltOnlineSMI()
{

}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineSMI::Init()
{
	clShipMovePointParser.Load("Online\\ShipInfo\\ShipMovePoint.txt");
	clShipMovePathParser.Load("Online\\ShipInfo\\ShipMovePass.txt");
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬 п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineSMI::Free()
{
	clShipMovePointParser.Free();
	clShipMovePathParser.Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ん檣お蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 望縑 賃偃曖 ん檣お陛 氈朝雖 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡陛 檜翕й 雖薄菟擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-20 螃�� 6:11:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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