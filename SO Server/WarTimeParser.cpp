#include <stdio.h>
#include "WarTimeParser.h"
#include "stdlib.h"
#include "Defines.h"


CWarTimeParser::CWarTimeParser()
{
	memset(siWarDay, -1, sizeof(short) * MAX_WAR_DAY);
}

CWarTimeParser::~CWarTimeParser()
{

}

void CWarTimeParser::Load(char* szFineName)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = " \t\n";
	CHAR	*pToken;
	int		i	=	0;

	if((fp = fopen(szFineName, "r")) != NULL)
	{
		fgets(szReadBuffer, 1023, fp);

		fgets(szReadBuffer, 1023, fp);
//		pToken		=	strtok(szReadBuffer, cSep);
//		pToken		=	strtok(NULL, cSep);
		
		for(int j = 0; j < MAX_WAR_DAY; j++)
		{
			pToken		= strtok(NULL, cSep);
			siWarDay[j] = atoi(pToken);
		}
			
		fgets(szReadBuffer, 1023, fp);
		pToken		=	strtok(szReadBuffer, cSep);
		pToken		=	strtok(NULL, cSep);
		siWarStart	=	atoi(pToken);
		printf("siWarStart: %d", siWarStart);
		
		pToken		=	strtok(NULL, cSep);
		siWarEnd	=	atoi(pToken);
		printf("siWarEnd: %d\n", siWarEnd);

		fgets(szReadBuffer, 1023, fp);
		pToken		=	strtok(szReadBuffer, cSep);
		pToken		=	strtok(NULL, cSep);
		siWarUpdate	=	atoi(pToken);
		pToken		=	strtok(NULL, cSep);
		siWarUpdate	=	atoi(pToken);

		fgets(szReadBuffer, 1023, fp);
		pToken		=	strtok(szReadBuffer, cSep);
		pToken				=	strtok(NULL, cSep);
		siPopulationStart	=	atoi(pToken);
		pToken				=	strtok(NULL, cSep);
		siPopulationEnd		=	atoi(pToken);

		fgets(szReadBuffer, 1023, fp);
		pToken		=	strtok(szReadBuffer, cSep);
		pToken				=	strtok(NULL, cSep);
		siInvestUpdateStart =	atoi(pToken);
		pToken				=	strtok(NULL, cSep);
		siInvestUpdateEnd	=	atoi(pToken);

	}

	fclose(fp);	
}

//공성하는 요일인지 체크한다
BOOL  CWarTimeParser::IsWarDay(int nDayOfWeek)
{
	if(nDayOfWeek < 0 || nDayOfWeek > 6) return FALSE;
	
	BOOL bwar = FALSE;
	for(int i = 0; i < MAX_WAR_DAY; i++)
	{
		if(siWarDay[i] == nDayOfWeek)
		{
			bwar = TRUE;
			break;
		}
	}

	return bwar;
}

//공성 하는 시간인지 체크한다.
BOOL CWarTimeParser::IsWarTime(int nTime)
{
	BOOL bWarTime = FALSE;
	
	if((nTime >= GetWarStart()) && (nTime < GetWarEnd()))
		bWarTime = TRUE;
	
	return bWarTime;
}

//공성하는 요일, 시간을 체크한다.
BOOL CWarTimeParser::IsWarCheck(int nDayOfWeek, int nTime)
{
	if(nDayOfWeek < 0 || nDayOfWeek > 6) return FALSE;
	if(nTime < 0 || nTime > 23) return FALSE;

	if((IsWarDay(nDayOfWeek) == TRUE ) && (IsWarTime(nTime) == TRUE)) return TRUE;

	return FALSE;
}