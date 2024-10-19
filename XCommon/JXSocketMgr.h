//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완				
// 최종 수정일 : 2002. 7. 2	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXSOCKETMGR_H
#define _JXSOCKETMGR_H

#include "JXList.h"

class JXSocket;
class JXPacket;
class JXAcceptSocketList;

class JXSocketMgr : public JXList
{
friend unsigned __stdcall JXSocketMgr_Recv_ThreadFunc( LPVOID param );
friend unsigned __stdcall JXSocketMgr_Send_ThreadFunc( LPVOID param );

public:
	JXSocketMgr();
	~JXSocketMgr();
	
public:
	void			Create( SI32 siMaxNumber, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize );

	void			StartThread();
	void			StopThread();

	void			AddSocket( SOCKET socket, SOCKADDR_IN sockaddr, JXPacket *pPacket );
	void			AddSocket( SOCKET socket, SOCKADDR_IN sockaddr );
	void			DelSocket( JXSocket *pSocket );

	void			Send();
	void			Recv();

	void			SendPacket( JXPacket *pPacket );

private:
	inline void		__send();
	inline void		__recv();

public:
	fd_set			m_fdset;
	timeval			m_timeout;

private:
	BOOL			m_bThreadRun;
	UI32			m_uiThreadID;

	HANDLE			m_hStopEvent;
	HANDLE			m_hExitEvent;

private:
	JXSocket*		m_pSocket;

private:
	fd_set			local_readfds;
	fd_set			local_writefds;
	fd_set			local_errfds;
};

#endif
