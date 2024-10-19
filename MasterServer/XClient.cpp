#include "XClient.h"
#include "..\\XCommon\\JXSocket.h"
#include "..\\Servermonitor\\MonitorProtocol.h"

XClient::XClient()
{
	m_bLoginOK = FALSE;
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
	m_bLoginOK = FALSE;
}


void XClient::SetSocket( JXSocket *pSocket )
{ 
	m_bLoginOK = FALSE;
	m_pSocket = pSocket; 
}
