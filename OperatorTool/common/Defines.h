
#pragma warning ( disable : 4786 )

#pragma once

#define WM_CONNECT					WM_USER + 1
#define WM_LOGIN					WM_USER + 2
#define WM_REQ_LOGIN				WM_USER + 3


#define IP			"220.85.19.80"
#define PORT		8520
#define MSG_SIZE	4096
#define ID_LENGTH	50
#define PW_LENGTH	50
#define ERROR_ID	-1
#define ERROR_PW	-2
#define ERROR_OK	0


enum { ON_RES_LOGIN = WM_LOGIN,				// 로그인 응답
	   ON_REQ_LOGIN,						// 로그인 요청
	   ON_RES_ITEM_EDIT,					// 아이템 에디터 응답
	   ON_REQ_ITEM_EDIT,					// 아이템 에디터 요청
	   ON_RES_LOGOUT,						// 로그 아웃
	   ON_REQ_LOGOUT,						//
	   ON_MAX_MSG
};

struct OnBaseMsg
{
public:
	int		GetType()	{ return nMsg_Type; }

protected:
	int		nMsg_Type;
};

struct ReqLogin : public OnBaseMsg
{
	ReqLogin::ReqLogin()	{ nMsg_Type = ON_REQ_LOGIN; }	

	char		strID[ID_LENGTH];
	char		strPW[ID_LENGTH];
};

struct ResLogin : public OnBaseMsg
{
	ResLogin::ResLogin()	{ nMsg_Type = ON_RES_LOGIN; }

	enum 		{ LOGIN_OK, INVALID_ID, INVALID_PW, CONNECTED_USER };

	int			nGrade;
	int			nLoginState;
	char		strLoginMsg[256];
};

struct ResItemEdit : public OnBaseMsg
{
	ResItemEdit::ResItemEdit()	{ nMsg_Type = ON_RES_ITEM_EDIT; }

	BOOL		bUsePossible;
	char		strUserName[256];
};

struct ReqItemEdit : public OnBaseMsg
{
	ReqItemEdit::ReqItemEdit()	{ nMsg_Type = ON_REQ_ITEM_EDIT; }
};