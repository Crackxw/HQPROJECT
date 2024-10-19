#ifndef _JWSENDBUFFER_H
#define _JWSENDBUFFER_H

#include "jwbase.h"

class JWSendBuffer 
{
public:
	JWSendBuffer();
	~JWSendBuffer();

public:
	void			CreateSendBuffer( SI32 size );
	void			ClearBuffer();

	BOOL			IsReadyToSend() { if( m_bComplete && m_siDataLen > 0 ) return TRUE; else return FALSE; };

	BOOL			GetSendParam( char **pSendPtr, SI32 *psiDataLen );

	void			Completion( SI32 siSentBytes );

	BOOL			Write( char *pPacket );

private:	
	char			*m_pSendBuf;
	
	SI32			m_siBufSize;
	SI32			m_siBeginPos;
	SI32			m_siEndPos;
	SI32			m_siDataLen;
	
	SI32			m_siNumPacket;

	BOOL			m_bComplete;

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
