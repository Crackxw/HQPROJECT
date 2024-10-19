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
// Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CFrontServerProc::CFrontServerProc():			 
CWinsock(sizeof(FRONT_GAMESERVER::sPacketHeader))
{

}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CFrontServerProc::~CFrontServerProc()
{
	
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ����Ʈ ������ ���̴� ��Ŷ�� ����� ��� �´� 
//--------------------------------------------------------------------------------------------
int CFrontServerProc::getPacketSize(char* header) 
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->packetSize;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ���� ���� ��Ŷ�� ����ִ� üũ���� ��� �´� 
//--------------------------------------------------------------------------------------------
int	CFrontServerProc::getChecksum(char* header)
{
	return ((FRONT_GAMESERVER::sPacketHeader*)header)->checksum;
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : �� ��Ŷ�� üũ�� ��� �Ͽ� ���� �Ѵ� 
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
// Desc : ����Ʈ ������ ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
bool CFrontServerProc::Connect()
{
	return CWinsock::Connect(g_GameServerInfoParser.GetFrontServerIP(), 20001);
}




//--------------------------------------------------------------------------------------------
// Name : 
// Desc : ����Ʈ ������ ��Ŷ�� �۽� �Ѵ� 
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
// Desc : ���� �����ڼ��� ���� �Ѵ� 
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