#ifndef _JWFQUEUEMISO_H
#define _JWFQUEUEMISO_H

#include "jwbase.h"

class JWFQueueMISO
{
public:
	JWFQueueMISO();
	~JWFQueueMISO();

public:
	void			ClearQueue();
	void			CreateQueue( SI32 size );
	
	BOOL			Enqueue( DWORD dwItem );
	BOOL			Dequeue( DWORD *pOut_dwItem );

	SI32			GetNumberOfItems() { return m_siNumItem; };

private:	
	DWORD			*m_pBuf;
	
	SI32			m_siBufSize;
	SI32			m_siBeginPos;
	SI32			m_siEndPos;
	
	SI32			m_siNumItem;

private:
	CRITICAL_SECTION	m_cs;

};

#endif