//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "JXConnectSocketMgr.h"
#include "JXConnectSocket.h"
#include <assert.h>
#include <stdexcept>

JXConnectSocketMgr::JXConnectSocketMgr()
{
	m_pConnectSocket = NULL;
}

JXConnectSocketMgr::~JXConnectSocketMgr()
{
	if( m_pConnectSocket ) delete [] m_pConnectSocket;
}

void JXConnectSocketMgr::Create( SI32 siMaxSocketNum )
{
	if( m_pConnectSocket ) delete [] m_pConnectSocket;

	try{	
		m_pConnectSocket = new JXConnectSocket[ siMaxSocketNum ];
	}
	catch(std::bad_alloc){
		assert(0);
	}

	for( SI32 i = 0; i < siMaxSocketNum; ++i ) {

		AddToFree( (JXObject *)&m_pConnectSocket[ i ] );
	}

}

JXConnectSocket* JXConnectSocketMgr::AddSocket( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize )
{
	JXConnectSocket *pSocket = (JXConnectSocket *)PushBack();

	if( pSocket == NULL ) return NULL;

	pSocket->Create( pszIP, usPort, siRecvBufSize, siSendBufSize, siPacketQueueSize );

	return pSocket;

}




