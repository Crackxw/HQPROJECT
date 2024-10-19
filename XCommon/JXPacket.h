//������������������������������������������������������������������������������
// JXPacket class				
//	
// �ۼ���      : ������			
// ���� ������ : 2002. 6. 20	
//������������������������������������������������������������������������������

#ifndef _JXPACKET_H	
#define _JXPACKET_H	

#include "jxdef.h"

class JXPacket
{
public:
	JXPacket();
	~JXPacket();

public:

	void    ClearPacket();										// ���۸� ����
	void	ClearZeroPacket();
	void    CompletePacket();									// ��Ŷ ���� �Ϸ�( ��Ŷ�� ũ�⸦ ���������� �����ϴ� ���� )	

	void    SetPacket( char *pSrc );							// ���ۿ� �ϼ��� ��Ŷ�� ����

	void    WriteValue( void *pSrc, int size );					// ���ۿ� ���� ũ�� ���� ����
	void    WriteString( char *pSrc, int maxsize );				// ���ۿ� ���ڿ��� ����
	void    WriteString2( char *pSrc, int siLen,int maxsize );				// ���ۿ� ���ڿ��� ����

	BOOL    ReadValue( void *pDest, int size );					// ���۷� ���� ���� ũ�� ���� �д´�	
	BOOL    ReadString( char *pDest, int maxsize );				// ���۷� ���� ���ڿ��� �д´�			
	SI32    ReadString2( char *pDest, int maxsize );				// ���۷� ���� ���ڿ��� �д´�			

	inline	char*	GetPacket() { return m_pPacketBuf; }		// ��Ŷ ���� ��ȯ			
	inline	SI32    GetPacketSize() { return m_siDataSize; };	// ��Ŷ ������ ��ȯ			

private:
	char	m_pPacketBuf[ JXPACKET_MAX_SIZE ];					// ��Ŷ ����

public:
	SI32	m_siDataSize;										// ���� ��Ŷ ũ��
	SI32    m_siReadPos;										// �����͸� ���� ���� ��ġ

private:
	// ��� �Լ����� ���� ������ ���

	SI32	local_siLen;
	UI16	local_usLen;

};

#endif

