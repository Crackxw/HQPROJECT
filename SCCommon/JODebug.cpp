
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JODebug.cpp

Author: 이민수 <nshangukin@joyon.com>
Currently Managed by: 이민수

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include <GSL.h>
#include "JODebug.h"
#include <stdio.h>
//#include <Psapi.h>
#include <time.h>
// #include <malloc.h>
#include <tchar.h>
#include <process.h>
#include <stdlib.h>

DWORD	g_dwApp;

/////////////////////////////////////////////////////////////////////////////

char	g_szDebugFilePath	[MAX_PATH];
char	g_szDebugApame		[2][MAX_PATH];
char	g_szTitle			[MAX_PATH] = {0,};
char	g_szSupposition		[256]	= {"Unknown"};
char	g_szNationCode		[64]	= {"Unknown"};

struct __STIME
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
};

__STIME  g_StartTime;
__STIME	 g_EndTime;


/////////////////////////////////////////////////////////////////////////////

LONG _CrashHandler(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	int					nIndex = 0;
	time_t				t;
	struct tm*			crashtime;
	PEXCEPTION_RECORD	pExceptionRecord	= pExceptionInfo->ExceptionRecord;
//	FILE*				hFile;
	char				szFileName		[MAX_PATH] = {0,};
	char				szExceptionInfo	[MAX_PATH] = {0,};
	char				szModule[MAX_PATH] = {0,};
	char				szDate[MAX_PATH] = {0,};
	char				szCrashAddress[MAX_PATH*2] = {0,};
	char				szEIP[MAX_PATH] = {0,};

	__try {

		time(&t);
		crashtime = localtime(&t);

		sprintf(szFileName, "%s\\%s_%04d%02d%02d_%02d%02d%02d.pdi",
			g_szDebugFilePath,
			g_szDebugApame[1],
			crashtime->tm_year + 1900,
			crashtime->tm_mon + 1,
			crashtime->tm_mday,
			crashtime->tm_hour,
			crashtime->tm_min,
			crashtime->tm_sec);

		g_EndTime.nYear		= crashtime->tm_year + 1900;
		g_EndTime.nMonth	= crashtime->tm_mon + 1;
		g_EndTime.nDay		= crashtime->tm_mday;
		g_EndTime.nHour		= crashtime->tm_hour;
		g_EndTime.nMinute	= crashtime->tm_min;
		g_EndTime.nSecond	= crashtime->tm_sec;

////	hFile = fopen(szFileName, "wt");
////		if(hFile == NULL)
////			return EXCEPTION_CONTINUE_SEARCH;

		nIndex = GetModuleFileName(NULL, szModule, MAX_PATH+1); 
		/*
		nIndex = GetModuleBaseName(
			GetCurrentProcess(),
			NULL,
			szExceptionInfo,
			MAX_PATH);
			*/
		nIndex += sprintf(szExceptionInfo, "caused on %s.\n\n",
			ConvertExceptionCodeToString(pExceptionInfo->ExceptionRecord->ExceptionCode));

////		fprintf(hFile, "UNHANDLED EXCEPTION ERROR LOG FILE\n");
			strncpy(szDate, asctime(crashtime), MAX_PATH);
////		fprintf(hFile, "%s\n", szDate);
////		fprintf(hFile, "MODULE : %s\n", szModule);
////		fprintf(hFile, szExceptionInfo);

////		if (pExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
////			if (pExceptionRecord->ExceptionInformation[0] == 0)
////				fprintf(hFile, "\tAttempted to read the inaccessible data: ");
////			else
////				fprintf(hFile, "\tAttempted to write to an inaccessible address: ");
////			fprintf(hFile, "0x%08X.\n", pExceptionRecord->ExceptionInformation[1]);
////		}
////		fprintf(hFile, "Registers:\n%s\n", GetRegisterStatus(pExceptionInfo));			
		
		sprintf(szCrashAddress, "%s: address = 0x%08X.\nLine address = 0x%08X.\n",
			ConvertExceptionCodeToString(pExceptionRecord->ExceptionCode),
			pExceptionRecord->ExceptionAddress,
			(int)pExceptionRecord->ExceptionAddress-0x00400000-0x1000);

		sprintf(szEIP, "EIP=0x%08X", pExceptionInfo->ContextRecord->Eip);

////		fprintf(hFile, "%s", szCrashAddress);

		

	//	fprintf(hFile, "%s: address = 0x%08X.\n",
	//		ConvertExceptionCodeToString(pExceptionRecord->ExceptionCode),
	//		pExceptionRecord->ExceptionAddress);

	}
	__finally {

//		fclose(hFile);
		CrashSender(ConvertExceptionCodeToNumber(pExceptionRecord->ExceptionCode), 
			(LONG)pExceptionRecord->ExceptionAddress, 
			(LONG)pExceptionRecord->ExceptionAddress-0x00400000-0x1000, 
			szEIP);
//		CrashSenderMail(szDate, szModule, szExceptionInfo, szCrashAddress);
//		if(g_dwApp == CRASH_CLIENT)
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

/////////////////////////////////////////////////////////////////////////////


void __stdcall CrashSenderMail(char* szDate, 
							   char* szModule, 
							   char* szExceptionInfo, 
							   char* szCrashAddress)
{
	char				szVersion[MAX_PATH];
	char				szStartTime[MAX_PATH];

	sprintf(szVersion, "%s : %s\n", g_szTitle, szDate);
	sprintf(szStartTime, "Start Time : %d.%d.%d %d:%d:%d", 
		g_StartTime.nYear, 
		g_StartTime.nMonth, 
		g_StartTime.nDay, 
		g_StartTime.nHour, 
		g_StartTime.nMinute, 
		g_StartTime.nSecond);

	//---------------------------------------------------------------------------	
	char szInfo[MAX_PATH*2] = {0,};	
	char *args[5];
	char prog[80] = "GCS.exe";
	args[0] = "GCS";
	args[1] = szVersion;
	args[2] = szModule;
	sprintf(szInfo, "%s\n%s\nSupposition:%s\n%s\n", 
		szExceptionInfo, szCrashAddress, g_szSupposition, szStartTime);
	args[3] = szInfo;
	args[4] = NULL;

	_execv( prog, args );
}

/////////////////////////////////////////////////////////////////////////////

void __stdcall CrashSender(SHORT snCrashType,
						   LONG  nCrashAddress, 
						   LONG  nLineAddress, 
						   char* szEIP)
{
	char				szStartTime[MAX_PATH]	= {0,};
	char				szCrashInfo[MAX_PATH*2]	= {0,};
	char				szLineAddress[64]		= {0,};
//	char *args[5];
	char *args[2];

	char prog[80] = "GCS.exe";

	// kuzimo
	args[0] = "GCS";
	args[1] = szCrashInfo;
/*
	args[1] = szStartTime;
	args[2] = szLineAddress;
	args[3] = szCrashInfo;
	args[4] = NULL;
*/
	// actdoll (2005/01/05 18:16) : 
	//	여기서 일단 클라이언트 버전이 운영자 모드일 경우
	//	텍스트 파일로 남기도록 한다.
	if( g_dwApp	== CRASH_CLIENT_ADMIN )
	{
		sprintf(szCrashInfo, " StartTime = %04d-%02d-%02d %02d:%02d:%02d\n EndTime = %04d-%02d-%02d %02d:%02d:%02d\n Type = %d\n CAddr = 0x%08X\n LAddr = 0x%08X\n Nation = %s EIP = %s\n", 
			g_StartTime.nYear, g_StartTime.nMonth, g_StartTime.nDay, 
			g_StartTime.nHour, g_StartTime.nMinute, g_StartTime.nSecond, 
			g_EndTime.nYear, g_EndTime.nMonth, g_EndTime.nDay, 
			g_EndTime.nHour, g_EndTime.nMinute, g_EndTime.nSecond, 
			snCrashType, nCrashAddress, nLineAddress, g_szNationCode, szEIP);
		FILE	*fp = fopen( "c:\\GersangLastCrash.txt", "wb" );
		fprintf( fp, szCrashInfo );
		fclose( fp );
	}

	sprintf(szCrashInfo, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %s %s", 
		g_StartTime.nYear, g_StartTime.nMonth, g_StartTime.nDay, 
		g_StartTime.nHour, g_StartTime.nMinute, g_StartTime.nSecond, 
		g_EndTime.nYear, g_EndTime.nMonth, g_EndTime.nDay, 
		g_EndTime.nHour, g_EndTime.nMinute, g_EndTime.nSecond, 
		snCrashType, nCrashAddress, nLineAddress, g_szNationCode, szEIP);
	
	_execv( prog, args );
}

/////////////////////////////////////////////////////////////////////////////

void __stdcall SetSupposition(const char szSupposition[256])
{
	strncpy(g_szSupposition, szSupposition, 256);
}

/////////////////////////////////////////////////////////////////////////////

void __stdcall SetNationCode(const char szNationCode[64])
{
	strncpy(g_szNationCode, szNationCode, 64);
}

/////////////////////////////////////////////////////////////////////////////

void __stdcall SetAppType(DWORD dwApp)
{
	g_dwApp	= dwApp;
}

/////////////////////////////////////////////////////////////////////////////

const char* __stdcall ConvertExceptionCodeToString(DWORD dwExceptionCode)
{
	switch (dwExceptionCode) {
		case EXCEPTION_ACCESS_VIOLATION:
			return "EXCEPTION_ACCESS_VIOLATION";
			break;

		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
			break;

		case EXCEPTION_BREAKPOINT:
			return "EXCEPTION_BREAKPOINT";
			break;

		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "EXCEPTION_DATATYPE_MISALIGNMENT";
			break;

		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "EXCEPTION_FLT_DENORMAL_OPERAND";
			break;

		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
			break;

		case EXCEPTION_FLT_INEXACT_RESULT:
			return "EXCEPTION_FLT_INEXACT_RESULT";
			break;

		case EXCEPTION_FLT_INVALID_OPERATION:
			return "EXCEPTION_FLT_INVALID_OPERATION";
			break;

		case EXCEPTION_FLT_OVERFLOW:
			return "EXCEPTION_FLT_OVERFLOW";
			break;

		case EXCEPTION_FLT_STACK_CHECK:
			return "EXCEPTION_FLT_STACK_CHECK";
			break;

		case EXCEPTION_FLT_UNDERFLOW:
			return "EXCEPTION_FLT_UNDERFLOW";
			break;

		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "EXCEPTION_ILLEGAL_INSTRUCTION";
			break;

		case EXCEPTION_IN_PAGE_ERROR:
			return "EXCEPTION_IN_PAGE_ERROR";
			break;

		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
			break;

		case EXCEPTION_INT_OVERFLOW:
			return "EXCEPTION_INT_OVERFLOW";
			break;

		case EXCEPTION_INVALID_DISPOSITION:
			return "EXCEPTION_INVALID_DISPOSITION";
			break;

		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
			break;

		case EXCEPTION_PRIV_INSTRUCTION:
			return "EXCEPTION_PRIV_INSTRUCTION";
			break;

		case EXCEPTION_SINGLE_STEP:
			return "EXCEPTION_SINGLE_STEP";
			break;

		case EXCEPTION_STACK_OVERFLOW:
			return "EXCEPTION_STACK_OVERFLOW";
			break;

		default:
			break;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

SHORT __stdcall ConvertExceptionCodeToNumber(DWORD dwExceptionCode)
{
	SHORT snRetVal = 0;
	
	switch (dwExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			snRetVal = 1;
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			snRetVal = 2;
			break;
		case EXCEPTION_BREAKPOINT:
			snRetVal = 3;
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			snRetVal = 4;
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			snRetVal = 5;
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			snRetVal = 6;
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			snRetVal = 7;
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			snRetVal = 8;
			break;
		case EXCEPTION_FLT_OVERFLOW:
			snRetVal = 9;
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			snRetVal = 10;
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			snRetVal = 11;
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			snRetVal = 12;
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			snRetVal = 13;
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			snRetVal = 14;
			break;
		case EXCEPTION_INT_OVERFLOW:
			snRetVal = 15;
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			snRetVal = 16;
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			snRetVal = 17;
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			snRetVal = 18;
			break;
		case EXCEPTION_SINGLE_STEP:
			snRetVal = 19;
			break;
		case EXCEPTION_STACK_OVERFLOW:
			snRetVal = 20;
			break;
		default:
			snRetVal = 0;
			break;
	}

	return snRetVal;
}

/////////////////////////////////////////////////////////////////////////////

void _stdcall InstallCrashHandler(const char* szApame1, 
								  const char* szApame2, 
								  const char* szTitle, 
								  DWORD dwApp)
{
	GetCurrentDirectory(MAX_PATH, g_szDebugFilePath);

	strcpy(g_szDebugApame[0], szApame1);
	strcpy(g_szDebugApame[1], szApame2);
	strcpy(g_szTitle, szTitle);
	g_dwApp = dwApp;

	time_t				t;
	struct tm*			crashtime;	
	
	time(&t);
	crashtime = localtime(&t);

	g_StartTime.nYear	= crashtime->tm_year+1900;
	g_StartTime.nMonth	= crashtime->tm_mon+1;
	g_StartTime.nDay	= crashtime->tm_mday;
	g_StartTime.nHour	= crashtime->tm_hour;
	g_StartTime.nMinute = crashtime->tm_min;
	g_StartTime.nSecond = crashtime->tm_sec;

	SetUnhandledExceptionFilter(
		(LPTOP_LEVEL_EXCEPTION_FILTER)_CrashHandler);
}

/////////////////////////////////////////////////////////////////////////////

const char*	__stdcall GetRegisterStatus(EXCEPTION_POINTERS* pExceptionInfo)
{
	static char	szRegisterStatus[512];

	sprintf(szRegisterStatus, 
		"EAX=0x%08X EBX=0x%08X ECX=0x%08X EDX=0x%08X ESI=0x%08X\n"	\
		"EDI=0x%08X EBP=0x%08X ESP=0x%08X EIP=0x%08X FLG=0x%08X\n"	\
		"CS=0x%04X DS=0x%04X SS=0x%04X ES=0x%04X FS=0x%04X GS=0x%04X\n",
		pExceptionInfo->ContextRecord->Eax,
		pExceptionInfo->ContextRecord->Ebx,
		pExceptionInfo->ContextRecord->Ecx,
		pExceptionInfo->ContextRecord->Edx,
		pExceptionInfo->ContextRecord->Esi,
		pExceptionInfo->ContextRecord->Edi,
		pExceptionInfo->ContextRecord->Ebp,
		pExceptionInfo->ContextRecord->Esp,
		pExceptionInfo->ContextRecord->Eip,
		pExceptionInfo->ContextRecord->EFlags,
		pExceptionInfo->ContextRecord->SegCs,
		pExceptionInfo->ContextRecord->SegDs,
		pExceptionInfo->ContextRecord->SegSs,
		pExceptionInfo->ContextRecord->SegEs,
		pExceptionInfo->ContextRecord->SegFs,
		pExceptionInfo->ContextRecord->SegGs);

	return szRegisterStatus;
}

/////////////////////////////////////////////////////////////////////////////

const void	__stdcall Write( char* pLog, char* chFileName )
{
	
	HANDLE		hFile;
	DWORD		dwRetWrite;
	int			nBufLength;	

	CreateDirectory( "\\Data", NULL) ;
	nBufLength	= strlen(pLog);
	hFile = CreateFile(chFileName,					  // file name
						GENERIC_WRITE|GENERIC_READ,   // open for writing 
						FILE_SHARE_WRITE,             // do not share 
						NULL,                         // no security 
						OPEN_ALWAYS,                  // open or create 
						FILE_ATTRIBUTE_NORMAL,        // normal file 
						NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{

	}
	else
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
		WriteFile(hFile, pLog, nBufLength, &dwRetWrite, NULL);
		CloseHandle(hFile);
	}
}

/////////////////////////////////////////////////////////////////////////////


