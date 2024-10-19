#ifndef	ONILNE_SHIP_MOVE_INFO_HEADER
#define ONILNE_SHIP_MOVE_INFO_HEADER

#include "SMPointParser.h"
#include "SMPathParser.h"

class	cltOnlineSMI
{
private:
	cltShipMovePointParser		clShipMovePointParser;
	cltShipMovePathParser		clShipMovePathParser;
public:
	cltOnlineSMI();			// 생성자
	~cltOnlineSMI();		// 소멸자.
	
	VOID	Init();			// 초기화.
	VOID	Free();			// 메모리 해제.

	BOOL	GetPoint(SI32 siPathID, SI32 siPointCount, SI32 *psiX, SI32 *psiY);		// 포인트를 얻어온다.
	BOOL	GetMovePoints(SI32 siPathID, POINT *pptDeparture, POINT *pptDepartureWarp, POINT *pptDestinationWarp, POINT *pptDestination);	// 배가 이동할 지점들을 얻어온다.
	SI32	GetPathPointNum(SI32 siPathID);											// 해당 길에 몇개의 포인트가 있는지 얻어온다.
};

#endif
