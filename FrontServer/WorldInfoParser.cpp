#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>


// CWorldInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "WorldInfoParser.h"


// CWorldInfoMgr
#include "../kojimodo/utility/Singleton.h"
#include "WorldInfoMgr.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CWorldInfoParser::CWorldInfoParser(char* filename)
: CJOGersangConfig()
{
#ifdef __CONFIG_FILE_LOAD__
	LoadWorldInfo();
#else
	LoadWorldInfo(filename);
#endif
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CWorldInfoParser::~CWorldInfoParser()
{
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : 이민수
//  Date : 2004.01.16 PM
//
//	Desc : 서버 정보를 Config에서 읽는다.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
bool CWorldInfoParser::LoadWorldInfo()
{	
	FRONT::sWorldInfo2		worldInfo;

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	SetConfigFileName(GERSANG_SERVER_CONFIG_FILE);
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	int nCount = GetFrontServerCount();

	if(nCount < 1)	return false;

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	for(int n = 0 ; n < nCount ; n++)
	{
		worldInfo.world_id = GetFrontServerID(n);
		worldInfo.setName(GetFrontServerName(n));

		int c_year = 0, c_month = 0, c_day = 0;

		c_year = GetFrontYear(n);
		c_month = GetFrontMonth(n);
		c_day = GetFrontDay(n);

		worldInfo.setCreateDate(c_year, c_month, c_day);

		worldInfo.setGateIP((char*)GetFrontGateIP(n));
		worldInfo.gatePort = GetFrontGatePort(n);

		worldInfo.setChatIP((char*)GetFrontChatIP(n));
		worldInfo.chatPort = GetFrontChatPort(n);

		worldInfo.concurrent = 0;

		worldInfo.world_state = FRONT::NO_HEARTBEAT;
		CWorldInfoMgr::P()->addWorld(&worldInfo);	
	}
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	return true;
}


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 월드 정보를 로드 한다 
//--------------------------------------------------------------------------------------------
bool CWorldInfoParser::LoadWorldInfo(char* filename)
{
	FILE* fp	 = NULL;
	char* pToken = NULL;


	char	buffer[1024];
	char	cSep[] = ";";

	FRONT::sWorldInfo2		worldInfo;


	if((fp = fopen(filename, "r")) != NULL)
	{
		while(fgets(buffer, 1024, fp) != NULL)
		{
			pToken = strtok(buffer, "\t");
						
			
			if(strcmp(pToken, "WorldInfo") == 0)
			{							


				// WORLD_ID
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.world_id = atoi(pToken);


				// WORLD_NAME
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.setName(pToken);


				// WORLD_CREATE_DATE
				//----------------------------------------------------------
				int c_year = 0, c_month = 0, c_day = 0;

				pToken			= strtok(NULL, cSep);
				if(pToken) c_year		= atoi(pToken);

				pToken			= strtok(NULL, cSep);
				if(pToken) c_month		= atoi(pToken);

				pToken			= strtok(NULL, cSep);
				if(pToken) c_day		= atoi(pToken);

				worldInfo.setCreateDate(c_year, c_month, c_day);


				// WORLD_GATE_IP
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.setGateIP(pToken);
				

				// WORLD_GATE_PORT
				//----------------------------------------------------------							
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.gatePort = atoi(pToken);


				// WORLD_CHAT_IP
				//----------------------------------------------------------
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.setChatIP(pToken);


				// WORLD_CHAT_PORT
				//----------------------------------------------------------							
				pToken			= strtok(NULL, cSep);
				if(pToken) worldInfo.chatPort = atoi(pToken);
				

				// WORLD_CONCURRENT
				//----------------------------------------------------------							
				worldInfo.concurrent = 0;


				// WORLD_STATE
				//----------------------------------------------------------							
				worldInfo.world_state = FRONT::NO_HEARTBEAT;



				CWorldInfoMgr::P()->addWorld(&worldInfo);

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




