#include "XServer.h"
#include "..\\XCommon\\JXSocket.h"

XServer::XServer()
{
	SetSocket( NULL );
}

XServer::~XServer()
{
 
}

JXSocket* XServer::GetSocket() 
{ 
	return m_pSocket; 
}

void XServer::Init()
{

}

void XServer::SetSocket( JXSocket *pSocket )
{ 
	m_pSocket = pSocket; 
}
