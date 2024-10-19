

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOCharListUpdateConfig.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __JO_CHARLIST_UPDATE_CONFIG_H_INCLUDED__
#define __JO_CHARLIST_UPDATE_CONFIG_H_INCLUDED__

#include <windows.h>
#include "..\JOCommon\JOConfig.h"

///////////////////////////////////////////////////////////////////////
// 디폴트 파일 이름
//
#define GERSANG_CHAR_LIST_UPDATE_INFO_FILE			_T("character list update.ini")
///////////////////////////////////////////////////////////////////////
//
// Key 종류
//
///////////////////////////////////////////////////////////////////////
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
#define CONFIG_COUNT								_T("count")
#define CONFIG_NAME									_T("name")
#define CONFIG_GAME_DB_CONNECTION_STRING			_T("DB Connection String")

///////////////////////////////////////////////////////////////////////
//
// Section 종류
//
///////////////////////////////////////////////////////////////////////
#define CHARLIST_TOOL_INFO_CONFIG					_T("CHARLIST UPDATE TOOL")
#define CHARLIST_INFO_CONFIG						_T("CHARLIST INFO")
///////////////////////////////////////////////////////////////////////

class CJOCharListUpdateConfig : public CJOConfig
{
public : 
	CJOCharListUpdateConfig(void);
	virtual ~CJOCharListUpdateConfig(void);

public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );

	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	void		SetConfigDirectoryName		(LPCTSTR szConfigDirectoryName);	
	
	int			DBConnectCountInt			();
	LPCTSTR		DBConnectionString			(int nCur);
	LPCTSTR		DBServerNameString			(int nCur);
};


#endif // __JO_CRASH_SERVER_CONFIG_H_INCLUDED__
