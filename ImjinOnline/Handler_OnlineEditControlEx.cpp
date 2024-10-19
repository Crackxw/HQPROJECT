//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 에디트 박스 컨트롤 확장판
//	File Name		: Handler_OnlineEditControlEx.cpp
//	Birth Date		: 2004. 05. 10.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		에디트 콘트롤에 대한 조작을 전담하는 통합함수이다. 이 함수는 프로그램 내에서 하나밖에 없다는 전제를 둔다.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"


HWND					Handler_OnlineEditControlEx::m_hMainWnd							= NULL;
OnlineEditControlEx		Handler_OnlineEditControlEx::m_clOec[DF_VALUE_MAX_COUNT_OEC];
HOEC					Handler_OnlineEditControlEx::m_hFocusedOec						= NULL;
UINT					Handler_OnlineEditControlEx::m_uiUsedCount						= 0;
BOOL					Handler_OnlineEditControlEx::m_bPushedShift						= false;

Handler_OnlineEditControlEx		g_clHandle_OecEx;					// 초기화용

//-----------------------------------------------------------------------------
// Name: Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: 생성자
//-----------------------------------------------------------------------------
Handler_OnlineEditControlEx::Handler_OnlineEditControlEx()
{

}

//-----------------------------------------------------------------------------
// Name: ~Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: 소멸자
//-----------------------------------------------------------------------------
Handler_OnlineEditControlEx::~Handler_OnlineEditControlEx()
{

}

//-----------------------------------------------------------------------------
// Name: ~Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: 초기화(1회성)
//-----------------------------------------------------------------------------
void	Handler_OnlineEditControlEx::Initialize( HWND hWnd )
{
	GS_IMEBase::Initialize( hWnd );
	OnlineEditControlEx::SetHwnd( hWnd );
	m_hMainWnd	= hWnd;
}

//-----------------------------------------------------------------------------
// Name: CreateControl()
// Code: actdoll (2004-05-10)
// Desc: 컨트롤을 생성해준다. 이것으로 생성을 시켜야 컨트롤을 제대로 동작시킬 수 있다.
//		dwOecType			- 만들려는 컨트롤의 속성( DF_FLAG_OEC_TYPE_ ... )
//		prcDraw				- 해당 컨트롤이 그려지는 범위
//		iMaxInputLen		- 최대 입력 글자수(널문자값 포함)
//		i64MaxInputValue	- 수치입력 속성(DF_FLAG_OEC_TYPE_ONLYNUMERIC)에서의 최대값
//		return				- 생성된 컨트롤의 핸들, 컨트롤 갯수가 한계에 도달하거나 실패할 경우 NULL
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::CreateControl( DWORD dwOecType, LPRECT prcDraw, int iMaxInputLen, LONGLONG i64MaxInputValue )
{
	if( m_uiUsedCount >= DF_VALUE_MAX_COUNT_OEC )	return NULL;		// 카운트가 꽉차면 못올라간다.
	if( !prcDraw )									return NULL;		// 정보가 다 안오면 못올라간다.

	HOEC	hOec = NULL;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		// 비어있는 핸들을 찾았다면 넣어라
		if( !m_clOec[i].GetHandle() )
		{
			hOec = (HOEC)&m_clOec[i];
			m_clOec[i].SetHandle		( hOec );
			m_clOec[i].SetTypeOn		( dwOecType );
			m_clOec[i].SetMaxStringSize	( iMaxInputLen );
			m_clOec[i].SetMaxValue		( i64MaxInputValue );
			m_clOec[i].SetRectOfDraw	( *prcDraw );
			m_clOec[i].SetShow			();
			m_clOec[i].SetColor			( RGB( 250, 250, 250 ) );
			m_clOec[i].SetCursorShow	( false );
//			m_clOec[i].KillFocus		();
			m_uiUsedCount++;
			break;
		}
	}

	return hOec;
}

//-----------------------------------------------------------------------------
// Name: KillControl()
// Code: actdoll (2004-05-10)
// Desc: 컨트롤을 삭제한다.
//		hOec	- 지우고자 하는 컨트롤의 핸들
//		return	- 정상적으로 삭제했으면 true, 문제가 있다면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::KillControl( HOEC &hOec )
{
	if( !hOec )						return FALSE;
	if( m_uiUsedCount <= 0 )		return FALSE;
	if( !IsCurrentHOEC( hOec ) )	return FALSE;

#ifdef _DEBUG
	BOOL	bFind	= false;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( m_clOec[i].GetHandle() == hOec )
		{
			bFind	= true;
			break;
		}
	}
	if( !bFind )				
	{
		MessageBox( NULL, "지정되지 않은 핸들입니다.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
#endif//_DEBUG
	
	OnlineEditControlEx	*pOecEx	= (OnlineEditControlEx*)hOec;
	// 만약 포커스가 존재하는 핸들일 경우 이전 핸들과 다음 핸들에서 이 핸들로 차후 포커스가 존재한다면 지워준다.
	if( m_hFocusedOec == hOec )
	{
		ClearCurrentFocus();
	}
	
	pOecEx->Clear				( true );
	pOecEx->ClearExceptionChar	();
	pOecEx->SetTypeOn			( 0 );
	pOecEx->SetMaxStringSize	( 0 );
	pOecEx->SetMaxValue			( 0 );
	pOecEx->SetRectOfDraw		( 0, 0, 0, 0 );
	pOecEx->SetNextHandle		( NULL );
	pOecEx->SetHandle			( NULL );
	m_uiUsedCount--;

	hOec	= NULL;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetNextHandle()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤에서 탭을 눌렀을 경우 다음 포커스가 맞춰질 컨트롤을 세팅한다.
//		hCurrentOec	- 기준 컨트롤 핸들
//		hNextOec	- 기준 컨트롤의 다음에 맞춰질 컨트롤
//		return	- 정상적으로 세팅했으면 true, 문제가 있다면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::SetNextHandle( HOEC hCurrentOec, HOEC hNextOec )
{
	if( !hCurrentOec )														hCurrentOec = m_hFocusedOec;
	if( !hCurrentOec || !hNextOec )										return FALSE;
	if( !IsCurrentHOEC( hCurrentOec ) || !IsCurrentHOEC( hNextOec ) )	return FALSE;

	OnlineEditControlEx	*pOecEx;
	
	pOecEx	= (OnlineEditControlEx*)hCurrentOec;
	pOecEx->SetNextHandle( hNextOec );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetNextHandle()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤에 포커스를 맞춰준다.
//		hOec	- 포커스를 맞추려는 컨트롤 핸들
//		return	- 정상적으로 세팅했으면 true, 문제가 있다면 false
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::SetFocus( HOEC hOec )
{
	if( m_uiUsedCount <= 0 )		return NULL;
	if( hOec == m_hFocusedOec )		return hOec;
	if( !IsCurrentHOEC( hOec ) )	return NULL;
	
	HOEC	hPrevOec	= m_hFocusedOec;
	m_hFocusedOec		= hOec;

	OnlineEditControlEx*	pOecEx;
	pOecEx	= (OnlineEditControlEx*)hPrevOec;
	if( pOecEx )			pOecEx->KillFocus();

	pOecEx	= (OnlineEditControlEx*)hOec;
	if( pOecEx )			pOecEx->SetFocus();

	// actdoll (2005/01/31 15:54) : 중국/대만 출장
	// 포커스 존재시에만 IME 집어넣고 빼기
	GS_IMEBase::SetImeDeviceOn( true );

	return hPrevOec;
}

//-----------------------------------------------------------------------------
// Name: ClearCurrentFocus()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤의 포커스를 죽인다.
//		return	- 마지막으로 맞춰져 있던 포커스 핸들. 없었다면 NULL리턴
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::ClearCurrentFocus()
{
	if( m_uiUsedCount <= 0 )	return NULL;
	if( !m_hFocusedOec )		return NULL;

	
	HOEC					hOec		= m_hFocusedOec;
	OnlineEditControlEx*	pOecEx		= (OnlineEditControlEx*)m_hFocusedOec;
	pOecEx->KillFocus();

	m_hFocusedOec	= NULL;

	// actdoll (2005/01/31 15:54) : 중국/대만 출장
	// 포커스 존재시에만 IME 집어넣고 빼기
	GS_IMEBase::SetImeDeviceOn( false );

	return hOec;
}

//-----------------------------------------------------------------------------
// Name: SetActive()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤을 활성화시킨다. 즉, 문자열 입력을 받을 수 있게 된다.
//		hOec	- 대상 컨트롤 핸들
//		bActive	- 살릴려면 true, 죽일려면 false
//		return	- 제대로 수행했을 경우 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::SetActive( HOEC hOec, BOOL bActive )
{
	if( !hOec )					return FALSE;
	if( m_uiUsedCount <= 0 )	return FALSE;
	
#ifdef _DEBUG
	BOOL	bFind	= false;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( m_clOec[i].GetHandle() == hOec )
		{
			bFind	= true;
			break;
		}
	}
	if( !bFind )				
	{
		MessageBox( NULL, "지정되지 않은 핸들입니다.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
#endif//_DEBUG

	OnlineEditControlEx		*pOecEx = (OnlineEditControlEx*)hOec;
	pOecEx->SetActive( bActive );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: IsActive()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤의 활성화 여부를 확인한다.
//		hOec	- 대상 컨트롤 핸들
//		return	- 활성화된 경우 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::IsActive( HOEC hOec )
{
	if( !hOec )					return FALSE;
	if( m_uiUsedCount <= 0 )	return FALSE;
	
#ifdef _DEBUG
	BOOL	bFind	= false;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( m_clOec[i].GetHandle() == hOec )
		{
			bFind	= true;
			break;
		}
	}
	if( !bFind )				
	{
		MessageBox( NULL, "지정되지 않은 핸들입니다.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
#endif//_DEBUG

	OnlineEditControlEx		*pOecEx = (OnlineEditControlEx*)hOec;
	return pOecEx->IsActive();
}

//-----------------------------------------------------------------------------
// Name: GetControl()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤의 포인터를 받는다.
//		hOec	- 대상 컨트롤 핸들
//		return	- 정상 핸들일 경우 해당 컨트롤 포인터 리턴, 없다면 NULL
//-----------------------------------------------------------------------------
OnlineEditControlEx*	Handler_OnlineEditControlEx::GetControl( HOEC hOec )
{
	if( !hOec )					return NULL;
	if( m_uiUsedCount <= 0 )	return NULL;

	OnlineEditControlEx* pOec	= NULL;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( m_clOec[i].GetHandle() == hOec )
		{
			pOec	= (OnlineEditControlEx*)hOec;
		}
	}

	return pOec;
}

//-----------------------------------------------------------------------------
// Name: GetHOec()
// Code: actdoll (2004-05-10)
// Desc: 해당 컨트롤의 포인터로 핸들을 찾는다.
//		pOec	- 대상 컨트롤 포인터
//		return	- 정상 컨트롤일 경우 해당 컨트롤 핸들 리턴, 없다면 NULL
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::GetHOec( OnlineEditControlEx* pOec )
{
	if( !pOec )					return NULL;
	if( m_uiUsedCount <= 0 )	return NULL;

	HOEC	hOec	= NULL;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( m_clOec[i].GetHandle() == hOec )
		{
			hOec	= (HOEC)pOec;
		}
	}

	return (HOEC)hOec;
}

//-----------------------------------------------------------------------------
// Name: Recv_KeyMsg()
// Code: actdoll (2004-05-10)
// Desc: 메인 프로시저 최상단에 장착 - 키 입력을 받는다.
//		hWnd, msg, wParam, lParam	- 프로시저 파라메터
//		return	- 메시지가 처리되었을 경우 true, 처리되지 않았다면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::Recv_KeyMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !hWnd )					return FALSE;
	if( m_uiUsedCount <= 0 )	return FALSE;
//	if( !m_hFocusedOec )		return FALSE;

	// 만약 포커스 먹은 컨트롤이 하나도 없을 때 다국어 버젼키와 관련된 메시지가 리턴된다면 무조건 영문으로 세팅
	OnlineEditControlEx	*pOecEx = GetControl( m_hFocusedOec );
	if( !pOecEx )
	{
		switch( msg )
		{
			case WM_IME_NOTIFY:
				switch( wParam )
				{
					// 입력키 변환이 들어왔을 경우만 체크한다.
					// 다른 경우는 모두 씹는다.
					case IMN_SETCONVERSIONMODE		:	
					case IMN_SETOPENSTATUS			:
						GS_IMEBase::Check_WM_IME_NOTIFY( wParam, lParam );
						break;
				}
				return TRUE;

			case WM_INPUTLANGCHANGE:
				{
					// 언어 변환이 들어왔을 경우 체크한다.
					GS_IMEBase::Check_WM_INPUTLANGCHANGE( lParam );
				}
				return TRUE;

//			case WM_IME_STARTCOMPOSITION:
//			case WM_IME_ENDCOMPOSITION:
//			case WM_IME_COMPOSITION:
//				{
//					// 이 경우는 모두 조합 취소로 날린다.
//					GS_IMEBase::SetCompStatusCancel();
//				}
//				return TRUE;
		}

		return FALSE;
	}

	// 복수 라인이 아닐 경우만 탭 명령이 동작한다.
	if( !pOecEx->IsTypeMultiLine() )
	{
		switch( msg )
		{
			case WM_KEYDOWN:
			{
				switch( wParam )
				{
					case VK_TAB:
					{
						if( pOecEx->GetNextHandle() )	
						{
							SetFocus( pOecEx->GetNextHandle() );
							return TRUE;
						}
					}
					break;

					case VK_ESCAPE:
					{
						pOecEx->Clear();
						return TRUE;
					}
					break;
				}
			}
			break;
		}
	}

	return ( (OnlineEditControlEx*)m_hFocusedOec )->Proc_WindowMsg( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: IsCurrentHOEC()
// Code: actdoll (2004-05-10)
// Desc: 현재의 핸들이 정상적인 핸들인지를 체크한다.
//		hOec	- 대상 컨트롤 핸들
//		return	- 맞다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::IsCurrentHOEC( HOEC hOec )
{
	if( !hOec )							return FALSE;
	if( hOec == m_hFocusedOec )			return TRUE;

	for( register int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		if( hOec == m_clOec[i].GetHandle() )
		{
			return TRUE;
		}
	}

	return FALSE;
}