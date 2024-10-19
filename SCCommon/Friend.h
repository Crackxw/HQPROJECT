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
	BOOL	bLogin;						// �α��� ����
	BOOL	bDenial;					// ���� �ź� ����
};

class cltFriendList : cltList<strFriendInfo>
{
public:
	cltFriendList();														// ������
	~cltFriendList();														// �Ҹ���
	VOID			Init();													// �ʱ�ȭ
	//strFriendInfo*	GetFriendList();									// ��ü ģ������� �����͸� ���´�.
	strFriendInfo*	GetFriend( CHAR *szID );								// Ư�� ģ���� ������ ���´�.
	strFriendInfo*	GetFriend( SI32 index );								// index�� Ư�� ģ���� ������ ���´�.
	SI32			GetNumFriends();										// ��ϵ� ģ���� ���� ���´�.
	BOOL			IsLogin( CHAR *szID );									// �α������ΰ�?
	VOID			SetLogin( CHAR *szID, BOOL bLogin );					// ģ���� �α���/�α׾ƿ� ���¸� �����Ѵ�.
	BOOL			IsDenial( CHAR *szID );									// ���Űźε� ģ���ΰ�?
	BOOL			SetDenial( CHAR *szID, BOOL bDenial );					// ģ���� ���Űź� ����/���� ���¸� �����Ѵ�.
	FRESULT			AddFriend( CHAR *szMyID, CHAR *szID, BOOL bLogin );		// ģ���� �߰��Ѵ�.
	BOOL			DeleteFriend( CHAR *szID );								// ģ���� �����Ѵ�.
	VOID			DeleteAllFriend();
};


#endif
