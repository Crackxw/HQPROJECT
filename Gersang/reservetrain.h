#ifndef _RESERVETRAIN_H
#define _RESERVETRAIN_H

#include "order.h"

// �������� ���� 
#define MAX_RESERVE_TRAIN_NUMBER  20

class _ReserveTrainBuffer {
public:
     _OrderDataInfo ReserveTrainOrder[MAX_RESERVE_TRAIN_NUMBER];
	 SHORT ReserveTrainIndex;

	 void Init();
	 BOOL PopReserveTrain(_OrderDataInfo& order);
	 // ����� ��� ���� ����� �����ش�. 
     BOOL PopReserveTrainGeneral(_OrderDataInfo& order);
	 BOOL PushReserveTrain(_OrderDataInfo order);
	 BOOL CanReserveTrain(_OrderDataInfo order);

	 // ����Ǿ��ִ� ����� ������ ���Ѵ�.
     SHORT GetReserveTrainNumber(SHORT order);
	 BOOL  DeleteReserveTrain(SHORT order);

	 // ����Ǿ� �ִ��� Ȯ���Ѵ�. 
     BOOL IsReserve(int orderatb, DWORD para1);


};


#endif