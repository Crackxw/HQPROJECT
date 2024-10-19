#include <GSL.h>
#include "SMPointParser.h"
#include "bindjxfile.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipMovePointParser::cltShipMovePointParser()
{
	siShipMovePointNum		=	0;
	ptShipMovePoint			=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipMovePointParser::~cltShipMovePointParser()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ȭ���� �ε��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltShipMovePointParser::Load(CHAR *pszFileName)
{
	SI32	i;
	FILE	*fp;
	CHAR	szReadBuffer[1024];
	BOOL	bRet = FALSE;
	CHAR	cSeps[]   = " ,\t\n\r";
	fpos_t	fposDataStart;
	CHAR	*pszToken;
	
	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL) goto END;

	// ó�� ������ Skip �Ѵ�.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos( fp, &fposDataStart );

	// ��� ����Ÿ�� �ִ��� ����.
	siShipMovePointNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken�� NULL�̶�� �����̴�.
			if( pszToken != NULL )
				siShipMovePointNum++;
		}		
	}	

	// ���� ������ 0�̶�� FALSE�� �����Ѵ�.
	if( siShipMovePointNum <= 0 )						goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos( fp, &fposDataStart );

	ptShipMovePoint			=	new POINT[siShipMovePointNum];
	
	for(i = 0; i < siShipMovePointNum; i++)
	{
		if(fgets(szReadBuffer, 1023, fp) == NULL) goto END;
		
		pszToken = strtok(szReadBuffer, cSeps);				// ID
		
		pszToken = strtok(NULL, cSeps);
		ptShipMovePoint[i].x	=	atoi(pszToken);				// X
		pszToken = strtok(NULL, cSeps);
		ptShipMovePoint[i].y	=	atoi(pszToken);				// Y
	}

	bRet	=	TRUE;
END:
	if(fp)
	{
		fclose(fp);
		fp	=	NULL;
	}

	return bRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸𸮸� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltShipMovePointParser::Free()
{
	if(ptShipMovePoint)
	{
		delete [] ptShipMovePoint;
		ptShipMovePoint	=	NULL;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ü ����Ʈ ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltShipMovePointParser::GetPointNum()
{
	return siShipMovePointNum;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����Ʈ �迭�� �����͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
POINT*	cltShipMovePointParser::GetPoint()
{
	return ptShipMovePoint;
}