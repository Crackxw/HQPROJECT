#ifndef	ONLINE_BASE_CHAR_KI
#define ONLINE_BASE_CHAR_KI

#include <windows.h>
#include <directives.h>
#include "Parametabox.h"
#include "OnlineText.h"

struct	strOnlineBaseCharKI
{
	UI16				uiID;
	UI32				uiName;
	SI16				siNation;
	SI16				siMoveSpeed;	
	_ImportantParameta	stImportantParameta;
	SI16				siItemDrop[4];
	SI32				siDropPercent[4];
	SI32				siGeneral;
};

class	cltOnlineBaseCharKI
{
private:
	UI32					uiTotalCharNum;
	strOnlineBaseCharKI		*pstOnlineBaseCharKI;
	
	OnlineText				*pOnlineText;
private:
	UI32	FindIndex(UI16 uiID);				// ID�� ����Ͽ� �ε����� ã�´�.
	UI32	FindIndex(CHAR *pszName);			// Name�� ����Ͽ� �ε����� ã�´�.
public:
	cltOnlineBaseCharKI();							// ������.
	virtual	~cltOnlineBaseCharKI();				// �Ҹ���.

	VOID	Init(OnlineText* ponlinetext, UI32 uitotalcharnum);			// �޸𸮸� �Ҵ� �޴´�.
	VOID	Free();								// �޸𸮸� �����Ѵ�.

	VOID	Init(UI32 uiIndex, UI16 uiID, UI32 uiName, SI16 siNation, SI16 siMoveSpeed, _ImportantParameta *pstParameta,
					SI16 siItemDrop[4], SI32 siDropPercent[4], SI32 siGeneral);		// �Ѹ��� ĳ���͸� �ʱ�ȭ�� �Ѵ�.
	
	SI16					GetNation(UI16 uiID);											// ������ ���´�.
	SI16					GetMoveSpeed(UI16 uiID);										// �̵� �ӵ��� ���´�.	
	_ImportantParameta*		GetParameta(UI16 uiID);											// �Ķ��Ÿ�� ���´�.
	CHAR*					GetNameByString(UI16 uiID);										// ĳ������ �̸�(���ڿ�)�� ���´�.
	UI32					GetNameByNumeric(UI16 uiID);									// ĳ������ �̸�(����)�� ���´�.
	strOnlineBaseCharKI*	GetBaseCharKI(UI16 uiID);										// ������ ID�� �̿��Ͽ� �⺻ ĳ���� ������ ���´�.
	strOnlineBaseCharKI*	GetBaseCharKI(CHAR *pszName);									// ������ �̸��� �̿��Ͽ� �⺻ ĳ���� ������ ���´�.
	BOOL					IsValidID(UI16 uiID);											// ��ȿ�� ID���� �˾ƿ´�.
	BOOL					IsValidName(CHAR *pszName);										// ��ȿ�� �̸����� �˾ƿ´�.

	SI16*					GetDropItem(UI16 uiID);											// ĳ���Ͱ� �׾����� ����߸��� �������� ���´�.
	SI32*					GetDropPercent(UI16 uiID);										// ĳ���Ͱ� �׾����� �������� ����߸��� Ȯ���� ���´�.

	SI32					GetGeneral(UI16 uiID);											// ������� ���´�.
	BOOL					IsFieldAttackUnit(UI16 uiID);									// ���������ΰ�.
};

#endif
