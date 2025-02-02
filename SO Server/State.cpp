#include <windows.h>
#include "State.h"


//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CState::CState()
{
	memset(m_inState, 0, sizeof(m_inState));
	memset(m_outState, 0, sizeof(m_outState));

	m_numTransState = 0;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CState::~CState()
{
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CState::addTransitionState( DWORD input, DWORD output )
{
	if( m_numTransState < MAX_TRANS )
	{
		m_inState[m_numTransState]  = input;
		m_outState[m_numTransState] = output;

		++m_numTransState;
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CState::delTransitionState( DWORD input, DWORD output )
{
	for(int i=0; i < m_numTransState; ++i)
	{
		if( m_inState[i] == input && m_outState[i] == output )
		{					
			int size = ( m_numTransState - (i+1) ) * sizeof(m_inState[0]);

			if( size != 0 )
			memcpy( &m_inState[i], &m_inState[i+1], size );
			
			--m_numTransState;

			return;
		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
DWORD CState::getTransitionState( DWORD input )
{
	for(int i=0; i < m_numTransState; ++i)
	{
		if( m_inState[i] == input )
		{
			return m_outState[i];
		}
	}

	return 0;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
DWORD CState::getState()
{
	return STATE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
void CState::setState( DWORD state )
{
	STATE = state;
}