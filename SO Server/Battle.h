// Latest Update 2002/6/26, 10:13 

#ifndef __CBATTLE__
#define __CBATTLE__

class BaseChar;

class CElement;
class CBattle:public CElement
{
	/*
		Player > User,Monster 
		(플레이어는 유저와 몬스터 모두를 포함하여 지칭하는 말)
	*/
private:
	enum { MAX_PLAYER = 7, MAX_FOLLOWER = 12 };
	enum { READY = 0, ACTION = 1 };			

		
	BaseChar*			 m_PlayerList[MAX_PLAYER];				// 플레이어 목록 
	
	int					 m_numPlayer;							// 전투에 참여한 모든 플레이어 수 
	int					 m_numUser;								// 전투에 참여한 유저의 수 

	bool				 m_bSingleMode;							// 전투에 참여한 유저가 1명일 경우 
	_Game				 m_inGame;								// 전투의 내부 처리 
	
    BYTE				 m_State;								// 현재 전투의 상태 
	DWORD				 m_LatestAddUserTime;					// 가장 최근에 유저를 추가한 시간 

	UI16				 m_uiPartyID;							// 전투를 생성한 파티의 ID

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

