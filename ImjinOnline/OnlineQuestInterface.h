//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ����Ʈâ
//	File Name		: OnlineQuestInterface.h
//	Birth Date		: 2004. 01. 01.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ����� �ֿܼ��� �ӹ� ���� ��� â�� ����ִ� �κ��Դϴ�.
//
//===================================================================================================

#pragma once 

//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: ���� ��� ���� �� �ܺ� Ŭ���� ����
//-----------------------------------------------------------------------------
#include "OnlineCommonStruct.h"
#include "OnlineBaseLB.h"

class cltOnlineWorld;				// ���� ���Ӻ� Ŭ���� ����� ���� ����


//-----------------------------------------------------------------------------
// Name: OnlineQuestInterface
// Code: actdoll (2004-01-01)
// Desc: Ŭ���� ����
//-----------------------------------------------------------------------------
class OnlineQuestInterface
{
private:
	cltOnlineWorld*		m_pMyOnlineWorld;		// ���� ���Ӻ� Ŭ������ ������, �ܺο��� �޾ƿ´�.

	_Button				m_btOK, m_btClose;		// ��ư UI, OK�� Close ��ư
	_Button				m_btUp, m_btDown;		// ��ư UI, Up, Down ��ũ�� ��ư
	OnlineSelectLB		m_LB_QuestList;			// ó�� ȭ�鿡 �ߴ� ����Ʈ ����� �����ֱ� ���� ����Ʈ �ڽ�

	XSPR				m_Spr_Frm_List;			// ��������Ʈ�� - ����Ʈ ���â ������
	XSPR				m_Spr_Frm_Info;			// ��������Ʈ�� - ����Ʈ ����â ������
	XSPR				m_Spr_Btn_ScrollUp;		// ��������Ʈ�� - ���� ��ũ�� ��ư
	XSPR				m_Spr_Btn_ScrollDown;	// ��������Ʈ�� - �Ʒ��� ��ũ�� ��ư
	XSPR				m_Spr_Bar_List;			// ��������Ʈ�� - ����Ʈ ���â �۴�� ��ũ��
	XSPR				m_Spr_Bar_Info;			// ��������Ʈ�� - ����Ʈ ����â �۴�� ��ũ��
	XSPR				m_Spr_Btn_ScrollHandle;	// ��������Ʈ�� - ��ũ�ѿ��� ���̴� �ڵ�
	XSPR				m_Spr_Btn_Common;		// ��������Ʈ�� - �Ϲ� ��ư

	BOOL				m_bAction;	
	BOOL				m_bMousePush;
	SI32				m_nNoField;

	SI32				m_siSelect;
	COLORREF			m_SelectColor;
	
	SI32				m_nQuestDataStatus;		// QuestData ����
	SPlayerQuest		m_QuestData;			// QuestData ����ü


public:
	OnlineQuestInterface( cltOnlineWorld *p ) : m_pMyOnlineWorld( p ) 	{ Init(); }
	~OnlineQuestInterface( void )										{ Free(); }


private:
	void	Init				( void );
	void	Free				( void );

	void	LoadImage			( void );
	void	FreeImage			( void );

	void	ToggleAction		( void )	{ m_bAction = !m_bAction; }
	BOOL	IsShowMainScroll	( void )	{ return ( GetCompletedQuestCount() + GetPlayingQuestCount() > 5 ? TRUE : FALSE ); }
//	void	SetQuestList		( void );

	void	Draw_QuestList		( LPDIRECTDRAWSURFACE7 pSurface );
	void	Draw_QuestInfo		( LPDIRECTDRAWSURFACE7 pSurface );

	void	Action_QuestList	( void );
	void	Action_QuestInfo	( void );

	void	(OnlineQuestInterface::*pfDraw)		( LPDIRECTDRAWSURFACE7 pSurface );
	void	(OnlineQuestInterface::*pfAction)	( void );


public:
	void	Action				( void );
	void	Draw				( LPDIRECTDRAWSURFACE7 pSurface );

	void	OnButtonPush				( void );
	BOOL	IsAction					( void )	{ return m_bAction; }

	SI16	GetCompletedQuestCount		( void );
	SI16	GetPlayingQuestCount		( void );
	SPlayerQuest*	const GetQuestData	( void )	{ return &m_QuestData; };
	void	SetQuestList		( void );	// robypark 2004/9/17 15:6 


//----------------------------------------------------------------------------
//	�������� ��Ŷ ���� �� �����ϴ� �Լ�
//----------------------------------------------------------------------------
public:
	BOOL	RecvMsg_SetQuestData		( SI16 PlayingQuestCount, SI16 CompletedQuestCount, SPlayingQuest* lpPlayingQuestData, SI16* lpCompletedQuestData );
	BOOL	RecvMsg_SetPlayingQuestData	( SPlayingQuest* lpPlayingQuestCodeData );
	void	RecvMsg_ClearAllQuest		( void );
	BOOL	RecvMsg_QuestStarted		( SI16 QuestCode );
	BOOL	RecvMsg_QuestFinished		( SI16 QuestCode );
	BOOL	RecvMsg_AddCompletedQuest	( SI16 QuestCode );
};
