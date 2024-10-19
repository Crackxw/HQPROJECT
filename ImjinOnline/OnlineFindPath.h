/************************************************************************************************
	파일명 : OnlineFindPath.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_FINDPATH_H
#define _ONLINE_FINDPATH_H

#include <PathFind.h>
#include <OnlineMap.h>

class	cltOnlineFindPath : public _FindPath
{
private:
	OnlineMap		*pMyOnlineMap;
	SI32			m_siCurCharMoveableTileAtb;
public:
	cltOnlineFindPath(SI16 siMmx, SI16 siMmy, OnlineMap *pOnlineMap);	

	virtual BOOL	IsColl(SI32 siX, SI32 siY);
	virtual BOOL	FindPathL(SI16 siSx, SI16 siSy, SI16 siDx, SI16 siDy, SI16 * pRx, SI16 * pRy, SI16 * pCof, SI16 siMfn, SI32 siDirection, SI32 siCutRange, SI32 siCharMoveableTileInfo);	// 실제로 길을 찾는 함수이다.
};

#endif
