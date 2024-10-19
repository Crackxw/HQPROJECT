#ifndef __CPLAYERLIST__
#define	__CPLAYERLIST__


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 간단한 플레이어 관리 리스트 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class SOPlayer;
class CPlayerList 
{
private:
	const int		MAX_PLAYER;

	int				m_numPlayer;
	SOPlayer**		m_PlayerList;

public:
	CPlayerList(int maxPlayer);
	~CPlayerList();

	SOPlayer*		get(int index);
	SOPlayer*		getLast();
	int				getNum();

	bool			add(SOPlayer* pPlayer);
	void			remove(SOPlayer* pPlayer);
	void			clear();	

	void			write(void* msg, int size);	
	void			write(void* msg, int size, SOPlayer* pExceptPlayer);
};


#endif