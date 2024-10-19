/********************************************************************************************
	파일명	:	OnlineFollower.h
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2002.03.30
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

	VOID						Init(cltOnlineWorld *pOnlineWorld);										// 초기화
	VOID						Initialize();
	VOID						Action();																// 액숀
	VOID						Draw(LPDIRECTDRAWSURFACE7 pSurface);									// 드로우
	VOID						Free();																	// 날려
	BOOL						IsAction()					{	return m_bAction;	};					// 현재 활성화 중인냐?
	VOID						SetAction( BOOL bflag, BOOL bNomove = TRUE);							// 활성화 & 비 활성화
	SI16						CheckSlot();															// 마우스가 현재 어느 슬룻위에 있는가?
	BOOL						CheckRect(POINT pt);

	VOID						Lock();
	VOID						UnLock();
	VOID						NoAction(BOOL bFlag)		{	m_bNoAction = bFlag; };
	inline  VOID				SetCharacter( UI08 uiSlot );											// 캐릭터를 지정한다.
	inline	VOID				SetImpend();															// 레벨업 임박한 캐릭터 찾는다.( 외부 호출 )
	SI16						TransKindToFont(SI32 CharID);											// 용병창의 캐릭터 이미지를 찾기위한 캐릭터 종류를 구해준다.
	VOID						SetNoMove(BOOL bFlag) { m_bNoMove = bFlag; };
	BOOL						IsNoMove()	{ return m_bNoMove; };
	XSPR						*GetFollowerSpr(BOOL bDie);												// 캐릭터 이미지 리턴
	XSPR						*GetFollowerBarSpr(BOOL bLife);
	SI08						ValidSlot(UI08 uiSlot);




private:
	UI08						GetLvUpImpend();														// 레벨업이 임박한 캐릭터를 찾는다.
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
