/************************************************************************************************
	���ϸ� : IOnlineObjectKI.cpp
	����� : ������
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

	// �ʱ�ȭ.
	BOOL	Init(OnlineResource *pOnlineResource, OnlineText *pOnlineText);

	// ���� 
	VOID	Free();

	// �̸��� ���Ѵ�.
	VOID	GetName(SI32 siKind, CHAR pszName[32]);
	VOID	GetNameFromIndex(SI32 siIndex, CHAR pszName[32]);

	// ������ ���Ѵ�.
	CHAR	*GetMark(SI32 siKind);
	CHAR	*GetMarkFromIndex(SI32 siIndex);

	// ȭ���� ���Ѵ�.
	SI16	GetFile(SI32 siKind);
	SI16	GetFileFromIndex(SI32 siIndex);

	// ��Ʈ�� ���Ѵ�.
	SI16	GetFont(SI32 siKind);
	SI16	GetFontFromIndex(SI32 siIndex);

	// Xsize�� ���Ѵ�.
	SI16	GetXsize(SI32 siKind);
	SI16	GetXsizeFromIndex(SI32 siIndex);

	// Ysize�� ���Ѵ�.
	SI16	GetYsize(SI32 siKind);
	SI16	GetYsizeFromIndex(SI32 siIndex);

	// ��������Ʈ X �߽����� ���Ѵ�.
	SI16	GetSprCenterX(SI32 siKind);
	SI16	GetSprCenterXFromIndex(SI32 siIndex);

	// ��������Ʈ Y �߽����� ���Ѵ�.
	SI16	GetSprCenterY(SI32 siKind);
	SI16	GetSprCenterYFromIndex(SI32 siIndex);

	// �Ӽ��� ���´�.
	SI32	GetAtbFromIndex(SI32 siIndex)	{ return pOnlineObjectKI[siIndex].GetAtb(); }
	SI32	GetAtb(SI32 siKind)				{ return pOnlineObjectKI[GetIndex(siKind)].GetAtb(); }


	// ���� ������ ���´�.
	SI16	GetVillageKind(SI32 siKind);
	SI16	GetVillageKindFromIndex(SI32 siIndex);

	// �� �������� ��´�.
	SI16	GetTotalFrame(SI32 siKine);
	SI16	GetTotalFrameFromIndex(SI32 siIndex);

	// �ε����� ã�´�.
	SI32	GetIndex(SI32 siKind);
};

#endif
