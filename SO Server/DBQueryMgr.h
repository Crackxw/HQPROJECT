#ifndef __CDBQUERYMGR__
#define __CDBQUERYMGR__



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : DB 요청을 관리 하는 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CDBGateQueue;
class CDBSubQueue;
class CDBQueryMgr
{

private:			
	enum { MAX_QUEUE = 3000000 , MAX_PACKET = 10000 };

	char					m_depository[ MAX_QUEUE ];  	// 쿼리 보관함 			

protected:
	int						m_depositoryPos;			    // 보관함의 현재 위치 		

protected:	
	CDBGateQueue*			m_pGateQueue;					// 게이트 쿼리 큐 
	CDBSubQueue*			m_pSubQueue;					// 서브	  쿼리 큐 

public:
	CDBQueryMgr();
	virtual ~CDBQueryMgr();
		
protected:	
	void					writeToDepository( char* header, int headerSize,
											   char* data,   int dataSize );	
public:	
	void					createQueue();
	void					emptiedDepository();

	void					createQueueThread();


};



#endif