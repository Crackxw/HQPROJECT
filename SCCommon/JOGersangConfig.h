
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOGersangConfig.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __JO_GERSANG_SETTING_CONFIG_H__
#define __JO_GERSANG_SETTING_CONFIG_H__

#include <windows.h>
#include "..\GCommon\JOConfig.h"

///////////////////////////////////////////////////////////////////////
// 디폴트 파일 이름
//
#define GERSANG_SERVER_CONFIG_FILE					_T("Gersang.ini")
///////////////////////////////////////////////////////////////////////
//
// Key 종류
//
///////////////////////////////////////////////////////////////////////

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Common
//
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
#define CONFIG_SERVER_ID							_T("server id")
#define CONFIG_NAME									_T("name")
#define CONFIG_USE_LANGUAGE							_T("language")
#define CONFIG_COUNT								_T("count")
#define CONFIG_DB_CATALOG							_T("catalog")
#define CONFIG_DB_USER								_T("db user")
#define CONFIG_DB_PASSWORD							_T("db password")
#define CONFIG_VERSION								_T("version")
#define CONFIG_USER_COUNT							_T("user count")
#define CONFIG_USER_PORT							_T("user port")
#define CONFIG_YEAR									_T("year")
#define CONFIG_MONTH								_T("month")
#define CONFIG_DAY									_T("day")
#define CONFIG_ID									_T("id")
#define CONFIG_PASSWORD								_T("password")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Auth Server
//
#define AUTH_CONFIG_SERVER_NUMBER					_T("server number")
#define AUTH_CONFIG_WORK_QUEUE_SIZE					_T("work queue size")
#define AUTH_CONFIG_CLASS_CONTEXT					_T("class context")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Master Server
//
#define MASTER_CONFIG_TYPE							_T("type")
#define MASTER_CONFIG_SERVER_COUNT					_T("server count")
#define MASTER_CONFIG_GROUP_COUNT					_T("group count")
#define MASTER_CONFIG_SERVER_NAME					_T("server name")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Monitor Server
//
#define MONITOR_CONFIG_GAME_SERVER_PATH				_T("game server path")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Front Server
//
#define FRONT_CONFIG_GATE_IP						_T("gate ip")
#define FRONT_CONFIG_CHAT_IP						_T("chat ip")
#define FRONT_CONFIG_GATE_PORT						_T("gate port")
#define FRONT_CONFIG_CHAT_PORT						_T("chat port")
#define FRONT_CONFIG_NOTICE							_T("notice")

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Game Server
//
#define GAME_CONFIG_WORLD_ID						_T("world id")
#define GAME_CONFIG_TEST_SERVER_FLAG				_T("test server")
#define GAME_CONFIG_MAX_CONCURRENT					_T("max concurrent")
#define GAME_CONFIG_AUTH_SERVER_IP					_T("AuthServerIP")
#define GAME_CONFIG_FRONT_SERVER_IP					_T("FrontServerIP")

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Version Server
//
#define VERSION_CONFIG_VERSION_COUNT				_T("version count")
#define VERSION_CONFIG_IP_COUNT						_T("ip count")
#define VERSION_CONFIG_AUTH_COUNT					_T("auth count")
#define VERSION_CONFING_VERSION_NUMBER				_T("ver")
///////////////////////////////////////////////////////////////////////
//
// Section 종류
//
///////////////////////////////////////////////////////////////////////

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Auth Server
//
#define AUTH_SERVER_CONFIG							_T("AUTH SERVER")
#define AUTH_SERVER_DB_INFO_CONFIG					_T("AUTH SERVER DB INFO")
#define AUTH_SERVER_AUTHORIZER_INFO_CONFIG			_T("AUTH SERVER AUTHORIZER INFO")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Master Server
//
#define MASTER_SERVER_CONFIG						_T("MASTER SERVER")
#define MASTER_SERVER_INFO_CONFIG					_T("MASTER SERVER INFO")
#define MASTER_SERVER_GROUP_CONFIG					_T("MASTER SERVER GROUP")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Monitor Server
//
#define MONITOR_SERVER_CONFIG						_T("MONITOR SERVER")
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Front Server
//
#define FRONT_SERVER_CONFIG							_T("FRONT SERVER")
#define FRONT_WORLD_INFO_CONFIG						_T("FRONT WORLD INFO")
#define FRONT_NOTICE_CONFIG							_T("FRONT NOTICE")


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Game Server
//
#define GAME_SERVER_CONFIG							_T("GAME SERVER")
#define GAME_LINK_SERVER_INFO_CONFIG				_T("GAME LINK SERVER INFO")
#define GAME_WORLD_INFO_CONFIG						_T("GAME WORLD INFO")
#define GAME_DB_INFO_CONFIG							_T("GAME DB INFO")
#define GAME_ITEM_DB_INFO_CONFIG					_T("GAME ITEM DB INFO")

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Version Server
//
#define VERSION_SERVER_CONFIG						_T("VERSION SERVER")
#define VERSION_SERVER_VERSION_LIST_CONFIG			_T("VERSION SERVER VERSION LIST")
#define VERSION_SERVER_IP_LIST_CONFIG				_T("VERSION SERVER IP LIST")
#define VERSION_SERVER_AUTH_LIST_CONFIG				_T("VERSION SERVER AUTH LIST")
//-------------------------------------------------------------------------------------


class CJOGersangConfig : public CJOConfig
{
private : 
//	char		m_strConfigFileName[CONFIG_FILE_NAME_MAX_SIZE];
public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );


	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	void		SetConfigDirectoryName		(LPCTSTR szConfigDirectoryName);


	// 마스터 서버
	LPCTSTR		GetMasterServerName			(int nGroup);	
	LPCTSTR     GetMasterServerIP			(int nGroup, int nCur);
	LPCTSTR		GetMasterServerGroupName	(int nGroup);
	int			GetMasterServerCount		(int nGroup);
	LPCTSTR		GetMasterServerType			(int nGroup);	
	int			GetMasterGroupCount			(void);

	// 모니터 서버
	LPCTSTR		GetMonitorOfGameServerPath	(LPCTSTR szSection);

	// 프런트 서버
	int			GetFrontServerCount			(void);
	int			GetFrontYear				(int nCur);
	int			GetFrontMonth				(int nCur);
	int			GetFrontDay					(int nCur);
	int			GetFrontServerID			(int nCur);
	LPCTSTR		GetFrontServerName			(int nCur);
	LPCTSTR		GetFrontGateIP				(int nCur);
	LPCTSTR		GetFrontChatIP				(int nCur);
	int			GetFrontGatePort			(int nCur);
	int			GetFrontChatPort			(int nCur);
	LPCTSTR		GetFrontNotice				(int nCur);
	int			GetNoticeCount				(void);

	// 버젼 서버
	int			GetVersionCountOfVersionServer	(void);
	int			GetIPCountOfVersionServer		(void);
	int			GetAuthCountOfVersionServer		(void);
	int			GetVersionListOfVersionServer	(int nCur);
	LPCTSTR		GetIPListOfVersionServer		(int nCur);
	LPCTSTR		GetIDListOfVersionServer		(int nCur);
	LPCTSTR		GetPasswordListOfVersionServer	(int nCur);

	CJOGersangConfig(void);
	virtual ~CJOGersangConfig(void);
};

#endif // __JO_GERSANG_SETTING_CONFIG_H__