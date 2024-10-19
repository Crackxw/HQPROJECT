#ifndef _MONITORSERVER_H
#define _MONITORSERVER_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\JXPacket.h"

#include "XUser.h"

class JXAcceptSocketList;
class JXGateSocket;
class JXSocketMgr;
class JXConnectSocket;
class JXWorkManager;

class XClientMgr;
class XServerMgr;

class XClient;
class XServer;

class JXPacket;

class MonitorServer
{
public:
	MonitorServer();
	~MonitorServer();

public:
	BOOL Init();
	void Start();
	void Stop();
	void Run();

	void ProcessConsole();
	
	int TurnOnServer();
	void TurnOffServer();

	void ProcPacketClient( XClient *pClient );
	void ProcPacketServer( XServer *pServer );

	void LogOutClient( XClient *pClient );
	void LogOutServer( XServer *pServer );

	int  SendToClient(void *buf,int size);
	int  SendToServer(JXPacket *pPacket);
	

	BOOL IsServerDead();
	

public:
	// 클라이언트
	JXGateSocket		*m_pGateSocketClient;
	JXAcceptSocketList	*m_pAcceptListClient;
	JXSocketMgr			*m_pConnectListClient;
	XClientMgr			*m_pClientMgr;

	// 서버
	JXGateSocket		*m_pGateSocketServer;
	JXAcceptSocketList	*m_pAcceptListServer;
	JXSocketMgr			*m_pConnectListServer;
	XServerMgr			*m_pServerMgr;

	JXPacket			m_LocalPacketA;
	

	
public:

	XClient				**m_pClientToRemove;
	SI32				m_siNumClientToRemove;

	XServer				**m_pServerToRemove;
	SI32				m_siNumServerToRemove;


public:
	BOOL				m_bServerRun;
	BOOL                m_bUpdateServerTime;
	SYSTEMTIME          m_ServerOnTime;


private:
	JXPacket			local_packet;
	UI16				local_usCmd;
	char				local_buf[ 30000 ];

	HANDLE              m_hServerProcess;
	DWORD               m_dwServerProcID;

	DWORD               m_dwRefreshTime; 
	int                 m_siRefreshCount;

    WORD                m_wCurrentUserNum;
	BOOL                m_bSendUserNum;




public:
	XUser				*m_pUser;

};

extern char g_OriginalPath[256];
void	writeInfoToFile(bool timeStamp, char* filename, char* str, ...);

#endif

