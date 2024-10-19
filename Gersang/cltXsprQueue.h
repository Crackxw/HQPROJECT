#ifndef	_XSPRQUEUE_H_
#define	_XSPRQUEUE_H_

#include "Graphics16.h"

#define	MAX_FILENAME	64


struct	strXsprQueueData
{
	char	szFileName[MAX_FILENAME];
	SI32	siFileIndex;
	XSPR	*pXspr;
};

class	cltXsprQueue
{
public:
	cltXsprQueue( SI32	siSize );
	~cltXsprQueue();
	
public:
	BOOL	Peek();
	BOOL	Enqueue( strXsprQueueData stData );
	BOOL	Dequeue( strXsprQueueData &stData );
	SI32	GetLength();
	BOOL	StrongDequeue( XSPR *pXspr, strXsprQueueData &stData );

private:
	strXsprQueueData	*m_pQueue;
	SI32				m_siLen;
	SI32				m_siMax;
	CRITICAL_SECTION	m_cs;
};


#endif