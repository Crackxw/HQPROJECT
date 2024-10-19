//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 에디트 박스 컨트롤 확장판
//	File Name		: OnlineEditControlEx.h
//	Birth Date		: 2004. 04. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 내 에디트 박스에 대한 콘트롤을 관장하는 클래스이다.
//
//===================================================================================================

#ifndef __ONLINE_EDITCONTROLEX_H__
#define __ONLINE_EDITCONTROLEX_H__


//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: 에디트 컨트롤 클래스에서 사용할 플래그들
//-----------------------------------------------------------------------------

#define	DF_FLAG_OEC_TYPE_MULTILINE			0x00000001		// 다중 라인을 사용한다.(이 플래그를 설정하면 아래 플래그들을 쓸 수 없다.)
#define	DF_FLAG_OEC_TYPE_PASSWORD			0x00000002		// 패스워드 형태(*)로 출력한다. 이것을 쓰면 DF_FLAG_OEC_TYPE_ONLYASCII는 자동으로 설정된다.
#define	DF_FLAG_OEC_TYPE_ONLYNUMERIC		0x00000004		// 오직 숫자만 입력받을 수 있으며 숫자 형태로 출력한다.(DF_FLAG_OEC_TYPE_ONLYASCII와 같이 쓸 수 없다)
#define	DF_FLAG_OEC_TYPE_ONLYASCII			0x00000008		// 오직 1바이트 형태의 문자만 입력받을 수 있다.(DF_FLAG_OEC_TYPE_ONLYNUMERIC와 같이 쓸 수 없다)
#define	DF_FLAG_OEC_TYPE_BACKUPSTRING		0x00000010		// 백업 문자열을 사용할 수 있게 한다.(단일 라인에서만 가능)

#define DF_VALUE_OEC_MAX_BACKUP_LINE		5				// 백업 라인 사용시 백업 버퍼의 최대 갯수

typedef		void*		HOEC;								// 온라인 에디트 박스의 핸들

//-----------------------------------------------------------------------------
// Name: OnlineEditControlEx
// Code: actdoll (2004-04-17)
// Desc: 에디트 컨트롤 클래스
//-----------------------------------------------------------------------------
class OnlineEditControlEx
{
private:
	HOEC	m_hOwnOec;							// (시스템 플래그)자기 자신에게 부여된 핸들값
	HOEC	m_hPrevOec;							// (시스템 플래그)자기 자신 이전에서 온 핸들값
	HOEC	m_hNextOec;							// (시스템 플래그)자기 자신 다음에서 온 핸들값
	BOOL	m_bActive;							// (시스템 플래그)현재 활성화(입력을 받을 수 있는)상태인가?
	BOOL	m_bShow;							// (시스템 플래그)글씨 내역을 보여줄 것인가?
	BOOL	m_bHideCursor;						// (시스템 플래그)커서를 숨길것인가?
	BOOL	m_bCanRecvInputChar;				// (시스템 플래그)현재 문자를 받을 수 있는 상태인가?
	BOOL	m_bPushedEnter;						// (시스템 플래그)엔터키를 쳤다
	BOOL	m_bPushedTab;						// (시스템 플래그)탭키를 쳤다


	GS_IMEBase	m_GS_IMEBase;					// IME 클래스

	BYTE	m_btExceptionCharacter[256];		// 입력할 수 없는 캐릭터 그룹(0일 경우 제약 없음, 1일경우 쓸 수 없는 캐릭터)

	char	m_pszShowAttr[1024];									// 보여지고 있는 문자열에서의 속성표시
	char	m_pszShowBuffer[1024];									// 보여지고 있는 문자열
	char	m_pszString[1024];										// 진짜 문자열 그룹
	char	m_pszBackUpString[DF_VALUE_OEC_MAX_BACKUP_LINE][1024];	// 백업 문자열 그룹
	int		m_iBackUpLineNo;										// (플래그)현재 문자열에서 접속하고 있는 스트링 번호

	int		m_iLenShowBuffer;					// 현재 보여지고 있는 문자열의 크기
	int		m_iLenString;						// 현재 진짜 문자열의 크기
	int		m_iCountCharShowBuffer;				// 현재 보여지고 있는 문자의 갯수
	int		m_iCountChar;						// 현재 진짜 문자열에 들어있는 문자의 갯수
	int		m_iLenLimit;						// 이 에디트 박스에서 받을 수 있는 안시형 문자열(널문자 제외)의 최대 크기
	LONGLONG	m_i64ValueLimit;					// 이 에디트 박스에서 관장할 수 있는 최대 수치(수치입력형태에서만 사용)

	int		m_iCaretAsAnsi;						// 커서의 위치(안시형태)
	int		m_iCaretAsChar;						// 커서의 위치(문자형태)
	int		m_iCompCaretAsAnsi;					// 조합문자에서 커서의 위치(안시형태)
	int		m_iCompCaretAsChar;					// 조합문자에서 커서의 위치(문자형태)

	RECT	m_rcDestOfDraw;						// 출력 버퍼가 찍힐 범위 구간
	int		m_iFirstCharOfDraw;					// 출력 버퍼에서 보여줘야 할 첫번째 포지션
	int		m_iLastCharOfDraw;					// 출력 버퍼에서 보여줘야 할 마지막 포지션
	POINT	m_ptCaretDrawPos;					// 커서의 화면상 출력좌표 위치
	HFONT	m_hFont;							// 글씨를 출력할 때 사용할 폰트
	BOOL	m_bDrawShadow;						// 글씨 찍을 때 그림자를 찍을 것인가 말것인가?
	DWORD	m_dwColor;							// 글씨의 색깔
	DWORD	m_dwShadowColor;					// 글씨 그림자의 색깔

	DWORD	m_dwType;							// (플래그)DF_FLAG_OEC_TYPE_..의 데이터 타입

	int		m_iCount_NoProcessAsAscii;			// (내부플래그)아스키코드로 처리하지 말아야 할 횟수
	BOOL	m_bFlag_IMEOn;						// (내부플래그)바로 직전에 처리된 IME의 작동유무
	DWORD	m_dwFlag_InputStatus;				// (내부플래그)바로 직전에 처리된 IME의 형태
	
	
	
	//----------------------------------------------------------------------------
	//	정적 변수 그룹
	//----------------------------------------------------------------------------
	static	HWND	m_hMainWnd;					// 메인 핸들갑
	static	BOOL	m_bIsCompTypeMultiChar;		// (내부플래그)문장조합이 일본어와 같은 다중문자조합형이라면 true, 영어 혹은 한국어와 같은 단일문자조합형이라면 false
	static	HFONT	m_hCandidateFont;			// 숙어창용 폰트
	static	HPEN	m_hPen[3];					// (내부플래그)속성값을 표현할 때 쓰는 밑줄 선

public:
	OnlineEditControlEx		( void );
	~OnlineEditControlEx	( void );

	//----------------------------------------------------------------------------
	//	플래그 등의 정보
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

	void	SetMaxStringSize( int iStrLen )		{ (iStrLen >= 1024) ? m_iLenLimit = 1023	:	m_iLenLimit = iStrLen; }	// 입력 문자수 최대값 입력
	int		GetMaxStringSize( void )			{ return m_iLenLimit; }														// 입력 문자수 최대값 받기
	void	SetMaxValue		( LONGLONG i64Value ){ (i64Value < 0) ? m_i64ValueLimit = 0	:	m_i64ValueLimit = i64Value; }	// 입력 수치수 최대값 입력
	LONGLONG	GetMaxValue		( void )			{ return m_i64ValueLimit; }													// 입력 수치수 최대값 받기

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
	//	데이터 얻기
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
	//	출력용 함수
	//----------------------------------------------------------------------------

	BOOL	Draw( HDC hDC, BOOL bRightAlign = false );



	//----------------------------------------------------------------------------
	//	프로시저의 메시지부
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
	//	내부 툴 함수
	//----------------------------------------------------------------------------

	void	UpdateShowBuffer				( BOOL bNotTouchShowBuffer = false );
	void	UpdateShowRangeForSingleLine	( HDC hDC );
	void	UpdateShowRangeForMultiLine		( HDC hDC );

	void	UpdateAttrForSingleLine			( HDC hDC, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd );
	void	UpdateAttrForMultiLine			( HDC hDC, char *pszFirstCharOfLine, int iLenOfLine, int &iDotStart, int &iDotEnd, int &iThinStart, int &iThinEnd, int &iThickStart, int &iThickEnd );


	
	//----------------------------------------------------------------------------
	//	스태틱 함수
	//----------------------------------------------------------------------------

public:
	static	void	SetHwnd					( HWND hWnd )						{ m_hMainWnd = hWnd; }
	static	HWND	GetHwnd					( void )							{ return m_hMainWnd; }
	static	UINT	GetCodePage				( void )							{ return GS_IMEBase::GetCodePage(); }				// 코드 페이지를 받아온다.
	static	void	SetCandidateFont		( HFONT hFont )						{ m_hCandidateFont = hFont; }						// 숙어창에 사용할 폰트를 받는다.
	static	HFONT	GetCandidateFont		( void )							{ return m_hCandidateFont; }						// 숙어창에 사용할 폰트를 알려준다.
	static	BOOL	IsCompTypeMultiChar		( void )							{ return m_bIsCompTypeMultiChar; }					// 다중조합문자형인가?
	static	BOOL	IsCompTypeSingleChar	( void )							{ return !m_bIsCompTypeMultiChar; }					// 단일조합문자형인가?
	static	BOOL	IsLanguageForKorean		( void )							{ return ( GS_IMEBase::IsLocaleKorean() ); }		// 한국어 형태인가?
	static	BOOL	IsLanguageForJapanese	( void )							{ return ( GS_IMEBase::IsLocaleJapanese() ); }		// 일본어 형태인가?
	static	BOOL	IsLanguageForChinese	( void )							{ return ( GS_IMEBase::IsLocaleSimpChinese() ); }	// 중국어 형태인가?
	static	BOOL	IsLanguageForTaiwanese	( void )							{ return ( GS_IMEBase::IsLocaleTradChinese() ); }	// 대만어 형태인가?
	static	char*	GetStrPtrByCharPos		( char *pszFirstStr, char *pszBaseStr, int iCharPos );
	static	int		GetCharCountByStrPtr	( char *pszFirstStr, char *pszBaseStr, int iPtrPos, BOOL bGetUpper = false );
	static	int		GetCharLen				( char *pszSrcStr );
	static char*	GetCharNext				( char *pszSrcStr, BOOL &bReturnExist );
	static char*	GetCharPrev				( char *pszFirstStr, char *pszSrcStr, BOOL &bReturnExist );
	static void		GetStringPixelSize		( HDC hDC, char *pszSrc, int iCount, int &width, int &height, BOOL bCountIsChar = false );
	static char*	GetSingleLineOfMultiStr	(HDC hDC, char *pszSrcStr, int iDestWidth, int &iLenPerLine, int &iCountPerLine, BOOL &bIncludeReturn );
};

#endif//__ONLINE_EDITCONTROLEX_H__