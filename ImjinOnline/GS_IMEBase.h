//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - IME 베이스
//	File Name		: GS_IMEBase.h
//	Birth Date		: 2004. 04. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 내 에디트 박스에서 쓰일 IME 기능을 구현한다.
//
//===================================================================================================

#ifndef __IME_BASE_H__
#define __IME_BASE_H__

#define					DF_VALUE_MAX_CANDIDATE		10
#define					DF_VALUE_IME_BUFFER_SIZE	256

extern	const	HKL		DF_VALUE_HKL_CHT_DEFAULT;		// 중국어 (번체) - Default
extern	const	HKL		DF_VALUE_HKL_CHS_USKEYBOARD;	// 중국어 (간체) - 번체 US KeyBoard
extern	const	HKL		DF_VALUE_HKL_CHT_USKEYBOARD;	// 중국어 (번체) - 간체 US KeyBoard
extern	const	HKL		DF_VALUE_HKL_CHS_DEFAULT;		// 중국어 (간체) - Default

extern	const	HKL		DF_VALUE_HKL_CHT_PHONETIC;		// 중국어 (번체) - Phonetic
extern	const	HKL		DF_VALUE_HKL_JPN_MSIME2002;		// 일본어 입력 시스템 (MS-IME2002)
extern	const	HKL		DF_VALUE_HKL_KOR_MSIME2000;		// 한국어 입력 시스템 (IME 2000)
extern	const	HKL		DF_VALUE_HKL_CHS_QUANPIN;		// 중국어 (간체) - QuanPin
extern	const	HKL		DF_VALUE_HKL_CHT_CHANGJIE;		// 중국어 (번체) - ChangJie
extern	const	HKL		DF_VALUE_HKL_CHS_SHUANGPIN;		// 중국어 (간체) - ShuangPin
extern	const	HKL		DF_VALUE_HKL_CHT_QUICK;			// 중국어 (번체) - Quick
extern	const	HKL		DF_VALUE_HKL_CHS_ZHENGMA;		// 중국어 (간체) - ZhengMa
extern	const	HKL		DF_VALUE_HKL_CHT_BIG5CODE;		// 중국어 (번체) - Big5 Code
extern	const	HKL		DF_VALUE_HKL_CHS_INTELABC;		// 중국어 (간체) - 지능 ABC
extern	const	HKL		DF_VALUE_HKL_CHT_ARRAY;			// 중국어 (번체) - Array
extern	const	HKL		DF_VALUE_HKL_CHS_NEIMA;			// 중국어 (간체) - NeiMa
extern	const	HKL		DF_VALUE_HKL_CHT_DAYI;			// 중국어 (번체) - DaYi
extern	const	HKL		DF_VALUE_HKL_CHT_UNICODE;		// 중국어 (번체) - Unicode
extern	const	HKL		DF_VALUE_HKL_CHT_NEWPHONETIC;	// 중국어 (번체) - New Phonetic
extern	const	HKL		DF_VALUE_HKL_CHT_NEWCHANGJIE;	// 중국어 (번체) - New ChangJie
extern	const	HKL		DF_VALUE_HKL_CHS_MSPY;			// 중국어 (간체) - Microsoft Pinyin IME 3.0
extern	const	HKL		DF_VALUE_HKL_CHT_ALPHANUMERIC;	// 중국어 (번체) - Alphanumeric
extern	const	HKL		DF_VALUE_HKL_KOR_MSIME2003;		// 한국어 입력 시스템 (MS-IME2003)

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: 발음창/숙어창에서 쓰는 구조체
//-----------------------------------------------------------------------------
struct ST_IME_CANDIDATE_INFO
{
	char		m_pszStringBuffer[DF_VALUE_MAX_CANDIDATE][DF_VALUE_IME_BUFFER_SIZE];	// 한번에 보여줄 숙어 리스트
	DWORD		m_dwPageSize;															// 이 숙어창이 보여줄 수 있는 단어의 최대 갯수
	DWORD		m_dwCount;															    // 이 숙어창에 들어있는 단어의 갯수
	DWORD		m_dwSelection;															// 이 숙어창에서 선택한 현재 단어의 위치
};

class GS_IMEBase
{
private:
	//----------------------------------------------------------------------------
	//	정적 변수 그룹
	//----------------------------------------------------------------------------

	static	HWND		m_hWnd;											// 이 IME 시스팀의 부모 윈도우 핸들
	static	HIMC		m_hImcDefault;									// 이 IME 시스템에 할당된 기본 IMC 핸들
	static	HINSTANCE	m_hDllImm32;									// IMM32.DLL의 인스턴스 핸들
	static	HINSTANCE	m_hDllIme;										// 각각의 IME 모듈의 인스턴스 핸들
	static	HKL			m_hKLCurrent;									// 현재 키보드 입력기 핸들


	//----------------------------------------------------------------------------
	//	정적 변수 그룹 - 외부 알림
	//----------------------------------------------------------------------------
	
	static	UINT		m_uiCodePage;									// 현재의 IME에서 사용해야 할 코드페이지 값
	static	int			m_enImeStatus;									// 현재의 IME 상태
	static	char		m_pszInputLocaleName[4];						// 현재의 입력 로케일 명칭
	static	char		m_pszImeLayoutName[128];						// 현재의 IME 출력기 명칭

	static	BOOL		m_bCandidateTypeVer;							// 세로로 출력되는 숙어창 형식이면 true, 가로라면 false
	static	BOOL		m_bCandidateWindowOn;							// 숙어창이 켜졌다면 true, 꺼졌다면 false
	static	BOOL		m_bReadingWindowOn;								// 발음창이 켜졌다면 true, 꺼졌다면 false

	static	BOOL		m_bFirstComposing;								// 조합 작업이 처음으로 개시된 상태인가?
	static	BOOL		m_bIsComposing;									// 현재 조합중인 상태인가?


	//----------------------------------------------------------------------------
	//	내부 변수 그룹
	//----------------------------------------------------------------------------
	static	int			m_siCompCaretPos;								// 조합 상태에서의 커서 현재 위치
	static	int			m_siLenResultBuff;								// 결과값 스트링의 문자길이
	static	int			m_siLenCompBuff;								// 조합값 스트링의 문자길이


	static	char		m_pszResultBuff[DF_VALUE_IME_BUFFER_SIZE];		// 결과값으로 떨어진 스트링을 임시 저장할 버퍼
	static	char		m_pszCompBuff[DF_VALUE_IME_BUFFER_SIZE];		// 조합값으로 떨어진 스트링을 임시 저장할 버퍼
	static	char		m_pszCompAttrBuff[DF_VALUE_IME_BUFFER_SIZE];	// 조합값의 스트링별 속성값을 임시 저장할 버퍼
	static	char		m_pszReadingBuff[DF_VALUE_IME_BUFFER_SIZE];		// 발음문자로 떨어진 스트링을 임시 저장할 버퍼
	
	static	ST_IME_CANDIDATE_INFO	m_stCandidateInfo;					// 숙어창 관련 정보


public:
	GS_IMEBase	( void );
	~GS_IMEBase	( void );

	static			void	Initialize				( HWND hWnd );
	static			void	Clear					( void );
	static			void	InitializeImm			( void );
	static			void	ReleaseImm				( void );
	static			void	CheckImeModule			( void );
	static			void	CheckDefaultImc			( void );

	//----------------------------------------------------------------------------
	//	정보 함수
	//----------------------------------------------------------------------------
	static			HKL		GetCurrentKeyboardLocale( void )		{ return m_hKLCurrent; }						// 현재의 입력 로케일 핸들 얻기
	static	const	char*	GetInputLocaleName		( void )		{ return (const char*)m_pszInputLocaleName; }	// 현재의 입력 로케일 명칭 얻기
	static	const	char*	GetImeLayoutName		( void )		{ return (const char*)m_pszImeLayoutName; }		// 현재 설정된 IME 명칭 얻기
	static			UINT	GetCodePage				( void )		{ return m_uiCodePage; }						// 현재 설정된 코드 페이지 얻기
	static			BOOL	IsImeRun				( void );
	static			BOOL	IsLocaleKorean			( void );
	static			BOOL	IsLocaleJapanese		( void );
	static			BOOL	IsLocaleTradChinese		( void );
	static			BOOL	IsLocaleSimpChinese		( void );
	static			BOOL	IsLocaleNonIMEType		( void );
	static			BOOL	IsInputAlphabet			( void );
	static			BOOL	IsInputNative			( void );
	static			BOOL	IsInputKatakana			( void );
	static			BOOL	IsInputFullShape		( void );
	static			BOOL	IsInputHanjaConvert		( void );


	//----------------------------------------------------------------------------
	//	IME 기능 및 상태와 관련된 내용
	//----------------------------------------------------------------------------

	static			void	UpdateInputStatus		( void );
	static			DWORD	GetImeId				( UINT uIndex = 0 );
	static			void	SetImeDeviceOn			( BOOL bDeviceOn );
	static			void	SetIMEStatus			( BOOL bOpen );
	static			BOOL	IsIMEOn					( void );
	static			void	SetInputStatus			( DWORD dwConv );
	static			void	GetInputStatus			( DWORD &dwConversion, DWORD &dwSentence );

	static			void	SetCompStatusCancel		( void );
	static			void	SetCompStatusDoComplete	( void );

	static			BOOL	IsFirstIMEComposing		( void )		{ return m_bFirstComposing; }	// IME가 현재 첫 조합중으로 들어갔는가?
	static			BOOL	IsIMEComposing			( void )		{ return m_bIsComposing; }		// IME가 현재 조합중의 상태인가?

	static	const	char*	GetReadingStringData	( void );
	static			BOOL	IsOpenReadingString		( void )		{ return m_bReadingWindowOn; }
	
	static			void	SetCandidateStatusOpen	( void );
	static			void	SetCandidateStatusClose	( void );
	static	const	char*	GetCandidateString		( int iIndex );
	static			DWORD	GetCandidatePageSize	( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwPageSize	: (DWORD)-1 ); }	// 이 숙어창이 한페이지에서 보여줄 수 있는 최대 갯수를 리턴한다.
	static			DWORD	GetCandidateCount		( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwCount		: (DWORD)-1 ); }	// 이 숙어창의 현재 페이지의 단어 갯수를 리턴한다.
	static			DWORD	GetCandidateSelection	( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwSelection	: (DWORD)-1 ); }	// 이 숙어창에서 선택된 항목을 리턴한다.
	static			BOOL	IsOpenCandidate			( void )		{ return m_bCandidateWindowOn; }															// 특수문자 조합 창이 떠있는 상태인가?
	static			BOOL	IsCandidateVertical		( void )		{ return m_bCandidateTypeVer; }																// 숙어창 표기 타입이 가로인가 세로인가?
	static			BOOL	IsCandidateExType		( void );



	//----------------------------------------------------------------------------
	//	IME 에서 확보한 데이터를 얻어오는 내용
	//----------------------------------------------------------------------------

	static	const	char*	GetResultStringData		( void )		{ return ( (const char*)m_pszResultBuff ); }	// 조합완료 스트링을 받는다.
	static	const	char*	GetComposingStringData	( void )		{ return ( (const char*)m_pszCompBuff ); }		// 조합중인 스트링을 받는다.
	static	const	char*	GetComposingAttrData	( void )		{ return ( (const char*)m_pszCompAttrBuff ); }	// 조합중인 스트링의 속성을 받는다.
	static			int		GetComposingCaretPos	( void )		{ return m_siCompCaretPos; }					// 조합중인 커서의 위치를 받는다.

	static			int		GetResultStringLen		( void )		{ return m_siLenResultBuff; }					// 조합완료 스트링의 크기를 받는다.
	static			int		GetComposingStringLen	( void )		{ return m_siLenCompBuff; }						// 조합중인 스트링의 크기를 받는다.



	//----------------------------------------------------------------------------
	//	프로시저의 메시지부에 설치해야 할 것들
	//----------------------------------------------------------------------------
public:
	static			BOOL	Check_WM_INPUTLANGCHANGE				( LPARAM lParam );
	static			BOOL	Check_WM_IME_STARTCOMPOSITION			( LPARAM lParam );
	static			BOOL	Check_WM_IME_ENDCOMPOSITION				( LPARAM lParam );

		//----------------------------------------------------------------------------
		//	actdoll (2004/10/27 16:52) : 
		//		주의! 이 Check_WM_IME_COMPOSITION_...()부분은 
		//		WM_IME_COMPOSITION 메시지가 떨어지는 부분에 반드시 순차적으로 배치되어야 합니다.
		//----------------------------------------------------------------------------

	static			BOOL	Check_WM_IME_COMPOSITION_Start			( WPARAM wParam, LPARAM lParam );
	static			BOOL	Check_WM_IME_COMPOSITION_ResultString	( LPARAM lParam, int iCanRecvLen	= -1 );
	static			BOOL	Check_WM_IME_COMPOSITION_CompString		( LPARAM lParam, int iRemainLen		= 2 );
	static			BOOL	Check_WM_IME_COMPOSITION_CompAttribute	( LPARAM lParam, int iRemainLen		= 2 );
	static			BOOL	Check_WM_IME_COMPOSITION_CompCursor		( LPARAM lParam, int iRemainLen		= 2 );
	static			BOOL	Check_WM_IME_COMPOSITION_Finish			( WPARAM wParam, LPARAM lParam );

		//----------------------------------------------------------------------------

	static			BOOL	Check_WM_IME_NOTIFY						( WPARAM wParam, LPARAM lParam );

private:
	static			void	ClearCandidateInfo						( void );
	static			int		LoadCandidateInfo						( HIMC hImc );
	static			void	GetPrivateReadingString					( void );

    static			WORD	GetLanguage								( void )	{ return LOWORD( m_hKLCurrent ); }					// 언어값 받기
    static			WORD	GetPrimaryLanguage						( void )	{ return PRIMARYLANGID( LOWORD( m_hKLCurrent ) ); }	// 주 언어값 받기
    static			WORD	GetSubLanguage							( void )	{ return SUBLANGID( LOWORD( m_hKLCurrent ) ); }		// 보조 언어값 받기	

	//----------------------------------------------------------------------------
	//	오버로딩 함수 그룹(함수 포인터를 이용한 DLL 접합)
	//----------------------------------------------------------------------------
	// 실질적 작동 함수(DLL 로딩)
	static			INPUTCONTEXT*	( WINAPI* _ImmLockIMC )			( HIMC );
	static			BOOL			( WINAPI* _ImmUnlockIMC )		( HIMC );
	static			LPVOID			( WINAPI* _ImmLockIMCC )		( HIMCC );
	static			BOOL			( WINAPI* _ImmUnlockIMCC )		( HIMCC );
	static			UINT			( WINAPI* _GetReadingString )	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	static			BOOL			( WINAPI* _ShowReadingWindow )	( HIMC, BOOL );
	// 껍데기 함수
    static			INPUTCONTEXT*	WINAPI Dummy_ImmLockIMC			( HIMC )									{ return NULL; }
    static			BOOL			WINAPI Dummy_ImmUnlockIMC		( HIMC )									{ return FALSE; }
    static			LPVOID			WINAPI Dummy_ImmLockIMCC		( HIMCC )									{ return NULL; }
    static			BOOL			WINAPI Dummy_ImmUnlockIMCC		( HIMCC )									{ return FALSE; }
    static			UINT			WINAPI Dummy_GetReadingString	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT )	{ return 0; }
    static			BOOL			WINAPI Dummy_ShowReadingWindow	( HIMC, BOOL )								{ return FALSE; }

};

#endif//__IME_BASE_H__