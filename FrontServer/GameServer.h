#ifndef __CGAMESERVER__
#define __CGAMESERVER__

class CAcceptSocket;




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 접속한 게임서버의 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CGameServer: public CAcceptSocket
{

private:
	int				m_BelongWorldID;



public:		
	CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize);
	~CGameServer();
	



private:

	//--------------------------------------------------------------------------------
	int				writeToSendQueue(char* data, int dataSize);

	int				calcChecksum(char* data, int size);
	int				getChecksum(char* header);
	int				getPacketSize(char* header);

	void			processPacket(char* header, char* data, int dataSize);
	


	//--------------------------------------------------------------------------------
	void			_CallBack_SockOpen() {};
	void			_CallBack_SockClose();
	bool			_CallBack_ProcessAcceptSock();
	





private:
	void			initGameServer();


	//--------------------------------------------------------------------------------		
	void			recvReportConcurrent	 ( char* data );
};

#endif