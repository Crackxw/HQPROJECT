#ifndef	SO_BASECHAR_HEADER
#define SO_BASECHAR_HEADER

#include <Directives.h>
#include <Windows.h>

#include "SOCommonHeader.h"
#include "OnlineCommonStruct.h"
#include "IOnlineFollower.h"
#include "Friend.h"


// SendFieldMsg 보낼때의 형식 (나한테 보내느냐?, 내 주위 모두에게 보내느냐)
#define	SO_SFM_CUSTOM			0
#define	SO_SFM_ONLYME			1
#define	SO_SFM_ONLYNEARCHAR		2
#define	SO_SFM_MEANDNEARCHAR	3
#define	SO_SFM_ALLCHAR			4

// 클라이언트 속도에 맞추어 준다 
#define	SO_CONVERSION_CHARMOVEPSEED(x)		((1000 * 100 / (x)) / 30)

class   CBattle;
class   CBattleMgr;

class	BaseChar
{

// 전투에 관련된 변수와 함수  
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
	SI16			siX;							// X 좌표.
	SI16			siY;							// Y 좌표.	
	SI16			siStatus;						// 현재 상태.
	UI16			uiKind;							// 종류
	UI16			uiAccount;						// 계정.
	UI16			uiMapID;						// 현재 이 캐릭터가 있는 맵의 id
	UI32			uiMoveableTileInfo;				// 이 캐릭터가 이동할 수 있는 타일의 정보.
	UI08			bDidSendMoveInfo;				// 이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가.
	CHAR			szID[ON_ID_LENGTH + 1];			// 캐릭터 이름.(문자열 ID)
	PlayerPlace		Place;							// 현재 플레이어가 있는 장소.(필드, 마을, 전투)

	UI32			uiGuildID;						// 상단의 ID.
	SI32			siClassInGuild;					// 길드내에서의 직급.
	
	BOOL			m_bAttackMode;					// 전투 모드


	// 플레이어소유의 아이템 정보
	IOnlineFollower		IFollower;					// Follower 객체
	cltFriendList		m_clFriendList;				// 친구 리스트



public:
	BaseChar();								// 생성자.
	virtual	~BaseChar();					// 소멸자.

	// 초기화를 한다
	virtual	VOID	Init(UI16 uiAccount, UI16 uiMapID, SI16 siX, SI16 siY, UI16 uiKind, CHAR *pszID);					
	virtual	VOID	Init();																								


	
	//----- 메시지 관련 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// 필드에서 메시지를 보낸다.(나, 내주위)	
	virtual	BOOL	SendFieldMsg(SI32 siMsg, SI32 siSendInfo, LPVOID pParam1 = NULL, LPVOID pParam2 = NULL, LPVOID pParam3 = NULL, LPVOID pParam4 = NULL, LPVOID pParam5 = NULL, LPVOID pParam6 = NULL);

	// 실질적으로 메시지를 Send 한다.
	virtual	VOID	SendMsg(SI32 siSendInfo, CHAR *cBuffer, SI16 siLength, SI32 siSendCharNum = 0, UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE] = NULL, BOOL bSendMe = FALSE);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
	BOOL	DelIDOnMap( SI32 siParam1 );						// 맵에서 ID를 삭제한다.
	BOOL	SetIDOnMap( SI32 siParam1 );						// 맵에다 ID를 설정한다.

	VOID	GetInfo(OnCharBaseInfo *pCharBaseInfo);				// 기본 캐릭터의 정보를 얻어온다.
	

	CHAR*	GetID();											// ID를 얻어온다.


	virtual	BOOL	Warp(UI16 uimapid, SI32 six, SI32 siy);		// 캐릭터를 워프 시킨다.

	// Static 함수.
	static	VOID	InitStaticVariable(SOWorld *pWorld);		// 스태틱 변수를 초기화 한다.

	// OnlineReceivedPlayerInfo의 배열 구조체에 puiCharIDs에 있는 캐릭터들의 정보를 얻어와서 넣어준다.
	SI32			MakeOnReceivedPlayerInfoPacket(CHAR *pBuffer, SI32 siPlayerNum, UI16 *puiCharIDs, UI16 *puiRealPlayerNum, SI32 siParam1 );	
																																											
	UI16			GetKind();									// 캐릭터의 종류를 얻어온다.
	SI16			GetStatus();								// 캐릭터의 상태(필드, 마을, 전투중)를 얻어온다.
	VOID			GetPos(UI16 *puiMapID, LPPOINT pPoint);		// 캐릭터의 필드에서의 좌표를 얻어온다.
	BOOL			IsValid();									// 유효한 캐릭터인가?


	SI32			GetFightingPower(void);						// 전투력을 얻음

	static	BOOL	IsValidID(UI16 uiID);						// 유효한 캐릭터의 id 인지 본다.

	MyItemData*		GetItemList(UI08 uiFollowerSlot);			// 아이템리스트를 돌려준다.

	MyItemData*		AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory ,BOOL bSendToDB = TRUE);								// 아이템을 추가해준다.
	MyItemData*		AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory);						// 아이템을 추가해준다.
	MyItemData*		AddItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );													// 아이템을 추가해준다.
	MyItemData*		AddItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );												// 아이템을 추가해준다.
	MyItemData*		DelItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );								// 아이템을 삭제한다.
	MyItemData*		DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );						// 아이템을 삭제한다.	
	MyItemData*		DelItemNoQuery(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity );													// 아이템을 추가해준다.
	MyItemData*		GetItem(UI08 uiFollowerID, UI16 uiID);											// 아이템의 정보를 얻어온다.
	BOOL			CanAddItem(UI08 uiFollowerID, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum);	// 아이템을 추가할 수 있는지 알아온다.
	BOOL		    HasItem(UI08 uiFollowerID, UI16 uiID, UI16 uiQuantity, SI16 siPosInInventory );	// 아이템이 있는지 알아온다.

	void			FindDeleteItem(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData);	// 아이템이 있는지 검사하고 삭제한다.
	void			FindDeleteItemNoQuery(UI16 uiItemID, SI32 siCount, SI16* lpDelCount, MercenaryItemData* lpMercenaryItemData);	// 아이템이 있는지 검사하고 삭제한다.
	BOOL			GiveItem( UI08 uiFollowerID, UI16 uiItemID, UI16 uiItemQuantity, 
							  BaseChar *pOppBaseChar, UI08 uiOppFollowerID, SI16 *psiInvenPos, SI16 *psiOppInvenPos );		// 상대방에게 아이템을 준다.

	DWORD			GetAllFollowerInfo(OnFollowerInfo *pFollowerInfo, UI16 *puiFollowerNum);		// 모든 용병의 정보를 얻어온다.
	DWORD			GetFollowerInfo(UI08 uiFollowerID, OnFollowerInfo *pFollowerInfo);				// 용병의 정보를 얻어온다.
	BOOL			DeleteFollower(UI08 uiFollowerID);												// 용병을 삭제한다.

	VOID			SetMoved();																		// 이동했다고 설정한다.
	UI08			DidSendMoveInfo();																// 이동 했을시에 이동 메시지를 주위에 플레이어들에게 보냈는가 알아온다.

	UI08            FindItemSlot(UI08 uiFollowerID,UI16 uiItemID);
	VOID			SetAttackMode( BOOL flag );														// 전투모드, 평화모드 셋팅
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
