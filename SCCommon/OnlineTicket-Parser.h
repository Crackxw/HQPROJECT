//�� �� �� : �� �� ��
#ifndef __ONLINETICKET_PARSER_H
#define __ONLINETICKET_PARSER_H


#include <windows.h>
#include <directives.h>
#include <GSCDefine.h>


typedef struct
{
	SI32	siTicketCode;																	// Ticket Code
	SI32	siTicketName;																	// Ticket Name

	UI16	uiStartVillageCode;																// ��� ���� �ڵ�
	UI16	uiEndVillageCode;																// ���� ���� �ڵ�

	SI32	siStartTime;																	// ��� �ð�
	SI32	siMoveTime;																		// �̵� �ð�
	MONEY	siMoney;																		// ���

	SI32	siShipID;																		// �� ID
	SI32	siShipPassID;																	// �� �̵� ��� ID
} TicketHeader;


class OnlineTicketParser
{
public:
	TicketHeader*			m_lpTicketHeader;													// Ticket�� ��� ������ ���� ����ü
	SI16					m_siTicketNum;														// �� Ticket
	
public:
	OnlineTicketParser();
	virtual	~OnlineTicketParser();

	BOOL					ReadText(char *pFileName);											// Ticket �ε�
	VOID					Free();																// �����

	TicketHeader*			GetTicket(SI32 TicketCode);											// Get Ticket
	UI16					GetID(CHAR *pToken);
	SI32					GetPassID(CHAR *pToken);

	SI32					GetPass(UI16 StartVillageCode, UI16 EndVillageCode, SI32 ShipID);			// Pass ã��
};


#endif
