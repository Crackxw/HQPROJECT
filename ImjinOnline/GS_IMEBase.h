//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - IME ���̽�
//	File Name		: GS_IMEBase.h
//	Birth Date		: 2004. 04. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �� ����Ʈ �ڽ����� ���� IME ����� �����Ѵ�.
//
//===================================================================================================

#ifndef __IME_BASE_H__
#define __IME_BASE_H__

#define					DF_VALUE_MAX_CANDIDATE		10
#define					DF_VALUE_IME_BUFFER_SIZE	256

extern	const	HKL		DF_VALUE_HKL_CHT_DEFAULT;		// �߱��� (��ü) - Default
extern	const	HKL		DF_VALUE_HKL_CHS_USKEYBOARD;	// �߱��� (��ü) - ��ü US KeyBoard
extern	const	HKL		DF_VALUE_HKL_CHT_USKEYBOARD;	// �߱��� (��ü) - ��ü US KeyBoard
extern	const	HKL		DF_VALUE_HKL_CHS_DEFAULT;		// �߱��� (��ü) - Default

extern	const	HKL		DF_VALUE_HKL_CHT_PHONETIC;		// �߱��� (��ü) - Phonetic
extern	const	HKL		DF_VALUE_HKL_JPN_MSIME2002;		// �Ϻ��� �Է� �ý��� (MS-IME2002)
extern	const	HKL		DF_VALUE_HKL_KOR_MSIME2000;		// �ѱ��� �Է� �ý��� (IME 2000)
extern	const	HKL		DF_VALUE_HKL_CHS_QUANPIN;		// �߱��� (��ü) - QuanPin
extern	const	HKL		DF_VALUE_HKL_CHT_CHANGJIE;		// �߱��� (��ü) - ChangJie
extern	const	HKL		DF_VALUE_HKL_CHS_SHUANGPIN;		// �߱��� (��ü) - ShuangPin
extern	const	HKL		DF_VALUE_HKL_CHT_QUICK;			// �߱��� (��ü) - Quick
extern	const	HKL		DF_VALUE_HKL_CHS_ZHENGMA;		// �߱��� (��ü) - ZhengMa
extern	const	HKL		DF_VALUE_HKL_CHT_BIG5CODE;		// �߱��� (��ü) - Big5 Code
extern	const	HKL		DF_VALUE_HKL_CHS_INTELABC;		// �߱��� (��ü) - ���� ABC
extern	const	HKL		DF_VALUE_HKL_CHT_ARRAY;			// �߱��� (��ü) - Array
extern	const	HKL		DF_VALUE_HKL_CHS_NEIMA;			// �߱��� (��ü) - NeiMa
extern	const	HKL		DF_VALUE_HKL_CHT_DAYI;			// �߱��� (��ü) - DaYi
extern	const	HKL		DF_VALUE_HKL_CHT_UNICODE;		// �߱��� (��ü) - Unicode
extern	const	HKL		DF_VALUE_HKL_CHT_NEWPHONETIC;	// �߱��� (��ü) - New Phonetic
extern	const	HKL		DF_VALUE_HKL_CHT_NEWCHANGJIE;	// �߱��� (��ü) - New ChangJie
extern	const	HKL		DF_VALUE_HKL_CHS_MSPY;			// �߱��� (��ü) - Microsoft Pinyin IME 3.0
extern	const	HKL		DF_VALUE_HKL_CHT_ALPHANUMERIC;	// �߱��� (��ü) - Alphanumeric
extern	const	HKL		DF_VALUE_HKL_KOR_MSIME2003;		// �ѱ��� �Է� �ý��� (MS-IME2003)

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: ����â/����â���� ���� ����ü
//-----------------------------------------------------------------------------
struct ST_IME_CANDIDATE_INFO
{
	char		m_pszStringBuffer[DF_VALUE_MAX_CANDIDATE][DF_VALUE_IME_BUFFER_SIZE];	// �ѹ��� ������ ���� ����Ʈ
	DWORD		m_dwPageSize;															// �� ����â�� ������ �� �ִ� �ܾ��� �ִ� ����
	DWORD		m_dwCount;															    // �� ����â�� ����ִ� �ܾ��� ����
	DWORD		m_dwSelection;															// �� ����â���� ������ ���� �ܾ��� ��ġ
};

class GS_IMEBase
{
private:
	//----------------------------------------------------------------------------
	//	���� ���� �׷�
	//----------------------------------------------------------------------------

	static	HWND		m_hWnd;											// �� IME �ý����� �θ� ������ �ڵ�
	static	HIMC		m_hImcDefault;									// �� IME �ý��ۿ� �Ҵ�� �⺻ IMC �ڵ�
	static	HINSTANCE	m_hDllImm32;									// IMM32.DLL�� �ν��Ͻ� �ڵ�
	static	HINSTANCE	m_hDllIme;										// ������ IME ����� �ν��Ͻ� �ڵ�
	static	HKL			m_hKLCurrent;									// ���� Ű���� �Է±� �ڵ�


	//----------------------------------------------------------------------------
	//	���� ���� �׷� - �ܺ� �˸�
	//----------------------------------------------------------------------------
	
	static	UINT		m_uiCodePage;									// ������ IME���� ����ؾ� �� �ڵ������� ��
	static	int			m_enImeStatus;									// ������ IME ����
	static	char		m_pszInputLocaleName[4];						// ������ �Է� ������ ��Ī
	static	char		m_pszImeLayoutName[128];						// ������ IME ��±� ��Ī

	static	BOOL		m_bCandidateTypeVer;							// ���η� ��µǴ� ����â �����̸� true, ���ζ�� false
	static	BOOL		m_bCandidateWindowOn;							// ����â�� �����ٸ� true, �����ٸ� false
	static	BOOL		m_bReadingWindowOn;								// ����â�� �����ٸ� true, �����ٸ� false

	static	BOOL		m_bFirstComposing;								// ���� �۾��� ó������ ���õ� �����ΰ�?
	static	BOOL		m_bIsComposing;									// ���� �������� �����ΰ�?


	//----------------------------------------------------------------------------
	//	���� ���� �׷�
	//----------------------------------------------------------------------------
	static	int			m_siCompCaretPos;								// ���� ���¿����� Ŀ�� ���� ��ġ
	static	int			m_siLenResultBuff;								// ����� ��Ʈ���� ���ڱ���
	static	int			m_siLenCompBuff;								// ���հ� ��Ʈ���� ���ڱ���


	static	char		m_pszResultBuff[DF_VALUE_IME_BUFFER_SIZE];		// ��������� ������ ��Ʈ���� �ӽ� ������ ����
	static	char		m_pszCompBuff[DF_VALUE_IME_BUFFER_SIZE];		// ���հ����� ������ ��Ʈ���� �ӽ� ������ ����
	static	char		m_pszCompAttrBuff[DF_VALUE_IME_BUFFER_SIZE];	// ���հ��� ��Ʈ���� �Ӽ����� �ӽ� ������ ����
	static	char		m_pszReadingBuff[DF_VALUE_IME_BUFFER_SIZE];		// �������ڷ� ������ ��Ʈ���� �ӽ� ������ ����
	
	static	ST_IME_CANDIDATE_INFO	m_stCandidateInfo;					// ����â ���� ����


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
	//	���� �Լ�
	//----------------------------------------------------------------------------
	static			HKL		GetCurrentKeyboardLocale( void )		{ return m_hKLCurrent; }						// ������ �Է� ������ �ڵ� ���
	static	const	char*	GetInputLocaleName		( void )		{ return (const char*)m_pszInputLocaleName; }	// ������ �Է� ������ ��Ī ���
	static	const	char*	GetImeLayoutName		( void )		{ return (const char*)m_pszImeLayoutName; }		// ���� ������ IME ��Ī ���
	static			UINT	GetCodePage				( void )		{ return m_uiCodePage; }						// ���� ������ �ڵ� ������ ���
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
	//	IME ��� �� ���¿� ���õ� ����
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

	static			BOOL	IsFirstIMEComposing		( void )		{ return m_bFirstComposing; }	// IME�� ���� ù ���������� ���°�?
	static			BOOL	IsIMEComposing			( void )		{ return m_bIsComposing; }		// IME�� ���� �������� �����ΰ�?

	static	const	char*	GetReadingStringData	( void );
	static			BOOL	IsOpenReadingString		( void )		{ return m_bReadingWindowOn; }
	
	static			void	SetCandidateStatusOpen	( void );
	static			void	SetCandidateStatusClose	( void );
	static	const	char*	GetCandidateString		( int iIndex );
	static			DWORD	GetCandidatePageSize	( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwPageSize	: (DWORD)-1 ); }	// �� ����â�� ������������ ������ �� �ִ� �ִ� ������ �����Ѵ�.
	static			DWORD	GetCandidateCount		( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwCount		: (DWORD)-1 ); }	// �� ����â�� ���� �������� �ܾ� ������ �����Ѵ�.
	static			DWORD	GetCandidateSelection	( void )		{ return ( ( m_bCandidateWindowOn )	? m_stCandidateInfo.m_dwSelection	: (DWORD)-1 ); }	// �� ����â���� ���õ� �׸��� �����Ѵ�.
	static			BOOL	IsOpenCandidate			( void )		{ return m_bCandidateWindowOn; }															// Ư������ ���� â�� ���ִ� �����ΰ�?
	static			BOOL	IsCandidateVertical		( void )		{ return m_bCandidateTypeVer; }																// ����â ǥ�� Ÿ���� �����ΰ� �����ΰ�?
	static			BOOL	IsCandidateExType		( void );



	//----------------------------------------------------------------------------
	//	IME ���� Ȯ���� �����͸� ������ ����
	//----------------------------------------------------------------------------

	static	const	char*	GetResultStringData		( void )		{ return ( (const char*)m_pszResultBuff ); }	// ���տϷ� ��Ʈ���� �޴´�.
	static	const	char*	GetComposingStringData	( void )		{ return ( (const char*)m_pszCompBuff ); }		// �������� ��Ʈ���� �޴´�.
	static	const	char*	GetComposingAttrData	( void )		{ return ( (const char*)m_pszCompAttrBuff ); }	// �������� ��Ʈ���� �Ӽ��� �޴´�.
	static			int		GetComposingCaretPos	( void )		{ return m_siCompCaretPos; }					// �������� Ŀ���� ��ġ�� �޴´�.

	static			int		GetResultStringLen		( void )		{ return m_siLenResultBuff; }					// ���տϷ� ��Ʈ���� ũ�⸦ �޴´�.
	static			int		GetComposingStringLen	( void )		{ return m_siLenCompBuff; }						// �������� ��Ʈ���� ũ�⸦ �޴´�.



	//----------------------------------------------------------------------------
	//	���ν����� �޽����ο� ��ġ�ؾ� �� �͵�
	//----------------------------------------------------------------------------
public:
	static			BOOL	Check_WM_INPUTLANGCHANGE				( LPARAM lParam );
	static			BOOL	Check_WM_IME_STARTCOMPOSITION			( LPARAM lParam );
	static			BOOL	Check_WM_IME_ENDCOMPOSITION				( LPARAM lParam );

		//----------------------------------------------------------------------------
		//	actdoll (2004/10/27 16:52) : 
		//		����! �� Check_WM_IME_COMPOSITION_...()�κ��� 
		//		WM_IME_COMPOSITION �޽����� �������� �κп� �ݵ�� ���������� ��ġ�Ǿ�� �մϴ�.
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

    static			WORD	GetLanguage								( void )	{ return LOWORD( m_hKLCurrent ); }					// �� �ޱ�
    static			WORD	GetPrimaryLanguage						( void )	{ return PRIMARYLANGID( LOWORD( m_hKLCurrent ) ); }	// �� �� �ޱ�
    static			WORD	GetSubLanguage							( void )	{ return SUBLANGID( LOWORD( m_hKLCurrent ) ); }		// ���� �� �ޱ�	

	//----------------------------------------------------------------------------
	//	�����ε� �Լ� �׷�(�Լ� �����͸� �̿��� DLL ����)
	//----------------------------------------------------------------------------
	// ������ �۵� �Լ�(DLL �ε�)
	static			INPUTCONTEXT*	( WINAPI* _ImmLockIMC )			( HIMC );
	static			BOOL			( WINAPI* _ImmUnlockIMC )		( HIMC );
	static			LPVOID			( WINAPI* _ImmLockIMCC )		( HIMCC );
	static			BOOL			( WINAPI* _ImmUnlockIMCC )		( HIMCC );
	static			UINT			( WINAPI* _GetReadingString )	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	static			BOOL			( WINAPI* _ShowReadingWindow )	( HIMC, BOOL );
	// ������ �Լ�
    static			INPUTCONTEXT*	WINAPI Dummy_ImmLockIMC			( HIMC )									{ return NULL; }
    static			BOOL			WINAPI Dummy_ImmUnlockIMC		( HIMC )									{ return FALSE; }
    static			LPVOID			WINAPI Dummy_ImmLockIMCC		( HIMCC )									{ return NULL; }
    static			BOOL			WINAPI Dummy_ImmUnlockIMCC		( HIMCC )									{ return FALSE; }
    static			UINT			WINAPI Dummy_GetReadingString	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT )	{ return 0; }
    static			BOOL			WINAPI Dummy_ShowReadingWindow	( HIMC, BOOL )								{ return FALSE; }

};

#endif//__IME_BASE_H__