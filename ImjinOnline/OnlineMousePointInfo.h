#ifndef	ON_MOUSE_POINTINFO_HEADER
#define	ON_MOUSE_POINTINFO_HEADER

#define	ON_MOUSE_POINT_FIELD		0
#define	ON_MOUSE_POINT_CHAR			1
#define	ON_MOUSE_POINT_OBJECT		2
#define	ON_MOUSE_POINT_ITEM			3
#define	ON_MOUSE_POINT_MILEPOST		4
#define	ON_MOUSE_POINT_INTERFACE	5
#define	ON_MOUSE_POINT_NPC			6

#include "Online.h"

// 마우스가 가리키고 있는 것의 대한 정보
class	OnlineMousePointInfo
{
private:
	SI32	siPointKind;			// 어떤 것을 가리키고 있는가?
	SI16	siX;					// 이동 X 좌표.(타일)
	SI16	siY;					// 이동 Y 좌표.(타일)
	DWORD	dwParam;
public:
	OnlineMousePointInfo();									// 생성자.

	VOID	Init();																	// 초기화.
	VOID	SetPointTo(SI32 sipointkind, SI16 six, SI16 siy, DWORD dwparam = 0);	// 현재 가르키고 있는 정보를 설정한다.
	SI32	GetPointKind();															// 현재 가르키고 있는 정보를 얻어온다.
	VOID	GetPos(SI16 *psiX, SI16 *psiY);											// 가르키고 있는 것의 타일 좌표를 얻어온다.
	DWORD	GetParam();																// 가르키고 있는 것의 대한 파라미터.		
};

#endif
