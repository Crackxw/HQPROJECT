#ifndef _MYNET_H
#define _MYNET_H


#include "..\Gersang\order.h"
#include <player.h>


#define MAXNAMELEN 15                 // 입력받은 이름 저장할곳의 배열크기 
#define MAX_GAME_LATENCY         1000   // 게임에서의 최대 지연 시간 
#define MIN_GAME_LATENCY         100

#define MAX_MSG_TYPE_NUMBER      100	// 라이브러리에서 처리할 수 있는 메시지의 갯수. 	

// 컴 상호간에 주고 받는 메시지  정의 (10100 ~ 10200)
#define GAME_MSG_MY              10103
#define GAME_MSG_CHAT            10105
#define GAME_MSG_CLIENT          10110
#define GAME_MSG_GOOUT           10112    // 강퇴 메시지 
#define GAME_MSG_CLIENT_ORDER    10113   // 클라이언트가 호스트에 명령을 전달할 때 사용하는 메시지 
#define GAME_MSG_CLIENTNO        10116
#define GAME_MSG_AGREENEWHOST    10117
#define GAME_MSG_UPDATE			 10118   // 사용자들의 중요 정보가 업데이트 된다.
#define GAME_MSG_BATTLESERVERCHECK 10119

#define MAX_FRAME_DIFF         15   // 최대 프레임 차이 

#define MY_MSG_DATA_SIZE     120        // 게임 데이터의 실제 사이즈 _OrderData


#define MAX_USER_ORDER_BUFFER_INDEX    20


typedef struct
{
    DWORD       dwType;         // Message type
} GWMSG_GENERIC, FAR *LPGWMSG_GENERIC;


class _ChatMsg{
public:
	DWORD   dwType;
private:
	SI32   Data;         // ff:사이즈 f: 메시지를 보낸 플레이어의 정보 f:동맹 메시지 여부 ffff
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
	UI32   Data;	// ff:데이터 사이즈  f:메시지를 보낸플레이어정보. fffff:메시지를 보낸 프레임 정보.
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

// 강퇴 메시지 
typedef struct{
	DWORD	dwType;
	SI32	PlayerNumber;
}GOOUTMSG, *LPGOOUTMSG;

// 전체 플레이어들의 DPID들 
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
	short  ComNumber;         // 컴퓨터 플레어어의 갯수 
	SHORT  GameMode;          // 게임 방식
	SHORT  Resource;          // 자원의 상태 
	SHORT  Land;              // 지형의 상태 
	SHORT  Complicated;       // 지형의 복잡성 
	SHORT  MapSize;           // 지도의 크기 
	SHORT  MapType;           // 지도의 종류 
	DWORD  Latency;           // 서비스 프로바이더의 지연 
	SHORT  UserMapSwitch;     // 사용자 지도를 선택할 것인가?
	unsigned short RandomSeed; 
    //////////
	char   PlayerName[MAX_MULTI_PLAYER][MAXNAMELEN];
	//////////

}PLAYERSDPIDMSG, *LPPLAYERSDPIDMSG;


typedef struct
{
	char   Name[MAX_MULTI_PLAYER][MAXNAMELEN];
}PLAYERSNAME, *LPPLAYERSNAME;


// 받은 메시지가 응용프로그램을 위한 것이면 그것에 대한 처리를 한다.
void DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize, DWORD idFrom, DWORD idTo);


// 구조체에 dpId를 해당한 배열에 NULL을 넣어 프로그램 루프에서 
// 그냥 넘어갈수 있게한다.
BOOL  DeletePlayerNumber(DWORD dpId);
// 구조체에 ID를 찾는다.
BOOL  GetPlayerNumber(DWORD dpId,int *id);


// 사용자의 명령을 호스트의 명령 버퍼에 넣는다. 
BOOL PushUserOrderBuffer(_OrderData order);
// 호스트의 사용자 명령 버퍼에서 명령을 하나 꺼낸다. 
BOOL PopUserOrderBuffer(_OrderData * order);




// 접속해제의 처리 
// 접속해제의 바로 처리한다. 
BOOL DoLogoutAction(DWORD dpId);


class _MyNet {
public :
	// 메시지의 본체를 만든다. 
	_ChatMsg		  ChatMsg;                 // 전송 채팅메시지 
	GOOUTMSG          GoOutMsg;                    // 강퇴 메시지 
	AGREENEWHOSTMSG   AgreeNewHostMsg;
	
	PLAYERSDPIDMSG    PlayersdpId;                 // 플레이어들의 ID배열

	DWORD             DPCheck[MAX_MULTI_PLAYER];   // 각 플레이어가 메시지를 보내온 최근 프레임. 

	DWORD MessageSendFrame;              // 정상 메시지가 보내진 시간. 

	// 새로운 호스트가 되었다는 사실을 받은 클라이언트가 응답하는 정보를 저장한다.
	BOOL NewHostReplySwitch[MAX_MULTI_PLAYER];


	SI32 MsgType[MAX_MSG_TYPE_NUMBER];
	BOOL (*MsgFunc[MAX_MSG_TYPE_NUMBER])(LPGWMSG_GENERIC, DWORD , DWORD, DWORD);

	DWORD  HostPlayerDpId;		// 호스트의 아이디 값

	PLAYERSNAME       PlayersName;                 // 플레이어의 이름을 저장한다. 


	// 최대 프레임 이격 
	SI32 DPMaxFrameDiff;

	// 메시지를 전송하는 간격 
	SI32 DPFrameRateForTrans;

	//--------------------------------------------------------------------------
	// Name: InitMyNet()
	// Desc:  정보를 초기화한다. 
	//--------------------------------------------------------------------------
	void Init();
	BOOL SetFunc(UI32 msg, BOOL (*func)(LPGWMSG_GENERIC, DWORD , DWORD, DWORD));
	SI32 FindFunc(UI32 msg);

	// 게임중 채팅 메시지를 보낸다. 
    BOOL SendGameChatMessage(char* message, SHORT message_mode);
	// 게임 메시지를 보낸다.
    BOOL SendGameMessage(LPGWMSG_GENERIC lpMsg, DWORD dpid);

	//--------------------------------------------------------------------------
	// Name: WaitForMessageFrom() 
	// Desc: 우리를 위한 메시지들이 있는지를 체크하고 있을때까지 기다린후 받는다.
	//--------------------------------------------------------------------------
	HRESULT WaitForMessageFrom(void);

	// 제한된 프레임 안에 있는지 확인한다. 
	BOOL PushDPCheck(SHORT id, DWORD frame);

	BOOL ReceiveMessages();
	// 자신이 Host Game Player 인지를 알려준다.
	BOOL IsHost(void);
	// 데이터를 보내기 위한 함수 
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

	// 온라인 전투에서 얻은 정보 변화를 전투 서버로 보낸다.
	BOOL UpdatePlayerInfoToServer();
};

// 게임 서버로 부터 온 메시지를 저장한다. 
BOOL PushOnlineBattleMsg(char* pmsg, SI32 size);
// 게임 서버로부터 온 메시지를 얻어온다. 
BOOL PopOnlineBattleMsg(char* pmsg, SI32* size);
// 클라이언트 메시지를 게임서버에 보내기 위해 저장한다.
BOOL PushClientBattleMsg(char* pmsg, SI32 size);
// 클라이언트 메시지를 게임서버에 보내기 위해 pop한다. 
BOOL PopClientBattleMsg(char* pmsg, SI32* size);



#endif
