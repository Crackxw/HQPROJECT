#ifndef	SO_BASECHAR_HEADER
#define SO_BASECHAR_HEADER

#include <Directives.h>
#include <Windows.h>

#include "SOCommonHeader.h"
#include "OnlineCommonStruct.h"
#include "IOnlineFollower.h"
#include "Friend.h"


// SendFieldMsg �������� ���� (������ ��������?, �� ���� ��ο��� ��������)
#define	SO_SFM_CUSTOM			0
#define	SO_SFM_ONLYME			1
#define	SO_SFM_ONLYNEARCHAR		2
#define	SO_SFM_MEANDNEARCHAR	3
#define	SO_SFM_ALLCHAR			4

// Ŭ���̾�Ʈ �ӵ��� ���߾� �ش� 
#define	SO_CONVERSION_CHARMOVEPSEED(x)		((1000 * 100 / (x)) / 30)

class   CBattle;
class   CBattleMgr;

class	BaseChar
{

// ������ ���õ� ������ �Լ�  
//---------------------------------------------------------------------
protected:
	static CBattleMgr*		m_pBattleMgr;
	int						m_battleKey;

public:		
	inline CBattle*			GetBattle(void);

	virtual void			PrepareBattle();				
	virtual	void			EndBattle();							
	void					CancelBattle();
	
protected:
	inline CBattleMgr*		GetBattleMgr(void);			
	inline int				GetBattleKey(void);		
	bool					PickBattleWithEnemy(int enmeyID);				

private:
	bool					AddToBattle(int battlekey);
	inline void				SetBattleKey(int key);
	BaseChar*				GetBaseCharFromID(int id);
	void					CheckJustBattle(int enemyID);

	bool					IsAdminPlayer(int id);
	bool					IsInShip(int id);
//----------------------------------------------------------------------
	

public:			
	static	SOWorld		*pMyWorld;	
	 
public:
	SI16			siX;							// X ��ǥ.
	SI16			siY;							// Y ��ǥ.	
	SI16			siStatus;						// ���� ����.
	UI16			uiKind;							// ����
	UI16			uiAccount;						// ����.
	UI16			uiMapID;						// ���� �� ĳ���Ͱ� �ִ� ���� id
	UI32			uiMoveableTileInfo;				// �� ĳ���Ͱ� �̵��� �� �ִ� Ÿ���� ����.
	UI08			bDidSendMoveInfo;				// �̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°�.
	CHAR			szID[ON_ID_LENGTH + 1];			// ĳ���� �̸�.(���ڿ� ID)
	PlayerPlace		Place;							// ���� �÷��̾ �ִ� ���.(�ʵ�, ����, ����)

	UI32			uiGuildID;						// ����� ID.
	SI32			siClassInGuild;					// ��峻������ ����.
	
	BOOL			m_bAttackMode;					// ���� ���


	// �÷��̾������ ������ ����
	IOnlineFollower		IFollower;					// Follower ��ü
	cltFriendList		m_clFriendList;				// ģ�� ����Ʈ



public:
	BaseChar();								// ������.
	virtual	~BaseChar();					// �Ҹ���.

	// �ʱ�ȭ�� �Ѵ�
	virtual	VOID	Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID);					
	virtual	VOID	Init();																								


	
	//----- �޽��� ���� --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// �ʵ忡�� �޽����� ������.(��, ������)	
	virtual	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);

	// ���������� �޽����� Send �Ѵ�.
	virtual	VOID	SendMsg(SI32 siSendInfo, CHAR *cBuffer, SI16 siLength, SI32 siSendCharNum = 0, UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE] = NULL, BOOL bSendMe = FALSE);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	BOOL	DelIDOnMap( SI32 siParam1 );						// �ʿ��� ID�� �����Ѵ�.
	BOOL	SetIDOnMap( SI32 siParam1 );						// �ʿ��� ID�� �����Ѵ�.

	VOID	GetInfo(OnCharBaseInfo *pCharBaseInfo);				// �⺻ ĳ������ ������ ���´�.
	

	CHAR*	GetID();											// ID�� ���´�.


	virtual	BOOL	Warp(UI16 uimapid, SI32 six, SI32 siy);		// ĳ���͸� ���� ��Ų��.

	// Static �Լ�.
	static	VOID	InitStaticVariable(SOWorld *pWorld);		// ����ƽ ������ �ʱ�ȭ �Ѵ�.

	// OnlineReceivedPlayerInfo�� �迭 ����ü�� puiCharIDs�� �ִ� ĳ���͵��� ������ ���ͼ� �־��ش�.
	SI32			MakeOnReceivedPlayerInfoPacket(CHAR *pBuffer, SI32 siPlayerNum, UI16 *puiCharIDs, UI16 *puiRealPlayerNum, SI32 siParam1 );	
																																											
	UI16			GetKind();									// ĳ������ ������ ���´�.
	SI16			GetStatus();								// ĳ������ ����(�ʵ�, ����, ������)�� ���´�.
	VOID			GetPos(UI16 *puiMapID, LPPOINT pPoint);		// ĳ������ �ʵ忡���� ��ǥ�� ���´�.
	BOOL			IsValid();									// ��ȿ�� ĳ�����ΰ�?


	SI32			GetFightingPower(void);						// �������� ����

	static	BOOL	IsValidID(UI16 uiID);						// ��ȿ�� ĳ������ id ���� ����.

	MyItemData*		GetItemList(UI08 uiFollowerSlot);			// �����۸���Ʈ�� �����ش�.

	MyItemData*		AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory ,BOOL bSendToDB = TRUE);								// �������� �߰����ش�.
	MyItemData*		AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory);						// �������� �߰����ش�.
	MyItemData*		AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );													// �������� �߰����ش�.
	MyItemData*		AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );												// �������� �߰����ش�.
	MyItemData*		DelItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );								// �������� �����Ѵ�.
	MyItemData*		DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );						// �������� �����Ѵ�.	
	MyItemData*		DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );													// �������� �߰����ش�.
	MyItemData*		GetItem(UI08 uiFollowerID, UI16 uiID);											// �������� ������ ���´�.
	BOOL			CanAddItem(UI08 uiFollowerID, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum);	// �������� �߰��� �� �ִ��� �˾ƿ´�.
	BOOL		    HasItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );	// �������� �ִ��� �˾ƿ´�.

	void			FindDeleteItem(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData);	// �������� �ִ��� �˻��ϰ� �����Ѵ�.
	void			FindDeleteItemNoQuery(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData);	// �������� �ִ��� �˻��ϰ� �����Ѵ�.
	BOOL			GiveItem( UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, 
							  BaseChar *pOppBaseChar, UI08 uiOppFollowerID, SI16 *psiInvenPos, SI16 *psiOppInvenPos );		// ���濡�� �������� �ش�.

	DWORD			GetAllFollowerInfo(OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum);		// ��� �뺴�� ������ ���´�.
	DWORD			GetFollowerInfo(UI08 uiFollowerID, OnFollowerInfo *pFollowerInfo);				// �뺴�� ������ ���´�.
	BOOL			DeleteFollower(UI08 uiFollowerID);												// �뺴�� �����Ѵ�.

	VOID			SetMoved();																		// �̵��ߴٰ� �����Ѵ�.
	UI08			DidSendMoveInfo();																// �̵� �����ÿ� �̵� �޽����� ������ �÷��̾�鿡�� ���´°� �˾ƿ´�.

	UI08            FindItemSlot(UI08 uiFollowerID,UI16 uiItemID);
	VOID			SetAttackMode( BOOL flag );														// �������, ��ȭ��� ����
	BOOL			GetAttackMode();
};	

//--------------------------------------------------------------------------------------------
//	Name : GetBattleMgr()
//	Desc : 
//--------------------------------------------------------------------------------------------
inline CBattleMgr*	BaseChar::GetBattleMgr(void)
{
	return m_pBattleMgr;
}

//--------------------------------------------------------------------------------------------
//	Name : SetBattleKey()
//	Desc : 
//--------------------------------------------------------------------------------------------
inline void BaseChar::SetBattleKey(int key)
{
	m_battleKey = key;
}

//--------------------------------------------------------------------------------------------
//	Name : GetBattleKey()
//	Desc : 
//--------------------------------------------------------------------------------------------
inline int	BaseChar::GetBattleKey(void)
{
	return m_battleKey;
}

#include "BattleMgr.h"

//--------------------------------------------------------------------------------------------
//	Name : GetBattle()
//	Desc : 
//--------------------------------------------------------------------------------------------
inline CBattle* BaseChar::GetBattle(void)
{
	if(m_battleKey > -1) return m_pBattleMgr->GetBattle(m_battleKey);
	return NULL;
}



#endif
