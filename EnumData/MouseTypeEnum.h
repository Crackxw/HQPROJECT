// MouseTypeEnum.h: interface for the CMouseTypeEnum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSETYPEENUM_H__70E13576_FDAC_43F4_A8AD_0995E308A4EB__INCLUDED_)
#define AFX_MOUSETYPEENUM_H__70E13576_FDAC_43F4_A8AD_0995E308A4EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum ENUM_MOUSETYPE
{
	ON_MOUSE_FIRST = 0,						// 시작
	ON_MOUSE_BASIC,							// 기본 마우스
	ON_MOUSE_BATTLE_SWORD,					// 검. 공격 마우스
	ON_MOUSE_FIELD_BATTLE_BOW,				// 공성전 필드 전투 원거리 공격 관련 마우스
	ON_MOUSE_BATTLE_MAGIC,					// 마법 사용, 캐스트
	ON_MOUSE_BATTLE_HEAL,					// 치료
	ON_MOUSE_BATTLE_HOVER_CHARACTER,		// 커서 아래 캐릭터 존재. 전투
	ON_MOUSE_GOVIL,							// 마을 들어가기
	ON_MOUSE_GETITEM,						// 아이템 집기
	ON_MOUSE_SCROLL_6H,						// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_7H30M,					// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_9H,						// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_10H30M,					// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_12H,					// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_1H30M,					// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_3H,						// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_SCROLL_4H30M,					// 전투 중 맵 스크롤 시 마우스 버튼
	ON_MOUSE_MAX
};

// 전투 마우스 서브 타입. 근거리 공격, 근거리 공격 불가능, 원거리 공격, 원거리 공격 불가능
enum	{
			ON_MOUSE_BATTLE_SUB_NONE = 0,
			ON_MOUSE_BATTLE_SUB_SHORTRANGE, 
			ON_MOUSE_BATTLE_SUB_DISABLED_SHORTRANGE,
			ON_MOUSE_BATTLE_SUB_LONGRANGE,
			ON_MOUSE_BATTLE_SUB_DISABLED_LONGRANGE,
		};

struct SEnumInfo
{
	char			m_szContent[48];
};

// 커서 이미지
typedef enum ENUM_CURSOR_IMAGE
{
	ON_CURSOR_IMAGE_FIRST = 0,
	ON_CURSOR_IMAGE_HAND,
	ON_CURSOR_IMAGE_SWORD,
	ON_CURSOR_IMAGE_OLD_BATTLE_MOUSE,
	ON_CURSOR_IMAGE_MAX
};

// 마우스 종류 열거
class CMouseTypeEnum
{
	public:
		CMouseTypeEnum();
		~CMouseTypeEnum();

	private:
		static SEnumInfo			m_pMouseTypeEnum[ON_MOUSE_MAX];

	private:
		static void	SetData( int index, char *pszStr );

	public:
		static ENUM_MOUSETYPE	GetData		( char *pszStr );
		static const char*		GetData		( ENUM_MOUSETYPE enIndex );
		static int				GetMaxCount	( void )	{ return ON_MOUSE_MAX; }
};

// 커서 이미지 파일 열거
class CCurosrImageEnum
{
	public:
		CCurosrImageEnum();
		~CCurosrImageEnum();

	private:
		static SEnumInfo			m_pCurosrImageEnum[ON_CURSOR_IMAGE_MAX];

	private:
		static void	SetData( int index, char *pszStr );

	public:
		static ENUM_CURSOR_IMAGE	GetData		( char *pszStr );
		static const char*			GetData		( ENUM_CURSOR_IMAGE enIndex );
		static int					GetMaxCount	( void )	{ return ON_CURSOR_IMAGE_MAX; }
};


#endif // !defined(AFX_MOUSETYPEENUM_H__70E13576_FDAC_43F4_A8AD_0995E308A4EB__INCLUDED_)
