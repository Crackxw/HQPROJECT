// MouseTypeEnum.cpp: implementation of the CMouseTypeEnum class.
//
//////////////////////////////////////////////////////////////////////

#include "MouseTypeEnum.h"
#include <windows.h>

SEnumInfo CMouseTypeEnum::m_pMouseTypeEnum[ON_MOUSE_MAX] = {0, };
CMouseTypeEnum			g_MouseTypeEnum;

SEnumInfo CCurosrImageEnum::m_pCurosrImageEnum[ON_CURSOR_IMAGE_MAX] = {0, };
CCurosrImageEnum		g_CurosrImageEnum;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouseTypeEnum::CMouseTypeEnum()
{
	ZeroMemory( m_pMouseTypeEnum, (sizeof(SEnumInfo) * ON_MOUSE_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
	SET_PROTOCOL( ON_MOUSE_BASIC					)	// 기본 마우스
	SET_PROTOCOL( ON_MOUSE_BATTLE_SWORD				)	// 검. 공격 마우스
	SET_PROTOCOL( ON_MOUSE_FIELD_BATTLE_BOW			)	// 공성전 필드 전투 원거리 공격 관련 마우스
	SET_PROTOCOL( ON_MOUSE_BATTLE_MAGIC				)	// 마법 사용, 캐스트
	SET_PROTOCOL( ON_MOUSE_BATTLE_HEAL				)	// 치료
	SET_PROTOCOL( ON_MOUSE_BATTLE_HOVER_CHARACTER	)	// 커서 아래 캐릭터 존재. 전투
	SET_PROTOCOL( ON_MOUSE_GOVIL					)	// 마을 들어가기
	SET_PROTOCOL( ON_MOUSE_GETITEM					)	// 아이템 집기
	SET_PROTOCOL( ON_MOUSE_SCROLL_6H				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_7H30M				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_9H				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_10H30M			)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_12H				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_1H30M				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_3H				)	// 전투 중 맵 스크롤 시 마우스 버튼
	SET_PROTOCOL( ON_MOUSE_SCROLL_4H30M				)	// 전투 중 맵 스크롤 시 마우스 버튼

#undef	SET_PROTOCOL
}

CMouseTypeEnum::~CMouseTypeEnum()
{

}

void	CMouseTypeEnum::SetData( int index, char *pszStr )
{
	strcpy( m_pMouseTypeEnum[index].m_szContent, pszStr ); 
}

ENUM_MOUSETYPE	CMouseTypeEnum::GetData( char *pszStr )
{
	if ( !pszStr )
		return ON_MOUSE_BASIC;

	for ( register int i = ON_MOUSE_FIRST; i < ON_MOUSE_MAX; i++ )	
	{
		if( strcmp( m_pMouseTypeEnum[i].m_szContent, pszStr ) == 0 )
			return (ENUM_MOUSETYPE)i;
	}

	return ON_MOUSE_BASIC;
}

const char* CMouseTypeEnum::GetData( ENUM_MOUSETYPE enIndex )
{
	if( enIndex <= ON_MOUSE_FIRST || enIndex >= ON_MOUSE_MAX )
		return m_pMouseTypeEnum[ON_MOUSE_BASIC].m_szContent;

	return (const char*)(m_pMouseTypeEnum[enIndex].m_szContent);
}
//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
CCurosrImageEnum::CCurosrImageEnum()
{
	ZeroMemory( m_pCurosrImageEnum, (sizeof(SEnumInfo) * ON_CURSOR_IMAGE_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
	SET_PROTOCOL( ON_CURSOR_IMAGE_HAND				)
	SET_PROTOCOL( ON_CURSOR_IMAGE_SWORD				)
	SET_PROTOCOL( ON_CURSOR_IMAGE_OLD_BATTLE_MOUSE	)

#undef	SET_PROTOCOL
}

CCurosrImageEnum::~CCurosrImageEnum()
{

}

void	CCurosrImageEnum::SetData( int index, char *pszStr )
{
	strcpy( m_pCurosrImageEnum[index].m_szContent, pszStr ); 
}

ENUM_CURSOR_IMAGE	CCurosrImageEnum::GetData( char *pszStr )
{
	if ( !pszStr )
		return ON_CURSOR_IMAGE_HAND;

	for ( register int i = ON_CURSOR_IMAGE_FIRST; i<ON_CURSOR_IMAGE_MAX; i++ )	
	{
		if( strcmp( m_pCurosrImageEnum[i].m_szContent, pszStr ) == 0 )
			return (ENUM_CURSOR_IMAGE)i;
	}

	return ON_CURSOR_IMAGE_HAND;
}

const char* CCurosrImageEnum::GetData( ENUM_CURSOR_IMAGE enIndex )
{
	if( enIndex <= ON_CURSOR_IMAGE_FIRST || enIndex >= ON_CURSOR_IMAGE_MAX )
		return m_pCurosrImageEnum[ON_CURSOR_IMAGE_HAND].m_szContent;

	return (const char*)(m_pCurosrImageEnum[enIndex].m_szContent);
}
//////////////////////////////////////////////////////////////////////



