//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "XGateServer.h"

#include "..\\XCommon\\JXConnectSocket.h"

XGateServer::XGateServer()
{
	m_pSocket = NULL;
	m_usServerID = 0;


	m_bRDisconnect = FALSE;
}

XGateServer::~XGateServer()
{
}

void XGateServer::SendPacket( JXPacket *pPacket )
{
	if( m_pSocket == NULL ) return;

	m_pSocket->SendPacket( pPacket );
}


