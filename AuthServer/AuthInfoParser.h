#ifndef __CAUTHINFO_PARSER__
#define __CAUTHINFO_PARSER__


#include "..\SCCommon\JOGersangConfig.h"

class CAuthorizer;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : 
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CAuthInfoParser: 
public CJOGersangConfig, 
public CSingleton<CAuthInfoParser>
{

private:	
	WORD		m_authserver_Port;	
	WORD		m_gameserver_Num;
	int			m_workqueue_Size;

	char		m_accountDB_IP    [16];
	char		m_accountDB_Name  [256];
	char		m_accountDB_ID    [128];
	char		m_accountDB_PW    [128];
	
	char		m_authorizer_Name [128];
	char		m_authorizer_IP	  [16];	
	WORD		m_authorizer_Port;
	DWORD		m_authorizer_clsCtx;
	


public:
	CAuthInfoParser( char* filename );
	~CAuthInfoParser();
	

private:
	bool						LoadAuthInfo(char* filename);		
	bool						LoadAuthInfo(void);
	
public:	
	WORD						getAuthServer_Port()	{ return m_authserver_Port;   };
	WORD						getGameServer_Num()		{ return m_gameserver_Num;    };
	int							getWorkQueue_Size()		{ return m_workqueue_Size;    };

	char*						getAccountDB_IP()		{ return m_accountDB_IP;      };	
	char*						getAccountDB_Name()		{ return m_accountDB_Name;    };
	char*						getAccountDB_ID()		{ return m_accountDB_ID;      };
	char*						getAccountDB_PW()		{ return m_accountDB_PW;      };

	char*						getAuthorizer_Name()	{ return m_authorizer_Name;   };
	char*						getAuthorizer_IP()		{ return m_authorizer_IP;	  };
	WORD						getAuthorizer_Port()	{ return m_authorizer_Port;	  };
	DWORD						getAuthorizer_clsCtx()	{ return m_authorizer_clsCtx; };
	
	CAuthorizer*				createAuthorizer();

	
};



#endif