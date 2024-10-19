#include <GSL.h>

#include	"OnlineSmithParser.h"
#include	"OnlineText.h"
#include	"bindjxfile.h"

OnlineSmithParser::OnlineSmithParser()
{
	m_lpSmithItemHeader = NULL;
	m_siSmithItemNum = 0;
}

OnlineSmithParser::~OnlineSmithParser()
{
	Free();
}


VOID OnlineSmithParser::Free()
{
	if (m_lpSmithItemHeader != NULL)
	{
		delete [] m_lpSmithItemHeader;
		m_lpSmithItemHeader = NULL;
	}

	m_siSmithItemNum = 0;
}


BOOL OnlineSmithParser::ReadText(char *pFileName)
{
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;

	FILE *fp = BindJXFile::GetInstance()->GetFile( pFileName );

	if( fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	while( fgets(szBuffer, 1024, fp) )
		m_siSmithItemNum++;


	fseek( fp, SEEK_CUR, 0 );
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	

	m_lpSmithItemHeader = new SmithItemHeader[m_siSmithItemNum];
	ZeroMemory(m_lpSmithItemHeader, sizeof(SmithItemHeader) * m_siSmithItemNum);

	for(i = 0; i < m_siSmithItemNum; i++)
	{
		fgets(szBuffer, 1024, fp);

		// Number (필요없음)
		pToken = strtok(szBuffer, "\t\n\r");
		
		// 아이템이름  (필요없음)
		pToken = strtok(NULL, "\t\n\r");

        // 원래 아이템 코드
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usOriginalItem = atoi(pToken);

		// 부서질 확률
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiBrokenProbablity = atoi(pToken);

		// 그대로 있을 확률? (쓰기 편하도록 그보다 하위확률에 누적해서 더한다.)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiOriginalProbablity = m_lpSmithItemHeader[i].uiBrokenProbablity + atoi(pToken);

		// 보통 아이템 코드 
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usNormalItem = atoi(pToken);

		// 보통아이템이 될 확률? (쓰기 편하도록 그보다 하위확률에 누적해서 더한다.)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiNormalProbablity = m_lpSmithItemHeader[i].uiOriginalProbablity + atoi(pToken);

		// 고급 아이템 코드 
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usSpecialItem = atoi(pToken);

		// 고급아이템이 될 확률? (쓰기 편하도록 그보다 하위확률에 누적해서 더한다.) (사실 이것은 쓸모없다. 무조건 10000)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiSpecialProbablity = m_lpSmithItemHeader[i].uiNormalProbablity + atoi(pToken);

		// 필요한 아이템
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usNeedItem = atoi(pToken);


		// 필요한 아이템 설명 (필요없음)
		pToken = strtok(NULL, "\t\n\r");

		// 필요한돈 (단위가 크므로 atol을 쓴다.)
		pToken = strtok(NULL, "\t\n\r");

		m_lpSmithItemHeader[i].moNeedMoney = atol(pToken);
		
	}

	fclose(fp);

	return TRUE;
}


SmithItemHeader *OnlineSmithParser::GetSmithItem(UI16 uiOldItemCode)
{
	for (int i = 0; i < m_siSmithItemNum; i++)
	{
		if (m_lpSmithItemHeader[i].usOriginalItem == uiOldItemCode)
		{
			return &m_lpSmithItemHeader[i];
		}
	}

	return NULL;
}