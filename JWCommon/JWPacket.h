#ifndef _JWPACKET_H	
#define _JWPACKET_H	

#include "jwbase.h"

class JWPacket
{
public:
	JWPacket();
	~JWPacket();

public:

	void    ClearPacket();										// ���۸� ����
	void    CompletePacket();									// ��Ŷ ���� �Ϸ�( ��Ŷ�� ũ�⸦ ���������� �����ϴ� ���� )

	void    SetPacket( char *pSrc );							// ���ۿ� �ϼ��� ��Ŷ�� ����

	void    WriteValue( void *pSrc, int size );					// ���ۿ� ���� ũ�� ���� ����
	void    WriteString( char *pSrc, int maxsize );				// ���ۿ� ���ڿ��� ����

	BOOL    ReadValue( void *pDest, int size );					// ���۷� ���� ���� ũ�� ���� �д´�	
	BOOL    ReadString( char *pDest, int maxsize );				// ���۷� ���� ���ڿ��� �д´�			

	char*	GetPacket() { return m_pPacketBuf; }				// ��Ŷ ���� ��ȯ			
	SI32    GetPacketSize() { return m_siDataSize; };			// ��Ŷ ������ ��ȯ			

private:
	char	m_pPacketBuf[ JWPACKET_MAX_SIZE ];					// ��Ŷ ����

public:
	SI32	m_siDataSize;										// ���� ��Ŷ ũ��
	SI32    m_siReadPos;										// �����͸� ���� ���� ��ġ

private:
	// ��� �Լ����� ���� ������ ���
	SI32	local_siLen;

};

#endif

