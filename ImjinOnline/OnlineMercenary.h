//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - 용병 출력 클래스
//	File Name		: OnlineMercenary.h
//	Birth Date		: 2003.
//	Creator			: 정 진 욱
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		필드 그래픽 출력 부분 중 용병에 대한 초상 출력을 담당하는 부분입니다.
//
//===================================================================================================

#ifndef __ONLINE_MERCENARY_H__
#define	__ONLINE_MERCENARY_H__

//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: 헤더 선언부
//-----------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <vector>

using namespace std;

class cltOnlineWorld;


//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: 용병 클래스
//-----------------------------------------------------------------------------
class OnlineMercenary
{
private:
	//-----------------------------------------------------------------------------
	//	변수 그룹
	//-----------------------------------------------------------------------------
	cltOnlineWorld		*m_pMyOnlineWorld;						// 총괄 게임 클래스 받아오기
	OnlinePortrait		*m_pOnlinePortrait;						// 초상 클래스 받아오기

	// 용병창 슬롯 정보
	//		이 구조체는 용병창에 적재되는 슬롯 한개에 대한 정보를 처리한다.
	struct ST_SLOT_INFO
	{
		RECT			m_Rect;									// 용병 슬롯의 화면상 구획

		SI32			m_siDBSlotNo;							// 이 슬롯에 적재된 용병의 DB측 슬롯 번호, 해당하지 않을 때는 -1이 된다.
		SI32			m_siDBID;								// 이 슬롯에 적재된 용병의 DB ID, 해당하지 않을 때는 -1이 된다.
		SI32			m_siSlotAreaAccessID;					// 이 슬롯을 필드 사용 불가로 등록시 발생하는 ID
	};
	ST_SLOT_INFO		m_stSlotInfo[ON_MAX_FOLLOWER_NUM];		// 프로그램상 가능한 용병창 슬롯 갯수
	SI32				m_siEnableSlotMaxCount;					// 현재 레벨에서 용병 고용이 가능한 슬롯의 최대 갯수
	SI32				m_siCurrentSlotCount;					// 용병이 존재하는 슬롯의 갯수

	SI32				m_siMouseOverSlot;						// 마우스가 위에 올라온 용병창 슬롯 번호
	SI32				m_siSelectedSlot;						// 선택된 용병창 슬롯 번호
	SI32				m_siDisableFieldAreaID;					// 전체 필드 구역을 비활성화 시켰을 때의 ID
	SI32				m_siPrevSelectSlot;

	BOOL				m_bAction;
	BOOL				m_bMouseLPush;
	BOOL				m_bMouseRPush;
	BOOL				m_bLock;
	BOOL				m_bTradeMode;
	BOOL				m_bLoadSpr;
	BOOL				m_bHighLight;

private:
	void	Init				( void );
	void	Free				( void );

	void	Update_MousePosition( void );
	void	Update_MouseBtn		( void );

	void	Check_SlotInfo		( void );
	void	Check_SlotArea		( void );
	void	Disable_SlotArea	( void );
	void	Enable_SlotArea		( void );

	void	toggleAction		( void )									{ m_bAction = !m_bAction; }
	
	void	DrawSelectPortait	(int x, int y, int nKind);
	void	DrawBar2			(int x, int y, int nSlot);
	void	DrawIcon			(int x, int y, int nSlot);

public:
	OnlineMercenary				( cltOnlineWorld *pOW, OnlinePortrait *pOP ) : m_pMyOnlineWorld(pOW), m_pOnlinePortrait(pOP)	{ Init(); }
	~OnlineMercenary			( void )																						{ Free(); }


	//-----------------------------------------------------------------------------
	//	외부에서 갱신되는 함수 그룹군
	//-----------------------------------------------------------------------------
	void	Initialize			( void );
	void	OnButtonPush		( void );
	void	Action				( void );
	void	Draw				(LPDIRECTDRAWSURFACE7 pSurface);

	BOOL	IsAction			( void )									{ return m_bAction; }
	void	NoAction			(BOOL b)									{ m_bLock = b; }

	void	Set_SelectedSlot	( SI32 siSlotNo, BOOL bSetAsDBSlotNo = true );
	SI32	Get_SelectedSlot	( BOOL bGetAsDBSlotNo = true );

	SI32	GetMouseOverSlotNo	( BOOL bGetAsDBSlotNo = false );
	SI32	GetMouseOverSlotDBID( void );

	void	Lock				( void );
	void	UnLock				( void );
	void	Refresh_Slot		( void );

//	SI16	TransKindToFont		(SI32 CharID);
	void	SetTradeMode		(BOOL b)									{ m_bTradeMode = b; }
	XSPR	*GetFollowerSpr		( void );

	void	DrawPortait_NOUSE		(int x, int y, int nKind);
	void	DrawBar1_NOUSE			(int x, int y, int nSlot);	
	void	DrawHpMp_NOUSE			(int x, int y, float fHp, float fMp);

	SI32	Convert_SlotToDBSlot( SI32 siMercenarySlotNo );
	SI32	Convert_DBSlotToSlot( SI32 siDBSlotNo );
	BOOL	Is_ValidSlot		( SI32 siSlotNo = -1 );
	void	SetHighLight(BOOL bfalg)	{ m_bHighLight = bfalg; };

};

#endif//__ONLINE_MERCENARY_H__