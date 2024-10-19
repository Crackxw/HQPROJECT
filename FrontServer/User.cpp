#include <winsock2.h>
#include <stdio.h>


// CUser
#include "../kojimodo/iocp/AcceptSocket.h"
#include "User.h"


// CWorldInfoMgr
#include "../kojimodo/utility/Singleton.h"
#include "WorldInfoMgr.h"


// CGameInfoParser
#include "GameInfoParser.h"


// 
#include "_FrontUserDefine.h"



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CUser::CUser(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(FRONT_USER::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CUser::~CUser()
{
}




//---------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �۽�ť�� ����� ���Խ��� ����Ÿ�� �ִ´� 
//---------------------------------------------------------------------------------------------
int CUser::writeToSendQueue(char* data, int dataSize)
{	

	FRONT_USER::sPacketHeader header;
			
	header.packetSize = HEADER_SIZE + dataSize;		
	header.checksum   = calcChecksum(data, dataSize);


	///////////////////////////////////////////////////////////////////////
	return CAcceptSocket::writeToSendQueue((char*)&header, data, dataSize);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���ڷ� ���� data�� üũ���� ���Ѵ�  
//--------------------------------------------------------------------------------------------
int CUser::calcChecksum(char* data, int size)
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
int CUser::getChecksum(char* header)
{
	return ((FRONT_USER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ ����� ��ϵ� ��Ŷ�� ũ�⸦ ��´� 
//--------------------------------------------------------------------------------------------
int CUser::getPacketSize(char* header)
{
	return ((FRONT_USER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ��Ŷ�� ó���Ѵ� 
//--------------------------------------------------------------------------------------------
void CUser::processPacket(char* header, char *data, int dataSize)
{
	
	using namespace FRONT_USER;


	DWORD command;
	memcpy(&command, data, sizeof(command));
	
	
	switch( command )	
	{
		case REQUEST_CHECK_VERSION:
			{
				printf("[%d] FRONT_USER::REQUEST_CHECK_VERSION\n",getSocket());
				recvRequestCheckVersion(data);
			}
			break;

		case REQUEST_WORLDS_INFO:
			{
				printf("[%d] FRONT_USER::REQUEST_WORLDS_INFO\n",getSocket());
				recvRequestWorldsInfo(data);
			}
			break;

		case REQUEST_SELECT_WORLD:
			{
				printf("[%d] FRONT_USER::REQUEST_SELECT_WORLD\n",getSocket());
				recvRequestSelectWorld(data);
			}
			break;

		case REQUEST_NOTICE:
			{
				printf("[%d] FRONT_USER::REQUEST_NOTICE\n",getSocket());
				recvRequestNotice(data);
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
bool CUser::_CallBack_ProcessAcceptSock() 
{	

	/* 30 �а� Ŭ���̾�Ʈ�� ���� ���� ��Ŷ�� ���ٸ� ������ ���´� */

	if( getPacketElapseTime() >= 1800000 ) return FALSE;
	return TRUE; 					
}








//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� üũ ��û�� ���� ��� 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestCheckVersion(char* data)
{
	using namespace FRONT_USER;
	
	sRequestCheckVersion* packet = (sRequestCheckVersion*)data;


	if( CGameInfoParser::P()->getGameVersion() == 0 ||
		CGameInfoParser::P()->getGameVersion() == packet->version ) {

		sendResponseCheckVersion(OK);
		return;
	}

	sendResponseCheckVersion(INVALID_VERSION);
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� üũ ��û�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CUser::sendResponseCheckVersion(int which)
{
	using namespace FRONT_USER;

	sResponseCheckVersion packet;

	packet.command		 =  RESPONSE_CHECK_VERSION;
	packet.responseKind  =  which;
	
	writeToSendQueue((char*)&packet, sizeof(packet));
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������� ������ ��û ���� ��� 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestWorldsInfo(char* data)
{
	sendResponseWorldsInfo();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������� ������ ���� �ش� 
//--------------------------------------------------------------------------------------------
void CUser::sendResponseWorldsInfo()
{
	using namespace FRONT_USER;

	sResponseWorldsInfo	packet;

	packet.command		= RESPONSE_WORLDS_INFO;
	packet.numWorld		= 0;

	packet.numWorld		= CWorldInfoMgr::P()->getWorldsInfo(&packet.worldsInfo[0]);

	int size = sizeof(sResponseWorldsInfoFront) + (sizeof(packet.worldsInfo[0]) * packet.numWorld);
	
	writeToSendQueue((char*)&packet, size);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���� ��û�� ���� ��� 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestSelectWorld(char* data)
{
	using namespace FRONT_USER;

	sRequestSelectWorld* packet = (sRequestSelectWorld*)data;

	
	if( CWorldInfoMgr::P()->isValidWorld( packet->world_id ) == FALSE ) {

		sendResponseSelectWorld( 0 );	
		return;
	}

	
	sendResponseSelectWorld( packet->world_id );
}	




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���� ��û�� �����Ѵ� 
//--------------------------------------------------------------------------------------------
void CUser::sendResponseSelectWorld(int world_id)
{
	using namespace FRONT_USER;

	sResponseSelectWorld  packet;

	packet.command		  = RESPONSE_SELECT_WORLD;
	packet.world_id		  = world_id;


	if( world_id == 0 ||
		CWorldInfoMgr::P()->getChatIP   (world_id, packet.chatIP) == FALSE ||
		CWorldInfoMgr::P()->getChatPort (world_id, &packet.chatPort) == FALSE ||
		CWorldInfoMgr::P()->getGateIP   (world_id, packet.gateIP) == FALSE ||
		CWorldInfoMgr::P()->getGatePort (world_id, &packet.gatePort) == FALSE )
		
	{		
		writeToSendQueue((char*)&packet, sizeof(sResponseSelectWorldFront));
		return;
	}

	writeToSendQueue((char*)&packet, sizeof(packet));
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���� ��û�� ���� ��� 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestNotice(char* data)
{
	sendResponseNotice();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ���� ���� ��û�� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
void CUser::sendResponseNotice()
{
	using namespace FRONT_USER;

	sResponseNotice packet;

	packet.command	= RESPONSE_NOTICE;
	
	memset(packet.notice, 0, sizeof(packet.notice));
	strcpy(packet.notice, CGameInfoParser::P()->getNotice());

	writeToSendQueue((char*)&packet, sizeof(sResponseNoticeFront) + strlen(packet.notice) + 1);	
}


