//-----------------------------------------------------------------------------------
//    2001.11.7 
//    V0.4
//    김태곤
//-----------------------------------------------------------------------------------

#ifndef _ONLINEBATTLEMSG_H
#define _ONLINEBATTLEMSG_H

#include <gscdefine.h>

#define ONLINE_BATTLE_MSG_INITBATTLE    10150		// 전투에 필요한 초기 정보. 

// 특정 플레이어의 특정 캐릭터가 죽었다는 것을 게임 서버에 알리는 메시지. 
// dwType :타입. PlayerID:플레이어 아이디, Data[0]:캐릭터 일련번호. 
#define ONLINE_BATTLE_MSG_DEAD			10151		// 캐릭터가 죽었다는 메시지. 

// 특정 플레이어의 특정 캐릭터의 생명력이 얼마가 되었다는 메시지.
// dwType :타입. PlayerID:플레이어 아이디, Data[0]:캐릭터 일련번호. 캐릭터의 체력 : BigData
#define ONLINE_BATTLE_MSG_LIFE			10152       

// 특정 플레이어의 특정 캐릭터의 마법력이 얼마가 되었다는 메시지.
// dwType :타입. PlayerID:플레이어 아이디, Data[0]:캐릭터 일련번호. 캐릭터의 체력 : BigData
#define ONLINE_BATTLE_MSG_MANA			10153       

// 특정 플레이어의 특정 캐릭터의 경험치가 얼마가 되었다는 메시지. 
// swType : 타입. PlayerID:플레이어 아이디, Data[0]:캐릭터 일련번호.  캐릭터의 경험치 :BigData
#define ONLINE_BATTLE_MSG_EXP			10154
       
// 특정 플레이어가 로그아웃했다는 메시지
// dwType :타입. PlayerID: 플레이어 아이디
#define ONLINE_BATTLE_MSG_LOGOUT		10161

// 누가 누구를 죽였는지 전투 서버에 아리는 메시지. 
// dwType :타입. PlayerID:  가해 플레이어 아이디, BigData:가해 캐릭터 일련번호. Data[0]:피해 캐릭터 플레이어 아이디. Data[1]:피해 캐릭터 일련번호. 
#define ONLINE_BATTLE_MSG_KILL			10163	

// 특정 플레이어가 유효하게 살아 있음을 알려주는 하트비트 메시지. (1분에 클라이언트에서 서버로 한번씩 전송)
// swType : 타입. PlayerID:플레이어 아이디
#define ONLINE_BATTLE_MSG_HEARTBEAT		10167

// 특정 캐릭터의 종류를 변경하는 메시지 
// swType : 타입. PlayerID:플레이어 아이디 BigData:변경할 캐릭터 종류(서버용코드). Data[0]:캐릭터 일련번호 
#define ONLINE_BATTLE_MSG_CHANGEKIND	10168

// 특정 플레이어의 모든 캐릭터에게 경험치를 올려준다.
// swType : 타입. PlayerID:플레이어 아이디 BigData:경험치.
#define ONLINE_BATTLE_MSG_EXPALL		10169

// 특정 플레이어가 해킹을 시도했음을 알려준다. 
// swType : 타입. PlayerID:플레이어 아이디
#define ONLINE_BATTLE_MSG_HACK			10170

#define MAX_ONLINE_BATTLE_MSG_QUE_SIZE     40   




#define PENALTY_MONEY_OF_DEADTH         5      // 주인공이 사망했을때 현금의 일정 비율(%)를 날린다. 


//───────────────────────────────────────────────────────────────────
// Name:
// Desc: Encrypt / Decrypt
//───────────────────────────────────────────────────────────────────
template <typename T>
class cltCrypt
{
public:
	T Encr( T data ){
		return	data ^ 2032;
	}
	T Decr( T data )
	{
		return	data ^ 2032;
	}
};

//------------------------------------------
class _OnlineBattleMsg {
private:
	DWORD dwType;
	SI32 PlayerID;
	SI32 BigData;
public:
	UI08 Data[10];

public:
	cltCrypt<DWORD>		clCryptMgr_Type;
	cltCrypt<SI32>		clCryptMgr_ID;
	cltCrypt<SI32>		clCryptMgr_BigData;
	void Init(){ZeroMemory(this, sizeof(_OnlineBattleMsg));}
	void SetType( SI32 type )
	{
		dwType	=	clCryptMgr_Type.Encr( (DWORD)type );
	}
	// SetCharID는 암호화하지 않는다. 소스에서 Data[8] 멤버에 있는 값들은 그냥 가져다 쓴다. type이 매번 다름.
	void SetCharID( SI32 charid )
	{
		Data[0]	=	charid;
	}
	DWORD GetType()
	{
		return	clCryptMgr_Type.Decr( dwType );
	}
	void SetPlayerID( SI32 id )
	{
		PlayerID	=	clCryptMgr_ID.Encr( id );
	}
	SI32 GetPlayerID()
	{
		return	clCryptMgr_ID.Decr( PlayerID );
	}
	// SetData, GetData는 암호화하지 않는다. 소스중 memcpy 이용하는 부분 수정해야 하기 때문에...
	/*
	UI08 GetData( SI32 index )
	{
		return	Data[index];
	}
	void SetData( SI32 index, UI08 data )
	{
		Data[index]	= data;
	}
	*/
	void SetBigData( SI32 data )
	{
		BigData		=	clCryptMgr_BigData.Encr( data );
	}
	SI32 GetBigData()
	{
		return	BigData		=	clCryptMgr_BigData.Decr( BigData );
	}
};



class _OnlineBattleMsgQue{
public:
	SI32 QueIndex; 
	_OnlineBattleMsg OBM[MAX_ONLINE_BATTLE_MSG_QUE_SIZE];
	SI32 OBMCheckSum[MAX_ONLINE_BATTLE_MSG_QUE_SIZE];

	void Init(){ZeroMemory(this, sizeof(_OnlineBattleMsg));}
	BOOL Push(_OnlineBattleMsg * pobm);
	BOOL Pop(_OnlineBattleMsg * pobm);

	// 사용자의 유니크 아이디가 유효한지 확인한다.
	BOOL IsValidID(SI32 playerid);
	// 온라인 캐릭터 코드가 유효한 것인지 확인한다. 
	SI32 IsValidKind(SI32 onlinekind);
	

	SI32 Encode(_OnlineBattleMsg* pobm);
	SI32 Decode(_OnlineBattleMsg* pobm);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(SI32 playerid, SI32 charid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_DEAD(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(SI32 playerid, SI32 charid, SI32 health);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LIFE(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *health);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(SI32 playerid, SI32 charid, SI32 mana);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_MANA(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *mana);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(SI32 playerid, SI32 charid, SI32 exp);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXP(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32 *charid, SI32 *exp);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_LOGOUT(SI32 playerid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_LOGOUT(_OnlineBattleMsg * pmsg, SI32 *playerid);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(SI32 killerplayerid, SI32 killercharid, SI32 victimplayerid, SI32 victimcharid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(_OnlineBattleMsg * pmsg, SI32 *killerplayerid, SI32 *killercharid, SI32 *victimplayerid, SI32 *victimcharid);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(SI32 playerid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HEARTBEAT(_OnlineBattleMsg * pmsg, SI32 *playerid);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(SI32 playerid, SI32 charid, SI32 kind);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_CHANGEKIND(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32* pcharid, SI32* pkind);

	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(SI32 playerid, SI32 exp);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_EXPALL(_OnlineBattleMsg * pmsg, SI32 *playerid, SI32* pexp);

	// 해킹되었음을 서버에 통보한다. 
	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(SI32 playerid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(_OnlineBattleMsg * pmsg, SI32 *playerid);

};



#endif
