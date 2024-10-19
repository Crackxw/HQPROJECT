//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 캐릭터 초상 출력 클래스
//	File Name		: OnlinePortrait.cpp
//	Birth Date		: 2003. 12. 27.
//	Creator			: 조 현 준 (actdoll@joyon.com)
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		캐릭터의 초상을 출력하는 클래스입니다.
//		경우에 따라 출력 양식을 다르게 쓸 수 있습니다.
//		이 클래스는 게임 자체를 구성하는 클래스라기보다 지정된 작업을 수행하는 클래스 내에서
//		인물 초상이 필요할 경우 해당 캐릭터의 인덱스를 기준으로 원하는 형태의 초상을 출력해 줍니다.
//		부가적으로 초상 출력시 표현할 수 있는 아이템 정보, 간단한 현재 상태 등도 같이 작업할 수 있게
//		하였습니다.
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


extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 

extern	unsigned char DarkTable[][256]; // 어두운 단계 ;

//-----------------------------------------------------------------------------
// Name: 
// Code: actdoll (2003-12-23)
// Desc: 정의들
//-----------------------------------------------------------------------------
const	UI32	DF_VALUE_PORTRAIT_NPC_NONE			= 10000000;

const	UI32	DF_VALUE_PORTRAIT_NPC_GOV			= 1;				// 관청 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_FARM			= 2;				// 농장 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_WEAPONSHOP	= 3;				// 무기점 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_NORMALMARKET	= 4;				// 시전 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_CROPMARKET	= 5;				// 싸전 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_DRUGMARKET	= 6;				// 약방 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_HARVOR		= 7;				// 부두 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_DOCTOR		= 8;				// 의원 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_BANK			= 9;				// 은행 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_BARRACK		= 10;				// 용병양성소 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_GBARRACK		= 11;				// 장수양성소 직원 초상
const	UI32	DF_VALUE_PORTRAIT_NPC_MBARRACK		= 12;				// 몬스터조련소 직원 초상


const	char	*DF_STR_PORTRAIT_SRCFILE_NPC					= "Online\\GameSpr\\mercenary\\PORTRAIT_NPC.Spr";						// NPC
const	char	*DF_STR_PORTRAIT_SRCFILE_NORMAL					= "Online\\GameSpr\\mercenary\\PORTRAIT_INVEN.Spr";						// 일반

const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_BG				= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_BASE_MEDIUM.Spr";			// 용병창 - 배경(비선택 시)	
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_BG_LARGE		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_BASE_LARGE.Spr";			// 용병창 - 배경(선택 시)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_CG				= "Online\\GameSpr\\mercenary\\PORTRAIT_MERCENARY_S.Spr";				// 용병창 - 용병 초상 보통(비선택 시)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_CG_LARGE		= "Online\\GameSpr\\mercenary\\PORTRAIT_MERCENARY_L.Spr";				// 용병창 - 용병 초상 대형(선택 시)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_HEAVY		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_HEAVY.Spr";			// 용병창 - 아이콘(과체적)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_LVUP		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_LVUP.Spr";				// 용병창 - 아이콘(레벨업)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_IMPEND		= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_NEARLY.Spr";			// 용병창 - 아이콘(근처?)
const	char	*DF_STR_PORTRAIT_SRCFILE_MERCEN_ICON_DIEOVER	= "Online\\GameSpr\\mercenary\\PORTRAIT_BOX_ICON_DIEOVER.Spr";			// 용병창 - 아이콘(아이템형 죽은사람 표시용)

//-----------------------------------------------------------------------------
// Name: Init()														[private]
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 생성
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
// Desc: 클래스 정보 생성 - NPC 초상용
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
// Desc: 클래스 정보 생성 - 일반 초상용
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
// Desc: 클래스 정보 생성 - 용병 초상용
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
// Desc: 클래스 정보 제거
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
// Desc: 클래스 정보 제거 - NPC 초상용
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free_NPC()
{
	clGrp.FreeXspr( m_SprNPC );
}

//-----------------------------------------------------------------------------
// Name: Free_Normal()												[private]
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 제거 - 일반 초상용
//-----------------------------------------------------------------------------
void	OnlinePortrait::Free_Normal()
{
	clGrp.FreeXspr( m_SprNormal );
}

//-----------------------------------------------------------------------------
// Name: Free_Mercenary												[private]
// Code: actdoll (2003-12-23)
// Desc: 클래스 정보 제거 - 용병 초상용
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
// Desc: 초상 그리기 - 마을의 각 가구에서 등장하는 판매 캐릭터
//		x, y		- 그릴 좌상단 좌표
//		siNPCID		- 해당 NPC유닛의 ID (NPC_KIND_... 인덱스를 사용하면 됨)
//		siNation	- 국가 코드 (ON_COUNTRY_... 인덱스를 사용하면 됨)
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNPC( SI32 x, SI32 y, SI32 siNPCID, SI32 siNation )
{
	// 해당 ID들이 정상인지 확인한다.
	SI32					siIndex			= ( siNation * 100 ) + siNPCID;
	MAP_PORTRAIT_NPC_ITER	iter			= m_PortraitNPCInfo.find( siIndex );
	ST_PORTRAIT_NPC_INFO	stPortNpcInfo	= iter->second;
	
	// 그림을 그려준다.
	clGrp.PutSpriteT( x, y, m_siNPCWidth, m_siNPCHeight, stPortNpcInfo.m_pSpr );
}

//-----------------------------------------------------------------------------
// Name: DrawAsNormal
// Code: actdoll (2003-12-31)
// Desc: 초상 그리기 - 전투, 시장 거래에서 사용될 얼굴들을 그린다.
//		x, y					- 그릴 좌상단 좌표
//		siID					- 해당 유닛의 DB 혹은 게임 ID
//		bIDTypeIsDB				- siID에 DB ID가 들어간 경우 true, 게임 ID가 들어간 경우 false
//		siDrawFlag				- DF_FLAG_PORTRAIT_DRAW_... 플래그를 조합하여 찍을 방법을 조절한다.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNormal( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siDrawFlag )
{
	SI32	siDBID;

	// 해당 ID들이 정상인지 확인한다.
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

	// 용병 그림 정보가 정상인지 체크
	ST_PORTRAIT_INFO	stPortInfo;
	MAP_PORTRAIT_ITER	iter		= m_PortraitInfo.find( siDBID );
	BOOL		bCanDrawPortrait	= false;
	if( iter != m_PortraitInfo.end() )	
	{
		bCanDrawPortrait	= true;
		stPortInfo			= iter->second;		
	}
	
	// 초상 그리기(이 옵션이 없다면 그려라)
	if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
	{
		if	( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )	// 반투명으로 그릴 경우
		{
			clGrp.PutSpriteT		( x, y, m_siNormalWidth, m_siNormalHeight, stPortInfo.m_pSprNormal, DarkTable[ 5 ] );	
		}
		else											// 보통으로 그릴 경우
		{
			clGrp.PutSpriteT		( x, y, m_siNormalWidth, m_siNormalHeight, stPortInfo.m_pSprNormal );
		}
	}

	// 아이콘 그리기 - 레벨업
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_LVUP );
	// 아이콘 그리기 - 레벨업에 거의 가까움
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_IMPEND );
	// 아이콘 그리기 - 중량초과
	if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 76, y + 24, DF_FLAG_PORTRAIT_ICON_OVERLOAD );
	
	// 하이라이트 그리기
	if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 3, y - 3, m_siNormalWidth + 6, m_siNormalHeight + 6 );
}

//-----------------------------------------------------------------------------
// Name: DrawAsNormalEx
// Code: actdoll (2003-12-31)
// Desc: 초상 그리기 - 초상의 확대 축소만 가능한 그리기
//		lpDDSDest				- 대상 서피스
//		x, y					- 그릴 좌상단 좌표
//		siID					- 해당 유닛의 DB 혹은 게임 ID
//		bIDTypeIsDB				- siID에 DB ID가 들어간 경우 true, 게임 ID가 들어간 경우 false
//		siNewWidth, siNewHeight	- 로딩된 폭이 아닌 지정한 크기로 그려준다.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsNormalEx( LPDIRECTDRAWSURFACE7 lpDDSDest, SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siNewWidth, SI32 siNewHeight )
{
	SI32	siDBID;

	if( !lpDDSDest )			return;

	// 해당 ID들이 정상인지 확인한다.
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
	
	// 그려주기
	RECT	rcDest;
	( siNewWidth <= 0 )		?	( siNewWidth	= x + m_siNormalWidth )		: ( siNewWidth	= x + siNewWidth );
	( siNewHeight <= 0 )	?	( siNewHeight	= y + m_siNormalHeight )	: ( siNewHeight	= y + siNewHeight );
	SetRect( &rcDest, x, y, siNewWidth, siNewHeight );

	// actdoll (2004/01/07 16:33) : 
	//	약간 번거로운 방법이다.
	//	외부에서 서피스를 락킹하고 있는 경우에 초상을 블릿 명령으로 그냥 찍으려 하면 DDERR_SURFACEBUSY 에러가 계속 발생하게 된다.
	//	때문에 서피스가 락킹 중일 경우 넘어온 서피스에 대해 잠깐 해제를 하고 찍은 뒤 다시 잡도록 한다.
	//	락킹중인 서피스는 대부분 이쪽으로 넘겨주는 lpDDSDest일 경우가 높기 때문에 이놈을 잡고 해제를 하도록 한다.
	//	만약 lpDDSDest가 락킹 서피스가 아니라면 안찍힐것이다. 
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
// Desc: 초상 그리기 - 용병창 초상 전용
//		x, y		- 그릴 좌상단 좌표
//		siID		- 해당 NPC유닛의 DB 혹은 게임 ID
//		bIDTypeIsDB	- siID에 DB ID가 들어간 경우 true, 게임 ID가 들어간 경우 false
//		siDrawFlag	- DF_FLAG_PORTRAIT_DRAW_... 플래그를 조합하여 찍을 방법을 조절한다.
//		uiHPBase	- HP 바를 그릴 때의 기본 수치, 0이면 그려주지 않는다.
//		uiHPNow		- HP 바를 그릴 때의 현재 수치, 0이면 그려주지 않는다.
//		uiMPBase	- MP 바를 그릴 때의 기본 수치, 0이면 그려주지 않는다.
//		uiMPNow		- MP 바를 그릴 때의 현재 수치, 0이면 그려주지 않는다.
//-----------------------------------------------------------------------------
void	OnlinePortrait::DrawAsMercenary( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB, SI32 siDrawFlag, 
										UI32 uiHPBase, UI32 uiHPNow, UI32 uiMPBase, UI32 uiMPNow )
{
	SI32	siDBID;

	// 해당 ID들이 정상인지 확인한다.
	if( !bIDTypeIsDB )
	{
		siDBID	= TransKindImjinToOnline( siID, false );
	}
	else
	{
		siDBID	= siID;
	}

	// 용병 그림 정보가 정상인지 체크
	ST_PORTRAIT_INFO	stPortInfo;
	MAP_PORTRAIT_ITER	iter		= m_PortraitInfo.find( siDBID );
	BOOL		bCanDrawPortrait	= false;
	if( iter != m_PortraitInfo.end() )	
	{
		bCanDrawPortrait	= true;
		stPortInfo			= iter->second;		
	}
	
	
	// 확대형으로 찍을 경우
	if( siDrawFlag & DF_FLAG_PORTRAIT_LARGE )
	{
		// 배경
		if( siDrawFlag & DF_FLAG_PORTRAIT_BG )					clGrp.PutSpriteLightCT	( x, y, m_SprMercenBGLarge.Header.Xsize,	m_SprMercenBGLarge.Header.Ysize,	m_SprMercenBGLarge.Image );

		// 초상 그리기(이 옵션이 없다면 그려라)
		if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
		{
			// 초상 그림 반투명으로 그릴 경우
	//		if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteLightCT	( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight,	stPortInfo.m_pSprMercenLarge );
			if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight, stPortInfo.m_pSprMercenLarge, DarkTable[ 5 ] );	
			// 초상 그림 보통으로 그릴 경우
			else												clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenLargeWidth,	m_siMercenLargeHeight,	stPortInfo.m_pSprMercenLarge );
		}

		// 게이지 그리기(죽었을 때는 hp/mp를 표시하지 않는다.)
		// HP	// MP
		if( uiHPBase > 0 && uiHPNow > 0 )				
		{
			Draw_StatusBar( x + 2, y + 35, m_SprMercenBGLarge.Header.Xsize - 4, 4, (R32)uiHPNow / (R32)uiHPBase, true, true );	
			// 만약 MP 캐릭터라면 찍어준다.
			if( uiMPBase > 0 )				Draw_StatusBar( x + 2, y + 40, m_SprMercenBGLarge.Header.Xsize - 4, 2, (R32)uiMPNow / (R32)uiMPBase, false, true );
		}

		// 아이콘 그리기 - 중량초과
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 76, y + 24, DF_FLAG_PORTRAIT_ICON_OVERLOAD );
		// 아이콘 그리기 - 레벨업에 거의 가까움
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_IMPEND );
		// 아이콘 그리기 - 레벨업
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 71, y + 2, DF_FLAG_PORTRAIT_ICON_LVUP );

		// 하이라이트 그리기
		if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 1, y - 1, m_SprMercenBGLarge.Header.Xsize + 2, m_SprMercenBGLarge.Header.Ysize + 2 );
	}
	// 일반형으로 찍을 경우
	else
	{
		// 배경
		if( siDrawFlag & DF_FLAG_PORTRAIT_BG )					clGrp.PutSpriteLightCT	( x, y, m_SprMercenBG.Header.Xsize,			m_SprMercenBG.Header.Ysize,			m_SprMercenBG.Image );

		// 초상 그리기(이 옵션이 없다면 그려라)
		if( bCanDrawPortrait && siDrawFlag ^ DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT )
		{
			// 초상 그림 반투명으로 그릴 경우
	//		if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteLightCT	( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen );
			if( siDrawFlag & DF_FLAG_PORTRAIT_HALFALPHA )		clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen, DarkTable[ 5 ] );	
			// 초상 그림 보통으로 그릴 경우
			else												clGrp.PutSpriteT		( x + 2, y + 2, m_siMercenWidth,	m_siMercenHeight,	stPortInfo.m_pSprMercen );
		}

		// 게이지 그리기(죽었을 때는 hp/mp를 표시하지 않는다.)
		// HP	// MP
		if( uiHPBase > 0 && uiHPNow > 0 )				
		{
			Draw_StatusBar( x + 2, y + 29, m_SprMercenBG.Header.Xsize - 4, 4, (R32)uiHPNow / (R32)uiHPBase, true, true );	
			if( uiMPBase > 0 )				Draw_StatusBar( x + 2, y + 34, m_SprMercenBG.Header.Xsize - 4, 2, (R32)uiMPNow / (R32)uiMPBase, false, true );
		}

		// 아이콘 그리기 - 중량초과
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_OVERLOAD )	Draw_Icons( x + 64, y + 18,	DF_FLAG_PORTRAIT_ICON_OVERLOAD );
		// 아이콘 그리기 - 레벨업에 거의 가까움
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_IMPEND )		Draw_Icons( x + 59, y + 2,	DF_FLAG_PORTRAIT_ICON_IMPEND );
		// 아이콘 그리기 - 레벨업
		if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_LVUP )		Draw_Icons( x + 59, y + 2,	DF_FLAG_PORTRAIT_ICON_LVUP );

		// 하이라이트 그리기
		if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )		Draw_HighLight( x - 1, y - 1, m_SprMercenBG.Header.Xsize + 2, m_SprMercenBG.Header.Ysize + 2);
	}
}

//-----------------------------------------------------------------------------
// Name: Draw_Icons
// Code: actdoll (2003-12-23)
// Desc: 아이콘을 그려준다.
//		x, y			- 아이콘의 좌상단 좌표
//		siDrawFlag_Icon	- 아이콘의 종류(DF_FLAG_PORTRAIT_ICON_... 시리즈)
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_Icons( SI32 x, SI32 y, SI32 siDrawFlag_Icon )
{
	// 아이콘 그리기 - 아이템 초상형태일 때 반투명 표시용
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_DIEOVER )
	{
		clGrp.PutSpriteLightCT	( x, y,	m_SprIconDieOver.Header.Xsize,	m_SprIconDieOver.Header.Ysize,		m_SprIconDieOver.Image );
	}
	// 아이콘 그리기 - 레벨업
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_LVUP )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconLvUp.Header.Xsize,		m_SprIconLvUp.Header.Ysize,			m_SprIconLvUp.Image );
	}
	// 아이콘 그리기 - 레벨업에 거의 가까움
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_IMPEND )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconImpend.Header.Xsize,	m_SprIconImpend.Header.Ysize,		m_SprIconImpend.Image );
	}
	// 아이콘 그리기 - 중량초과
	if	( siDrawFlag_Icon & DF_FLAG_PORTRAIT_ICON_OVERLOAD )
	{
		clGrp.PutSpriteT		( x, y,	m_SprIconOverLoad.Header.Xsize,	m_SprIconOverLoad.Header.Ysize,		m_SprIconOverLoad.Image );
	}
}

//-----------------------------------------------------------------------------
// Name: Draw_HighLight
// Code: actdoll (2003-12-23)
// Desc: 하이라이트를 그려준다.
//		x, y			- 하이라이트 좌상단 좌표
//		width, height	- 하이라이트의 높이, 너비
//		color			- 하이라이트의 색깔 인덱스(기본값 0x27)
//		uiThickness		- 하이라이트의 픽셀 두께(기본값 2)
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_HighLight( SI32 x, SI32 y, SI32 width, SI32 height, UI08 color, UI32 uiThickness )
{
	if( width < 1 || height < 1 || uiThickness < 1 )		return;
	clGrp.Box( x, y, x + width - 1, y + height - 1, color, 2 );
}

//-----------------------------------------------------------------------------
// Name: Draw_HighLight
// Code: actdoll (2003-12-23)
// Desc: 하이라이트를 그려준다.
//		rcDest			- 하이라이트 구획
//		color			- 하이라이트의 색깔 인덱스(기본값 0x27)
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
// Desc: 스테이터스 바를 그려준다.
//		x, y			- 각 바의 좌상단 좌표
//		width, height	- 스테이터스의 높이, 너비
//		fRate			- 스테이터스 바의 비율
//		bIsHP			- 스테이터스 바가 HP라면 true, MP라면 false
//		bShowEmpty		- 비어있을 때도 창을 보여줄거라면 true, 아니라면 false
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_StatusBar( SI32 x, SI32 y, SI32 width, SI32 height, R32 fRate, BOOL bIsHP, BOOL bShowEmpty )
{
	if( width < 1 || height < 1 )			return;
	if( fRate <= 0.0f && !bShowEmpty )		return;	// 수치가 0이 되었고 비어있는 거 보이지 말라면 리턴
	if( fRate > 1.0f )		fRate = 1.0f;			// 비율 넘어갈때는 그냥 고정시켜라

	UI08	uiColor, uiShadowColor;
	if( bIsHP )		// HP타입
	{								// 명암 대									// 명암 중
		if		( fRate > 0.66f )	uiColor = 0x44, uiShadowColor = 0xe5;//uiColor = 0xe7, uiShadowColor = 0xe5;	
		else if	( fRate > 0.33f )	uiColor = 0x46, uiShadowColor = 0xe1;//uiColor = 0xe3, uiShadowColor = 0xe1;
		else						uiColor = 0x45, uiShadowColor = 0xe9;//uiColor = 0xeb, uiShadowColor = 0xe9;

		// 일단 체력 표시 해주고
		UI32	uiLength		= width * fRate;
		if( uiLength >= 1 )			clGrp.FillBox	( x,			y,		x + uiLength - 1,	y + height - 1,		uiColor );	// 체력 표시
	}
	else			// MP타입
	{
		uiColor = 0xb4, uiShadowColor = 0x92;
	//	if( fRate <= 0.0f )					return;
		UI32	uiLength		= width * fRate;
		// 일단 아래에 그림자 그리고 위에 실제를 그린다. 선이 얇게 보이므로 어쩔 수 없다.
		if( uiLength >= 1 )			clGrp.FillBox	( x,			y,		x + uiLength - 1,	y + height - 1,		uiColor );			// 색상 표시

	}
}

//-----------------------------------------------------------------------------
// Name: Draw_StatusBar
// Code: actdoll (2003-12-23)
// Desc: 스테이터스 바를 그려준다.
//		fRate			- 스테이터스의 범위
//		fRate			- 스테이터스 바의 비율
//		bIsHP			- 스테이터스 바가 HP라면 true, MP라면 false
//-----------------------------------------------------------------------------
void	OnlinePortrait::Draw_StatusBar( RECT &rcDest, R32 fRate, BOOL bIsHP, BOOL bShowEmpty )
{
	Draw_StatusBar( rcDest.left, rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, fRate, bIsHP );
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfNormal
// Code: actdoll (2003-12-23)
// Desc: NPC 초상의 크기를 받는다.
//		siWidth, siHeight	- 초상의 너비, 높이가 적재될 변수
//-----------------------------------------------------------------------------
void	OnlinePortrait::GetRangeOfNPC( SI32 &siWidth, SI32 &siHeight )
{
	siWidth		= m_SprNPC.Header.Xsize;
	siHeight	= m_SprNPC.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfNormal
// Code: actdoll (2003-12-23)
// Desc: 일반 초상의 크기를 받는다.
//		siWidth, siHeight	- 초상의 너비, 높이가 적재될 변수
//-----------------------------------------------------------------------------
void	OnlinePortrait::GetRangeOfNormal( SI32 &siWidth, SI32 &siHeight )
{
	siWidth		= m_SprNormal.Header.Xsize;
	siHeight	= m_SprNormal.Header.Ysize;
}

//-----------------------------------------------------------------------------
// Name: GetRangeOfMercenary
// Code: actdoll (2003-12-23)
// Desc: 용병 초상의 크기를 받는다.
//		siWidth, siHeight	- 초상의 너비, 높이가 적재될 변수
//		bGetAsWide			- 큰 초상의 값을 받을려면 true, 일반적 크기를 받을려면 false (기본값 false)
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
// Desc: 초상 리스트에 정보를 하나씩 적재한다.
//		적재되는 초상은 용병 초상과 일반 초상 두가지다.
//		siGameID	- 게임 필드에서 사용하는 캐릭터 ID
//		siNPCFontNo	- NPC 초상에 대하여 임의로 적재할 때 NPC 초상의 그림 폰트 번호, 디폴트는 -1이며
//					이 변수는 siGameID가 NPC값으로 들어올 때만 작동된다.
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_UnitInfo( SI32 siGameID )
{
	SI32					siDBID, siFontNoNormal, siFontNoMercenary;
	SI32					ClipLeft, ClipTop, ClipRight, ClipBottom;
	XSPR_HEADER				*pHeader;
	ST_PORTRAIT_INFO		stPortInfo = {0,};

	// 기본 정보 적재
	siDBID							= TransKindImjinToOnline( siGameID, false );					// DB ID 얻고
	// actdoll (2003/12/30 21:37) : DB ID가 음수로 왔다면 없는 캐릭이다.
	if( siDBID < 0 )	return;
	stPortInfo.m_siGameID			= siGameID;														// 필드 ID 얻고
	
	siFontNoNormal		= KI[siGameID].GetOnlinePortraitFont();		// 일반 초상에서의 폰트 번호
	siFontNoMercenary	= KI[siGameID].GetOnlineMercenaryFont();	// 용병 초상에서의 폰트 번호
	


	// 일반 초상 정보 적재
	pHeader						= &m_SprNormal.Header;											// 그림 데이터 헤더 정보 접수
	stPortInfo.m_pSprNormal		= &m_SprNormal.Image[pHeader->Start[siFontNoNormal]];			// 이미지 포인터 적재

	clGrp.CreateOffScreen	( stPortInfo.m_pDDSNormal, pHeader->Xsize, pHeader->Ysize, TRUE, FALSE );	// 서피스 생성
	clGrp.ClearSurface		( stPortInfo.m_pDDSNormal );												// 서피스 클린
		// 그림을 서피스에 복사
	clGrp.GetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );
	if(clGrp.LockSurface( stPortInfo.m_pDDSNormal ))
	{
		clGrp.SetClipArea	( 0, 0, pHeader->Xsize - 1,	pHeader->Ysize - 1 );					// 복사 구획 잡고
		clGrp.FillBox		( 0, 0, pHeader->Xsize - 1,	pHeader->Ysize - 1, TRANSCOLOR );		// 컬러키까지 복사하는 구문이 없기 때문에 배경을 컬러키로 한번 칠해준다.
		clGrp.PutSpriteT	( 0, 0, pHeader->Xsize,		pHeader->Ysize,							// 그림 복사
								&m_SprNormal.Image[pHeader->Start[siFontNoNormal]] );
		clGrp.UnlockSurface	( stPortInfo.m_pDDSNormal );
	}
	clGrp.SetClipArea		( ClipLeft, ClipTop, ClipRight, ClipBottom );



	if( siFontNoMercenary >= 0 )// 0이상이라면 용병 초상이 있다.
	{
		// 용병 초상 정보 적재
		pHeader							= &m_SprMercen.Header;											// 그림 데이터 헤더 정보 접수
		stPortInfo.m_pSprMercen			= &m_SprMercen.Image[pHeader->Start[siFontNoMercenary]];		// 이미지 포인터 받는다.
		
		// 큰 용병 초상 정보 적재
		pHeader							= &m_SprMercenLarge.Header;										// 그림 데이터 헤더 정보 접수
		stPortInfo.m_pSprMercenLarge	= &m_SprMercenLarge.Image[pHeader->Start[siFontNoMercenary]];		// 이미지 포인터 받는다.
	}
	// 데이터 리스트에 적재
	m_PortraitInfo[siDBID]	= stPortInfo;
}

//-----------------------------------------------------------------------------
// Name: Init_NPCInfo												[private]
// Code: actdoll (2003-12-23)
// Desc: NPC의 초상 리스트에 정보를 하나씩 적재한다.
//		siNation	- 해당 NPC의 국가번호
//		siNPCID		- 게임 필드에서 사용하는 NPC 캐릭터의 ID
//		siNPCFontNo	- 해당 NPC의 그림 폰트 번호
//-----------------------------------------------------------------------------
void	OnlinePortrait::Init_NPCInfo( SI32 siNation, SI32 siNPCID, SI32 siNPCFontNo )
{
	SI32					siIndex;
	XSPR_HEADER				*pHeader;
	ST_PORTRAIT_NPC_INFO	stPortNpcInfo = {0,};

	siIndex		= ( siNation * 100 ) + siNPCID;
	
	// 기본 정보 적재
	stPortNpcInfo.m_siUnitID	= siNPCID;
	stPortNpcInfo.m_siNation	= siNation;
	
	// NPC 초상 정보 적재
	pHeader						= &m_SprNPC.Header;									// 그림 데이터 헤더 정보 접수
	stPortNpcInfo.m_pSpr		= &m_SprNPC.Image[pHeader->Start[siNPCFontNo]];		// 이미지 포인터 받는다.
	
	// 데이터 리스트에 적재
	m_PortraitNPCInfo[siIndex]	= stPortNpcInfo;
}

//-----------------------------------------------------------------------------
// Name: CreateAllInfo												[private]
// Code: actdoll (2003-12-23)
// Desc: 초상들을 여기서 "하드코딩"으로 생성한다. 오, 신이여...T^T)
//-----------------------------------------------------------------------------
void	OnlinePortrait::CreateAllInfo()
{
	SI32	i, j, siFontNo;
	// 필드 캐릭터 정보 초기화
	for( i=KIND_MANAGER1; i<KIND_ON_MAX; i++)
	{
		Init_UnitInfo( i );
	}

	// 국가별로 회전
	for( i=ON_COUNTRY_KOREA; i<ON_COUNTRY_ALL; i++ )
	{
		// 상인별로 회전
		for( j=DF_VALUE_PORTRAIT_NPC_GOV; j<=DF_VALUE_PORTRAIT_NPC_MBARRACK; j++ )
		{
			// 폰트 번호를 연산해서 집어넣는다.
			siFontNo = ( i - 1 ) * 20 + j;
			Init_NPCInfo( i, j, siFontNo );
		}
	}
}
