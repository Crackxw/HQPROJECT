#ifndef __CDBSUBQUEUE__
#define __CDBSUBQUEUE__



//������������������������������������������������������������������������������������������������������������������������
//
// class : ���� ť
//
//������������������������������������������������������������������������������������������������������������������������
class CAuthServerProc;
class CDBQueueProc;
class CDBSubQueue: public CDBQueueProc	  
{
public:
	CAuthServerProc*  m_pAuthServer;

public:
	CDBSubQueue(int queue_max_size, int packet_max_size);
	~CDBSubQueue();


protected:
	void	setDBInfo(DWORD command, char* data);
	

public:	
	bool	isAbleQuery(char* data);	
};


#endif