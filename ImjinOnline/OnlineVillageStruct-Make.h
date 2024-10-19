/*********************************************************************************************************
	���ϸ� : OnlineVillageStruct-Make.h
	�ۼ��� : ������
	�ۼ��� : 2001.11.21
	������ : 2001.12.27
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
	
	
	SI16					m_siListNum;																				// ����Ʈ��
	SI16					m_siX, m_siY;
	RECT					m_rtList;
	BOOL					m_bMouseDown;
	BOOL					m_bActive;
	SI16					m_siScroll;
	POINT					m_pt;
	BOOL					m_bSuccess;
	SI16					m_siSelectList;																				// ������ ����Ʈ�� �����ϱ� ���� �ӽ÷� �����ϴ� ����
	BOOL					m_bMaster;																					// ���� �ΰ�?
	UI08					m_uiSlotNum;																				// ��� ���� ������ ���� ��

	RECT					m_rtButton[ON_MAKE_SHOW_LIST];
	RECT					m_rtScroll;
	SI16					m_siScrollPos;
	
public:
	SI32					m_siStatus;																					// ���� ����
	UI08					m_uiKind;
	MakeList				m_MakeList[ ON_MAKE_MAXLIST ];																// ����Ʈ	
	CHAR					m_szKind[4];																				// ���� �ǹ� �ΰ�?
	SI16					m_siSelect;																					// ����


public:
	OnlineVillageStructMake();
	~OnlineVillageStructMake();

	VOID				Init(cltOnlineWorld *pOnlineWorld);																// �ʱ�ȭ
	VOID				Free();																							// ����.
	VOID				Poll(LPDIRECTDRAWSURFACE7 pSurface);															// �׼�.
	VOID				SetList( SI32 siKind, SI16 x, SI16 y );															// ������ �����Ѵ�.	
	BOOL				IsActive();																						// ���� Ȱ�����ΰ�?
	SI16				GetListNum();
	BOOL				IsEnter();																						// ��� ���°�?
	SI16				GetSelectList()				{ return m_siSelectList; };											// ������ ������ ���� �ش�.
	BOOL				GetMaster()					{ return m_bMaster;		 };											// ���� �ΰ�?
	UI08				GetSlotNum()				{ return m_uiSlotNum;	 };											// ��� ���� ������ ���� ���� ������ �ش�.
	UI08				GetPlantsKind();
	VOID				SetSlotNum( UI08 uiSlot )	{ m_uiSlotNum = uiSlot;	 };											// ���� ���� �����Ѵ�.
	MakeList			*GetMakeList();																					// 

	// robypark 2004/8/25 18:4
	// ������ ���� �ü� ��� ����Ʈ ���ڸ� �ݴ´�.
	VOID				CloseList_ProductionEstablishments(VOID);
};


#endif