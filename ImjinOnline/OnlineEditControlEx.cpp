//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ����Ʈ �ڽ� ��Ʈ�� Ȯ����
//	File Name		: OnlineEditControlEx.cpp
//	Birth Date		: 2004. 04. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �� ����Ʈ �ڽ��� ���� ��Ʈ���� �����ϴ� Ŭ�����̴�.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: �Ӽ� ���� �÷���
//-----------------------------------------------------------------------------
typedef enum EN_FLAG_OEC_ATTRLINE
{
	EN_FLAG_OEC_ATTRLINE_DOT,		// ����		- �Է� ��
	EN_FLAG_OEC_ATTRLINE_THICK,		// ������	- ��ȯ ��
	EN_FLAG_OEC_ATTRLINE_THIN,		// ������	- ��ȯ ��� ��
	EN_FLAG_OEC_ATTRLINE_MAX
};

HWND	OnlineEditControlEx::m_hMainWnd							= NULL;
BOOL	OnlineEditControlEx::m_bIsCompTypeMultiChar				= false;
HPEN	OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_MAX]	= {0,};
HFONT	OnlineEditControlEx::m_hCandidateFont					= NULL;

//-----------------------------------------------------------------------------
// Name: OnlineEditControlEx()
// Code: actdoll (2004-04-17)
// Desc: ������
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
// Desc: �Ҹ���
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
// Desc: ���� �����Ǿ� �ִ� �Է±⿡ ���� ���� ������ �ʱ�ȭ�Ѵ�.
//		bClearBackUps	- ����� ��ü�� ���ַ��� true, �� ���� ���۸� ������ false (�⺻�� false)
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

	// ���� �� Ŭ������ ��ġ���̶�� 0�� ��� �־��ش�.
	if( IsTypeOnlyNumeric() )	SetValue( 0 );
	else						UpdateShowBuffer();
}

//-----------------------------------------------------------------------------
// Name: SetFocus()
// Code: actdoll (2004-04-17)
// Desc: ��Ŀ���� ���� ���� ����� ó���� ����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetFocus()
{
	// IME �ý����� Ŭ�����Ų��.
	GS_IMEBase::Clear();

	// �н�����/�����Է�/1����Ʈ ���� �Է� ���� �� �ϳ����
	if( IsTypePassword() || IsTypeOnlyNumeric() || IsTypeOnlyAscii() )	GS_IMEBase::SetImeDeviceOn( false );
	// �׷��� �ʴٸ� �Է±� ���� ���·� �����Ѵ�.
	else																GS_IMEBase::SetImeDeviceOn( true );

	SetRecvInputChar( true );
	SetCursorShow	( true );
	SetActive		( true );
}

//-----------------------------------------------------------------------------
// Name: SetFocus()
// Code: actdoll (2004-04-17)
// Desc: ��Ŀ���� ���Լ� ����� ����� ó���� ����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::KillFocus()
{
	// �������� ��Ȳ�� ��� �ϴ� ������ �ϰ��Ų��.
	GS_IMEBase::SetCompStatusDoComplete();
	// IME �ý����� Ŭ�����Ų��.
	GS_IMEBase::Clear();
	// IME�� �Է±� ������ �� ���´�.
	GS_IMEBase::SetImeDeviceOn( false );

	SetActive		( false );
	SetRecvInputChar( false );
	SetCursorShow	( false );
}

//-----------------------------------------------------------------------------
// Name: IsPushEnter()
// Code: actdoll (2004-04-17)
// Desc: ������ ����
//	��� ���� ���͸� �������� �ȴ��������� üũ�Ѵ�. �� �Լ��� ȣ���� ��
//	���Ϳ� ���õ� ��ȣ�� false�� ���ư���.
//		return		- ���͸� �����ٸ� true, �ƴ϶�� false
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
// Desc: ������ ����
//	��� ���� ��Ű�� �������� �ȴ��������� üũ�Ѵ�. �� �Լ��� ȣ���� ��
//	�ǰ� ���õ� ��ȣ�� false�� ���ư���.
//		return		- ���� �����ٸ� true, �ƴ϶�� false
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
// Desc: Ŀ���� ��ġ�� ����ִ� �Լ��̴�. �Ƚ� ��Ʈ�� ���¸� �������� �Ѵ�.
//		iPos	- Ŀ���� ��Ʈ�� ������
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
// Desc: ���� ���������� ��ġ�� ����ִ� �Լ��̴�. ���� ���¸� �������� �Ѵ�.
//		iPos	- Ŀ���� ���� ���� ������
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
// Desc: �÷��� ����
//	������ ����Ʈ �ڽ��� ���� �Ӽ��� �����ϴ� �Լ���.
//		dwFlag	- DF_FLAG_OEC_TYPE_...�� ������ �÷���
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::SetTypeOn( DWORD dwFlag )
{
	if( dwFlag & DF_FLAG_OEC_TYPE_MULTILINE )													dwFlag = DF_FLAG_OEC_TYPE_MULTILINE;	// �� �÷��װ��� ����ִٸ� ���� ���� �÷��׵��� �� �����.
	if( dwFlag & DF_FLAG_OEC_TYPE_PASSWORD )													dwFlag |= DF_FLAG_OEC_TYPE_ONLYASCII;	// �� �÷��װ��� ����ִٸ� �ƽ�Ű���� �߰��Ѵ�.
	if( ( dwFlag & DF_FLAG_OEC_TYPE_ONLYNUMERIC ) && ( dwFlag & DF_FLAG_OEC_TYPE_ONLYASCII ) )	dwFlag &= ~DF_FLAG_OEC_TYPE_ONLYASCII;	// �� �÷��װ� ����ִٸ� �ƽ�Ű���� �� ������.

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
// Desc: ������ ����
//	������ �ؽ�Ʈ�� �����Ѵ�. ���� �̰��� ����� ��� ���� ���̴� ��Ʈ������ �������� �ȴ�.
//	������ ���� ���� ���õ� �÷��װ� ���� �������� ���� �� �����Ƿ� ������ ���ؼ� �ؾ� �Ѵ�.
//	��ġ �Է� ���뿡���� �� �Լ��� �������� �ʴ´�.
//		pszString	- ������ ��Ʈ�� ��
//		return		- ����� �ھҴٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::SetString( char *pszString )
{
	if( IsTypeOnlyNumeric() )			return FALSE;
	if( !pszString )					return FALSE;
	
	// ���� ����
	int		iLen	= strlen( pszString );
	if( iLen > m_iLenLimit )			iLen = m_iLenLimit;
	ZeroMemory( m_pszString, sizeof(m_pszString) );
	strncpy( m_pszString, pszString, iLen );

	// ��Ÿ ���� ����
	m_iLenString			= iLen;																				// ���� ���� �ް�
	m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// ���� ���� �ް�
	m_iCaretAsAnsi			= m_iLenString;																		// Ŀ���� �� ������ ���߰�
	m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// ���ڿ� ������ Ŀ�� ����
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// ��� ���� ����
	UpdateShowBuffer();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetValue()
// Code: actdoll (2004-04-17)
// Desc: ������ ����
//	������ �ؽ�Ʈ�� �־��� ��ġ�� �����Ѵ�. ���� �̰��� ����� ��� ���� ���̴� ��Ʈ������ �������� �ȴ�.
//	������ ���� ���� ���õ� �÷��װ� ���� �������� ���� �� �����Ƿ� ������ ���ؼ� �ؾ� �Ѵ�.
//	��� �������� ����� �� �ִ�.
//		i64Value	- �Է��� ��ġ��
//		return		- ����� �ھҴٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::SetValue( LONGLONG i64Value )
{
	if( i64Value < 0 )						i64Value = 0;
	else if( i64Value > m_i64ValueLimit )	i64Value = m_i64ValueLimit;

	// ���� ����
	ZeroMemory( m_pszString, sizeof(m_pszString) );
	_i64toa( i64Value, m_pszString, 10 );
	int	iLen	= strlen( m_pszString );
	if( iLen > m_iLenLimit )	iLen = m_iLenLimit;
		
	strncpy( m_pszString, m_pszString, iLen );

	// ��Ÿ ���� ����
	m_iLenString			= iLen;																				// ���� ���� �ް�
	m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// ���� ���� �ް�
	m_iCaretAsAnsi			= m_iLenString;																		// Ŀ���� �� ������ ���߰�
	m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// ���ڿ� ������ Ŀ�� ����
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// ��� ���� ����
	UpdateShowBuffer();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: SetValue()
// Code: actdoll (2004-04-17)
// Desc: ������ ���
//	���� �ؽ�Ʈ�� ��ġ�� ��ȯ�Ͽ� ���� �����ش�.
//	��ġ �Է� ���� ���������� �� �� �ִ�.
//		return	- int64�� ��ġ
//-----------------------------------------------------------------------------
LONGLONG	OnlineEditControlEx::GetValue()
{
	if( !IsTypeOnlyNumeric() || m_iLenString <= 0 )		return 0L;

	return _atoi64( m_pszString );
}

//-----------------------------------------------------------------------------
// Name: ChangeCaretNext()
// Code: actdoll (2004-04-17)
// Desc: ��Ƽ���ο� �����ϴ� Ŀ���� �������� ��Ʈ���� �����̳� ���ٷ� �̵��Ѵ�.
//		bSetNext		- ������ �����ٷ� �ѱ� ���̶�� true, �ƴ϶�� false
//		bSetFirstAndLast- ������ ���� ��Ʈ�Ӹ��� �Դٰ����� ���̶�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::ChangeCaretNext( BOOL bSetNext, BOOL bSetFirstAndLast )
{
	if( !IsTypeMultiLine() )			return;

	// �ϴ� �۾� ũ�⸦ �˻��ϱ� ���� HDC�� ȣ���Ѵ�.
	HDC		hDC		= GetDC( m_hMainWnd );
	if( !hDC )		return;

	if( m_hFont )	::SelectObject( hDC, m_hFont );

	// ���� Ŀ���� ��ġ�� ������ �� ���� �޴´�.
	int		iLenPerLine, iCharPerLine;
	BOOL	bIsReturn;
	char	*pCurrent, *pPrev, *pNext;
	char	*pCaretPos		= GetStrPtrByCharPos( m_pszString, NULL, m_iCaretAsChar );

	BOOL	bBreak	= false;
	pCurrent		= pPrev	= m_pszString;
	while(1)
	{
		// �� ���� �˻��ϰ�
		pNext	= GetSingleLineOfMultiStr( hDC, pCurrent, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
		
		// ������ ���� �˻��ϰ� �ִٸ� ����
		if( !pNext )										break;

		// �����ִ� ������ ã���� ��� Ŀ���� �ִ� ù��° ���ڸ� �ް� ����
		if( pCaretPos >= pCurrent && pCaretPos < pNext )	break;

		pPrev		= pCurrent;
		pCurrent	= pNext;
	}

	// ���� �׳� �� ���� ���� ù���ڸ� �Դٰ��� �ϴ� ���̶�� �׷��� �����ش�.
	if( bSetFirstAndLast )
	{
		// ���� ���ڸ��� �����ش�.
		if( bSetNext )
		{
			if( bIsReturn || ( !bIsReturn && pNext ) )
			{
				iLenPerLine		-= 2;	// ���๮�ڰ� �ִٸ� ���� �α��ڸ� �����ش�.
				iCharPerLine	--;		// ���ڴ� �ϳ��� �����.
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

	// Ŀ���� �����ִ� ���ڿ������� ���� ������ �˾Ƴ��´�.
	int		iRangeFromFirst	= pCaretPos - pCurrent;
	
	// �����ٷ� �̵��ϴ� ���̶��
	if( bSetNext )	
	{
		// �̵� ������ �������
		if( pNext != NULL && pCurrent < pNext )	
		{
			GetSingleLineOfMultiStr( hDC, pNext, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
			if( iRangeFromFirst > iLenPerLine )					iRangeFromFirst = iLenPerLine;	// ���� ������ �Ѿ ��� �����ְ�
			if( iRangeFromFirst == iLenPerLine && bIsReturn )	iRangeFromFirst -= 2;			// ���๮�ڰ� �ִٸ� ���� �α��ڸ� �����ش�.
			
			SetCaretPosAsAnsi( ( pNext - m_pszString ) + iRangeFromFirst );
		}
		// ���̻� �̵��� �Ұ����ϴٸ� ���ڸ��� �����ش�.
		else
		{
			m_iCaretAsAnsi	= m_iLenString;
			m_iCaretAsChar	= m_iCountChar;
		}
	}
	// �����ٷ� �̵��ϴ� ���̶��
	else
	{
		// �ٿ� ���̰� �־��ٸ�
		if( pPrev < pCurrent && pPrev >= m_pszString )	
		{
			GetSingleLineOfMultiStr( hDC, pPrev, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharPerLine, bIsReturn );
			if( iRangeFromFirst > iLenPerLine )					iRangeFromFirst = iLenPerLine;	// ���� ������ �Ѿ ��� �����ְ�
			if( iRangeFromFirst == iLenPerLine && bIsReturn )	iRangeFromFirst -= 2;			// ���๮�ڰ� �ִٸ� ���� �α��ڸ� �����ش�.
			
			SetCaretPosAsAnsi( ( pPrev - m_pszString ) + iRangeFromFirst );
		}
		// ���̻� �̵��� �Ұ����ϴٸ� ���ڸ��� �����ش�.
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
// Desc: ���� ��Ʈ���� ��� ��Ʈ���� ����ִ´�.
//		bSetNext	- ���� �� �������� �������� �ѱ�� �ʹٸ� true, �������� �ѱ�� �ʹٸ� false
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
// Desc: ��� ��Ʈ���� �ִ� �����͸� ���� ��Ʈ���� ����ִ´�.
//		bGetNext	- ���� �ٿ��� ���� ���� �����͸� �ް� �ʹٸ� true, ���� ���� �����͸� �ް� �ʹٸ� false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::ChangeFromBackUpToString( BOOL bGetNext )
{
	if( !IsTypeBackUpString() )			return;
	if( IsTypePassword() )				return;

	int		i;
	BOOL	bExist = false;
	// �����ٷ� �ѱ�°�?
	if( bGetNext )		
	{
		( m_iBackUpLineNo >= DF_VALUE_OEC_MAX_BACKUP_LINE - 1 ) ? m_iBackUpLineNo = 0 : m_iBackUpLineNo++;	// ���� ����
		// ������ ���κ��� �������� ��Ʈ���� ����ִ��� Ȯ���Ѵ�.
		for( i=m_iBackUpLineNo; i<DF_VALUE_OEC_MAX_BACKUP_LINE; i++ )
		{
			if( strlen( m_pszBackUpString[i] ) != 0 ) 
			{
				m_iBackUpLineNo	= i;
				bExist			= true; 
				break; 
			}
		}
		// ������ ���µ��� ��ã�Ҵٸ� ���ʵ� ã�ƺ���.
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
	// �����ٷ� �ѱ�°�?
	else
	{
		( m_iBackUpLineNo > 0 ) ? m_iBackUpLineNo-- : m_iBackUpLineNo = DF_VALUE_OEC_MAX_BACKUP_LINE - 1;	// ���� ����
		// ������ ���κ��� �������� ��Ʈ���� ����ִ��� Ȯ���Ѵ�.
		for( i=m_iBackUpLineNo; i>=0; i-- )
		{
			if( strlen( m_pszBackUpString[i] ) != 0 ) 
			{
				m_iBackUpLineNo	= i;
				bExist			= true; 
				break; 
			}
		}
		// ������ ���µ��� ��ã�Ҵٸ� ���ʵ� ã�ƺ���.
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
	
	// ������ ��� �����ϰ� �ٽ� ����ش�.
	GS_IMEBase::Clear();
	ZeroMemory	( m_pszString, sizeof(m_pszString) );
	strcpy		( m_pszString, m_pszBackUpString[m_iBackUpLineNo] );
	
	// ��Ÿ ���õ�
	m_iLenString	= strlen( m_pszString );
	m_iCountChar	= OnlineEditControlEx::GetCharLen( m_pszString );
	m_iCaretAsAnsi			= 0;
	m_iCaretAsChar			= 0;
	m_iCompCaretAsAnsi		= 0;
	m_iCompCaretAsChar		= 0;

	// ��� ���� ����
	UpdateShowBuffer();
}

//-----------------------------------------------------------------------------
// Name: Draw()
// Code: actdoll (2004-04-17)
// Desc: ��Ʈ���� ����Ѵ�.
//		hDC			- �� ������ DC
//		prcDest		- ��� ������ ������ �ִ� RECT�� ������
//		bRightAlign	- true�̸� ������ ���ķ� ���, false�̸� ���� ���ķ� ��� (�⺻�� true)
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Draw( HDC hDC, BOOL bRightAlign )
{
	if( !hDC )							return FALSE;
	if( !m_bShow )						return FALSE;

	static	DWORD	dwLatestTime	= 0;
	static	BOOL	bCaretOn		= true;

	// ������Ʈ�� ��Ʈ ������ �־��ش�.
	if( m_hFont )	::SelectObject( hDC, m_hFont );

	// Ŀ�� ������ ����
	if( GetTickCount() - dwLatestTime > 200 )
	{
		dwLatestTime	= GetTickCount();
		bCaretOn		= !bCaretOn;
	}
	
	// ���� �Ӽ� ���� ���ٸ� ������
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT]	= CreatePen( PS_DOT,	1, RGB( 125, 125, 125 ) );
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK]	= CreatePen( PS_SOLID,	3, RGB( 250, 250, 250 ) );
	if( !OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] )		OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN]	= CreatePen( PS_SOLID,	1, RGB( 250, 250, 250 ) );

	// ������ ���� �� ���ϰ�
	int		w, h, iDistHeightCaret;
	OnlineEditControlEx::GetStringPixelSize( hDC, "1", 1, w, h );
	

	// ���� ������ ���
	if( !IsTypeMultiLine() )
	{
		// ����
		UpdateShowRangeForSingleLine( hDC );		// ��ġ ����
		
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

		// ���ڰ� ������ �����ϴ� ���̸� ���Ѵ�.
		iDistHeightCaret	= ( m_rcDestOfDraw.bottom - m_rcDestOfDraw.top - h ) >> 1;
		
		// �Ӽ���(��ü �߱��� Ȥ�� �Ϻ���� ����)
		if( m_bActive && GS_IMEBase::IsIMEComposing() && !GS_IMEBase::IsLocaleKorean() )
		{
			// ���̱� �����ϴ� ��ġ�������� �ۼ��Ѵ�.
			int iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd;
			UpdateAttrForSingleLine( hDC, iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd );

			// ���� ��ȯ ��⼱���� �׸���.
			HPEN	hPen;
			hPen	= (HPEN)SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] );
			if( iThinStart >= 0 && iThinEnd >= 0 )
			{
				MoveToEx( hDC, m_rcDestOfDraw.left + iThinStart,	m_rcDestOfDraw.top + iDistHeightCaret + h, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iThinEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h );
			}
			// ������ ��ȯ���� �׸���.
			if( iThickStart >= 0 && iThickEnd >= 0 )
			{
				SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] );
				MoveToEx( hDC, m_rcDestOfDraw.left + iThickStart,	m_rcDestOfDraw.top + iDistHeightCaret + h + 1, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iThickEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h + 1 );
			}
			// ������ �Է¼��� �׸���.
			if( iDotStart >= 0 && iDotEnd >= 0 )
			{
				SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] );
				MoveToEx( hDC, m_rcDestOfDraw.left + iDotStart,		m_rcDestOfDraw.top + iDistHeightCaret + h, NULL );
				LineTo	( hDC, m_rcDestOfDraw.left + iDotEnd,		m_rcDestOfDraw.top + iDistHeightCaret + h );
			}
			SelectObject( hDC, hPen );
		}

		// Ŀ��
		if( bCaretOn && !m_bHideCursor )
		{
			int		iCaretWidth, iCaretHeight;
			OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iCaretAsChar + m_iCompCaretAsChar - m_iFirstCharOfDraw, iCaretWidth, iCaretHeight, true );
			iCaretHeight = m_rcDestOfDraw.top + iDistHeightCaret;
			if( !IsTypeOnlyNumeric() )	TextOut( hDC, m_rcDestOfDraw.left + iCaretWidth - 2, iCaretHeight, "|", 1 );
			else						TextOut( hDC, m_rcDestOfDraw.right + 2, iCaretHeight, "|", 1 );

			// actdoll (2004/11/04 21:17) : ����â�� ���� ��ǥ ����
			m_ptCaretDrawPos.x = m_rcDestOfDraw.left + iCaretWidth - 2;
			m_ptCaretDrawPos.y = iCaretHeight;
		}
	}
	// ���� ������ ���
	else
	{
		// ���� �Է��� ���� �� �ִ� ��Ȳ�̶��
		if( m_bCanRecvInputChar )
		{
			char	*pFirstLine, *pLastLine, *pNextLine, *pCaretPos;
			int		iCurrentCaretPos;
			int		iLen	= 0, iCharCount	= 0, iLineCount = 0;
			int		iLineTop = m_rcDestOfDraw.top;
			BOOL	bIncludeReturn;


			// ��ġ ����
			UpdateShowRangeForMultiLine( hDC );

			// ������� Ŀ�� ��ġ ����س���
			iCurrentCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
			if( iCurrentCaretPos < 0 )		iCurrentCaretPos = 0;

			// ������� ������ ��ġ ����س���
			pFirstLine	= GetStrPtrByCharPos( m_pszShowBuffer, NULL, m_iFirstCharOfDraw );					// ���� ���� ù���� ��ġ
			pLastLine	= GetStrPtrByCharPos( pFirstLine, NULL, m_iLastCharOfDraw - m_iFirstCharOfDraw );	// ������ ���� ù���� ��ġ
			pCaretPos	= GetStrPtrByCharPos( pFirstLine, NULL, iCurrentCaretPos - m_iFirstCharOfDraw );	// Ŀ�� ��ġ

			while( pFirstLine && pFirstLine <= pLastLine )
			{
				// �� ���� �����ؼ� ���´�.
				pNextLine	= GetSingleLineOfMultiStr( hDC, pFirstLine, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLen, iCharCount, bIncludeReturn );
				if( bIncludeReturn )	
				{
//					iLen = OnlineEditControlEx::GetStrPtrByCharPos( pFirstLine, NULL, iCharCount - 1 ) - pFirstLine;
					iLen		-= 2;
				}

				// ����
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

				// �Ӽ���
				if( m_bActive && GS_IMEBase::IsIMEComposing() && !GS_IMEBase::IsLocaleKorean() )
				{
					// ���̱� �����ϴ� ��ġ�������� �ۼ��Ѵ�.
					int iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd;
					UpdateAttrForMultiLine( hDC, pFirstLine, iLen, iDotStart, iDotEnd, iThinStart, iThinEnd, iThickStart, iThickEnd );

					// ���� ��ȯ ��⼱���� �׸���.
					HPEN	hPen;
					hPen	= (HPEN)SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THIN] );
					if( iThinStart >= 0 && iThinEnd >= 0 )
					{
						MoveToEx( hDC, m_rcDestOfDraw.left + iThinStart,	iLineTop + h, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iThinEnd,		iLineTop + h );
					}
					// ������ ��ȯ���� �׸���.
					if( iThickStart >= 0 && iThickEnd >= 0 )
					{
						SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_THICK] );
						MoveToEx( hDC, m_rcDestOfDraw.left + iThickStart,	iLineTop + h + 1, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iThickEnd,		iLineTop + h + 1 );
					}
					// ������ �Է¼��� �׸���.
					if( iDotStart >= 0 && iDotEnd >= 0 )
					{
						SelectObject( hDC, OnlineEditControlEx::m_hPen[EN_FLAG_OEC_ATTRLINE_DOT] );
						MoveToEx( hDC, m_rcDestOfDraw.left + iDotStart,		iLineTop + h, NULL );
						LineTo	( hDC, m_rcDestOfDraw.left + iDotEnd,		iLineTop + h );
					}
					SelectObject( hDC, hPen );
				}

				// Ŀ��
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

					// actdoll (2004/11/04 21:17) : ����â�� ���� ��ǥ ����
					m_ptCaretDrawPos.x = m_rcDestOfDraw.left + iCaretWidth - 2;
					m_ptCaretDrawPos.y = iLineTop;
				}


				// ���� ����
				iLineTop	+= h;
				iLineCount	++;
				pFirstLine	= pNextLine;
			}

		}
		// �Է� �޴� ���� �Ұ����� ��Ȳ�̶��
		else
		{
			// �׸��� ����ְ�
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
// Desc: ���� �޽����� �޾� ó���ϴ� �κ��̴�.
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
				// ���� ���� ���� ���ڰ� ���߹���Ʈ ������ ��� WM_CHAR���� �ش� Ƚ���� ���� ó������ �ʴ´�.
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
// Desc: ���� �޽��� - ���� ����
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_StartComposition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	GS_IMEBase::Check_WM_IME_STARTCOMPOSITION( lParam );
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_StartComposition()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - ���� ����
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_EndComposition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	// �Ϻ��� �������� ��
	if( OnlineEditControlEx::IsLanguageForJapanese() )
	{
		// �����߿� ���ᰡ �������ٴ� ���� ��ҵǾ����� �ǹ��Ѵ�.
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
// Desc: ���� �޽��� - ���� ��
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_Composition( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// ���� �����̴�!
	GS_IMEBase::Check_WM_IME_COMPOSITION_Start( wParam, lParam );

	// �ϼ� ���°� ��������!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_ResultString	( lParam, m_iLenLimit - m_iLenString ) )	// �̷��� �صθ� ���ڴ� �ݵ�� ���� �������θ� �޴´�.
	{
		// ���ڿ� üũ
		char	*pszToken;
		int		iRet	= GS_IMEBase::GetResultStringLen();					// �޾ƿ� ��Ʈ���� ũ�⸦ �ް�
		
		// �� ���ڿ��� ����(���� ���۸� �ӽ÷� Ȱ��)
		ZeroMemory	( m_pszShowBuffer,						sizeof(m_pszShowBuffer) );
		strcpy		( m_pszShowBuffer,						m_pszString + m_iCaretAsAnsi );				// �޿��� �����س���
		strncpy		( m_pszString + m_iCaretAsAnsi,			GS_IMEBase::GetResultStringData(),	iRet	);	// �� ���̿� �ϼ��� ���ڿ��� �޴´�.
		// actdoll (2004/08/12 11:47) : ���⼭ ���! �ϼ��� ���ڿ��� �Բ� ������ ö�ڰ� �� �� �����Ƿ� �װ��� Ȯ���ϵ��� �Ѵ�.
		pszToken	= m_pszString + m_iCaretAsAnsi;
		while( *pszToken )
		{
			// ���ڰ� ��Ƽ����Ʈ ���ڶ�� �ϴ� ��ŵ
			if( IsDBCSLeadByteEx( GetCodePage(), (BYTE)*pszToken ) )	{ pszToken = CharNextExA( GetCodePage(), pszToken, 0 ); continue; }
			// �Ƚù��ڶ�� �˻�
			if( IsExceptionChar( (BYTE)*pszToken ) )	// ���� ���ڸ� �߰��ߴ�
			{
				strcpy( pszToken, pszToken + 1 );
				iRet--;
				continue;
			}
			// ������ ����ƴٸ� ���� �ѱ��.
			pszToken++;
		}
		strcpy		( m_pszString + m_iCaretAsAnsi + iRet,	m_pszShowBuffer );							// �Ĺ濡 �����͸� ���δ�.

		// ��Ÿ ������ ����
		m_iLenString			= strlen( m_pszString );															// ��Ʈ�� ������ �Է�
		m_iCountChar			= OnlineEditControlEx::GetCharLen( m_pszString );										// ���� ���ڼ� Ȯ��
		m_iCaretAsAnsi			+= iRet;																			// Ŀ�� ����
		m_iCaretAsChar			= OnlineEditControlEx::GetCharCountByStrPtr( m_pszString, NULL, m_iCaretAsAnsi );	// ���ڿ� ������ Ŀ�� ����
		m_iCompCaretAsAnsi		= 0;
		m_iCompCaretAsChar		= 0;

		// ��� ���� ����
		UpdateShowBuffer();
	}

	// ���� ���°� ��������!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompString	( lParam, m_iLenLimit - m_iLenString ) )
	{
		// ���ڿ� üũ
		int		iRet	= GS_IMEBase::GetComposingStringLen();						// �޾ƿ� ��Ʈ���� ũ�⸦ �ް�

		// �� ���ڿ��� ����
		ZeroMemory	( m_pszShowBuffer,							sizeof(m_pszShowBuffer) );
		strncpy		( m_pszShowBuffer,							m_pszString,							m_iCaretAsAnsi	);	// ���� ���ڿ��� �� ���ڿ� ����
		strncpy		( m_pszShowBuffer + m_iCaretAsAnsi,			GS_IMEBase::GetComposingStringData(),	iRet			);	// ���� ���ڿ� �Է�
		strcpy		( m_pszShowBuffer + m_iCaretAsAnsi + iRet,	m_pszString + m_iCaretAsAnsi							);	// �� �ڿ� ������ ���� ���ڿ� ����

		// ��Ÿ ������ ����
		m_iCompCaretAsAnsi	= iRet;
		m_iCompCaretAsChar	= OnlineEditControlEx::GetCharLen( (char*)GS_IMEBase::GetComposingStringData() );

		// ��� ���� ����
		UpdateShowBuffer( true );
	}

	// ���� �Ӽ��� ��������!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompAttribute	( lParam, m_iLenLimit - m_iLenString ) )
	{
		memset( m_pszShowAttr, -1, sizeof(m_pszShowAttr) );
		CopyMemory( m_pszShowAttr + m_iCaretAsAnsi, GS_IMEBase::GetComposingAttrData(), GS_IMEBase::GetComposingStringLen() );
	}

	// ���� ������ Ŀ�� ��ġ�� ��������!
	if( GS_IMEBase::Check_WM_IME_COMPOSITION_CompCursor	( lParam, m_iLenLimit - m_iLenString ) )
	{
		m_iCompCaretAsAnsi	= GS_IMEBase::GetComposingCaretPos();
		m_iCompCaretAsChar	= OnlineEditControlEx::GetCharCountByStrPtr( (char*)GS_IMEBase::GetComposingStringData(), NULL, m_iCompCaretAsAnsi );
	}

	// ���� ������!
	GS_IMEBase::Check_WM_IME_COMPOSITION_Finish( wParam, lParam );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_Notify()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - IME��ȣ ������
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_Notify( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	GS_IMEBase::Check_WM_IME_NOTIFY( wParam, lParam );

	// �н�����/�����Է�/1����Ʈ ���� �Է� ���� �� �ϳ����
	if( IsTypePassword() || IsTypeOnlyNumeric() || IsTypeOnlyAscii() )	GS_IMEBase::SetImeDeviceOn( false );
	// �׷��� �ʴٸ� �Է±� ���� ���·� �����Ѵ�.
	else																GS_IMEBase::SetImeDeviceOn( true );

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_AnsiChar()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - ���� ����
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_AnsiChar( HWND hWnd, WPARAM wParam )
{
	// ���� ��尡 �ƴ� ��� ���� ���� ���߹���Ʈ �ڵ尡 �ִٸ� ������
	if( m_iCount_NoProcessAsAscii > 0 )
	{
		m_iCount_NoProcessAsAscii--;
		if( m_iCount_NoProcessAsAscii < 0 )		m_iCount_NoProcessAsAscii = 0;
		return FALSE;
	}
	

	// ���� �� ���� �����̰ų� ���� ���� �ѱ�ų� ���ѵ� ������ ��� ����
	if( !m_bCanRecvInputChar )							return FALSE;
	// ���ǿ� �ִ� �ƽ�Ű���� ���Դ�.
	if( wParam < 0x20 || wParam > 0x7e )
	{
		if( wParam != '\n' && wParam != '\r' )			return FALSE;	// ���๮�ڱ��� �ƴ϶�� ����
		else if( !IsTypeMultiLine() )					return FALSE;	// ��Ƽ���� Ÿ���� �ƴ϶�� ����
		else if( m_iLenString + 2 > m_iLenLimit )		return FALSE;	// ���๮�ڰ� �� ������ ���ٸ� ����
	}
	if( IsExceptionChar( (BYTE)wParam )	)				return FALSE;
	if( m_iLenString >= m_iLenLimit )					return FALSE;

	// ���� �Է� ������ ��
	if( IsTypeOnlyNumeric() )
	{
		if( wParam < 0x0030 || wParam > 0x0039 )	return FALSE;	// ���ڰ� �ƴ϶�� ����
		// �� ���ڸ��� 0�� ���
		if( m_pszString[0] == '0' )
		{
			// 0�� �ٽ� ���Դٸ� ���� ����
			if( wParam == 0x0030 )					return FALSE;
			// �׷��� �ʴٸ� ���� ��ġ��� ����
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

	// ���۸� �����Ѵ�.(�ӽ÷� ��� ���۸� ���)
	ZeroMemory	( m_pszShowBuffer, sizeof(m_pszShowBuffer) );
	strcpy		( m_pszShowBuffer,						m_pszString + m_iCaretAsAnsi );	// ���� �� ����

	// ���� ���� �ƴҶ��� �׳� ���̰�
	if( wParam != '\n' && wParam != '\r' )
	{
		m_pszString[m_iCaretAsAnsi] = (char)wParam;											// ���� ����ִ´�.
		strcpy		( m_pszString + m_iCaretAsAnsi + 1,		m_pszShowBuffer );				// ������ ���� ����
	}
	// ���� �����϶��� '\r\n'�� ���δ�
	else
	{
		m_pszString[m_iCaretAsAnsi]		= '\r';
		m_pszString[m_iCaretAsAnsi + 1] = '\n';
		strcpy		( m_pszString + m_iCaretAsAnsi + 2,		m_pszShowBuffer );
	}

	// ���� �Է� �� ���� �ٽ� ���
	if( IsTypeOnlyNumeric() )
	{
		LONGLONG	i64Value = _atoi64( m_pszString );
		if( i64Value > m_i64ValueLimit )
		{
			SetValue( i64Value );
			return TRUE;
		}
	}

//	strncpy		( m_pszShowBuffer,						m_pszString,	m_iCaretAsAnsi );	// ���� �� ����
//	m_pszShowBuffer[m_iCaretAsAnsi] = (char)wParam;											// �ܾ� ����ִ´�.
//	strcpy		( m_pszShowBuffer + m_iCaretAsAnsi + 1, pCurPtr );							// ������ ���� ����
//	strcpy		( m_pszString,							m_pszShowBuffer );					// �������� �ٽ� ����
	
	// �޸�����
	if( wParam != '\n' && wParam != '\r' )
	{
		m_iLenString++;		
		m_iCaretAsAnsi++;
	}
	else
	{
		m_iLenString	+= 2;	// ��Ʈ�� ������ ����
		m_iCaretAsAnsi	+= 2;	// Ŀ�� ����
	}
	m_iCountChar++;		// ���� ���ڼ� Ȯ��
	m_iCaretAsChar++;	// ���ڿ� ������ Ŀ�� ����

	// ��� ���� ����
	UpdateShowBuffer();
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_Move()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - Ű Ŀ�� ���� �޽��� ������
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Move( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )			return FALSE;

	// ���� �Է� ���¿����� Ŀ��Ű�� �� �� ����
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
// Desc: ���� �޽��� - Ű ���� - Delete
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Delete( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// ���ǿ� �ȸ��� �� ����
	if( wParam != VK_DELETE )									return FALSE;
	if( m_iLenString <= 0 || m_iCaretAsAnsi == m_iLenString )	return FALSE;

	// ������ Ŀ������ ���� ���ڸ� �޴´�.
	BOOL	bReturnExist;
	char	*pNewPtr = OnlineEditControlEx::GetCharNext( m_pszString + m_iCaretAsAnsi, bReturnExist );

	// ���۸� �����Ѵ�.
	strcpy( m_pszString + m_iCaretAsAnsi, pNewPtr );

	// �޸�����
	m_iLenString		= strlen( m_pszString );	// ��Ʈ�� ������ �Է�
	m_iCountChar		= OnlineEditControlEx::GetCharLen( m_pszString );	// ���� ���ڼ� Ȯ��

	// ��� ���� ����
	UpdateShowBuffer();
	
	return TRUE;	
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_BackSpace()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - Ű ���� - BackSpace
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_BackSpace( HWND hWnd, WPARAM wParam )
{
	if( !m_bCanRecvInputChar )					return FALSE;

	// ���ǿ� �ȸ��� �� ����
	if( wParam != VK_BACK )										return FALSE;
	if( m_iLenString <= 0 || m_iCaretAsAnsi <= 0 )				return FALSE;

	// ������ Ŀ������ ���� ��ġ�� �޴´�.
	BOOL	bReturnExist;
	char	*pNewPtr	= OnlineEditControlEx::GetCharPrev( m_pszString, m_pszString + m_iCaretAsAnsi, bReturnExist );

	// ���۸� �����Ѵ�.
	strcpy( pNewPtr, m_pszString + m_iCaretAsAnsi );

	// ��ġ�� �Է¹޴� �����̸� �۾��� �ϳ��� �ȳ����� ���� 0(�ѱ���)�� �������ش�.
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
		// �޸�����
		m_iLenString	= strlen( m_pszString );		// ��Ʈ�� ������ �Է�
		if( m_iCountChar > 0 )		m_iCountChar--;		// ���� ���ڼ� Ȯ��
		m_iCaretAsAnsi		= pNewPtr - m_pszString;	// Ŀ�� ����
		if( m_iCaretAsChar > 0 )	m_iCaretAsChar--;;	// ���ڿ� ������ Ŀ�� ����
		m_iCompCaretAsAnsi	= 0;
		m_iCompCaretAsChar	= 0;
	}

	// ��� ���� ����
	UpdateShowBuffer();
	
	return TRUE;	
}

//-----------------------------------------------------------------------------
// Name: Proc_WindowMsg_KeyDown_Enter()
// Code: actdoll (2004-04-17)
// Desc: ���� �޽��� - Ű ���� - ENTER, TAB
//-----------------------------------------------------------------------------
BOOL	OnlineEditControlEx::Proc_WindowMsg_KeyDown_Enter( HWND hWnd, WPARAM wParam )
{
	// ���ǿ� �ȸ��� �� ����
	if( wParam != VK_RETURN && wParam != VK_TAB )				return FALSE;
	// �߱��� ��/��ü ����98������ �����߿� ����Ű�� �� ��� �����Ѵ�.
	if( GS_IMEBase::IsLocaleSimpChinese() || GS_IMEBase::IsLocaleTradChinese() )
	{
		if( GS_IMEBase::IsIMEComposing() )						return FALSE;
	}

	// ���� ������ ���
	if( wParam == VK_RETURN )
	{
		if( IsTypeSingleLine() )
		{
			if( IsTypeBackUpString() )	ChangeFromStringToBackUp();	// ��� ��Ʈ���� ����ϰ� �ִٸ� ���ڸ� ����ض�
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
// Desc: ���� ��Ʈ���� ���� ��� ��Ʈ���� ������Ʈ �Ѵ�.
//		���� �ܺο��� m_pszShowBuffer�� �ǵ���� ��� bNotTouchShowBuffer�� true�� �ָ� �ȴ�.
//		bNotTouchShowBuffer	- m_pszShowBuffer�� �ǵ帮�� ���� ���̸� true, ������ ���� �����Ѵٸ� false
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowBuffer( BOOL bNotTouchShowBuffer )
{
	if( !bNotTouchShowBuffer )
	{
		ZeroMemory( m_pszShowBuffer, sizeof(m_pszShowBuffer) );
		
		// �н����� Ÿ���� ���
		if( IsTypePassword() )
		{
			memset( m_pszShowBuffer, '*', m_iLenString );	// ���� ��Ʈ�� ������ ��ŭ ��ǥ�� ���δ�.
		}
		// ��ġ�� �Է¹޴� Ÿ���� ���
		else if( IsTypeOnlyNumeric() )
		{
			char	*pDest, *pSrc;
			pSrc	= m_pszString;					// ������ �ް�
			pDest	= m_pszShowBuffer;				// ������ �ް�
			int		iValue	= m_iLenString / 3;		// 3������ ���� ���� �޴´�.
			int		iRemain	= m_iLenString % 3;		// 3������ ���� �������� �޴´�.

			// �ڸ����� ������� �� �ִ� ��Ȳ�� ���
			while( *pSrc != NULL )	
			{
				// �������� 0�� ��� 3������ ������
				if( iRemain == 0 ) 
				{
					iValue--;
					strncpy( pDest, pSrc, 3 );
					pDest += 3, pSrc += 3;
					if( iValue > 0 )				{ *pDest = ','; pDest++; }
				}
				// �������� 0�� �ƴ� ��� �ش� ������ �ѹ��� ������.
				else
				{
					strncpy( pDest, pSrc, iRemain );
					pDest += iRemain, pSrc += iRemain;
					if( iValue >= 1 && iRemain > 0 ){ *pDest = ','; pDest++; }
					iRemain = 0;
				}
			}
		}
		// �� ���� ���� ���縸 �Ѵ�.
		else
		{
			strcpy( m_pszShowBuffer, m_pszString );
		}
	}

	// ��Ÿ ���� ����
	m_iLenShowBuffer		= strlen( m_pszShowBuffer );						// ���� �ް�
	m_iCountCharShowBuffer	= OnlineEditControlEx::GetCharLen( m_pszShowBuffer );	// ���� �ް�
}

//-----------------------------------------------------------------------------
// Name: UpdateShowRangeForSingleLine()								[private]
// Code: actdoll (2004-04-17)
// Desc: ��� ��Ʈ���� ������ �� ������ �����Ѵ�.
//	���⿡�� m_iFirstCharOfDraw�� m_iLastCharOfDraw�� �������� �Ǵ� ������ ��ġ�� ���Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowRangeForSingleLine( HDC hDC )
{
	if( !hDC )		return;

	// ���� ��� ���ۿ� ����ִ� ��Ʈ�� ���� �������� ������ ���Ѵ�.
	char	*pStart, *pCaret, *pLast;
	int		iLineWidth, iLineHeight, iCaretPos, iLimitWidth, iMoveRange;

	// �þ� ���� ���� ����
	if( m_iLastCharOfDraw > m_iCountCharShowBuffer )	m_iLastCharOfDraw	= m_iCountCharShowBuffer;
	if( m_iFirstCharOfDraw > m_iLastCharOfDraw - 1 )	m_iFirstCharOfDraw	= m_iLastCharOfDraw - 3;
	if( m_iFirstCharOfDraw < 0 )						m_iFirstCharOfDraw	= 0;

	// �ϴ� ��� ���� ��ġ������ ���� �������� �Ÿ��� �������� ������ �ǵ�
	iCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
	iLimitWidth = m_rcDestOfDraw.right - m_rcDestOfDraw.left;
	pStart		= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	m_iFirstCharOfDraw );
	pLast		= GetStrPtrByCharPos( m_pszShowBuffer,	pStart,	m_iLastCharOfDraw - m_iFirstCharOfDraw );

	// �������� ������ ũ�⿡ ���� �� �۴ٸ� ��� �ѹ� �ٽ� �Ѵ�.
	OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iLastCharOfDraw - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );
	while( iLineWidth < iLimitWidth && m_iLastCharOfDraw < m_iCountCharShowBuffer )
	{
		// ���ڸ� �ϳ��� �߰��Ͽ� ū�� ��ū�� ���캻��.
		OnlineEditControlEx::GetStringPixelSize( hDC, pStart, m_iLastCharOfDraw + 1 - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );
		if( iLineWidth < iLimitWidth && m_iLastCharOfDraw < m_iCountCharShowBuffer )	m_iLastCharOfDraw++;
	}
	
	// ���� ��� ���� ������ ���� Ŀ���� ������ ���� ���
	if( m_iLastCharOfDraw <= iCaretPos )	
	{
		// �ϴ� ������ ������ ��ġ���� ����� �ڷ� ������.
		iMoveRange	= iCaretPos - m_iLastCharOfDraw;
		if( iMoveRange < 3 )	iMoveRange = 3;
		iCaretPos	+= iMoveRange;
		if( iCaretPos > m_iCountCharShowBuffer )	iCaretPos = m_iCountCharShowBuffer;

		// ������ ���۹��ڿ��� ���ο� �����ڱ����� �ȼ��� ���
		BOOL	bReturnExist;
		while(1)
		{
			OnlineEditControlEx::GetStringPixelSize( hDC, pStart, iCaretPos - m_iFirstCharOfDraw, iLineWidth, iLineHeight, true );

			// ���ڿ��� ���� ũ�ٸ� ��� �ٿ�������.
			if( iLineWidth < iLimitWidth || iCaretPos - m_iFirstCharOfDraw <= 0 )	break;
			pStart = OnlineEditControlEx::GetCharNext( pStart, bReturnExist );
			m_iFirstCharOfDraw++;
		}

		// �̵��� ���� �־��ش�.
		m_iLastCharOfDraw	= iCaretPos;
	}
	// ���� ��� ���� ���� ���� Ŀ���� ������ Ŭ ��� �̵�
	else if( m_iFirstCharOfDraw >= iCaretPos )	
	{
		// �ϴ� ������ ������ ��ġ���� ����� �ڷ� ������.
		iMoveRange	= m_iFirstCharOfDraw - iCaretPos;
		if( iMoveRange < 3 )	iMoveRange = 3;
		iCaretPos	-= iMoveRange;
		if( iCaretPos < 0 )	iCaretPos = 0;

		// ���ο� ���۹��ڿ��� ���� �����ڱ����� �ȼ��� ���
		pCaret		= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	iCaretPos );
		while(1)
		{
			OnlineEditControlEx::GetStringPixelSize( hDC, pCaret, m_iLastCharOfDraw - iCaretPos, iLineWidth, iLineHeight, true );

			// ���ڿ��� ���� ũ�ٸ� ��� �ٿ�������.
			if( iLineWidth < iLimitWidth || m_iLastCharOfDraw - iCaretPos <= 0 )	break;
			m_iLastCharOfDraw--;
		}

		// �̵��� ���� �־��ش�.
		m_iFirstCharOfDraw	= iCaretPos;
	}
}

//-----------------------------------------------------------------------------
// Name: UpdateShowRangeForMultiLine()								[private]
// Code: actdoll (2004-04-17)
// Desc: ��� ��Ʈ���� ������ �� ������ �����Ѵ�.
//	���⿡�� m_iFirstCharOfDraw�� m_iLastCharOfDraw�� 
//	�������� �Ǵ� ù��° ������ �� �� ��ġ�� ������ ������ �� �� ��ġ�� ���Ѵ�.
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateShowRangeForMultiLine( HDC hDC )
{
	if( !hDC )		return;

	// ���� ��� ���ۿ� ����ִ� ��Ʈ�� ���� �������� ������ ���Ѵ�.
	char	*pSrc, *pNext;
	int		iMaxShowLineCount, iLineWidth, iLineHeight, iCaretPos;
	int		iFirstLineNo, iLastLineNo, iCaretLineNo, iCaretLineCharCount;
	int		iLenPerLine = 0, iCharCountPerLine = 0, iCharCount = 0, iLineCount = 0, iAllLineCount = 0;
	BOOL	bIncludeReturn;

	// ���� ������ ���س��´�.
	OnlineEditControlEx::GetStringPixelSize( hDC, "1", 1, iLineWidth, iLineHeight, false );						// �ϴ� ���ڴ� ���̸� ���ϰ�
	iMaxShowLineCount = ( iLineHeight )	?	( m_rcDestOfDraw.bottom - m_rcDestOfDraw.top ) / iLineHeight + 1: 10;	// �� �ڽ����� �ѹ��� ���� �� �ִ� ���� ������ ���ϰ�

	// �� ���μ��� ���Ѵ�.
	pSrc	= m_pszShowBuffer;
	while( pSrc ){ pSrc	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn ); iAllLineCount++; }
	iCaretPos	= m_iCaretAsChar + m_iCompCaretAsChar;
	
	// �þ� ���� ���� ����
	if( m_iLastCharOfDraw > m_iCountCharShowBuffer )	m_iLastCharOfDraw	= m_iCountCharShowBuffer;
	if( m_iFirstCharOfDraw >= m_iLastCharOfDraw )		m_iFirstCharOfDraw	= m_iLastCharOfDraw;
	if( m_iFirstCharOfDraw < 0 )						m_iFirstCharOfDraw	= 0;

	// ���� ������ ����Ѵ�.
	pSrc	= m_pszShowBuffer;
	while( pSrc )
	{
		// �� ���� ���ϰ�
		pNext	= GetSingleLineOfMultiStr( hDC, pSrc, m_rcDestOfDraw.right - m_rcDestOfDraw.left, iLenPerLine, iCharCountPerLine, bIncludeReturn );

		// �������� ���� ù��°�� ã�´�.
		if( m_iFirstCharOfDraw >= iCharCount	&& m_iFirstCharOfDraw < iCharCount + iCharCountPerLine )	
		{
			iFirstLineNo		= iLineCount;
			m_iFirstCharOfDraw	= iCharCount;

			// �ϴ� ù��° ���� �������� ������ �� �ִ� ������ ���� ���μ��� ���� ���´�.
			iLastLineNo			= iFirstLineNo + iMaxShowLineCount - 1;
			if( iLastLineNo >= iAllLineCount )	iLastLineNo = iAllLineCount - 1;
		}

		// �������� ���� �������� ã�´�.
		if( iLastLineNo == iLineCount )
//		if( m_iLastCharOfDraw >= iCharCount		&& m_iLastCharOfDraw < iCharCount + iCharCountPerLine )		
		{
//			iLastLineLen		= pSrc - m_pszShowBuffer;
			m_iLastCharOfDraw	= iCharCount;
		}

		// Ŀ���� �� ���ֳ��� ������ Ȯ���ߴٸ� Ŀ�� �� Ȯ��
		if( iCaretPos >= iCharCount				&& iCaretPos < iCharCount + iCharCountPerLine )			
		{
			iCaretLineNo		= iLineCount;
//			iCaretLineLen		= pSrc - m_pszShowBuffer;
			iCaretLineCharCount	= iCharCount;
		}
		// ���ֳ��� ������ ������ �� ������ ���� ��쵵 Ȯ��
		else if( !pNext && iCaretPos >= m_iCountCharShowBuffer )
		{
			iCaretLineNo		= iLineCount;
//			iCaretLineLen		= pSrc - m_pszShowBuffer;
			iCaretLineCharCount	= iCharCount;
		}

		// �����ٷ� �Ѿ��.
		iLineCount	++;
		iCharCount	+= iCharCountPerLine;
		pSrc		= pNext;
	}

	iCharCount = 0;
	// �������ٺ��� ĳ���� �Ʒ� ���ִٸ�
	if( iCaretLineNo > iLastLineNo )
	{
		// �ϴ� ������ ���� ĳ�� �ٷ� �����ְ�
		iLastLineNo			= iCaretLineNo;
//		iLastLineLen		= iCaretLineLen;
		m_iLastCharOfDraw	= iCaretLineCharCount;

		// ù���� �ٽ� ������ش�.
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
	// ù�ٺ��� ĳ���� ���� ���ִٸ� ĳ�� �ٷ� �����ش�.
	else if( iCaretLineNo < iFirstLineNo )	
	{
		iFirstLineNo		= iCaretLineNo;
//		iFirstLineLen		= iCaretLineLen;
		m_iFirstCharOfDraw	= iCaretLineCharCount;

		// ���������� �ٽ� ������ش�.
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
// Desc: �Ӽ� ��¿��� ������ ���� ������ �� ������ ����Ѵ�.
//		hDC						- ���� DC��
//		iDotStart, iDotEnd,		- ��Ʈ ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//		iThinStart, iThinEnd	- ���¼� ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//		 iThickStart, iThickEnd	- ������ ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateAttrForSingleLine( HDC hDC, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd )
{
	iDotStart = iDotEnd = iThinStart = iThinEnd = iThickStart = iThickEnd = -1;
	if( !hDC )		return;

	// �ʿ� ���� ����
	char	*pCompStart, *pCompEnd, *pDrawStartString, *pAttrStart, *pAttrEnd, *pDrawStartAttr, *pAttrInput;
	int		iTemp, iPosFromFront, iShowRange, iDrawDistFromStart, iDrawDistFromAttr;
	iShowRange			= m_rcDestOfDraw.right - m_rcDestOfDraw.left;							// ����Ʈ �ڽ��� ����

	pCompStart			= m_pszShowBuffer + m_iCaretAsAnsi;										// ���� ���۵� ��ġ
	pCompEnd			= pCompStart + GS_IMEBase::GetComposingStringLen();					// ���� ������ ��ġ
	pDrawStartString	= GetStrPtrByCharPos( m_pszShowBuffer,	NULL,	m_iFirstCharOfDraw );	// ���ڿ��� �׷����� �����ϴ� ��ġ
	iDrawDistFromStart	= pCompStart - pDrawStartString;										// ���� ���� ��ġ�� ���ڿ��� �׷����� �����ϴ� ��ġ�� ����

	// ���� ��ġ �˻�
	( iDrawDistFromStart > 0 )	? iPosFromFront	= iDrawDistFromStart : iPosFromFront	= 0;		// ���� �׷����� ������ ���� ��ġ�� ã�´�. ���濡 ���� ��� �� �տ��� �׸��� �ǹǷ� 0�� ����� �ȴ�.
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iPosFromFront, iThinStart, iTemp );	// ���� ���� �ȼ� ��ġ �Ϸ�

	// ���� ��ġ �˻�
	int		iLen = ( pCompStart > pDrawStartString ) ? pCompEnd - pCompStart : pCompEnd - pDrawStartString;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iLen, iThinEnd, iTemp );	// �������� ���ڿ��� �ȼ����̸� ���Ѵ�.
	( iShowRange - iThinStart < iThinEnd ) ? iThinEnd = iShowRange : iThinEnd += iThinStart;							// �þ� ������ �˻��Ͽ� ���� ���� �ȼ� ��ġ �Ϸ�


	// ��Ÿ �Ӽ� �˻�
	pAttrStart			= m_pszShowAttr + m_iCaretAsAnsi;							// ���� ���۵� �Ӽ��� ��ġ
	pAttrEnd			= pAttrStart + GS_IMEBase::GetComposingStringLen();		// ���� ������ �Ӽ��� ��ġ
	pDrawStartAttr		= m_pszShowAttr + ( pDrawStartString - m_pszShowBuffer );	// ���� �Ӽ��� �׷����� �����ϴ� ��ġ
	
	// �Է����� �Ӽ��� ã�´�.
	int		iSizeStart, iSizeEnd;
	int		iAttrType = -1, iCountAttrInput = 0;
	char	*pTemp = pAttrInput = pAttrStart;
	while( pTemp - m_pszShowAttr < GS_IMEBase::GetComposingStringLen() || *pTemp != -1 )
	{
		// ��ȯ ��� �� ���� �ƴ� ���
		if( *pTemp < EN_FLAG_OEC_ATTRLINE_THIN )
		{
			// ó������ �߰��ߴ�
			if( iCountAttrInput <= 0 )
			{
				pAttrInput	= pTemp;	// ���۵� ������ �ް�
				iAttrType	= *pTemp;	// �Ӽ��� �ް�
			}
			iCountAttrInput++;
		}
		pTemp++;
	}
	// ���� �Է� ���ڰ� �޾����� �ִ� ������ ����Ѵ�.
	iDrawDistFromAttr	= pAttrInput - pDrawStartAttr;

	// ���� ��ġ �˻�
	( iDrawDistFromAttr > 0 )	? iPosFromFront	= iDrawDistFromAttr : iPosFromFront = 0;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iPosFromFront, iSizeStart, iTemp );				// ���� �������� �����δ� �������� �ȼ����� ���Ѵ�.

	// ���� ��ġ �˻�
	iLen = ( pAttrInput > pDrawStartAttr ) ? iCountAttrInput : ( pAttrInput + iCountAttrInput ) - pDrawStartAttr;
	OnlineEditControlEx::GetStringPixelSize( hDC, pDrawStartString, iLen, iSizeEnd, iTemp );		// ���� ���ڿ��� �ȼ�ũ�⸦ ���Ѵ�.
	( iShowRange - iSizeStart < iSizeEnd ) ? iSizeEnd = iShowRange : iSizeEnd += iSizeStart;

	// �Ӽ����� ���� �޸� ���� �����Ѵ�.
	if( iAttrType == EN_FLAG_OEC_ATTRLINE_THICK )	{ iThickStart	= iSizeStart;	iThickEnd	= iSizeEnd; }
	else											{ iDotStart		= iSizeStart;	iDotEnd		= iSizeEnd; }
}

//-----------------------------------------------------------------------------
// Name: UpdateAttrForSingleLine()											[private]
// Code: actdoll (2004-04-17)
// Desc: �Ӽ� ��¿��� ������ ���� ������ �� ������ ����Ѵ�.
//		hDC						- ���� DC��
//		iDotStart, iDotEnd,		- ��Ʈ ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//		iThinStart, iThinEnd	- ���¼� ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//		 iThickStart, iThickEnd	- ������ ǥ���� ���۰� ��. DC���� ���ų� �׸��� �ʾƵ� �ȴٸ� -1����
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::UpdateAttrForMultiLine( HDC hDC, char *pszFirstCharOfLine, int iLenOfLine, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd )
{
	iDotStart = iDotEnd = iThinStart = iThinEnd = iThickStart = iThickEnd = -1;
	
	if( !hDC )		return;
	if( pszFirstCharOfLine < m_pszShowBuffer )							return;
	else if( pszFirstCharOfLine > m_pszShowBuffer + m_iLenShowBuffer )	return;

	// �ʿ� ���� ����
	char	*pCompStart, *pCompEnd;
	int		iAttrFirstPos, iAttrLastPos;
	int		iShowRange;
	iShowRange			= m_rcDestOfDraw.right - m_rcDestOfDraw.left;							// ����Ʈ �ڽ��� ����

	pCompStart			= m_pszShowBuffer + m_iCaretAsAnsi;										// ���� ���۵� ��ġ
	pCompEnd			= pCompStart + GS_IMEBase::GetComposingStringLen();					// ���� ������ ��ġ

	// ���� ���� ��ġ�� ������ ����.
	if( pCompStart <= pszFirstCharOfLine )						iAttrFirstPos	= 0;	// �� �� ���� ���� �ξ� ������ ��ŸƮ ������ �ִٸ� �� �������� 0
	else if( pCompStart > pszFirstCharOfLine + iLenOfLine )		return;					// �� �� ������ �ξ� �ڿ� �������� �ִٸ� �׳� ����
	else																				// �׷��� �ʴٸ� �� ������ �������� �� �������� ��ġ�� ����س��´�.
	{
		iAttrFirstPos	= pCompStart - pszFirstCharOfLine;
	}

	// ���� ���� ��ġ�� ������ ����.
	if( pCompEnd < pszFirstCharOfLine )							return;							// �� �� ���� ���� �ξ� ������ ���� ������ �ִٸ� ����
	else if( pCompEnd >= pszFirstCharOfLine + iLenOfLine )		iAttrLastPos	= iLenOfLine;	// �� �� ������ �ξ� �ڿ� ���� ������ �ִٸ� �������� ���� ������
	else																						// �׷��� �ʴٸ� �� ������ �������� �� �������� ��ġ�� ����س��´�.
	{
		iAttrLastPos	= pCompEnd - pszFirstCharOfLine;
	}

	// �������� �ִٸ� �� ������ �м��غ���.
	int		i, iAttrType = -1, iCountAttrInput = 0;
	char	*pTemp, *pAttrInput = NULL;
	for( i = iAttrFirstPos; i<iAttrLastPos; i++ )
	{
		 pTemp = m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) + i;
		// ��ȯ ��� �� ���� �ƴ� ���
		if( *pTemp < EN_FLAG_OEC_ATTRLINE_THIN )
		{
			// ó������ �߰��ߴ�
			if( iCountAttrInput <= 0 )
			{
				pAttrInput	= pTemp;	// ���۵� ������ �ް�
				iAttrType	= *pTemp;	// �Ӽ��� �ް�
			}
			iCountAttrInput++;
		}
		pTemp++;
	}

	int		iWidth, iHeight, iWidth2, iHeight2;
	// ��ü Ǯ ������ ��ġ �˻�
	GetStringPixelSize( hDC, pszFirstCharOfLine, iAttrFirstPos, iThinStart, iHeight );
	GetStringPixelSize( hDC, pszFirstCharOfLine, iAttrLastPos, iThinEnd, iHeight );

	// Ư�� ���� �˻�
	if( pTemp )		// Ư�� ������ �־��ٸ�
	{
		GetStringPixelSize( hDC, pszFirstCharOfLine, pAttrInput - ( m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) ),						iWidth, iHeight );
		GetStringPixelSize( hDC, pszFirstCharOfLine, pAttrInput - ( m_pszShowAttr + ( pszFirstCharOfLine - m_pszShowBuffer ) ) + iCountAttrInput,	iWidth2, iHeight2 );

		// �Ӽ����� ���� �޸� ���� �����Ѵ�.
		if( iAttrType == EN_FLAG_OEC_ATTRLINE_THICK )	{ iThickStart	= iWidth;	iThickEnd	= iWidth2; }
		else											{ iDotStart		= iWidth;	iDotEnd		= iWidth2; }
	}
}

//-----------------------------------------------------------------------------
// Name: SetCodePage()									[static]
// Code: actdoll (2004-04-17)
// Desc: �����Լ�
//	���� Ŭ���̾�Ʈ�� ��� �ڵ� �������� �����Ѵ�. �ƿ﷯ ���� ������ ���չ����� ��� ����â�� ������ �� ���ΰ� �ƴѰ��� �����Ѵ�.
//		pszFirstStr	- �˻��ϰ��� �ϴ� NULL�� �ƴ� ��Ʈ�� ���� ������
//		pszBaseStr	- pszFirstStr�� �˻� ������ġ ������. �̰��� NULL�� ��� �� ���ʺ��� ã�´�.
//		iCharPos	- ���° ��ġ�� ������ �����͸� ã�� ������? -1�� ��� NULL���ڰ� ��ġ�� ������ ����
//		return		- �ش� ��°�� ������ ����, pszSrcStr�� NULL�̰ų� ������ ��� ��� NULL����
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
// Desc: �����Լ�
//	�־��� ���ڿ����� ���° ���ڿ� ���� ��ġ�� �䱸�� ��� �� ��ü�� �ش�Ǵ� �����͸� �ѱ��.
//		pszFirstStr	- �˻��ϰ��� �ϴ� NULL�� �ƴ� ��Ʈ�� ���� ������
//		pszBaseStr	- pszFirstStr�� �˻� ������ġ ������. �̰��� NULL�� ��� �� ���ʺ��� ã�´�.
//		iCharPos	- ���° ��ġ�� ������ �����͸� ã�� ������? -1�� ��� NULL���ڰ� ��ġ�� ������ ����
//		return		- �ش� ��°�� ������ ����, pszSrcStr�� NULL�̰ų� ������ ��� ��� NULL����
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetStrPtrByCharPos( char *pszFirstStr, char *pszBaseStr, int iCharPos )
{
	if( !pszFirstStr )									return NULL;

	// ���ǽ� ����
	if( !pszBaseStr || pszBaseStr - pszFirstStr < 0 )	pszBaseStr = pszFirstStr;
	if( iCharPos == 0 )									return pszBaseStr;


	// ���������� �˻��� ����.
	BOOL	bReturnExist;
	if( iCharPos > 0 )
	{
		while( pszBaseStr && iCharPos-- )	
		{
			// ���� ���๮�� �뿭�� �޾��� ��� �ϳ� �� �̵����ش�.
//			if( *pszBaseStr == '\n' || *pszBaseStr == '\r' )	
//			{
//				if( *( pszBaseStr + 1 ) == '\n' || *( pszBaseStr + 1 ) == '\r' )									pszBaseStr++;
//			}
//			pszBaseStr	= CharNextExA( GetCodePage(), pszBaseStr, 0 );
			pszBaseStr	= GetCharNext( pszBaseStr, bReturnExist );
		}
	}
	// �������� �˻��� ����.
	else
	{
		while( pszBaseStr && iCharPos++ )	
		{
			// ���� ���๮�� �뿭�� �޾��� ��� �ϳ� �� �̵����ش�.
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
// Desc: �����Լ�
//	���� �����ͷκ��� ���° �����Ϳ� ��ġ�� ���ڱ��� ��� ���ڵ��� �����ϴ����� �˷��ش�.
//	�������� ��ġ�� ���ָ� ��� ��� -1�� �����Ѵ�.
//	bGetUpper�� true�� ��� ���� ��°�� ī��Ʈ�� �ø��ϰ�, �׷��� ������ �����Ͽ� ����Ѵ�.
//		pszFirstStr	- �ڷ� �̵��� ���� NULL�� �ƴ� ���ι���. ��, ���ڿ� ���� ���� ���� ���� �������̴�.
//		pszBaseStr	- �˻��ϰ��� �ϴ� ��Ʈ�� ���� �������� ������ġ����. �� ���� NULL�� ��� pszFirstStr�� ������ �ȴ�.
//		iPtrPos		- pszBaseStr�κ��� ���° ��ġ�� �����͸� ã�� ���ΰ�?
//		bGetupper	- ������� ���� ������ ��� �ø��̶�� true, �����̶�� false(�⺻�� false)
//		return		- �־��� �������� ���ڹ�° ����, ���ǽ��� �ȸ°ų� ���ڸ� �״���� ��� 0 ����
//-----------------------------------------------------------------------------
int		OnlineEditControlEx::GetCharCountByStrPtr( char *pszFirstStr, char *pszBaseStr, int iPtrPos, BOOL bGetUpper )
{
	if( !pszFirstStr )										return 0;
	if( iPtrPos == 0 )										return 0;

	// ���ǽ� ����
	if( !pszBaseStr || pszBaseStr - pszFirstStr < 0 )				pszBaseStr = pszFirstStr;
	int	iLen = strlen( pszBaseStr );
	if( iPtrPos > iLen )											iPtrPos	= iLen;
	else if( iPtrPos < 0 && pszFirstStr - pszBaseStr > iPtrPos  )	iPtrPos = pszFirstStr - pszBaseStr;

	// �۾��� �˻��Ѵ�.
	int		iCount = 0;
	char	*pszPrev, *pszNext;
	pszPrev = pszNext = pszBaseStr;

	// ���������� �˻��� �� ���
	BOOL	bEnterExist;
	if( iPtrPos >= 0 )
	{
		while(1)
		{
			iCount++;

//			pszNext	= CharNextExA( GetCodePage(), pszPrev, 0 );
			pszNext	= GetCharNext( pszPrev, bEnterExist );
			if( ( pszNext == pszPrev )|| ( pszNext - pszBaseStr == iPtrPos ) )	break;

			// ���� ���� üũ
//			if( *pszPrev == '\n' || *pszPrev == '\r' )
//			{
//				if( *pszNext == '\n' || *pszNext == '\r' )	pszNext++;
//			}

			if( pszNext - pszBaseStr > iPtrPos )		// ���ָ� �Ѿ�� ���
			{
				if( !bGetUpper )		iCount--;
				break;
			}
			pszPrev = pszNext;
		}
	}
	// �������� �˻��� �� ���
	else
	{
		while(1)
		{
			iCount--;
//			pszNext	= CharPrevExA( GetCodePage(), pszFirstStr, pszPrev, 0 );
			pszNext	= GetCharPrev( pszFirstStr, pszPrev, bEnterExist );
			if( ( pszNext == pszPrev )|| ( pszNext - pszBaseStr == iPtrPos ) )	break;

			// ���� ���� üũ
//			if( *pszPrev == '\n' || *pszPrev == '\r' )
//			{
//				if( pszNext > pszFirstStr && ( *pszNext == '\n' || *pszNext == '\r' ) )	pszNext--;
//			}
			
			if( pszNext - pszBaseStr < iPtrPos )		// ���ָ� �Ѿ�� ���
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
// Desc: �����Լ�
//	�־��� ���ڿ��� ���Ե� ���ڼ��� ���Ѵ�.
//		pszSrcStr	- �˻��ϰ��� �ϴ� NULL�� �ƴ� ��Ʈ�� ���� ������
//		return		- �־��� ���ڿ��� ����ִ� ���ڼ� ����, ���� �߻��� -1 ����
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
// Desc: �����Լ�
//	�־��� ���ڿ��� �����ܿ� �ɸ��� ���ڸ� �����Ѵ�.
//	���๮�� �׷��� �ѵ��̷� ����Ѵ�.
//		pszSrcStr	- �˻��ϰ��� �ϴ� NULL�� �ƴ� ��Ʈ�� ���� ������
//		bReturnExist- �̹� �۾��� ���๮�� ó�����ٸ� true, �ƴ϶�� false
//		return		- �־��� ���ǿ� �����ϴ� ������ ����
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetCharNext( char *pszSrcStr, BOOL &bReturnExist )
{
	if( !pszSrcStr )		return NULL;

	bReturnExist	= false;
	char	*pNext	= CharNextExA( GetCodePage(), pszSrcStr, 0 );

	// ���⼭ ���๮�ڰ� �� ��� �� �ѵ���� �� ���ڷ� ����Ѵ�.
	if( *pszSrcStr == '\r' && *pNext == '\n' )							{ pNext++; bReturnExist = true; }

	return pNext;
}

//-----------------------------------------------------------------------------
// Name: GetCharPrev()												[static]
// Code: actdoll (2004-04-17)
// Desc: �����Լ�
//	�־��� ���ڿ��� �մܿ� �ɸ��� ���ڸ� �޴´�.
//	���๮�� �׷��� �ѵ��̷� ����Ѵ�.
//		pszFirstStr	- �˻��ϰ��� �ϴ� NULL�� �ƴ� ��Ʈ�� ���� ������
//		pszSrcStr	- ���� �˻��� ������ �� FirstStr�� ������
//		bReturnExist- �̹� �۾��� ���๮�� ó�����ٸ� true, �ƴ϶�� false
//		return		- �־��� ���ǿ� �����ϴ� ������ ����
//-----------------------------------------------------------------------------
char*	OnlineEditControlEx::GetCharPrev( char *pszFirstStr, char *pszSrcStr, BOOL &bReturnExist )
{
	if( !pszFirstStr || !pszSrcStr )		return NULL;

	bReturnExist	= false;
	char	*pPrev	= CharPrevExA( GetCodePage(), pszFirstStr, pszSrcStr, 0 );

	// ���⼭ ���๮�ڰ� �� ��� �� �ѵ���� �� ���ڷ� ����Ѵ�.
	if( pPrev > pszFirstStr && *pPrev == '\n' && *( pPrev - 1 )== '\r' )	{ pPrev--; bReturnExist = true; }

	return pPrev;
}

//-----------------------------------------------------------------------------
// Name: GetStringPixelSize()										[static]
// Code: actdoll (2004-04-17)
// Desc: �����Լ�
//	�־��� ���ڿ��� �մܰ� ���ܱ����� �ȼ� �Ÿ��� ���Ѵ�.
//		hDC				- �ش� HDC
//		pszSrc			- ���̸� ���� ��Ʈ��
//		iCount			- �󸶸�ŭ�� ���ڿ��� ����� ���ΰ�? -1�� ��� �����α��� ���
//		width, height	- �ȼ� ���̸� �޾ƿ� ����
//		bCountIsChar	- iCount�� ���� ���� ������ ��� true, ����Ʈ ������ ��� false (�⺻�� false)
//-----------------------------------------------------------------------------
void	OnlineEditControlEx::GetStringPixelSize( HDC hDC, char *pszSrc, int iCount, int &width, int &height, BOOL bCountIsChar )
{
	width = height = 0;
	if( !hDC || !pszSrc )		return;

	// ī��Ʈ�� ������ ��� �����α����� �ȼ� �Ÿ��� �˷��޶�� �ǹ̴�.
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
// Desc: �����Լ�
//	�־��� ���� ���� ���ڿ����� ������ ���� ���� ã�Ƽ� �����ش�.
//	iDestWidth�� ���� �̸����� �� ���� �߶��ִ��� ���๮��(\n, \r��)�� ���� ��� 
//	�װͱ��� �����Ͽ� ���ڸ� �߶��ش�.
//		hDC				- �ش� HDC
//		pszSrcStr		- ���� ��Ʈ��
//		iDestWidth		- ���ڸ� �ڸ� �ȼ� ����(1�̻�)
//		iLenPerLine		- �־��� ���ǽĿ� ���� �� �� �ִ� ����Ʈ ������ �����Ѵ�.
//		iCountPerLine	- �־��� ���ǽĿ� ���� �� �� �ִ� ���� ������ �����Ѵ�.
//		return			- ���� ��Ʈ������ �߸� ���� ������ ���� �����͸� �����Ѵ�.
//						������ ������ �� ��쳪 ������ �߻����� ��� NULL�� �����Ѵ�.
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
		pRet	= GetCharNext( pSrc, bIncludeReturn );					// ���� ���� �����͸� �ް�
		if( pSrc == pRet )						return NULL;			// �������� �����Ͱ� ������ ���ٸ� ���� �����ߴ�.

		GetTextExtentPoint32( hDC, pszSrcStr, pRet - pszSrcStr - ((bIncludeReturn)?2:0), &size );	// ���� ���ڸ� ����ִ� ����� ������ ���
		if( size.cx > iDestWidth )	break;

		// �׷��� �ʴٸ� ��� �����ذ���.
		iLenPerLine += pRet - pSrc;										// �ش� ������ ����Ʈ ũ�⸦ ÷���ϰ�
		iCountPerLine++;
		pSrc = pRet;
	}

	return pSrc;
}
