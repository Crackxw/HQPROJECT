/********************************************************************************************
	���ϸ�	:	OnlineFollower.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2002.03.30
********************************************************************************************/

#ifndef __ONLINE_FOLLOWER_H
#define __ONLINE_FOLLOWER_H

class OnlineFollowerInfo
{
private:
	cltOnlineWorld				*m_pMyOnlineWorld;	
	_Button						m_BChar[ ON_MAX_FOLLOWER_NUM ];

	XSPR						*m_pButtonSpr, m_LvSpr01, m_LvSpr02, m_LifeSpr, m_ExpSpr, m_CahrSpr,
								m_CharDieSpr, m_FollowerInfo;
	XSPR						m_CharacterSlotSelect,m_CharacterSlotX, m_WeightOverSpr;
	BOOL						m_bAction, m_bNoAction;
	BOOL						m_bMouseDown, m_bMouseRight;
	UI08						m_uiSelectSlot;
	SI16						m_siNoField, m_siNoFieldAll, m_siFont;
	RECT						m_rt;
	SI16						m_siX, m_siY;
	DWORD						m_dwDelay, m_dwAniFrame;
	UI08						m_uiImpend;
	SI16						m_index[ON_MAX_FOLLOWER_NUM];
	SI08						m_CurFollowerNum;
	BOOL						m_bScrollRight;
	BOOL						m_bScrollLeft;
	SI16						m_siScrollX;
	SI16						m_siMaxFollowerNum;
	BOOL						m_bNoMove;
	BOOL						m_bOpenNow;



public:
	OnlineFollowerInfo();
	~OnlineFollowerInfo();

	VOID						Init(cltOnlineWorld *pOnlineWorld);										// �ʱ�ȭ
	VOID						Initialize();
	VOID						Action();																// �׼�
	VOID						Draw(LPDIRECTDRAWSURFACE7 pSurface);									// ��ο�
	VOID						Free();																	// ����
	BOOL						IsAction()					{	return m_bAction;	};					// ���� Ȱ��ȭ ���γ�?
	VOID						SetAction( BOOL bflag, BOOL bNomove = TRUE);							// Ȱ��ȭ & �� Ȱ��ȭ
	SI16						CheckSlot();															// ���콺�� ���� ��� �������� �ִ°�?
	BOOL						CheckRect(POINT pt);

	VOID						Lock();
	VOID						UnLock();
	VOID						NoAction(BOOL bFlag)		{	m_bNoAction = bFlag; };
	inline  VOID				SetCharacter( UI08 uiSlot );											// ĳ���͸� �����Ѵ�.
	inline	VOID				SetImpend();															// ������ �ӹ��� ĳ���� ã�´�.( �ܺ� ȣ�� )
	SI16						TransKindToFont(SI32 CharID);											// �뺴â�� ĳ���� �̹����� ã������ ĳ���� ������ �����ش�.
	VOID						SetNoMove(BOOL bFlag) { m_bNoMove = bFlag; };
	BOOL						IsNoMove()	{ return m_bNoMove; };
	XSPR						*GetFollowerSpr(BOOL bDie);												// ĳ���� �̹��� ����
	XSPR						*GetFollowerBarSpr(BOOL bLife);
	SI08						ValidSlot(UI08 uiSlot);




private:
	UI08						GetLvUpImpend();														// �������� �ӹ��� ĳ���͸� ã�´�.
	VOID						ScrollDrawRight(LPDIRECTDRAWSURFACE7 pSurface);
	VOID						ScrollDrawLeft(LPDIRECTDRAWSURFACE7 pSurface);
};


#endif

inline VOID OnlineFollowerInfo::SetImpend()
{
	m_uiImpend = GetLvUpImpend();
}

inline VOID OnlineFollowerInfo::SetCharacter( UI08 uiSlot )
{
	m_uiSelectSlot	=	uiSlot;
}
