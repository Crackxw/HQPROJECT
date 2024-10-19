#ifndef __CNSSQL__
#define __CMSSQL__



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : MSSQL Repper Class
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CMSSQL
{
private:
	char		m_DB_IP   [16];
	char		m_DB_Name [256];
	char		m_DB_ID   [128];
	char		m_DB_PW   [128];


public:
	CMSSQL();
	~CMSSQL();
	
	bool		SQLRun (char* SQL,...);

	
	RETCODE		DBResults();
	RETCODE		DBBind(INT column, INT vartype, DBINT varlen, LPBYTE varaddr);
	STATUS		DBNextRow();

	
	void		clearCMDBuf();
	void		clearRows();

	bool		Connect(char* dbIP, char* dbName, char* id, char* password);
	void		Dissconnect();


private:
	bool		Reconnect();


private:

	DBPROCESS	*m_dbproc;	
	LOGINREC	*m_login;
	char        CMD[1024];

};

#endif