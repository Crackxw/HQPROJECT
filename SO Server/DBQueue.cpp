#include <windows.h>
#include <stdio.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 생성자 
//--------------------------------------------------------------------------------------------
CDBQueue::CDBQueue( int a, int b ): 
			   QUEUE_MAX_SIZE(a), PACKET_MAX_SIZE(b), PACKET_HEADER_SIZE(sizeof(DBQUERY_HEADER))

{
	m_queue       = new char[ QUEUE_MAX_SIZE ];
	temp_packet   = new char[ PACKET_MAX_SIZE ];


	memset(m_queue,		 0, sizeof(m_queue));
	memset(temp_packet,	 0, sizeof(temp_packet));


	m_writePos     = 0;
	m_readPos	   = 0;
	m_curPos	   = 0;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 소멸자 
//--------------------------------------------------------------------------------------------
CDBQueue::~CDBQueue()
{
	delete m_queue;
	delete temp_packet;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐에 패킷을 집어 넣는다 
//--------------------------------------------------------------------------------------------
int	CDBQueue::write(char* data, int size)
{	

	// 큐 오버 플로우 	
	if( m_curPos + size > QUEUE_MAX_SIZE ) return -1;
	if( m_curPos == 0 ) 
	{
		m_writePos = m_readPos = 0;
	}


	int writeEnd = m_writePos + size;

	// 패킷이 나눠져야 하는 경우 
	if( writeEnd > QUEUE_MAX_SIZE )
	{
		int sub = writeEnd - QUEUE_MAX_SIZE; 
		
		// 나눠서 복사 한다 
		memcpy(m_queue + m_writePos, data               , size - sub);   // front
		memcpy(m_queue             , data + (size - sub), sub);			 // back 

		m_writePos = sub;
	}
	else
	{
		memcpy(m_queue + m_writePos, data, size);
		m_writePos += size;
	}

	m_curPos += size;
	return m_curPos;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 큐의 제일 앞에 있는 패킷을 읽어 온다 
//--------------------------------------------------------------------------------------------
char* CDBQueue::read()
{	

	// 큐가 비었을 경우 
	if( m_curPos == 0 ) return NULL;
		
	int sub = QUEUE_MAX_SIZE - m_readPos;
	
	// 나누어졌고  헤더 사이즈 보다 적게 나누어 졌을 경우 
	if( sub < PACKET_HEADER_SIZE )
	{		
		memcpy(temp_packet	  , m_queue + m_readPos, sub ); // 나눠진 앞부분
		memcpy(temp_packet+sub, m_queue            , PACKET_MAX_SIZE - sub);

		int size  = ((DBQUERY_HEADER*)temp_packet)->packetSize;

		m_readPos = (m_readPos + size) - QUEUE_MAX_SIZE;

		m_curPos -= size;
		return temp_packet;
	}
	else
	{			
		char* packet = m_queue + m_readPos;

		int size = ((DBQUERY_HEADER*)packet)->packetSize;

		// 나누어 졌고 헤더 사이즈보다 크게 나누어 졌을 경우 
		if( m_readPos + size > QUEUE_MAX_SIZE )
		{
			memcpy(temp_packet    , m_queue + m_readPos, sub); // 나눠진 앞부분 
			memcpy(temp_packet+sub, m_queue            , size - sub);
			
			m_readPos = (m_readPos + size) - QUEUE_MAX_SIZE;
			
			m_curPos -= size;
			return temp_packet;
		}

		m_readPos+=size;

		m_curPos -= size;
		return packet;
	}

	return NULL;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 현재 큐에 쌓인 패킷의 양을 얻어온다 
//--------------------------------------------------------------------------------------------
int	CDBQueue::getSize()
{
	return m_curPos;
}