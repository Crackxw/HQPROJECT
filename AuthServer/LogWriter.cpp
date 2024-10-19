

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: LogWriter.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/////////////////////////////////////////////////////////////////////

#include "LogWriter.h"
#include <malloc.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>

/////////////////////////////////////////////////////////////////////

CLogWriter::CLogWriter() : m_pFile(NULL)
{
#ifdef __LOG_ACTIVE__
	::InitializeCriticalSection(&m_csLock);
#endif
}

/////////////////////////////////////////////////////////////////////

CLogWriter::~CLogWriter()
{
#ifdef __LOG_ACTIVE__
	Enter();
	if(m_pFile) fclose(m_pFile);
	Leave();

	::DeleteCriticalSection(&m_csLock);
#endif
}

/////////////////////////////////////////////////////////////////////

HRESULT CLogWriter::Start(TCHAR* pFileName)
{
#ifdef __LOG_ACTIVE__
	Enter();

	_ASSERTE(NULL == m_pFile);

	m_pFile	=	fopen(pFileName, "at");
	
	Leave();
	return m_pFile != NULL ? 0 : -1;
#else
	return 0;
#endif
}

/////////////////////////////////////////////////////////////////////

void CLogWriter::End()
{
#ifdef __LOG_ACTIVE__
	Enter();
	if(m_pFile){
		fclose(m_pFile);
		m_pFile = NULL;
	}
	Leave();
#endif
}

/////////////////////////////////////////////////////////////////////

void CLogWriter::Write(TCHAR* _Log)
{
#ifdef __LOG_ACTIVE__
	fprintf(m_pFile, "%s\r\n", _Log);
#endif
}

/////////////////////////////////////////////////////////////////////

void CLogWriter::PrintF(const TCHAR* pFileName, const int nLine, const TCHAR* _Format, ...)
{
#ifdef __LOG_ACTIVE__
	Enter();	
	if(m_pFile == NULL)
	{
		Leave();
		return;
	}

	TCHAR fmtBuf[1024] = {0};
	TCHAR buf[_MAX_DIR*3];
	int nLen = 0;

	va_list args;
	va_start(args, _Format);
	size_t nWritten = _vsntprintf(fmtBuf, sizeof(fmtBuf)/sizeof(TCHAR), _Format, args);
	va_end(args);

	printf("%s\n", fmtBuf);
	nLen += _stprintf(buf+nLen, "File: %s\t", pFileName);
	nLen += _stprintf(buf+nLen, "Line: %d\t", nLine);
	nLen += _stprintf(buf+nLen, "Contents:%s", fmtBuf);
	
	Write(buf);

	Leave();
#endif	
}

/////////////////////////////////////////////////////////////////////

void CLogWriter::PrintF(const TCHAR* pFileName, const TCHAR* _Format, ...)
{
#ifdef __LOG_ACTIVE__
	Enter();	

	TCHAR	szFilePath[1024]={0,};
	TCHAR	thismonth[128]={0,};
	TCHAR	szDateBuf[128]={0,};
	TCHAR	szTimeBuf[128]={0,};
	TCHAR	fmtBuf[1024] = {0,};
	time_t	ltime;
	struct	tm *today;	
	TCHAR buf[_MAX_DIR*3];
	int nLen = 0;

	time(&ltime);
	today = localtime( &ltime );
	strftime( thismonth, 128, "%Y%m%d", today );

	_stprintf( szFilePath, "[%s] %s", thismonth, pFileName);
	Start(szFilePath);

	if(m_pFile == NULL)
	{
		Leave();
		return;
	}

	va_list args;
	va_start(args, _Format);
	size_t nWritten = _vsntprintf(fmtBuf, sizeof(fmtBuf)/sizeof(TCHAR), _Format, args);
	va_end(args);

	printf("%s\n", fmtBuf);
	
	_strtime(szTimeBuf);
	_strdate(szDateBuf);
	
	nLen += _stprintf(buf+nLen, "[%s %s]", szDateBuf, szTimeBuf);	
	nLen += _stprintf(buf+nLen, "Contents:%s", fmtBuf);
	Write(buf);

	End();

	Leave();
#endif	
}

/////////////////////////////////////////////////////////////////////
