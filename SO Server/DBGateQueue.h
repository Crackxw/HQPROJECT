#ifndef __CDBGATEQUEUE__
#define __CDBGATEQUEUE__



//������������������������������������������������������������������������������������������������������������������������
//
// class : ����Ʈ ť
//
//������������������������������������������������������������������������������������������������������������������������
class CDBQueueProc;
class CDBGateQueue: public CDBQueueProc	  
{

public:
	CDBGateQueue(int queue_max_size, int packet_max_size);
	~CDBGateQueue();


protected:
	void	setDBInfo(DWORD command, char* data);
	

public:	
	bool	isAbleQuery(char* data);	
};


#endif