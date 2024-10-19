/************************************************************************************************
	파일명 : OnlineFindPath.h
	담당자 : 이윤석

	길찾기 Class
************************************************************************************************/
#include <GSL.h>
#include <Main.h>
#include "OnlineFindPath.h"

//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자
//----------------------------------------------------------------------------------------------------------------
cltOnlineFindPath::cltOnlineFindPath(SI16 siMmx, SI16 siMmy, OnlineMap *pOnlineMap) : _FindPath(siMmx, siMmy)
{
	//  아무것도 해주지 않는다.
	pMyOnlineMap	=	pOnlineMap;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	충돌 여부를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

BOOL	cltOnlineFindPath::IsColl(SI32 siX, SI32 siY)
{
	return 	pMyOnlineMap->IsColl(siX, siY, m_siCurCharMoveableTileAtb);
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	실제로 길을 찾는 함수이다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineFindPath::FindPathL(SI16 siSx, SI16 siSy, SI16 siDx, SI16 siDy, SI16 * pRx, SI16 * pRy, SI16 * pCof, SI16 siMfn, SI32 siDirection, SI32 siCutRange, SI32 siCharMoveableTileInfo)
{
	m_siCurCharMoveableTileAtb	=	siCharMoveableTileInfo;

	return _FindPath::FindPathL(siSx, siSy, siDx, siDy, pRx, pRy, pCof, siMfn, siDirection, siCutRange);
}

