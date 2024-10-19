
/////////////////////////////////////////////////////////////////

#ifndef __CRASHMAIL_PROTOCOL_H_INCLUDED__
#define __CRASHMAIL_PROTOCOL_H_INCLUDED__

/////////////////////////////////////////////////////////////////

#define	    CRASH_MAX_LOAD_NUMBER		10
#define		CRASH_MAX_CRASH_TYPE_NUMBER 21

/////////////////////////////////////////////////////////////////

#define		CRASH_MAIL_SERVER_IP		"220.85.19.80"

/////////////////////////////////////////////////////////////////

#define		CRASH_MAIL_USER_SERVER_PORT	20010
#define		CRASH_MAIL_TOOL_SERVER_PORT	20011

/////////////////////////////////////////////////////////////////

#define		CRASH_SERVER_RECV_BUF_SIZE	30000
#define		CRASH_SERVER_SEND_BUF_SIZE	30000
#define		CRASH_SERVER_QUEUE_SIZE		30000

/////////////////////////////////////////////////////////////////

#define		CRASH_SERVER_HEADER_SIZE	2

/////////////////////////////////////////////////////////////////

#define		ON_CRASH_MAIL_CLIENT		100
#define		ON_CRASH_MAIL_TOOL			200

/////////////////////////////////////////////////////////////////

#define ADO_CRASH_USER_INFO_SAVE		1
#define ADO_CRASH_TOOL_SEARCH_INFO		2
#define ADO_CRASH_TOOL_STATISTICS_INFO	3

/////////////////////////////////////////////////////////////////

// Message Type

// User
#define		ON_REQ_CRASH_USER_INFO_SAVE		(DWORD(10000))

// Tool
#define		ON_REQ_CRASH_SEARCH_INFO		(DWORD(10100))
#define		ON_RESP_CRASH_SEARCH_INFO		(DWORD(10101))

#define		ON_REQ_CRASH_STATISTICSINFO		(DWORD(10200))
#define		ON_RESP_CRASH_STATISTICSINFO	(DWORD(10201))

/////////////////////////////////////////////////////////////////

typedef struct __STIME
{
	SHORT nYear;
	SHORT nMonth;
	SHORT nDay;
	SHORT nHour;
	SHORT nMinute;
	SHORT nSecond;
}STIME;

//////////////////////////////////////////////////////////////////

typedef struct _ReqCrashInfo
{
	SHORT	nCrashType;
	char	szOSType[64];
	char	szProcessType[64];	
	char	szIP[20];
	SHORT	nProcessNumber;
	SHORT	nProcessLevel;
	LONG	nPageSize;
	STIME	sTime;
	STIME	eTime;
	LONG	nCrashAddress;
	LONG	nLineAddress;
	char	szNationCode[64];
	char	szSupposition[256];	
}ON_REQ_CRASHINFO, *ON_P_REQ_CRASHINFO;

//////////////////////////////////////////////////////////////////

struct		SearchCrashInfo
{
	BYTE	byWhereAndOr;		// 1 - AND 검색
								// 2 - OR 검색

	BYTE	byFiledID;			// 1 - Crash Type
								// 2 - Crash Address
								// 3 - Line Address
								// 4 - Crash Time
								// 5 - Play Time
	BYTE	byWhere;            // 1 - =
								// 2 - <>
								// 3 - <
								// 4 - >
								// 5 - <=
								// 6 - >=
	char	szValue[64];
};

//////////////////////////////////////////////////////////////////

typedef struct _ReqSearchCrashInfo
{
	BYTE	byAllField;			// 0 - 조건에 맞는 필드 검색
								// 1 - 전체 필드 검색

	char	szQuery[256];		// 
}ON_REQ_SEARCHCRASHINFO, *ON_P_REQ_SEARCHCRASHINFO;

/////////////////////////////////////////////////////////////////

typedef struct _RespSearchCrashInfo
{
	DWORD	dwID;
	SHORT	nCrashType;	
	char	szCrashContents[64];
	char	szOSType[64];
	char	szProcessType[64];	
	char	szIP[20];
	SHORT	nProcessNumber;
	SHORT	nProcessLevel;
	LONG	nPageSize;
	char	szsTime[64];
	char	szeTime[64];
	char	szCrashAddress[16];
	char	szLineAddress[16];
	char	szNationCode[64];
	char	szSupposition[256];	
}ON_RESP_SEARCHCRASHINFO, *ON_P_RESP_SEARCHCRASHINFO;

/////////////////////////////////////////////////////////////////

typedef struct _RespSearchCrashInfoList
{
	BYTE	byResult;			// 1 - 성공
								// 0 - 실패
	BYTE	byLastList;
	int		nCount;
}ON_RESP_SEARCHCRASHINFO_LIST, *ON_P_RESP_SEARCHCRASHINFO_LIST;

/////////////////////////////////////////////////////////////////

typedef struct _ReqCrashStatisticsInfo
{
	BYTE	byAllField;			// 0 - 조건에 맞는 필드 검색
								// 1 - 전체 필드 검색

	char	szQuery[256];		// 
}ON_REQ_CRASHSTATISTICSINFO, *ON_P_REQ_CRASHSTATISTICSINFO;

/////////////////////////////////////////////////////////////////

typedef struct _RespCrashStatisticsInfo
{
	BYTE    byResult;	
	int		nCrashCount[CRASH_MAX_CRASH_TYPE_NUMBER];

}ON_RESP_CRASHSTATISTICSINFO, *ON_P_RESP_CRASHSTATISTICSINFO;

/////////////////////////////////////////////////////////////////

#endif