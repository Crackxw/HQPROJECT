#ifndef __CWINSOCK__
#define __CWINSOCK__



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : 
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
class CWinsock
{

public:
	enum {

		FAIL_CONNECT			= -99 , 
		FAIL_SEND					  ,
		FAIL_RECV					  ,	
		OVERFLOW_RECVQUEUE			  ,		
		OVERFLOW_SENDQUEUE			  ,
		OVER_WAITTIME				  ,	
		ERROR_CHECKSUM				  ,
		ERROR_SELECT				  ,		
		DONOT_CONNECT
	};


protected:	
	const int			 HEADER_SIZE;
	const int			 MAX_SENDQUEUE;
	const int			 MAX_RECVQUEUE;
	const DWORD			 WAIT_TIME;
	


private:	
	SOCKET				 m_socket;
	
	timeval				 m_timeout;

	fd_set				 m_fdError;
	fd_set				 m_fdUser;

	int					 m_rcurPos;								
	int					 m_scurPos;								

	char*				 m_recvQueue;   		
	char*				 m_sendQueue;

	int					 m_lastError;



public:
	CWinsock(int header_size, int wait_time = 15000, int max_sendQ = 10000, int max_recvQ = 10000);
	virtual ~CWinsock();



private:
	void					initialize();
	
	void					clearFD_SET();		

	bool					Recv();
	bool					Send();

	
protected:		
	int						getLastError();
	void					setLastError(int error);

	bool					isSendable();
	bool					isRecvable();	

	bool					Connect (char* ip, int port);		
	void					Dissconnect();

	bool					Recv(char* header, char* data, int dataSize);	// BlockFunction
	bool					Send(char* header, char* data, int dataSize);	// BlockFunction

	bool					getRecvPacket(char* header, char* data, int dataSize);
	bool					setSendPacket(char* header, char* data, int dataSize);


public:		
	void					startupWinsock();
	void					cleanupWinsock();
	
	


protected:

	//--------------------------------------------------------------------------------
	virtual int				getPacketSize(char* header) = 0;	
	virtual int				getChecksum(char* header) = 0;	
	virtual int				calcChecksum(char* data, int size) = 0;

};

#endif