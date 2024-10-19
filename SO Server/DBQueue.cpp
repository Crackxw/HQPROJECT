#include <windows.h>
#include <stdio.h>
#include "_DBQueryDefine.h"
#include "DBQueue.h"


//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ������ 
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
//	Desc : �Ҹ��� 
//--------------------------------------------------------------------------------------------
CDBQueue::~CDBQueue()
{
	delete m_queue;
	delete temp_packet;
}



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť�� ��Ŷ�� ���� �ִ´� 
//--------------------------------------------------------------------------------------------
int	CDBQueue::write(char* data, int size)
{	

	// ť ���� �÷ο� 	
	if( m_curPos + size > QUEUE_MAX_SIZE ) return -1;
	if( m_curPos == 0 ) 
	{
		m_writePos = m_readPos = 0;
	}


	int writeEnd = m_writePos + size;

	// ��Ŷ�� �������� �ϴ� ��� 
	if( writeEnd > QUEUE_MAX_SIZE )
	{
		int sub = writeEnd - QUEUE_MAX_SIZE; 
		
		// ������ ���� �Ѵ� 
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
//	Desc : ť�� ���� �տ� �ִ� ��Ŷ�� �о� �´� 
//--------------------------------------------------------------------------------------------
char* CDBQueue::read()
{	

	// ť�� ����� ��� 
	if( m_curPos == 0 ) return NULL;
		
	int sub = QUEUE_MAX_SIZE - m_readPos;
	
	// ����������  ��� ������ ���� ���� ������ ���� ��� 
	if( sub < PACKET_HEADER_SIZE )
	{		
		memcpy(temp_packet	  , m_queue + m_readPos, sub ); // ������ �պκ�
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

		// ������ ���� ��� ������� ũ�� ������ ���� ��� 
		if( m_readPos + size > QUEUE_MAX_SIZE )
		{
			memcpy(temp_packet    , m_queue + m_readPos, sub); // ������ �պκ� 
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
//	Desc : ���� ť�� ���� ��Ŷ�� ���� ���´� 
//--------------------------------------------------------------------------------------------
int	CDBQueue::getSize()
{
	return m_curPos;
}