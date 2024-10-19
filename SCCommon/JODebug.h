
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JODebug.h

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef	__JO_DEBUG_H_INCLUDED__
#define __JO_DEBUG_H_INCLUDED__

/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
// #include <crtdbg.h>

/////////////////////////////////////////////////////////////////////////////

#define __ENABLE_CRASH_FILE__

#define CRASH_CLIENT		1
#define CRASH_SERVER		2
#define CRASH_CLIENT_ADMIN	3	// actdoll (2005/01/05 18:10) : 클라이언트가 운영자 모드로 돌아가는가?

/////////////////////////////////////////////////////////////////////////////
/*
inline void EnableMemoryLeakDetect()
{
	int	nDebugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	_CrtSetDbgFlag(nDebugFlag | _CRTDBG_LEAK_CHECK_DF);
}
*/

/////////////////////////////////////////////////////////////////////////////

extern	const char*	__stdcall ConvertExceptionCodeToString	(DWORD dwExceptionCode);
extern  SHORT		__stdcall ConvertExceptionCodeToNumber	(DWORD dwExceptionCode);
extern	void		__stdcall InstallCrashHandler			(const char* szAppName1, 
															 const char* szAppName2, 
															 const char* szTitle, 
															 DWORD dwApp);
extern	const char*	__stdcall GetRegisterStatus				(EXCEPTION_POINTERS* pExceptionInfo);
extern	const void	__stdcall Write							( char* pLog, char* chFileName ) ;
extern  void		__stdcall CrashSenderMail				(char* szDate, 
															 char* szModule, 
															 char* szExceptionInfo, 
															 char* szCrashAddress);
extern	void		__stdcall CrashSender					(SHORT snCrashType,
															 LONG  nCrashAddress, 
															 LONG  nLineAddress, 
															 char* szEIP);

/////////////////////////////////////////////////////////////////////////////

extern void			__stdcall SetSupposition				(const char szSupposition[256]);
extern void			__stdcall SetNationCode					(const char szNationCode[64]);
extern void			__stdcall SetAppType					(DWORD dwApp);

/////////////////////////////////////////////////////////////////////////////

/*
void SetNationCode		(const char szNationCode[64]);
void SetSupposition		(const char szSupposition[256]);
void SetStartTime		(int nYear, 
						 int nMonth, 
						 int nDay, 
						 int nHour, 
						 int nMinute, 
						 int nSecond);
*/
#endif	//	__JO_DEBUG_H_INCLUDED__

/////////////////////////////////////////////////////////////////////////////
