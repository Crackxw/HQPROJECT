

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOCrashClientConfig.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __JO_CRASH_CLIENT_CONFIG_H_INCLUDED__
#define __JO_CRASH_CLIENT_CONFIG_H_INCLUDED__

#include <windows.h>
#include "..\JOCommon\JOConfig.h"

///////////////////////////////////////////////////////////////////////
// 디폴트 파일 이름
//
#define GERSANG_CRASH_SERVER_INFO_FILE				_T("Crash client info.ini")
///////////////////////////////////////////////////////////////////////
//
// Key 종류
//
///////////////////////////////////////////////////////////////////////
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
///////////////////////////////////////////////////////////////////////
//
// Section 종류
//
///////////////////////////////////////////////////////////////////////
#define CRASH_CLIENT_INFO_CONFIG					_T("CRASH CLIENT")
///////////////////////////////////////////////////////////////////////

class CJOCrashClientConfig : public CJOConfig
{
public : 
	CJOCrashClientConfig(void);
	virtual ~CJOCrashClientConfig(void);

public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );

	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	void		SetConfigDirectoryName		(LPCTSTR szConfigDirectoryName);	
	
	LPCTSTR		GetCrashServerIP			(void);
	void		SetCrashServerIP			(LPCTSTR szIP);
};


#endif // __JO_CRASH_CLIENT_CONFIG_H_INCLUDED__
