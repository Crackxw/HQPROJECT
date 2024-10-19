

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOChinaAuthServerConfig.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __JO_CHINA_AUTH_SERVER_CONFIG_H_INCLUDED__
#define __JO_CHINA_AUTH_SERVER_CONFIG_H_INCLUDED__

#include <windows.h>
#include "..\GCommon\JOConfig.h"

///////////////////////////////////////////////////////////////////////
// 디폴트 파일 이름
//
#define GERSANG_GASH_CLIENT_INFO_FILE				_T("Gash client info.ini")
///////////////////////////////////////////////////////////////////////
//
// Key 종류
//
///////////////////////////////////////////////////////////////////////
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
#define CONFIG_MAX_CONNECTION_COUNT					_T("max connection count")
#define CONFIG_CONNECTION_TIMEOUT					_T("timeout second")
#define CONFIG_NATION_CODE							_T("nation code")
#define CONFIG_SERVICE_CODE							_T("service code")
#define CONFIG_REGION_CODE							_T("region")
#define CONFIG_LOCK_FLAG							_T("lock flag")
#define CONFIG_AUTHENTICATION_FLAG					_T("authentication flag")
#define CONFIG_TYPE									_T("type")
#define CONFIG_DUPLICATE_CHECK						_T("duplicate check")

///////////////////////////////////////////////////////////////////////
//
// Section 종류
//
///////////////////////////////////////////////////////////////////////
#define NATION_CODE_CONFIG							_T("NATION CODE")
#define AUTHENTICATION_CONFIG						_T("AUTHENTICATION INFO")
#define AUTHORIZATION_CONFIG						_T("AUTHORIZATION INFO")
#define ACCOUTING_CONFIG							_T("ACCOUTING INFO")
#define CONNECTION_SOCKET_CONFIG					_T("CONNECTION INFO")
///////////////////////////////////////////////////////////////////////

class CJOChinaAuthServerConfig : public CJOConfig
{
public : 
	CJOChinaAuthServerConfig(void);
	virtual ~CJOChinaAuthServerConfig(void);

public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );


	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	void		SetConfigDirectoryName		(LPCTSTR szConfigDirectoryName);

	// Nation code.
	LPCTSTR		GetNationCode				(void);
		
	// Authentication.
	LPCTSTR		Authentication_IP			(void);
	int			Authentication_Port			(void);
	LPCTSTR		Authentication_ServiceCode	(void);
	LPCTSTR		Authentication_Region		(void);
	LPCTSTR		Authentication_LockFlag		(void);

	// Authorization.
	LPCTSTR		Authorization_IP			(void);
	int			Authorization_Port			(void);
	LPCTSTR		Authorization_ServiceCode	(void);
	LPCTSTR		Authorization_Region		(void);
	LPCTSTR		Authorization_AuthenticationFlag(void);
	LPCTSTR		Authorization_Type			(void);
	LPCTSTR		Authorization_DuplicateCheck(void);
	
	// Accounting
	LPCTSTR		Accounting_IP				(void);
	int			Accounting_Port				(void);
	LPCTSTR		Accounting_ServiceCode		(void);
	LPCTSTR		Accounting_Region			(void);
	LPCTSTR		Accounting_Type				(void);

	// Socket Connection
	int			GetMaxConnectionSocketCount		(void);
	int			GetSocketConnectTimeoutSecond	(void);
};


#endif
