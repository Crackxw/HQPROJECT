#ifndef __CLISTENSOCKET__
#define __CLISTENSOCKET__



//������������������������������������������������������������������������������������������������������������������������
//
// class : 
//
//������������������������������������������������������������������������������������������������������������������������
class CListenSocket
{	

private:	
	const int			MAX_SOCKET;								// ������ �ִ� �� 
	int					m_numSocket;							// ������ ���� �� 
	
	
	HANDLE				m_hCompletionPort;						// ������ �Ϸ� ��Ʈ 	
	DWORD				m_sockOrderSeed;					    // ������ ���� ���� ���ذ� 		

	
	CAcceptSocket**		m_SocketList;							// ������ ��ü ����Ʈ 	


	SOCKET				m_ListenSocket;							// ���� ��� ���� 
	WSAEVENT			m_wsaConnectEvent[2];					// ���ϵ��� ���� ó�� �̺�Ʈ	

	bool				m_bShutdown;							// ������� �������� ���� ���� 
		

protected:
	// warning when used thread or child class			
	CRITICAL_SECTION	m_csSocketList;							// ������ ����Ʈ ���� ũ��Ƽ�� ����		
					


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