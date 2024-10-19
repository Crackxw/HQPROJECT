#include <GSL.h>

#include "GuildClassParser.h"
#include "bindJXFile.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������.
//	���� ����	:	2002-02-26 ���� 10:56:52 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltGuildClassParser::cltGuildClassParser()
{
	m_siClassNum		=	0;
	m_pstGuildClassInfo	=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�Ҹ���.
//	���� ����	:	2002-02-26 ���� 10:57:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltGuildClassParser::~cltGuildClassParser()
{
	Free();	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-02-26 ���� 10:58:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	// 13���� Skip �Ѵ�.	
	for(i = 0; i < 6; i++)	{if(fgets(szReadBuffer, 1024, fp) == NULL) goto END;}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos(fp, &fposDataStart);

	// ��� ����Ÿ�� �ִ��� ����.
	m_siClassNum	=	0;
	while(1)
	{
		ZeroMemory(szReadBuffer, sizeof(szReadBuffer));

		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken�� NULL�̶�� �����̴�.
			if(pszToken != NULL)
				m_siClassNum++;
		}		
	}	

	// ���� ������ 0����� FALSE�� �����Ѵ�.
	if(m_siClassNum <= 0)	goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos(fp, &fposDataStart);

	// �޸𸮸� �Ҵ� �޴´�.
	m_pstGuildClassInfo	=	new	strGuildClassInfo[m_siClassNum];
	ZeroMemory(m_pstGuildClassInfo, sizeof(strGuildClassInfo) * m_siClassNum);

	for(i = 0; i < 	m_siClassNum; i++)
	{
		if(fgets(szReadBuffer, 1024, fp) == NULL)	break;
		else
		{
			pszToken =	strtok(szReadBuffer, cSeps);
			
			// pszToken�� NULL�̶�� �����̴�.
			if(pszToken != NULL)
			{
				// ������ ���´�.				
				m_pstGuildClassInfo[i].siRanking		=	UI32(atoi(pszToken));

				// �̸��� �ǳ� ���.
				pszToken =	strtok(NULL, cSeps);

				// �̸��ڵ带 ���´�				
				pszToken =	strtok(NULL, cSeps);
				m_pstGuildClassInfo[i].uiClassName		=	UI32(atoi(pszToken));

				// ���� �ڵ�.(����)�� ���´�.
				pszToken =	strtok(NULL, cSeps);
				m_pstGuildClassInfo[i].siClassCode		=	atoi(pszToken);

				// ���� �ڵ�.(����)�� ���´�.
				pszToken =	strtok(NULL, cSeps);
				strncpy(m_pstGuildClassInfo[i].szClassCode, pszToken, SO_MAX_GUILD_SC_LENGTH);

				// ����� �о����� �˻��Ѵ�.
				// �� ȭ�ϸ�ŭ�� ���������� ����� �о�߸� �Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����.
//	���� ����	:	2002-02-26 ���� 11:19:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltGuildClassParser::Free()
{
	if(m_pstGuildClassInfo != NULL)
	{
		delete [] m_pstGuildClassInfo;
		m_pstGuildClassInfo	=	NULL;
	}

	m_siClassNum	=	0;
}