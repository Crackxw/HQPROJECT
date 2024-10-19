// GersangServerMonitor.h: interface for the CGersangServerMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GERSANGSERVERMONITOR_H__143C115B_8F57_461F_BA35_BFE077AEEDF5__INCLUDED_)
#define AFX_GERSANGSERVERMONITOR_H__143C115B_8F57_461F_BA35_BFE077AEEDF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SERVER  256

#define READSERVERLIST_NORMAL           0
#define READSERVERLIST_SERVERINFO       1
#define READSERVERLIST_STRUCTURE        2
#define READSERVERLIST_COMMON           3

#define MAX_SMS_BUFFER					512



#include "MySocket.h"
#include "MonitorProtocol.h"

class CServerMonitorDlg;
class CGersangServerStatus;
class CMyPacket;

enum process_result
{
	PROCESS_NOPROBLEM = 0,
	PROCESS_CONNECTION_FAILED,
	PROCESS_CONNECTION_SUCCEEDED,
	PROCESS_CONNECTION_LOST
	

};



class CGersangServerMonitor  : public CMySocket
{
	friend CServerMonitorDlg;
public:
	
	int SendFile(int servernum, WORD *serverlist,CString &filename,CString &filenamenopath);
	int SendPacket(int servernum, WORD *serverlist,CMyPacket *pPacket);
	int SendNotice(char *szNotice);
	int SendNotice(int servernum, WORD *serverlist,char *szNotice);
	int SendLogout();
	int SendLogout(int servernum, WORD *serverlist);
	int SendChat(TCHAR *szChat);



	int GetFileTransferStatus(char *szBuf);

	int Process();

	CGersangServerMonitor();
	virtual ~CGersangServerMonitor();

	int Init();

//	int ReadServerList(const char *filename);

	TCHAR *GetServerUsage(int num);
	int GetServerGroupNum() {return m_siServerGroupNum;}; 
	int GetServerPerGroup() {return m_siServerPerGroup;};

	const TCHAR *GetServerName(int num);
	int GetServerState(int num,int serverkind);
	int GetServerInfo(int num,TCHAR *szBuf);

	int ConnectToServer();

	BOOL IsConnected() {return m_bIsConnected;};

	//sms 包访.
//	void		   PushSmsData(char* szCallMsg,char* szOrgPhone);
//	MonitorSmsReq* PopSmsData();


	

private:
	int m_siServerGroupNum;
	int m_siServerPerGroup;
	

public:
	CGersangServerStatus *m_pServer;
private:
	TCHAR **m_ppszServerUsage;

	// 颇老傈价
	BOOL m_bFileTransfer;
	CString m_szTransferFileName;
	CString m_szTransferFileNameNoPath;
	FILE *m_fp;
	int m_siSendFileByte;
	int m_siSendFileByteTotal;
	WORD m_wFileSendServerNum;
	WORD m_pwFileSendServerList[256];
	

	// 立加吝?
	BOOL m_bIsConnected;

	TCHAR m_szID[256];
	TCHAR m_szPassword[256];

	//sms 包访.
	MonitorSmsReq	m_stMonitorSms[MAX_SMS_BUFFER];
	int				m_siPopPos;
	int				m_siPushPos;
	int				m_siCount;

	
};

extern CGersangServerMonitor g_GersangServerMonitor;

int GetQuotedString(char *szSrc,char *szDest);

#endif // !defined(AFX_GERSANGSERVERMONITOR_H__143C115B_8F57_461F_BA35_BFE077AEEDF5__INCLUDED_)
