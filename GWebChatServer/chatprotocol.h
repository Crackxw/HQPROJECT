//-----------------------------------------------------------------------------
//
//  �Ż� �� ä�� ��������    
//
//  ��  ��  �� : �� �� ��
//  ��  ��  �� : 2003/4/2
//  ���������� : 2003/4/2
//
//  - packet command define ����Ģ
//
//	  REQUEST_    ��û ( Ŭ���̾�Ʈ -> ���� )
//    REPORT_	  ���� ( Ŭ���̾�Ʈ -> ���� )
//    RESPONSE_   ���� ( ���� -> Ŭ���̾�Ʈ )
//    NOTIFY_     �˸� ( ���� -> Ŭ���̾�Ʈ )
//	  COMMAND_    ��� ( ���� -> Ŭ���̾�Ʈ )
//
//
//
//                ��������             ��������             �������� 
//
//                   ^                     ^                    ^
//                   | �α׾ƿ�            | �α׾ƿ�           | �α׾ƿ�(��ȭ�� ����)
//                   |                     |  (���� ����)     |
//                   |                     |                    |
//
//  - �α��� --->   none      --->       ����      --->     ��ȭ��
//                         ���� ����           ��ȭ�� ���� 
//
//                                                   <---
//                                               ��ȭ�� ���� 
//
//
// ** �α���/�α׾ƿ� **
//
//	  (��û�ʿ�)�α���						- �α���
//    (����)�α���							- �α��� ���
//                                            ( ������ ��� ��ӵ� ���� �ڵ� Ȥ�� ���ڿ��� �Բ� ���� ���� )
//    (��û�ʿ�)�α׾ƿ�					- �α׾ƿ���û
//                                            ( ��û �� �׳� ���� ���� ���� �ص��� 
//                                              �α׾ƿ� ����� ��ٸ� �Ŷ��, Ư�� �ð� ���ȸ� ��ٸ��� �׳� ������� �� )
//    (���)�α׾ƿ�						- �α׾ƿ�( ������ ��û, �������� � ������ )
//
//
//
// ** ���� **
//		
//    (��û�ʿ�)��ȭ�� ����Ʈ �޾ƿ���		- ��ü �ޱ�, �κ� �ޱ�
//
//    (��û�ʿ�)��ȭ�� ����					- �� ����, �� �Ӽ�, ��������� ��� �н�����
//    (����)��ȭ�� ����						- ���� ���( ������ ��� �� ��ȣ ����,
//											             ������ ��� ��ӵ� ���� �ڵ� Ȥ�� ���ڿ��� �Բ� ���� ���� )
//
//    (��û�ʿ�)��ȭ�� ����					- ���ȣ, ��������� ��� �н����� �ʿ�
//    (����)��ȭ�� ����						- �� ���� ����( ������ ��� ��ӵ� ���� �ڵ� Ȥ�� ���ڿ��� �Բ� ���� ���� )
//
//
//
//    (�˸�����)��ȭ�� ���� ���� �ޱ�		- ��ȭ��� ����( ������ ����� ������ ��� )
//											- �� �Ӽ� ���� ����
//											  ( 1. ������ ������/������ �Ҷ� ���� Ȥ�� 
//												   Ư�� �ֱ� ���� ������Ʈ ��� �� �ϳ�
//												   �̰� �������� ���� 
//												2. ������ �� �Ӽ��� ������ ��� )
//
//    (�˸�����)��ȭ�� ����/���� ���� �ޱ�	- 
//
//    (��û�ʿ�)���ǿ� �ִ� ����� ���� �ޱ� 
//
//    * ���� ä���� �����ϰ� �ϳ�?
//
//
// ** ��ȭ�� **
//    
//    (��û�ʿ� Ȥ�� ����� �ڵ���������)��ȭ ���� ���� ���� �޾ƿ���  
//
//
//    (��û)������
//
//    (���)�������							- ���� ���� ( �߹�, �� ���� )
//
//
//    (�˸�����)����� ���� �˸�				- ���� ��� ( �Ϲ� ����, �ʴ� ���� )
//    (�˸�����)����� ���� �˸�		        - ���� ���� ( ������, �߹�, �� ����, ���� ���� )
//    
//    (�˸�����)������ ���� �˸�				- ������ �������� ���� ���� ��
//    (�˸�����)�������� ���� �˸�				- ������ ���� Ȥ�� ������ ���� ������ �Ѱ��� �� 
//    
//
//    
//    [���� ����]
//
//    (��û)������ ����
//    (����)������ ���� 
//
//    (��û)������ ����
//    (����)������ ���� 
//
//    (��û)��Ӽ� ����
//    (����)��Ӽ� ����
//
//    (��û)
//
//    (��û)���� ���� �ѱ�
//    (����)���� ���� �ѱ�
//
//-----------------------------------------------------------------------------


#ifndef _CHATPROTOCOL_H
#define _CHATPROTOCOL_H

#include "../JWCommon/jwbase.h"

// �α��� / �α� �ƿ� 
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


// Lobby����..
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


// ��ȭ��
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

// ��ȭ�濡�� ���� ����

#define REQUEST_CHANGE_ROOMINFO					3101
#define REQUEST_CHANGE_ROOMMASTER				3102
#define REQUEST_KICK_USER_OUT					3103

#define RESPONSE_CHANGE_ROOMINFO				3201
#define RESPONSE_CHANGE_ROOMMASTER				3202
#define RESPONSE_KICK_USER_OUT					3203


// �ʴ� ��û/����
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

// �濡��
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

	UI16				usUserType;								// 0 ����  1 ���  2 ���� 
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
	UI16				usRoomType;								// 0 ����  1 �����

	UI16				usNumCapacity;							// �ִ� �ο�
	UI16				usNumJoinedUser;						// ���� �ο�
};

struct sRoomInfo
{
	UI16				usRoomNum;								// �� ��ȣ 
	UI16				usRoomID;								// �� ������ȣ
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// �� ����
	sRoomMasterInfo		roomMasterInfo;							// ���� ����
	sRoomAttr			roomAttr;								// �� �Ӽ� ����

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// �� �н����� (�������� ��쿡�� ����)
};


struct sRoomViewInfo
{
	UI16				usRoomNum;								// �� ��ȣ
	UI16				usRoomID;								// �� ������ȣ
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// �� ����
	sRoomMasterInfo		roomMasterInfo;							// ���� ����
	sRoomAttr			roomAttr;								// �� �Ӽ� ����
};


struct sRoomInfoChangable
{
	DWORD               dwFlag;

	char				szRoomTitle[ MAX_ROOM_TITLE ];			// �� ����
	sRoomAttr			roomAttr;								// �� �Ӽ� ����
	
	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// �� �н����� (�������� ��쿡�� ����)
};

struct sMakeRoomInfo
{
	char				szRoomTitle[ MAX_ROOM_TITLE ];			// �� ����
	sRoomAttr			roomAttr;								// �� �Ӽ� ����

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// �� �н����� (�������� ��쿡�� ����)
};

struct sPacketHeader
{
	UI16	size;
	UI16	command;
};


//-----------------------------------------------------------------------------
// �α��� / �α׾ƿ�
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
// ���ǿ����� ��û/����/�˸�
//-----------------------------------------------------------------------------

struct sRequest_RoomList
{
    sPacketHeader		packetHeader;
};


struct sRequest_MakeRoom
{
    sPacketHeader		packetHeader;

	char				szRoomTitle[ MAX_ROOM_TITLE ];			// �� ����
	sRoomAttr			roomAttr;								// �� �Ӽ� ����

	char				szRoomPassword[ MAX_ROOM_PASSWORD ];	// �� �н����� (�������� ��쿡�� ����)
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
// ��ȭ�濡���� ��û/����/�˸�
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
// �ʴ��ϱ� ����
//-----------------------------------------------------------------------------
struct sRequest_InviteUser
{
	sPacketHeader		packetHeader;

	char				szInviteID[ MAX_USER_ID ];
	UI16				usStrLength;
	//char				szInviteMsg[ usStrLength ];			//�ʴ��Ҷ� ������ �޽���
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
	//char				szInviteMsg[ usStrLength ];			//�ʴ��� ����� ���� �޽���
};

#pragma pack()

#endif