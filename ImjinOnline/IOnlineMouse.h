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
		~IOnlineMouse();		// �Ҹ���.

		VOID	Init();			// �ʱ�ȭ
		BOOL	LoadCursorInfo();
		VOID	Free();			// ����.

		VOID	SetMouse(SI16 siMouse, bool bEnabled = true);	// ���콺�� �����Ѵ�.
		SI16	GetMouse();									// ���콺�� ���´�.
		VOID	SetPos(SI32 siX, SI32 siY);					// ��ġ�� �����Ѵ�.
		VOID	GetPos(LPPOINT pPoint);						// ��ġ�� ���´�.
		VOID	ShowMouse(BOOL bshow);						// ���콺�� ���̵��� �����Ѵ�.
		BOOL	IsShowMouse();								// ���콺�� ���̴� �������� ���´�.
		VOID	Draw();										// ���콺�� ȭ�鿡 �׸���.
		VOID	Action();									// ���콺�� �׼�.
		VOID	SetLoginMouse(BOOL bLogin);
};

#endif
