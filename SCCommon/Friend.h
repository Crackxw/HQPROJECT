#ifndef _FRIEND_H_
#define _FRIEND_H_


#define	MAX_FRIEND_NUM			30

#include "cltList.h"
#include "defines.h"

typedef enum {	FRIEND_OK,
				FRIEND_ALREADY_EXIST,
				FRIEND_CANNOT_ADD_MYSELF,
				FRIEND_INVALID_NAME,
				FRIEND_FULL }	FRESULT;

struct strFriendInfo
{
	CHAR	szID[ON_ID_LENGTH+1];		// ID
	BOOL	bLogin;						// 로그인 상태
	BOOL	bDenial;					// 수신 거부 상태
};

class cltFriendList : cltList<strFriendInfo>
{
public:
	cltFriendList();														// 생성자
	~cltFriendList();														// 소멸자
	VOID			Init();													// 초기화
	//strFriendInfo*	GetFriendList();									// 전체 친구목록의 포인터를 얻어온다.
	strFriendInfo*	GetFriend( CHAR *szID );								// 특정 친구의 정보를 얻어온다.
	strFriendInfo*	GetFriend( SI32 index );								// index로 특정 친구의 정보를 얻어온다.
	SI32			GetNumFriends();										// 등록된 친구의 수를 얻어온다.
	BOOL			IsLogin( CHAR *szID );									// 로그인중인가?
	VOID			SetLogin( CHAR *szID, BOOL bLogin );					// 친구의 로그인/로그아웃 상태를 설정한다.
	BOOL			IsDenial( CHAR *szID );									// 수신거부된 친구인가?
	BOOL			SetDenial( CHAR *szID, BOOL bDenial );					// 친구의 수신거부 설정/해제 상태를 설정한다.
	FRESULT			AddFriend( CHAR *szMyID, CHAR *szID, BOOL bLogin );		// 친구를 추가한다.
	BOOL			DeleteFriend( CHAR *szID );								// 친구를 삭제한다.
	VOID			DeleteAllFriend();
};


#endif
