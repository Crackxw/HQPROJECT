/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOConfig.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef	__CONFIG_H_INCLUDED__
#define __CONFIG_H_INCLUDED__
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define CONFIG_FILE_NAME_MAX_SIZE			128

/// #define __CONFIG_FILE_LOAD__

class  CJOConfig
{
private:
	char		m_strConfigFileName[CONFIG_FILE_NAME_MAX_SIZE];;

public:
	CJOConfig			();
	virtual ~CJOConfig	();

protected:
	BOOL		Open		(LPCTSTR szConfigFileName);
	int			GetInt		(LPCTSTR szSection, LPCTSTR szKey, int nDefault = 0);
	LPCTSTR		GetString	(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault = "");
	BOOL		WriteInt	(LPCTSTR szSection, LPCTSTR szKey, int nValue);
	BOOL		WriteString	(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
};

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#endif	//	__CONFIG_H_INCLUDED__
