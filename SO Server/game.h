// Latest Update 2002/6/26, 10:22

#ifndef _GAME_H
#define _GAME_H

// �� ��ȣ���� �ְ� �޴� �޽���  ���� (0x100 ~ 0x200)
#define GAME_MSG_MY					10103
#define GAME_MSG_CHAT				10105
#define GAME_MSG_CLIENT				10110
#define GAME_MSG_GOOUT				10112	   // ���� �޽��� 
#define GAME_MSG_CLIENT_ORDER		10113   // Ŭ���̾�Ʈ�� ȣ��Ʈ�� ����� ������ �� ����ϴ� �޽��� 
#define GAME_MSG_CLIENTNO			10116
#define GAME_MSG_AGREENEWHOST		10117
#define GAME_MSG_UPDATE				10118   // ����ڵ��� �߿� ������ ������Ʈ �ȴ�.
#define GAME_MSG_BATTLESERVERCHECK  10119   // ���� ������ Ŭ���̾�Ʈ�� ���� ���θ� Ȯ���ϱ� ���ؼ� ������ �޽���. 

#define MAX_DATA_NUMBER					100
#define MAX_DATA_SIZE					240
#define DEFAULT_MULTI_PLAY_DELAY_TIME	50
#define MAX_FRAME_DIFF					15   // �ִ� ������ ���� 

#define MAX_MULTI_PLAYER				8
#define MAXNAMELEN						15
#define MAX_SELECTED_CHAR_NUMBER		10


#define BY_PLAYER     0		
#define BY_COMPUTER   1

#define MY_MSG_DATA_SIZE     120        // ���� �������� ���� ������ _OrderData

#define MAX_NOMSG_BACKUP_STORAGE		8   // GAME_MSG_NO���� ���� �������� ������ �Բ� ����ȴ�. 
											// ���� �������� �ִ� ����. 

#define MAX_USER_ORDER_BUFFER_INDEX    20
#define MAX_SYNC_SIGNAL_NUMBER         15

// ���ð����� ������ ���� 
#define COUNTRY_NO        0      // �������� ���� 
#define COUNTRY_KOREA     1      // ���� 
#define COUNTRY_JAPAN     2      // �Ϻ� 
#define COUNTRY_CHINA     3      // ��
#define	COUNTRY_KORYU	  4		 // ���.
#define COUNTRY_RANDOM    5      // ������ 
#define COUNTRY_NEUTRAL   6      // �߸���

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
	UI32   Data;  //f:����ñ׳�  ff:�÷��̾�  fffff:������. 
}CLIENTNOMSG, *LPCLIENTNOMSG;

// ���� �޽��� 
typedef struct{
	DWORD	dwType;
	SI32	PlayerNumber;
}GOOUTMSG, *LPGOOUTMSG;

typedef struct{
	DWORD  dwType;
	UI32   Data;	// ff:������ ������  f:�޽����� �����÷��̾�����. fffff:�޽����� ���� ������ ����.
	char   data[MY_MSG_DATA_SIZE];

}MYMSG, *LPMYMSG;

// ff:nomsg�� ���� ���������� ���� f:�޽����� �����÷��̾�����. fffff:�޽����� ���� ������ ����. 
typedef struct{
	DWORD  dwType;
	UI32   Data;
}NOMSG, *LPNOMSG;

// ���κ��� �޽����� �������� ������ �ٽ� ������ ���� �䱸�ϴ� �޽��� 
typedef struct{
	DWORD  dwType;
	DWORD  IDFrom;
	DWORD  dwMyFrame;	// ����� ��û�ϴ� ���� ������. 
	DWORD  dwFrame;		// ������ (����� �ʿ��� ������.)
}BACKUPREQUESTMSG, *LPBACKUPREQUESTMSG;

class _ChatMsg{
public:
	DWORD   dwType;
private:
	SI32   Data;         // ff:������ f: �޽����� ���� �÷��̾��� ���� f:���� �޽��� ���� ffff
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
	// ������ �ΰ� �����Ѵ�. 
    void SetFriendSwitch(SHORT pnumber, SHORT mode);

};

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

	// õ���� ��ȭ������ ������ ����ũ ������ �����Ѵ�. 
#ifdef _MYTH
	DWORD MapUnique;
#endif

	// ������2������ ������ �̸����� �����Ѵ�. 
#ifdef _IMJIN2
	char   UserFileName[USER_FILE_NAME_SIZE];  // ����� ���� �̸� 
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

// _OrderData �� �ٽ� ���� 
class _OrderDataInfo{
public:
	SHORT Order;               // �����ؾ� �� ��� �ڵ� 
	char HostMode;                // ����� ���� ��ü (�÷��̾� �Ǵ� ��ǻ�� )
	char ClearSwitch;           // ����� ���޹��� ĳ������ ������ ���۵��� ������ΰ�?
	                            //  TRUE : ���� ����� �����.        -> ��� ����� �����Ѵ�. 
	                            // FALSE : ���� ����� ������ �ʴ´�. ->  ����� ���� ���ۿ� �ִ´�.  

	int Para1;
	_NewID Para2;        // ��ɿ� ���� ���� �Ķ��Ÿ 

};

// ����� ��ü 
// ��ǻ�Ͱ� ������ �����ϴ� ����� ������ 
class _OrderData {
public:
    _OrderDataInfo ODI;

	SI08 PlayerNumber;                  // ����� ������ �÷��̾� ��ȣ 
	SI08 CharNumber;                    // ����� �����ؾ� �� ĳ������ ���� 
	UI08 IDData[MAX_SELECTED_CHAR_NUMBER];  // ����� �����ؾ� �� ĳ������ ���̵�  (f:unique fff:id)
};


class CBattle;
class _Game{
	public:

		SI32 UserPlayerNumber ;				// �� �����ȿ��� ������ �ִ�� 
		UI32 Frame;							// ���� ������. 
		DWORD PrevMultiClock;				
		DWORD BlockStartClock ;				// inframe �Լ��� �ɷ��� ����� �ð� 
		DWORD CurrentClock;
		DWORD LastClientCheckClock;

		SI32 DataIndex;
		char Data[MAX_DATA_NUMBER][MAX_DATA_SIZE];
		UI32 Length[MAX_DATA_NUMBER];

		UI32 LastClock ;
		SI32 DPMaxFrameDiff ;
		SI32 DPFrameRateForTrans;
		DWORD MessageSendClock;              // ���� �޽����� ������ �ð�. 

		PLAYERSDPIDMSG    PlayersdpId;                 // �÷��̾���� ID�迭
		

		DWORD             DPCheck[MAX_MULTI_PLAYER];   // �� �÷��̾ �޽����� ������ �ֱ� ������. 

		CBattle			 *pBattle;		

		UI32			  SyncSignalIndex[MAX_MULTI_PLAYER];
		UI32			  SyncSignal[MAX_MULTI_PLAYER][MAX_SYNC_SIGNAL_NUMBER];

		_OrderData UserOrderBuffer[MAX_USER_ORDER_BUFFER_INDEX];
		SI32	   UserOrderBufferIndex;

		char TempData[1024];		
		GOOUTMSG          GoOutMsg;                    // ���� �޽��� 
		
		_OnlineBattleMsgQue OBMQue;

		bool Init(CBattle *pbattle);
		BOOL PushData(char*data, UI32 length);
		BOOL PopData(char*data, UI32 &length);
		// ������ �ٰŷ� ���� ��� �������� �����Ѵ�. 
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

		// ���� �޽����� �������α׷��� ���� ���̸� �װͿ� ���� ó���� �Ѵ�.
		void DoApplicationMessage(LPGWMSG_GENERIC lpMsg, DWORD dwMsgSize);

		// ���� �޽����� ������.
		BOOL SendGameMessage(LPGWMSG_GENERIC lpMsg);

		BOOL  DoLogoutAction(DWORD dpId);
		BOOL  DeletePlayerNumber(DWORD dpId);

		// ������� ����� ȣ��Ʈ�� ��� ���ۿ� �ִ´�.		
		BOOL PushUserOrderBuffer(_OrderData order);
		// ȣ��Ʈ�� ����� ��� ���ۿ��� ����� �ϳ� ������. 
		BOOL PopUserOrderBuffer(_OrderData * order);

		SI32 EncodeData(char* data, char* buffer, int size);
		SI32 DecodeData(char* data, char* buffer, int size);

		BOOL Action();

		int IsValidFollower(int uniqid, int charindex);

		// �������� ����ǰ�� ����ٴ� ����� �˷��ش�. 
		// playernumber: ����ǰ�� ���� �÷��̾� 
		// slotnumber : ����ǰ�� ���� ĳ������ �ε��� 
		// lostplayer : ����ǰ�� ���� �÷��̾� 
		// lostslotnumber : ����ǰ�� ���� ĳ������ �ε���.
		// itemcode:ȹ���� ����ǰ�� �ڵ� 
		void PushBattleItemInfo(SI32 playernumber, SI32 slotnumber, SI32 lostplayer, SI32 lostslotnumber, SI32 itemcode);

		// �÷��̾� �ε����� ���Ѵ�. 		
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


// ���ڿ��� 16������ �ٲ㼭 ����Ѵ�.
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

	// �ܼ� ȭ�鿡 �α׸� ǥ���Ѵ�.
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
