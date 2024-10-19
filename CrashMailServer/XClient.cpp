#include "XClient.h"
#include "..\\XCommon\\JXSocket.h"
#include "..\\Servermonitor\\MonitorProtocol.h"
#include <direct.h>


XClient::XClient()
{
	m_bReady = FALSE;
	SetSocket( NULL );
}


XClient::~XClient()
{
	
}


JXSocket* XClient::GetSocket() 
{ 
	return m_pSocket; 
}


void XClient::Init()
{

}


void XClient::SetSocket( JXSocket *pSocket )
{ 
	m_pSocket = pSocket; 
}


void XClient::SendPacket( char *pPacket, int len )
{

	UI16 usCmd = MMC_MONITOR_TO_MASTER_ALLCLIENT;
	UI16 usLen = len;

	local_packet.ClearPacket();
	local_packet.WriteValue( &usCmd, 2 );
	local_packet.WriteValue( &usLen, 2 );
	local_packet.WriteValue( pPacket, len );
	local_packet.CompletePacket();

	m_pSocket->SendPacket( &local_packet );
}


