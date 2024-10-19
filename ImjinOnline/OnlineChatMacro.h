//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 콘솔 >> 반복 말하기 설정창
//	File Name		: OnlineChatMacro.h
//	Birth Date		: 2003. 01. 13.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 사용자의 콘솔에서 반복 말하기 기능을 설정하는 창을 출력하는 클래스입니다.
//
//===================================================================================================

#ifndef __ONLINE_CHATMACRO_H__
#define __ONLINE_CHATMACRO_H__

#pragma once

//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: 포함 헤더 선언 및 외부 클래스 정의
//-----------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
// Name: OnlineChatMacro
// Code: actdoll (2003-12-23)
// Desc: 반복 말하기창 클래스
//-----------------------------------------------------------------------------
class OnlineChatMacro
{
private:
	enum { OK, CANCEL, MAXCOUNT };
	enum { MAX_EDIT_BOX = 5 };

private:
	cltOnlineWorld				*m_pMyOnlineWorld;					// 게임 몸통 클래스. 외부에서 받아옴

	// actdoll (2004/05/07 12:28) : [OECEX]
//	vector<OnlineEditControl*>	m_EditBox_Slot;						// 에디트 박스 UI - 매크로에서 사용할 텍스트를 입력하는 각각의 에딧 박스
//	vector<RECT>				m_Size_EditBox;						// 에디트 박스 UI - 각 에디트 박스별 구획 선언
	vector<HOEC>				m_hOecSlot;							// 에디트 박스 UI - 매크로에서 사용할 텍스트를 입력하는 각각의 에딧 박스
	_Button						m_Btn_Common[MAXCOUNT];				// 버튼 UI - 확인, 취소

	XSPR						m_Spr_Frm;							// 스프라이트들 - 프레임
	XSPR						m_Spr_Btn_ScrollBar;				// 스프라이트들 - 속도 지정 스크롤 바의 핸들

	RECT						m_Size_ScrollBar;					// 속도 지정 스크롤 바의 범위

	char						m_pszString[MAX_EDIT_BOX][512];		// 에디트 박스별로 들어가는 매크로 글

	SI32						m_siNoFieldHandle;					// 필드 지정 불가를 선언할 때의 핸들
	SI32						m_siDrawIndex;						// 매크로 동작 시 출력할 채팅 메시지의 인덱스
	SI32						m_siDrawTimeGap;					// 채팅 메시지 출력 간격(ms)

	SI32						m_siScrollBarXPos;					// 스크롤 바의 현재 X 위치값

	DWORD						m_dwLastUpdateTime;					// 마지막으로 받은 출력 시간

	BOOL						m_bIsRunning;						// 매크로를 동작 시켰느냐 아니냐

	BOOL						m_bMousePush;
	BOOL						m_bIsActive;

public:
	OnlineChatMacro			( cltOnlineWorld *p ) : m_pMyOnlineWorld(p)	{ Init(); }
	~OnlineChatMacro		( void )									{ Free(); }

	void	Initialize		( void );
	void	Draw			( LPDIRECTDRAWSURFACE7 lpSurface );
	void	Action			( void );

	void	OnButtonPush	( void );	
	void	OnRunningToggle	( void );
	void	End				( void )									{ m_bIsRunning = FALSE; }

	BOOL	IsRunning		( void )									{ return m_bIsRunning; }
	BOOL	IsActive		( void )									{ return m_bIsActive; }

private:	
	void	Init			( void );
	void	Free			( void );
	
	void	SetData_Settings( void );
	void	SaveString		( void );


	void	Draw_Settings	( LPDIRECTDRAWSURFACE7 lpSurface );
	void	Action_Settings	( void );

	void	GetSpeedValue	( SI32 &siTimeGap, SI32 &siScrollBarXPos );

};
#endif//__ONLINE_CHATMACRO_H__