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

//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: 포함 헤더
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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include "OnlineHelp.h"
#include "OnlineTrade.h"


//-----------------------------------------------------------------------------
// Name: OnlineFieldChat
// Code: All (2004-02-17)
// Desc: 외부 클래스 선언 및 정의들
//-----------------------------------------------------------------------------
// 채팅 리스트 창의 표시 형태
#define	ON_CHAT_STATUS_CLOSE				0		// 채팅 리스트 닫혀진 상태
#define	ON_CHAT_STATUS_MEDIUMUP				1		// 채팅 리스트 중간 형태로 올라오고 있는 상태
#define	ON_CHAT_STATUS_MEDIUM				2		// 채팅 리스트 중간 상태
#define	ON_CHAT_STATUS_HIGHUP				3		// 채팅 리스트 최대 형태로 올라오고 있는 상태
#define	ON_CHAT_STATUS_HIGH					4		// 채팅 리스트 최대 상태

// 채팅 리스트 창의 사이즈
#define ON_CHAT_BACKIMAGE_WIDTH				398
#define ON_CHAT_BACKIMAGE_HEIGHT			65

// 채팅 리스트 창 표시 형태에 따른 높이값
#define ON_CHAT_MAINBOARD_X					201
#define ON_CHAT_MAINBOARD_CLOSE_Y			574
#define ON_CHAT_MAINBOARD_MEDIUM_Y			509
#define ON_CHAT_MAINBOARD_HIGH_Y			470
#define ON_CHAT_EDITBAR_HEIGHT				25

// 채팅 모드 버튼 사이즈
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
// Desc: 생성자
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
// Desc: 소멸자
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
/** @author 김성준 
	@date   2004-10-05 16:23
	@par    채팅창에 관련된 인터페이스의 초기 좌표를 지정합니다
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
	// 채팅 박스 만들기
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
	// 채팅 박스 만들기
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
	// 채팅 박스 만들기
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
		// 채팅내용의 메모리 서피스로 사용될 서피스를 생성한다
		CUIGSSurfaceBuffer* pSurfaceBuffer = GetSurfaceBufferMgr()->CreateSurfaceBuffer(GetChatModeOfChatWndID(i), 
																						m_rtWndRect[WND_RECT_BIG_CHATBOX], 
																						m_pMyOnlineWorld->pOnlineDirectDraw);
		assert(pSurfaceBuffer);
		// 채팅박스의 디폴트는 스몰 크기이므로 스몰 좌표값을 설정해 둔다 
		pSurfaceBuffer->SetRealArea(m_rtWndRect[WND_RECT_SMALL_CHATBOX_CONTENT].left, m_rtWndRect[WND_RECT_SMALL_CHATBOX_CONTENT].top);
	}
}

void OnlineFieldChat::CreateChatBoxScroll(IUIWnd* pAttachWindow)
{
	/* date : 2004-10-05 16:23 (sagolboss)
	   par  : 함수 포인터값을 저장할 객체들

	   CommandBinder 객체들이 살아지면 함수 포인터값을 잃어버린게 된다 그렇기 때문에 static으로 선언해주어야 한다
	   원래는 CUIButton 클래스들이 CommandBinder 객체들을 가지고 있어야 된다 
	   하지만 CommandBinder 객체가 템플릿이기 때문에 CUIButton도 템플릿으로 하던지 함수 템플릿을 이용하여야 한다
	   VC++ 6.0 함수 템플릿을 제대로 지원한다면 CUIButton 클래스에 함수 템플릿을 적용해서 간단하고 쉽게 할수
	   있으나 VC++ 6.0 에서는 함수 템플릿이 제대로 지원되지 않기 때문에 부득이 하게 CUIButton 클래스에 CommandBinder를
	   has-a 관계로 가지지 않는다. CUIButton을 템플릿으로 사용하지 않는 이유는 따로 CommandBinder 클래스를 관리하는것
	   보다 CUIButton을 템플릿으로 사용하는게 더 불편하기 때문이다. 
	*/

	static CommandBinder<CUIGSScrollBar> CBProcessMouseMove;
	static CommandBinder<CUIGSScrollBar> CBProcessMouseLButtonDown;
	static CommandBinder<CUIGSScrollBar> CBProcessMouseLButtonUp;

	static CommandBinder<CUIGSScrollBar> CBProcessUpBtnDown;
	static CommandBinder<CUIGSScrollBar> CBProcessDownBtnDown;

	// 스크롤 위 버튼 만들기 
	CUIGSTextButton* pUIGSTextButtonUp = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_SMALLCHATBOX_UP_SCROLL_BTN], 
																	  	  m_pMyOnlineWorld->pOnlineDirectDraw,
																		  m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		  UIWND_ID_GTB_SCROLLUP);
	assert(pUIGSTextButtonUp);
	pUIGSTextButtonUp->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_UP.Spr");

	// 스크롤 가운데 버튼 만들기
	CUIGSTextButton* pUIGSTextButtonMid = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_MID_SCROLL_BTN], 
																		   m_pMyOnlineWorld->pOnlineDirectDraw,
																		   m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		   UIWND_ID_GTB_SCROLLMID);
	assert(pUIGSTextButtonMid);
	pUIGSTextButtonMid->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_HANDLE.Spr");

	// 스크롤 다운 버튼 만들기 
	CUIGSTextButton* pUIGSTextButtonDown = GetUIGSMgr()->CreateGSTextButton(m_rtWndRect[WND_RECT_DOWN_SCROLL_BTN], 
																		    m_pMyOnlineWorld->pOnlineDirectDraw,
																		    m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2), 
																		    UIWND_ID_GTB_SCROLLDOWN);
	assert(pUIGSTextButtonDown);
	pUIGSTextButtonDown->SetSpriteImage("GersangUI\\MAIN_CHAT_SCR_DOWN.Spr");

	// 스크롤바 만들기
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
	// 처음 채팅 박스의 크기는 스몰이므로 4칸만 보이게 한다 
	pUIGSScrollBar->SetFixedShowCount(SCROLLBAR_SMALLCHATBOX_LINE);

	// Command 클래스에 함수 포인터값 바인딩 시키기 
	CBProcessMouseMove.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseMove);
	CBProcessMouseLButtonDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseLButtonDown);
	CBProcessMouseLButtonUp.BindCallback(pUIGSScrollBar, pUIGSScrollBar->ProcessMouseLButtonUp);

	CBProcessUpBtnDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->OnScrollButtonUp);
	CBProcessDownBtnDown.BindCallback(pUIGSScrollBar, pUIGSScrollBar->OnScrollButtonDown);

	// 스크롤 버튼들에 Command 들 추가하기 
	pUIGSTextButtonMid->AttachCommand(WM_MOUSEMOVE,   &CBProcessMouseMove);
	pUIGSTextButtonMid->AttachCommand(WM_LBUTTONDOWN, &CBProcessMouseLButtonDown);
	pUIGSTextButtonMid->AttachCommand(WM_LBUTTONUP,   &CBProcessMouseLButtonUp);

	pUIGSTextButtonUp->AttachCommand(WM_LBUTTONDOWN,   &CBProcessUpBtnDown);
	pUIGSTextButtonDown->AttachCommand(WM_LBUTTONDOWN, &CBProcessDownBtnDown);
}

/** @author 김성준 
	@date   2004-10-05 16:23
	@par    채팅창에 관련된 인터페이스들을 초기화 합니다
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

/** @author 김성준 
	@date   2004-10-05 16:23
	@par    채팅창에 관련된 인터페이스들을 움직입니다 (Y축만)
	
	@param  const int y 움직이고자 하는 Y값 (절대값)
*/
void OnlineFieldChat::MoveChatBox(const int y)
{
	for( int i = 0; i < ON_CHAT_MODE_COUNT; ++i)
	{
		// 채팅박스를 움직인다 
		GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(i))->SetRealArea(m_rtWndRect[WND_RECT_BIG_CHATBOX].left, y);
		
		// 채팅내용이 적히는 Surface를 움직인다 (실제로 찍혀지는 좌표값을 바꾼다)
		GetSurfaceBufferMgr()->GetUIGSSurfaceBuffer(GetChatModeOfChatWndID(i))->SetRealArea(m_rtWndRect[WND_RECT_BIG_CHATBOX_CONTENT].left, y + CHATBOX_CONTENT_GAP);
	}

	// 스크롤 바를 움직인다
	GetUIGSMgr()->GetGSMultiListBox(UIWND_ID_SCROLLBAR_CHATBOX)->MoveWindow(m_rtWndRect[WND_RECT_VIEW_BIGCHATBOX_SCROLLBAR].left, y);
	// 스크롤 위에 버튼을 움직인다 (CHATBOX_SCROLLBAR_GAP 만큼 Gap을 준다)
	GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLUP)->MoveWindow(m_rtWndRect[WND_RECT_BIGCHATBOX_UP_SCROLL_BTN].left, y + CHATBOX_SCROLLBAR_GAP);
}

/** @author 김성준 
	@date   2004-10-05 16:23
	@par    채팅창에 관련된 인터페이스들의 폼을 변경합니다 
	
	@param  int nType 채팅창 폼의 타입 (CHAT_HEIGHT_HIGH   - 높은 상태,
									    CHAT_HEIGHT_MEDIUM - 보통 상태,
										CHAT_HEIGHT_CLOSE  - 닫힌 상태)
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
	// 채팅창이 높아졌다면
	if( nType == CHAT_HEIGHT_HIGH)
	{
		// 스크롤바의 실제 크기를 변경하여 준다 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetScrollRect(m_rtWndRect[WND_RECT_TRUE_BIGCHATBOX_SCROLLBAR]);
		// 스크롤바에 보여지는 아이템 갯수를 정한다 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetFixedShowCount(SCROLLBAR_BIGCHATBOX_LINE);
		// 스크롤 가운데 버튼이 움직여야 함으로 채팅창이 움직일 동안 잠수 안보이게 한다 
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(false);
	}
	else if( nType == CHAT_HEIGHT_MEDIUM) // 채팅창이 보통 크기라면 
	{
		// 채팅창이 꺼져 있다가 켜지는 상태이므로 채팅창에 관련된 인터페이스들을 보여지게 한다 
		GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(GetMode()))->ShowWindow(true);
		GetUIGSMgr()->GetGSMultiListBox(UIWND_ID_SCROLLBAR_CHATBOX)->ShowWindow(true);

		GetSurfaceBufferMgr()->GetUIGSSurfaceBuffer(GetChatModeOfChatWndID(GetMode()))->Show(true);
		
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLUP)->ShowWindow(true);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(true);
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLDOWN)->ShowWindow(true);

		// 스크롤바의 실제 크기를 변경하여 준다
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetScrollRect(m_rtWndRect[WND_RECT_TRUE_SMALLCHATBOX_SCROLLBAR]);
		// 스크롤바에 보여지는 아이템 갯수를 정한다 
		GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->SetFixedShowCount(SCROLLBAR_SMALLCHATBOX_LINE);
		// 스크롤 가운데 버튼이 움직여야 함으로 채팅창이 움직일 동안 잠수 안보이게 한다 
		GetUIGSMgr()->GetGSUITextButton(UIWND_ID_GTB_SCROLLMID)->ShowWindow(false);
	}
	else if( nType == CHAT_HEIGHT_CLOSE) // 채팅창이 꺼졌다면
	{
		// 채팅창이 모두 꺼져야 함으로 채팅창에 관련된 인터페이스들을 안보이게 한다
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
// Desc: 클래스 초기화. 게임 생성시 한번만 호출한다.
//-----------------------------------------------------------------------------
VOID	OnlineFieldChat::Init( cltOnlineWorld *pOnlineWorld, SI32 siAddDistY )
{
	// 기본 설정
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

	// 이미지 로드
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_mode.Spr",	m_Spr_Btn_ChatMode );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_base_2.Spr", m_Spr_Frm_UnderBar );
	
	// 버튼, 채팅창 노필드 설정
	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos - m_siAddDistY + ON_CHAT_MODEBUTTON_HEIGHT );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, m_siChatBoardYPos - m_siAddDistY + ON_CHAT_BACKIMAGE_HEIGHT );

	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		// 채팅모드 버튼 생성 
		m_Btn_ChatMode[i].Create(m_siModeBtnXPos+2 + (ON_CHAT_MODEBUTTON_WIDTH + 3) * i , m_siModeBtnYPos - m_siAddDistY + 2, 
			ON_CHAT_MODEBUTTON_WIDTH, ON_CHAT_MODEBUTTON_HEIGHT, "", BUTTON_PUT_LEFT, TRUE);

		// 채팅 목록 박스 초기화
		m_ChatList[i].Init(m_pMyOnlineWorld, ON_INN_ID_CONTENT, ON_GAMESPR_CHATTING_BACK, ON_GAMESPR_PANNEL_SCROLL_BAR, 4, 60, 
			m_siChatBoardXPos,			m_siChatBoardYPos - m_siAddDistY, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			9,							59, 
			m_siChatBoardXPos + 9,		m_siChatBoardYPos - m_siAddDistY + 9, 13);
	}

	// 버튼 이름 세팅
	m_Btn_ChatMode[0].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NORMALCHAT));
	m_Btn_ChatMode[1].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADECHAT));
	m_Btn_ChatMode[2].SetMenuName(m_pMyOnlineWorld->pOnlineText->Get(8000498));

#ifdef _USE_GSUI
//--> Test Code (sagolboss)
	// 기본 설정
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

	// 이미지 로드
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_mode.Spr",	m_Spr_Btn_ChatMode );
	clGrp.LoadXspr( "Online\\GameSpr\\panel\\main_chat_base_2.Spr", m_Spr_Frm_UnderBar );
	
	// 버튼, 채팅창 노필드 설정
	m_siButtonNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_siModeBtnXPos + (ON_CHAT_MODEBUTTON_WIDTH + 3)* ON_CHAT_MODE_COUNT, m_siModeBtnYPos - m_siAddDistY + ON_CHAT_MODEBUTTON_HEIGHT );
	m_siNoField			= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siChatBoardXPos, m_siChatBoardYPos - m_siAddDistY, m_siChatBoardXPos + ON_CHAT_BACKIMAGE_WIDTH, m_siChatBoardYPos - m_siAddDistY + ON_CHAT_BACKIMAGE_HEIGHT );

	for(int i = 0; i < ON_CHAT_MODE_COUNT; i++)
	{
		// 채팅모드 버튼 생성 
		m_Btn_ChatMode[i].Create(m_siModeBtnXPos+2 + (ON_CHAT_MODEBUTTON_WIDTH + 3) * i , m_siModeBtnYPos - m_siAddDistY + 2, 
			ON_CHAT_MODEBUTTON_WIDTH, ON_CHAT_MODEBUTTON_HEIGHT, "", BUTTON_PUT_LEFT, TRUE);

		/*
		// 채팅 목록 박스 초기화
		m_ChatList[i].Init(m_pMyOnlineWorld, ON_INN_ID_CONTENT, ON_GAMESPR_CHATTING_BACK, ON_GAMESPR_PANNEL_SCROLL_BAR, 4, 60, 
			m_siChatBoardXPos,			m_siChatBoardYPos - m_siAddDistY, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			m_siChatBoardXPos + 386,	m_siChatBoardYPos - m_siAddDistY + 4, 
			9,							59, 
			m_siChatBoardXPos + 9,		m_siChatBoardYPos - m_siAddDistY + 9, 13);
		*/
	}

	// 버튼 이름 세팅
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
// Desc: 클래스 제거. 게임 소멸시 한번만 호출한다.
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
// Desc: 정보를 다시 초기화할 때 사용
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
	초기화
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

			// 현재 거래중이라면 '상단채팅'을 사용할 수가 없다.
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

			// 장부, 인벤토리가 떠있을 경우 겹치므로 닫는다.
//			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction() || m_pMyOnlineWorld->pOnlineInventory->IsActive())
//				SetHeightStatus(CHAT_HEIGHT_CLOSE);
		}
		break;
	case ON_CHAT_STATUS_MEDIUMUP:
		{
			SI16	StartPos = 0;
			float	TempFloat;
			DWORD	TickCount = GetTickCount() - m_dwLastUpdateTime;
			
			// 일정 시간이 지나면 Poll 로 이동
			if(TickCount > 270)		
			{
				TickCount = 270;
				m_siStatus = ON_CHAT_STATUS_MEDIUM;
				for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
					m_Btn_ChatMode[i].SetAction(TRUE);

				#ifdef _USE_GSUI
				//--> Test Code (sagolboss)
				// 더 이상 채팅창이 안움직여도 된다면 스크롤바 가운데 버튼을 보여지게 한다 
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
			// 채팅 박스를 움직인다 
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			//<-- Test Code
			#endif
			
			if(m_siStatus == ON_CHAT_STATUS_MEDIUM)
			{
				// 노 필드 지역을 설정한다.
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
			
			// 일정 시간이 지나면 Poll 로 이동
			if(TickCount > 270)		
			{
				TickCount = 270;
				m_siStatus = ON_CHAT_STATUS_HIGH;
				for(i = 0; i < ON_CHAT_MODE_COUNT; i++)
					m_Btn_ChatMode[i].SetAction(TRUE);

				#ifdef _USE_GSUI
				//--> Test Code (sagolboss)
				// 더 이상 채팅창이 안움직여도 된다면 스크롤바 가운데 버튼을 보여지게 한다
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
			// 채팅 박스를 움직인다 
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			//<-- Test Code
			#endif
			
			if(m_siStatus == ON_CHAT_STATUS_HIGH)
			{
				// 노 필드 지역을 설정한다.
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
	출력한다.
********************************************************************************************/
VOID	OnlineFieldChat::Draw( LPDIRECTDRAWSURFACE7 pSurface )
{
	HDC		hdc;

	if(m_siStatus != ON_CHAT_STATUS_CLOSE)
		m_ChatList[m_uiChatMode].Process(pSurface, ON_FONT_CHAT2);			// 채팅 목록창 프로세스 호출

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		if(m_siStatus != ON_CHAT_STATUS_CLOSE)
		{
			// 채팅모드 버튼이미지
			clGrp.PutSpriteLightT(m_siModeBtnXPos, m_siModeBtnYPos - m_siAddDistY, m_Spr_Btn_ChatMode.Header.Xsize, m_Spr_Btn_ChatMode.Header.Ysize, 
				&m_Spr_Btn_ChatMode.Image[m_Spr_Btn_ChatMode.Header.Start[m_uiChatMode]]);

			// 채팅창 하단바
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
	활성화 시킬 것인가?
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
			// 채팅 박스를 움직인다 (채팅창을 맨 밑으로 보내둔다)
			MoveChatBox(m_siChatBoardYPos - m_siAddDistY);
			// 채팅창의 폼을 닫힌 상태로 바꾼다
			ChangeChatBoxForm(CHAT_HEIGHT_CLOSE);
			//<--
			#endif

			return;
		}
		break;
	case CHAT_HEIGHT_MEDIUM:
		{
			if(m_siStatus != ON_CHAT_STATUS_CLOSE)		return;		// 닫힌 상태에서만 Medium 상태로 올라갈 수 있다.
			m_siStatus = ON_CHAT_STATUS_MEDIUMUP;

			// 장부, 인벤토리가 떠있을 경우 겹치므로 닫는다.
/*			if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction() || m_pMyOnlineWorld->pOnlineInventory->IsActive())
			{
				m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
			}*/
			
			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// 채팅창의 폼을 보통 상태로 바꾼다
			ChangeChatBoxForm(CHAT_HEIGHT_MEDIUM);
			//<--
			#endif
		}
		break;
	case CHAT_HEIGHT_HIGH:
		{
			if(m_siStatus != ON_CHAT_STATUS_MEDIUM)		return;		// Medium 상태에서만 Hight상태로 올라갈 수 있다.
			m_siStatus = ON_CHAT_STATUS_HIGHUP;

			#ifdef _USE_GSUI
			//--> Test Code (sagolboss)
			// 채팅창의 폼을 높은 상태로 바꾼다
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
	지금 활성화 되어 있는가 알아 본다.
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
/** @author 김성준 
	@date   2004-10-05 16:23
	@par    채팅창에 채팅 내용을 포함 시킨다 
	
	@param  pszID     채팅한 사람의 ID
	@param  pszStr	  채팅 내용
	@param  nChatMode 채팅 모드 

	@see    m_nChatBoxRowCount 현재까지의 채팅 줄수, 채팅내용을 기입할때 칸을 넘기기 위해서 필요
*/
void OnlineFieldChat::AddInChatBox(const char* pszID, const char* pszStr, const int nChatMode)
{
	RECT  rtIDTextBox;
	RECT  rtChatTextBox;

	// 채팅 내용들의 칼라
	DWORD dwDefaultColorID  = RGB(0, 255, 0);
	DWORD dwDefaultColorStr = RGB(255, 255, 0);
	DWORD dwHoverColorStr   = RGB(255, 255, 255);

	// Text Box 들의 크기를 지정하여 준다 
	SetRect(&rtIDTextBox,   0, 0, 70, 13);
	SetRect(&rtChatTextBox, 0, 0, 266, 13);
	
	// ID TextBox 만들기
	CUIGSTextButton* pIDTextBox = GetUIGSMgr()->CreateGSTextButton(rtIDTextBox, 
																   GetSurfaceBufferMgr()->GetSurfaceBuffer(GetChatModeOfChatWndID(GetMode())), 
																   m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2),
																   0);
	pIDTextBox->UpdateText(pszID, dwDefaultColorID, DT_LEFT, 0, FALSE);

	// Str TextBox 만들기 
	CUIGSTextButton* pStrTextBox = GetUIGSMgr()->CreateGSTextButton(rtChatTextBox, 
																    GetSurfaceBufferMgr()->GetSurfaceBuffer(GetChatModeOfChatWndID(GetMode())), 
																	m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2),
																	0);
	pStrTextBox->UpdateText(pszStr, dwDefaultColorStr, DT_LEFT, 0);

	// nChatMode가 현재 모드랑 틀리다면 채팅 내용을 보여주지 않는다 
	if( GetMode() != nChatMode)
		pStrTextBox->ShowWindow(false);
	
	// 리스트 박스에 추가 시키기 
	CUIGSMultiListbox* pMultiListBox = GetUIGSMgr()->GetGSMultiListBox(GetChatModeOfChatWndID(nChatMode));
	pMultiListBox->AddItem(m_nChatBoxRowCount[nChatMode], pIDTextBox);
	pMultiListBox->AddItem(m_nChatBoxRowCount[nChatMode], pStrTextBox);

	// 스크롤에 정보를 업데이트 시켜준다
	GetUIGSMgr()->GetGSScrollBar(UIWND_ID_SCROLLBAR_CHATBOX)->UpdateScrollBar();
	
	// nChatMode가 현재 모드랑 같을때에만 m_nChatBoxRowCount를 증가 시켜준다 
	if( GetMode() == nChatMode)
		m_nChatBoxRowCount[nChatMode]++; // 다음에 포함될 채팅이 지금 찍힌 채팅의 다음 행에 찍히게 한다
}
#endif

/********************************************************************************************
	버퍼에 문자열 입력
********************************************************************************************/
BOOL	OnlineFieldChat::AddChat(UI32 uiID, CHAR *pStr, UI08 uiChatMode)
{
	// 채팅 목록 버퍼에 삽입을 한다.
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
		m_ChatList[uiChatMode].Add(pMyCharacterInfo->name, pStr);				// 내가 말한거
	}
	else																		// 다른 사람이 말한거
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
	버퍼에 문자열 입력
********************************************************************************************/
BOOL	OnlineFieldChat::AddChatID(CHAR *pID, CHAR *pStr, UI08 uiChatMode)
{
	// 채팅 목록 버퍼에 삽입을 한다.
	m_ChatList[uiChatMode].Add( pID, pStr);
	
	#ifdef _USE_GSUI
	//--> Test Code (sagolboss)
	AddInChatBox(pID, pStr, uiChatMode);
	//<--
	#endif
	return TRUE;
}

/********************************************************************************************
	버퍼에 문자열 입력
********************************************************************************************/
BOOL	OnlineFieldChat::AddChat(CHAR *pStr, SI32 siContentColor, UI08 uiChatMode)
{
	// 채팅 목록 버퍼에 삽입을 한다.
	m_ChatList[uiChatMode].Add(pStr, siContentColor);			// 내가 말한거
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetDistY()
// Code: All (2004-02-17)
// Desc: Y값을 재설정한다.
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

