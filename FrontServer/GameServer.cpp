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
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CGameServer::CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(FRONT_GAMESERVER::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{	
	initGameServer();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CGameServer::~CGameServer()
{
}




//---------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �۽�ť�� ����� ���Խ��� ����Ÿ�� �ִ´� 
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
//	Desc : ���ڷ� ���� data�� üũ���� ���Ѵ�  
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
//	Desc : ��Ŷ ����� ��ϵ� üũ���� ��´� 
//--------------------------------------------------------------------------------------------
int CGameServer::getChecksum(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ ����� ��ϵ� ��Ŷ�� ũ�⸦ ��´� 
//--------------------------------------------------------------------------------------------
int CGameServer::getPacketSize(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ�� ó���Ѵ� 
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
//	Desc : �ֱ������� ó���ؾ� �� ���� �۾��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CGameServer::_CallBack_ProcessAcceptSock() 
{ 

	/* 5 �а� ���Ӽ����� ���� ���� ��Ŷ�� ���ٸ� ������ ���´� */

	if( getPacketElapseTime() >= 300000 ) return FALSE;
	return TRUE; 
};




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ ������ �ؾ��� �۾��� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CGameServer::_CallBack_SockClose()
{	
	
	/* STATE: NO_HEARTBEAT */

	CWorldInfoMgr::P()->setWorldState (m_BelongWorldID, FRONT::NO_HEARTBEAT);

	initGameServer();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���������ڼ� �˸��� �޾��� ��� 
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
//	Desc : CGameServer ��ü�� �ʱ�ȭ �Ѵ� 
//--------------------------------------------------------------------------------------------
void CGameServer::initGameServer()
{
	m_BelongWorldID = 0;
}

