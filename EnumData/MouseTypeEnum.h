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
	ON_MOUSE_FIRST = 0,						// ����
	ON_MOUSE_BASIC,							// �⺻ ���콺
	ON_MOUSE_BATTLE_SWORD,					// ��. ���� ���콺
	ON_MOUSE_FIELD_BATTLE_BOW,				// ������ �ʵ� ���� ���Ÿ� ���� ���� ���콺
	ON_MOUSE_BATTLE_MAGIC,					// ���� ���, ĳ��Ʈ
	ON_MOUSE_BATTLE_HEAL,					// ġ��
	ON_MOUSE_BATTLE_HOVER_CHARACTER,		// Ŀ�� �Ʒ� ĳ���� ����. ����
	ON_MOUSE_GOVIL,							// ���� ����
	ON_MOUSE_GETITEM,						// ������ ����
	ON_MOUSE_SCROLL_6H,						// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_7H30M,					// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_9H,						// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_10H30M,					// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_12H,					// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_1H30M,					// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_3H,						// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_SCROLL_4H30M,					// ���� �� �� ��ũ�� �� ���콺 ��ư
	ON_MOUSE_MAX
};

// ���� ���콺 ���� Ÿ��. �ٰŸ� ����, �ٰŸ� ���� �Ұ���, ���Ÿ� ����, ���Ÿ� ���� �Ұ���
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

// Ŀ�� �̹���
typedef enum ENUM_CURSOR_IMAGE
{
	ON_CURSOR_IMAGE_FIRST = 0,
	ON_CURSOR_IMAGE_HAND,
	ON_CURSOR_IMAGE_SWORD,
	ON_CURSOR_IMAGE_OLD_BATTLE_MOUSE,
	ON_CURSOR_IMAGE_MAX
};

// ���콺 ���� ����
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

// Ŀ�� �̹��� ���� ����
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
