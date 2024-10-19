//--------------------------------------------------------------------------
//   H.Q ONLINE SERVER
//   Copyright (c) 2000 
//--------------------------------------------------------------------------
//   MSSQLDB.cpp   : DataBase���� �����Ѵ�.
//--------------------------------------------------------------------------
#include <windows.h>
#include <time.h>
#include <stdio.h>

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "MSSQLDB.h"

#include "SOMain.h"




//--------------------------------------------------------------------------------------------
// Name: HQSQLDB()
// Desc: 
//--------------------------------------------------------------------------------------------
HQSQLDB::HQSQLDB() 
{	
}



//--------------------------------------------------------------------------------------------
// Name: ~HQSQLDB() 
// Desc: 
//--------------------------------------------------------------------------------------------
HQSQLDB::~HQSQLDB() 
{	
}



//--------------------------------------------------------------------------------------------
// Name: DBConnect()
// Desc: ������ �����Ѵ�.
//--------------------------------------------------------------------------------------------
bool HQSQLDB::DBConnect(char* DATABASENAME, char* DEVICENAME, char* USERNAME, char* PWD) 
{		
	login = dblogin();

	DBSETLUSER(login, USERNAME);
	DBSETLPWD(login, PWD);
	DBSETLVERSION(login,DBVER60);
	
	if((dbproc=dbopen(login,DATABASENAME)) == NULL) return false;
	
	if(dbuse(dbproc,DEVICENAME) == FAIL) return false;

	return true;
}



//--------------------------------------------------------------------------------------------
// Name: SQLRun()
// Desc: SQL ������ �����Ѵ�.
//--------------------------------------------------------------------------------------------
bool  HQSQLDB::SQLRun(char* SQL,...) 
{
	va_list ap;
	memset(CMD, 0, sizeof(CMD));
	va_start(ap, SQL);
	_vsnprintf(CMD,1024,SQL,ap);
	va_end(ap);     
				
	//printf("%s \n", CMD);

	if( dbcmd(dbproc,CMD) == FAIL )
	{
		//---------------------------------------------------------------------------------
		writeInfoToFile( "DBCmdFailLog.txt", "%s Fail!", CMD );
		//---------------------------------------------------------------------------------		
	}


	if(dbsqlexec(dbproc) == FAIL) 
	{	
		//---------------------------------------------------------------------------------
		writeInfoToFile( "DBError.txt", "%s Fail!", CMD );
		//---------------------------------------------------------------------------------		

		if( strstr( CMD, "EXEC SPHQ_OnCountByItemBank" ) != NULL )
		{
			// it's try reconnect.
			DBClose();			
			
			
			if( DBConnect(g_GameServerInfoParser.GetDB2IPString(), g_GameServerInfoParser.GetDBName2String(), 
						g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd") == false )
			{
				//---------------------------------------------------------------------------------
				writeInfoToFile( "SPHQ_OnCountByItemBank.txt", "%s DB Reconnect failed!", CMD );
				//---------------------------------------------------------------------------------		
			}
			else
			{			
				//---------------------------------------------------------------------------------
				writeInfoToFile( "SPHQ_OnCountByItemBank.txt", "%s DB Reconnect successed!", CMD );
				//---------------------------------------------------------------------------------		
			}


		}
		else
		{

			g_forceShutdown = TRUE;
			

			// it's try reconnect.
			DBClose();
			DBConnect(g_GameServerInfoParser.GetDB1IPString(), g_GameServerInfoParser.GetDBName1String(), 
					  g_GameServerInfoParser.GetDBUserString(), "dhs1fk2dls3rj4tkd");
		}

		return false;
	}

	return true;
}



//--------------------------------------------------------------------------------------------
// Name: GetCMD()
// Desc: CMD ���۸�  �޾ư���.
//--------------------------------------------------------------------------------------------
CHAR*	HQSQLDB::GetCMD()
{
	return CMD;
}



//--------------------------------------------------------------------------------------------
// Name: dbresults()
// Desc: Result�� �޾ư���.
//--------------------------------------------------------------------------------------------
RETCODE HQSQLDB::DBResults()
{
	return ::dbresults(dbproc);
}



//--------------------------------------------------------------------------------------------
// Name: dbbind()
// Desc: ������ ���¸� �޾ư���.
//--------------------------------------------------------------------------------------------
RETCODE HQSQLDB::DBBind(INT column, INT vartype, DBINT varlen, LPBYTE varaddr)
{
	RETCODE	retcode =	::dbbind(dbproc,column,vartype,varlen,varaddr);
	
	if( retcode == FAIL )
	{
		writeInfoToFile( "DBBind.txt", "FAIL [ %s ]", CMD );
	}

	return retcode;
}


//--------------------------------------------------------------------------------------------
// Name: dbnextrow()
// Desc: ���� Row�� �޾ƿ´�.
//--------------------------------------------------------------------------------------------
STATUS HQSQLDB::DBNextRow()
{
	STATUS	status = ::dbnextrow(dbproc);	

	if( status == FAIL )
	{
		writeInfoToFile( "DBNextRow.txt", "FAIL [ %s ]", CMD );
	}

	return status;
}



//--------------------------------------------------------------------------------------------
// Name: ClearCMDBuf()
// Desc: Clear Command Buffer
//--------------------------------------------------------------------------------------------
void HQSQLDB::ClearCMDBuf()
{
	dbfreebuf(dbproc);
}



//--------------------------------------------------------------------------------------------
// Name: ClearRows()
// Desc: Clear Rows
//--------------------------------------------------------------------------------------------
void HQSQLDB::ClearRows()
{
	// Now process each row. 
	while (dbnextrow(dbproc) != NO_MORE_ROWS)
	{
		;
	}
}



//--------------------------------------------------------------------------------------------
// Name: DBClose()  
// Desc: DB�� �ݴ´�.
//--------------------------------------------------------------------------------------------
void HQSQLDB::DBClose()  
{		
	dbfreelogin(login);
	dbclose(dbproc);
}
