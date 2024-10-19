//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 퀘스트창
//	File Name		: OnlineQuestInterface.h
//	Birth Date		: 2004. 01. 01.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 사용자 콘솔에서 임무 수행 목록 창을 띄워주는 부분입니다.
//
//===================================================================================================

#pragma once 

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: 포함 헤더 선언 및 외부 클래스 정의
//-----------------------------------------------------------------------------
#include "OnlineCommonStruct.h"
#include "OnlineBaseLB.h"

class cltOnlineWorld;				// 통합 게임부 클래스 사용을 위한 선언


//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: 클래스 선언
//-----------------------------------------------------------------------------
class OnlineQuestInterface
{
private:
	cltOnlineWorld*		m_pMyOnlineWorld;		// 통합 게임부 클래스의 포인터, 외부에서 받아온다.

	_Button				m_btOK, m_btClose;		// 버튼 UI, OK와 Close 버튼
	_Button				m_btUp, m_btDown;		// 버튼 UI, Up, Down 스크롤 버튼
	OnlineSelectLB		m_LB_QuestList;			// 처음 화면에 뜨는 퀘스트 목록을 보여주기 위한 리스트 박스

	XSPR				m_Spr_Frm_List;			// 스프라이트들 - 퀘스트 목록창 프레임
	XSPR				m_Spr_Frm_Info;			// 스프라이트들 - 퀘스트 정보창 프레임
	XSPR				m_Spr_Btn_ScrollUp;		// 스프라이트들 - 위로 스크롤 버튼
	XSPR				m_Spr_Btn_ScrollDown;	// 스프라이트들 - 아래로 스크롤 버튼
	XSPR				m_Spr_Bar_List;			// 스프라이트들 - 퀘스트 목록창 작대기 스크롤
	XSPR				m_Spr_Bar_Info;			// 스프라이트들 - 퀘스트 정보창 작대기 스크롤
	XSPR				m_Spr_Btn_ScrollHandle;	// 스프라이트들 - 스크롤에서 쓰이는 핸들
	XSPR				m_Spr_Btn_Common;		// 스프라이트들 - 일반 버튼

	BOOL				m_bAction;	
	BOOL				m_bMousePush;
	SI32				m_nNoField;

	SI32				m_siSelect;
	COLORREF			m_SelectColor;
	
	SI32				m_nQuestDataStatus;		// QuestData 상태
	SPlayerQuest		m_QuestData;			// QuestData 구조체


public:
	OnlineQuestInterface( cltOnlineWorld *p ) : m_pMyOnlineWorld( p ) 	{ Init(); }
	~OnlineQuestInterface( void )										{ Free(); }


private:
	void	Init				( void );
	void	Free				( void );

	void	LoadImage			( void );
	void	FreeImage			( void );

	void	ToggleAction		( void )	{ m_bAction = !m_bAction; }
	BOOL	IsShowMainScroll	( void )	{ return ( GetCompletedQuestCount() + GetPlayingQuestCount() > 5 ? TRUE : FALSE ); }
//	void	SetQuestList		( void );

	void	Draw_QuestList		( LPDIRECTDRAWSURFACE7 pSurface );
	void	Draw_QuestInfo		( LPDIRECTDRAWSURFACE7 pSurface );

	void	Action_QuestList	( void );
	void	Action_QuestInfo	( void );

	void	(OnlineQuestInterface::*pfDraw)		( LPDIRECTDRAWSURFACE7 pSurface );
	void	(OnlineQuestInterface::*pfAction)	( void );


public:
	void	Action				( void );
	void	Draw				( LPDIRECTDRAWSURFACE7 pSurface );

	void	OnButtonPush				( void );
	BOOL	IsAction					( void )	{ return m_bAction; }

	SI16	GetCompletedQuestCount		( void );
	SI16	GetPlayingQuestCount		( void );
	SPlayerQuest*	const GetQuestData	( void )	{ return &m_QuestData; };
	void	SetQuestList		( void );	// robypark 2004/9/17 15:6 


//----------------------------------------------------------------------------
//	서버에서 패킷 수신 시 적용하는 함수
//----------------------------------------------------------------------------
public:
	BOOL	RecvMsg_SetQuestData		( SI16 PlayingQuestCount, SI16 CompletedQuestCount, SPlayingQuest* lpPlayingQuestData, SI16* lpCompletedQuestData );
	BOOL	RecvMsg_SetPlayingQuestData	( SPlayingQuest* lpPlayingQuestCodeData );
	void	RecvMsg_ClearAllQuest		( void );
	BOOL	RecvMsg_QuestStarted		( SI16 QuestCode );
	BOOL	RecvMsg_QuestFinished		( SI16 QuestCode );
	BOOL	RecvMsg_AddCompletedQuest	( SI16 QuestCode );
};
