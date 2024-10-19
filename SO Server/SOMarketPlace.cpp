#include "SOMarketPlace.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 좌판대 포인터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
stBOOTH*	SOMarketPlace::GetBooth(UI16 BoothCode)
{
	for(int i=0; i<clBoothInfo.siMapStoreNum;i++)
	{
		if( pBooth[i].pHeader->siMapStoreCode	==	BoothCode )
		{
			return &pBooth[i];
		}
	}
	printf("Code가[%u]인 좌판대를 얻어올 수 없다.",BoothCode);
	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전체 좌판대를 검색해서 플레이어를 나가게 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	SOMarketPlace::LeaveBooth(SOPlayer *pPlayer)
{
	for(int i=0; i<clBoothInfo.siMapStoreNum; i++ )	
	{
		if(pBooth[i].LeaveBooth(pPlayer) == TRUE )
			return TRUE;
	}
	return FALSE;
}

