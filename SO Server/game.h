// Latest Update 2002/6/26, 10:22

#ifndef _GAME_H
#define _GAME_H

// 컴 상호간에 주고 받는 메시지  정의 (0x100 ~ 0x200)
#define GAME_MSG_MY					10103
#define GAME_MSG_CHAT				10105
#define GAME_MSG_CLIENT				10110
#define GAME_MSG_GOOUT				10112	   // 강퇴 메시지 
#define GAME_MSG_CLIENT_ORDER		10113   // 클라이언트가 호스트에 명령을 전달할 때 사용하는 메시지 
#define GAME_MSG_CLIENTNO			10116
#define GAME_MSG_AGREENEWHOST		10117
#define GAME_MSG_UPDATE				10118   // 사용자들의 중요 정보가 업데이트 된다.
#define GAME_MSG_BATTLESERVERCHECK  10119   // 전투 서버가 클라이언트의 생존 여부를 확인하기 위해서 보내는 메시지. 

#define MAX_DATA_NUMBER					100
#define MAX_DATA_SIZE					240
#define DEFAULT_MULTI_PLAY_DELAY_TIME	50
#define MAX_FRAME_DIFF					15   // 최대 프레임 차이 

#define MAX_MULTI_PLAYER				8
#define MAXNAMELEN						15
#define MAX_SELECTED_CHAR_NUMBER		10


#define BY_PLAYER     0		
#define BY_COMPUTER   1

#define MY_MSG_DATA_SIZE     120        // 게임 데이터의 실제 사이즈 _OrderData

#define MAX_NOMSG_BACKUP_STORAGE		8   // GAME_MSG_NO에는 과거 프레임의 정보도 함께 저장된다. 
											// 과거 프레임의 최대 숫자. 

#define MAX_USER_ORDER_BUFFER_INDEX    20
#define MAX_SYNC_SIGNAL_NUMBER         15

// 선택가능한 국가의 종류 
#define COUNTRY_NO        0      // 등장하지 않음 
#define COUNTRY_KOREA     1      // 조선 
#define COUNTRY_JAPAN     2      // 일본 
#define COUNTRY_CHINA     3      // 명
#define	COUNTRY_KORYU	  4		 // 고려.
#define COUNTRY_RANDOM    5      // 랜덤국 
#define COUNTRY_NEUTRAL   6      // 중립국

#include <OnlineBattleMsg.h>

typedef struct
{
    DWORD       dwType;         // Message type
} GWMSG_GENERIC, FAR *LPGWMSG_GENERIC;

/*
typedef struct{
	DWORD   dwType;
} MSG_GENERIC, *LPMSG_GENERIC;
  */

typedef struct{
	DWORD  dwType;
	UI32   Data;  //f:동기시그널  ff:플레이어  fffff:프레임. 
}CLIENTNOMSG, *LPCLIENTNOMSG;

// 강퇴 메시지 
typedef struct{
	DWORD	dwType;
	SI32	PlayerNumber;
}GOOUTMSG, *LPGOOUTMSG;

typedef struct{
	DWORD  dwType;
	UI32   Data;	// ff:데이터 사이즈  f:메시지를 보낸플레이어정보. fffff:메시지를 보낸 프레임 정보.
	char   data[MY_MSG_DATA_SIZE];

}MYMSG, *LPMYMSG;

// ff:nomsg를 받은 이전프레임 정보 f:메시지를 보낸플레이어정보. fffff:메시지를 보낸 프레임 정보. 
typedef struct{
	DWORD  dwType;
	UI32   Data;
}NOMSG, *LPNOMSG;

// 상대로부터 메시지가 도착하지 않을때 다시 보내줄 것을 요구하는 메시지 
typedef struct{
	DWORD  dwType;
	DWORD  IDFrom;
	DWORD  dwMyFrame;	// 백업을 요청하는 컴의 프레임. 
	DWORD  dwFrame;		// 프레임 (백업이 필요한 프레임.)
}BACKUPREQUESTMSG, *LPBACKUPREQUESTMSG;

class _ChatMsg{
public:
	DWORD   dwType;
private:
	SI32   Data;         // ff:사이즈 f: 메시지를 보낸 플레이어의 정보 f:동맹 메시지 여부 ffff
	char   StrData[60];

public:
	VOID SetDataSize(SI32 size);
	SI32 GetDataSize();
	VOID CopyStrData(char* str);
	BOOL IsAlly();
	void SetAlly(char allymode);
	char* GetStrDataPtr(){return StrData;}
	SI32 GetTotalSize(){return ( sizeof(GWMSG_GENERIC) + sizeof(SI32) + GetDataSize());}

};

class _PlayerInfo
{
public:

	unsigned char Country;
	unsigned char PlayerNumber;
	unsigned char Host;
	unsigned char MyBit;
	unsigned char EnemyBit;
	unsigned char PlayerGroup;

	void Init(SHORT pnumber);
	void ChangePlayerGroup(SHORT pgroup);
	// 동맹을 맺고 해제한다. 
    void SetFriendSwitch(SHORT pnumber, SHORT mode);

};

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

	// 천년의 신화에서는 지도를 유니크 값으로 구분한다. 
#ifdef _MYTH
	DWORD MapUnique;
#endif

	// 임진록2에서는 지도를 이름으로 구분한다. 
#ifdef _IMJIN2
	char   UserFileName[USER_FILE_NAME_SIZE];  // 사용자 지도 이름 
#endif

	unsigned short RandomSeed; 
    //////////
	char   PlayerName[MAX_MULTI_PLAYER][MAXNAMELEN];
	//////////

}PLAYERSDPIDMSG, *LPPLAYERSDPIDMSG;

class _NewID {
public:
	SHORT ID;
	unsigned short Unique;

	void Init(){ID=0; Unique=0;}
	BOOL IsEmpty(){ if(ID==0 && Unique==0)return TRUE; return FALSE;}
	BOOL IsSame(_NewID id){ if(id.ID==ID && id.Unique==Unique)return TRUE; return FALSE;}


};

// _OrderData 의 핵심 정보 
class _OrderDataInfo{
public:
	SHORT Order;               // 수행해야 할 명령 코드 
	char HostMode;                // 명령을 내린 주체 (플레이어 또는 컴퓨터 )
	char ClearSwitch;           // 명령을 전달받은 캐릭터의 예약명령 버퍼들을 지울것인가?
	                            //  TRUE : 예약 명령을 지운다.        -> 즉시 명령을 실행한다. 
	                            // FALSE : 예약 명령을 지우지 않는다. ->  명령을 예약 버퍼에 넣는다.  

	int Para1;
	_NewID Para2;        // 명령에 딸려 가는 파라메타 

};

// 명령의 실체 
// 컴퓨터가 실제로 전송하는 명령의 데이터 
class _OrderData {
public:
    _OrderDataInfo ODI;

	SI08 PlayerNumber;                  // 명령을 수행할 플레이어 번호 
	SI08 CharNumber;                    // 명령을 수행해야 할 캐릭터의 갯수 
	UI08 IDData[MAX_SELECTED_CHAR_NUMBER];  // 명령을 수행해야 할 캐릭터의 아이디  (f:unique fff:id)
};


class CBattle;
class _Game{
	public:

		SI32 UserPlayerNumber ;				// 한 전투안에서 유저의 최대수 
		UI32 Frame;							// 게임 프레임. 
		DWORD PrevMultiClock;				
		DWORD BlockStartClock ;				// inframe 함수에 걸려서 경과된 시간 
		DWORD CurrentClock;
		DWORD LastClientCheckClock;

		SI32 DataIndex;
		char Data[MAX_DATA_NUMBER][MAX_DATA_SIZE];
		UI32 Length[MAX_DATA_NUMBER];

		UI32 LastClock ;
		SI32 DPMaxFrameDiff ;
		SI32 DPFrameRateForTrans;
		DWORD MessageSendClock;              // 정상 메시지가 보내진 시간. 

		PLAYERSDPIDMSG    PlayersdpId;                 // 플레이어들의 ID배열
		

		DWORD             DPCheck[MAX_MULTI_PLAYER];   // 각 플레이어가 메시지를 보내온 최근 프레임. 

		CBattle			 *pBattle;		

		UI32			  SyncSignalIndex[MAX_MULTI_PLAYER];
		UI32			  SyncSignal[MAX_MULTI_PLAYER][MAX_SYNC_SIGNAL_NUMBER];

		_OrderData UserOrderBuffer[MAX_USER_ORDER_BUFFER_INDEX];
		SI32	   UserOrderBufferIndex;

		char TempData[1024];		
		GOOUTMSG          GoOutMsg;                    // 강퇴 메시지 
		
		_OnlineBattleMsgQue OBMQue;

		bool Init(CBattle *pbattle);
		BOOL PushData(char*data, UI32 length);
		BOOL PopData(char*data, UI32 &length);
		// 지연을 근거로 각종 통신 변수들을 설정한다. 
		void DecideTransVary(SI32 latency);

		SI32 GetIDFromInClientNoMsg(LPCLIENTNOMSG lpmsg);
		UI32 GetFrameInClientNoMsg(LPCLIENTNOMSG lpmsg);
		UI32 GetSignalInClientNoMsg(LPCLIENTNOMSG lpmsg);
		BOOL SetFrameInMyMsg(LPMYMSG lpmsg, UI32 frame);
		UI32 GetFrameInMyMsg(LPMYMSG lpmsg);
		BOOL SetDataSizeInMyMsg(LPMYMSG lpmsg,  SI32 size);
		SI32 GetDataSizeInMyMsg(LPMYMSG lpmsg);
		BOOL SetIDFromInMyMsg(LPMYMSG lpmsg, SI32 idfrom);
		SI32 GetIDFromInMyMsg(LPMYMSG lpmsg);
		BOOL SetFrameInNoMsg(LPNOMSG lpmsg, UI32 frame);
		UI32 GetFrameInNoMsg(LPNOMSG lpmsg);
		BOOL SetIDFromInNoMsg(LPNOMSG lpmsg, SI32 idfrom);
		BOOL SetOldFrameVaryInNoMsg(LPNOMSG lpmsg, SI32 oldframebit, BOOL mode);

		BOOL WaitForMessageFrom(void);

		// 받은 메시지가 응용프로그램을 위한 것이면 그것에 대한 처리를 한다.
		void DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize);

		// 게임 메시지를 보낸다.
		BOOL SendGameMessage(LPGWMSG_GENERIC lpMsg);

		BOOL  DoLogoutAction(DWORD dpId);
		BOOL  DeletePlayerNumber(DWORD dpId);

		// 사용자의 명령을 호스트의 명령 버퍼에 넣는다.		
		BOOL PushUserOrderBuffer(_OrderData order);
		// 호스트의 사용자 명령 버퍼에서 명령을 하나 꺼낸다. 
		BOOL PopUserOrderBuffer(_OrderData * order);

		SI32 EncodeData(char* data, char* buffer, int size);
		SI32 DecodeData(char* data, char* buffer, int size);

		BOOL Action();

		int IsValidFollower(int uniqid, int charindex);

		// 누군가가 전리품을 얻었다는 사실을 알려준다. 
		// playernumber: 전리품을 얻은 플레이어 
		// slotnumber : 전리품을 얻은 캐릭터의 인덱스 
		// lostplayer : 전리품을 잃은 플레이어 
		// lostslotnumber : 전리품을 잃은 캐릭터의 인덱스.
		// itemcode:획득한 전리품의 코드 
		void PushBattleItemInfo(SI32 playernumber, SI32 slotnumber, SI32 lostplayer, SI32 lostslotnumber, SI32 itemcode);

		// 플레이어 인덱스를 구한다. 		
		//-------------------------------------------------------------------
		int  GetPlayerIndex(int uniqid);
		//-------------------------------------------------------------------
	

		inline void LogFile(char *outfile, char *str, ...)
		{
			static SI32 uiAddCount=0;
			FILE *fpLogFile;
			char szBuffer[1024];
			char *pArguments;
	//		if(uiAddCount > MAX_LOG_LINE) 
	//		{
	//			fpLogFile  = fopen(outfile,"w");
	//			uiAddCount = 0;
	//		}
	//		else
	//		{
				fpLogFile = fopen(outfile,"a");
	//		}
			if(fpLogFile==NULL) 
				return;
			pArguments=(char *)&str+sizeof(str);
			vsprintf(szBuffer,str,pArguments);
			/*printf(szBuffer)*/;
			fprintf(fpLogFile,szBuffer);
			++uiAddCount;
			fclose(fpLogFile);
		};


// 문자열을 16진수로 바꿔서 출력한다.
	inline VOID LogDataString(char *outfile, const char *cBuf,UI16 uiSize, UI32 uiPlayer, SI32 siPhase)
	{
		UI32 iCount; 
		LogFile(outfile, "Player[%-4d], Phase[%-4d]\n",uiPlayer, siPhase);
		for(iCount=0;iCount<uiSize;++iCount)
		{
			LogFile(outfile,"[%2X]",(UI08)cBuf[iCount]);
		}
		LogFile(outfile,"\n\n");
	}

	// 콘솔 화면에 로그를 표시한다.
	inline void DisplayLog(BOOL b, char *str, ...)
	{
		if(b)
		{
			char szBuffer[1024];
			char *pArguments;
			pArguments=(char *)&str+sizeof(str);
			vsprintf(szBuffer,str,pArguments);
			printf(szBuffer);
		}
	}
};

#endif
