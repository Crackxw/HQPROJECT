
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: TNConfig.h

Author: 이민수 <nshangukin@hotmail.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __TNCONFIG_H__
#define __TNCONFIG_H__

#include <windows.h>
#include <tchar.h>
//#include "Defines.h"

//-------------------------------------------------------------------------------------
// 디폴트 파일 이름
//
#define DEFAULT_CONFIG_FILE					_T("Gersang Patch Setting.ini")
#define DESTINATION_PATH_CONFIG_FILE		_T("Destination File List.ini")
//-------------------------------------------------------------------------------------
// Key 종류
//
#define CONFIG_PATCH_COUNT					_T("count")
#define CONFIG_PATCH_DIR					_T("dir")
#define CONFIG_SETTING_DIR					_T("setting directory")
#define CONFIG_SERVER_NAME					_T("server name")
#define CONFIG_DB_CONNECTION_STRING			_T("DB Connection String")
#define CONFIG_IP							_T("ip")
#define CONFIG_PORT							_T("port")
//-------------------------------------------------------------------------------------
// Section 종류
//
#define PATCH_COUNT_CONFIG					_T("PATCH_COUNT")
#define PATCH_DIR_CONFIG					_T("PATCH_DIR")
#define PATCH_SOURCE_DIR_CONFIG				_T("PATCH_SOURCE_DIR")
#define PATCH_DB_CONNECTION_STRING_CONFIG	_T("PATCH_DB_CONNECTION_STRING")
#define PATCH_ATTACH_VERSION_DIR_CONFIG		_T("ATTACH_VERSION_PATH")
#define MASTER_SERVER_INFO_CONFIG			_T("MASTER_SERVER")
#define DESTINATION_PATH_CONFIG				_T("DESTINATION_PATH_LIST")
#define PATH_COUNT_COUNT					_T("PATH_COUNT")
//-------------------------------------------------------------------------------------

#define CONFIG_FILE_NAME_MAX_SIZE			128
//-------------------------------------------------------------------------------------

class CTNDBConfig
{
private : 
	char		m_strConfigFileName[CONFIG_FILE_NAME_MAX_SIZE];

private :
	BOOL		Open		(const char szConfigFileName[CONFIG_FILE_NAME_MAX_SIZE]);
	int			GetInt		(LPCTSTR szSection, LPCTSTR szKey, int nDefault = 0);
	LPCTSTR		GetString	(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault = "");
	BOOL		WriteInt	(LPCTSTR szSection, LPCTSTR szKey, int nValue);
	BOOL		WriteString	(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);

public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );


	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	BOOL		SetSourcePath				(LPCTSTR szValue);
	BOOL		SetAttachPath				(LPCTSTR szValue);


	int			GetPatchCount				(void);
	LPCTSTR		GetPatchDir					(BYTE byDir);
	LPCTSTR		GetSettingDir				(BYTE byDir);
	LPCTSTR		GetSourcePatchDir			(void);
	LPCTSTR		GetServerName				(BYTE byDir);
	LPCTSTR		GetGMSDBConnectionString	(void);
	LPCTSTR		GetAttachDir				(void);
	LPCTSTR		GetMasterServerIP			(void);
	int			GetMasterServerPort			(void);
	LPCTSTR		GetDestinationPath			(int nDir);
	BOOL		SetDestinationPath			(int nDir, LPCTSTR szValue);
	int			GetPathCount				(void);
	BOOL		SetPathCount				(int nValue);
	

	CTNDBConfig(void);
	virtual ~CTNDBConfig(void);
};

#endif