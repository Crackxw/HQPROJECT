//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 필드 채팅창
//	File Name		: OnlineFieldChat.cpp
//	Birth Date		: 2001. 07. 25.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용자의 콘솔에서 채팅창을 출력하는 클래스입니다.
//
//===================================================================================================

#ifndef __ONLINE_FIELDCHAT_H__
#define __ONLINE_FIELDCHAT_H__

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
enum WND_RECT
{
	WND_RECT_INVALID = -1,

	WND_RECT_SMALL_CHATBOX,
	WND_RECT_SMALL_CHATBOX_CONTENT,

	WND_RECT_BIG_CHATBOX,
	WND_RECT_BIG_CHATBOX_CONTENT,
	
	WND_RECT_VIEW_SMALLCHATBOX_SCROLLBAR,
	WND_RECT_TRUE_SMALLCHATBOX_SCROLLBAR,

	WND_RECT_VIEW_BIGCHATBOX_SCROLLBAR,
	WND_RECT_TRUE_BIGCHATBOX_SCROLLBAR,

	WND_RECT_SMALLCHATBOX_UP_SCROLL_BTN,
	WND_RECT_BIGCHATBOX_UP_SCROLL_BTN,
	
	WND_RECT_MID_SCROLL_BTN,
	WND_RECT_DOWN_SCROLL_BTN,

	WND_RECT_MAX,
};
class IUIWnd;
//<--
#endif

//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: 포함 헤더 선언 및 외부 클래스 정의
//-----------------------------------------------------------------------------
#include "OnlineChatList.h"
#define ON_CHAT_MODE_COUNT			3

//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: 필드 채팅창 클래스
//-----------------------------------------------------------------------------
class	OnlineFieldChat
{
private:
	cltOnlineWorld	*m_pMyOnlineWorld;						// 게임 몸통 클래스. 외부에서 받아옴
	OnlineChatList	m_ChatList[ON_CHAT_MODE_COUNT];			// 채팅 목록 박스
	_Button			m_Btn_ChatMode[ON_CHAT_MODE_COUNT];		// 버튼 UI - 채팅 모드

	XSPR			m_Spr_Btn_ChatMode, m_Spr_Frm_UnderBar;	// 스프라이트들 - 채팅모드 버튼 이미지, 채팅창 하단 바 이미지

	SI16			m_siChatBoardXPos, m_siChatBoardYPos;	// 채팅 출력 화면 좌표
	SI16			m_siAddDistY;							// 채팅 출력 좌표 Y 보정값
	SI16			m_siModeBtnXPos, m_siModeBtnYPos;		// 설정 버튼 좌표

	UI08			m_uiChatMode;							// 0 : 일반채팅, 1 : 상단채팅, 2 : 파티채팅, 3 : 귓속말
	SI16			m_siStatus;								// 채팅창 출력 상태
	DWORD			m_dwLastUpdateTime;						// 채팅창 출력 상태를 바꾸었을 때의 시간

	SI32			m_siNoField, m_siButtonNoField;			// 필드 지정 불가를 선언할 때의 핸들
	BOOL			m_bMouseDown;

public:
	OnlineFieldChat();
	~OnlineFieldChat();

	VOID			Poll();
	VOID			Initialize();
	VOID			Init(cltOnlineWorld *pOnlineWorld, SI32 siAddDistY = 0 );	// 초기화
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );						// 미니맵을 출력한다.
	VOID			Free();
	BOOL			AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode);
	BOOL			AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode);
	BOOL			AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode);

	VOID			SetHeightStatus( SI16 siHeight );	// 0 : CHAT_HEIGHT_CLOSE, 1 : CHAT_HEIGHT_MEDIUM, 2 : CHAT_HEIGHT_HIGH
	SI16			GetHeightStatus();

	VOID			SetMode(UI08 uiChatMode);

	UI08			GetMode()	{ return m_uiChatMode; };
	VOID			SetDistY( SI32 siAddDistY = 0 );

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	void AddInChatBox(const char* pszID, const char* pszStr, const int nChatMode);
	void ChangeChatBoxForm(int nType);

protected:
	void CreateChatBox();
	void ChangeChatBoxType(const int nChatBoxType);

private:
	void InitWndRectContainer();

	IUIWnd* CreateNormalChatBox();
	IUIWnd* CreateGuildChatBox();
	IUIWnd* CreateNoticeChatBox();
	
	void CreateChatBoxScroll(IUIWnd* pAttachWindow);
	void CreateChatBoxSurfaceBuffer();

	void MoveChatBox(const int y);
	int  GetChatModeOfChatWndID(const int nChatMode);

private:
	int  m_nChatBoxRowCount[ON_CHAT_MODE_COUNT];
	RECT m_rtWndRect[WND_RECT_MAX];
//<--
#endif
};


#endif//__ONLINE_FIELDCHAT_H__