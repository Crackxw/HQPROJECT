

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOCrashClientConfig.h

Author: �̹μ� <nshangukin@joyon.com>
Currently Managed by: �̹μ�

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
// ����Ʈ ���� �̸�
//
#define GERSANG_CRASH_SERVER_INFO_FILE				_T("Crash client info.ini")
///////////////////////////////////////////////////////////////////////
//
// Key ����
//
///////////////////////////////////////////////////////////////////////
#define CONFIG_IP									_T("ip")
#define CONFIG_PORT									_T("port")
///////////////////////////////////////////////////////////////////////
//
// Section ����
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
