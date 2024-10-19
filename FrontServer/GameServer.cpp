#include <winsock2.h>
#include <stdio.h>


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CWorldInfoParser
#include "../kojimodo/utility/Singleton.h"
#include "WorldInfoMgr.h"


// 
#include "_FrontGameServerDefine.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CGameServer::CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(FRONT_GAMESERVER::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{	
	initGameServer();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CGameServer::~CGameServer()
{
}




//---------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 송신큐에 헤더를 포함시켜 데이타를 넣는다 
//---------------------------------------------------------------------------------------------
int CGameServer::writeToSendQueue(char* data, int dataSize)
{	
	
	FRONT_GAMESERVER::sPacketHeader header;
			
	header.packetSize = HEADER_SIZE + dataSize;		
	header.checksum   = calcChecksum(data, dataSize);



	///////////////////////////////////////////////////////////////////////
	return CAcceptSocket::writeToSendQueue((char*)&header, data, dataSize);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 인자로 들어온 data의 체크섬을 구한다  
//--------------------------------------------------------------------------------------------
int CGameServer::calcChecksum(char* data, int size)
{
	char checksum = 0;
	for(int i=0; i < size; i++)
	{
		checksum ^= data[i];
	}

	return checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷 헤더에 기록된 체크섬을 얻는다 
//--------------------------------------------------------------------------------------------
int CGameServer::getChecksum(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷 헤더에 기록된 패킷의 크기를 얻는다 
//--------------------------------------------------------------------------------------------
int CGameServer::getPacketSize(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷을 처리한다 
//--------------------------------------------------------------------------------------------
void CGameServer::processPacket(char* header, char *data, int dataSize)
{

	using namespace FRONT_GAMESERVER;


	DWORD command;
	memcpy(&command, data, sizeof(command));
	

	switch( command )	
	{

		case REPORT_CONCURRENT:
			{
				printf("[%d] FRONT_GAMESERVER::REPORT_CONCURRENT\n",getSocket());
				recvReportConcurrent(data);
			}
			break;

		default:
			break;
	}		
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 주기적으로 처리해야 할 소켓 작업을 수행 한다 
//--------------------------------------------------------------------------------------------
bool CGameServer::_CallBack_ProcessAcceptSock() 
{ 

	/* 5 분간 게임서버로 부터 받은 패킷이 없다면 연결을 끊는다 */

	if( getPacketElapseTime() >= 300000 ) return FALSE;
	return TRUE; 
};




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소켓이 닫힐때 해야할 작업을 수행 한다 
//--------------------------------------------------------------------------------------------
void CGameServer::_CallBack_SockClose()
{	
	
	/* STATE: NO_HEARTBEAT */

	CWorldInfoMgr::P()->setWorldState (m_BelongWorldID, FRONT::NO_HEARTBEAT);

	initGameServer();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 동시접속자수 알림을 받았을 경우 
//--------------------------------------------------------------------------------------------
void CGameServer::recvReportConcurrent(char* data)
{
	using namespace FRONT_GAMESERVER;

	sReportConcurrent* packet = (sReportConcurrent*)data;
		

	if( CWorldInfoMgr::P()->setConcurrent (packet->world_id, packet->concurrent) ) {


		/* STATE: HEARTBEAT_OK */

		if( CWorldInfoMgr::P()->setWorldState (packet->world_id, FRONT::OK) ) {


			m_BelongWorldID = packet->world_id;
		}
	}
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : CGameServer 객체를 초기화 한다 
//--------------------------------------------------------------------------------------------
void CGameServer::initGameServer()
{
	m_BelongWorldID = 0;
}

