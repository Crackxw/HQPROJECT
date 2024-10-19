//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 에디트 박스 컨트롤 확장판
//	File Name		: Handler_OnlineEditControlEx.h
//	Birth Date		: 2004. 05. 03.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		에디트 콘트롤에 대한 조작을 전담하는 통합함수이다. 이 함수는 프로그램 내에서 하나밖에 없다는 전제를 둔다.
//
//===================================================================================================

#ifndef	__HANDLER_ONLINE_EDITCONTROLEX_H__
#define	__HANDLER_ONLINE_EDITCONTROLEX_H__

#define DF_VALUE_MAX_COUNT_OEC		50			// 이 핸들러에서 한번에 관장할 에디트 박스의 최대 갯수

//-----------------------------------------------------------------------------
// Name: Handler_OnlineEditControlEx
// Code: actdoll (2004-04-17)
// Desc: 에디트 컨트롤 박스를 조작할 핸들러
//-----------------------------------------------------------------------------
class Handler_OnlineEditControlEx
{
private:
	static	HWND					m_hMainWnd;									// 이 핸들러에서 사용할 메인 윈도 핸들
	static	OnlineEditControlEx		m_clOec[DF_VALUE_MAX_COUNT_OEC];			// 각각의 에디트 콘트롤들
	static	HOEC					m_hFocusedOec;								// 현재 포커스가 맞춰진 컨트롤
	
	
	static	UINT					m_uiUsedCount;								// (내부플래그)에디트 콘트롤이 사용된 횟수
	static	BOOL					m_bPushedShift;								// 쉬프트키가 눌린상태다.


public:
	Handler_OnlineEditControlEx					( void );
	~Handler_OnlineEditControlEx				( void );

	static	void	Initialize					( HWND hWnd );
	static	HOEC	CreateControl				( DWORD dwOecType, LPRECT prcDraw, int iMaxInputLen, LONGLONG i64MaxInputValue = 0L );
	static	BOOL	KillControl					( HOEC &hOec );
	static	BOOL	SetNextHandle				( HOEC hCurrentOec, HOEC hNextOec );
	
	static	HOEC	SetFocus					( HOEC hOec );
	static	HOEC	ClearCurrentFocus			( void );
	static	HOEC	GetFocus					( void )				{ return m_hFocusedOec; }
	static	BOOL	IsFocus						( HOEC hOec )			{ return ( hOec && m_hFocusedOec && m_hFocusedOec == hOec ); }

	static	BOOL	SetActive					( HOEC hOec, BOOL bActive = true );
	static	BOOL	IsActive					( HOEC hOec );

	static	OnlineEditControlEx*	GetControl	( HOEC hOec );
	static	HOEC	GetHOec						( OnlineEditControlEx* pOec );

	
	static	BOOL	Recv_KeyMsg					( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	static	UINT	GetCodePage					( void )				{ OnlineEditControlEx::GetCodePage(); }
	static	void	SetCandidateFont			( HFONT hFont )			{ OnlineEditControlEx::SetCandidateFont( hFont ); }	// 초반부 첫 세팅 - 숙어창 폰트
	static	BOOL	IsCurrentHOEC				( HOEC hOec );
};


#endif//__HANDLER_ONLINE_EDITCONTROLEX_H__