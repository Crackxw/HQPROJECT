

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: LogWirter.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

//////////////////////////////////////////////////////////////////////////////////

#ifndef __LOGWRITER_H__
#define __LOGWRITER_H__

//////////////////////////////////////////////////////////////////////////////////

#include <tchar.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////////////

#define		__LOG_ACTIVE__

//////////////////////////////////////////////////////////////////////////////////

class CLogWriter{
public :
	void PrintF(const TCHAR* pFileName, const int nLine, const TCHAR* _Format, ...);
	void PrintF(const TCHAR* pFileName, const TCHAR* _Format, ...);
	HRESULT Start(TCHAR* pFileName);
	void End();

	CLogWriter();
	~CLogWriter();

private :
	void Write(TCHAR* _Log);

	void Enter()		{ ::EnterCriticalSection(&m_csLock); }
	void Leave()		{ ::LeaveCriticalSection(&m_csLock); }

private :
	FILE*  m_pFile;
	TCHAR  m_szFileName[128];

private : 
	CRITICAL_SECTION	m_csLock;
};

//////////////////////////////////////////////////////////////////////////////////

#endif