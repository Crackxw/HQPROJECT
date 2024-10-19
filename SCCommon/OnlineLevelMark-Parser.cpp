#include <GSL.h>

#include	"GSCDefine.h"

#include	"Directives.h"
#include	"OnlineLevelMark-Parser.h"
#include	"bindJXFile.h"


OnlineLevelMarkParser::OnlineLevelMarkParser()
{
	m_MinLevel = 0;

	m_MaxLevelMark = 0;
	m_lpLevelMark  = NULL;
}

OnlineLevelMarkParser::~OnlineLevelMarkParser()
{
	Release();
}

BOOL OnlineLevelMarkParser::Create(char* lpFileName)
{
	Release();

	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	char	szBufferUpText[1024];	
	char*	lpTok;
	int		TempNowMarkCount;

	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if(fp == NULL) return FALSE;

	TempNowMarkCount = 0;
	while(!feof(fp))
	{
		ZeroMemory(szBuffer, 1024);
		fgets(szBuffer, 1024, fp);

		CopyMemory(szBufferUpText, szBuffer, sizeof(szBuffer));
		_strlwr(szBufferUpText);

		// 이 라인의 Code를 얻어옴
		lpTok = strtok((char*)szBufferUpText, " \n\t\r:=,");
		if(lpTok)
		{
			if(strcmp(lpTok, "minlevel") == 0)
			{
				lpTok = strtok((char*)szBuffer, " \n\t\r:=,");

				lpTok      = strtok(NULL, " \n\t\r:=,");
				m_MinLevel = atoi((char*)lpTok);
			}
			else if(strcmp(lpTok, "totalmark") == 0)
			{
				lpTok = strtok((char*)szBuffer, " \n\t\r:=,");
				
				lpTok          = strtok(NULL, " \n\t\r:=,");
				m_MaxLevelMark = atoi((char*)lpTok);
				m_lpLevelMark  = new LevelMarkHeader[m_MaxLevelMark];
				ZeroMemory(m_lpLevelMark, sizeof(LevelMarkHeader) * m_MaxLevelMark);

				TempNowMarkCount = 0;
			}
			else if(strcmp(lpTok, "markdata") == 0)
			{
				lpTok = strtok((char*)szBuffer, " \n\t\r:=,");
				
				lpTok                                           = strtok(NULL, " \n\t\r:=,");
				m_lpLevelMark[TempNowMarkCount].m_LevelMarkCode = atoi((char*)lpTok);

				lpTok                                      = strtok(NULL, " \n\t\r:=,");
				m_lpLevelMark[TempNowMarkCount].m_MinLevel = atoi((char*)lpTok);

				lpTok                                      = strtok(NULL, " \n\t\r:=,");
				m_lpLevelMark[TempNowMarkCount].m_MaxLevel = atoi((char*)lpTok);

				TempNowMarkCount++;
			}
		}
	}

	fclose(fp);

	return TRUE;
}

void OnlineLevelMarkParser::Release()
{
	m_MinLevel = 0;

	m_MaxLevelMark = 0;
	if(m_lpLevelMark)
	{
		delete[] m_lpLevelMark;
		m_lpLevelMark  = NULL;
	}
}

LevelMarkHeader* OnlineLevelMarkParser::GetLevelMarkByLevel(SI32 NowLevel)
{
	SI32		MarkCount;

	if(NowLevel <= 0) return NULL;

	for(MarkCount = 0; MarkCount < m_MaxLevelMark; MarkCount++)
	{
		if((m_lpLevelMark[MarkCount].m_MinLevel >= NowLevel) && (m_lpLevelMark[MarkCount].m_MaxLevel <= NowLevel))
		{
			return &m_lpLevelMark[MarkCount];
		}
	}

	return NULL;
}

LevelMarkHeader* OnlineLevelMarkParser::GetLevelMarkByMarkCode(SI32 LevelCode)
{
	SI32		MarkCount;

	for(MarkCount = 0; MarkCount < m_MaxLevelMark; MarkCount++)
	{
		if(m_lpLevelMark[MarkCount].m_LevelMarkCode == LevelCode)
		{
			return &m_lpLevelMark[MarkCount];
		}
	}

	return NULL;
}
