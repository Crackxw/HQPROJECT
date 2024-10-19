#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"
#include "DBGateQueue.h"
#include "DBSubQueue.h"
#include "DBQueryMgr.h"
#include "SODatabase.h"
#include "QuestManager.h"


//----------------------------
#include "SOMain.h"
#include "SOCommonHeader.h"
#include "MSSQLDB.h"
#include "SOMap.h"
#include "SOVillage.h"
#include "IGuild.h"
#include "SODBStructure.h"
//----------------------------

//----------------------------
#include "AuctionList.h"
#include "AuctionMarket.h"
#include "PlayerList.h"
#include "ExpoRank.h"
#include "SOGMList.h"
//----------------------------



//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 생성자 
//-----------------------------------------------------------------------------------------------------------------------------
SODatabase::SODatabase()
{	
	m_bConnect = FALSE;

	InitializeCriticalSection( &m_cs1 );
	InitializeCriticalSection( &m_cs2 );
	InitializeCriticalSection( &m_cs3 );

	m_countWrite = 0;
	m_countGetDB = 0;
}



//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 소멸자 
//-----------------------------------------------------------------------------------------------------------------------------
SODatabase::~SODatabase()
{
	DeleteCriticalSection( &m_cs1 );
	DeleteCriticalSection( &m_cs2 );
	DeleteCriticalSection( &m_cs3 );
}



//-----------------------------------------------------------------------------------------------------------------------------
//	name :	
//  desc : 보관함에 쿼리를 쌓아 둔다 
//-----------------------------------------------------------------------------------------------------------------------------
void  SODatabase::writeDBQuery(int command, void* pIn, int pInSize)
{
	DBQUERY_HEADER		header;	
	memset( &header, 0, sizeof(header) );

	header.command		= command;
	header.packetSize	= sizeof(DBQUERY_HEADER) + pInSize;

	//----------------------------------------------------------------------
	writeToDepository( (char*)&header, sizeof(header), (char*)pIn, pInSize );							  

	m_countWrite++;
}



//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 데이타 베이스에 접속 한다 
//-----------------------------------------------------------------------------------------------------------------------------
BOOL SODatabase::ConnectDatabase()
{		
	printf("[%s], [%s], [%s]\n", g_GameServerInfoParser.GetDB1IPString(), g_GameServerInfoParser.GetDBName1String(), g_GameServerInfoParser.GetDBUserString());
	if(SQLDB.DBConnect(g_GameServerInfoParser.GetDB1IPString(), g_GameServerInfoParser.GetDBName1String(), 
						g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd") == true)
	{
		printf("---\tDATABASE Connected!!\t---\n");
		m_bConnect = TRUE;
		return TRUE;
	}
	else 
		printf("########## DATABASE Connecting Error! ###########\n");


	return FALSE;
}


//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 데이타 베이스에 접속 한다 
//-----------------------------------------------------------------------------------------------------------------------------
BOOL SODatabase::ConnectDatabase2()
{		
	printf("[%s], [%s], [%s]\n", g_GameServerInfoParser.GetDB2IPString(), g_GameServerInfoParser.GetDBName2String(), g_GameServerInfoParser.GetDBUserString());
	if(SQLDB.DBConnect(g_GameServerInfoParser.GetDB2IPString(), g_GameServerInfoParser.GetDBName2String(), 
						g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd") == true)
	{
		printf("---\tDATABASE Connected!!\t---\n");
		m_bConnect = TRUE;
		return TRUE;
	}
	else 
		printf("########## DATABASE Connecting Error! ###########\n");


	return FALSE;
}


//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 캐릭터 생성 삭제 관련(LOGIN) 
//-----------------------------------------------------------------------------------------------------------------------------

BOOL	SODatabase::GetDBInfo3( SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver )
{
	if( m_bConnect == FALSE ) return FALSE;	
	
	EnterCriticalSection( &m_cs3 );

	BOOL	bSuccess = TRUE;

	switch( siInfoKind ) 
	{
	case SO_LOGIN_DB_NEWCHAR:
		{

			sDB_NewChar *pNewChar = (sDB_NewChar *)pIn;
			int	*pRet = (int *)pOut;

			*pRet = 0;

			pNewChar->name[ LOGIN::NAME_MAX_LENGTH - 1 ] = NULL;

			if( SQLDB.SQLRun( "EXEC SPHQ_On2CreateCharacter %d, %d, '%s ', %d, %d, %d, %d, %d, %d", 
				pNewChar->dbAccount, pNewChar->slot, pNewChar->name, pNewChar->kind, 
				pNewChar->Str, pNewChar->Dex, pNewChar->Vit, pNewChar->Int, ON_MAXGENERAL_LOW ) == false ) {
				
				bSuccess = FALSE;
				
				break;
			}

			if( SQLDB.DBResults() == SUCCEED ) {

				SQLDB.DBBind( 1, INTBIND, (DBINT)0,	(BYTE *)pRet );
				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS ) {					
				}
			}

		}
		break;

	case SO_LOGIN_DB_DELCHAR:
		{

			sDB_DelChar *pDelChar = (sDB_DelChar *)pIn;
			int	*pRet = (int *)pOut;

			*pRet = 0;

			if( SQLDB.SQLRun( "EXEC SPHQ_On2DelCharacter %d, %d", 
				pDelChar->dbAccount, pDelChar->slot ) == false ) {
				
				bSuccess = FALSE;
				break;
			}

			if( SQLDB.DBResults() == SUCCEED ) {

				SQLDB.DBBind( 1, INTBIND, (DBINT)0,	(BYTE *)pRet );
				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS ){
				}
			}

		}
		break;

	case SO_LOGIN_DB_CHARSINFO:
		{
			
			sDB_CharsInfo *pCharsInfo = (sDB_CharsInfo *)pIn;
			SOPlayer *pPlayer = (SOPlayer *)pOut;

			char	buf[ 64 ];
			int		slot, kind, level, credit;
			
			DBNUMERIC				money;
			memset(&money,  0, sizeof(DBNUMERIC));

			if( SQLDB.SQLRun( "EXEC SPHQ_On3GetCharsInfo %d", pCharsInfo->dbAccount ) == false ) {
				
				bSuccess = FALSE;
				break;
			}			

			slot = 0; kind = 0; level = 0; credit = 0;

			if( SQLDB.DBResults() == SUCCEED ) {

				SQLDB.DBBind( 1, TINYBIND,			0,		( BYTE* )&slot );
				SQLDB.DBBind( 2, NTBSTRINGBIND,		0,		( BYTE* )buf );
				SQLDB.DBBind( 3, SMALLBIND,			0,		( BYTE* )&kind );				
				SQLDB.DBBind( 4, SRCNUMERICBIND,	0,		( BYTE* )&money );
				SQLDB.DBBind( 5, INTBIND,			0,		( BYTE* )&credit );				
				SQLDB.DBBind( 6, TINYBIND,			0,		( BYTE* )&level );								
		
				pPlayer->clearCharsInfo();

				memset( pPlayer->m_charsInfo, 0, sizeof( LOGIN::sCharInfo ) * LOGIN::CHAR_MAX_NUM );

				while ( SQLDB.DBNextRow() != NO_MORE_ROWS ) {					

					if( slot < 1 || slot > LOGIN::CHAR_MAX_NUM ) continue;
					
					LOGIN::sCharInfo *pCharInfo = &pPlayer->m_charsInfo[ slot - 1 ];

					pCharInfo->slot = slot;

					memcpy( pCharInfo->name, buf, LOGIN::NAME_MAX_LENGTH );
					pCharInfo->name[ LOGIN::NAME_MAX_LENGTH - 1 ] = NULL;

					pCharInfo->kind = kind;

					memcpy(&pCharInfo->mnMoney, money.val, sizeof(MONEY));					
					pCharInfo->credit	=	credit;
					pCharInfo->level	=	level;
					
				}
			}			
		}
		break;
	}

	SQLDB.ClearCMDBuf();
	SQLDB.ClearRows();

	LeaveCriticalSection( &m_cs3 );

	return bSuccess;
}



//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : ITEM BANK 관련 
//-----------------------------------------------------------------------------------------------------------------------------
BOOL	SODatabase::GetDBInfo2( SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver )
{
	if( m_bConnect == FALSE ) return FALSE;

	EnterCriticalSection( &m_cs2 );

	BOOL	bSuccess = TRUE;

	switch( siInfoKind )
	{
	case SO_DB_INFOKIND_GETITEMCOUNT_ITEMBANK:
		{

			DBIKPlayerDBInfo *pPlayerInfo = (DBIKPlayerDBInfo*)	pIn;

			UI16 *pResult = (UI16 *)pOut;
			UI32 uiCount;

			*pResult = 0;

			if( SQLDB.SQLRun( "EXEC SPHQ_OnCountByItemBank %d",  pPlayerInfo->DBAccount  ) == false ) {
				
				bSuccess = FALSE;
				break;
			}

			if( SQLDB.DBResults() == SUCCEED ) {

				SQLDB.DBBind( 1, INTBIND, (DBINT)0,	(BYTE *)&uiCount );
				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS ) {

					*pResult = (UI16)uiCount;
				}
			}

			break;
		}

	case SO_DB_INFOKIND_ITEMLIST_ITEMBANK:
		{

			DBIKPlayerDBInfo *pPlayerInfo = (DBIKPlayerDBInfo*)	pIn;
			
			UI32 uiItemID, uiQuantity;
			UI16 uiCount;
			UI16 *pList = (UI16 *)pOut;

			pList[ 0 ] = 0;

			if( SQLDB.SQLRun( "EXEC SPHQ_OnListByItemBank %d",  pPlayerInfo->DBAccount ) == false ) {
				bSuccess = FALSE;
				break;
			}	


			if( SQLDB.DBResults() == SUCCEED ) {

				SQLDB.DBBind( 1, INTBIND, (DBINT)0,	(BYTE *)&uiItemID );
				SQLDB.DBBind( 2, INTBIND, (DBINT)0,	(BYTE *)&uiQuantity );
			
				uiCount = 0;

				while ( SQLDB.DBNextRow() != NO_MORE_ROWS ) {

					pList[ uiCount * 2 + 1 ] = (UI16)uiItemID;
					pList[ uiCount * 2 + 2 ] = (UI16)uiQuantity;
					uiCount++;
				}

				pList[ 0 ] = uiCount;
			}

		}
		break;

	}

	
	SQLDB.ClearCMDBuf();
	SQLDB.ClearRows();

	LeaveCriticalSection( &m_cs2 );

	return bSuccess;
}


 
//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : DB에서 정보를 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------------
BOOL	SODatabase::GetDBInfo(SI32 siInfoKind, LPVOID pIn, LPVOID pOut, UI16 Receiver)
{
	if( m_bConnect == FALSE ) return FALSE;

	EnterCriticalSection( &m_cs1 );

	BOOL		bSuccess = TRUE;

	
	switch(siInfoKind)
	{		
	// (서버 시작)
	// 10 년 동안 연간 마을의 수익(시전, 은행, 훈련소, 의원, 항구, 주막, 생산건물(농장, 공장, 목장))을 얻는다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETVILLAGECAPITAL:
		{
			SOVillage		   *pVillage = (SOVillage*)	pIn;;
			
			DBNUMERIC			TempNumeric[ON_MAX_PROFIT_SORT];						
			memset(TempNumeric, 0, sizeof(DBNUMERIC)*ON_MAX_PROFIT_SORT);


			if( SQLDB.SQLRun("EXEC SPHQ_OnVillageInterestGet %d", pVillage->uiVillageCode) == false )
			{				
				bSuccess = FALSE;
				break;
			}

			SI32				i=0,j=0;
			SI16				siYear;

			if( SQLDB.DBResults() == SUCCEED )
			{
				SQLDB.DBBind ( 1, SMALLBIND,	(DBSMALLINT)	 0,	(BYTE *)&siYear );
								
				for( i=0; i < ON_MAX_PROFIT_SORT; i++ )
				{				
					SQLDB.DBBind( i+2, SRCNUMERICBIND, 0, (BYTE*)&TempNumeric[i] );
				}
								
				while( SQLDB.DBNextRow() != NO_MORE_ROWS )
				{
					pVillage->VillageCapital[j].Year = siYear;
					
					for( i=0; i < ON_MAX_PROFIT_SORT; i++ )
					{					
						memcpy(&pVillage->VillageCapital[j].Profit[i], TempNumeric[i].val, sizeof(MONEY));
					}					

					memset(TempNumeric, 0, sizeof(DBNUMERIC)*ON_MAX_PROFIT_SORT);
					j++;
				}
			}
		}
		break;


	// 게임에서 연도가 변경 될 경우 유저가 얻을 연간 수익을 얻는다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETADDMONEY:
		{
			DBIKPlayerDBInfo *pPlayerInfo = (DBIKPlayerDBInfo*)	pIn;
						
			DBNUMERIC				TempNumeric;
			memset(&TempNumeric,  0, sizeof(DBNUMERIC));			

			
			if( SQLDB.SQLRun("EXEC SPHQ_OnGetYearInvestProfit %d, %u", pPlayerInfo->DBAccount, pPlayerInfo->uiSlot) == false )
			{				
				bSuccess	=	FALSE;
				break;
			}

			if( (SQLDB.DBResults()) == SUCCEED )
			{
				SQLDB.DBBind (1, SRCNUMERICBIND,	  0,	(BYTE *)&TempNumeric);
							
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
								
				memcpy(pOut,   TempNumeric.val,  sizeof(MONEY));								
			}
		}
		break;
	
	// 통장 만들기 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_OPENSAVINGACCOUNT:
		{
			DBIKOpenSavingAccountInfo	*pOpenSavingAccountInfo;							
			DBIKRetOpenSavingAccount	*pRetOpenSavingAccount;

			pOpenSavingAccountInfo	=	(DBIKOpenSavingAccountInfo*)pIn;
			pRetOpenSavingAccount	=	(DBIKRetOpenSavingAccount*)pOut;


			if(SQLDB.SQLRun("EXEC SPHQ_On2CreateBankbook %d, %d, %d, %d, %I64d, %d", 
							pOpenSavingAccountInfo->uiVillageCode, 
							pOpenSavingAccountInfo->uiAccountKind,
							pOpenSavingAccountInfo->uiCharacterDBAccount,
							pOpenSavingAccountInfo->uiCharacterSlot,
							pOpenSavingAccountInfo->siFirstDepositMoney,
							pOpenSavingAccountInfo->siInterest) == false)
			{				
				bSuccess = FALSE;
				break;
			}
	

			SI32 siAccountIndex;
			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, INTBIND, (DBINT) 0, (BYTE *)&siAccountIndex);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				if(siAccountIndex >= 1)
				{
					// 성공적으로 통장을 개설하였다.
					pRetOpenSavingAccount->pAccountNumber->uiVillageCode	=	pOpenSavingAccountInfo->uiVillageCode;
					pRetOpenSavingAccount->pAccountNumber->uiAccountKind	=	pOpenSavingAccountInfo->uiAccountKind;
					pRetOpenSavingAccount->pAccountNumber->uiUserDBAccount	=	pOpenSavingAccountInfo->uiCharacterDBAccount;
					pRetOpenSavingAccount->pAccountNumber->uiCharSlot		=	pOpenSavingAccountInfo->uiCharacterSlot;
					pRetOpenSavingAccount->pAccountNumber->uiAccountIndex	=	siAccountIndex;					
					pRetOpenSavingAccount->siErrorRet						=	ON_RET_BANK_OK;
				}					
				else
				{
					switch(siAccountIndex)
					{
					// 더 이상의 통장 개설 안됨
					case 0:						
						pRetOpenSavingAccount->siErrorRet	=	ON_RET_BANK_FULLSAVINGACCOUNT;						
						break;

					// 해당 통장이 이미 만들어져 있다.						
					case -1:						
						pRetOpenSavingAccount->siErrorRet	=	ON_RET_BANK_EXISTOPENSAVINGACCOUNT;												
						break;

					// 한 플레이어가 더 이상 계좌를 만들 수가 없다.						
					case -2:						
						pRetOpenSavingAccount->siErrorRet	=	ON_RET_BANK_FULLTOTALSAVINGACCOUNT;						
						break;
										
					default:
						pRetOpenSavingAccount->siErrorRet	=	ON_RET_BANK_NO;						
						break;
					}				
				}
			}					
		}
		break;

	
	// 연간 전장 수익을 얻어온다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BANKCAPITAL_CALCULATE:
		{
			SOVillage				*pVillage;
			DBNUMERIC				TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));

			pVillage	=	(SOVillage*)pIn;

			if(SQLDB.SQLRun("EXEC SPHQ_OnVillageTradeCapitalCalculate %d", pVillage->uiVillageCode) == false)
			{				
				bSuccess	=	FALSE;
				break;
			}
			if ((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, SRCNUMERICBIND,	  0,	(BYTE *)&TempNumeric);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(&pVillage->CurrentCapital->BankProfit, TempNumeric.val,sizeof(MONEY));
			}
		}
		break;


	// (서버 시작)
	// 마을이 가진 아이템의 수량을 얻어온다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEMARKETSELLITEM:
		{
			SI32	i = 0;
			SI32	siItemID;
			SI32	siItemQuantity;			
			
			UI16	*puiVillageCode;
			SI32	*psiItemQuantityInfo;			

			puiVillageCode				=	(UI16 *)pIn;
			psiItemQuantityInfo			=	(SI32 *)pOut;			

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetVillageItem %d", *puiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}			

			if ((SQLDB.DBResults()) == SUCCEED) 
			{								
				SQLDB.DBBind (1, INTBIND,	(DBINT)	 0,	(BYTE *)&siItemID);			// item id
				SQLDB.DBBind (2, INTBIND,	(DBINT)  0,	(BYTE *)&siItemQuantity);	// 수량
					
				while(SQLDB.DBNextRow() != NO_MORE_ROWS)
				{				
					psiItemQuantityInfo[i * 2]		=	siItemID;					
					psiItemQuantityInfo[i * 2 + 1]	=	UI16( siItemQuantity );										
					++i;				
				}				
			}
		}		
		break;


	// (서버 시작)
	// 마을의 생산건물의 갯수등을 얻어온다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_GETINFO:
		{			
			SOVillage_IndustrialComplexs	*pIndustrialComplexs;
			UI08							uiPlantID;
			CHAR							szOwnerName[(ON_ID_LENGTH + 2)];
			UI32							uiIndex;
			SI32							siDBAccount;
			UI08							uiDBSlot;			
			UI08							uiAvailSlotNum;			
			SI32							siDayNotWorked;		
			UI16							uiVillageCode;

			uiVillageCode			=	UI16( pIn );
			pIndustrialComplexs		=	( SOVillage_IndustrialComplexs* )pOut; 	
			
			if(SQLDB.SQLRun( "EXEC SPHQ_OnGetFarmInfo %d", uiVillageCode ) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiPlantID);
				SQLDB.DBBind (2, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)szOwnerName);
				SQLDB.DBBind (3, INTBIND,	(DBINT)	     0,	(BYTE *)&uiIndex);					
				SQLDB.DBBind (4, INTBIND,	(DBINT)	     0,	(BYTE *)&siDBAccount);				
				SQLDB.DBBind (5, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiDBSlot);
				SQLDB.DBBind (6, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiAvailSlotNum);
				SQLDB.DBBind (7, INTBIND,	(DBINT)		 0,	(BYTE *)&siDayNotWorked);

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pIndustrialComplexs->InitIndustrialComplexFromDB( uiIndex, siDBAccount, uiDBSlot, szOwnerName, uiAvailSlotNum, siDayNotWorked );

					/*
					pFarm->Farm[uiIndex].nIndex			=	uiIndex;
					pFarm->Farm[uiIndex].siOwnerDBAcnt	=	siDBAccount;
					pFarm->Farm[uiIndex].uiOwnerSlot	=	uiDBSlot;
					pFarm->Farm[uiIndex].siWorkDay		=	siWorkDay;
					strcpy(pFarm->Farm[uiIndex].FarmInfo.szOwner, szOwnerName);
					pFarm->Farm[uiIndex].FarmInfo.uiAvailSlot		=	uiAvailSlotNum;
					*/
				}
			}
		}
		break;

	
	// (서버 시작)
	// 생산시설에 저장된 아이템의 갯수를 얻어온다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_STORAGE_GETDATA:
		{
			//SOVillage						*pVillage;
			UI08							uiPlantID;
			stFarmSlot						FarmSlot;						
			

			UI16							uiVillageCode;	
			SOVillage_IndustrialComplexs	*pIndustrialComplexs;			
			UI08							uiIndustrialComplexID;
			UI08							uiFactoryID;
			UI08							uiLevel;
			SI16							siPay;
			UI32							uiAmountOfWork;
			UI16							uiItemID0, uiItemID1, uiItemID2, uiItemID3, uiItemID4, 
											uiItemID5, uiItemID6, uiItemID7, uiItemID8, uiItemID9;
			UI16							uiItemQuantity0, uiItemQuantity1, uiItemQuantity2, uiItemQuantity3, uiItemQuantity4, 
											uiItemQuantity5, uiItemQuantity6, uiItemQuantity7, uiItemQuantity8, uiItemQuantity9;

			
			uiVillageCode			=	UI16( pIn );
			pIndustrialComplexs		=	( SOVillage_IndustrialComplexs* )pOut;

			//pVillage = (SOVillage*)pIn;			
			//if( pVillage->m_pFactory->GetFactories() == NULL ) break;

//			pFacility =	pVillage->clWorld->clBuilding.GetFacility( "STG" );						// 'STG'는 창고를 의미.
			
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetStorageData %u", uiVillageCode ) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				FarmSlot.Init();

				SQLDB.DBBind (1, TINYBIND,	(DBTINYINT)  0,	(BYTE *)&uiPlantID);				// farm_index
				SQLDB.DBBind (2, TINYBIND,	(DBTINYINT)  0,	(BYTE *)&uiIndustrialComplexID);	// 산업 단지의 ID
				SQLDB.DBBind (3, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiFactoryID);				// 산업 단지의 공장의 ID
				SQLDB.DBBind (4, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiLevel);					// Level
				SQLDB.DBBind (5, INTBIND,	(DBINT)		 0,	(BYTE *)&siPay);					// 비용
				SQLDB.DBBind (6, INTBIND,	(DBINT)		 0,	(BYTE *)&uiAmountOfWork);			// 노동
				// 이민수 수정 - small int type bind -> int type bind(2004.01.28)
				SQLDB.DBBind (7, INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID0);
				SQLDB.DBBind (8, INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID1);
				SQLDB.DBBind (9, INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID2);
				SQLDB.DBBind (10,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID3);
				SQLDB.DBBind (11,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID4);
				SQLDB.DBBind (12,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID5);
				SQLDB.DBBind (13,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID6);
				SQLDB.DBBind (14,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID7);
				SQLDB.DBBind (15,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID8);
				SQLDB.DBBind (16,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemID9);
				SQLDB.DBBind (17,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity0);
				SQLDB.DBBind (18,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity1);
				SQLDB.DBBind (19,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity2);
				SQLDB.DBBind (20,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity3);
				SQLDB.DBBind (21,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity4);
				SQLDB.DBBind (22,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity5);
				SQLDB.DBBind (23,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity6);
				SQLDB.DBBind (24,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity7);
				SQLDB.DBBind (25,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity8);
				SQLDB.DBBind (26,INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemQuantity9);
				
				while(SQLDB.DBNextRow() != NO_MORE_ROWS)
				{					
					pIndustrialComplexs->InitStorageFromDB( uiIndustrialComplexID, uiFactoryID, 
														uiLevel, siPay, uiAmountOfWork,
														uiItemID0, uiItemID1, uiItemID2, uiItemID3, uiItemID4, 
														uiItemID5, uiItemID6, uiItemID7, uiItemID8, uiItemID9,
														uiItemQuantity0, uiItemQuantity1, uiItemQuantity2, uiItemQuantity3, uiItemQuantity4, 
														uiItemQuantity5, uiItemQuantity6, uiItemQuantity7, uiItemQuantity8, uiItemQuantity9 );
														

					
/*
					if((pFarm = pVillage->m_pFactory->GetFactories()->GetFarm(uiIndex)) != NULL)
					{
						FarmSlot.pFacility		=	pFacility;
						FarmSlot.nSlotIndex		=	uiSlot;
						FarmSlot.bStore			=	TRUE;
						FarmSlot.pFarm			=	pFarm;

						if(FarmSlot.siPay > 0)
						{
							FarmSlot.bWorkable		=	TRUE;
							FarmSlot.ProductItem	=	ON_PLANT_SLOTUPGRADE_CODE;
							FarmSlot.uiRemainLabor	=	pFacility->UpLabor[FarmSlot.uiLevel] - FarmSlot.uiLabor;
							pFarm->FarmInfo.uiRemainLabor	+= FarmSlot.uiRemainLabor;
						}
						else
						{
							FarmSlot.bWorkable		=	FALSE;
							FarmSlot.ProductItem	=	0;
							FarmSlot.uiRemainLabor	=	0;
						}
						
						if(pFarm->GetFarmSlot(uiSlot))
							memcpy(pFarm->GetFarmSlot(uiSlot), &FarmSlot, sizeof(stFarmSlot));
					}
*/
				}
			}
			else
				bSuccess	=	FALSE;
		}
		break;


	// (서버 시작)
	// 생산 건물의 상세 정보를 얻어 온다 
	//------------------------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_FARM_GETDATA:	
		{
			stFarmSlot						FarmSlot;
			UI08							uiPlantID;
			
			UI16							uiVillageCode;
			SOVillage_IndustrialComplexs	*pIndustrialComplexs;			
			UI32							uiIndustrialComplexID;
			UI08							uiFactoryID;
			CHAR							cFactoryCode[4];
			UI08							uiLevel;
			SI16							siPay;
			UI32							uiAmountOfWork;			
			UI08							uiEquipment0Level, uiEquipment1Level, uiEquipment2Level, uiEquipment3Level, uiEquipment4Level;
			UI16							uiProducingItemID;
			UI32							uiProducedItemQuantity;			// 생산된 아이템.
			UI32							uiMaxProduceItemQuantity;		// 생산해야 할 최대 아이템의 수.

			

			uiVillageCode			=	UI16( pIn );
			pIndustrialComplexs		=	( SOVillage_IndustrialComplexs* )pOut;

			//pVillage				=	(SOVillage*)	pIn;
			//ZeroMemory( StrucCode, sizeof(StrucCode) );		
			

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetFarmData %d", uiVillageCode ) == false)
			{				
				bSuccess = FALSE;
				break;
			}
			
			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				FarmSlot.Init();
				SQLDB.DBBind (1, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiPlantID );					//slot	
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&uiIndustrialComplexID );		// 산업 단지의 ID
				SQLDB.DBBind (3, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiFactoryID );					// 산업 단지의 공장의 ID
				SQLDB.DBBind (4, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)cFactoryCode );
				SQLDB.DBBind (5, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiLevel);
				SQLDB.DBBind (6, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiEquipment0Level );
				SQLDB.DBBind (7, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiEquipment1Level );
				SQLDB.DBBind (8, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiEquipment2Level );
				SQLDB.DBBind (9, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiEquipment3Level );
				SQLDB.DBBind (10, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&uiEquipment4Level );
				SQLDB.DBBind (11, INTBIND,	(DBINT)		 0,	(BYTE *)&uiProducingItemID );
				SQLDB.DBBind (12, INTBIND,	(DBINT)		 0,	(BYTE *)&uiProducedItemQuantity );
				SQLDB.DBBind (13, INTBIND,	(DBINT)		 0,	(BYTE *)&uiMaxProduceItemQuantity );
				SQLDB.DBBind (14, SMALLBIND,(DBSMALLINT) 0,	(BYTE *)&siPay );
				SQLDB.DBBind (15, INTBIND,	(DBINT)		 0,	(BYTE *)&uiAmountOfWork );
				
				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pIndustrialComplexs->InitFactoryFromDB( uiIndustrialComplexID, uiFactoryID, uiLevel, siPay, uiAmountOfWork, cFactoryCode, 
														uiProducingItemID, uiProducedItemQuantity, uiMaxProduceItemQuantity);
														

/*
					pFarm					=	pVillage->m_pFactory->GetFactories()->GetFarm(uiIndex);
					if( pFarm )
					{
						FarmSlot.nSlotIndex			=	uiSlot;
						FarmSlot.pFacility			=	pVillage->clWorld->clBuilding.GetFacility(StrucCode);
						FarmSlot.bStore				=	FALSE;
						FarmSlot.Structure.siRate	=	0;
						FarmSlot.pFarm				=	pFarm;

						if( FarmSlot.siPay > 0 )
						{
							FarmSlot.bWorkable		=	TRUE;
							
							if( FarmSlot.ProductItem	==	ON_PLANT_SLOTUPGRADE_CODE)
							{
								FarmSlot.uiRemainLabor	=	FarmSlot.pFacility->UpLabor[FarmSlot.uiLevel] - FarmSlot.uiLabor;
							}
							else
							{
								FarmSlot.uiRemainLabor	=	
									pVillage->GetLaborByQuantity(FarmSlot.ProductItem, (FarmSlot.Structure.uiFinalNum-FarmSlot.Structure.uiNum)) - FarmSlot.uiLabor;
							}
							
							pFarm->FarmInfo.uiRemainLabor	+= FarmSlot.uiRemainLabor;
						}
						else
						{
							FarmSlot.bWorkable		=	FALSE;
							FarmSlot.ProductItem	=	0;
							FarmSlot.uiRemainLabor	=	0;
						}
						memcpy( pFarm->GetFarmSlot( uiSlot ), &FarmSlot, sizeof(stFarmSlot) );
						ZeroMemory( StrucCode, sizeof(StrucCode) );

					}
					*/
				}
			}
			else
				bSuccess	=	FALSE;
		}
		break;


	// 통장 갯수를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BANKBOOK_NUM:
		{			
			SI32							*siReturn;
			SOPlayer						*pPlayer;
						
			pPlayer						=	(SOPlayer *)	pIn;
			siReturn					=	(SI32*)	pOut;
						
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBankbookCount %d, %u",	pPlayer->stAccountInfo.siDBAccount,	pPlayer->stAccountInfo.uiSlot) == false)
			{				
				bSuccess = FALSE;
				break;
			}
			
			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)siReturn);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;



	// 주막에서 게시판에 글을 등록 한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_REGISTNOTICE:
		{		
			DBIKRegistryNoticeInfo		*pRegistryNoticeInfo;			
			SI32						*pNoticeIndex;

			pRegistryNoticeInfo		=	(DBIKRegistryNoticeInfo*)pIn;
			pNoticeIndex			=	(SI32*)pOut;
			
			if(SQLDB.SQLRun("EXEC SPHQ_OnAddBillboard %d, '%s', '%s', '%s', '%s'",	pRegistryNoticeInfo->uiVillageCode,
																	pRegistryNoticeInfo->pszTitle,
																	pRegistryNoticeInfo->pszNotice,																	
																	pRegistryNoticeInfo->szID,
																	pRegistryNoticeInfo->szDate) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)pNoticeIndex);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}			
		}
		break;



	// 주막에서 게시판의 글을 삭제 한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DELETENOTICE:
		{
			DBIKDeleteNotice			*pDeleteNotice;
			BOOL						*pbDeleteNoticeSuccess;			

			pDeleteNotice			=	(DBIKDeleteNotice*)pIn;
			pbDeleteNoticeSuccess	=	(BOOL*)pOut;

			*pbDeleteNoticeSuccess	=	FALSE;
			if(SQLDB.SQLRun("EXEC SPHQ_OnDeleteBillboard %d, %d, '%s'",	pDeleteNotice->uiVillageCode, 
																		pDeleteNotice->uiNoticeIndex,
																		pDeleteNotice->szWroteID) == false)
			{		
				bSuccess = FALSE;
				break;
			}	

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)pbDeleteNoticeSuccess);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}			
		}
		break;


	// 주막에서 게시판의 글을 수정 한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_MODIFYNOTICE:
		{
			DBIKModifyNoticeInfo		*pModifyNotice;
			BOOL						*pbModifyNoticeSuccess;			

			pModifyNotice			=	(DBIKModifyNoticeInfo*)pIn;
			pbModifyNoticeSuccess	=	(BOOL*)pOut;

			*pbModifyNoticeSuccess	=	FALSE;
			if(SQLDB.SQLRun("EXEC SPHQ_OnModifyBillboard %d, %d, '%s', '%s', '%s'",	
								pModifyNotice->uiVillageCode, pModifyNotice->uiNoticeIndex, 
								pModifyNotice->pszTitle, pModifyNotice->pszNotice, pModifyNotice->szDate) == false)
			{						
				bSuccess = FALSE;
				break;
			}	

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)pbModifyNoticeSuccess);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}			
		}
		break;

	// 유저의 전장 재산 정보를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_AUDIT_TOTAL_1:
		{
			ACCOUNT_INFO				*pAccountInfo;
			PROPERTY_INFO				*pPropertyInfo;
			DBNUMERIC					TempNumeric1;
			DBNUMERIC					TempNumeric2;
			memset(&TempNumeric1, 0, sizeof(DBNUMERIC));
			memset(&TempNumeric2, 0, sizeof(DBNUMERIC));

			pAccountInfo				=	(ACCOUNT_INFO*)	pIn;
			pPropertyInfo				=	(PROPERTY_INFO*)	pOut;
			
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBankProperty %d, %d",pAccountInfo->siDBAccount, pAccountInfo->uiSlot) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, SRCNUMERICBIND,	    0,	(BYTE *)&TempNumeric1);
				SQLDB.DBBind (2, SRCNUMERICBIND,		0,	(BYTE *)&TempNumeric2);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(&pPropertyInfo->siSaveProperty, TempNumeric1.val, sizeof(MONEY));
				memcpy(&pPropertyInfo->siLoanProperty, TempNumeric2.val, sizeof(MONEY));
			}
			else
				bSuccess	=	FALSE;
		}
		break;
	


	// 마을이 속한 국가의 코드를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGE_GETNATION:
		{
			if(SQLDB.SQLRun("EXEC SPHQ_OnVillageGetNation %d", DWORD(pIn)) == false)
			{				
				bSuccess = FALSE;
				break;
			}			
			if ((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, CHARBIND,	(DBCHAR) 0,	(BYTE *)pOut);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;



	// 마을에 설명 글을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEDESCRIPT:
		{
			SI32			siResult = 0;
			CHAR			*descript;
			UI16			uiVillageCode;

			uiVillageCode	=	(DWORD)	pIn;
			descript		=	(CHAR*)	pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnReadDescript %d", uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}			
			if ((SQLDB.DBResults()) == SUCCEED)
			{
				memset(descript, 0, MAX_DESCRIPT_LENGTH);
				SQLDB.DBBind (1, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)descript);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;



	// 통장에 돈을 저금 한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DEPOSIT:
		{			
			OnAccountNumber			*pAccountNumber;
			MONEY					*puiBalance;
			MONEY					*pmnMoney;
		
			DBNUMERIC				TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));

			pAccountNumber		=	( OnAccountNumber* )pIn;
			pmnMoney			=	( MONEY* )pOut;
			

			if(SQLDB.SQLRun("EXEC SPHQ_On2DepositInBank %d, %d, %d, %d, %d, %I64d", 
							pAccountNumber->uiVillageCode,
							pAccountNumber->uiAccountKind,
							pAccountNumber->uiUserDBAccount,
							pAccountNumber->uiCharSlot,
							pAccountNumber->uiAccountIndex,
							*pmnMoney) == false)							
			{								
				bSuccess = FALSE;
				break;
			}

			puiBalance			=	( MONEY* )pOut;
			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (1, SRCNUMERICBIND,	  0,	(BYTE *)&TempNumeric);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(puiBalance, TempNumeric.val,sizeof(MONEY));
			}
		}
		break;	


	// 통장에서 돈을 찾는다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_DRAWING:
		{
			// 은행에서 플레이어 잔액을 얻어온다.
			OnAccountNumber			*pAccountNumber;
			MONEY					*puiBalance;
			MONEY					*pmnMoney;
			DBNUMERIC				TempNumeric;

			SI32					siReturn = TRUE;
						
			memset( &TempNumeric, 0, sizeof( DBNUMERIC ) );

			pAccountNumber		=	( OnAccountNumber* )pIn;
			pmnMoney			=	( MONEY* )pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_On2WithdrawFromBank %d, %d, %d, %d, %d, %I64d", 
							pAccountNumber->uiVillageCode,
							pAccountNumber->uiAccountKind,
							pAccountNumber->uiUserDBAccount,
							pAccountNumber->uiCharSlot,
							pAccountNumber->uiAccountIndex,
							*pmnMoney) == false)							
			{
								
				bSuccess = FALSE;
				break;
			}

			puiBalance			=	( MONEY* )pOut;
			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (1, SRCNUMERICBIND,		0,	(BYTE *)&TempNumeric);
				SQLDB.DBBind (2, INTBIND,	(DBINT)	    0,	(BYTE *)&siReturn);
				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(puiBalance, TempNumeric.val, sizeof(MONEY));
			}
			
			if( siReturn == 0 )
			{
				bSuccess  = FALSE;
				break;
			}
		}
		break;

	// 담보 요청을 처리한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_COLLATERAL_1:
		{
			char							cTemp1[1024], cTemp2[1024];
			SI32						   *siReturn;

			DBIKVillageCollateral			*pVillageCollateral;
			
			
			DBIKCollateralList				*pCList;

			pVillageCollateral			=	(DBIKVillageCollateral *)pIn;
			siReturn					=	(SI32*)pOut;

			pCList						=	(DBIKCollateralList*)pVillageCollateral->pCollateralList;

			memset(cTemp1, 0, sizeof(cTemp1));
			

			if(SQLDB.SQLRun("EXEC SPHQ_OnCreatePawn %u, %d, %u, %u, %u, %I64d", 													
												  pVillageCollateral->uiVillageCode,
												  pVillageCollateral->pPlayer->stAccountInfo.siDBAccount,
												  pVillageCollateral->pPlayer->stAccountInfo.uiSlot,
												  pVillageCollateral->uiMonth*30,
												  pVillageCollateral->uiMonth,
												  pVillageCollateral->siMoney) == false)
			{				
				bSuccess = FALSE;
				break;
			}			
			
			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)siReturn);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
			
			SQLDB.ClearCMDBuf();
			SQLDB.ClearRows();

			if( (*siReturn) != 0 )
			{
				for(int i = 0 ; i< pVillageCollateral->pCollateralList->siCollateralNum ; i++ )
				{
					memset(cTemp2, 0, sizeof(cTemp2));
					wsprintf(cTemp2,"%d;%u;%u;",*siReturn,pVillageCollateral->pCollateralList->uiITemCode[i],pVillageCollateral->pCollateralList->uiNum[i]);
					strcat(cTemp1, cTemp2);
				}

				if(SQLDB.SQLRun("EXEC SPHQ_OnCreatePawnItem '%s'", cTemp1) == false)
				{					
					bSuccess = FALSE;
					break;
				}
				
				if((SQLDB.DBResults()) == SUCCEED)
				{
					SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)siReturn);
					while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
				}				
			}
		}
		break;	

	// 마을의 은행 정보를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BANKINFO:
		{
			// 은행의 이율을 얻어온다.
			DBIKBankInfo	*pBankInfo;
			DBNUMERIC		BankNumeric,TradeNumeric;
			memset(&BankNumeric, 0, sizeof(DBNUMERIC));
			memset(&TradeNumeric, 0, sizeof(DBNUMERIC));

			pBankInfo		=	(DBIKBankInfo*)pIn;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBankInfo %d", pBankInfo->uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (1, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&pBankInfo->siInterest);
				SQLDB.DBBind (2, SRCNUMERICBIND,	     0,	(BYTE *)&BankNumeric);
				SQLDB.DBBind (3, SRCNUMERICBIND,	     0,	(BYTE *)&TradeNumeric);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(&pBankInfo->siBankCapital, BankNumeric.val, sizeof(MONEY));
				memcpy(&pBankInfo->siTradeCapital, TradeNumeric.val, sizeof(MONEY));
			}
			else
				bSuccess	=	FALSE;
		}
		break;	


	// 아이템의 총 갯수를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_ITEM_TOTALNUM:
		{
			SI32				*ItemNum;
			ItemNum			=	(SI32*)pIn;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetItemTotalNum") == false)
			{				
				bSuccess = FALSE;
				break;
			}			
			if ((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT) 0,	(BYTE *)ItemNum);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;



	// 각 아이템의 가격을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_ITEM_DBPRICE:
		{
			SI32			ItemCode;
			SI32			ItemPrice;			
			SI32			*pItemCode;
			SI32			*pItemPrice;

			pItemCode		=	(SI32*)	pIn;
			pItemPrice		=	(SI32*)	pOut;
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetItemDBPrice") == false)
			{				
				bSuccess = FALSE;
				break;
			}

			int i=0;
			if ((SQLDB.DBResults()) == SUCCEED)  
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)		 0,	(BYTE *)&ItemCode);
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&ItemPrice);

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pItemCode[i]	=	ItemCode;
					pItemPrice[i]	=	ItemPrice;
					i++;
				}
			}
		}
		break;

/*	// 통장을 제거한다 ( SET으로 옮겨도 됨 )
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_CLOSESAVINGACCOUNT:
		{
			OnAccountNumber	*pAccountNumber;
			MONEY			siBalance;

			pAccountNumber	=	(OnAccountNumber*)pIn;

			if(SQLDB.SQLRun("EXEC SPHQ_On2DeleteBankbook %d, %d, %d, %d, %d", 
							pAccountNumber->uiVillageCode,
							pAccountNumber->uiAccountKind,
							pAccountNumber->uiUserDBAccount,
							pAccountNumber->uiCharSlot,
							pAccountNumber->uiAccountIndex) == false)							
			{								
				bSuccess = FALSE;
				break;
			}	

			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&siBalance);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;

*/
	case SO_DB_INFO_GETEVENTITEM:
	{
		char*   szDate;
		SI32	siQuestEventID = 0 , QuestEventItemCount = 0;
		SI32	siDelivery = 0;	//0이면 지급아이템,1이면 배달 아이템.
		DBIKQuestEventItemInfoMgr*	pQuestEventItemInfo;

		szDate				 =	(CHAR*)pIn;
		pQuestEventItemInfo	 = 	(DBIKQuestEventItemInfoMgr*)pOut;

		if(SQLDB.SQLRun("EXEC SPHQ_OnGetItemByEvent %d, '%s'",pQuestEventItemInfo->siQuestID,szDate) == false)
		{
			bSuccess = FALSE;
			break;
		}
		if((SQLDB.DBResults()) == SUCCEED)
		{
			//여기서 얻어온다.
			SQLDB.DBBind (1, INTBIND,		 (DBINT)0,	(BYTE *)&siQuestEventID);
			SQLDB.DBBind (2, INTBIND,		 (DBINT)0,	(BYTE *)&siDelivery);
			SQLDB.DBBind (3, INTBIND,		 (DBINT)0,	(BYTE *)&QuestEventItemCount);

			while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
			{/*
				pQuestEventItemInfo[siQuestEventID].siQuestEventID	=	siQuestEventID;
				pQuestEventItemInfo[siQuestEventID].siPreItemCount	=	QuestEventItemCount;
				pQuestEventItemInfo[siQuestEventID].siCurItemCount	=	QuestEventItemCount;
				pQuestEventItemInfo[siQuestEventID].siDelivery		=	siDelivery;
			 */
				pQuestEventItemInfo->stQuestEventItemInfo[siQuestEventID].siQuestEventID	=	siQuestEventID;
				pQuestEventItemInfo->stQuestEventItemInfo[siQuestEventID].siPreItemCount	=	QuestEventItemCount;
				pQuestEventItemInfo->stQuestEventItemInfo[siQuestEventID].siCurItemCount	=	QuestEventItemCount;
				pQuestEventItemInfo->stQuestEventItemInfo[siQuestEventID].siCurItemCount	=	siDelivery;
			}			
		}
	}
	break;
	// 현재 게임의 시간을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETWORLDTIME:
		{
			SOnlineTime		*pTime;			

			pTime	=	(SOnlineTime*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetWorldTime") == false)							
			{				
				bSuccess = FALSE;
				break;
			}			

			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&pTime->Year);
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&pTime->Month);
				SQLDB.DBBind (3, INTBIND,	(DBINT)	     0,	(BYTE *)&pTime->Day);
				SQLDB.DBBind (4, INTBIND,	(DBINT)	     0,	(BYTE *)&pTime->Hour);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;


	// 떨어진 아이템을 얻어와 맵에 설정한다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_ALLFIELDITEM:
		{			
			SOMap			*pMap;
			SI16			siX, siY;
			UI32			uiID;
			UI16			uiQuantity;

			pMap	=	(SOMap*)pIn;
	
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetItemListOnMap %d", pMap->GetMapID()) == false)							
			{				
				bSuccess = FALSE;
				break;
			}
			
			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&siX);				
				SQLDB.DBBind (2, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&siY);				
				SQLDB.DBBind (3, INTBIND,	(DBINT)	     0,	(BYTE *)&uiID);
				// 이민수 수정 - small int type bind -> int type bind(2004.01.28)
				SQLDB.DBBind (4, INTBIND,	(DBINT)		 0,	(BYTE *)&uiQuantity);		
				
				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					// 맵에 Item을 설정한다.
					pMap->SetItem(siX, siY, UI16(uiID), uiQuantity);
				}
			}
		}
		break;	
	// 독도 퀘스트를 완료한 사용자 리스트를 얻어온다. 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFO_DOKDO_QUEST_COMPLETED_LIST : 
		{
			UI32 uiQuestCount = 0;			
			uiQuestCount = *(UI32*)pIn;

			SI16 uiQuestCode = *(SI16*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetQuestCount %d", uiQuestCode) == false)
			{
				bSuccess = FALSE;
				break;
			}
			
			if ((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)		 0,	(BYTE *)&uiQuestCount);

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					memcpy(pIn, &uiQuestCount, sizeof(UI32));
				}
			}
		}
		break;	
	// 마을의 인구를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEPOPULATION:
		{
			UI16	*puiVillageCode;
			//UI32	*puiPopulation;
			DBIKGetVillageInfo* pGetVillageInfo;

			puiVillageCode	=	(UI16*)pIn;
			pGetVillageInfo	=	(DBIKGetVillageInfo*)pOut;

			
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetPopulation %d", *puiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED)  
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&pGetVillageInfo->uiPopulation);				
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&pGetVillageInfo->uiMaxPopulation);
			//	SQLDB.DBBind (3, INTBIND,	(DBINT)	     0,	(BYTE *)&pGetVillageInfo->siDefenceSoldier);
				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;


	// 상단을 생성 한다 
	//----------------------------------------------------------------------------------------
	/*case SO_DB_INFOKIND_GUILD_CREATE:
		{
			DBIKGuildCreateInfo	*pGuildCreateInfo;
			UI32				*puiGuildID;

			pGuildCreateInfo	=	(DBIKGuildCreateInfo*)pIn;
			puiGuildID			=	(UI32*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_On2MakeGuild %d, '%s', %d, %d, %I64d ",	pGuildCreateInfo->uiVillageCode, pGuildCreateInfo->szGuildName, pGuildCreateInfo->siAccount,pGuildCreateInfo->uiSlot, pGuildCreateInfo->moMoney	) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if(SQLDB.DBResults() == SUCCEED)
			{	
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)puiGuildID);				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}												
		}
		break;*/


	// 마을을 차지하고 있는 길드를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEGUILDID_GET:
		{
			UI16	uiVillageCode;
			UI32	*puiGuildID;

			uiVillageCode	=	UI16(pIn);
			puiGuildID		=	(UI32*)pOut;
		
			if(SQLDB.SQLRun("EXEC SPHQ_OnVillageOwnerGet %d", uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED)  
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)puiGuildID);				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;


	// 유저가 가진 부동산(생산건물)을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_AUDIT_OWN:
		{
			DBIKAuditOwnList			*pOwnList;				
			SOPlayer					*pPlayer;
			OnAuditOwnInfo				AuditOwnInfo;

			pPlayer						=	(SOPlayer*)	pIn;
			pOwnList					=	(DBIKAuditOwnList*)	pOut;

					
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetAuditPlantInfo %d, %d",
				pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			int i=0;
			if ((SQLDB.DBResults()) == SUCCEED)  
			{
				SQLDB.DBBind (1, TINYBIND,	(DBTINYINT)	 0,	(BYTE *)&AuditOwnInfo.PlantID);
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&AuditOwnInfo.uiVillageCode);
				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pOwnList->AuditOwn[i] = AuditOwnInfo;
					i++;
				}
				pOwnList->OwnNum	=	i;				
			}
		}
		break;


	// 장부에서의 통장 목록을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_AUDIT_BANKACCOUNT:
		{			
			DBIKBankBookList			*pBankBookList;
			SOPlayer					*pPlayer;
			SI32						siCount = 0;
			UI32						uiAccountKind;
			UI32						uiAccountIndex;
			UI32						uiVillageCode;
			DBNUMERIC					TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));

			pPlayer						=	(SOPlayer*)	pIn;
			pBankBookList				=	(DBIKBankBookList*)	pOut;
			
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBankbookList %d, %u",
							pPlayer->stAccountInfo.siDBAccount,	pPlayer->stAccountInfo.uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}	
			
			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, INTBIND, (DBINT)		 0, (BYTE *)&uiVillageCode);
				SQLDB.DBBind (2, SMALLBIND,	(DBSMALLINT) 0, (BYTE *)&uiAccountKind);
				SQLDB.DBBind (3, TINYBIND,	(DBTINYINT)	 0, (BYTE *)&uiAccountIndex);
				SQLDB.DBBind (4, SRCNUMERICBIND,		 0, (BYTE *)&TempNumeric);
				
				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pBankBookList->AccountNumber[siCount].uiAccountKind		=	uiAccountKind;
					pBankBookList->AccountNumber[siCount].uiAccountIndex	=	uiAccountIndex;
					pBankBookList->AccountNumber[siCount].uiVillageCode		=	uiVillageCode;
					pBankBookList->AccountNumber[siCount].uiCharSlot		=	pPlayer->stAccountInfo.uiSlot;
					pBankBookList->AccountNumber[siCount].uiUserDBAccount	=	pPlayer->stAccountInfo.siDBAccount;
					memcpy(&pBankBookList->siMoney[siCount], TempNumeric.val,sizeof(MONEY));
					
					siCount++;
				}

				pBankBookList->siBankBookNum = siCount;
			}
		}
		break;

/*	case SO_DB_INFO_GETEVENTITEM:
		{
			DBIKQuestEventItem		stQuestEventItem;
			SI32					siEventItemID;
			
			stQuestEventItem		=	(DBIKQuestEventItem*) pIn;

			if(SQLDB.SQLRun() == false)
			{
				bSuccess = FALSE;
				break;
			}
			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1,INITBIND,(DBINT)0,(BYTE*)&siEventItemID);
				while(SQLDB.DBNextRow() != NO_MORE_ROWS);
			}
			siEventItemID;
			
			
		}
		break;
*/
	// 마이너스 통장이 있는가 없는가?
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_CHECKLOAN:
		{
			SOPlayer					*pPlayer;
			SI32						siResult=0;	
			pPlayer						=	(SOPlayer*)	pIn;

			if(SQLDB.SQLRun("EXEC SPHQ_OnCheckLoanAccount %d, %u",
							pPlayer->stAccountInfo.siDBAccount,	pPlayer->stAccountInfo.uiSlot) == false)
			{								
				bSuccess = FALSE;
				break;
			}	
			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, INTBIND, (DBINT) 0, (BYTE *)&siResult);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}			
			if(siResult != 0 )
				bSuccess = FALSE;
		}
		break;

	// 그 마을 안에서의 통장 리스트를 얻어 온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_MYBANKBOOK:
		{			
			SI32						i = 0;
			DBIKBankBookList	       *pBankBookList;
			DBIKVillageBankBookInfo	   *pVillageBankBookInfo;			
			UI32						uiAccountKind;
			UI32						uiAccountIndex;
			DBNUMERIC					TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));

			pVillageBankBookInfo		=	(DBIKVillageBankBookInfo*)pIn;			
			pBankBookList				=	(DBIKBankBookList*)pOut;


			// 총 통장의 개수를 얻어온다.
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetVillageBankbookCount %d, %d, %d", pVillageBankBookInfo->pPlayer->stAccountInfo.siDBAccount, 
																			  pVillageBankBookInfo->pPlayer->stAccountInfo.uiSlot,
																			  pVillageBankBookInfo->uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND, (DBINT) 0, (BYTE *)&pBankBookList->siBankBookNum);				
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}


			SQLDB.ClearCMDBuf();
			SQLDB.ClearRows();


			// 통장의 리스트를 얻어온다.
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetVillageBankbookList %d, %d, %d", pVillageBankBookInfo->pPlayer->stAccountInfo.siDBAccount, 
																		 pVillageBankBookInfo->pPlayer->stAccountInfo.uiSlot,
																		 pVillageBankBookInfo->uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			i=0;
			if((SQLDB.DBResults()) == SUCCEED)
			{								
				SQLDB.DBBind (1, INTBIND, (DBINT) 0, (BYTE *)&uiAccountKind);
				SQLDB.DBBind (2, INTBIND, (DBINT) 0, (BYTE *)&uiAccountIndex);					
				SQLDB.DBBind (3, SRCNUMERICBIND,  0, (BYTE *)&TempNumeric);

				while( SQLDB.DBNextRow() != NO_MORE_ROWS )
				{
					pBankBookList->AccountNumber[i].uiVillageCode		=	pVillageBankBookInfo->uiVillageCode;
					pBankBookList->AccountNumber[i].uiAccountKind		=	uiAccountKind;					
					pBankBookList->AccountNumber[i].uiUserDBAccount		=	pVillageBankBookInfo->pPlayer->stAccountInfo.siDBAccount;
					pBankBookList->AccountNumber[i].uiCharSlot			=	pVillageBankBookInfo->pPlayer->stAccountInfo.uiSlot;
					pBankBookList->AccountNumber[i].uiAccountIndex		=	uiAccountIndex;

					memcpy(&pBankBookList->siMoney[i], TempNumeric.val, sizeof(MONEY));
					++i;
				}				
			}
		}
		break;


	// 담보리스틀 얻어 온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_COLLATERALLIST:
		{
			DBIKVillageCollateralInfo	*pVillageCollateralInfo;
			DBIKCollateralList			*pCollateralList;
			
			pVillageCollateralInfo	= (DBIKVillageCollateralInfo*)pIn;
			pCollateralList			= (DBIKCollateralList*)pOut;

			UI16	uiItemCode;
			UI32	uiItemNum;
			SI32	i = 0 ;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetPawn %d, %u, %u",pVillageCollateralInfo->siAccount,	pVillageCollateralInfo->uiSlot,pVillageCollateralInfo->uiVillageCode) == false)
			{
				bSuccess = FALSE;				
				break;
			}	
			
			if ((SQLDB.DBResults()) == SUCCEED) 
			{				
				SQLDB.DBBind (4, INTBIND,	(DBINT)	     0,	(BYTE *)&uiItemCode);
				SQLDB.DBBind (5, INTBIND,	(DBINT)		 0,	(BYTE *)&uiItemNum);

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pCollateralList->uiITemCode[i]	=	uiItemCode;
					pCollateralList->uiNum[i]		=	uiItemNum;
					i++;
				}
				pCollateralList->siCollateralNum = i;				
			}
		}
		break;

	// 게시판에서의 글을 갯수를 얻어 온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_TOTAL_NOTICESNUM:
		{
			UI16			uiVillageCode;
			UI32			*puiTotalNoticesNum;

			// 마을 총 게시물의 수를 얻어온다.
			uiVillageCode		=	(UI16)pIn;
			puiTotalNoticesNum	=	(UI32*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBillboardListCount %d", uiVillageCode) == false)
			{						
				bSuccess = FALSE;
				break;
			}	

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND, (DBINT) 0, (BYTE *)puiTotalNoticesNum);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}			
		}
		break;


	// 게시판에서 전체 게시물의 기본정보를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_TOTAL_NOTICESHEADER:
		{
			// 마을 총 게시물을 얻어온다.			
			DBIKGetNoticesInfo			*pNoticesNum;			
			UI16						*puiNoticeNumber;

			pNoticesNum			=	(DBIKGetNoticesInfo*)pIn;
			puiNoticeNumber		=	(UI16*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBillboardList %d", pNoticesNum->uiVillageCode) == false)
			{						
				bSuccess = FALSE;
				break;
			}	
			
			int i =0;
			if((SQLDB.DBResults()) == SUCCEED)
			{
				UI16 noticeNumber;
												
				SQLDB.DBBind (1, SMALLBIND,	(DBSMALLINT) 0, (BYTE *)&noticeNumber);					
				while( SQLDB.DBNextRow() != NO_MORE_ROWS )				
				{
					puiNoticeNumber[i] = noticeNumber;
					++i;
				}				
			}			
		}
		break;
		 
		/*
	// 게시판에서 한 게시물의 기본정보를 얻어오낟 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETNOTICESHEADER:
		{
			// 마을의 헤더를 얻어온다.			
			DBIKGetNoticesHeaderInfoIn			*pGetNoticesHeaderInfoIn;
			DBIKGetNoticesHeaderInfoOut			*pGetNoticesHeaderInfoOut;				

			pGetNoticesHeaderInfoIn		=	(DBIKGetNoticesHeaderInfoIn*)pIn;
			pGetNoticesHeaderInfoOut	=	(DBIKGetNoticesHeaderInfoOut*)pOut;
			
			pGetNoticesHeaderInfoOut->uiNoticesNum	=	0;
			for(int i = 0; i < ON_SEND_NOTICESNUM_PER_ONCE; i++)
			{
				if((pGetNoticesHeaderInfoIn->siTopIndex - i) >= 0)
				{
					pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].uiIndex	=	pGetNoticesHeaderInfoIn->puiNoticesIndexArray[pGetNoticesHeaderInfoIn->siTopIndex - i];

					if(SQLDB.SQLRun("EXEC SPHQ_OnGetBillboardHead %d, %d", pGetNoticesHeaderInfoIn->uiVillageCode, 
									pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].uiIndex) == false)
					{								
						bSuccess = FALSE;
						break;
					}	 
					
					if((SQLDB.DBResults()) == SUCCEED)
					{					
						SQLDB.DBBind (1, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)&pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].szTitle);
						SQLDB.DBBind (2, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)&pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].szID);
						SQLDB.DBBind (3, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)&pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].szDate);
						SQLDB.DBBind (4, INTBIND, (DBINT) 0,	(BYTE *)&pGetNoticesHeaderInfoOut->NoticeHeaderInfo[i].dwReadCount);
						while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
					}			

					SQLDB.ClearCMDBuf();
					SQLDB.ClearRows();

					// 실제 읽은 Notice를 증가시켜준다.
					pGetNoticesHeaderInfoOut->uiNoticesNum++;
				}
				else
					break;		
			}		

			return TRUE;
		}
		break;	
	*/

	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_BANKCAPITAL:
		{
			// 마을 금융 자본을 얻어온다.
			UI16			uiVillageCode;
			MONEY*			psiBankCapital;
			DBNUMERIC		TempNumeric;
			memset(&TempNumeric, 0, sizeof(DBNUMERIC));
			uiVillageCode		=	(UI32)pIn;
			psiBankCapital		=	(MONEY*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetCapital 'b', %d",uiVillageCode) == false)
			{						
				bSuccess = FALSE;
				break;
			}	
			
			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, SRCNUMERICBIND,	0, (BYTE *)&TempNumeric);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(psiBankCapital, TempNumeric.val, sizeof(MONEY));				
			}
		}
		break;


	// 게시판에서 한 글을 본문을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETNOTICECONTENT:
		{
			
			DBIKGetNoticeContentIn		*pGetNoticeContentIn;		
			CHAR						*pszString;


			pGetNoticeContentIn		=	(DBIKGetNoticeContentIn*)pIn;
			pszString				=	(CHAR*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetBillboard %d, %d", pGetNoticeContentIn->uiVillageCode, 
																   pGetNoticeContentIn->uiNoticeID) == false)
			{						
				bSuccess = FALSE;
				break;
			}	

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (1, NTBSTRINGBIND,	(DBCHAR) 0, (BYTE *)pszString);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;			

	// 유저의 이메일을 얻어 온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_USERMAILADDRESS:
		{
			CHAR	*pszCharName;
			CHAR	*pszUserMailAddress;

			
			pszCharName			=	(CHAR*)pIn;
			pszUserMailAddress	=	(CHAR*)pOut;
			 

			if(SQLDB.SQLRun("EXEC SPHQ_FindMember '%s'", pszCharName) == false)
			{						
				bSuccess = FALSE;
				break;
			}				

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind (5, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)pszUserMailAddress);			// E-Mail
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;
		
	// 전체 길드의 리스틀 얻어온다 (서버 시작)
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_GETGUILDLIST:
		{
			cltIGuild	*pclIGuild;			
			UI32		uiGuildID;
			CHAR		szGuildName[ON_GUILDNAME_LENGTH + 2];
			CHAR		szDAIID[ON_ID_LENGTH + 2];
			UI16		uiOwnerVillageNum;
			UI32		uiGuildMemberNum;
			SI32		siGuildFlag;

			pclIGuild	=	(cltIGuild*)pIn;
			
			ZeroMemory(szGuildName, sizeof(szGuildName));
			ZeroMemory(szDAIID, sizeof(szDAIID));

			if(SQLDB.SQLRun("EXEC SPHQ_OnGuildGetList") == false)
			{				
				bSuccess = FALSE;
				break;
			}
			
			if(SQLDB.DBResults() == SUCCEED)
			{				
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&uiGuildID);			// GuildID를 얻어온다.
				SQLDB.DBBind (2, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)szGuildName);			// 길드의 이름을 얻어온다.
				SQLDB.DBBind (3, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)szDAIID);				// 대방의 이름을 얻어온다.
				SQLDB.DBBind (4, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&uiOwnerVillageNum);	// 상단이 소유한 마을의 수
				SQLDB.DBBind (5, INTBIND,	(DBINT)	     0,	(BYTE *)&uiGuildMemberNum);		// 현 상단원의 수
				SQLDB.DBBind (6, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildFlag);			// 상단의 깃발.

				// 현재 소유한 마을의 수를 얻어온다.
				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pclIGuild->InitGuild(uiGuildID, szGuildName, szDAIID, uiGuildMemberNum, siGuildFlag);
					printf("Guild ID : [%u], Guild Name : [%s]\n", uiGuildID, szGuildName);
				}
			}
		}
		break;	
	case SO_DB_INFOKIND_GET_GUILD_ITEM:
		{
			cltGuild	*pclGuild;			
			cltIGuild	*pclIGuild;
			UI32		uiGuildID;
			SI32		siGuildItem[ON_MAX_GUILD_UNIT];
//			SI32		siSupply;

			memset(siGuildItem, 0, sizeof(SI32) * ON_MAX_GUILD_UNIT);

			pclIGuild	=	(cltIGuild*)pIn;
			if(SQLDB.SQLRun("EXEC SPHQ_On2GetServerSupplyItem") == false)
			{
				bSuccess = FALSE;
				break;
			}
			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&uiGuildID);			// GuildID를 얻어온다.
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[0]);		// 
				SQLDB.DBBind (3, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[1]);
				SQLDB.DBBind (4, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[2]);
				SQLDB.DBBind (5, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[3]);
				SQLDB.DBBind (6, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[4]);
				SQLDB.DBBind (7, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[5]);
				SQLDB.DBBind (8, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[6]);
				SQLDB.DBBind (9, INTBIND,	(DBINT)	     0,	(BYTE *)&siGuildItem[7]);
//				SQLDB.DBBind (10, INTBIND,	(DBINT)	     0,	(BYTE *)&siSupply);
			}
			while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
			{
				pclGuild = pclIGuild->GetGuildByID(uiGuildID);
				int j = 0;
				for(int i = 0 ; i < ON_MAX_GUILD_UNIT ; i++, j++)
				{
					if(i >= 4)
						pclGuild->m_GuildUnit.SetGuildUnit(i + 1, (SI16)siGuildItem[j]);
					else
						pclGuild->m_GuildUnit.SetGuildUnit(i, (SI16)siGuildItem[j]);

				}
//				pclGuild->SetGuildSupply(siSupply);
			}
			//pclIGuild->m_Guilds->m_GuildUnit.IncreaseGuildUnit(
		}
		break;

	// 길드가 소유한 길드 원을 얻어온다   
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_GETGUILDMEMBER:
		{			
			DBNUMERIC					TempTotalMoney;			
			UI32						uiGuildID;						
			strGuildMemberInfo			stMemberInfo;
			GuildMemberManager			*pMemberManager;		
			SI32						siTotalMembersNum;
			CHAR						szTempName[ON_ID_LENGTH+2];
			
			ZeroMemory(&TempTotalMoney, sizeof(TempTotalMoney));

			uiGuildID		=	UI32(pIn);
			pMemberManager	=	(GuildMemberManager*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetGuildMember %d", uiGuildID) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if(SQLDB.DBResults() == SUCCEED)
			{	

				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&stMemberInfo.siAccount);				
				SQLDB.DBBind (2, TINYBIND,	(DBINT)	     0,	(BYTE *)&stMemberInfo.uiSlot);				

				SQLDB.DBBind (3, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)szTempName);
				SQLDB.DBBind (4, INTBIND,	(DBINT)	     0,	(BYTE *)&stMemberInfo.siClassInGuild);				
				SQLDB.DBBind (5, SRCNUMERICBIND,	     0, (BYTE *)&TempTotalMoney);
				SQLDB.DBBind (6, INTBIND,	(DBINT)	     0,	(BYTE *)&stMemberInfo.siLevel);				
				SQLDB.DBBind (7, INTBIND,	(DBINT)	     0,	(BYTE *)&stMemberInfo.uiTradeCredit);				

				siTotalMembersNum	=	0;

				ZeroMemory( szTempName, sizeof(szTempName) );

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					// 이름을 복사해준다.
					memcpy( stMemberInfo.szName, szTempName, sizeof(stMemberInfo.szName) );
					// 돈을 복사해준다.
					memcpy(&stMemberInfo.mnPropery, TempTotalMoney.val, sizeof(MONEY));						
				
					// 지원자인지 정식 멤버인지 구분한다.					
					pMemberManager->AddMember(stMemberInfo.szName, stMemberInfo.siClassInGuild, 
											stMemberInfo.mnPropery, stMemberInfo.siLevel, stMemberInfo.uiTradeCredit,stMemberInfo.siAccount,stMemberInfo.uiSlot);
				
					siTotalMembersNum++;

					// 데이타를 다시 초기화 시켜준다.
					ZeroMemory( &stMemberInfo, sizeof(stMemberInfo) );
					ZeroMemory( szTempName, sizeof(szTempName) );
				}
			}
		}	
		break;
	case SO_DB_INFOKIND_GET_GUILD_RESULT:
		{
			UI32						uiGuildID;
			cltGuild*					pclGuild;
			SI16						siVillageCode;
			BYTE						bytePercent;
			char						szTempName[11];
			SI16						i = 0;


			uiGuildID				=	UI32(pIn);
			pclGuild				=	(cltGuild*)pOut;

			ZeroMemory(pclGuild->m_stGuildResult,sizeof(cltGuildResult)*ON_MAX_GUILD_RESULT);

			if(SQLDB.SQLRun("EXEC SPHQ_On2ReadVillage %d", uiGuildID) == false)
			{				
				bSuccess = FALSE;
				break;
			}
			
			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind (1, SMALLBIND,	(DBSMALLINT)  0,	(BYTE *)&siVillageCode);				
				SQLDB.DBBind (2, TINYBIND,	(DBTINYINT)	  0,	(BYTE *)&bytePercent);
				SQLDB.DBBind (3, NTBSTRINGBIND,	(DBCHAR) 0,		(BYTE *)szTempName);

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					pclGuild->m_stGuildResult[i].SetCharName(szTempName);
					pclGuild->m_stGuildResult[i].SetPercent((SI16)bytePercent);
					pclGuild->m_stGuildResult[i].SetVillageCode(siVillageCode);
					i++;
				}
			}

		}
		break;

	// 상단과의 관계를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILD_GETRELATION:		
		{
			UI32			uiGuildID, uiTempGuild;
			cltIGuild		*pclIGuild;						
			SI32			siTime;
			CHAR			szTemp[200];

			uiGuildID	=	UI32(pIn);
			pclIGuild	=	(cltIGuild*)pOut;
			ZeroMemory(szTemp,sizeof(szTemp));

			if(SQLDB.SQLRun("EXEC SPHQ_OnGuildGetHostile %d", uiGuildID) == false)
			{				
				bSuccess = FALSE;
				break;
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)&uiTempGuild);
				SQLDB.DBBind (2, INTBIND,	(DBINT)	     0,	(BYTE *)&siTime);
				SQLDB.DBBind (3, NTBSTRINGBIND, (DBCHAR)  0, (BYTE*)szTemp);
				//2. 시간 데이터를 가져온다.

				while(SQLDB.DBNextRow()!=NO_MORE_ROWS)
				{
					// 길드의 관계를 설정한다.(uiGuildID: 자신의 길드, uiTempGuild: 타 상단)
					//마지막 인자에 처음 디비에서 DB에서 가져온 시간을 넣는다.
					pclIGuild->SetRelation(uiGuildID, uiTempGuild, ON_GUILD_RELATION_ENEMY, TRUE,siTime);
					pclIGuild->SetMessage(uiGuildID, uiTempGuild,szTemp);
				}
			}
		}
		break;

	//  마을의 행수 이름을 얻어 온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEHNSNAME_GET:
		{
			UI16	uiVillageCode;
			CHAR	*pszName;

			uiVillageCode	=	UI16(pIn);
			pszName			=	(CHAR*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGetTradeMaster %d", uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;		
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)pszName);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;


	// 길드내 직급을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GUILDCLASS:
		{
			CHAR	*pszName;
			SI32	*psiGuildClass;

			pszName			=	(CHAR*)pIn;
			psiGuildClass	=	(SI32*)pOut;

			if(SQLDB.SQLRun("EXEC SPHQ_OnGuildGetClass '%s '", pszName) == false)
			{				
				bSuccess = FALSE;
				break;		
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	     0,	(BYTE *)psiGuildClass);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;

	
	// 마을의 방어력을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEDEFENCE_GET:
		{
			UI16		uiVillageCode;
			VDEF		*pvdVillageDefence;
			DBNUMERIC	TempNumeric;


			memset(&TempNumeric, 0, sizeof(DBNUMERIC));
			
			uiVillageCode		=	UI16(pIn);
			pvdVillageDefence	=	(VDEF*)pOut;
			
			if(SQLDB.SQLRun("EXEC SPHQ_OnVillageGetDurability %d", uiVillageCode) == false)
			{				
				bSuccess = FALSE;
				break;		
			}

			if ((SQLDB.DBResults()) == SUCCEED) 
			{
				SQLDB.DBBind (1, SRCNUMERICBIND,	     0, (BYTE *)&TempNumeric);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				memcpy(pvdVillageDefence, TempNumeric.val, sizeof(MONEY));
			}			
		}
		break;


	// 배표 정보를 얻어 온다
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_GETTICKET:
		{
			DBIKGetTicket*		lpDBIKGetTicket;
			DBIKGetTicketInfo*	lpDBIKGetTicketInfo;

			lpDBIKGetTicket     = (DBIKGetTicket*)pIn;
			lpDBIKGetTicketInfo = (DBIKGetTicketInfo*)pOut;

			int			TempKind;

			lpDBIKGetTicketInfo->siTicketKind = -1;
			if(SQLDB.SQLRun("EXEC SPHQ_OnGetTicket %d, %d", lpDBIKGetTicket->DBAccount, lpDBIKGetTicket->uiSlot) == false)
			{				
				bSuccess = FALSE;
				break;		
			}

			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind(1, INTBIND, (DBINT)0, (BYTE*)&TempKind);
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );

				lpDBIKGetTicketInfo->siTicketKind = TempKind;
			}
			else
			{				
				bSuccess = FALSE;
				break;		
			}
		}
		break;

	// 마을에 연간 출입자수를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEENTERPLAYERGET:
		{
			DBIKVillageEnterPlauerGet*			lpDBIKVillageEnterPlauerGet;
			DBIKVillageEnterPlauerGetInfo*		lpDBIKVillageEnterPlauerGetInfo;

			lpDBIKVillageEnterPlauerGet     = (DBIKVillageEnterPlauerGet*)pIn;
			lpDBIKVillageEnterPlauerGetInfo = (DBIKVillageEnterPlauerGetInfo*)pOut;
			lpDBIKVillageEnterPlauerGetInfo->uiPlayerNum = 0;

			if( SQLDB.SQLRun("EXEC SPHQ_OnVillageEnterPlayerGet %d", lpDBIKVillageEnterPlauerGet->uiVillageCode) == FALSE )
			{
				bSuccess = FALSE;
				break;		
			}

			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind(1, INTBIND,  0, (BYTE*)&lpDBIKVillageEnterPlauerGetInfo->uiPlayerNum);
				while(SQLDB.DBNextRow() != NO_MORE_ROWS);
			}
		}
		break;
	case SO_DB_INFOKIND_GET_VILLAGE_DAMAGE:
		{
			SI32	siDamage;
			UI32	uiGuildID;
			SI16	i = 0;

			SI16 siTempVillageCode = (SI16)pIn;
			SOVillage*		pVillage = (SOVillage*)	pOut;;

			if( SQLDB.SQLRun("EXEC SPHQ_On2ReadDamage %d", siTempVillageCode) == FALSE )
			{
				bSuccess = FALSE;
				break;		
			}
			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind (1, INTBIND,	(DBINT)	0,	(BYTE*)&uiGuildID);
				SQLDB.DBBind( 2, INTBIND,	(DBINT)0,  (BYTE *)&siDamage);
				//SQLDB.DBBind (2, SMALLBIND,	(DBSMALLINT)	0,	(BYTE*)&siVillageCode);
				
				while(SQLDB.DBNextRow() != NO_MORE_ROWS)
				{
					pVillage->m_stAttackGuild[i].siDamage  = siDamage;
					pVillage->m_stAttackGuild[i].uiGuildID = uiGuildID;
					i++;
				}
			}

		}
	break;

	// 마을에 투자된 금액을 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_VILLAGEGETCAPITAL:
		{
			DBIKVillageGetCapital*		lpDBIKVillageGetCapital;
			MONEY*						lpMoney;
			DBNUMERIC					TempNumeric;

			lpDBIKVillageGetCapital = (DBIKVillageGetCapital*)pIn;
			lpMoney                 = (MONEY*)pOut;

			(*lpMoney) = 0;
			if(SQLDB.SQLRun("EXEC SPHQ_OnVillageGetCapital %d", lpDBIKVillageGetCapital->uiVillageCode) == FALSE)
			{
				bSuccess = FALSE;
				break;
			}

			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind(1, SRCNUMERICBIND, 0, (BYTE*)&TempNumeric);
			
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
				
				memcpy(lpMoney, TempNumeric.val, sizeof(MONEY));
			}
		}
		break;

	// 최고 투자자 정보를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVESTORGETINFO:
		{
			DBIKInvestorGetInfo*		lpDBIKInvestorGetInfo;
			DBIKInvestorGetInfo_Result*	lpDBIKInvestorGetInfo_Result;
			UI08						TempName[21];
			UI08						TempFlag;
			SI32						TempUpdateTime;
			UI08						TempName1[21];
			UI08						TempFlag1;
			SI32						TempUpdateTime1;

			lpDBIKInvestorGetInfo        = (DBIKInvestorGetInfo*)pIn;
			lpDBIKInvestorGetInfo_Result = (DBIKInvestorGetInfo_Result*)pOut;

			ZeroMemory(TempName, 21);
			TempFlag        = -1;
			TempUpdateTime  = 0;
			ZeroMemory(TempName1, 21);
			TempFlag1       = -1;
			TempUpdateTime1 = 0;
			
			SQLDB.SQLRun("EXEC SPHQ_OnInvestorGetInfo %d", lpDBIKInvestorGetInfo->uiVillageCode);
			if(SQLDB.DBResults() == SUCCEED)
			{
				SQLDB.DBBind(1, NTBSTRINGBIND,  0, (BYTE*)TempName);			// 실 투자자
				SQLDB.DBBind(2, TINYBIND,       0, (BYTE*)&TempFlag);			// 실 투자자 Type
				SQLDB.DBBind(3, INTBIND,        0, (BYTE*)&TempUpdateTime);		// 실 투자율
				SQLDB.DBBind(4, NTBSTRINGBIND,  0, (BYTE*)TempName1);			// 현재 투자자
				SQLDB.DBBind(5, TINYBIND,       0, (BYTE*)&TempFlag1);			// 현재 투자자 Type
				SQLDB.DBBind(6, INTBIND,        0, (BYTE*)&TempUpdateTime1);	// 현재 투자율
				while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
			}
			CopyMemory(lpDBIKInvestorGetInfo_Result->szRealBestInvestorID, TempName, 21);
			lpDBIKInvestorGetInfo_Result->uiRealBestInvestorType       = TempFlag;
			lpDBIKInvestorGetInfo_Result->uiRealBestInvestorUpdateTime = TempUpdateTime;
			CopyMemory(lpDBIKInvestorGetInfo_Result->szNowBestInvestorID, TempName1, 21);
			lpDBIKInvestorGetInfo_Result->uiNowBestInvestorType        = TempFlag1;
			lpDBIKInvestorGetInfo_Result->uiNowBestInvestorUpdateTime  = TempUpdateTime1;
		}
		break;

	// 마을 투자자 리스트를 얻어온다 
	//----------------------------------------------------------------------------------------
	case SO_DB_INFOKIND_INVEST_GETINVESTMENT:
		{
			DBIKInvestGetInvestment*		lpDBIKGetInvestment;
			DBIKInvestGetInvestment_Result*	lpDBIKGetInvestment_Result;
			CHAR							TempChar;
			__int64							TempInt64;
			UI08							TempName[21];
			SI32							TempUpdateTime;
			DBNUMERIC						TempNumeric;
			SI32							TempDBAccount;
			UI08							TempDBSlot;

			lpDBIKGetInvestment        = (DBIKInvestGetInvestment*)pIn;
			lpDBIKGetInvestment_Result = (DBIKInvestGetInvestment_Result*)pOut;

			lpDBIKGetInvestment_Result->siInvestmentCount       = 0;
			lpDBIKGetInvestment_Result->lpstVillageInvestorData = new stVillageInvestorData[128];
			ZeroMemory(lpDBIKGetInvestment_Result->lpstVillageInvestorData, sizeof(stVillageInvestorData) * 128);

			SQLDB.SQLRun("EXEC SPHQ_OnGetInvestment %d", lpDBIKGetInvestment->uiVillageCode);
			if(SQLDB.DBResults() == SUCCEED)
			{
				ZeroMemory(TempName, 21);
				
				SQLDB.DBBind(1, INTBIND,        0, (BYTE*)&TempDBAccount);		// 투자자 DB Account
				SQLDB.DBBind(2, TINYBIND,       0, (BYTE*)&TempDBSlot);			// 투자자 DB Slot
				SQLDB.DBBind(3, NTBSTRINGBIND,  0, (BYTE*)TempName);			// 투자자
				SQLDB.DBBind(4, TINYBIND,       0, (BYTE*)&TempChar);			// 투자자 Type
				SQLDB.DBBind(5, SRCNUMERICBIND, 0, (BYTE*)&TempNumeric);		// 투자 금액
				SQLDB.DBBind(6, INTBIND,        0, (BYTE*)&TempUpdateTime);		// 투자 시간
				while(SQLDB.DBNextRow() != NO_MORE_ROWS)
				{
					memcpy(&TempInt64, TempNumeric.val, sizeof(__int64));

					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_siInvestorDBAccount = TempDBAccount;
					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_uiInvestorSlot      = TempDBSlot;
					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_InvestorType        = TempChar;
					strncpy((char*)lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_InvestorID, (char*)TempName, ON_ID_LENGTH);
					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_Percent             = 0.0f;
					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_Money               = TempInt64;
					lpDBIKGetInvestment_Result->lpstVillageInvestorData[lpDBIKGetInvestment_Result->siInvestmentCount].m_UpdateTime          = TempUpdateTime;

					lpDBIKGetInvestment_Result->siInvestmentCount++;
					ZeroMemory(TempName, 21);
				};
			}
		}
		break;

	case SO_DB_INFOKIND_PUTITEM_FROMITEMBANK:
		{
			DBIKPutItemFromItemBank *lpDBInfo = (DBIKPutItemFromItemBank *)pIn;
			

			if( SQLDB.SQLRun( "EXEC SPHQ_On2PutItemFromItemBank %d, %d, %d, %d, %d, %d",
				lpDBInfo->DBAccount, lpDBInfo->uiSlot, lpDBInfo->uiFollowerID, lpDBInfo->uiPosition, lpDBInfo->siItemID, lpDBInfo->siQuantity ) == FALSE ) {
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{

				SQLDB.DBBind( 1, INTBIND, 0, (BYTE*)pOut );

				while( SQLDB.DBNextRow() != NO_MORE_ROWS);
			}

		}
		break;


	// Expo Event에 관련된 정보들을 얻어온다.
	case SO_DB_INFOKIND_EVENT_EXPO:
		{
			
			ExpoRank	*pExpoRankingSystem	=	( ExpoRank* )pOut;			
			SI32		siDBAccount, siBattleDBAccount;
			UI08		uiDBSlot, uiBattleDBSlot;			
			SI32		siWinNum, siLoseNum;
			SI32		siPoint;
			CHAR		szBirthDay[ 16 ];
			SI32		siCounter;
			SI32		siBattleCounter;			
			SI32		siDBAccounts[ 3000 ];
			
			struct	stExpoBattleInfo
			{
				SI32	siDBAccount;
				SI32	siBattleNum;
				UI08	uiDBSlot;
			};

			struct	stExpoData
			{
				SI32	siDBAccount;				
				SI32	siWinNum;
				SI32	siLoseNum;
				SI32	siPoint;
				
				SI32	siBattleDBAccount[ 3000 ];
				SI32	siBattleCount[ 3000 ];

				CHAR	szBirthDay[ 16 ];
				UI08	uiDBSlot;
			};


			stExpoData	*ExpoData; //[ 3000 ];

			ExpoData	=	new	stExpoData[ 3000 ];

			stExpoData	*pstExpoData;

			ZeroMemory( ExpoData, sizeof( stExpoData ) * 3000 ); 

			if( SQLDB.SQLRun( "EXEC SPHQ_On3ExpoCharacter" ) == FALSE ) {
				
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{				
				SQLDB.DBBind( 1, INTBIND,			0,	(BYTE *)&siDBAccount );
				SQLDB.DBBind( 2, TINYBIND,			0,	(BYTE *)&uiDBSlot );
				SQLDB.DBBind( 3, INTBIND,			0,	(BYTE *)&siWinNum );
				SQLDB.DBBind( 4, INTBIND,			0,	(BYTE *)&siLoseNum );
				SQLDB.DBBind( 5, INTBIND,			0,	(BYTE *)&siPoint );
				SQLDB.DBBind( 6, NTBSTRINGBIND,		0,	(BYTE *)szBirthDay );
				

				siCounter	=	0;
				while( SQLDB.DBNextRow() != NO_MORE_ROWS)
				{				
					if( siCounter >= 3000 )		break;

					pstExpoData		=	&ExpoData[ siCounter ];

					pstExpoData->siDBAccount	=	siDBAccount;
					pstExpoData->uiDBSlot		=	uiDBSlot;
					pstExpoData->siWinNum		=	siWinNum;
					pstExpoData->siLoseNum		=	siLoseNum;
					pstExpoData->siPoint		=	siPoint;

					strncpy( pstExpoData->szBirthDay, szBirthDay, 8 );

					siCounter++;
				}				
			}

			SQLDB.ClearCMDBuf();
			SQLDB.ClearRows();			

			//---------------------------------------------------------------------------------------------------------
			SI32		i, j;
		
			for( i = 0; i < 3000; i++ )				siDBAccounts[ i ] = ExpoData[ i ].siDBAccount;
			for( i = 0; i < 3000; i++ )				memcpy( ExpoData[ i ].siBattleDBAccount, siDBAccounts, sizeof( siDBAccounts ) );	

			

			if( SQLDB.SQLRun( "EXEC SPHQ_On3ExpoBattleInfo" ) == FALSE ) {
				
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind( 1, INTBIND,			0,	(BYTE *)&siDBAccount );
				SQLDB.DBBind( 2, TINYBIND,			0,	(BYTE *)&uiDBSlot );
				SQLDB.DBBind( 3, INTBIND,			0,	(BYTE *)&siBattleDBAccount );
				SQLDB.DBBind( 4, TINYBIND,			0,	(BYTE *)&uiBattleDBSlot );
				SQLDB.DBBind( 5, TINYBIND,			0,	(BYTE *)&siBattleCounter );

				while( SQLDB.DBNextRow() != NO_MORE_ROWS)
				{
					// A 유저의 전투 횟수를 설정
					for( i = 0; i < 3000; i++ )
					{
						if( ( ExpoData[ i ].siDBAccount != siDBAccount ) || ( ExpoData[ i ].uiDBSlot != uiDBSlot ) )	continue;						

						pstExpoData		=	&ExpoData[ i ];

						for( j = 0; j < 3000; j++ )
						{
							if( pstExpoData->siBattleDBAccount[ j ]  != siBattleDBAccount )							continue;							

							pstExpoData->siBattleCount[ j ]	+=	siBattleCounter;							
						}					

					}
					
					// B 유저의 전투 횟수를 설정
					for( i = 0; i < 3000; i++ )
					{
						if( ( ExpoData[ i ].siDBAccount != siBattleDBAccount ) || ( ExpoData[ i ].uiDBSlot != uiBattleDBSlot ) )	continue;						

						pstExpoData		=	&ExpoData[ i ];

						for( j = 0; j < 3000; j++ )
						{
							if( pstExpoData->siBattleDBAccount[ j ]  != siDBAccount )							continue;							

							pstExpoData->siBattleCount[ j ]	+=	siBattleCounter;							
						}					

					}
				}
			
			}			



			FILE	*fp;
			fp	=	fopen( "EventData.txt", "wt" );
			for( i = 0; i < 3000; i++ )
			{
				pExpoRankingSystem->AddUser(	ExpoData[ i ].siDBAccount, 																								
												ExpoData[ i ].siWinNum,
												ExpoData[ i ].siLoseNum,
												ExpoData[ i ].siPoint,
												ExpoData[ i ].siWinNum + ExpoData[ i ].siLoseNum,
												ExpoData[ i ].szBirthDay,
												ExpoData[ i ].siBattleDBAccount,
												ExpoData[ i ].siBattleCount );

				fprintf( fp, "%d, 0, %d, %d, %d, %s\n", ExpoData[ i ].siDBAccount, ExpoData[ i ].siWinNum, ExpoData[ i ].siLoseNum, 
													ExpoData[ i ].siPoint, ExpoData[ i ].szBirthDay );
			}

			fclose( fp );

			if( ExpoData )			
			{
				delete [] ExpoData;
				ExpoData	=	NULL;
			}
		}
		break;

	case SO_DB_INFO_KIND_GET_GM_LIST:
		{
			cltGMManager		*pclGMManager;
			SI32				siAccount, siGrade, siWorld;
			CHAR				szIP[ 128 ];
			CHAR				szID[ ON_ID_LENGTH + 1 ];

			pclGMManager	=	( cltGMManager* )pOut;

			
			if( SQLDB.SQLRun( "EXEC SPHQ_On3getAdminList" ) == FALSE ) {
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{				
				SQLDB.DBBind( 1, INTBIND,			0,		( BYTE* )&siAccount );
				SQLDB.DBBind( 2, INTBIND,			0,		( BYTE* )&siGrade );
				SQLDB.DBBind( 3, INTBIND,			0,		( BYTE* )&siWorld );
				SQLDB.DBBind( 4, NTBSTRINGBIND,		0,		( BYTE* )szIP );
				SQLDB.DBBind( 5, NTBSTRINGBIND,		0,		( BYTE* )szID );

				while( SQLDB.DBNextRow() != NO_MORE_ROWS)
				{
					pclGMManager->AddGM( szIP );
				}
			}

		}
		break;

	case SO_DB_INFOKIND_WAR_LOGIN:
		{
			DBIKPlayerDBInfo*	pPlayerInfo = (DBIKPlayerDBInfo*)	pIn;			
			LoginInfo*			pInfo		= (LoginInfo*)	pOut;
			CharacterBaseInfo*	pBase = &pInfo->charBaseInfo;
			CharacterWarInfo*   pWar  = &pInfo->charWarInfo;


			if( SQLDB.SQLRun( "EXEC SPHQ_On2WarLogin %d, %d", pPlayerInfo->DBAccount, pPlayerInfo->uiSlot ) == FALSE )
			{
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind( 1, SMALLBIND,(DBSMALLINT)0,  (BYTE *)&pWar->PrevKind);
				SQLDB.DBBind( 2, INTBIND,  (DBINT)0,       (BYTE *)&pWar->Hp);
				SQLDB.DBBind( 3, INTBIND, (DBINT)0,		   (BYTE *)&pWar->Mp);
				SQLDB.DBBind( 4, TINYBIND, (DBTINYINT)0,   (BYTE *)&pWar->bCaptain);
				SQLDB.DBBind( 5, INTBIND,	(DBINT)0,	   (BYTE *)&pWar->Delay);				
//				SQLDB.DBBind( 5, INTBIND, (DBINT)0,		   (BYTE *)&pWar->Supply);
			}
			while(SQLDB.DBNextRow() != NO_MORE_ROWS);

			//공성유닛인 상태이면.
			if(pWar->PrevKind != 0 && pWar->Hp > 0)
			{
				WORD siTempKind = pBase->kind;
				pBase->kind     = pWar->PrevKind;
				pWar->PrevKind  = siTempKind;
			}
		}
		break;
		
	case SO_DB_INFOKIND_GET_WARGIVEUP:
		{
			UI32    *puiGuildID  = (UI32*)pIn;      
			DBIKWarGiveUp *pGiveUp = (DBIKWarGiveUp*)pOut;

			if( SQLDB.SQLRun( "EXEC SPHQ_On2GetWarGiveup %d", *puiGuildID) == FALSE )
			{
				bSuccess = FALSE;
				break;
			}

			if((SQLDB.DBResults()) == SUCCEED)
			{
				SQLDB.DBBind( 1, SMALLBIND, (DBSMALLINT)0, (BYTE *)&pGiveUp->uiGuildID);
				SQLDB.DBBind( 2, SMALLBIND, (DBSMALLINT)0, (BYTE *)&pGiveUp->uiVillageCode);
				SQLDB.DBBind( 3, INTBIND, (DBINT)0,        (BYTE *)&pGiveUp->mnMoney);
				SQLDB.DBBind( 4, TINYBIND, (DBTINYINT)0,   (BYTE *)&pGiveUp->siFlag);
			}
			while(SQLDB.DBNextRow() != NO_MORE_ROWS);
		}
		break;
	}


	SQLDB.ClearCMDBuf();
	SQLDB.ClearRows();

	m_countGetDB++;

	LeaveCriticalSection( &m_cs1 );

	return bSuccess;
}



//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 로그인 정보를 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------------
bool  SODatabase::GetLoginInfo( DWORD dbAccount, char slot, char *ip, void* outPut )
{

	LoginInfo* pInfo = (LoginInfo*)outPut;

	
	// -- RunProcedure
	//-------------------------------------------------------------------------------------------------------------------------
	if( SQLDB.SQLRun("EXEC SPHQ_OnGetLoginInfo %d, %d, '%s', %d", 
		dbAccount, slot, ip, g_GameServerInfoParser.GetWorldID()) == FALSE )
	{	
		return FALSE;
	}

	
	// 1. 캐릭터 기본 정보를 얻어온다 
	CharacterBaseInfo*	pBase = &pInfo->charBaseInfo;
	CHAR				szTempName[ON_ID_LENGTH+2];
	//-------------------------------------------------------------------------------------------------------------------------	
	if ( SQLDB.DBResults() == SUCCEED ) 
	{				
		SQLDB.DBBind (1,  INTBIND,	         (DBINT)0,	(BYTE *)&pBase->account		 ); // account
		SQLDB.DBBind (2,  NTBSTRINGBIND,	(DBCHAR)0,	(BYTE *)szTempName	         ); // name
		SQLDB.DBBind (3,  SMALLBIND,	(DBSMALLINT)0,	(BYTE *)&pBase->kind         ); // kind
		SQLDB.DBBind (4,  INTBIND,			 (DBINT)0,	(BYTE *)&pBase->userGrade    ); // userGrade
		SQLDB.DBBind (5,  SMALLBIND,	(DBSMALLINT)0,	(BYTE *)&pBase->satiety	     ); // satiety

		// 임시 변수 
		DBNUMERIC tempNumeric, tempNumeric2, tempNumeric3,tempNumeric4;
		memset(&tempNumeric, 0, sizeof(DBNUMERIC));
		memset(&tempNumeric2, 0, sizeof(DBNUMERIC));
		memset(&tempNumeric3, 0, sizeof(DBNUMERIC));
		memset(&tempNumeric4, 0, sizeof(DBNUMERIC));

		SQLDB.DBBind (6,  SRCNUMERICBIND,	        0,  (BYTE *)&tempNumeric		 ); // money
		SQLDB.DBBind (7,  SRCNUMERICBIND,	        0,  (BYTE *)&tempNumeric2		 ); // saving                                                                             ; // Saving
		SQLDB.DBBind (8,  SRCNUMERICBIND,	        0,  (BYTE *)&tempNumeric3		 ); // investProfit
		SQLDB.DBBind (9,  SRCNUMERICBIND,	        0,  (BYTE *)&tempNumeric4		 ); // additionProfit
		
		SQLDB.DBBind (10,  SMALLBIND,	(DBSMALLINT)0,	(BYTE *)&pBase->x_pos        ); // x_pos
		SQLDB.DBBind (11, SMALLBIND,	(DBSMALLINT)0,	(BYTE *)&pBase->y_pos	     ); // y_pos		
		SQLDB.DBBind (12, SMALLBIND,	(DBSMALLINT)0,	(BYTE *)&pBase->mapID		 ); // mapID
		SQLDB.DBBind (13, SMALLBIND,	(DBSMALLINT)0,  (BYTE *)&pBase->nearVillage  );	// nearVillage

		SQLDB.DBBind (14, INTBIND,	         (DBINT)0,  (BYTE *)&pBase->credit_trade ); // credit_trade
		SQLDB.DBBind (15, INTBIND,	         (DBINT)0,  (BYTE *)&pBase->credit_daily ); // credit_daily

		SQLDB.DBBind (16, INTBIND,	         (DBINT)0,  (BYTE *)&pBase->guildID		 ); // guildID
		SQLDB.DBBind (17, INTBIND,			 (DBINT)0,  (BYTE *)&pBase->guildGrade	 ); // guildGrade
		
		SQLDB.DBBind (18, TINYBIND,			 (DBINT)0,  (BYTE *)&pBase->VMercenary); // VMercenary
		SQLDB.DBBind (19, TINYBIND,		 (DBTINYINT)0,  (BYTE *)&pBase->maxGeneral); // 고용 가능한 장수수

		while ( SQLDB.DBNextRow() != NO_MORE_ROWS ) 
		{
			strcpy( pBase->name, szTempName );

			////////////////////////////////////////////////////
			if( strlen(szTempName) >= 11 )
			{
				writeInfoToFile( "Bingo.txt", "11111111111 name( %s )", szTempName );
				//writeInfoToFile( "Bingo.txt", "11111111111 name(%s)  strlen(%d" );//strlen(szTempName) );
				//writeInfoToFile( "SoDataBaseError.txt", "FriendListError [ %d ]  strlen[ %s ] %d, ", 10, szTempName, 10, 109 );

				writeInfoToFile( "SoDataBaseError.txt", "FriendListError [ %d ], [ %s ], [ %u ]", 10, szTempName, 10 );
			}	
			////////////////////////////////////////////////////

			memcpy(&pBase->money,			tempNumeric.val,  sizeof(__int64));
			memcpy(&pBase->saving,	        tempNumeric2.val, sizeof(__int64));

			if (tempNumeric2.sign == 0)
			{


				pBase->saving = -pBase->saving;

			}

			memcpy(&pBase->investProfit,	tempNumeric3.val, sizeof(__int64));
			memcpy(&pBase->additionProfit,	tempNumeric4.val, sizeof(__int64));
		}

		
	}			

	// 2. 완료된 퀘스트 정보를 얻어온다 
	WORD* pCompleteQuest = pInfo->completeQuests;
	//-------------------------------------------------------------------------------------------------------------------------
	if ( SQLDB.DBResults() == SUCCEED )
	{
		int	 count = 0;	
		WORD questCode;
		
		SQLDB.DBBind (1, SMALLBIND, 0, (BYTE*)&questCode);		// completeQuest Code 	

		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{
			if( count >= ON_QUSETDATA_MAXCOMLETEDQUEST )
			{
				writeInfoToFile("SoDataBaseError.txt", "CompletedQuestError [ %d ], [ %u ]", count, dbAccount );
/*				FILE	*fp;

				if( ( fp  = fopen( "..\\SoDataBaseError.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "CompletedQuestError [ %d ], [ %u ]\n", count, dbAccount );
					fclose( fp );
				}
*/
			}

			pCompleteQuest[count++] = questCode;
		}
	}



	// 3. 진행중인 퀘스트 정보를 얻어온다 
	ProgressQuestInfo* pProgressQuest = pInfo->progressQuests;
	//-------------------------------------------------------------------------------------------------------------------------
	if ( SQLDB.DBResults() == SUCCEED )
	{		
		WORD	slot;

		WORD	questCode;
		DWORD	questObject1,questObject2;


		SQLDB.DBBind(1, SMALLBIND, 0, (BYTE*)&slot);			// Quest Slot	
		SQLDB.DBBind(2, SMALLBIND, 0, (BYTE*)&questCode);		// Quest Code
		SQLDB.DBBind(3, INTBIND,   0, (BYTE*)&questObject1);	// Quest Object Data1				
		SQLDB.DBBind(4, INTBIND,   0, (BYTE*)&questObject2);	// Quest Object Data2


		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{
			if( slot >= ON_QUSETDATA_MAXPLAYINGQUEST )
			{
				writeInfoToFile("SoDataBaseError.txt", "ProgressQuestError [ %d ], [ %u ]\n", slot, dbAccount );
				/*
				FILE	*fp;

				if( ( fp  = fopen( "..\\SoDataBaseError.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "ProgressQuestError [ %d ], [ %u ]\n", slot, dbAccount );
					fclose( fp );
				}
				*/
			}

			pProgressQuest[slot].questCode		= questCode;
			pProgressQuest[slot].questObject1	= questObject1;
			pProgressQuest[slot].questObject2  = questObject2;
		}	
	}



	// 4. 용병들 정보를 얻어온다 
	MercenaryInfo* pMercenary = pInfo->mercenaryInfos;
	//-------------------------------------------------------------------------------------------------------------------------
	if ( SQLDB.DBResults() == SUCCEED )
	{	
		WORD		slot;
		char		name[12];
		WORD		kind;

		DWORD		Exp,Level,Str,Dex,Vit,Int,Life,Mana,AC,Bonus,DamageRegist,MagicRegist,EquipMinDamage,EquipMaxDamage;


		SQLDB.DBBind (1,	SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&slot);
		SQLDB.DBBind (2,	NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)&name); 
		SQLDB.DBBind (3,	SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&kind);
		
		SQLDB.DBBind (4,	INTBIND,		 (DBINT) 0,	(BYTE *)&Exp);
		SQLDB.DBBind (5,	INTBIND,		 (DBINT) 0,	(BYTE *)&Level);
		SQLDB.DBBind (6,	INTBIND,		 (DBINT) 0,	(BYTE *)&Str);
		SQLDB.DBBind (7,	INTBIND,		 (DBINT) 0,	(BYTE *)&Dex);
		SQLDB.DBBind (8,	INTBIND,		 (DBINT) 0,	(BYTE *)&Vit);
		SQLDB.DBBind (9,	INTBIND,		 (DBINT) 0,	(BYTE *)&Int);
		SQLDB.DBBind (10,	INTBIND,		 (DBINT) 0,	(BYTE *)&Life);
		SQLDB.DBBind (11,	INTBIND,		 (DBINT) 0,	(BYTE *)&Mana);
		SQLDB.DBBind (12,	INTBIND,		 (DBINT) 0,	(BYTE *)&AC);
		SQLDB.DBBind (13,	INTBIND,		 (DBINT) 0,	(BYTE *)&Bonus);
		SQLDB.DBBind (14,	INTBIND,		 (DBINT) 0,	(BYTE *)&DamageRegist);
		SQLDB.DBBind (15,	INTBIND,		 (DBINT) 0,	(BYTE *)&MagicRegist);
		SQLDB.DBBind (16,	INTBIND,		 (DBINT) 0,	(BYTE *)&EquipMinDamage);
		SQLDB.DBBind (17,	INTBIND,		 (DBINT) 0,	(BYTE *)&EquipMaxDamage);
			
		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{					
			if( slot >= ON_MAX_FOLLOWER_NUM )
			{
				writeInfoToFile("SoDataBaseError.txt", "MercenaryInfo [ %d ], [ %u ]\n", slot, dbAccount );
/*
				FILE	*fp;

				if( ( fp  = fopen( "..\\SoDataBaseError.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "MercenaryInfo [ %d ], [ %u ]\n", slot, dbAccount );
					fclose( fp );
				}
*/
			}


			memcpy( pMercenary[slot].name, name, LOGIN::NAME_MAX_LENGTH );
			
			////////////////////////////////////////////////////
			if( strlen(name) >= 11 )
			{
				writeInfoToFile( "Bingo.txt", "222222222 name(%s)  strlen(%d)", name, strlen(name) );
			}
			////////////////////////////////////////////////////

			pMercenary[slot].slot			= slot;
			pMercenary[slot].kind			= kind;
			pMercenary[slot].Exp			= Exp;
			pMercenary[slot].Level			= Level;
			pMercenary[slot].Str			= Str;
			pMercenary[slot].Dex			= Dex;
			pMercenary[slot].Vit			= Vit;
			pMercenary[slot].Int			= Int;
			pMercenary[slot].Life			= Life;
			pMercenary[slot].Mana			= Mana;
			pMercenary[slot].AC				= AC; 
			pMercenary[slot].Bonus			= Bonus;
			pMercenary[slot].DamageRegist   = DamageRegist;
			pMercenary[slot].MagicRegist	= MagicRegist;
			pMercenary[slot].EquipMinDamage = EquipMinDamage;
			pMercenary[slot].EquipMaxDamage = EquipMaxDamage;		
		}
	}


	// 5. 용병들의 인벤토리 내 아이템 정보를 얻어 온다 	
	//-------------------------------------------------------------------------------------------------------------------------
	if ( SQLDB.DBResults() == SUCCEED )
	{	
		WORD	slot = 0;
		WORD	itemID;
		WORD    posInInventory;
		WORD	quantity;

		SQLDB.DBBind (1, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&slot);
		SQLDB.DBBind (2, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&itemID);
		SQLDB.DBBind (3, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&posInInventory);
		SQLDB.DBBind (4, SMALLBIND,	(DBSMALLINT) 0,	(BYTE *)&quantity);	

		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{		
//			if( ( slot >= ON_MAX_FOLLOWER_NUM ) || ( posInInventory >= 10 ) )
			if( ( slot >= ON_MAX_FOLLOWER_NUM ) || ( posInInventory >= ON_MYDATA_MAX_ITEM_NUM ) )
			{
				writeInfoToFile("SoDataBaseError.txt", "MercenaryItemError [ %d ][ %d ][ %u ]\n", slot, posInInventory, dbAccount );
/*
				FILE	*fp;
				if( ( fp  = fopen( "..\\SoDataBaseError.txt", "at" ) ) != NULL )
				{
					fprintf( fp, "MercenaryItemError [ %d ][ %d ][ %u ]\n", slot, posInInventory, dbAccount );
					fclose( fp );
				}
*/
			}

			pInfo->mercenaryItemInfos[slot][posInInventory].itemID		   = itemID;
			pInfo->mercenaryItemInfos[slot][posInInventory].posInInventory = posInInventory;
			pInfo->mercenaryItemInfos[slot][posInInventory].quantity	   = quantity;			
		}
	}

	// 7. 장착된 아이템 정보를 얻어 온다
	EquipItemInfo* pEquip	  = pInfo->mercenaryEquipItemInfos;
	//-------------------------------------------------------------------------------------------------------------------------
	if ( SQLDB.DBResults() == SUCCEED )
	{
		WORD			slot = 0;
		EquipItemInfo	TempItemInfo;

		SQLDB.DBBind (1, SMALLBIND,  (DBSMALLINT)  0, (BYTE *)&slot);
		//Head
		SQLDB.DBBind (2, SMALLBIND,  (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_HELMET_ITEM]);
		SQLDB.DBBind (3, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_HELMET_ITEM]);
		SQLDB.DBBind (4, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_HELMET_ITEM]);
		//belt
		SQLDB.DBBind (5, SMALLBIND,  (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_BELT_ITEM]);
		SQLDB.DBBind (6, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_BELT_ITEM]);
		SQLDB.DBBind (7, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_BELT_ITEM]);
		//glove
		SQLDB.DBBind (8, SMALLBIND,  (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_GLOVE_ITEM]);
		SQLDB.DBBind (9, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_GLOVE_ITEM]);
		SQLDB.DBBind (10,INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_GLOVE_ITEM]);
		//armor
		SQLDB.DBBind (11, SMALLBIND,  (DBSMALLINT) 0, (BYTE *)&TempItemInfo.id[ON_WEAR_ARMOR_ITEM]);
		SQLDB.DBBind (12, INTBIND,	   (DBINT)	   0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_ARMOR_ITEM]);
		SQLDB.DBBind (13, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_ARMOR_ITEM]);
		//weapon
		SQLDB.DBBind (14, SMALLBIND, (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_WEAPON_ITEM]);
		SQLDB.DBBind (15, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_WEAPON_ITEM]);
		SQLDB.DBBind (16, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_WEAPON_ITEM]);
		//ring1
		SQLDB.DBBind (17, SMALLBIND, (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_RRING_ITEM]);
		SQLDB.DBBind (18, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_RRING_ITEM]);
		SQLDB.DBBind (19, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_RRING_ITEM]);
		//ring2
		SQLDB.DBBind (20, SMALLBIND, (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_LRING_ITEM]);
		SQLDB.DBBind (21, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_LRING_ITEM]);
		SQLDB.DBBind (22, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_LRING_ITEM]);

		//shoes
		SQLDB.DBBind (23, SMALLBIND, (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_SHOES_ITEM]);
		SQLDB.DBBind (24, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_SHOES_ITEM]);
		SQLDB.DBBind (25, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_SHOES_ITEM]);

		//guardian
		SQLDB.DBBind (26, SMALLBIND, (DBSMALLINT)  0, (BYTE *)&TempItemInfo.id[ON_WEAR_GUARDIAN_ITEM]);
		SQLDB.DBBind (27, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.startTime[ON_WEAR_GUARDIAN_ITEM]);
		SQLDB.DBBind (28, INTBIND,	   (DBINT)     0, (BYTE *)&TempItemInfo.endTime[ON_WEAR_GUARDIAN_ITEM]);

		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{
			if( slot >= ON_MAX_FOLLOWER_NUM )
			{
				writeInfoToFile("SoDataBaseError.txt", "EquipItemError [ %d ], [ %u ]\n", slot, dbAccount );
			}
			memcpy(&pEquip[slot],&TempItemInfo,sizeof(EquipItemInfo));
		}
	}



	// Ticket
	pInfo->charBaseInfo.ticketKind = -1;
	if ( SQLDB.DBResults() == SUCCEED )
	{
		SQLDB.DBBind(1, INTBIND, (DBINT)0, (BYTE*)&pInfo->charBaseInfo.ticketKind);
		
		while ( SQLDB.DBNextRow() != NO_MORE_ROWS );
	}

	// Friend List
	if ( SQLDB.DBResults() == SUCCEED )
	{
		char	szID[12];
		int		num	=	0;

		SQLDB.DBBind (1, NTBSTRINGBIND,	(DBCHAR) 0,	(BYTE *)&szID); 

		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{
			if( ( num >= MAX_FRIEND_NUM ) || ( strlen( szID ) >= ( ON_ID_LENGTH + 1 ) ) )
			{
				writeInfoToFile( "SoDataBaseError.txt", "FriendListError [ %d ], [ %s ], [ %u ]", num, szID, dbAccount );
			}

			strcpy( pInfo->friendInfos[num++].szID, szID );
		}
	}


	SQLDB.ClearCMDBuf();
	SQLDB.ClearRows();


	return TRUE;
}
                    


//-----------------------------------------------------------------------------------------------------------------------------
//  name :
//  desc : 아이템 경매 정보들을 얻어온다 
//-----------------------------------------------------------------------------------------------------------------------------
bool SODatabase::GetItemAuctions(void* pParam)
{
	CAuctionMarket* pAuctionMarket = (CAuctionMarket*)pParam;

	if( SQLDB.SQLRun("EXEC SPHQ_OnGetAuctionItem") == FALSE )
	{
		return FALSE;
	}


	if( SQLDB.DBResults() == SUCCEED )
	{
		AUCTION::sArticleItemInfo	itemInfo;
		AUCTION::sBaseInfo			auctionInfo;
		CHAR						szTempAuctioneer[LOGIN::NAME_MAX_LENGTH+2];

		memset(&itemInfo, 0, sizeof(itemInfo));
		memset(&auctionInfo, 0, sizeof(auctionInfo));

		
		SQLDB.DBBind(1, INTBIND,	   (DBINT)	   0, (BYTE*)&auctionInfo.auctioneerDBAccount);
		SQLDB.DBBind(2, TINYBIND,	   (DBTINYINT) 0, (BYTE*)&auctionInfo.auctioneerCharSlot);		
	    SQLDB.DBBind(3,	NTBSTRINGBIND, (DBCHAR)    0, (BYTE*)&szTempAuctioneer); 
		SQLDB.DBBind(4, INTBIND,	   (DBINT)     0, (BYTE*)&auctionInfo.minPrice);
		SQLDB.DBBind(5, INTBIND,	   (DBINT)	   0, (BYTE*)&auctionInfo.maxPrice);
		SQLDB.DBBind(6, TINYBIND,	   (DBTINYINT) 0, (BYTE*)&auctionInfo.numFail);
		SQLDB.DBBind(7, SMALLBIND,	   (DBSMALLINT)0, (BYTE*)&itemInfo.itemID);
		SQLDB.DBBind(8, SMALLBIND,	   (DBSMALLINT)0, (BYTE*)&itemInfo.quantity);


		while ( SQLDB.DBNextRow() != NO_MORE_ROWS )
		{
			// 유찰 회수 만큼 최소가를 감소 시킨다 
			for(int i=0; i < auctionInfo.numFail; ++i) {

				 auctionInfo.minPrice -= auctionInfo.minPrice * 0.2;
			}

			auctionInfo.articleKind = AUCTION::ARTICLE_ITEM;
			auctionInfo.setData( (char*)&itemInfo, sizeof(itemInfo) );
			strcpy( auctionInfo.strAuctioneer, szTempAuctioneer );
			
			if( !pAuctionMarket->addNewAuction( &auctionInfo ) ) 
			{				
				getch();
			}
		}
	}

	SQLDB.ClearCMDBuf();
	SQLDB.ClearRows();

	return TRUE;
}