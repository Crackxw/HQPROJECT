#ifndef	_FREELIST_H_
#define	_FREELIST_H_

#include "cltList.h"
#include "cltXsprQueue.h"


class cltFreeXsprList : public cltList<strXsprQueueData>
{
public:
	cltFreeXsprList( SI32 siSize );
	virtual		~cltFreeXsprList();

public:
	BOOL		AddXspr( strXsprQueueData *pstData );
	BOOL		DeleteXspr( strXsprQueueData *pstData );

private:
	CRITICAL_SECTION	m_cs;
};


#endif