
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOChinaAuthServerConfig.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "JOChinaAuthServerConfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOChinaAuthServerConfig::CJOChinaAuthServerConfig(void) : CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOChinaAuthServerConfig::~CJOChinaAuthServerConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CJOChinaAuthServerConfig::SetConfigFileName(LPCTSTR szConfigFileName)
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

void CJOChinaAuthServerConfig::SetConfigDirectoryName(LPCTSTR szConfigDirectoryName)
{
	LPCTSTR		strConfigFile;	

	strConfigFile = szConfigDirectoryName;

	Open(strConfigFile);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( 
			szConfigName, 
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR	CJOChinaAuthServerConfig::GetNationCode()
{	
	return GetString(
			NATION_CODE_CONFIG, 
			CONFIG_NATION_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// Authentication.
LPCTSTR CJOChinaAuthServerConfig::Authentication_IP()
{
	return GetString(
			AUTHENTICATION_CONFIG, 
			CONFIG_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::Authentication_Port()
{
	return GetInt(
			AUTHENTICATION_CONFIG, 
			CONFIG_PORT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authentication_ServiceCode()
{
	return GetString(
			AUTHENTICATION_CONFIG, 
			CONFIG_SERVICE_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authentication_Region()
{
	return GetString(
			AUTHENTICATION_CONFIG, 
			CONFIG_REGION_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authentication_LockFlag()
{
	return GetString(
			AUTHENTICATION_CONFIG, 
			CONFIG_LOCK_FLAG);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// Authorization.
LPCTSTR CJOChinaAuthServerConfig::Authorization_IP()
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::Authorization_Port()
{
	return GetInt(
			AUTHORIZATION_CONFIG, 
			CONFIG_PORT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authorization_ServiceCode()
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_SERVICE_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authorization_Region()
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_REGION_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authorization_AuthenticationFlag(void)
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_AUTHENTICATION_FLAG);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authorization_Type()
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_TYPE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Authorization_DuplicateCheck()
{
	return GetString(
			AUTHORIZATION_CONFIG, 
			CONFIG_DUPLICATE_CHECK);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
// Accounting.
LPCTSTR CJOChinaAuthServerConfig::Accounting_IP()
{
	return GetString(
			ACCOUTING_CONFIG, 
			CONFIG_IP);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::Accounting_Port()
{
	return GetInt(
			ACCOUTING_CONFIG, 
			CONFIG_PORT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Accounting_ServiceCode()
{
	return GetString(
			ACCOUTING_CONFIG, 
			CONFIG_SERVICE_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Accounting_Region()
{
	return GetString(
			ACCOUTING_CONFIG, 
			CONFIG_REGION_CODE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOChinaAuthServerConfig::Accounting_Type()
{
	return GetString(
			ACCOUTING_CONFIG, 
			CONFIG_TYPE);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::GetMaxConnectionSocketCount()
{
	return GetInt(
			CONNECTION_SOCKET_CONFIG, 
			CONFIG_MAX_CONNECTION_COUNT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOChinaAuthServerConfig::GetSocketConnectTimeoutSecond()
{
	return GetInt(
			CONNECTION_SOCKET_CONFIG, 
			CONFIG_CONNECTION_TIMEOUT);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/