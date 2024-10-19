// �� �� �� : �� �� ��
#include <GSL.h>
#include "ShipInfoParser.h"
#include "bindJXFile.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipInfoParser::cltShipInfoParser()
{
	siTotalShipNum	=	0;
	pstShipInfo		=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipInfoParser::~cltShipInfoParser()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ȭ���� �ε��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltShipInfoParser::Init(CHAR *pszFileName)
{
	FILE	*fp;
	BOOL	bRet	=	FALSE;	
	CHAR	szReadBuffer[1024];
	SI32	i;
	CHAR	cSeps[]   = ",\t\n\"\r";
	fpos_t	fposDataStart;
	CHAR	*pszToken;

	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL)	goto END;
	
	// ó�� ������ Skip �Ѵ�.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos( fp, &fposDataStart );

	// ��� ����Ÿ�� �ִ��� ����.
	siTotalShipNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken�� NULL�̶�� �����̴�.
			if( pszToken != NULL )
				siTotalShipNum++;
		}		
	}	

	// ���� ������ 0�̶�� FALSE�� �����Ѵ�.
	if( siTotalShipNum <= 0 )						goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos( fp, &fposDataStart );

	pstShipInfo		=	new strShipInfo[siTotalShipNum];

	for(i = 0; i < 	siTotalShipNum; i++)
	{
		if(fgets(szReadBuffer, 1023, fp) == NULL) goto END;
		pszToken						=	strtok(szReadBuffer, cSeps);		// ID
		pstShipInfo[i].siID				=	GetID(pszToken);

		pszToken						=	strtok(NULL, cSeps);		// Name�� �׳� ��ŵ�Ѵ�.

		pszToken						=	strtok(NULL, cSeps);		// Name�� �׳� ��ŵ�Ѵ�.
		pstShipInfo[i].siName			=	atoi(pszToken);

		//ZeroMemory(pstShipInfo[i].szName, sizeof(pstShipInfo[i].szName));
		//strncpy(pstShipInfo[i].szName, pszToken, ON_MAX_SHIPNAME_STRING_LENGTH);


		pszToken						=	strtok(NULL, cSeps);		// Max Custom
		pstShipInfo[i].uiMaxCustom		=	atoi(pszToken);
		pszToken						=	strtok(NULL, cSeps);		// Move Speed
		pstShipInfo[i].siMoveSpeed		=	atoi(pszToken);
	}


	bRet	=	TRUE;
END:
	if(fp)
	{
		fclose(fp);
	}

	return bRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸𸮸� �����Ѵ�.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltShipInfoParser::Free()
{
	if(pstShipInfo)
	{
		delete [] pstShipInfo;
		pstShipInfo	=	NULL;
	}

	pstShipInfo	=	0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
strShipInfo*	cltShipInfoParser::GetShipInfo(SI32 siIndex)
{
	return &pstShipInfo[siIndex];
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�� ���� ���� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltShipInfoParser::GetTotalShipNum()
{
	return siTotalShipNum;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	 �ؽ�Ʈ���� 2����Ʈ ID�� �����.(���߿� OnlineChar-Parser �� ���� �����ؾ� �� �κ��̴�.)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	cltShipInfoParser::GetID( CHAR *pToken )
{
	UI16	uiVal;

	uiVal		=	MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}