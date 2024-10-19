#ifndef	SO_MSSQLDB_HEADER
#define SO_MSSQLDB_HEADER

//--------------------------------------------------------------------------
// Name: SQLDB CLASS
// Desc: SQL 문을 제어하는 Class
//--------------------------------------------------------------------------
#include <time.h>

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#include <sys/timeb.h>

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
	bool		bConnected;
	
protected:
	LOGINREC	*login;
	DBPROCESS	*dbproc;
	char		CMD[1024];

};

#endif