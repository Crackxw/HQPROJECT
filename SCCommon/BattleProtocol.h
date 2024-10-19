#ifndef	BATTLE_PROTOCOL_HEADER
#define	BATTLE_PROTOCOL_HEADER

#include "SOCommonHeader.h"

// 전투 서버와 게임 서버간(UDP)의 공통 패킷 헤더 

typedef struct
{
	WORD	dataSize;
	BYTE	checkSum;

}HEADER;



// 1. 전투 정보 전송 				   [ 게임서버  -------->  전투 서버 ]
typedef struct 
{
	WORD   command;						// ON_REQUEST_CREATE_BATTLEROOM

	WORD   numPlayer;					// 참여한 플레이어의 수 (NPC(몬스터)포함)
	DWORD  uniqID;						// 게임 서버에서 판단할 유니크한 전투 아이디 	
}REQUEST_INFOGAME;


// 2. 전투 생성 응답 					[ 게임서버  <--------  전투 서버 ]
typedef struct 
{
	WORD    command;					// ON_RESPONSE_CREATE_BATTLEROOM	
	WORD	battleIndex;				// 배틀서버에서의 전투 인덱스, 전투 생성에 실패 했을 경우 -1 										
	DWORD	uniqID;						// 전투 생성에 성공한 게임서버에서의 유니크 아이디  

}RESPONSE_CREATEGAME;


// 3. 전투 종료,변경된 데이타 알림  	[ 게임서버  <--------  전투 서버 ]
typedef struct							
{
	WORD   command;						// ON_RESPONSE_FINISH_BATTLE	
	WORD   numPlayer;					// 전투가 완벽히 종료 되었을 경우 +90 된 값이 간다 
	DWORD  uniqID;						// 게임서버에서의 UniqID
}RESPONSE_FINISHGAME;


// 1. 전투에 있는 플레이어 정보 요청 	[ 게임서버  -------->  전투 서버 ]
typedef struct 
{
	WORD	command;					// ON_REQUEST_CREATE_BATTLEROOM
	WORD	uiDBSlot;					// DBSlot	
	SI32	siDBAccount;				// DBAccount;		
}REQUEST_BATTLEPLAYERINFO;

// 전투가 종료되었다는 것을 게임 서버에게 알려줌
typedef struct 
{
	WORD	command;					// ON_BATTLEROOM_FINISHED
	WORD	battleIndex;				// 배틀서버에서의 전투 인덱스
}BATTLE_ROOMFINISHED;


// 전투가 종료되었다는 메시지를 받았다는 것을 전투 서버에게 다시 알려줌
typedef struct 
{
	WORD	command;					// ON_BATTLEROOM_FINISHED_RECV_OK
	WORD	battleIndex;				// 배틀서버에서의 전투 인덱스
}BATTLE_ROOMFINISHED_RECV_OK;

/*
	예외 상황  

  유저가 전투서버에 들어가서 비정상 종료(네트웍연결이 갑자기 끊긴경우)시 게임서버는 전투서버로 
  부터 종료 메시지를 받은후 유저의 종료를 처리해야 한다. 
  유저의 접속이 끊긴후 30초이상(미정) 전투서버로 부터 종료메시지가 도착하지 않으면 전투서버로 
  종료요청을 할수있다. 
  ( 게임서버에서만 접속이 끊기고, 전투서버에서는 끊기지 않은경우, 게임서버가 전투서버보다 먼저 
    유저의 비정상 종료를 알았을 경우 ) 
 
*/

#endif