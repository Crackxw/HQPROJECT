#ifndef __CDBQUEUE__
#define __CDBQUEUE__



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : DB 쿼리 큐 ( 원형 큐 )
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CDBQueue 
{	
protected: 
	const int		QUEUE_MAX_SIZE;							// 큐의 최대 크기 
	const int		PACKET_MAX_SIZE;						// 큐에 들어가는 패킷의 최대 크기 
	const int	    PACKET_HEADER_SIZE;						// 큐에 들어가는 패킷의 헤더 사이즈 
	
private:
	char*			m_queue;								// 큐 메모리  
			
	int				m_curPos;								// 현재 큐의 크기 
	int				m_writePos;								// 현재 쓰기 위치 
	int				m_readPos;								// 현재 읽기 위치 

	char*			temp_packet;							// 임시 패킷 보관 변수 
	




public:
	CDBQueue(int queue_max_size, int packet_max_size);
	~CDBQueue();
	

protected:
	int				write(char* data, int size);
	char*			read();	


public:	
	int				getSize();

};



#endif