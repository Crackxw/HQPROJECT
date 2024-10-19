//--------------------------------------------------------------------
//  담당 : 김태곤
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
extern	SHORT				SoloPlaySwitch;			// 혼자 하기 모드인가 여럿이 하기 모드인가?
extern	int					MultiPlayDelayTime;

extern	int					FrameClockDelay; 
extern	OnlineBattleInfo	OBInfo;
extern	cltCharInterface	clifChar;							// 캐릭터 관련 인터페이스 

extern	SHORT				GameStatus;
extern	BOOL				GoOutSwitch;			// 강퇴를 요구받았는가를 알려주는 스위치 

//CLIENTDPIDMSG		ClientdpId[MAX_MULTI_PLAYER]; // 다른 클라이언트의 정보들 
BOOL				ClientId[MAX_MULTI_PLAYER];   // 다른 클라이언트가 정보를 보냈는지 알려준다.
DWORD				gdwClientStartTime = 0;      // Client에서 Start메시지를 첨 받았을때의 시간 

BOOL				gbGetMessageCurrent= FALSE;  // Thread에서 CURRENT_PLAYER의 정보를 받았다.
BOOL				gbGetMessageHost   = FALSE;  // Thread에서 HOST_MESSAGE의 정보를 받았다.

BOOL				gbStartWaiting     = FALSE;  // 게임시작 메시지를 받고 기다리고 있는가?
BOOL				gbGetMessageClient = FALSE;  // Thread에서 CLIENT_MESSAGE의 정보를 받았다.
DWORD				gdpIdHostID = 0;             // HOST_MESSAGE를 보낸 사람의 DP ID

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
		// Client의 데이터를 가지고 있는 배열 초기화 
//		ZeroMemory(&ClientdpId[i],sizeof(CLIENTDPIDMSG));
//		ClientdpId[i].dwType  = GAME_MSG_CLIENT;
		// Client의 정보를 받았음을 알려주는 배열 
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
// Desc: 각 player의 Ping을 구하여 가장 큰 값을 구한다.
//       이때 Delay은 다음과 같은 식으로 계산한다.
//--------------------------------------------------------------------------
int _MyNetGame::GetGameLatency()
{
	int i;

	// Default Latency 는 200으로 한다.
	DWORD max_ping_delay = MIN_GAME_LATENCY;


	// 최대 8명까지 각 Player의 Delay를 구한다.
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
		// 게임이 드랍되었음을 설정한다. 
		GameDropSwitch = TRUE;
	}
	// 강퇴하는  플레이어의 캐릭터를 모두 제거한다. 
	
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
	
	// 호스트 인정 메시지가 누구에게서 온것인지 확인한다. 
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

	// 사용자의 채팅 메시지가 도착한 것이라면 무시한다. 
	if(id == UserPlayerNumber)return FALSE;


	// 동맹에게 보내는 메시지는 색다르게 표시한다.
	if(lpChMsg->IsAlly() == TRUE)
	{
		wsprintf(buffer, "%s:%s> ", MyNet.PlayersName.Name[id], Text.AllyPlayer.Get());
	}
	else
	{
		wsprintf(buffer, "%s> ", MyNet.PlayersName.Name[id]);
	}
	
	// 맨 앞의 한바이트는 메시지의 유형에 대한 정보다. 
	strcat(buffer, lpChMsg->GetStrDataPtr());
	
	// 모두에게 보내는 메시지이거나 
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
	
	// 내가 호스트이면 처리를 위해 실행용 버퍼에 넣는다.
	// 이 버퍼에 넣어진 명령은 잠시후 다른 사용자들에게로 전송되며 호스트 자신은 
	// 실제로 명령을 실행한다.
	PushUserOrderBuffer(temporder);

	return TRUE;
}



// 게임 메시지를 받는다. 
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
		
		// 자기 사용자의 일반적인 명령이면 처리하지 않는다. 
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



// 지연을 근거로 각종 통신 변수들을 설정한다. 
void _MyNetGame::DecideTransVary(SI32 latency)
{
	// 최대 프레임 간격 

	MyNet.DPMaxFrameDiff = ( latency / MultiPlayDelayTime) + 1;
	if(MyNet.DPMaxFrameDiff > MAX_FRAME_DIFF) MyNet.DPMaxFrameDiff = MAX_FRAME_DIFF;
	if(MyNet.DPMaxFrameDiff < 4)MyNet.DPMaxFrameDiff = 4;

	// 메시지를 전송하는 간격 
	MyNet.DPFrameRateForTrans = MyNet.DPMaxFrameDiff / 3;
	if(MyNet.DPFrameRateForTrans < 1)MyNet.DPFrameRateForTrans = 1;				


}
