#include <windows.h>


// CWorkQueue
#include "../kojimodo/utility/Queue.h"
#include "WorkQueue.h"




//--------------------------------------------------------------------------------------------
CWorkQueue::CWorkQueue( int max_queue ):
CQueue( sizeof(WORK::sPacketHeader), max_queue )
{	
}


//--------------------------------------------------------------------------------------------
CWorkQueue::~CWorkQueue()
{
}


//--------------------------------------------------------------------------------------------
int CWorkQueue::getPacketSize(char* header)
{
	return ((WORK::sPacketHeader*)header)->packetSize;
}


//--------------------------------------------------------------------------------------------
int CWorkQueue::Write( AUTH::sUniqueID	gameServerUniqID, 
					   AUTH::sUniqueID  userUniqID, char* data, int dataSize )
{
	
	WORK::sPacketHeader header;
			
	header.packetSize		= HEADER_SIZE + dataSize;		
	header.userUniqID		= userUniqID;
	header.gameServerUniqID = gameServerUniqID;
	

	/////////////////////////////////////////////////////
	return CQueue::Write((char*)&header, data, dataSize);	
}

