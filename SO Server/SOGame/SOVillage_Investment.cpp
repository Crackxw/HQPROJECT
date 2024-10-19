// Type이 -1이면 모든 종류의 Type을 말하는 거다.
#include "SOMain.h"
#include "SOWorld.h"
#include "SOVillage.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	마을 정보 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void		SOVillage::GetVillageInfo(SOPlayer *pPlayer)
{
	pPlayer->SendFieldMsg( ON_RESPONSE_GOVERNMENT_VILLAGEINFO, SO_SFM_ONLYME, LPVOID( szDescription ), LPVOID( GetVillageInOutPlayer() ),
							LPVOID( &vdDefence ), LPVOID( &vdMaxDefence ) );
}
