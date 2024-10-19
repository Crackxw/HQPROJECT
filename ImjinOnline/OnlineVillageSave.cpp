/****************************************************************************************
	파 일 명 : OnlineVillageSave.cpp
	작 성 자 : 정진욱
	담 담 자 : 정진욱
	작 성 일 : 2001.11.12
	수 정 일 : 2002.03.28
	내    용 : 장부에서 마을에 관한 데이타를 파일에 저장하고 읽어 온다.
****************************************************************************************/

#include <GSL.h>

#include	<Main.h>
#include	<DebugTool.h>

#include	"OnlineWorld.h"
#include	"OnlineVillageSave.h"
#include	"OnlineMydata.h"
#include	"OnlineItem.h"

///////////////////////////////////
// STL
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;
///////////////////////////////////



#define		ON_SAVE_DIR						"Data"
#define		ON_SEARCH_FILE					"Search"

enum		{ ON_SAVE_VERSION };

INT		qsort_TradeBook(const VOID *arg1, const VOID *arg2 );
BOOL	qualityCompare(const ITemSearchTable &lhs, const ITemSearchTable &rhs);


OnlineVillageSave::OnlineVillageSave()
{
	m_pSaveHeader		=	NULL;	
	m_pSearchHeader		=	NULL;
	m_pSort				=	NULL;
	m_pItemIDTable		=	NULL;
	m_pAllSearch		=	NULL;
	m_pITemSearchTable	=	NULL;
	m_bSearch			=	FALSE;
	m_bError			=	FALSE;	
	m_bSuccess			=	FALSE;
	m_siVersion			=	ON_SAVE_VERSION;
	m_siCount			=	0;
	m_uiPrecent01		=	0;


	ZeroMemory( m_pTradeData, sizeof( m_pTradeData ) );
	ZeroMemory( m_pSearchData, sizeof( m_pSearchData ) );
}


OnlineVillageSave::~OnlineVillageSave()
{
	Free();
}

VOID	OnlineVillageSave::Initialize()
{
	m_bSearch			=	FALSE;
	m_bError			=	FALSE;	
	m_bSuccess			=	FALSE;
	m_siVersion			=	ON_SAVE_VERSION;
	m_siCount			=	0;
	m_uiPrecent01		=	0;
	ZeroMemory( m_pTradeData, sizeof( m_pTradeData ) );
	ZeroMemory( m_pSearchData, sizeof( m_pSearchData ) );

	Free();
}

VOID	OnlineVillageSave::Init( cltOnlineWorld *pMyOnlineWorld )
{
	FILE		*fp;
	CHAR		szDir[100], szTemp[10], szTemp1[50];	
	SI16		i;

	m_pMyOnlineWorld	=	pMyOnlineWorld;
	m_siTotalVillage	=	1;
	m_pSaveHeader		=	NULL;
	
	SI32	siAccount   = m_pMyOnlineWorld->pMyData->GetMyCharDBAccount();
	SI16	siSlot		= (SI16)m_pMyOnlineWorld->pMyData->GetSelectMyCharacterSlot();
	
	switch(g_stConnectServerInfo.m_WorldID)
	{
	case 1:
		sprintf( szTemp, "Data");
		sprintf( szDir, "%s\\%d%d", szTemp, siAccount, siSlot );
		sprintf( m_szPath, "Online\\%s",szDir );
		CreateDirectory( "Online\\Data", NULL );
		break;
	default:
		sprintf( szTemp, "Data%d", g_stConnectServerInfo.m_WorldID - 1);
		sprintf( szDir, "%s\\%d%d", szTemp, siAccount, siSlot );
		sprintf( m_szPath, "Online\\%s",szDir );

		sprintf( szTemp1, "Online\\%s", szTemp);
		CreateDirectory( szTemp1, NULL );
		break;
	}
	
	CreateDirectory( m_szPath, NULL );		

	sprintf( szDir, "%s\\%d", m_szPath, 00000000 );

	fp = fopen( szDir, "rb" );

	if( fp == NULL )		return;

	fread( &m_siTotalVillage, sizeof( SI32 ), 1, fp );

	// robypark 2004/9/23 10:21
	// 마을 정보 개수가 초과될 경우
	if (m_siTotalVillage >= COUNT_SEARCH_DATA)
	{
		fclose(fp);
		m_siTotalVillage = 0;
		Free();
		return;
	}

	m_pSaveHeader = new SAVEFILE_HEADER[ m_siTotalVillage ];
	fread( m_pSaveHeader, sizeof( SAVEFILE_HEADER ),  m_siTotalVillage, fp );
	fclose( fp );

	// 임시로 헤더 데이타를 받는다.
	SAVEFILE_HEADER *pTempHeader = new SAVEFILE_HEADER[ m_siTotalVillage ];	
	memcpy( pTempHeader, m_pSaveHeader, sizeof(SAVEFILE_HEADER) * m_siTotalVillage);

	// 장부 데이타를 읽어 온다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	BOOL	bDelete;
	SI16	TempTotalVillage = m_siTotalVillage;

	int j = 1;
	for( i = 1 ; i < TempTotalVillage; i++ )
	{
		bDelete = FALSE;

		sprintf( szDir, "%s\\%s", m_szPath, m_pSaveHeader[i].m_SaveFileName );
		fp = fopen( szDir, "rb" );

		if( fp )
		{
			// 날짜가 6개월을 안넘었나 검사한다.
			SAVE_DATE	Date;
			fread( &Date, sizeof( SAVE_DATE ), 1, fp );
			if(m_pSaveHeader[i].m_uiVillageCode >= 2000)
			{
				bDelete = TRUE;
				pTempHeader[i].m_uiVillageCode = 0;
			}
			else
			{
				// 파일에서 읽어 들인다.
				m_pTradeData[j].m_Date = Date;
				fread( &m_pTradeData[j].m_siItemNum, sizeof( SI16 ), 1, fp );			
				fread( m_pTradeData[j].m_pData, sizeof( SAVEFILE_DATA ), m_pTradeData[j].m_siItemNum, fp );
				
				m_pSearchData[j].m_uiKey	=	m_pSaveHeader[i].m_uiVillageCode;
				m_pSearchData[j].m_siIndex	=	j;
				j++;
			}
		}
		else
			break;

		fclose( fp );

		if(bDelete)
		{
			// 파일 삭제
			DeleteTrade();
			return;
/*			DeleteFile(szDir);
			m_siTotalVillage--;*/
		}
	}
	
	qsort((VOID *)m_pSearchData, m_siTotalVillage, sizeof(SASAVEFILE_SEARCH), qsort_TradeBook);

	// 헤더 변경(삭제된 파일 적용)
	if(m_pSaveHeader)	delete [] m_pSaveHeader;

	m_pSaveHeader = new SAVEFILE_HEADER[ m_siTotalVillage ];

	j = 1;
	for( i = 1 ; i < TempTotalVillage; i++ )
	{
		if(pTempHeader[i].m_uiVillageCode)
		{
			m_pSaveHeader[j].m_uiVillageCode	=	pTempHeader[i].m_uiVillageCode;
			sprintf( m_pSaveHeader[j].m_SaveFileName, "%d", pTempHeader[i].m_uiVillageCode );
			
			j++;
		}
	}

	// 헤더파일(0번파일) 세이브
	sprintf( szDir, "%s\\%d", m_szPath, 00000000 );
	fp = fopen( szDir, "wb" );
	if( fp == NULL )		return;
	
	fwrite( &j, sizeof( SI32 ), 1, fp );								// 먼저 총 몇개의 헤더 있는지 먼저 쓴다.		
	fwrite( m_pSaveHeader, sizeof( SAVEFILE_HEADER ),  j, fp );			// 다음 헤더를 파일에 쓴다.
	
	//qsort((VOID *)m_pSearchData, m_siTotalVillage, sizeof(SASAVEFILE_SEARCH), qsort_TradeBook);
	
	fclose( fp );

	// SAVEFILE_DATAHEADER(데이타 파일) 세이브
	for( i = 1 ; i < j; i++ )
	{
		sprintf( szDir, "%s\\%s", m_szPath, m_pSaveHeader[i].m_SaveFileName );
		
		fp = fopen( szDir, "wb" );
		
		if( fp == NULL )	break;	
		
		fwrite( &m_pTradeData[i].m_Date, sizeof( SAVE_DATE ), 1, fp );									// 갱신일
		fwrite( &m_pTradeData[i].m_siItemNum, sizeof( SI16 ), 1, fp );									// 마을에서 거래하는 총 아이템 갯수
		fwrite( m_pTradeData[i].m_pData, sizeof( SAVEFILE_DATA ), m_pTradeData[i].m_siItemNum, fp );	// 아이템의 관한 정보
		
		fclose( fp );
	}

	m_pSort	=	NULL;
	delete	[] pTempHeader;
}

VOID	OnlineVillageSave::Free()
{
	SI16	i;

	if( m_pSaveHeader )
	{
		delete []m_pSaveHeader;
		m_pSaveHeader	 =	NULL;
	}	

	if( m_pSearchHeader )
	{
		delete [] m_pSearchHeader;
		m_pSearchHeader = NULL;
	}

	if( m_pSort )
	{
		delete [] m_pSort;
		m_pSort			=	NULL;
	}

	if( m_pAllSearch )
	{
		delete [] m_pAllSearch;
		m_pAllSearch	=	NULL;
	}
	
	if( m_pItemIDTable )
	{
		for( i=0 ; i<m_siTotalVillage-1; i++ )
		{
			if( m_pItemIDTable[i].m_pItemTable )	delete [] m_pItemIDTable[i].m_pItemTable;
			m_pItemIDTable[i].m_pItemTable	=	NULL;
		}
		
		delete [] m_pItemIDTable;
		m_pItemIDTable = NULL;
	}

	if( m_pITemSearchTable )
	{
		delete [] m_pITemSearchTable;
		m_pITemSearchTable	=	NULL;
	}
}

BOOL	OnlineVillageSave::SaveInfo( UI16 uiVillageCode, SAVEFILE_DATAHEADER *pHeader )
{
	FILE		*fp;
	CHAR		szDir[100];


	SAVEFILE_HEADER		*pTempHeader;	

	if( uiVillageCode == -1 )				return FALSE;


	// 지금 같은 아이디가 있는지 알아 본다.
	m_siVillageIndex	=	GetVillageIndex(uiVillageCode);	

	// 만약 같은 마을이 없다면 헤더를 새로 만들어 파일에 저장을 다시 해준다.
	if( m_siVillageIndex == -1 )
	{
		if( m_pSaveHeader != NULL )
		{
			// robypark 2004/9/23 10:21
			// 더 이상 저장할 수 없다면(마을 정보 개수가 초과될 경우)
			if (m_siTotalVillage >= COUNT_SEARCH_DATA)
				return FALSE;

			pTempHeader			=	new SAVEFILE_HEADER[ m_siTotalVillage + 1 ];			// 임시로 모든 데이타를 저장을 한다.
			ZeroMemory( pTempHeader, sizeof( SAVEFILE_HEADER ) * (m_siTotalVillage + 1) );

			memcpy( pTempHeader, m_pSaveHeader, sizeof(SAVEFILE_HEADER) * m_siTotalVillage);

			if(m_pSaveHeader)
				delete [] m_pSaveHeader;													// 전에 데이타를 날리고 

			m_siVillageIndex	=	m_siTotalVillage;
			m_pSaveHeader		=	pTempHeader;											// 새로 만든 데이타로 바꾸어준다.
		}
		else
		{
			m_siTotalVillage	=	1;
			m_siVillageIndex	=	1;
			m_pSaveHeader		=	new SAVEFILE_HEADER[ m_siTotalVillage + 1 ];
			ZeroMemory( m_pSaveHeader, sizeof( SAVEFILE_HEADER ) * (m_siTotalVillage + 1) );
		}

		m_pSaveHeader[ m_siVillageIndex ].m_uiVillageCode	=	uiVillageCode;
		if(m_pSearchData)
		{
			m_pSearchData[ m_siVillageIndex ].m_uiKey			=	uiVillageCode;
			m_pSearchData[ m_siVillageIndex ].m_siIndex			=	m_siVillageIndex;
		}

		sprintf( m_pSaveHeader[ m_siVillageIndex ].m_SaveFileName, "%d", uiVillageCode );
		sprintf( szDir, "%s\\%d", m_szPath, 00000000 );

		fp = fopen( szDir, "wb" );
		if( fp == NULL )		return FALSE;

		m_siTotalVillage++;																// 총 수를 한개 올려 준다.
		fwrite( &m_siTotalVillage, sizeof( SI32 ), 1, fp );								// 먼저 총 몇개의 헤더 있는지 먼저 쓴다.		
		fwrite( m_pSaveHeader, sizeof( SAVEFILE_HEADER ),  m_siTotalVillage, fp );		// 다음 헤더를 파일에 쓴다.

		if(m_pSearchData)
			qsort((VOID *)m_pSearchData, m_siTotalVillage, sizeof(SASAVEFILE_SEARCH), qsort_TradeBook);

		fclose( fp );		
	}

	if(m_pTradeData)
	{
		m_pTradeData[ m_siVillageIndex ].m_Date			=	pHeader->m_Date;
		m_pTradeData[ m_siVillageIndex ].m_siItemNum	=	pHeader->m_siItemNum;
		memcpy( m_pTradeData[ m_siVillageIndex ].m_pData, pHeader->m_pData, sizeof( SAVEFILE_DATA ) * pHeader->m_siItemNum );
	}

	sprintf( szDir, "%s\\%s", m_szPath, m_pSaveHeader[ m_siVillageIndex ].m_SaveFileName );

	fp = fopen( szDir, "wb" );

	if( fp == NULL )	return FALSE;	

	fwrite( &pHeader->m_Date, sizeof( SAVE_DATE ), 1, fp );								// 갱신일
	fwrite( &pHeader->m_siItemNum, sizeof( SI16 ), 1, fp );								// 마을에서 거래하는 총 아이템 갯수
	fwrite( pHeader->m_pData, sizeof( SAVEFILE_DATA ), pHeader->m_siItemNum, fp );		// 아이템의 관한 정보

	fclose( fp );
	return TRUE;
}


SAVEFILE_DATAHEADER* OnlineVillageSave::GetData( UI16 uiVillageCode )
{
	if( uiVillageCode == -1 )			return NULL;
	
	SASAVEFILE_SEARCH		TempData;
	SASAVEFILE_SEARCH		*pFindData=NULL;


	TempData.m_uiKey	=	uiVillageCode;
	if(m_pSearchData)
		pFindData		=	(SASAVEFILE_SEARCH*)bsearch( &TempData, m_pSearchData, m_siTotalVillage, sizeof( SASAVEFILE_SEARCH ), qsort_TradeBook );

	if( pFindData == NULL )	return NULL;
	else					
	{
		if(m_pTradeData == NULL)	return NULL;
		else						return &m_pTradeData[pFindData->m_siIndex];
	}
}


SI32	OnlineVillageSave::GetVillageIndex( UI16 uiVillageCode )
{
	SI32	i;

	if( m_pSaveHeader == NULL )		return -1;


	for( i=1 ; i<m_siTotalVillage ; i++ )
	{
		if( m_pSaveHeader[i].m_uiVillageCode == uiVillageCode )
			return i;
	}

	return -1;
}


SI32	OnlineVillageSave::GetTotal()
{
	return m_siTotalVillage;
}


SAVEFILE_HEADER		*OnlineVillageSave::GetData()
{
	return m_pSaveHeader;
}

////////////////////////////////////////////////////////////////////////////
// 아이템이 파는 마을 리턴해 준다.
SAVEFILE_SEARCH		*OnlineVillageSave::Search( UI32 uiItemID, SI16 &siVillageNum )
{
	SI16	i, ix;	
	SI16	siSearchNum	=	0;

	SAVEFILE_DATAHEADER		*pTempHeaer;

	if( m_pSearchHeader )
	{
		delete [] m_pSearchHeader;
		m_pSearchHeader = NULL;
	}

	if( m_pSaveHeader == NULL )
	{
		siVillageNum	=	0;
		return NULL;
	}


	if( uiItemID == 0 )		return NULL;	


	m_pSearchHeader	=	new SAVEFILE_SEARCH[ m_siTotalVillage ];
	ZeroMemory( m_pSearchHeader, sizeof( SAVEFILE_SEARCH ) * m_siTotalVillage );

	for( i=1 ; i<m_siTotalVillage; i++ )
	{
		pTempHeaer	=	GetData( m_pSaveHeader[i].m_uiVillageCode );

		if( pTempHeaer )
		{		
			for( ix=0 ; ix<pTempHeaer->m_siItemNum; ix++ )
			{
				if( pTempHeaer->m_pData[ix].m_uiItemID == uiItemID )
				{
					m_pSearchHeader[siSearchNum].m_Date						=	pTempHeaer->m_Date;
					m_pSearchHeader[siSearchNum].m_uiVillageCode			=	m_pSaveHeader[i].m_uiVillageCode;
					m_pSearchHeader[siSearchNum].m_Data.m_siItemQuantity	=	pTempHeaer->m_pData[ix].m_siItemQuantity;
					m_pSearchHeader[siSearchNum].m_Data.m_uiBuyValue		=	pTempHeaer->m_pData[ix].m_uiBuyValue;
					m_pSearchHeader[siSearchNum].m_Data.m_uiItemID			=	pTempHeaer->m_pData[ix].m_uiItemID;
					m_pSearchHeader[siSearchNum].m_Data.m_uiSellValue		=	pTempHeaer->m_pData[ix].m_uiSellValue;

					siSearchNum++;
					break;
				}
			}
		}
	}

	siVillageNum	=	siSearchNum;
	return m_pSearchHeader;
}

////////////////////////////////////////////////////////////////////////////
// 아이템을 정렬해서 가장 이득이 많이 나는 순으로 정렬을 해준다.
SAVEFILE_SORT		*OnlineVillageSave::SortSearch( UI32 uiItemID, SI16 &siVillageNum )
{
	siVillageNum	=	0;

	if( uiItemID == 0 )		return NULL;
	if( m_pSort == NULL )	return NULL;
	
	SI16			i;
	SAVEFILE_SORT	TempSort[100];
	SAVEFILE_SORT	*pTemp = NULL;
	

	for( i=0; i<m_siSearchNum; i++ )
	{
		if( m_pSort[i].m_uiItemID == uiItemID )
		{
			TempSort[siVillageNum].m_Date		=	m_pSort[i].m_Date;
			TempSort[siVillageNum].m_nBuy		=	m_pSort[i].m_nBuy;
			TempSort[siVillageNum].m_nSell		=	m_pSort[i].m_nSell;
			TempSort[siVillageNum].m_nProfit	=	m_pSort[i].m_nProfit;

			TempSort[siVillageNum].m_uiBuyVillageCode	=	m_pSort[i].m_uiBuyVillageCode;
			TempSort[siVillageNum].m_uiVillageCode		=	m_pSort[i].m_uiVillageCode;
			TempSort[siVillageNum].m_uiItemID			=	m_pSort[i].m_uiItemID;

			siVillageNum++;
		}
	}

	if( siVillageNum == 0 )		return NULL;

	pTemp = TempSort;
	return pTemp;
}

////////////////////////////////////////////////////////////////////////////
// 아이템을 정렬해서 가장 이득이 많이 나는 순으로 정렬을 해준다. ( 전체 검색 )
SAVEFILE_SORT	*OnlineVillageSave::SortSearch( SI16 &siSearchNum )
{
	if( !m_bSearch )
	{
		if( m_pSaveHeader == NULL )		
		{
			siSearchNum	=	-2;
			return NULL;
		}
		
		if(m_pITemSearchTable) 		delete [] m_pITemSearchTable;
		
		m_pITemSearchTable	=	new ITemSearchTable[ m_pMyOnlineWorld->pItemsInfo->GetCount() ];
		ZeroMemory( m_pITemSearchTable, sizeof( ITemSearchTable ) * m_pMyOnlineWorld->pItemsInfo->GetCount() );
		
		
		m_bSearch			=	TRUE;
		m_bSuccess			=	FALSE;
		m_siAllCount		=	1;
		m_uiPrecent01		=	0;
	}	
	else if( m_siAllCount >= m_siTotalVillage )														// 검색 완료
	{
		SI16 i,siValue;
		
		typedef vector<ITemSearchTable> IntVector;
		typedef IntVector::iterator IntVectorIt;
		IntVectorIt start, end, it ;

		IntVector	m_TempSort(m_pMyOnlineWorld->pItemsInfo->GetCount());
		
		m_bSearch	=	FALSE;		
		m_bSuccess	=	TRUE;

		if( m_pSort )	delete [] m_pSort;
		
		m_siAllCount = 0;

		// 데이타를 너어 준다.
		for( i=0; i<m_pMyOnlineWorld->pItemsInfo->GetCount(); i++ )
		{
			m_TempSort[i] = m_pITemSearchTable[i];
			if(m_pITemSearchTable[i].m_siProfit > 0)	m_siAllCount++;
		}

		siValue		=	m_siAllCount > 10 ? 10 : m_siAllCount;

		m_siAllCount = 0;

		m_pSort	=	new SAVEFILE_SORT[ siValue ];
		start	=	m_TempSort.begin();
		end		=	m_TempSort.end();

		// 소트를 siValue 만큼 해준다.
		partial_sort( start, start + siValue, end, qualityCompare );

		for(it = start; m_siAllCount<siValue ; it++)
		{
			m_pSort[m_siAllCount].m_Date	= it->m_Date;
			m_pSort[m_siAllCount].m_nBuy	= it->m_uiBuy;
			m_pSort[m_siAllCount].m_nProfit	= it->m_siProfit;
			m_pSort[m_siAllCount].m_nSell	= it->m_uiSell;

			m_pSort[m_siAllCount].m_uiBuyVillageCode	=	it->m_uiBuyVillageCode;
			m_pSort[m_siAllCount].m_uiItemID			=	it->m_uiItemID;
			m_pSort[m_siAllCount].m_uiVillageCode		=	it->m_uiSellVillageCode;
			m_siAllCount++;
		}
		
		siSearchNum	=	siValue;
		m_TempSort.clear();
		delete [] m_pITemSearchTable;
		m_pITemSearchTable = NULL;		
		return m_pSort;
	}
	else																									// 검색
	{	
		SI16	i;
		UI16	uiIndex;
		SAVEFILE_DATAHEADER	*pTempData;				

		pTempData		=	GetData( m_pSaveHeader[m_siAllCount].m_uiVillageCode );
		m_uiPrecent01	=	(UI08)m_siAllCount;	

		if( pTempData )
		{
			SOnlineTime		Time;

			Time = m_pMyOnlineWorld->pTimer->GetNowTime();

			SI16 Difference = (Time.Year * 12 + Time.Month) - (pTempData->m_Date.m_siYear * 12 + pTempData->m_Date.m_siMonth);
			if( Difference < 6 )
			{			
				for( i=0 ; i<pTempData->m_siItemNum; i++ )
				{			
					uiIndex	=	pTempData->m_pData[i].m_uiItemID;						
					
					if( uiIndex < m_pMyOnlineWorld->pItemsInfo->GetCount() )
					{
						m_pITemSearchTable[uiIndex].m_uiItemID	=	pTempData->m_pData[i].m_uiItemID;
						
						// 테이블에 저장되어 있는 값보다 더 싸다면 새로운 가격을 추가해 준다.
						if( m_pITemSearchTable[uiIndex].m_uiBuy > pTempData->m_pData[i].m_uiBuyValue )
						{
							m_pITemSearchTable[uiIndex].m_uiBuy				= pTempData->m_pData[i].m_uiBuyValue;
							m_pITemSearchTable[uiIndex].m_uiBuyVillageCode	= m_pSaveHeader[m_siAllCount].m_uiVillageCode;
						}
						else if( m_pITemSearchTable[uiIndex].m_uiBuy == 0 )
						{
							m_pITemSearchTable[uiIndex].m_uiBuy				= pTempData->m_pData[i].m_uiBuyValue;
							m_pITemSearchTable[uiIndex].m_uiBuyVillageCode	= m_pSaveHeader[m_siAllCount].m_uiVillageCode;
						}
						
						if( m_pITemSearchTable[uiIndex].m_uiSell < pTempData->m_pData[i].m_uiSellValue )
						{
							m_pITemSearchTable[uiIndex].m_uiSell			= pTempData->m_pData[i].m_uiSellValue;
							m_pITemSearchTable[uiIndex].m_uiSellVillageCode	= m_pSaveHeader[m_siAllCount].m_uiVillageCode;
						}			
						else if( m_pITemSearchTable[uiIndex].m_uiSell == 0 )
						{
							m_pITemSearchTable[uiIndex].m_uiSell			= pTempData->m_pData[i].m_uiSellValue;
							m_pITemSearchTable[uiIndex].m_uiSellVillageCode	= m_pSaveHeader[m_siAllCount].m_uiVillageCode;
						}
						
						// 날짜를 구해 준다.
						if( m_pITemSearchTable[uiIndex].m_Date.m_siYear == 0 )
							m_pITemSearchTable[uiIndex].m_Date	=	pTempData->m_Date;
						
						else if( m_pITemSearchTable[uiIndex].m_Date.m_siYear > pTempData->m_Date.m_siYear )
							m_pITemSearchTable[uiIndex].m_Date	=	pTempData->m_Date;
						
						else if( m_pITemSearchTable[uiIndex].m_Date.m_siYear == pTempData->m_Date.m_siYear )
						{
							if( m_pITemSearchTable[uiIndex].m_Date.m_siMonth > pTempData->m_Date.m_siMonth )
								m_pITemSearchTable[uiIndex].m_Date	=	pTempData->m_Date;
							
							else if( m_pITemSearchTable[uiIndex].m_Date.m_siMonth == pTempData->m_Date.m_siMonth )
							{
								if( m_pITemSearchTable[uiIndex].m_Date.m_siDay > pTempData->m_Date.m_siDay )
									m_pITemSearchTable[uiIndex].m_Date	=	pTempData->m_Date;
								
								else if( m_pITemSearchTable[uiIndex].m_Date.m_siDay	==	pTempData->m_Date.m_siDay )
								{
									if( m_pITemSearchTable[uiIndex].m_Date.m_siMonth > pTempData->m_Date.m_siHour )
										m_pITemSearchTable[uiIndex].m_Date	=	pTempData->m_Date;
								}
							}
						}
						
						// 이익을 구한다.
						m_pITemSearchTable[uiIndex].m_siProfit = m_pITemSearchTable[uiIndex].m_uiSell - m_pITemSearchTable[uiIndex].m_uiBuy;
						m_pITemSearchTable[uiIndex].m_siProfit = (SI32)((float)m_pITemSearchTable[uiIndex].m_siProfit / (float)m_pITemSearchTable[uiIndex].m_uiBuy * 100.0 );
					}
				}
			}
		}

		m_siAllCount++;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////
// 검색 중지
VOID	OnlineVillageSave::StopSearch()
{
	SI16 i;

	if( m_pSort )
	{
		delete []m_pSort;
		m_pSort = NULL;
	}

	if( m_pItemIDTable )
	{
		for( i=0 ; i<m_siTotalVillage-1; i++ )
		{
			if( m_pItemIDTable[i].m_pItemTable )	delete [] m_pItemIDTable[i].m_pItemTable;
			m_pItemIDTable[i].m_pItemTable	=	NULL;
		}

		delete [] m_pItemIDTable;
		m_pItemIDTable = NULL;
	}

	m_siAllCount	= 0;
	m_siCount		= 0;
	m_uiSortItemID	= 0;
	m_bSearch		= FALSE;	
	m_bError		= FALSE;
}

////////////////////////////////////////////////////////////////////////////
// 검색한 내용을 파일로 저장해 준다.
VOID	OnlineVillageSave::SearchSave( SAVEFILE_SORT *pSort, SI16 siNum )
{
	FILE	*fp;
	CHAR	szTemp[ 100 ];

	if( siNum == 0 )	return;
	if( pSort == NULL)	return;

	sprintf( szTemp, "%s\\%s", m_szPath, ON_SEARCH_FILE );

	fp	=	fopen( szTemp, "wb" );
	
	fwrite( &siNum, sizeof( SI16 ), 1, fp );
	fwrite( pSort, sizeof( SAVEFILE_SORT ), siNum, fp );	

	if( m_pAllSearch )		delete []m_pAllSearch;

	m_pAllSearch	=	NULL;
	m_pAllSearch	=	new SAVEFILE_SORT[ siNum ];
	m_siSearchNum	=	siNum;

	memcpy( m_pAllSearch, pSort, sizeof( SAVEFILE_SORT ) * siNum );

	fclose( fp );
}

////////////////////////////////////////////////////////////////////////////
// 검색 되어 있는 데이타를 리턴을 해준다.
SAVEFILE_SORT	*OnlineVillageSave::GetSearchData( SI16 &siVillageNum )
{
	FILE	*fp;
	CHAR	szTemp[ 100 ];

	if( m_pAllSearch == NULL )
	{
		wsprintf( szTemp, "%s\\%s", m_szPath, ON_SEARCH_FILE );

		fp	=	fopen( szTemp, "rb" );

		if( fp )
		{
			fread( &siVillageNum, sizeof( SI16 ), 1, fp );

			m_pAllSearch = new SAVEFILE_SORT[ siVillageNum ];

			fread( m_pAllSearch, sizeof( SAVEFILE_SORT ), siVillageNum, fp );

			m_siAllCount = siVillageNum;

			fclose( fp );
		}
	}

	siVillageNum	=	m_siSearchNum;
	return m_pAllSearch;
}

////////////////////////////////////////////////////////////////////////////
// 새로운 데이타가 들어 오면 검색을 해준 데이타와 비교를 한다.
VOID	OnlineVillageSave::CheckSearchItem( UI16 uiVillageCode )
{
	SI16	i, j;
	BOOL	bSort = FALSE;

	SAVEFILE_DATAHEADER *pData = GetData( uiVillageCode );
	SAVEFILE_SORT		TempData;


	if(pData == NULL)			return;
	if(m_pAllSearch == NULL)	return;

	// 먼저 검색한 데이타 중에 이 마을이 있뜸 비교를 한다.
	for( i=0 ; i<m_siSearchNum; i++ )
	{		
		if( m_pAllSearch[i].m_uiBuyVillageCode == uiVillageCode )				// 사는 아이템 
		{
			for( j=0 ; i<pData->m_siItemNum; j++ )
			{
				// 만약 동일한 아이템 이라면....
				if( pData->m_pData[j].m_uiItemID == m_pAllSearch[i].m_uiItemID )
				{
					// 날짜를 업데이트해 주고
					m_pAllSearch[i].m_Date		= pData->m_Date;
					m_pAllSearch[i].m_nSell		= pData->m_pData[j].m_uiSellValue;
					m_pAllSearch[i].m_nProfit	= (__int64)( ( (double)( m_pAllSearch[i].m_nSell - m_pAllSearch[i].m_nBuy ) / (double)m_pAllSearch[i].m_nBuy ) * 100.0 );
					bSort						= TRUE;
					break;
				}
			}
		}
		else if( m_pAllSearch[i].m_uiVillageCode == uiVillageCode )				// 파는 아이템
		{
			for( j=0 ; i<pData->m_siItemNum; j++ )
			{
				// 만약 동일한 아이템 이라면....
				if( pData->m_pData[j].m_uiItemID == m_pAllSearch[i].m_uiItemID )
				{
					// 날짜를 업데이트해 주고
					m_pAllSearch[i].m_Date		= pData->m_Date;
					m_pAllSearch[i].m_nBuy		= pData->m_pData[j].m_uiBuyValue;
					m_pAllSearch[i].m_nProfit	= (__int64)( ( (double)( m_pAllSearch[i].m_nSell - m_pAllSearch[i].m_nBuy ) / (double)m_pAllSearch[i].m_nBuy ) * 100.0 );
					bSort						= TRUE;
					break;
				}
			}

		}
		
	}

	if( bSort )
	{
		for( i=0 ; i<m_siSearchNum; i++ )
		{
			for( j=1 ; j<m_siSearchNum - i; j++)
			{
				if( ( m_pAllSearch[j].m_nProfit > m_pAllSearch[j-1].m_nProfit ) && ( m_pAllSearch[j].m_uiItemID != 0 ) && ( m_pAllSearch[j-1].m_uiItemID != 0 ) )
				{
					memcpy( &TempData, &m_pAllSearch[j-1], sizeof( SAVEFILE_SORT ) );
					memcpy( &m_pAllSearch[j-1], &m_pAllSearch[j], sizeof( SAVEFILE_SORT ) );
					memcpy( &m_pAllSearch[j], &TempData, sizeof( SAVEFILE_SORT ) );
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// 장부 파일을 지운다.
BOOL	OnlineVillageSave::DeleteTrade()
{
	HANDLE	hFind;
	SI32 siCount = 0;
	WIN32_FIND_DATA lpfd;
	ZeroMemory(&lpfd, sizeof(LPWIN32_FIND_DATA));
	CHAR findfilename[256];

	sprintf(findfilename, "%s\\*", m_szPath);
	hFind = FindFirstFile(findfilename, &lpfd);

	if( hFind == INVALID_HANDLE_VALUE ) return FALSE;

	while(1)
	{
		sprintf(findfilename, "%s\\%s", m_szPath, lpfd.cFileName);
		DeleteFile(findfilename);

		if( FindNextFile(hFind, &lpfd) == 0 ) break;
	}

	FindClose(hFind);

	if(m_pSaveHeader)	
	{
		delete [] m_pSaveHeader;
		m_pSaveHeader = NULL;
	}

	//m_pSaveHeader = new SAVEFILE_HEADER[ m_siTotalVillage ];

	if(m_pTradeData)
		ZeroMemory( m_pTradeData, sizeof( m_pTradeData ) );
	if(m_pSearchData)
		ZeroMemory( m_pSearchData, sizeof( m_pSearchData ) );
/*	for( int i = 1 ; i < m_siTotalVillage; i++ )
	{
		m_pSaveHeader[j].m_uiVillageCode	=	0;
	}
*/
	return TRUE;
}

// 현재 몇 % 로딩을 했는지 알아 온다.
VOID	OnlineVillageSave::GetLoadingPercent( CHAR *pTemp )
{
	if( m_bSuccess == FALSE )
		sprintf( pTemp, "%d %%", (UI08)(((float)m_uiPrecent01 / ((float)m_siTotalVillage - 1))*100.0) );
}

INT		qsort_TradeBook(const VOID *arg1, const VOID *arg2 )
{
	SASAVEFILE_SEARCH	*pData1, *pData2;

	pData1	=	(SASAVEFILE_SEARCH*)arg1;
	pData2	=	(SASAVEFILE_SEARCH*)arg2;

	if(pData1 == NULL || pData2 == NULL)			return 0;

	if( pData1->m_uiKey < pData2->m_uiKey )			return -1;
	else if( pData1->m_uiKey > pData2->m_uiKey )	return 1;
	else											return 0;
}


BOOL	qualityCompare(const ITemSearchTable &lhs, const ITemSearchTable &rhs)
{
	if( lhs.m_siProfit > rhs.m_siProfit )	return TRUE;
	else									return FALSE;
}