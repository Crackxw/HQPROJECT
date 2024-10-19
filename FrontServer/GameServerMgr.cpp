#include <winsock2.h>
#include <stdio.h>


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CGameServerMgr
#include "../kojimodo/iocp/ListenSocket.h"
#include "../kojimodo/utility/Singleton.h"
#include "GameServerMgr.h"


//
#include "_FrontGameServerDefine.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGameServerMgr::CGameServerMgr(char* ip, int port, int max_socket): 
CListenSocket( ip, port, max_socket )
{		
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGameServerMgr::~CGameServerMgr()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CAcceptSocket* CGameServerMgr::_CallBack_CreateSocket(int index)
{	
	return new CGameServer( index, FRONT_GAMESERVER::MAX_PACKETSIZE, 
								   FRONT_GAMESERVER::MAX_PACKETSIZE, 50000, 50000 );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGameServer* CGameServerMgr::getGameServer(int index)
{
	return (CGameServer*)getSocket(index);
}







