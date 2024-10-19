#include <GSL.h>
#include "SMPointParser.h"
#include "SMPathParser.h"
#include <DebugTool.h>
#include "bindjxfile.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipMovePathParser::cltShipMovePathParser()
{
	siShipMovePathNum	=	0;
	pstShipMovePath		=	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
cltShipMovePathParser::~cltShipMovePathParser()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ȭ���� �ε��Ѵ�.
//	���� ����	:	2002-06-20 ���� 6:20:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltShipMovePathParser::Load(CHAR *pszFileName)
{
	SI32	i, j;
	FILE	*fp;
	CHAR	szReadBuffer[1024];
	BOOL	bRet = FALSE;
	CHAR	cSeps[]   = " ,\t\n\"\r";
	fpos_t	fposDataStart;
	CHAR	*pszToken;
	POINT						*pstShipMovePoint;
	cltShipMovePointParser		clShipMovePointParser;

	// ����Ʈ �ļ��� �ε��Ѵ�.
	if(clShipMovePointParser.Load("Online\\ShipInfo\\ShipMovePoint.txt") == FALSE)	goto	END;
	pstShipMovePoint	=	clShipMovePointParser.GetPoint();
		
	
	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL) goto END;

	// ó�� ������ Skip �Ѵ�.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ������ ��ġ�� �����Ѵ�.
	fgetpos( fp, &fposDataStart );

	// ��� ����Ÿ�� �ִ��� ����.
	siShipMovePathNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken�� NULL�̶�� �����̴�.
			if( pszToken != NULL )
				siShipMovePathNum++;
		}		
	}	

	// ���� ������ 0�̶�� FALSE�� �����Ѵ�.
	if( siShipMovePathNum <= 0 )						goto	END;

	// ȭ�� �����͸� �ٽ� �ǵ�����.
	fsetpos( fp, &fposDataStart );

	pstShipMovePath			=	new strShipMovePath[siShipMovePathNum];
		
	for(i = 0; i < siShipMovePathNum; i++)
	{
		if(fgets(szReadBuffer, 1023, fp) == NULL) goto END;		
		pszToken = strtok(szReadBuffer, cSeps);
		pstShipMovePath[i].siID		=	GETPATHID(pszToken);		// ID		

		pszToken = strtok(NULL, cSeps);
		pstShipMovePath[i].siNum	=	atoi(pszToken);				// Num
		
		pstShipMovePath[i].puiShipMovePointArray	=	new UI16[pstShipMovePath[i].siNum];

		for(j = 0; j < pstShipMovePath[i].siNum; j++)
		{
			pszToken = strtok(NULL, cSeps);
			pstShipMovePath[i].puiShipMovePointArray[j]		=	atoi(pszToken);
		}

		// ����ϴ� ������ ���Ѵ�.
		pstShipMovePath[i].ptDeparture				=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]];

		// �������� ���Ѵ�.
		pstShipMovePath[i].ptDestination			=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[pstShipMovePath[i].siNum - 1]];

		// ������ ������ ���Ѵ�.		
		GetWarpPoint(	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]], 
						pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[1]],
						&pstShipMovePath[i].ptDepartureWarp);								

		// �����ؼ� �̵��� ������ ���Ѵ�.
		GetWarpPoint(	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[pstShipMovePath[i].siNum - 1]], 
						pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[pstShipMovePath[i].siNum - 2]],
						&pstShipMovePath[i].ptDestinationWarp);		

		
	}

	bRet	=	TRUE;
END:
	if(fp)
	{
		fclose(fp);
		fp	=	NULL;
	}

	clShipMovePointParser.Free();

	return bRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�޸𸮸� �����Ѵ�.	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltShipMovePathParser::Free()
{
	SI32	i;	

	if(pstShipMovePath)
	{
		for(i = 0; i < siShipMovePathNum; i++)
		{
			if(pstShipMovePath[i].puiShipMovePointArray)
			{
				delete [] pstShipMovePath[i].puiShipMovePointArray;
				pstShipMovePath[i].puiShipMovePointArray	=	NULL;
			}
		}

		delete [] pstShipMovePath;
		pstShipMovePath	=	NULL;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�н� �迭�� �����͸� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
strShipMovePath*	cltShipMovePathParser::GetPath(SI32 siID)
{
	SI32	siFindIndex;

	siFindIndex	=	FindIndex(siID);

	if(siFindIndex != -1)
		return &pstShipMovePath[siFindIndex];
	else
		return NULL;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ش� id�� �ε����� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltShipMovePathParser::FindIndex(SI32 siID)
{
	SI32	i;

	for(i = 0; i < siShipMovePathNum; i++)
	{
		if(siID == pstShipMovePath[i].siID)
			return i;
	}
	
	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� �� ���̿��� ������ ���� ã�´�.
//	���� ����	:	2002-06-20 ���� 6:48:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltShipMovePathParser::GetWarpPoint(POINT ptStart, POINT ptDest, POINT *pptWarp)
{
	SI32	siXDiff;	
	SI32	siYDiff;

	*pptWarp	=	ptStart;	

	if(abs(ptStart.x - ptDest.x) > abs(ptStart.y - ptDest.y))
	{
		// X ���̰� �� ũ��.
		if(ptStart.x > ptDest.x)		pptWarp->x	=	max(ptStart.x - 10, ptDest.x);
		else if(ptStart.x < ptDest.x)	pptWarp->x	=	min(ptStart.x + 10, ptDest.x);		
		else pptWarp->x = ptDest.x;

		siXDiff		=	abs(pptWarp->x - ptStart.x);
		siYDiff		=	siXDiff * abs(ptDest.y - ptStart.y) / abs(ptDest.x - ptStart.x);		

		if(ptStart.y > ptDest.y)		pptWarp->y	=	max(ptStart.y - siYDiff, ptDest.y);
		else if(ptStart.y < ptDest.y)	pptWarp->y	=	min(ptStart.y + siYDiff, ptDest.y);		
		else pptWarp->y = ptDest.y;
	}
	else
	{
		// Y ���̰� �� ũ��.		
		if(ptStart.y > ptDest.y)		pptWarp->y	=	max(ptStart.y - 10, ptDest.y);
		else if(ptStart.y < ptDest.y)	pptWarp->y	=	min(ptStart.y + 10, ptDest.y);
		else pptWarp->y = ptDest.y;

		siYDiff		=	abs(pptWarp->y - ptStart.y);
		siXDiff		=	siYDiff * abs(ptDest.x - ptStart.x) / abs(ptDest.y - ptStart.y);		

		if(ptStart.x > ptDest.x)		pptWarp->x	=	max(ptStart.x - siXDiff, ptDest.x);
		else if(ptStart.x < ptDest.x)	pptWarp->x	=	min(ptStart.x + siXDiff, ptDest.x);		
		else pptWarp->x = ptDest.x;
	}
}