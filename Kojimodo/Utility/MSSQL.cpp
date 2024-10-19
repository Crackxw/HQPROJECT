#include <windows.h>
#include <time.h>
#include <stdio.h>

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include "MSSQL.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CMSSQL::CMSSQL() 
{
	memset( m_DB_IP,	0, sizeof(m_DB_IP) );
	memset( m_DB_Name,	0, sizeof(m_DB_Name) );
	memset( m_DB_ID,	0, sizeof(m_DB_ID) );
	memset( m_DB_PW,	0, sizeof(m_DB_PW) );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CMSSQL::~CMSSQL() 
{		
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB와 연결을 끊는다 
//--------------------------------------------------------------------------------------------
void CMSSQL::Dissconnect()
{
	dbfreelogin(m_login);
	dbclose(m_dbproc);	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB와 연결 한다 
//--------------------------------------------------------------------------------------------
bool CMSSQL::Connect(char* dbIP, char* dbName, char* id, char* password) 
{		
	m_login = dblogin();

	DBSETLUSER	 (m_login, id);
	DBSETLPWD	 (m_login, password);
	DBSETLVERSION(m_login, DBVER60);
	
	if( (m_dbproc=dbopen(m_login, dbIP) ) == NULL) return FALSE;	
	if( dbuse(m_dbproc, dbName) == FAIL ) return FALSE;
	

	strcpy( m_DB_IP,		dbIP	 );
	strcpy( m_DB_Name,		dbName	 );
	strcpy( m_DB_ID,		id		 );
	strcpy( m_DB_PW,		password );

	return TRUE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : DB와 재연결(이전 성공된 연결의 정보) 한다 
//--------------------------------------------------------------------------------------------
bool CMSSQL::Reconnect()
{
	Dissconnect();

	return Connect( m_DB_IP, m_DB_Name, m_DB_ID, m_DB_PW );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 한 SQL 쿼리문의 실행을 DB로 요청 한다 
//--------------------------------------------------------------------------------------------
bool  CMSSQL::SQLRun(char* SQL,...) 
{
	va_list ap;
	memset(CMD, 0, sizeof(CMD));
	va_start(ap, SQL);
	_vsnprintf(CMD, 1024, SQL, ap);
	va_end(ap);     
		
	wsprintf(CMD,"%s",CMD);

	dbcmd(m_dbproc, CMD);
	if(dbsqlexec(m_dbproc) == FAIL) 
	{	
		clearCMDBuf();
		clearRows();
		
			
		//---------------------------------------------
		FILE* fp;		
		if( (fp = fopen("CMSSQL_DBError.txt","at")) != NULL )		
		{
			char time[128];
			char date[128];

			_strtime( time );
			_strdate( date );

			fprintf(fp,"[%s %s] %s\n", date, time, CMD);

			fclose(fp);
		}
		//---------------------------------------------


		
		Reconnect();

		return FALSE;
	}

	return TRUE;
}




//-------------------------------------------------------------------------------------------------------------------------
// Name: 
// Desc: DB 결과를 얻어온다 
//-------------------------------------------------------------------------------------------------------------------------
RETCODE CMSSQL::DBResults()
{
	return ::dbresults(m_dbproc);
}




//-------------------------------------------------------------------------------------------------------------------------
// Name: 
// Desc: DB 결과를 알맞은 변수 타입으로 바인드 한다 
//-------------------------------------------------------------------------------------------------------------------------
RETCODE CMSSQL::DBBind(INT column, INT vartype, DBINT varlen, LPBYTE varaddr)
{
	return ::dbbind(m_dbproc, column, vartype, varlen, varaddr);

}




//-------------------------------------------------------------------------------------------------------------------------
// Name: 
// Desc: 연속되는 DB 결과일 경우 다음 행의 결과를 얻어 온다 
//-------------------------------------------------------------------------------------------------------------------------
STATUS CMSSQL::DBNextRow()
{
	return ::dbnextrow(m_dbproc);
}




//-------------------------------------------------------------------------------------------------------------------------
// Name: 
// Desc: DB로 결과 요청으로 생긴 버퍼를 초기화 한다 
//-------------------------------------------------------------------------------------------------------------------------
void CMSSQL::clearCMDBuf()
{
	dbfreebuf(m_dbproc);
}




//-------------------------------------------------------------------------------------------------------------------------
// Name: 
// Desc: 다른 DB 쿼리를 전송하기전에 남아있는 결과를 초기화 한다 
//-------------------------------------------------------------------------------------------------------------------------
void CMSSQL::clearRows()
{	
	while (dbnextrow(m_dbproc) != NO_MORE_ROWS)
	{
		;
	}
}



