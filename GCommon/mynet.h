#ifndef _MYNET_H
#define _MYNET_H


#include "..\Gersang\order.h"
#include <player.h>


#define MAXNAMELEN 15                 // �Է¹��� �̸� �����Ұ��� �迭ũ�� 
#define MAX_GAME_LATENCY         1000   // ���ӿ����� �ִ� ���� �ð� 
#define MIN_GAME_LATENCY         100

#define MAX_MSG_TYPE_NUMBER      100	// ���̺귯������ ó���� �� �ִ� �޽����� ����. 	

// �� ��ȣ���� �ְ� �޴� �޽���  ���� (10100 ~ 10200)
#define GAME_MSG_MY              10103
#define GAME_MSG_CHAT            10105
#define GAME_MSG_CLIENT          10110
#define GAME_MSG_GOOUT           10112    // ���� �޽��� 
#define GAME_MSG_CLIENT_ORDER    10113   // Ŭ���̾�Ʈ�� ȣ��Ʈ�� ����� ������ �� ����ϴ� �޽��� 
#define GAME_MSG_CLIENTNO        10116
#define GAME_MSG_AGREENEWHOST    10117
#define GAME_MSG_UPDATE			 10118   // ����ڵ��� �߿� ������ ������Ʈ �ȴ�.
#define GAME_MSG_BATTLESERVERCHECK 10119

#define MAX_FRAME_DIFF         15   // �ִ� ������ ���� 

#define MY_MSG_DATA_SIZE     120        // ���� �������� ���� ������ _OrderData


#define MAX_USER_ORDER_BUFFER_INDEX    20


typedef struct
{
    DWORD       dwType;         // Message type
} GWMSG_GENERIC, FAR *LPGWMSG_GENERIC;


class _ChatMsg{
public:
	DWORD   dwType;
private:
	SI32   Data;         // ff:������ f: �޽����� ���� �÷��̾��� ���� f:���� �޽��� ���� ffff
	char   StrData[128];

public:
	VOID SetDataSize(SI32 size);
	SI32 GetDataSize();
	VOID CopyStrData(char* str);
	BOOL IsAlly();
	void SetAlly(char allymode);
	char* GetStrDataPtr(){return StrData;}
	SI32 GetTotalSize(){return ( sizeof(GWMSG_GENERIC) + sizeof(SI32) + GetDataSize());}
	VOID SetPlayer(SI32 player);
	SI32 GetPlayer();


};

typedef struct{
	DWORD  dwType;
	UI32   Data;	// ff:������ ������  f:�޽����� �����÷��̾�����. fffff:�޽����� ���� ������ ����.
	char   data[MY_MSG_DATA_SIZE];

}MYMSG, *LPMYMSG;

// new host 
typedef struct{
	DWORD  dwType;
	DWORD  HostID;
	DWORD PrevHostID;
}NEWHOSTMSG, *LPNEWHOSTMSG;

typedef struct{
	DWORD  dwType;
	DWORD  IDFrom;
}AGREENEWHOSTMSG, *LPAGREENEWHOSTMSG;

// ���� �޽��� 
typedef struct{
	DWORD	dwType;
	SI32	PlayerNumber;
}GOOUTMSG, *LPGOOUTMSG;

// ��ü �÷��̾���� DPID�� 
typedef struct
{
	DWORD  dwType;
	DWORD  dpId[MAX_MULTI_PLAYER];
	_PlayerInfo Player[MAX_MULTI_PLAYER];
	BOOL   bImReady[MAX_MULTI_PLAYER];
	char   RandomPos[MAX_MULTI_PLAYER];
	char   FileName[32];
	DWORD  Version;
	short  Res;
	short  ComAlly;
	short  ComNumber;         // ��ǻ�� �÷������ ���� 
	SHORT  GameMode;          // ���� ���
	SHORT  Resource;          // �ڿ��� ���� 
	SHORT  Land;              // ������ ���� 
	SHORT  Complicated;       // ������ ���⼺ 
	SHORT  MapSize;           // ������ ũ�� 
	SHORT  MapType;           // ������ ���� 
	DWORD  Latency;           // ���� ���ι��̴��� ���� 
	SHORT  UserMapSwitch;     // ����� ������ ������ ���ΰ�?
	unsigned short RandomSeed; 
    //////////
	char   PlayerName[MAX_MULTI_PLAYER][MAXNAMELEN];
	//////////

}PLAYERSDPIDMSG, *LPPLAYERSDPIDMSG;


typedef struct
{
	char   Name[MAX_MULTI_PLAYER][MAXNAMELEN];
}PLAYERSNAME, *LPPLAYERSNAME;


// ���� �޽����� �������α׷��� ���� ���̸� �װͿ� ���� ó���� �Ѵ�.
void DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);


// ����ü�� dpId�� �ش��� �迭�� NULL�� �־� ���α׷� �������� 
// �׳� �Ѿ�� �ְ��Ѵ�.
BOOL  DeletePlayerNumber(DWORD dpId);
// ����ü�� ID�� ã�´�.
BOOL  GetPlayerNumber(DWORD dpId,int *id);


// ������� ����� ȣ��Ʈ�� ��� ���ۿ� �ִ´�. 
BOOL PushUserOrderBuffer(_OrderData order);
// ȣ��Ʈ�� ����� ��� ���ۿ��� ����� �ϳ� ������. 
BOOL PopUserOrderBuffer(_OrderData * order);




// ���������� ó�� 
// ���������� �ٷ� ó���Ѵ�. 
BOOL DoLogoutAction(DWORD dpId);


class _MyNet {
public :
	// �޽����� ��ü�� �����. 
	_ChatMsg		  ChatMsg;                 // ���� ä�ø޽��� 
	GOOUTMSG          GoOutMsg;                    // ���� �޽��� 
	AGREENEWHOSTMSG   AgreeNewHostMsg;
	
	PLAYERSDPIDMSG    PlayersdpId;                 // �÷��̾���� ID�迭

	DWORD             DPCheck[MAX_MULTI_PLAYER];   // �� �÷��̾ �޽����� ������ �ֱ� ������. 

	DWORD MessageSendFrame;              // ���� �޽����� ������ �ð�. 

	// ���ο� ȣ��Ʈ�� �Ǿ��ٴ� ����� ���� Ŭ���̾�Ʈ�� �����ϴ� ������ �����Ѵ�.
	BOOL NewHostReplySwitch[MAX_MULTI_PLAYER];


	SI32 MsgType[MAX_MSG_TYPE_NUMBER];
	BOOL (*MsgFunc[MAX_MSG_TYPE_NUMBER])(LPGWMSG_GENERIC, DWORD , DWORD, DWORD);

	DWORD  HostPlayerDpId;		// ȣ��Ʈ�� ���̵� ��

	PLAYERSNAME       PlayersName;                 // �÷��̾��� �̸��� �����Ѵ�. 


	// �ִ� ������ �̰� 
	SI32 DPMaxFrameDiff;

	// �޽����� �����ϴ� ���� 
	SI32 DPFrameRateForTrans;

	//--------------------------------------------------------------------------
	// Name: InitMyNet()
	// Desc:  ������ �ʱ�ȭ�Ѵ�. 
	//--------------------------------------------------------------------------
	void Init();
	BOOL SetFunc(UI32 msg, BOOL (*func)(LPGWMSG_GENERIC, DWORD , DWORD, DWORD));
	SI32 FindFunc(UI32 msg);

	// ������ ä�� �޽����� ������. 
    BOOL SendGameChatMessage(char* message, SHORT message_mode);
	// ���� �޽����� ������.
    BOOL SendGameMessage(LPGWMSG_GENERIC lpMsg, DWORD dpid);

	//--------------------------------------------------------------------------
	// Name: WaitForMessageFrom() 
	// Desc: �츮�� ���� �޽������� �ִ����� üũ�ϰ� ���������� ��ٸ��� �޴´�.
	//--------------------------------------------------------------------------
	HRESULT WaitForMessageFrom(void);

	// ���ѵ� ������ �ȿ� �ִ��� Ȯ���Ѵ�. 
	BOOL PushDPCheck(SHORT id, DWORD frame);

	BOOL ReceiveMessages();
	// �ڽ��� Host Game Player ������ �˷��ش�.
	BOOL IsHost(void);
	// �����͸� ������ ���� �Լ� 
	BOOL SendMessage(DWORD dwFlags, LPVOID lpData, DWORD dwDataSize);

	SI32 GetDataSizeInMyMsg(LPMYMSG lpmsg);
	BOOL SetDataSizeInMyMsg(LPMYMSG lpmsg,  SI32 size);
	SI32 GetIDFromInMyMsg(LPMYMSG lpmsg);
	BOOL SetIDFromInMyMsg(LPMYMSG lpmsg, SI32 idfrom);
	UI32 GetFrameInMyMsg(LPMYMSG lpmsg);
	BOOL SetFrameInMyMsg(LPMYMSG lpmsg, UI32 frame);

	SI32 EncodeData(char* data, char* buffer, int size);
	SI32 DecodeData(char* data, char* buffer, int size);

	BOOL OperateHostQuit(DWORD dpid);

	// �¶��� �������� ���� ���� ��ȭ�� ���� ������ ������.
	BOOL UpdatePlayerInfoToServer();
};

// ���� ������ ���� �� �޽����� �����Ѵ�. 
BOOL PushOnlineBattleMsg(char* pmsg, SI32 size);
// ���� �����κ��� �� �޽����� ���´�. 
BOOL PopOnlineBattleMsg(char* pmsg, SI32* size);
// Ŭ���̾�Ʈ �޽����� ���Ӽ����� ������ ���� �����Ѵ�.
BOOL PushClientBattleMsg(char* pmsg, SI32 size);
// Ŭ���̾�Ʈ �޽����� ���Ӽ����� ������ ���� pop�Ѵ�. 
BOOL PopClientBattleMsg(char* pmsg, SI32* size);



#endif
