//#include <iostream>
#include "CODBC.h"

#define PAGESIZE 2048L

CODBC::CODBC()
{
	henv = NULL;
	hdbc = NULL;
	m_pHstmt = NULL;
	IsPooling = FALSE;
	IsAutoCommit = TRUE;
	IsCommit = FALSE;
}

CODBC::~CODBC()
{
	FreeStmt();
	if( !IsAutoCommit && !IsCommit ) Commit(); 
	if( hdbc )
	{
		SQLDisconnect( hdbc );
		SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
	}
	
	if( henv )
	{
		SQLFreeHandle( SQL_HANDLE_ENV,henv );
	}
}

void CODBC::FreeStmt()
{
	if( m_pHstmt )
	{
		SQLHSTMTLIST *t1,*t2;
		t1 = t2 = m_pHstmt ;
		while( t1 )
		{
			t2 = t1->next ;
			if( t1->queryType == QUERY_SELECT )
			{
				SQLCloseCursor( t1->hstmt );
				t1->FreeData();
			}
			
			SQLFreeHandle( SQL_HANDLE_STMT , t1->hstmt );
			delete ( t1 );
			t1 = t2 ;
		}
		m_pHstmt = NULL;
	}
}

BOOL CODBC::CheckError(SQLSMALLINT hType, SQLHANDLE handle, char* hint )
{
	SQLCHAR       SqlState[6], Msg[1024];
	SQLINTEGER    NativeError =0;
	SQLSMALLINT   i = 1, MsgLen;
	SQLRETURN	 thisRet ;
	BOOL	ret = TRUE ;

	ZeroMemory ( SqlState, sizeof(SqlState ) );
	ZeroMemory ( Msg, sizeof(Msg ) );

	if( hint )
	{
		fprintf(stderr, "%s\n", hint );
	}

	while ((thisRet = SQLGetDiagRec(hType, handle, i, SqlState, &NativeError,Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)) 
	{
		if( thisRet == SQL_INVALID_HANDLE )
		{
			fprintf(stderr, "Invalid Handle\n");
			break;
		} 
		else if ( thisRet == SQL_ERROR ) 
		{
			fprintf(stderr, "RecNumber was negative or 0 \n");
			fprintf(stderr, "BufferLength was less than zero \n");
			break;
		} 

		if( hType == SQL_HANDLE_DBC && handle == hdbc && sqlRet == SQL_SUCCESS_WITH_INFO && !strcmp( (char*)SqlState , "01000" ) ) 
			ret = FALSE ;
		else 
		{
			ret = TRUE ;
			fprintf(stderr, "%s %s\n", SqlState, Msg);
		}

		i++;
	}

	return ret;
} 

BOOL SQLHSTMTLIST::Allocate(SQLHDBC dbc)
{
	BOOL ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc , &hstmt );
	SQLINTEGER val = 20;
	
	SQLSetStmtAttr ( hstmt, SQL_ATTR_QUERY_TIMEOUT, (PTR)val, SQL_IS_UINTEGER );
	
	return ret;
}

void SQLHSTMTLIST::FreeData ()
{
	for( int i = 0 ; dataArr && i < dataSize ; i++ )
	{
		if( dataArr[i] )
		{
			free( dataArr[i] );
			dataArr[i] = NULL;
		}
	}
	if( dataArr )
	{
		free( dataArr );
		dataArr = NULL;
	}

	dataSize = 0;
}

BOOL CODBC::Connect( char* ServerName, char* dbid, char* dbpw )
{
	SQLINTEGER val ;
	if( IsPooling )
	{
		sqlRet = SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (PTR)SQL_CP_ONE_PER_DRIVER , SQL_IS_UINTEGER );
		if( IsError() ) 
		{
			CheckError( SQL_HANDLE_ENV, henv );
			return FALSE;
		}
	}

	sqlRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if( IsError() )
	{
		CheckError( SQL_HANDLE_ENV, henv );
		return FALSE;
	}

	sqlRet = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (PTR)SQL_OV_ODBC3, SQL_IS_UINTEGER );
	if( IsError() )
	{
		CheckError( SQL_HANDLE_ENV, henv );
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
		henv = NULL;
		return FALSE;
	}

	sqlRet = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if( IsError() )
	{
		CheckError( SQL_HANDLE_DBC, hdbc );
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
		henv = NULL;
		return FALSE;
	}

	sqlRet = SQLConnect( hdbc, (SQLCHAR*)ServerName, SQL_NTS, (SQLCHAR*)dbid , SQL_NTS , (SQLCHAR*)dbpw, SQL_NTS ); 
	if( IsError() )
	{
		if( CheckError( SQL_HANDLE_DBC, hdbc ) ) 
		{
			SQLFreeHandle( SQL_HANDLE_ENV, henv );
			SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
			henv = NULL;
			hdbc = NULL;
			return FALSE;
		}
	}

	val = 30;
	sqlRet = SQLSetConnectAttr ( hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (PTR)val, SQL_IS_UINTEGER );
	if( IsError() )
	{
		CheckError( SQL_HANDLE_DBC, hdbc );
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
		SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
		henv = NULL;
		hdbc = NULL;
		return FALSE;
	}


	if( !IsAutoCommit )
	{
		sqlRet = SQLSetConnectAttr ( hdbc, SQL_ATTR_AUTOCOMMIT, (PTR)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER );
		if( IsError() )
		{
			CheckError( SQL_HANDLE_DBC, hdbc );
			SQLFreeHandle( SQL_HANDLE_ENV, henv );
			SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
			henv = NULL;
			hdbc = NULL;

			return FALSE;
		}
	}
	return TRUE;
}

void CODBC::DisConnect()
{
	FreeStmt();
	if(  !IsAutoCommit && !IsCommit ) Commit(); 
	SQLDisconnect( hdbc );
	SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
	SQLFreeHandle( SQL_HANDLE_ENV,henv );
	hdbc = NULL;
	henv = NULL;
}

int CODBC::Query( char* sql )
{
	if( !sql || sql[0] == 0 || strlen( sql ) == 0) 
	{
		fprintf(stderr, "Invalid Sql %s\n", sql);
		return -1;
	}
	char cmd[1024];
	int type ;
	char *te = sql;

	while ( *te )
	{
		while( *te == ' ' || *te == '\t' || *te == '\n' )
			te++;
		
		if( *te == '\0' ) break;
		
		int len = (int)strcspn( te, " \t\n" );
		if( len > sizeof(cmd ) ) 
		{
			fprintf(stderr, "Invliad Sql %s\n", sql);
			return -1;
		}

		strncpy( cmd , te , len );
		cmd[len] = 0;
		te = te + len + 1 ;
	
		if( !_stricmp( cmd , "SELECT" ) )
		{
			type = QUERY_SELECT;
			break;
		} 
		else 
			type = QUERY_OTHER;		
	}
	
	SQLHSTMTLIST * pHstmt = new SQLHSTMTLIST( type ) ;
	sqlRet = pHstmt->Allocate( hdbc );
	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, pHstmt->hstmt );
	}

	int index = AddStmt( pHstmt );
	sqlRet = SQLPrepare( pHstmt->hstmt , (SQLCHAR*) sql , SQL_NTS );
	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, pHstmt->hstmt );
		return -1;
	}

	sqlRet = SQLExecute( pHstmt->hstmt ); 
	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, pHstmt->hstmt );
		return -1;
	}

	return index ;
}

inline BOOL CODBC::IsError()
{
	if( sqlRet != SQL_SUCCESS)
	{
		return TRUE;
	} 
	return FALSE;
}

/* return value:
Error : -1
Success : 1
NoData : 0 
Note:
*/
int CODBC::Fetch( int index )
{
	if( index < 0 ) 
	{
		fprintf(stderr, "Invalid Cursor %d\n", index);
		return -1 ;
	}

	SQLHSTMTLIST *p_hstmt = GetStmtList( index );

	if( !p_hstmt || p_hstmt->queryType != QUERY_SELECT )
	{
		fprintf(stderr, "Unknown Cursor or Not a SELECT Query \n");
		return -1 ;
	}

	sqlRet = SQLFetch( p_hstmt->hstmt );
	if( sqlRet == SQL_NO_DATA )
	{
		return 0;
	} 

	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, p_hstmt->hstmt );
		return -1;
	} 

	// free previous 
	p_hstmt->FreeData();

	SQLSMALLINT colSize = 0;
	long colDataSize = 0L, at= 0L;
	SQLINTEGER  realDataSize ;
	sqlRet = SQLNumResultCols( p_hstmt->hstmt , &colSize );

	if( colSize < 1 )
	{
		fprintf(stderr, "Error Column Size : %d\n", colSize);
		return -1;
	}

	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, p_hstmt->hstmt, "SQLNumResultCols Error" );
		return -1;
	}

	p_hstmt->dataSize = colSize ;
	p_hstmt->dataArr = (char**) calloc( colSize, sizeof(char*) );
	for( int i = 0 ; i < colSize ; i++ )
	{
		colDataSize = GetColumnSize( p_hstmt->hstmt, i );
		if( colDataSize <= 0 ) 
		{
			fprintf(stderr, "Error Column Data Size : %d\n", colDataSize);
			p_hstmt->FreeData();
			return -1;
		}
		
		p_hstmt->dataArr[i] = (char*) calloc( colDataSize+1 , sizeof(char) );
reget:
		sqlRet = SQLGetData( p_hstmt->hstmt, i+1 , SQL_C_CHAR, &p_hstmt->dataArr[i][at] , colDataSize+1 , &realDataSize );
		if( realDataSize == SQL_NULL_DATA )
			ZeroMemory( p_hstmt->dataArr[i], colDataSize+1 );
		
		if( colDataSize < realDataSize )
		{
			if( p_hstmt->dataArr[i] ) 
			{
				char* temp = p_hstmt->dataArr[i]; 
				at = colDataSize ;
				colDataSize = realDataSize;

				p_hstmt->dataArr[i] = (char*) calloc( colDataSize +1 , sizeof(char) );
				memcpy( p_hstmt->dataArr[i] , temp, at );
				free( temp );

				goto reget;
			}
		}

		if( IsError() )
		{
			char* hint ;
			if( p_hstmt->dataArr[i] ) hint = "SQLGetData Error";
			else hint = "Alloction For Data Error";
			
			CheckError( SQL_HANDLE_STMT, p_hstmt->hstmt, hint );
			p_hstmt->FreeData();
			
			return -1;
		}
	}

	return 1;
}

/*
number start 0
if return NULL : error
*/

char* CODBC::GetColumn( int cur, int nCol )
{
	SQLHSTMTLIST *p_hstmt = GetStmtList( cur );
	if( !p_hstmt || p_hstmt->queryType != QUERY_SELECT )
	{
		fprintf( stderr, "Unknown Cursor or Not a SELECT Query \n" ); 
		return NULL;
	}

	if( p_hstmt->dataSize == 0 || !p_hstmt->dataArr ) 
	{
		fprintf( stderr, "No Fetch Data\n" ); 
		return NULL;
	}

	if( nCol < 0 || nCol >= p_hstmt->dataSize )
	{
		fprintf( stderr, "Invalid Column Index\n" ); 
		return NULL;
	}

	return p_hstmt->dataArr[nCol];
}

long CODBC::GetColumnSize( SQLHSTMT hstmt, int nCol )
{
	UCHAR svColName[256];
	SQLSMALLINT swCol,swType,swScale,swNull;
	SQLUINTEGER colSize = 0;

	sqlRet = SQLDescribeCol( hstmt,nCol+1,svColName,sizeof(svColName) ,&swCol,&swType,&colSize,&swScale,&swNull );

	if( IsError() )
	{
		CheckError( SQL_HANDLE_STMT, hstmt );
		return -1;
	}

	if( colSize > PAGESIZE ) return PAGESIZE;
	return colSize;
}
/*
return index , start at 0
*/
int CODBC::AddStmt( SQLHSTMTLIST * pHstmt )
{
	if( m_pHstmt == NULL )
	{
		m_pHstmt = pHstmt ;
		return 0;
	} 
		
	int index = 1;
	SQLHSTMTLIST * t = m_pHstmt ;
	while( t->next )
	{
		index++;
		t = t->next;
	}

	t->next = pHstmt ;

	return index;	
}

SQLHSTMTLIST* CODBC::GetStmtList( int index )
{
	SQLHSTMTLIST * t = m_pHstmt ;
	int i = 0;
	while( t )
	{
		if( i == index )
		{
			return t;
		}
		i++;
		t = t->next ;
	}

	return NULL;
}

void CODBC::Commit()
{
	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	IsCommit = TRUE;
}
//commit or rollback
//SQLEndTran
void CODBC::RollBack()
{
	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
	IsCommit = TRUE;
}

