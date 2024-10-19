// �� �� �� : �� �� ��
#include <GSL.h>

#include	"GSCDefine.h"
#include	"OnlinePortal-Parser.h"
#include	"BindJxFile.h"
     
OnlinePortalParser::OnlinePortalParser()
{
	pPortalHeader = NULL;
	siPortalNum   = 0;
}

OnlinePortalParser::~OnlinePortalParser()
{
	Free();
}

BOOL OnlinePortalParser::ReadText(char *pFileName)
{
	BOOL	bSuccess	=	FALSE;
	FILE	*fp		= NULL;
	CHAR	*pszToken = NULL; 
	CHAR	szReadBuffer[1024];	
	CHAR	cSeps[] = "\t\n\r";
	fpos_t	fposDataStart;
	SI16	i;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );	
	if( fp == NULL) return FALSE;

	// ó�� ������ Skip �Ѵ�.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos( fp, &fposDataStart );

	// ��� ����Ÿ�� �ִ��� ����.
	siPortalNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );
			
			// pszToken�� NULL�̶�� �����̴�.
			if( pszToken != NULL )
				siPortalNum++;
		}		
	}	

	// ���� ������ 0�̶�� FALSE�� �����Ѵ�.
	if( siPortalNum <= 0 )							goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos( fp, &fposDataStart );

	pPortalHeader = new PortalHeader[siPortalNum];
	ZeroMemory(pPortalHeader, sizeof(PortalHeader) * siPortalNum);

	for(i = 0; i < siPortalNum; i++)
	{
		fgets(szReadBuffer, 1024, fp);

		// Code
		pszToken = strtok(szReadBuffer, cSeps);
		pPortalHeader[i].siPortalCode = atoi(pszToken);

		// Type
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalType = atoi(pszToken);

		// Image Type
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalImage = atoi(pszToken);
		
		// Map X Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMapXPosition = atoi(pszToken);
		
		// Map Y Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMapYPosition = atoi(pszToken);
		
		// Map X End Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMapXEPosition = atoi(pszToken);
		
		// Map Y End Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMapYEPosition = atoi(pszToken);
		
		// Map Index
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMapIndex = atoi(pszToken);
		
		// Move Map X Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMoveMapXPosition = atoi(pszToken);
		
		// Move Map Y Position
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMoveMapYPosition = atoi(pszToken);
		
		// Move Map Index
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalMoveMapIndex = atoi(pszToken);
		
		// Description
		pszToken = strtok(NULL, cSeps);
		
		// Description Code
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siPortalDescriptionCode = atoi(pszToken);
		
		// ���� ����ϴµ� �ʿ��� �ּ� Level
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siReqBattlePower = atoi(pszToken);

		// �� ���� ������� ���� ����� ���� Type
		pszToken = strtok(NULL, cSeps);
		switch(pszToken[0])
		{
			case 'K':	case 'k':
				pPortalHeader[i].siStateSound = ON_COUNTRY_KOREA;
				break;

			case 'J':	case 'j':
				pPortalHeader[i].siStateSound = ON_COUNTRY_JAPAN;
				break;

			case 'C':	case 'c':
				pPortalHeader[i].siStateSound = ON_COUNTRY_CHINA;
				break;

			case 'T':	case 't':
				pPortalHeader[i].siStateSound = ON_COUNTRY_TAIWAN;
				break;
		}

		// �� ���� Tile ������� �׷��������� Flag
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siTileFlag = atoi(pszToken);

		// Portal �̿�� ��� �ݾ�
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].i64PortalMoney = _atoi64(pszToken);

		// Portal �̿�� �ʿ��� LV(�ݾ��� ������츸 ����)
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siNeedLV = atoi(pszToken);

		// Portal �̿�� �ʿ��� Credit(�ݾ��� ������츸 ����)
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siNeedCredit = atoi(pszToken);

		// ���� ����ϴµ� �ʿ��� �ִ� Level
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siReqMaxLv = atoi(pszToken);

		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siLimitNum = atoi(pszToken);
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siStartTime = atoi(pszToken);
		pszToken = strtok(NULL, cSeps);
		pPortalHeader[i].siEndTime = atoi(pszToken);
	}

	bSuccess	=	TRUE;
END:
	if( fp )
	{
		fclose(fp);
		fp	=	NULL;
	}

	if( bSuccess )
	{
		ZeroMemory(&VilliagePortalHeader, sizeof(PortalHeader));
		VilliagePortalHeader.siPortalCode = 30000;
		VilliagePortalHeader.siPortalType = 2;
	}
	else
	{
		Free();
	}

	return bSuccess;
}

VOID OnlinePortalParser::Free()
{
	if(pPortalHeader)
	{
		delete[] pPortalHeader;
		pPortalHeader = NULL;
	}

	pPortalHeader = NULL;
	siPortalNum   = 0;
}

PortalHeader* OnlinePortalParser::GetPortal(SI16 XPos, SI16 YPos, SI16 MapIndex)
{
	if(MapIndex >= 10000)
	{
		// ������
		return &VilliagePortalHeader;
	}
	else
	{
		// �Ϲݸ�
		for(SI16 i = 0; i < siPortalNum; i++)
		{
			switch(pPortalHeader[i].siPortalType)
			{
				case 0:
					if((pPortalHeader[i].siPortalMapXPosition == XPos) && (pPortalHeader[i].siPortalMapYPosition == YPos) && (pPortalHeader[i].siPortalMapIndex == MapIndex))
					{
						return &pPortalHeader[i];
					}
					break;

				case 1:
					if((pPortalHeader[i].siPortalMapXPosition <= XPos) && (pPortalHeader[i].siPortalMapYPosition <= YPos) && (pPortalHeader[i].siPortalMapXEPosition >= XPos) && (pPortalHeader[i].siPortalMapYEPosition >= YPos) && (pPortalHeader[i].siPortalMapIndex == MapIndex))
					{
						return &pPortalHeader[i];
					}
					break;
			}
		}
	}

	return NULL;
}

PortalHeader* OnlinePortalParser::GetPortal(SI16 PortalCode)
{
	if(PortalCode == 30000)
	{
		return &VilliagePortalHeader;
	}
	else 
	{
		for(SI16 i = 0; i < siPortalNum; i++)
		{
			if(pPortalHeader[i].siPortalCode == PortalCode)
			{
				return &pPortalHeader[i];
			}
		}
	}

	return NULL;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��Ż�� �ε����� ���´�.
//	���� ����	:	2002-08-05 ���� 5:27:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	OnlinePortalParser::GetPortalIndex(UI16 PortalCode)
{
	SI32	i;

	for(i = 0; i < siPortalNum; i++)
	{
		if(pPortalHeader[i].siPortalCode == PortalCode)		return i;
	}	

	return	 -1;
}
