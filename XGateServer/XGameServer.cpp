//������������������������������������������������������������������������������
// �ۼ���      : ������			
// ���� ������ : 2002. 7. 7
//������������������������������������������������������������������������������

#include "..\\XCommon\\JXSocket.h"

#include "XGameServer.h"

XGameServer::XGameServer()
{
	m_pSocket = NULL;
}

XGameServer::~XGameServer()
{

}

void XGameServer::SendPacket( JXPacket *pPacket )
{
	m_pSocket->SendPacket( pPacket );
}
