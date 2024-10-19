//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - ĳ���� �ʻ� ��� Ŭ����
//	File Name		: OnlinePortrait.h
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

#ifndef __ONLINEPORTRAIT_H__
#define __ONLINEPORTRAIT_H__

#include <map>
#include <iostream>

using namespace std;

// �뺴â �׸� ���� �ɼ�
#define		DF_FLAG_PORTRAIT_LARGE					0x00000001		// �뺴 �׸� �� ū ������ ������� �׷���

#define		DF_FLAG_PORTRAIT_BG						0x00000002		// �뺴 �׸� �� ��� �׸��� ����
#define		DF_FLAG_PORTRAIT_DONOTDRAWPORTRAIT		0x00000004		// �뺴�� �׸��� ���ƶ�
#define		DF_FLAG_PORTRAIT_HALFALPHA				0x00000008		// �뺴 �׸� �� ���������� ����
#define		DF_FLAG_PORTRAIT_HIGHLIGHT				0x00000010		// �뺴 �׸� �� ���̶���Ʈ ����

#define		DF_FLAG_PORTRAIT_ICON_LVUP				0x00010000		// ������ �׸��� - ������
#define		DF_FLAG_PORTRAIT_ICON_IMPEND			0x00020000		// ������ �׸��� - ���� ���� ����Ʈ ����
#define		DF_FLAG_PORTRAIT_ICON_OVERLOAD			0x00040000		// ������ �׸��� - �뷮�ʰ�
#define		DF_FLAG_PORTRAIT_ICON_DIEOVER			0x00080000		// ������ �׸��� - ITEM�� �ʻ� ���� �� ������� ǥ�ÿ�

//-----------------------------------------------------------------------------
// Name: OnlinePortrait
// Code: actdoll (2003-12-23)
// Desc: �ʻ� Ŭ����
//-----------------------------------------------------------------------------
class OnlinePortrait
{
private	:
	//-----------------------------------------------------------------------------
	//	���� �׷�
	//-----------------------------------------------------------------------------

	// ���ֺ� �ʻ� ��Ʈ�ѷ�
	//		�� ����ü�� �� ����Ʈ�� �̿��� �ε����� �迭 ���·� ������ �Ǹ�, �ε����� ���� ���Ǵ� 
	//		DB�� ����� �ε����� �������� ó���Ѵ�. ��, �ش� �ʻ��� DB�� �ش���� �ʴ� ������
	//		(Ex: ���� ��)�� �ε����� ���⼭ ������ ������ ����ó���� ��� �Ѵ�.
	struct ST_PORTRAIT_INFO
	{
		SI32					m_siGameID;			// �� ĳ���Ϳ� ��ġ�Ǵ� �ʵ� ĳ������ ID

		LPDIRECTDRAWSURFACE7	m_pDDSNormal;		// ���ǽ� �ش� ������ - �Ϲ� �ʻ�
		UI08					*m_pSprNormal;		// ��������Ʈ �ش� ������ - �Ϲ� �ʻ�
		UI08					*m_pSprMercen;		// ��������Ʈ �ش� ������ - �뺴â ����
		UI08					*m_pSprMercenLarge;	// ��������Ʈ �ش� ������ - �뺴â ����
	};
	// ������ ����Ʈ
	typedef	map< int, ST_PORTRAIT_INFO >			MAP_PORTRAIT;		// �� ����( DB ID, �ش� ID�� �ʻ����� )
	typedef	map< int, ST_PORTRAIT_INFO >::iterator	MAP_PORTRAIT_ITER;	// �ݺ��� ����
	MAP_PORTRAIT		m_PortraitInfo;									// �ʻ� ���� ����Ʈ
	SI32				m_siNormalWidth,		m_siNormalHeight;		// �Ϲ� �ʻ� ��������Ʈ�� ���� �ʺ�
	SI32				m_siMercenWidth,		m_siMercenHeight;		// �Ϲ� �뺴 �ʻ� ��������Ʈ�� ���� �ʺ�
	SI32				m_siMercenLargeWidth,	m_siMercenLargeHeight;	// ���� �뺴 �ʻ� ��������Ʈ�� ���� �ʺ�

	// NPC �ʻ� ��Ʈ�ѷ�
	//		�� ����ü�� �� ����Ʈ�� �̿��� �ε����� �迭 ���·� ������ �Ǹ�, 
	//		(�����ڵ�*100 + NPC ������ȣ) ���� �ε����� ó���Ѵ�.
	struct ST_PORTRAIT_NPC_INFO
	{
		SI32	m_siUnitID;							// �� NPC ������ ���� ��ȣ
		SI32	m_siNation;							// �� NPC�� ���� ����
		UI08	*m_pSpr;							// ��������Ʈ �ش� ������ - NPC
	};
	// ������ ����Ʈ
	typedef	map< int, ST_PORTRAIT_NPC_INFO >			MAP_PORTRAIT_NPC;		// �� ����( �ε�����, �ش� NPC�� �ʻ����� )
	typedef	map< int, ST_PORTRAIT_NPC_INFO >::iterator	MAP_PORTRAIT_NPC_ITER;	// �ݺ��� ����
	MAP_PORTRAIT_NPC	m_PortraitNPCInfo;								// NPC �ʻ� ���� ����Ʈ
	SI32				m_siNPCWidth,			m_siNPCHeight;			// NPC �ʻ� ��������Ʈ�� ���� �ʺ�

	// ���� �׸� ������
	XSPR	m_SprNPC;																							// ��������Ʈ - NPC
	XSPR	m_SprNormal;																						// ��������Ʈ - �Ϲ�
	XSPR	m_SprMercenBG,		m_SprMercen,		m_SprMercenHP,		m_SprMercenMP,		m_SprMercenHL;		// ��������Ʈ - �뺴â ����(��, ��)
	XSPR	m_SprMercenBGLarge,	m_SprMercenLarge,	m_SprMercenHPLarge,	m_SprMercenMPLarge,	m_SprMercenHLLarge;	// ��������Ʈ - �뺴â ����(��, ��)
	XSPR	m_SprIconLvUp,		m_SprIconImpend,	m_SprIconOverLoad,	m_SprIconDieOver;						// ��������Ʈ - �뺴â ������ ǥ��
	LPDIRECTDRAWSURFACE7		m_pSurfaceSprNormal;															// �ҽ� ���ǽ� - ��������Ʈ - �Ϲ�
	SI32	m_pSurfaceSprNormalWidth,				m_pSurfaceSprNormalHeight;									// �ҽ� ���ǽ��� ���� �ʺ�

	//-----------------------------------------------------------------------------
	//	�Լ� �׷�
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
	//	���� ���·� �׸���
	//-----------------------------------------------------------------------------

	void	DrawAsNPC		( SI32 x, SI32 y, SI32 siID, SI32 siNation );
	void	DrawAsNormal	( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siDrawFlag = 0 );
	void	DrawAsNormalEx	( LPDIRECTDRAWSURFACE7 lpDDSDest, SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siNewWidth = 0, SI32 siNewHeight = 0 );
	void	DrawAsMercenary	( SI32 x, SI32 y, SI32 siID, BOOL bIDTypeIsDB = true, SI32 siDrawFlag = 0, 
								UI32 uiHPBase = 0, UI32 uiHPNow = 0, UI32 uiMPBase = 0, UI32 uiMPNow = 0 );

	//-----------------------------------------------------------------------------
	//	�߰� ���
	//-----------------------------------------------------------------------------

	void	Draw_Icons			( SI32 x, SI32 y, SI32 siDrawFlag_Icon );

	void	Draw_HighLight		( SI32 x, SI32 y, SI32 width, SI32 height, UI08 color = 0xAD, UI32 uiThickness = 2 );
	void	Draw_HighLight		( RECT &rcDest, UI08 color = 0xAD, UI32 uiThickness = 2 );

	void	Draw_StatusBar		( SI32 x, SI32 y, SI32 width, SI32 height, R32 fRate, BOOL bIsHP, BOOL bShowEmpty = false );
	void	Draw_StatusBar		( RECT &rcDest, R32 fRate, BOOL bIsHP, BOOL bShowEmpty = false );


	//-----------------------------------------------------------------------------
	//	����
	//-----------------------------------------------------------------------------

	void	GetRangeOfNPC		( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfNormal	( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfInventory	( SI32 &siWidth, SI32 &siHeight );
	void	GetRangeOfMercenary	( SI32 &siWidth, SI32 &siHeight, BOOL bGetAsWide = false );


private	:
	//-----------------------------------------------------------------------------
	//	���� �Լ� �׷�
	//-----------------------------------------------------------------------------
	void	Init_UnitInfo	( SI32 siGameID );
	void	Init_NPCInfo	( SI32 siNation, SI32 siNPCID, SI32 siNPCFontNo );
	void	CreateAllInfo	( void );
//	SI32	GetMercenaryFont( SI32 siDBID );
};
#endif//__ONLINEPORTRAIT_H__