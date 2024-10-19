/************************************************************************************************
	파일명 : OnlineSetting.h
	담당자 : 이 준 석
	날짜   : 2002.07.25
	내  용 : 환경설정
************************************************************************************************/
#ifndef __ON_SETTING__H
#define __ON_SETTING__H


class OnlineSetting
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// 온라인 월드 포인터
	_Button							m_BLogIn, m_BExit, m_BCompany, m_BShopping, m_BBack, m_BOption;

	BOOL							m_bAction;												// 현재 활동중인가 알아 보는 변수
	XSPR							m_BackImageSpr, m_ButtonSpr;
	SI16							m_siX, m_siY, m_siNoField;								// 기준 좌표
	BOOL							m_bMouseDown;											// 마우스 상태

	BOOL							m_OptionMode;
	SI16							m_OptionNoField;

	BOOL							m_bExistShop;	// actdoll (2004/05/26 17:58) : 쇼핑몰이 있을 경우 이것은 true가 된다..

private:
	void			LoadImage();
	void			FreeImage();

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