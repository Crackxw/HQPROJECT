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
		
	SET_PROTOCOL( ON_MOUSE_BASIC					)	// �⺻ ���콺
	SET_PROTOCOL( ON_MOUSE_BATTLE_SWORD				)	// ��. ���� ���콺
	SET_PROTOCOL( ON_MOUSE_FIELD_BATTLE_BOW			)	// ������ �ʵ� ���� ���Ÿ� ���� ���� ���콺
	SET_PROTOCOL( ON_MOUSE_BATTLE_MAGIC				)	// ���� ���, ĳ��Ʈ
	SET_PROTOCOL( ON_MOUSE_BATTLE_HEAL				)	// ġ��
	SET_PROTOCOL( ON_MOUSE_BATTLE_HOVER_CHARACTER	)	// Ŀ�� �Ʒ� ĳ���� ����. ����
	SET_PROTOCOL( ON_MOUSE_GOVIL					)	// ���� ����
	SET_PROTOCOL( ON_MOUSE_GETITEM					)	// ������ ����
	SET_PROTOCOL( ON_MOUSE_SCROLL_6H				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_7H30M				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_9H				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_10H30M			)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_12H				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_1H30M				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_3H				)	// ���� �� �� ��ũ�� �� ���콺 ��ư
	SET_PROTOCOL( ON_MOUSE_SCROLL_4H30M				)	// ���� �� �� ��ũ�� �� ���콺 ��ư

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



