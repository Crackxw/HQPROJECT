#ifndef	_ONLINE_MOUSE_POINT_H
#define _ONLINE_MOUSE_POINT_H

#include "Online.h"
#include "OnlineMouse.h"
#include "OnlineResource.h"
#include "OnlineMap.h"

#define MAX_MOUSEPOINT		1


typedef struct
{
	SI32	sifCreate;
	SI32	siTileX;
	SI32	siTileY;
	SI32	siMapIndex;
	SI32	siCount;
} SMousePointInfo;


class OnlineMousePoint
{
private:
	BOOL					m_fCreate;
	OnlineResource			*m_pOnlineResource;
	OnlineMap				*m_pOnlineMap;
	SMousePointInfo			m_MousePointData[MAX_MOUSEPOINT];

public:	
	OnlineMousePoint();													// 생성자
	virtual ~OnlineMousePoint();										// 소멸자.

	VOID	Init(OnlineResource *pOnlineResource, OnlineMap *pOnlineMap);							// 초기화
	VOID	Free();														// 해제.

	VOID	SetMouse(SI32 siTileX, SI32 siTileY, SI32 siMapIndex);		// Mouse Point 를 설정한다.

	VOID	Clear(void);												// Mouse Point 모두 초기화
	VOID	Action(void);												// Mouse Point 의 액션.
	VOID	Draw(void);													// Mouse Point 를 화면에 그린다.
};

#endif
