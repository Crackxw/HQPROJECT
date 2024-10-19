#ifndef _XCHATSERVER_H
#define _XCHATSERVER_H

#include "..\\XCommon\\jxdef.h"
#include "..\\XCommon\\JXObject.h"
#include "..\\XCommon\\JXPacket.h"

class JXAcceptSocketList;
class JXGateSocket;
class JXSocketMgr;
class JXConnectSocket;

class XMonitor;

class XPlayerMgr;
class XPlayer;

class XMap;

class XChatServer
{
public:
	XChatServer();
	~XChatServer();

public:
	BOOL Init();
	void Start();
	void Stop();
	void Run();

	void ProcessConsole();

public:
	void ProcPacketPlayer( XPlayer *pPlayer );

	void LogOutPlayer( UI32 uiAccount );
	void LogOutPlayer( XPlayer *pPlayer );

	void ProcPacketMonitor( XMonitor *pMonitor );
	void SendHeartBeat();
	void SendHeartBeatToPlayers();

public:
	// 플레이어
	JXGateSocket		*m_pGateSocketPlayer;
	JXAcceptSocketList	*m_pAcceptListPlayer;
	JXSocketMgr			*m_pConnectListPlayer;

	JXConnectSocket		*m_pMonitorSocket;
	XMonitor			*m_pMonitor;

	XPlayerMgr			*m_pPlayerMgr;

	XPlayer				**m_pPlayerToRemove;
	SI32				m_siNumPlayerToRemove;

	JXObjectPtr			*m_pPlayerPtr;
	JXList				*m_pPlayerPtrList;

	XMap				*m_pMap;

public:
	BOOL				m_bServerRun;


private:
	JXPacket			local_packetA;
	JXPacket			local_packetB;
	JXPacket			local_packetC;
	JXPacket			local_packetD;
	JXPacket			local_packetE;
	char				local_buf[ JXPACKET_MAX_SIZE ];

	UI16				local_usCmd;

};

#endif
