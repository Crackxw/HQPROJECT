//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
//#define INITGUID   


#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include "..\Gersang\maindata.h"
#include <etc.h>
#include "..\Gersang\YListButton.h"
#include "..\Gersang\GMenu.h"
#include "..\Gersang\text.h"
#include <OnlineImjin2i.h>
#include <OnlineBattleMsg.h>

#include <mynet.h>
#include <MyNet-Game.h>

extern _clGame* pGame;

extern	SI32				UserPlayerNumber;
extern	clock_t				CurrentClock;
extern	_MainData			GD;
extern	_Text				Text;
extern	BOOL				GameDropSwitch;
extern	_MyNetGame			MyNet;

extern	int					FrameClockDelay; 
extern	SHORT				GameStatus;

extern OnlineBattleInfo OBInfo;
extern _OnlineBattleMsgQue OBMQue;

// global

char			TempData[1024];

_OrderData		UserOrderBuffer[MAX_USER_ORDER_BUFFER_INDEX];
SHORT			UserOrderBufferIndex;

BOOL GoOutSwitch = FALSE;   // 강퇴를 요구받았는가?


// 서버로부터 온 메시지를 저장하는 공간. 
SI32 OnlineBattleMsgIndex;
char OnlineBattleMsgBuffer[500][200];
SI32 OnlineBattleMsgSize[500];

// 클라이언트 메시지를 서버로 보내기 위해 저장하는 공간. 
SI32 ClientBattleMsgIndex;
char ClientBattleMsgBuffer[500][200];
SI32 ClientBattleMsgSize[500];

//---------------------------------------------------------------------------
// _ChatMsg 관련 
//---------------------------------------------------------------------------
VOID _ChatMsg::SetDataSize(SI32 size)
{
	Data  = ((Data & 0x00ffffff) | (size<<24)) ;
}

SI32 _ChatMsg::GetDataSize()
{
	return ((Data & 0xff000000) >> 24) ;
}

VOID _ChatMsg::CopyStrData(char* str)
{
	strcpy(StrData, str);
}

BOOL _ChatMsg::IsAlly()
{
	if(((Data & 0x000f0000) >> 16))return TRUE;

	return FALSE;
}

void _ChatMsg::SetAlly(char allymode)
{
	Data = ((Data & 0xfff0ffff) | (allymode << 16)) ;
}

VOID _ChatMsg::SetPlayer(SI32 player)
{
	Data  = ((Data & 0xff0fffff) | (player << 20)) ;
}

SI32 _ChatMsg::GetPlayer()
{
	return ((Data & 0x00f00000) >> 20);
}

//--------------------------------------------------------------------------
// Name: InitDPlay()
// Desc: dplay 정보를 초기화한다. 
//--------------------------------------------------------------------------
void _MyNet::Init()
{
	SI32 i;

	// 모든 구조체를 초기화한다.
	ZeroMemory(this,   sizeof(_MyNet));


	// 설정한 구조체에 형태를 지정한다.

	ZeroMemory(&ChatMsg,sizeof(_ChatMsg));
	ChatMsg.dwType   = GAME_MSG_CHAT;

	ZeroMemory(&PlayersName,sizeof(PLAYERSNAME));
	ZeroMemory(DPCheck,sizeof(DWORD)*MAX_MULTI_PLAYER);

	ZeroMemory(&AgreeNewHostMsg,sizeof(AGREENEWHOSTMSG));
	AgreeNewHostMsg.dwType  = GAME_MSG_AGREENEWHOST;

	UserOrderBufferIndex = 0;
	HostPlayerDpId       = 0;

    for(i = 0; i < MAX_MULTI_PLAYER; i++)
	{

		PlayersdpId.Player[i].Init(i);			
		PlayersdpId.Player[i].PlayerNumber=(BYTE)i;			
	}


	MessageSendFrame	= 1000;              // 정상 메시지가 보내진 시간. 

	OnlineBattleMsgIndex = 0;
	ClientBattleMsgIndex = 0;
}


BOOL _MyNet::SetFunc(UI32 msg, BOOL (*func)(LPGWMSG_GENERIC, DWORD , DWORD, DWORD))
{

	SI32 i;

	for(i = 1;i < MAX_MSG_TYPE_NUMBER;i++)
	{
		if(MsgType[i] == 0)
		{
			MsgType[i] = msg;
			MsgFunc[i] = func;
			
			return TRUE;
		}

		if(MsgType[i] == (int)msg)
		{
			return FALSE;
		}
	}

	return FALSE;

}

SI32 _MyNet::FindFunc(UI32 msg)
{

	SI32 i;

	for(i = 1;i < MAX_MSG_TYPE_NUMBER;i++)
	{
		if(MsgType[i] == (int)msg)
		{
			return i;
		}
	}
	return 0;

}



BOOL _MyNet::ReceiveMessages()
{
	char pmsg[1024];
	SI32 size = 0;

	if(PopOnlineBattleMsg(pmsg, &size) == TRUE)
	{
		DoApplicationMessage((LPGWMSG_GENERIC) pmsg, size, 0, 0);
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: SendMessage()
// Desc: 데이터를 보내기 위한 함수 
//-----------------------------------------------------------------------------
 
BOOL _MyNet::SendMessage(/*DWORD idTo, */DWORD dwFlags, LPVOID lpData, DWORD dwDataSize)
{
	
	if(dwDataSize >= 160)
	{
		clGrp.Error("Message Size Overflow", "MessageSize : [%d]", dwDataSize);
		return FALSE;
	}

	
	LPGWMSG_GENERIC lp = (LPGWMSG_GENERIC)lpData; 
	if(lp->dwType  <= 0)
	{
		char buffer[128];
		wsprintf(buffer, "`1111111111111- %d %d", lp->dwType, dwDataSize);
		SetMessage(buffer);
	}

	return PushClientBattleMsg((char*)lpData, dwDataSize);
	
}




// 게임중 채팅 메시지를 보낸다. 
BOOL _MyNet::SendGameChatMessage(char* message, SHORT message_mode)
{
	// 최초 한바이트는 누구에게 보내는 것인가와 관련된 공간. 
	ChatMsg.SetDataSize(lstrlen(message) + 1);
				
	// 누구에게 보내는 메시지인가에 대한 정보가 포함된다.
	if(message_mode == MESSAGEMODE_ALL)
		ChatMsg.SetAlly(0);
	else
		ChatMsg.SetAlly(1);

	// 누가 보내는 메시지인가의 정보가 포함된다. 
	ChatMsg.SetPlayer(UserPlayerNumber);
	
	// 채팅 정보를 복사한다. 
	ChatMsg.CopyStrData(message);

	SendGameMessage((LPGWMSG_GENERIC)&ChatMsg,  0);

	return TRUE;
}


// 게임 메시지를 보낸다.
BOOL _MyNet::SendGameMessage(LPGWMSG_GENERIC lpMsg, DWORD dpid)
{
	int               nBytes;
	DWORD             dwFlags = 0;
	LPMYMSG           lpMyMsg, lpClientOrderMsg;
	_ChatMsg*         lpChMsg;
	LPNEWHOSTMSG       lpNewHostMsg;

	// 메시지 타입에 따라 전송 바이트수를 정한다.
	switch(lpMsg->dwType)
	{
		// 새로운 호스트를 인정함을 호스트에게 알린다. 
	case GAME_MSG_AGREENEWHOST:
		{

			lpNewHostMsg = (LPNEWHOSTMSG)lpMsg;
			nBytes       = sizeof(AGREENEWHOSTMSG);

			if( SendMessage(dwFlags, (LPVOID)lpMsg, nBytes) == FALSE ) 
			{
				return FALSE;
			}

		}
		break;


		// 클라이언트에서 호스트로 명령을 전달한다. 
	case GAME_MSG_CLIENT_ORDER:
		{

			lpClientOrderMsg = (LPMYMSG) lpMsg;
			
			if(GetDataSizeInMyMsg(lpClientOrderMsg) > MY_MSG_DATA_SIZE)
			{
				SetMessage("overflow in message");
			}

			SetIDFromInMyMsg(lpClientOrderMsg, UserPlayerNumber);
			
			nBytes = sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpClientOrderMsg);

			if( SendMessage(dwFlags, (LPVOID)lpMsg, nBytes) == FALSE ) 
			{
				return FALSE;
			}

		}
		break;

		// 온라인 전투에서 플레이어들의 중요 업데이트 내용을 보낸다. 
	case GAME_MSG_UPDATE:
		{

			lpClientOrderMsg = (LPMYMSG) lpMsg;
			
			if(GetDataSizeInMyMsg(lpClientOrderMsg) > MY_MSG_DATA_SIZE)
			{
				SetMessage("overflow in message");
			}
			
			nBytes = sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpClientOrderMsg);
			
			SendMessage(dwFlags, (LPVOID)lpMsg, nBytes);
		}
		break;


        // 그밖에 명령에서는 각각의 크기를 설정한다.
	case GAME_MSG_MY:
		{
			lpMyMsg = (LPMYMSG) lpMsg;
			
			if(GetDataSizeInMyMsg(lpMyMsg) > MY_MSG_DATA_SIZE)
			{
				SetMessage("overflow in message");
			}

			SetIDFromInMyMsg(lpMyMsg, UserPlayerNumber);
			
			nBytes = sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpMyMsg);

			// 특정 플레이어에게만 보내기로 지정되어 있다면, 
			if( SendMessage(dwFlags, (LPVOID)lpMsg, nBytes) == FALSE ) 
			{
				return FALSE;
			}
		}
		
		break;
	
	case GAME_MSG_CHAT:
		{
			lpChMsg = (_ChatMsg*) lpMsg;
			nBytes = lpChMsg->GetTotalSize();

			if( SendMessage(dwFlags, (LPVOID)lpMsg, nBytes) == FALSE ) 
			{
				return FALSE;
			}

		}
		break;
	}
	

	return TRUE;
	
}


//--------------------------------------------------------------------------
// Name: WaitForMessageFrom() 
// Desc: 우리를 위한 메시지들이 있는지를 체크하고 있을때까지 기다린후 받는다.
//--------------------------------------------------------------------------
HRESULT _MyNet::WaitForMessageFrom(void)
{
	_OrderData temporder;

	SI32 diff = 1;

	if(IsHost() == TRUE)
	{
		diff = 5;
	}
	else
	{
		diff = 10;
	}

	// 사용자로부터 입력된 명령이 있다면,
	if(GD.OrderBufferIndex)
	{
		// 호스트가 아니면, 호스트로 명령을 보낸다. 
		temporder       = GD.OrderBuffer[0];
	
		// 자기 사용자의 일반적인 명령이면 바로 처리한다.
		if(temporder.PlayerNumber == UserPlayerNumber)
		{
			PushOrder(temporder);
		}
			
		MYMSG MyMsg;
		MyMsg.dwType    = GAME_MSG_CLIENT_ORDER;
		SetFrameInMyMsg(&MyMsg, GD.Frame) ;// 현재 나의 프레임을 넘겨준다.
		SetDataSizeInMyMsg(&MyMsg, sizeof(_OrderData)); 
		
		CopyMemory(&MyMsg.data, &temporder,sizeof(_OrderData));
		
		SendGameMessage((LPGWMSG_GENERIC)&MyMsg,  0);

		
		// 사용자의 명령을 초기화한다. 
		GD.PopOrderBuffer();
			
	}
	// 캐릭터의 정보를 업데이트 한다. 
	else if((GD.Frame + UserPlayerNumber)  % diff == 0)
	{
		// 한 프레임에는 메시지를 한번만 보낼 수 있다. 
		if(MessageSendFrame != GD.Frame)
		{

			// 메시지를 보낸 시간을 재 설정한다. 
			MessageSendFrame  = GD.Frame;
			
			// 혼자하기가 아니어야 업데이트를 한다. 
			if(OBInfo.IsSingleMode == FALSE)
			{
				// 업데이트 해야할 대상이 있으면, 
				while(pGame->pBattle->PopUpdateCharInfo(&temporder))
				{
					MYMSG MyMsg;
					MyMsg.dwType    = GAME_MSG_CLIENT_ORDER;
					SetFrameInMyMsg(&MyMsg, GD.Frame) ;// 현재 나의 프레임을 넘겨준다.
					SetDataSizeInMyMsg(&MyMsg, sizeof(_OrderData)); 
					
					CopyMemory(&MyMsg.data, &temporder,sizeof(_OrderData));
					
					SendGameMessage((LPGWMSG_GENERIC)&MyMsg,  0);
				}

				// 업데이트할 캐릭터를 찾는다.
				SI32 updateid = pGame->pBattle->FindUpdateCharInfoChar();

				if(updateid == 0)
				{
					pGame->pBattle->LastUpdateCharInfo = 0;
				}

				
				if(updateid)
				{
					pGame->pBattle->PushUpdateCharInfo(updateid);
				}


			}
		}
	}

	return TRUE;
}


//--------------------------------------------------------------------------
// Name: PushDPCheck()
// Desc: ?????
//--------------------------------------------------------------------------
BOOL _MyNet::PushDPCheck(SHORT id, DWORD frame)
{

	// 입력 받은 값이 DPCheck보다 작으면 입력할 수 없다. 
	if(DPCheck[id] >= frame)
	{
		//clGrp.Error("PushDPCheck", "108236:[%d][%d]", DPCheck[id], frame);
		return FALSE;
	}
	else
	{
		DPCheck[id] = frame;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: DoApplicationMessage()
// Desc: 시스템 메시지를 처리하는 함수 
//       받은 메시지가 응용프로그램을 위한 것이면 그것에 대한 처리를 한다.
//-----------------------------------------------------------------------------


// 받은 메시지가 응용프로그램을 위한 것이면 그것에 대한 처리를 한다.
void DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo)
{
	SI32 index = 0;

	index = MyNet.FindFunc(lpMsg->dwType);
	
	if(index)
	{
		(MyNet.MsgFunc[index])(lpMsg, dwMsgSize,  idFrom, idTo);
	}
	else
	{
		clGrp.Error("fsd9327", "fd325[%d]", lpMsg->dwType);
	}

}

//--------------------------------------------------------------------------
// Name: DeletePlayerNumber()
// Desc: 구조체에 dpId를 해당한 배열에 NULL을 넣어 프로그램 루프에서 
//       그냥 넘어갈수 있게한다.
//--------------------------------------------------------------------------
BOOL  DeletePlayerNumber(DWORD dpId)
{
	int i;
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(MyNet.PlayersdpId.dpId[i]==dpId)
		{
			MyNet.PlayersdpId.dpId[i]                  = NULL;
			MyNet.PlayersdpId.Player[i].Country        = COUNTRY_NO;
			MyNet.PlayersdpId.Player[i].Host           = BY_COMPUTER;		
			MyNet.PlayersdpId.bImReady[i]              = FALSE;			
			return TRUE;
		}
	}
	return FALSE;
}


//--------------------------------------------------------------------------
// Name: GetPlayerNumber()
// Desc: 구조체에 ID를 찾는다.
//--------------------------------------------------------------------------
BOOL  GetPlayerNumber(DWORD dpId,int *id)
{
	int i;
	for(i=0;i<MAX_MULTI_PLAYER;i++)
	{
		if(MyNet.PlayersdpId.dpId[i]==dpId)
		{
			*id = i;
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------
// Name: DoLogoutAction()
// Desc: 직접 Logout 메시지를 처리한다.
//--------------------------------------------------------------------------
BOOL DoLogoutAction(DWORD dpId)
{
	int id;
	char buffer[128];

	if(dpId == 0)return FALSE;
	
	GetPlayerNumber(dpId, &id);
	wsprintf(buffer, "[%s]%s", MyNet.PlayersName.Name[id], Text.QuitGameMessage.Get());
	SetMessage(buffer);
	
	// 구조체에 소멸된 dpId값을 지운다.
	DeletePlayerNumber(dpId);
	
	// 해제는 아주 간단하다 세션을 빠져나간 사람의 아이디를 각자의 가지고 있는 
	// DPID값에서 지워주면 된다.
	// 단 이값을 지워줌으로해서 순간 이아이디의 메시지를 받을려고 대기할수가 
	// 있으므로 메시지를 대기하는 루프에서는 필히 현재의 아이디가 존재하는지를
	// 계속 체크하여야한다.

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 사용자의 명령을 호스트의 명령 버퍼에 넣는다. 
BOOL PushUserOrderBuffer(_OrderData order)
{
	if(UserOrderBufferIndex >= MAX_USER_ORDER_BUFFER_INDEX)return FALSE;

	UserOrderBuffer[UserOrderBufferIndex] = order;
	UserOrderBufferIndex ++ ;

	return TRUE;
}


// 호스트의 사용자 명령 버퍼에서 명령을 하나 꺼낸다. 
BOOL PopUserOrderBuffer(_OrderData * order)
{
	int i;

	if(UserOrderBufferIndex == 0) return FALSE;

	*order = UserOrderBuffer[0];

	for(i = 1 ;i < UserOrderBufferIndex ;i++)
	{
		UserOrderBuffer[i - 1] = UserOrderBuffer[i];
	}

	UserOrderBufferIndex --;

	return TRUE;
}

SI32 _MyNet::EncodeData(char* data, char* buffer, int size)
{
	int i;
	int count =0;
	int index = 0;

	memcpy(buffer, data, size);
	return size;

	for(i=0;i<size;i++)
	{

		if(data[i])
		{
			if(count)
			{
				buffer[index] = 0;
				index++;
				buffer[index] = count;
				index++;

				count=0;
			}

			buffer[index] = data[i];
			index++;

		}
		else
		{
			count++;
		}
	}

	{
		char buffer[128];
		wsprintf(buffer, "%d", index);
		SetMessage(buffer);
	}

	return index;



}

SI32 _MyNet::DecodeData(char* data, char* buffer, int size)
{
	int i, j;
	int count =0;
	int index = 0;
	

	memcpy(buffer, data, size);
	return size;

	for(i=0;i<size;i++)
	{

		if(data[i])
		{
			buffer[index] = data[i];
			index++;

		}
		else
		{
			i++;
			count = data[i];
			for(j=0;j<count;j++)
			{
				buffer[index]=0;
				index++;
			}
		}
	}

	return index;
}





SI32 _MyNet::GetDataSizeInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0xff000000) >> 24) ;
	}

	return 0;
}

BOOL _MyNet::SetDataSizeInMyMsg(LPMYMSG lpmsg,  SI32 size)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0x00ffffff) | (size<<24)) ;
		return TRUE;
	}

	return FALSE;
}


SI32 _MyNet::GetIDFromInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return ((lpmsg->Data & 0x00f00000) >> 20);
	}

	return 0;
}

BOOL _MyNet::SetIDFromInMyMsg(LPMYMSG lpmsg, SI32 idfrom)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xff0fffff) | (idfrom<<20)) ;
		return TRUE;
	}

	return FALSE;
}

UI32 _MyNet::GetFrameInMyMsg(LPMYMSG lpmsg)
{
	if(lpmsg)
	{
		return (lpmsg->Data & 0x000fffff);
	}

	return 0;
}

BOOL _MyNet::SetFrameInMyMsg(LPMYMSG lpmsg, UI32 frame)
{
	if(lpmsg)
	{
		lpmsg->Data  = ((lpmsg->Data & 0xfff00000) | frame) ;
		return TRUE;
	}

	return FALSE;
}


BOOL _MyNet::OperateHostQuit(DWORD dpid)
{
	SI32 pnumber;

	// 유효한 플레이어만 체크한다. 
	if(GetPlayerNumber(dpid, &pnumber) == FALSE)return FALSE;

	DoLogoutAction(dpid);

	return TRUE;


}

// 온라인 전투에서 얻은 정보 변화를 전투 서버로 보낸다.
BOOL _MyNet::UpdatePlayerInfoToServer()
{
	_OnlineBattleMsg obm;
	if(OBMQue.Pop(&obm) == TRUE)
	{
		
		MYMSG MyMsg;
		ZeroMemory(&MyMsg, sizeof(MYMSG));
		MyMsg.dwType   = GAME_MSG_UPDATE;
		SetFrameInMyMsg(&MyMsg,		GD.Frame) ;// 현재 나의 프레임을 넘겨준다.
		SetDataSizeInMyMsg(&MyMsg,	sizeof(_OnlineBattleMsg)); 
		SetIDFromInMyMsg(&MyMsg,	UserPlayerNumber);
		
		CopyMemory(&MyMsg.data,		&obm, sizeof(_OnlineBattleMsg));
		
		SendGameMessage((LPGWMSG_GENERIC)&MyMsg,  0);
	}
	return TRUE;
}


BOOL _MyNet::IsHost()
{

	SI32 i;
	
	for(i = 0;i < MAX_MULTI_PLAYER;i++)
	{
		if(PlayersdpId.dpId[i] && ISPLAYER(PlayersdpId.dpId[i]) == TRUE )
		{
			if(i == UserPlayerNumber)
			{
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	
	return FALSE;
}

// 게임 서버로 부터 온 메시지를 저장한다. 
BOOL PushOnlineBattleMsg(char* pmsg, SI32 size)
{
	if(OnlineBattleMsgIndex < 500 - 1)
	{
		if(size>=200)
		{
			clGrp.Error("over flow", "size:[%d]", size);
			return FALSE;
		}

		memcpy(OnlineBattleMsgBuffer[OnlineBattleMsgIndex], pmsg, size);
		OnlineBattleMsgSize[OnlineBattleMsgIndex] = size;

		OnlineBattleMsgIndex++;
		return TRUE;
	}

	return FALSE;

}

// 게임 서버로부터 온 메시지를 얻어온다. 
BOOL PopOnlineBattleMsg(char* pmsg, SI32* size)
{
	SI32 i;

	if(OnlineBattleMsgIndex == 0)return FALSE;

	memcpy(pmsg, OnlineBattleMsgBuffer[0], OnlineBattleMsgSize[0]);
	*size = OnlineBattleMsgSize[0];

	for(i = 1;i < OnlineBattleMsgIndex;i++)
	{
		memcpy(OnlineBattleMsgBuffer[i-1], OnlineBattleMsgBuffer[i], OnlineBattleMsgSize[i]);
		OnlineBattleMsgSize[i-1] = OnlineBattleMsgSize[i];
	}

	OnlineBattleMsgIndex--;

	return TRUE;
}

// 클라이언트 메시지를 게임서버에 보내기 위해 저장한다.
BOOL PushClientBattleMsg(char* pmsg, SI32 size)
{

	if(ClientBattleMsgIndex < 500 - 1)
	{
		if(size>=200)
		{
			clGrp.Error("over flow", "size:[%d]", size);
			return FALSE;
		}

		memcpy(ClientBattleMsgBuffer[ClientBattleMsgIndex], pmsg, size);
		ClientBattleMsgSize[ClientBattleMsgIndex] = size;

		ClientBattleMsgIndex++;
		return TRUE;
	}

	return FALSE;

}

// 클라이언트 메시지를 게임서버에 보내기 위해 pop한다. 
BOOL PopClientBattleMsg(char* pmsg, SI32* size)
{
	SI32 i;

	if(ClientBattleMsgIndex == 0)return FALSE;

	memcpy(pmsg, ClientBattleMsgBuffer[0], ClientBattleMsgSize[0]);
	*size = ClientBattleMsgSize[0];

	for(i = 1;i < ClientBattleMsgIndex;i++)
	{
		memcpy(ClientBattleMsgBuffer[i-1], ClientBattleMsgBuffer[i], ClientBattleMsgSize[i]);
		ClientBattleMsgSize[i-1] = ClientBattleMsgSize[i];
	}
	ClientBattleMsgIndex--;

	return TRUE;


}
