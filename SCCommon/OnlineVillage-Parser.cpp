/********************************************************************************************
	파일명	:	OnlineVillage-Parser.cpp
	작성자	:	정 진 욱
	담당자  :   이 상 원
	작성일	:	2001.06.22
	수정일	:	2001.11.16
	내  용  :	마을 정보를 텍스트에서 읽어온다.
********************************************************************************************/

#include <GSL.h>

#include	"OnlineVillage-Parser.h"
#include	"GSCDefine.h"
#include	"BindJxFile.h"
     
OnlineVillageParser::OnlineVillageParser()
{

}

OnlineVillageParser::~OnlineVillageParser()
{
	Free();
}
/**************************************************************************************************
	함수명 : ReadText
	내  용 : 텍스트 이름을 받어서 마을 구조체에 넣어 준다.
**************************************************************************************************/
BOOL OnlineVillageParser::ReadText( char *pFileName, char *pFileStruct )
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	CHAR	*pTemp  = NULL;
	char	szBuffer[ 1024 ];	

	SI16	ix;
	char	seps[] = "\n\t\r";

	if( ( fp = BindJXFile::GetInstance()->GetFile( pFileName ) ) == NULL )	return FALSE;

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );


	VillageHeader	TempVillageHeader;	
	ZeroMemory( &TempVillageHeader, sizeof( VillageHeader ));


	while( fgets( szBuffer, 1024, fp ) )
	{
		// 도시이름
		pToken = strtok( szBuffer, seps );

		if( pToken == NULL )	break;

		pToken = strtok( NULL, seps );
		pToken = strtok( NULL, seps );
		TempVillageHeader.siVillageName		= atoi( pToken );
		
		pToken = strtok( NULL, seps );
		TempVillageHeader.uiVillageCode		= atoi( pToken );

		
		// 도시 좌표 X
		pToken = strtok( NULL, seps );
		TempVillageHeader.siX	=	atoi( pToken );

		// 도시 좌표 Y
		pToken = strtok( NULL, seps );
		TempVillageHeader.siY	=	atoi( pToken );

		// Map Index
		pToken = strtok( NULL, seps );
		TempVillageHeader.siMapIndex = atoi( pToken );

		pToken = strtok( NULL, seps );
		TempVillageHeader.uiPop	= atoi( pToken );

		pToken = strtok( NULL, seps );
		TempVillageHeader.uiMinPop	=	atoi( pToken );

		pToken = strtok( NULL, seps );
		TempVillageHeader.uiMaxPop	=	atoi( pToken );
 
		// Config file
		pToken = strtok( NULL, seps );
		ZeroMemory( TempVillageHeader.szConfig, sizeof( TempVillageHeader.szConfig ) );
		TempVillageHeader.siType	=	0;
		
		if( pToken != NULL )		
		{			
			switch(pToken[0])
			{
			case 'K':	TempVillageHeader.uiNation = ON_COUNTRY_KOREA;	break;
			case 'J':	TempVillageHeader.uiNation = ON_COUNTRY_JAPAN;	break;
			case 'C':	TempVillageHeader.uiNation = ON_COUNTRY_CHINA;	break;
			case 'T':	TempVillageHeader.uiNation = ON_COUNTRY_TAIWAN;	break;
			}

			// 사이즈를 얻어온다.
			switch(pToken[1])
			{
			case 'S':
				TempVillageHeader.siSize	=	5;
				TempVillageHeader.szSize = 'S';
				break;
			case 'M':
				TempVillageHeader.siSize	=	5;
				TempVillageHeader.szSize = 'M';
				break;
			case 'L':
				TempVillageHeader.siSize	=	5;
				TempVillageHeader.szSize = 'L';
				break;
			}

			pTemp = ( pToken + 1 );

			// 타입을 얻는다.
			TempVillageHeader.siType = GetType( pTemp );
			wsprintf( TempVillageHeader.szConfig, "%s.Spr", pToken+4 );
		}

		// 이미지 파일 이름
		pToken = strtok( NULL, seps );		
		strcpy( TempVillageHeader.szImageFile, pToken );
		
		// BGM
		pToken = strtok( NULL, seps );

		// 아이템은 총 20 이다..
		for( ix = 0; ix < ON_VILLAGE_MAX_ITEM ; ix++)
		{
			pToken = strtok( NULL, seps );
			TempVillageHeader.uiItemID[ ix ] = atoi( pToken );
			pToken = strtok( NULL, seps );
			TempVillageHeader.siQuantity[ ix ] = atoi( pToken );
			pToken = strtok( NULL, seps );

			if( atoi( pToken ) == 0 )			TempVillageHeader.siProperty[ ix ] = ON_VILLAGE_CONSUMPTION;
			else								TempVillageHeader.siProperty[ ix ] = ON_VILLAGE_PRODUCTION;
		}

		// 마을 건물은 총 10 이다.
		for( ix = 0; ix < ON_VILLAGE_MAX_BUILDING ; ix++)
		{
			pToken = strtok( NULL, seps );
			ZeroMemory( TempVillageHeader.szVillage[ ix ], sizeof( TempVillageHeader.szVillage[ ix ] ) );
			TempVillageHeader.siStructureKinds[ix]		=	ON_VIL_ST_KIND_UNKNOWN;
			ZeroMemory(TempVillageHeader.szStructureKinds[ix], sizeof(TempVillageHeader.szStructureKinds[ix]));

			if( pToken[0] != '0' )
			{
				strcpy( TempVillageHeader.szVillage[ ix ], pToken);
				TempVillageHeader.siStructureKinds[ix]	=	 GetVillageKind( pToken );
				GetStructKind(TempVillageHeader.siStructureKinds[ix], *TempVillageHeader.szStructureKinds[ix]);
			}			
			
			pToken = strtok( NULL, seps );
		}
		
		// 성문 파일 이름
		pToken = strtok( NULL, seps );
		ZeroMemory( TempVillageHeader.szGateImageFileName, sizeof( TempVillageHeader.szGateImageFileName ) );
		strcpy( TempVillageHeader.szGateImageFileName, pToken );

		m_VillageHeader.push_back( TempVillageHeader );
	}

	sort( m_VillageHeader.begin(), m_VillageHeader.end() );

	fclose( fp );

	if( pFileStruct != NULL )		return LoadType( pFileStruct );
	else							return TRUE;
}

/**************************************************************************************************
	함수명 : GetType
	내  용 : 마을의 타입을 리턴해준다
**************************************************************************************************/
SI16 OnlineVillageParser::GetType( char *pToken )
{
	SI16	siType;
	char	szTemp, szTemp2[2];
	SI16	siTemp;

	szTemp	= pToken[0];
	szTemp2[0] = pToken[1];
	szTemp2[1] = pToken[2];

	siTemp = atoi( szTemp2 );

	siType = szTemp;
	siType = ( ( siType << 8 ) & 0xff00 ) | ( siTemp & 0x00ff );

	return siType;
}
/**************************************************************************************************
	함수명 : GetType
	내  용 : 마을의 타입을 리턴해준다
**************************************************************************************************/
BOOL OnlineVillageParser::LoadType( char *pFileStruct )
{
	FILE	*fp		=	NULL;
	CHAR	*pToken	=	NULL;
	CHAR	szBuffer[1024];
	SI16	siCnt	=	0;
	SI16	i, siTypeNum;
	

	if((fp = BindJXFile::GetInstance()->GetFile( pFileStruct )) == NULL)	
		return FALSE;

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );

	// 총 타입수를 얻는다.
	pToken		=	strtok( szBuffer, "\n\t\r" );
	pToken		=	strtok( NULL, "\n\t\r" );
	siTypeNum	=	atoi( pToken );

	m_VillageType.reserve(siTypeNum);

	fgets( szBuffer, 1024, fp );

	// 구조체를 생성함.
	VillageType		TempVillageType;	

	while(  fgets( szBuffer, 1024, fp ) != NULL )
	{
		ZeroMemory( &TempVillageType, sizeof( VillageType ) );
		// 타입 이름
		pToken						=	strtok( szBuffer, "\n\t\r" );
		TempVillageType.siType		=	GetType( pToken );

		pToken						=	strtok( NULL, "\n\t\r" );
		TempVillageType.siStateCode	=	ON_COUNTRY_NONE;
		if(strcmp(pToken, "K") == 0)
		{
			TempVillageType.siStateCode	=	ON_COUNTRY_KOREA;
		}
		else if(strcmp(pToken, "J") == 0)
		{
			TempVillageType.siStateCode	=	ON_COUNTRY_JAPAN;
		}
		else if(strcmp(pToken, "C") == 0)
		{
			TempVillageType.siStateCode	=	ON_COUNTRY_CHINA;
		}
		else if(strcmp(pToken, "T") == 0)
		{
			TempVillageType.siStateCode	=	ON_COUNTRY_TAIWAN;
		}

		for( i=0 ; i<ON_VILLAGE_MAX_BUILDING ; i++)
		{
			// 건물들이 좌표
			pToken							=	strtok( NULL, "\n\t\r" );
			TempVillageType.Xpos[i]	=	atoi( pToken );

			pToken							=	strtok( NULL, "\n\t\r" );
			TempVillageType.Ypos[i]	=	atoi( pToken );
		}

		// 나가기 버튼 위치
		pToken						=	strtok( NULL, "\n\t\r" );
		TempVillageType.ExitXpos	=	atoi( pToken );
		
		pToken						=	strtok( NULL, "\n\t\r" );
		TempVillageType.ExitYpos	=	atoi( pToken );
		
		m_VillageType.push_back( TempVillageType );
	}

	fclose( fp );

	return TRUE;
}

/**************************************************************************************************
	함수명 : GetVillageKind
	내  용 : 건물의 종류를 알아낸다.
**************************************************************************************************/
SI16	OnlineVillageParser::GetVillageKind( CHAR *pFileName )
{
	if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_BARRACK, 3) == 0)			return ON_VIL_ST_KIND_BARRACK;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_MARKET, 3) == 0)		return ON_VIL_ST_KIND_MARKET;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_BANK, 3) == 0)			return ON_VIL_ST_KIND_BANK;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_HOSPITAL, 3) == 0)		return ON_VIL_ST_KIND_HOSPITAL;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_INN, 3) == 0)			return ON_VIL_ST_KIND_INN;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_WHARF, 3) == 0)			return ON_VIL_ST_KIND_WHARF;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_FARM, 3) == 0)			return ON_VIL_ST_KIND_FARM;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_GOVERNMENT, 3) == 0)	return ON_VIL_ST_KIND_GOVERNMENT;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_WSTORE, 3) == 0)		return ON_VIL_ST_KIND_WSTORE;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_DSTORE, 3) == 0)		return ON_VIL_ST_KIND_DSTORE;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_FSTORE, 3) == 0)		return ON_VIL_ST_KIND_FSTORE;	
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_WEAPON, 3) == 0)		return ON_VIL_ST_KIND_WEAPON;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_ARMOR, 3) == 0)			return ON_VIL_ST_KIND_ARMOR;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_PASTURE, 3) == 0)		return ON_VIL_ST_KIND_PASTURE;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_FACTORY, 3) == 0)		return ON_VIL_ST_KIND_FACTORY;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_GBARRACK, 3) == 0)		return ON_VIL_ST_KIND_GBARRACK;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_MBARRACK, 3) == 0)		return ON_VIL_ST_KIND_MBARRACK;
	else if(strnicmp(pFileName, ON_VIL_ST_KIND_CODE_RING, 3) == 0)			return ON_VIL_ST_KIND_RING;
	else																	
	{
		MessageBox( NULL, "Unknow Struct Code", pFileName, MB_OK | MB_TOPMOST );
		return ON_VIL_ST_KIND_UNKNOWN;
	}
}

// 건물종류를 문자열로 바꿔준다.
BOOL	OnlineVillageParser::GetStructKind(SI32 siKind, CHAR &szKind)
{
	switch( siKind )
	{
	case ON_VIL_ST_KIND_BARRACK:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_BARRACK );
		break;
	case ON_VIL_ST_KIND_MARKET:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_MARKET );
		break;
	case ON_VIL_ST_KIND_BANK:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_BANK );
		break;
	case ON_VIL_ST_KIND_HOSPITAL:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_HOSPITAL );
		break;
	case ON_VIL_ST_KIND_INN:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_INN );
		break;
	case ON_VIL_ST_KIND_WHARF:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_WHARF );
		break;
	case ON_VIL_ST_KIND_FARM:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_FARM );
		break;
	case ON_VIL_ST_KIND_GOVERNMENT:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_GOVERNMENT );
		break;
	case ON_VIL_ST_KIND_WSTORE:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_WSTORE );
		break;
	case ON_VIL_ST_KIND_DSTORE:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_DSTORE );
		break;
	case ON_VIL_ST_KIND_FSTORE:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_FSTORE );
		break;
	case ON_VIL_ST_KIND_RING:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_RING );
		break;
	case ON_VIL_ST_KIND_WEAPON:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_WEAPON );
		break;
	case ON_VIL_ST_KIND_ARMOR:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_ARMOR );
		break;
	case ON_VIL_ST_KIND_PASTURE:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_PASTURE );
		break;
	case ON_VIL_ST_KIND_FACTORY:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_FACTORY );
		break;
	case ON_VIL_ST_KIND_GBARRACK:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_GBARRACK );
		break;
	case ON_VIL_ST_KIND_MBARRACK:
		strcpy( &szKind, ON_VIL_ST_KIND_CODE_MBARRACK );
		break;
	default:
			return FALSE;
	}
	return TRUE;
}

VillageHeader *OnlineVillageParser::GetVillageHeaderCode(UI16 uiCode)
{
	VillageHeader Temp;
	Temp.uiVillageCode = uiCode;	
	VWlterPair p = equal_range( m_VillageHeader.begin(), m_VillageHeader.end(), Temp );
	return p.first != p.second ? p.first : NULL;
}

BOOL	OnlineVillageParser::IsStructure(SI32 HeaderIndex, UI16 code)
{
	if( code == 0 ) return FALSE;
	for(int i = 0; i < ON_VILLAGE_MAX_BUILDING; i++) 
		if( m_VillageHeader[HeaderIndex].siStructureKinds[i] == code )		return TRUE;
	return FALSE;
}

VillageType *OnlineVillageParser::GetType( SI16 siType, SI16 StateCode)
{
	for( vector<VillageType>::iterator i=m_VillageType.begin() ; i != m_VillageType.end(); i++)
		if((i->siType == siType) && (i->siStateCode == StateCode))			return i;
	return NULL;
}
