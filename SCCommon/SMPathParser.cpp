#include <GSL.h>
#include "SMPointParser.h"
#include "SMPathParser.h"
#include <DebugTool.h>
#include "bindjxfile.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipMovePathParser::cltShipMovePathParser()
{
	siShipMovePathNum	=	0;
	pstShipMovePath		=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipMovePathParser::~cltShipMovePathParser()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	�倣狨� 煎萄и棻.
//	熱薑 橾濠	:	2002-06-20 螃�� 6:20:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// ん檣お だ憮蒂 煎萄и棻.
	if(clShipMovePointParser.Load("Online\\ShipInfo\\ShipMovePoint.txt") == FALSE)	goto	END;
	pstShipMovePoint	=	clShipMovePointParser.GetPoint();
		
	
	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL) goto END;

	// 籀擠 賃還擊 Skip и棻.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ⑷營曖 嬪纂蒂 盪濰и棻.
	fgetpos( fp, &fposDataStart );

	// 賃偃曖 等檜顫陛 氈朝雖 撫棻.
	siShipMovePathNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken檜 NULL檜塭賊 綴還檜棻.
			if( pszToken != NULL )
				siShipMovePathNum++;
		}		
	}	

	// 虜擒 偃熱陛 0檜塭賊 FALSE蒂 葬欐и棻.
	if( siShipMovePathNum <= 0 )						goto	END;

	// �倣� ん檣攪蒂 棻衛 腎給萼棻.
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

		// 轎嫦ж朝 雖薄擊 掘и棻.
		pstShipMovePath[i].ptDeparture				=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]];

		// 跡瞳雖蒂 掘и棻.
		pstShipMovePath[i].ptDestination			=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[pstShipMovePath[i].siNum - 1]];

		// 錶Щй 雖薄擊 掘и棻.		
		GetWarpPoint(	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]], 
						pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[1]],
						&pstShipMovePath[i].ptDepartureWarp);								

		// 錶Щп憮 檜翕й 雖薄擊 掘и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 п薯и棻.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ぬ蝶 寡翮曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strShipMovePath*	cltShipMovePathParser::GetPath(SI32 siID)
{
	SI32	siFindIndex;

	siFindIndex	=	FindIndex(siID);

	if(siFindIndex != -1)
		return &pstShipMovePath[siFindIndex];
	else
		return NULL;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 id曖 檣策蝶蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	舒 薄 餌檜縑憮 錶Щй 夠擊 瓊朝棻.
//	熱薑 橾濠	:	2002-06-20 螃�� 6:48:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltShipMovePathParser::GetWarpPoint(POINT ptStart, POINT ptDest, POINT *pptWarp)
{
	SI32	siXDiff;	
	SI32	siYDiff;

	*pptWarp	=	ptStart;	

	if(abs(ptStart.x - ptDest.x) > abs(ptStart.y - ptDest.y))
	{
		// X 離檜陛 渦 觼棻.
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
		// Y 離檜陛 渦 觼棻.		
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