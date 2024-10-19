//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 6. 20	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXSOCKET_H
#define _JXSOCKET_H

#include "jxdef.h"
#include "JXObject.h"
#include "JXPacket.h"

class JXPacketQueue;

class JXSocket : public JXObject
{
public:	
	JXSocket();
	virtual ~JXSocket();

public:
	void			CreateSocket( SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize ) 
					{ Create( siRecvBufSize, siSendBufSize, siPacketQueueSize ); };
	void			Create( SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize );	

	void			SetSocket( SOCKET socket, SOCKADDR_IN sockaddr );	
	SOCKET 			GetSocket() { return m_socket; };
	
	void			IsConnect( BOOL *pbRet );
	
	JXPacketQueue*	GetQueue() { return m_pPacketQueue; };

	void			Recv();	
	void			Send();	

	void			NBRecv();	
	void			NBSend();	

	void			SendPacket( JXPacket *pPacket );
	void			SendPacket( char *pPacket, int len );

	void			Disconnect();

	SI32			GetRecvDataSize(){ return m_siRecvDataLen; };
	SI32			GetSendDataSize(){ return m_siSendDataLen; };

protected:
	// 버퍼 초기화
	void			__clearSendBuffer();
	void			__clearRecvBuffer();

	void			__disconnect();

	void			__send();
	void			__recv();
	

public:
	SOCKET			m_socket;
	SOCKADDR_IN		m_sockaddr;

	BOOL			m_bConnect;

	DWORD			m_dwLastTickCount;
	BOOL			m_bOdering;

protected:
	// Recv 함수 관련..	
	char			*m_pRecvBuf;
	SI32			m_siRecvBufSize;

	BOOL			m_bRecvPacketSize;
	SI32			m_siRecvDataLen;
	UI16			m_usRecvBytesToRead;

	// Send 함수 관련..
	char			*m_pSendBuf;
	SI32			m_siSendBufSize;

	SI32			m_siSendStartPos;						// 데이터 시작 지점	
	SI32			m_siSendDataLen;						// 데이터의 길이	

	// PacketQueue
	JXPacketQueue	*m_pPacketQueue;

protected:
	CRITICAL_SECTION	m_cs;	

private:
	DWORD			local_dwError;

};

#endif
