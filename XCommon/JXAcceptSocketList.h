//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 17
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXACCEPTSOCKETLIST_H
#define _JXACCEPTSOCKETLIST_H

#include "JXList.h"


class JXSocket;
class JXPacket;

class JXAcceptSocketList : public JXList
{
public:
	JXAcceptSocketList();
	virtual ~JXAcceptSocketList();

public:
	void Create( SI32 siMaxNumber, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize );

	void AddSocket( SOCKET socket, SOCKADDR_IN sockaddr, JXPacket *pPacket );
	void AddSocket( SOCKET socket, SOCKADDR_IN sockaddr );

private:
	JXSocket		*m_pSocket;

private:
	JXSocket		*local_pSocket;
};

#endif

