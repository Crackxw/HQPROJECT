#include <windows.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"
#include "DBQueueProc.h"
#include "DBSubQueue.h"

//----------------------------
#include "SOMain.h"
#include "SOCommonHeader.h"
#include "MSSQLDB.h"
#include "SOVillage.h"
#include "IGuild.h"
#include "SODBStructure.h"
//----------------------------



// CAuthServerProc
#include "../kojimodo/utility/Winsock.h"
#include "AuthServerProc.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자 
//--------------------------------------------------------------------------------------------
CDBSubQueue::CDBSubQueue(int a, int b): CDBQueueProc(a, b)
{
	m_pAuthServer = new CAuthServerProc();	
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소멸자 
//--------------------------------------------------------------------------------------------
CDBSubQueue::~CDBSubQueue()
{
	if( m_pAuthServer ) {

		delete m_pAuthServer;
	}
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 처리 할수 있는 쿼리 인가를 알아 본다 
//--------------------------------------------------------------------------------------------
bool CDBSubQueue::isAbleQuery(char* data)
{
	DBQUERY_HEADER* header = (DBQUERY_HEADER*)data;
		
	if( header->command >= 300 && header->command < 350 ) return TRUE;
		
	return FALSE;	
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 쿼리를 처리 한다 
//--------------------------------------------------------------------------------------------
void CDBSubQueue::setDBInfo(DWORD command, char* data)
{
	BOOL	bSuccess = TRUE;
	DWORD	result	 = 0;

	//--------------------------------
	SI32		siInfoKind = command;
	LPVOID		pIn		   = data;
	LPVOID		pOut	   = &result;	
	//--------------------------------


	switch(siInfoKind)
	{
		// 로그아웃 시작을 설정한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_LOGOUTSTART:
		{

			sDB_LogoutStart	  *pLogoutStart;
			pLogoutStart = (sDB_LogoutStart*)	pIn;
			

			if(m_pSQL->SQLRun("EXEC SPHQ_OnLogoutStart %d", 
					pLogoutStart->dbAccount) == false)					
			{	
				
				bSuccess	=	FALSE;
				break;
			}

			
			if ((m_pSQL->DBResults()) == SUCCEED)
			{	
				
				// 결과값은 현재 무시된다 				
				int result = 0;
				m_pSQL->DBBind(1, INTBIND, (DBINT)0, (BYTE*)&result);


				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );					
		
				//=====================================================			 								   											
				// AUTHENTICATION: 인증서버에 로그아웃을 요청 한다 
				//=====================================================
				
				m_pAuthServer->RequestLogout( pLogoutStart->id, 
											  pLogoutStart->password, 
											  pLogoutStart->ip, 
											  pLogoutStart->dbAccount );

								
			}			
		}
		break;




		// 로그아웃 정보를 설정한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_LOGOUTINFO:
		{

			sDB_LogoutInfo	  *pLogoutInfo;
			pLogoutInfo = (sDB_LogoutInfo*)	pIn;
			

			if(m_pSQL->SQLRun("EXEC SPHQ_On2LogoutInfo %d, %d, %d, %d, %d, '%s'", 
					pLogoutInfo->dbAccount, 
					pLogoutInfo->slot, 
					pLogoutInfo->xpos, 
					pLogoutInfo->ypos, 
					pLogoutInfo->mapID,
					pLogoutInfo->IP ) == false) 
			{		
				
				bSuccess	=	FALSE;
				break;

			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
				
			}

		}
		break;

		case SO_DB_INFOKIND_WARINFO:
		{
			sDB_LogoutWarInfo*	LogoutWarInfo;
			LogoutWarInfo   =   (sDB_LogoutWarInfo*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_On2WarLogout %d, %d, %d, %d, %d, %d, %d", 
					LogoutWarInfo->dbAccount, 
					LogoutWarInfo->slot,
					LogoutWarInfo->PrevKind,
					LogoutWarInfo->bCaptain,
					LogoutWarInfo->Hp,
					LogoutWarInfo->MP,
					LogoutWarInfo->siDelay) == false)
			{		
				bSuccess	=	FALSE;
				break;
			}

			if ((m_pSQL->DBResults()) == SUCCEED)
			{				
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );
			}
		}
		break;

		// 로그 아웃시에 포만감을 저장 한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_SETSATIETY:
			{			
				DBIKSatiety		*pSatiety;

				pSatiety	=	(DBIKSatiety*)pIn;

				if(m_pSQL->SQLRun("EXEC SPHQ_OnHungrySetValue %d, %d, %d", pSatiety->DBAccount, pSatiety->uiSlot, pSatiety->siSatiety) == false)
				{
					bSuccess = FALSE;
					break;
				}

				m_pSQL->DBResults();
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			break;


		// 로그 아웃시에 용병의 모든 파라미터를 저장 한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_LOGOUTSAVEALLPARAM:
			{
				DBIKFollowerParam	*pFollowerInfo;

				pFollowerInfo	=	(DBIKFollowerParam*)pIn;

					
				if( pFollowerInfo->Mana >= 10000 )
				{
					writeInfoToFile("SO_DB_INFOKIND_LOGOUTSAVEALLPARAM.txt", "[ %u, %d, %d ] :: [ %d ] \n", 
									pFollowerInfo->DBAccount, pFollowerInfo->uiSlot, 
									pFollowerInfo->uiFollowerSlot, pFollowerInfo->Mana );
/*
					FILE	*fp;
					if( ( fp = fopen( "..\\SO_DB_INFOKIND_LOGOUTSAVEALLPARAM.txt", "at" ) ) != NULL )
					{
						fprintf( fp, "[ %u, %d, %d ] :: [ %d ] \n", pFollowerInfo->DBAccount, pFollowerInfo->uiSlot, 
																	pFollowerInfo->uiFollowerSlot, pFollowerInfo->Mana );
						fclose( fp );
					}
*/
				}

				if(m_pSQL->SQLRun("EXEC SPHQ_OnSetMercenaryParam %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
									pFollowerInfo->DBAccount,
									pFollowerInfo->uiSlot,
									pFollowerInfo->uiFollowerSlot,
									pFollowerInfo->Kind,
									pFollowerInfo->Exp,
									pFollowerInfo->Level,
									pFollowerInfo->Str,
									pFollowerInfo->Dex,
									pFollowerInfo->Vit,
									pFollowerInfo->Int,
									pFollowerInfo->Life,
									pFollowerInfo->Mana,
									pFollowerInfo->AC,
									pFollowerInfo->Bonus,
									pFollowerInfo->DmResist,
									pFollowerInfo->MgResist,
									pFollowerInfo->EquipMin,
									pFollowerInfo->EquipMax) == false)
				{				
					bSuccess	=	FALSE;
					break;
				}

				m_pSQL->DBResults();
				while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
			}
			break;


		// 마을의 출입자수를 설정한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_VILLAGEENTERPLAYERSET:
		{
			DBIKVillageEnterPlauerSet*			lpDBIKVillageEnterPlauerSet;

			lpDBIKVillageEnterPlauerSet = (DBIKVillageEnterPlauerSet*)pIn;
			m_pSQL->SQLRun("EXEC SPHQ_OnVillageEnterPlayerSet %d, %d", lpDBIKVillageEnterPlauerSet->uiVillageCode, lpDBIKVillageEnterPlauerSet->uiPlayerNum);

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;


		// 현재 시각을 설정한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_SETWORLDTIME:
		{
			DBIKWorldTime		*pTime;

			pTime	=	(DBIKWorldTime*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_OnSetWorldTime %d, %d, %d, %d", 
							pTime->Year, pTime->Month, pTime->Day, pTime->Hour) == false)							
			{								
				bSuccess = FALSE;
				break;
			}			

			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}		
		break;


		// 현재 동시 접속자수를 설정한다 
		//----------------------------------------------------------------------------------------
		case SO_DB_INFOKIND_CONCURRENTUSER:
		{
			DWORD *currentUser = (DWORD*)pIn;

			if(m_pSQL->SQLRun("EXEC SPHQ_SetConcurrent %d", *currentUser))
			{
				bSuccess = FALSE;
				break;
			}
			
			m_pSQL->DBResults();
			while( m_pSQL->DBNextRow() != NO_MORE_ROWS );		
		}
		break;
			
		default:
			return;
	}


	m_pSQL->ClearCMDBuf();
	m_pSQL->ClearRows();
}
