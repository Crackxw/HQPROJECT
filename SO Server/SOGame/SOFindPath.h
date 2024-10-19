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

	UI16	uiMapID;												// ���� ��ã�⸦ �ϰ� �ִ� ���� ID.
	UI16	uiCurCharMoveableTileAtb;								// ���� ��ã�⸦ �����ϰ� ĳ���Ͱ� �̵��� �� �ִ� Ÿ���� �Ӽ�.,
public:
	cltSOFindPath(SI16 siMmx, SI16 siMmy);							// ������.

	virtual BOOL	IsColl(SI32 siX, SI32 siY);						// �浹 Ÿ������ �˾ƿ´�.	
	static	VOID	InitStaticVariable(ISOMap *pISOMap);			// ����ƽ ������ �ʱ�ȭ �Ѵ�.
	virtual BOOL	FindPathL(SI16 siSx, SI16 siSy, SI16 siDx, SI16 siDy, SI16 * pRx, SI16 * pRy, SI16 * pCof, SI16 siMfn, UI16 uiMapID, SI32 siCharMoveableTileAtb);	// ��ã�⸦ �����Ѵ�.
};

#endif
