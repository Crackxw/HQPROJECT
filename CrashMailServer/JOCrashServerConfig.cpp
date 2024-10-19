
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOChinaAuthServerConfig.cpp

Author: �̹μ� <nshangukin@joyon.com>
Currently Managed by: �̹μ�

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "JOCrashServerConfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCrashServerConfig::CJOCrashServerConfig(void) : CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOCrashServerConfig::~CJOCrashServerConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOCrashServerConfig::SetConfigFileName(LPCTSTR szConfigFileName)
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

void CJOCrashServerConfig::SetConfigDirectoryName(LPCTSTR szConfigDirectoryName)
{
	LPCTSTR		strConfigFile;	

	strConfigFile = szConfigDirectoryName;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOCrashServerConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( 
			szConfigName, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOCrashServerConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR	CJOCrashServerConfig::CrashDBConnectionString()
{	
	return GetString(
			CRASH_SERVER_INFO_CONFIG, 
			CONFIG_CRASH_DB_CONNECTION_STRING);
}