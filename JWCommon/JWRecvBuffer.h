#ifndef _JWRECVBUFFER_H
#define _JWRECVBUFFER_H

#include "jwbase.h"

class JWRecvBuffer 
{
public:
	JWRecvBuffer();
	~JWRecvBuffer();

public:
	void			CreateRecvBuffer( SI32 size );
	void			ClearBuffer();
	
	void			GetRecvParam( char **pRecvPtr, SI32 *psiFreeSize );

	BOOL			Completion( SI32 siRecvSize );

	char*			GetFirstPacket();
	void			RemoveFirstPacket();

private:	
	char			*m_pRecvBuf;

	char			*m_pBasePtr;
	char			*m_pBeginPtr;
	char			*m_pCompletePtr;
	char			*m_pLastPtr;
	char			*m_pEndPtr;

	SI32			m_siNumPacket;
		
	BOOL			m_bRemovable;			

private:
	CRITICAL_SECTION	m_cs;

private:
	char			*local_pCheckPtr;
	char			*local_pInterestPtr;

	UI16			local_usDataSize;

	SI32			local_siTemp;

	SI32			local_siNumPacket;
};

#endif
