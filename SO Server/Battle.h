// Latest Update 2002/6/26, 10:13 

#ifndef __CBATTLE__
#define __CBATTLE__

class BaseChar;

class CElement;
class CBattle:public CElement
{
	/*
		Player > User,Monster 
		(�÷��̾�� ������ ���� ��θ� �����Ͽ� ��Ī�ϴ� ��)
	*/
private:
	enum { MAX_PLAYER = 7, MAX_FOLLOWER = 12 };
	enum { READY = 0, ACTION = 1 };			

		
	BaseChar*			 m_PlayerList[MAX_PLAYER];				// �÷��̾� ��� 
	
	int					 m_numPlayer;							// ������ ������ ��� �÷��̾� �� 
	int					 m_numUser;								// ������ ������ ������ �� 

	bool				 m_bSingleMode;							// ������ ������ ������ 1���� ��� 
	_Game				 m_inGame;								// ������ ���� ó�� 
	
    BYTE				 m_State;								// ���� ������ ���� 
	DWORD				 m_LatestAddUserTime;					// ���� �ֱٿ� ������ �߰��� �ð� 

	UI16				 m_uiPartyID;							// ������ ������ ��Ƽ�� ID

public:
	CBattle();
	~CBattle();

	void				 InitBattleProperty();	
		
	bool				 AddPlayer(BaseChar* player);
	int					 RemovePlayer(BaseChar* player);
		
	void				 Action();
	void				 PushData(char* data, int size);

	bool				 IsJoinable();

	void				 LaunchBattle();
	void				 PrepareBattle();	
	void				 CleanupBattle();

	VOID				 SetPartyID( UI16 uiPartyID );
	UI16				 GetPartyID();

	// for _Game class
	//------------------------------------------------------------------------------
	BaseChar*			 GetPlayer(int index);
	OnlineFollower*      GetFollower(int playerindex, int followerindex);
	_ImportantParameta*  GetFollowerParam(int playerindex, int followerindex);
	
	bool				 IsSingleMode();

	void				 SendToAll(char* data, int size);
	//-------------------------------------------------------------------------------

private:		
	void				 ResetPlayerList();
	void				 SetPlayer(int index, BaseChar* player);	
	
	int					 GetBattleInfo(char* data);
	void				 GetBattleFollowerInfo(OnImjinBattlePlayerInfo* OIBPI, int playerindex);
	void				 SendBattleInfoToAll();

	void				 SetState(int state);
	int					 GetState();		
};

#endif

