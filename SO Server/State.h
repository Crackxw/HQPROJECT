#ifndef __CSTATE__
#define __CSTATE__

class CState
{
private:
	enum { MAX_TRANS = 10 };							

	DWORD		STATE;

	
	DWORD		m_inState[MAX_TRANS];			// 전이 상태의 목록 
	DWORD		m_outState[MAX_TRANS];			

	
	int			m_numTransState;				// 현재 전이 상태의 갯수 

public:
	CState();
	~CState();

	void   addTransitionState( DWORD input, DWORD output );
	void   delTransitionState( DWORD input, DWORD output );

	DWORD  getTransitionState( DWORD input );

	DWORD  getState();
	void   setState( DWORD state );
};




#endif