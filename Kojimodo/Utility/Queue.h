#ifndef __CQUEUE__
#define __CQUEUE__




//������������������������������������������������������������������������������������������������������������������������
//
// class : ť ���� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CQueue
{

protected:
	
	const int				HEADER_SIZE;						// ����� ũ�� 
	const int				MAX_QUEUE;							// ť�� �ִ� ũ�� 	
	
	

private:

	char*					m_queue;							// ���� ť ������ 
	int						m_queuePos;							// ���� ť�� ��ġ 
	CRITICAL_SECTION		m_csQueue;							// ť ���� ũ��Ƽ�� ���� 

	



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