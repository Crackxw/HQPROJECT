// Latest Update 2002/6/26, 10:13 

#ifndef __CBATTLEMGR__
#define __CBATTLEMGR__

class CBattle;

class CBattleMgr
{
private:
	enum { MAX_BATTLE = 2000 };
	
	CBattle*	m_BattleList[MAX_BATTLE];						// ������� 
	int			m_numBattle;									// ���� ������ ���� 
	
public:
	CBattleMgr();
	~CBattleMgr();

	int			CreateBattle( UI16 uiPartyID );										
	void		DestroyBattle(int key);

	void		ActionPoll();

	CBattle*	GetBattle(int key);
};

#endif




