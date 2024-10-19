//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ĳ���� �ʻ� ��� Ŭ����
//	File Name		: OnlinePortrait.cpp
//	Birth Date		: 2003. 12. 27.
//	Creator			: �� �� �� (actdoll@joyon.com)
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		ĳ������ �ʻ��� ����ϴ� Ŭ�����Դϴ�.
//		��쿡 ���� ��� ����� �ٸ��� �� �� �ֽ��ϴ�.
//		�� Ŭ������ ���� ��ü�� �����ϴ� Ŭ������⺸�� ������ �۾��� �����ϴ� Ŭ���� ������
//		�ι� �ʻ��� �ʿ��� ��� �ش� ĳ������ �ε����� �������� ���ϴ� ������ �ʻ��� ����� �ݴϴ�.
//		�ΰ������� �ʻ� ��½� ǥ���� �� �ִ� ������ ����, ������ ���� ���� � ���� �۾��� �� �ְ�
//		�Ͽ����ϴ�.
//
//===================================================================================================
#include <GSL.h>

#include "OnlineWorld.h"
#include "OnlinePortrait.h"
#include "GSCDefine.h"

#include <kindinfo.h>

// #include <crtdbg.h>
#include <assert.h>

#include "..\EnumData\Enum_KI_Character.h"		// KindInfo - Character


extern	_KindInfo				KI[];								// ĳ���� ������ ������ 

extern	unsigned char DarkTable[][256]; // ��ο� �ܰ� ;

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2003-12-23)
// Desc: ���ǵ�
//-----------------------------------------------------------------------------
const	UI32	DF_VALUE_PORTRAIT_NPC_NONE			= 10000000;

const	UI32	DF_VALUE_PORTRAIT_NPC_GOV			= 1;				// ��û ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_FARM			= 2;				// ���� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_WEAPONSHOP	= 3;				// ������ ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_NORMALMARKET	= 4;				// ���� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_CROPMARKET	= 5;				// ���� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_DRUGMARKET	= 6;				// ��� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_HARVOR		= 7;				// �ε� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_DOCTOR		= 8;				// �ǿ� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_BANK			= 9;				// ���� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_BARRACK		= 10;				// �뺴�缺�� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_GBARRACK		= 11;				// ����缺�� ���� �ʻ�
const	UI32	DF_VALUE_PORTRAIT_NPC_MBARRACK		= 12;				// �������ü� ���� �ʻ�


const	char	*DF_STR_PORTRAIT_SRCFILE_NPC					= "Online\\GameSpr\\mercenary\\PORTRAIT_NPC.Spr";						// NPC
const	char	*DF_STR_PORTRAIT_SRCFILE_NORMAL					= "Online\\GameSpr\\mercenary\\PORTRAIT_INVEN.Spr";						// �Ϲ�

const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_BG				= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_BASE_MEDIUM.Spr";			// �뺴â - ���(���� ��)	
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_BG_LARGE		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_BASE_LARGE.Spr";			// �뺴â - ���(���� ��)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_CG				= "Online\\GameSpr\\mercenary\\PORTRAIT_MERCENARY_S.Spr";				// �뺴â - �뺴 �ʻ� ����(���� ��)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_CG_LARGE		= "Online\\GameSpr\\mercenary\\PORTRAIT_MERCENARY_L.Spr";				// �뺴â - �뺴 �ʻ� ����(���� ��)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_HEAVY		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_HEAVY.Spr";			// �뺴â - ������(��ü��)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_LVUP		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_LVUP.Spr";				// �뺴â - ������(������)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_IMPEND		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_NEARLY.Spr";			// �뺴â - ������(��ó?)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_DIEOVER	= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_DIEOVER.Spr";			// �뺴â - ������(�������� ������� ǥ�ÿ�)

//-----------------------------------------------------------------------------
// Name: Init()														[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ����
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init()
{
	Init_NPC		();
	Init_Normal		();
	Init_Mercenary	();
	CreateAllInfo	();
}

//-----------------------------------------------------------------------------
// Name: Init_NPC()													[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - NPC �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_NPC()
{
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_NPC,			m_SprNPC );
	m_siNPCWidth	= m_SprNPC.Header.Xsize;
	m_siNPCHeight	= m_SprNPC.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: Init_Normal()												[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - �Ϲ� �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_Normal()
{
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_NORMAL,		m_SprNormal );	
	m_siNormalWidth		= m_SprNormal.Header.Xsize;
	m_siNormalHeight	= m_SprNormal.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: Init_Mercenary()											[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - �뺴 �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_Mercenary()
{
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_BG,				m_SprMercenBG );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_BG_LARGE,			m_SprMercenBGLarge );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_CG,				m_SprMercen );		
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_CG_LARGE,			m_SprMercenLarge );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_HEAVY,		m_SprIconOverLoad );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_LVUP,		m_SprIconLvUp );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_IMPEND,		m_SprIconImpend );	
	clGrp.LoadXspr( (char*)DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_DIEOVER,		m_SprIconDieOver );	
	m_siMercenWidth			= m_SprMercen.Header.Xsize;
	m_siMercenHeight		= m_SprMercen.Header.Ysize;
	m_siMercenLargeWidth	= m_SprMercenLarge.Header.Xsize;
	m_siMercenLargeHeight	= m_SprMercenLarge.Header.Ysize;

}

//-----------------------------------------------------------------------------
// Name: Free()														[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ����
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free()
{
	MAP_PORTRAIT_ITER		iter;
	ST_PORTRAIT_INFO		Info;
	for( iter = m_PortraitInfo.begin(); iter != m_PortraitInfo.end(); iter++ )
	{
		Info = iter->second;
		Info.m_pDDSNormal->Release();
		Info.m_pDDSNormal = NULL;
	}
	m_PortraitInfo.clear();

	m_PortraitNPCInfo.clear();

	Free_NPC		();
	Free_Normal		();
	Free_Mercenary	();
}

//-----------------------------------------------------------------------------
// Name: Free_NPC()													[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - NPC �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free_NPC()
{
	clGrp.FreeXspr( m_SprNPC );
}

//-----------------------------------------------------------------------------
// Name: Free_Normal()												[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - �Ϲ� �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free_Normal()
{
	clGrp.FreeXspr( m_SprNormal );
}

//-----------------------------------------------------------------------------
// Name: Free_Mercenary												[private]
// Code: actdoll (2003-12-23)
// Desc: Ŭ���� ���� ���� - �뺴 �ʻ��
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free_Mercenary()
{
	clGrp.FreeXspr( m_SprMercenBG );	
	clGrp.FreeXspr( m_SprMercenBGLarge );	
	clGrp.FreeXspr( m_SprMercen );		
	clGrp.FreeXspr( m_SprMercenLarge );	
	clGrp.FreeXspr( m_SprIconOverLoad );	
	clGrp.FreeXspr( m_SprIconLvUp );	
	clGrp.FreeXspr( m_SprIconImpend );
	clGrp.FreeXspr( m_SprIconDieOver );	
}

//-----------------------------------------------------------------------------
// Name: DrawAsNPC
// Code: actdoll (2003-12-31)
// Desc: �ʻ� �׸��� - ������ �� �������� �����ϴ� �Ǹ� ĳ����
//		x, y		- �׸� �»�� ��ǥ
//		siNPCID		- �ش� NPC������ ID (NPC_KIND_... �ε����� ����ϸ� ��)
//		siNation	- ���� �ڵ� (ON_COUNTRY_... �ε����� ����ϸ� ��)
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNPC( SI32 x, SI32 y, SI32 siNPCID, SI32 siNation )
{
	// �ش� ID���� �������� Ȯ���Ѵ�.
	SI32					siIndex			= ( siNation * 100 ) + siNPCID;
	MAP_PORTRAIT_NPC_ITER	iter			= m_PortraitNPCInfo.find( siIndex );
	ST_PORTRAIT_NPC_INFO	stPortNpcInfo	= iter->second;
	
	// �׸��� �׷��ش�.
	clGrp.PutSpriteT( x, y, m_siNPCWidth, m_siNPCHeight, stPortNpcInfo.m_pSpr );
}

//-----------------------------------------------------------------------------
// Name: DrawAsNormal
// Code: actdoll (2003-12-31)
// Desc: �ʻ� �׸��� - ����, ���� �ŷ����� ���� �󱼵��� �׸���.
//		x, y					- �׸� �»�� ��ǥ
//		siID					- �ش� ������ DB Ȥ�� ���� ID
//		bIDTypeIsDB				- siID�� DB ID�� �� ��� true, ���� ID�� �� ��� false
//		siDrawFlag				- DF_FLAG_PORTRAIT_DRAW_... �÷��׸� �����Ͽ� ���� ����� �����Ѵ�.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNormal( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siDrawFlag )
{
	SI32	siDBID;

	// �ش� ID���� �������� Ȯ���Ѵ�.
	if( !bIDTypeIsDB )
	{
		siDBID	= TransKindImjinToOnline( siID, false );
//		_ASSERTE( siDBID >= 0 );
	}
	else
	{
//		_ASSERTE( TransKindOnlineToImjin( siID, false ) >= 0 );
		siDBID	= siID;
	}

	// �뺴 �׸� ������ �������� üũ
	ST_PORTRAIT_INFO	stPortInfo;
	MAP_PORTRAIT_ITER	iter		= m_PortraitInfo.find( siDBID );
	BOOL		bCanDrawPortrait	= false;
	if( iter != m_PortraitInfo.end() )	
	{
		bCanDrawPortrait	= true;
		stPortInfo			= iter->second;		
	}
	
	// �ʻ� �׸���(�� �ɼ��� ���ٸ� �׷���)
	if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
	{
		if	( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )	// ���������� �׸� ���
		{
			clGrp.PutSpriteT		( x, y, m_siNormalWidth, m_siNormalHeight, stPortInfo.m_pSprNormal, DarkTable[ 5 ] );	
		}
		else											// �������� �׸� ���
		{
			clGrp.PutSpriteT		( x, y, m_siNormalWidth, m_siNormalHeight, stPortInfo.m_pSprNormal );
		}
	}

	// ������ �׸��� - ������
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_LVUP );
	// ������ �׸��� - �������� ���� �����
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_IMPEND );
	// ������ �׸��� - �߷��ʰ�
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 76, y + 24, DF_FLAG_PORTRAIT_ICON_OVERLOAD );
	
	// ���̶���Ʈ �׸���
	if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 3, y - 3, m_siNormalWidth + 6, m_siNormalHeight + 6 );
}

//-----------------------------------------------------------------------------
// Name: DrawAsNormalEx
// Code: actdoll (2003-12-31)
// Desc: �ʻ� �׸��� - �ʻ��� Ȯ�� ��Ҹ� ������ �׸���
//		lpDDSDest				- ��� ���ǽ�
//		x, y					- �׸� �»�� ��ǥ
//		siID					- �ش� ������ DB Ȥ�� ���� ID
//		bIDTypeIsDB				- siID�� DB ID�� �� ��� true, ���� ID�� �� ��� false
//		siNewWidth, siNewHeight	- �ε��� ���� �ƴ� ������ ũ��� �׷��ش�.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNormalEx( LPDIRECTDRAWSURFACE7 lpDDSDest, SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siNewWidth, SI32 siNewHeight )
{
	SI32	siDBID;

	if( !lpDDSDest )			return;

	// �ش� ID���� �������� Ȯ���Ѵ�.
	if( !bIDTypeIsDB )
	{
		siDBID	= TransKindImjinToOnline( siID, false );
//		_ASSERTE( siDBID >= 0 );
	}
	else
	{
//		_ASSERTE( TransKindOnlineToImjin( siID, false ) >= 0 );
		siDBID	= siID;
	}

	MAP_PORTRAIT_ITER	iter		= m_PortraitInfo.find( siDBID );
	if( iter == m_PortraitInfo.end() )		return;
	ST_PORTRAIT_INFO	stPortInfo	= iter->second;
	
	// �׷��ֱ�
	RECT	rcDest;
	( siNewWidth <= 0 )		?	( siNewWidth	= x + m_siNormalWidth )		: ( siNewWidth	= x + siNewWidth );
	( siNewHeight <= 0 )	?	( siNewHeight	= y + m_siNormalHeight )	: ( siNewHeight	= y + siNewHeight );
	SetRect( &rcDest, x, y, siNewWidth, siNewHeight );

	// actdoll (2004/01/07 16:33) : 
	//	�ణ ���ŷο� ����̴�.
	//	�ܺο��� ���ǽ��� ��ŷ�ϰ� �ִ� ��쿡 �ʻ��� �� ������� �׳� ������ �ϸ� DDERR_SURFACEBUSY ������ ��� �߻��ϰ� �ȴ�.
	//	������ ���ǽ��� ��ŷ ���� ��� �Ѿ�� ���ǽ��� ���� ��� ������ �ϰ� ���� �� �ٽ� �⵵�� �Ѵ�.
	//	��ŷ���� ���ǽ��� ��κ� �������� �Ѱ��ִ� lpDDSDest�� ��찡 ���� ������ �̳��� ��� ������ �ϵ��� �Ѵ�.
	//	���� lpDDSDest�� ��ŷ ���ǽ��� �ƴ϶�� ���������̴�. 
	if( clGrp.IsLockSurface() )
	{
		RECT	rcClip;
		clGrp.GetClipRect( &rcClip );
		clGrp.UnlockSurface( lpDDSDest );
		lpDDSDest->Blt( &rcDest, stPortInfo.m_pDDSNormal, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL );
		clGrp.SetClipArea( rcClip.left, rcClip.top, rcClip.right, rcClip.bottom );
		clGrp.LockSurface( lpDDSDest );
	}
	else
	{
		lpDDSDest->Blt( &rcDest, stPortInfo.m_pDDSNormal, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL );
	}
}

//-----------------------------------------------------------------------------
// Name: DrawAsMercenary
// Code: actdoll (2003-12-31)
// Desc: �ʻ� �׸��� - �뺴â �ʻ� ����
//		x, y		- �׸� �»�� ��ǥ
//		siID		- �ش� NPC������ DB Ȥ�� ���� ID
//		bIDTypeIsDB	- siID�� DB ID�� �� ��� true, ���� ID�� �� ��� false
//		siDrawFlag	- DF_FLAG_PORTRAIT_DRAW_... �÷��׸� �����Ͽ� ���� ����� �����Ѵ�.
//		uiHPBase	- HP �ٸ� �׸� ���� �⺻ ��ġ, 0�̸� �׷����� �ʴ´�.
//		uiHPNow		- HP �ٸ� �׸� ���� ���� ��ġ, 0�̸� �׷����� �ʴ´�.
//		uiMPBase	- MP �ٸ� �׸� ���� �⺻ ��ġ, 0�̸� �׷����� �ʴ´�.
//		uiMPNow		- MP �ٸ� �׸� ���� ���� ��ġ, 0�̸� �׷����� �ʴ´�.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsMercenary( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siDrawFlag, 
										UI32 uiHPBase, UI32 uiHPNow, UI32 uiMPBase, UI32 uiMPNow )
{
	SI32	siDBID;

	// �ش� ID���� �������� Ȯ���Ѵ�.
	if( !bIDTypeIsDB )
	{
		siDBID	= TransKindImjinToOnline( siID, false );
	}
	else
	{
		siDBID	= siID;
	}

	// �뺴 �׸� ������ �������� üũ
	ST_PORTRAIT_INFO	stPortInfo;
	MAP_PORTRAIT_ITER	iter		= m_PortraitInfo.find( siDBID );
	BOOL		bCanDrawPortrait	= false;
	if( iter != m_PortraitInfo.end() )	
	{
		bCanDrawPortrait	= true;
		stPortInfo			= iter->second;		
	}
	
	
	// Ȯ�������� ���� ���
	if( siDrawFlag & DF_FLAG_PORTRAIT_LARGE )
	{
		// ���
		if( siDrawFlag & DF_FLAG_PORTRAIT_BG )					clGrp.PutSpriteLightCT	( x, y, m_SprMercenBGLarge.Header.Xsize,	m_SprMercenBGLarge.Header.Ysize,	m_SprMercenBGLarge.Image );

		// �ʻ� �׸���(�� �ɼ��� ���ٸ� �׷���)
		if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
		{
			// �ʻ� �׸� ���������� �׸� ���
	//		if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteLightCT	( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight,	stPortInfo.m_pSprMercenLarge );
			if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight, stPortInfo.m_pSprMercenLarge, DarkTable[ 5 ] );	
			// �ʻ� �׸� �������� �׸� ���
			else												clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight,	stPortInfo.m_pSprMercenLarge );
		}

		// ������ �׸���(�׾��� ���� hp/mp�� ǥ������ �ʴ´�.)
		// HP	// MP
		if( uiHPBase > 0 && uiHPNow > 0 )				
		{
			Draw_StatusBar( x + 2, y + 35, m_SprMercenBGLarge.Header.Xsize - 4, 4, (R32)uiHPNow / (R32)uiHPBase, true, true );	
			// ���� MP ĳ���Ͷ�� ����ش�.
			if( uiMPBase > 0 )				Draw_StatusBar( x + 2, y + 40, m_SprMercenBGLarge.Header.Xsize - 4, 2, (R32)uiMPNow / (R32)uiMPBase, false, true );
		}

		// ������ �׸��� - �߷��ʰ�
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 76, y + 24, DF_FLAG_PORTRAIT_ICON_OVERLOAD );
		// ������ �׸��� - �������� ���� �����
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_IMPEND );
		// ������ �׸��� - ������
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_LVUP );

		// ���̶���Ʈ �׸���
		if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 1, y - 1, m_SprMercenBGLarge.Header.Xsize + 2, m_SprMercenBGLarge.Header.Ysize + 2 );
	}
	// �Ϲ������� ���� ���
	else
	{
		// ���
		if( siDrawFlag & DF_FLAG_PORTRAIT_BG )					clGrp.PutSpriteLightCT	( x, y, m_SprMercenBG.Header.Xsize,			m_SprMercenBG.Header.Ysize,			m_SprMercenBG.Image );

		// �ʻ� �׸���(�� �ɼ��� ���ٸ� �׷���)
		if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
		{
			// �ʻ� �׸� ���������� �׸� ���
	//		if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteLightCT	( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen );
			if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen, DarkTable[ 5 ] );	
			// �ʻ� �׸� �������� �׸� ���
			else												clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen );
		}

		// ������ �׸���(�׾��� ���� hp/mp�� ǥ������ �ʴ´�.)
		// HP	// MP
		if( uiHPBase > 0 && uiHPNow > 0 )				
		{
			Draw_StatusBar( x + 2, y + 29, m_SprMercenBG.Header.Xsize - 4, 4, (R32)uiHPNow / (R32)uiHPBase, true, true );	
			if( uiMPBase > 0 )				Draw_StatusBar( x + 2, y + 34, m_SprMercenBG.Header.Xsize - 4, 2, (R32)uiMPNow / (R32)uiMPBase, false, true );
		}

		// ������ �׸��� - �߷��ʰ�
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 64, y + 18,	DF_FLAG_PORTRAIT_ICON_OVERLOAD );
		// ������ �׸��� - �������� ���� �����
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 59, y + 2,	DF_FLAG_PORTRAIT_ICON_IMPEND );
		// ������ �׸��� - ������
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 59, y + 2,	DF_FLAG_PORTRAIT_ICON_LVUP );

		// ���̶���Ʈ �׸���
		if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 1, y - 1, m_SprMercenBG.Header.Xsize + 2, m_SprMercenBG.Header.Ysize + 2);
	}
}

//-----------------------------------------------------------------------------
// Name: Draw_Icons
// Code: actdoll (2003-12-23)
// Desc: �������� �׷��ش�.
//		x, y			- �������� �»�� ��ǥ
//		siDrawFlag_Icon	- �������� ����(DF_FLAG_PORTRAIT_ICON_... �ø���)
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_Icons( SI32 x, SI32 y, SI32 siDrawFlag_Icon )
{
	// ������ �׸��� - ������ �ʻ������� �� ������ ǥ�ÿ�
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_DIEOVER )
	{
		clGrp.PutSpriteLightCT	( x, y,	m_SprIconDieOver.Header.Xsize,	m_SprIconDieOver.Header.Ysize,		m_SprIconDieOver.Image );
	}
	// ������ �׸��� - ������
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_LVUP )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconLvUp.Header.Xsize,		m_SprIconLvUp.Header.Ysize,			m_SprIconLvUp.Image );
	}
	// ������ �׸��� - �������� ���� �����
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_IMPEND )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconImpend.Header.Xsize,	m_SprIconImpend.Header.Ysize,		m_SprIconImpend.Image );
	}
	// ������ �׸��� - �߷��ʰ�
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_OVERLOAD )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconOverLoad.Header.Xsize,	m_SprIconOverLoad.Header.Ysize,		m_SprIconOverLoad.Image );
	}
}

//-----------------------------------------------------------------------------
// Name: Draw_HighLight
// Code: actdoll (2003-12-23)
// Desc: ���̶���Ʈ�� �׷��ش�.
//		x, y			- ���̶���Ʈ �»�� ��ǥ
//		width, height	- ���̶���Ʈ�� ����, �ʺ�
//		color			- ���̶���Ʈ�� ���� �ε���(�⺻�� 0x27)
//		uiThickness		- ���̶���Ʈ�� �ȼ� �β�(�⺻�� 2)
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_HighLight( SI32 x, SI32 y, SI32 width, SI32 height, UI08 color, UI32 uiThickness )
{
	if( width < 1 || height < 1 || uiThickness < 1 )		return;
	clGrp.Box( x, y, x + width - 1, y + height - 1, color, 2 );
}

//-----------------------------------------------------------------------------
// Name: Draw_HighLight
// Code: actdoll (2003-12-23)
// Desc: ���̶���Ʈ�� �׷��ش�.
//		rcDest			- ���̶���Ʈ ��ȹ
//		color			- ���̶���Ʈ�� ���� �ε���(�⺻�� 0x27)
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_HighLight( RECT &rcDest, UI08 color, UI32 uiThickness )
{
	if( rcDest.right - 1 < rcDest.left )		return;
	if( rcDest.bottom - 1 < rcDest.top )		return;
	if( uiThickness < 1 )						return;
	clGrp.Box( rcDest.left, rcDest.top, rcDest.right - 1, rcDest.bottom - 1, color, uiThickness );
}

//-----------------------------------------------------------------------------
// Name: Draw_StatusBar
// Code: actdoll (2003-12-23)
// Desc: �������ͽ� �ٸ� �׷��ش�.
//		x, y			- �� ���� �»�� ��ǥ
//		width, height	- �������ͽ��� ����, �ʺ�
//		fRate			- �������ͽ� ���� ����
//		bIsHP			- �������ͽ� �ٰ� HP��� true, MP��� false
//		bShowEmpty		- ������� ���� â�� �����ٰŶ�� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_StatusBar( SI32 x, SI32 y, SI32 width, SI32 height, R32 fRate, BOOL bIsHP, BOOL bShowEmpty )
{
	if( width < 1 || height < 1 )			return;
	if( fRate <= 0.0f && !bShowEmpty )		return;	// ��ġ�� 0�� �Ǿ��� ����ִ� �� ������ ����� ����
	if( fRate > 1.0f )		fRate = 1.0f;			// ���� �Ѿ���� �׳� �������Ѷ�

	UI08	uiColor, uiShadowColor;
	if( bIsHP )		// HPŸ��
	{								// ��� ��									// ��� ��
		if		( fRate > 0.66f )	uiColor = 0x44, uiShadowColor = 0xe5;//uiColor = 0xe7, uiShadowColor = 0xe5;	
		else if	( fRate > 0.33f )	uiColor = 0x46, uiShadowColor = 0xe1;//uiColor = 0xe3, uiShadowColor = 0xe1;
		else						uiColor = 0x45, uiShadowColor = 0xe9;//uiColor = 0xeb, uiShadowColor = 0xe9;

		// �ϴ� ü�� ǥ�� ���ְ�
		UI32	uiLength		= width * fRate;
		if( uiLength >= 1 )			clGrp.FillBox	( x,			y,		x + uiLength - 1,	y + height - 1,		uiColor );	// ü�� ǥ��
	}
	else			// MPŸ��
	{
		uiColor = 0xb4, uiShadowColor = 0x92;
	//	if( fRate <= 0.0f )					return;
		UI32	uiLength		= width * fRate;
		// �ϴ� �Ʒ��� �׸��� �׸��� ���� ������ �׸���. ���� ��� ���̹Ƿ� ��¿ �� ����.
		if( uiLength >= 1 )			clGrp.FillBox	( x,			y,		x + uiLength - 1,	y + height - 1,		uiColor );			// ���� ǥ��

	}
}

//-----------------------------------------------------------------------------
// Name: Draw_StatusBar
// Code: actdoll (2003-12-23)
// Desc: �������ͽ� �ٸ� �׷��ش�.
//		fRate			- �������ͽ��� ����
//		fRate			- �������ͽ� ���� ����
//		bIsHP			- �������ͽ� �ٰ� HP��� true, MP��� false
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_StatusBar( RECT &rcDest, R32 fRate, BOOL bIsHP, BOOL bShowEmpty )
{
	Draw_StatusBar( rcDest.left, rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, fRate, bIsHP );
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfNormal
// Code: actdoll (2003-12-23)
// Desc: NPC �ʻ��� ũ�⸦ �޴´�.
//		siWidth, siHeight	- �ʻ��� �ʺ�, ���̰� ����� ����
//-----------------------------------------------------------------------------
void	OnlinePortrait::GetRangeOfNPC( SI32 &siWidth, SI32 &siHeight )
{
	siWidth		= m_SprNPC.Header.Xsize;
	siHeight	= m_SprNPC.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfNormal
// Code: actdoll (2003-12-23)
// Desc: �Ϲ� �ʻ��� ũ�⸦ �޴´�.
//		siWidth, siHeight	- �ʻ��� �ʺ�, ���̰� ����� ����
//-----------------------------------------------------------------------------
void	OnlinePortrait::GetRangeOfNormal( SI32 &siWidth, SI32 &siHeight )
{
	siWidth		= m_SprNormal.Header.Xsize;
	siHeight	= m_SprNormal.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfMercenary
// Code: actdoll (2003-12-23)
// Desc: �뺴 �ʻ��� ũ�⸦ �޴´�.
//		siWidth, siHeight	- �ʻ��� �ʺ�, ���̰� ����� ����
//		bGetAsWide			- ū �ʻ��� ���� �������� true, �Ϲ��� ũ�⸦ �������� false (�⺻�� false)
//-----------------------------------------------------------------------------
void	OnlinePortrait::GetRangeOfMercenary( SI32 &siWidth, SI32 &siHeight, BOOL bGetAsWide )
{
	if( bGetAsWide )
	{
		siWidth		= m_SprMercenBGLarge.Header.Xsize;
		siHeight	= m_SprMercenBGLarge.Header.Ysize;
	}
	else
	{
		siWidth		= m_SprMercenBG.Header.Xsize;
		siHeight	= m_SprMercenBG.Header.Ysize;
	}
}

//-----------------------------------------------------------------------------
// Name: Init_UnitInfo												[private]
// Code: actdoll (2003-12-23)
// Desc: �ʻ� ����Ʈ�� ������ �ϳ��� �����Ѵ�.
//		����Ǵ� �ʻ��� �뺴 �ʻ�� �Ϲ� �ʻ� �ΰ�����.
//		siGameID	- ���� �ʵ忡�� ����ϴ� ĳ���� ID
//		siNPCFontNo	- NPC �ʻ� ���Ͽ� ���Ƿ� ������ �� NPC �ʻ��� �׸� ��Ʈ ��ȣ, ����Ʈ�� -1�̸�
//					�� ������ siGameID�� NPC������ ���� ���� �۵��ȴ�.
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_UnitInfo( SI32 siGameID )
{
	SI32					siDBID, siFontNoNormal, siFontNoMercenary;
	SI32					ClipLeft, ClipTop, ClipRight, ClipBottom;
	XSPR_HEADER				*pHeader;
	ST_PORTRAIT_INFO		stPortInfo = {0,};

	// �⺻ ���� ����
	siDBID							= TransKindImjinToOnline( siGameID, false );					// DB ID ���
	// actdoll (2003/12/30 21:37) : DB ID�� ������ �Դٸ� ���� ĳ���̴�.
	if( siDBID < 0 )	return;
	stPortInfo.m_siGameID			= siGameID;														// �ʵ� ID ���
	
	siFontNoNormal		= KI[siGameID].GetOnlinePortraitFont();		// �Ϲ� �ʻ󿡼��� ��Ʈ ��ȣ
	siFontNoMercenary	= KI[siGameID].GetOnlineMercenaryFont();	// �뺴 �ʻ󿡼��� ��Ʈ ��ȣ
	


	// �Ϲ� �ʻ� ���� ����
	pHeader						= &m_SprNormal.Header;											// �׸� ������ ��� ���� ����
	stPortInfo.m_pSprNormal		= &m_SprNormal.Image[pHeader->Start[siFontNoNormal]];			// �̹��� ������ ����

	clGrp.CreateOffScreen	( stPortInfo.m_pDDSNormal, pHeader->Xsize, pHeader->Ysize, TRUE, FALSE );	// ���ǽ� ����
	clGrp.ClearSurface		( stPortInfo.m_pDDSNormal );												// ���ǽ� Ŭ��
		// �׸��� ���ǽ��� ����
	clGrp.GetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );
	if(clGrp.LockSurface( stPortInfo.m_pDDSNormal ))
	{
		clGrp.SetClipArea	( 0, 0, pHeader->Xsize - 1,	pHeader->Ysize - 1 );					// ���� ��ȹ ���
		clGrp.FillBox		( 0, 0, pHeader->Xsize - 1,	pHeader->Ysize - 1, TRANSCOLOR );		// �÷�Ű���� �����ϴ� ������ ���� ������ ����� �÷�Ű�� �ѹ� ĥ���ش�.
		clGrp.PutSpriteT	( 0, 0, pHeader->Xsize,		pHeader->Ysize,							// �׸� ����
								&m_SprNormal.Image[pHeader->Start[siFontNoNormal]] );
		clGrp.UnlockSurface	( stPortInfo.m_pDDSNormal );
	}
	clGrp.SetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );



	if( siFontNoMercenary >= 0 )// 0�̻��̶�� �뺴 �ʻ��� �ִ�.
	{
		// �뺴 �ʻ� ���� ����
		pHeader							= &m_SprMercen.Header;											// �׸� ������ ��� ���� ����
		stPortInfo.m_pSprMercen			= &m_SprMercen.Image[pHeader->Start[siFontNoMercenary]];		// �̹��� ������ �޴´�.
		
		// ū �뺴 �ʻ� ���� ����
		pHeader							= &m_SprMercenLarge.Header;										// �׸� ������ ��� ���� ����
		stPortInfo.m_pSprMercenLarge	= &m_SprMercenLarge.Image[pHeader->Start[siFontNoMercenary]];		// �̹��� ������ �޴´�.
	}
	// ������ ����Ʈ�� ����
	m_PortraitInfo[siDBID]	= stPortInfo;
}

//-----------------------------------------------------------------------------
// Name: Init_NPCInfo												[private]
// Code: actdoll (2003-12-23)
// Desc: NPC�� �ʻ� ����Ʈ�� ������ �ϳ��� �����Ѵ�.
//		siNation	- �ش� NPC�� ������ȣ
//		siNPCID		- ���� �ʵ忡�� ����ϴ� NPC ĳ������ ID
//		siNPCFontNo	- �ش� NPC�� �׸� ��Ʈ ��ȣ
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_NPCInfo( SI32 siNation, SI32 siNPCID, SI32 siNPCFontNo )
{
	SI32					siIndex;
	XSPR_HEADER				*pHeader;
	ST_PORTRAIT_NPC_INFO	stPortNpcInfo = {0,};

	siIndex		= ( siNation * 100 ) + siNPCID;
	
	// �⺻ ���� ����
	stPortNpcInfo.m_siUnitID	= siNPCID;
	stPortNpcInfo.m_siNation	= siNation;
	
	// NPC �ʻ� ���� ����
	pHeader						= &m_SprNPC.Header;									// �׸� ������ ��� ���� ����
	stPortNpcInfo.m_pSpr		= &m_SprNPC.Image[pHeader->Start[siNPCFontNo]];		// �̹��� ������ �޴´�.
	
	// ������ ����Ʈ�� ����
	m_PortraitNPCInfo[siIndex]	= stPortNpcInfo;
}

//-----------------------------------------------------------------------------
// Name: CreateAllInfo												[private]
// Code: actdoll (2003-12-23)
// Desc: �ʻ���� ���⼭ "�ϵ��ڵ�"���� �����Ѵ�. ��, ���̿�...T^T)
//-----------------------------------------------------------------------------
void	OnlinePortrait::CreateAllInfo()
{
	SI32	i, j, siFontNo;
	// �ʵ� ĳ���� ���� �ʱ�ȭ
	for( i=KIND_MANAGER1; i<KIND_ON_MAX; i++)
	{
		Init_UnitInfo( i );
	}

	// �������� ȸ��
	for( i=ON_COUNTRY_KOREA; i<ON_COUNTRY_ALL; i++ )
	{
		// ���κ��� ȸ��
		for( j=DF_VALUE_PORTRAIT_NPC_GOV; j<=DF_VALUE_PORTRAIT_NPC_MBARRACK; j++ )
		{
			// ��Ʈ ��ȣ�� �����ؼ� ����ִ´�.
			siFontNo = ( i - 1 ) * 20 + j;
			Init_NPCInfo( i, j, siFontNo );
		}
	}
}
