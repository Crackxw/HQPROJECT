#include <GSL.h>
#include "SMPointParser.h"
#include "SMPathParser.h"
#include <DebugTool.h>
#include "bindjxfile.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
cltShipMovePathParser::cltShipMovePathParser()
{
	siShipMovePathNum	=	0;
	pstShipMovePath		=	NULL;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¼Ò¸êÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
cltShipMovePathParser::~cltShipMovePathParser()
{
	Free();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	È­ÀÏÀ» ·ÎµåÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-06-20 ¿ÀÈÄ 6:20:41 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// Æ÷ÀÎÆ® ÆÄ¼­¸¦ ·ÎµåÇÑ´Ù.
	if(clShipMovePointParser.Load("Online\\ShipInfo\\ShipMovePoint.txt") == FALSE)	goto	END;
	pstShipMovePoint	=	clShipMovePointParser.GetPoint();
		
	
	if((fp = BindJXFile::GetInstance()->GetFile( pszFileName )) == NULL) goto END;

	// Ã³À½ ¸îÁÙÀ» Skip ÇÑ´Ù.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ÇöÀçÀÇ À§Ä¡¸¦ ÀúÀåÇÑ´Ù.
	fgetpos( fp, &fposDataStart );

	// ¸î°³ÀÇ µ¥ÀÌÅ¸°¡ ÀÖ´ÂÁö ¼¾´Ù.
	siShipMovePathNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszTokenÀÌ NULLÀÌ¶ó¸é ºóÁÙÀÌ´Ù.
			if( pszToken != NULL )
				siShipMovePathNum++;
		}		
	}	

	// ¸¸¾à °³¼ö°¡ 0ÀÌ¶ó¸é FALSE¸¦ ¸®ÅÏÇÑ´Ù.
	if( siShipMovePathNum <= 0 )						goto	END;

	// È­ÀÏ Æ÷ÀÎÅÍ¸¦ ´Ù½Ã µÇµ¹¸°´Ù.
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

		// Ãâ¹ßÇÏ´Â ÁöÁ¡À» ±¸ÇÑ´Ù.
		pstShipMovePath[i].ptDeparture				=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]];

		// ¸ñÀûÁö¸¦ ±¸ÇÑ´Ù.
		pstShipMovePath[i].ptDestination			=	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[pstShipMovePath[i].siNum - 1]];

		// ¿öÇÁÇÒ ÁöÁ¡À» ±¸ÇÑ´Ù.		
		GetWarpPoint(	pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[0]], 
						pstShipMovePoint[pstShipMovePath[i].puiShipMovePointArray[1]],
						&pstShipMovePath[i].ptDepartureWarp);								

		// ¿öÇÁÇØ¼­ ÀÌµ¿ÇÒ ÁöÁ¡À» ±¸ÇÑ´Ù.
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸Þ¸ð¸®¸¦ ÇØÁ¦ÇÑ´Ù.	
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÆÐ½º ¹è¿­ÀÇ Æ÷ÀÎÅÍ¸¦ ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
strShipMovePath*	cltShipMovePathParser::GetPath(SI32 siID)
{
	SI32	siFindIndex;

	siFindIndex	=	FindIndex(siID);

	if(siFindIndex != -1)
		return &pstShipMovePath[siFindIndex];
	else
		return NULL;	
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÇØ´ç idÀÇ ÀÎµ¦½º¸¦ Ã£´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	µÎ Á¡ »çÀÌ¿¡¼­ ¿öÇÁÇÒ °÷À» Ã£´Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-06-20 ¿ÀÈÄ 6:48:25 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltShipMovePathParser::GetWarpPoint(POINT ptStart, POINT ptDest, POINT *pptWarp)
{
	SI32	siXDiff;	
	SI32	siYDiff;

	*pptWarp	=	ptStart;	

	if(abs(ptStart.x - ptDest.x) > abs(ptStart.y - ptDest.y))
	{
		// X Â÷ÀÌ°¡ ´õ Å©´Ù.
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
		// Y Â÷ÀÌ°¡ ´õ Å©´Ù.		
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