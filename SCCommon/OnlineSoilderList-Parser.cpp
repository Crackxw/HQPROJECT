#include <GSL.h>

#include	"GSCDefine.h"

#include	"Directives.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineSoilderList-Parser.h"
#include	"bindJxFile.h"

     
OnlineSolderListParser::OnlineSolderListParser()
{
	ZeroMemory(m_lpSolderListHeader, sizeof(SolderListHeader) * MAX_SOLDERLIST);
}

OnlineSolderListParser::~OnlineSolderListParser()
{
	Free();
}

BOOL OnlineSolderListParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;
	SI32	TempCount;

	fp = fopen( pFileName, "rt" );

	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	ZeroMemory(m_lpSolderListHeader, sizeof(SolderListHeader) * MAX_SOLDERLIST);
	for(i = 0; i < MAX_SOLDERLIST; i++)
	{
		m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_NONE;
	}

	i = 0;
	while(!feof(fp))
	{
		ZeroMemory(szBuffer, 1024);
		fgets(szBuffer, 1024, fp);
		if(szBuffer[0] == NULL) break;

		// StructCode
		pToken = strtok(szBuffer, "\n\t");
		if(pToken == NULL) break;

		m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_NONE;
		if(strcmp(pToken, ON_VIL_ST_KIND_CODE_INN) == 0)
		{
			m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_INN;
		}
		else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_BARRACK) == 0)
		{
			m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_BARRACK;
		}
		else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_GBARRACK) == 0)
		{
			m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_GBARRACK;
		}
		else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_MBARRACK) == 0)
		{
			m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_MBARRACK;
		}
		else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_GEXBARRACK) == 0)
		{
			m_lpSolderListHeader[i].siStructorCode = STRUCTCODE_GEXBARRACK;
		}
		pToken = strtok(NULL, "\n\t");

		// State Code
		m_lpSolderListHeader[i].siStateCode = ON_COUNTRY_KOREA;

		pToken = strtok(NULL, "\n\t");
		switch(atoi(pToken))
		{
			case 1:
				m_lpSolderListHeader[i].siStateCode = ON_COUNTRY_KOREA;
				break;

			case 10:
				m_lpSolderListHeader[i].siStateCode = ON_COUNTRY_JAPAN;
				break;

			case 100:
				m_lpSolderListHeader[i].siStateCode = ON_COUNTRY_CHINA;
				break;

			case 1000:
				m_lpSolderListHeader[i].siStateCode = ON_COUNTRY_TAIWAN;
				break;
		}

		// Solder Code
		for(TempCount = 0; TempCount < MAX_SOLDER; TempCount++)
		{
			pToken = strtok(NULL, "\n\t");
			m_lpSolderListHeader[i].siSellSolder[TempCount] = GetID(pToken);
		}
		i++;
	}

	fclose(fp);

	return TRUE;
}

VOID OnlineSolderListParser::Free()
{
	ZeroMemory(m_lpSolderListHeader, sizeof(SolderListHeader) * MAX_SOLDERLIST);
}

SolderListHeader* OnlineSolderListParser::GetSolderList(SI32 StructCode, SI32 StateCode)
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < MAX_SOLDERLIST; TempCount++)
	{
		if(m_lpSolderListHeader[TempCount].siStructorCode == StructCode)
		{
			if(m_lpSolderListHeader[TempCount].siStateCode == StateCode)
			{
				return &m_lpSolderListHeader[TempCount];
			}
		}
	}

	return NULL;
}


UI16 OnlineSolderListParser::GetID(CHAR *pToken)
{
	UI16	uiVal;

	if(pToken == NULL)   return 0;
	if(pToken[0] == '0') return 0;

	uiVal = MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}
