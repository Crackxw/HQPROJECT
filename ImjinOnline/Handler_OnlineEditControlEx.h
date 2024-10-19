//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ����Ʈ �ڽ� ��Ʈ�� Ȯ����
//	File Name		: Handler_OnlineEditControlEx.h
//	Birth Date		: 2004. 05. 03.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		����Ʈ ��Ʈ�ѿ� ���� ������ �����ϴ� �����Լ��̴�. �� �Լ��� ���α׷� ������ �ϳ��ۿ� ���ٴ� ������ �д�.
//
//===================================================================================================

#ifndef	__HANDLER_ONLINE_EDITCONTROLEX_H__
#define	__HANDLER_ONLINE_EDITCONTROLEX_H__

#define DF_VALUE_MAX_COUNT_OEC		50			// �� �ڵ鷯���� �ѹ��� ������ ����Ʈ �ڽ��� �ִ� ����

//-----------------------------------------------------------------------------
// Name: Handler_OnlineEditControlEx
// Code: actdoll (2004-04-17)
// Desc: ����Ʈ ��Ʈ�� �ڽ��� ������ �ڵ鷯
//-----------------------------------------------------------------------------
class Handler_OnlineEditControlEx
{
private:
	static	HWND					m_hMainWnd;									// �� �ڵ鷯���� ����� ���� ���� �ڵ�
	static	OnlineEditControlEx		m_clOec[DF_VALUE_MAX_COUNT_OEC];			// ������ ����Ʈ ��Ʈ�ѵ�
	static	HOEC					m_hFocusedOec;								// ���� ��Ŀ���� ������ ��Ʈ��
	
	
	static	UINT					m_uiUsedCount;								// (�����÷���)����Ʈ ��Ʈ���� ���� Ƚ��
	static	BOOL					m_bPushedShift;								// ����ƮŰ�� �������´�.


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
	static	void	SetCandidateFont			( HFONT hFont )			{ OnlineEditControlEx::SetCandidateFont( hFont ); }	// �ʹݺ� ù ���� - ����â ��Ʈ
	static	BOOL	IsCurrentHOEC				( HOEC hOec );
};


#endif//__HANDLER_ONLINE_EDITCONTROLEX_H__