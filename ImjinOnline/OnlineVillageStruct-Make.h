/*********************************************************************************************************
	파일명 : OnlineVillageStruct-Make.h
	작성자 : 정진욱
	작성일 : 2001.11.21
	수정일 : 2001.12.27
*********************************************************************************************************/
#ifndef __VILLAGE_MAKE_H
#define __VILLAGE_MAKE_H


#define ON_MAKE_MAXLIST						ON_MAX_INDUSTRIAL_COMPLEX_NUM
#define ON_MAKE_SHOW_LIST					20


#define ON_MAKE_INIT						0
#define ON_MAKE_POLL						1
#define ON_MAKE_BUY							2
#define ON_MAKE_BUY_WAIT					3
#define ON_MAKE_RENAME_INIT					4
#define ON_MAKE_RENAME_POLL					5



struct	MakeList
{
	char		m_szFarmName[ON_MAKE_MAXLIST];
	char		m_szID[ON_ID_LENGTH+1];
	UI08		m_uiSlotNum;
	MONEY       m_moMaxPay;
	BOOL		m_bSelect;
	BOOL		m_bOver;
	SI32		m_siWork;
};
 

class	OnlineVillageStructMake
{
private:
	cltOnlineWorld			*m_pMyOnlineWorld;
	XSPR					m_ButtonSpr, m_ListBorderSpr, *m_pScrollBar, m_IsBuySpr,m_ListBorderLineSpr;
	XSPR					m_IsBuyLineSpr;

	_Button					m_BBuy, m_BEnter, m_BRename, m_BExit;
	_Button					m_BIsBuy, m_BBack;
	
	
	SI16					m_siListNum;																				// 리스트수
	SI16					m_siX, m_siY;
	RECT					m_rtList;
	BOOL					m_bMouseDown;
	BOOL					m_bActive;
	SI16					m_siScroll;
	POINT					m_pt;
	BOOL					m_bSuccess;
	SI16					m_siSelectList;																				// 선택한 리스트를 리턴하기 위기 임시로 저장하는 변수
	BOOL					m_bMaster;																					// 주인 인가?
	UI08					m_uiSlotNum;																				// 들어 가는 농장의 슬롯 수

	RECT					m_rtButton[ON_MAKE_SHOW_LIST];
	RECT					m_rtScroll;
	SI16					m_siScrollPos;
	
public:
	SI32					m_siStatus;																					// 상태 변수
	UI08					m_uiKind;
	MakeList				m_MakeList[ ON_MAKE_MAXLIST ];																// 리스트	
	CHAR					m_szKind[4];																				// 무슨 건물 인가?
	SI16					m_siSelect;																					// 선택


public:
	OnlineVillageStructMake();
	~OnlineVillageStructMake();

	VOID				Init(cltOnlineWorld *pOnlineWorld);																// 초기화
	VOID				Free();																							// 해제.
	VOID				Poll(LPDIRECTDRAWSURFACE7 pSurface);															// 액션.
	VOID				SetList( SI32 siKind, SI16 x, SI16 y );															// 리스를 설정한다.	
	BOOL				IsActive();																						// 현재 활동중인가?
	SI16				GetListNum();
	BOOL				IsEnter();																						// 들어 가는가?
	SI16				GetSelectList()				{ return m_siSelectList; };											// 선택한 리스를 보여 준다.
	BOOL				GetMaster()					{ return m_bMaster;		 };											// 주인 인가?
	UI08				GetSlotNum()				{ return m_uiSlotNum;	 };											// 들어 가는 농장의 슬롯 수를 리턴해 준다.
	UI08				GetPlantsKind();
	VOID				SetSlotNum( UI08 uiSlot )	{ m_uiSlotNum = uiSlot;	 };											// 슬롯 수를 설정한다.
	MakeList			*GetMakeList();																					// 

	// robypark 2004/8/25 18:4
	// 열려진 생산 시설 목록 리스트 상자를 닫는다.
	VOID				CloseList_ProductionEstablishments(VOID);
};


#endif