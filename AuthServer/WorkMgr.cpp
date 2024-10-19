#include <windows.h>


// CWorkQueue
#include "../kojimodo/utility/Queue.h"
#include "WorkQueue.h"


// CWorkMgr
#include "WorkMgr.h"




//--------------------------------------------------------------------------------------------
CWorkMgr::CWorkMgr()
{
}


//--------------------------------------------------------------------------------------------
CWorkMgr::~CWorkMgr()
{
	if( m_QueryQueue ) {

		delete m_QueryQueue;
	}

	if( m_ResultQueue ) {

		delete m_ResultQueue;
	}
}


//--------------------------------------------------------------------------------------------
void CWorkMgr::CreateQueue( int max_queue )
{
	m_QueryQueue	= new CWorkQueue( max_queue );
	m_ResultQueue	= new CWorkQueue( max_queue );
}


//--------------------------------------------------------------------------------------------
int CWorkMgr::setResult ( AUTH::sUniqueID	gameServerUniqID, 
					      AUTH::sUniqueID   userUniqID, char* data, int dataSize )
{
	return m_ResultQueue->Write( gameServerUniqID, userUniqID, data, dataSize );
}


//--------------------------------------------------------------------------------------------
int CWorkMgr::setQuery  ( AUTH::sUniqueID	gameServerUniqID, 
						  AUTH::sUniqueID   userUniqID, char* data, int dataSize )
{
	return m_QueryQueue->Write( gameServerUniqID, userUniqID, data, dataSize );
}


//--------------------------------------------------------------------------------------------
int CWorkMgr::getResult (char* outData, int maxOutSize)
{
	return m_ResultQueue->Read( outData, maxOutSize );
}


//--------------------------------------------------------------------------------------------
int CWorkMgr::getQuery  (char* outData, int maxOutSize)
{
	return m_QueryQueue->Read( outData, maxOutSize );
}

