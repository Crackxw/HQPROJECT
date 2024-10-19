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

//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: ���� ���
//-----------------------------------------------------------------------------
#include <GSL.h>

#include <mouse.h>

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
#include "OnlineChatAbilityButton.h"

#include "..\GersangUI\UIGSInfoHeader.h"
#include "..\GersangUI\UIGSPreCompileHeader.h"
#include "..\GersangUI\UIGSMgr.h"
#include "..\GersangUI\UIGSSurfaceBufferMgr.h"

const int SCROLLBAR_GAP = 13;
const int SCROLLBAR_SMALLCHATBOX_LINE = 4;
const int SCROLLBAR_BIGCHATBOX_LINE   = 7;
//<--
#endif


#include "OnlineWorld.h"
#include "OnlineFieldChat.h"
#include "OnlineFont.h"
#include "OnlineFieldArea.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include "OnlineHelp.h"
#include "OnlineTrade.h"


//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: �ܺ� Ŭ���� ���� �� ���ǵ�
//-----------------------------------------------------------------------------
// ä�� ����Ʈ â�� ǥ�� ����
#define	ON_CHAT_STATUS_CLOSE				0		// ä�� ����Ʈ ������ ����
#define	ON_CHAT_STATUS_MEDIUMUP				1		// ä�� ����Ʈ �߰� ���·� �ö���� �ִ� ����
#define	ON_CHAT_STATUS_MEDIUM				2		// ä�� ����Ʈ �߰� ����
#define	ON_CHAT_STATUS_HIGHUP				3		// ä�� ����Ʈ �ִ� ���·� �ö���� �ִ� ����
#define	ON_CHAT_STATUS_HIGH					4		// ä�� ����Ʈ �ִ� ����

// ä�� ����Ʈ â�� ������
#define ON_CHAT_BACKIMAGE_WIDTH				398
#define ON_CHAT_BACKIMAGE_HEIGHT			65

// ä�� ����Ʈ â ǥ�� ���¿� ���� ���̰�
#define ON_CHAT_MAINBOARD_X					201
#define ON_CHAT_MAINBOARD_CLOSE_Y			574
#define ON_CHAT_MAINBOARD_MEDIUM_Y			509
#define ON_CHAT_MAINBOARD_HIGH_Y			470
#define ON_CHAT_EDITBAR_HEIGHT				25

// ä�� ��� ��ư ������
#define ON_CHAT_MODEBUTTON_X				204
#define ON_CHAT_MODEBUTTON_Y				11
#define ON_CHAT_MODEBUTTON_WIDTH			56
#define ON_CHAT_MODEBUTTON_HEIGHT			13

extern _InputDevice		IpD;

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
const int CHATBOX_SCROLLBAR_GAP = 5;
const int CHATBOX_CONTENT_GAP   = 9;
//<--
#endif


//-----------------------------------------------------------------------------
// Name: OnlineFieldChat()
// Code: All (2004-02-17)
// Desc: ������
//-----------------------------------------------------------------------------
OnlineFieldChat::OnlineFieldChat()
{
	m_siNoField = -1;
	m_siButtonNoField = -1;
	m_uiChatMode = CHATTINGMODE_NORMAL;

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	memset(&m_nChatBoxRowCount, 0, sizeof(m_nChatBoxRowCount));
	InitWndRectContainer();
//<--
#endif
}

//-----------------------------------------------------------------------------
// Name: ~OnlineFieldChat()
// Code: All (2004-02-17)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
OnlineFieldChat::~OnlineFieldChat()
{
	Free();
}

VOID OnlineFieldChat::SetMode(UI08 uiChatMode)
{
	m_uiChatMode = uiChatMode; 

#ifdef _USE_GSUI
	//--> Test Code(sagolboss)
	ChangeChatBoxType(uiChatMode);
	//<--
#endif

}

#ifdef _USE_GSUI
/** @author �輺�� 
	@date   2004-10-05 16:23
	@par    ä��â�� ���õ� �������̽��� �ʱ� ��ǥ�� �����մϴ�
*/

void OnlineFieldChat::InitWndRectContainer()
{
	SetRect(&m_rtWndRect[WND_RECT_SMALL_CHATBOX],         204, 468, 580, 529);
	SetRect(&m_rtWndRect[WND_RECT_SMALL_CHATBOX_CONTENT], 214, 478, 590, 539);
	
	SetRect(&m_rtWndRect[WND_RECT_BIG_CHATBOX],         204, 438, 580, 538);
	SetRect(&m_rtWndRect[WND_RECT_BIG_CHATBOX_CONTENT], 214, 448, 590, 548);

	SetRect(&m_rtWndRect[WND_RECT_VIEW_SMALLCHATBOX_SCROLLBAR],  580, 468, 602, 529);
	SetRect(&m_rtWndRect[WND_RECT_TRUE_SMALLCHATBOX_SCROLLBAR],  582, 486, 600, 518);

	SetRect(&m_rtWndRect[WND_RECT_VIEW_BIGCHATBOX_SCROLLBAR],  580, 430, 602, 438);
	SetRect(&m_rtWndRect[WND_RECT_TRUE_BIGCHATBOX_SCROLLBAR],  582, 448, 600, 518);
	
	SetRect(&m_rtWndRect[WND_RECT_SMALLCHATBOX_UP_SCROLL_BTN],   582, 473, 595, 478);
	SetRect(&m_rtWndRect[WND_RECT_BIGCHATBOX_UP_SCROLL_BTN],     582, 443, 595, 455);

	SetRect(&m_rtWndRect[WND_RECT_MID_SCROLL_BTN],  0, 0, 14, 11);
	SetRect(&m_rtWndRect[WND_RECT_DOWN_SCROLL_BTN], 582, 519, 595, 533);
}

IUIWnd* OnlineFieldChat::CreateNormalChatBox()
{
	// ä�� �ڽ� �����
	CUIGSMultiListbox* pUIGSListBox = GetUIGSMgr()->CreateGSMultiListBox(m_rtWndRect[WND_RECT_BIG_CHATBOX], 
																		 m_pMyOnlineWorld->pOnlineDirectDraw,
																		 STANDARD_BASELINE_WIDTH, 
																		 UIWND_ID_MULTILISTBOX_NORMAL_CHATBOX, 
																		 UIWND_ORDER_MULTILISTBOX_CHATBOX);
	assert(pUIGSListBox);
	pUIGSListBox->SetSpriteImage("GersangUI\\MAIN_CHAT_BASE_4.Spr");
	pUIGSListBox->SetRealArea(m_rtWndRect[WND_RECT_SMALL_CHATBOX].left, m_rtWndRect[WND_RECT_SMALL_CHATBOX].top);

	return pUIGSListBox;
}

IUIWnd* OnlineFieldChat::CreateGuildChatBox()
{
	// ä�� �ڽ� �����
	CUIGSMultiListbox* pUIGSListBox = GetUIGSMgr()->CreateGSMultiListBox(m_rtWndRect[WND_RECT_BIG_CHATBOX], 
																		 m_pMyOnlineWorld->pOnlineDirectDraw,
																		 STANDARD_BASELINE_WIDTH, 
																		 UIWND_ID_MULTILISTBOX_GUILD_CHATBOX, 
																		 UIWND_ORDER_MULTILISTBOX_CHATBOX);
	assert(pUIGSListBox);
	pUIGSListBox->SetSpriteImage("GersangUI\\MAIN_CHAT_BASE_4.Spr");
	pUIGSListBox->SetRealArea(m_rtWndRect[WND_RECT_SMALL_CHATBOX].left, m_rtWndRect[WND_RECT_SMALL_CHATBOX].top);

	return pUIGSListBox;
}

IUIWnd* OnlineFieldChat::CreateNoticeChatBox()
{
	// ä�� �ڽ� �����
	CUIGSMultiListbox* pUIGSListBox = GetUIGSMgr()->CreateGSMultiListBox(m_rtWndRect[WND_RECT_BIG_CHATBOX], 
																		 m_pMyOnlineWorld->pOnlineDirectDraw,
																		 STANDARD_BASELINE_WIDTH, 
																		 UIWND_ID_MULTILISTBOX_NOTICE_CHATBOX, 
																		 UIWND_ORDER_MULTILISTBOX_CHATBOX);
	assert(pUIGSListBox);
	pUIGSListBox->SetSpriteImage("GersangUI\\MAIN_CHAT_BASE_4.Spr");
	pUIGSListBox->SetRealArea(m_rtWndRect[WND_RECT_SMALL_CHATBOX].left, m_rtWndRect[WND_RECT_SMALL_CHATBOX].top);

	return pUIGSListBox;
}

void OnlineFieldChat::CreateChatBoxSurfaceBuffer()
{
	for( int i = 0; i < ON_CHAT_MODE_COUNT; ++i)
	{
		// ä�ó����� �޸� ���ǽ��� ���� ���ǽ��� �����Ѵ�
		CUIGSSurfaceBuffer* pSurfaceBuffer = GetSurfaceBufferMgr()->CreateSurfaceBuffer(GetChatModeOfChatWndID(i), 
																						m_rtWndRect[WND_RECT_BIG_CHATBOX], 
																						m_pMyOnlineWorld->pOnlineDirectDraw);
		assert(pSurfaceBuffer);
		// ä�ùڽ��� ����Ʈ�� ���� ũ���̹Ƿ� ���� ��ǥ���� ������ �д� 
		pSurfaceBuffer->SetRealArea(m_rtWndRect[WND_RECT_SMALL_CHATBOX_CONTENT].left, m_rtWndRect[WND_RECT_SMALL_CHATBOX_CONTENT].top);
	}
}

void OnlineFieldChat::CreateChatBoxScroll(IUIWnd* pAttachWindow)
{
	/* date : 2004-10-05 16:23 (sagolboss)
	   par  : �Լ� �����Ͱ��� ������ ��ü��

	   CommandBinder ��ü���� ������� �Լ� �����Ͱ��� �Ҿ������ �ȴ� �׷��� ������ static���� �������־�� �Ѵ�
	   ������ CUIButton Ŭ�������� CommandBinder ��ü���� ������ �־�� �ȴ� 
	   ������ CommandBinder ��ü�� ���ø��̱� ������ CUIButton�� ���ø����� �ϴ��� �Լ� ���ø��� �̿��Ͽ��� �Ѵ�
	   VC++ 6.0 �Լ� ���ø��� ����� �����Ѵٸ� CUIButton Ŭ������ �Լ� ���ø��� �����ؼ� �����ϰ� ���� �Ҽ�
	   ������ VC++ 6.0 ������ �Լ� ���ø��� ����� �������� �ʱ� ������ �ε��� �ϰ� CUIButton Ŭ������ CommandBinder��
	   has-a ����� ������ �ʴ´�. CUIButton�� ���ø����� ������� �ʴ� ������ ���� CommandBinder Ŭ������ �����ϴ°�
	   ���� CUIButton�� ���ø����� ����ϴ°� �� �����ϱ� �����̴�. 
	*/

	static CommandBinder<CUIGSScrollBar> CBProcessMouseMove;
	static CommandBinder<CUIGSScrollBar> CBProcessMouseLButtonDown;
	static CommandBinder<CUIGSScrollBar> CBProcessMouseLButtonUp;

	static CommandBinder<CUIGSScrollBar> CBProcessUpBtnDown;
	static CommandBinder<CUIGSScrollBar> CBProcessDownBtnDown;

	// ��ũ�� �� ��ư ����� 
	CUIGSTextButton* pUIGSTextButtonUp = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_SMALLCHATBOX_UP_SCROLL_BTN], 
																	  	  m_pMyOnlineWorld->pOnlineDirectDraw,
																		  m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		  UIWND_ID_GTB_SCROLLUP);
	assert(pUIGSTextButtonUp);
	pUIGSTextButtonUp->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_UP.Spr");

	// ��ũ�� ��� ��ư �����
	CUIGSTextButton* pUIGSTextButtonMid = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_MID_SCROLL_BTN], 
																		   m_pMyOnlineWorld->pOnlineDirectDraw,
																		   m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		   UIWND_ID_GTB_SCROLLMID);
	assert(pUIGSTextButtonMid);
	pUIGSTextButtonMid->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_HANDLE.Spr");

	// ��ũ�� �ٿ� ��ư ����� 
	CUIGSTextButton* pUIGSTextButtonDown = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_DOWN_SCROLL_BTN], 
																		    m_pMyOnlineWorld->pOnlineDirectDraw,
																		    m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		    UIWND_ID_GTB_SCROLLDOWN);
	assert(pUIGSTextButtonDown);
	pUIGSTextButtonDown->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_DOWN.Spr");

	// ��ũ�ѹ� �����
	CUIGSScrollBar* pUIGSScrollBar = GetUIGSMgr()->CreateGSScrollBar(m_rtWndRect[WND_RECT_TRUE_SMALLCHATBOX_SCROLLBAR],
																	 m_rtWndRect[WND_RECT_VIEW_SMALLCHATBOX_SCROLLBAR], 
																	 m_pMyOnlineWorld->pOnlineDirectDraw, 
																	 pAttachWindow,
																	 pUIGSTextButtonMid,
																	 SCROLLBAR_GAP,
																	 FALSE,
																	 UIWND_ID_SCROLLBAR_CHATBOX, 
																	 UIWND_ORDER_LISTBOX_SCROLLBAR);
	assert(pUIGSScrollBar);
	pUIGSScrollBar->SetSpriteImage("GersangUI\\MAIN_CHAT_BASE_4_SCR.Spr");
	// ó�� ä�� �ڽ��� ũ��� �����̹Ƿ� 4ĭ�� ���̰� �Ѵ� 
	pUIGSScrollBar->SetFixedShowCount(SCROLLBAR_SMALLCHATBOX_LINE);

	// Command Ŭ������ �Լ� �����Ͱ� ���ε� ��Ű�� 
	CBProcessMouseMove.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseMove);
	CBProcessMouseLButtonDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseLButtonDown);
	CBProcessMouseLButtonUp.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseLButtonUp);

	CBProcessUpBtnDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->OnScrollButtonUp);
	CBProcessDownBtnDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->OnScrollButtonDown);

	// ��ũ�� ��ư�鿡 Command �� �߰��ϱ� 
	pUIGSTextButtonMid->AttachCommand(WM_MOUSEMOVE,   &CBProcessMouseMove);
	pUIGSTextButtonMid->AttachCommand(WM_LBUTTONDOWN, &CBProcessMouseLButtonDown);
	pUIGSTextButtonMid->AttachCommand(WM_LBUTTONUP,   &CBProcessMouseLButtonUp);

	pUIGSTextButtonUp->AttachCommand(WM_LBUTTONDOWN,   &CBProcessUpBtnDown);
	pUIGSTextButtonDown->AttachCommand(WM_LBUTTONDOWN, &CBProcessDownBtnDown);
}

/** @author �輺�� 
	@date   2004-10-05 16:23
	@par    ä��â�� ���õ� �������̽����� �ʱ�ȭ �մϴ�
*/
void OnlineFieldChat::CreateChatBox()
{
	IUIWnd* pAttachWindow = NULL;

	CreateChatBoxSurfaceBuffer();
	pAttachWindow = CreateNormalChatBox();
	CreateChatBoxScroll(pAttachWindow);

	CreateGuildChatBox();
	CreateNoticeChatBox();
}

/** @author �輺�� 
	@date   2004-10-05 16:23
	@par    ä��â�� ���õ� �������̽����� �����Դϴ� (Y�ุ)
	
	@param  const int y �����̰��� �ϴ� Y�� (���밪)
*/
void OnlineFieldChat::MoveChatBox(const int y)
{
	for( int i = 0; i < ON_CHAT_MODE_COUNT; ++i)
	{
		// ä�ùڽ��� �����δ� 
		GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(i))->SetRealArea(m_rtWndRect[WND_RECT_BIG_CHATBOX].left, y);
		
		// ä�ó����� ������ Surface�� �����δ� (������ �������� ��ǥ���� �ٲ۴�)
		GetSurfaceBufferMgr()->GetUIGSSurfaceBuffer(GetChatModeOfChatWndID(i))->SetRealArea(m_rtWndRect[WND_RECT_BIG_CHATBOX_CONTENT].left, y + CHATBOX_CONTENT_GAP);
	}

	// ��ũ�� �ٸ� �����δ�
	GetUIGSMgr()->GetGSMultiListBox(UIWND_ID_SCROLLBAR_CHATBOX)->MoveWindow(m_rtWndRect[WND_RECT_VIEW_BIGCHATBOX_SCROLLBAR].left, y);
	// ��ũ�� ���� ��ư�� �����δ� (CHATBOX_SCROLLBAR_GAP ��ŭ Gap�� �ش�)
	GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLUP)->MoveWindow(m_rtWndRect[WND_RECT_BIGCHATBOX_UP_SCROLL_BTN].left, y + CHATBOX_SCROLLBAR_GAP);
}

/** @author �輺�� 
	@date   2004-10-05 16:23
	@par    ä��â�� ���õ� �������̽����� ���� �����մϴ� 
	
	@param  int nType ä��â ���� Ÿ�� (CHAT_HEIGHT_HIGH   - ���� ����,
									    CHAT_HEIGHT_MEDIUM - ���� ����,
										CHAT_HEIGHT_CLOSE  - ���� ����)
*/
void OnlineFieldChat::ChangeChatBoxType(const int nChatBoxType)
{
	switch( nChatBoxType)
	{
	case CHATTINGMODE_NORMAL:
		{
			CUIGSMultiListbox* pcChatBox = GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(nChatBoxType));
			assert( pcChatBox);

			pcChatBox->AllItemLineUp();
			pcChatBox->ShowWindow(true);
			GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetAttachToWindow(pcChatBox);
			
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_GUILD))->ShowWindow(false);
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_PARTY))->ShowWindow(false);
		}
		break;
	
	case CHATTINGMODE_GUILD:
		{
			CUIGSMultiListbox* pcChatBox = GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(nChatBoxType));
			assert( pcChatBox);

			pcChatBox->AllItemLineUp();
			pcChatBox->ShowWindow(true);
			GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetAttachToWindow(pcChatBox);
			
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_NORMAL))->ShowWindow(false);
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_PARTY))->ShowWindow(false);
		}
		break;

	case CHATTINGMODE_PARTY:
		{
			CUIGSMultiListbox* pcChatBox = GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(nChatBoxType));
			assert( pcChatBox);

			pcChatBox->ShowWindow(true);
			pcChatBox->AllItemLineUp();
			GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetAttachToWindow(pcChatBox);
			
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_GUILD))->ShowWindow(false);
			GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(CHATTINGMODE_NORMAL))->ShowWindow(false);
		}
		break;
	}
}

int OnlineFieldChat::GetChatModeOfChatWndID(const int nChatMode)
{
	switch( nChatMode)
	{
	case CHATTINGMODE_NORMAL:
		return UIWND_ID_MULTILISTBOX_NORMAL_CHATBOX;
	
	case CHATTINGMODE_GUILD:
		return UIWND_ID_MULTILISTBOX_GUILD_CHATBOX;

	case CHATTINGMODE_PARTY:
		return UIWND_ID_MULTILISTBOX_NOTICE_CHATBOX;
	}

	return UIWND_ID_INVALID;
}

void OnlineFieldChat::ChangeChatBoxForm(int nType)
{
	// ä��â�� �������ٸ�
	if( nType == CHAT_HEIGHT_HIGH)
	{
		// ��ũ�ѹ��� ���� ũ�⸦ �����Ͽ� �ش� 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetScrollRect(m_rtWndRect[WND_RECT_TRUE_BIGCHATBOX_SCROLLBAR]);
		// ��ũ�ѹٿ� �������� ������ ������ ���Ѵ� 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetFixedShowCount(SCROLLBAR_BIGCHATBOX_LINE);
		// ��ũ�� ��� ��ư�� �������� ������ ä��â�� ������ ���� ��� �Ⱥ��̰� �Ѵ� 
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(false);
	}
	else if( nType == CHAT_HEIGHT_MEDIUM) // ä��â�� ���� ũ���� 
	{
		// ä��â�� ���� �ִٰ� ������ �����̹Ƿ� ä��â�� ���õ� �������̽����� �������� �Ѵ� 
		GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(GetMode()))->ShowWindow(true);
		GetUIGSMgr()->GetGSMultiListBox(UIWND_ID_SCROLLBAR_CHATBOX)->ShowWindow(true);

		GetSurfaceBufferMgr()->GetUIGSSurfaceBuffer(GetChatModeOfChatWndID(GetMode()))->Show(true);
		
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLUP)->ShowWindow(true);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(true);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLDOWN)->ShowWindow(true);

		// ��ũ�ѹ��� ���� ũ�⸦ �����Ͽ� �ش�
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetScrollRect(m_rtWndRect[WND_RECT_TRUE_SMALLCHATBOX_SCROLLBAR]);
		// ��ũ�ѹٿ� �������� ������ ������ ���Ѵ� 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetFixedShowCount(SCROLLBAR_SMALLCHATBOX_LINE);
		// ��ũ�� ��� ��ư�� �������� ������ ä��â�� ������ ���� ��� �Ⱥ��̰� �Ѵ� 
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(false);
	}
	else if( nType == CHAT_HEIGHT_CLOSE) // ä��â�� �����ٸ�
	{
		// ä��â�� ��� ������ ������ ä��â�� ���õ� �������̽����� �Ⱥ��̰� �Ѵ�
		GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(GetMode()))->ShowWindow(false);
		GetUIGSMgr()->GetGSMultiListBox(UIWND_ID_SCROLLBAR_CHATBOX)->ShowWindow(false);
		
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->ShowWindow(false);

		GetSurfaceBufferMgr()->GetUIGSSurfaceBuffer(GetChatModeOfChatWndID(GetMode()))->Show(false);

		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLUP)->ShowWindow(false);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(false);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLDOWN)->ShowWindow(false);
	}
}
#endif

//-----------------------------------------------------------------------------
// Name: Init()
// Code: All (2004-02-17)
// Desc: Ŭ���� �ʱ�ȭ. ���� ������ �ѹ��� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
VOID	OnlineFieldChat::Init( cltOnlineWorld *pOnlineWorld, SI32 siAddDistY )
{
	// �⺻ ����
	m_pMyOnlineWorld	= pOnlineWorld;

	m_siNoField			= -1;
	m_siButtonNoField	= -1;

	m_siStatus			= ON_CHAT_STATUS_MEDIUM;
	m_uiChatMode		= CHATTINGMODE_NORMAL;
	m_siAddDistY		= siAddDistY;

	m_siChatBoardXPos	= ON_CHAT_MAINBOARD_X;
	m_siChatBoardYPos	= ON_CHAT_MAINBOARD_MEDIUM_Y;
	m_siModeBtnXPos		= m_siChatBoardXPos + ON_CHAT_MODEBUTTON_X;
	m_siModeBtnYPos		= m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

	// �̹��� �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_mode.Spr",	m_Spr_Btn_ChatMode );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_base_2.Spr", m_Spr_Frm_UnderBar );
	
	// ��ư, ä��â ���ʵ� ����
	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos - m_siAddDistY + ON_CHAT_MODEBUTTON_HEIGHT );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, m_siChatBoardYPos - m_siAddDistY + ON_CHAT_BACKIMAGE_HEIGHT );

	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		// ä�ø�� ��ư ���� 
		m_Btn_ChatMode[i].Create(m_siModeBtnXPos+2 + (ON_CHAT_MODEBUTTON_WIDTH + 3) * i , m_siModeBtnYPos - m_siAddDistY + 2, 
			ON_CHAT_MODEBUTTON_WIDTH, ON_CHAT_MODEBUTTON_HEIGHT, "", BUTTON_PUT_LEFT, TRUE);

		// ä�� ��� �ڽ� �ʱ�ȭ
		m_ChatList[i].Init(m_pMyOnlineWorld, ON_INN_ID_CONTENT, ON_GAMESPR_CHATTING_BACK, ON_GAMESPR_PANNEL_SCROLL_BAR, 4, 60, 
			m_siChatBoardXPos,			m_siChatBoardYPos - m_siAddDistY, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			9,							59, 
			m_siChatBoardXPos + 9,		m_siChatBoardYPos - m_siAddDistY + 9, 13);
	}

	// ��ư �̸� ����
	m_Btn_ChatMode[0].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NORMALCHAT));
	m_Btn_ChatMode[1].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADECHAT));
	m_Btn_ChatMode[2].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(8000498));

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	// �⺻ ����
	m_pMyOnlineWorld	= pOnlineWorld;

	m_siNoField			= -1;
	m_siButtonNoField	= -1;

	m_siStatus			= ON_CHAT_STATUS_MEDIUM;
	m_uiChatMode		= CHATTINGMODE_NORMAL;
	m_siAddDistY		= siAddDistY;

	m_siChatBoardXPos	= ON_CHAT_MAINBOARD_X;
	m_siChatBoardYPos	= ON_CHAT_MAINBOARD_MEDIUM_Y;
	m_siModeBtnXPos		= m_siChatBoardXPos + ON_CHAT_MODEBUTTON_X;
	m_siModeBtnYPos		= m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

	// �̹��� �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_mode.Spr",	m_Spr_Btn_ChatMode );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_base_2.Spr", m_Spr_Frm_UnderBar );
	
	// ��ư, ä��â ���ʵ� ����
	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos - m_siAddDistY + ON_CHAT_MODEBUTTON_HEIGHT );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, m_siChatBoardYPos - m_siAddDistY + ON_CHAT_BACKIMAGE_HEIGHT );

	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		// ä�ø�� ��ư ���� 
		m_Btn_ChatMode[i].Create(m_siModeBtnXPos+2 + (ON_CHAT_MODEBUTTON_WIDTH + 3) * i , m_siModeBtnYPos - m_siAddDistY + 2, 
			ON_CHAT_MODEBUTTON_WIDTH, ON_CHAT_MODEBUTTON_HEIGHT, "", BUTTON_PUT_LEFT, TRUE);

		/*
		// ä�� ��� �ڽ� �ʱ�ȭ
		m_ChatList[i].Init(m_pMyOnlineWorld, ON_INN_ID_CONTENT, ON_GAMESPR_CHATTING_BACK, ON_GAMESPR_PANNEL_SCROLL_BAR, 4, 60, 
			m_siChatBoardXPos,			m_siChatBoardYPos - m_siAddDistY, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			9,							59, 
			m_siChatBoardXPos + 9,		m_siChatBoardYPos - m_siAddDistY + 9, 13);
		*/
	}

	// ��ư �̸� ����
	m_Btn_ChatMode[0].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NORMALCHAT));
	m_Btn_ChatMode[1].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADECHAT));
	m_Btn_ChatMode[2].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(8000498));

	CreateChatBox();
	new OnlineChatAbilityButton(pOnlineWorld->pOnlineDirectDraw, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
//<--
#endif
}

//-----------------------------------------------------------------------------
// Name: Free()
// Code: All (2004-02-17)
// Desc: Ŭ���� ����. ���� �Ҹ�� �ѹ��� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
VOID	OnlineFieldChat::Free()
{
	clGrp.FreeXspr(m_Spr_Btn_ChatMode);
	clGrp.FreeXspr(m_Spr_Frm_UnderBar);

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	delete GetChatAbilityButton();
//<--
#endif
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: All (2004-02-17)
// Desc: ������ �ٽ� �ʱ�ȭ�� �� ���
//-----------------------------------------------------------------------------
VOID	OnlineFieldChat::Initialize()
{
	m_siStatus			= ON_CHAT_STATUS_MEDIUM;
	m_uiChatMode		= CHATTINGMODE_NORMAL;

	m_siChatBoardYPos	= ON_CHAT_MAINBOARD_MEDIUM_Y;
	m_siModeBtnYPos		= m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siButtonNoField );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, ON_SCREEN_YSIZE - m_siAddDistY);
	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos + ON_CHAT_MODEBUTTON_HEIGHT - m_siAddDistY);
				
	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		m_Btn_ChatMode[i].SetY(m_siModeBtnYPos - m_siAddDistY);

		m_ChatList[i].Initialize();
		m_ChatList[i].SetY(m_siChatBoardYPos - m_siAddDistY);
		m_ChatList[i].SetMaxStrLine(4);
	}
}

/********************************************************************************************
	�ʱ�ȭ
********************************************************************************************/
VOID	OnlineFieldChat::Poll()
{
	SI08 i;
	POINT pt;
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;
	CHAR szTemp[1024];
	ZeroMemory( szTemp,  sizeof( szTemp ) );

	switch(m_siStatus)
	{
	case ON_CHAT_STATUS_MEDIUM:
	case ON_CHAT_STATUS_HIGH:
		{
			if( m_pMyOnlineWorld->pMyData->GetGuildClass() == 0 || m_pMyOnlineWorld->pMyData->GetGuildClass() == 90 )
				m_Btn_ChatMode[ CHATTINGMODE_GUILD ].SetAction(FALSE);
			else
				m_Btn_ChatMode[ CHATTINGMODE_GUILD ].SetAction(TRUE);

			// ���� �ŷ����̶�� '���ä��'�� ����� ���� ����.
			if( m_pMyOnlineWorld->pOnlineTrade->IsActive() == TRUE )
			{
				m_Btn_ChatMode[ CHATTINGMODE_GUILD ].SetAction( FALSE );
			}

			m_Btn_ChatMode[ CHATTINGMODE_PARTY ].SetAction(FALSE);
		
			for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
			{
				if(m_Btn_ChatMode[i].Process(m_bMouseDown) == TRUE)
				{
					m_uiChatMode = i;
					
					#ifdef _USE_GSUI
					//-->Test Code (sagolboss)
					SetMode(m_uiChatMode);
					//<--
					#endif
				}
			}
			
			if(m_pMyOnlineWorld->pOnlineHelp->IsActive() == FALSE)
			{
				if(m_Btn_ChatMode[0].IsInMouse() == TRUE)
					sprintf(szTemp, "%s(N)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NORMALCHAT));
				else if(m_Btn_ChatMode[1].IsInMouse() == TRUE)
					sprintf(szTemp, "%s(G)", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADECHAT));
				else if(m_Btn_ChatMode[2].IsInMouse() == TRUE)
					sprintf(szTemp, "%s", m_pMyOnlineWorld->pOnlineText->Get(8000498));

				if( szTemp[0] != 0 )
					m_pMyOnlineWorld->pOnlineHelp->SetText( szTemp, IpD.Mouse_X, IpD.Mouse_Y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
			}

			// ���, �κ��丮�� ������ ��� ��ġ�Ƿ� �ݴ´�.
//			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction() || m_pMyOnlineWorld->pOnlineInventory->IsActive())
//				SetHeightStatus(CHAT_HEIGHT_CLOSE);
		}
		break;
	case ON_CHAT_STATUS_MEDIUMUP:
		{
			SI16	StartPos = 0;
			float	TempFloat;
			DWORD	TickCount = GetTickCount() - m_dwLastUpdateTime;
			
			// ���� �ð��� ������ Poll �� �̵�
			if(TickCount > 270)		
			{
				TickCount = 270;
				m_siStatus = ON_CHAT_STATUS_MEDIUM;
				for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
					m_Btn_ChatMode[i].SetAction(TRUE);

				#ifdef _USE_GSUI
				//--> Test Code (sagolboss)
				// �� �̻� ä��â�� �ȿ������� �ȴٸ� ��ũ�ѹ� ��� ��ư�� �������� �Ѵ� 
				GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(true);
				//<--
				#endif
			}
			
			TempFloat = (float)sin(((float)(TickCount) / 3.0f) * 0.017444f);
			StartPos  = (int)(TempFloat * TempFloat * (float)(ON_CHAT_MAINBOARD_CLOSE_Y-ON_CHAT_MAINBOARD_MEDIUM_Y));
	
			m_siChatBoardYPos = ON_CHAT_MAINBOARD_CLOSE_Y - StartPos;
			m_siModeBtnYPos = m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

			for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
			{
				m_Btn_ChatMode[i].SetY(m_siModeBtnYPos - m_siAddDistY);
				
				m_ChatList[i].SetY(m_siChatBoardYPos - m_siAddDistY);
				m_ChatList[i].SetMaxStrLine(4);
			}
			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// ä�� �ڽ��� �����δ� 
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			//<-- Test Code
			#endif
			
			if(m_siStatus == ON_CHAT_STATUS_MEDIUM)
			{
				// �� �ʵ� ������ �����Ѵ�.
				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siButtonNoField);
				m_siButtonNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos + ON_CHAT_MODEBUTTON_HEIGHT - m_siAddDistY);
				m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, ON_SCREEN_YSIZE - m_siAddDistY);
			}
		}
		break;
	case ON_CHAT_STATUS_HIGHUP:
		{
			SI16	StartPos = 0;
			float	TempFloat;
			DWORD	TickCount = GetTickCount() - m_dwLastUpdateTime;
			
			// ���� �ð��� ������ Poll �� �̵�
			if(TickCount > 270)		
			{
				TickCount = 270;
				m_siStatus = ON_CHAT_STATUS_HIGH;
				for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
					m_Btn_ChatMode[i].SetAction(TRUE);

				#ifdef _USE_GSUI
				//--> Test Code (sagolboss)
				// �� �̻� ä��â�� �ȿ������� �ȴٸ� ��ũ�ѹ� ��� ��ư�� �������� �Ѵ�
				GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(true);
				//<--
				#endif
			}
			
			TempFloat = (float)sin(((float)(TickCount) / 3.0f) * 0.017444f);
			StartPos  = (int)(TempFloat * TempFloat * (float)(ON_CHAT_MAINBOARD_MEDIUM_Y-ON_CHAT_MAINBOARD_HIGH_Y));
	
			m_siChatBoardYPos = ON_CHAT_MAINBOARD_MEDIUM_Y - StartPos;
			m_siModeBtnYPos = m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

			for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
			{
				m_Btn_ChatMode[i].SetY(m_siModeBtnYPos - m_siAddDistY);
				
				m_ChatList[i].SetY(m_siChatBoardYPos - m_siAddDistY);
				m_ChatList[i].SetMaxStrLine(7);
			}

			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// ä�� �ڽ��� �����δ� 
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			//<-- Test Code
			#endif
			
			if(m_siStatus == ON_CHAT_STATUS_HIGH)
			{
				// �� �ʵ� ������ �����Ѵ�.
				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siButtonNoField);
				m_siButtonNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos + ON_CHAT_MODEBUTTON_HEIGHT - m_siAddDistY);
				m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siChatBoardXPos, m_siChatBoardYPos, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, ON_SCREEN_YSIZE - m_siAddDistY);
			}
		}
		break;
	}

	if( IpD.LeftPressSwitch )			m_bMouseDown	=	TRUE;
	else								m_bMouseDown	=	FALSE;
}

/********************************************************************************************
	����Ѵ�.
********************************************************************************************/
VOID	OnlineFieldChat::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;

	if(m_siStatus != ON_CHAT_STATUS_CLOSE)
		m_ChatList[m_uiChatMode].Process(pSurface, ON_FONT_CHAT2);			// ä�� ���â ���μ��� ȣ��

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		if(m_siStatus != ON_CHAT_STATUS_CLOSE)
		{
			// ä�ø�� ��ư�̹���
			clGrp.PutSpriteLightT(m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_Spr_Btn_ChatMode.Header.Xsize, m_Spr_Btn_ChatMode.Header.Ysize, 
				&m_Spr_Btn_ChatMode.Image[m_Spr_Btn_ChatMode.Header.Start[m_uiChatMode]]);

			// ä��â �ϴܹ�
			clGrp.PutSpriteLightT(ON_CHAT_MAINBOARD_X, ON_SCREEN_YSIZE - ON_CHAT_EDITBAR_HEIGHT - m_siAddDistY - m_Spr_Frm_UnderBar.Header.Ysize, m_Spr_Frm_UnderBar.Header.Xsize, m_Spr_Frm_UnderBar.Header.Ysize, m_Spr_Frm_UnderBar.Image);
		}

		clGrp.UnlockSurface(pSurface);
	}

	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		SelectObject(hdc, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
		SetBkMode(hdc, TRANSPARENT);

		if(m_siStatus != ON_CHAT_STATUS_CLOSE)
		{
			for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
			{
				if(m_uiChatMode == i)
					m_Btn_ChatMode[i].Put(hdc, (DWORD)0, RGB(207, 194, 285));
				else
					m_Btn_ChatMode[i].Put(hdc, (DWORD)0, RGB(150, 150, 150));
			}
		}
		pSurface->ReleaseDC(hdc);
	}
}

/********************************************************************************************
	Ȱ��ȭ ��ų ���ΰ�?
********************************************************************************************/
VOID	OnlineFieldChat::SetHeightStatus(SI16 siHeight )
{	
	int i;
	switch(siHeight)
	{
	case CHAT_HEIGHT_CLOSE:
		{
			m_siStatus = ON_CHAT_STATUS_CLOSE;

			m_siChatBoardYPos	= ON_CHAT_MAINBOARD_CLOSE_Y;
			m_siModeBtnYPos		= m_siChatBoardYPos - ON_CHAT_MODEBUTTON_Y;

			for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
			{
				m_Btn_ChatMode[i].SetY(m_siModeBtnYPos - m_siAddDistY);
				m_ChatList[i].SetY(m_siChatBoardYPos - m_siAddDistY);
			}

			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siButtonNoField);
			m_siNoField = -1;
			m_siButtonNoField = -1;

			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// ä�� �ڽ��� �����δ� (ä��â�� �� ������ �����д�)
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			// ä��â�� ���� ���� ���·� �ٲ۴�
			ChangeChatBoxForm(CHAT_HEIGHT_CLOSE);
			//<--
			#endif

			return;
		}
		break;
	case CHAT_HEIGHT_MEDIUM:
		{
			if(m_siStatus != ON_CHAT_STATUS_CLOSE)		return;		// ���� ���¿����� Medium ���·� �ö� �� �ִ�.
			m_siStatus = ON_CHAT_STATUS_MEDIUMUP;

			// ���, �κ��丮�� ������ ��� ��ġ�Ƿ� �ݴ´�.
/*			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction() || m_pMyOnlineWorld->pOnlineInventory->IsActive())
			{
				m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
			}*/
			
			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// ä��â�� ���� ���� ���·� �ٲ۴�
			ChangeChatBoxForm(CHAT_HEIGHT_MEDIUM);
			//<--
			#endif
		}
		break;
	case CHAT_HEIGHT_HIGH:
		{
			if(m_siStatus != ON_CHAT_STATUS_MEDIUM)		return;		// Medium ���¿����� Hight���·� �ö� �� �ִ�.
			m_siStatus = ON_CHAT_STATUS_HIGHUP;

			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// ä��â�� ���� ���� ���·� �ٲ۴�
			ChangeChatBoxForm(CHAT_HEIGHT_HIGH);
			//<--
			#endif
		}
		break;
	}

	m_dwLastUpdateTime = GetTickCount();

	for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
		m_Btn_ChatMode[i].SetAction(FALSE);
}

/********************************************************************************************
	���� Ȱ��ȭ �Ǿ� �ִ°� �˾� ����.
********************************************************************************************/
SI16	OnlineFieldChat::GetHeightStatus()
{
	if(m_siStatus == ON_CHAT_STATUS_MEDIUM)
		return CHAT_HEIGHT_MEDIUM;
	else if(m_siStatus == ON_CHAT_STATUS_HIGH)
		return CHAT_HEIGHT_HIGH;
	else
		return CHAT_HEIGHT_CLOSE;
}

#ifdef _USE_GSUI
/** @author �輺�� 
	@date   2004-10-05 16:23
	@par    ä��â�� ä�� ������ ���� ��Ų�� 
	
	@param  pszID     ä���� ����� ID
	@param  pszStr	  ä�� ����
	@param  nChatMode ä�� ��� 

	@see    m_nChatBoxRowCount ��������� ä�� �ټ�, ä�ó����� �����Ҷ� ĭ�� �ѱ�� ���ؼ� �ʿ�
*/
void OnlineFieldChat::AddInChatBox(const char* pszID, const char* pszStr, const int nChatMode)
{
	RECT  rtIDTextBox;
	RECT  rtChatTextBox;

	// ä�� ������� Į��
	DWORD dwDefaultColorID  = RGB(0, 255, 0);
	DWORD dwDefaultColorStr = RGB(255, 255, 0);
	DWORD dwHoverColorStr   = RGB(255, 255, 255);

	// Text Box ���� ũ�⸦ �����Ͽ� �ش� 
	SetRect(&rtIDTextBox,   0, 0, 70, 13);
	SetRect(&rtChatTextBox, 0, 0, 266, 13);
	
	// ID TextBox �����
	CUIGSTextButton* pIDTextBox = GetUIGSMgr()->CreateGSTextButton(rtIDTextBox, 
																   GetSurfaceBufferMgr()->GetSurfaceBuffer(GetChatModeOfChatWndID(GetMode())), 
																   m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2),
																   0);
	pIDTextBox->UpdateText(pszID, dwDefaultColorID, DT_LEFT, 0, FALSE);

	// Str TextBox ����� 
	CUIGSTextButton* pStrTextBox = GetUIGSMgr()->CreateGSTextButton(rtChatTextBox, 
																    GetSurfaceBufferMgr()->GetSurfaceBuffer(GetChatModeOfChatWndID(GetMode())), 
																	m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2),
																	0);
	pStrTextBox->UpdateText(pszStr, dwDefaultColorStr, DT_LEFT, 0);

	// nChatMode�� ���� ���� Ʋ���ٸ� ä�� ������ �������� �ʴ´� 
	if( GetMode() != nChatMode)
		pStrTextBox->ShowWindow(false);
	
	// ����Ʈ �ڽ��� �߰� ��Ű�� 
	CUIGSMultiListbox* pMultiListBox = GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(nChatMode));
	pMultiListBox->AddItem(m_nChatBoxRowCount[nChatMode], pIDTextBox);
	pMultiListBox->AddItem(m_nChatBoxRowCount[nChatMode], pStrTextBox);

	// ��ũ�ѿ� ������ ������Ʈ �����ش�
	GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->UpdateScrollBar();
	
	// nChatMode�� ���� ���� ���������� m_nChatBoxRowCount�� ���� �����ش� 
	if( GetMode() == nChatMode)
		m_nChatBoxRowCount[nChatMode]++; // ������ ���Ե� ä���� ���� ���� ä���� ���� �࿡ ������ �Ѵ�
}
#endif

/********************************************************************************************
	���ۿ� ���ڿ� �Է�
********************************************************************************************/
BOOL	OnlineFieldChat::AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode)
{
	// ä�� ��� ���ۿ� ������ �Ѵ�.
	LOGIN::sCharInfo	*pMyCharacterInfo	=	NULL;
	CHAR				*pCharName			=	NULL;

	if(uiID == 0)
	{
		pMyCharacterInfo = m_pMyOnlineWorld->pMyData->GetMyCharacters(m_pMyOnlineWorld->pMyData->GetSelectMyCharacterSlot());
		
		#ifdef _USE_GSUI
		//--> Test Code (sagolboss)	
		AddInChatBox(pMyCharacterInfo->name, pStr, uiChatMode);
		//<--
		#endif
		m_ChatList[uiChatMode].Add(pMyCharacterInfo->name, pStr);				// ���� ���Ѱ�
	}
	else																		// �ٸ� ����� ���Ѱ�
	{
		pMyCharacterInfo = m_pMyOnlineWorld->pMyData->GetMyCharacters(m_pMyOnlineWorld->pMyData->GetSelectMyCharacterSlot());

		pCharName = m_pMyOnlineWorld->pIOnlineChar->GetCharName(uiID);
		if(pCharName == NULL)
		{
			clGrp.Error("", "AddChat[%d]", uiID);
			return FALSE;
		}
		m_ChatList[uiChatMode].Add(pCharName, pStr);
	}
	return TRUE;
}

/********************************************************************************************
	���ۿ� ���ڿ� �Է�
********************************************************************************************/
BOOL	OnlineFieldChat::AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode)
{
	// ä�� ��� ���ۿ� ������ �Ѵ�.
	m_ChatList[uiChatMode].Add( pID, pStr);
	
	#ifdef _USE_GSUI
	//--> Test Code (sagolboss)
	AddInChatBox(pID, pStr, uiChatMode);
	//<--
	#endif
	return TRUE;
}

/********************************************************************************************
	���ۿ� ���ڿ� �Է�
********************************************************************************************/
BOOL	OnlineFieldChat::AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode)
{
	// ä�� ��� ���ۿ� ������ �Ѵ�.
	m_ChatList[uiChatMode].Add(pStr, siContentColor);			// ���� ���Ѱ�
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetDistY()
// Code: All (2004-02-17)
// Desc: Y���� �缳���Ѵ�.
//-----------------------------------------------------------------------------
VOID	OnlineFieldChat::SetDistY( SI32 siAddDistY )
{
	m_siAddDistY		= siAddDistY;

	if( m_siNoField != -1)			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
	if( m_siButtonNoField != -1)	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siButtonNoField );
	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		m_Btn_ChatMode[i].SetY(m_siModeBtnYPos - m_siAddDistY);
		m_ChatList[i].SetXY( 	m_siChatBoardXPos,			m_siChatBoardYPos - m_siAddDistY, 
								m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
								m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
								m_siChatBoardXPos + 9,		m_siChatBoardYPos - m_siAddDistY + 9 );
		m_ChatList[i].SetY(m_siChatBoardYPos - m_siAddDistY);
	}

	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos - m_siAddDistY + ON_CHAT_MODEBUTTON_HEIGHT );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, ON_SCREEN_YSIZE - m_siAddDistY );
}

