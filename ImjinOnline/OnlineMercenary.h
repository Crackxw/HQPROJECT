//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �뺴 ��� Ŭ����
//	File Name		: OnlineMercenary.h
//	Birth Date		: 2003.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2003.
//	Comments		: 
//		�ʵ� �׷��� ��� �κ� �� �뺴�� ���� �ʻ� ����� ����ϴ� �κ��Դϴ�.
//
//===================================================================================================

#ifndef __ONLINE_MERCENARY_H__
#define	__ONLINE_MERCENARY_H__

//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: ��� �����
//-----------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <vector>

using namespace std;

class cltOnlineWorld;


//-----------------------------------------------------------------------------
// Name: OnlineMercenary
// Code: actdoll (2003-12-23)
// Desc: �뺴 Ŭ����
//-----------------------------------------------------------------------------
class OnlineMercenary
{
private:
	//-----------------------------------------------------------------------------
	//	���� �׷�
	//-----------------------------------------------------------------------------
	cltOnlineWorld		*m_pMyOnlineWorld;						// �Ѱ� ���� Ŭ���� �޾ƿ���
	OnlinePortrait		*m_pOnlinePortrait;						// �ʻ� Ŭ���� �޾ƿ���

	// �뺴â ���� ����
	//		�� ����ü�� �뺴â�� ����Ǵ� ���� �Ѱ��� ���� ������ ó���Ѵ�.
	struct ST_SLOT_INFO
	{
		RECT			m_Rect;									// �뺴 ������ ȭ��� ��ȹ

		SI32			m_siDBSlotNo;							// �� ���Կ� ����� �뺴�� DB�� ���� ��ȣ, �ش����� ���� ���� -1�� �ȴ�.
		SI32			m_siDBID;								// �� ���Կ� ����� �뺴�� DB ID, �ش����� ���� ���� -1�� �ȴ�.
		SI32			m_siSlotAreaAccessID;					// �� ������ �ʵ� ��� �Ұ��� ��Ͻ� �߻��ϴ� ID
	};
	ST_SLOT_INFO		m_stSlotInfo[ON_MAX_FOLLOWER_NUM];		// ���α׷��� ������ �뺴â ���� ����
	SI32				m_siEnableSlotMaxCount;					// ���� �������� �뺴 ����� ������ ������ �ִ� ����
	SI32				m_siCurrentSlotCount;					// �뺴�� �����ϴ� ������ ����

	SI32				m_siMouseOverSlot;						// ���콺�� ���� �ö�� �뺴â ���� ��ȣ
	SI32				m_siSelectedSlot;						// ���õ� �뺴â ���� ��ȣ
	SI32				m_siDisableFieldAreaID;					// ��ü �ʵ� ������ ��Ȱ��ȭ ������ ���� ID
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
	//	�ܺο��� ���ŵǴ� �Լ� �׷챺
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