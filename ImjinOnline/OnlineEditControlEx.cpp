//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 에디트 박스 컨트롤 확장판
//	File Name		: OnlineEditControlEx.cpp
//	Birth Date		: 2004. 04. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 내 에디트 박스에 대한 콘트롤을 관장하는 클래스이다.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: 속성 라인 플래그
//-----------------------------------------------------------------------------
typedef enum EN_FLAG_OEC_ATTRLINE
{
	EN_FLAG_OEC_ATTRLINE_DOT,		// 점선		- 입력 중
	EN_FLAG_OEC_ATTRLINE_THICK,		// 굵은선	- 변환 중
	EN_FLAG_OEC_ATTRLINE_THIN,		// 얇은선	- 변환 대기 중
	EN_FLAG_OEC_ATTRLINE_MAX
};

HWND	OnlineEditControlEx::m_hMainWnd							= NULL;
BOOL	OnlineEditControlEx::m_bIsCompTypeMultiChar				= false;
HPEN	OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_MAX]	= {0,};
HFONT	OnlineEditControlEx::m_hCandidateFont					= NULL;

//-----------------------------------------------------------------------------
// Name: OnlineEditControlEx()
// Code: actdoll (2004-04-17)
// Desc: 생성자
//-----------------------------------------------------------------------------
OnlineEditControlEx::OnlineEditControlEx()
{
	m_hOwnOec				= NULL;
	m_hPrevOec				= NULL;
	m_hNextOec				= NULL;
	m_bActive				= false;
	m_bShow					= false;
	m_bHideCursor			= false;
	m_bCanRecvInputChar		= false;
	m_bPushedEnter			= false;
	m_bPushedTab			= false;

	GS_IMEBase::Clear();

	ZeroMemory( m_btExceptionCharacter,	sizeof(m_btExceptionCharacter) );

	ZeroMemory( m_pszShowBuffer,	sizeof(m_pszShowBuffer) );
	ZeroMemory( m_pszString,		sizeof(m_pszString) );
	ZeroMemory( m_pszBackUpString,	sizeof(m_pszBackUpString) );
	m_iBackUpLineNo	= 0;
	
	m_iLenLimit				= 0;
	m_iLenShowBuffer		= 0;
	m_iLenString			= 0;
	m_iCountChar			= 0;
	m_i64ValueLimit			= 0;
	
	m_iCaretAsAnsi			= 0;
	m_iCaretAsChar			= 0;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	SetRectEmpty( &m_rcDestOfDraw );
	m_iFirstCharOfDraw		= 0;
	m_iLastCharOfDraw		= 0;
	m_hFont					= NULL;
	m_bDrawShadow			= false;
	m_dwColor				= 0;
	m_dwShadowColor			= 0;

	m_dwType				= 0;

	m_iCount_NoProcessAsAscii	= 0;
	m_bFlag_IMEOn				= false;
	m_dwFlag_InputStatus		= 0;

	ZeroMemory( OnlineEditControlEx::m_hPen, sizeof(OnlineEditControlEx::m_hPen) );
}

//-----------------------------------------------------------------------------
// Name: ~OnlineEditControlEx()
// Code: actdoll (2004-04-17)
// Desc: 소멸자
//-----------------------------------------------------------------------------
OnlineEditControlEx::~OnlineEditControlEx()
{
	m_hOwnOec				= NULL;
	m_hPrevOec				= NULL;
	m_hNextOec				= NULL;
	m_bActive				= false;
	m_bShow					= false;
	m_bHideCursor			= false;
	m_bCanRecvInputChar		= false;
	m_bPushedEnter			= false;
	m_bPushedTab			= false;

	GS_IMEBase::Clear();

	ZeroMemory( m_btExceptionCharacter,	sizeof(m_btExceptionCharacter) );

	ZeroMemory( m_pszShowBuffer,	sizeof(m_pszShowBuffer) );
	ZeroMemory( m_pszString,		sizeof(m_pszString) );
	ZeroMemory( m_pszBackUpString,	sizeof(m_pszBackUpString) );
	m_iBackUpLineNo	= 0;
	
	m_iLenLimit				= 0;
	m_iLenShowBuffer		= 0;
	m_iLenString			= 0;
	m_iCountChar			= 0;
	m_i64ValueLimit			= 0;
	
	m_iCaretAsAnsi			= 0;
	m_iCaretAsChar			= 0;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	SetRectEmpty( &m_rcDestOfDraw );
	m_iFirstCharOfDraw		= 0;
	m_iLastCharOfDraw		= 0;
	m_hFont					= NULL;
	m_bDrawShadow			= false;
	m_dwColor				= 0;
	m_dwShadowColor			= 0;

	m_dwType				= 0;

	m_iCount_NoProcessAsAscii	= 0;
	m_bFlag_IMEOn				= false;
	m_dwFlag_InputStatus		= 0;

	if( OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] )
	{
		for( int i=0; i<EN_FLAG_OEC_ATTRLINE_MAX; i++ )
		{
			DeleteObject( OnlineEditControlEx::m_hPen[i] );
			OnlineEditControlEx::m_hPen[i]	= NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: Clear()
// Code: actdoll (2004-04-17)
// Desc: 현재 설정되어 있는 입력기에 대한 줄의 정보를 초기화한다.
//		bClearBackUps	- 백업줄 전체를 없애려면 true, 주 라인 버퍼만 지울라면 false (기본값 false)
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::Clear( BOOL bClearBackUps )
{
	GS_IMEBase::Clear();

	ZeroMemory( m_pszShowAttr, sizeof(m_pszShowAttr) );
	ZeroMemory( m_pszShowBuffer, sizeof(m_pszShowBuffer) );
	ZeroMemory( m_pszString, sizeof(m_pszString) );
	if( bClearBackUps )	
	{
		ZeroMemory( m_pszBackUpString,	sizeof(m_pszBackUpString) );
		m_iBackUpLineNo	= 0;
	}
	
	m_iLenShowBuffer		= 0;
	m_iLenString			= 0;
	m_iCountCharShowBuffer	= 0;
	m_iCountChar			= 0;
	
	m_iCaretAsAnsi			= 0;
	m_iCaretAsChar			= 0;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	m_iFirstCharOfDraw		= 0;
	m_iLastCharOfDraw		= 0;

	m_iCount_NoProcessAsAscii	= 0;

	m_bPushedEnter				= false;
	m_bPushedTab				= false;

	// 만약 이 클래스가 수치형이라면 0을 대신 넣어준다.
	if( IsTypeOnlyNumeric() )	SetValue( 0 );
	else						UpdateShowBuffer();
}

//-----------------------------------------------------------------------------
// Name: SetFocus()
// Code: actdoll (2004-04-17)
// Desc: 포커스가 내게 왔을 경우의 처리를 담당한다.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetFocus()
{
	// IME 시스템을 클리어시킨다.
	GS_IMEBase::Clear();

	// 패스워드/숫자입력/1바이트 문자 입력 상태 중 하나라면
	if( IsTypePassword() || IsTypeOnlyNumeric() || IsTypeOnlyAscii() )	GS_IMEBase::SetImeDeviceOn( false );
	// 그렇지 않다면 입력기 동작 상태로 세팅한다.
	else																GS_IMEBase::SetImeDeviceOn( true );

	SetRecvInputChar( true );
	SetCursorShow	( true );
	SetActive		( true );
}

//-----------------------------------------------------------------------------
// Name: SetFocus()
// Code: actdoll (2004-04-17)
// Desc: 포커스가 내게서 벗어났을 경우의 처리를 담당한다.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::KillFocus()
{
	// 조합중의 상황일 경우 일단 문장을 완결시킨다.
	GS_IMEBase::SetCompStatusDoComplete();
	// IME 시스템을 클리어시킨다.
	GS_IMEBase::Clear();
	// IME의 입력기 동작을 꺼 놓는다.
	GS_IMEBase::SetImeDeviceOn( false );

	SetActive		( false );
	SetRecvInputChar( false );
	SetCursorShow	( false );
}

//-----------------------------------------------------------------------------
// Name: IsPushEnter()
// Code: actdoll (2004-04-17)
// Desc: 데이터 설정
//	방금 전에 엔터를 눌렀는지 안눌렀는지를 체크한다. 이 함수를 호출한 뒤
//	엔터와 관련된 신호는 false로 돌아간다.
//		return		- 엔터를 눌렀다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::IsPushEnter()
{
	if( IsTypeMultiLine() )			return FALSE;
	if( !m_bPushedEnter )			return FALSE;
	m_bPushedEnter	= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: IsPushEnter()
// Code: actdoll (2004-04-17)
// Desc: 데이터 설정
//	방금 전에 탭키를 눌렀는지 안눌렀는지를 체크한다. 이 함수를 호출한 뒤
//	탭과 관련된 신호는 false로 돌아간다.
//		return		- 탭을 눌렀다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::IsPushTab()
{
	if( IsTypeMultiLine() )			return FALSE;
	if( !m_bPushedTab )				return FALSE;

	m_bPushedTab	= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetCaretPosAsAnsi()
// Code: actdoll (2004-04-17)
// Desc: 커서의 위치를 잡아주는 함수이다. 안시 스트링 형태를 기준으로 한다.
//		iPos	- 커서의 스트링 포지션
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetCaretPosAsAnsi( int iPos )
{
	if( iPos < 0 )					iPos	= 0;
	else if( iPos >= m_iLenString )	iPos	= m_iLenString;

	m_iCaretAsAnsi			= iPos;
	m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;
}

//-----------------------------------------------------------------------------
// Name: SetCaretPosAsChar()
// Code: actdoll (2004-04-17)
// Desc: 문자 기준으로의 위치를 잡아주는 함수이다. 문자 형태를 기준으로 한다.
//		iPos	- 커서의 문자 기준 포지션
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetCaretPosAsChar( int iPos )
{
	if( iPos < 0 )						iPos	= 0;
	else if( iPos >= m_iCountChar )		iPos	= m_iCountChar;

	m_iCaretAsChar			= iPos;
	m_iCaretAsAnsi			= OnlineEditControlEx::GetStrPtrByCharPos( m_pszString, NULL, m_iCaretAsChar ) - m_pszString;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;
}

//-----------------------------------------------------------------------------
// Name: SetTypeOn()
// Code: actdoll (2004-04-17)
// Desc: 플래그 설정
//	현재의 에디트 박스에 대한 속성을 설정하는 함수다.
//		dwFlag	- DF_FLAG_OEC_TYPE_...을 복합한 플래그
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetTypeOn( DWORD dwFlag )
{
	if( dwFlag & DF_FLAG_OEC_TYPE_MULTILINE )													dwFlag = DF_FLAG_OEC_TYPE_MULTILINE;	// 이 플래그값이 들어있다면 지금 들어온 플래그들은 다 지운다.
	if( dwFlag & DF_FLAG_OEC_TYPE_PASSWORD )													dwFlag |= DF_FLAG_OEC_TYPE_ONLYASCII;	// 이 플래그값이 들어있다면 아스키값을 추가한다.
	if( ( dwFlag & DF_FLAG_OEC_TYPE_ONLYNUMERIC ) && ( dwFlag & DF_FLAG_OEC_TYPE_ONLYASCII ) )	dwFlag &= ~DF_FLAG_OEC_TYPE_ONLYASCII;	// 두 플래그가 들어있다면 아스키값을 꺼 버린다.

	m_dwType = dwFlag;
	if( dwFlag & DF_FLAG_OEC_TYPE_ONLYNUMERIC )		
	{
		SetValue( 0 );
		SetCursorShow( false );
	}
}

//-----------------------------------------------------------------------------
// Name: SetString()
// Code: actdoll (2004-04-17)
// Desc: 데이터 설정
//	내부의 텍스트를 수정한다. 만약 이것이 진행될 경우 눈에 보이는 스트링까지 고쳐지게 된다.
//	주의할 것은 현재 세팅된 플래그가 전혀 동작하지 않을 수 있으므로 신중을 가해서 해야 한다.
//	수치 입력 전용에서는 이 함수는 동작하지 않는다.
//		pszString	- 변경할 스트링 값
//		return		- 제대로 박았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::SetString( char *pszString )
{
	if( IsTypeOnlyNumeric() )			return FALSE;
	if( !pszString )					return FALSE;
	
	// 정보 복사
	int		iLen	= strlen( pszString );
	if( iLen > m_iLenLimit )			iLen = m_iLenLimit;
	ZeroMemory( m_pszString, sizeof(m_pszString) );
	strncpy( m_pszString, pszString, iLen );

	// 기타 정보 적재
	m_iLenString			= iLen;																				// 실제 길이 받고
	m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// 문자 갯수 받고
	m_iCaretAsAnsi			= m_iLenString;																		// 커서는 맨 끝으로 맞추고
	m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// 문자열 기준의 커서 보정
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// 출력 버퍼 조정
	UpdateShowBuffer();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetValue()
// Code: actdoll (2004-04-17)
// Desc: 데이터 설정
//	내부의 텍스트를 주어진 수치로 수정한다. 만약 이것이 진행될 경우 눈에 보이는 스트링까지 고쳐지게 된다.
//	주의할 것은 현재 세팅된 플래그가 전혀 동작하지 않을 수 있으므로 신중을 가해서 해야 한다.
//	모든 설정에서 사용할 수 있다.
//		i64Value	- 입력할 수치값
//		return		- 제대로 박았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::SetValue( LONGLONG i64Value )
{
	if( i64Value < 0 )						i64Value = 0;
	else if( i64Value > m_i64ValueLimit )	i64Value = m_i64ValueLimit;

	// 정보 복사
	ZeroMemory( m_pszString, sizeof(m_pszString) );
	_i64toa( i64Value, m_pszString, 10 );
	int	iLen	= strlen( m_pszString );
	if( iLen > m_iLenLimit )	iLen = m_iLenLimit;
		
	strncpy( m_pszString, m_pszString, iLen );

	// 기타 정보 적재
	m_iLenString			= iLen;																				// 실제 길이 받고
	m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// 문자 갯수 받고
	m_iCaretAsAnsi			= m_iLenString;																		// 커서는 맨 끝으로 맞추고
	m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// 문자열 기준의 커서 보정
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// 출력 버퍼 조정
	UpdateShowBuffer();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetValue()
// Code: actdoll (2004-04-17)
// Desc: 데이터 얻기
//	내부 텍스트를 수치로 전환하여 값을 전해준다.
//	수치 입력 전용 설정에서만 쓸 수 있다.
//		return	- int64형 수치
//-----------------------------------------------------------------------------
LONGLONG	OnlineEditControlEx::GetValue()
{
	if( !IsTypeOnlyNumeric() || m_iLenString <= 0 )		return 0L;

	return _atoi64( m_pszString );
}

//-----------------------------------------------------------------------------
// Name: ChangeCaretNext()
// Code: actdoll (2004-04-17)
// Desc: 멀티라인에 존재하는 커서를 기준으로 스트링의 앞줄이나 뒷줄로 이동한다.
//		bSetNext		- 현재의 다음줄로 넘길 것이라면 true, 아니라면 false
//		bSetFirstAndLast- 현재의 줄의 끄트머리를 왔다갔다할 것이라면 true, 아니라면 false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::ChangeCaretNext( BOOL bSetNext, BOOL bSetFirstAndLast )
{
	if( !IsTypeMultiLine() )			return;

	// 일단 글씨 크기를 검사하기 위해 HDC를 호출한다.
	HDC		hDC		= GetDC( m_hMainWnd );
	if( !hDC )		return;

	if( m_hFont )	::SelectObject( hDC, m_hFont );

	// 현재 커서가 위치한 라인의 줄 수를 받는다.
	int		iLenPerLine, iCharPerLine;
	BOOL	bIsReturn;
	char	*pCurrent, *pPrev, *pNext;
	char	*pCaretPos		= GetStrPtrByCharPos( m_pszString, NULL, m_iCaretAsChar );

	BOOL	bBreak	= false;
	pCurrent		= pPrev	= m_pszString;
	while(1)
	{
		// 한 줄을 검색하고
		pNext	= GetSingleLineOfMultiStr( hDC, pCurrent, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
		
		// 마지막 줄을 검색하고 있다면 종료
		if( !pNext )										break;

		// 속해있는 라인을 찾았을 경우 커서가 있는 첫번째 문자를 받고 종료
		if( pCaretPos >= pCurrent && pCaretPos < pNext )	break;

		pPrev		= pCurrent;
		pCurrent	= pNext;
	}

	// 만약 그냥 이 줄의 끝과 첫문자만 왔다갔다 하는 것이라면 그렇게 맞춰준다.
	if( bSetFirstAndLast )
	{
		// 줄의 끝자리로 맞춰준다.
		if( bSetNext )
		{
			if( bIsReturn || ( !bIsReturn && pNext ) )
			{
				iLenPerLine		-= 2;	// 개행문자가 있다면 끝에 두글자를 지워준다.
				iCharPerLine	--;		// 문자는 하나를 지운다.
			}
			m_iCaretAsAnsi	= ( pCurrent - m_pszString ) + iLenPerLine;
			m_iCaretAsChar	= GetCharCountByStrPtr( m_pszString, NULL, pCurrent - m_pszString ) + iCharPerLine;
		}
		else
		{
			m_iCaretAsAnsi	= pCurrent - m_pszString;
			m_iCaretAsChar	= GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );
		}
		return;
	}

	// 커서가 속해있는 문자열에서의 앞쪽 갯수를 알아놓는다.
	int		iRangeFromFirst	= pCaretPos - pCurrent;
	
	// 다음줄로 이동하는 것이라면
	if( bSetNext )	
	{
		// 이동 가능한 범위라면
		if( pNext != NULL && pCurrent < pNext )	
		{
			GetSingleLineOfMultiStr( hDC, pNext, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
			if( iRangeFromFirst > iLenPerLine )					iRangeFromFirst = iLenPerLine;	// 글자 범위를 넘어갈 경우 맞춰주고
			if( iRangeFromFirst == iLenPerLine && bIsReturn )	iRangeFromFirst -= 2;			// 개행문자가 있다면 끝에 두글자를 지워준다.
			
			SetCaretPosAsAnsi( ( pNext - m_pszString ) + iRangeFromFirst );
		}
		// 더이상 이동이 불가능하다면 끝자리로 맞춰준다.
		else
		{
			m_iCaretAsAnsi	= m_iLenString;
			m_iCaretAsChar	= m_iCountChar;
		}
	}
	// 이전줄로 이동하는 것이라면
	else
	{
		// 줄에 차이가 있었다면
		if( pPrev < pCurrent && pPrev >= m_pszString )	
		{
			GetSingleLineOfMultiStr( hDC, pPrev, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
			if( iRangeFromFirst > iLenPerLine )					iRangeFromFirst = iLenPerLine;	// 글자 범위를 넘어갈 경우 맞춰주고
			if( iRangeFromFirst == iLenPerLine && bIsReturn )	iRangeFromFirst -= 2;			// 개행문자가 있다면 끝에 두글자를 지워준다.
			
			SetCaretPosAsAnsi( ( pPrev - m_pszString ) + iRangeFromFirst );
		}
		// 더이상 이동이 불가능하다면 앞자리로 맞춰준다.
		else
		{
			m_iCaretAsAnsi	= 0;
			m_iCaretAsChar	= 0;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: ChangeFromStringToBackUp()
// Code: actdoll (2004-04-17)
// Desc: 본문 스트링을 백업 스트링에 집어넣는다.
//		bSetNext	- 적재 후 선택줄을 다음으로 넘기고 싶다면 true, 이전으로 넘기고 싶다면 false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::ChangeFromStringToBackUp( BOOL bSetNext )
{
	if( !IsTypeBackUpString() )			return;
	if( IsTypePassword() )				return;

	strcpy( m_pszBackUpString[m_iBackUpLineNo], m_pszString );
	if( bSetNext )
	{
		m_iBackUpLineNo++;
		if( m_iBackUpLineNo >= DF_VALUE_OEC_MAX_BACKUP_LINE )		m_iBackUpLineNo = 0;
	}
	else
	{
		m_iBackUpLineNo--;
		if( m_iBackUpLineNo < 0 )									m_iBackUpLineNo = DF_VALUE_OEC_MAX_BACKUP_LINE - 1;
	}
}

//-----------------------------------------------------------------------------
// Name: ChangeAsBackUpString()
// Code: actdoll (2004-04-17)
// Desc: 백업 스트링에 있던 데이터를 본문 스트링에 집어넣는다.
//		bGetNext	- 현재 줄에서 다음 줄의 데이터를 받고 싶다면 true, 이전 줄의 데이터를 받고 싶다면 false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::ChangeFromBackUpToString( BOOL bGetNext )
{
	if( !IsTypeBackUpString() )			return;
	if( IsTypePassword() )				return;

	int		i;
	BOOL	bExist = false;
	// 다음줄로 넘기는가?
	if( bGetNext )		
	{
		( m_iBackUpLineNo >= DF_VALUE_OEC_MAX_BACKUP_LINE - 1 ) ? m_iBackUpLineNo = 0 : m_iBackUpLineNo++;	// 라인 설정
		// 설정한 라인부터 끝까지에 스트링이 들어있는지 확인한다.
		for( i=m_iBackUpLineNo; i<DF_VALUE_OEC_MAX_BACKUP_LINE; i++ )
		{
			if( strlen( m_pszBackUpString[i] ) != 0 ) 
			{
				m_iBackUpLineNo	= i;
				bExist			= true; 
				break; 
			}
		}
		// 끝까지 갔는데도 못찾았다면 앞쪽도 찾아본다.
		if( !bExist )
		{
			for( i=0; i<m_iBackUpLineNo; i++ )
			{
				if( strlen( m_pszBackUpString[i] ) != 0 )
				{
					m_iBackUpLineNo	= i;
					break;
				}
			}
		}
	}
	// 이전줄로 넘기는가?
	else
	{
		( m_iBackUpLineNo > 0 ) ? m_iBackUpLineNo-- : m_iBackUpLineNo = DF_VALUE_OEC_MAX_BACKUP_LINE - 1;	// 라인 설정
		// 설정한 라인부터 끝까지에 스트링이 들어있는지 확인한다.
		for( i=m_iBackUpLineNo; i>=0; i-- )
		{
			if( strlen( m_pszBackUpString[i] ) != 0 ) 
			{
				m_iBackUpLineNo	= i;
				bExist			= true; 
				break; 
			}
		}
		// 끝까지 갔는데도 못찾았다면 앞쪽도 찾아본다.
		if( !bExist )
		{
			for( i=DF_VALUE_OEC_MAX_BACKUP_LINE-1; i>m_iBackUpLineNo; i-- )
			{
				if( strlen( m_pszBackUpString[i] ) != 0 )
				{
					m_iBackUpLineNo	= i;
					break;
				}
			}
		}
	}
	
	// 데이터 모두 해제하고 다시 잡아준다.
	GS_IMEBase::Clear();
	ZeroMemory	( m_pszString, sizeof(m_pszString) );
	strcpy		( m_pszString, m_pszBackUpString[m_iBackUpLineNo] );
	
	// 기타 세팅들
	m_iLenString	= strlen( m_pszString );
	m_iCountChar	= OnlineEditControlEx::GetCharLen( m_pszString );
	m_iCaretAsAnsi			= 0;
	m_iCaretAsChar			= 0;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// 출력 버퍼 갱신
	UpdateShowBuffer();
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-04-17)
// Desc: 스트링을 출력한다.
//		hDC			- 이 윈도의 DC
//		prcDest		- 출력 범위를 가지고 있는 RECT의 포인터
//		bRightAlign	- true이면 오른쪽 정렬로 출력, false이면 왼쪽 정렬로 출력 (기본값 true)
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Draw( HDC hDC, BOOL bRightAlign )
{
	if( !hDC )							return FALSE;
	if( !m_bShow )						return FALSE;

	static	DWORD	dwLatestTime	= 0;
	static	BOOL	bCaretOn		= true;

	// 오브젝트에 폰트 정보를 주어준다.
	if( m_hFont )	::SelectObject( hDC, m_hFont );

	// 커서 깜빡임 연산
	if( GetTickCount() - dwLatestTime > 200 )
	{
		dwLatestTime	= GetTickCount();
		bCaretOn		= !bCaretOn;
	}
	
	// 만약 속성 펜이 없다면 만들어라
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT]	= CreatePen( PS_DOT,	1, RGB( 125, 125, 125 ) );
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK]	= CreatePen( PS_SOLID,	3, RGB( 250, 250, 250 ) );
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN]	= CreatePen( PS_SOLID,	1, RGB( 250, 250, 250 ) );

	// 글자의 높이 좀 구하고
	int		w, h, iDistHeightCaret;
	OnlineEditControlEx::GetStringPixelSize( hDC, "1", 1, w, h );
	

	// 단일 라인일 경우
	if( !IsTypeMultiLine() )
	{
		// 본문
		UpdateShowRangeForSingleLine( hDC );		// 위치 보정
		
		UINT	uiOption = DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE;
		( bRightAlign ) ? uiOption |= DT_RIGHT : uiOption |= DT_LEFT;
		
		char *pStart;
		pStart	= GetStrPtrByCharPos( m_pszShowBuffer, NULL, m_iFirstCharOfDraw );
		if( m_bDrawShadow )
		{
			RECT	rcShadow = { m_rcDestOfDraw.left + 1, m_rcDestOfDraw.top + 1, m_rcDestOfDraw.right + 1, m_rcDestOfDraw.bottom + 1 };
			SetTextColor( hDC, m_dwShadowColor );
			DrawText( hDC, pStart, ( m_pszShowBuffer + m_iLenShowBuffer ) - pStart, &rcShadow, uiOption );
		}
		SetTextColor( hDC, m_dwColor );
		DrawText( hDC, pStart, ( m_pszShowBuffer + m_iLenShowBuffer ) - pStart, &m_rcDestOfDraw, uiOption );

		// 글자가 찍히기 시작하는 높이를 구한다.
		iDistHeightCaret	= ( m_rcDestOfDraw.bottom - m_rcDestOfDraw.top - h ) >> 1;
		
		// 속성줄(간체 중국어 혹은 일본어에만 적용)
		if( m_bActive && GS_IMEBase::IsIMEComposing() && !GS_IMEBase::IsLocaleKorean() )
		{
			// 보이기 시작하는 위치에서부터 작성한다.
			int iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd;
			UpdateAttrForSingleLine( hDC, iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd );

			// 먼저 변환 대기선부터 그린다.
			HPEN	hPen;
			hPen	= (HPEN)SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] );
			if( iThinStart >= 0 && iThinEnd >= 0 )
			{
				MoveToEx( hDC, m_rcDestOfDraw.left + iThinStart,	m_rcDestOfDraw.top + iDistHeightCaret + h, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iThinEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h );
			}
			// 다음에 변환선을 그린다.
			if( iThickStart >= 0 && iThickEnd >= 0 )
			{
				SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] );
				MoveToEx( hDC, m_rcDestOfDraw.left + iThickStart,	m_rcDestOfDraw.top + iDistHeightCaret + h + 1, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iThickEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h + 1 );
			}
			// 다음에 입력선을 그린다.
			if( iDotStart >= 0 && iDotEnd >= 0 )
			{
				SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] );
				MoveToEx( hDC, m_rcDestOfDraw.left + iDotStart,		m_rcDestOfDraw.top + iDistHeightCaret + h, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iDotEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h );
			}
			SelectObject( hDC, hPen );
		}

		// 커서
		if( bCaretOn && !m_bHideCursor )
		{
			int		iCaretWidth, iCaretHeight;
			OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iCaretAsChar + m_iCompCaretAsChar - m_iFirstCharOfDraw, iCaretWidth, iCaretHeight, true );
			iCaretHeight = m_rcDestOfDraw.top + iDistHeightCaret;
			if( !IsTypeOnlyNumeric() )	TextOut( hDC, m_rcDestOfDraw.left + iCaretWidth - 2, iCaretHeight, "|", 1 );
			else						TextOut( hDC, m_rcDestOfDraw.right + 2, iCaretHeight, "|", 1 );

			// actdoll (2004/11/04 21:17) : 발음창을 위한 좌표 세팅
			m_ptCaretDrawPos.x = m_rcDestOfDraw.left + iCaretWidth - 2;
			m_ptCaretDrawPos.y = iCaretHeight;
		}
	}
	// 복수 라인일 경우
	else
	{
		// 현재 입력을 받을 수 있는 상황이라면
		if( m_bCanRecvInputChar )
		{
			char	*pFirstLine, *pLastLine, *pNextLine, *pCaretPos;
			int		iCurrentCaretPos;
			int		iLen	= 0, iCharCount	= 0, iLineCount = 0;
			int		iLineTop = m_rcDestOfDraw.top;
			BOOL	bIncludeReturn;


			// 위치 보정
			UpdateShowRangeForMultiLine( hDC );

			// 상대적인 커서 위치 계산해놓고
			iCurrentCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
			if( iCurrentCaretPos < 0 )		iCurrentCaretPos = 0;

			// 상대적인 포인터 위치 계산해놓고
			pFirstLine	= GetStrPtrByCharPos( m_pszShowBuffer, NULL, m_iFirstCharOfDraw );					// 시작 라인 첫문자 위치
			pLastLine	= GetStrPtrByCharPos( pFirstLine, NULL, m_iLastCharOfDraw - m_iFirstCharOfDraw );	// 마지막 라인 첫문자 위치
			pCaretPos	= GetStrPtrByCharPos( pFirstLine, NULL, iCurrentCaretPos - m_iFirstCharOfDraw );	// 커서 위치

			while( pFirstLine && pFirstLine <= pLastLine )
			{
				// 한 줄을 구분해서 얻어온다.
				pNextLine	= GetSingleLineOfMultiStr( hDC, pFirstLine, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLen, iCharCount, bIncludeReturn );
				if( bIncludeReturn )	
				{
//					iLen = OnlineEditControlEx::GetStrPtrByCharPos( pFirstLine, NULL, iCharCount - 1 ) - pFirstLine;
					iLen		-= 2;
				}

				// 본문
				UINT	uiOption = DT_NOPREFIX | DT_TOP | DT_LEFT | DT_SINGLELINE;
				RECT	rcShow	= { m_rcDestOfDraw.left, iLineTop, m_rcDestOfDraw.right, iLineTop + h };
				if( m_bDrawShadow )
				{
					RECT	rcShadow = { rcShow.left + 1, rcShow.top + 1, rcShow.right + 1, rcShow.bottom + 1 };
					SetTextColor( hDC, m_dwShadowColor );
					DrawText( hDC, pFirstLine, iLen, &rcShadow, uiOption );
				}
				SetTextColor( hDC, m_dwColor );
				DrawText( hDC, pFirstLine, iLen, &rcShow, uiOption );

				// 속성줄
				if( m_bActive && GS_IMEBase::IsIMEComposing() && !GS_IMEBase::IsLocaleKorean() )
				{
					// 보이기 시작하는 위치에서부터 작성한다.
					int iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd;
					UpdateAttrForMultiLine( hDC, pFirstLine, iLen, iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd );

					// 먼저 변환 대기선부터 그린다.
					HPEN	hPen;
					hPen	= (HPEN)SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] );
					if( iThinStart >= 0 && iThinEnd >= 0 )
					{
						MoveToEx( hDC, m_rcDestOfDraw.left + iThinStart,	iLineTop + h, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iThinEnd,		iLineTop + h );
					}
					// 다음에 변환선을 그린다.
					if( iThickStart >= 0 && iThickEnd >= 0 )
					{
						SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] );
						MoveToEx( hDC, m_rcDestOfDraw.left + iThickStart,	iLineTop + h + 1, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iThickEnd,		iLineTop + h + 1 );
					}
					// 다음에 입력선을 그린다.
					if( iDotStart >= 0 && iDotEnd >= 0 )
					{
						SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] );
						MoveToEx( hDC, m_rcDestOfDraw.left + iDotStart,		iLineTop + h, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iDotEnd,		iLineTop + h );
					}
					SelectObject( hDC, hPen );
				}

				// 커서
				BOOL	bArguementIsTrue = false;
				if( bIncludeReturn )
				{
					if( pCaretPos >= pFirstLine && pCaretPos <= pFirstLine + iLen )			bArguementIsTrue	= true;
				}
				else
				{
					if( pCaretPos >= pFirstLine && pCaretPos < pFirstLine + iLen )				bArguementIsTrue	= true;
					else if( !pNextLine && pCaretPos >= m_pszShowBuffer + m_iLenShowBuffer )	bArguementIsTrue	= true;
				}
				if( bCaretOn && !m_bHideCursor && bArguementIsTrue )
				{
					int		iCaretWidth, iCaretHeight;
					OnlineEditControlEx::GetStringPixelSize( hDC, pFirstLine, pCaretPos - pFirstLine, iCaretWidth, iCaretHeight );
					TextOut( hDC, m_rcDestOfDraw.left + iCaretWidth - 2, iLineTop, "|", 1 );

					// actdoll (2004/11/04 21:17) : 발음창을 위한 좌표 세팅
					m_ptCaretDrawPos.x = m_rcDestOfDraw.left + iCaretWidth - 2;
					m_ptCaretDrawPos.y = iLineTop;
				}


				// 정보 갱신
				iLineTop	+= h;
				iLineCount	++;
				pFirstLine	= pNextLine;
			}

		}
		// 입력 받는 것이 불가능한 상황이라면
		else
		{
			// 그림자 찍어주고
			if( m_bDrawShadow )
			{
				RECT	rcShadow = { m_rcDestOfDraw.left + 1, m_rcDestOfDraw.top + 1, m_rcDestOfDraw.right + 1, m_rcDestOfDraw.bottom + 1 };
				SetTextColor( hDC, m_dwShadowColor );
				DrawTextEx( hDC, m_pszShowBuffer, -1, &rcShadow, DT_NOPREFIX | DT_TOP | DT_LEFT | DT_WORDBREAK | DT_MODIFYSTRING | DT_WORD_ELLIPSIS, NULL );
			}
			SetTextColor( hDC, m_dwColor );
			DrawTextEx( hDC, m_pszShowBuffer, -1, &m_rcDestOfDraw, DT_NOPREFIX | DT_TOP | DT_LEFT | DT_WORDBREAK | DT_MODIFYSTRING | DT_WORD_ELLIPSIS, NULL );
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지를 받아 처리하는 부분이다.
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !hWnd )							return FALSE;
	if( !m_bActive )					return FALSE;

	switch( msg )
	{
		case WM_INPUTLANGCHANGE	:
			{
				if( GS_IMEBase::Check_WM_INPUTLANGCHANGE( lParam ) )			return TRUE;
			}
			break;
		case WM_IME_STARTCOMPOSITION	:
			{
				if( Proc_WindowMsg_StartComposition	( hWnd, wParam, lParam ) )	return TRUE;
			}
			break;
		case WM_IME_ENDCOMPOSITION		:
			{
				if( Proc_WindowMsg_EndComposition	( hWnd, wParam, lParam ) )	return TRUE;
			}
			break;
		case WM_IME_COMPOSITION			:
			{
				if( Proc_WindowMsg_Composition		( hWnd, wParam, lParam ) )	return TRUE;
			}
			break;
		case WM_IME_NOTIFY				:
			{
				if( Proc_WindowMsg_Notify			( hWnd, wParam, lParam ) )	return TRUE;
			}
			break;
		case WM_IME_CHAR				:
			{
				// 지금 들어온 조합 문자가 다중바이트 문자일 경우 WM_CHAR에서 해당 횟수에 걸쳐 처리하지 않는다.
				if( wParam > 0xff )	m_iCount_NoProcessAsAscii	+= 2;
				else				m_iCount_NoProcessAsAscii	+= 1;
			}
			break;
		case WM_CHAR					:
			{				
				if( GS_IMEBase::IsIMEComposing() )
				{
					m_iCount_NoProcessAsAscii--;
					if( m_iCount_NoProcessAsAscii < 0 )		m_iCount_NoProcessAsAscii = 0;
					return FALSE;
				}
				if( Proc_WindowMsg_KeyDown_AnsiChar	( hWnd, wParam ) )		return TRUE;
			}
			break;
		case WM_KEYDOWN					:
			{
				if( Proc_WindowMsg_KeyDown_Move		( hWnd, wParam ) )		return TRUE;
				if( Proc_WindowMsg_KeyDown_Delete	( hWnd, wParam ) )		return TRUE;
				if( Proc_WindowMsg_KeyDown_BackSpace( hWnd, wParam ) )		return TRUE;
				if( Proc_WindowMsg_KeyDown_Enter	( hWnd, wParam ) )		return TRUE;
				switch( wParam )
				{
					case VK_SPACE:		break;
					case VK_ESCAPE:		break;
				}

			}
			break;

		case WM_IME_COMPOSITIONFULL		:
		case WM_IME_CONTROL				:
		case WM_IME_SELECT				:
		case WM_IME_SETCONTEXT			:
			{
				int	i = 3;
			}
			break;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_StartComposition()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 조합 개시
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_StartComposition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	GS_IMEBase::Check_WM_IME_STARTCOMPOSITION( lParam );
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_StartComposition()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 조합 종료
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_EndComposition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	// 일본어 조합중일 때
	if( OnlineEditControlEx::IsLanguageForJapanese() )
	{
		// 조합중에 종료가 떨어졌다는 것은 취소되었음을 의미한다.
		if( GS_IMEBase::IsIMEComposing() )
		{
			m_iCompCaretAsAnsi	= 0;
			m_iCompCaretAsChar	= 0;
			UpdateShowBuffer();
		}
	}
	GS_IMEBase::Check_WM_IME_ENDCOMPOSITION( lParam );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_StartComposition()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 조합 중
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_Composition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// 조합 시작이닷!
	GS_IMEBase::Check_WM_IME_COMPOSITION_Start( wParam, lParam );

	// 완성 형태가 떨어졌다!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_ResultString	( lParam, m_iLenLimit - m_iLenString ) )	// 이렇게 해두면 글자는 반드시 여백 범위내로만 받는다.
	{
		// 문자열 체크
		char	*pszToken;
		int		iRet	= GS_IMEBase::GetResultStringLen();					// 받아온 스트링의 크기를 받고
		
		// 본 문자열을 조합(보기 버퍼를 임시로 활용)
		ZeroMemory	( m_pszShowBuffer,						sizeof(m_pszShowBuffer) );
		strcpy		( m_pszShowBuffer,						m_pszString + m_iCaretAsAnsi );				// 뒷열을 복사해놓고
		strncpy		( m_pszString + m_iCaretAsAnsi,			GS_IMEBase::GetResultStringData(),	iRet	);	// 그 사이에 완성된 문자열을 받는다.
		// actdoll (2004/08/12 11:47) : 여기서 잠깐! 완성된 문자열과 함께 금지된 철자가 올 수 있으므로 그것을 확인하도록 한다.
		pszToken	= m_pszString + m_iCaretAsAnsi;
		while( *pszToken )
		{
			// 문자가 멀티바이트 문자라면 일단 스킵
			if( IsDBCSLeadByteEx( GetCodePage(), (BYTE)*pszToken ) )	{ pszToken = CharNextExA( GetCodePage(), pszToken, 0 ); continue; }
			// 안시문자라면 검사
			if( IsExceptionChar( (BYTE)*pszToken ) )	// 금지 문자를 발견했다
			{
				strcpy( pszToken, pszToken + 1 );
				iRet--;
				continue;
			}
			// 무사히 통과됐다면 글자 넘긴다.
			pszToken++;
		}
		strcpy		( m_pszString + m_iCaretAsAnsi + iRet,	m_pszShowBuffer );							// 후방에 데이터를 붙인다.

		// 기타 정보들 세팅
		m_iLenString			= strlen( m_pszString );															// 스트링 사이즈 입력
		m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// 현재 문자수 확인
		m_iCaretAsAnsi			+= iRet;																			// 커서 보정
		m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// 문자열 기준의 커서 보정
		m_iCompCaretAsAnsi		= 0;
		m_iCompCaretAsChar		= 0;

		// 출력 버퍼 갱신
		UpdateShowBuffer();
	}

	// 조합 형태가 떨어졌다!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompString	( lParam, m_iLenLimit - m_iLenString ) )
	{
		// 문자열 체크
		int		iRet	= GS_IMEBase::GetComposingStringLen();						// 받아온 스트링의 크기를 받고

		// 본 문자열을 조합
		ZeroMemory	( m_pszShowBuffer,							sizeof(m_pszShowBuffer) );
		strncpy		( m_pszShowBuffer,							m_pszString,							m_iCaretAsAnsi	);	// 원본 문자열의 앞 문자열 복사
		strncpy		( m_pszShowBuffer + m_iCaretAsAnsi,			GS_IMEBase::GetComposingStringData(),	iRet			);	// 받은 문자열 입력
		strcpy		( m_pszShowBuffer + m_iCaretAsAnsi + iRet,	m_pszString + m_iCaretAsAnsi							);	// 맨 뒤에 나머지 원본 문자열 복사

		// 기타 정보들 세팅
		m_iCompCaretAsAnsi	= iRet;
		m_iCompCaretAsChar	= OnlineEditControlEx::GetCharLen( (char*)GS_IMEBase::GetComposingStringData() );

		// 출력 버퍼 갱신
		UpdateShowBuffer( true );
	}

	// 조합 속성이 떨어졌다!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompAttribute	( lParam, m_iLenLimit - m_iLenString ) )
	{
		memset( m_pszShowAttr, -1, sizeof(m_pszShowAttr) );
		CopyMemory( m_pszShowAttr + m_iCaretAsAnsi, GS_IMEBase::GetComposingAttrData(), GS_IMEBase::GetComposingStringLen() );
	}

	// 조합 상태의 커서 위치가 떨어졌다!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompCursor	( lParam, m_iLenLimit - m_iLenString ) )
	{
		m_iCompCaretAsAnsi	= GS_IMEBase::GetComposingCaretPos();
		m_iCompCaretAsChar	= OnlineEditControlEx::GetCharCountByStrPtr( (char*)GS_IMEBase::GetComposingStringData(), NULL, m_iCompCaretAsAnsi );
	}

	// 조합 끝났닷!
	GS_IMEBase::Check_WM_IME_COMPOSITION_Finish( wParam, lParam );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_Notify()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - IME신호 떨어짐
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_Notify( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	GS_IMEBase::Check_WM_IME_NOTIFY( wParam, lParam );

	// 패스워드/숫자입력/1바이트 문자 입력 상태 중 하나라면
	if( IsTypePassword() || IsTypeOnlyNumeric() || IsTypeOnlyAscii() )	GS_IMEBase::SetImeDeviceOn( false );
	// 그렇지 않다면 입력기 동작 상태로 세팅한다.
	else																GS_IMEBase::SetImeDeviceOn( true );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_AnsiChar()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 문자 들어옴
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_AnsiChar( HWND hWnd, WPARAM wParam )
{
	// 영문 모드가 아닐 경우 아직 들어올 다중바이트 코드가 있다면 제껴라
	if( m_iCount_NoProcessAsAscii > 0 )
	{
		m_iCount_NoProcessAsAscii--;
		if( m_iCount_NoProcessAsAscii < 0 )		m_iCount_NoProcessAsAscii = 0;
		return FALSE;
	}
	

	// 받을 수 없는 문자이거나 문자 수를 넘기거나 제한된 문자일 경우 리턴
	if( !m_bCanRecvInputChar )							return FALSE;
	// 자판에 있는 아스키값이 들어왔다.
	if( wParam < 0x20 || wParam > 0x7e )
	{
		if( wParam != '\n' && wParam != '\r' )			return FALSE;	// 개행문자군이 아니라면 리턴
		else if( !IsTypeMultiLine() )					return FALSE;	// 멀티라인 타입이 아니라면 리턴
		else if( m_iLenString + 2 > m_iLenLimit )		return FALSE;	// 개행문자가 들어갈 여유가 없다면 리턴
	}
	if( IsExceptionChar( (BYTE)wParam )	)				return FALSE;
	if( m_iLenString >= m_iLenLimit )					return FALSE;

	// 숫자 입력 형태일 때
	if( IsTypeOnlyNumeric() )
	{
		if( wParam < 0x0030 || wParam > 0x0039 )	return FALSE;	// 숫자가 아니라면 리턴
		// 맨 앞자리가 0일 경우
		if( m_pszString[0] == '0' )
		{
			// 0이 다시 들어왔다면 받지 마라
			if( wParam == 0x0030 )					return FALSE;
			// 그렇지 않다면 원래 수치대로 세팅
			else
			{
				m_pszString[0]			= NULL;
				m_iLenString			= 0;
				m_iCaretAsChar			= m_iCaretAsAnsi	= 0;
				m_iCompCaretAsAnsi		= 0;
				m_iCompCaretAsChar		= 0;
			}
		}
	}

	// 버퍼를 조합한다.(임시로 출력 버퍼를 사용)
	ZeroMemory	( m_pszShowBuffer, sizeof(m_pszShowBuffer) );
	strcpy		( m_pszShowBuffer,						m_pszString + m_iCaretAsAnsi );	// 뒷쪽 값 복사

	// 개행 문자 아닐때는 그냥 붙이고
	if( wParam != '\n' && wParam != '\r' )
	{
		m_pszString[m_iCaretAsAnsi] = (char)wParam;											// 문자 집어넣는다.
		strcpy		( m_pszString + m_iCaretAsAnsi + 1,		m_pszShowBuffer );				// 나머지 값을 복사
	}
	// 개행 문자일때는 '\r\n'을 붙인다
	else
	{
		m_pszString[m_iCaretAsAnsi]		= '\r';
		m_pszString[m_iCaretAsAnsi + 1] = '\n';
		strcpy		( m_pszString + m_iCaretAsAnsi + 2,		m_pszShowBuffer );
	}

	// 숫자 입력 시 버퍼 다시 계산
	if( IsTypeOnlyNumeric() )
	{
		LONGLONG	i64Value = _atoi64( m_pszString );
		if( i64Value > m_i64ValueLimit )
		{
			SetValue( i64Value );
			return TRUE;
		}
	}

//	strncpy		( m_pszShowBuffer,						m_pszString,	m_iCaretAsAnsi );	// 앞쪽 값 복사
//	m_pszShowBuffer[m_iCaretAsAnsi] = (char)wParam;											// 단어 집어넣는다.
//	strcpy		( m_pszShowBuffer + m_iCaretAsAnsi + 1, pCurPtr );							// 나머지 값을 복사
//	strcpy		( m_pszString,							m_pszShowBuffer );					// 원본으로 다시 복사
	
	// 뒷마무리
	if( wParam != '\n' && wParam != '\r' )
	{
		m_iLenString++;		
		m_iCaretAsAnsi++;
	}
	else
	{
		m_iLenString	+= 2;	// 스트링 사이즈 보정
		m_iCaretAsAnsi	+= 2;	// 커서 보정
	}
	m_iCountChar++;		// 현재 문자수 확인
	m_iCaretAsChar++;	// 문자열 기준의 커서 보정

	// 출력 버퍼 갱신
	UpdateShowBuffer();
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_Move()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 키 커서 관련 메시지 떨어짐
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Move( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )			return FALSE;

	// 숫자 입력 상태에서는 커서키를 쓸 수 없다
	if( IsTypeOnlyNumeric() )			return FALSE;

	switch( wParam )
	{
		case VK_UP:
			{
				if( IsTypeMultiLine() )		ChangeCaretNext( false );
				else						ChangeFromBackUpToString( false );
			}
			break;
		case VK_DOWN:
			{
				if( IsTypeMultiLine() )		ChangeCaretNext( true );
				else						ChangeFromBackUpToString( true );
			}
			break;

		case VK_LEFT:
			{
				m_iCaretAsChar--;
				if( m_iCaretAsChar < 0 )	m_iCaretAsChar = 0;
				SetCaretPosAsChar( m_iCaretAsChar );
			}
			break;

		case VK_RIGHT:
			{
				m_iCaretAsChar++;
				if( m_iCaretAsChar > m_iCountChar )	m_iCaretAsChar = m_iCountChar;
				SetCaretPosAsChar( m_iCaretAsChar );
			}
			break;
		case VK_HOME:
			{
				if( IsTypeMultiLine() )		ChangeCaretNext( false, true );
				else
				{
					m_iCaretAsChar			= 0;
					m_iCaretAsAnsi			= 0;
				}
			}
			break;
		case VK_END:
			{
				if( IsTypeMultiLine() )		ChangeCaretNext( true, true );
				else
				{
					m_iCaretAsChar			= m_iCountChar;
					m_iCaretAsAnsi			= m_iLenString;
				}
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_Delete()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 키 누름 - Delete
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Delete( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// 조건에 안맞을 때 리턴
	if( wParam != VK_DELETE )									return FALSE;
	if( m_iLenString <= 0 || m_iCaretAsAnsi == m_iLenString )	return FALSE;

	// 현재의 커서에서 다음 글자를 받는다.
	BOOL	bReturnExist;
	char	*pNewPtr = OnlineEditControlEx::GetCharNext( m_pszString + m_iCaretAsAnsi, bReturnExist );

	// 버퍼를 조합한다.
	strcpy( m_pszString + m_iCaretAsAnsi, pNewPtr );

	// 뒷마무리
	m_iLenString		= strlen( m_pszString );	// 스트링 사이즈 입력
	m_iCountChar		= OnlineEditControlEx::GetCharLen( m_pszString );	// 현재 문자수 확인

	// 출력 버퍼 갱신
	UpdateShowBuffer();
	
	return TRUE;	
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_BackSpace()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 키 누름 - BackSpace
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_BackSpace( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// 조건에 안맞을 때 리턴
	if( wParam != VK_BACK )										return FALSE;
	if( m_iLenString <= 0 || m_iCaretAsAnsi <= 0 )				return FALSE;

	// 현재의 커서에서 이전 위치를 받는다.
	BOOL	bReturnExist;
	char	*pNewPtr	= OnlineEditControlEx::GetCharPrev( m_pszString, m_pszString + m_iCaretAsAnsi, bReturnExist );

	// 버퍼를 조합한다.
	strcpy( pNewPtr, m_pszString + m_iCaretAsAnsi );

	// 수치만 입력받는 형태이며 글씨가 하나도 안남았을 경우는 0(한글자)을 세팅해준다.
	if( IsTypeOnlyNumeric() && strlen( m_pszString ) == 0 )		
	{
		m_pszString[0]			= '0';
		m_pszString[1]			= NULL;
		m_iLenString			= 1;
		m_iCountChar			= 1;
		m_iCaretAsAnsi			= 1;
		m_iCaretAsChar			= 1;
		m_iCompCaretAsAnsi		= 0;
		m_iCompCaretAsChar		= 0;
	}
	else
	{
		// 뒷마무리
		m_iLenString	= strlen( m_pszString );		// 스트링 사이즈 입력
		if( m_iCountChar > 0 )		m_iCountChar--;		// 현재 문자수 확인
		m_iCaretAsAnsi		= pNewPtr - m_pszString;	// 커서 보정
		if( m_iCaretAsChar > 0 )	m_iCaretAsChar--;;	// 문자열 기준의 커서 보정
		m_iCompCaretAsAnsi	= 0;
		m_iCompCaretAsChar	= 0;
	}

	// 출력 버퍼 갱신
	UpdateShowBuffer();
	
	return TRUE;	
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_Enter()
// Code: actdoll (2004-04-17)
// Desc: 윈도 메시지 - 키 누름 - ENTER, TAB
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Enter( HWND hWnd, WPARAM wParam )
{
	// 조건에 안맞을 때 리턴
	if( wParam != VK_RETURN && wParam != VK_TAB )				return FALSE;
	// 중국어 번/간체 윈도98에서는 조합중에 엔터키가 올 경우 무시한다.
	if( GS_IMEBase::IsLocaleSimpChinese() || GS_IMEBase::IsLocaleTradChinese() )
	{
		if( GS_IMEBase::IsIMEComposing() )						return FALSE;
	}

	// 단일 라인일 경우
	if( wParam == VK_RETURN )
	{
		if( IsTypeSingleLine() )
		{
			if( IsTypeBackUpString() )	ChangeFromStringToBackUp();	// 백업 스트링을 사용하고 있다면 문자를 백업해라
		}
		m_bPushedEnter	= true;
	}
	else if( wParam == VK_TAB )
	{
		if( IsTypeSingleLine() )		m_bPushedTab	= true;
	}

	return TRUE;	
}

//-----------------------------------------------------------------------------
// Name: UpdateShowBuffer()											[private]
// Code: actdoll (2004-04-17)
// Desc: 본문 스트링에 맞춰 출력 스트링을 업데이트 한다.
//		만약 외부에서 m_pszShowBuffer를 건드렸을 경우 bNotTouchShowBuffer를 true로 주면 된다.
//		bNotTouchShowBuffer	- m_pszShowBuffer를 건드리지 않을 것이면 true, 본문에 맞춰 수정한다면 false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowBuffer( BOOL bNotTouchShowBuffer )
{
	if( !bNotTouchShowBuffer )
	{
		ZeroMemory( m_pszShowBuffer, sizeof(m_pszShowBuffer) );
		
		// 패스워드 타입일 경우
		if( IsTypePassword() )
		{
			memset( m_pszShowBuffer, '*', m_iLenString );	// 원본 스트링 사이즈 만큼 별표를 붙인다.
		}
		// 수치만 입력받는 타입일 경우
		else if( IsTypeOnlyNumeric() )
		{
			char	*pDest, *pSrc;
			pSrc	= m_pszString;					// 포인터 받고
			pDest	= m_pszShowBuffer;				// 포인터 받고
			int		iValue	= m_iLenString / 3;		// 3단위로 나눈 몫을 받는다.
			int		iRemain	= m_iLenString % 3;		// 3단위로 나눈 나머지를 받는다.

			// 자릿수가 만들어질 수 있는 상황일 경우
			while( *pSrc != NULL )	
			{
				// 나머지가 0일 경우 3단위씩 나누고
				if( iRemain == 0 ) 
				{
					iValue--;
					strncpy( pDest, pSrc, 3 );
					pDest += 3, pSrc += 3;
					if( iValue > 0 )				{ *pDest = ','; pDest++; }
				}
				// 나머지가 0이 아닐 경우 해당 단위로 한번만 나눈다.
				else
				{
					strncpy( pDest, pSrc, iRemain );
					pDest += iRemain, pSrc += iRemain;
					if( iValue >= 1 && iRemain > 0 ){ *pDest = ','; pDest++; }
					iRemain = 0;
				}
			}
		}
		// 그 외의 경우는 복사만 한다.
		else
		{
			strcpy( m_pszShowBuffer, m_pszString );
		}
	}

	// 기타 정보 설정
	m_iLenShowBuffer		= strlen( m_pszShowBuffer );						// 길이 받고
	m_iCountCharShowBuffer	= OnlineEditControlEx::GetCharLen( m_pszShowBuffer );	// 갯수 받고
}

//-----------------------------------------------------------------------------
// Name: UpdateShowRangeForSingleLine()								[private]
// Code: actdoll (2004-04-17)
// Desc: 출력 스트링이 찍혀야 할 범위를 구분한다.
//	여기에서 m_iFirstCharOfDraw와 m_iLastCharOfDraw는 보여지게 되는 문자의 위치를 말한다.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowRangeForSingleLine( HDC hDC )
{
	if( !hDC )		return;

	// 현재 출력 버퍼에 들어있는 스트링 값이 보여지는 범위를 구한다.
	char	*pStart, *pCaret, *pLast;
	int		iLineWidth, iLineHeight, iCaretPos, iLimitWidth, iMoveRange;

	// 시야 범위 조정 보정
	if( m_iLastCharOfDraw > m_iCountCharShowBuffer )	m_iLastCharOfDraw	= m_iCountCharShowBuffer;
	if( m_iFirstCharOfDraw > m_iLastCharOfDraw - 1 )	m_iFirstCharOfDraw	= m_iLastCharOfDraw - 3;
	if( m_iFirstCharOfDraw < 0 )						m_iFirstCharOfDraw	= 0;

	// 일단 출력 시작 위치서부터 문장 끝까지의 거리가 범위내에 들어오나 판독
	iCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
	iLimitWidth = m_rcDestOfDraw.right - m_rcDestOfDraw.left;
	pStart		= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	m_iFirstCharOfDraw );
	pLast		= GetStrPtrByCharPos( m_pszShowBuffer,	pStart,	m_iLastCharOfDraw - m_iFirstCharOfDraw );

	// 보여지는 범위가 크기에 비해 넘 작다면 계산 한번 다시 한다.
	OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iLastCharOfDraw - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );
	while( iLineWidth < iLimitWidth && m_iLastCharOfDraw < m_iCountCharShowBuffer )
	{
		// 문자를 하나씩 추가하여 큰지 안큰지 살펴본다.
		OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iLastCharOfDraw + 1 - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );
		if( iLineWidth < iLimitWidth && m_iLastCharOfDraw < m_iCountCharShowBuffer )	m_iLastCharOfDraw++;
	}
	
	// 만약 출력 범위 오른쪽 값이 커서의 값보다 작을 경우
	if( m_iLastCharOfDraw <= iCaretPos )	
	{
		// 일단 기존의 오른쪽 위치에서 몇글자 뒤로 돌린다.
		iMoveRange	= iCaretPos - m_iLastCharOfDraw;
		if( iMoveRange < 3 )	iMoveRange = 3;
		iCaretPos	+= iMoveRange;
		if( iCaretPos > m_iCountCharShowBuffer )	iCaretPos = m_iCountCharShowBuffer;

		// 기존의 시작문자에서 새로운 끝문자까지의 픽셀을 계산
		BOOL	bReturnExist;
		while(1)
		{
			OnlineEditControlEx::GetStringPixelSize( hDC, pStart, iCaretPos - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );

			// 문자열이 아직 크다면 계속 줄여나간다.
			if( iLineWidth < iLimitWidth || iCaretPos - m_iFirstCharOfDraw <= 0 )	break;
			pStart = OnlineEditControlEx::GetCharNext( pStart, bReturnExist );
			m_iFirstCharOfDraw++;
		}

		// 이동한 값을 넣어준다.
		m_iLastCharOfDraw	= iCaretPos;
	}
	// 만약 출력 범위 왼쪽 값이 커서의 값보다 클 경우 이동
	else if( m_iFirstCharOfDraw >= iCaretPos )	
	{
		// 일단 기존의 오른쪽 위치에서 몇글자 뒤로 돌린다.
		iMoveRange	= m_iFirstCharOfDraw - iCaretPos;
		if( iMoveRange < 3 )	iMoveRange = 3;
		iCaretPos	-= iMoveRange;
		if( iCaretPos < 0 )	iCaretPos = 0;

		// 새로운 시작문자에서 기존 끝문자까지의 픽셀을 계산
		pCaret		= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	iCaretPos );
		while(1)
		{
			OnlineEditControlEx::GetStringPixelSize( hDC, pCaret, m_iLastCharOfDraw - iCaretPos, iLineWidth, iLineHeight, true );

			// 문자열이 아직 크다면 계속 줄여나간다.
			if( iLineWidth < iLimitWidth || m_iLastCharOfDraw - iCaretPos <= 0 )	break;
			m_iLastCharOfDraw--;
		}

		// 이동한 값을 넣어준다.
		m_iFirstCharOfDraw	= iCaretPos;
	}
}

//-----------------------------------------------------------------------------
// Name: UpdateShowRangeForMultiLine()								[private]
// Code: actdoll (2004-04-17)
// Desc: 출력 스트링이 찍혀야 할 범위를 구분한다.
//	여기에서 m_iFirstCharOfDraw와 m_iLastCharOfDraw는 
//	보여지게 되는 첫번째 라인의 맨 앞 위치와 마지막 라인의 맨 앞 위치를 뜻한다.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowRangeForMultiLine( HDC hDC )
{
	if( !hDC )		return;

	// 현재 출력 버퍼에 들어있는 스트링 값이 보여지는 범위를 구한다.
	char	*pSrc, *pNext;
	int		iMaxShowLineCount, iLineWidth, iLineHeight, iCaretPos;
	int		iFirstLineNo, iLastLineNo, iCaretLineNo, iCaretLineCharCount;
	int		iLenPerLine = 0, iCharCountPerLine = 0, iCharCount = 0, iLineCount = 0, iAllLineCount = 0;
	BOOL	bIncludeReturn;

	// 각종 정보를 구해놓는다.
	OnlineEditControlEx::GetStringPixelSize( hDC, "1", 1, iLineWidth, iLineHeight, false );						// 일단 글자당 높이를 구하고
	iMaxShowLineCount = ( iLineHeight )	?	( m_rcDestOfDraw.bottom - m_rcDestOfDraw.top ) / iLineHeight + 1: 10;	// 이 박스에서 한번에 보일 수 있는 줄의 갯수를 구하고

	// 총 라인수를 구한다.
	pSrc	= m_pszShowBuffer;
	while( pSrc ){ pSrc	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn ); iAllLineCount++; }
	iCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
	
	// 시야 범위 조정 보정
	if( m_iLastCharOfDraw > m_iCountCharShowBuffer )	m_iLastCharOfDraw	= m_iCountCharShowBuffer;
	if( m_iFirstCharOfDraw >= m_iLastCharOfDraw )		m_iFirstCharOfDraw	= m_iLastCharOfDraw;
	if( m_iFirstCharOfDraw < 0 )						m_iFirstCharOfDraw	= 0;

	// 줄의 갯수를 계산한다.
	pSrc	= m_pszShowBuffer;
	while( pSrc )
	{
		// 한 줄을 구하고
		pNext	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn );

		// 보여지는 줄의 첫번째를 찾는다.
		if( m_iFirstCharOfDraw >= iCharCount	&& m_iFirstCharOfDraw < iCharCount + iCharCountPerLine )	
		{
			iFirstLineNo		= iLineCount;
			m_iFirstCharOfDraw	= iCharCount;

			// 일단 첫번째 줄을 기준으로 보여질 수 있는 마지막 줄의 라인수를 맞춰 놓는다.
			iLastLineNo			= iFirstLineNo + iMaxShowLineCount - 1;
			if( iLastLineNo >= iAllLineCount )	iLastLineNo = iAllLineCount - 1;
		}

		// 보여지는 줄의 마지막을 찾는다.
		if( iLastLineNo == iLineCount )
//		if( m_iLastCharOfDraw >= iCharCount		&& m_iLastCharOfDraw < iCharCount + iCharCountPerLine )		
		{
//			iLastLineLen		= pSrc - m_pszShowBuffer;
			m_iLastCharOfDraw	= iCharCount;
		}

		// 커서가 그 범주내에 있음을 확인했다면 커서 줄 확보
		if( iCaretPos >= iCharCount				&& iCaretPos < iCharCount + iCharCountPerLine )			
		{
			iCaretLineNo		= iLineCount;
//			iCaretLineLen		= pSrc - m_pszShowBuffer;
			iCaretLineCharCount	= iCharCount;
		}
		// 범주내에 들지는 않지만 맨 마지막 줄일 경우도 확보
		else if( !pNext && iCaretPos >= m_iCountCharShowBuffer )
		{
			iCaretLineNo		= iLineCount;
//			iCaretLineLen		= pSrc - m_pszShowBuffer;
			iCaretLineCharCount	= iCharCount;
		}

		// 다음줄로 넘어간다.
		iLineCount	++;
		iCharCount	+= iCharCountPerLine;
		pSrc		= pNext;
	}

	iCharCount = 0;
	// 마지막줄보다 캐럿이 아래 가있다면
	if( iCaretLineNo > iLastLineNo )
	{
		// 일단 마지막 줄을 캐럿 줄로 맞춰주고
		iLastLineNo			= iCaretLineNo;
//		iLastLineLen		= iCaretLineLen;
		m_iLastCharOfDraw	= iCaretLineCharCount;

		// 첫줄을 다시 계산해준다.
		iFirstLineNo		= iLastLineNo - iMaxShowLineCount + 1;
		if( iFirstLineNo < 0 )	iFirstLineNo = 0;

		pSrc				= m_pszShowBuffer;
//		iFirstLineLen		= 0;
		m_iFirstCharOfDraw	= 0;
		while( pSrc && iFirstLineNo > iCharCount )
		{
			pNext	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn );
			iCharCount++;
//			iFirstLineLen		= pSrc - m_pszShowBuffer;
			m_iFirstCharOfDraw	+= iCharCountPerLine;
			pSrc				= pNext;
		}
	}
	// 첫줄보다 캐럿이 위에 가있다면 캐럿 줄로 맞춰준다.
	else if( iCaretLineNo < iFirstLineNo )	
	{
		iFirstLineNo		= iCaretLineNo;
//		iFirstLineLen		= iCaretLineLen;
		m_iFirstCharOfDraw	= iCaretLineCharCount;

		// 마지막줄을 다시 계산해준다.
		iLastLineNo			= iFirstLineNo + iMaxShowLineCount - 1;
		if( iLastLineNo >= iLineCount )		iLastLineNo = iLineCount - 1;

		pSrc				= m_pszShowBuffer;
//		iLastLineLen		= 0;
		m_iLastCharOfDraw	= 0;
		while( pSrc && iLastLineNo > iCharCount )
		{
			pNext	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn );
			iCharCount++;
//			iLastLineLen		= pSrc - m_pszShowBuffer;
			m_iLastCharOfDraw	+= iCharCountPerLine;
			pSrc				= pNext;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: UpdateAttrForSingleLine()											[private]
// Code: actdoll (2004-04-17)
// Desc: 속성 출력에서 각각의 선이 찍혀야 할 범위를 계산한다.
//		hDC						- 윈도 DC값
//		iDotStart, iDotEnd,		- 도트 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//		iThinStart, iThinEnd	- 가는선 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//		 iThickStart, iThickEnd	- 굵은선 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateAttrForSingleLine( HDC hDC, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd )
{
	iDotStart = iDotEnd = iThinStart = iThinEnd = iThickStart = iThickEnd = -1;
	if( !hDC )		return;

	// 필요 정보 추출
	char	*pCompStart, *pCompEnd, *pDrawStartString, *pAttrStart, *pAttrEnd, *pDrawStartAttr, *pAttrInput;
	int		iTemp, iPosFromFront, iShowRange, iDrawDistFromStart, iDrawDistFromAttr;
	iShowRange			= m_rcDestOfDraw.right - m_rcDestOfDraw.left;							// 에디트 박스의 범위

	pCompStart			= m_pszShowBuffer + m_iCaretAsAnsi;										// 조합 시작된 위치
	pCompEnd			= pCompStart + GS_IMEBase::GetComposingStringLen();					// 조합 마지막 위치
	pDrawStartString	= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	m_iFirstCharOfDraw );	// 문자열이 그려지기 시작하는 위치
	iDrawDistFromStart	= pCompStart - pDrawStartString;										// 조합 시작 위치와 문자열이 그려지기 시작하는 위치의 차이

	// 좌측 위치 검색
	( iDrawDistFromStart > 0 )	? iPosFromFront	= iDrawDistFromStart : iPosFromFront	= 0;		// 선이 그려지기 시작할 버퍼 위치를 찾는다. 전방에 있을 경우 맨 앞에서 그리면 되므로 0을 만들면 된다.
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iPosFromFront, iThinStart, iTemp );	// 좌측 시작 픽셀 위치 완료

	// 우측 위치 검색
	int		iLen = ( pCompStart > pDrawStartString ) ? pCompEnd - pCompStart : pCompEnd - pDrawStartString;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iLen, iThinEnd, iTemp );	// 조합중인 문자열의 픽셀길이를 구한다.
	( iShowRange - iThinStart < iThinEnd ) ? iThinEnd = iShowRange : iThinEnd += iThinStart;							// 시야 범위를 검사하여 우측 종료 픽셀 위치 완료


	// 기타 속성 검색
	pAttrStart			= m_pszShowAttr + m_iCaretAsAnsi;							// 조합 시작된 속성의 위치
	pAttrEnd			= pAttrStart + GS_IMEBase::GetComposingStringLen();		// 조합 마지막 속성의 위치
	pDrawStartAttr		= m_pszShowAttr + ( pDrawStartString - m_pszShowBuffer );	// 조합 속성이 그려지기 시작하는 위치
	
	// 입력중인 속성을 찾는다.
	int		iSizeStart, iSizeEnd;
	int		iAttrType = -1, iCountAttrInput = 0;
	char	*pTemp = pAttrInput = pAttrStart;
	while( pTemp - m_pszShowAttr < GS_IMEBase::GetComposingStringLen() || *pTemp != -1 )
	{
		// 변환 대기 중 값이 아닐 경우
		if( *pTemp < EN_FLAG_OEC_ATTRLINE_THIN )
		{
			// 처음으로 발견했다
			if( iCountAttrInput <= 0 )
			{
				pAttrInput	= pTemp;	// 시작된 포인터 받고
				iAttrType	= *pTemp;	// 속성값 받고
			}
			iCountAttrInput++;
		}
		pTemp++;
	}
	// 현재 입력 문자가 받아지고 있는 범위를 계산한다.
	iDrawDistFromAttr	= pAttrInput - pDrawStartAttr;

	// 좌측 위치 검색
	( iDrawDistFromAttr > 0 )	? iPosFromFront	= iDrawDistFromAttr : iPosFromFront = 0;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iPosFromFront, iSizeStart, iTemp );				// 조합 시작점과 뷰파인더 시작점의 픽셀차를 구한다.

	// 우측 위치 검색
	iLen = ( pAttrInput > pDrawStartAttr ) ? iCountAttrInput : ( pAttrInput + iCountAttrInput ) - pDrawStartAttr;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iLen, iSizeEnd, iTemp );		// 조합 문자열의 픽셀크기를 구한다.
	( iShowRange - iSizeStart < iSizeEnd ) ? iSizeEnd = iShowRange : iSizeEnd += iSizeStart;

	// 속성값에 따라 달리 값을 제시한다.
	if( iAttrType == EN_FLAG_OEC_ATTRLINE_THICK )	{ iThickStart	= iSizeStart;	iThickEnd	= iSizeEnd; }
	else											{ iDotStart		= iSizeStart;	iDotEnd		= iSizeEnd; }
}

//-----------------------------------------------------------------------------
// Name: UpdateAttrForSingleLine()											[private]
// Code: actdoll (2004-04-17)
// Desc: 속성 출력에서 각각의 선이 찍혀야 할 범위를 계산한다.
//		hDC						- 윈도 DC값
//		iDotStart, iDotEnd,		- 도트 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//		iThinStart, iThinEnd	- 가는선 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//		 iThickStart, iThickEnd	- 굵은선 표시의 시작과 끝. DC값이 없거나 그리지 않아도 된다면 -1리턴
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateAttrForMultiLine( HDC hDC, char *pszFirstCharOfLine, int iLenOfLine, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd )
{
	iDotStart = iDotEnd = iThinStart = iThinEnd = iThickStart = iThickEnd = -1;
	
	if( !hDC )		return;
	if( pszFirstCharOfLine < m_pszShowBuffer )							return;
	else if( pszFirstCharOfLine > m_pszShowBuffer + m_iLenShowBuffer )	return;

	// 필요 정보 추출
	char	*pCompStart, *pCompEnd;
	int		iAttrFirstPos, iAttrLastPos;
	int		iShowRange;
	iShowRange			= m_rcDestOfDraw.right - m_rcDestOfDraw.left;							// 에디트 박스의 범위

	pCompStart			= m_pszShowBuffer + m_iCaretAsAnsi;										// 조합 시작된 위치
	pCompEnd			= pCompStart + GS_IMEBase::GetComposingStringLen();					// 조합 마지막 위치

	// 조합 시작 위치를 점검해 본다.
	if( pCompStart <= pszFirstCharOfLine )						iAttrFirstPos	= 0;	// 이 줄 시작 보다 훨씬 이전에 스타트 지점이 있다면 선 시작점은 0
	else if( pCompStart > pszFirstCharOfLine + iLenOfLine )		return;					// 이 줄 끝보다 훨씬 뒤에 시작점이 있다면 그냥 리턴
	else																				// 그렇지 않다면 이 라인을 기준으로 한 시작점의 위치를 계산해놓는다.
	{
		iAttrFirstPos	= pCompStart - pszFirstCharOfLine;
	}

	// 조합 종료 위치를 점검해 본다.
	if( pCompEnd < pszFirstCharOfLine )							return;							// 이 줄 시작 보다 훨씬 이전에 종료 지점이 있다면 리턴
	else if( pCompEnd >= pszFirstCharOfLine + iLenOfLine )		iAttrLastPos	= iLenOfLine;	// 이 줄 끝보다 훨씬 뒤에 종료 지점이 있다면 종료점은 선의 마지막
	else																						// 그렇지 않다면 이 라인을 기준으로 한 종료점의 위치를 계산해놓는다.
	{
		iAttrLastPos	= pCompEnd - pszFirstCharOfLine;
	}

	// 조합점이 있다면 그 내용을 분석해본다.
	int		i, iAttrType = -1, iCountAttrInput = 0;
	char	*pTemp, *pAttrInput = NULL;
	for( i = iAttrFirstPos; i<iAttrLastPos; i++ )
	{
		 pTemp = m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) + i;
		// 변환 대기 중 값이 아닐 경우
		if( *pTemp < EN_FLAG_OEC_ATTRLINE_THIN )
		{
			// 처음으로 발견했다
			if( iCountAttrInput <= 0 )
			{
				pAttrInput	= pTemp;	// 시작된 포인터 받고
				iAttrType	= *pTemp;	// 속성값 받고
			}
			iCountAttrInput++;
		}
		pTemp++;
	}

	int		iWidth, iHeight, iWidth2, iHeight2;
	// 전체 풀 라인의 위치 검색
	GetStringPixelSize( hDC, pszFirstCharOfLine, iAttrFirstPos, iThinStart, iHeight );
	GetStringPixelSize( hDC, pszFirstCharOfLine, iAttrLastPos, iThinEnd, iHeight );

	// 특수 라인 검색
	if( pTemp )		// 특수 라인이 있었다면
	{
		GetStringPixelSize( hDC, pszFirstCharOfLine, pAttrInput - ( m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) ),						iWidth, iHeight );
		GetStringPixelSize( hDC, pszFirstCharOfLine, pAttrInput - ( m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) ) + iCountAttrInput,	iWidth2, iHeight2 );

		// 속성값에 따라 달리 값을 제시한다.
		if( iAttrType == EN_FLAG_OEC_ATTRLINE_THICK )	{ iThickStart	= iWidth;	iThickEnd	= iWidth2; }
		else											{ iDotStart		= iWidth;	iDotEnd		= iWidth2; }
	}
}

//-----------------------------------------------------------------------------
// Name: SetCodePage()									[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	현재 클라이언트의 언어 코드 페이지를 설정한다. 아울러 다중 복합형 조합문자의 경우 숙어창을 열리게 할 것인가 아닌가를 결정한다.
//		pszFirstStr	- 검색하고자 하는 NULL이 아닌 스트링 버퍼 포인터
//		pszBaseStr	- pszFirstStr내 검색 기준위치 포인터. 이것이 NULL일 경우 맨 앞쪽부터 찾는다.
//		iCharPos	- 몇번째 위치한 문자의 포인터를 찾고 싶은가? -1일 경우 NULL문자가 위치한 포인터 리턴
//		return		- 해당 번째의 포인터 리턴, pszSrcStr가 NULL이거나 범위를 벗어날 경우 NULL리턴
//-----------------------------------------------------------------------------
//void	OnlineEditControlEx::SetCodePage( UINT uiCodePage )
//{
//	GS_IMEBase::SetCodePage( uiCodePage );
//
//	switch( uiCodePage )
//	{
//		case DF_VALUE_OEC_CP_KOREA		:	
//		case DF_VALUE_OEC_CP_TAIWAN		:	
//		case DF_VALUE_OEC_CP_CHINA		:	
//			m_bIsCompTypeMultiChar	= false;
//			break;
//
//		case DF_VALUE_OEC_CP_JAPAN		:
//		case DF_VALUE_OEC_CP_ENGLISH	:
//			m_bIsCompTypeMultiChar	= true;
//			break;
//	}
//}

//-----------------------------------------------------------------------------
// Name: GetStrPtrByCharPos()									[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 문자열에서 몇번째 문자에 대한 위치를 요구할 경우 그 위체에 해당되는 포인터를 넘긴다.
//		pszFirstStr	- 검색하고자 하는 NULL이 아닌 스트링 버퍼 포인터
//		pszBaseStr	- pszFirstStr내 검색 기준위치 포인터. 이것이 NULL일 경우 맨 앞쪽부터 찾는다.
//		iCharPos	- 몇번째 위치한 문자의 포인터를 찾고 싶은가? -1일 경우 NULL문자가 위치한 포인터 리턴
//		return		- 해당 번째의 포인터 리턴, pszSrcStr가 NULL이거나 범위를 벗어날 경우 NULL리턴
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetStrPtrByCharPos( char *pszFirstStr, char *pszBaseStr, int iCharPos )
{
	if( !pszFirstStr )									return NULL;

	// 조건식 보정
	if( !pszBaseStr || pszBaseStr - pszFirstStr < 0 )	pszBaseStr = pszFirstStr;
	if( iCharPos == 0 )									return pszBaseStr;


	// 오른쪽으로 검색해 들어간다.
	BOOL	bReturnExist;
	if( iCharPos > 0 )
	{
		while( pszBaseStr && iCharPos-- )	
		{
			// 만약 개행문자 대열을 받았을 경우 하나 더 이동해준다.
//			if( *pszBaseStr == '\n' || *pszBaseStr == '\r' )	
//			{
//				if( *( pszBaseStr + 1 ) == '\n' || *( pszBaseStr + 1 ) == '\r' )									pszBaseStr++;
//			}
//			pszBaseStr	= CharNextExA( GetCodePage(), pszBaseStr, 0 );
			pszBaseStr	= GetCharNext( pszBaseStr, bReturnExist );
		}
	}
	// 왼쪽으로 검색해 들어간다.
	else
	{
		while( pszBaseStr && iCharPos++ )	
		{
			// 만약 개행문자 대열을 받았을 경우 하나 더 이동해준다.
//			if( *pszBaseStr == '\n' || *pszBaseStr == '\r' )	
//			{
//				if( pszBaseStr > pszFirstStr && ( *( pszBaseStr - 1 ) == '\n' || *( pszBaseStr - 1 ) == '\r' ) )	pszBaseStr--;
//			}
//			pszBaseStr	= CharPrevExA( GetCodePage(), pszFirstStr, pszBaseStr, 0 );
			pszBaseStr	= GetCharPrev( pszFirstStr, pszBaseStr, bReturnExist );
		}
	}
	return pszBaseStr;
}

//-----------------------------------------------------------------------------
// Name: GetCharCountByStrPtr()									[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	기준 포인터로부터 몇번째 포인터에 위치한 문자까지 몇개의 문자들이 존재하는지를 알려준다.
//	포인터의 위치가 범주를 벗어날 경우 -1을 리턴한다.
//	bGetUpper가 true일 경우 문자 번째의 카운트를 올림하고, 그렇지 않으면 내림하여 계산한다.
//		pszFirstStr	- 뒤로 이동할 때의 NULL이 아닌 선두번지. 즉, 문자열 버퍼 내의 가장 선두 포인터이다.
//		pszBaseStr	- 검색하고자 하는 스트링 버퍼 포인터의 기준위치번지. 이 값이 NULL일 경우 pszFirstStr이 기준이 된다.
//		iPtrPos		- pszBaseStr로부터 몇번째 위치한 포인터를 찾을 것인가?
//		bGetupper	- 결과값이 반이 나왔을 경우 올림이라면 true, 내림이라면 false(기본값 false)
//		return		- 주어진 포인터의 문자번째 리턴, 조건식이 안맞거나 그자리 그대로일 경우 0 리턴
//-----------------------------------------------------------------------------
int		OnlineEditControlEx::GetCharCountByStrPtr( char *pszFirstStr, char *pszBaseStr, int iPtrPos, BOOL bGetUpper )
{
	if( !pszFirstStr )										return 0;
	if( iPtrPos == 0 )										return 0;

	// 조건식 보정
	if( !pszBaseStr || pszBaseStr - pszFirstStr < 0 )				pszBaseStr = pszFirstStr;
	int	iLen = strlen( pszBaseStr );
	if( iPtrPos > iLen )											iPtrPos	= iLen;
	else if( iPtrPos < 0 && pszFirstStr - pszBaseStr > iPtrPos  )	iPtrPos = pszFirstStr - pszBaseStr;

	// 글씨를 검색한다.
	int		iCount = 0;
	char	*pszPrev, *pszNext;
	pszPrev = pszNext = pszBaseStr;

	// 오른쪽으로 검색이 갈 경우
	BOOL	bEnterExist;
	if( iPtrPos >= 0 )
	{
		while(1)
		{
			iCount++;

//			pszNext	= CharNextExA( GetCodePage(), pszPrev, 0 );
			pszNext	= GetCharNext( pszPrev, bEnterExist );
			if( ( pszNext == pszPrev )|| ( pszNext - pszBaseStr == iPtrPos ) )	break;

			// 개행 문자 체크
//			if( *pszPrev == '\n' || *pszPrev == '\r' )
//			{
//				if( *pszNext == '\n' || *pszNext == '\r' )	pszNext++;
//			}

			if( pszNext - pszBaseStr > iPtrPos )		// 범주를 넘어갔을 경우
			{
				if( !bGetUpper )		iCount--;
				break;
			}
			pszPrev = pszNext;
		}
	}
	// 왼쪽으로 검색이 갈 경우
	else
	{
		while(1)
		{
			iCount--;
//			pszNext	= CharPrevExA( GetCodePage(), pszFirstStr, pszPrev, 0 );
			pszNext	= GetCharPrev( pszFirstStr, pszPrev, bEnterExist );
			if( ( pszNext == pszPrev )|| ( pszNext - pszBaseStr == iPtrPos ) )	break;

			// 개행 문자 체크
//			if( *pszPrev == '\n' || *pszPrev == '\r' )
//			{
//				if( pszNext > pszFirstStr && ( *pszNext == '\n' || *pszNext == '\r' ) )	pszNext--;
//			}
			
			if( pszNext - pszBaseStr < iPtrPos )		// 범주를 넘어갔을 경우
			{
				if( !bGetUpper )		iCount--;
				break;
			}
			pszPrev = pszNext;
		}
	}
	return iCount;
}

//-----------------------------------------------------------------------------
// Name: GetCharLen()												[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 문자열에 포함된 문자수를 구한다.
//		pszSrcStr	- 검색하고자 하는 NULL이 아닌 스트링 버퍼 포인터
//		return		- 주어진 문자열에 들어있는 문자수 리턴, 에러 발생시 -1 리턴
//-----------------------------------------------------------------------------
int		OnlineEditControlEx::GetCharLen( char *pszSrcStr )
{
	if( !pszSrcStr )		return -1;
	if( !(*pszSrcStr) )		return 0;

	return GetCharCountByStrPtr( pszSrcStr, NULL, strlen( pszSrcStr ) );
}

//-----------------------------------------------------------------------------
// Name: GetCharNext()												[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 문자열의 다음단에 걸리는 글자를 리턴한다.
//	개행문자 그룹은 한덩이로 취급한다.
//		pszSrcStr	- 검색하고자 하는 NULL이 아닌 스트링 버퍼 포인터
//		bReturnExist- 이번 작업이 개행문자 처리였다면 true, 아니라면 false
//		return		- 주어진 조건에 만족하는 포인터 리턴
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetCharNext( char *pszSrcStr, BOOL &bReturnExist )
{
	if( !pszSrcStr )		return NULL;

	bReturnExist	= false;
	char	*pNext	= CharNextExA( GetCodePage(), pszSrcStr, 0 );

	// 여기서 개행문자가 올 경우 그 한덩어리를 한 문자로 취급한다.
	if( *pszSrcStr == '\r' && *pNext == '\n' )							{ pNext++; bReturnExist = true; }

	return pNext;
}

//-----------------------------------------------------------------------------
// Name: GetCharPrev()												[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 문자열의 앞단에 걸리는 글자를 받는다.
//	개행문자 그룹은 한덩이로 취급한다.
//		pszFirstStr	- 검색하고자 하는 NULL이 아닌 스트링 버퍼 포인터
//		pszSrcStr	- 우측 검색의 기준이 될 FirstStr내 포인터
//		bReturnExist- 이번 작업이 개행문자 처리였다면 true, 아니라면 false
//		return		- 주어진 조건에 만족하는 포인터 리턴
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetCharPrev( char *pszFirstStr, char *pszSrcStr, BOOL &bReturnExist )
{
	if( !pszFirstStr || !pszSrcStr )		return NULL;

	bReturnExist	= false;
	char	*pPrev	= CharPrevExA( GetCodePage(), pszFirstStr, pszSrcStr, 0 );

	// 여기서 개행문자가 올 경우 그 한덩어리를 한 문자로 취급한다.
	if( pPrev > pszFirstStr && *pPrev == '\n' && *( pPrev - 1 )== '\r' )	{ pPrev--; bReturnExist = true; }

	return pPrev;
}

//-----------------------------------------------------------------------------
// Name: GetStringPixelSize()										[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 문자열의 앞단과 끝단까지의 픽셀 거리를 구한다.
//		hDC				- 해당 HDC
//		pszSrc			- 길이를 구할 스트링
//		iCount			- 얼마만큼의 문자열을 계산할 것인가? -1일 경우 끝라인까지 계산
//		width, height	- 픽셀 길이를 받아올 버퍼
//		bCountIsChar	- iCount가 문자 갯수 단위일 경우 true, 바이트 단위일 경우 false (기본값 false)
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::GetStringPixelSize( HDC hDC, char *pszSrc, int iCount, int &width, int &height, BOOL bCountIsChar )
{
	width = height = 0;
	if( !hDC || !pszSrc )		return;

	// 카운트가 음수일 경우 끝라인까지의 픽셀 거리를 알려달라는 의미다.
	if( iCount < 0 )
	{
		iCount	= strlen( pszSrc );
	}
	else
	{
		if( bCountIsChar )	iCount = GetStrPtrByCharPos( pszSrc, NULL, iCount ) - pszSrc;
	}
	
	
	SIZE	size;
	GetTextExtentPoint32( hDC, pszSrc, iCount, &size );

	width	= size.cx;
	height	= size.cy;
}

//-----------------------------------------------------------------------------
// Name: GetSingleLineOfMultiStr()									[static]
// Code: actdoll (2004-04-17)
// Desc: 도구함수
//	주어진 다중 라인 문자열에서 문장을 끊을 데를 찾아서 끊어준다.
//	iDestWidth의 길이 미만으로 한 줄을 잘라주던가 개행문자(\n, \r등)가 있을 경우 
//	그것까지 포함하여 글자를 잘라준다.
//		hDC				- 해당 HDC
//		pszSrcStr		- 원본 스트링
//		iDestWidth		- 글자를 자를 픽셀 범위(1이상)
//		iLenPerLine		- 주어진 조건식에 맞춰 들어갈 수 있는 바이트 갯수를 리턴한다.
//		iCountPerLine	- 주어진 조건식에 맞춰 들어갈 수 있는 문자 갯수를 리턴한다.
//		return			- 원본 스트링에서 잘린 문자 범위의 다음 포인터를 리턴한다.
//						문장의 끝까지 간 경우나 문제가 발생했을 경우 NULL을 리턴한다.
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetSingleLineOfMultiStr( HDC hDC, char *pszSrcStr, int iDestWidth, int &iLenPerLine, int &iCountPerLine, BOOL &bIncludeReturn )
{
	iCountPerLine = iLenPerLine = 0;
	bIncludeReturn				= false;

	if( !hDC || !pszSrcStr )		return NULL;
	if( iDestWidth <= 0 )			return NULL;
	if( pszSrcStr[0] == 0 )			return NULL;

	SIZE	size;
	char	*pRet, *pSrc;
	pSrc	= pszSrcStr;
	while( !bIncludeReturn )
	{
		pRet	= GetCharNext( pSrc, bIncludeReturn );					// 다음 문자 포인터를 받고
		if( pSrc == pRet )						return NULL;			// 다음문자 포인터가 기존과 같다면 끝에 도달했다.

		GetTextExtentPoint32( hDC, pszSrcStr, pRet - pszSrcStr - ((bIncludeReturn)?2:0), &size );	// 실제 문자만 들어있는 사이즈를 가지고 계산
		if( size.cx > iDestWidth )	break;

		// 그렇지 않다면 계속 추적해간다.
		iLenPerLine += pRet - pSrc;										// 해당 문자의 바이트 크기를 첨가하고
		iCountPerLine++;
		pSrc = pRet;
	}

	return pSrc;
}
