// 담 당 자 : 이 윤 석
#include <GSL.h>

#include	"Directives.h"
#include	"GSCDefine.h"
#include	"OnlineGameOption.h"
#include	"bindJXFIle.h"

     
OnlineGameOption::OnlineGameOption()
{
	m_Option_PKFlag = FALSE;
}

OnlineGameOption::~OnlineGameOption()
{
	Free();
}

BOOL OnlineGameOption::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL) return FALSE;

	while(!feof(fp))
	{
		ZeroMemory(szBuffer, 1024);
		fgets(szBuffer, 1024, fp);
		if(szBuffer[0] == NULL) break;

		// Option Type
		pToken = strtok(szBuffer, "\n\t= ");
		if(pToken == NULL) break;

		strupr(pToken);
		if(strcmp(pToken, "PK") == 0)
		{
			pToken = strtok(NULL, "\n\t= ");
			
			strupr(pToken);
			if(strcmp(pToken, "TRUE") == 0)
			{
				m_Option_PKFlag = TRUE;
			}
			else if(strcmp(pToken, "FALSE") == 0)
			{
				m_Option_PKFlag = FALSE;
			}
		}
	}

	fclose(fp);

	return TRUE;
}

VOID OnlineGameOption::Free()
{
	m_Option_PKFlag = FALSE;
}
