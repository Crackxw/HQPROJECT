#ifndef __CLISTENSOCKET__
#define __CLISTENSOCKET__



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CListenSocket
{	

private:	
	const int			MAX_SOCKET;								// 소켓의 최대 수 
	int					m_numSocket;							// 소켓의 현재 수 
	
	
	HANDLE				m_hCompletionPort;						// 소켓의 완료 포트 	
	DWORD				m_sockOrderSeed;					    // 소켓의 순서 배정 기준값 		

	
	CAcceptSocket**		m_SocketList;							// 소켓의 전체 리스트 	


	SOCKET				m_ListenSocket;							// 접속 대기 소켓 
	WSAEVENT			m_wsaConnectEvent[2];					// 소켓들의 접속 처리 이벤트	

	bool				m_bShutdown;							// 사용중인 쓰레드의 종료 여부 
		

protected:
	// warning when used thread or child class			
	CRITICAL_SECTION	m_csSocketList;							// 소켓의 리스트 관리 크리티컬 섹션		
					


public:	
	CListenSocket( char *ip, int port, int max_socket );
	virtual	~CListenSocket();



	void				Shutdown();	
	
	void				ConnectPoll();
	void				IOCompletionPoll();
	void				ProcessPoll();

	
	void				CreateSocketList();
	void				ReleaseSocketList();


protected:		
	

	//--------------------------------------------------------------------------------	
	virtual CAcceptSocket*			_CallBack_CreateSocket(int index)  = 0;										
	virtual void					_CallBack_ProcessPoll()			   = 0;
		

	CAcceptSocket*		getSocket(int index);	// warning when used thread	or child class		
	int					getNumSocket();
	
						
private:	


	//--------------------------------------------------------------------------------		
	int					addSocket(SOCKET socket, SOCKADDR_IN *sockAddrIn);
	void				removeSocket(int index);
	void				closingSocket(int index);


	
	bool				associateHandleToIOCompetionPort(HANDLE handle, DWORD key);
	DWORD				calcSockOrderKey();
	SOCKADDR_IN			makeSockAddrIn(char* ip, int port);



	//--------------------------------------------------------------------------------
	void				occurConnect(SOCKET socket, SOCKADDR_IN *sockAddrIn);	
	void			    occurIOCompletion(DWORD		   completionKey, 
										  DWORD		   numberOfBytesTransferred, 
										  LPOVERLAPPED lpOverlapped);	






public:


	//--------------------------------------------------------------------------------
	inline static unsigned int __stdcall _IOCompletionThread(void* pArg)
	{		
		printf(" [CListenSocket] IOCompletionThread Start.....\n");
						
		CListenSocket* pListenSocket = NULL;
		pListenSocket = static_cast<CListenSocket*>(pArg);	
		pListenSocket->IOCompletionPoll();

		printf(" [CListenSocket] IOCompletionThread Ended.\n");

		return 0;
	}	


	inline static unsigned int __stdcall _ConnectThread(void* pArg)
	{
		printf(" [CListenSocket] ConnectThread Start.....\n");

		CListenSocket* pListenSocket = NULL;
		pListenSocket = static_cast<CListenSocket*>(pArg);	
		pListenSocket->ConnectPoll();

		printf(" [CListenSocket] ConnectThread Ended.\n");

		return 0;
	}
};

#endif