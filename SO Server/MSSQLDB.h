#ifndef	SO_MSSQLDB_HEADER
#define SO_MSSQLDB_HEADER

#include <time.h>
#include "SOCommonHeader.h"

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include <sys/timeb.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : SQL 문장 처리 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class HQSQLDB 
{
public:
	HQSQLDB();
	~HQSQLDB();
	
	bool		DBConnect(char* DATABASENAME, char* DEVICENAME, char* USERNAME, char* PWD);
	bool		SQLRun(char* SQL,...);
	RETCODE		DBResults();
	RETCODE		DBBind( INT column, INT vartype, DBINT varlen, LPBYTE varaddr );
	STATUS		DBNextRow();
	void		ClearCMDBuf();
	void		ClearRows();
	void		DBClose();	
	CHAR*		GetCMD();
	
protected:	
	LOGINREC	*login;
	DBPROCESS	*dbproc;
	char		CMD[1024];

};

#endif