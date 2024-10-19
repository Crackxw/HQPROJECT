#ifndef	IPORTAL_H
#define IPORTAL_H

#include <windows.h>
#include "OnlinePortal-Parser.h"

struct	stPortalData
{
	SI32	uiMaxUsablePlayerNumPerDay;			// 하루 동안에 최대 사용할 수 있는 사람의 수.
	SI32	uiUsedPlayerNumPerDay;				// 하루 동안의 사용한 사람의 수.
};

class	IPortal : public OnlinePortalParser
{
private:
	stPortalData		*m_pstPortalData;		// 각 포탈의 데이타.
	
public:
	IPortal();									// 생성자.
	virtual	~IPortal();							// 소멸자.

	BOOL	Init(CHAR *pszPortalFileName);		// 초기화.
	VOID	Free();								// 해제.

	UI16	GetDestMapID(UI16 uiPortalID);		// 목적지 맵의 ID를 얻어온다.
	POINT	GetDestPos(UI16 uiPortalID);		// 목적지의 좌표를 얻어온다.
	VOID	UsePortal(UI16 uiPortalID);			// 포탈을 사용한다.
	BOOL	CanUsePortal(UI16 uiPortalID);		// 포탈을 사용할 수 있는지 알아온다.

	VOID	DayChanged();						// 하루가 변경되었다.
};

#endif
