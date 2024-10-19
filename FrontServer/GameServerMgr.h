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
	CGameServer*				getGameServer(int index);

	//--------------------------------------------------------------------------------	
	CAcceptSocket*				_CallBack_CreateSocket(int index); 
	void						_CallBack_ProcessPoll() {};

};

#endif