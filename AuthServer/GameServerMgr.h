#ifndef __CGAMESERVERMGR__
#define __CGAMESERVERMGR__

class CGameServer;
class CListenSocket;

	  

//������������������������������������������������������������������������������������������������������������������������
//
// class : ������ ���Ӽ��� ���� Ŭ����  
//
//������������������������������������������������������������������������������������������������������������������������
class CGameServerMgr:public CListenSocket, public CSingleton<CGameServerMgr>
{


public:
	CGameServerMgr(char* ip, int port, int max_socket);
	~CGameServerMgr();



private:
	

	//--------------------------------------------------------------------------------	
	CAcceptSocket*				_CallBack_CreateSocket(int index); 
	void						_CallBack_ProcessPoll();


	CGameServer*				getGameServer(int index);


public:

	int							writeToSendQueue( AUTH::sUniqueID gameServerUniqID, 
												  AUTH::sUniqueID userUniqID, 
												  char* data, int dataSize );
	

};

#endif