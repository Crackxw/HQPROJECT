#include <winsock2.h>
#include <stdio.h>


// CGameServer
#include "../kojimodo/iocp/AcceptSocket.h"
#include "GameServer.h"


// CWorkMgr
#include "WorkMgr.h"




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CGameServer::CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(AUTH::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{	
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
	return ((AUTH::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ ����� ��ϵ� ��Ŷ�� ũ�⸦ ��´� 
//--------------------------------------------------------------------------------------------
int CGameServer::getPacketSize(char* header)
{
	return ((AUTH::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ�� ó���Ѵ� 
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
//	Desc : ���ڷ� ���� uniqueID�� �� ���Ӽ����� uniqueID�� ���� ���� �˾� ���� 
//--------------------------------------------------------------------------------------------
bool CGameServer::isMatchUniqueID( AUTH::sUniqueID uniqueID )
{
	if( uniqueID.orderKey   == getSockOrderKey()  && 
		uniqueID.arrayIndex == getSockCompletionKey() ) return TRUE;
	
	return FALSE;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� ���Ӽ����� UniqueID�� ��� �´� 
//--------------------------------------------------------------------------------------------
AUTH::sUniqueID	 CGameServer::getUniqueID()
{
	AUTH::sUniqueID  A;

	A.set( getSockOrderKey() , getSockCompletionKey() );

	return A;
}




