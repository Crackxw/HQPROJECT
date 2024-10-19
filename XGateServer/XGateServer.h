//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7		
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _XGATESERVER_H
#define _XGATESERVER_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\JXPacket.h"

class JXAcceptSocketList;
class JXGateSocket;
class JXSocketMgr;
class JXConnectSocket;
class JXWorkManager;

class XMonitor;

class XPlayerMgr;
class XPlayer;

class XGameServerMgr;
class XGameServer;
class XControlServer;

class XDBProc;

class XGateServer
{
public:
	XGateServer();
	~XGateServer();

public:
	BOOL Init();
	void Start();
	void Stop();
	void Run();

	void ProcessConsole();

public:
	BOOL ProcPacketGameServer( XGameServer *pGameServer );
	void ProcPacketPlayer( XPlayer *pPlayer );
	BOOL ProcPacketControlServer( XControlServer *pControlServer );

	void LogOutGameServer( XGameServer *pGameServer );
	void LogOutPlayer( UI32 uiLocalID );
	void LogOutPlayer( UI16 usAccount );
	void LogOutPlayer( XPlayer *pPlayer );

	void SendLogOut( UI16 usAccount );

	void ProcPacketMonitor( XMonitor *pMonitor );
	void SendHeartBeat();

public:
	// 플레이어
	JXGateSocket		*m_pGateSocketPlayer;
	JXAcceptSocketList	*m_pAcceptListPlayer;
	JXSocketMgr			*m_pConnectListPlayer;

	XPlayerMgr			*m_pPlayerMgr;

	JXConnectSocket		*m_pGameServerSocket;
	XGameServer			*m_pGameServer;


	// 컨트롤 서버 접속 소켓 클래스
	JXConnectSocket		*m_pControlServerSocket;
	XControlServer		*m_pControlServer;

	// DB..
	XDBProc				*m_pDBProc;
	JXWorkManager		*m_pWorkManager;

	XPlayer				*m_pPlayerOnAccount[ 10000 ];

	XPlayer				**m_pPlayerToRemove;
	SI32				m_siNumPlayerToRemove;


	// 모니터 서버
	JXConnectSocket		*m_pMonitorSocket;
	XMonitor			*m_pMonitor;

public:
	BOOL				m_bServerRun;

private:
	JXPacket			local_packetA;
	JXPacket			local_packetB;
	JXPacket			local_packetD;
	JXPacket			local_packetE;
	char				local_buf[ JXPACKET_MAX_SIZE ];

	UI32				local_uiCmd;
	UI32				local_uiID;
	UI16				local_usAccount;
	SI32				local_siPacketSize;
	SI32				local_siSendNum;
	UI16				local_usAccounts[ 5000 ];

};

#endif
