//-----------------------------------------------------------------------------
//
//  거상 웹 채팅 프로토콜    
//
//  작  성  자 : 이 제 완
//  작  성  일 : 2003/4/2
//  최종수정일 : 2003/4/2
//
//  - packet command define 명명규칙
//
//	  REQUEST_    요청 ( 클라이언트 -> 서버 )
//    REPORT_	  보고 ( 클라이언트 -> 서버 )
//    RESPONSE_   응답 ( 서버 -> 클라이언트 )
//    NOTIFY_     알림 ( 서버 -> 클라이언트 )
//	  COMMAND_    명령 ( 서버 -> 클라이언트 )
//
//
//
//                접속종료             접속종료             접속종료 
//
//                   ^                     ^                    ^
//                   | 로그아웃            | 로그아웃           | 로그아웃(대화방 퇴장)
//                   |                     |  (대기실 퇴장)     |
//                   |                     |                    |
//
//  - 로그인 --->   none      --->       대기실      --->     대화방
//                         대기실 입장           대화방 입장 
//
//                                                   <---
//                                               대화방 퇴장 
//
//
// ** 로그인/로그아웃 **
//
//	  (요청필요)로그인						- 로그인
//    (응답)로그인							- 로그인 허용
//                                            ( 실패한 경우 약속된 에러 코드 혹은 문자열과 함께 사유 전송 )
//    (요청필요)로그아웃					- 로그아웃요청
//                                            ( 요청 후 그냥 소켓 접속 종료 해도됨 
//                                              로그아웃 명령을 기다릴 거라면, 특정 시간 동안만 기다리고 그냥 끊어버릴 것 )
//    (명령)로그아웃						- 로그아웃( 유저의 요청, 서버에서 어떤 이유로 )
//
//
//
// ** 대기실 **
//		
//    (요청필요)대화방 리스트 받아오기		- 전체 받기, 부분 받기
//
//    (요청필요)대화방 생성					- 방 제목, 방 속성, 비공개방일 경우 패스워드
//    (응답)대화방 생성						- 생성 결과( 성공한 경우 방 번호 받음,
//											             실패한 경우 약속된 에러 코드 혹은 문자열과 함께 사유 전송 )
//
//    (요청필요)대화방 입장					- 방번호, 비공개방일 경우 패스워드 필요
//    (응답)대화방 입장						- 방 입장 여부( 실패한 경우 약속된 에러 코드 혹은 문자열과 함께 사유 전송 )
//
//
//
//    (알림받음)대화방 변경 정보 받기		- 대화방명 변경( 방장이 방명을 변경한 경우 )
//											- 방 속성 변경 사항
//											  ( 1. 유저의 방입장/방퇴장 할때 마다 혹은 
//												   특정 주기 마다 업데이트 방식 중 하나
//												   이건 서버에서 결정 
//												2. 방장이 방 속성을 변경한 경우 )
//
//    (알림받음)대화방 생성/삭제 정보 받기	- 
//
//    (요청필요)대기실에 있는 사용자 정보 받기 
//
//    * 대기실 채팅이 가능하게 하나?
//
//
// ** 대화방 **
//    
//    (요청필요 혹은 입장시 자동응답전송)대화 참여 유저 정보 받아오기  
//
//
//    (요청)나가기
//
//    (명령)퇴장당함							- 퇴장 사유 ( 추방, 방 닫힘 )
//
//
//    (알림받음)사용자 입장 알림				- 입장 방식 ( 일반 입장, 초대 받음 )
//    (알림받음)사용자 퇴장 알림		        - 퇴장 사유 ( 나가기, 추방, 방 닫힘, 접속 끊김 )
//    
//    (알림받음)방정보 변경 알림				- 방장이 방정보를 변경 했을 때
//    (알림받음)방장정보 변경 알림				- 방장이 퇴장 혹은 방장이 방장 권한을 넘겼을 때 
//    
//
//    
//    [방장 권한]
//
//    (요청)방정보 변경
//    (응답)방정보 변경 
//
//    (요청)방제목 변경
//    (응답)방제목 변경 
//
//    (요청)방속성 변경
//    (응답)방속성 변경
//
//    (요청)
//
//    (요청)방장 권한 넘김
//    (응답)방장 권한 넘김
//
//-----------------------------------------------------------------------------


#ifndef _CHATPROTOCOL_H
#define _CHATPROTOCOL_H

#include "../JWCommon/jwbase.h"

// 로그인 / 로그 아웃 
#define REQUEST_LOGIN							101
#define REQUEST_LOGOUT							102

#define RESPONSE_LOGIN							111
#define RESPONSE_LOGOUT							112

#define COMMAND_LOGOUT							141

// global

#define REQUEST_USERMSG							901
#define RESPONSE_USERMSG						911

#define NOTIFY_USERMSG							921

#define NOTIFY_NOTICEMSG						922


// Lobby에서..
#define REQUEST_ROOMLIST						1001

#define REQUEST_MAKE_ROOM						1002
#define REQUEST_JOIN_ROOM						1003

#define RESPONSE_MAKE_ROOM						1101
#define RESPONSE_JOIN_ROOM						1102

#define RESPONSE_BEGIN_ROOMLIST					1103
#define RESPONSE_ROOMVIEWINFO					1104
#define RESPONSE_END_ROOMLIST					1105

#define NOTIFY_ROOMVIEWINFO_CHANGED				1201
#define NOTIFY_ROOMTITLE_CHANGED				1202
#define NOTIFY_ROOMATTR_CHANGED					1203

#define NOTIFY_ROOM_CREATED						1204
#define NOTIFY_ROOM_DELETED						1205


// 대화방
#define REQUEST_USERLIST						2101
#define REQUEST_EXIT_ROOM						2102

#define REQUEST_CHATMSG							2103
#define REQUEST_WHISPER							2104

#define RESPONSE_USERLIST						2201
#define RESPONSE_EXIT_ROOM						2202
#define RESPONSE_WHISPER						2203

#define NOTIFY_CHATMSG							2301
#define NOTIFY_WHISPER							2302

#define NOTIFY_USER_JOINED						2303
#define NOTIFY_USER_EXITED						2304
#define NOTIFY_USERINFO_CHANGED					2305
#define NOTIFY_ROOMINFO_CHANGED					2306
#define NOTIFY_ROOMMASTER_CHANGED				2307
#define NOTIFY_KICKEDOUT						2308

#define COMMAND_EMPOWERED_ROOMMASTER			2401

// 대화방에서 방장 권한

#define REQUEST_CHANGE_ROOMINFO					3101
#define REQUEST_CHANGE_ROOMMASTER				3102
#define REQUEST_KICK_USER_OUT					3103

#define RESPONSE_CHANGE_ROOMINFO				3201
#define RESPONSE_CHANGE_ROOMMASTER				3202
#define RESPONSE_KICK_USER_OUT					3203


// 초대 요청/응답
#define REQUEST_INVITE_USER						4101
#define RESPONSE_INVITE_USER					4201
#define NOTIFY_INVITED							4301


#define MAX_USER_ID								12
#define MAX_USER_NAME							12
#define MAX_ROOM_TITLE							48
#define MAX_ROOM_PASSWORD						12
#define MAX_CHATMSG								1024

#define ROOMTYPE_GENERAL						0
#define ROOMTYPE_SECRET							1

#define EXITCODE_USERREQUEST					0
#define EXITCODE_KICKED							1
#define EXITCODE_DISCONNECT						2
#define EXITCODE_ROOMCLOSED						3

#define RESULT_SUCCESS							0
#define RESULT_UNKNOWN_FAILED					99

// make room
#define RESULT_NOMORE_EMPTYROOM					1
#define RESULT_INVALID_PASSWORD					2
#define RESULT_INVALID_ROOMATTR					3
#define RESULT_INVALID_ROOMTITLE				4

// join room
#define RESULT_OVER_ROOMCAPACITY				5
#define RESULT_ROOMCLOSED						6
#define RESULT_INVALID_ROOMNUM					7
#define RESULT_INCORRECT_PASSWORD				8

// 방에서
#define RESULT_NOT_ROOMMASTER					10

// change room flag
#define CHANGE_ROOM_TITLE						(DWORD)1 << 1
#define CHANGE_ROOM_ATTR						(DWORD)1 << 2
#define CHANGE_ROOM_PASSWORD					(DWORD)1 << 3


// roomtype
#define USERTYPE_INLOBBY						0
#define USERTYPE_ROOMMEMBER						1
#define USERTYPE_ROOMMASTER						2

#pragma pack(1)

struct sUserInfo
{
	char				szID[ MAX_USER_ID ];
	char				szName[ MAX_USER_NAME ];

	UI16				usUserType;								// 0 대기실  1 방원  2 방장 
};

struct sUserViewInfo
{
	char				szID[ MAX_USER_ID ];
	char				szName[ MAX_USER_NAME ];

	UI16				usUserType;
};

struct sUserInfoChangable
{
	char				szName[ MAX_USER_NAME ];
};

struct sRoomMasterInfo
{
	char				szID[ MAX_USER_ID ];
	char				szName[ MAX_USER_NAME ];
};

struct sRoomAttr
{
	UI16				usRoomType;								// 0 공개  1 비공개

	UI16				usNumCapacity;							// 최대 인원
	UI16				usNumJoinedUser;						// 참여 인원
};

struct sRoomInfo
{
	UI16				usRoomNum;								// 방 번호 
	UI16				usRoomID;								// 방 고유번호
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// 방 제목
	sRoomMasterInfo		roomMasterInfo;							// 방장 정보
	sRoomAttr			roomAttr;								// 방 속성 정보

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// 방 패스워드 (공개방인 경우에는 무시)
};


struct sRoomViewInfo
{
	UI16				usRoomNum;								// 방 번호
	UI16				usRoomID;								// 방 고유번호
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// 방 제목
	sRoomMasterInfo		roomMasterInfo;							// 방장 정보
	sRoomAttr			roomAttr;								// 방 속성 정보
};


struct sRoomInfoChangable
{
	DWORD               dwFlag;

	char				szRoomTitle[ MAX_ROOM_TITLE ];			// 방 제목
	sRoomAttr			roomAttr;								// 방 속성 정보
	
	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// 방 패스워드 (공개방인 경우에는 무시)
};

struct sMakeRoomInfo
{
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// 방 제목
	sRoomAttr			roomAttr;								// 방 속성 정보

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// 방 패스워드 (공개방인 경우에는 무시)
};

struct sPacketHeader
{
	UI16	size;
	UI16	command;
};


//-----------------------------------------------------------------------------
// 로그인 / 로그아웃
//-----------------------------------------------------------------------------

struct sRequest_LogIn
{
    sPacketHeader		packetHeader;

	char				szID[ MAX_USER_ID ];
	char				szName[ MAX_USER_NAME ];
};

struct sRequest_LogOut
{
    sPacketHeader		packetHeader;
};

struct sResponse_LogIn
{
    sPacketHeader		packetHeader;

	UI32	result;
};


struct sCommand_LogOut
{
    sPacketHeader		packetHeader;
};



//-----------------------------------------------------------------------------
// 대기실에서의 요청/응답/알림
//-----------------------------------------------------------------------------

struct sRequest_RoomList
{
    sPacketHeader		packetHeader;
};


struct sRequest_MakeRoom
{
    sPacketHeader		packetHeader;

	char				szRoomTitle[ MAX_ROOM_TITLE ];			// 방 제목
	sRoomAttr			roomAttr;								// 방 속성 정보

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// 방 패스워드 (공개방인 경우에는 무시)
};

struct sRequest_JoinRoom
{
    sPacketHeader		packetHeader;

	UI16				usRoomNum;
	UI16				usRoomID;
	char				szRoomPassword[ MAX_ROOM_PASSWORD ];
};


struct sResponse_BeginRoomList
{
	sPacketHeader		packetHeader;
	UI16				usCount;

};

struct sResponse_RoomViewInfo
{
	sPacketHeader		packetHeader;
	sRoomViewInfo		roomViewInfo;

};

struct sResponse_EndRoomList
{
	sPacketHeader		packetHeader;
};


struct sResponse_MakeRoom
{
    sPacketHeader		packetHeader;
	
	UI32				result;
	
	UI16				usRoomNum;
	UI16				usRoomID;
};

struct sResponse_JoinRoom
{
    sPacketHeader		packetHeader;
	
	UI32				result;
};


struct sNotify_RoomTitleChanged
{
    sPacketHeader		packetHeader;

	char				szRoomTitle[ MAX_ROOM_TITLE ];
};

struct sNotify_RoomAttrChanged
{
    sPacketHeader		packetHeader;
	
	sRoomAttr			roomAttr;
};



//-----------------------------------------------------------------------------
// 대화방에서의 요청/응답/알림
//-----------------------------------------------------------------------------

struct sRequest_UserList
{
    sPacketHeader		packetHeader;

};


struct sRequest_ExitRoom
{
    sPacketHeader		packetHeader;
};


struct sRequest_ChatMsg
{
    sPacketHeader		packetHeader;

	UI16				usFontType;
	UI16				usTextSize;
	UI16				usTextColor;

	UI16				usStrLength;
//  char                str[ usStrLength ]

};


struct sRequest_Whisper
{
    sPacketHeader		packetHeader;

	char				szReceiverID[ MAX_USER_ID ];
	UI16				usStrLength;
//  char                str[ usStrLength ]

};


struct sRequest_ChangeRoomMaster
{
    sPacketHeader		packetHeader;

	char				szUserID[ MAX_USER_ID ];
};


struct sRequest_ChangeRoomInfo
{
    sPacketHeader		packetHeader;
	sRoomInfoChangable	roomInfoChangable;

};

struct sRequest_KickUserOut
{
    sPacketHeader		packetHeader;

	char				szUserID[ MAX_USER_ID ];
};

struct sResponse_UserList
{
	sPacketHeader		packetHeader;

	UI16				usCount;
//  sUserViewInfo		userViewInfo[ usCount ]
};



struct sResponse_ExitRoom
{
	sPacketHeader		packetHeader;

	UI32				result;
};

struct sResponse_Whisper
{
	sPacketHeader		packetHeader;

	UI32				result;

	char				szReceiverID[ MAX_USER_ID ];
	UI16				usStrLength;
//  char                str[ usStrLength ]
};


struct sResponse_ChangeRoomInfo
{
	sPacketHeader		packetHeader;

	UI32				result;
};

struct sResponse_ChangeRoomMaster
{
	sPacketHeader		packetHeader;

	UI32				result;
};

struct sResponse_KickUserOut
{
    sPacketHeader		packetHeader;
 
	UI32				result;

};

struct sNotify_KickedOut
{
	sPacketHeader		packetHeader;
};

struct sNotify_RoomMasterChanged
{
    sPacketHeader		packetHeader;

	char				szUserID[ MAX_USER_ID ];
};


struct sNotify_RoomInfoChanged
{
    sPacketHeader		packetHeader;
	sRoomInfoChangable	roomInfoChangable;
};

struct sNotify_UserJoined
{
    sPacketHeader		packetHeader;

	sUserViewInfo		userViewInfo;
};

struct sNotify_UserExited 
{
	sPacketHeader		packetHeader;

	char				szUserID[ MAX_USER_ID ];
	UI16				usExitCode;

};

struct sNotify_ChatMsg
{
	sPacketHeader		packetHeader;

	char				szUserID[ MAX_USER_ID ];

	UI16				usFontType;
	UI16				usTextSize;
	UI16				usTextColor;

	UI16				usStrLength;
//	char				str[ usStrLength ]
};

struct sNotify_Whisper
{
	sPacketHeader		packetHeader;

	char				szSenderID[ MAX_USER_ID ];
	UI16				usStrLength;
//	char				str[ usStrLength ]

};


//-----------------------------------------------------------------------------
// 초대하기 관련
//-----------------------------------------------------------------------------
struct sRequest_InviteUser
{
	sPacketHeader		packetHeader;

	char				szInviteID[ MAX_USER_ID ];
	UI16				usStrLength;
	//char				szInviteMsg[ usStrLength ];			//초대할때 보내는 메시지
};

struct sResponse_InviteUser
{
	sPacketHeader		packetHeader;
	UI16				result;
};

struct sNotify_InvitedUser
{
	sPacketHeader		packetHeader;
	char				szInvitedID[ MAX_USER_ID ];
	UI16				usStrLength;
	//char				szInviteMsg[ usStrLength ];			//초대한 사람이 보낸 메시지
};

#pragma pack()

#endif