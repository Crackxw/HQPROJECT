#ifndef _JWPACKET_H	
#define _JWPACKET_H	

#include "jwbase.h"

class JWPacket
{
public:
	JWPacket();
	~JWPacket();

public:

	void    ClearPacket();										// 버퍼를 비운다
	void    CompletePacket();									// 패킷 쓰기 완료( 패킷의 크기를 최종적으로 설정하는 역할 )

	void    SetPacket( char *pSrc );							// 버퍼에 완성된 패킷을 설정

	void    WriteValue( void *pSrc, int size );					// 버퍼에 고정 크기 값을 쓴다
	void    WriteString( char *pSrc, int maxsize );				// 버퍼에 문자열을 쓴다

	BOOL    ReadValue( void *pDest, int size );					// 버퍼로 부터 고정 크기 값을 읽는다	
	BOOL    ReadString( char *pDest, int maxsize );				// 버퍼로 부터 문자열을 읽는다			

	char*	GetPacket() { return m_pPacketBuf; }				// 패킷 버퍼 반환			
	SI32    GetPacketSize() { return m_siDataSize; };			// 패킷 사이즈 반환			

private:
	char	m_pPacketBuf[ JWPACKET_MAX_SIZE ];					// 패킷 버퍼

public:
	SI32	m_siDataSize;										// 현재 패킷 크기
	SI32    m_siReadPos;										// 데이터를 읽을 버퍼 위치

private:
	// 멤버 함수에서 지역 변수로 사용
	SI32	local_siLen;

};

#endif

