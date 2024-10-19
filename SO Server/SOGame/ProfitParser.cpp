// ���� ���� : 2002-02-19 ���� 10:18:11 - ������

#include "ProfitParser.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltProfitParser::cltProfitParser()
{
	m_pstProfit	=	NULL;
	m_siProfitNum	=	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltProfitParser::~cltProfitParser()
{
	Free();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltProfitParser::Init(CHAR *pszFileName)
{
	FILE	*fp = NULL;
	SI32	i;
	CHAR	szReadBuffer[1024];
	fpos_t	fposDataStart;
	CHAR	cSeps[] = " ,\t\n";
	CHAR	*pszToken;
	BOOL	bSucess	=	FALSE;

	if((fp = fopen(pszFileName, "rt")) == NULL)	goto END;

	// 13���� Skip �Ѵ�.	
	for(i = 0; i < 13; i++)	{if(fgets(szReadBuffer, 1024, fp) == NULL) goto END;}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos(fp, &fposDataStart);

	// ��� ����Ÿ�� �ִ��� ����.
	m_siProfitNum	=	0;
	while(1)
	{
		ZeroMemory(szReadBuffer, sizeof(szReadBuffer));

		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken�� NULL�̶�� �����̴�.
			if(pszToken != NULL)
				m_siProfitNum++;
		}		
	}	

	// ���� ������ 0����� FALSE�� �����Ѵ�.
	if(m_siProfitNum <= 0)	goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos(fp, &fposDataStart);

	// �޸𸮸� �Ҵ� �޴´�.
	m_pstProfit	=	new	strProfit[m_siProfitNum];

	for(i = 0; i < 	m_siProfitNum; i++)
	{
		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken�� NULL�̶�� �����̴�.
			if(pszToken != NULL)
			{
				// ���� ������ ���´�.
				m_pstProfit[i].uiBasePrice		=	atoi(pszToken);

				// �ǸŰ�,���԰�����(%)�� ���´�.
				if((pszToken = strtok(NULL, cSeps)) == NULL) goto END;
				m_pstProfit[i].siProfitPercent	=	atoi(pszToken);

				// ������(%)�� ���´�.
				if((pszToken = strtok(NULL, cSeps)) == NULL) goto END;
				m_pstProfit[i].siRangePercent	=	atoi(pszToken);
			}			
		}		
	}

	bSucess	=	TRUE;

END:
	if(fp != NULL)	
		fclose(fp);

	if(bSucess == FALSE)
	{
		// �����ߴٸ� �Ҵ��ߴ� �޸𸮸� �ǵ�����.
		Free();
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltProfitParser::Free()
{
	if(m_pstProfit != NULL)
	{
		delete [] m_pstProfit;
		m_pstProfit	=	NULL;
	}
		
	m_siProfitNum	=	0;
}

