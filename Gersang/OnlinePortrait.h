//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 캐릭터 초상 출력 클래스
//	File Name		: OnlinePortrait.h
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

#ifndef __ONLINEPORTRAIT_H__
#define __ONLINEPORTRAIT_H__

#include <map>
#include <iostream>

using namespace std;

// 용병창 그릴 때의 옵션
#define		DF_FLAG_PORTRAIT_LARGE					0x00000001		// 용병 그릴 때 큰 형태의 사이즈로 그려라

#define		DF_FLAG_PORTRAIT_BG						0x00000002		// 용병 그릴 때 배경 그림을 찍어라
#define		DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT		0x00000004		// 용병을 그리지 말아라
#define		DF_FLAG_PORTRAIT_HALFALPHA				0x00000008		// 용병 그릴 때 반투명으로 찍어라
#define		DF_FLAG_PORTRAIT_HIGHLIGHT				0x00000010		// 용병 그릴 때 하이라이트 찍어라

#define		DF_FLAG_PORTRAIT_ICON_LVUP				0x00010000		// 아이콘 그리기 - 레벨업
#define		DF_FLAG_PORTRAIT_ICON_IMPEND			0x00020000		// 아이콘 그리기 - 아직 돌릴 포인트 남음
#define		DF_FLAG_PORTRAIT_ICON_OVERLOAD			0x00040000		// 아이콘 그리기 - 용량초과
#define		DF_FLAG_PORTRAIT_ICON_DIEOVER			0x00080000		// 아이콘 그리기 - ITEM형 초상 찍을 때 죽은사람 표시용

//-----------------------------------------------------------------------------
// Name: OnlinePortrait
// Code: actdoll (2003-12-23)
// Desc: 초상 클래스
//-----------------------------------------------------------------------------
class OnlinePortrait
{
private	:
	//-----------------------------------------------------------------------------
	//	변수 그룹
	//-----------------------------------------------------------------------------

	// 유닛별 초상 컨트롤러
	//		이 구조체는 맵 리스트를 이용해 인덱스를 배열 형태로 가지게 되며, 인덱스에 대한 정의는 
	//		DB에 저장된 인덱스를 기준으로 처리한다. 단, 해당 초상이 DB에 해당되지 않는 데이터
	//		(Ex: 상인 등)은 인덱스를 여기서 지정한 놈으로 따로처리해 줘야 한다.
	struct ST_PORTRAIT_INFO
	{
		SI32					m_siGameID;			// 이 캐릭터와 매치되는 필드 캐릭터의 ID

		LPDIRECTDRAWSURFACE7	m_pDDSNormal;		// 서피스 해당 데이터 - 일반 초상
		UI08					*m_pSprNormal;		// 스프라이트 해당 데이터 - 일반 초상
		UI08					*m_pSprMercen;		// 스프라이트 해당 데이터 - 용병창 보통
		UI08					*m_pSprMercenLarge;	// 스프라이트 해당 데이터 - 용병창 선택
	};
	// 데이터 리스트
	typedef	map< int, ST_PORTRAIT_INFO >			MAP_PORTRAIT;		// 맵 정의( DB ID, 해당 ID의 초상정보 )
	typedef	map< int, ST_PORTRAIT_INFO >::iterator	MAP_PORTRAIT_ITER;	// 반복자 정의
	MAP_PORTRAIT		m_PortraitInfo;									// 초상 정보 리스트
	SI32				m_siNormalWidth,		m_siNormalHeight;		// 일반 초상 스프라이트의 폭과 너비
	SI32				m_siMercenWidth,		m_siMercenHeight;		// 일반 용병 초상 스프라이트의 폭과 너비
	SI32				m_siMercenLargeWidth,	m_siMercenLargeHeight;	// 대형 용병 초상 스프라이트의 폭과 너비

	// NPC 초상 컨트롤러
	//		이 구조체는 맵 리스트를 이용해 인덱스를 배열 형태로 가지게 되며, 
	//		(국가코드*100 + NPC 지정번호) 값을 인덱스로 처리한다.
	struct ST_PORTRAIT_NPC_INFO
	{
		SI32	m_siUnitID;							// 이 NPC 유닛의 지정 번호
		SI32	m_siNation;							// 이 NPC의 존재 국가
		UI08	*m_pSpr;							// 스프라이트 해당 데이터 - NPC
	};
	// 데이터 리스트
	typedef	map< int, ST_PORTRAIT_NPC_INFO >			MAP_PORTRAIT_NPC;		// 맵 정의( 인덱스값, 해당 NPC의 초상정보 )
	typedef	map< int, ST_PORTRAIT_NPC_INFO >::iterator	MAP_PORTRAIT_NPC_ITER;	// 반복자 정의
	MAP_PORTRAIT_NPC	m_PortraitNPCInfo;								// NPC 초상 정보 리스트
	SI32				m_siNPCWidth,			m_siNPCHeight;			// NPC 초상 스프라이트의 폭과 너비

	// 실제 그림 데이터
	XSPR	m_SprNPC;																							// 스프라이트 - NPC
	XSPR	m_SprNormal;																						// 스프라이트 - 일반
	XSPR	m_SprMercenBG,		m_SprMercen,		m_SprMercenHP,		m_SprMercenMP,		m_SprMercenHL;		// 스프라이트 - 용병창 보통(생, 사)
	XSPR	m_SprMercenBGLarge,	m_SprMercenLarge,	m_SprMercenHPLarge,	m_SprMercenMPLarge,	m_SprMercenHLLarge;	// 스프라이트 - 용병창 선택(생, 사)
	XSPR	m_SprIconLvUp,		m_SprIconImpend,	m_SprIconOverLoad,	m_SprIconDieOver;						// 스프라이트 - 용병창 아이템 표식
	LPDIRECTDRAWSURFACE7		m_pSurfaceSprNormal;															// 소스 서피스 - 스프라이트 - 일반
	SI32	m_pSurfaceSprNormalWidth,				m_pSurfaceSprNormalHeight;									// 소스 서피스의 폭과 너비

	//-----------------------------------------------------------------------------
	//	함수 그룹
	//-----------------------------------------------------------------------------

	void	Init			( void );
	void	Init_NPC		( void );
	void	Init_Normal		( void );
	void	Init_Mercenary	( void );

	void	Free			( void );
	void	Free_NPC		( void );
	void	Free_Normal		( void );
	void	Free_Mercenary	( void );

public	:
	OnlinePortrait			( void )			{ Init(); }
	~OnlinePortrait			( void )			{ Free(); }

	//-----------------------------------------------------------------------------
	//	정형 형태로 그리기
	//-----------------------------------------------------------------------------

	void	DrawAsNPC		( SI32 x, SI32 y, SI32 siID, SI32 siNation );
	void	DrawAsNormal	( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siDrawFlag = 0 );
	void	DrawAsNormalEx	( LPDIRECTDRAWSURFACE7 lpDDSDest, SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siNewWidth = 0, SI32 siNewHeight = 0 );
	void	DrawAsMercenary	( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siDrawFlag = 0, 
								UI32 uiHPBase = 0, UI32 uiHPNow = 0, UI32 uiMPBase = 0, UI32 uiMPNow = 0 );

	//-----------------------------------------------------------------------------
	//	추가 찍기
	//-----------------------------------------------------------------------------

	void	Draw_Icons			( SI32 x, SI32 y, SI32 siDrawFlag_Icon );

	void	Draw_HighLight		( SI32 x, SI32 y, SI32 width, SI32 height, UI08 color = 0xAD, UI32 uiThickness = 2 );
	void	Draw_HighLight		( RECT &rcDest, UI08 color = 0xAD, UI32 uiThickness = 2 );

	void	Draw_StatusBar		( SI32 x, SI32 y, SI32 width, SI32 height, R32 fRate, BOOL bIsHP, BOOL bShowEmpty = false );
	void	Draw_StatusBar		( RECT &rcDest, R32 fRate, BOOL bIsHP, BOOL bShowEmpty = false );


	//-----------------------------------------------------------------------------
	//	정보
	//-----------------------------------------------------------------------------

	void	GetRangeOfNPC		( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfNormal	( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfInventory	( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfMercenary	( SI32 &siWidth, SI32 &siHeight, BOOL bGetAsWide = false );


private	:
	//-----------------------------------------------------------------------------
	//	도구 함수 그룹
	//-----------------------------------------------------------------------------
	void	Init_UnitInfo	( SI32 siGameID );
	void	Init_NPCInfo	( SI32 siNation, SI32 siNPCID, SI32 siNPCFontNo );
	void	CreateAllInfo	( void );
//	SI32	GetMercenaryFont( SI32 siDBID );
};
#endif//__ONLINEPORTRAIT_H__