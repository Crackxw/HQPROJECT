//--------------------------------------------------------------------
//  ��� : ���°�
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

BOOL GoOutSwitch = FALSE;   // ���� �䱸�޾Ҵ°�?


// �����κ��� �� �޽����� �����ϴ� ����. 
SI32 OnlineBattleMsgIndex;
char OnlineBattleMsgBuffer[500][200];
SI32 OnlineBattleMsgSize[500];

// Ŭ���̾�Ʈ �޽����� ������ ������ ���� �����ϴ� ����. 
SI32 ClientBattleMsgIndex;
char ClientBattleMsgBuffer[500][200];
SI32 ClientBattleMsgSize[500];

//---------------------------------------------------------------------------
// _ChatMsg ���� 
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
// Desc: dplay ������ �ʱ�ȭ�Ѵ�. 
//--------------------------------------------------------------------------
void _MyNet::Init()
{
	SI32 i;

	// ��� ����ü�� �ʱ�ȭ�Ѵ�.
	ZeroMemory(this,   sizeof(_MyNet));


	// ������ ����ü�� ���¸� �����Ѵ�.

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


	MessageSendFrame	= 1000;              // ���� �޽����� ������ �ð�. 

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
// Desc: �����͸� ������ ���� �Լ� 
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




// ������ ä�� �޽����� ������. 
BOOL _MyNet::SendGameChatMessage(char* message, SHORT message_mode)
{
	// ���� �ѹ���Ʈ�� �������� ������ ���ΰ��� ���õ� ����. 
	ChatMsg.SetDataSize(lstrlen(message) + 1);
				
	// �������� ������ �޽����ΰ��� ���� ������ ���Եȴ�.
	if(message_mode == MESSAGEMODE_ALL)
		ChatMsg.SetAlly(0);
	else
		ChatMsg.SetAlly(1);

	// ���� ������ �޽����ΰ��� ������ ���Եȴ�. 
	ChatMsg.SetPlayer(UserPlayerNumber);
	
	// ä�� ������ �����Ѵ�. 
	ChatMsg.CopyStrData(message);

	SendGameMessage((LPGWMSG_GENERIC)&ChatMsg,  0);

	return TRUE;
}


// ���� �޽����� ������.
BOOL _MyNet::SendGameMessage(LPGWMSG_GENERIC lpMsg, DWORD dpid)
{
	int               nBytes;
	DWORD             dwFlags = 0;
	LPMYMSG           lpMyMsg, lpClientOrderMsg;
	_ChatMsg*         lpChMsg;
	LPNEWHOSTMSG       lpNewHostMsg;

	// �޽��� Ÿ�Կ� ���� ���� ����Ʈ���� ���Ѵ�.
	switch(lpMsg->dwType)
	{
		// ���ο� ȣ��Ʈ�� �������� ȣ��Ʈ���� �˸���. 
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


		// Ŭ���̾�Ʈ���� ȣ��Ʈ�� ����� �����Ѵ�. 
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

		// �¶��� �������� �÷��̾���� �߿� ������Ʈ ������ ������. 
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


        // �׹ۿ� ��ɿ����� ������ ũ�⸦ �����Ѵ�.
	case GAME_MSG_MY:
		{
			lpMyMsg = (LPMYMSG) lpMsg;
			
			if(GetDataSizeInMyMsg(lpMyMsg) > MY_MSG_DATA_SIZE)
			{
				SetMessage("overflow in message");
			}

			SetIDFromInMyMsg(lpMyMsg, UserPlayerNumber);
			
			nBytes = sizeof(GWMSG_GENERIC) + sizeof(UI32) + GetDataSizeInMyMsg(lpMyMsg);

			// Ư�� �÷��̾�Ը� ������� �����Ǿ� �ִٸ�, 
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
// Desc: �츮�� ���� �޽������� �ִ����� üũ�ϰ� ���������� ��ٸ��� �޴´�.
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

	// ����ڷκ��� �Էµ� ����� �ִٸ�,
	if(GD.OrderBufferIndex)
	{
		// ȣ��Ʈ�� �ƴϸ�, ȣ��Ʈ�� ����� ������. 
		temporder       = GD.OrderBuffer[0];
	
		// �ڱ� ������� �Ϲ����� ����̸� �ٷ� ó���Ѵ�.
		if(temporder.PlayerNumber == UserPlayerNumber)
		{
			PushOrder(temporder);
		}
			
		MYMSG MyMsg;
		MyMsg.dwType    = GAME_MSG_CLIENT_ORDER;
		SetFrameInMyMsg(&MyMsg, GD.Frame) ;// ���� ���� �������� �Ѱ��ش�.
		SetDataSizeInMyMsg(&MyMsg, sizeof(_OrderData)); 
		
		CopyMemory(&MyMsg.data, &temporder,sizeof(_OrderData));
		
		SendGameMessage((LPGWMSG_GENERIC)&MyMsg,  0);

		
		// ������� ����� �ʱ�ȭ�Ѵ�. 
		GD.PopOrderBuffer();
			
	}
	// ĳ������ ������ ������Ʈ �Ѵ�. 
	else if((GD.Frame + UserPlayerNumber)  % diff == 0)
	{
		// �� �����ӿ��� �޽����� �ѹ��� ���� �� �ִ�. 
		if(MessageSendFrame != GD.Frame)
		{

			// �޽����� ���� �ð��� �� �����Ѵ�. 
			MessageSendFrame  = GD.Frame;
			
			// ȥ���ϱⰡ �ƴϾ�� ������Ʈ�� �Ѵ�. 
			if(OBInfo.IsSingleMode == FALSE)
			{
				// ������Ʈ �ؾ��� ����� ������, 
				while(pGame->pBattle->PopUpdateCharInfo(&temporder))
				{
					MYMSG MyMsg;
					MyMsg.dwType    = GAME_MSG_CLIENT_ORDER;
					SetFrameInMyMsg(&MyMsg, GD.Frame) ;// ���� ���� �������� �Ѱ��ش�.
					SetDataSizeInMyMsg(&MyMsg, sizeof(_OrderData)); 
					
					CopyMemory(&MyMsg.data, &temporder,sizeof(_OrderData));
					
					SendGameMessage((LPGWMSG_GENERIC)&MyMsg,  0);
				}

				// ������Ʈ�� ĳ���͸� ã�´�.
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

	// �Է� ���� ���� DPCheck���� ������ �Է��� �� ����. 
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
// Desc: �ý��� �޽����� ó���ϴ� �Լ� 
//       ���� �޽����� �������α׷��� ���� ���̸� �װͿ� ���� ó���� �Ѵ�.
//-----------------------------------------------------------------------------


// ���� �޽����� �������α׷��� ���� ���̸� �װͿ� ���� ó���� �Ѵ�.
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
// Desc: ����ü�� dpId�� �ش��� �迭�� NULL�� �־� ���α׷� �������� 
//       �׳� �Ѿ�� �ְ��Ѵ�.
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
// Desc: ����ü�� ID�� ã�´�.
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
// Desc: ���� Logout �޽����� ó���Ѵ�.
//--------------------------------------------------------------------------
BOOL DoLogoutAction(DWORD dpId)
{
	int id;
	char buffer[128];

	if(dpId == 0)return FALSE;
	
	GetPlayerNumber(dpId, &id);
	wsprintf(buffer, "[%s]%s", MyNet.PlayersName.Name[id], Text.QuitGameMessage.Get());
	SetMessage(buffer);
	
	// ����ü�� �Ҹ�� dpId���� �����.
	DeletePlayerNumber(dpId);
	
	// ������ ���� �����ϴ� ������ �������� ����� ���̵� ������ ������ �ִ� 
	// DPID������ �����ָ� �ȴ�.
	// �� �̰��� �����������ؼ� ���� �̾��̵��� �޽����� �������� ����Ҽ��� 
	// �����Ƿ� �޽����� ����ϴ� ���������� ���� ������ ���̵� �����ϴ�����
	// ��� üũ�Ͽ����Ѵ�.

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ������� ����� ȣ��Ʈ�� ��� ���ۿ� �ִ´�. 
BOOL PushUserOrderBuffer(_OrderData order)
{
	if(UserOrderBufferIndex >= MAX_USER_ORDER_BUFFER_INDEX)return FALSE;

	UserOrderBuffer[UserOrderBufferIndex] = order;
	UserOrderBufferIndex ++ ;

	return TRUE;
}


// ȣ��Ʈ�� ����� ��� ���ۿ��� ����� �ϳ� ������. 
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

	// ��ȿ�� �÷��̾ üũ�Ѵ�. 
	if(GetPlayerNumber(dpid, &pnumber) == FALSE)return FALSE;

	DoLogoutAction(dpid);

	return TRUE;


}

// �¶��� �������� ���� ���� ��ȭ�� ���� ������ ������.
BOOL _MyNet::UpdatePlayerInfoToServer()
{
	_OnlineBattleMsg obm;
	if(OBMQue.Pop(&obm) == TRUE)
	{
		
		MYMSG MyMsg;
		ZeroMemory(&MyMsg, sizeof(MYMSG));
		MyMsg.dwType   = GAME_MSG_UPDATE;
		SetFrameInMyMsg(&MyMsg,		GD.Frame) ;// ���� ���� �������� �Ѱ��ش�.
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

// ���� ������ ���� �� �޽����� �����Ѵ�. 
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

// ���� �����κ��� �� �޽����� ���´�. 
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

// Ŭ���̾�Ʈ �޽����� ���Ӽ����� ������ ���� �����Ѵ�.
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

// Ŭ���̾�Ʈ �޽����� ���Ӽ����� ������ ���� pop�Ѵ�. 
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
