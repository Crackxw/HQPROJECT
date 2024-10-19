
#include <winsock2.h>
#include <stdio.h>
#include "SOGameServerParser.h"
#include "stdlib.h"
#include "Defines.h"

void GetHostIP(char* serverIP)
{
	char				hostName[100];
	memset(hostName, 0, sizeof(hostName));

	struct hostent *	pHostEnt;						
	
	BYTE				h1, h2, h3, h4;				    

	
	if( gethostname(hostName,sizeof(hostName)) == 0 )
	{
		pHostEnt = gethostbyname(hostName);         
		
		h1=(*pHostEnt).h_addr_list[0][0];         
		h2=(*pHostEnt).h_addr_list[0][1];
		h3=(*pHostEnt).h_addr_list[0][2];
		h4=(*pHostEnt).h_addr_list[0][3];
		
		sprintf(serverIP,"%i.%i.%i.%i",h1,h2,h3,h4);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOGameServerParser::SOGameServerParser() : CJOGersangConfig()
{
	ZeroMemory(szDB1IP,		   sizeof(szDB1IP));
	ZeroMemory(szDB2IP,		   sizeof(szDB2IP));
	ZeroMemory(szDBName1,	   sizeof(szDBName1));
	ZeroMemory(szDBName2,	   sizeof(szDBName2));
	ZeroMemory(szDBUser,	   sizeof(szDBUser));
	ZeroMemory(szLanguageCode, sizeof(szLanguageCode));	
	
	m_bTestServer				= FALSE;
	m_bGameGuard				= FALSE;
	m_bEvent					= FALSE;
	m_worldID					= 0;
	m_numAllowedConcurrentUser	= ON_MAX_CONNECTION;
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOGameServerParser::~SOGameServerParser()
{
	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 檜團熱
//  Date : 2004.01.16 PM
//
//	Desc : 憮幗 薑爾蒂 Config縑憮 檗朝棻.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
BOOL	SOGameServerParser::Load()
{	
	CHAR	cSep[128]={0,};	
	CHAR	szDirectory[256] = {0,};	
	char	szHostName[100] = {0,};
	CHAR	szSection[128] = {0,};
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	ZeroMemory(szLanguageCode, sizeof(szLanguageCode));	
	szLanguageCode[0] = 'K';			// <- 晦獄 措陛 囀萄陛 Korea檜棻.
	m_bTestServer = FALSE;
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	_stprintf(szDirectory, "..\\%s", GERSANG_SERVER_CONFIG_FILE);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	SetConfigDirectoryName(szDirectory);	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=		

	gethostname(szHostName,sizeof(szHostName));	

	printf("SOGameServerPaserLoad()-Host name:%s\n", szHostName);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Link server ip load.
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	strcpy(szFrontServerIP, 
		GetStr(GAME_LINK_SERVER_INFO_CONFIG, GAME_CONFIG_FRONT_SERVER_IP));
	strcpy(szAuthServerIP,  
		GetStr(GAME_LINK_SERVER_INFO_CONFIG, GAME_CONFIG_AUTH_SERVER_IP));
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// DB information load.
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	sprintf(szSection, "%s:%s", GAME_DB_INFO_CONFIG, szHostName);

	strcpy(szDB1IP,		GetStr(szSection,			CONFIG_IP));
	strcpy(szDBName1,	GetStr(szSection,			CONFIG_DB_CATALOG));
	strcpy(szDBUser,	GetStr(szSection,			CONFIG_DB_USER));

	sprintf(szSection, "%s:%s", GAME_ITEM_DB_INFO_CONFIG, szHostName);
	strcpy(szDB2IP,		GetStr(szSection,	CONFIG_IP));	
	strcpy(szDBName2,	GetStr(szSection,	CONFIG_DB_CATALOG));	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// World information load.
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	sprintf(szSection, "%s:%s", GAME_WORLD_INFO_CONFIG, szHostName);
	strcpy(szLanguageCode,	GetStr(szSection, CONFIG_USE_LANGUAGE));
	m_worldID =				GetNum(szSection, GAME_CONFIG_WORLD_ID);
	strcpy(cSep,			GetStr(szSection, GAME_CONFIG_TEST_SERVER_FLAG));

	if( stricmp(cSep, "TRUE")	== 0 || 
		stricmp(cSep, "1")		== 0 || 
		stricmp(cSep, "YES")	== 0 || 
		stricmp(cSep, "OK")		== 0)
	{
		m_bTestServer		=	TRUE;
	}
	else
		m_bTestServer		=	FALSE;

	m_numAllowedConcurrentUser = 
		GetNum(szSection, GAME_CONFIG_MAX_CONCURRENT);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=	
			
	return TRUE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	だ橾擊 煎萄и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOGameServerParser::Load(CHAR *pszFileName)
{
	FILE	*fp = NULL;
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = " =\t\n";
	CHAR	*pToken;

	ZeroMemory(szLanguageCode, sizeof(szLanguageCode));	
	szLanguageCode[0] = 'K';			// <- 晦獄 措陛 囀萄陛 Korea檜棻.
	m_bTestServer = FALSE;


	if((fp = fopen(pszFileName, "r")) != NULL)
	{
		while(fgets(szReadBuffer, 1023, fp) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);

			if(stricmp(pToken, "FrontServerIP") == 0)
			{
				// FrontServer IP
				pToken =	strtok(NULL, cSep);
				strncpy(szFrontServerIP, pToken, 15);
			}

			else if(stricmp(pToken, "AuthServerIP") == 0)
			{
				// AuthServer  IP
				pToken =	strtok(NULL, cSep);
				strncpy(szAuthServerIP, pToken, 15);
			}

			else if(stricmp(pToken, "DBIP1") == 0)
			{
				// DB1 Server IP
				pToken	=	strtok(NULL, cSep);
				strncpy(szDB1IP, pToken, 15);
			}

			else if(stricmp(pToken, "DBIP2") == 0)
			{
				// DB2 Server IP
				pToken	=	strtok(NULL, cSep);
				strncpy(szDB2IP, pToken, 15);
			}

			else if(stricmp(pToken, "DBName1") == 0)
			{
				// DB1 Name
				pToken	=	strtok(NULL, cSep);
				strncpy(szDBName1, pToken, 255);
			}

			else if(stricmp(pToken, "DBName2") == 0)
			{
				// DB2 Name
				pToken	=	strtok(NULL, cSep);
				strncpy(szDBName2, pToken, 255);
			}

			else if(stricmp(pToken, "DBUser") == 0)
			{
				// DB User
				pToken	=	strtok(NULL, cSep);
				strncpy(szDBUser, pToken, 255);
			}

			else if(stricmp(pToken, "Language") == 0)
			{
				// DB User
				pToken				=	strtok(NULL, cSep);
				ZeroMemory(szLanguageCode, sizeof(szLanguageCode));
				strncpy(szLanguageCode, pToken, 1);
			}

			else if(stricmp(pToken, "WorldID") == 0)
			{
				// WorldID
				pToken				=	strtok(NULL, cSep);
				
				if(pToken) m_worldID = atoi(pToken);
			}

			else if(stricmp(pToken, "TestServer") == 0)
			{
				pToken				=	strtok(NULL, cSep);
				if( stricmp(pToken, "TRUE") == 0 || stricmp(pToken, "1") == 0 || stricmp(pToken, "YES") == 0 || stricmp(pToken, "OK") == 0 )
					m_bTestServer		=	TRUE;
				else
					m_bTestServer		=	FALSE;
			}

			else if(stricmp(pToken, "MaxConcurrent") == 0 )
			{
				// MaxUser
				pToken				=	strtok( NULL, cSep );

				if( pToken )	m_numAllowedConcurrentUser	=	atoi( pToken );
			}

			else if(stricmp(pToken, "GameGuard") == 0 )
			{
				// MaxUser
				pToken				=	strtok( NULL, cSep );

				if( stricmp(pToken, "TRUE") == 0 || stricmp(pToken, "1") == 0 || stricmp(pToken, "YES") == 0 || stricmp(pToken, "OK") == 0 )
					m_bGameGuard		=	TRUE;
				else
					m_bGameGuard		=	FALSE;
			}
			else if(stricmp(pToken,"Event") == 0)
			{
				pToken				=	strtok( NULL, cSep );

				if( stricmp(pToken, "TRUE") == 0 || stricmp(pToken, "1") == 0 || stricmp(pToken, "YES") == 0 || stricmp(pToken, "OK") == 0 )
					m_bEvent			=	TRUE;
				else
					m_bEvent			=	FALSE;
			}
		}	

		fclose(fp);
		fp	=	NULL;

		return TRUE;
	}

	return FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
WORD	SOGameServerParser::GetConcurrent()
{
	return m_concurrent;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void	SOGameServerParser::SetConcurrent(WORD concurrent)
{
	m_concurrent = concurrent;
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
WORD	SOGameServerParser::GetWorldID()
{
	return m_worldID;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetAuthServerIP()
{
	return szAuthServerIP;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetFrontServerIP()
{
	return szFrontServerIP;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB1 蝶お葭 IP蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetDB1IPString()
{
	return szDB1IP;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB2 蝶お葭 IP蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetDB2IPString()
{
	return szDB2IP;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB1 檜葷 蝶お葭擊 橢橫螞棻.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetDBName1String()
{
	return szDBName1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB2 檜葷 蝶お葭擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetDBName2String()
{
	return szDBName2;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	DB 餌辨濠 蝶お葭擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	SOGameServerParser::GetDBUserString()
{
	return szDBUser;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Language Code蒂 橢橫褥
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR	SOGameServerParser::GetLanguageCode()
{
	return szLanguageCode[0];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Test Server檣雖 掘碟 Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOGameServerParser::IsTestServer()
{
	return m_bTestServer;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	譆渠 ъ辨 蕾樓濠熱 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
WORD	SOGameServerParser::GetNumAllowedConcurrentUser()
{
	return m_numAllowedConcurrentUser;
}

BOOL	SOGameServerParser::IsGameGuard(VOID)
{
	return(m_bGameGuard);
}

BOOL	SOGameServerParser::IsGameEvent()
{
	return m_bEvent;
}