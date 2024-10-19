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
	OnlineMousePoint();													// ������
	virtual ~OnlineMousePoint();										// �Ҹ���.

	VOID	Init(OnlineResource *pOnlineResource, OnlineMap *pOnlineMap);							// �ʱ�ȭ
	VOID	Free();														// ����.

	VOID	SetMouse(SI32 siTileX, SI32 siTileY, SI32 siMapIndex);		// Mouse Point �� �����Ѵ�.

	VOID	Clear(void);												// Mouse Point ��� �ʱ�ȭ
	VOID	Action(void);												// Mouse Point �� �׼�.
	VOID	Draw(void);													// Mouse Point �� ȭ�鿡 �׸���.
};

#endif
