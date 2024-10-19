
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: TNConfig.h

Author: 檜團熱 <nshangukin@hotmail.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "stdafx.h"
#include "tndbconfig.h"
#include <stdio.h>

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CTNDBConfig::CTNDBConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CTNDBConfig::~CTNDBConfig(void)
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void CTNDBConfig::SetConfigFileName(LPCTSTR szConfigFileName)
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

LPCTSTR CTNDBConfig::GetStr(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetString( szConfigName, szKey );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CTNDBConfig::GetNum(LPCTSTR szConfigName, LPCTSTR szKey )
{
	return GetInt(
			szConfigName,
			szKey);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CTNDBConfig::GetPatchCount()
{
	return GetInt(
		PATCH_COUNT_CONFIG,
		CONFIG_PATCH_COUNT );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetPatchDir( BYTE byDir )
{
	char szConfig[256] = {0,};

	sprintf( szConfig, "%s:%d", PATCH_DIR_CONFIG, byDir );

	return GetString(
		szConfig,
		CONFIG_PATCH_DIR );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetSettingDir( BYTE byDir )
{
	char szConfig[256] = {0,};

	sprintf( szConfig, "%s:%d", PATCH_DIR_CONFIG, byDir );

	return GetString(
		szConfig,
		CONFIG_SETTING_DIR );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetDestinationPath( int nDir )
{
	char szSection[256] = {0,};

	sprintf( szSection, "%s%d", CONFIG_PATCH_DIR, nDir );

	return GetString(
		DESTINATION_PATH_CONFIG,
		szSection );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::SetDestinationPath(int nDir, LPCTSTR szValue)
{
	char szSection[256] = {0,};

	sprintf( szSection, "%s%d", CONFIG_PATCH_DIR, nDir );

	return WriteString( 
		DESTINATION_PATH_CONFIG, 
		szSection, 
		szValue );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CTNDBConfig::GetPathCount()
{
	return GetInt(
		PATH_COUNT_COUNT,
		CONFIG_PATCH_COUNT );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::SetPathCount(int nValue)
{
	return WriteInt( 
		PATH_COUNT_COUNT, 
		CONFIG_PATCH_COUNT, 
		nValue );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetSourcePatchDir()
{
	return GetString(
		PATCH_SOURCE_DIR_CONFIG,
		CONFIG_PATCH_DIR );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetAttachDir()
{
	return GetString(
		PATCH_ATTACH_VERSION_DIR_CONFIG,
		CONFIG_PATCH_DIR );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetMasterServerIP()
{
	return GetString(
		MASTER_SERVER_INFO_CONFIG,
		CONFIG_IP );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CTNDBConfig::GetMasterServerPort()
{
	return GetInt(
		MASTER_SERVER_INFO_CONFIG,
		CONFIG_PORT );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::SetSourcePath(LPCTSTR szValue)
{
	return WriteString( 
		PATCH_SOURCE_DIR_CONFIG, 
		CONFIG_PATCH_DIR, 
		szValue );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetServerName( BYTE byDir )
{
	char szConfig[256] = {0,};

	sprintf( szConfig, "%s:%d", PATCH_DIR_CONFIG, byDir );

	return GetString(
		szConfig,
		CONFIG_SERVER_NAME );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetGMSDBConnectionString()
{
	return GetString(
		PATCH_DB_CONNECTION_STRING_CONFIG ,
		CONFIG_DB_CONNECTION_STRING );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::SetAttachPath(LPCTSTR szValue)
{
	return WriteString( 
		PATCH_ATTACH_VERSION_DIR_CONFIG, 
		CONFIG_PATCH_DIR, 
		szValue );
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::Open(const char szConfigFileName[CONFIG_FILE_NAME_MAX_SIZE])
{
	strncpy( m_strConfigFileName, szConfigFileName, CONFIG_FILE_NAME_MAX_SIZE);

	return TRUE;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CTNDBConfig::GetInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault /*= 0*/)
{
	return GetPrivateProfileInt(
			szSection,
			szKey,
			nDefault,
			m_strConfigFileName);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CTNDBConfig::GetString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault /**/)
{
	LPCTSTR		strValue;
	TCHAR		szValue[256];

	GetPrivateProfileString(
		szSection,
		szKey,
		szDefault,
		szValue,
		256,
		m_strConfigFileName);

	strValue = szValue;

	return strValue;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::WriteInt(LPCTSTR szSection, LPCTSTR szKey, int nValue)
{
	LPCTSTR	strValue;
	char szBuf[256] = {0,};
	sprintf( szBuf, "%d", nValue );

	strValue = szBuf;	

	return WriteString(szSection, szKey, strValue);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CTNDBConfig::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	return WritePrivateProfileString(
		szSection, szKey, szValue, m_strConfigFileName);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/



