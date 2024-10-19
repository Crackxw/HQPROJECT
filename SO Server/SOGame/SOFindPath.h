#ifndef	_SO_FINDPATH_H
#define _SO_FINDPATH_H

#include <windows.h>
#include <directives.h>

#include <PathFind.h>
#include <IOnlineMap.h>

class	cltSOFindPath : public _FindPath
{
private:
	static	ISOMap	*pISOMap;

	UI16	uiMapID;												// 현재 길찾기를 하고 있는 맵의 ID.
	UI16	uiCurCharMoveableTileAtb;								// 현재 길찾기를 수행하고 캐릭터가 이동할 수 있는 타일의 속성.,
public:
	cltSOFindPath(SI16 siMmx, SI16 siMmy);							// 생성자.

	virtual BOOL	IsColl(SI32 siX, SI32 siY);						// 충돌 타일인지 알아온다.	
	static	VOID	InitStaticVariable(ISOMap *pISOMap);			// 스태틱 변수를 초기화 한다.
	virtual BOOL	FindPathL(SI16 siSx, SI16 siSy, SI16 siDx, SI16 siDy, SI16 * pRx, SI16 * pRy, SI16 * pCof, SI16 siMfn, UI16 uiMapID, SI32 siCharMoveableTileAtb);	// 길찾기를 수행한다.
};

#endif
