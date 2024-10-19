//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#include "JXDBController.h"

#include <stdio.h>

int JXDBController::RefCount = 0;

JXDBController::JXDBController()
{
	m_pLoginRec = NULL;
	m_pDBProc = NULL;

	m_pszCmd[ 0 ] = NULL;

	m_pszServerName[ 0 ] = NULL;
	m_pszDatabaseName[ 0 ] = NULL;
	m_pszUser[ 0 ] = NULL;
	m_pszPassword[ 0 ] = NULL;

	if( RefCount == 0 ) dbinit();

	++RefCount;
}

JXDBController::~JXDBController()
{
	DBClose();

	if( m_pLoginRec ) dbfreelogin( m_pLoginRec );

	--RefCount;
}

void JXDBController::Create( char *pszServerName, char *pszDatabaseName, char *pszUser, char *pszPassword )
{
	
	DBClose();

	m_pszCmd[ 0 ] = NULL;

	strcpy( m_pszServerName, pszServerName );
	strcpy( m_pszDatabaseName, pszDatabaseName );
	strcpy( m_pszUser, m_pszUser );
	strcpy( m_pszPassword, m_pszPassword );

	if( m_pLoginRec ) { 
		dbfreelogin( m_pLoginRec ); 
		m_pLoginRec = NULL; 
	}

	m_pLoginRec = dblogin();

	DBSETLUSER( m_pLoginRec, m_pszUser );
	DBSETLPWD( m_pLoginRec, m_pszPassword );
	DBSETLVERSION( m_pLoginRec, DBVER60 );
}

BOOL JXDBController::Connect()
{
	DBClose();

	m_pDBProc = dbopen( m_pLoginRec, m_pszServerName );

	if( m_pDBProc == NULL ) {
		printf( "(JXDBController)DB접속 실패\n");
		return FALSE;
	}
	if( dbuse( m_pDBProc, m_pszDatabaseName ) == FAIL ) {
		printf( "(JXDBController)DB접속 실패\n");
		DBClose();
		return FALSE;
	}

	return TRUE;
}

void JXDBController::DBClose()
{
	if( m_pDBProc ) { dbclose( m_pDBProc ); m_pDBProc = NULL; }
}

BOOL JXDBController::SQLExec( char *str, ... )
{

	if( m_pDBProc == NULL ) {
		if( Connect() == FALSE ) return FALSE;
	}

	va_list ap;
	memset( m_pszCmd, 0, sizeof( m_pszCmd ) );
	va_start( ap, str );
	_vsnprintf( m_pszCmd, JXDBCONTROLLER_CMDBUFSIZE, str, ap );
	va_end( ap );     

	if( dbcmd( m_pDBProc, m_pszCmd ) == FAIL ) return FALSE;
		
	if( dbsqlexec( m_pDBProc ) == FAIL ) { 
		DBClose();
		return FALSE;
	}
	
	return TRUE;	
}

RETCODE JXDBController::DBResults()
{
	RETCODE ret;

	if( m_pDBProc == NULL ) {
		if( Connect() == FALSE ) return FAIL;
	}

	ret = dbresults( m_pDBProc );

	if( ret == FAIL ) DBClose();

	return ret;
}

void JXDBController::DBFreeCmdBuf()
{
	if( m_pDBProc == NULL ) return;

	dbfreebuf( m_pDBProc );
}

RETCODE JXDBController::DBBind( INT col, INT vartype, DBINT varlen, LPBYTE varaddr )
{
	if( m_pDBProc == NULL ) return FAIL;

	return dbbind( m_pDBProc, col, vartype, varlen, varaddr );
}

STATUS JXDBController::DBNextRow()
{
	if( m_pDBProc == NULL ) return FAIL;

	return dbnextrow( m_pDBProc );
}

void JXDBController::ClearRows()
{
	if( m_pDBProc == NULL ) return;

	while( dbnextrow( m_pDBProc ) != NO_MORE_ROWS );
}

