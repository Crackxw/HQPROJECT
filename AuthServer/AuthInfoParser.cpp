#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>


// CAuthInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "AuthInfoParser.h"


// CAuthorizer
#include "Authorizer.h"
#include "Ras.h"
#include "Gash.h"


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CAuthInfoParser::CAuthInfoParser(char* filename) : CJOGersangConfig()
{	
	m_authserver_Port = 0;	
	m_gameserver_Num  = 0;
	m_workqueue_Size  = 0;
	

	memset(&m_accountDB_IP,      0, sizeof(m_accountDB_IP));		
	memset(&m_accountDB_Name,    0, sizeof(m_accountDB_Name));		
	memset(&m_accountDB_ID,      0, sizeof(m_accountDB_ID));		
	memset(&m_accountDB_PW,      0, sizeof(m_accountDB_PW));	
	
	memset(&m_authorizer_Name,   0, sizeof(m_authorizer_Name));
	memset(&m_authorizer_IP,	 0, sizeof(m_authorizer_IP));

	m_authorizer_Port   = 0;
	m_authorizer_clsCtx = 0;
	
	bool bRetVal = false;
#ifdef __CONFIG_FILE_LOAD__
	bRetVal = LoadAuthInfo();

	if(bRetVal == false)
	{
		printf("Authinfo file(Config.ini) is invalid! It causes many problem.\n");
		getch();
		exit(-1);
	}
#else
	bRetVal = LoadAuthInfo(filename);

	if(bRetVal == false)
	{
		printf("Authinfo file(AuthInfo.txt) is invalid! It causes many problem.\n");
		getch();
		exit(-1);
	}
#endif
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CAuthInfoParser::~CAuthInfoParser()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CAuthorizer* CAuthInfoParser::createAuthorizer()
{
	if( strcmp("RAS", getAuthorizer_Name()) == 0 )
	{
		return new CRas();
	}

	else if( strcmp("GASH", getAuthorizer_Name()) == 0 )
	{
		return new CGash();
	}

	else if( strcmp("NONE", getAuthorizer_Name()) == 0 )
	{
		return new CNoneAuthorizer();
	}

	return NULL;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 이민수
//  Date : 2004.01.16 PM
//
//	Desc : 서버 정보를 Config에서 읽는다.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
bool CAuthInfoParser::LoadAuthInfo()
{	
	char	szSep[128]={0,};

	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);

	////////////////////////////////////////////////////////////
	// Server information load.	
	////////////////////////////////////////////////////////////
	m_authserver_Port = 
		GetNum(AUTH_SERVER_CONFIG, CONFIG_PORT);
	m_gameserver_Num = 
		GetNum(AUTH_SERVER_CONFIG, AUTH_CONFIG_SERVER_NUMBER);
	m_workqueue_Size = 
		GetNum(AUTH_SERVER_CONFIG, AUTH_CONFIG_WORK_QUEUE_SIZE);

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	printf("\n authserver_Port:  [%d]\n	",  m_authserver_Port );
	printf("\n gameserver_Num:   [%d]\n	",  m_gameserver_Num  );
	printf("\n workqueue_Size:   [%d]\n\n ", m_workqueue_Size  );
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	////////////////////////////////////////////////////////////
	// DB information load.
	////////////////////////////////////////////////////////////
	strcpy(m_accountDB_IP,		GetStr(AUTH_SERVER_DB_INFO_CONFIG, CONFIG_IP));
	strcpy(m_accountDB_Name,	GetStr(AUTH_SERVER_DB_INFO_CONFIG, CONFIG_DB_CATALOG));
	strcpy(m_accountDB_ID,		GetStr(AUTH_SERVER_DB_INFO_CONFIG, CONFIG_DB_USER));
	strcpy(m_accountDB_PW,		GetStr(AUTH_SERVER_DB_INFO_CONFIG, CONFIG_DB_PASSWORD));

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	printf("\n accountDB_IP:    [%s]\n",   m_accountDB_IP );
	printf("\n accountDB_NAME:  [%s]\n",   m_accountDB_Name );
	printf("\n accountDB_ID:    [%s]\n",   m_accountDB_ID );
	printf("\n accountDB_PW:    [%s]\n\n",   m_accountDB_PW );
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	////////////////////////////////////////////////////////////
	// Authorizer information load.
	////////////////////////////////////////////////////////////
	strcpy(m_authorizer_Name, GetStr(AUTH_SERVER_AUTHORIZER_INFO_CONFIG, CONFIG_NAME));
	strcpy(szSep, GetStr(AUTH_SERVER_AUTHORIZER_INFO_CONFIG, AUTH_CONFIG_CLASS_CONTEXT));

	if( strcmp(szSep, "LOCAL_SERVER") == 0 )
	{
		m_authorizer_clsCtx = CLSCTX_LOCAL_SERVER;
	}
	else if( strcmp(szSep, "REMOTE_SERVER") == 0 )
	{
		m_authorizer_clsCtx = CLSCTX_REMOTE_SERVER;
	}					
	else if( strcmp(szSep, "0") == 0 )
	{
		// it means not to use
		m_authorizer_clsCtx = 0;
	}
	else
		return false;

	strcpy(m_authorizer_IP, GetStr(AUTH_SERVER_AUTHORIZER_INFO_CONFIG, CONFIG_IP));
	m_authorizer_Port = GetInt(AUTH_SERVER_AUTHORIZER_INFO_CONFIG, CONFIG_PORT);
	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	printf("\n authorizer_Name:   [%s]\n	", m_authorizer_Name );
	printf("\n authorizer_clsCtx: [%d]\n	", m_authorizer_clsCtx );
	printf("\n authorizer_IP:     [%s]\n	", m_authorizer_IP	 );
	printf("\n authorizer_Port:   [%d]\n\n\n", m_authorizer_Port );
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	return true;
}


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
bool CAuthInfoParser::LoadAuthInfo(char* filename)
{
	FILE* fp	 = NULL;
	char* pToken = NULL;


	char	buffer[1024];
	char	cSep[] = ";";

	
	if((fp = fopen(filename, "r")) != NULL)
	{
		while(fgets(buffer, 1024, fp) != NULL)
		{
			pToken = strtok(buffer, "\t");
						
			if(strcmp(pToken, "ServerInfo") == 0)
			{
				// authserver_Port
				//----------------------------------------------------------
				pToken		= strtok(NULL, cSep);
				if(pToken) m_authserver_Port = atoi(pToken);
				else return FALSE;

				// gameserver_Num
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_gameserver_Num = atoi(pToken);
				else return FALSE;

				// workqueue_size
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_workqueue_Size = atoi(pToken);
				else return FALSE;



				printf("\n authserver_Port:  [%d]\n	",  m_authserver_Port );
				printf("\n gameserver_Num:   [%d]\n	",  m_gameserver_Num  );
				printf("\n workqueue_Size:   [%d]\n\n ", m_workqueue_Size  );
			}

			if(strcmp(pToken, "Authorizer") == 0)
			{
				// authorizer_Name
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_authorizer_Name, pToken);
				else return FALSE;


				// authorizer_clsCtx
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) {
				
					if( strcmp(pToken, "LOCAL_SERVER") == 0 )
					{
						m_authorizer_clsCtx = CLSCTX_LOCAL_SERVER;
					}
					else if( strcmp(pToken, "REMOTE_SERVER") == 0 )
					{
						m_authorizer_clsCtx = CLSCTX_REMOTE_SERVER;
					}					
					else if( strcmp(pToken, "0") == 0 )
					{
						// it means not to use
						m_authorizer_clsCtx = 0;
					}
					else return FALSE;
				
				}
				else return FALSE;


				// authorizer_IP
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_authorizer_IP, pToken);
				else return FALSE;


				// authorizer_Port
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_authorizer_Port = atoi(pToken);
				else return FALSE;


				printf("\n authorizer_Name:   [%s]\n	", m_authorizer_Name );
				printf("\n authorizer_clsCtx: [%d]\n	", m_authorizer_clsCtx );
				printf("\n authorizer_IP:     [%s]\n	", m_authorizer_IP	 );
				printf("\n authorizer_Port:   [%d]\n\n\n", m_authorizer_Port );				
				
			}

			else if(strcmp(pToken, "AccountDB") == 0)
			{							


				// accountDB_IP
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_accountDB_IP, pToken);
				else return FALSE;
				

				// accountDB_Name
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_accountDB_Name, pToken);
				else return FALSE;


				// accountDB_ID
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_accountDB_ID, pToken);
				else return FALSE;


				// accountDB_PW
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) strcpy(m_accountDB_PW, pToken);
				else return FALSE;
				



				printf("\n accountDB_IP:    [%s]\n",   m_accountDB_IP );
				printf("\n accountDB_NAME:  [%s]\n",   m_accountDB_Name );
				printf("\n accountDB_ID:    [%s]\n",   m_accountDB_ID );
				printf("\n accountDB_PW:    [%s]\n\n",   m_accountDB_PW );				
			}
		}	

		fclose(fp);
				
		return TRUE;
	}
	else {

		printf("%s file load failed!\n", filename);
		getch();
	}


	return FALSE;
}