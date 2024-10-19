
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOChinaAuthServerConfig.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "JOCrashClientConfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCrashClientConfig::CJOCrashClientConfig(void) : CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCrashClientConfig::~CJOCrashClientConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOCrashClientConfig::SetConfigFileName(LPCTSTR szConfigFileName)
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

void CJOCrashClientConfig::SetConfigDirectoryName(LPCTSTR szConfigDirectoryName)
{
	LPCTSTR		strConfigFile;	

	strConfigFile = szConfigDirectoryName;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOCrashClientConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( 
			szConfigName, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOCrashClientConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR	CJOCrashClientConfig::GetCrashServerIP()
{	
	return GetString(
			CRASH_CLIENT_INFO_CONFIG, 
			CONFIG_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOCrashClientConfig::SetCrashServerIP(LPCTSTR szIP)
{
	WriteString(
		CRASH_CLIENT_INFO_CONFIG, 
		CONFIG_IP, 
		szIP);
}