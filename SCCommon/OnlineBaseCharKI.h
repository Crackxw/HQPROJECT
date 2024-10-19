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
	UI32	FindIndex(UI16 uiID);				// ID를 사용하여 인덱스를 찾는다.
	UI32	FindIndex(CHAR *pszName);			// Name을 사용하여 인덱스를 찾는다.
public:
	cltOnlineBaseCharKI();							// 생성자.
	virtual	~cltOnlineBaseCharKI();				// 소멸자.

	VOID	Init(OnlineText* ponlinetext, UI32 uitotalcharnum);			// 메모리를 할당 받는다.
	VOID	Free();								// 메모리를 해제한다.

	VOID	Init(UI32 uiIndex, UI16 uiID, UI32 uiName, SI16 siNation, SI16 siMoveSpeed, _ImportantParameta *pstParameta,
					SI16 siItemDrop[4], SI32 siDropPercent[4], SI32 siGeneral);		// 한명의 캐릭터를 초기화를 한다.
	
	SI16					GetNation(UI16 uiID);											// 국가를 얻어온다.
	SI16					GetMoveSpeed(UI16 uiID);										// 이동 속도를 얻어온다.	
	_ImportantParameta*		GetParameta(UI16 uiID);											// 파라메타를 얻어온다.
	CHAR*					GetNameByString(UI16 uiID);										// 캐릭터의 이름(문자열)을 얻어온다.
	UI32					GetNameByNumeric(UI16 uiID);									// 캐릭터의 이름(숫자)을 얻어온다.
	strOnlineBaseCharKI*	GetBaseCharKI(UI16 uiID);										// 몬스터의 ID를 이용하여 기본 캐릭터 정보를 얻어온다.
	strOnlineBaseCharKI*	GetBaseCharKI(CHAR *pszName);									// 몬스터의 이름을 이용하여 기본 캐릭터 정보를 얻어온다.
	BOOL					IsValidID(UI16 uiID);											// 유효한 ID인지 알아온다.
	BOOL					IsValidName(CHAR *pszName);										// 유효한 이름인지 알아온다.

	SI16*					GetDropItem(UI16 uiID);											// 캐릭터가 죽었을시 떨어뜨리는 아이템을 얻어온다.
	SI32*					GetDropPercent(UI16 uiID);										// 캐릭터가 죽었을시 아이템을 떨어뜨리는 확률을 얻어온다.

	SI32					GetGeneral(UI16 uiID);											// 장수값을 얻어온다.
	BOOL					IsFieldAttackUnit(UI16 uiID);									// 공성유닛인가.
};

#endif
