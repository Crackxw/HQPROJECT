#ifndef __CUSERMGR__
#define __CUSERMGR__

class CUser;
class CListenSocket;



//������������������������������������������������������������������������������������������������������������������������
//
// class : ������ ���� ���� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CUserMgr:public CListenSocket, public CSingleton<CUserMgr>
{

public:
	CUserMgr(char* ip, int port, int max_socket);
	~CUserMgr();


private:
	CUser*					getUser(int index);



	//--------------------------------------------------------------------------------	
	CAcceptSocket*			_CallBack_CreateSocket(int index); 
	void					_CallBack_ProcessPoll() {};	
};

#endif


