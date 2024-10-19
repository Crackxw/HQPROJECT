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
//	Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CUser::CUser(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize): 
CAcceptSocket(completionKey, sizeof(FRONT_USER::sPacketHeader), rbsize, sbsize, rqsize, sqsize)
{
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CUser::~CUser()
{
}




//---------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 송신큐에 헤더를 포함시켜 데이타를 넣는다 
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
//	Desc : 인자로 들어온 data의 체크섬을 구한다  
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
//	Desc : 패킷 헤더에 기록된 체크섬을 얻는다 
//--------------------------------------------------------------------------------------------
int CUser::getChecksum(char* header)
{
	return ((FRONT_USER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷 헤더에 기록된 패킷의 크기를 얻는다 
//--------------------------------------------------------------------------------------------
int CUser::getPacketSize(char* header)
{
	return ((FRONT_USER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 패킷을 처리한다 
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
//	Desc : 주기적으로 처리해야 할 소켓 작업을 수행 한다  
//--------------------------------------------------------------------------------------------
bool CUser::_CallBack_ProcessAcceptSock() 
{	

	/* 30 분간 클라이언트로 부터 받은 패킷이 없다면 연결을 끊는다 */

	if( getPacketElapseTime() >= 1800000 ) return FALSE;
	return TRUE; 					
}








//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 버전 체크 요청을 받은 경우 
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
//	Desc : 버전 체크 요청에 응답 한다 
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
//	Desc : 월드들의 정보를 요청 받은 경우 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestWorldsInfo(char* data)
{
	sendResponseWorldsInfo();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 월드들의 정보를 보내 준다 
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
//	Desc : 월드 선택 요청을 받은 경우 
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
//	Desc : 월드 선택 요청에 응답한다 
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
//	Desc : 공지 사항 요청을 받은 경우 
//--------------------------------------------------------------------------------------------
void CUser::recvRequestNotice(char* data)
{
	sendResponseNotice();
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 공지 사항 요청에 응답 한다 
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


