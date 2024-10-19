//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 5		
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXCONNECTSOCKET_H
#define _JXCONNECTSOCKET_H

#include "JXSocket.h"

class JXConnectSocket : public JXSocket
{
friend unsigned __stdcall JXConnectSocket_ThreadFunc( LPVOID param );
friend unsigned __stdcall JXConnectSocket_Recv_ThreadFunc( LPVOID param );
friend unsigned __stdcall JXConnectSocket_Send_ThreadFunc( LPVOID param );

public:
	JXConnectSocket();
	~JXConnectSocket();

public:
	void			Create( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize, BOOL bConnThreadRun = TRUE );
	void			TryConnect();
	BOOL			Connect();
	BOOL			Connect(const char* pIP, UI16 usPort);

	void			StartThread();
	void			StopThread();

private:
	char			m_pszIP[ 32 ];
	UI16			m_usPort;

private:
	UI32			m_uiThreadID;

	HANDLE			m_hTryConnectEvent;
	HANDLE			m_hExitEvent;

private:
	BOOL			m_bThreadRun;
	UI32			m_uiThread2ID;
	HANDLE			m_hEvent;
};


#endif
