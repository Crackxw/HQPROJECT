#ifndef __CDBGATEQUEUE__
#define __CDBGATEQUEUE__



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
// class : 啪檜お 聽
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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