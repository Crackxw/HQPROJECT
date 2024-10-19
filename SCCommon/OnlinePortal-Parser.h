#ifndef __ONLINEPORTAL_PARSER_H
#define __ONLINEPORTAL_PARSER_H

struct	PortalHeader
{
	SI16	siPortalCode;																	// Code

	SI16	siPortalType;																	// Portal Type(0 : 문, 1 : 지역, 2 : 마을 출구)
	SI16	siPortalImage;																	// Portal Image 
	SI16	siPortalMapXPosition;															// Map X Position
	SI16	siPortalMapYPosition;															// Map Y Position
	SI16	siPortalMapXEPosition;															// Map X End Position
	SI16	siPortalMapYEPosition;															// Map Y End Position
	SI16	siPortalMapIndex;																// Map Index
	SI16	siPortalMoveMapXPosition;														// Move Map X Position
	SI16	siPortalMoveMapYPosition;														// Move Map Y Position
	SI16	siPortalMoveMapIndex;															// Move Map Index
	SI32	siPortalDescriptionCode;														// Description Code
	SI32	siReqBattlePower;																// 문을 사용하는데 필요한 최소 전투력
	SI32	siReqMaxLv;																		// 문을 사용하는데 필요한 최대 Level
	SI16	siStateSound;																	// 이 문을 지날경우 나는 배경음 국가 Type
	SI16	siTileFlag;																		// 이 문이 Tile 방식으로 그려질것인지 Flag

	__int64	i64PortalMoney;																	// Portal 이용시 드는 금액
	SI32	siNeedLV;																		// Portal 이용시 필요한 LV(금액이 있을경우만 적용)
	SI32	siNeedCredit;																	// Portal 이용시 필요한 Credit(금액이 있을경우만 적용)

	SI32	siLimitNum;
	SI32	siStartTime;
	SI32	siEndTime;
};


class OnlinePortalParser
{
public:
	PortalHeader			*pPortalHeader;														// Portal의 모든 정보를 가진 구조체
	PortalHeader			VilliagePortalHeader;												// Village Portal의 정보(단 하나만 있음)
	SI16					siPortalNum;														// 총 Portal
	
public:
	OnlinePortalParser();
	virtual	~OnlinePortalParser();

	BOOL					ReadText(char *pFileName);											// Portal 로드
	VOID					Free();																// 지운다

	PortalHeader*			GetPortal(SI16 XPos, SI16 YPos, SI16 MapIndex);						// Get Portal
	PortalHeader*			GetPortal(SI16 PortalCode);													// Get Portal
	SI32					GetPortalIndex(UI16 PortalCode);									// 포탈의 인덱스를 얻어온다.
};


#endif
