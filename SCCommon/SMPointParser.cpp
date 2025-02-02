#include <GSL.h>
#include "SMPointParser.h"
#include "bindjxfile.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipMovePointParser::cltShipMovePointParser()
{
	siShipMovePointNum		=	0;
	ptShipMovePoint			=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipMovePointParser::~cltShipMovePointParser()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�倣狨� 煎萄и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 籀擠 賃還擊 Skip и棻.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ⑷營曖 嬪纂蒂 盪濰и棻.
	fgetpos( fp, &fposDataStart );

	// 賃偃曖 等檜顫陛 氈朝雖 撫棻.
	siShipMovePointNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken檜 NULL檜塭賊 綴還檜棻.
			if( pszToken != NULL )
				siShipMovePointNum++;
		}		
	}	

	// 虜擒 偃熱陛 0檜塭賊 FALSE蒂 葬欐и棻.
	if( siShipMovePointNum <= 0 )						goto	END;

	// �倣� ん檣攪蒂 棻衛 腎給萼棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 п薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltShipMovePointParser::Free()
{
	if(ptShipMovePoint)
	{
		delete [] ptShipMovePoint;
		ptShipMovePoint	=	NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪羹 ん檣お 偃熱蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltShipMovePointParser::GetPointNum()
{
	return siShipMovePointNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ん檣お 寡翮曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
POINT*	cltShipMovePointParser::GetPoint()
{
	return ptShipMovePoint;
}