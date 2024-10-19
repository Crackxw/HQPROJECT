/************************************************************************************************
	파일명 : IOnlineObjectKI.cpp
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_IOBJECTKI_H
#define _ONLINE_IOBJECTKI_H

#include <OnlineObjectKI.h>

class OnlineText;

class	IOnlineObjectKI
{
private:
	OnlineObjectKI		*pOnlineObjectKI;
	SI32				siObjectNum;
public:
	IOnlineObjectKI();
	~IOnlineObjectKI();

	// 초기화.
	BOOL	Init(OnlineResource *pOnlineResource, OnlineText *pOnlineText);

	// 해제 
	VOID	Free();

	// 이름을 구한다.
	VOID	GetName(SI32 siKind, CHAR pszName[32]);
	VOID	GetNameFromIndex(SI32 siIndex, CHAR pszName[32]);

	// 설명을 구한다.
	CHAR	*GetMark(SI32 siKind);
	CHAR	*GetMarkFromIndex(SI32 siIndex);

	// 화일을 구한다.
	SI16	GetFile(SI32 siKind);
	SI16	GetFileFromIndex(SI32 siIndex);

	// 폰트를 구한다.
	SI16	GetFont(SI32 siKind);
	SI16	GetFontFromIndex(SI32 siIndex);

	// Xsize를 구한다.
	SI16	GetXsize(SI32 siKind);
	SI16	GetXsizeFromIndex(SI32 siIndex);

	// Ysize를 구한다.
	SI16	GetYsize(SI32 siKind);
	SI16	GetYsizeFromIndex(SI32 siIndex);

	// 스프라이트 X 중심점을 구한다.
	SI16	GetSprCenterX(SI32 siKind);
	SI16	GetSprCenterXFromIndex(SI32 siIndex);

	// 스프라이트 Y 중심점을 구한다.
	SI16	GetSprCenterY(SI32 siKind);
	SI16	GetSprCenterYFromIndex(SI32 siIndex);

	// 속성을 얻어온다.
	SI32	GetAtbFromIndex(SI32 siIndex)	{ return pOnlineObjectKI[siIndex].GetAtb(); }
	SI32	GetAtb(SI32 siKind)				{ return pOnlineObjectKI[GetIndex(siKind)].GetAtb(); }


	// 마을 종류를 얻어온다.
	SI16	GetVillageKind(SI32 siKind);
	SI16	GetVillageKindFromIndex(SI32 siIndex);

	// 총 프래임을 얻는다.
	SI16	GetTotalFrame(SI32 siKine);
	SI16	GetTotalFrameFromIndex(SI32 siIndex);

	// 인덱스를 찾는다.
	SI32	GetIndex(SI32 siKind);
};

#endif
