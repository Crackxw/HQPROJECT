/********************************************************************************************
	파일명	:	OnlineFieldParser.cpp
	작성자	:	정 진 욱
	담장다  :   이 상 원
	작성일	:	2001.07.24
	수정일	:	2001.07.25
	내  용  :	이정표 정보를 텍스트에서 읽어 온다.
********************************************************************************************/


#include <GSL.h>
#include	<search.h>

#include	"Directives.h"
#include	"OnlineFieldParser.h"
#include	"BindJXFile.h"
#include	"utility.h"

//INT		qsort_CompareForMilePostSearch(const VOID *arg1, const VOID *arg2 );


OnlineFieldParser::OnlineFieldParser()
{
}

OnlineFieldParser::~OnlineFieldParser()
{
	Free();
}
/********************************************************************************************
	초기화
********************************************************************************************/
BOOL	OnlineFieldParser::Init( CHAR *pFileName ) 
{
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pToken = NULL;

	fp	=	BindJXFile::GetInstance()->GetFile( pFileName );
//	fp	=	fopen( pFileName, "r" );

	if( fp == NULL )						return FALSE;
	
	// 4줄은 그냥 넘어 간다.
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );

	
	MilepostHeader	m;
	while( fgets( szBuffer, 1024, fp ) )
	{
		ZeroMemory( &m, sizeof( m ) );

		pToken						=	strtok( szBuffer, "\t\n\r" );

		pToken						=	strtok( NULL, "\t\n\r" );							// X 좌표
		m.uiID	=   atoi( pToken ) << 16;


		pToken						=	strtok( NULL, "\t\n\r" );							// Y 좌표
		m.uiID  |=   atoi( pToken );

		pToken					        	=	strtok( NULL, "\t\n\r" );					// Map Index
		m.siNowMapIndex  |=  atoi( pToken );


		pToken						=	strtok( NULL, "\t\n\r" );							// 걍 넘겨

		pToken						=	strtok( NULL, "\t\n\r" );							// 이미지 파일 이름
		strcpy( m.szImageFileName, pToken );


		pToken						=	strtok( NULL, "\t\n\r" );							// BGM
		m.siBGM	=	atoi( pToken );

		
		pToken						=	strtok( NULL, "\t\n\r" );							// 텍스트
		pToken						=	strtok( NULL, "\t\n\r" );
		m.siText	=	atoi( pToken );

		efficientAddOrUpdata( _Milepost, m.uiID, m );
	}

	fclose( fp );

	return TRUE;
}

/********************************************************************************************
	총 표지판수를 리턴한다.
********************************************************************************************/
SI16	OnlineFieldParser::GetTotalNum()
{
	return _Milepost.size();
}

/********************************************************************************************
	이정표의 해당 인덱스를 받아 리턴해준다.
********************************************************************************************/
MilepostHeader	*OnlineFieldParser::GetMilepost( SI16 siIndex )
{
	Milepost::iterator i=_Milepost.begin();
	for( int ix=0; ix<_Milepost.size(); ++ix, ++i );
	return &i->second;
}

/********************************************************************************************
	이정표의 해당 인덱스를 받아 리턴해준다.
********************************************************************************************/
VOID	OnlineFieldParser::Free()
{
	_Milepost.clear();
}
/********************************************************************************************
	리소스의 인덱스를 설정한다.
********************************************************************************************/
BOOL	OnlineFieldParser::SetFile( CHAR *pFileName, SI16 siResFile )
{	
	for( Milepost::iterator i=_Milepost.begin(); i!= _Milepost.end(); ++i )
	{
		if( stricmp( i->second.szImageFileName, pFileName ) == 0 )
		{
			i->second.siFile	=	siResFile;
			return TRUE;
		}
	}

	return FALSE;
}
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	VillageSearch을 퀵소트하기 위한 함수이다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
INT		qsort_CompareForMilePostSearch(const VOID *arg1, const VOID *arg2 )
{
	MilepostHeader			*pMilepostHeader1;
	MilepostHeader			*pMilepostHeader2;

	pMilepostHeader1	=	(MilepostHeader*)arg1;
	pMilepostHeader2	=	(MilepostHeader*)arg2;
		
	if(pMilepostHeader1->uiID < pMilepostHeader2->uiID)
		return -1;
	else if(pMilepostHeader1->uiID > pMilepostHeader2->uiID)
		return 1;
	else
		return 0;
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	오브젝트의 인덱스를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MilepostHeader*	OnlineFieldParser::GetMilePost(SI16 siX, SI16 siY)
{
/*
	MilepostHeader		Data;
	MilepostHeader		*pFindData;


	Data.uiID	=	siX << 16 | siY;	
	pFindData	=	(MilepostHeader*)bsearch(&Data, pMilepostHeader, siTotal, sizeof(MilepostHeader), qsort_CompareForMilePostSearch);

	if(pFindData == NULL)
		return 0;
	else
		return pFindData;
*/
	UI32 uiID = siX << 16 | siY;
	Milepost::iterator find = _Milepost.lower_bound(uiID);

	
	if( find != _Milepost.end() && !(_Milepost.key_comp()(uiID, find->first)) )
	{
		return &find->second;
	}

	return NULL;
}
