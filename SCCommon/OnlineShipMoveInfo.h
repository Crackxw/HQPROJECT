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
	cltOnlineSMI();			// ������
	~cltOnlineSMI();		// �Ҹ���.
	
	VOID	Init();			// �ʱ�ȭ.
	VOID	Free();			// �޸� ����.

	BOOL	GetPoint(SI32 siPathID, SI32 siPointCount, SI32 *psiX, SI32 *psiY);		// ����Ʈ�� ���´�.
	BOOL	GetMovePoints(SI32 siPathID, POINT *pptDeparture, POINT *pptDepartureWarp, POINT *pptDestinationWarp, POINT *pptDestination);	// �谡 �̵��� �������� ���´�.
	SI32	GetPathPointNum(SI32 siPathID);											// �ش� �濡 ��� ����Ʈ�� �ִ��� ���´�.
};

#endif
