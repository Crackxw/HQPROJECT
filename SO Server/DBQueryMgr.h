#ifndef __CDBQUERYMGR__
#define __CDBQUERYMGR__



//������������������������������������������������������������������������������������������������������������������������
//
// class : DB ��û�� ���� �ϴ� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CDBGateQueue;
class CDBSubQueue;
class CDBQueryMgr
{

private:			
	enum { MAX_QUEUE = 3000000 , MAX_PACKET = 10000 };

	char					m_depository[ MAX_QUEUE ];  	// ���� ������ 			

protected:
	int						m_depositoryPos;			    // �������� ���� ��ġ 		

protected:	
	CDBGateQueue*			m_pGateQueue;					// ����Ʈ ���� ť 
	CDBSubQueue*			m_pSubQueue;					// ����	  ���� ť 

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