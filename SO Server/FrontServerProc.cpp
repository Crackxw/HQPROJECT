#include <winsock2.h>
#include <stdio.h>

// CFrontServerProc
#include "../kojimodo/utility/Winsock.h"
#include "FrontServerProc.h"


// 
#include "../FrontServer/_FrontGameServerDefine.h"


// etc..
#include "SOMain.h"





//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 생 성 자 
//--------------------------------------------------------------------------------------------
CFrontServerProc::CFrontServerProc():			 
CWinsock(sizeof(FRONT_GAMESERVER::sPacketHeader))
{

}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 소 멸 자 
//--------------------------------------------------------------------------------------------
CFrontServerProc::~CFrontServerProc()
{
	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 프론트 서버에 쓰이는 패킷의 사이즈를 얻어 온다 
//--------------------------------------------------------------------------------------------
int CFrontServerProc::getPacketSize(char* header) 
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 수신 받은 패킷에 들어있는 체크섬을 얻어 온다 
//--------------------------------------------------------------------------------------------
int	CFrontServerProc::getChecksum(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 한 패킷의 체크섬 계산 하여 리턴 한다 
//--------------------------------------------------------------------------------------------
int CFrontServerProc::calcChecksum(char* data, int size) 
{
	char checksum = 0;
	for(int i=0; i < size; i++)
	{
		checksum ^= data[i];
	}

	return checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 프론트 서버와 연결 한다 
//--------------------------------------------------------------------------------------------
bool CFrontServerProc::Connect()
{
	return CWinsock::Connect(g_GameServerInfoParser.GetFrontServerIP(), 20001);
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 프론트 서버에 패킷을 송신 한다 
//--------------------------------------------------------------------------------------------
bool CFrontServerProc::Send(char* data, int dataSize)
{
	FRONT_GAMESERVER::sPacketHeader header;
			
	header.packetSize = HEADER_SIZE + dataSize;		
	header.checksum   = calcChecksum(data, dataSize);

	return CWinsock::Send( (char*)&header, data, dataSize );
}











//--------------------------------------------------------------------------------------------
// Name : 
// Desc : 동시 접속자수를 보고 한다 
//--------------------------------------------------------------------------------------------
bool CFrontServerProc::ReportConcurrent( int world_id, int concurrent )
{
	
	FRONT_GAMESERVER::sReportConcurrent	 repPacket;
	

	repPacket.command	   =    FRONT_GAMESERVER::REPORT_CONCURRENT;
	repPacket.world_id	   =    world_id;
	repPacket.concurrent   =    concurrent;
	
	
	while(TRUE)
	{
		if( Send((char*)&repPacket, sizeof(repPacket)) )
		{		
			return TRUE;
		}


		if( getLastError() == CWinsock::OVER_WAITTIME ) {

			Connect();
			break;
		}
				
		Connect();
	}

	return FALSE;
}