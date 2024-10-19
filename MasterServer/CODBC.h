#ifndef __CODBC_H__
#define __CODBC_H__


#include <stdio.h>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

#define QUERY_SELECT 1
#define QUERY_OTHER 2

class SQLHSTMTLIST 
{
public:
	SQLHSTMT hstmt ;
	SQLHSTMTLIST *next;
	BOOL Allocate(SQLHDBC dbc);
	void FreeData ();

	short queryType ;
	char **dataArr;
	int dataSize;

	SQLHSTMTLIST( int type = 0 )
	{ 
		hstmt = NULL; next = NULL ; queryType = type;
		dataArr = NULL , dataSize = 0;
	}
	
	~SQLHSTMTLIST(){}
};

class CODBC
{
protected:
	SQLRETURN sqlRet ;
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMTLIST * m_pHstmt;
	BOOL IsPooling ;
	BOOL IsAutoCommit ;
	BOOL IsCommit ;
protected:
	inline BOOL IsError();
	BOOL CheckError(SQLSMALLINT hType, SQLHANDLE handle,char* hint=NULL);
	void FreeStmt();
	int AddStmt( SQLHSTMTLIST * pHstmt );
	long GetColumnSize( SQLHSTMT hstmt, int nCol );
	SQLHSTMTLIST* GetStmtList( int index );

public:
	CODBC();
	~CODBC();

	void SetPooling( BOOL data ){ IsPooling = data ; }
	void SetAutoCommit( BOOL data ){ IsAutoCommit = data; }

	BOOL Connect(char* ServerName, char* dbid, char* dbpw);
	void DisConnect();

	int Query ( char* sql );
	int Fetch ( int cur );

	char* GetColumn( int index, int nCol );

	void Commit();
	void RollBack();
};

#endif