#include <stdio.h>
#include "WarpParser.h"
#include "stdlib.h"
#include "Defines.h"
#include "gscProtocol.h"

CWarpParser::CWarpParser()
{

}

CWarpParser::~CWarpParser()
{

}

void CWarpParser::Load(char* szFileName)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = "\t";
	CHAR	*pToken;
	int		i	=	0;

	ZeroMemory(&m_stWarpData,sizeof(WarpData) * 8);

	if((fp = fopen(szFileName, "r")) != NULL)
	{
		fgets(szReadBuffer, 1023, fp);
		fgets(szReadBuffer, 1023, fp);

		while(fgets(szReadBuffer, 1023, fp) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			m_stWarpData[i].siKind	=	atoi(pToken);
			pToken =	strtok(NULL, cSep);
			m_stWarpData[i].siMapIndex	=	atoi(pToken);
			pToken =	strtok(NULL, cSep);
			m_stWarpData[i].siX	=	atoi(pToken);
			pToken =	strtok(NULL, cSep);
			m_stWarpData[i].siY	=	atoi(pToken);
			i++;
		}
	}
	fclose(fp);
}

short CWarpParser::GetMapPosition(unsigned short siKind)
{
	for(int i = 0 ; i < 8 ; i++)
	{
		if(siKind == m_stWarpData[i].siKind)
		{
			return i;
		}
	}
	return -1;
}