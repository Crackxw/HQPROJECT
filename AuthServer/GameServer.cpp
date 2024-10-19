#include <winsock2.h>
#include <stdio.h>


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CWorkMgr
#include "WorkMgr.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CGameServer::CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(AUTH::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{	
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
int CGameServer::writeToSendQueue(AUTH::sUniqueID userUniqID, char* data, int dataSize)
{	
	
	AUTH::sPacketHeader header;
			
	header.packetSize  = HEADER_SIZE + dataSize;		
	header.checksum    = calcChecksum(data, dataSize);
	header.userUniqID  = userUniqID;



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
	return ((AUTH::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷 헤더에 기록된 패킷의 크기를 얻는다 
//--------------------------------------------------------------------------------------------
int CGameServer::getPacketSize(char* header)
{
	return ((AUTH::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷을 처리한다 
//--------------------------------------------------------------------------------------------
void CGameServer::processPacket(char* header, char* data, int dataSize)
{
	
	DWORD command;
	memcpy(&command, data, sizeof(command));
	

		
	switch( command )	
	{
		case AUTH::REQUEST_RESET:
			{
				CAccountDBWorkMgr::P()->setQuery( getUniqueID(),												 
								 ((AUTH::sPacketHeader*)header)->userUniqID, 
								 data, dataSize );
			}
			break;

		default:
			{

				CAuthWorkMgr::P()->setQuery( getUniqueID(),												 
								 ((AUTH::sPacketHeader*)header)->userUniqID, 
								 data, dataSize );
			}
			break;
	}

}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//--------------------------------------------------------------------------------------------
bool CGameServer::_CallBack_ProcessAcceptSock() 
{ 
	if( getPacketElapseTime() >= 1800000 ) return FALSE;

	return TRUE; 
}


	





//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 인자로 들어온 uniqueID가 이 게임서버의 uniqueID와 동일 한지 알아 본다 
//--------------------------------------------------------------------------------------------
bool CGameServer::isMatchUniqueID( AUTH::sUniqueID uniqueID )
{
	if( uniqueID.orderKey   == getSockOrderKey()  && 
		uniqueID.arrayIndex == getSockCompletionKey() ) return TRUE;
	
	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 이 게임서버의 UniqueID를 얻어 온다 
//--------------------------------------------------------------------------------------------
AUTH::sUniqueID	 CGameServer::getUniqueID()
{
	AUTH::sUniqueID  A;

	A.set( getSockOrderKey() , getSockCompletionKey() );

	return A;
}




