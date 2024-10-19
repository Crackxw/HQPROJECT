#ifndef	_ONLINE_VILLAGEBASESTRUCTURE_HEADER
#define _ONLINE_VILLAGEBASESTRUCTURE_HEADER

#include	<DDraw.h>
#include	<Button.h>

class	VillageStructManager;

#include	"Online.h"
#include	"OnlineVillage-Parser.h"


#define	ON_VILLAGE_ITEM_LISE_MAX		20

// ����� : ������
// �ǹ�Ŭ������ �⺻


// ������ �⺻ �ǹ� Ŭ����.
class	OnlineVillageBaseStructure
{
protected:
	SI32					siKind;															// � �ǹ��ΰ�.(�Ʒü�, ����, ����...)
	_Button					BExit;															// ���� ��ư.

	cltOnlineWorld			*pMyOnlineWorld;												// �¶��� ����.

	XSPR					BackImage;														// �� ��� �̹���
	XSPR					*pButtonSpr;													// ��ư�� �̹���

	UI16					uiVillageCode;													// ������ Code
	BOOL					bMouseDown;														// ���콺 ���¸� üũ��
	POINT					ptMouse;														// ���콺 ��ǥ
	SI16					siStatus;

	VillageHeader			*pVillageHeader;												// ���� ����ü
	VillageStructManager	*pVillageManager;

	POINT					StartMousePos;													// ���۽� ���콺 ��ġ

public:
	OnlineVillageBaseStructure();			// ������.
	virtual	~OnlineVillageBaseStructure();													// �Ҹ���.

	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);					// �ʱ�ȭ.
	virtual	VOID			Free();															// ����.	
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);							// �׼�.
	virtual	BOOL			Action() = 0;													// �׼�

	SI32					GetKind();														// ������ ����(�Ʒü�, ����, ����...)�� ���´�.
	BOOL					IsVaildStructure();												// ��ȿ�� �ǹ����� �˾ƿ´�.
	VOID					SetStartMousePos(POINT ptStartMousePos);						// Start Mouse Pos ����

protected:

	BOOL					CheckExit();													// ���콺 ���¸� üũ�ؼ� �ǹ����� ������ ���� �˾� ����.
};

#endif
