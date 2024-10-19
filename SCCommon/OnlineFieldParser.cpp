/********************************************************************************************
	���ϸ�	:	OnlineFieldParser.cpp
	�ۼ���	:	�� �� ��
	�����  :   �� �� ��
	�ۼ���	:	2001.07.24
	������	:	2001.07.25
	��  ��  :	����ǥ ������ �ؽ�Ʈ���� �о� �´�.
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
	�ʱ�ȭ
********************************************************************************************/
BOOL	OnlineFieldParser::Init( CHAR *pFileName ) 
{
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pToken = NULL;

	fp	=	BindJXFile::GetInstance()->GetFile( pFileName );
//	fp	=	fopen( pFileName, "r" );

	if( fp == NULL )						return FALSE;
	
	// 4���� �׳� �Ѿ� ����.
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

		pToken						=	strtok( NULL, "\t\n\r" );							// X ��ǥ
		m.uiID	=   atoi( pToken ) << 16;


		pToken						=	strtok( NULL, "\t\n\r" );							// Y ��ǥ
		m.uiID  |=   atoi( pToken );

		pToken					        	=	strtok( NULL, "\t\n\r" );					// Map Index
		m.siNowMapIndex  |=  atoi( pToken );


		pToken						=	strtok( NULL, "\t\n\r" );							// �� �Ѱ�

		pToken						=	strtok( NULL, "\t\n\r" );							// �̹��� ���� �̸�
		strcpy( m.szImageFileName, pToken );


		pToken						=	strtok( NULL, "\t\n\r" );							// BGM
		m.siBGM	=	atoi( pToken );

		
		pToken						=	strtok( NULL, "\t\n\r" );							// �ؽ�Ʈ
		pToken						=	strtok( NULL, "\t\n\r" );
		m.siText	=	atoi( pToken );

		efficientAddOrUpdata( _Milepost, m.uiID, m );
	}

	fclose( fp );

	return TRUE;
}

/********************************************************************************************
	�� ǥ���Ǽ��� �����Ѵ�.
********************************************************************************************/
SI16	OnlineFieldParser::GetTotalNum()
{
	return _Milepost.size();
}

/********************************************************************************************
	����ǥ�� �ش� �ε����� �޾� �������ش�.
********************************************************************************************/
MilepostHeader	*OnlineFieldParser::GetMilepost( SI16 siIndex )
{
	Milepost::iterator i=_Milepost.begin();
	for( int ix=0; ix<_Milepost.size(); ++ix, ++i );
	return &i->second;
}

/********************************************************************************************
	����ǥ�� �ش� �ε����� �޾� �������ش�.
********************************************************************************************/
VOID	OnlineFieldParser::Free()
{
	_Milepost.clear();
}
/********************************************************************************************
	���ҽ��� �ε����� �����Ѵ�.
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
//	����	:	VillageSearch�� ����Ʈ�ϱ� ���� �Լ��̴�.
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
//	����	:	������Ʈ�� �ε����� ���´�.
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
