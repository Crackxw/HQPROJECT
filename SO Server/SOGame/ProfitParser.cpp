// 수정 일자 : 2002-02-19 오전 10:18:11 - 양정모

#include "ProfitParser.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltProfitParser::cltProfitParser()
{
	m_pstProfit	=	NULL;
	m_siProfitNum	=	0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltProfitParser::~cltProfitParser()
{
	Free();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 13줄을 Skip 한다.	
	for(i = 0; i < 13; i++)	{if(fgets(szReadBuffer, 1024, fp) == NULL) goto END;}

	// 현재의 위치를 저장한다.
	fgetpos(fp, &fposDataStart);

	// 몇개의 데이타가 있는지 센다.
	m_siProfitNum	=	0;
	while(1)
	{
		ZeroMemory(szReadBuffer, sizeof(szReadBuffer));

		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken이 NULL이라면 빈줄이다.
			if(pszToken != NULL)
				m_siProfitNum++;
		}		
	}	

	// 만약 수량이 0개라면 FALSE를 리턴한다.
	if(m_siProfitNum <= 0)	goto	END;

	// 화일 포인터를 다시 되돌린다.
	fsetpos(fp, &fposDataStart);

	// 메모리를 할당 받는다.
	m_pstProfit	=	new	strProfit[m_siProfitNum];

	for(i = 0; i < 	m_siProfitNum; i++)
	{
		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken이 NULL이라면 빈줄이다.
			if(pszToken != NULL)
			{
				// 기준 가격을 얻어온다.
				m_pstProfit[i].uiBasePrice		=	atoi(pszToken);

				// 판매가,구입가차이(%)를 얻어온다.
				if((pszToken = strtok(NULL, cSeps)) == NULL) goto END;
				m_pstProfit[i].siProfitPercent	=	atoi(pszToken);

				// 변동폭(%)을 얻어온다.
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
		// 실패했다면 할당했던 메모리를 되돌린다.
		Free();
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	해제.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltProfitParser::Free()
{
	if(m_pstProfit != NULL)
	{
		delete [] m_pstProfit;
		m_pstProfit	=	NULL;
	}
		
	m_siProfitNum	=	0;
}

