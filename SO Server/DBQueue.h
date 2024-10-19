#ifndef __CDBQUEUE__
#define __CDBQUEUE__



//������������������������������������������������������������������������������������������������������������������������
//
// class : DB ���� ť ( ���� ť )
//
//������������������������������������������������������������������������������������������������������������������������
class CDBQueue 
{	
protected: 
	const int		QUEUE_MAX_SIZE;							// ť�� �ִ� ũ�� 
	const int		PACKET_MAX_SIZE;						// ť�� ���� ��Ŷ�� �ִ� ũ�� 
	const int	    PACKET_HEADER_SIZE;						// ť�� ���� ��Ŷ�� ��� ������ 
	
private:
	char*			m_queue;								// ť �޸�  
			
	int				m_curPos;								// ���� ť�� ũ�� 
	int				m_writePos;								// ���� ���� ��ġ 
	int				m_readPos;								// ���� �б� ��ġ 

	char*			temp_packet;							// �ӽ� ��Ŷ ���� ���� 
	




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