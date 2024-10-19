
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOGersangConfig.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "JOGersangConfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOGersangConfig::CJOGersangConfig(void) : CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOGersangConfig::~CJOGersangConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOGersangConfig::SetConfigFileName(LPCTSTR szConfigFileName)
{
	LPCTSTR		strConfigFile;
	TCHAR		szCurrentDirectory[MAX_PATH];
	char		szBuf[256]={0,};

	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	sprintf( szBuf, "%s\\%s", szCurrentDirectory, szConfigFileName );

	strConfigFile = szBuf;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOGersangConfig::SetConfigDirectoryName(LPCTSTR szConfigDirectoryName)
{
	LPCTSTR		strConfigFile;	

	strConfigFile = szConfigDirectoryName;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( 
			szConfigName, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// 마스터 서버 관려 시작 부분.
//
LPCTSTR	CJOGersangConfig::GetMasterServerName(int nGroup)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", MASTER_SERVER_GROUP_CONFIG, nGroup+1);
	return GetString( 
			szSection, 
			MASTER_CONFIG_SERVER_NAME);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetMasterServerIP(int nGroup, int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d-%d", MASTER_SERVER_INFO_CONFIG, nGroup+1, nCur+1);
	return GetString(
			szSection, 
			CONFIG_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetMasterServerGroupName(int nGroup)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", MASTER_SERVER_GROUP_CONFIG, nGroup+1);

	return GetString(
			szSection, 
			CONFIG_NAME);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetMasterServerCount(int nGroup)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", MASTER_SERVER_GROUP_CONFIG, nGroup+1);

	return GetInt(
			szSection, 
			MASTER_CONFIG_SERVER_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR	CJOGersangConfig::GetMasterServerType(int nGroup)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", MASTER_SERVER_GROUP_CONFIG, nGroup+1);
	return GetString(
			szSection, 
			MASTER_CONFIG_TYPE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetMasterGroupCount()
{
	return GetInt(
			MASTER_SERVER_CONFIG, 
			MASTER_CONFIG_GROUP_COUNT);
}
// 마스터 서버 관련 끝.
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// 모니터 서버 관련 시작 부분.
//
LPCTSTR	CJOGersangConfig::GetMonitorOfGameServerPath(LPCTSTR szSection)
{
	char szSection1[128]={0,};
	_stprintf(szSection1, "%s:%s", MONITOR_SERVER_CONFIG, szSection);

	return GetString(
			szSection1, 
			MONITOR_CONFIG_GAME_SERVER_PATH);
}
// 모니터 서버 관련 끝.
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// 프런트 서버 관련 시작 부분.
//
int CJOGersangConfig::GetFrontYear(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			CONFIG_YEAR);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontMonth(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			CONFIG_MONTH);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontDay(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			CONFIG_DAY);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontServerID(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			CONFIG_SERVER_ID);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetFrontServerName(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetString(
			szSection, 
			CONFIG_NAME);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetFrontGateIP(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetString(
			szSection, 
			FRONT_CONFIG_GATE_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetFrontChatIP(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetString(
			szSection, 
			FRONT_CONFIG_CHAT_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontGatePort(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			FRONT_CONFIG_GATE_PORT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontChatPort(int nCur)
{
	char szSection[128]={0,};

	_stprintf(szSection, "%s:%d", FRONT_WORLD_INFO_CONFIG, nCur+1);
	return GetInt(
			szSection, 
			FRONT_CONFIG_CHAT_PORT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetFrontServerCount()
{	
	return GetInt(
			FRONT_SERVER_CONFIG, 
			CONFIG_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetFrontNotice(int nCur)
{
	char szKey[128]={0,};
	_stprintf(szKey, "%s:%d", FRONT_CONFIG_NOTICE, nCur+1);

	return GetString(
			FRONT_NOTICE_CONFIG, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int	CJOGersangConfig::GetNoticeCount(void)
{
	return GetInt(
			FRONT_NOTICE_CONFIG, 
			CONFIG_COUNT);
}


// 프런트 서버 관련 끝.
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// 버젼 서버 관련 시작 부분
int CJOGersangConfig::GetVersionCountOfVersionServer()
{	
	return GetInt(
			VERSION_SERVER_CONFIG, 
			VERSION_CONFIG_VERSION_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetIPCountOfVersionServer()
{	
	return GetInt(
			VERSION_SERVER_CONFIG, 
			VERSION_CONFIG_IP_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetAuthCountOfVersionServer()
{	
	return GetInt(
			VERSION_SERVER_CONFIG, 
			VERSION_CONFIG_AUTH_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOGersangConfig::GetVersionListOfVersionServer(int nCur)
{
	char szKey[128]={0,};

	_stprintf(szKey, "%s:%d", VERSION_CONFING_VERSION_NUMBER, nCur+1);
	return GetInt(
			VERSION_SERVER_VERSION_LIST_CONFIG, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetIPListOfVersionServer(int nCur)
{
	char szKey[128]={0,};

	_stprintf(szKey, "%s:%d", CONFIG_IP, nCur+1);

	return GetString(
			VERSION_SERVER_IP_LIST_CONFIG, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetIDListOfVersionServer(int nCur)
{
	char szKey[128]={0,};

	_stprintf(szKey, "%s:%d", CONFIG_ID, nCur+1);

	return GetString(
			VERSION_SERVER_AUTH_LIST_CONFIG, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOGersangConfig::GetPasswordListOfVersionServer(int nCur)
{
	char szKey[128]={0,};

	_stprintf(szKey, "%s:%d", CONFIG_PASSWORD, nCur+1);

	return GetString(
			VERSION_SERVER_AUTH_LIST_CONFIG, 
			szKey);
}
// 버젼 서버 관련 끝.
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/