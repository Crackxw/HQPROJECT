#include "GSL.h"
#include "Friend.h"
#include "cltList.h"
#include "Defines.h"


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 儅撩濠
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
cltFriendList::cltFriendList()
{
	Init();
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 模資濠
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
cltFriendList::~cltFriendList()
{
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 蟾晦��
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
VOID	cltFriendList::Init()
{
	Create( MAX_FRIEND_NUM );
}


/*
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘 葬蝶お曖 ん檣攪蒂 橢橫螞棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
strFriendInfo*	cltFriendList::GetFriendList()
{
}
*/


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// か薑 耀掘曖 薑爾蒂 橢橫螞棻. 跡煙縑 氈戲賊 掘褻羹 ん檣攪蒂 葬欐ж堅 橈戲賊 NULL擊 葬欐и棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
strFriendInfo*	cltFriendList::GetFriend( CHAR *szID )
{
	strFriendInfo	*pstFriend;

	if(szID == NULL) return NULL;

	ResetPos();
	while( (pstFriend = GetNextData()) != NULL )
	{
		if( strcmp(szID, pstFriend->szID) == 0 )		return pstFriend;
	}

	return NULL;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// か薑 耀掘曖 薑爾蒂 橢橫螞棻. 跡煙縑 氈戲賊 掘褻羹 ん檣攪蒂 葬欐ж堅 橈戲賊 NULL擊 葬欐и棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
strFriendInfo*	cltFriendList::GetFriend( SI32 index )
{
	return GetData( index );
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 蛔煙脹 耀掘曖 熱蒂 橢橫螞棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
SI32	cltFriendList::GetNumFriends()
{
	return GetLength();
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘陛 煎斜檣 鼻鷓檜賊 TRUE蒂 葬欐и棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
BOOL	cltFriendList::IsLogin( CHAR *szID )
{
	strFriendInfo	*pstFriend;

	if( (pstFriend = GetFriend(szID)) != NULL )
	{
		if( pstFriend->bLogin )		return TRUE;
	}

	return FALSE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘蒂 煎斜檣 鼻鷓煎 虜萇棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
VOID	cltFriendList::SetLogin( CHAR *szID, BOOL bLogin )
{
	strFriendInfo	*pstFriend;

	if( (pstFriend = GetFriend(szID)) != NULL )
	{
		pstFriend->bLogin	=	bLogin;
	}
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 熱褐剪睡 鼻鷓檣 耀掘檣陛?
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
BOOL	cltFriendList::IsDenial( CHAR *szID )
{
	strFriendInfo	*pstFriend;

	if( (pstFriend = GetFriend(szID)) != NULL )
	{
		if( pstFriend->bDenial )		return TRUE;
	}

	return FALSE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘蒂 熱褐剪睡 鼻鷓煎 虜萇棻. 憮幗陛 嶸雖ж朝 葬蝶お檜嘎煎 DB諦朝 鼻婦 橈棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
BOOL	cltFriendList::SetDenial( CHAR *szID, BOOL bDenial )
{
	strFriendInfo	*pstFriend;

	if( (pstFriend = GetFriend(szID)) != NULL )
	{
		pstFriend->bDenial	=	bDenial;
	}

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘蒂 跡煙縑 蹺陛и棻. 憮幗陛 嶸雖ж朝 葬蝶お檜嘎煎 DB諦朝 鼻婦 橈棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
FRESULT	cltFriendList::AddFriend( CHAR *szMyID, CHAR *szNewID, BOOL bLogin )
{
	strFriendInfo	stFriend;

	// 耀掘陛 檜嘐 蛔煙腎橫 氈戲賊 褒ぬ 籀葬и棻.
	if( GetFriend(szNewID) != NULL )				return FRIEND_ALREADY_EXIST;
	// 濠晦 濠褐擊 蹺陛ж溥朝 唳辦 褒ぬ籀葬и棻.
	if( strcmp(szMyID, szNewID) == 0 )				return FRIEND_CANNOT_ADD_MYSELF;

	// 掘褻羹蒂 瓣遴棻.
	ZeroMemory(stFriend.szID, sizeof(stFriend.szID));
	strncpy( stFriend.szID, szNewID, ON_ID_LENGTH );
	stFriend.bLogin		=	bLogin;
	stFriend.bDenial	=	FALSE;

	// add a friend. it'll fail if friend list is full.
	if( Add( &stFriend ) == FALSE )					return FRIEND_FULL;

	return FRIEND_OK;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘蒂 跡煙縑憮 餉薯и棻. 憮幗陛 嶸雖ж朝 葬蝶お檜嘎煎 DB諦朝 鼻婦 橈棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
BOOL	cltFriendList::DeleteFriend( CHAR *szID )
{
	strFriendInfo	*pstFriend;

	if( szID == NULL )								return FALSE;
	// 耀掘陛 蛔煙腎橫 氈雖 彊戲賊 褒ぬ 籀葬и棻.
	if( (pstFriend = GetFriend(szID)) == NULL )		return FALSE;

	Delete( pstFriend );
	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// 耀掘 葬蝶お蒂 賅舒 餉薯и棻.
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
VOID	cltFriendList::DeleteAllFriend()
{
	MakeEmpty();
}
