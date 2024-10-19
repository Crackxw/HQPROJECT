#ifndef	_ONLINE_IMOUSE_H
#define _ONLINE_IMOUSE_H

#include "Online.h"
#include "OnlineMouse.h"

class IOnlineMouse
{
	private:
		SI16					m_siCurrentMouse;
		POINT					m_ptMouse;
		BOOL					m_bShow;

		OnlineBaseMouse			m_MouseCursor[ON_MOUSE_MAX];

		CCursorImageMgr			m_CursorImageMgr;

	public:	
		~IOnlineMouse();		// 소멸자.

		VOID	Init();			// 초기화
		BOOL	LoadCursorInfo();
		VOID	Free();			// 해제.

		VOID	SetMouse(SI16 siMouse, bool bEnabled = true);	// 마우스를 설정한다.
		SI16	GetMouse();									// 마우스를 얻어온다.
		VOID	SetPos(SI32 siX, SI32 siY);					// 위치를 설정한다.
		VOID	GetPos(LPPOINT pPoint);						// 위치를 얻어온다.
		VOID	ShowMouse(BOOL bshow);						// 마우스를 보이도록 설정한다.
		BOOL	IsShowMouse();								// 마우스가 보이는 상태인지 얻어온다.
		VOID	Draw();										// 마우스를 화면에 그린다.
		VOID	Action();									// 마우스의 액션.
		VOID	SetLoginMouse(BOOL bLogin);
};

#endif
