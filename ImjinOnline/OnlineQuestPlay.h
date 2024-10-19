//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 수행원과의 대화창
//	File Name		: OnlineQuestPlay.h
//	Birth Date		: 2002. 07. 08.
//	Creator			: 이 윤 석
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		인터페이스에서 수행인에게 퀘스트 관련 정보 및 수락/취소를 하는 창을 띄우는 부분입니다.
//
//===================================================================================================

#ifndef __ONLINE_QUESTPLAY_H__
#define __ONLINE_QUESTPLAY_H__


//-----------------------------------------------------------------------------
// Name: OnlineQuestPlay
// Code: All (2004-02-16)
// Desc: 클래스 정의
//-----------------------------------------------------------------------------
class OnlineQuestPlay
{
private:	
	cltOnlineWorld*		m_pOnlineWorld;								// 통합 게임부 클래스의 포인터, 외부에서 받아온다.

	XSPR*				m_pSpr_Frm;									// 스프라이트들 - 프레임(외부의 리소스로 적용)
	XSPR				m_Spr_Btn;									// 스프라이트들 - 버튼

	_Button				m_Accept;									// 버튼 UI - Quest 수락 버튼
	_Button				m_QuestCancel;								// 버튼 UI - Quest 취소 버튼 
	_Button				m_Cancel;									// 버튼 UI - 대화 취소 버튼
	_Button				m_Next;										// 버튼 UI - 다음 대화로
	_Button				m_End;										// 버튼 UI - 대화 끝

	BOOL				m_bAction;
	BOOL				m_bMouseDown;
	SI16				m_siNoFieldHandle;

	SI16				m_QuestCode;								// 퀘스트 인덱스
	SI16				m_ChattingType;								// Chatting Type
	SI16				m_ChattingNowPage;							// Chatting Now Page
	SI16				m_ChattingMaxPage;							// Chatting Max Page
	SI16				m_ChattingMaxSize;
	SI16				m_ChattingScroll;

	HRGN				m_TextClipper;
	HRGN				m_TextClipperOld;

	SI32				m_QuestStartBeforePrivQuestNPC;				// 대답이 Start Before일때 이전에 하지 않을 Quest가 있을경우

public:
	OnlineQuestPlay();
	~OnlineQuestPlay();

	BOOL			Init(cltOnlineWorld *pOnlineWorld);				// 초기화
	VOID			Free(void);										// 해제

	VOID			Poll(void);										// 처리
	VOID			Draw(LPDIRECTDRAWSURFACE7 pSurface);			// 출력

	VOID			SetAction(BOOL fAction, SI16 QuestCode = 0, SI16 ChattingType = 0);	// 활성화 한다.
	BOOL			IsAction(void) {return m_bAction;};							// 활성화 검사

protected:
	UI32			GetTextCode(void);								// 현재 사용돼는 Text를 얻어옴
	SI32			GetTextMaxLine(HDC hDC);						// 현재 사용돼는 Text의 최대 Line을 얻어옴
	SI32			GetPrevQuestCompleted(SQuestHeader* lpQuest);	// 이전에 이 Quest를 한적이 있는지 검사. 이전에 한적이 없으면 그 Quest의 NPC Code를 넘겨준다.
};


#endif//__ONLINE_QUESTPLAY_H__
