// GersangServerMonitor.h: interface for the CGersangServerPatch class.
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



#include "..\ServerMonitor\MySocket.h"

class CPatchToolDlg;
class CMyPacket;

enum process_result
{
	PROCESS_NOPROBLEM = 0,
	PROCESS_CONNECTION_FAILED,
	PROCESS_CONNECTION_SUCCEEDED,
	PROCESS_CONNECTION_LOST
	

};



class CGersangServerPatch  : public CMySocket
{
	friend class CPatchToolDlg;	
public:
	
	int SendFile
		(
			int servernum, 
			WORD *serverlist,
			BYTE newversion,
			CString &destpath,
			CString &filename,
			CString &filenamenopath
		);
	int SendPacket(int servernum, WORD *serverlist,CMyPacket *pPacket);
	
	int GetFileTransferStatus(char *szBuf);

	int Process();

	CGersangServerPatch();
	virtual ~CGersangServerPatch();

	int Init();

//	int ReadServerList(const char *filename);

	TCHAR *GetServerUsage(int num);
	int GetServerGroupNum() {return m_siServerGroupNum;}; 
	int GetServerPerGroup() {return m_siServerPerGroup;};

	int ConnectToServer();

	BOOL IsConnected() {return m_bIsConnected;};

private:
	int m_siServerGroupNum;
	int m_siServerPerGroup;	
public :
	char m_szServerName[MAX_SERVER][32];

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
};

extern CGersangServerPatch g_GersangServerPatch;

int GetQuotedString(char *szSrc,char *szDest);

#endif // !defined(AFX_GERSANGSERVERMONITOR_H__143C115B_8F57_461F_BA35_BFE077AEEDF5__INCLUDED_)
