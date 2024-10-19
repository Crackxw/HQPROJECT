

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOCrashServerConfig.h

Author: �̹μ� <nshangukin@joyon.com>
Currently Managed by: �̹μ�

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#ifndef __JO_CRASH_SERVER_CONFIG_H_INCLUDED__
#define __JO_CRASH_SERVER_CONFIG_H_INCLUDED__

#include <windows.h>
#include "..\JOCommon\JOConfig.h"

///////////////////////////////////////////////////////////////////////
// ����Ʈ ���� �̸�
//
#define GERSANG_CRASH_SERVER_INFO_FILE				_T("Crash server info.ini")
///////////////////////////////////////////////////////////////////////
//
// Key ����
//
///////////////////////////////////////////////////////////////////////
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
#define CONFIG_CRASH_DB_CONNECTION_STRING			_T("Crash DB Connection String")

///////////////////////////////////////////////////////////////////////
//
// Section ����
//
///////////////////////////////////////////////////////////////////////
#define CRASH_SERVER_INFO_CONFIG					_T("CRASH SERVER")
///////////////////////////////////////////////////////////////////////

class CJOCrashServerConfig : public CJOConfig
{
public : 
	CJOCrashServerConfig(void);
	virtual ~CJOCrashServerConfig(void);

public:
	LPCTSTR		GetStr						(LPCTSTR szConfigName, LPCTSTR szKey );
	int			GetNum						(LPCTSTR szConfigName, LPCTSTR szKey );

	void		SetConfigFileName			(LPCTSTR szConfigFileName);
	void		SetConfigDirectoryName		(LPCTSTR szConfigDirectoryName);	
	
	LPCTSTR		CrashDBConnectionString		(void);		
};


#endif // __JO_CRASH_SERVER_CONFIG_H_INCLUDED__
