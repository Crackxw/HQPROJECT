#ifndef _ONLINE_FIELDAREA_H
#define _ONLINE_FIELDAREA_H

#include "Online.h"

#define	ON_MAX_NOFIELD_AREA			30		// 필드 지역이 아닌 사각형 부분의 최대 개수.

class	OnlineFieldArea
{
private:
	UI08				*puiNoFieldArea;						// 화면 상에서 필드 지역이 아닌 부분을 가지고 있는 메모리(rcNoFieldArea와 혼합해서 써야 한다.)
	RECT				rcNoFieldArea[ON_MAX_NOFIELD_AREA];		// 필드 지역이 아닌 부분의 최대 사각형 개수.

protected:
	SI16		FindEmptyHandle();															// 비어있는 핸들을 찾는다.
public:
	OnlineFieldArea();																		// 생성자.
	~OnlineFieldArea();																		// 소멸자.

	VOID		Init();											// 초기화
	VOID		Free();																		// 해제.

	SI16		SetNoFieldArea(SI16 siLeft, SI16 siTop, SI16 siRight, SI16 siBottom);		// 필드 지역이 아닌 부분을 설정한다.
	SI16		SetNoFieldArea(LPRECT pRect);												// 필드 지역이 아닌 부분을 설정한다.
	VOID		SetNoFieldArea(SI16 siX, SI16 siY, XSPR *pXspr);							// 특정 메모리 영역을 사용하여 투명색(254)인 부분은 필드 지역으로 설정한다.
	VOID		SetFieldArea(SI16 siHandle);												// 필드 지역이 아닌 부분을 필드 지역으로 재 설정한다.

	BOOL		IsFieldArea(SI16 siX, SI16 siY);											// 필드 지역인지 얻어온다.
	UI08		GetNoFieldAreaInfo(SI16 siX, SI16 siY);										// 필드 지역이 아닌 부분을 가지고 있는 버퍼에서 특정 위치의 값을 얻어온다.,
};

#endif
