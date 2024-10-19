#ifndef _ONLINE_FIELDAREA_H
#define _ONLINE_FIELDAREA_H

#include "Online.h"

#define	ON_MAX_NOFIELD_AREA			30		// �ʵ� ������ �ƴ� �簢�� �κ��� �ִ� ����.

class	OnlineFieldArea
{
private:
	UI08				*puiNoFieldArea;						// ȭ�� �󿡼� �ʵ� ������ �ƴ� �κ��� ������ �ִ� �޸�(rcNoFieldArea�� ȥ���ؼ� ��� �Ѵ�.)
	RECT				rcNoFieldArea[ON_MAX_NOFIELD_AREA];		// �ʵ� ������ �ƴ� �κ��� �ִ� �簢�� ����.

protected:
	SI16		FindEmptyHandle();															// ����ִ� �ڵ��� ã�´�.
public:
	OnlineFieldArea();																		// ������.
	~OnlineFieldArea();																		// �Ҹ���.

	VOID		Init();											// �ʱ�ȭ
	VOID		Free();																		// ����.

	SI16		SetNoFieldArea(SI16 siLeft, SI16 siTop, SI16 siRight, SI16 siBottom);		// �ʵ� ������ �ƴ� �κ��� �����Ѵ�.
	SI16		SetNoFieldArea(LPRECT pRect);												// �ʵ� ������ �ƴ� �κ��� �����Ѵ�.
	VOID		SetNoFieldArea(SI16 siX, SI16 siY, XSPR *pXspr);							// Ư�� �޸� ������ ����Ͽ� �����(254)�� �κ��� �ʵ� �������� �����Ѵ�.
	VOID		SetFieldArea(SI16 siHandle);												// �ʵ� ������ �ƴ� �κ��� �ʵ� �������� �� �����Ѵ�.

	BOOL		IsFieldArea(SI16 siX, SI16 siY);											// �ʵ� �������� ���´�.
	UI08		GetNoFieldAreaInfo(SI16 siX, SI16 siY);										// �ʵ� ������ �ƴ� �κ��� ������ �ִ� ���ۿ��� Ư�� ��ġ�� ���� ���´�.,
};

#endif
