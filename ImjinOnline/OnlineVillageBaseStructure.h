#ifndef	_ONLINE_VILLAGEBASESTRUCTURE_HEADER
#define _ONLINE_VILLAGEBASESTRUCTURE_HEADER

#include	<DDraw.h>
#include	<Button.h>

class	VillageStructManager;

#include	"Online.h"
#include	"OnlineVillage-Parser.h"


#define	ON_VILLAGE_ITEM_LISE_MAX		20

// 담당자 : 정진욱
// 건물클래스의 기본


// 마을의 기본 건물 클래스.
class	OnlineVillageBaseStructure
{
protected:
	SI32					siKind;															// 어떤 건물인가.(훈련소, 시장, 병원...)
	_Button					BExit;															// 종료 버튼.

	cltOnlineWorld			*pMyOnlineWorld;												// 온라인 월드.

	XSPR					BackImage;														// 뒷 배경 이미지
	XSPR					*pButtonSpr;													// 버튼의 이미지

	UI16					uiVillageCode;													// 마을의 Code
	BOOL					bMouseDown;														// 마우스 상태를 체크함
	POINT					ptMouse;														// 마우스 좌표
	SI16					siStatus;

	VillageHeader			*pVillageHeader;												// 마을 구조체
	VillageStructManager	*pVillageManager;

	POINT					StartMousePos;													// 시작시 마우스 위치

public:
	OnlineVillageBaseStructure();			// 생성자.
	virtual	~OnlineVillageBaseStructure();													// 소멸자.

	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager);					// 초기화.
	virtual	VOID			Free();															// 해제.	
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface);							// 액션.
	virtual	BOOL			Action() = 0;													// 액션

	SI32					GetKind();														// 마을의 종류(훈련소, 시장, 병원...)를 얻어온다.
	BOOL					IsVaildStructure();												// 유효한 건물인지 알아온다.
	VOID					SetStartMousePos(POINT ptStartMousePos);						// Start Mouse Pos 지정

protected:

	BOOL					CheckExit();													// 마우스 상태를 체크해서 건물에서 나가는 것을 알아 낸다.
};

#endif
