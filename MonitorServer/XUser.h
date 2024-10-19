#ifndef __XUSER_H
#define __XUSER_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\jxpacket.h"
#include "unzip.h"


struct Monitor_Command_FileTransferStart;
struct Patch_Command_FileTransferStart;

class XClient;
class MonitorServer;

class XUser
{
public:
	XUser();
	~XUser();

public:
	void Process( char *buf, int len, XClient *pClient ,MonitorServer *pMonitor);
	void Connect(XClient *pClient ,MonitorServer *pMonitor);
	void Disconnect();

	void            Init();
	
	void            FileTransferStart(Monitor_Command_FileTransferStart *);
	void			PatchFileTransferStart(Patch_Command_FileTransferStart *pMcf);
	void            FileTransferStop();
	void            WriteFile(char *buf,int length,XClient *pClient);
	void            Process();
	void            Destroy();


	
	
	
	void SendPacket( char *pPacket, int len, XClient *pClient );

private:
	JXPacket		local_packet;

public:	
	UI16	m_usercode;
	BOOL	m_bConnect;

	BOOL m_bFileTransferMode;
	FILE *m_fp;
	int  m_siNowFileSize;
	int  m_siMaxFileSize;
	char m_szFileName[256];
	char m_szDirectory[256];
	

};

#endif