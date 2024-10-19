#ifndef _ONLINE_VILLAGEMANAGER_H
#define _ONLINE_VILLAGEMANAGER_H


#include "OnlineVillage-Parser.h"


struct	VillageSearchDataByPos
{
	__int64				dwKey;				// Key���� (X << 16 | Y << 16 | MapIndex << 32)�� ����.	
	VillageHeader*		lpData;				// ������ ������.
};


class	OnlineVillageManager
{
private:
	OnlineVillageParser			VillageParser;						// �ؽ�Ʈ ȭ���� �ļ��ϱ� ���� Ŭ����.

	VillageSearchDataByPos		*pSearchDataByPos;
	SI32						siTotalVillageNum;
	
	cltOnlineWorld				*pMyOnlineWorld;

public:
	OnlineVillageManager();									// ������.
	~OnlineVillageManager();								// �Ҹ���.

	BOOL						Init(CHAR *pszVillageFileName, CHAR *pszPlacementFileName, cltOnlineWorld *pOnlineWorld);		// �ʱ�ȭ.
	VOID						Free();																							// �޸� ����.

	VillageHeader*				GetVillageByPos(SI32 siX, SI32 siY, SI16 MapIndex);												// �ش� ��ġ�� ������ ���´�.
	VillageType*				GetVillageStructPosType(SI16 siType, SI16 StateCode);											// �ش� Ÿ���� ���� ���� �ǹ� ��ġ���� ������ �ִ� ����ü�� �����͸� ���´�.	
	OnlineVillageParser*		GetVillageParser(void);																			// Village Parser�� ��´�.
	VillageHeader*				GetVillageByCode(UI16 Code);
};	


#endif
