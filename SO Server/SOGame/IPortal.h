#ifndef	IPORTAL_H
#define IPORTAL_H

#include <windows.h>
#include "OnlinePortal-Parser.h"

struct	stPortalData
{
	SI32	uiMaxUsablePlayerNumPerDay;			// �Ϸ� ���ȿ� �ִ� ����� �� �ִ� ����� ��.
	SI32	uiUsedPlayerNumPerDay;				// �Ϸ� ������ ����� ����� ��.
};

class	IPortal : public OnlinePortalParser
{
private:
	stPortalData		*m_pstPortalData;		// �� ��Ż�� ����Ÿ.
	
public:
	IPortal();									// ������.
	virtual	~IPortal();							// �Ҹ���.

	BOOL	Init(CHAR *pszPortalFileName);		// �ʱ�ȭ.
	VOID	Free();								// ����.

	UI16	GetDestMapID(UI16 uiPortalID);		// ������ ���� ID�� ���´�.
	POINT	GetDestPos(UI16 uiPortalID);		// �������� ��ǥ�� ���´�.
	VOID	UsePortal(UI16 uiPortalID);			// ��Ż�� ����Ѵ�.
	BOOL	CanUsePortal(UI16 uiPortalID);		// ��Ż�� ����� �� �ִ��� �˾ƿ´�.

	VOID	DayChanged();						// �Ϸ簡 ����Ǿ���.
};

#endif
