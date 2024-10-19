#ifndef _RESERVETRAIN_H
#define _RESERVETRAIN_H

#include "order.h"

// 예약생산용 버퍼 
#define MAX_RESERVE_TRAIN_NUMBER  20

class _ReserveTrainBuffer {
public:
     _OrderDataInfo ReserveTrainOrder[MAX_RESERVE_TRAIN_NUMBER];
	 SHORT ReserveTrainIndex;

	 void Init();
	 BOOL PopReserveTrain(_OrderDataInfo& order);
	 // 예약된 장수 생산 명령을 돌려준다. 
     BOOL PopReserveTrainGeneral(_OrderDataInfo& order);
	 BOOL PushReserveTrain(_OrderDataInfo order);
	 BOOL CanReserveTrain(_OrderDataInfo order);

	 // 예약되어있는 명령의 갯수를 구한다.
     SHORT GetReserveTrainNumber(SHORT order);
	 BOOL  DeleteReserveTrain(SHORT order);

	 // 예약되어 있는지 확인한다. 
     BOOL IsReserve(int orderatb, DWORD para1);


};


#endif