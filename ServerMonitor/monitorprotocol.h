#ifndef _MONITOR_PROTOCOL_
#define _MONITOR_PROTOCOL_

#pragma pack(1)

#define MONITOR_COMMAND_NOTICE   0

// 모니터 클라이언트 -> 모니터 서버

struct Monitor_Command_Notice
{	
	char szNotice[256];		
};


#define MONITOR_COMMAND_FILETRANSFERSTART   1
#define PATCH_COMMAND_FILETRANSFERSTART		101

struct Monitor_Command_FileTransferStart
{
	int siFileSize;
	char szFileName[256];
	char szDirectory[256];
};

struct Patch_Command_FileTransferStart
{
	int siFileSize;
	char szFileName[256];
	char szDirectory[256];
	BYTE byNewVersion;
};

#define MONITOR_COMMAND_FILETRANSFER        2
#define MONITOR_COMMAND_FILETRANSFEREND     3

struct Monitor_Command_FileTransferEnd
{	
	char szFileName[256];		
};


#define MONITOR_COMMAND_SERVER_LIVE         4
#define MONITOR_COMMAND_SERVER_DEAD         5
#define MONITOR_COMMAND_SERVER_ON           6
#define MONITOR_COMMAND_SERVER_OFF          7

#define MONITOR_COMMAND_SERVER_CURRENTNUM   8

#define MONITOR_COMMAND_SERVER_TIMEOUT      9

struct Monitor_Command_Server_CurrentNum
{
	WORD wCurrentNum;	
};
 
#define MONITOR_COMMAND_REBOOT			  10

struct Monitor_Command_Reboot
{
	char szFileName[256];	
};

#define MONITOR_COMMAND_QUERY_EXECFILENAME   11

#define MONITOR_COMMAND_ANSWER_EXECFILENAME  12

struct Monitor_Command_Answer_Execfilename
{
	char szFileName[256];	
};

#define MONITOR_COMMAND_SERVER_ON_TIME       13

struct Monitor_Command_Server_On_Time
{
	SYSTEMTIME timeServerOn;

};

#define MONITOR_COMMAND_SERVER_DEAD_REASON   14

struct Monitor_Command_Server_Dead_Reason
{
	DWORD ServerDeadReason;

};

#define MONITOR_COMMAND_SERVER_QUEUE_STATUS   15

struct Monitor_Command_Server_Queue_Status
{
	DWORD GateQueueSize;
	DWORD SubQueueSize;

};

/*enum
{
	DEAD_REASON_UNKNOWN                    =    0,                       // 이유를 모르겠다.
	DEAD_REASON_DBERROR                    =    1,                       // DB ERROR!!!
	DEAD_REASON_USERMONITORTERMINATE       =    2,                       // 모니터 툴이 죽였다.

} DEAD_REASON;*/

//#define MONITOR_SERVER_PORT                 20010
#define MONITOR_SERVER_PORT                 20001
#define MASTER_SERVER_PORT					8011

#define MMC_QUERY_LOGIN						90					// ID( 32byte ) + pass( 32byte )
#define MMC_REPORT_LOGIN					91					// Result( 1 byte - 0: 성공, 1: 아뒤없음, 2: 패스워드틀림, 3:이미접속한아이디 )

#define MMC_QUERY_ADDUSER					92					// ID( 32byte ) + pass( 32byte ) + name( 32byte )
#define MMC_REPORT_ADDUSER					93					// Result( 1 byte - 0: 성공, 1: 중복아이디(실패) )

#define MMC_QUERY_DELUSER					94					// ID( 32byte )
#define MMC_REPORT_DELUSER					95					// Result( 1 byte - 0: 성공, 1: 아이디없음(실패) )

#define MMC_QUERY_USERLIST					96					
#define MMC_REPORT_USERLIST					97					// 유저 숫자( 4byte ) + 
																// ID( 32byte ) + name( 32byte ) + ...

#define MMC_QUERY_CONNECTUSERLIST			98
#define MMC_REPORT_CONNECTUSERLIST			99					// 유저 숫자( 4byte ) +
																// ID( 32byte ) + name( 32byte ) + ...

#define MMC_REPORT_USER_ENTER				120					// ID( 32byte ) + name( 32byte )
#define MMC_REPORT_USER_EXIT				122					// ID( 32byte ) + name( 32byte )

#define MMC_QUERY_CHAT						110					// 스트링길이( 2byte ) + 문자열( ?byte )
#define MMC_REPORT_CHAT						111					// ID( 32byte ) + name( 32byte ) + 스트링길이( 2byte ) + 문자열( ?byte )

#define MMC_QUERY_SERVER_STRUCT				100	
#define MMC_QUERY_WORLD_LIST				101
#define MMC_QUERY_SERVER_STATE_ALL			102
#define MMC_QUERY_SERVER_STATE				103

#define MMC_REPORT_SMS						104
#define	MMC_QUERY_SMS						105

// 각 월드의 서버 구성에 대한 보고
#define MMC_REPORT_SERVER_STRUCT			200					// 월드당 서버 대수( 4byte ) + 
																// 서버이름( 32byte ) + 서버이름( 32byte ) + ...
// 월드 정보
#define MMC_REPORT_WORLD_LIST				201					// 월드 개수( 4byte ) +
																// 월드 이름( 32byte ) + 월드이름( 32byte ) + ...
// 서버 상태
/*
#define MMC_REPORT_SERVER_STATE_ALL			202					// 서버 코드( 2byte ) + 상태( 2byte ) + ...
#define MMC_REPORT_SERVER_STATE				203					// 서버 코드( 2byte ) + 상태( 2byte )
*/

#define MMC_CLIENT_TO_SERVER				300					// 받을 서버 개수( 2byte ) + 서버코드( 2byte ) + ... + 패킷크기( 2byte ) + 패킷( ?byte )
#define MMC_SERVER_TO_CLIENT				301					// 서버 코드( 2byte ) + 패킷크기( 2byte ) + 패킷( ?byte )

#define MMC_MONITOR_TO_MASTER_CLIENT		302					// 클라이언트 코드( 2byte ) + 패킷크기( 2byte ) + 패킷( ?byte )
#define MMC_MONITOR_TO_MASTER_ALLCLIENT		303					// 패킷크기( 2byte ) + 패킷( ?byte )

#define MMC_MASTER_TO_MONITOR				304					// 클라이언트 코드( 2byte ) + 패킷크기( 2byte ) + 패킷( ?byte )

#define MMC_CLIENT_CONNECT					400					// 클라이언트 코드( 2byte )
#define MMC_CLIENT_DISCONNECT				401					// 클라이언트 코드( 2byte )

// 모니터 -> GAME

// 공지사항 

#define MONITOR_QUERY_NOTICE                2000                 // 공지내용 (256byte)
#define MONITOR_QUERY_LOGOUT				2002				 // 플레이어 로그아웃


struct Monitor_Query_Notice
{	
	char szNotice[256];		
};

struct MonitorSmsReq
{
	char szOrgPhone[32];
	char szCallMessage[100];
};



#define MAKESERVERCODE(worldnum,servernum)                      ((worldnum) * 10 + (servernum))

#pragma pack()

#endif


