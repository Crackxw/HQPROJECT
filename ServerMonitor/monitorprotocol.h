#ifndef _MONITOR_PROTOCOL_
#define _MONITOR_PROTOCOL_

#pragma pack(1)

#define MONITOR_COMMAND_NOTICE   0

// ����� Ŭ���̾�Ʈ -> ����� ����

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
	DEAD_REASON_UNKNOWN                    =    0,                       // ������ �𸣰ڴ�.
	DEAD_REASON_DBERROR                    =    1,                       // DB ERROR!!!
	DEAD_REASON_USERMONITORTERMINATE       =    2,                       // ����� ���� �׿���.

} DEAD_REASON;*/

//#define MONITOR_SERVER_PORT                 20010
#define MONITOR_SERVER_PORT                 20001
#define MASTER_SERVER_PORT					8011

#define MMC_QUERY_LOGIN						90					// ID( 32byte ) + pass( 32byte )
#define MMC_REPORT_LOGIN					91					// Result( 1 byte - 0: ����, 1: �Ƶھ���, 2: �н�����Ʋ��, 3:�̹������Ѿ��̵� )

#define MMC_QUERY_ADDUSER					92					// ID( 32byte ) + pass( 32byte ) + name( 32byte )
#define MMC_REPORT_ADDUSER					93					// Result( 1 byte - 0: ����, 1: �ߺ����̵�(����) )

#define MMC_QUERY_DELUSER					94					// ID( 32byte )
#define MMC_REPORT_DELUSER					95					// Result( 1 byte - 0: ����, 1: ���̵����(����) )

#define MMC_QUERY_USERLIST					96					
#define MMC_REPORT_USERLIST					97					// ���� ����( 4byte ) + 
																// ID( 32byte ) + name( 32byte ) + ...

#define MMC_QUERY_CONNECTUSERLIST			98
#define MMC_REPORT_CONNECTUSERLIST			99					// ���� ����( 4byte ) +
																// ID( 32byte ) + name( 32byte ) + ...

#define MMC_REPORT_USER_ENTER				120					// ID( 32byte ) + name( 32byte )
#define MMC_REPORT_USER_EXIT				122					// ID( 32byte ) + name( 32byte )

#define MMC_QUERY_CHAT						110					// ��Ʈ������( 2byte ) + ���ڿ�( ?byte )
#define MMC_REPORT_CHAT						111					// ID( 32byte ) + name( 32byte ) + ��Ʈ������( 2byte ) + ���ڿ�( ?byte )

#define MMC_QUERY_SERVER_STRUCT				100	
#define MMC_QUERY_WORLD_LIST				101
#define MMC_QUERY_SERVER_STATE_ALL			102
#define MMC_QUERY_SERVER_STATE				103

#define MMC_REPORT_SMS						104
#define	MMC_QUERY_SMS						105

// �� ������ ���� ������ ���� ����
#define MMC_REPORT_SERVER_STRUCT			200					// ����� ���� ���( 4byte ) + 
																// �����̸�( 32byte ) + �����̸�( 32byte ) + ...
// ���� ����
#define MMC_REPORT_WORLD_LIST				201					// ���� ����( 4byte ) +
																// ���� �̸�( 32byte ) + �����̸�( 32byte ) + ...
// ���� ����
/*
#define MMC_REPORT_SERVER_STATE_ALL			202					// ���� �ڵ�( 2byte ) + ����( 2byte ) + ...
#define MMC_REPORT_SERVER_STATE				203					// ���� �ڵ�( 2byte ) + ����( 2byte )
*/

#define MMC_CLIENT_TO_SERVER				300					// ���� ���� ����( 2byte ) + �����ڵ�( 2byte ) + ... + ��Ŷũ��( 2byte ) + ��Ŷ( ?byte )
#define MMC_SERVER_TO_CLIENT				301					// ���� �ڵ�( 2byte ) + ��Ŷũ��( 2byte ) + ��Ŷ( ?byte )

#define MMC_MONITOR_TO_MASTER_CLIENT		302					// Ŭ���̾�Ʈ �ڵ�( 2byte ) + ��Ŷũ��( 2byte ) + ��Ŷ( ?byte )
#define MMC_MONITOR_TO_MASTER_ALLCLIENT		303					// ��Ŷũ��( 2byte ) + ��Ŷ( ?byte )

#define MMC_MASTER_TO_MONITOR				304					// Ŭ���̾�Ʈ �ڵ�( 2byte ) + ��Ŷũ��( 2byte ) + ��Ŷ( ?byte )

#define MMC_CLIENT_CONNECT					400					// Ŭ���̾�Ʈ �ڵ�( 2byte )
#define MMC_CLIENT_DISCONNECT				401					// Ŭ���̾�Ʈ �ڵ�( 2byte )

// ����� -> GAME

// �������� 

#define MONITOR_QUERY_NOTICE                2000                 // �������� (256byte)
#define MONITOR_QUERY_LOGOUT				2002				 // �÷��̾� �α׾ƿ�


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


