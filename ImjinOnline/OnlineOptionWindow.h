/*
#ifndef __ONLINE_OPTIONWINDOW__
#define __ONLINE_OPTIONWINDOW__


class OnlineOptionWindow
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// 온라인 월드 포인터
	_Button							m_BLogIn, m_BExit, m_BJoyon, m_BBack;


public:
	OnlineSetting() : m_pMyOnlineWorld(0), m_bAction(FALSE) {};
	~OnlineSetting() { Free(); };


	VOID			Init( cltOnlineWorld *pOnlineWorld );									// 초기화
	VOID			Action();																// 액션
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );									// 출력
	BOOL			IsAction()	{	return m_bAction;	};									// 현재 활동 중인가?
	VOID			SetAction( BOOL bFlag );
	VOID			Free();																	// 날림
	VOID			GameInit();																// 로그인 돌아가기 전에 게임 초기화
};

#endif
*/