//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include "..\Gersang\maindata.h"
#include "..\Gersang\music.h"
#include "..\Gersang\YListButton.h"
//#include "..\Gersang\SetGame.h"
#include "..\Gersang\GMenu.h"
#include "..\Gersang\text.h"

#include <mynet.h>
#include <OnlineImjin2i.h>
#include <mynet-game.h>
#include <OnlineBattleMsg.h>
#include <OnlineWorld.h>
#include <Etc.h>
#include <CharInterface.h>
#include <SetChar.h>
#include <clGame.h>

extern _clGame *pGame;

extern	SI32				UserPlayerNumber;
extern	_MyNetGame			MyNet;
extern	BOOL				GameDropSwitch;
extern	clock_t				CurrentClock;
extern	DWORD				PrevMultiClock;
extern	_Text				Text;
extern	_MainData			GD;
extern	SHORT				SoloPlaySwitch;			// ȥ�� �ϱ� ����ΰ� ������ �ϱ� ����ΰ�?
extern	int					MultiPlayDelayTime;

extern	int					FrameClockDelay; 
extern	OnlineBattleInfo	OBInfo;
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 

extern	SHORT				GameStatus;
extern	BOOL				GoOutSwitch;			// ���� �䱸�޾Ҵ°��� �˷��ִ� ����ġ 

//CLIENTDPIDMSG		ClientdpId[MAX_MULTI_PLAYER]; // �ٸ� Ŭ���̾�Ʈ�� ������ 
BOOL				ClientId[MAX_MULTI_PLAYER];   // �ٸ� Ŭ���̾�Ʈ�� ������ ���´��� �˷��ش�.
DWORD				gdwClientStartTime = 0;      // Client���� Start�޽����� ÷ �޾������� �ð� 

BOOL				gbGetMessageCurrent= FALSE;  // Thread���� CURRENT_PLAYER�� ������ �޾Ҵ�.
BOOL				gbGetMessageHost   = FALSE;  // Thread���� HOST_MESSAGE�� ������ �޾Ҵ�.

BOOL				gbStartWaiting     = FALSE;  // ���ӽ��� �޽����� �ް� ��ٸ��� �ִ°�?
BOOL				gbGetMessageClient = FALSE;  // Thread���� CLIENT_MESSAGE�� ������ �޾Ҵ�.
DWORD				gdpIdHostID = 0;             // HOST_MESSAGE�� ���� ����� DP ID

_OnlineBattleMsgQue OBMQue;

BOOL GoOutProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL ReturnPingProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL PingProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL AgreeNewHostProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL GameChatProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL ClientProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL ClientOrderProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL MyProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);
BOOL BattleServerCheckProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);

void _MyNetGame::Init()
{

	int i;

	_MyNet::Init();

	OBMQue.Init();

    for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{
		// Client�� �����͸� ������ �ִ� �迭 �ʱ�ȭ 
//		ZeroMemory(&ClientdpId[i],sizeof(CLIENTDPIDMSG));
//		ClientdpId[i].dwType  = GAME_MSG_CLIENT;
		// Client�� ������ �޾����� �˷��ִ� �迭 
		ClientId[i] = FALSE;
	}

	SetFunc(GAME_MSG_GOOUT,			GoOutProc);
	SetFunc(GAME_MSG_AGREENEWHOST,	AgreeNewHostProc);
	SetFunc(GAME_MSG_CHAT,			GameChatProc);
	SetFunc(GAME_MSG_CLIENT,		ClientProc);
	SetFunc(GAME_MSG_CLIENT_ORDER,	ClientOrderProc);
	SetFunc(GAME_MSG_MY,			MyProc);
	SetFunc(GAME_MSG_BATTLESERVERCHECK,	BattleServerCheckProc);
}


//--------------------------------------------------------------------------
// Name: GetGameLatency()
// Desc: �� player�� Ping�� ���Ͽ� ���� ū ���� ���Ѵ�.
//       �̶� Delay�� ������ ���� ������ ����Ѵ�.
//--------------------------------------------------------------------------
int _MyNetGame::GetGameLatency()
{
	int i;

	// Default Latency �� 200���� �Ѵ�.
	DWORD max_ping_delay = MIN_GAME_LATENCY;


	// �ִ� 8����� �� Player�� Delay�� ���Ѵ�.
	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		if(PlayersdpId.dpId[i] == 0)continue;

		if(PingDelay[i] > max_ping_delay)
		{
			max_ping_delay = PingDelay[i];
		}
	}

	max_ping_delay = (max_ping_delay / 100 + 1) * 100;

	return min(MAX_GAME_LATENCY, max_ping_delay);
}

BOOL GoOutProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	LPGOOUTMSG         lpGoOutMsg;

	lpGoOutMsg = (LPGOOUTMSG) lpMsg;
	
	if(lpGoOutMsg->PlayerNumber  == UserPlayerNumber)
	{
		// ������ ����Ǿ����� �����Ѵ�. 
		GameDropSwitch = TRUE;
	}
	// �����ϴ�  �÷��̾��� ĳ���͸� ��� �����Ѵ�. 
	
	SI32 temp_i;
	for(temp_i = 0 ;temp_i < MAX_CHAR_NUMBER;temp_i++)
	{
		if(IsAlive(temp_i) && clifChar.GetPlayerNumber(temp_i) == lpGoOutMsg->PlayerNumber)
		{
			DeleteChar(temp_i);
		}
	}
	DoLogoutAction( MyNet.PlayersdpId.dpId[lpGoOutMsg->PlayerNumber] );
	

	return TRUE;

}


BOOL AgreeNewHostProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	int id;
	LPAGREENEWHOSTMSG   lpAgreeNewHostMsg;

	lpAgreeNewHostMsg = (LPAGREENEWHOSTMSG) lpMsg;
	
	// ȣ��Ʈ ���� �޽����� �������Լ� �°����� Ȯ���Ѵ�. 
	id = lpAgreeNewHostMsg->IDFrom;
	
	if(MyNet.NewHostReplySwitch[id]==FALSE)
	{
		MyNet.NewHostReplySwitch[id] = TRUE;
	}
	
	return TRUE;
}

BOOL GameChatProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	_ChatMsg*			lpChMsg;      
	int					id;
	char				buffer[128];


	//			pLock=new HQLock(&gCriticalSection);	
	
	lpChMsg= (_ChatMsg*) lpMsg;
	id = lpChMsg->GetPlayer();

	// ������� ä�� �޽����� ������ ���̶�� �����Ѵ�. 
	if(id == UserPlayerNumber)return FALSE;


	// ���Ϳ��� ������ �޽����� ���ٸ��� ǥ���Ѵ�.
	if(lpChMsg->IsAlly() == TRUE)
	{
		wsprintf(buffer, "%s:%s> ", MyNet.PlayersName.Name[id], Text.AllyPlayer.Get());
	}
	else
	{
		wsprintf(buffer, "%s> ", MyNet.PlayersName.Name[id]);
	}
	
	// �� ���� �ѹ���Ʈ�� �޽����� ������ ���� ������. 
	strcat(buffer, lpChMsg->GetStrDataPtr());
	
	// ��ο��� ������ �޽����̰ų� 
	if(lpChMsg->IsAlly() == FALSE
		|| (lpChMsg->IsAlly() == TRUE
		&& GD.IsFriendTo(id, UserPlayerNumber) == TRUE) )
	{
//		SetMessage(buffer);
		pGame->m_ChatList.Add(MyNet.PlayersName.Name[id],lpChMsg->GetStrDataPtr());
	}
	
	//			delete pLock;

	return TRUE;
}

BOOL ClientProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	return TRUE;
}



BOOL ClientOrderProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	int				id;
	LPMYMSG				lpClientOrderMsg;
	MYMSG				MyMsg;
	_OrderData        temporder;

	lpClientOrderMsg = (LPMYMSG) lpMsg;
	
	id = MyNet.GetIDFromInMyMsg(lpClientOrderMsg);
	
	CopyMemory(&MyMsg, lpClientOrderMsg, sizeof(GWMSG_GENERIC) + sizeof(UI32) + MyNet.GetDataSizeInMyMsg(lpClientOrderMsg));
	CopyMemory(&temporder, &MyMsg.data, sizeof(_OrderData));
	
	// ���� ȣ��Ʈ�̸� ó���� ���� ����� ���ۿ� �ִ´�.
	// �� ���ۿ� �־��� ����� ����� �ٸ� ����ڵ鿡�Է� ���۵Ǹ� ȣ��Ʈ �ڽ��� 
	// ������ ����� �����Ѵ�.
	PushUserOrderBuffer(temporder);

	return TRUE;
}



// ���� �޽����� �޴´�. 
BOOL MyProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	int					id;
	LPMYMSG				lpMyMsg ;
	MYMSG				MyMsg;
	_OrderData temporder;

	lpMyMsg	= (LPMYMSG) lpMsg;
	id		= MyNet.GetIDFromInMyMsg(lpMyMsg);

	if(MyNet.PlayersdpId.dpId[id])
	{
		
		MyNet.PushDPCheck(id, MyNet.GetFrameInMyMsg(lpMyMsg) ) ;
		
		CopyMemory(&MyMsg, lpMyMsg, sizeof(GWMSG_GENERIC) +  sizeof(UI32) + MyNet.GetDataSizeInMyMsg(lpMyMsg));
		
		MyNet.DecodeData(MyMsg.data, (char*)&temporder, MyNet.GetDataSizeInMyMsg(lpMyMsg));
		
		// �ڱ� ������� �Ϲ����� ����̸� ó������ �ʴ´�. 
		if(temporder.ODI.Order == ORDER_GETITEM || (temporder.PlayerNumber != UserPlayerNumber)) 
		{
			PushOrder(temporder);
		}
	}


	return TRUE;
}



BOOL BattleServerCheckProc(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	
	return TRUE;
}



// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
void _MyNetGame::DecideTransVary(SI32 latency)
{
	// �ִ� ������ ���� 

	MyNet.DPMaxFrameDiff = ( latency / MultiPlayDelayTime) + 1;
	if(MyNet.DPMaxFrameDiff > MAX_FRAME_DIFF) MyNet.DPMaxFrameDiff = MAX_FRAME_DIFF;
	if(MyNet.DPMaxFrameDiff < 4)MyNet.DPMaxFrameDiff = 4;

	// �޽����� �����ϴ� ���� 
	MyNet.DPFrameRateForTrans = MyNet.DPMaxFrameDiff / 3;
	if(MyNet.DPFrameRateForTrans < 1)MyNet.DPFrameRateForTrans = 1;				


}
