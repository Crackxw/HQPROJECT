//담 당 자 : 이 상 원
#include <GSL.h>

#include	"OnlineTicket-Parser.h"
#include	"bindjxfile.h"

     
OnlineTicketParser::OnlineTicketParser()
{
	m_lpTicketHeader = NULL;
	m_siTicketNum    = 0;
}

OnlineTicketParser::~OnlineTicketParser()
{
	Free();
}

BOOL OnlineTicketParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;

	if((fp = BindJXFile::GetInstance()->GetFile( pFileName )) == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	while( fgets(szBuffer, 1024, fp) )
		m_siTicketNum++;

	fseek( fp, SEEK_CUR, 0 );
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	

	m_lpTicketHeader = new TicketHeader[m_siTicketNum];
	ZeroMemory(m_lpTicketHeader, sizeof(TicketHeader) * m_siTicketNum);

	for(i = 0; i < m_siTicketNum; i++)
	{
		fgets(szBuffer, 1024, fp);

		// Ticket Code
		pToken = strtok(szBuffer, "\n\t\r");
		m_lpTicketHeader[i].siTicketCode = atoi(pToken);

		pToken = strtok(NULL, "\n\t\r");
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siTicketName = atoi(pToken);

		// 출발 마을
		pToken = strtok(NULL, "\n\t\r");
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].uiStartVillageCode = atoi(pToken);

		// 도착 마을
		pToken = strtok(NULL, "\n\t\r");
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].uiEndVillageCode = atoi(pToken);

		// 출발 시간
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siStartTime = atoi(pToken);

		// 이동 시간
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siMoveTime = atoi(pToken);
		
		// 요금
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siMoney = _atoi64(pToken);
		
		// 배 ID
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siShipID = GetID(pToken);
		
		// 배 이동 경로 ID
		pToken = strtok(NULL, "\n\t\r");
		m_lpTicketHeader[i].siShipPassID = GetPassID(pToken);
	}

	fclose(fp);

	return TRUE;
}

VOID OnlineTicketParser::Free()
{
	if(m_lpTicketHeader)
	{
		delete[] m_lpTicketHeader;
		m_lpTicketHeader = NULL;
	}

	m_lpTicketHeader = NULL;
	m_siTicketNum    = 0;
}

TicketHeader* OnlineTicketParser::GetTicket(SI32 TicketCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_siTicketNum; TempCount++)
	{
		if(m_lpTicketHeader[TempCount].siTicketCode == TicketCode)
		{
			return &m_lpTicketHeader[TempCount];
		}
	}

	return NULL;
}

UI16 OnlineTicketParser::GetID(CHAR *pToken)
{
	UI16	uiVal;

	uiVal = MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}

SI32 OnlineTicketParser::GetPassID(CHAR *pToken)
{
	return (SI32)GetID(pToken);
}

SI32 OnlineTicketParser::GetPass(UI16 StartVillageCode, UI16 EndVillageCode, SI32 ShipID)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < m_siTicketNum; TempCount++)
	{
		if((m_lpTicketHeader[TempCount].uiStartVillageCode == StartVillageCode)
			&& (m_lpTicketHeader[TempCount].uiEndVillageCode == EndVillageCode)
			&& (m_lpTicketHeader[TempCount].siShipID == ShipID))
		{
			return m_lpTicketHeader[TempCount].siShipPassID;
		}
	}

	return -1;
}
