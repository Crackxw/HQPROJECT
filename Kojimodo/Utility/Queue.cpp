#include <windows.h>
#include <stdio.h>
#include "Queue.h"
	



//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CQueue::CQueue( int headerSize, int max_queue ): 

HEADER_SIZE(headerSize), 
MAX_QUEUE(max_queue)

{	
	m_queue		   = new char[MAX_QUEUE];	

	memset(m_queue,	0, MAX_QUEUE);	
	m_queuePos  = 0;

	InitializeCriticalSection(&m_csQueue);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �� �� �� 
//--------------------------------------------------------------------------------------------
CQueue::~CQueue()
{
	if( m_queue ) {

		delete m_queue;
	}


	DeleteCriticalSection(&m_csQueue);
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť���� ����Ÿ�� ��� �´� 
//--------------------------------------------------------------------------------------------
char* CQueue::Front()
{
	EnterCriticalSection(&m_csQueue);

	
	if( m_queuePos >= HEADER_SIZE )
	{					 			
		int		packetSize	 =	getPacketSize( m_queue );	  // ��Ŷ�� ũ�� 
		char*	data		 =  m_queue + HEADER_SIZE;		  // ����Ÿ�� ���� ������ 


		if( m_queuePos >= packetSize ) {							
						
			LeaveCriticalSection(&m_csQueue);
			return m_queue;
		}
	}		


	LeaveCriticalSection(&m_csQueue);
	return NULL;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť���� ����Ÿ�� �о� �´� (�о�� ����Ÿ�� �����ȴ�)
//--------------------------------------------------------------------------------------------
int CQueue::Read(char* outData, int maxOutSize)
{	
	EnterCriticalSection(&m_csQueue);


	int readSize = 0;
	if( m_queuePos >= HEADER_SIZE )
	{					 			
		int		packetSize	 =	getPacketSize( m_queue );	  // ��Ŷ�� ũ�� 
		char*	data		 =  m_queue + HEADER_SIZE;		  // ����Ÿ�� ���� ������ 

		

		if( m_queuePos >= packetSize ) {							
						
			if( outData && packetSize <= maxOutSize ) {
				
				memcpy(outData, m_queue, packetSize);
				
				readSize = packetSize;
			}

			else {

				readSize = -1;
			}
			
			m_queuePos -= packetSize;								
			
			if(m_queuePos > 0)
			memmove(m_queue, m_queue + packetSize, m_queuePos);													
		}
	}		

	LeaveCriticalSection(&m_csQueue);
	return readSize;
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ť�� ����Ÿ�� �ִ´� 
//--------------------------------------------------------------------------------------------
int CQueue::Write(char* header, char* data, int dataSize)
{
	if( header != NULL && data != NULL && dataSize > 0 )
	{
		EnterCriticalSection(&m_csQueue);

		int packetSize = HEADER_SIZE + dataSize;

		if(m_queuePos + packetSize <= MAX_QUEUE)	
		{		 			
			
			/* HEADER COPY */
			memcpy(m_queue + m_queuePos, header, HEADER_SIZE);
			m_queuePos += HEADER_SIZE;


			/* DATA COPY */
			memcpy(m_queue + m_queuePos, data, dataSize);
			m_queuePos += dataSize;							

	
			LeaveCriticalSection(&m_csQueue);

			return packetSize;
		}
		else
		{	
			printf("Queue Overflow!\n");	
			
			LeaveCriticalSection(&m_csQueue);
			
			return -1;
		}	
	}

	return 0;
}




