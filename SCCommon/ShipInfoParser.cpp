// 氬 渡 濠 : 檜 鼻 錳
#include <GSL.h>
#include "ShipInfoParser.h"
#include "bindJXFile.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipInfoParser::cltShipInfoParser()
{
	siTotalShipNum	=	0;
	pstShipInfo		=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltShipInfoParser::~cltShipInfoParser()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	�倣狨� 煎萄и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	// 籀擠 賃還擊 Skip и棻.	
	for( i = 0; i < 5; i++ )
	{
		if( fgets(szReadBuffer, 1024, fp) == NULL )		goto END;
	}

	// ⑷營曖 嬪纂蒂 盪濰и棻.
	fgetpos( fp, &fposDataStart );

	// 賃偃曖 等檜顫陛 氈朝雖 撫棻.
	siTotalShipNum	=	0;
	while(1)
	{
		ZeroMemory( szReadBuffer, sizeof(szReadBuffer) );

		if( fgets(szReadBuffer, 1024, fp) == NULL )		break;
		else
		{
			pszToken =	strtok( szReadBuffer, cSeps );

			// pszToken檜 NULL檜塭賊 綴還檜棻.
			if( pszToken != NULL )
				siTotalShipNum++;
		}		
	}	

	// 虜擒 偃熱陛 0檜塭賊 FALSE蒂 葬欐и棻.
	if( siTotalShipNum <= 0 )						goto	END;

	// �倣� ん檣攪蒂 棻衛 腎給萼棻.
	fsetpos( fp, &fposDataStart );

	pstShipInfo		=	new strShipInfo[siTotalShipNum];

	for(i = 0; i < 	siTotalShipNum; i++)
	{
		if(fgets(szReadBuffer, 1023, fp) == NULL) goto END;
		pszToken						=	strtok(szReadBuffer, cSeps);		// ID
		pstShipInfo[i].siID				=	GetID(pszToken);

		pszToken						=	strtok(NULL, cSeps);		// Name擎 斜傖 蝶霾и棻.

		pszToken						=	strtok(NULL, cSeps);		// Name擎 斜傖 蝶霾и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 п薯и棻.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltShipInfoParser::Free()
{
	if(pstShipInfo)
	{
		delete [] pstShipInfo;
		pstShipInfo	=	NULL;
	}

	pstShipInfo	=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	寡曖 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strShipInfo*	cltShipInfoParser::GetShipInfo(SI32 siIndex)
{
	return &pstShipInfo[siIndex];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	識 寡曖 熱蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltShipInfoParser::GetTotalShipNum()
{
	return siTotalShipNum;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	 臢蝶お縑憮 2夥檜お ID擊 虜萇棻.(釭醞縑 OnlineChar-Parser 諦 偽檜 鱔мп撿 й 睡碟檜棻.)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltShipInfoParser::GetID( CHAR *pToken )
{
	UI16	uiVal;

	uiVal		=	MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}