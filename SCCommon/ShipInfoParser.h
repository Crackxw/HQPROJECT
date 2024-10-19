#ifndef	SHIPINFO_PARSER_HEADER
#define SHIPINFO_PARSER_HEADER

#include <GSCDefine.h>
#include <windows.h>
#include <directives.h>


struct	strShipInfo
{
	SI32	siID;											// ID
	SI32	siName;											// Name
	UI16	uiMaxCustom;									// 최대 수용 인원
	SI16	siMoveSpeed;									// 배의 이동 속도.
};

class	cltShipInfoParser
{
private:
	SI32			siTotalShipNum;					// 최대 배의 수.
	strShipInfo		*pstShipInfo;					// 배의 정보.

public:
	cltShipInfoParser();					// 생성자.
	virtual	~cltShipInfoParser();			// 소멸자.

	BOOL			Init(CHAR *pszFileName);		// 화일을 초기화 한다.
	VOID			Free();							// 메모리를 해제한다.	
	strShipInfo*	GetShipInfo(SI32 siIndex);		// 배의 정보를 얻어온다.
	SI32			GetTotalShipNum();				// 총 배의 수를 얻어온다.
	UI16			GetID( CHAR *pToken );			// 텍스트에서 2바이트 ID을 만든다.(나중에 OnlineChar-Parser 와 같이 통합해야 할 부분이다.)
};

#endif
