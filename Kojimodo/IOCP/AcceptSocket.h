#ifndef __CACCEPTSOCKET__
#define __CACCEPTSOCKET__



struct sOverlappedEX {

	OVERLAPPED			overlapped;								// ���� ������ ����ü 
	char				transferMode;							// ���� ���(0-->Recv, 1-->Send)									
};



//������������������������������������������������������������������������������������������������������������������������
//
// class :  
//
//������������������������������������������������������������������������������������������������������������������������
class CAcceptSocket
{
private:	
	const DWORD			m_sockCompletionKey;					// ������ �Ϸ� Ű 
	DWORD				m_sockOrderKey;							// ������ ���� Ű 
	
	bool				m_bSockOpen;							// ������ �����ִ��� ���� 
	bool				m_bSockClosing;							// ������ ������ ������ ���� 
	
	DWORD				m_packetRecvedTime;						// ���� �ֱ� ��Ŷ�� ���� ���� �ð� 
	
	
	SOCKET				m_socket;								// ���� 
	SOCKADDR_IN			m_sockAddrIn;							// ���� �ּ� ����ü 
	char				m_sockIP[16];							// ������ IP

	sOverlappedEX		m_recvOverlapped;						// ���� ������ ����ü 
	WSABUF				m_wsaRecvBuffer;						// ���� ���� ���� 
	
	sOverlappedEX		m_sendOverlapped;						// �۽� ������ ����ü 
	WSABUF				m_wsaSendBuffer;						// ���� �۽� ���� 

	char*				m_recvQueue;							// ���� ť 
	int					m_recvQueuePos;							// ���� ť�� ���� ��ġ 

	char*				m_sendQueue;							// �۽� ť 
	int					m_sendQueuePos;							// �۽� ť�� ���� ��ġ 

	CRITICAL_SECTION	m_csRecv;								// ���� ���� ũ��Ƽ�� ���� 
	CRITICAL_SECTION	m_csSend;								// �۽� ���� ũ��Ƽ�� ���� 

	char*				m_recvBuffer;							// ���� ���� 
	char*				m_sendBuffer;							// �۽� ���� 
	


protected:
	const int			MAX_RECVBUFFER;							// ���� ���� �ִ� ũ�� 
	const int			MAX_SENDBUFFER;							// �۽� ���� �ִ� ũ�� 

	const int			MAX_RECVQUEUE;							// ���� ť �ִ� ũ�� 
	const int			MAX_SENDQUEUE;							// �۽� ť �ִ� ũ�� 

	const int			HEADER_SIZE;							// ��Ŷ�� ��� ũ�� 
	
	


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