#include <windows.h>
#include <conio.h>
#include <stdio.h>


// CGameInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "GameInfoParser.h"


//
#include "_FrontDefine.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CGameInfoParser::CGameInfoParser( char* notice_filename, char* gameinfo_filename )
: CJOGersangConfig()
{
	using namespace FRONT;
	
	m_notice	= new char[NOTICE_MAX_LENGTH];
	memset(m_notice, 0, NOTICE_MAX_LENGTH);
	

#ifdef __CONFIG_FILE_LOAD__
	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);	
	LoadNotice();
	LoadGameInfo();
#else
	LoadNotice	 ( notice_filename );
	LoadGameInfo(gameinfo_filename);
#endif
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CGameInfoParser::~CGameInfoParser()
{
	if( m_notice != NULL ) {

		delete m_notice;
	}
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 이민수
//  Date : 2004.01.16 PM
//
//	Desc : Notice를 Config에서 읽는다.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
bool CGameInfoParser::LoadNotice()
{
	memset(m_notice, 0, FRONT::NOTICE_MAX_LENGTH);
	
	int nLength = 0;
	int nCount = GetNoticeCount();

	for(int n = 0 ; n < nCount ; n++)
	{
		nLength += sprintf(m_notice+nLength, "%s",  GetFrontNotice(n));
	}

	printf("\n NOTICE \n\n%s\n\n",m_notice);

	return true;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 이민수
//  Date : 2004.01.16 PM
//
//	Desc : 서버 정보를 Config에서 읽는다.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
bool CGameInfoParser::LoadGameInfo()
{	
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	m_userPort			= GetInt(FRONT_SERVER_CONFIG, CONFIG_USER_PORT);
	m_numUser			= GetInt(FRONT_SERVER_CONFIG, CONFIG_USER_COUNT);
	m_gameServerPort	= GetInt(FRONT_SERVER_CONFIG, CONFIG_PORT);
	m_numGameServer		= GetInt(FRONT_SERVER_CONFIG, CONFIG_SERVER_ID);
	m_gameVersion		= GetInt(FRONT_SERVER_CONFIG, CONFIG_VERSION);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	
	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 공지 사항을 로드 한다 
//--------------------------------------------------------------------------------------------
bool CGameInfoParser::LoadNotice(char* filename)
{
	FILE*	fp;	
	
	if((fp = fopen(filename, "r")) != NULL)
	{			
		memset(m_notice, 0, FRONT::NOTICE_MAX_LENGTH);

		// NOTICE
		//----------------------------------------------------------
		fread(m_notice, sizeof(char), FRONT::NOTICE_MAX_LENGTH, fp);
	
		fclose(fp);

		printf("\n NOTICE \n\n%s\n\n",m_notice);

		return TRUE;
	}

	else {

		printf("%s file load failed!\n", filename);
		getch();
	}

	return FALSE;
	
}

//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 게임 정보를 로드 한다 
//--------------------------------------------------------------------------------------------
bool CGameInfoParser::LoadGameInfo(char* filename)
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
				// User Port 
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_userPort	= atoi(pToken);

				// Number of user
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_numUser	= atoi(pToken);

				// GameServer Port 
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_gameServerPort	= atoi(pToken);

				// Number of gameserver
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_numGameServer	= atoi(pToken);



				printf("\n User Port            : %d  ",   m_userPort );
				printf("\n Number of User       : %d  ",   m_numUser );
				printf("\n Gameserver Port      : %d  ",   m_gameServerPort );
				printf("\n Number of Gameserver : %d\n\n", m_numGameServer );
			}


			if(strcmp(pToken, "BaseInfo") == 0)
			{							

				// GAME VERSION
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) m_gameVersion	= atoi(pToken);

								
				printf("\n GAME VERSION : %d  \n\n", m_gameVersion );
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




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 게임 버전을 얻어온다 
//--------------------------------------------------------------------------------------------
WORD CGameInfoParser::getGameVersion()
{
	return m_gameVersion;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 공지 사항을 얻어온다 
//--------------------------------------------------------------------------------------------
const char* CGameInfoParser::getNotice() const
{
	return m_notice;
}
