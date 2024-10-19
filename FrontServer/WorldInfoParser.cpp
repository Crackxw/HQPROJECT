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
//	Desc : �� �� �� 
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
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CWorldInfoParser::~CWorldInfoParser()
{
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=
//	Name : �̹μ�
//  Date : 2004.01.16 PM
//
//	Desc : ���� ������ Config���� �д´�.
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
//	Desc : ���� ������ �ε� �Ѵ� 
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




