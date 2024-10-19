//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ����Ʈ �ڽ� ��Ʈ�� Ȯ����
//	File Name		: OnlineEditControlEx.h
//	Birth Date		: 2004. 04. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �� ����Ʈ �ڽ��� ���� ��Ʈ���� �����ϴ� Ŭ�����̴�.
//
//===================================================================================================

#ifndef __ONLINE_EDITCONTROLEX_H__
#define __ONLINE_EDITCONTROLEX_H__


//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: ����Ʈ ��Ʈ�� Ŭ�������� ����� �÷��׵�
//-----------------------------------------------------------------------------

#define	DF_FLAG_OEC_TYPE_MULTILINE			0x00000001		// ���� ������ ����Ѵ�.(�� �÷��׸� �����ϸ� �Ʒ� �÷��׵��� �� �� ����.)
#define	DF_FLAG_OEC_TYPE_PASSWORD			0x00000002		// �н����� ����(*)�� ����Ѵ�. �̰��� ���� DF_FLAG_OEC_TYPE_ONLYASCII�� �ڵ����� �����ȴ�.
#define	DF_FLAG_OEC_TYPE_ONLYNUMERIC		0x00000004		// ���� ���ڸ� �Է¹��� �� ������ ���� ���·� ����Ѵ�.(DF_FLAG_OEC_TYPE_ONLYASCII�� ���� �� �� ����)
#define	DF_FLAG_OEC_TYPE_ONLYASCII			0x00000008		// ���� 1����Ʈ ������ ���ڸ� �Է¹��� �� �ִ�.(DF_FLAG_OEC_TYPE_ONLYNUMERIC�� ���� �� �� ����)
#define	DF_FLAG_OEC_TYPE_BACKUPSTRING		0x00000010		// ��� ���ڿ��� ����� �� �ְ� �Ѵ�.(���� ���ο����� ����)

#define DF_VALUE_OEC_MAX_BACKUP_LINE		5				// ��� ���� ���� ��� ������ �ִ� ����

typedef		void*		HOEC;								// �¶��� ����Ʈ �ڽ��� �ڵ�

//-----------------------------------------------------------------------------
// Name: OnlineEditControlEx
// Code: actdoll (2004-04-17)
// Desc: ����Ʈ ��Ʈ�� Ŭ����
//-----------------------------------------------------------------------------
class OnlineEditControlEx
{
private:
	HOEC	m_hOwnOec;							// (�ý��� �÷���)�ڱ� �ڽſ��� �ο��� �ڵ鰪
	HOEC	m_hPrevOec;							// (�ý��� �÷���)�ڱ� �ڽ� �������� �� �ڵ鰪
	HOEC	m_hNextOec;							// (�ý��� �÷���)�ڱ� �ڽ� �������� �� �ڵ鰪
	BOOL	m_bActive;							// (�ý��� �÷���)���� Ȱ��ȭ(�Է��� ���� �� �ִ�)�����ΰ�?
	BOOL	m_bShow;							// (�ý��� �÷���)�۾� ������ ������ ���ΰ�?
	BOOL	m_bHideCursor;						// (�ý��� �÷���)Ŀ���� ������ΰ�?
	BOOL	m_bCanRecvInputChar;				// (�ý��� �÷���)���� ���ڸ� ���� �� �ִ� �����ΰ�?
	BOOL	m_bPushedEnter;						// (�ý��� �÷���)����Ű�� �ƴ�
	BOOL	m_bPushedTab;						// (�ý��� �÷���)��Ű�� �ƴ�


	GS_IMEBase	m_GS_IMEBase;					// IME Ŭ����

	BYTE	m_btExceptionCharacter[256];		// �Է��� �� ���� ĳ���� �׷�(0�� ��� ���� ����, 1�ϰ�� �� �� ���� ĳ����)

	char	m_pszShowAttr[1024];									// �������� �ִ� ���ڿ������� �Ӽ�ǥ��
	char	m_pszShowBuffer[1024];									// �������� �ִ� ���ڿ�
	char	m_pszString[1024];										// ��¥ ���ڿ� �׷�
	char	m_pszBackUpString[DF_VALUE_OEC_MAX_BACKUP_LINE][1024];	// ��� ���ڿ� �׷�
	int		m_iBackUpLineNo;										// (�÷���)���� ���ڿ����� �����ϰ� �ִ� ��Ʈ�� ��ȣ

	int		m_iLenShowBuffer;					// ���� �������� �ִ� ���ڿ��� ũ��
	int		m_iLenString;						// ���� ��¥ ���ڿ��� ũ��
	int		m_iCountCharShowBuffer;				// ���� �������� �ִ� ������ ����
	int		m_iCountChar;						// ���� ��¥ ���ڿ��� ����ִ� ������ ����
	int		m_iLenLimit;						// �� ����Ʈ �ڽ����� ���� �� �ִ� �Ƚ��� ���ڿ�(�ι��� ����)�� �ִ� ũ��
	LONGLONG	m_i64ValueLimit;					// �� ����Ʈ �ڽ����� ������ �� �ִ� �ִ� ��ġ(��ġ�Է����¿����� ���)

	int		m_iCaretAsAnsi;						// Ŀ���� ��ġ(�Ƚ�����)
	int		m_iCaretAsChar;						// Ŀ���� ��ġ(��������)
	int		m_iCompCaretAsAnsi;					// ���չ��ڿ��� Ŀ���� ��ġ(�Ƚ�����)
	int		m_iCompCaretAsChar;					// ���չ��ڿ��� Ŀ���� ��ġ(��������)

	RECT	m_rcDestOfDraw;						// ��� ���۰� ���� ���� ����
	int		m_iFirstCharOfDraw;					// ��� ���ۿ��� ������� �� ù��° ������
	int		m_iLastCharOfDraw;					// ��� ���ۿ��� ������� �� ������ ������
	POINT	m_ptCaretDrawPos;					// Ŀ���� ȭ��� �����ǥ ��ġ
	HFONT	m_hFont;							// �۾��� ����� �� ����� ��Ʈ
	BOOL	m_bDrawShadow;						// �۾� ���� �� �׸��ڸ� ���� ���ΰ� �����ΰ�?
	DWORD	m_dwColor;							// �۾��� ����
	DWORD	m_dwShadowColor;					// �۾� �׸����� ����

	DWORD	m_dwType;							// (�÷���)DF_FLAG_OEC_TYPE_..�� ������ Ÿ��

	int		m_iCount_NoProcessAsAscii;			// (�����÷���)�ƽ�Ű�ڵ�� ó������ ���ƾ� �� Ƚ��
	BOOL	m_bFlag_IMEOn;						// (�����÷���)�ٷ� ������ ó���� IME�� �۵�����
	DWORD	m_dwFlag_InputStatus;				// (�����÷���)�ٷ� ������ ó���� IME�� ����
	
	
	
	//----------------------------------------------------------------------------
	//	���� ���� �׷�
	//----------------------------------------------------------------------------
	static	HWND	m_hMainWnd;					// ���� �ڵ鰩
	static	BOOL	m_bIsCompTypeMultiChar;		// (�����÷���)���������� �Ϻ���� ���� ���߹����������̶�� true, ���� Ȥ�� �ѱ���� ���� ���Ϲ����������̶�� false
	static	HFONT	m_hCandidateFont;			// ����â�� ��Ʈ
	static	HPEN	m_hPen[3];					// (�����÷���)�Ӽ����� ǥ���� �� ���� ���� ��

public:
	OnlineEditControlEx		( void );
	~OnlineEditControlEx	( void );

	//----------------------------------------------------------------------------
	//	�÷��� ���� ����
	//----------------------------------------------------------------------------

	void	Clear			( BOOL bAllLine = false );

	void	SetHandle		( HOEC hOec )					{ m_hOwnOec = hOec; }
	HOEC	GetHandle		( void )						{ return m_hOwnOec; }
	void	SetPrevHandle	( HOEC hOec )					{ m_hPrevOec = hOec; }
	HOEC	GetPrevHandle	( void )						{ return m_hPrevOec; }
	void	SetNextHandle	( HOEC hOec )					{ m_hNextOec = hOec; }
	HOEC	GetNextHandle	( void )						{ return m_hNextOec; }
	void	SetActive		( BOOL bOn = true )				{ m_bActive = bOn; }
	BOOL	IsActive		( void )						{ return m_bActive; }
	void	SetShow			( BOOL bOn = true )				{ m_bShow = bOn; }
	BOOL	IsShow			( void )						{ return m_bShow; }
	void	SetCursorShow	( BOOL bOn = true )				{ m_bHideCursor = !bOn; }
	BOOL	IsCursorShow	( void )						{ return !m_bHideCursor; }
	void	SetRecvInputChar	( BOOL bOn = true )			{ m_bCanRecvInputChar = bOn; SetCursorShow( bOn ); }
	BOOL	IsCanRecvInputChar	( void )					{ return m_bCanRecvInputChar; }
	void	SetFocus		();
	void	KillFocus		();
	void	SetPushEnter		( BOOL bPush = true )						{ m_bPushedEnter = bPush; };
	BOOL	IsPushEnter			( void );
	void	SetPushTab			( BOOL bPush = true )						{ m_bPushedTab = bPush; };
	BOOL	IsPushTab			( void );

	void	SetMaxStringSize( int iStrLen )		{ (iStrLen >= 1024) ? m_iLenLimit = 1023	:	m_iLenLimit = iStrLen; }	// �Է� ���ڼ� �ִ밪 �Է�
	int		GetMaxStringSize( void )			{ return m_iLenLimit; }														// �Է� ���ڼ� �ִ밪 �ޱ�
	void	SetMaxValue		( LONGLONG i64Value ){ (i64Value < 0) ? m_i64ValueLimit = 0	:	m_i64ValueLimit = i64Value; }	// �Է� ��ġ�� �ִ밪 �Է�
	LONGLONG	GetMaxValue		( void )			{ return m_i64ValueLimit; }													// �Է� ��ġ�� �ִ밪 �ޱ�

	void	SetCaretPosAsAnsi	( int iPos );
	int		GetCaretPosAsAnsi	( void )						{ return m_iCaretAsAnsi; }
	void	SetCaretPosAsChar	( int iPos );
	int		GetCaretPosAsChar	( void )						{ return m_iCaretAsChar; }

	void	SetRectOfDraw		( int iLeft, int iTop, int iRight, int iBottom )	{ SetRect( &m_rcDestOfDraw, iLeft, iTop, iRight, iBottom ); }
	void	SetRectOfDraw		( RECT &rcSrc )										{ m_rcDestOfDraw = rcSrc; }
	void	GetRectOfDraw		( RECT &rcDest )									{ rcDest = m_rcDestOfDraw; }
	BOOL	IsPointInRect		( POINT pt )										{ return PtInRect( &m_rcDestOfDraw, pt ); }
	POINT*	GetCaretDrawPos		( void )											{ return &m_ptCaretDrawPos; }

	void	SetFont				( HFONT hFont )										{ m_hFont = hFont; }
	HFONT	GetFont				( void )											{ return m_hFont; }
	void	SetDrawShadow		( BOOL bOn = true )									{ m_bDrawShadow = bOn; }
	DWORD	IsDrawShadowEnable	( void )											{ return m_bDrawShadow; }
	void	SetColor			( DWORD dwColor, BOOL bIsShadow = false )			{ ( !bIsShadow ) ? m_dwColor = dwColor : m_dwShadowColor = dwColor; }
	DWORD	GetColor			( BOOL bIsShadow = false )							{ return ( ( !bIsShadow ) ? m_dwColor : m_dwShadowColor ); }

	void	SetTypeOn			( DWORD dwFlag );
	void	SetTypeOff			( DWORD dwFlag )				{ m_dwType &= ~dwFlag; }
	BOOL	IsTypeSingleLine	( void )						{ return !( m_dwType		& DF_FLAG_OEC_TYPE_MULTILINE	); }
	BOOL	IsTypeMultiLine		( void )						{ return  ( m_dwType		& DF_FLAG_OEC_TYPE_MULTILINE	); }
	BOOL	IsTypePassword		( void )						{ return  ( m_dwType		& DF_FLAG_OEC_TYPE_PASSWORD		); }
	BOOL	IsTypeOnlyNumeric	( void )						{ return  ( m_dwType		& DF_FLAG_OEC_TYPE_ONLYNUMERIC	); }
	BOOL	IsTypeOnlyAscii		( void )						{ return  ( m_dwType		& DF_FLAG_OEC_TYPE_ONLYASCII	); }
	BOOL	IsTypeBackUpString	( void )						{ return  ( m_dwType		& DF_FLAG_OEC_TYPE_BACKUPSTRING	); }

	void	ClearExceptionChar	( void )									{ ZeroMemory( m_btExceptionCharacter, sizeof(m_btExceptionCharacter) ); }
	void	SetExceptionChar	( BYTE btCharCode, BOOL bNotUse = true )	{ ( bNotUse ) ? m_btExceptionCharacter[btCharCode] = 1 : m_btExceptionCharacter[btCharCode] = 0; }
	BOOL	IsExceptionChar		( BYTE btCharCode )							{ return (BOOL)m_btExceptionCharacter[btCharCode]; }
	
	GS_IMEBase*	GetIMEBase	( void )		{ return &m_GS_IMEBase; }

	//----------------------------------------------------------------------------
	//	������ ���
	//----------------------------------------------------------------------------

	const char* GetShowString		( void )				{ return (const char*)m_pszShowBuffer; }
	BOOL		SetString			( char *pszString );
	const char*	GetString			( void )				{ return (const char*)m_pszString; }
	BOOL		SetValue			( LONGLONG i64Value );
	LONGLONG	GetValue			( void );
	const char*	GetBackUpString		( int iNo )	{ return ( ( iNo >= 0 && iNo < DF_VALUE_OEC_MAX_BACKUP_LINE ) ? (const char*)m_pszBackUpString[iNo] : NULL ); }
	int			GetStringLen		( void )	{ return m_iLenString; }
	int			GetShowStringLen	( void )	{ return m_iLenShowBuffer; }

	
	void		ChangeCaretNext				( BOOL bSetNext = true, BOOL bSetFirstAndLast = false );
	void		ChangeFromStringToBackUp	( BOOL bSetNext = true );
	void		ChangeFromBackUpToString	( BOOL bGetNext = true );


	//----------------------------------------------------------------------------
	//	��¿� �Լ�
	//----------------------------------------------------------------------------

	BOOL	Draw( HDC hDC, BOOL bRightAlign = false );



	//----------------------------------------------------------------------------
	//	���ν����� �޽�����
	//----------------------------------------------------------------------------
public:

	BOOL	Proc_WindowMsg					( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	BOOL	Proc_WindowMsg_StartComposition	( HWND hWnd, WPARAM wParam, LPARAM lParam );
	BOOL	Proc_WindowMsg_EndComposition	( HWND hWnd, WPARAM wParam, LPARAM lParam );
	BOOL	Proc_WindowMsg_Composition		( HWND hWnd, WPARAM wParam, LPARAM lParam );
	BOOL	Proc_WindowMsg_Notify			( HWND hWnd, WPARAM wParam, LPARAM lParam );

	BOOL	Proc_WindowMsg_KeyDown_AnsiChar	( HWND hWnd, WPARAM wParam );
	BOOL	Proc_WindowMsg_KeyDown_Move		( HWND hWnd, WPARAM wParam );
	BOOL	Proc_WindowMsg_KeyDown_Delete	( HWND hWnd, WPARAM wParam );
	BOOL	Proc_WindowMsg_KeyDown_BackSpace( HWND hWnd, WPARAM wParam );
	BOOL	Proc_WindowMsg_KeyDown_Enter	( HWND hWnd, WPARAM wParam );

	//----------------------------------------------------------------------------
	//	���� �� �Լ�
	//----------------------------------------------------------------------------

	void	UpdateShowBuffer				( BOOL bNotTouchShowBuffer = false );
	void	UpdateShowRangeForSingleLine	( HDC hDC );
	void	UpdateShowRangeForMultiLine		( HDC hDC );

	void	UpdateAttrForSingleLine			( HDC hDC, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd );
	void	UpdateAttrForMultiLine			( HDC hDC, char *pszFirstCharOfLine, int iLenOfLine, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd );


	
	//----------------------------------------------------------------------------
	//	����ƽ �Լ�
	//----------------------------------------------------------------------------

public:
	static	void	SetHwnd					( HWND hWnd )						{ m_hMainWnd = hWnd; }
	static	HWND	GetHwnd					( void )							{ return m_hMainWnd; }
	static	UINT	GetCodePage				( void )							{ return GS_IMEBase::GetCodePage(); }				// �ڵ� �������� �޾ƿ´�.
	static	void	SetCandidateFont		( HFONT hFont )						{ m_hCandidateFont = hFont; }						// ����â�� ����� ��Ʈ�� �޴´�.
	static	HFONT	GetCandidateFont		( void )							{ return m_hCandidateFont; }						// ����â�� ����� ��Ʈ�� �˷��ش�.
	static	BOOL	IsCompTypeMultiChar		( void )							{ return m_bIsCompTypeMultiChar; }					// �������չ������ΰ�?
	static	BOOL	IsCompTypeSingleChar	( void )							{ return !m_bIsCompTypeMultiChar; }					// �������չ������ΰ�?
	static	BOOL	IsLanguageForKorean		( void )							{ return ( GS_IMEBase::IsLocaleKorean() ); }		// �ѱ��� �����ΰ�?
	static	BOOL	IsLanguageForJapanese	( void )							{ return ( GS_IMEBase::IsLocaleJapanese() ); }		// �Ϻ��� �����ΰ�?
	static	BOOL	IsLanguageForChinese	( void )							{ return ( GS_IMEBase::IsLocaleSimpChinese() ); }	// �߱��� �����ΰ�?
	static	BOOL	IsLanguageForTaiwanese	( void )							{ return ( GS_IMEBase::IsLocaleTradChinese() ); }	// �븸�� �����ΰ�?
	static	char*	GetStrPtrByCharPos		( char *pszFirstStr, char *pszBaseStr, int iCharPos );
	static	int		GetCharCountByStrPtr	( char *pszFirstStr, char *pszBaseStr, int iPtrPos, BOOL bGetUpper = false );
	static	int		GetCharLen				( char *pszSrcStr );
	static char*	GetCharNext				( char *pszSrcStr, BOOL &bReturnExist );
	static char*	GetCharPrev				( char *pszFirstStr, char *pszSrcStr, BOOL &bReturnExist );
	static void		GetStringPixelSize		( HDC hDC, char *pszSrc, int iCount, int &width, int &height, BOOL bCountIsChar = false );
	static char*	GetSingleLineOfMultiStr	(HDC hDC, char *pszSrcStr, int iDestWidth, int &iLenPerLine, int &iCountPerLine, BOOL &bIncludeReturn );
};

#endif//__ONLINE_EDITCONTROLEX_H__