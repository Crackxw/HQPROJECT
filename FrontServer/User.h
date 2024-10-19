#ifndef __CUSER__
#define __CUSER__

class CAcceptSocket;



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 접속한 유저의 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CUser: public CAcceptSocket
{

public:		
	CUser(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize);
	~CUser();
	

	
private:		

	//--------------------------------------------------------------------------------
	int				writeToSendQueue(char* data, int dataSize);

	int				calcChecksum(char* data, int size);
	int				getChecksum(char* header);
	int				getPacketSize(char* header);

	void			processPacket(char* header, char* data, int dataSize);
	


	//--------------------------------------------------------------------------------
	void			_CallBack_SockOpen() {};
	void			_CallBack_SockClose() {};
	bool			_CallBack_ProcessAcceptSock();
							  
	


private:

	//--------------------------------------------------------------------------------
	void			recvRequestCheckVersion ( char* data );
	void			recvRequestWorldsInfo   ( char* data );
	void			recvRequestSelectWorld  ( char* data );
	void			recvRequestNotice		( char* data );

	void			sendResponseCheckVersion( int which );
	void			sendResponseWorldsInfo();
	void			sendResponseSelectWorld ( int world_id );
	void			sendResponseNotice();

};

#endif

