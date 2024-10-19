//담 당 자 : 이 상 원
#ifndef __ONLINETICKET_PARSER_H
#define __ONLINETICKET_PARSER_H


#include <windows.h>
#include <directives.h>
#include <GSCDefine.h>


typedef struct
{
	SI32	siTicketCode;																	// Ticket Code
	SI32	siTicketName;																	// Ticket Name

	UI16	uiStartVillageCode;																// 출발 마을 코드
	UI16	uiEndVillageCode;																// 도착 마을 코드

	SI32	siStartTime;																	// 출발 시간
	SI32	siMoveTime;																		// 이동 시간
	MONEY	siMoney;																		// 요금

	SI32	siShipID;																		// 배 ID
	SI32	siShipPassID;																	// 배 이동 경로 ID
} TicketHeader;


class OnlineTicketParser
{
public:
	TicketHeader*			m_lpTicketHeader;													// Ticket의 모든 정보를 가진 구조체
	SI16					m_siTicketNum;														// 총 Ticket
	
public:
	OnlineTicketParser();
	virtual	~OnlineTicketParser();

	BOOL					ReadText(char *pFileName);											// Ticket 로드
	VOID					Free();																// 지운다

	TicketHeader*			GetTicket(SI32 TicketCode);											// Get Ticket
	UI16					GetID(CHAR *pToken);
	SI32					GetPassID(CHAR *pToken);

	SI32					GetPass(UI16 StartVillageCode, UI16 EndVillageCode, SI32 ShipID);			// Pass 찾기
};


#endif
