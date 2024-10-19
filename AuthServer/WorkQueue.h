#ifndef	__CWORKQUEUE__
#define __CWORKQUEUE__


#include "_WorkDefine.h"


class CWorkQueue: public CQueue
{

public:
	CWorkQueue( int max_queue );
	~CWorkQueue();



public:
	int			Write( AUTH::sUniqueID	gameServerUniqID, 
					   AUTH::sUniqueID  userUniqID, char* data, int dataSize );


protected:
    int			getPacketSize(char* packetSize);	// pure virtual

};


#endif