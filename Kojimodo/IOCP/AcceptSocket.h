#ifndef __CACCEPTSOCKET__
#define __CACCEPTSOCKET__



struct sOverlappedEX {

	OVERLAPPED			overlapped;								// 원본 오버랩 구조체 
	char				transferMode;							// 전송 모드(0-->Recv, 1-->Send)									
};



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class :  
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CAcceptSocket
{
private:	
	const DWORD			m_sockCompletionKey;					// 소켓의 완료 키 
	DWORD				m_sockOrderKey;							// 소켓의 순서 키 
	
	bool				m_bSockOpen;							// 소켓이 열려있는지 여부 
	bool				m_bSockClosing;							// 소켓이 닫히는 중인지 여부 
	
	DWORD				m_packetRecvedTime;						// 가장 최근 패킷을 수신 받은 시각 
	
	
	SOCKET				m_socket;								// 소켓 
	SOCKADDR_IN			m_sockAddrIn;							// 소켓 주소 구조체 
	char				m_sockIP[16];							// 소켓의 IP

	sOverlappedEX		m_recvOverlapped;						// 수신 오버랩 구조체 
	WSABUF				m_wsaRecvBuffer;						// 원속 수신 버퍼 
	
	sOverlappedEX		m_sendOverlapped;						// 송신 오버랩 구조체 
	WSABUF				m_wsaSendBuffer;						// 윈속 송신 버퍼 

	char*				m_recvQueue;							// 수신 큐 
	int					m_recvQueuePos;							// 수신 큐의 현재 위치 

	char*				m_sendQueue;							// 송신 큐 
	int					m_sendQueuePos;							// 송신 큐의 현재 위치 

	CRITICAL_SECTION	m_csRecv;								// 수신 관리 크리티컬 섹션 
	CRITICAL_SECTION	m_csSend;								// 송신 관리 크리티컬 섹션 

	char*				m_recvBuffer;							// 수신 버퍼 
	char*				m_sendBuffer;							// 송신 버퍼 
	


protected:
	const int			MAX_RECVBUFFER;							// 수신 버퍼 최대 크기 
	const int			MAX_SENDBUFFER;							// 송신 버퍼 최대 크기 

	const int			MAX_RECVQUEUE;							// 수신 큐 최대 크기 
	const int			MAX_SENDQUEUE;							// 송신 큐 최대 크기 

	const int			HEADER_SIZE;							// 패킷의 헤더 크기 
	
	


public:
	CAcceptSocket( DWORD completion_key,
				   int header_size,
				   int recvbuffer_size, 
		           int sendbuffer_size, 
				   int recvqueue_size,
				   int sendqueue_size );

	virtual ~CAcceptSocket();



	void				SockOpen (SOCKET socket, SOCKADDR_IN* sockAddrIn, DWORD orderKey);
	void				SockClose();
	void				SockClosing();
	

	//--------------------------------------------------------------------------------
	SOCKET				getSocket();		
	const SOCKADDR_IN*	getSockAddrIn() const;
	const char*			getSockIP() const;
	DWORD				getSockCompletionKey();
	DWORD				getSockOrderKey();	
	DWORD				getPacketElapseTime();

	
	bool				isSockOpen();
	bool				isSockClosing();

	int					Send();	
	int					Recv(int numberOfBytesTransferred);

	int					processAcceptSock();
	

protected:	

	
	virtual void		_CallBack_SockOpen()  = 0;
	virtual void		_CallBack_SockClose() = 0;
	virtual bool		_CallBack_ProcessAcceptSock() = 0;


	//--------------------------------------------------------------------------------	
	virtual void		processPacket(char* header, char* data, int dataSize) = 0;		
	
	virtual int			getPacketSize(char* header) = 0;	
	virtual int			getChecksum(char* header) = 0;	
	virtual int			calcChecksum(char* data, int size) = 0;

	int					writeToSendQueue(char* header, char* data, int dataSize);



private:	
											
	//--------------------------------------------------------------------------------
	int					writeToRecvQueue(char* data, int size);
	bool				processRecvQueue();	


	//--------------------------------------------------------------------------------
	void				setSocket(SOCKET socket);
	void				setSockAddrIn(SOCKADDR_IN* sockAddrIn);
	void				setSockOrderKey(DWORD orderKey);
	void				setSockOpen(bool which);
	void				setSockClosing(bool which);
	void				setPacketRecvedTime(DWORD recvedTime);


	//--------------------------------------------------------------------------------
	void			    initAcceptSocket();	
	void				initializeSocketInfo();
	void				initializeRSBuffer();									
	void				initializeRSQueue();	
	void				initializeRSOverlapped();

};

#endif 