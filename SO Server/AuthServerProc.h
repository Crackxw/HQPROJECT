#ifndef __CAUTHSERVERPROC__
#define __CAUTHSERVERPROC__

#include "../AuthServer/_AuthDefine.h"


class CWinsock;
class CMSSQL;




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : 
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CAuthServerProc: public CWinsock
{

private:
	CMSSQL*					m_GameDB;


public:
	CAuthServerProc(int wait_time = 15000);	
	~CAuthServerProc();

	
	

	//--------------------------------------------------------------------------------
	bool					RequestLogin	 ( AUTH::sUniqueID userUniqID, char* id, char* password, char* ip, 
											   char* responseKind, DWORD* dbAccount );
	bool					RequestLogout    ( char* id, char* password, char* ip, DWORD dbAccount );
	bool					RequestReset	 ( int world_id, char* responseKind );
	
	bool					ReportHeartbeat  ();

	bool					RequestLoginEnd  ( AUTH::sUniqueID userUniqID, DWORD dbAccount, char* responseKind );




private:
	

	//--------------------------------------------------------------------------------
	bool					Connect();

	bool					Send(AUTH::sUniqueID userUniqueID, char* data, int dataSize);
	bool					Send(char* data, int dataSize);


	int						getPacketSize(char* header);	
	int						getChecksum(char* header);	
	int						calcChecksum(char* data, int size);


public:

	//--------------------------------------------------------------------------------
	//int						DB_LoginEnd( DWORD dbAccount );

};



#endif


