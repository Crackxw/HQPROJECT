//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - IME 베이스
//	File Name		: GS_IMEBase.cpp
//	Birth Date		: 2004. 04. 14.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상 내 에디트 박스에서 쓰일 IME 기능을 구현한다.
//
//===================================================================================================

#include <GSL.h>

#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
#include "GS_IMEBase.h"

//-----------------------------------------------------------------------------
// Name: GS_IMEBase
// Code: actdoll (2004-10-20)
// Desc: 매크로
//-----------------------------------------------------------------------------
// 함수포인터 접착
#define MC_FUNC_SETPROC( Module, APIName, Temp )	{ Temp = GetProcAddress( Module, #APIName );	if( Temp )	{ *(FARPROC*)&_##APIName = Temp; } }
// 함수포인터 해제
#define MC_FUNC_KILLPROC( APIName )					{ _##APIName = Dummy_##APIName; }
// IME 버전값 구하기
#define MAKEIMEVERSION( major, minor )				( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

//-----------------------------------------------------------------------------
// Name: GS_IMEBase
// Code: actdoll (2004-10-20)
// Desc: 정의들
//-----------------------------------------------------------------------------
// IME 가동 속성
enum	EN_IME_STATUS
{
	EN_IME_STATUS_INVALID	= -1						,	// 에러값
	EN_IME_STATUS_INIT		= 0							,	// 초기값
	EN_IME_STATUS_FIRST		= 0							,	// 처음값

	EN_IME_STATUS_OFF		= EN_IME_STATUS_FIRST		,	// IME 가동 안함
	EN_IME_STATUS_ON									,	// IME 켜짐(국가별 언어 모드)
	EN_IME_STATUS_ENGLISH								,	// IME 켜짐(영문 모드)

	EN_IME_STATUS_LAST		= EN_IME_STATUS_ENGLISH		,	// 마지막 값
	EN_IME_STATUS_MAX											// 총 갯수
};

const	char*	DF_FILENAME_IMM32_DLL	=	"\\imm32.dll";	// IME DLL 파일명

// 번자체 - 대만, 홍콩 계열
const	char*	DF_FILENAME_IME_CHT1	=	"TINTLGNT.IME";	// New Phonetic
const	char*	DF_FILENAME_IME_CHT2	=	"CINTLGNT.IME";	// New Chang Jie
const	char*	DF_FILENAME_IME_CHT3	=	"MSTCIPHA.IME";	// Phonetic 5.1
// 간자체 - 중국 본토 계열
const	char*	DF_FILENAME_IME_CHS1	=	"PINTLGNT.IME";	// MSPY1.5/2/3
const	char*	DF_FILENAME_IME_CHS2	=	"MSSCIPYA.IME";	// MSPY3 for OfficeXP

// 키보드 입력기 정의값
const	HKL		DF_VALUE_HKL_CHT_DEFAULT		=	( (HKL)(INT_PTR)0x04040404 );	// 중국어 (번체) - Default
const	HKL		DF_VALUE_HKL_CHS_USKEYBOARD		=	( (HKL)(INT_PTR)0x04040804 );	// 중국어 (간체) - 번체 US KeyBoard
const	HKL		DF_VALUE_HKL_CHT_USKEYBOARD		=	( (HKL)(INT_PTR)0x08040404 );	// 중국어 (번체) - 간체 US KeyBoard
const	HKL		DF_VALUE_HKL_CHS_DEFAULT		=	( (HKL)(INT_PTR)0x08040804 );	// 중국어 (간체) - Default

const	HKL		DF_VALUE_HKL_CHT_PHONETIC		=	( (HKL)(INT_PTR)0xE0010404 );	// 중국어 (번체) - Phonetic
const	HKL		DF_VALUE_HKL_JPN_MSIME2002		=	( (HKL)(INT_PTR)0xE0010411 );	// 일본어 입력 시스템 (MS-IME2002)
const	HKL		DF_VALUE_HKL_KOR_MSIME2000		=	( (HKL)(INT_PTR)0xE0010412 );	// 한국어 입력 시스템 (IME 2000)
const	HKL		DF_VALUE_HKL_CHS_QUANPIN		=	( (HKL)(INT_PTR)0xE0010804 );	// 중국어 (간체) - QuanPin
const	HKL		DF_VALUE_HKL_CHT_CHANGJIE		=	( (HKL)(INT_PTR)0xE0020404 );	// 중국어 (번체) - ChangJie
const	HKL		DF_VALUE_HKL_CHS_SHUANGPIN		=	( (HKL)(INT_PTR)0xE0020804 );	// 중국어 (간체) - ShuangPin
const	HKL		DF_VALUE_HKL_CHT_QUICK			=	( (HKL)(INT_PTR)0xE0030404 );	// 중국어 (번체) - Quick
const	HKL		DF_VALUE_HKL_CHS_ZHENGMA		=	( (HKL)(INT_PTR)0xE0030804 );	// 중국어 (간체) - ZhengMa
const	HKL		DF_VALUE_HKL_CHT_BIG5CODE		=	( (HKL)(INT_PTR)0xE0040404 );	// 중국어 (번체) - Big5 Code
const	HKL		DF_VALUE_HKL_CHS_INTELABC		=	( (HKL)(INT_PTR)0xE0040804 );	// 중국어 (간체) - 지능 ABC
const	HKL		DF_VALUE_HKL_CHT_ARRAY			=	( (HKL)(INT_PTR)0xE0050404 );	// 중국어 (번체) - Array
const	HKL		DF_VALUE_HKL_CHS_NEIMA			=	( (HKL)(INT_PTR)0xE0050804 );	// 중국어 (간체) - NeiMa
const	HKL		DF_VALUE_HKL_CHT_DAYI			=	( (HKL)(INT_PTR)0xE0060404 );	// 중국어 (번체) - DaYi
const	HKL		DF_VALUE_HKL_CHT_UNICODE		=	( (HKL)(INT_PTR)0xE0070404 );	// 중국어 (번체) - Unicode
const	HKL		DF_VALUE_HKL_CHT_NEWPHONETIC	=	( (HKL)(INT_PTR)0xE0080404 );	// 중국어 (번체) - New Phonetic
const	HKL		DF_VALUE_HKL_CHT_NEWCHANGJIE	=	( (HKL)(INT_PTR)0xE0090404 );	// 중국어 (번체) - New ChangJie
const	HKL		DF_VALUE_HKL_CHS_MSPY			=	( (HKL)(INT_PTR)0xE00E0804 );	// 중국어 (간체) - Microsoft Pinyin IME 3.0
const	HKL		DF_VALUE_HKL_CHT_ALPHANUMERIC	=	( (HKL)(INT_PTR)0xE01f0404 );	// 중국어 (번체) - Alphanumeric
const	HKL		DF_VALUE_HKL_KOR_MSIME2003		=	( (HKL)(INT_PTR)0xE0200412 );	// 한국어 입력 시스템 (MS-IME2003)

// 중국어권 IME ID 설정
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
// Desc: 스태틱변수 초기화
//-----------------------------------------------------------------------------
HWND						GS_IMEBase::m_hWnd					= NULL;							// 이 IME 시스팀의 부모 윈도우 핸들
HIMC						GS_IMEBase::m_hImcDefault			= NULL;							// 이 IME 시스템에 할당된 기본 IMC 핸들
HINSTANCE					GS_IMEBase::m_hDllImm32				= NULL;							// IMM32.DLL의 인스턴스 핸들
HINSTANCE					GS_IMEBase::m_hDllIme				= NULL;							// 각각의 IME 모듈의 인스턴스 핸들
HKL							GS_IMEBase::m_hKLCurrent			= NULL;							// 현재 키보드 입력기 핸들

UINT						GS_IMEBase::m_uiCodePage			= CP_ACP;						// 현재의 IME에서 사용해야 할 코드페이지 값
int							GS_IMEBase::m_enImeStatus			= EN_IME_STATUS_INIT;			// 현재의 IME 상태
char						GS_IMEBase::m_pszInputLocaleName[4]	= {0,};							// 현재의 입력 로케일 명칭
char						GS_IMEBase::m_pszImeLayoutName[128]	= {0,};							// 현재의 IME 출력기 명칭

BOOL						GS_IMEBase::m_bReadingWindowOn		= false;						// 발음창의 작동개시 여부 상태
BOOL						GS_IMEBase::m_bCandidateWindowOn	= false;						// 숙어창의 작동개시 여부 상태
BOOL						GS_IMEBase::m_bCandidateTypeVer		= false;						// 숙어창의 가로 세로 출력 여부

BOOL						GS_IMEBase::m_bFirstComposing		= false;						// 조합 작업이 처음으로 개시된 상태인가?
BOOL						GS_IMEBase::m_bIsComposing			= false;						// 현재 조합중인 상태인가?

int							GS_IMEBase::m_siCompCaretPos		= 0;							// 조합 상태에서의 커서 현재 위치
int							GS_IMEBase::m_siLenResultBuff		= 0;							// 결과값 스트링의 문자길이
int							GS_IMEBase::m_siLenCompBuff			= 0;							// 조합값 스트링의 문자길이

char						GS_IMEBase::m_pszResultBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// 결과값으로 떨어진 스트링을 임시 저장할 버퍼							
char						GS_IMEBase::m_pszCompBuff[DF_VALUE_IME_BUFFER_SIZE]			= {0,};	// 조합값으로 떨어진 스트링을 임시 저장할 버퍼							
char						GS_IMEBase::m_pszCompAttrBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// 조합값의 스트링별 속성값을 임시 저장할 버퍼							
char						GS_IMEBase::m_pszReadingBuff[DF_VALUE_IME_BUFFER_SIZE]		= {0,};	// 발음문자로 떨어진 스트링을 임시 저장할 버퍼							

ST_IME_CANDIDATE_INFO		GS_IMEBase::m_stCandidateInfo		= {0,};							// 숙어창 관련 정보


//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: 함수포인터 초기화
//-----------------------------------------------------------------------------
// 구형 IME 작동시 필요한 호출 함수(DLL에서 직접 얻어옴)
INPUTCONTEXT*	( WINAPI*	GS_IMEBase::_ImmLockIMC )		( HIMC )									= GS_IMEBase::Dummy_ImmLockIMC;
BOOL			( WINAPI*	GS_IMEBase::_ImmUnlockIMC )		( HIMC )									= GS_IMEBase::Dummy_ImmUnlockIMC;
LPVOID			( WINAPI*	GS_IMEBase::_ImmLockIMCC )		( HIMCC )									= GS_IMEBase::Dummy_ImmLockIMCC;
BOOL			( WINAPI*	GS_IMEBase::_ImmUnlockIMCC )	( HIMCC )									= GS_IMEBase::Dummy_ImmUnlockIMCC;
// 중국어권 IME 작동시 필요한 호출 함수(DLL에서 직접 얻어옴)
UINT			( WINAPI*	GS_IMEBase::_GetReadingString )	( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT )	= GS_IMEBase::Dummy_GetReadingString;
BOOL			( WINAPI*	GS_IMEBase::_ShowReadingWindow )( HIMC, BOOL )								= GS_IMEBase::Dummy_ShowReadingWindow;


#define		DF_VALUE_IME_DEFAULT_ATTR		0xFF				// IME 조합 속성에서 디폴트 속성

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2004-04-17)
// Desc: 데이터 활성화를 위해 전역 변수 한번 호출
//-----------------------------------------------------------------------------
GS_IMEBase		g_clImeBase;

//-----------------------------------------------------------------------------
// Name: GS_IMEBase()
// Code: actdoll (2004-04-17)
// Desc: 생성자
//-----------------------------------------------------------------------------
GS_IMEBase::GS_IMEBase()
{
	// IMM 관련 함수 가동
	InitializeImm();

	Clear();
}

//-----------------------------------------------------------------------------
// Name: GS_IMEBase()
// Code: actdoll (2004-04-17)
// Desc: 소멸자
//-----------------------------------------------------------------------------
GS_IMEBase::~GS_IMEBase()
{
	Clear();

	// IMM 관련 함수 해제
	ReleaseImm();
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Code: actdoll (2004-10-20)
// Desc: 초기화로 한번만 해주면 된다.
//		hWnd	- 메인 윈도우 핸들값
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
//	현재 이 클래스에 들어있는 변수 정보들을 초기화시킨다.
//-----------------------------------------------------------------------------
void	GS_IMEBase::Clear()
{
	SetCompStatusCancel();		// 조합중인 글자가 있으면 다 날린다.
	SetCandidateStatusClose();	// 숙어창이 열려있다면 닫아준다.

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
// Desc: IMM DLL 관련 함수들의 로딩
//-----------------------------------------------------------------------------
void	GS_IMEBase::InitializeImm()
{
	if( m_hDllImm32 )										return;

	// 시스템 경로 받고
	char	pszPath[MAX_PATH + 1];
	if( !::GetSystemDirectoryA( pszPath, MAX_PATH + 1 ) )	return;
	strcat( pszPath, DF_FILENAME_IMM32_DLL );

	// 구형 IME를 위해, DLL을 로드하여 정의가 안된 IMM 함수모듈을 장착한다.
    m_hDllImm32		= LoadLibrary( pszPath );
    if( m_hDllImm32 )
    {
	    FARPROC		Temp;
        MC_FUNC_SETPROC( m_hDllImm32, ImmLockIMC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmUnlockIMC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmLockIMCC,		Temp );
        MC_FUNC_SETPROC( m_hDllImm32, ImmUnlockIMCC,	Temp );
    }

	// IMM 모듈을 확인한다.
	CheckImeModule();
}

//-----------------------------------------------------------------------------
// Name: ReleaseImm()
// Code: actdoll (2004-10-20)
// Desc: IMM DLL 관련 함수들의 해제
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
// Desc: IME DLL 관련 함수의 로딩
//	키보드 입력기값을 통해 중국어 등에서 사용하는 특별한 IME API 함수가 
//	존재하면 해당 모듈을 로딩하여 함수로 갱신/물려준다.
//	입력 로케일이 변경될때 먼저 키보드 입력기를 확인하고 그 다음에 이것을 처리한다.
//-----------------------------------------------------------------------------
void	GS_IMEBase::CheckImeModule()
{
	// 기존 함수모듈 초기화
	_GetReadingString	= NULL;
	_ShowReadingWindow	= NULL;

	// 해당 IME 모듈의 인스턴스를 확보
	char	pszPath[MAX_PATH + 1];
	if( !ImmGetIMEFileName( m_hKLCurrent, pszPath, MAX_PATH + 1 ) )		return;
	if( m_hDllIme )	FreeLibrary( m_hDllIme );
	m_hDllIme		= LoadLibrary( pszPath );

	// 함수모듈 다시 장착
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
//	기본 IMC 디바이스를 얻는다.(반드시 밖에서 초기화때 한번만 사용해 줘야 한다)
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
//	IME 시스템 자체가 가동중인지를 체크한다. IsInputAlphabet()과는 약간 틀리다.
//	순수하게 IME 시스템이 가동하고 있는지를 확인하려면 이 함수를 사용한다.
//		return	- 맞다면 true, 틀리면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsImeRun()
{
	return ( m_enImeStatus != EN_IME_STATUS_OFF );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: 입력기의 언어가 한국인가?
//		return - 맞다면 true, 틀리면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleKorean()
{
	return ( GetPrimaryLanguage() == LANG_KOREAN );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: 입력기의 언어가 일본인가?
//		return - 맞다면 true, 틀리면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleJapanese()
{
	return ( GetPrimaryLanguage() == LANG_JAPANESE );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: 입력기의 언어가 중문 번체인가?
//		return - 맞다면 true, 틀리면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleTradChinese()
{
	return ( GetLanguage() == LANG_CHT );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: 입력기의 언어가 중문 간체인가?
//		return - 맞다면 true, 틀리면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsLocaleSimpChinese()
{
	return ( GetLanguage() == LANG_CHS );
}

//-----------------------------------------------------------------------------
// Name: IsLocaleKorean()
// Code: actdoll (2004-10-20)
// Desc: 입력기의 언어가 IME가 없는 국가형태인가?
//		return - 맞다면 true, 틀리면 false
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
//	알파벳 언어체계로 가동중인가?
//		return	- 맞다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsInputAlphabet()
{
	// 중문 번체의 경우 입력기 중 하나가 
	// 영문만 되는데 IME로 인식한다. 예외처리 할 것
	if( GetLanguage() == LANG_CHT && m_hKLCurrent == DF_VALUE_HKL_CHT_ALPHANUMERIC )		return TRUE;

	return ( m_enImeStatus != EN_IME_STATUS_ON );
}

//-----------------------------------------------------------------------------
// Name: IsInputNative()
// Code: actdoll (2004-04-17)
// Desc: 
//	해당 국가별 언어체계로 가동중인가?
//		return	- 맞다면 true, 아니라면 false
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
//	카타카나 언어체계로 가동중인가?
//		return	- 카타카나라면 true, 히라가나라면 false
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
//	전각 언어체계로 가동중인가?
//		return	- 전각이라면 true, 반각이라면 false
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
//	한자변환 언어체계로 가동중인가?
//		return	- 맞다면 true, 틀리면 false
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
// Desc: 입력 로케일 체크
//	현재 설정된 IME가 어떤 종류인지를 확인한다.
//	m_hKLCurrent, m_bCandidateTypeVer, 
//	m_enImeStatus, m_uiCodePage 등이 이 함수를 통해 변경된다.
//-----------------------------------------------------------------------------
void	GS_IMEBase::UpdateInputStatus()
{
	// 일단 입력 로케일 자체가 변동이 생겼을 경우 새로운 정보로 갱신하도록 한다.
	static HKL hklPrev	= 0;
	m_hKLCurrent		= GetKeyboardLayout( 0 );
	if( hklPrev != m_hKLCurrent )
	{
		hklPrev = m_hKLCurrent;

		// 언어값 받는다.
		switch ( GetPrimaryLanguage() )
		{
			// 중국어 계열
			case LANG_CHINESE:
				m_bCandidateTypeVer = true;
				switch ( GetSubLanguage() )
				{
					// 중국어 간체(본토)
					case SUBLANG_CHINESE_SIMPLIFIED:
						m_bCandidateTypeVer = ( GetImeId() == 0 );
						break;
					// 중국어 번체(대만, 홍콩)
					case SUBLANG_CHINESE_TRADITIONAL:
						break;
					// 알 수 없는 놈이다.
					default:
						break;
				}
				break;
			// 한국어
			case LANG_KOREAN:
				m_bCandidateTypeVer		= false;
				break;
			// 일본어
			case LANG_JAPANESE:
				m_bCandidateTypeVer		= true;
				break;
			// 알 수 없는 놈
			default:
				break;
		}

		// 입력 로케일의 단축 명칭을 받는다.
        GetLocaleInfo( MAKELCID( GetLanguage(), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, m_pszInputLocaleName, 4 );
		m_pszInputLocaleName[3] = 0;
		CharUpper( m_pszInputLocaleName );	// 대문자로 변환시킨다.
		

		// 부 명칭을 얻어온다.(레지스트리에서 추출)
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


		// 현재 유저가 설정한 언어의 코드페이지를 얻는다.
		char	pszCodePage[8];
		m_uiCodePage	= CP_ACP;
		if( GetLocaleInfo( MAKELCID( GetLanguage(), SORT_DEFAULT ), LOCALE_IDEFAULTANSICODEPAGE,	pszCodePage, 8 ) )
		{
			m_uiCodePage = strtoul( pszCodePage, NULL, 0 );
		}
	}

	// 이 입력 로케일이 IME 시스템인지 확인
	BOOL	bIme		= ImmIsIME( m_hKLCurrent );
	BOOL	bChineseIME	= ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

	HIMC	hImc;
	// IME 컨텍스트가 존재한다면
	if( hImc = ImmGetContext( m_hWnd ) )
	{
		// 중국어권 IME일 경우
		if( bChineseIME )
		{
			DWORD	dwConvMode, dwSentMode;
			ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			// 본토 모드일 경우 켜진 것이고 아니라면 영문 모드이다.
			m_enImeStatus = ( dwConvMode & IME_CMODE_NATIVE )		?	EN_IME_STATUS_ON	: EN_IME_STATUS_ENGLISH;
		}
		// 그 밖의 IME일 경우
		else
		{
			// OpenStatus가 존재하면 EN_IME_STATUS_ON, 없다면 EN_IME_STATUS_OFF
			m_enImeStatus = ( bIme && ImmGetOpenStatus( hImc ) )	?	EN_IME_STATUS_ON	: EN_IME_STATUS_ENGLISH;
		}
		ImmReleaseContext( m_hWnd, hImc );
	}
	// 이 때는 IME가 없거나 일부러 뺀 경우이다.
	else
	{
		m_enImeStatus = EN_IME_STATUS_OFF;
	}
}

//-----------------------------------------------------------------------------
// Name: GetImeId()
// Code: actdoll (2004-10-20)
// Desc: 해당 IME의 ID 얻기
//	윈XP에서까지 정의되지 않았던 로케일 ID값을 알아오기 위해서 우리가 직접
//	찾아야 하는 상황이 발생할 수 있다. 이 함수는 그 상황에서 사용한다.
//	더 많은 정보는 CompareString() 함수 참조.
//		return		- 비중국어 입력로케일/오래된 중국어 IME/다른 에러의 경우 0리턴
//					- uIndex가 0일 경우 
//						bit 31-24:	Major version
//						bit 23-16:	Minor version
//						bit 15-0:	Language ID
//					- uIndex가 1일 경우
//						( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS
//					버전과 언어정보를 알고싶다면 IMEID_VER & IMEID_LANG 매크로를 사용할 것.
//-----------------------------------------------------------------------------
DWORD	GS_IMEBase::GetImeId( UINT uIndex )
{
	static	HKL		hklPrev = 0;			// 이전 키보드 입력기 핸들
	static	DWORD	dwID[2] = { 0, 0 };		// 이전 결과값 캐슁

	DWORD   dwVerSize;
	DWORD   dwVerHandle;
	LPVOID  lpVerBuffer;
	LPVOID  lpVerData;
	UINT    cbVerData;
	char    szTmp[1024];

	if( uIndex >= 2 )				return 0;
	if( hklPrev == m_hKLCurrent )	return dwID[uIndex];

	// 이전 데이터 저장
	hklPrev = m_hKLCurrent;

	// 중국어 IME 옛날 놈 쓰고 있는지 체크
	if( !( ( m_hKLCurrent == DF_VALUE_HKL_CHT_NEWPHONETIC ) || ( m_hKLCurrent == DF_VALUE_HKL_CHT_NEWCHANGJIE ) || ( m_hKLCurrent == DF_VALUE_HKL_CHS_MSPY ) ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// IME 실 파일 명을 받아온다.
	if( !ImmGetIMEFileName( m_hKLCurrent, szTmp, 1023 ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// 발음 조합 API(Reading String API)가 존재하는지를 체크
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

	// 버전 조합 개시
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
//	IMC 장치를 살릴 것인지 죽일 것인지를 체크한다.
//	죽이게 될 경우 현재 설치된 IME와 상관없이 전부 동작하지 않는다. 즉 영문만 가능하다.
//		bDeviceOn	- 가동시킨다면 true, 해제한다면 false
//-----------------------------------------------------------------------------
void	GS_IMEBase::SetImeDeviceOn( BOOL bDeviceOn )
{
	// IME 작동을 끈다
	if( !bDeviceOn )
	{
		ImmAssociateContext( m_hWnd, NULL );
	}
	// IME 작동을 켠다
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

	// 현재의 IME 종류를 갱신한다.
	UpdateInputStatus();
}

//-----------------------------------------------------------------------------
// Name: SetIMEStatus()
// Code: actdoll (2004-04-17)
// Desc: 
//	IME의 기능을 켠 상태로 할 것인지 끈 상태로 할 것인지를 결정한다.
//		bOpen	- 연 상태로 한다면 true, 아니라면 false
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
//	IME의 기능이 켜져있는지를 체크한다.
//		return	- IME가 켜져있는 상태라면 true, 아니라면 false
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
//	원하는 입력기 언어체계로 세팅한다.
//		dwConv	- 적용할 변환 언어값(IME_CMODE_...)
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
//	현재 세팅된 입력기의 언어체계를 받는다.
//		dwConversion	- 컨버젼 스테이터스값 받음
//		dwSentence		- 센텐스 스테이터스값 받음
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
// Desc: 프로시저 메시지 중 WM_INPUTLANGCHANGE 내에 장착
//	입력 로케일이 바뀌었을 경우 이 부분을 통해서 신호가 떨어진다.
//		lParam		- LPARAM 파라미터 값
//		return		- 무언가 변환이 있었다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_INPUTLANGCHANGE( LPARAM lParam )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;

	// 여기서 바뀐 입력정보를 확인한다.
	UpdateInputStatus();

	// IME가 교체되었을때를 대비하여 새로운 DLL 모듈을 확인한다.
	CheckImeModule();

	// 발음창 API가 존재한다면 일단 명령어 세팅을!
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
// Desc: 프로시저 메시지 중 WM_IME_STARTCOMPOSITION내에 장착
//	IME조합이 시작되었을 때 최초로 떨어지는 메시지다.
//		lParam		- LPARAM 파라미터 값
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_STARTCOMPOSITION( LPARAM lParam )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;

	m_bReadingWindowOn		= false;
	m_bFirstComposing		= true;
	m_bIsComposing			= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_STARTCOMPOSITION()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_STARTCOMPOSITION내에 장착
//	IME조합이 시작되었을 때 최초로 떨어지는 메시지다.
//		lParam		- LPARAM 파라미터 값
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_ENDCOMPOSITION( LPARAM lParam )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;

	m_bReadingWindowOn		= false;
	m_bFirstComposing		= false;
	m_bIsComposing			= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_Start()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내 가장 앞부분에 장착
//	별 의미는 없지만 디버그 처리나 신호를 먼저 받고 테스트하기 위한 용도로 사용할 수 있다.
//		wParam		- WPARAM 파라미터 값
//		lParam		- LPARAM 파라미터 값
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_Start( WPARAM wParam, LPARAM lParam )
{

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_ResultString()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내에 장착
//	조합중인 IME에서 완성된 문자열이 떨어졌을 경우 그 스트링을 구하는 함수다.
//	받아온 스트링은 m_pszResultBuff에 임시 저장되며, 글씨수는 m_siLenResultBuff에
//	저장된다.
//		lParam		- LPARAM 파라미터 값
//		iCanRecvLen	- 넘어온 문자열에서 얼마만큼 받을 것인가? -1이라면 들어온대로 다 받아라
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_ResultString( LPARAM lParam, int iCanRecvLen )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_RESULTSTR ) )								return FALSE;

	ZeroMemory( m_pszResultBuff, sizeof(m_pszResultBuff) );
	m_siLenResultBuff	= 0;

	// 컨텍스트 준비
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 );
	m_bIsComposing	= false;

	// 완성된 글씨가 없거나 에러 상황이 발생했다면 리턴
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	bReturn	= false;

	// 완성된 글씨 받기(받을 수 있는 크기가 0이 아닐 경우만 통과)
	if( bReturn && ( iRet > 0 && iCanRecvLen != 0 ) )
	{
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;
		m_siLenResultBuff	= ImmGetCompositionString( hImc, GCS_RESULTSTR, m_pszResultBuff, iRet );
		m_pszResultBuff[m_siLenResultBuff]	= 0;

		// 받을 수 있는 크기가 제약이 있고 들어온 글씨가 받은 글씨보다 크다면 글씨에 맞춰 자른다.
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

	// 완성이 떨어졌다는 것은 기존의 조합 문자열은 끝났다는 의미이므로 클리어 시켜준다.
	ZeroMemory( m_pszCompBuff, sizeof(m_pszCompBuff) );
	memset( m_pszCompAttrBuff, DF_VALUE_IME_DEFAULT_ATTR, sizeof(m_pszCompAttrBuff) );

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_CompString()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내에 장착
//	조합중인 IME에서 조합상태 문자열이 떨어졌을 경우 그 스트링을 구하는 함수다.
//	받아온 스트링은 m_pszCompBuff에 임시 저장되며, 글씨수는 m_siLenCompBuff에
//	저장된다.
//		lParam		- LPARAM 파라미터 값
//		iRemainLen	- 조합 문자가 들어갈 잉여공간의 크기. 이것이 2바이트 미만이라면 조합중 문자는 받을 수 없다.
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompString( LPARAM lParam, int iRemainLen )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_COMPSTR ) )									return FALSE;

	ZeroMemory( m_pszCompBuff, sizeof(m_pszCompBuff) );
	m_siLenCompBuff	= 0;		

	// 컨텍스트 준비
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );

	// 완성된 글씨가 없거나 에러 상황이 발생했다면 리턴
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	bReturn = false;

	// 만약 첫 조합 상태에서 여분이 2바이트 이상 남지 않을 경우 리턴시켜버린다.
	if( ( m_bFirstComposing || !m_bIsComposing ) && iRemainLen < 2 )
	{
		ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
		iRet	= 0;
		bReturn	= false;
	}

	// 문자를 더 받을 수 있는 상태에서 옛날형 중국어 번체 IME는 조합문자를 통해 발음문자가 들어온다.
	if( ( GetLanguage() == LANG_CHT || GetLanguage() == LANG_CHS ) && !GetImeId() && bReturn )
	{
		ZeroMemory( &m_pszReadingBuff, sizeof(m_pszReadingBuff) );

		// 제한 버퍼크기를 넘어가면 안된다.
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;

		// 일단 발음문자를 받는다.
		iRet = ImmGetCompositionString( hImc, GCS_COMPSTR, m_pszReadingBuff, iRet );
		
		// 받아온 글자가 있다면 발음창은 켜진다.
		m_bReadingWindowOn	= ( iRet > 0 );

		// 중문 간체일 경우 숙어창까지 완전히 닫힌 상태에서 iRet이 0이 온다면 한 글자에 대한 모든 작업이 끝난 셈이다.
		// 이때 문제는 WM_IME_ENDCOMPOSITION이 완벽하게 떨어지지 않는다는 것이다. 
		// 문제가 발생할 소지가 다분히 있기 때문에 일단 여기서 강제적으로 종료 메시지를 날려준다.
		if( GetLanguage() == LANG_CHS && iRet <= 0 )
		{
			// 숙어창이 꺼져있는 상태라면 문장조합이 종료된 것이다.
			if( !m_bCandidateWindowOn )	SetCompStatusCancel();
		}
	}
	// 그렇지 않으면 완성된 글씨를 받는다.
	else if( iRet > 0 )
	{
		// 제한 버퍼크기를 넘어가면 안된다.
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet	= DF_VALUE_IME_BUFFER_SIZE - 1;

		// 글씨 받기
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
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내에 장착
//	조합중인 IME에서 조합상태 문자열에 대한 속성값이 떨어졌을 경우 그 속성을 구하는 함수다.
//	받아온 속성은 m_pszCompAttrBuff에 임시 저장되며, 글씨수는 m_siLenCompBuff을
//	참조하면 된다.
//		lParam		- LPARAM 파라미터 값
//		iRemainLen	- 조합 문자가 들어갈 잉여공간의 크기. 이것이 2바이트 미만이라면 조합중 문자는 받을 수 없다.
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompAttribute( LPARAM lParam, int iRemainLen )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_COMPATTR ) )								return FALSE;

	memset( m_pszCompAttrBuff, DF_VALUE_IME_DEFAULT_ATTR, sizeof(m_pszCompAttrBuff) );

	// 컨텍스트 준비
	int		iRet;
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	iRet			= ImmGetCompositionString( hImc, GCS_COMPATTR, NULL, 0 );

	// 완성된 글씨가 없거나 에러 상황이 발생했다면 리턴
	if( iRet == IMM_ERROR_NODATA || iRet == IMM_ERROR_GENERAL )	return FALSE;

	// 만약 첫 조합 상태에서 여분이 2바이트 이상 남지 않을 경우 리턴시켜버린다.
	if( ( m_bFirstComposing || !m_bIsComposing ) && iRemainLen < 2 )
	{
		iRet	= 0;
		bReturn	= false;
	}

	// 완성된 글씨 받기
	if( iRet > 0 )
	{
		// (0:쓰고 있는 중, 1: 문자열 전환 대상 실행, 2: 문자열 전환 대상 대기)
		if( iRet >= DF_VALUE_IME_BUFFER_SIZE )		iRet = DF_VALUE_IME_BUFFER_SIZE - 1;
		ImmGetCompositionString( hImc, GCS_COMPATTR, m_pszCompAttrBuff, iRet );
	}

	ImmReleaseContext( m_hWnd, hImc );

	return bReturn;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_COMPOSITION_CompCursor()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내에 장착
//	조합중인 IME에서 현재 커서의 위치를 구하는 함수다.
//	현재 커서의 위치는 m_siCompCaretPos에 저장되며, 그 기준은 조합중인 문자열의 첫 머리가 0인 셈이다.
//		lParam		- LPARAM 파라미터 값
//		iRemainLen	- 조합 문자가 들어갈 잉여공간의 크기. 이것이 2바이트 미만이라면 조합중 문자는 받을 수 없다.
//		return		- 문제없이 값을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_CompCursor( LPARAM lParam, int iRemainLen )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;
	if( !( lParam & GCS_CURSORPOS ) )								return FALSE;

	// 컨텍스트 준비
	BOOL	bReturn	= true;
	HIMC	hImc	= ImmGetContext( m_hWnd );
	m_siCompCaretPos= ImmGetCompositionString( hImc, GCS_CURSORPOS, NULL, 0 );

	// 에러 상황이 발생했다면 리턴
	if( m_siCompCaretPos == IMM_ERROR_NODATA || m_siCompCaretPos == IMM_ERROR_GENERAL )	m_siCompCaretPos = 0;

	// 만약 첫 조합 상태에서 여분이 2바이트 이상 남지 않을 경우 리턴시켜버린다.
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
// Desc: 프로시저 메시지 중 WM_IME_COMPOSITION내 가장 뒷부분에 장착
//	WM_IME_COMPOSITION 메시지에서 최종적으로 처리를 해야 하는 부분에 사용한다.
//		wParam		- WPARAM 파라미터 값
//		lParam		- LPARAM 파라미터 값
//		return		- 문제없이 문자열을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_COMPOSITION_Finish( WPARAM wParam, LPARAM lParam )
{
	// 여기까지 왔다면 일단 첫 조합 상태는 종료된 것이다.
	m_bFirstComposing		= false;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: Check_WM_IME_NOTIFY()
// Code: actdoll (2004-04-17)
// Desc: 프로시저 메시지 중 WM_IME_NOTIFY내에 장착
//	IME와 관련된 각종 신호들이 떨어지는 부분이다.
//		wParam	- WPARAM 파라미터 값
//		lParam	- LPARAM 파라미터 값
//		return	- 문제없이 값을 받았다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::Check_WM_IME_NOTIFY( WPARAM wParam, LPARAM lParam )
{
	// 조건 확인
	if( !m_hWnd )													return FALSE;
	if( !wParam )													return FALSE;

	// 컨텍스트 준비
	HIMC	hImc;

	switch( wParam )
	{
		// 한자 혹은 변환문자 내역이 나왔을 때의 상황
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

		// 입력키 변환이 들어왔을 경우
		// IME 자체의 상태 변화가 발생했을 경우
		case IMN_SETCONVERSIONMODE		:	
		case IMN_SETOPENSTATUS			:
			{
				// 입력 상태 확인
				UpdateInputStatus();

				// 일본어 입력이 들어왔을 경우
				if( IsLocaleJapanese() && m_enImeStatus != EN_IME_STATUS_OFF )
				{
					// 반각 카타카나 시스템 형태가 들어왔을 경우 무조건 히라가나 형태로 돌릴 것
					if( IsInputKatakana() && !IsInputFullShape() )	SetInputStatus( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE );
				}
			}
			break;

		case IMN_PRIVATE				:
			{
				// 예전의 중국권 IME는 여기서 발음창을 체크한다.
				GetPrivateReadingString();

				// 윈도 기본의 발음창을 없애기 위한 작업
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

		// IME가 동작 가능한 상태인지 아닌지를 판별
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
//	조합 상태 중일 경우 현재의 조합 문자를 취소시킨다.
//	이 함수가 실행될 경우 WM_IME_ENDCOMPOSITION만 호출되고 IME조합은 종료된다.
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
//	조합 상태 중일 경우 현재의 조합 문자를 강제로 완결시킨다.
//	이 함수가 실행될 경우 WM_IME_ENDCOMPOSITION와 함께 
//	WM_IME_COMPOSITION을 통해 조합중인 문자를 종료된 문자열로 리턴해준다.
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
//	발음창에 들어온 스트링을 받는다.
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
//	숙어창을 열어달라고 신호를 보낸다.
//	이 함수가 실행될 경우 WM_IME_NOTIFY메시지의
//	IMN_OPENCANDIDATE을 통해 숙어창이 열렸음을 알려줄 것이다.
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
//	숙어창이 열려있는 상태일 경우 그 상태를 닫아준다.
//	이 함수가 실행될 경우 WM_IME_NOTIFY메시지의
//	IMN_CLOSECANDIDATE을 통해 숙어창이 닫혔음을 알려줄 것이다.
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
//	특수문자 조합 스트링에 들어있는 글씨를 끄집어낸다. 
//	숙어 조합기가 가동중일때만 가능하다.
//	중문 계열의 경우 bShiftString이 true이면 쉬프트와 숫자를 눌러서 추가할 수 있는
//	계열의 숙어라는 것이다.
//		iIndex			- 받기를 원하는 스트링의 인덱스
//		bShiftString	- true이면 쉬프트를 눌러서 연계받을 수 있는 스트링, false라면 그냥 받을 수 있는 스트링
//		return			- 제대로 받았으면 해당 인덱스가 들어있는 버퍼를 리턴, 아니라면 NULL
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
//	중문에서 숙어창이 확장타입으로 뜨는 계열이 존재한다.
//	(메모장 등에서는 이럴때는 색깔이 바뀐다.)
//		return		- true면 확장숙어창, false면 일반숙어창
//-----------------------------------------------------------------------------
BOOL	GS_IMEBase::IsCandidateExType()
{
	if( ( GetLanguage() == LANG_CHS || GetLanguage() == LANG_CHT ) && m_bCandidateTypeVer )
	{
		// 확장숙어창의 특징은 수직 타입에 발음창에 아무런 내용이 들어있지 않으면서 
		// 숙어창이 열려있는 상태인 것이다.
		return ( !m_bReadingWindowOn && m_bCandidateWindowOn );
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: ClearCandidateInfo()										[private]
// Code: actdoll (2004-04-17)
// Desc: 
//	숙어창 관련 정보를 클리어시킨다.
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
//	해당 HIMC에 들어온 특수문자 조합 스트링의 리스트를 받는다.
//		hImc	- 이 윈도의 HIMC
//		return	- 제대로 받았으면 받은 문자의 개수 리턴, 문제가 있거나 없다면 0
//-----------------------------------------------------------------------------
int		GS_IMEBase::LoadCandidateInfo( HIMC hImc )
{
	if( !hImc )						return 0;

	// 데이터 준비
	DWORD			dwRet;
	CANDIDATELIST	*pstCddList;

	// actdoll (2005/01/31 15:54) : 중국/대만 출장
	//	숙어문구를 받는 버퍼를 고정형에서 유동형으로 교체
	dwRet = ImmGetCandidateList( hImc, 0x00, NULL, 0 );
	if( dwRet <= 0 )					return 0;
	pstCddList = (CANDIDATELIST*)HeapAlloc( GetProcessHeap(), 0, dwRet );
	if( !pstCddList )					return 0;

	m_bCandidateWindowOn	= true;
	
	// 들어온 사이즈만큼 버퍼를 잡아서 리스트를 읽는다.
	ImmGetCandidateList( hImc, 0x00, pstCddList, dwRet );

	// 일단 부가 정보 받고
	m_stCandidateInfo.m_dwPageSize	= min( pstCddList->dwPageSize, DF_VALUE_MAX_CANDIDATE );
	
	// 표시하기 시작할 위치 정보 계산
	int		iPageTopIndex;
	// 일본어 IME는 다른 IME와 달리 변환 문자열의 시작 위치 계산을 틀리게 온다.
	if( GetPrimaryLanguage() == LANG_JAPANESE )	
	{
		iPageTopIndex	= ( pstCddList->dwSelection / pstCddList->dwPageSize ) * pstCddList->dwPageSize;
//		iPageTopIndex	= ( m_stCandidateInfo.m_dwSelection / m_stCandidateInfo.m_dwPageSize ) * m_stCandidateInfo.m_dwPageSize;
	}
	// actdoll (2004/11/05 1:37) : 주의!
	//	중국어 간체 지능 ABC 입력법에서는 웃기게도 dwPageStart값으로 나와야 할 부분이
	//	dwSelection으로 떨어지게 된다. 따라서 그 처리를 여기서 해준다.
	else if( m_hKLCurrent == DF_VALUE_HKL_CHS_INTELABC )
	{
		iPageTopIndex	= pstCddList->dwSelection;
	}
	// 일반적으로는 이 정보를 받으면 된다.
	else
	{
		iPageTopIndex = pstCddList->dwPageStart;
	}
	// 선택 위치에 대한 정보
	m_stCandidateInfo.m_dwSelection	= ( GetPrimaryLanguage() == LANG_KOREAN || 
										( 
											( GetLanguage() == LANG_CHS || GetLanguage() == LANG_CHT ) && !GetImeId() 
										)
									  )
									  ?		(DWORD)-1	:	pstCddList->dwSelection - iPageTopIndex;
	// 페이지당 들어있는 갯수
	m_stCandidateInfo.m_dwCount		= pstCddList->dwCount - iPageTopIndex;
	if( m_stCandidateInfo.m_dwCount > pstCddList->dwPageSize )	m_stCandidateInfo.m_dwCount = pstCddList->dwPageSize;

	// 문자열 데이터 받기
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
// Desc: 발음창 출력에 사용되는 문자열을 받는다.
//-----------------------------------------------------------------------------
void	GS_IMEBase::GetPrivateReadingString()
{
	// 일단 IME ID를 받아보고 확인
	DWORD	dwId = GetImeId();
	if( !dwId )
	{
		m_bReadingWindowOn = false;
		return;
	}
	
	// 컨텍스트 존재하는지 확인
	HIMC	hImc = ImmGetContext( m_hWnd );
	if( !hImc )
	{
		m_bReadingWindowOn = false;
		return;
	}
	
	BOOL			bUnicodeIme					= false;	// IME 장치가 유니코드 기반인지 아닌지 확인
	DWORD			dwReadingStrLen				= 0;
	DWORD			dwErr						= 0;
	WCHAR			*wstr						= 0;
	WCHAR			*pwszReadingStringBuffer	= NULL;		// GetReadingString()을 IME가 지원할 때 사용할 임시버퍼
	INPUTCONTEXT	*lpIC						= NULL;
	
	// 발음문자 받는 함수가 존재한다면 바로 그냥 읽는다.
	if( _GetReadingString )
	{
		UINT	uMaxUiLen;
		BOOL	bVertical;
		// 발음문자의 사이즈 확인
		dwReadingStrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		if( dwReadingStrLen )
		{
			wstr = pwszReadingStringBuffer = (LPWSTR)HeapAlloc( GetProcessHeap(), 0, sizeof(WCHAR) * dwReadingStrLen );
			if( !pwszReadingStringBuffer )
			{
				// 메모리 부족.. 설마..-_-;
				ImmReleaseContext( m_hWnd, hImc );
				return;
			}
			
			// 발음문자 받는다.
			dwReadingStrLen = _GetReadingString( hImc, dwReadingStrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		}
		
		// 유니코드 타입 IME다.
		bUnicodeIme = true;
	}
	// 발음문자 받는 함수가 없다면 수동작업이다!
	else
	{
		lpIC	= _ImmLockIMC( hImc );
        
		// 이게 대체 무슨 수식일까... 분석이 안된다...-_-;;;
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
	
	// 발음창 정보 초기화
	ZeroMemory( m_pszReadingBuff, sizeof(m_pszReadingBuff) );

	// 발음문자를 숙어창 리스트에 집어넣기
	if( bUnicodeIme )	WideCharToMultiByte( m_uiCodePage, 0, wstr, dwReadingStrLen, m_pszReadingBuff, DF_VALUE_IME_BUFFER_SIZE, NULL, NULL );
	else				strcpy( m_pszReadingBuff, (char*)wstr );

	// 컨텍스트들 해제
	if( !_GetReadingString )
	{
		_ImmUnlockIMCC	( lpIC->hPrivate );
		_ImmUnlockIMC	( hImc );
	}
	ImmReleaseContext( m_hWnd, hImc );
	if( pwszReadingStringBuffer )	HeapFree( GetProcessHeap(), 0, pwszReadingStringBuffer );
	
	// 발음창이 출력되야 하는지 여부를 설정한다.
	if( dwReadingStrLen > 0 )	m_bReadingWindowOn = true;
	else						m_bReadingWindowOn = false;
}

