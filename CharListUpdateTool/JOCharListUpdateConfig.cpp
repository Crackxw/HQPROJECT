
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOChinaAuthServerConfig.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "StdAfx.h"
#include "JOCharListUpdateConfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCharListUpdateConfig::CJOCharListUpdateConfig(void) : CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCharListUpdateConfig::~CJOCharListUpdateConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOCharListUpdateConfig::SetConfigFileName(LPCTSTR szConfigFileName)
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

void CJOCharListUpdateConfig::SetConfigDirectoryName(LPCTSTR szConfigDirectoryName)
{
	LPCTSTR		strConfigFile;	

	strConfigFile = szConfigDirectoryName;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOCharListUpdateConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( 
			szConfigName, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOCharListUpdateConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR	CJOCharListUpdateConfig::DBConnectionString(int nCur)
{
	char szSection[256] = {0, };

	_stprintf(szSection, "%s:%d", CHARLIST_INFO_CONFIG, nCur+1);

	return GetStr(
			szSection, 
			CONFIG_GAME_DB_CONNECTION_STRING);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


LPCTSTR	CJOCharListUpdateConfig::DBServerNameString(int nCur)
{
	char szSection[256] = {0, };

	_stprintf(szSection, "%s:%d", CHARLIST_INFO_CONFIG, nCur+1);

	return GetStr(
			szSection, 
			CONFIG_NAME);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOCharListUpdateConfig::DBConnectCountInt()
{
	return GetNum(
		CHARLIST_TOOL_INFO_CONFIG, 
		CONFIG_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/