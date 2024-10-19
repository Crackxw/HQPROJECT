//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> �ʵ� ä��â
//	File Name		: OnlineFieldChat.cpp
//	Birth Date		: 2001. 07. 25.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ������� �ֿܼ��� ä��â�� ����ϴ� Ŭ�����Դϴ�.
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
// Desc: ���� ��� ���� �� �ܺ� Ŭ���� ����
//-----------------------------------------------------------------------------
#include "OnlineChatList.h"
#define ON_CHAT_MODE_COUNT			3

//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: �ʵ� ä��â Ŭ����
//-----------------------------------------------------------------------------
class	OnlineFieldChat
{
private:
	cltOnlineWorld	*m_pMyOnlineWorld;						// ���� ���� Ŭ����. �ܺο��� �޾ƿ�
	OnlineChatList	m_ChatList[ON_CHAT_MODE_COUNT];			// ä�� ��� �ڽ�
	_Button			m_Btn_ChatMode[ON_CHAT_MODE_COUNT];		// ��ư UI - ä�� ���

	XSPR			m_Spr_Btn_ChatMode, m_Spr_Frm_UnderBar;	// ��������Ʈ�� - ä�ø�� ��ư �̹���, ä��â �ϴ� �� �̹���

	SI16			m_siChatBoardXPos, m_siChatBoardYPos;	// ä�� ��� ȭ�� ��ǥ
	SI16			m_siAddDistY;							// ä�� ��� ��ǥ Y ������
	SI16			m_siModeBtnXPos, m_siModeBtnYPos;		// ���� ��ư ��ǥ

	UI08			m_uiChatMode;							// 0 : �Ϲ�ä��, 1 : ���ä��, 2 : ��Ƽä��, 3 : �ӼӸ�
	SI16			m_siStatus;								// ä��â ��� ����
	DWORD			m_dwLastUpdateTime;						// ä��â ��� ���¸� �ٲپ��� ���� �ð�

	SI32			m_siNoField, m_siButtonNoField;			// �ʵ� ���� �Ұ��� ������ ���� �ڵ�
	BOOL			m_bMouseDown;

public:
	OnlineFieldChat();
	~OnlineFieldChat();

	VOID			Poll();
	VOID			Initialize();
	VOID			Init(cltOnlineWorld *pOnlineWorld, SI32 siAddDistY = 0 );	// �ʱ�ȭ
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );						// �̴ϸ��� ����Ѵ�.
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