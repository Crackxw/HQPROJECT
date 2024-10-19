/************************************************************************************************
	파일명 : OnlineNotice.h
	담당자 : 이상원

	내  용 : 공지사항
************************************************************************************************/
#ifndef __ON_NOTICE__H
#define __ON_NOTICE__H


#include "OnlineChatList.h"


class OnlineNotice
{
private:
	cltOnlineWorld					*m_pMyOnlineWorld;										// 온라인 월드 포인터
	OnlineChatList					*pNoticeList;
	_Button							m_BClose;

	BOOL							m_bAction;												// 현재 활동중인가 알아 보는 변수
	XSPR							m_BackImageSpr, m_Button2;
	SI16							m_siX, m_siY;											// 기준 좌표
	BOOL							m_bMouseDown;											// 마우스 상태
	BOOL							m_bClose;

public:
	OnlineNotice() : m_pMyOnlineWorld(0), m_bAction(FALSE), m_bClose(FALSE) {};
	~OnlineNotice() { Free(); };


	VOID			Init( cltOnlineWorld *pOnlineWorld );									// 초기화
	VOID			Action();																// 액션
	VOID			Draw( LPDIRECTDRAWSURFACE7 pSurface );									// 출력
	BOOL			IsAction()	{	return m_bAction;	};									// 현재 활동 중인가?
	VOID			SetAction( const char* pNotice, BOOL bFlag );
	VOID			Free();																	// 날림
	BOOL			IsClose()	{	return m_bClose;	};	
};

#endif