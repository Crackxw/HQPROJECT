#include <GSL.h>

#include "GuildClassParser.h"
#include "bindJXFile.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	생성자.
//	수정 일자	:	2002-02-26 오전 10:56:52 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuildClassParser::cltGuildClassParser()
{
	m_siClassNum		=	0;
	m_pstGuildClassInfo	=	NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	소멸자.
//	수정 일자	:	2002-02-26 오전 10:57:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltGuildClassParser::~cltGuildClassParser()
{
	Free();	
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	초기화.
//	수정 일자	:	2002-02-26 오전 10:58:08 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltGuildClassParser::Init(CHAR *pszFileName)
{
	FILE	*fp = NULL;
	BOOL	bSuccess = FALSE;
	CHAR	szReadBuffer[1024];
	CHAR	cSeps[] = " ,\t\n\r";
	fpos_t	fposDataStart;
	CHAR	*pszToken;
	SI32	i;

			
	
	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL)	goto END;	

	// 13줄을 Skip 한다.	
	for(i = 0; i < 6; i++)	{if(fgets(szReadBuffer, 1024, fp) == NULL) goto END;}

	// 현재의 위치를 저장한다.
	fgetpos(fp, &fposDataStart);

	// 몇개의 데이타가 있는지 센다.
	m_siClassNum	=	0;
	while(1)
	{
		ZeroMemory(szReadBuffer, sizeof(szReadBuffer));

		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken이 NULL이라면 빈줄이다.
			if(pszToken != NULL)
				m_siClassNum++;
		}		
	}	

	// 만약 수량이 0개라면 FALSE를 리턴한다.
	if(m_siClassNum <= 0)	goto	END;

	// 화일 포인터를 다시 되돌린다.
	fsetpos(fp, &fposDataStart);

	// 메모리를 할당 받는다.
	m_pstGuildClassInfo	=	new	strGuildClassInfo[m_siClassNum];
	ZeroMemory(m_pstGuildClassInfo, sizeof(strGuildClassInfo) * m_siClassNum);

	for(i = 0; i < 	m_siClassNum; i++)
	{
		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken이 NULL이라면 빈줄이다.
			if(pszToken != NULL)
			{
				// 순위를 얻어온다.				
				m_pstGuildClassInfo[i].siRanking		=	UI32(atoi(pszToken));

				// 이름은 건너 띈다.
				pszToken =	strtok(NULL, cSeps);

				// 이름코드를 얻어온다				
				pszToken =	strtok(NULL, cSeps);
				m_pstGuildClassInfo[i].uiClassName		=	UI32(atoi(pszToken));

				// 직급 코드.(숫자)를 얻어온다.
				pszToken =	strtok(NULL, cSeps);
				m_pstGuildClassInfo[i].siClassCode		=	atoi(pszToken);

				// 직급 코드.(문자)를 얻어온다.
				pszToken =	strtok(NULL, cSeps);
				strncpy(m_pstGuildClassInfo[i].szClassCode, pszToken, SO_MAX_GUILD_SC_LENGTH);

				// 제대로 읽었는지 검사한다.
				// 이 화일만큼은 절대적으로 제대로 읽어야만 한다.
				if(m_pstGuildClassInfo[i].siClassCode == 0 || strlen(m_pstGuildClassInfo[i].szClassCode) == 0)
					goto END;
			}			
		}		
	}

	bSuccess =	TRUE;

END:
	if(fp != NULL)
	{
		fclose(fp);
		fp	=	NULL;
	}


	if(bSuccess == FALSE)
	{
		Free();
	}

	return bSuccess;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	해제.
//	수정 일자	:	2002-02-26 오전 11:19:03 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltGuildClassParser::Free()
{
	if(m_pstGuildClassInfo != NULL)
	{
		delete [] m_pstGuildClassInfo;
		m_pstGuildClassInfo	=	NULL;
	}

	m_siClassNum	=	0;
}