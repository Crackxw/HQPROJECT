/********************************************************************************************
	파일명	:	OnlineMessenger.h
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2002.07.06
	수정일  :	
********************************************************************************************/

#ifndef __ONINLE_MESSENGER_H
#define __ONLINE_MESSENGER_H

#include "OnlineChatList.h"

#define ON_MAX_CHAT_CONTENT	44

#define	MESSENGER_POLL			0
#define	MESSENGER_SEND			1
#define	MESSENGER_RECEIVE		2
#define	MESSENGER_DRAGING		3


#define MESSENGER_FRIEND_MAX_NUM	10


typedef struct
{
	char		szFriendName[ON_ID_LENGTH + 1];
	char        szGuildName[ON_GUILDNAME_LENGTH + 2];
	SI32        siClassInGuildID;
	SI32		siLevel;
} FriendInfo;


class OnlineMessenger
{
private:
	cltOnlineWorld		*m_pMyOnlineWorld;
	BOOL				m_bAction;
	BOOL				m_bMouseDown;
	BOOL				m_bIsNewMessage[MESSENGER_FRIEND_MAX_NUM];
	BOOL				m_bBlink;
	BOOL				m_bMini;
	SI16				m_siX, m_siY;
	SI16				m_siNoField;
	DWORD				m_dwTime;
	SI08				m_siStatus;
	SI08				m_siTabStatus;

	SI16				m_siDragDifX, m_siDragDifY;
	RECT				m_rtEditRect;
	RECT				m_rtDragRect;
	FriendInfo			m_FriendInfo[MESSENGER_FRIEND_MAX_NUM];

	CHAR				m_szSendContent[ON_MAX_CHAT_CONTENT + 1];
	CHAR				m_szReceiveContent[ON_MAX_CHAT_CONTENT + 1];

	XSPR				m_MainSpr, m_NameButSpr, m_MiniSpr, m_CloseSpr, m_ScrBarSpr;

	_Button				m_BTab[MESSENGER_FRIEND_MAX_NUM], m_BScrollBar, m_BMini, m_BClose;
//	OnlineEditControl	*m_pInputEditBox;
	HOEC				m_hOecEditBox;									// 에디트 박스 UI 핸들- 
	OnlineChatList		m_ChatList[MESSENGER_FRIEND_MAX_NUM];
public:
	BOOL				m_bWhisperOn;

public:
	OnlineMessenger();
	~OnlineMessenger();

	VOID				Free();																// 날림
	VOID				Init(cltOnlineWorld *pMyOnlineWorld);								// 초기화
	VOID				Action();															// 메인 폴
	VOID				SetAction(BOOL bFlag, CHAR *pFriendName = NULL);
	BOOL				IsAction()					{ return m_bAction; };
	VOID				Draw( LPDIRECTDRAWSURFACE7 pSurface );								// 출력
	BOOL				AddChat(CHAR *szID, CHAR *pStr);
	VOID				SetFocusEditBox();
	VOID				SetStatus(SI08 siStatus)	{ m_siStatus = siStatus; };
	VOID				SetMini(BOOL bMini);
	BOOL				IsMini()					{ return m_bMini; };
	VOID				SetTabStatus(SI08 siStatus);
	VOID				SetGuildInfo(CHAR *szID, CHAR *szGuildName, SI32 siClassInGuildID, SI32 siLevel = 1);
	BOOL				IsNewMessage();
};

#endif