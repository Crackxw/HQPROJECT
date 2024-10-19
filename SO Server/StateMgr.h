#ifndef __CSTATEMGR__
#define __CSTATEMGR__

#define NORMAL_MOVING		 1
#define FOLLOW_MOVING		 2
#define	GROUP_MOVING		 3
#define	AVOID_MOVING		 4
#define SEEN_STRONG_ENEMY	 5
#define SEEN_WEAK_ENEMY		 6

class CState;
class CStateMgr
{
private:
	enum { MAX_STATE = 10 };

	CState			m_StateList[MAX_STATE];
	int				m_numState;
	
	DWORD			m_curState;

public:
	CStateMgr();
	~CStateMgr();
	
	void			addState( CState& state );
	void			delState( DWORD state );

	DWORD			getCurState();
	void			setCurState( DWORD state );
	
	void			transitionState( DWORD input );
};

#endif 