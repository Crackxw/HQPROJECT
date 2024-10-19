#ifndef _CMDSTRUCT_H
#define _CMDSTRUCT_H

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 명령 구조체
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnRcvMsg
{
	UI32	uiCmdMsg;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그인 구조체
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnRcvLogin
{
	UI32	uiCmdMsg;
	UI32	uiDBAccount;
	UI32	uiUniqueID;
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그인 구조체
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnRetLogin
{
	UI32	uiCmdMsg;
	UI32	uiFlag;				// 로그인 허가 또는 거부....
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 대화할 상대를 추가하거나 삭제한다..
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnControlChar
{
	UI32	uiCmdMsg;
	UI32	uiDBAccount;
	UI32	uiFlag;				// 0:추가 | 1:삭제
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 메세지를 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnRcvChat
{
	UI32	uiCmdMsg;
	UI32	uiChatLength;
};
struct OnSndChat
{
	UI32	uiCmdMsg;
	UI32	uiUniqueID;			// 메세지를 보낸사람의 UniqueID.
	UI32	uiChatLength;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 귓속말을 보낸다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnRcvEarMsg
{
	UI32	uiCmdMsg;
	UI32	uiDBAccount;
	UI32	uiChatLength;
};

struct OnSndEarMsg
{
	UI32	uiCmdMsg;
	UI32	uiUniqueID;			// 메세지를 보낸사람의 UniqueID.
	UI32	uiChatLength;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 채팅이 가능한 상태인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct OnRcvInitList
{
	UI32	uiCmdMsg;
};

#endif