//-----------------------------------------------------------------------------------
//    2001.11.7 
//    V0.4
//    ���°�
//-----------------------------------------------------------------------------------

#ifndef _ONLINEBATTLEMSG_H
#define _ONLINEBATTLEMSG_H

#include <gscdefine.h>

#define ONLINE_BATTLE_MSG_INITBATTLE    10150		// ������ �ʿ��� �ʱ� ����. 

// Ư�� �÷��̾��� Ư�� ĳ���Ͱ� �׾��ٴ� ���� ���� ������ �˸��� �޽���. 
// dwType :Ÿ��. PlayerID:�÷��̾� ���̵�, Data[0]:ĳ���� �Ϸù�ȣ. 
#define ONLINE_BATTLE_MSG_DEAD			10151		// ĳ���Ͱ� �׾��ٴ� �޽���. 

// Ư�� �÷��̾��� Ư�� ĳ������ ������� �󸶰� �Ǿ��ٴ� �޽���.
// dwType :Ÿ��. PlayerID:�÷��̾� ���̵�, Data[0]:ĳ���� �Ϸù�ȣ. ĳ������ ü�� : BigData
#define ONLINE_BATTLE_MSG_LIFE			10152       

// Ư�� �÷��̾��� Ư�� ĳ������ �������� �󸶰� �Ǿ��ٴ� �޽���.
// dwType :Ÿ��. PlayerID:�÷��̾� ���̵�, Data[0]:ĳ���� �Ϸù�ȣ. ĳ������ ü�� : BigData
#define ONLINE_BATTLE_MSG_MANA			10153       

// Ư�� �÷��̾��� Ư�� ĳ������ ����ġ�� �󸶰� �Ǿ��ٴ� �޽���. 
// swType : Ÿ��. PlayerID:�÷��̾� ���̵�, Data[0]:ĳ���� �Ϸù�ȣ.  ĳ������ ����ġ :BigData
#define ONLINE_BATTLE_MSG_EXP			10154
       
// Ư�� �÷��̾ �α׾ƿ��ߴٴ� �޽���
// dwType :Ÿ��. PlayerID: �÷��̾� ���̵�
#define ONLINE_BATTLE_MSG_LOGOUT		10161

// ���� ������ �׿����� ���� ������ �Ƹ��� �޽���. 
// dwType :Ÿ��. PlayerID:  ���� �÷��̾� ���̵�, BigData:���� ĳ���� �Ϸù�ȣ. Data[0]:���� ĳ���� �÷��̾� ���̵�. Data[1]:���� ĳ���� �Ϸù�ȣ. 
#define ONLINE_BATTLE_MSG_KILL			10163	

// Ư�� �÷��̾ ��ȿ�ϰ� ��� ������ �˷��ִ� ��Ʈ��Ʈ �޽���. (1�п� Ŭ���̾�Ʈ���� ������ �ѹ��� ����)
// swType : Ÿ��. PlayerID:�÷��̾� ���̵�
#define ONLINE_BATTLE_MSG_HEARTBEAT		10167

// Ư�� ĳ������ ������ �����ϴ� �޽��� 
// swType : Ÿ��. PlayerID:�÷��̾� ���̵� BigData:������ ĳ���� ����(�������ڵ�). Data[0]:ĳ���� �Ϸù�ȣ 
#define ONLINE_BATTLE_MSG_CHANGEKIND	10168

// Ư�� �÷��̾��� ��� ĳ���Ϳ��� ����ġ�� �÷��ش�.
// swType : Ÿ��. PlayerID:�÷��̾� ���̵� BigData:����ġ.
#define ONLINE_BATTLE_MSG_EXPALL		10169

// Ư�� �÷��̾ ��ŷ�� �õ������� �˷��ش�. 
// swType : Ÿ��. PlayerID:�÷��̾� ���̵�
#define ONLINE_BATTLE_MSG_HACK			10170

#define MAX_ONLINE_BATTLE_MSG_QUE_SIZE     40   




#define PENALTY_MONEY_OF_DEADTH         5      // ���ΰ��� ��������� ������ ���� ����(%)�� ������. 


//��������������������������������������������������������������������������������������������������������������������������������������
// Name:
// Desc: Encrypt / Decrypt
//��������������������������������������������������������������������������������������������������������������������������������������
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
	// SetCharID�� ��ȣȭ���� �ʴ´�. �ҽ����� Data[8] ����� �ִ� ������ �׳� ������ ����. type�� �Ź� �ٸ�.
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
	// SetData, GetData�� ��ȣȭ���� �ʴ´�. �ҽ��� memcpy �̿��ϴ� �κ� �����ؾ� �ϱ� ������...
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

	// ������� ����ũ ���̵� ��ȿ���� Ȯ���Ѵ�.
	BOOL IsValidID(SI32 playerid);
	// �¶��� ĳ���� �ڵ尡 ��ȿ�� ������ Ȯ���Ѵ�. 
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

	// ��ŷ�Ǿ����� ������ �뺸�Ѵ�. 
	void SendBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(SI32 playerid);
	BOOL InterpretBattleServerGameMsg_ONLINE_BATTLE_MSG_HACK(_OnlineBattleMsg * pmsg, SI32 *playerid);

};



#endif
