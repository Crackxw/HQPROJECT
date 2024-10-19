#ifndef _ONLINE_VILLAGEMANAGER_H
#define _ONLINE_VILLAGEMANAGER_H


#include "OnlineVillage-Parser.h"


struct	VillageSearchDataByPos
{
	__int64				dwKey;				// Key지만 (X << 16 | Y << 16 | MapIndex << 32)과 같다.	
	VillageHeader*		lpData;				// 마을의 포인터.
};


class	OnlineVillageManager
{
private:
	OnlineVillageParser			VillageParser;						// 텍스트 화일을 파서하기 위한 클래스.

	VillageSearchDataByPos		*pSearchDataByPos;
	SI32						siTotalVillageNum;
	
	cltOnlineWorld				*pMyOnlineWorld;

public:
	OnlineVillageManager();									// 생성자.
	~OnlineVillageManager();								// 소멸자.

	BOOL						Init(CHAR *pszVillageFileName, CHAR *pszPlacementFileName, cltOnlineWorld *pOnlineWorld);		// 초기화.
	VOID						Free();																							// 메모리 해제.

	VillageHeader*				GetVillageByPos(SI32 siX, SI32 siY, SI16 MapIndex);												// 해당 위치의 마을을 얻어온다.
	VillageType*				GetVillageStructPosType(SI16 siType, SI16 StateCode);											// 해당 타입의 마을 내의 건물 위치들을 가지고 있는 구조체의 포인터를 얻어온다.	
	OnlineVillageParser*		GetVillageParser(void);																			// Village Parser를 얻는다.
	VillageHeader*				GetVillageByCode(UI16 Code);
};	


#endif
