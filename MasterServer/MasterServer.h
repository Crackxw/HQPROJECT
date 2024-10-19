#ifndef _MASTERSERVER_H
#define _MASTERSERVER_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\JXPacket.h"
#include "..\SCCommon\JOGersangConfig.h"
// #include "SMS Server.h"

class JXAcceptSocketList;
class JXGateSocket;
class JXSocketMgr;
class JXConnectSocket;
class JXWorkManager;

class XClientMgr;
class XServerMgr;

class XClient;
class XServer;

struct XUser {
	char id[ 32 ];
	char name[ 32 ];
	char password[ 32 ];
	char level;
};


class MasterServer : public CJOGersangConfig
{
public:
	MasterServer();
	~MasterServer();

public:
	BOOL Init();
	void Start();
	void Stop();
	void Run();

	void ProcessConsole();

	void ProcPacketClient( XClient *pClient );
	void ProcPacketServer( XServer *pServer );

	void LogOutClient( XClient *pClient );
	void LogOutServer( XServer *pServer );

	BOOL LoadFile();
	BOOL LoadConfigFile();


	void	ReadUserDB();
	void	WriteUserDB();

	void    GetIP(SOCKET s, char *szIP);

	int     FindServerIndex(SOCKET s);
public:
	// 클라이언트
	JXGateSocket		*m_pGateSocketClient;
	JXAcceptSocketList	*m_pAcceptListClient;
	JXSocketMgr			*m_pConnectListClient;
	XClientMgr			*m_pClientMgr;

	// 서버
	JXConnectSocket		*m_pConnectServerSocket;

	JXGateSocket		*m_pGateSocketServer;
	JXAcceptSocketList	*m_pAcceptListServer;
	JXSocketMgr			*m_pConnectListServer;
	XServerMgr			*m_pServerMgr;
	
//	CSMSServer			*m_pSMSServer;
	
public:
	XClient				**m_pClientToRemove;
	SI32				m_siNumClientToRemove;

	XServer				**m_pServerToRemove;
	SI32				m_siNumServerToRemove;

public:
	BOOL				m_bServerRun;

private:
	JXPacket			local_packet;
	JXPacket			local_packetB;
	UI16				local_usCmd;
	char				local_buf[ 30000 ];

public:
	SI32				m_siServerGroupNum;
	SI32				m_siServerNum;

	char				m_szServerIP[ 50 ][ 10 ][ 20 ];
	char				m_szServerGroupName[ 50 ][ 32 ];
	char				m_szServerName[ 10 ][ 32 ];
	char				m_szServerType[ 10 ][ 32 ];

	XUser				m_user[ 200 ];	
	SI32				m_usernum;

	DWORD				m_tickServer;
	DWORD               m_dwRefreshDB;

};

#endif
