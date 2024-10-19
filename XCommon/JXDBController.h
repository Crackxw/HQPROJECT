//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXDBCONTROLLER_H
#define _JXDBCONTROLLER_H

#include <windows.h>

#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>

#ifndef JXDBCONTROLLER_CMDBUFSIZE
#define JXDBCONTROLLER_CMDBUFSIZE		2048
#endif

class JXDBController
{
public:
	JXDBController();
	~JXDBController();

	static	int RefCount;

public:
	void		Create( char *pszServerName, char *pszDatabaseName, char *pszUser, char *pszPassword );

	BOOL		Connect();
	void		DBClose();
	BOOL		SQLExec( char *str, ... );
	RETCODE		DBResults();
	void		DBFreeCmdBuf();
	RETCODE		DBBind( INT col, INT vartype, DBINT varlen, LPBYTE varaddr );
	STATUS		DBNextRow();
	void		ClearRows();

private:
	LOGINREC	*m_pLoginRec;
	DBPROCESS	*m_pDBProc;

private:
	char		m_pszServerName[ 64 ];
	char		m_pszDatabaseName[ 64 ];
	char		m_pszUser[ 64 ];
	char		m_pszPassword[ 64 ];

	char		m_pszCmd[ JXDBCONTROLLER_CMDBUFSIZE ];
};


#endif