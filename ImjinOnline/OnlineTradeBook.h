/********************************************************************************************
	���ϸ�	:	OnlineTradeBook.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.11.08
	������  :	2001.11.26
********************************************************************************************/


#ifndef	__TRADEBOOK_H
#define	__TRADEBOOK_H


//---------------------------------------------------------
// ���� 
#define ON_BOOK_TOTAL_MONEY				0	// �������
#define ON_BOOK_FRIEND					1	// ģ�����
#define ON_BOOK_STATUS					3
//#define ON_BOOK_PROPERTY				4
#define ON_BOOK_QUEST					5
#define ON_BOOK_INVESTMENT				6	//���ڱ�
#define	ON_BOOK_GUILD					7	// ���
#define ON_BOOK_PARTY					8	// ��Ƽ
#define ON_BOOK_SIEGEWARFARE			9	// ����� ������ �������̽�


//���ڱ� ����.
#define ON_BOOK_INVESTMENT_INIT					0
#define ON_BOOK_INVESTMENT_LIST					1
#define ON_BOOK_INVESTMENT_SELECT				2
#define ON_BOOK_INVESTMENT_POLL					3
#define ON_BOOK_INVESTMENT_EXIT					4

// ���� �����
#define ON_BOOK_TOTAL_MONEY_INIT		0
#define ON_BOOK_TOTAL_MONEY_POLL		1
#define ON_BOOK_TOTAL_MONEY_EXIT		2
#define ON_BOOK_TOTAL_MONEY_RESET		3
#define ON_BOOK_TOTAL_MONEY_SELECT		4


struct SAVEFILE_HEADER;
struct SAVEFILE_SEARCH;
struct SAVEFILE_SORT;
struct OnAccountNumber;

#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include <vector>
#include <list>
using namespace std;


#define ON_BOOK_MAX_ACCOUNTLIST		20


struct TOTALITEM
{
	UI16							m_uiItemID;
	MONEY							m_siItemNum;
	UI16							m_uiItemCount;
};


struct TOTALMONEY																						// TAB 1
{	
	TOTALITEM						m_Item[ON_MYDATA_MAX_ITEM_NUM * ON_MAX_FOLLOWER_NUM];
	OnAuditOwnInfo					*m_pAuditOwnInfo;
	SI16							m_siItemNum;
	SI16							m_MyMoneyStatus;													// ����꿡���� ���� ����
	UI32							m_uiTotalUser;														// ��ü ��� ����ڼ�
	UI32							m_uiRanking;														// �ڽ��� ��ŷ
	MONEY							m_uiProperty;														// �� �ڻ�.	
	MONEY							m_uiBankMoney;														// ���� �ܰ�
	MONEY							m_siLoanMoney;														// ���� ����
	BOOL							m_bFlag;															// ������ ���� ����Ÿ�� �ѹ��� �޴´�.
	SI16							m_siX,m_siY;
};

// ������ ��� �������̽� ������ ���� ��� ���� ���� ����ü
struct SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE
{
	// ������ ����
	OnGuildResult		*m_pOccupiedVillageList;					// �� ����� ������ ���� ����Ʈ �� ���� %
	SI16				m_siOccupiedVillageCount;					// �� ����� ������ ���� ����

	SI16				m_siScroll;									// ��ũ��
	RECT				m_rtScrollBar;								// ��ũ�� �簢����
	SI16				m_siScrollPos;	
	SI16				m_siSelectedItemIdx;

	// ��ũ�� �� ��ư
	_Button				m_btnScrollUp;
	_Button				m_btnScrollDown;

	// ��ũ�� �� ��ũ�� �̹���
	XSPR				m_sprScrollBar;

	SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE()
	{
		m_pOccupiedVillageList = NULL;
		m_siOccupiedVillageCount = 0;

		m_siScroll = 0;												// ��ũ��
		SetRect(&m_rtScrollBar, 112, 262, 112 + 9, 262 + 90);		// ��ũ�� �簢����
		m_siScrollPos = 0;	
	}
};

// ������ ��� �������̽� ���� ����ü
struct SBOOK_SIEGEWARFARE														// TAB 2
{	
	SI16				m_siStatus;									// ���� ����
	BOOL				m_bFlag;									// ������ ���� ����Ÿ�� �ѹ��� �޴´�.
	SI16				m_siX,m_siY;

	// ���� ���� ����Ʈ
	SI32				m_siGuildCount[ON_MAX_GUILD_UNIT];			// ��� ���� �����ϴ� ���� ���� ��. ���� ����
	BOOL				m_bDownloadedGuildUnitCount;

	MONEY				m_uiGuildSupplyGoodsAmount;					// ��� ���� ����ǰ ����
	BOOL				m_bDownloadedGuildSupplyGoodsAmount;

	// ������ ���� ���
	SBOOK_SIEGEWARFARE_OCCUPIED_VILLAGE		m_sBook_Siegewarfare_Occupied_Village;
	BOOL				m_bDownloadedOccuiedVillage;

	// ��ܿ� ��� ���� ������
	SI16				m_siCurPage, m_siMaxPage;

	// ��ܿ� ����Ʈ
	strGuildMemberInfo	*m_pMemberList;
	UI16				m_uiMemberNum;
	SI16				m_siSelectedGuildMemberIdx;
	BOOL				m_bDownloadedGuildMember;

	RECT				m_rtOccupiedVillageListBox;		// ���ɵ� ���� ����Ʈ �ڽ� ũ��
	RECT				m_rtGuildMemberListBox;			// ��ܿ� ��� ����Ʈ �ڽ� ũ��

	XSPR				m_sprButtonRecruitArmy;

	// �����Ϸ��� ������ ���� ����
	SI32				m_siRecruitArmyKind;		// �����Ϸ��� ĳ������ Imperium.txt���� �о���� ������ ���� �ε���
	SI32				m_siNoFieldHandle_Calc;		// ���� �������� No Field Area
	// ���� ���� ��ư
	_Button				m_btnRecruitArmy_SwordMan;		// ���� �˺�
	_Button				m_btnRecruitArmy_SpearMan;		// ���� â��
	_Button				m_btnRecruitArmy_ArcherMan;		// ���� �ú�
	_Button				m_btnRecruitArmy_HorseMan;		// ���� �⸶��
	_Button				m_btnRecruitArmy_SiegeTower;	// ���� ����
	_Button				m_btnRecruitArmy_Catapult;		// ���� �߼���
	_Button				m_btnRecruitArmy_SupplyWagon;	// ���� ���� ����

	_Button				m_btnGiveACompetency;			// ���� �ֱ� ��ư

	SBOOK_SIEGEWARFARE()
	{
		m_siStatus = m_bFlag = m_siX = m_siY = m_uiGuildSupplyGoodsAmount = m_siCurPage = m_siMaxPage = m_uiMemberNum = 0;

		m_bDownloadedGuildMember = m_bDownloadedGuildSupplyGoodsAmount = m_bDownloadedGuildUnitCount = m_bDownloadedOccuiedVillage = FALSE;

		ZeroMemory(m_siGuildCount, sizeof(SI32) * ON_MAX_GUILD_UNIT);

		m_pMemberList = NULL;

		SetRect(&m_rtOccupiedVillageListBox, 14, 256, 14 + 95, 256 + 105);
		SetRect(&m_rtGuildMemberListBox, 123, 256, 123 + 95, 256 + 105);
	}
};

struct GUILD						
{
	SI16							m_siStatus;
	SI16							m_siX,m_siY;
};

struct PARTY
{
	SI16							m_siStatus;
	SI16							m_siX,m_siY;
};
/*
struct PROPERTY																							// TAB 3
{ 
	OnAuditOwnInfo					*m_pAuditOwnInfo;
	SI16							m_PropertyStatus;													// �ε����� ���� ����
	SI16							m_siOwnNum;															// �ε��� ��
	SI16							m_siOverMouse;
	RECT							*m_pRect;
	BOOL							m_bFlag;															// ������ ���� ����Ÿ�� �ѹ��� �޴���.
};
*/

struct INVESTMENT
{
	OnAuditVillageInfo*				m_pAuditVillageInfo;
	SI16							m_InvestmentStatus;
	SI16							m_InvestmentCount;
	SI16							m_InvestmentUserCount;
	SI16							m_siOverMouse;
	RECT*							m_pRect;
	SI16							m_siScroll;
	RECT*							m_pListRect;
	RECT							m_rtScrollBar;
	SI16							m_siScrollPos;
	SI16							m_siTempScroll;
	MONEY							m_uMoney;
};

struct VILLAGEINFO																						// TAB 4
{
	SAVEFILE_HEADER					*m_pHeader;															// ����ü ���
	SAVEFILE_SEARCH					*m_pSearchHeader;	
	SAVEFILE_SORT					*m_pSort;	
	RECT							*m_pRect, *m_pSearchRect;											// ���� �̸� (��ư��)	
	SI16							m_VillageStatus;													// ���� ������ ���� ����
	SI16							m_VillageTotal;														// ���� ������ ����	
	SI16							m_siScroll;															// ��ũ��
	RECT							m_rtScrollBar;														// ��ũ�� �簢����
	SI16							m_siScrollPos;	
	SI16							m_siSelect;															// ������ ������ �ε���
	RECT							*m_prtItem;															// ������ ����â
	SI16							m_siSearchNum;														// �� �˻��� ��	
	UI32							m_uiSearchID;														// ���� �˻� ���� �������� ���̵�
	SI16							m_siVillageOver;
	SI16							m_siItemOver;														// ���콺�� ��ư�� ���� ������?
	SI16							m_siSelectOver, m_siSelectSearch;
};

struct FRIENDLIST																						// TAB 1
{
	SI16							m_FriendStatus;														// ģ������� ���� ����
	BOOL							m_bFlag;															// ������ ���� ����Ÿ�� �ѹ��� �޴���.
	_Button							m_BAdd, m_BRefusal, m_BRefusalCancel, m_BDelete;//, m_BMessenger;		// �߰�, ���Űź�, �ź����, ģ������, 1:1��ȭ
//	_Button							m_BCheck[MAX_FRIEND_NUM]; //m_BCheckAll;								// ģ������ üũ��ư 
//	BOOL							m_bIsCheckAll;
	BOOL							m_bCheck[MAX_FRIEND_NUM];
//	OnlineEditControl				*m_pFriendEditBox;
	HOEC							m_hOecFriend;
	
	CHAR							m_szFriendName[ON_ID_LENGTH+1];										// ����Ʈ �ڽ��� �ؽ�Ʈ
	RECT							*m_pRect;															// ����Ʈ �ڽ� ����
	CHAR							m_szPath[256];														// ģ����� ���
	XSPR							m_LoginSpr;
	SI16							m_siX,m_siY;
};


class OnlineCharStatus;
 

class OnlineTradeBook
{
public:
	OnlineCharStatus				*m_pOnlineCharStatus;
	FRIENDLIST						*m_pFriendList;														// tab1�� ����ü( ģ�����)

private:
	cltOnlineWorld					*m_pMyOnlineWorld;
	
	///////////////////////////////////////////////////////////////////////////////
	XSPR							m_BookMyMoneySpr;
	XSPR							m_BookSiegeWarfare;		// ������ ��� �������̽� ���� �̹���
	XSPR							m_MoneySmallScrollBaseSpr,m_MoneyLargeScrollBaseSpr;
	XSPR							m_BookScrollBarSpr,m_BookScrollBttnUpSpr,m_BookScrollBttnDnSpr;
	XSPR							m_BookExit;
	XSPR							m_BtnPartyBaseSpr,m_NameBaseSpr;
	XSPR							m_BookFriendSpr,m_BookGuildSpr,m_BookPartySpr;
	XSPR							m_BookTab;
	XSPR							m_FirendRefusalSpr;
	XSPR							m_GuildSmallScrollBaseSpr,m_GuildLargeScrollBaseSpr;

	XSPR							m_FriendBttnSpr;

	XSPR							m_BttnFriendMenu;

	XSPR							m_sprTown_button_medium;

	RECT							m_rtMyMoney;
	RECT							m_rtVillageScroll,m_rtInvestmentScroll;

	_Button							m_BttnMoneyScrollBar,m_BttnMoneyScrollUp,m_BttnMoneyScrollDn;
	_Button							m_BttnInvestScrollBar,m_BttnInvestScrollUp,m_BttnInvestScrollDn;
	_Button							m_BttnMoneyExit;
	_Button							m_BookFriendTab,m_BookGuildTab,m_BookPartyTab;

	// robypark 2004/11/15 19:7
	_Button							m_btnBookESTATE;		// ������� ���� �� ��ư
	_Button							m_btnBookSiegeWarfare;	// ��� ������ ����, ���� ���� ���� ���� �� ��ư

	_Button							m_BttnAdd, m_BttnRefusalOk,m_BttnDelete,m_BttnMessenger;
	_Button							m_BttnRefusalCancel;
	

	_Button							m_BttnGuildScrollUp1,m_BttnGuildScrollUp2,m_BttnGuildScrollUp3;
	_Button							m_BttnGuildScrollDn1,m_BttnGuildScrollDn2,m_BttnGuildScrollDn3;
	_Button							m_BttnGuildBar1,m_BttnGuildBar2,m_BttnGuildBar3;

	POINT							m_ptMenu;
	///////////////////////////////////////////////////////////////////////////////
	
	XSPR							m_CheckSpr, m_QuestBSpr;

	
	SI16							m_siX, m_siY;														// ���� ��ǥ
	SI16							m_siXsize;
	SI16							m_siStatus;															// ���� ��� ����
	BOOL							m_bMouseDown;														// ���콺 ����
	BOOL							m_bActive;				
	BOOL							m_bScroll;
	POINT							m_ptMouse;															// ���콺 ��ǥ
	RECT							m_rtBook;	
//	SI16							m_siPageStatus;														// ��� ���° �������ΰ�


	TOTALMONEY						m_Total;															// Tab1�� ����ü( �ڻ� )
//	PROPERTY						m_Property;															// Tab3�� ����ü( �ε��� ���� )
//	VILLAGEINFO						m_VillageInfo;														// Tab4�� ����ü( ���� ���� )
	SI16							m_QuestDataStatus;													// Quest Data ����
	SPlayerQuest					m_QuestData;														// Quest Data
	MONEY							m_QuestBankBookCount;												// Quest�� Data, ���� ���¼�
	INVESTMENT						m_Investment;
	GUILD							m_Guild;
	PARTY							m_Party;
	SBOOK_SIEGEWARFARE				m_BookDataSiegeWarfare;												// ��� ������ �������̽�


	SI16							m_siNoFieldHandle;													// �� �ʵ� ���� ����	
	RECT							m_rtScrollBar;														// ��ũ�ѹ��� �簢 ����
	SI16							m_siScrollPos;														// ��ũ�ѹ��� ��ġ
	SI16							m_siScroll;															// ��ũ��
	SI16							m_siSelect;															// ���� ������ ��ġ

	BOOL							m_bScrollRight;
	BOOL							m_bScrollLeft;
	BOOL							m_bScrollNow1;	
	UI32							m_siScrollX;

	OnlineMsgBox					*pMsgBox;

	SI16							m_siQuestScroll;													// Quest Scroll
//	OnAuditVillageInfo*				pOnAuditVillageInfo;
	OnInvestmentInfo*				m_pOnInvestmentInfo;
	strAuditGuildMemberInfo*		m_pOnGuildMemberInfo;



	SI32							m_siLoginGuildMember;
	SI32							m_siNowGuildMember;
	SI32							m_siSHNExceptMember;

	SI32							m_siDAICount;
	SI32							m_siDHSCount;
	SI32							m_siHNSCount;
	SI32							m_siHNDCount;
	
	strAuditGuildMemberInfo*		m_pOnGuildMemberDAI;
	strAuditGuildMemberInfo*		m_pOnGuildMemberDHS;
	strAuditGuildMemberInfo*		m_pOnGuildMemberHNS;
	strAuditGuildMemberInfo*		m_pOnGuildMemberHND;

	SI32							m_siGuildScroll1Count;
	SI32							m_siGuildScroll2Count;
	

public:
	OnlineTradeBook();
	~OnlineTradeBook();

	VOID							Init( cltOnlineWorld *pMyOnlineWorld );								// �ʱ�ȭ
	VOID							Initialize();														// �α������� ���ư����� �ʱ�ȭ
	VOID							Poll();																// ���� ��
	VOID							Draw( LPDIRECTDRAWSURFACE7 pSurface );								// ���
	VOID							Free();																// ����

	BOOL							IsAction();															// ���� Ȱ��ȭ �ΰ�?
	VOID							SetAction( BOOL bFlag, SI32 siFlag = 0 );							// Ȱ��ȭ �Ѵ�.
	VOID							BonusUp( BOOL bFlag );												// ���ʽ��� �ø���.
	SI16							GetStatus();														// ���� ��� â�� ���� �ִ��� �˾� �´�.
	VOID							SetStatus( SI16 siFlag );											// ��� â�� �����ΰ� ����
	VOID							SetSelect( UI08 uiSlot );
	SI16							GetWidth() { return m_siXsize; };
	VOID							KillScroll();
	VOID							GetBookMoney(SI16 t) { m_Total.m_MyMoneyStatus = t ;};
	INVESTMENT*						GetInvestmentInfo()	{ return &m_Investment; };
	TOTALMONEY*						GetTotalMoney()		{ return &m_Total;};

	_Button							m_BPartyOutBasebttn;

	// ���� ���� ��ȭ���ڰ� Ȱ��ȭ �Ǿ����� �˻�
	BOOL							IsActivityBook(SI08 siBookKind);

	// ��ܿ� ��� ��û ���� ó��
	UI16							SetGuildMemberList(BYTE *pMsg);

	// robypark 2004/11/16 15:36
	// ������ ��� ���� ���� ����Ʈ ��û ����
	// ON_RESPONSE_LIST_GUILD_UNIT
	UI16							OnResponseSiegeWarfareUnitList(BYTE *pMsg);

	// ��ܿ� �ִ� ������ ���� �δ븦 ������ ���������� ��ȯ�� ���
	UI16							OnDecreaseGuildUnit(BYTE* pMsg);

	// ������ ��� ����ǰ ���� ���
	// ON_RESPONSE_LIST_GUILD_SUPPLY
	UI16							OnResponseListGuildSupply(BYTE *pMsg);

	// robypark 2004/12/2 10:56
	// ������ ��� ������ ���� ����Ʈ �� ���� % ����
	// ON_RESPONSE_GUILD_VILLAGEATTACK_RESULT
	UI16							OnResponseGuildVillageattackResult(BYTE *pMsg);

	// robypark 2004/12/2 13:45
	// ������ ���� �Ŀ� ������ ���� ���� ������ Ư�� ĳ���Ϳ� �ֱ� ��û
	// ON_RESPONSE_GUILD_CHOICE_PLAYER
	UI16							OnResponseGuildChoicePlayer(BYTE *pMsg);

private:

	VOID							CheckButton();														// ��ư�� üũ�Ѵ�.	


	VOID							ActionMyMoney();													// ���� ����� �׼�~
	VOID							ActionFriend();														// ģ�����
	VOID							ActionGuild();
	VOID							ActionParty();
	VOID							ActionSiegeWarfare();	// ��� ������ ���� �������̽� ó��

	// robypark 2004/12/24 13:34
	VOID							UpdateButtonSiegeWarfare();	// ��� ������ ���� �������̽� ��� ��ư ���� ����

	VOID							DrawMyMoney( LPDIRECTDRAWSURFACE7 pSurface );						// ���� ����� ��ο�
	VOID							DrawFriend( LPDIRECTDRAWSURFACE7 pSurface );						// ģ����� ��ο�
	VOID							DrawGuild(	LPDIRECTDRAWSURFACE7 pSurface );
	VOID							DrawParty(	LPDIRECTDRAWSURFACE7 pSurface );
	VOID							DrawSiegeWarfare(LPDIRECTDRAWSURFACE7 pSurface );	// ��� ������ ���� �������̽� �׸���
	BOOL							DeleteFriendName();
	SI16							ItemSearch(SI16 total,UI16 id);

	// ��� �������̽�
	// ��ܿ� ��� ó��
	SI16							ActionMemberList(void);
	// ��ܿ� ��� ������ ó��
	SI16							ActionPage();
	// ��ܿ� ��� �׸���
	VOID							DrawMemberList(HDC hdc);
	// ��ܿ� ��� ������ ��ȣ �׸���
	VOID							DrawPage(HDC hdc);

public:
	BOOL							Quest_SetBankBookCount(MONEY Count);
	MONEY							Quest_GetBankBookCount( void )	{ return m_QuestBankBookCount; }		// actdoll (2004/01/14 17:02) : �߰�

	void							AuditTotal(BYTE *pMsg);
	void							FriendAddOne(BYTE *pMsg);
	void							FriendRemove(BYTE *pMsg);
	void							FriendRemoveAll(BYTE *pMsg);
	void							AuditOwn(BYTE *pMsg);
	void							VillageList(BYTE *pMsg);
	void							InvestmentList(BYTE *pMsg);
	void							GuildInfo(BYTE *pMsg);

	VOID							DrawTab( LPDIRECTDRAWSURFACE7	pSurface);
	HOEC							GetHOECFriend( void )	{ return m_pFriendList->m_hOecFriend; }

	
//	void							StatiStics(BYTE *pMsg);	
};


#endif
