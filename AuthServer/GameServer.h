#ifndef __CGAMESERVER__
#define __CGAMESERVER__

class CAcceptSocket;

#include "_AuthDefine.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 접속한 게임서버의 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CGameServer: public CAcceptSocket
{

public:		
	CGameServer(DWORD completionKey, int rbsize, int sbsize, int rqsize, int sqsize);
	~CGameServer();
	



private:

	//--------------------------------------------------------------------------------
	int					calcChecksum(char* data, int dataSize);
	int					getChecksum(char* header);
	int					getPacketSize(char* header);

	void				processPacket(char* header, char* data, int dataSize);
	


	//--------------------------------------------------------------------------------
	void				_CallBack_SockOpen() {};
	void				_CallBack_SockClose() {};
	bool				_CallBack_ProcessAcceptSock(); 
	


	AUTH::sUniqueID		getUniqueID();
	


public:
	
	bool				isMatchUniqueID( AUTH::sUniqueID workUniqID );

	//--------------------------------------------------------------------------------
	int					writeToSendQueue( AUTH::sUniqueID userUniqID, 
										  char* data, int dataSize );
	
};

#endif