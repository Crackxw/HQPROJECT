#ifndef __CQUEUE__
#define __CQUEUE__




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 큐 관리 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CQueue
{

protected:
	
	const int				HEADER_SIZE;						// 헤더의 크기 
	const int				MAX_QUEUE;							// 큐의 최대 크기 	
	
	

private:

	char*					m_queue;							// 실제 큐 포인터 
	int						m_queuePos;							// 현재 큐의 위치 
	CRITICAL_SECTION		m_csQueue;							// 큐 관리 크리티컬 섹션 

	



protected:
	
	CQueue( int headerSize, int max_queue );
	virtual ~CQueue();


public:	

	//--------------------------------------------------------------------------------
	char*				Front();
	int					Read(char* outData, int maxOutSize);
		

protected:

	//--------------------------------------------------------------------------------
	int					Write(char* header, char* data, int dataSize);
	virtual int			getPacketSize(char* header) = 0;	
	
};



#endif