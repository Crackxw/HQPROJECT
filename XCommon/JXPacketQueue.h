//������������������������������������������������������������������������������
// �ۼ���      : ������			
// ���� ������ : 2002. 6. 20	
//������������������������������������������������������������������������������

#ifndef _JXPACKETQUEUE_H
#define _JXPACKETQUEUE_H

#include "jxdef.h"

class JXPacket;

class JXPacketQueue
{
public:
	JXPacketQueue();
	~JXPacketQueue();

public:
	void			Create( SI32 bufsize );
	void			Clear();
	
	BOOL			Enqueue( JXPacket *pPacket );
	BOOL			Enqueue( char *pPacket );

	BOOL			Dequeue( JXPacket *pPacket );
	BOOL			Dequeue( char *pPacket );
	
	UI32			GetNumPacket() { return m_siNumPacket; };
	SI32			GetDataSize() { return m_siDataLen; };

private:
	BOOL			__dequeue( char *pPacket );

private:
	char*			m_pQueueBuf;
	SI32			m_siBufSize;
	SI32			m_siBeginPos;
	SI32			m_siEndPos;
	SI32			m_siDataLen;
	SI32			m_siNumPacket;

private:
	// ����ȭ ����
	CRITICAL_SECTION	m_cs;

private:
	// ��� �Լ����� ����� ���� ������

	SI32			local_siLenA;
	SI32			local_siLenB;
	SI32			local_siLenC;

};

#endif

