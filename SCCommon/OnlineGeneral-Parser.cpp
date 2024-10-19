// 담 당 자 : 이 윤 석
#include <GSL.h>

#include	"Directives.h"
#include	"GSCDefine.h"
#include	"OnlineGeneral-Parser.h"
#include	"bindJXFile.h"

     
OnlineGeneralParser::OnlineGeneralParser()
{
	pGeneralHeader = NULL;
	siGeneralNum   = 0;
}

OnlineGeneralParser::~OnlineGeneralParser()
{
	Free();
}

BOOL OnlineGeneralParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	while( fgets(szBuffer, 1024, fp) )
		siGeneralNum++;

	fseek(fp, SEEK_SET, 0 );
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	pGeneralHeader = new GeneralHeader[siGeneralNum];
	ZeroMemory(pGeneralHeader, sizeof(GeneralHeader) * siGeneralNum);

	for(i = 0; i < siGeneralNum; i++)
	{
		fgets(szBuffer, 1024, fp);

		// Character ID
		pToken = strtok(szBuffer, "\t\n\r");
		pGeneralHeader[i].siGeneralCharacterCode = GetID(pToken);

		// Name & Name Code
		pToken = strtok(NULL, "\t\n\r");
		pToken = strtok(NULL, "\t\n\r");

		// Merc
		pToken = strtok(NULL, "\t\n\r");
		pGeneralHeader[i].siGeneralBaseCharacterCode = GetID(pToken);

		// Need Level
		pToken = strtok(NULL, "\t\n\r");
		pGeneralHeader[i].siGeneralChangeNeedCharacterLv = atoi(pToken);

		// Need Grade
		pToken = strtok(NULL, "\t\n\r");
		pGeneralHeader[i].siGeneralChangeNeedTradeGrade = atoi(pToken);
	}

	fclose(fp);

	return TRUE;
}

VOID OnlineGeneralParser::Free()
{
	if(pGeneralHeader)
	{
		delete[] pGeneralHeader;
		pGeneralHeader = NULL;
	}

	pGeneralHeader = NULL;
	siGeneralNum   = 0;
}

GeneralHeader* OnlineGeneralParser::GetGeneral(SI32 siGeneralCharacterCode)
{
	for(SI16 i = 0; i < siGeneralNum; i++)
	{
		if(pGeneralHeader[i].siGeneralCharacterCode == siGeneralCharacterCode)
		{
			return &pGeneralHeader[i];
		}
	}

	return NULL;
}

GeneralHeader*	OnlineGeneralParser::GetGeneralChar(SI32 siCharCode)
{
	for(SI16 i = 0; i< siGeneralNum; i++)
	{
		if((pGeneralHeader[i].siGeneralBaseCharacterCode == siCharCode)
			|| (pGeneralHeader[i].siGeneralCharacterCode == siCharCode))
			return &pGeneralHeader[i];
	}
	return NULL;

}


SI32	OnlineGeneralParser::GetID(char* lpID)
{
	SI32	uiVal;

	uiVal		=	MAKEWORD(lpID[0], atoi(&lpID[1]));	

	return uiVal;
}
