//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - IME ���̽�
//	File Name		: GS_IMEBase.cpp
//	Birth Date		: 2004. 04. 14.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż� �� ����Ʈ �ڽ����� ���� IME ����� �����Ѵ�.
//
//===================================================================================================

#include <GSL.h>

#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"

//-----------------------------------------------------------------------------
// Name: GS_IMEBase
// Code: actdoll (2004-10-20)
// Desc: ��ũ��
//-----------------------------------------------------------------------------
// �Լ������� ����
#define MC_FUNC_SETPROC( Module, APIName, Temp )	{ Temp = GetProcAddress( Module, #APIName );	if( Temp )	{ *(FARPROC*)&_##APIName = Temp; } }
// �Լ������� ����
#define MC_FUNC_KILLPROC( APIName )					{ _##APIName = Dummy_##APIName; }
// IME ������ ���ϱ�
#define MAKEIMEVERSION( major, minor )				( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

//-----------------------------------------------------------------------------
// Name: GS_IMEBase
// Code: actdoll (2004-10-20)
// Desc: ���ǵ�
//-----------------------------------------------------------------------------
// IME ���� �Ӽ�
enum	EN_IME_STATUS
{
	EN_IME_STATUS_INVALID	= -1						,	// ������
	EN_IME_STATUS_INIT		= 0							,	// �ʱⰪ
	EN_IME_STATUS_FIRST		= 0							,	// ó����

	EN_IME_STATUS_OFF		= EN_IME_STATUS_FIRST		,	// IME ���� ����
	EN_IME_STATUS_ON									,	// IME ����(������ ��� ���)
	EN_IME_STATUS_ENGLISH								,	// IME ����(���� ���)

	EN_IME_STATUS_LAST		= EN_IME_STATUS_ENGLISH		,	// ������ ��
	EN_IME_STATUS_MAX											// �� ����
};

const	char*	DF_FILENAME_IMM32_DLL	=	"\\imm32.dll";	// IME DLL ���ϸ�

// ����ü - �븸, ȫ�� �迭
const	char*	DF_FILENAME_IME_CHT1	=	"TINTLGNT.IME";	// New Phonetic
const	char*	DF_FILENAME_IME_CHT2	=	"CINTLGNT.IME";	// New Chang Jie
const	char*	DF_FILENAME_IME_CHT3	=	"MSTCIPHA.IME";	// Phonetic 5.1
// ����ü - �߱� ���� �迭
const	char*	DF_FILENAME_IME_CHS1	=	"PINTLGNT.IME";	// MSPY1.5/2/3
const	char*	DF_FILENAME_IME_CHS2	=	"MSSCIPYA.IME";	// MSPY3 for OfficeXP

// Ű���� �Է±� ���ǰ�
const	HKL		DF_VALUE_HKL_CHT_DEFAULT		=	( (HKL)(INT_PTR)0x04040404 );	// �߱��� (��ü) - Default
const	HKL		DF_VALUE_HKL_CHS_USKEYBOARD		=	( (HKL)(INT_PTR)0x04040804 );	// �߱��� (��ü) - ��ü US KeyBoard
const	HKL		DF_VALUE_HKL_CHT_USKEYBOARD		=	( (HKL)(INT_PTR)0x08040404 );	// �߱��� (��ü) - ��ü US KeyBoard
const	HKL		DF_VALUE_HKL_CHS_DEFAULT		=	( (HKL)(INT_PTR)0x08040804 );	// �߱��� (��ü) - Default

const	HKL		DF_VALUE_HKL_CHT_PHONETIC		=	( (HKL)(INT_PTR)0xE0010404 );	// �߱��� (��ü) - Phonetic
const	HKL		DF_VALUE_HKL_JPN_MSIME2002		=	( (HKL)(INT_PTR)0xE0010411 );	// �Ϻ��� �Է� �ý��� (MS-IME2002)
const	HKL		DF_VALUE_HKL_KOR_MSIME2000		=	( (HKL)(INT_PTR)0xE0010412 );	// �ѱ��� �Է� �ý��� (IME 2000)
const	HKL		DF_VALUE_HKL_CHS_QUANPIN		=	( (HKL)(INT_PTR)0xE0010804 );	// �߱��� (��ü) - QuanPin
const	HKL		DF_VALUE_HKL_CHT_CHANGJIE		=	( (HKL)(INT_PTR)0xE0020404 );	// �߱��� (��ü) - ChangJie
const	HKL		DF_VALUE_HKL_CHS_SHUANGPIN		=	( (HKL)(INT_PTR)0xE0020804 );	// �߱��� (��ü) - ShuangPin
const	HKL		DF_VALUE_HKL_CHT_QUICK			=	( (HKL)(INT_PTR)0xE0030404 );	// �߱��� (��ü) - Quick
const	HKL		DF_VALUE_HKL_CHS_ZHENGMA		=	( (HKL)(INT_PTR)0xE0030804 );	// �߱��� (��ü) - ZhengMa
const	HKL		DF_VALUE_HKL_CHT_BIG5CODE		=	( (HKL)(INT_PTR)0xE0040404 );	// �߱��� (��ü) - Big5 Code
const	HKL		DF_VALUE_HKL_CHS_INTELABC		=	( (HKL)(INT_PTR)0xE0040804 );	// �߱��� (��ü) - ���� ABC
const	HKL		DF_VALUE_HKL_CHT_ARRAY			=	( (HKL)(INT_PTR)0xE0050404 );	// �߱��� (��ü) - Array
const	HKL		DF_VALUE_HKL_CHS_NEIMA			=	( (HKL)(INT_PTR)0xE0050804 );	// �߱��� (��ü) - NeiMa
const	HKL		DF_VALUE_HKL_CHT_DAYI			=	( (HKL)(INT_PTR)0xE0060404 );	// �߱��� (��ü) - DaYi
const	HKL		DF_VALUE_HKL_CHT_UNICODE		=	( (HKL)(INT_PTR)0xE0070404 );	// �߱��� (��ü) - Unicode
const	HKL		DF_VALUE_HKL_CHT_NEWPHONETIC	=	( (HKL)(INT_PTR)0xE0080404 );	// �߱��� (��ü) - New Phonetic
const	HKL		DF_VALUE_HKL_CHT_NEWCHANGJIE	=	( (HKL)(INT_PTR)0xE0090404 );	// �߱��� (��ü) - New ChangJie
const	HKL		DF_VALUE_HKL_CHS_MSPY			=	( (HKL)(INT_PTR)0xE00E0804 );	// �߱��� (��ü) - Microsoft Pinyin IME 3.0
const	HKL		DF_VALUE_HKL_CHT_ALPHANUMERIC	=	( (HKL)(INT_PTR)0xE01f0404 );	// �߱��� (��ü) - Alphanumeric
const	HKL		DF_VALUE_HKL_KOR_MSIME2003		=	( (HKL)(INT_PTR)0xE0200412 );	// �ѱ��� �Է� �ý��� (MS-IME2003)

// �߱���� IME ID ����
const	WORD	LANG_CHT			=	MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
const	WORD	LANG_CHS			=	MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
const	DWORD	IMEID_CHT_VER42		=	( LANG_CHT | MAKEIMEVERSION( 4, 2 ) );	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
const	DWORD	IMEID_CHT_VER43		=	( LANG_CHT | MAKEIMEVERSION( 4, 3 ) );	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
const	DWORD	IMEID_CHT_VER44		=	( LANG_CHT | MAKEIMEVERSION( 4, 4 ) );	// New ChanJie IME98b          : 4.4.x.x // WinXP
const	DWORD	IMEID_CHT_VER50		=	( LANG_CHT | MAKEIMEVERSION( 5, 0 ) );	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
const	DWORD	IMEID_CHT_VER51		=	( LANG_CHT | MAKEIMEVERSION( 5, 1 ) );	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
const	DWORD	IMEID_CHT_VER52		=	( LANG_CHT | MAKEIMEVERSION( 5, 2 ) );	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
const	DWORD	IMEID_CHT_VER60		=	( LANG_CHT | MAKEIMEVERSION( 6, 0 ) );	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
const	DWORD	IMEID_CHS_VER41		=	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) );	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
const	DWORD	IMEID_CHS_VER42		=	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) );	// MSPY2	// Win2k/WinME
const	DWORD	IMEID_CHS_VER53		=	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) );	// MSPY3	// WinXP

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: ����ƽ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
HWND						GS_IMEBase::m_hWnd					= NULL;							// �� IME �ý����� �θ� ������ �ڵ�
HIMC						GS_IMEBase::m_hImcDefault			= NULL;							// �� IME �ý��ۿ� �Ҵ�� �⺻ IMC �ڵ�
HINSTANCE					GS_IMEBase::m_hDllImm32				= NULL;							// IMM32.DLL�� �ν��Ͻ� �ڵ�
HINSTANCE					GS_IMEBase::m_hDllIme				= NULL;							// ������ IME ����� �ν��Ͻ� �ڵ�
HKL							GS_IMEBase::m_hKLCurrent			= NULL;							// ���� Ű���� �Է±� �ڵ�

UINT						GS_IMEBase::m_uiCodePage			= CP_ACP;						// ������ IME���� ����ؾ� �� �ڵ������� ��
int							GS_IMEBase::m_enImeStatus			= EN_IME_STATUS_INIT;			// ������ IME ����
char						GS_IMEBase::m_pszInputLocaleName[4]	= {0,};							// ������ �Է� ������ ��Ī
char						GS_IMEBase::m_pszImeLayoutName[128]	= {0,};							// ������ IME ��±� ��Ī

BOOL						GS_IMEBase::m_bReadingWindowOn		= false;						// ����â�� �۵����� ���� ����
BOOL						GS_IMEBase::m_bCandidateWindowOn	= false;						// ����â�� �۵����� ���� ����
BOOL						GS_IMEBase::m_bCandidateTypeVer		= false;						// ����â�� ���� ���� ��� ����

BOOL						GS_IMEBase::m_bFirstComposing		= false;						// ���� �۾��� ó������ ���õ� �����ΰ�?
BOOL						GS_IMEBase::m_bIsComposing			= false;						// ���� �������� �����ΰ�?

int							GS_IMEBase::m_siCompCaretPos		= 0;							// ���� ���¿����� Ŀ�� ���� ��ġ
int							GS_IMEBase::m_siLenResultBuff		= 0;							// ����� ��Ʈ���� ���ڱ���
int							GS_IMEBase::m_siLenCompBuff			= 0;							// ���հ� ��Ʈ���� ���ڱ���

char						GS_IMEBase::m_pszResultBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// ��������� ������ ��Ʈ���� �ӽ� ������ ����							
char						GS_IMEBase::m_pszCompBuff[DF_VALUE_IME_BUFFER_SIZE]			= {0,};	// ���հ����� ������ ��Ʈ���� �ӽ� ������ ����							
char						GS_IMEBase::m_pszCompAttrBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// ���հ��� ��Ʈ���� �Ӽ����� �ӽ� ������ ����							
char						GS_IMEBase::m_pszReadingBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// �������ڷ� ������ ��Ʈ���� �ӽ� ������ ����							

ST_IME_CANDIDATE_INFO		GS_IMEBase::m_stCandidateInfo		= {0,};							// ����â ���� ����


//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: �Լ������� �ʱ�ȭ
//-----------------------------------------------------------------------------
// ���� IME �۵��� �ʿ��� ȣ�� �Լ�(DLL���� ���� ����)
INPUTCONTEXT*	( WINAPI*	GS_IMEBase::_ImmLockIMC )		( HIMC )									= GS_IMEBase::Dummy_ImmLockIMC;
BOOL			( WINAPI*	GS_IMEBase::_ImmUnlockIMC )		( HIMC )									= GS_IMEBase::Dummy_ImmUnlockIMC;
LPVOID			( WINAPI*	GS_IMEBase::_ImmLockIMCC )		( HIMCC )									= GS_IMEBase::Dummy_ImmLockIMCC;
BOOL			( WINAPI*	GS_IMEBase::_ImmUnlockIMCC )	( HIMCC )									= GS_IMEBase::Dummy_ImmUnlockIMCC;
// �߱���� IME �۵��� �ʿ��� ȣ�� �Լ�(DLL���� ���� ����)
UINT			( WINAPI*	GS_IMEBase::_GetReadingString )	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT )	= GS_IMEBase::Dummy_GetReadingString;
BOOL			( WINAPI*	GS_IMEBase::_ShowReadingWindow )( HIMC, BOOL )								= GS_IMEBase::Dummy_ShowReadingWindow;


#define		DF_VALUE_IME_DEFAULT_ATTR		0xFF				// IME ���� �Ӽ����� ����Ʈ �Ӽ�

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: ������ Ȱ��ȭ�� ���� ���� ���� �ѹ� ȣ��
//-----------------------------------------------------------------------------
GS_IMEBase		g_clImeBase;

//-----------------------------------------------------------------------------
// Name: GS_IMEBase()
// Code: actdoll (2004-04-17)
// Desc: ������
//-----------------------------------------------------------------------------
GS_IMEBase::GS_IMEBase()
{
	// IMM ���� �Լ� ����
	InitializeImm();

	Clear();
}

//-----------------------------------------------------------------------------
// Name: GS_IMEBase()
// Code: actdoll (2004-04-17)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
GS_IMEBase::~GS_IMEBase()
{
	Clear();

	// IMM ���� �Լ� ����
	ReleaseImm();
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-10-20)
// Desc: �ʱ�ȭ�� �ѹ��� ���ָ� �ȴ�.
//		hWnd	- ���� ������ �ڵ鰪
//-----------------------------------------------------------------------------
void	GS_IMEBase::Initialize( HWND hWnd )
{
	if( !hWnd )				return;

	m_hWnd	= hWnd;
	CheckDefaultImc();
	UpdateInputStatus();
}

//-----------------------------------------------------------------------------
// Name: Clear()
// Code: actdoll (2004-04-17)
// Desc: 
//	���� �� Ŭ������ ����ִ� ���� �������� �ʱ�ȭ��Ų��.
//-----------------------------------------------------------------------------
void	GS_IMEBase::Clear()
{
	SetCompStatusCancel();		// �������� ���ڰ� ������ �� ������.
	SetCandidateStatusClose();	// ����â�� �����ִٸ� �ݾ��ش�.

	m_bCandidateWindowOn	= false;
	m_bReadingWindowOn		= false;

	m_bFirstComposing		= false;
	m_bIsComposing			= false;

	m_siCompCaretPos		= 0;
	m_siLenResultBuff		= 0;
	m_siLenCompBuff			= 0;

	ZeroMemory	( m_pszResultBuff,		sizeof(m_pszResultBuff) );
	ZeroMemory	( m_pszCompBuff,		sizeof(m_pszCompBuff) );
	memset		( m_pszCompAttrBuff, DF_VALUE_IME_DEFAULT_ATTR, sizeof(m_pszResultBuff) );
	ZeroMemory	( m_pszReadingBuff,		sizeof( m_pszReadingBuff ) );

	ZeroMemory	( &m_stCandidateInfo,	sizeof(m_stCandidateInfo) );

	UpdateInputStatus();
}

//-----------------------------------------------------------------------------
// Name: InitializeImm()
// Code: actdoll (2004-10-20)
// Desc: IMM DLL ���� �Լ����� �ε�
//-----------------------------------------------------------------------------
void	GS_IMEBase::InitializeImm()
{
	if( m_hDllImm32 )										return;

	// �ý��� ��� �ް�
	char	pszPath[MAX_PATH + 1];
	if( !::GetSystemDirectoryA( pszPath, MAX_PATH + 1 ) )	return;
	strcat( pszPath, DF_FILENAME_IMM32_DLL );

	// ���� IME�� ����, DLL�� �ε��Ͽ� ���ǰ� �ȵ� IMM �Լ������ �����Ѵ�.
    m_hDllImm32		= LoadLibrary( pszPath );
    if( m_hDllImm32 )
    {
	    FARPROC		Temp;
        MC_FUNC_SETPROC( m_hDllImm32, ImmLockIMC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmUnlockIMC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmLockIMCC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmUnlockIMCC,	Temp );
    }

	// IMM ����� Ȯ���Ѵ�.
	CheckImeModule();
}

//-----------------------------------------------------------------------------
// Name: ReleaseImm()
// Code: actdoll (2004-10-20)
// Desc: IMM DLL ���� �Լ����� ����
//-----------------------------------------------------------------------------
void	GS_IMEBase::ReleaseImm()
{
	if( m_hDllImm32 )
	{
		MC_FUNC_KILLPROC( ImmLockIMC );
		MC_FUNC_KILLPROC( ImmUnlockIMC );
		MC_FUNC_KILLPROC( ImmLockIMCC );
		MC_FUNC_KILLPROC( ImmUnlockIMCC );

		FreeLibrary( m_hDllImm32 );
		m_hDllImm32		= NULL;
	}

	if( m_hDllIme )
	{
		MC_FUNC_KILLPROC( GetReadingString );
		MC_FUNC_KILLPROC( ShowReadingWindow );		

		FreeLibrary( m_hDllIme );
		m_hDllIme		= NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: CheckImeModule()
// Code: actdoll (2004-10-20)
// Desc: IME DLL ���� �Լ��� �ε�
//	Ű���� �Է±Ⱚ�� ���� �߱��� ��� ����ϴ� Ư���� IME API �Լ��� 
//	�����ϸ� �ش� ����� �ε��Ͽ� �Լ��� ����/�����ش�.
//	�Է� �������� ����ɶ� ���� Ű���� �Է±⸦ Ȯ���ϰ� �� ������ �̰��� ó���Ѵ�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::CheckImeModule()
{
	// ���� �Լ���� �ʱ�ȭ
	_GetReadingString	= NULL;
	_ShowReadingWindow	= NULL;

	// �ش� IME ����� �ν��Ͻ��� Ȯ��
	char	pszPath[MAX_PATH + 1];
	if( !ImmGetIMEFileName( m_hKLCurrent, pszPath, MAX_PATH + 1 ) )		return;
	if( m_hDllIme )	FreeLibrary( m_hDllIme );
	m_hDllIme		= LoadLibrary( pszPath );

	// �Լ���� �ٽ� ����
	if( m_hDllIme )
	{
		FARPROC		Temp;
		MC_FUNC_SETPROC( m_hDllIme, GetReadingString,	Temp );
		MC_FUNC_SETPROC( m_hDllIme, ShowReadingWindow,	Temp );
	}
}

//-----------------------------------------------------------------------------
// Name: CheckDefaultImc()											[static]
// Code: actdoll (2004-10-20)
// Desc: 
//	�⺻ IMC ����̽��� ��´�.(�ݵ�� �ۿ��� �ʱ�ȭ�� �ѹ��� ����� ��� �Ѵ�)
//-----------------------------------------------------------------------------
void	GS_IMEBase::CheckDefaultImc()
{
	m_hImcDefault	= ImmGetContext( m_hWnd );
	ImmReleaseContext( m_hWnd, m_hImcDefault );
}

//-----------------------------------------------------------------------------
// Name: IsImeRun()
// Code: actdoll (2004-04-17)
// Desc: 
//	IME �ý��� ��ü�� ������������ üũ�Ѵ�. IsInputAlphabet()���� �ణ Ʋ����.
//	�����ϰ� IME �ý����� �����ϰ� �ִ����� Ȯ���Ϸ��� �� �Լ��� ����Ѵ�.
//		return	- �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsImeRun()
{
	return ( m_enImeStatus != EN_IME_STATUS_OFF );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: �Է±��� �� �ѱ��ΰ�?
//		return - �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleKorean()
{
	return ( GetPrimaryLanguage() == LANG_KOREAN );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: �Է±��� �� �Ϻ��ΰ�?
//		return - �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleJapanese()
{
	return ( GetPrimaryLanguage() == LANG_JAPANESE );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: �Է±��� �� �߹� ��ü�ΰ�?
//		return - �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleTradChinese()
{
	return ( GetLanguage() == LANG_CHT );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: �Է±��� �� �߹� ��ü�ΰ�?
//		return - �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleSimpChinese()
{
	return ( GetLanguage() == LANG_CHS );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: �Է±��� �� IME�� ���� ���������ΰ�?
//		return - �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleNonIMEType()
{
	return	( 
				!IsLocaleKorean()		&& !IsLocaleJapanese()		&&
				!IsLocaleTradChinese()	&& !IsLocaleTradChinese()
			);
}

//-----------------------------------------------------------------------------
// Name: IsInputNative()
// Code: actdoll (2004-04-17)
// Desc: 
//	���ĺ� ���ü��� �������ΰ�?
//		return	- �´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputAlphabet()
{
	// �߹� ��ü�� ��� �Է±� �� �ϳ��� 
	// ������ �Ǵµ� IME�� �ν��Ѵ�. ����ó�� �� ��
	if( GetLanguage() == LANG_CHT && m_hKLCurrent == DF_VALUE_HKL_CHT_ALPHANUMERIC )		return TRUE;

	return ( m_enImeStatus != EN_IME_STATUS_ON );
}

//-----------------------------------------------------------------------------
// Name: IsInputNative()
// Code: actdoll (2004-04-17)
// Desc: 
//	�ش� ������ ���ü��� �������ΰ�?
//		return	- �´ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputNative()
{
	DWORD	dwCon, dwSen;
	GetInputStatus( dwCon, dwSen );
	return ( dwCon & IME_CMODE_NATIVE );
}

//-----------------------------------------------------------------------------
// Name: IsInputNative()
// Code: actdoll (2004-04-17)
// Desc: 
//	īŸī�� ���ü��� �������ΰ�?
//		return	- īŸī����� true, ���󰡳���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputKatakana()
{
	DWORD	dwCon, dwSen;
	GetInputStatus( dwCon, dwSen );
	return ( dwCon & IME_CMODE_KATAKANA );
}

//-----------------------------------------------------------------------------
// Name: IsInputFullShape()
// Code: actdoll (2004-04-17)
// Desc: 
//	���� ���ü��� �������ΰ�?
//		return	- �����̶�� true, �ݰ��̶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputFullShape()
{
	DWORD	dwCon, dwSen;
	GetInputStatus( dwCon, dwSen );
	return ( dwCon & IME_CMODE_FULLSHAPE );
}

//-----------------------------------------------------------------------------
// Name: IsInputHanjaConvert()
// Code: actdoll (2004-04-17)
// Desc: 
//	���ں�ȯ ���ü��� �������ΰ�?
//		return	- �´ٸ� true, Ʋ���� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputHanjaConvert()
{
	DWORD	dwCon, dwSen;
	GetInputStatus( dwCon, dwSen );
	return ( dwCon & IME_CMODE_HANJACONVERT );
}

//-----------------------------------------------------------------------------
// Name: UpdateInputStatus()
// Code: actdoll (2004-10-20)
// Desc: �Է� ������ üũ
//	���� ������ IME�� � ���������� Ȯ���Ѵ�.
//	m_hKLCurrent, m_bCandidateTypeVer, 
//	m_enImeStatus, m_uiCodePage ���� �� �Լ��� ���� ����ȴ�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::UpdateInputStatus()
{
	// �ϴ� �Է� ������ ��ü�� ������ ������ ��� ���ο� ������ �����ϵ��� �Ѵ�.
	static HKL hklPrev	= 0;
	m_hKLCurrent		= GetKeyboardLayout( 0 );
	if( hklPrev != m_hKLCurrent )
	{
		hklPrev = m_hKLCurrent;

		// �� �޴´�.
		switch ( GetPrimaryLanguage() )
		{
			// �߱��� �迭
			case LANG_CHINESE:
				m_bCandidateTypeVer = true;
				switch ( GetSubLanguage() )
				{
					// �߱��� ��ü(����)
					case SUBLANG_CHINESE_SIMPLIFIED:
						m_bCandidateTypeVer = ( GetImeId() == 0 );
						break;
					// �߱��� ��ü(�븸, ȫ��)
					case SUBLANG_CHINESE_TRADITIONAL:
						break;
					// �� �� ���� ���̴�.
					default:
						break;
				}
				break;
			// �ѱ���
			case LANG_KOREAN:
				m_bCandidateTypeVer		= false;
				break;
			// �Ϻ���
			case LANG_JAPANESE:
				m_bCandidateTypeVer		= true;
				break;
			// �� �� ���� ��
			default:
				break;
		}

		// �Է� �������� ���� ��Ī�� �޴´�.
        GetLocaleInfo( MAKELCID( GetLanguage(), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, m_pszInputLocaleName, 4 );
		m_pszInputLocaleName[3] = 0;
		CharUpper( m_pszInputLocaleName );	// �빮�ڷ� ��ȯ��Ų��.
		

		// �� ��Ī�� ���´�.(������Ʈ������ ����)
		char	pszRegPath[MAX_PATH];
		HKEY	hKey;
		sprintf( pszRegPath, "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%x", m_hKLCurrent );
		LONG	lRc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, pszRegPath, 0, KEY_READ, &hKey );
		if( lRc == ERROR_SUCCESS )
		{
			DWORD	dwSize = DF_VALUE_IME_BUFFER_SIZE, dwType;
			lRc	= RegQueryValueEx( hKey, "Layout Text", NULL, &dwType, (PBYTE)m_pszImeLayoutName, &dwSize );
			if( lRc != ERROR_SUCCESS )	strcpy( m_pszImeLayoutName, "Unknown Type Keyboard Layout" );
			RegCloseKey( hKey );
		}


		// ���� ������ ������ ����� �ڵ��������� ��´�.
		char	pszCodePage[8];
		m_uiCodePage	= CP_ACP;
		if( GetLocaleInfo( MAKELCID( GetLanguage(), SORT_DEFAULT ), LOCALE_IDEFAULTANSICODEPAGE,	pszCodePage, 8 ) )
		{
			m_uiCodePage = strtoul( pszCodePage, NULL, 0 );
		}
	}

	// �� �Է� �������� IME �ý������� Ȯ��
	BOOL	bIme		= ImmIsIME( m_hKLCurrent );
	BOOL	bChineseIME	= ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

	HIMC	hImc;
	// IME ���ؽ�Ʈ�� �����Ѵٸ�
	if( hImc = ImmGetContext( m_hWnd ) )
	{
		// �߱���� IME�� ���
		if( bChineseIME )
		{
			DWORD	dwConvMode, dwSentMode;
			ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			// ���� ����� ��� ���� ���̰� �ƴ϶�� ���� ����̴�.
			m_enImeStatus = ( dwConvMode & IME_CMODE_NATIVE )		?	EN_IME_STATUS_ON	: EN_IME_STATUS_ENGLISH;
		}
		// �� ���� IME�� ���
		else
		{
			// OpenStatus�� �����ϸ� EN_IME_STATUS_ON, ���ٸ� EN_IME_STATUS_OFF
			m_enImeStatus = ( bIme && ImmGetOpenStatus( hImc ) )	?	EN_IME_STATUS_ON	: EN_IME_STATUS_ENGLISH;
		}
		ImmReleaseContext( m_hWnd, hImc );
	}
	// �� ���� IME�� ���ų� �Ϻη� �� ����̴�.
	else
	{
		m_enImeStatus = EN_IME_STATUS_OFF;
	}
}

//-----------------------------------------------------------------------------
// Name: GetImeId()
// Code: actdoll (2004-10-20)
// Desc: �ش� IME�� ID ���
//	��XP�������� ���ǵ��� �ʾҴ� ������ ID���� �˾ƿ��� ���ؼ� �츮�� ����
//	ã�ƾ� �ϴ� ��Ȳ�� �߻��� �� �ִ�. �� �Լ��� �� ��Ȳ���� ����Ѵ�.
//	�� ���� ������ CompareString() �Լ� ����.
//		return		- ���߱��� �Է·�����/������ �߱��� IME/�ٸ� ������ ��� 0����
//					- uIndex�� 0�� ��� 
//						bit 31-24:	Major version
//						bit 23-16:	Minor version
//						bit 15-0:	Language ID
//					- uIndex�� 1�� ���
//						( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS
//					������ ��������� �˰�ʹٸ� IMEID_VER & IMEID_LANG ��ũ�θ� ����� ��.
//-----------------------------------------------------------------------------
DWORD	GS_IMEBase::GetImeId( UINT uIndex )
{
	static	HKL		hklPrev = 0;			// ���� Ű���� �Է±� �ڵ�
	static	DWORD	dwID[2] = { 0, 0 };		// ���� ����� ĳ��

	DWORD   dwVerSize;
	DWORD   dwVerHandle;
	LPVOID  lpVerBuffer;
	LPVOID  lpVerData;
	UINT    cbVerData;
	char    szTmp[1024];

	if( uIndex >= 2 )				return 0;
	if( hklPrev == m_hKLCurrent )	return dwID[uIndex];

	// ���� ������ ����
	hklPrev = m_hKLCurrent;

	// �߱��� IME ���� �� ���� �ִ��� üũ
	if( !( ( m_hKLCurrent == DF_VALUE_HKL_CHT_NEWPHONETIC ) || ( m_hKLCurrent == DF_VALUE_HKL_CHT_NEWCHANGJIE ) || ( m_hKLCurrent == DF_VALUE_HKL_CHS_MSPY ) ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// IME �� ���� ���� �޾ƿ´�.
	if( !ImmGetIMEFileName( m_hKLCurrent, szTmp, 1023 ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// ���� ���� API(Reading String API)�� �����ϴ����� üũ
	LCID	lcEngBase	= MAKELCID( MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ), SORT_DEFAULT );
	if ( !_GetReadingString )
	{
		if( ( CompareStringA( lcEngBase, NORM_IGNORECASE, szTmp, -1, DF_FILENAME_IME_CHT1, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( lcEngBase, NORM_IGNORECASE, szTmp, -1, DF_FILENAME_IME_CHT2, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( lcEngBase, NORM_IGNORECASE, szTmp, -1, DF_FILENAME_IME_CHT3, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( lcEngBase, NORM_IGNORECASE, szTmp, -1, DF_FILENAME_IME_CHS1, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( lcEngBase, NORM_IGNORECASE, szTmp, -1, DF_FILENAME_IME_CHS2, -1 ) != CSTR_EQUAL ) )
		{
			dwID[0] = dwID[1] = 0;
			return dwID[uIndex];
		}
	}

	// ���� ���� ����
	dwVerSize = GetFileVersionInfoSize( szTmp, &dwVerHandle );
	if( dwVerSize )
	{
		lpVerBuffer = HeapAlloc( GetProcessHeap(), 0, dwVerSize );
		if( lpVerBuffer )
		{
			if( GetFileVersionInfo( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) )
			{
				if( VerQueryValue( lpVerBuffer, "\\", &lpVerData, &cbVerData ) )
				{
					DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
					dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
					if( _GetReadingString						||
						(	GetLanguage() == LANG_CHT			&&
							(	
								dwVer == MAKEIMEVERSION(4, 2)	|| 
								dwVer == MAKEIMEVERSION(4, 3)	|| 
								dwVer == MAKEIMEVERSION(4, 4)	|| 
								dwVer == MAKEIMEVERSION(5, 0)	||
								dwVer == MAKEIMEVERSION(5, 1)	||
								dwVer == MAKEIMEVERSION(5, 2)	||
								dwVer == MAKEIMEVERSION(6, 0) 
							) 
						)										||
						(	GetLanguage() == LANG_CHS			&&
							(	
								dwVer == MAKEIMEVERSION(4, 1)	||
								dwVer == MAKEIMEVERSION(4, 2)	||
								dwVer == MAKEIMEVERSION(5, 3)
							) 
						)
					  )
					{
						dwID[0] = dwVer | GetLanguage();
						dwID[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
					}
				}
			}
			HeapFree( GetProcessHeap(), 0, lpVerBuffer );
		}
	}

	return dwID[uIndex];
}

//-----------------------------------------------------------------------------
// Name: SetImeDeviceOn()											[static]
// Code: actdoll (2004-10-20)
// Desc: 
//	IMC ��ġ�� �츱 ������ ���� �������� üũ�Ѵ�.
//	���̰� �� ��� ���� ��ġ�� IME�� ������� ���� �������� �ʴ´�. �� ������ �����ϴ�.
//		bDeviceOn	- ������Ų�ٸ� true, �����Ѵٸ� false
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetImeDeviceOn( BOOL bDeviceOn )
{
	// IME �۵��� ����
	if( !bDeviceOn )
	{
		ImmAssociateContext( m_hWnd, NULL );
	}
	// IME �۵��� �Ҵ�
	else
	{
		HIMC	hImc = ImmGetContext( m_hWnd );
		ImmReleaseContext( m_hWnd, hImc );
		if( hImc != m_hImcDefault )
		{
			ImmAssociateContext( m_hWnd, NULL );
			ImmAssociateContext( m_hWnd, m_hImcDefault );
		}
	}

	// ������ IME ������ �����Ѵ�.
	UpdateInputStatus();
}

//-----------------------------------------------------------------------------
// Name: SetIMEStatus()
// Code: actdoll (2004-04-17)
// Desc: 
//	IME�� ����� �� ���·� �� ������ �� ���·� �� �������� �����Ѵ�.
//		bOpen	- �� ���·� �Ѵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetIMEStatus( BOOL bOpen )
{
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmSetOpenStatus( hImc, bOpen );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: IsIMEOn()
// Code: actdoll (2004-04-17)
// Desc: 
//	IME�� ����� �����ִ����� üũ�Ѵ�.
//		return	- IME�� �����ִ� ���¶�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsIMEOn()
{
	if( !m_hWnd )		return FALSE;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	BOOL	bRet	= ImmGetOpenStatus( hImc );
	ImmReleaseContext( m_hWnd, hImc );

	return bRet;
}

//-----------------------------------------------------------------------------
// Name: SetInputStatus()
// Code: actdoll (2004-04-17)
// Desc: 
//	���ϴ� �Է±� ���ü��� �����Ѵ�.
//		dwConv	- ������ ��ȯ ��(IME_CMODE_...)
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetInputStatus( DWORD dwConv )
{
	if( !m_hWnd )				return;
	
	DWORD	dwRet1, dwRet2;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmGetConversionStatus( hImc, &dwRet1, &dwRet2 );
	
	ImmSetConversionStatus( hImc, dwConv, dwRet2 );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: GetInputStatus()
// Code: actdoll (2004-04-17)
// Desc: 
//	���� ���õ� �Է±��� ���ü�踦 �޴´�.
//		dwConversion	- ������ �������ͽ��� ����
//		dwSentence		- ���ٽ� �������ͽ��� ����
//-----------------------------------------------------------------------------
void	GS_IMEBase::GetInputStatus( DWORD &dwConversion, DWORD &dwSentence )
{
	if( !m_hWnd )		return;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: Check_WM_INPUTLANGCHANGE()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_INPUTLANGCHANGE ���� ����
//	�Է� �������� �ٲ���� ��� �� �κ��� ���ؼ� ��ȣ�� ��������.
//		lParam		- LPARAM �Ķ���� ��
//		return		- ���� ��ȯ�� �־��ٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_INPUTLANGCHANGE( LPARAM lParam )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;

	// ���⼭ �ٲ� �Է������� Ȯ���Ѵ�.
	UpdateInputStatus();

	// IME�� ��ü�Ǿ������� ����Ͽ� ���ο� DLL ����� Ȯ���Ѵ�.
	CheckImeModule();

	// ����â API�� �����Ѵٸ� �ϴ� ��ɾ� ������!
	if( _ShowReadingWindow )
	{
		HIMC	hImc;
		if( hImc = ImmGetContext( m_hWnd ) )
		{
			_ShowReadingWindow( hImc, false );
			ImmReleaseContext( m_hWnd, hImc );
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_STARTCOMPOSITION()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_STARTCOMPOSITION���� ����
//	IME������ ���۵Ǿ��� �� ���ʷ� �������� �޽�����.
//		lParam		- LPARAM �Ķ���� ��
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_STARTCOMPOSITION( LPARAM lParam )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;

	m_bReadingWindowOn		= false;
	m_bFirstComposing		= true;
	m_bIsComposing			= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_STARTCOMPOSITION()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_STARTCOMPOSITION���� ����
//	IME������ ���۵Ǿ��� �� ���ʷ� �������� �޽�����.
//		lParam		- LPARAM �Ķ���� ��
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_ENDCOMPOSITION( LPARAM lParam )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;

	m_bReadingWindowOn		= false;
	m_bFirstComposing		= false;
	m_bIsComposing			= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_Start()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION�� ���� �պκп� ����
//	�� �ǹ̴� ������ ����� ó���� ��ȣ�� ���� �ް� �׽�Ʈ�ϱ� ���� �뵵�� ����� �� �ִ�.
//		wParam		- WPARAM �Ķ���� ��
//		lParam		- LPARAM �Ķ���� ��
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_Start( WPARAM wParam, LPARAM lParam )
{

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_ResultString()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION���� ����
//	�������� IME���� �ϼ��� ���ڿ��� �������� ��� �� ��Ʈ���� ���ϴ� �Լ���.
//	�޾ƿ� ��Ʈ���� m_pszResultBuff�� �ӽ� ����Ǹ�, �۾����� m_siLenResultBuff��
//	����ȴ�.
//		lParam		- LPARAM �Ķ���� ��
//		iCanRecvLen	- �Ѿ�� ���ڿ����� �󸶸�ŭ ���� ���ΰ�? -1�̶�� ���´�� �� �޾ƶ�
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_ResultString( LPARAM lParam, int iCanRecvLen )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_RESULTSTR ) )								return FALSE;

	ZeroMemory( m_pszResultBuff, sizeof(m_pszResultBuff) );
	m_siLenResultBuff	= 0;

	// ���ؽ�Ʈ �غ�
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 );
	m_bIsComposing	= false;

	// �ϼ��� �۾��� ���ų� ���� ��Ȳ�� �߻��ߴٸ� ����
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	bReturn	= false;

	// �ϼ��� �۾� �ޱ�(���� �� �ִ� ũ�Ⱑ 0�� �ƴ� ��츸 ���)
	if( bReturn && ( iRet > 0 && iCanRecvLen != 0 ) )
	{
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;
		m_siLenResultBuff	= ImmGetCompositionString( hImc, GCS_RESULTSTR, m_pszResultBuff, iRet );
		m_pszResultBuff[m_siLenResultBuff]	= 0;

		// ���� �� �ִ� ũ�Ⱑ ������ �ְ� ���� �۾��� ���� �۾����� ũ�ٸ� �۾��� ���� �ڸ���.
		if( iCanRecvLen > 0 && iRet > iCanRecvLen )
		{
			char	*pszNext, *pszPrev;
			pszNext = pszPrev = m_pszResultBuff;
			while(1)
			{
				pszNext = CharNextExA( m_uiCodePage, pszPrev, 0 );
				if( !pszNext )			break;
				if( pszNext - m_pszResultBuff > iCanRecvLen )	
				{
					*pszPrev	= 0;
					break;
				}
				pszPrev = pszNext;
			}
			m_siLenResultBuff	= strlen( m_pszResultBuff );
		}
	}

	// �ϼ��� �������ٴ� ���� ������ ���� ���ڿ��� �����ٴ� �ǹ��̹Ƿ� Ŭ���� �����ش�.
	ZeroMemory( m_pszCompBuff, sizeof(m_pszCompBuff) );
	memset( m_pszCompAttrBuff, DF_VALUE_IME_DEFAULT_ATTR, sizeof(m_pszCompAttrBuff) );

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_CompString()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION���� ����
//	�������� IME���� ���ջ��� ���ڿ��� �������� ��� �� ��Ʈ���� ���ϴ� �Լ���.
//	�޾ƿ� ��Ʈ���� m_pszCompBuff�� �ӽ� ����Ǹ�, �۾����� m_siLenCompBuff��
//	����ȴ�.
//		lParam		- LPARAM �Ķ���� ��
//		iRemainLen	- ���� ���ڰ� �� �׿������� ũ��. �̰��� 2����Ʈ �̸��̶�� ������ ���ڴ� ���� �� ����.
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompString( LPARAM lParam, int iRemainLen )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_COMPSTR ) )									return FALSE;

	ZeroMemory( m_pszCompBuff, sizeof(m_pszCompBuff) );
	m_siLenCompBuff	= 0;		

	// ���ؽ�Ʈ �غ�
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );

	// �ϼ��� �۾��� ���ų� ���� ��Ȳ�� �߻��ߴٸ� ����
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	bReturn = false;

	// ���� ù ���� ���¿��� ������ 2����Ʈ �̻� ���� ���� ��� ���Ͻ��ѹ�����.
	if( ( m_bFirstComposing || !m_bIsComposing ) && iRemainLen < 2 )
	{
		ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
		iRet	= 0;
		bReturn	= false;
	}

	// ���ڸ� �� ���� �� �ִ� ���¿��� ������ �߱��� ��ü IME�� ���չ��ڸ� ���� �������ڰ� ���´�.
	if( ( GetLanguage() == LANG_CHT || GetLanguage() == LANG_CHS ) && !GetImeId() && bReturn )
	{
		ZeroMemory( &m_pszReadingBuff, sizeof(m_pszReadingBuff) );

		// ���� ����ũ�⸦ �Ѿ�� �ȵȴ�.
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;

		// �ϴ� �������ڸ� �޴´�.
		iRet = ImmGetCompositionString( hImc, GCS_COMPSTR, m_pszReadingBuff, iRet );
		
		// �޾ƿ� ���ڰ� �ִٸ� ����â�� ������.
		m_bReadingWindowOn	= ( iRet > 0 );

		// �߹� ��ü�� ��� ����â���� ������ ���� ���¿��� iRet�� 0�� �´ٸ� �� ���ڿ� ���� ��� �۾��� ���� ���̴�.
		// �̶� ������ WM_IME_ENDCOMPOSITION�� �Ϻ��ϰ� �������� �ʴ´ٴ� ���̴�. 
		// ������ �߻��� ������ �ٺ��� �ֱ� ������ �ϴ� ���⼭ ���������� ���� �޽����� �����ش�.
		if( GetLanguage() == LANG_CHS && iRet <= 0 )
		{
			// ����â�� �����ִ� ���¶�� ���������� ����� ���̴�.
			if( !m_bCandidateWindowOn )	SetCompStatusCancel();
		}
	}
	// �׷��� ������ �ϼ��� �۾��� �޴´�.
	else if( iRet > 0 )
	{
		// ���� ����ũ�⸦ �Ѿ�� �ȵȴ�.
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;

		// �۾� �ޱ�
		m_siLenCompBuff	= ImmGetCompositionString( hImc, GCS_COMPSTR, m_pszCompBuff, iRet );
		m_pszCompBuff[m_siLenCompBuff]	= 0;
		m_bIsComposing	= true;
	}

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_CompAttribute()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION���� ����
//	�������� IME���� ���ջ��� ���ڿ��� ���� �Ӽ����� �������� ��� �� �Ӽ��� ���ϴ� �Լ���.
//	�޾ƿ� �Ӽ��� m_pszCompAttrBuff�� �ӽ� ����Ǹ�, �۾����� m_siLenCompBuff��
//	�����ϸ� �ȴ�.
//		lParam		- LPARAM �Ķ���� ��
//		iRemainLen	- ���� ���ڰ� �� �׿������� ũ��. �̰��� 2����Ʈ �̸��̶�� ������ ���ڴ� ���� �� ����.
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompAttribute( LPARAM lParam, int iRemainLen )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_COMPATTR ) )								return FALSE;

	memset( m_pszCompAttrBuff, DF_VALUE_IME_DEFAULT_ATTR, sizeof(m_pszCompAttrBuff) );

	// ���ؽ�Ʈ �غ�
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_COMPATTR, NULL, 0 );

	// �ϼ��� �۾��� ���ų� ���� ��Ȳ�� �߻��ߴٸ� ����
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	return FALSE;

	// ���� ù ���� ���¿��� ������ 2����Ʈ �̻� ���� ���� ��� ���Ͻ��ѹ�����.
	if( ( m_bFirstComposing || !m_bIsComposing ) && iRemainLen < 2 )
	{
		iRet	= 0;
		bReturn	= false;
	}

	// �ϼ��� �۾� �ޱ�
	if( iRet > 0 )
	{
		// (0:���� �ִ� ��, 1: ���ڿ� ��ȯ ��� ����, 2: ���ڿ� ��ȯ ��� ���)
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet = DF_VALUE_IME_BUFFER_SIZE - 1;
		ImmGetCompositionString( hImc, GCS_COMPATTR, m_pszCompAttrBuff, iRet );
	}

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_CompCursor()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION���� ����
//	�������� IME���� ���� Ŀ���� ��ġ�� ���ϴ� �Լ���.
//	���� Ŀ���� ��ġ�� m_siCompCaretPos�� ����Ǹ�, �� ������ �������� ���ڿ��� ù �Ӹ��� 0�� ���̴�.
//		lParam		- LPARAM �Ķ���� ��
//		iRemainLen	- ���� ���ڰ� �� �׿������� ũ��. �̰��� 2����Ʈ �̸��̶�� ������ ���ڴ� ���� �� ����.
//		return		- �������� ���� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompCursor( LPARAM lParam, int iRemainLen )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_CURSORPOS ) )								return FALSE;

	// ���ؽ�Ʈ �غ�
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	m_siCompCaretPos= ImmGetCompositionString( hImc, GCS_CURSORPOS, NULL, 0 );

	// ���� ��Ȳ�� �߻��ߴٸ� ����
	if( m_siCompCaretPos == IMM_ERROR_NODATA || m_siCompCaretPos == IMM_ERROR_GENERAL )	m_siCompCaretPos = 0;

	// ���� ù ���� ���¿��� ������ 2����Ʈ �̻� ���� ���� ��� ���Ͻ��ѹ�����.
	if( ( m_bFirstComposing || !m_bIsComposing ) && iRemainLen < 2 )
	{
		m_siCompCaretPos	= 0;
		bReturn	= false;
	}

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_Finish()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_COMPOSITION�� ���� �޺κп� ����
//	WM_IME_COMPOSITION �޽������� ���������� ó���� �ؾ� �ϴ� �κп� ����Ѵ�.
//		wParam		- WPARAM �Ķ���� ��
//		lParam		- LPARAM �Ķ���� ��
//		return		- �������� ���ڿ��� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_Finish( WPARAM wParam, LPARAM lParam )
{
	// ������� �Դٸ� �ϴ� ù ���� ���´� ����� ���̴�.
	m_bFirstComposing		= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_NOTIFY()
// Code: actdoll (2004-04-17)
// Desc: ���ν��� �޽��� �� WM_IME_NOTIFY���� ����
//	IME�� ���õ� ���� ��ȣ���� �������� �κ��̴�.
//		wParam	- WPARAM �Ķ���� ��
//		lParam	- LPARAM �Ķ���� ��
//		return	- �������� ���� �޾Ҵٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_NOTIFY( WPARAM wParam, LPARAM lParam )
{
	// ���� Ȯ��
	if( !m_hWnd )													return FALSE;
	if( !wParam )													return FALSE;

	// ���ؽ�Ʈ �غ�
	HIMC	hImc;

	switch( wParam )
	{
		// ���� Ȥ�� ��ȯ���� ������ ������ ���� ��Ȳ
		case IMN_OPENCANDIDATE			:	
		case IMN_CHANGECANDIDATE		:	
			{
				ClearCandidateInfo();
				hImc	= ImmGetContext( m_hWnd );
				LoadCandidateInfo( hImc );
				ImmReleaseContext( m_hWnd, hImc );
			}
			return TRUE;

		case IMN_CLOSECANDIDATE			:	
			{
				ClearCandidateInfo();
			}
			return TRUE;

		// �Է�Ű ��ȯ�� ������ ���
		// IME ��ü�� ���� ��ȭ�� �߻����� ���
		case IMN_SETCONVERSIONMODE		:	
		case IMN_SETOPENSTATUS			:
			{
				// �Է� ���� Ȯ��
				UpdateInputStatus();

				// �Ϻ��� �Է��� ������ ���
				if( IsLocaleJapanese() && m_enImeStatus != EN_IME_STATUS_OFF )
				{
					// �ݰ� īŸī�� �ý��� ���°� ������ ��� ������ ���󰡳� ���·� ���� ��
					if( IsInputKatakana() && !IsInputFullShape() )	SetInputStatus( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE );
				}
			}
			break;

		case IMN_PRIVATE				:
			{
				// ������ �߱��� IME�� ���⼭ ����â�� üũ�Ѵ�.
				GetPrivateReadingString();

				// ���� �⺻�� ����â�� ���ֱ� ���� �۾�
				switch( GetImeId() )
				{
					case IMEID_CHT_VER42:
					case IMEID_CHT_VER43:
					case IMEID_CHT_VER44:
					case IMEID_CHS_VER41:
					case IMEID_CHS_VER42:
						if( ( lParam == 1 ) || ( lParam == 2 ) )														return TRUE;
						break;
						
					case IMEID_CHT_VER50:
					case IMEID_CHT_VER51:
					case IMEID_CHT_VER52:
					case IMEID_CHT_VER60:
					case IMEID_CHS_VER53:
						if( (lParam == 16) || (lParam == 17) || (lParam == 26) || (lParam == 27) || (lParam == 28) )	return TRUE;
						break;
				}
			}
			break;

		// IME�� ���� ������ �������� �ƴ����� �Ǻ�
		case IMN_OPENSTATUSWINDOW		:	
		case IMN_CLOSESTATUSWINDOW		:	

		case IMN_SETSENTENCEMODE		:	
		case IMN_SETCANDIDATEPOS		:
		case IMN_SETCOMPOSITIONFONT		:
		case IMN_SETCOMPOSITIONWINDOW	:
		case IMN_SETSTATUSWINDOWPOS		:
		case IMN_GUIDELINE				:
			{
				int	i = 3;
			}
			break;

	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: SetCompStatusCancel()
// Code: actdoll (2004-04-17)
// Desc: 
//	���� ���� ���� ��� ������ ���� ���ڸ� ��ҽ�Ų��.
//	�� �Լ��� ����� ��� WM_IME_ENDCOMPOSITION�� ȣ��ǰ� IME������ ����ȴ�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetCompStatusCancel()
{
	if( !m_hWnd )		return;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: SetCompStatusDoComplete()
// Code: actdoll (2004-04-17)
// Desc: 
//	���� ���� ���� ��� ������ ���� ���ڸ� ������ �ϰ��Ų��.
//	�� �Լ��� ����� ��� WM_IME_ENDCOMPOSITION�� �Բ� 
//	WM_IME_COMPOSITION�� ���� �������� ���ڸ� ����� ���ڿ��� �������ش�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetCompStatusDoComplete()
{
	if( !m_hWnd )		return;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0 );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: GetReadingStringData()
// Code: actdoll (2004-04-17)
// Desc: 
//	����â�� ���� ��Ʈ���� �޴´�.
//-----------------------------------------------------------------------------
const char*	GS_IMEBase::GetReadingStringData()
{
	if( !m_bReadingWindowOn )							return NULL;

	return (const char*)m_pszReadingBuff;
}

//-----------------------------------------------------------------------------
// Name: SetCandidateStatusOpen()
// Code: actdoll (2004-04-17)
// Desc: 
//	����â�� ����޶�� ��ȣ�� ������.
//	�� �Լ��� ����� ��� WM_IME_NOTIFY�޽�����
//	IMN_OPENCANDIDATE�� ���� ����â�� �������� �˷��� ���̴�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetCandidateStatusOpen()
{
	if( !m_hWnd )		return;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmNotifyIME( hImc, NI_OPENCANDIDATE, 0x00, 0 );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: SetCandidateStatusClose()
// Code: actdoll (2004-04-17)
// Desc: 
//	����â�� �����ִ� ������ ��� �� ���¸� �ݾ��ش�.
//	�� �Լ��� ����� ��� WM_IME_NOTIFY�޽�����
//	IMN_CLOSECANDIDATE�� ���� ����â�� �������� �˷��� ���̴�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetCandidateStatusClose()
{
	if( !m_hWnd )		return;
	
	HIMC	hImc	= ImmGetContext( m_hWnd );
	ImmNotifyIME( hImc, NI_CLOSECANDIDATE, 0x00, 0 );
	ImmReleaseContext( m_hWnd, hImc );
}

//-----------------------------------------------------------------------------
// Name: GetCandidateString()
// Code: actdoll (2004-04-17)
// Desc: 
//	Ư������ ���� ��Ʈ���� ����ִ� �۾��� �������. 
//	���� ���ձⰡ �������϶��� �����ϴ�.
//	�߹� �迭�� ��� bShiftString�� true�̸� ����Ʈ�� ���ڸ� ������ �߰��� �� �ִ�
//	�迭�� ������ ���̴�.
//		iIndex			- �ޱ⸦ ���ϴ� ��Ʈ���� �ε���
//		bShiftString	- true�̸� ����Ʈ�� ������ ������� �� �ִ� ��Ʈ��, false��� �׳� ���� �� �ִ� ��Ʈ��
//		return			- ����� �޾����� �ش� �ε����� ����ִ� ���۸� ����, �ƴ϶�� NULL
//-----------------------------------------------------------------------------
const char*	GS_IMEBase::GetCandidateString( int iIndex )
{
	if( !m_bCandidateWindowOn )							return NULL;
	if( (DWORD)iIndex >= DF_VALUE_MAX_CANDIDATE )		return NULL;
	if( (DWORD)iIndex >= m_stCandidateInfo.m_dwCount )	return NULL;

	return (const char*)m_stCandidateInfo.m_pszStringBuffer[iIndex];
}

//-----------------------------------------------------------------------------
// Name: IsCandidateExType()
// Code: actdoll (2004-04-17)
// Desc: 
//	�߹����� ����â�� Ȯ��Ÿ������ �ߴ� �迭�� �����Ѵ�.
//	(�޸��� ����� �̷����� ������ �ٲ��.)
//		return		- true�� Ȯ�����â, false�� �Ϲݼ���â
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsCandidateExType()
{
	if( ( GetLanguage() == LANG_CHS || GetLanguage() == LANG_CHT ) && m_bCandidateTypeVer )
	{
		// Ȯ�����â�� Ư¡�� ���� Ÿ�Կ� ����â�� �ƹ��� ������ ������� �����鼭 
		// ����â�� �����ִ� ������ ���̴�.
		return ( !m_bReadingWindowOn && m_bCandidateWindowOn );
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: ClearCandidateInfo()										[private]
// Code: actdoll (2004-04-17)
// Desc: 
//	����â ���� ������ Ŭ�����Ų��.
//-----------------------------------------------------------------------------
void	GS_IMEBase::ClearCandidateInfo()
{
	ZeroMemory( &m_stCandidateInfo, sizeof(m_stCandidateInfo) );
	m_bCandidateWindowOn	= false;
}

//-----------------------------------------------------------------------------
// Name: LoadCandidateInfo()										[private]
// Code: actdoll (2004-04-17)
// Desc: 
//	�ش� HIMC�� ���� Ư������ ���� ��Ʈ���� ����Ʈ�� �޴´�.
//		hImc	- �� ������ HIMC
//		return	- ����� �޾����� ���� ������ ���� ����, ������ �ְų� ���ٸ� 0
//-----------------------------------------------------------------------------
int		GS_IMEBase::LoadCandidateInfo( HIMC hImc )
{
	if( !hImc )						return 0;

	// ������ �غ�
	DWORD			dwRet;
	CANDIDATELIST	*pstCddList;

	// actdoll (2005/01/31 15:54) : �߱�/�븸 ����
	//	������� �޴� ���۸� ���������� ���������� ��ü
	dwRet = ImmGetCandidateList( hImc, 0x00, NULL, 0 );
	if( dwRet <= 0 )					return 0;
	pstCddList = (CANDIDATELIST*)HeapAlloc( GetProcessHeap(), 0, dwRet );
	if( !pstCddList )					return 0;

	m_bCandidateWindowOn	= true;
	
	// ���� �����ŭ ���۸� ��Ƽ� ����Ʈ�� �д´�.
	ImmGetCandidateList( hImc, 0x00, pstCddList, dwRet );

	// �ϴ� �ΰ� ���� �ް�
	m_stCandidateInfo.m_dwPageSize	= min( pstCddList->dwPageSize, DF_VALUE_MAX_CANDIDATE );
	
	// ǥ���ϱ� ������ ��ġ ���� ���
	int		iPageTopIndex;
	// �Ϻ��� IME�� �ٸ� IME�� �޸� ��ȯ ���ڿ��� ���� ��ġ ����� Ʋ���� �´�.
	if( GetPrimaryLanguage() == LANG_JAPANESE )	
	{
		iPageTopIndex	= ( pstCddList->dwSelection / pstCddList->dwPageSize ) * pstCddList->dwPageSize;
//		iPageTopIndex	= ( m_stCandidateInfo.m_dwSelection / m_stCandidateInfo.m_dwPageSize ) * m_stCandidateInfo.m_dwPageSize;
	}
	// actdoll (2004/11/05 1:37) : ����!
	//	�߱��� ��ü ���� ABC �Է¹������� ����Ե� dwPageStart������ ���;� �� �κ���
	//	dwSelection���� �������� �ȴ�. ���� �� ó���� ���⼭ ���ش�.
	else if( m_hKLCurrent == DF_VALUE_HKL_CHS_INTELABC )
	{
		iPageTopIndex	= pstCddList->dwSelection;
	}
	// �Ϲ������δ� �� ������ ������ �ȴ�.
	else
	{
		iPageTopIndex = pstCddList->dwPageStart;
	}
	// ���� ��ġ�� ���� ����
	m_stCandidateInfo.m_dwSelection	= ( GetPrimaryLanguage() == LANG_KOREAN || 
										( 
											( GetLanguage() == LANG_CHS || GetLanguage() == LANG_CHT ) && !GetImeId() 
										)
									  )
									  ?		(DWORD)-1	:	pstCddList->dwSelection - iPageTopIndex;
	// �������� ����ִ� ����
	m_stCandidateInfo.m_dwCount		= pstCddList->dwCount - iPageTopIndex;
	if( m_stCandidateInfo.m_dwCount > pstCddList->dwPageSize )	m_stCandidateInfo.m_dwCount = pstCddList->dwPageSize;

	// ���ڿ� ������ �ޱ�
	char	*pStr;
	for( register int i=iPageTopIndex, j=0; i<(int)(pstCddList->dwCount) && j<m_stCandidateInfo.m_dwPageSize; i++, j++ )
	{
		pStr	= (char*)( ((BYTE*)pstCddList) + pstCddList->dwOffset[i] );
		strcpy( m_stCandidateInfo.m_pszStringBuffer[j], pStr );
	}

	HeapFree( GetProcessHeap(), 0, (LPVOID)pstCddList );

	return dwRet;
}

//-----------------------------------------------------------------------------
// Name: GetPrivateReadingString()
// Code: actdoll (2004-10-20)
// Desc: ����â ��¿� ���Ǵ� ���ڿ��� �޴´�.
//-----------------------------------------------------------------------------
void	GS_IMEBase::GetPrivateReadingString()
{
	// �ϴ� IME ID�� �޾ƺ��� Ȯ��
	DWORD	dwId = GetImeId();
	if( !dwId )
	{
		m_bReadingWindowOn = false;
		return;
	}
	
	// ���ؽ�Ʈ �����ϴ��� Ȯ��
	HIMC	hImc = ImmGetContext( m_hWnd );
	if( !hImc )
	{
		m_bReadingWindowOn = false;
		return;
	}
	
	BOOL			bUnicodeIme					= false;	// IME ��ġ�� �����ڵ� ������� �ƴ��� Ȯ��
	DWORD			dwReadingStrLen				= 0;
	DWORD			dwErr						= 0;
	WCHAR			*wstr						= 0;
	WCHAR			*pwszReadingStringBuffer	= NULL;		// GetReadingString()�� IME�� ������ �� ����� �ӽù���
	INPUTCONTEXT	*lpIC						= NULL;
	
	// �������� �޴� �Լ��� �����Ѵٸ� �ٷ� �׳� �д´�.
	if( _GetReadingString )
	{
		UINT	uMaxUiLen;
		BOOL	bVertical;
		// ���������� ������ Ȯ��
		dwReadingStrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		if( dwReadingStrLen )
		{
			wstr = pwszReadingStringBuffer = (LPWSTR)HeapAlloc( GetProcessHeap(), 0, sizeof(WCHAR) * dwReadingStrLen );
			if( !pwszReadingStringBuffer )
			{
				// �޸� ����.. ����..-_-;
				ImmReleaseContext( m_hWnd, hImc );
				return;
			}
			
			// �������� �޴´�.
			dwReadingStrLen = _GetReadingString( hImc, dwReadingStrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		}
		
		// �����ڵ� Ÿ�� IME��.
		bUnicodeIme = true;
	}
	// �������� �޴� �Լ��� ���ٸ� �����۾��̴�!
	else
	{
		lpIC	= _ImmLockIMC( hImc );
        
		// �̰� ��ü ���� �����ϱ�... �м��� �ȵȴ�...-_-;;;
		LPBYTE	p = 0;
		switch( dwId )
		{
		case IMEID_CHT_VER42:		// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
		case IMEID_CHT_VER43:		// New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
		case IMEID_CHT_VER44:		// New ChanJie IME98b          : 4.4.x.x // WinXP
			{
				p				= *(LPBYTE*)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 24 );
				if( !p )		break;
				dwReadingStrLen	= *(DWORD* )( p + 7*4 + 32*4 );
				dwErr			= *(DWORD* )( p + 8*4 + 32*4 );
				wstr			=  (WCHAR* )( p + 56 );
				bUnicodeIme		= true;
			}
			break;
			
		case IMEID_CHT_VER50:		// 5.0.x.x // WinME
			{
				p				= *(LPBYTE*)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 3*4 );
				if( !p )		break;
				p				= *(LPBYTE*)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
				if( !p )		break;
				dwReadingStrLen	= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 );
				dwErr			= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 + 1*4 );
				wstr			=  (WCHAR *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 );
				bUnicodeIme		= false;
			}
			break;
			
		case IMEID_CHT_VER51:		// 5.1.x.x // IME2002(w/OfficeXP)
		case IMEID_CHT_VER52:		// 5.2.x.x // (w/whistler)
		case IMEID_CHS_VER53:		// 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
			{
				p				= *(LPBYTE*)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 4 );
				if( !p )		break;
				p				= *(LPBYTE*)( (LPBYTE)p + 1*4 + 5*4 );
				if( !p )		break;
				dwReadingStrLen	= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 * 2 );
				dwErr			= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 * 2 + 1*4 );
				wstr			=  (WCHAR *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 );
				bUnicodeIme		= true;
			}
			break;
			
			
		case IMEID_CHS_VER41:		// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
			{
				int		iOffset;
				iOffset			= ( GetImeId( 1 ) >= 0x00000002 ) ? 8 : 7;
				
				p				= *(LPBYTE*)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + iOffset*4 );
				if( !p )		break;
				dwReadingStrLen	= *(DWORD *)( p + 7*4 + 16*2*4 );
				dwErr			= *(DWORD *)( p + 8*4 + 16*2*4 );
				dwErr			= min		( dwErr, dwReadingStrLen );
				wstr			=  (WCHAR *)( p + 6*4 + 16*2*1 );
				bUnicodeIme		= true;
			}
			break;
			
		case IMEID_CHS_VER42:		// 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
			{
				OSVERSIONINFOW	osi;
				osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
				GetVersionExW( &osi );
				
				int iTcharSize	= ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(WCHAR) : sizeof(char);
				p				= *(LPBYTE*)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4 );
				if( !p )		break;
				dwReadingStrLen	= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 * iTcharSize );
				dwErr			= *(DWORD *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 + 16 * iTcharSize + 1*4 );
				wstr			=  (WCHAR *)( p + 1*4 + ( 16*2 + 2*4 ) + 5*4 );
				bUnicodeIme		= ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
			}
			break;
		}
	}
	
	// ����â ���� �ʱ�ȭ
	ZeroMemory( m_pszReadingBuff, sizeof(m_pszReadingBuff) );

	// �������ڸ� ����â ����Ʈ�� ����ֱ�
	if( bUnicodeIme )	WideCharToMultiByte( m_uiCodePage, 0, wstr, dwReadingStrLen, m_pszReadingBuff, DF_VALUE_IME_BUFFER_SIZE, NULL, NULL );
	else				strcpy( m_pszReadingBuff, (char*)wstr );

	// ���ؽ�Ʈ�� ����
	if( !_GetReadingString )
	{
		_ImmUnlockIMCC	( lpIC->hPrivate );
		_ImmUnlockIMC	( hImc );
	}
	ImmReleaseContext( m_hWnd, hImc );
	if( pwszReadingStringBuffer )	HeapFree( GetProcessHeap(), 0, pwszReadingStringBuffer );
	
	// ����â�� ��µǾ� �ϴ��� ���θ� �����Ѵ�.
	if( dwReadingStrLen > 0 )	m_bReadingWindowOn = true;
	else						m_bReadingWindowOn = false;
}

