//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ����Ʈ �ڽ� ��Ʈ�� Ȯ����
//	File Name		: Handler_OnlineEditControlEx.cpp
//	Birth Date		: 2004. 05. 10.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		����Ʈ ��Ʈ�ѿ� ���� ������ �����ϴ� �����Լ��̴�. �� �Լ��� ���α׷� ������ �ϳ��ۿ� ���ٴ� ������ �д�.
//
//===================================================================================================

#include <GSL.h>
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"


HWND					Handler_OnlineEditControlEx::m_hMainWnd							= NULL;
OnlineEditControlEx		Handler_OnlineEditControlEx::m_clOec[DF_VALUE_MAX_COUNT_OEC];
HOEC					Handler_OnlineEditControlEx::m_hFocusedOec						= NULL;
UINT					Handler_OnlineEditControlEx::m_uiUsedCount						= 0;
BOOL					Handler_OnlineEditControlEx::m_bPushedShift						= false;

Handler_OnlineEditControlEx		g_clHandle_OecEx;					// �ʱ�ȭ��

//-----------------------------------------------------------------------------
// Name: Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: ������
//-----------------------------------------------------------------------------
Handler_OnlineEditControlEx::Handler_OnlineEditControlEx()
{

}

//-----------------------------------------------------------------------------
// Name: ~Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
Handler_OnlineEditControlEx::~Handler_OnlineEditControlEx()
{

}

//-----------------------------------------------------------------------------
// Name: ~Handler_OnlineEditControlEx()
// Code: actdoll (2004-05-10)
// Desc: �ʱ�ȭ(1ȸ��)
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
// Desc: ��Ʈ���� �������ش�. �̰����� ������ ���Ѿ� ��Ʈ���� ����� ���۽�ų �� �ִ�.
//		dwOecType			- ������� ��Ʈ���� �Ӽ�( DF_FLAG_OEC_TYPE_ ... )
//		prcDraw				- �ش� ��Ʈ���� �׷����� ����
//		iMaxInputLen		- �ִ� �Է� ���ڼ�(�ι��ڰ� ����)
//		i64MaxInputValue	- ��ġ�Է� �Ӽ�(DF_FLAG_OEC_TYPE_ONLYNUMERIC)������ �ִ밪
//		return				- ������ ��Ʈ���� �ڵ�, ��Ʈ�� ������ �Ѱ迡 �����ϰų� ������ ��� NULL
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::CreateControl( DWORD dwOecType, LPRECT prcDraw, int iMaxInputLen, LONGLONG i64MaxInputValue )
{
	if( m_uiUsedCount >= DF_VALUE_MAX_COUNT_OEC )	return NULL;		// ī��Ʈ�� ������ ���ö󰣴�.
	if( !prcDraw )									return NULL;		// ������ �� �ȿ��� ���ö󰣴�.

	HOEC	hOec = NULL;
	for( int i=0; i<DF_VALUE_MAX_COUNT_OEC; i++ )
	{
		// ����ִ� �ڵ��� ã�Ҵٸ� �־��
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
// Desc: ��Ʈ���� �����Ѵ�.
//		hOec	- ������� �ϴ� ��Ʈ���� �ڵ�
//		return	- ���������� ���������� true, ������ �ִٸ� false
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
		MessageBox( NULL, "�������� ���� �ڵ��Դϴ�.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
#endif//_DEBUG
	
	OnlineEditControlEx	*pOecEx	= (OnlineEditControlEx*)hOec;
	// ���� ��Ŀ���� �����ϴ� �ڵ��� ��� ���� �ڵ�� ���� �ڵ鿡�� �� �ڵ�� ���� ��Ŀ���� �����Ѵٸ� �����ش�.
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
// Desc: �ش� ��Ʈ�ѿ��� ���� ������ ��� ���� ��Ŀ���� ������ ��Ʈ���� �����Ѵ�.
//		hCurrentOec	- ���� ��Ʈ�� �ڵ�
//		hNextOec	- ���� ��Ʈ���� ������ ������ ��Ʈ��
//		return	- ���������� ���������� true, ������ �ִٸ� false
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
// Desc: �ش� ��Ʈ�ѿ� ��Ŀ���� �����ش�.
//		hOec	- ��Ŀ���� ���߷��� ��Ʈ�� �ڵ�
//		return	- ���������� ���������� true, ������ �ִٸ� false
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

	// actdoll (2005/01/31 15:54) : �߱�/�븸 ����
	// ��Ŀ�� ����ÿ��� IME ����ְ� ����
	GS_IMEBase::SetImeDeviceOn( true );

	return hPrevOec;
}

//-----------------------------------------------------------------------------
// Name: ClearCurrentFocus()
// Code: actdoll (2004-05-10)
// Desc: �ش� ��Ʈ���� ��Ŀ���� ���δ�.
//		return	- ���������� ������ �ִ� ��Ŀ�� �ڵ�. �����ٸ� NULL����
//-----------------------------------------------------------------------------
HOEC	Handler_OnlineEditControlEx::ClearCurrentFocus()
{
	if( m_uiUsedCount <= 0 )	return NULL;
	if( !m_hFocusedOec )		return NULL;

	
	HOEC					hOec		= m_hFocusedOec;
	OnlineEditControlEx*	pOecEx		= (OnlineEditControlEx*)m_hFocusedOec;
	pOecEx->KillFocus();

	m_hFocusedOec	= NULL;

	// actdoll (2005/01/31 15:54) : �߱�/�븸 ����
	// ��Ŀ�� ����ÿ��� IME ����ְ� ����
	GS_IMEBase::SetImeDeviceOn( false );

	return hOec;
}

//-----------------------------------------------------------------------------
// Name: SetActive()
// Code: actdoll (2004-05-10)
// Desc: �ش� ��Ʈ���� Ȱ��ȭ��Ų��. ��, ���ڿ� �Է��� ���� �� �ְ� �ȴ�.
//		hOec	- ��� ��Ʈ�� �ڵ�
//		bActive	- �츱���� true, ���Ϸ��� false
//		return	- ����� �������� ��� true, �ƴ϶�� false
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
		MessageBox( NULL, "�������� ���� �ڵ��Դϴ�.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
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
// Desc: �ش� ��Ʈ���� Ȱ��ȭ ���θ� Ȯ���Ѵ�.
//		hOec	- ��� ��Ʈ�� �ڵ�
//		return	- Ȱ��ȭ�� ��� true, �ƴ϶�� false
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
		MessageBox( NULL, "�������� ���� �ڵ��Դϴ�.(Incorrect Control Handle!)", "OecEx Error", MB_OK | MB_ICONWARNING );
		return FALSE;
	}
#endif//_DEBUG

	OnlineEditControlEx		*pOecEx = (OnlineEditControlEx*)hOec;
	return pOecEx->IsActive();
}

//-----------------------------------------------------------------------------
// Name: GetControl()
// Code: actdoll (2004-05-10)
// Desc: �ش� ��Ʈ���� �����͸� �޴´�.
//		hOec	- ��� ��Ʈ�� �ڵ�
//		return	- ���� �ڵ��� ��� �ش� ��Ʈ�� ������ ����, ���ٸ� NULL
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
// Desc: �ش� ��Ʈ���� �����ͷ� �ڵ��� ã�´�.
//		pOec	- ��� ��Ʈ�� ������
//		return	- ���� ��Ʈ���� ��� �ش� ��Ʈ�� �ڵ� ����, ���ٸ� NULL
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
// Desc: ���� ���ν��� �ֻ�ܿ� ���� - Ű �Է��� �޴´�.
//		hWnd, msg, wParam, lParam	- ���ν��� �Ķ����
//		return	- �޽����� ó���Ǿ��� ��� true, ó������ �ʾҴٸ� false
//-----------------------------------------------------------------------------
BOOL	Handler_OnlineEditControlEx::Recv_KeyMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !hWnd )					return FALSE;
	if( m_uiUsedCount <= 0 )	return FALSE;
//	if( !m_hFocusedOec )		return FALSE;

	// ���� ��Ŀ�� ���� ��Ʈ���� �ϳ��� ���� �� �ٱ��� ����Ű�� ���õ� �޽����� ���ϵȴٸ� ������ �������� ����
	OnlineEditControlEx	*pOecEx = GetControl( m_hFocusedOec );
	if( !pOecEx )
	{
		switch( msg )
		{
			case WM_IME_NOTIFY:
				switch( wParam )
				{
					// �Է�Ű ��ȯ�� ������ ��츸 üũ�Ѵ�.
					// �ٸ� ���� ��� �ô´�.
					case IMN_SETCONVERSIONMODE		:	
					case IMN_SETOPENSTATUS			:
						GS_IMEBase::Check_WM_IME_NOTIFY( wParam, lParam );
						break;
				}
				return TRUE;

			case WM_INPUTLANGCHANGE:
				{
					// ��� ��ȯ�� ������ ��� üũ�Ѵ�.
					GS_IMEBase::Check_WM_INPUTLANGCHANGE( lParam );
				}
				return TRUE;

//			case WM_IME_STARTCOMPOSITION:
//			case WM_IME_ENDCOMPOSITION:
//			case WM_IME_COMPOSITION:
//				{
//					// �� ���� ��� ���� ��ҷ� ������.
//					GS_IMEBase::SetCompStatusCancel();
//				}
//				return TRUE;
		}

		return FALSE;
	}

	// ���� ������ �ƴ� ��츸 �� ����� �����Ѵ�.
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
// Desc: ������ �ڵ��� �������� �ڵ������� üũ�Ѵ�.
//		hOec	- ��� ��Ʈ�� �ڵ�
//		return	- �´ٸ� true, �ƴ϶�� false
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