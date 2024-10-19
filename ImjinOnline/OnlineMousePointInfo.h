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

// ���콺�� ����Ű�� �ִ� ���� ���� ����
class	OnlineMousePointInfo
{
private:
	SI32	siPointKind;			// � ���� ����Ű�� �ִ°�?
	SI16	siX;					// �̵� X ��ǥ.(Ÿ��)
	SI16	siY;					// �̵� Y ��ǥ.(Ÿ��)
	DWORD	dwParam;
public:
	OnlineMousePointInfo();									// ������.

	VOID	Init();																	// �ʱ�ȭ.
	VOID	SetPointTo(SI32 sipointkind, SI16 six, SI16 siy, DWORD dwparam = 0);	// ���� ����Ű�� �ִ� ������ �����Ѵ�.
	SI32	GetPointKind();															// ���� ����Ű�� �ִ� ������ ���´�.
	VOID	GetPos(SI16 *psiX, SI16 *psiY);											// ����Ű�� �ִ� ���� Ÿ�� ��ǥ�� ���´�.
	DWORD	GetParam();																// ����Ű�� �ִ� ���� ���� �Ķ����.		
};

#endif
