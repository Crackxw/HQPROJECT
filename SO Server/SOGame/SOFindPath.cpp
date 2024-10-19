#include "SOMain.h"
#include "SOFindPath.h"

ISOMap*	cltSOFindPath::pISOMap	=	NULL;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltSOFindPath::cltSOFindPath(SI16 siMmx, SI16 siMmy) : _FindPath(siMmx, siMmy)
{
	_FindPath::SetFindPathSize(siMmx, siMmy);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길찾기를 수행한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltSOFindPath::FindPathL(SI16 siSx, SI16 siSy, SI16 siDx, SI16 siDy, SI16 * pRx, SI16 * pRy, SI16 * pCof, SI16 siMfn, UI16 uimapid, SI32 siCharMoveableTileAtb)
{
	uiCurCharMoveableTileAtb	=	siCharMoveableTileAtb;
	uiMapID						=	uimapid;


	return _FindPath::FindPathL(siSx, siSy, siDx, siDy, pRx, pRy, pCof, siMfn, 8, 1);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	충돌 타일인지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltSOFindPath::IsColl(SI32 siX, SI32 siY)
{
	return pISOMap->IsColl(uiMapID, siX, siY, uiCurCharMoveableTileAtb);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	스태틱 변수를 초기화 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltSOFindPath::InitStaticVariable(ISOMap *pisomap)
{
	pISOMap		=	pisomap;
}
