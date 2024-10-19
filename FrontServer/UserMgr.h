#ifndef __CUSERMGR__
#define __CUSERMGR__

class CUser;
class CListenSocket;



//ΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜ
//
// class : 立加茄 蜡历 包府 努贰胶 
//
//ΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜ
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


