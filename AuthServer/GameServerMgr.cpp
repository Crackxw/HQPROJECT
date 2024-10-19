#include <winsock2.h>
#include <stdio.h>


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CGameServerMgr
#include "../kojimodo/iocp/ListenSocket.h"
#include "../kojimodo/utility/Singleton.h"
#include "GameServerMgr.h"


// CWorkMgr
#include "WorkMgr.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자
//--------------------------------------------------------------------------------------------
CGameServerMgr::CGameServerMgr(char* ip, int port, int max_socket): 
CListenSocket( ip, port, max_socket )
{		
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자
//--------------------------------------------------------------------------------------------
CGameServerMgr::~CGameServerMgr()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓 객체 생성의 콜백 함수 
//--------------------------------------------------------------------------------------------
CAcceptSocket* CGameServerMgr::_CallBack_CreateSocket(int index)
{	
	return new CGameServer( index, AUTH::MAX_PACKETSIZE, 
								   AUTH::MAX_PACKETSIZE, 100000, 100000 );
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓의 메인처리 콜백 함수 
//--------------------------------------------------------------------------------------------
void CGameServerMgr::_CallBack_ProcessPoll()
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
CGameServer* CGameServerMgr::getGameServer(int index)
{
	return (CGameServer*)getSocket(index);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc :  
//--------------------------------------------------------------------------------------------
int CGameServerMgr::writeToSendQueue( AUTH::sUniqueID gameServerUniqID,
									  AUTH::sUniqueID userUniqID, 
									  char* data, int dataSize )
{
	EnterCriticalSection(&m_csSocketList);
	
	CGameServer* pGS = getGameServer(gameServerUniqID.arrayIndex);
	
	int retValue = 0;

	if( pGS	&& pGS->isMatchUniqueID(gameServerUniqID) ) {

		retValue = pGS->writeToSendQueue( userUniqID, data, dataSize );
	}

	LeaveCriticalSection(&m_csSocketList);

	return retValue;
}







