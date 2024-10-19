/************************************************************************************************
	파일명 : HQChattingServerPacket.cpp
	담당자 : 이윤석
************************************************************************************************/
#ifndef _HQPACKET_H
#define _HQPACKET_H


class HQPacket
{
public:
	HQPacket();
	virtual ~HQPacket();

public:
	void    ClearPacket();
	void    CompletePacket();

	void    RecvPacket( char *pSrc );

	void    WriteValue( void *pSrc, int size );
	void    WriteString( char *pSrc, int maxsize );

	BOOL    ReadValue( void *pDest, int size );
	BOOL    ReadString( char *pDest, int maxsize );

	char*	GetPacket(){ return m_pPacket; }
	UI16    GetPacketSize(){ return m_usSize; };

private:
	char    m_pPacket[ 1024 ];
	UI16	m_usSize;

	UI16    m_usReadPos;
};


#endif

