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

		// Number (�ʿ����)
		pToken = strtok(szBuffer, "\t\n\r");
		
		// �������̸�  (�ʿ����)
		pToken = strtok(NULL, "\t\n\r");

        // ���� ������ �ڵ�
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usOriginalItem = atoi(pToken);

		// �μ��� Ȯ��
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiBrokenProbablity = atoi(pToken);

		// �״�� ���� Ȯ��? (���� ���ϵ��� �׺��� ����Ȯ���� �����ؼ� ���Ѵ�.)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiOriginalProbablity = m_lpSmithItemHeader[i].uiBrokenProbablity + atoi(pToken);

		// ���� ������ �ڵ� 
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usNormalItem = atoi(pToken);

		// ����������� �� Ȯ��? (���� ���ϵ��� �׺��� ����Ȯ���� �����ؼ� ���Ѵ�.)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiNormalProbablity = m_lpSmithItemHeader[i].uiOriginalProbablity + atoi(pToken);

		// ��� ������ �ڵ� 
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usSpecialItem = atoi(pToken);

		// ��޾������� �� Ȯ��? (���� ���ϵ��� �׺��� ����Ȯ���� �����ؼ� ���Ѵ�.) (��� �̰��� �������. ������ 10000)
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].uiSpecialProbablity = m_lpSmithItemHeader[i].uiNormalProbablity + atoi(pToken);

		// �ʿ��� ������
		pToken = strtok(NULL, "\t\n\r");
		m_lpSmithItemHeader[i].usNeedItem = atoi(pToken);


		// �ʿ��� ������ ���� (�ʿ����)
		pToken = strtok(NULL, "\t\n\r");

		// �ʿ��ѵ� (������ ũ�Ƿ� atol�� ����.)
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