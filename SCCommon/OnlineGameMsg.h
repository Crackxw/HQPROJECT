// ���� ���� : 2002-02-18 ���� 3:58:40 - ������

#ifndef	_ONLINE_GAMEMSG_H
#define _ONLINE_GAMEMSG_H

#include "GSCDefine.h"
#include "Defines.h"
#include "OnlineCommonStruct.h"
#include "Timer.h"

// encryption key

struct OnEncryptionKey
{
	DWORD		dwMsgType;
	
	char		cKey1;
	char		cKey2;

	DWORD		dwRoundKey[32]; // Seed Ű 
};



//---------------------------- ������ ���� �Ǹ� ���� ----------------------------------

struct OnResItemReceivedMsg				// ������ ���� �˸�
{
	DWORD		dwMsgType;
	UI16		uiCount;				//	������ ������ ���� 
};

struct OnReqItemListMsg					// ������ ����Ʈ ��û
{
	DWORD		dwMsgType;
};

struct OnResItemListMsg					// ������ ����Ʈ ��ȯ
{
	DWORD		dwMsgType;
	UI16		uiCount;
	// �ڿ� ItemID( 2byte ) , Quantity( 2byte ) ����( uiCount ) ��ŭ ����..
};

struct OnReqTakeOutItemMsg
{
	DWORD		dwMsgType;
	UI08		uiFollowerID;
	UI16		uiItemID;
	UI16		uiQuantity;
};

struct OnResTakeOutItemMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
};

struct OnReqSaveItemMsg
{
	DWORD		dwMsgType;
	UI08		uiFollowerID;
	UI16		uiItemID;
	UI16		uiQuantity;

	
};

struct OnResSaveItemMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
};

struct OnReqPersonalInfo
{
	DWORD		dwMsgType;
	char        szID[12];
	
	
};

struct OnResPersonalInfo
{
	DWORD		dwMsgType;
	DWORD       dwResponse;

	char        szID[12];

	SI32        siClassInGuildID;
	char        szGuildName[12];
		
};



//---------------------------- ���������� ����ϴ� ����ü -----------------------------//
struct	OnMsg
{
	DWORD		dwMsgType;	
};

struct	OnResponseMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
};



struct OnHeartBeatMsg	// ON_HEARTBEAT_INTERVAL( 1/1000�� ����)�� �ѹ��� ������.
{
	DWORD		dwMsgType;
	DWORD		dwTimer;			// ���ǵ��� ������ ���� Ŭ���̾�Ʈ�� Ÿ�̸Ӹ� ������.
	UI16		uiType;
};

struct	OnTextMsg
{
	DWORD		dwMsgType;			// ON_TEXT_MSG
	DWORD		dwResponse;			// 
	SI32		siStringLength;		// ���� ���ڿ��� ����

	// �ڿ��� CHAR������ ���ڿ��� ����´�.
};

//-------------- ���� ������ �����ϰ� �α� �ƿ��Ҷ� ������ �޽��� -------------------//
struct	OnReqGameServerLogInMsg
{
	DWORD		dwMsgType;
	CHAR		Guid[ON_MAX_GUID_LENGTH];
	UI08		uiCharSlot;
};

struct	OnReqGameServerLogOutMsg
{
	DWORD		dwMsgType;
};

struct	OnResGameServerLogOutMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	UI16		uiCharID;
};

//-------------------------------------------------------------------------------------//

//-------------- ���� ���� ���� �ÿ� ���� ������ ���� �޴� �޽��� ------//

// �α��� ������ ���� ������ ���� ���.
struct	OnLogInMsgHeader								
{
	DWORD						dwResponse;				// ������ ������ ����.
};

// ���� ������ �α��� �������� ����� �޽���,
struct	OnGameServerLogInMsg  : public OnResponseMsg			
{		
	// dwResponse�� ON_RET_OK �� ��� OnGameServerLogInMsgData �� �̾ �´�.
};

//--------------------------------------------------------------------------------------//

//------------------------ ĳ���͸� �̵� ��ų�� ������ �޽��� --------------------------//
struct	OnReqMyCharMoveMsg
{
	DWORD						dwMsgType;
	UI16						uiX;
	UI16						uiY;
	BOOL						bWarUnit;
};

struct OnCharMoveMsg
{
	DWORD						dwMsgType;
	UI16						uiMovedCharNum;

	// UI16 : ID
	// UI16 : X
	// UI16 : Y
	// UI16 : bOverWeight
};
//--------------------------------------------------------------------------------------//


// ĳ���͸� ���� ��ų�� ������ �޽���
struct	OnReqWarpMsg
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];
	UI16						uiWarpCode;				// warp code
};

struct	OnResWarpMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_WARP
	DWORD						dwResponse;				// ON_RET_OK : ����, ON_RET_NO : ����
};

struct	OnResWarpOkMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_WARP
	DWORD						dwResponse;				// ON_RET_OK : ����, ON_RET_NO : ����
	UI16						uiMapID;				// �� ���ο� ���� �ε���.
	UI16						uiX;					// �� ���ο� X ��ǥ.
	UI16						uiY;					// �� ���ο� Y ��ǥ.
	UI16						uiPlayerNum;			// �� ������ �÷��̾� ��.	
	UI16						uiFieldItemNum;			// �� ������ ���� ������ ������ ��. 
	// ������ �÷��̾�, ���� ������ �������� ������ �̾ �ִ�.
};


//------------------------ ������ ���ٴ� �޽���   ----------------------------------//
struct	OnReqGotoVillageMsg
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct	OnResGotoVillageOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiCharID;
};

struct OnReqWarVillageDefence
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct OnResWarVillageDefence
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	SI32						siLimitTime;
	UI16						uiUniqueID;
};

struct OnReqWarVillageIn
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct OnResWarVillageIn
{
	DWORD						dwMsgType;
	DWORD                       dwResponse;
	UI16						uiUniqueID;
	//�ڱ� �ڽ����� ������ 
};

//--------------------------------------------------------------------------------------//

//------------------------ �������� �����ٴ� �޽���   ----------------------------------//
struct	OnReqGoOutVillageMsg
{
	DWORD						dwMsgType;
};	

struct	OnResGoOutVillageMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGoOutVillageOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiPlayerNum;
	UI16						uiFieldItemNum;
};
	
struct	OnResGoOutVillageNoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResGoOutVillagPlayerMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	CHAR						szID[ON_ID_LENGTH + 1];
	UI16						uiCharID;
	UI16						uiX;
	UI16						uiY;
	UI16						uiKind;
};
//--------------------------------------------------------------------------------------//

//----------------------------- ä�� ���� �޽���  --------------------------------------//
struct	OnReqChattingMsg
{
	DWORD						dwMsgType;
	DWORD						dwChatLength;
};

struct	OnResChattingMsg
{
	DWORD						dwMsgType;
	DWORD						dwChatLength;
	UI16						uiCharID;	
};
//--------------------------------------------------------------------------------------//

//---------------------------- �����Ѵٴ� �޽��� ---------------------------------------//
struct	OnReqBattleMsg
{
	DWORD						dwMsgType;	
	UI16						uiCharID;
};

struct	OnResBattleMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBattleStartMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwBattleServerIP;
	UI16						uiBattleServerChannelNum;		
	UI16						uiRandomSeed;
};	

typedef struct
{
	DWORD						dwMsgType;

	UI16						uiUniqueID;
	UI08						fFlag;						// 1 : Battle Wait, 0 : Battle Cancel
} OnResBattlePlayer;

//--------------------------------------------------------------------------------------//

//---------------------- Ŭ���̾�Ʈ�� ĳ���͸� �߰� �� ���� �϶�� �޽��� --------------//
struct	OnNewInfoOnMoveMsg
{
	DWORD						dwMsgType;
	UI16						uiAddCharNum;
	UI16						uiDelCharNum;
	UI16						uiItemNum;
};

struct	OnAddCharMsg
{
	DWORD						dwMsgType;
	
	// �÷��̾ ���� �Ѹ����� ���� ������ ����´�.
};

struct	OnDelCharMsg
{
	DWORD						dwMsgType;
	UI16						uiCharID;
};
//-------------------------------------------------------------------------------------//

//-------------------------���忡�� �������� ��� �ȶ� ����ϴ� �޽��� ----------------//
struct	OnReqItemListInMarketMsg
{
	DWORD						dwMsgType;	
};

struct	OnResItemListInMarketMsg
{
	DWORD						dwMsgType;
	DWORD						dwItemNum;
};

struct	OnReqBuyItemMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiItemID;	
	UI16						uiItemQuantity;	
	UI16						uiPosInInventory;
};

struct	OnResBuyItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResBuyItemOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						siMoney;
	UI32						uiTradeCredit;
};

struct	OnReqSellItemMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiItemID;	
	UI16						uiItemQuantity;	
};

struct	OnResSellItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResSellItemOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						siMoney;
	UI32						uiTradeCredit;
};

struct	OnChangeItemInfoMsg
{
	DWORD						dwMsgType;
	OnlineReceivedItemInfo		ItemInfo;
};
//-------------------------------------------------------------------------------------//

//---------------------------- �ʵ忡�� �������� ������ �ݴµ� ����ϴ� �޽��� --------//
struct	OnReqThrowItemMsg
{
	DWORD						dwMsgType;
	UI16						uiItemID;
	UI16						uiQuantity;	
	UI08						uiFollowerID;
};

typedef	OnResponseMsg			OnResThrowItemMsg;

/*
struct	OnResThrowItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct	OnResThrowItemOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;		
	UI16						uiCharID;			
	UI16						uiX;
	UI16						uiY;
	UI16						uiQuantity;
	UI16						uiItemID;	

};


struct	OnResThrowItemNoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
*/
struct	OnReqPickUpItemMsg
{
	DWORD						dwMsgType;
	UI16						uiX;
	UI16						uiY;
	UI16						uiItemID;
	UI16						uiQuantity;	
	UI16						uiPosInInventory;
	UI08						uiFollowerID;
};

typedef	OnResponseMsg			OnResPickUpItemMsg;
/*
struct	OnResPickUpItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResPickUpItemOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	UI16						uiCharID;		
	UI16						uiItemID;
	UI16						uiQuantity;	
	UI16						uiQuantityOfMap;
	UI16						uiX;
	UI16						uiY;	
	UI16						uiPosInInventory;
	UI08						uiFollowerID;
};

struct	OnResPickUpItemNoMsg
{
	DWORD						dwMsgType;
	DWORD						dwReponse;
};
*/
struct	OnChangeFieldItemMsg
{
	DWORD						dwMsgType;	
	SI16						siX;
	SI16						siY;
	UI16						uiItemID;
	UI16						uiItemQuantity;
};

//-------------------------------------------------------------------------------------//

//------------------------------ ��ο��� ����Ҷ��� �޽��� ---------------------------//
// Tab 1. ���ڻ�
struct	OnReqAuditTotalMsg
{
	DWORD						dwMsgType;
};

struct	OnResAuditTotalMsg
{
	DWORD						dwMsgType;
	DWORD						dwReponse;
};

struct	OnResAuditTotalOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwReponse;
	UI32						uiTotalUser;				// ��ü ��� ����ڼ�	
	MONEY						siProperty;					// �� �ڻ�.
	MONEY						siBankMoney;				// ���� �ܰ� (+)
	MONEY						siLoanMoney;				// ���� ���� (-)
};

// Tab 2. ��� ����.
struct	OnReqAuditAccountListMsg
{
	DWORD						dwMsgType;	
};

struct	OnResAuditAccountListMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResAuditAccountListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	DWORD						dwBankBookNum;	//�ִ� 20�� �̴�.
	
	// �ڿ��� dwBankBookNum ��ŭ ������ ���� ��ȣ(OnAccountNumber)�� ����´� (���¹�ȣ���� ���� ����)

	// �ڿ��� dwBankBookNum ��ŭ �� ������ �� �ִ� ��(MONEY)�� ����´�.
};

// Tab 3. �ε��� ����.
struct OnReqAuditOwnListMsg
{
	DWORD						dwMsgType;	
};


struct OnResAuditOwnListMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResAuditOwnListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwOwnNum;

	// �ڿ��� dwOwnNum��ŭ �ε��� ����� ����´�. (OnAuditOwnInfo ����ü)
};


struct OnReqAuditOwnInfoMsg
{
	DWORD						dwMsgType;
	UI32						uiOwnID;
};

struct OnResAuditOwnInfoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResAuditOwnInfoOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// ����, ����, �ü� ��� �� ������ ����´�.
};

// Tab 4. ���� ����

struct OnReqAuditVillageInfoMsg
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct OnResAuditVillageInfoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResAuditVillageInfoOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	// ������ ����.
	// ��� ��ǰ ����
	// ��� ��ǰ ������ �´�.
};


// Tab 5. ���� ����
struct OnReqSetNameMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerNum;
	CHAR						cName[ON_ID_LENGTH];
};

struct OnResSetNameMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

/*
struct OnReqAuditFollowerListMsg
{
	DWORD						dwMsgType;
};

struct OnResAuditFollowerListMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResAuditFollowerListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwFollowerNum;
	
	// �ڿ��� dwFollowerNum��ŭ uiFollowerID�� ����´�(UI08).

	// �ڿ��� dwFollowerNum��ŭ ���� �̸��� ����´�
};


struct OnReqAuditFollowerInfoMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
};

struct OnResAuditFollowerInfoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResAuditFollowerInfoOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// ����, ��ġ, ����, ��, ��ø, �ǰ�, ����, �����, ����, �����, ����,
	// �ּҰ���, �ִ����, ���� ����, ȸ�Ǽ��� ����
};*/
//-------------------------------------------------------------------------------------//

//------------------------------- �ʵ忡�� �Ϲ����� �޽��� ----------------------------//

struct	OnResMoneyInfo : public OnResponseMsg
{	
	MONEY						Money;	
};

struct OnResRelationMsg : public OnResponseMsg
{
	BOOL						bRelation;
};

// ���� �����ڵ�
struct	OnReqVillageGetNation
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct	OnResVillageGetNation
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiVillageCode;
	CHAR						cNationCode;
	VDEF						Durability;
	VDEF						MaxDurability;
	CHAR						GuildName[ON_GUILDNAME_LENGTH];
	SI32						siGuildFlag;
	DWORD						dwGuildRelation;				// ����� ����.
	SI32						siDefenceSoldier;
	SI16						siAttackType;
};


// ���� �ȳ���
struct	OnReqWriteDescript			// ���� �ȳ��� ����
{
	DWORD						dwMsgType;
	UI16						uiLength;				// ���� �ȳ��� ����.
	// �ڿ��� uiLength  Byte��ŭ Text�� �´�.
};

struct	OnResWriteDescript
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnReqVillageDescript		// ���� �ȳ��� �б�
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct	OnResVillageDescript
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
	CHAR						szGuildID[ON_GUILDNAME_LENGTH];		// �� ������ ������ ����� ID	
	UI16						uiLength;							// ���� �ȳ��� ����.
	// �ڿ��� uiLength  Byte��ŭ Text�� �´�.
};

struct	OnReqImoticonMsg				//	�̸�Ƽ�� ǥ��
{
	DWORD						dwMsgType;
	UI16						uiImoticonID;
};

struct	OnResImoticonMsg
{
	DWORD						dwMsgType;
	UI16						UserID;					// ǥ���� ���� ���
	UI16						uiImoticonID;			// �̸�Ƽ�� ����
};

//-------------------------------------------------------------------------------------//

//--------------------------------- ���� �Ÿ����� �޼��� ------------------------------//
// ���Ǵ뿡 ���ٴ� �޼��� (OK,NO,PLAYER)
struct	OnReqChargeBoothMsg 
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
};

struct	OnResChargeBoothMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// ���Ǵ뿡�� �����ٴ� �޼���
struct	OnReqLeaveBoothMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
};

struct	OnResLeaveBoothMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// ���Ǵ뿡 �������� �ø�
struct	OnReqSetBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
	UI08						uiNum;							// ������ ����.( 0~5 ��)
	// �ڿ��� uiNum ��ŭ OnBoothItem ����ü�� �´�.
};

struct	OnResSetBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// ���Ǵ뿡 �÷� ���� ������ ���
struct	OnReqCancelBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
	UI08						uiNum;
};

struct	OnResCancelBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// ���Ǵ뿡 �ִ� �������� ��û�ϴ� �޼��� (OK,NO,EMPTY)
struct	OnReqDisplayBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
};

struct	OnResDisplayBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiBoothID;
};

struct	OnResDisplayBoothItemOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiBoothID;
	UI16						uiItemID[ON_MAX_BOOTH_ITEM];
};

// ���Ǵ��� �������� Ŭ��������, �������� ������� �����ִ� ���� �޼���
struct	OnReqGetBoothInfoMsg
{
	DWORD						dwMsgType;	
	UI16						uiBoothID;
};

struct	OnResGetBoothInfoMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI08						uiNum;
//	uiNum ��ŭ OnBoothItemInfo ����ü�� ���� �´�.
};

// ���Ǵ뿡�� ������ �춧 �޼���(����: ��°� ��������. �ǻ��:�ǰ� ��������. �ֺ�������:���̴°� �޶������� �޶����ٴ� ����)
struct	OnReqBuyBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
	UI16						uiItemID;
	UI08						uiFollowerID;
	UI16						uiPos;
	UI16						uiQnt;		// ����� ���� ����
};
struct	OnResBuyBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
struct	OnResSellBoothItemMsg		// �� ����� �޴� �޼���.
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiInvenPos;		//	������ ������Ʈ �Ѵ�. (��ġ)
	UI16						uiItemID;
	UI16						uiQuantity;		//	(���� ����- �����)
	MONEY						siMoney;		//	���ؼ� �����ϰ� �� ��
};

//-------------------------------------------------------------------------------------//


//��Ƽ ��û.
struct  OnReqAskPartyMsg
{
	DWORD						dwMsgType;
	UI16						UserID;		// ��Ƽ ��û ���� Ŭ���̾�Ʈ�� ID
};

//��Ƽ ��û ���� Ŭ���̾�Ʈ�� ���� �޼���.
struct	OnReceiveAskPartyMsg					
{
	DWORD						dwMsgType;
	UI16						UserID;					// ��Ƽ�� ���� ����� ID.			
	CHAR						szID[ON_ID_LENGTH + 1];	// �̸�.
};


//��Ƽ ��û ���� Ŭ���̾�Ʈ�� ������ �޼���.���� �ε���
struct OnResAskPartyMsg
{
	DWORD						dwMsgType;				
	DWORD						dwResponse;				//OK,NO �� ����.	
};

//��Ƽ ��û ���� ����� ���� Ŭ���̾�Ʈ�� ����� ���� ���� ����.
struct	OnResAcceptPartyMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// OK�� ��� siNum��ŭ ��Ƽ���� ���� ���ؼ� �����ش�.
	CHAR						szID[ON_ID_LENGTH + 1];
	SI16						siNum;					//siNum��ŭ ��Ƽ���� ���� ���Ѵ�.OnPartyInfo 

};

struct	OnResLeavePartyMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_LEAVE_PARTY
	DWORD						dwResponse;
	SI32						siMembersNum;
	CHAR						szLeavedPlayerID[ ON_ID_LENGTH + 1 ];	
};

//----------------------------------- �ŷ� ���� �޼��� --------------------------------//
// �ŷ��� ��û�ϴ� �޼���.
struct	OnReqAskTradeMsg				//	(�ŷ��� ��û�� ���)
{	
	DWORD						dwMsgType;
	UI16						UserID;					// ��ȯ�ϰ��� �ϴ� ���	
};

typedef	OnResponseMsg			OnResAskTradeMsg;

struct	OnReceiveAskTradeMsg			//	(�ŷ��� ��û ���� ���)
{
	DWORD						dwMsgType;
	UI16						UserID;					// �ŷ��� ��û�� ��� ���̵�.	
	CHAR						szID[ON_ID_LENGTH + 1];	// �ŷ��� ��û�� ��� �̸�.
};

//  ������ �÷� ������ �޼���.
struct	OnReqPreTradeMsg				//	(������ �÷����� ���)
{
	DWORD						dwMsgType;
	UI08						uiItemNum;			//	�������� ��
	UI08						uiFollowerNum;		//	�뺴�� ��
	MONEY						Money;				//	�� �׼�
	//		�����ۼ� ��ŭ �����۱�ȯ ����ü�� �´�.   --	 TradeItem[uiItemNum] ����ü �迭.
	//		  �뺴�� ��ŭ �뺴��ȯ   ����ü�� �´�.
};

struct	OnReceivePreTradeMsg			//	(������ �޴� �޼���)
{
	DWORD						dwMsgType;
	UI08						uiItemNum;			//	�������� ��
	UI08						uiFollowerNum;		//	�뺴�� ��
	MONEY						Money;				//	�� �׼�
	//		�����ۼ� ��ŭ �����۱�ȯ ����ü�� �´�.   --	 TradeItem[uiItemNum] ����ü �迭.
	//		  �뺴�� ��ŭ �뺴��ȯ   ����ü�� �´�.
};

//	������ �÷����� ������ų�� - ������ư
struct	OnReqConfirmMsg
{
	DWORD						dwMsgType;
	BOOL						bType;					//	������ư On, Off
};

struct	OnReceiveConfirmMsg
{
	DWORD						dwMsgType;
	BOOL						bType;					//	������ư On, Off
};

//  ������ ��ȯ �Ҷ� �޼���.
struct	OnReqTradeSuccessMsg				//	(������ �÷����� ���)
{
	DWORD						dwMsgType;
	UI08						uiSendItemNum;		//	�������� ��	
	UI08						uiRecvItemNum;		//	�޴� ������ ��
	UI08						uiSendFollowerNum;	//	���� �뺴�� ��
	UI08						uiRecvFollowerNum;	//	�޴� �뺴 ��.
	MONEY						SendMoney;			//	�ִ� �� �׼�
	MONEY						RecvMoney;			//	�޴� �� �׼�
	// uiItemNum��ŭ MyItemData ����ü�� �´�.
	// uiRecvItemNum��ŭ SI16ũ���� �κ��丮 ��ġ�� �´�.
};

// ���� ��ȯ �Ҷ��� ����.
struct	OnResTradeSuccessMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	/*UI08						RecvItemNum;			//	�������� ��
	UI08						RecvFollowerNum;		//	�뺴�� ��
	MONEY						RecvMoney;				//	�� �׼�
	UI08						SendItemNum;			//	�������� ��
	UI08						SendFollowerNum;		//	�뺴�� ��
	MONEY						SendMoney;				//	�� �׼�
	//	A��	�����ۼ� ��ŭ �����۱�ȯ ����ü�� �´�.   --	 TradeItem[uiItemNum] ����ü �迭.
	//	A��  �뺴�� ��ŭ �뺴��ȯ   ����ü�� �´�.
	//	B��	�����ۼ� ��ŭ �����۱�ȯ ����ü�� �´�.   --	 TradeItem[uiItemNum] ����ü �迭.
	//	B��	  �뺴�� ��ŭ �뺴��ȯ   ����ü�� �´�.*/
};

// �ŷ��� ����ؼ� ������ �޼���
struct	OnReqCancelTradeMsg
{
	DWORD						dwMsgType;				// ��� ������ ���.
};


// ������ �Ҹ� �Ѵٴ� �޼���.
struct	OnReqSpendItemMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;			//	����
	UI16						uiItemID;				//	������
	UI16						uiQuantity;				//	��
	UI16						uiPosInventory;			//	�κ��丮 ��ġ
};
 
struct	OnResSpendItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};
//-------------------------------------------------------------------------------------//

//------------------------------ ���࿡�� ����Ҷ��� �޽��� ---------------------------//
struct	OnReqBankInfoMsg
{
	DWORD						dwMsgType;
};

struct	OnResBankInfoMsg
{
	DWORD						dwMsgType;
	SI16						siInterest;
	MONEY						siBankCapital;
};

struct	OnReqBankDepositMsg
{
	DWORD						dwMsgType;
	OnAccountNumber				AccountNumber;	
	MONEY						siMoney;
};

struct	OnResBankDepositMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResBankDepositOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	MONEY						siBalance;					// ������ �ܰ� �����ش�.
	MONEY						siMoney;					// ���� �ܾ��� �����ش�.	
};

struct	OnReqBankDrawingMsg
{
	DWORD						dwMsgType;
	OnAccountNumber				AccountNumber;	
	MONEY						siMoney;
};

struct	OnResBankDrawingMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBankDrawingOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	MONEY						siBalance;
	MONEY						siMoney;
};
// gguo
// ���� �Ա�
struct	OnReqBankSaveMsg
{
	DWORD						dwMsgType;
	MONEY						moMoney;
};

struct	OnResBankSaveMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	MONEY						moSaving;
	MONEY						moMoney;
};

// ���� ���
struct	OnReqBankWithdrawMsg
{
	DWORD						dwMsgType;	
	MONEY						moMoney;
};

struct	OnResBankWithdrawMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	MONEY						moSaving;
	MONEY						moMoney;
};


//���� ������ ��û�Ѵ�.
struct OnReqBankAccountSort
{
	DWORD						dwMsgType;
	UI16						uiKind;
};

// ���¸� �����.
struct	OnReqBankOpenSavingAccountMsg
{
	DWORD						dwMsgType;
	UI16						uiKind;						// ������ ����	
	MONEY						siFirstDepositMoney;		// ó���� �Ա��ϴ� ��
	UI16						uiExpireMonth;				// ���� ��¥ (������ �ƴ� ��� 0)
};

// ���¸� ����ٴ� �޽����� ������ ������ ����
struct	OnResBankOpenSavingAccountMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// ���¸� ����ٴ� �޽����� ������ ������ ����(����)
struct	OnResBankOpenSavingAccountOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	OnAccountNumber				AccountNumber;
	MONEY						siMoney;
};

// ������ ����Ʈ�� �޶�� �ϴ� �޽���
struct	OnReqBankBankBookListMsg
{
	DWORD						dwMsgType;	
};

struct	OnResBankBankBookListMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResBankBankBookListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	DWORD						dwBankBookNum;	
	
	// �ڿ��� dwBankBookNum ��ŭ ������ ���� ��ȣ(OnAccountNumber)�� ����´�

	// �ڿ��� dwBankBookNum ��ŭ �� ������ �� �ִ� ��(MONEY)�� ����´�.
};

// ������ ����Ʈ�� �޶�� �ϴ� �޽���
struct	OnReqBankBankBookListCountMsg
{
	DWORD						dwMsgType;	
};

struct	OnResBankBankBookListCountMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	

	MONEY						BankBookCount;
};

struct	OnReqBankMaxCreditLoanMsg					//�ſ������ �ѵ��ݾ�
{
	DWORD						dwMSgType;
};


struct	OnResBankMaxCreditLoanMsg
{
	DWORD						dwMSgType;
	DWORD						dwResponse;	
};

struct	OnResBankMaxCreditLoanOkMsg					//�ſ������ �ѵ��ݾ�
{
	DWORD						dwMSgType;
	DWORD						dwResponse;	
	MONEY						uiMaxMoney;
	MONEY						siProperty;			//	�� �ڻ�.
	//UI32						uiCreditRank;		//	�ſ뵵 ��ŷ
};

// �㺸
struct	OnReqCollateralLoanMsg
{
	DWORD						dwMsgType;
	UI16						uiMonth;
	MONEY						siMoney;
	DWORD						dwItemNum;

	// �ڿ��� dwItemNum ��ŭ �� �������� ���� �ִ� ���.	(UI08 uiFollowerID[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �������ڵ尡 ����´�.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �����ۺ� ������ ����´�.	(UI16 uiNum[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �κ��丮 ��ġ�� ����´�.	(SI16 siInvenLoc[MAX_COLLATERAL_NUM])
};

struct	OnResCollateralLoanMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct	OnResCollateralLoanOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	OnAccountNumber				AccountNumber;
	MONEY						siMoney;
};

// �㺸List�� �䱸�ϴ� �޼��� (���� ���� �����϶�, ���Ǹ� ã�� ��...)
struct	OnReqCollateralListMsg
{
	DWORD						dwMsgType;
};

struct	OnResCollateralListMsg					//�ڽ��� �㺸 List�� �������� �޼���
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct	OnResCollateralListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwItemNum;
	// �ڿ��� dwItemNum ��ŭ �� �������ڵ尡 ����´�.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �����ۺ� ������ ����´�.	(UI08 uiNum[MAX_COLLATERAL_NUM])
};

//List�� �ִ� ��� �㺸�� ã�ư��ٴ� �޼���
struct	OnReqTakeCollateralMsg
{
	DWORD						dwMsgType;
	DWORD						dwItemNum;	
	// �ڿ��� dwItemNum ��ŭ �� �������� ���� �ִ� ���.	(UI08 uiFollowerID[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �������ڵ尡 ����´�.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �����ۺ� ������ ����´�.	(UI16 uiNum[MAX_COLLATERAL_NUM])
	// �ڿ��� dwItemNum ��ŭ �� �κ��丮 ��ġ�� ����´�.	(SI16 siInvenLoc[MAX_COLLATERAL_NUM])
};

struct	OnResTakeCollateralMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};




//-------------------------------------------------------------------------------------//

//------------------------------ ���忡�� ����Ҷ��� �޽��� ---------------------------//
struct	OnReqFarmInfoMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
};

struct	OnResFarmInfoMsg
{
	DWORD						dwMsgType;
	UI32						uiIndustrialComplexNum;

	// uiIndustrialComplexNum ��ŭ OnIndustrialComplexInfo ����Ÿ�� �̾ �´�.
	//OnIndustrialComplexInfo		IndustrialComplexInfo[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ];
};

struct	OnReqFarmBuyMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� �����ΰ�?	(0~9)
};

struct	OnResFarmBuyMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// ������ �Ǵ�.
struct	OnReqFarmSellMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� �����ΰ�?	(0~9)
};

struct	OnResFarmSellMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResFarmSellOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						Money;				//	������ �Ȱ� ���� ��..
};
// ó�� ���忡 ���� �޴� �޼���
struct OnReqFarmSlotInfoMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ������ ���Ե��� ���� ������?	(0~9)
};

struct OnResFarmSlotInfoMsg
{
	DWORD						dwMsgType;							// ON_RESPONSE_FARM_SLOTINFO
	SI16						siMaxConstructibleFactoryNum;		// �ִ� �Ǽ��� �� �ִ� ������ ��.
	SI16						siFactoryNum;						// siFactoryNum�� -1 �̸� �����̴�.

	// �ڿ��� siFactoryNum(������ �ǹ� ��)��ŭ OnFarmSlotList�� �´�.
};

/*
struct OnResFarmSlotInfoOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwListNum;			//	�ǹ��� ������ ����..
	// �ڿ��� dwListNum(������ �ǹ� ��)��ŭ OnFarmSlotList�� �´�.
};
*/
struct	OnReqFarmBuildStructMsg							//	������ ���Կ� �ǹ��� ������ �Ҷ�.
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	��� �����ΰ�?
	CHAR						Code[4];			//	�������� �ǹ� ����
	SI16						siPay;				//  �ӱ�
};

struct OnResFarmBuildStructMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmExpandSlotMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ���� ������ �ø��°�?
};


struct OnResFarmExpandSlotMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmDestroyStructMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ���� ������ �ø��°�?
	UI08						uiSlotIndex;		//	� ������ �νǰ��ΰ�?
};

struct OnResFarmDestroyStructMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmSetEquipmentMsg						//	�ü��� ��ġ(���׷��̵�) �޼��� 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�
	UI08						uiUpIndex;			//	����� ��ġ �ϰڴ°�?
};

struct OnResFarmSetEquipmentMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmUpgradeSlotMsg						// ���׷��̵� ���� ��ȹ�Ѵ�.
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	� ���Կ�
	SI16						siPay;				//	��1 �� ǰ��
};
struct OnResFarmUpgradeSlotMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmItemProductMsg						//	������ ���� ��û �޼���.
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�	
	UI16						uiItemCode;			//	� ��������
	UI32						uiQuantity;			//	� ������ ���ΰ�?
	UI32						uiPay;				//	��1 �� ǰ��
};

struct OnResFarmItemProductMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct OnReqPlantChangePayMsg						//	�ӱ��� �����ϴ� �޼���
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�	
	SI16						siPay;				//	�ٲ�� �ӱ�
};

struct OnResPlantChangePayMsg						//	������ ���� ��û �޼���.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						ChangedMoney;		//	�ٲ� �� �׼�.
};

struct OnReqPlantCancelPlanMsg						//	�ӱ��� �����ϴ� �޼���
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�	
};

struct OnResPlantCancelPlanMsg						//	������ ���� ��û �޼���.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						ChangedMoney;		//	�ٲ� �� �׼�.
};

struct OnReqFarmWorkStartMsg
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	� ���Կ��� ���ҷ��°�?
};

struct OnResFarmWorkStartMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmWorkMsg								//	���� �ߴٴ� �޼���
{
	DWORD						dwMsgType;
	UI08						uiGameType;			//	�̴ϰ�������
	UI08						uiSlotIndex;		//	� ���Կ�
	SI32						siLabor;			//	�뵿��
	SI08						siStep;				//  �� �ܰ����� ��Ÿ��
};

struct OnFarmWorkHack
{
	DWORD						dwMsgType;
	UI08						uiType;				// �̴ϰ��� ������ ���� ¦���߱� +10, ���� +20, ���Ϲޱ� +30
													// �̴ϰ��� �߿� �ð� ��ŷ 0, ����Ʈ ��ŷ 1, �۾��� ��ŷ 2, �̴ϰ��� ���� �� ����ü Labor���� 3
	SI08						siStep;				// �ܰ�
	SI32						siPoint1;			// ������ ��
	SI32						siPoint2;			// ��ŷ�� ã������ ���ܳ��� ���� �� 
};

struct OnResFarmWorkMsg : OnResponseMsg
{		
	UI32						uiSlotRemainLabor;	//	���� �뵿��
	MONEY						siPay;				//	���ؼ� ���� ǰ��.
};

struct OnReqFarmWorkInfoMsg							//	�ü��� ��ġ(���׷��̵�) �޼��� 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�
};

struct OnResFarmWorkInfoMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResFarmWorkInfoOkMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
	
	UI16						uiItem;
	SI32						uiNum;				//	������ ����
	UI32						uiFinalNum;			//	���� ��ǥ �����.(0�̸� ������ ���������� ����)
	SI16						siPay;
};

struct OnReqStorageInputItemMsg						//	â�� �������� �ִ� �Լ�
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:����, 1:����, 2:����, 3:����,  (GSCdefine�� �ֽ�.)
	UI08						uiPartID;			//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�
	UI08						uiFollowerID;		//	������
	SI16						siPosInInventory;	//	��� �ִ�
	UI16						uiItemCode;			//	� ��������
	UI32						uiQunatity;			//	��� ���� �ִ°�?
};

struct OnResStorageInputItemMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqStorageOutputItemMsg					//	â�� �������� ������ �Լ� 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:����, 1:����, 2:����, 3:����,  (GSCdefine�� �ֽ�.)
	UI08						uiPartID;			//	��� ����
	UI08						uiSlotIndex;		//	� ���Կ�
	UI08						uiFollowerID;		//	������
	UI16						uiItemCode;			//	� ��������
	SI16						siPosInInventory;
	UI32						uiQunatity;
};

struct OnResStorageOutputItemMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqStorageInfoMsg			//	â�� �ִ� ��� �������� ���´�.(����, ����, ����, ���� ��ο� �� ����,)
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:����, 1:����, 2:����, 3:����,  (GSCdefine�� �ֽ�.)
	UI08						uiPartID;			//	��� �ǹ� (FarmIndex�� ���� ����)
	UI08						uiSlotIndex;		//	â�� �ִ� ����
};

struct OnResStorageInfoMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnResStorageInfoOkMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI08						uiNum;				//	������ ����	
	// uiNum��ŭ STORAGE_ITEM �� �´�.				
};

typedef	OnMsg	OnFarmWorkCompletedMsg;				// ON_FARMWORK_COMPLETED

//-------------------------------------------------------------------------------------//


//------------------- ������ �ϰ� �ٽ� �¶��� �������� ���ư���.  ---------------------//
struct	OnReqReturnOnlineMsg
{
	DWORD						dwMsgType;
	BOOL						dwBattleResult;			// TRUE - ��, FALSE - ��
}; 

struct	OnResReturnOnlineMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// ����
	UI16						uiFollowerNum;			// ���ϵ��� ��.
	MONEY						siMoney;				// ��
	SI32						siHealth;				// ����� ��ġ.		
};

struct	OnResReturnOnlineFieldMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// ����
	UI16						uiFollowerNum;			// ���ϵ��� ��.
	MONEY						siMoney;				// ��
	SI32						siHealth;				// ����� ��ġ.		
	UI16						uiPlayerNum;			// �� ������ �÷��̾� ��.	
	UI16						uiFieldItemNum;			// �� ������ ���� ������ ������ ��. 	
	SI32						siEventScore;			// �̺�Ʈ ����.
	SI32						siEventRanking;			// �̺�Ʈ ��ŷ.
	// �� �ڿ��� ������, �÷��̾�, ���� ������ �������� ���� ������ �ִ�.
};

struct	OnResReturnOnlineVillageMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// ����
	UI16						uiFollowerNum;			// ���ϵ��� ��.
	MONEY						siMoney;				// ��
	SI32						siHealth;				// ����� ��ġ.		
	UI16						uiVillageCode;			// ������ ID.
	OnRecvVillageInfo			VillageInfo;			// ������ ����
	// �� �ڿ��� ���ϵ��� ������ �ִ�.
};

//-------------------------------------------------------------------------------------//

//--------------------- �÷��̾�(���� ����)�� ������ �޶�� �Ѵ�. ---------------------//
struct	OnReqPlayerInfoMsg
{
	DWORD						dwMsgType;
	UI16						uiCharNum;
};

struct	OnResPlayerInfoMsg
{
	DWORD						dwMsgType;	
	UI16						uiCharNum;

	/*
	UI16						uiCharID;
	UI16						uiX;
	UI16						uiY;
	UI16						uiKind;
	// ���� ��� �÷��̾��� ��� �ڿ� ID�� �߰��ȴ�.

    // ���� ��� �÷��̾��� ��� �ڿ� �� ������� IP�� �߰��ȴ�.
	*/
};
//-------------------------------------------------------------------------------------//

//--------------------- �� ������ �ִ� ĳ���͵��� ���� ����------------------------------//
struct	OnResAroundCharInfoMsg
{
	DWORD						dwMsgType;
	UI16						uiCharNum;
	// �ڿ� ĳ���͵��� ID
};
//-------------------------------------------------------------------------------------//


//----------------------------- ����, ���������� �޽���.------------------------------//
struct	OnReqEnterInnMsg
{
	DWORD						dwMsgType;
};

struct	OnResEnterInnMsg
{
	DWORD						dwMsgType;			
};
//-------------------------------------------------------------------------------------//


//-------------------------- ����, ����, ��ȭ�濡���� �޽���.--------------------------//
struct	OnReqEnterWaitRoomMsg
{
	DWORD						dwMsgType;
};

struct	OnResEnterWaitRoomMsg
{
	DWORD						dwMsgType;		
	DWORD						dwResponse;
};

struct	OnResEnterWaitRoomOkMsg
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
	UI16						uiTotalChatRoomNum;

	// ���� ������ uiTotalChatRoom(OnInnChatRoomInfo) ��ŭ ����´�.
	
	// ���� ID�� uiTotalChatRoom(OnInnChatRoomInfo.uiChatRoomNameStringLength)��ŭ ����´�.
};

// ���ǿ��� �޽���
struct	OnReqWaitRoomChangeInfoMsg
{
	DWORD						dwMsgType;
	DWORD						dwChangeInfoKind;
};

struct	ReqLeaveInnMsg
{
	DWORD						dwMsgType;
};

struct	ResLeaveInnMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqCreateChatRoomMsg
{
	DWORD						dwMsgType;
	UI08						uiStringLength;

	// �ڿ��� �������� ����´�.
};

struct	OnResCreateChatRoomMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqJoinVilInnChatRoomMsg
{
	DWORD						dwMsgType;
	UI16						uiChatRoomID;
};

struct	OnResJoinVilInnChatRoomMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResJoinVilInnChatRoomOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI32						uiPlayerNum;
	DWORD						dwUniqueID;
	// �ڿ��� �÷��̾���� ������ �´�.(OnVilINNChatPlayerInfo ��)
};

struct	OnReqLeaveVilInnChatRoomMsg
{
	DWORD						dwMsgType;
};

struct	OnResLeaveVilInnChatRoomMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResLeaveVilInnChatRoomOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiTotalChatRoomNum;

	// ���� ������ uiTotalChatRoom(OnInnChatRoomInfo) ��ŭ ����´�.
	
	// ���� ID�� uiTotalChatRoom(OnInnChatRoomInfo.uiChatRoomNameStringLength)��ŭ ����´�.
};

struct	OnVilInnChatRoomJoinMsg
{
	DWORD						dwMsgType;
	OnVilINNChatPlayerInfo		JoinPlayerInfo;
};

struct	OnVilInnChatRoomLeaveMsg
{
	DWORD						dwMsgType;
	UI16						uiPlayerUniqueID;
};

struct	OnVilInnChatRoomChatting
{
	DWORD						dwMsgType;
	DWORD						dwIP;
	char						m_ID[32];
	char						m_Chatting[256];
};
//-------------------------------------------------------------------------------------//

//--------------------- ����, ����, �Խ��ǿ����� �޽���.------------------------------//
// �Խ��ǿ� ���� �޽���
struct	OnReqEnterNoticeBoardMsg
{
	DWORD						dwMsgType;
	DWORD						dwNoticeBoardKind;	
};

struct	OnResEnterNoticeBoardMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResEnterNoticeBoardOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	

	DWORD						dwTotalPageNum;
};

struct	OnReqNoticesListMsg
{
	DWORD						dwMsgType;
	UI16						uiPage;
};

struct	OnResNoticesListMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwTotalPageNum;
};

struct	OnResNoticesListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	DWORD						dwTotalPageNum;

	UI08						uiNoticesNum;			// �Խù� �� ��.

	// �ڿ��� uiNoticesNum(OnInnNBNoticeInfo) ��ŭ�� ���� ����´�.
};

struct	OnReqRegistNoticeMsg
{
	DWORD						dwMsgType;
	
	UI08						uiTitleStringLength;
	UI16						uiNoticeStringLength;
};

struct	OnResRegistNoticeMsg
{
	DWORD						dwMsgType;
	UI08						uiResponse;
};

struct	OnReqDeleteNoticeMsg
{
	DWORD						dwMsgType;
	DWORD						dwPageNum;
	UI16						uiNoticeIndex;	
};

struct	OnResDeleteNoticeMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResDeleteNoticeOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
	DWORD						dwTotalPageNum;	
};


struct	OnReqSearchNoticeMsg
{
	DWORD						dwMsgType;
	UI08						uiSearchKind;
	UI08						uiStringLength;
};

struct	OnResSearchNoticeMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnResSearchNoticeOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwTotalPageNum;							// ��ü ������ ��.
	DWORD						dwSearchedNoticesNum;					// ��ü �Խù� ��.
};

struct	OnReqViewNoticeMsg
{
	DWORD						dwMsgType;	
	UI16						uiNoticeIndex;	
};

struct	OnResViewNoticeMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResViewNoticeOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	UI16						uiLength;
};

struct	OnReqModifyNoticeMsg
{	
	DWORD						dwMsgType;					// ON_REQUEST_MODIFY_NOTICE

	DWORD						dwNoticeNum;				// �����Ϸ��� ���� �ε���
	
	UI08						uiTitleStringLength;		// ������ ��Ʈ�� ����
	UI16						uiNoticeStringLength;		// ���빰�� ��Ʈ�� ����

	// ������ uiTitleStringLength ����Ʈ ��ŭ ������ �ڵ���´�.

	// ���ο� ���� uiNoticeStringLength ����Ʈ ��ŭ ���빰�� �ڵ���´�.
};

struct	OnResModifyNoticeMsg
{
	DWORD						dwMsgType;					// ON_RESPONSE_MODIFY_NOTICE
	UI08						uiResponse;					// ON_RET_OK : ����, ON_RET_NO : ����
};

// ���� ���
struct	OnReqInnWBEnter
{
	DWORD						dwMsgType;	
};

struct	OnResInnWBEnter
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResInnWBEnterOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI16						dwBuyWorkerListCount;

	// �ڿ� ���� ����(OnInnBuyWorkerInfo)�� dwBuyWorkerListCount��ŭ �ִ�.
};

struct	OnReqInnWBBuyWorker
{
	DWORD						dwMsgType;
	SI32						dwBuyWorkID;
	BYTE						szName[ON_ID_LENGTH + 1];
};

struct	OnResInnWBBuyWorker
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResInnWBBuyWorkerOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnFollowerInfo				FollowerInfo;

};

//-------------------------------------------------------------------------------------//
//--------------------------------- �Ʒü� ���� �޽��� --------------------------------//
struct	OnReqBarrackGetList
{
	DWORD						dwMsgType;
};

struct	OnResBarrackGetList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBarrackGetListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwBuySolderDiscountPercent;
	SI32						dwBuySolderCount;

	// �ڿ� �뺴 ����(OnInnBuySolderInfo)�� dwBuySolderCount��ŭ �ִ�.
};

struct	OnReqBarrackBuySolder
{
	DWORD						dwMsgType;
	SI32						dwBuySolderID;
	BYTE						szName[ON_ID_LENGTH + 1];
};

struct	OnResBarrackBuySolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBarrackBuySolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnFollowerInfo				FollowerInfo;
};

struct	OnReqBarrackGetDismissalList
{
	DWORD						dwMsgType;
};

struct	OnResBarrackGetDismissalList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBarrackGetDismissalListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwDismissalSolderCount;

	// �ڿ� �뺴 ����(OnDismissalSolderInfo)�� dwDismissalSolderCount��ŭ �ִ�.
};

struct	OnReqBarrackDismissalSolder
{
	DWORD						dwMsgType;
	SI32						dwSlot;
};

struct	OnResBarrackDismissalSolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResBarrackDismissalSolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;
	UI08						Slot;
};

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------2�� ���� ��� �缺�� ���� �޽��� ---------------------------------//
//-------------------------------------------------------------------------------------------------------//
/*
struct	OnReqGBarrackGeneralExGetList
{
	DWORD						dwMsgType;
};

struct  OnResGBarrackGeneralExGetList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGeneralExGetListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwBuyGeneralExDiscountPercent;		//�̰� ������ ������.--;
	SI32						dwBuyGeneralExCount;

	// �ڿ� �뺴 ����(OnInnBuySolderInfo)�� dwBuyGeneralExCount��ŭ �ִ�.
};

struct	OnReqGBarrackBuyGeneralEx
{
	DWORD						dwMsgType;
	SI32						dwBuyGeneralExID;
	BYTE						szName[ON_ID_LENGTH + 1];
};

struct	OnResGBarrackBuyGeneralEx
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackBuyGeneralExOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnFollowerInfo				FollowerInfo;
};

struct	OnReqGBarrackGetChangeGeneralExList
{
	DWORD						dwMsgType;
};

struct	OnResGBarrackGetChangeGeneralExList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGetChangeGeneralExListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwChangeGeneralExDiscountPercent;
	SI32						dwChangeGeneralExCount;

	// �ڿ� �뺴 ����(OnChangeJobSolderInfo)�� dwChangeGeneralExCount��ŭ �ִ�.
};

struct	OnReqGBarrackBuyChangeGeneralEx
{
	DWORD						dwMsgType;
	SI32						dwChangeGeneralExSlot;
	SI32						dwChangeGeneralExKind;
};

struct	OnResGBarrackBuyChangeGeneralEx
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackBuyChangeGeneralExOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnChangeJobFollowerInfo		FollowerInfo;
};

struct	OnReqGBarrackGetDismissalGeneralExList
{
	DWORD						dwMsgType;
};

struct	OnResGBarrackGetDismissalGeneralExList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGetDismissalGeneralExListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwDismissalGeneralExCount;

	// �ڿ� �뺴 ����(OnDismissalSolderInfo)�� dwDismissalGeneralExCount ��ŭ �ִ�.
};

struct	OnReqGBarrackSellDismissalGeneralEx
{
	DWORD						dwMsgType;
	SI32						dwSlot;
};

struct	OnResGBarrackSellDismissalGeneralEx
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackSellDismissalGeneralExOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;
	UI08						Slot;
};
*/

//-------------------------------------------------------------------------------------//
//--------------------------------- ����缺�� ���� �޽��� --------------------------------//
struct	OnReqGBarrackGetList
{
	DWORD						dwMsgType;
};

struct	OnResGBarrackGetList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGetListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwBuySolderDiscountPercent;
	SI32						dwBuySolderCount;

	// �ڿ� �뺴 ����(OnInnBuySolderInfo)�� dwBuySolderCount��ŭ �ִ�.
};

struct	OnReqGBarrackBuySolder
{
	DWORD						dwMsgType;
	SI32						dwBuySolderID;
	BYTE						szName[ON_ID_LENGTH + 1];
};

struct	OnResGBarrackBuySolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackBuySolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnFollowerInfo				FollowerInfo;
};

struct	OnReqGBarrackGetChangeJobList
{
	DWORD						dwMsgType;
};

struct	OnResGBarrackGetChangeJobList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGetChangeJobListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwChangeJobSolderDiscountPercent;
	SI32						dwChangeJobSolderCount;

	// �ڿ� �뺴 ����(OnChangeJobSolderInfo)�� dwChangeJobSolderCount��ŭ �ִ�.
};

struct	OnReqGBarrackChangeJobSolder
{
	DWORD						dwMsgType;
	SI32						dwChangeJobSlot;
	SI32						dwChangeJobKind;
};

struct	OnResGBarrackChangeJobSolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackChangeJobSolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnChangeJobFollowerInfo		FollowerInfo;
};

struct	OnReqGBarrackGetDismissalList
{
	DWORD						dwMsgType;
};

struct	OnResGBarrackGetDismissalList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackGetDismissalListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwDismissalSolderCount;

	// �ڿ� �뺴 ����(OnDismissalSolderInfo)�� dwDismissalSolderCount��ŭ �ִ�.
};

struct	OnReqGBarrackDismissalSolder
{
	DWORD						dwMsgType;
	SI32						dwSlot;
};

struct	OnResGBarrackDismissalSolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResGBarrackDismissalSolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;
	UI08						Slot;
};

//-------------------------------------------------------------------------------------//
//--------------------------------- ���ü� ���� �޽��� --------------------------------//
struct	OnReqMBarrackGetList
{
	DWORD						dwMsgType;
};

struct	OnResMBarrackGetList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResMBarrackGetListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwBuyMonsterDiscountPercent;
	SI32						dwBuyMonsterCount;

	// �ڿ� ���� ����(OnInnBuySolderInfo)�� dwBuyMonsterListCount��ŭ �ִ�.
};

struct	OnReqMBarrackBuyMonster
{
	DWORD						dwMsgType;
	SI32						dwBuyMonsterID;
	BYTE						szName[ON_ID_LENGTH + 1];
};

struct	OnResMBarrackBuyMonster
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResMBarrackBuyMonsterOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	UI08						Slot;
	OnFollowerInfo				FollowerInfo;
};

struct	OnReqMBarrackGetDismissList
{
	DWORD						dwMsgType;
};

struct	OnResMBarrackGetDismissList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResMBarrackGetDismissListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwDismissMonsterCount;

	// �ڿ� ���� ����(OnDismissalSolderInfo)�� dwDismissMonsterCount��ŭ �ִ�.
};

struct	OnReqMBarrackDismissMonster
{
	DWORD						dwMsgType;
	SI32						dwSlot;
};

struct	OnResMBarrackDismissMonster
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResMBarrackDismissMonsterOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;
	UI08						Slot;
};

//-------------------------------------------------------------------------------------//
//--------------------------------- ġ��� ���� �޽��� --------------------------------//
struct	OnReqHospitalGetCureList
{
	DWORD						dwMsgType;
};

struct	OnResHospitalGetCureList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResHospitalGetCureListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						dwDiscount;
	SI32						dwCureSolderCount;

	// �ڿ� �뺴 Slot(UI08)�� dwCureWorkerListCount��ŭ �ִ�.
	// �ڿ� �뺴 ġ�ᰡ��(MONEY)�� dwCureWorkerListCount��ŭ �ִ�.
	// �ڿ� �뺴 ��Ȱ����(MONEY)�� dwCureWorkerListCount��ŭ �ִ�.(ü���� 0�̸� ��Ȱ������ ������, �� ������ ������ ġ�Ḧ �Ҽ� ����.)
};

struct	OnReqHospitalCureSolder
{
	DWORD						dwMsgType;
	UI08						dwCureSolderSlot;
};

struct	OnResHospitalCureSolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResHospitalCureSolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	UI08						dwCureSolderSlot;
	MONEY						Money;

	SI32						siLife;						// Life
	SI32						siMana;						// Mana
};

struct	OnReqHospitalCureAllSolder
{
	DWORD						dwMsgType;
};

struct	OnResHospitalCureAllSolder
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResHospitalCureAllSolderOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						Money;

	SI16						CureSolderCount;
	strCureSolder				CureSolder[ON_MAX_FOLLOWER_NUM];
};

//-------------------------------------------------------------------------------------//
//--------------------------------- �ε� ���� �޽��� ----------------------------------//
struct	OnReqWharf_Enter							// �εο� ����.
{
	DWORD						dwMsgType;
};

struct	OnResWharf_Enter							// �εο� ����.(����)(Ticket List�� �ٷ� ChangeTicketList �޼����� ������ ó���Ѵ�.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// ���� �ڽ��� ���� Ticket
	SI32						siTicketKind;
};

struct	OnResWharf_ChangeTicketList					// ǥ�� List�� ������Ʈ �Ѵ�.(�����ð����� ������ ����.)
{
	DWORD						dwMsgType;

	SI32						siTicketCount;
	SI32						siDiscountPercent;
	// �ڿ� TicketInfo(OnTicketInfo)�� siTicketCount��ŭ �پ �´�.
	//         .
	//         .
	//         .
};

struct	OnReqWharf_BuyTicket						// �εο��� ǥ�� ���.
{
	DWORD						dwMsgType;

	SI32						siTicketKind;
};

struct	OnResWharf_BuyTicket						// �εο��� ǥ�� ���.(����)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						NowMoney;

	// ���� �ڽ��� ���� Ticket
	SI32						siTicketKind;
};

struct	OnReqWharf_Exit								// �εο��� ������.
{
	DWORD						dwMsgType;
};

struct	OnResWharf_Exit								// �εο��� ������.(����)(Ticket List�� �ٷ� ChangeTicketList �޼����� ������ ó���Ѵ�.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqWharfWaitRoom_Enter					// �εδ��ǿ� ����.
{
	DWORD						dwMsgType;
};

struct	OnResWharfWaitRoom_Enter					// �εδ��ǿ� ����.(����)(���� ��� �Ұ����� Ticket�̶�� ������ �Ѵ�.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// ���� �ڽ��� ���� Ticket
	SI32						siTicketKind;
};

struct	OnReqWharfWaitRoom_Exit						// �εδ��ǿ��� ������.(���⼭ �����ٴ� ������ ���ٴ°��̴�. �ε� ǥ��°����� �̵��ϴ°��� Wharf Enter�� ó���Ѵ�.)
{
	DWORD						dwMsgType;
};

struct	OnResWharfWaitRoom_Exit						// �εδ��ǿ��� ������.(����)(���� ��� �Ұ����� Ticket�̶�� ������ �Ѵ�.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResWharfWaitRoom_MoveInShip				// �εδ��ǿ��� �¼��ϴ�.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

//-------------------------------------------------------------------------------------//
//--------------------------------- ���� ���� �޽��� ------------------------------//
struct	OnReqEnterMarketMsg
{
	DWORD						dwMsgType;	
};

struct	OnResEnterMarketMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqLeaveMarketMsg
{
	DWORD						dwMsgType;	
};

struct	OnResLeaveMarketMsg
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
};
//-------------------------------------------------------------------------------------//

//-----------------------------��û���� ������ �̵� -----------------------------------//
typedef struct
{
	DWORD						dwMsgType;
} OnReqEnterGovernment;

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
} OnResEnterGovernment;

typedef struct
{
	DWORD						dwMsgType;	
} OnReqLeaveGovernment;

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
} OnResLeaveGovernment;

typedef struct
{
	DWORD						dwMsgType;	

	SI16						m_Type;				// �� ����â ����
} OnReqGovernmentEnterInvestment; 

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;

	// ���� ����
	SI16						m_Type;						// �� ����â ����
	SI32						m_InvestmentMinPercent;		// �ּ� ���� Percent
	SI32						m_InvestmentMaxPercent;		// �ִ� ���� Percent
	SI32						m_MyInvestmentCanTime;      // �����Ҽ��ִ� ��¥
} OnResGovernmentEnterInvestment; 

typedef struct
{
	DWORD						dwMsgType;	
} OnReqGovernmentLeaveInvestment;
 
typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
} OnResGovernmentLeaveInvestment;

typedef struct
{
	DWORD						dwMsgType;	

//	SI16						m_Type;				// �� ����â ����
	SI16						siVillageCode;

} OnReqGovernmentGetInvestmentList;

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;

	CHAR						m_MaxInvestmentID[21];		// �ְ� ������ ID

	SI16						m_Type;						// �� ����â ����

	MONEY						m_GainingsMoney;			// ���� ����
	BYTE						m_fGetOut;					// ���ڱ� ȸ�� Flag
	BYTE						m_GetOutTex;				// ���ڱ� ȸ���� ���� Percent

	SI16						m_InvestmentListCount;		// ������ ����� ��

	SI32						m_MyInvestmentCanTime;		// �ڽ��� �����Ҽ� �ִ� �ð�(�� �ð��� 0�̸� ���� ����)
	// �ڿ� OnInvestmentInfo ������ m_InvestmentListCount ��ŭ �ٴ´�.
} OnResGovernmentGetInvestmentList;

typedef struct
{
	DWORD						dwMsgType;	

//	SI16						m_Type;						// ������ ����
	MONEY						m_InvestmentMoney;			// ���� �ݾ�
	MONEY						m_InvestmentSaveMoney;
} OnReqGovernmentSetInvestment;

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;
} OnResGovernmentSetInvestment;

typedef struct
{
	DWORD						dwMsgType;	
} OnReqGovernmentVillageInfo;

typedef struct
{
	DWORD						dwMsgType;	

	char						m_VillageInfo_Descript[256];
	SI32						m_VillageInfo_InOutPlayerCount;
	VDEF						CurDefencePower;					// ���� ����
	VDEF						MaxDefencePower;					// �ִ� ����
} OnResGovernmentVillageInfo;

struct	OnReqProfitStatistics	//	���� ���� �ֱ� ��� ��û
{
	DWORD						dwMsgType;	
};

struct	OnResProfitStatistics	//	���� ���� �ֱ� ��� ����
{
	DWORD						dwMsgType;
	SI08						siNum;
	// �ڿ��� siNum ��ŭ stCapital ����ü�� �´�.
};

struct	OnReqGovernmentInvestmentGetOut		// ��û, �����ѵ� ������ ������
{
	DWORD						dwMsgType;

	MONEY						GetOutMoney;
};

struct	OnResGovernmentInvestmentGetOut		// ��û, �����ѵ� ������ ������ ����
{
	DWORD						dwMsgType;

	BYTE						dwSuccess;			// ��������. 0 : ����, 1 : ���ڱ��� ����, 2 : �����ڸ� ��ã��

	MONEY						GetOutMoney;
	MONEY						Tex;
};

//-------------------------------------------------------------------------------------//

//-----------------------------�κ��丮���� ������ �̵� -------------------------------//

struct	OnResMoveItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqShareItemMsg									//	�������� �ѷ� �ɰ���.
{
	DWORD						dwMsgType;	
	UI16						uiItemID;	
	UI16						uiItemQuantity;
	
	UI08						uiFollowerID;						
	UI08						uiReceiverID;
	UI16						uiNewPosInInventory;	
};

struct	OnResShareItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
//-------------------------------------------------------------------------------------//

//---------------------------------  ����Ʈ ����  -------------------------------------//
struct	OnReqQuestSelectNPC
{
	DWORD		dwMsgType;

	SI16		siNPCCode;
	SI32		siBookCount;
};

struct	OnResQuestSelectNPC
{
	DWORD		dwMsgType;
	DWORD		dwResponse;

	SI16		m_QuestCode;
};

struct	OnResQuestSelectNPC_EndQuest
{
	DWORD			dwMsgType;
	DWORD			dwResponse;

	SI16			m_QuestCode;
	MONEY			m_DeleteMoney;
	SI16			m_DeleteItemCount;

	// ��
	SQuestPayData	m_QuestPay[5];

	// �ڿ� Delete �ϴ� Item ������ �ٴ´�.(MercenaryItemData Type)
};

struct	OnReqQuestAcceptQuest
{
	DWORD		dwMsgType;

	SI16		m_QuestCode;
};

struct	OnResQuestAcceptQuest
{
	DWORD		dwMsgType;
	DWORD		dwResponse;

	SI16		m_QuestCode;
};

struct	OnReqQuestCancelQuest
{
	DWORD		dwMsgType;

	SI16		m_QuestCode;
};

struct	OnResQuestCancelQuest
{
	DWORD		dwMsgType;
	DWORD		dwResponse;

	SI16		m_QuestCode;
};

struct	OnReqQuestGetPlayQuest
{
	DWORD		dwMsgType;
};

struct	OnResQuestGetPlayQuest
{
	DWORD		dwMsgType;

	SI16		m_PlayingQuestCodeCount;
	SI16		m_CompletedQuestListCount;
};

struct	OnResQuestGetPlayQuestData
{
	DWORD			dwMsgType;

	SPlayingQuest	m_PlayingQuestData;
};

struct	OnResQuestAllClearQuest
{
	DWORD			dwMsgType;
};
//-------------------------------------------------------------------------------------//

struct	OnReqWearItemInfoMsg	// �ʱ� ����
{	
	DWORD						dwMsgType;
	UI08						uiFollowerID;
};

struct	OnResWearItemInfoMsg
{
	DWORD						dwMsgType;		
	UI16						uiHelmetID;				//	���� 
	UI16						uiWeaponID;				//	����
	UI16						uiArmorID;				//	����
	UI16						uiBeltID;				//	�㸮��.
	UI16						uiGloveID;				//	�尩
	UI16						uiShoesID;				//	�Ź�
	UI16						uiRRingID;				//	������ ����
	UI16						uiLRingID;				//	�޼� ����.
	UI16						uiGuardianID;				//  ��ȣ��.
};

struct	OnReqStripItemMsg	//	����
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;	
	UI08						uiWearLoc;			//	������ ��ġ (0~7)
	UI16						uiItemID;
	UI16						uiPosInventory;		// ���� ���� �������� ���� ��ġ.
};

struct	OnResStripItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqWearItemMsg	//	����
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI08						uiWearLoc;			//	�����ϴ� ��ġ (0~8)
	UI16						uiItemID;
	UI16						uiPosInventory;		// ���� ������ �������� �ִ� ��ġ.
};

struct	OnResWearItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnChangeClothesMsg		//	�ֺ� ��� ���� �ٲ����� �޼���
{
	DWORD						dwMSgType;
	UI16						uiCharacID;				//	���� �ٲ� ���� ����� ���̵�.
	UI16						uiItemID[WEAR_ITEM_NUM];			//	����, ����, ������ ������ �ڵ�
};

struct	OnChangeCharacterCodeMsg //	�ֺ� ��� ĳ�����ڵ尡 �ٲ����� �޽���
{
	DWORD						dwMSgType;
	UI16						uiCharacID;				//	���� �ٲ� ���� ����� ���̵�.
	UI16						uiNewCharCode;			//	���ο� ĳ���� �ڵ�.
};

struct	OnChangeCharacterNationMsg // ������ �ٲ����� �޽���
{
	DWORD						dwMSgType;	
	UI16						uiNewCharCode;			//	���ο� ���� �ڵ�.
};

struct OnImperiumUseMsg
{
	DWORD						dwMsgType;
	UI16						uiUniqueID;
	UI16						uiKind;
	SI16						siHp;
	SI16						siSupply;
	BOOL						bCaptain;
	UI16						uiPrevKind;
};

struct	OnReqReplaceItemMsg	//	�ٲٱ�:	����+����
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiItemID;
	UI16						uiDropPosInventory;	//	���� ���� �������� ���� ��ġ.
	UI16						uiPickPosInventory;	//	���� ������ �������� �ִ� ��ġ.
};

struct	OnResReplaceItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
//-------------------------------------------------------------------------------------//

//----------------------------- �ʵ� ���� �޽���-------------------------------------//
struct	OnTargetMeMsg		//	���Ͱ� �ڽ��� �븱�� �޼���
{
	DWORD						dwMsgType;
	UI16						uiMonsterID;
};

struct	OnReleaseMeMsg		//	���Ͱ� �� �븮�� ������ �޼���
{
	DWORD						dwMsgType;
	UI16						uiMonsterID;
};
//-------------------------------------------------------------------------------------//

//-----------------------------  �ð� ���� �޽��� -------------------------------------//
struct	OnChangedTimeMsg
{
	DWORD						dwMsgType;
	SOnlineTime					Time;
	OnRealWorldTime				RealWorldTime;
};
//-------------------------------------------------------------------------------------//


//-----------------------------  ���� ���� �޽��� -------------------------------------//
struct	OnReqMoveMapMsg
{
	DWORD						dwMsgType;
	UI16						uiPortalID;					// ������ Potal�� ID
};

struct	OnResMoveMapMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResMoveMapOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	
	MONEY						mnMoney;					// ���ο� ��
	UI16						uiAroundCharNum;			// �� ������ �÷��̾� ��.	
	UI16						uiAroundItemNum;			// �� ������ ���� ������ ������ ��. 
	//  �÷��̾�, ���� ������ �������� ���� ������ �ִ�.
};
//-------------------------------------------------------------------------------------//

//-----------------------------  �� ž�� �� ���� �޽��� -------------------------------//
struct	OnBoardShipMsg
{
	DWORD						dwMsgType;
	UI16						uiAccount;					// ž���� ���� ID	

	UI16						uiAroundCharNum;			// �� ������ �÷��̾� ��.	
	UI16						uiAroundItemNum;			// �� ������ ���� ������ ������ ��. 
	//  �÷��̾�, ���� ������ �������� ���� ������ �ִ�.
};

struct	OnGetOutShipMsg
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;			// ������ ID
	OnRecvVillageInfo			VillageInfo;			// ������ ����
};	

struct	OnMoveShipMsg
{
	DWORD						dwMsgType;
	UI16						uiX;
	UI16						uiY;
};

//-------------------------------------------------------------------------------------//

// ���ʽ��� ����Ͽ� �ɷ�ġ�� ������Ű�� �޽���
struct	OnReqIncAbilityByBonusMsg
{
	DWORD						dwMsgType;
	AbilityKindType				AbilityKind;		// AbilityKindType ���� �ϳ����� �Ѵ�.		
	UI08						uiFollowerID;		// Bonus�� ����ϴ� ĳ������ ����.
};

struct	OnResIncAbilityByBonusMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// ���ʽ��� ����Ͽ� �ɷ�ġ�� ������Ű�� �޽���

// ��� ��� ���� �޽���
struct	OnCommandMsg
{
	DWORD		dwMsgType;
	SI32		siStringLength;		// ��� ���ڿ��� ����

	// �ڿ��� CHAR������ ���ڿ��� ����´�.
};
// ��� ��� ���� �޽���

// ���
struct	OnReqCreateGuildMsg			// ����� �����Ѵ�.(�䱸)
{
	DWORD		dwMsgType;			// ON_REQUEST_CREATE_GUILD
	UI08		uiGuildNameLength;	// ��� �̸� ���ڿ��� ����

	// �ڿ��� ����� �̸��� �Ѿ�´�.
};

struct	OnResCreateGuildMsg			// ����� �����Ѵ�.(����)
{
	DWORD		dwMsgType;			// ON_RESPONSE_CREATE_GUILD
	DWORD		dwResponse;			// ����. ON_RET_OK : ����, ON_RET_NO : ����
};

struct	OnReqDeleteGuildMsg			// ����� �����Ѵ�.(�䱸)
{
	DWORD		dwMsgType;			// ON_REQUEST_DELETE_GUILD
};

struct	OnResDeleteGuildMsg			// ����� �����Ѵ�.(����)
{
	DWORD		dwMsgType;			// ON_RESPONSE_DELETE_GUILD
	DWORD		dwResponse;			// ����. ON_RET_OK : ����, ON_RET_NO : ����
};

struct	OnReqJoinLeaveGuildMsg		// ��ܿ� ����/Ż���Ѵ�.(�䱸)
{
	DWORD		dwMsgType;			// ON_REQUEST_JOIN_LEAVE_GUILD
};

struct	OnResJoinLeaveGuildMsg		// ��ܿ� ����/Ż���Ѵ�.(����)
{
	DWORD		dwMsgType;			// ON_RESPONSE_JOIN_LEAVE_GUILD
	DWORD		dwResponse;			// ����. 
};

struct	OnReqGuildWaitingJoinMembersListMsg		// ��ܿ� �����ϱ� ���ϴ� ����������� ����Ʈ�� ���´�.(�䱸)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST	

	DWORD		dwPage;							// �� ������ �߿��� �䱸�ϴ� ������
};

struct	OnResGuildWaitingJoinMembersListMsg		// ��ܿ� �����ϱ� ���ϴ� ����������� ����Ʈ�� ���´�.(����)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST
	DWORD		dwResponse;						// ����. ON_RET_OK : ����, ON_RET_NO : ����

	DWORD		dwTotalCandidateListPageNum;	// �� �������� ������ ��
	DWORD		dwNowCandidateListPageNum;		// ���� �������� ������ ��
	UI16		uiCandidateMembersNum;			// �̹��� ������ �������� ��	

	// strGuildWaitingJoinMemberInfo(OnlineCommon.h ����)�� uiWaitingJoinMembersNum�� ����ŭ ������ �ִ�.
};

struct	OnReqJoinGuildOkNoMsg					// ��ܿ� �����ϱ� ���ϴ� ���� ����ڿ��� OK, NO�� ������.(�䱸)
{
	DWORD		dwMsgType;						// ON_REQUEST_JOINGUILD_OKNO(�䱸)
	DWORD		dwResponse;						// ON_RET_OK or ON_RET_NO 
	CHAR		szID[ON_ID_LENGTH];				// �ش� ĳ������ ID
};

struct	OnResJoinGuildOkNoMsg					// ��ܿ� �����ϱ� ���ϴ� ���� ����ڿ��� OK, NO�� ������.(����)
{
	DWORD		dwMsgType;						// ON_RESPONSE_JOINGUILD_OKNO(����)
	DWORD		dwResponse;						// ����. ON_RET_OK : ����, ON_RET_NO : ����(������ Ȯ���� ��� ON_RET_NO_GLD_ADMIT_FULL, ON_RET_NO_GLD_ADMIT_MAXINVESTOR)
};

struct	OnReqDismissalMsg						// ��ܿ��� �ذ��Ѵ�.(�䱸)
{
	DWORD		dwMsgType;						// ON_REQUEST_DISMISSAL
	CHAR		szID[ON_ID_LENGTH];				// �ش� ĳ������ ID
};

struct	OnResDismissalMsg						// ��ܿ��� �ذ��Ѵ�.(����)
{
	DWORD		dwMsgType;						// ON_RESPONSE_DISMISSAL
	DWORD		dwResponse;						// ����. ON_RET_OK : ����, ON_RET_NO : ����
};

struct	OnReqGuildListMsg						// ����� ����Ʈ�� �䱸�Ѵ�.(�䱸)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_LIST
};

struct	OnResGuildListMsg						// ����� ����Ʈ�� �䱸�Ѵ�.(�䱸)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_LIST
	UI32		uiGuildNum;						// �� ����� ��.	

	// strGuildInfo(OnlineCommon.h ����)�� uiGuildNum�� ����ŭ ������ �ִ�.
};

struct	OnReqGuildDetailInfoMsg
{
	DWORD		dwMsgType;							// ON_REQUEST_GUILD_DETAIL_INFO
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];		// ����� ID
};

struct	OnResGuildDetailInfoMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_DETAIL_INFO
	DWORD		dwResponse;						// ON_RET_OK : ����, ON_RET_NO : ����
	
	UI16		uiOwnVillageNum;				// �����ϰ� �ִ� ������ ��.	

	// �����ϰ� �ִ� �������� ����(OnGuildDetailInfo)�� uiOwnVillageNum ��ŭ �ڵ���´�.
};

struct	OnReqGuildMembersListMsg				// ��ܿ� ����Ʈ�� ���´�.(�䱸)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_MEMBERS_LIST
	UI32		uiGuidlID;						// ����Ʈ�� ���ϴ� ����� ID
	DWORD		dwPage;							// �� ������ �߿��� �䱸�ϴ� ������
};

struct	OnResGuildMembersListMsg				// ��ܿ� ����Ʈ�� ���´�.(����)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_MEMBERS_LIST	
	DWORD		dwResponse;						// ����. ON_RET_OK : ����, ON_RET_NO : ����
	DWORD		dwTotalMemberListPageNum;		// ��ü ������ ��
	DWORD		dwNowMemberListPageNum;			// ���� ������ ��
	UI32		uiMaxGuildMemebersNum;			// �� ����� �ִ� ��ܿ� ��
	UI32		uiGuildMemebersNum;				// �̹� ���������� ������ ��ܿ� ��			

	// strGuildMemeberInfo(OnlineCommon.h ����)�� uiGuildMemebersNum�� ����ŭ ������ �ִ�.
};

struct  OnResAuditGuildMemberListMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_AUDIT_GUILD_MEMBER_LIST
	DWORD		dwResponse;						// ����. ON_RET_OK : ����, ON_RET_NO : ����
	UI32		uiGuildMembersNum;				// ������ ��ܿ� ��			

	//strAuditGuildMemberInfo �� uiGuildMembersNum��ŭ �´�.
};

struct	OnGuildMsg
{
	DWORD		dwMsgType;						// ON_GUILD_MSG
	DWORD		dwResponse;		
};

struct	OnReqGuildSetRelationMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_SET_RELATION
	DWORD		dwResponse;						// ���� : ON_RET_GLD_ALLY, �� : ON_RET_GLD_ENEMY, �߸� : ON_RET_GLD_NEUTRAL
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];	// ����� ID
};



struct	OnReqGuildChangeFlag
{
	DWORD		dwMsgType;						// ON_REQUEST_CHANGE_GUILD_FLAG
	SI32		siFlag;							// ������ ���.
};

struct	OnResGuildChangeFlag
{
	DWORD		dwMsgType;						// ON_RESPONSE_CHANGE_GUILD_FLAG
	DWORD		dwResponse;						// ����	
	SI32		siFlag;							// ����� ���.
};

struct	OnReqMaxVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT
};

struct	OnResMaxVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT
	DWORD		dwResponse;						// ����

	VDEF		NowDef;							// ���� ����
	VDEF		MaxDef;							// �ִ� ����
	MONEY		DefPerMoney;					// ���� 1�� ��
	MONEY		MaxMoney;						// ���� ������ �ְ� ��
};

struct	OnReqSetVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT

	MONEY		InvestmentMoney;				// �����ѵ�
};

struct	OnResSetVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT
	DWORD		dwResponse;						// ����

	VDEF		NowDef;							// ���� ����
};

typedef	OnMsg	OnReqUsedFlagList;				// dwMsgType = ON_REQUEST_USED_FLAG_LIST	

struct	OnResUsedFlagList
{
	DWORD		dwMsgType;						// ON_RESPONSE_USED_FLAG_LIST	
	SI32		siUsedFlagNum;					// ���� ����� ��.
	UI32		uiGuildMemberNum;				// ��ܿ� ��.

	// strFlagInfo(OnlineCommon.h ����)�� siUsedFlagNum��ŭ �� ����´�.
};
// ���

// ����� 
struct	OnHugryMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HEALTH : Health�� ���϶�, ON_RET_HUR_CHANGE_HP : HP�� ���϶�.(�̶� Health�� 0��)
												// ON_RET_HUR_CHANGE_HEALTH�� ��� OnHugryChangeHealthMsg�� Cast.
												// ON_RET_HUR_CHANGE_HP�� ��� OnHugryChangeHPMsg�� Cast.
};

struct	OnHugryChangeHealthMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HEALTH : Health�� ���϶�
	SI32		siHealth;						// ���� �����(������) ��ġ.
};

struct	OnHugryChangeHPMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HP : HP�� ���϶�.(�̶� Health�� 0��)
	UI08		uiFollowerNum;					// ���ϵ��� ��.
	
	// �ڿ��� ���ϵ��� ����(strHungryFollowerInfo)�� uiFollowerNum��ŭ ����´�.
};
// ����� 

struct OnReqNormalChar
{
	DWORD	dwMsgType;
	UI16	uiUniqueID;
};

struct OnResNormalChar
{
	DWORD	dwMsgType;
	UI16	uiUniqueID;
	UI16	uiCharCode;
};

struct OnReqWarWarp
{
	DWORD		dwMsgType;
	UI16		uiUniqueID;			// �����ؼ� ������ �Ϲ����� ���̵�.
};

//���� Ÿ�Լ�����.
struct OnReqFieldAttackRdyMsg
{
	DWORD		dwMsgType;		
	UI16		uiUniqueID;		//�´»���� ���̵�.
};

struct OnResFieldAttackRdyMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	DWORD       dwTime;         //���� ���н� ���� �ð�   
	UI16		uiAttackCharID;	//��������� ���̵�.
	UI16		uiEnemyCharID;  //�´»���� ���̵�.
	UI16		uiKind;
	BYTE        bIsLong;        //���Ÿ� �����ΰ�(0: �ƴϴ� 1:�´�) 
};

struct OnReqFieldAttackActionMsg
{
	DWORD		dwMsgType;		
	UI16		uiUniqueID;		//�´»���� ���̵�.
	SI16        siAttackType;	//�̳��� ���忷�� �ֳ� ����
};

struct OnResFieldAttackActionMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	UI16		uiShooterID;	//��������� ���̵�.
	UI16		uiEnemyID;		//���� ����� ���̵�.
	SI16		siDamage;
	BOOL		bAlive;
	SI16		siAttackType;	//�̳��� �����߳� ������ ����߳�.üũ
};

struct OnReqFieldAttackDeadMsg
{
	DWORD		dwMsgType;
	UI16		uiShooterID;	//��������� ���̵�.
};

struct OnResFieldAttackDeadMsg
{
	DWORD		dwMsgType;
	UI16		uiShoorterID;	//���� ����� ���̵�.
	UI16		uiDeadCharID;	//�״� ����� ���̵�.
};

struct OnResDeadPenalty
{
	DWORD		dwMsgType;
};

struct OnChangedWarEnd
{
	DWORD		dwMsgType;
	UI16		uiKind;
	UI16		uiPlayerID;			//	���� �ٲ� ���� ����� ���̵�.
};


struct OnReqLongAttackVillageRdyMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;	//���� ������ �ڵ�(ID�� �ƴϴ�.)
};

struct OnResLongAttackVillageRdyMsg  // [2/21/2005] ����
{
	DWORD		dwMsgType;      // �޼��� Ÿ��
	DWORD		dwResponse;     // ���� ����
	DWORD       dwTime;         // ���� �ð�
	UI16		uiAttackCharID;	// ��������� ���̵�.
	UI16		uiVillageCode;	// ������ �ڵ�.(ID�� �ƴϴ�)	
	UI16		uiKind;
	BOOL        bIsLong;        
};

struct OnReqLongAttackVillageActionMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;	//���� ������ �ڵ�(ID�� �ƴϴ�.)
	SI16        siAttackType;	
};


struct OnResLongAttackVillageActionMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;				//
	UI16		uiAttackCharID;				// ������ �����ϴ� �ΰ��� ���̵�.
	SI16		siVillageDefenceDamage;		// ���ݿ� ���� �پ�� ���� ����.
	SI16		siDefenceSoldierDamage;		// ���ݿ� ���� �پ�� ���� ��
	VDEF		vdDefence;					// ���� ���� ����
	SI32		siDefenceSoldier;			// ���� ���� ����.
	SI16		siPlayerDamage;				// 
	SI16		siAttackType;				//�̳��� �����߳� üũ.
};

struct OnReqGetSupplyAtGuildUnit
{
	DWORD		dwMsgType;			//
	UI16		uiTargetID;			// ���޼����� ����� AccountID
	SI16		siRequireSupply;	// ���� �ް��� �ϴ� Supply�� ��
};

//�ֺ� ����鿡�� ��� ���� �޼���. �ڱ� �ڽ��� ��츸 Supply �� �־��ְ� ������ ����� ����.
struct OnResGetSupplyAtGuildUnit   
{
	DWORD		dwMsgType;			// 
	/*UI16		uiUniqueID;			// ���޹޴� ����� AccountID
	SI16		siSupply;			// ���޹��� Supply
	UI16		uiTargetID;			// ���޼����� AccountID
	SI16		siSupplyCharge;		// ���޼����� Supply ��.
	*/
	UI16        uiUniqueID;         //��û�� ���(���޼���)
	SI16        siHeAmount;         //ġ�����ؾ�
	SI16        siNowMP;            //���� mp��
	UI16        uiTagetID;          //ġ����� ���־��̵�
	SI16        siHP;               //ġ����� ���� Hp
};

struct OnReqVillageAttackTypeCheck  //������ AttackType�� �����Ҽ� �ֳ� �˾ƺ���.(��û)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;			// ���� �ڵ�
};

struct OnResVillageAttackTypeCheck  //������ AttackType�� �����Ҽ� �ֳ� �˾ƺ���.(����)
{
	DWORD	dwMsgType;
	DWORD	dwResponse;
	UI16    uiVillageCode;
};

struct OnReqVillagePlayerOilAttack	//������ AttackType�� �⸧�������� �ٲ۴�.(��û)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
};

struct OnResVillagePlayerOilAttack	//������ AttackType�� �⸧�������� �ٲ۴�.(����)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
	DWORD	dwResponse;
	MONEY	mnMoney;
	SI32	siLimitTime;
	UI16	uiUniqueID;				
};

struct OnReqVillagePlayerStoneAttack //������ AttackType�� ���������� �ٲ۴�.(��û)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
};

struct OnResVillagePlayerStoneAttack //������ AttackType�� ���������� �ٲ۴�.(����)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
	DWORD	dwResponse;
	MONEY	mnMoney;
	SI32	siLimitTime;
	UI16	uiUniqueID;
};

struct OnReqPlayerVillageDefenceUp
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
};

struct OnResPlayerVillageDefenceUp
{
	DWORD	dwMsgType;
	DWORD	dwResponse;
	UI16	uiVillageCode;			// ���� �ڵ�.
	VDEF	vdDefenceUp;			// �ö� ���� ����.
	MONEY	mnMoney;				// �Һ��� ������ ��.
	SI32	siLimitTime;
	UI16	uiUniqueID;	
	
};

struct OnChangedVillageAttackType    // �ð��� �� �Ǿ� ���� AttackType�� ������� ������. ����->Ŭ���̾�Ʈ
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
	SI16	siAttackType;
};


struct  OnReqGuildFriendlyMessageOffer
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//���� ����� �� ����̸�.
	CHAR    szMessage[81];		
};

struct	OnResGuildFriendlyMessageOffer
{
	DWORD	dwMsgType;
	DWORD	dwResponse;			//ON_RET_OK, ON_RET_NO = ������ ���� �޼����� Ȯ������ �ʾҴ�.�޼����� ������ ����.
};

struct	OnReqGuildFriendlyMessageAccept
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//�޼����� �� ����� �̸�.	
};

struct	OnResGuildFriendlyMessageAccept
{
	DWORD	dwMsgType;
//	DWORD	dwResponse;
};

struct	OnReqGuildFreindlyMessageCancel
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//�޼����� �� ����� �̸�.
};

struct OnResGuildFriendlyMessageCancel
{
	DWORD	dwMsgType;
//	DWORD	dwResponse;
};


// ������ �����Ѵٴ� �޽���
struct	OnReqAttackVillageMsg
{
	DWORD		dwMsgtype;						// ON_REQUEST_ATTACK_VILLAGE
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)
	SI16        siAttackType;					// Attack Type
};
/*
struct	OnResAttackVillageMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_ATTACK_VILLAGE	
	UI16		uiCharID;						// �����ϴ� ĳ������ ID.
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
	SI16		siVillageDefenceDamage;			// ���ݿ� ���� �پ�� ���� ����.
	SI16		siDefenceSoldierDamage;			// ���ݿ� ���� �پ�� ���� ��
	VDEF		vdDefence;						// ���� ���� ����
	SI32		siDefenceSoldier;				// ���� ���� ����.
	SI16		siPlayerDamage;					// �÷��̾ �Դ� ������.
	SI16		siAttackType;			 	    //�̳��� �����߳� üũ.
	UI16		uiKind;
}; 
*/
struct	OnResAttackVillageMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_ATTACK_VILLAGE	
	UI16		uiCharID;						// �����ϴ� ĳ������ ID.
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)
	SI16		siVillageDefenceDamage;         // ���ݿ� ���� �پ�� ���� ����.
	VDEF		vdDefence;						// ���� ���� ����
	SI16		siAttackType;                   // ġ���� �����ΰ�
	UI16		uiKind;
	BYTE        bIsLong;                        // ���Ÿ� �����ΰ�?(1:LONG) 
};

/*
struct	OnReqRepairVillageMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_REPAIR_VILLAGE
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
	VDEF		vdDefence;						// ����� ����.
};

struct	OnResRepairVillageMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_REPAIR_VILLAGE
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
	VDEF		vdChangedDefence;				// ����� ����.
};
// ������ �����Ѵٴ� �޽���
*/

struct OnReqIncreaseGuildUnit	//����� ���������� ������Ų��.(��û)
{
	DWORD		dwMsgType;		
	SI16		siKind;							// ����.
	SI16		siCount;						// �߰��� ����.
	UI16		uiVillageCode;					// ���� �ڵ带 �ش�.
};


struct OnResIncreaseGuildUnit	//����� ���������� ������Ų��.(����)
{
	DWORD		dwMsgType;
	SI16		siKind;							// ����.
	SI16		siTotalCount;					// �ش� Kind�� �Ѱ���.
	SI16		siSupply;						// �Ҹ�� ����ǰ�� ��.
	MONEY		mnMoney;						// �Ҹ�� ��.
	DWORD		dwResponse;						// ����.
};	


struct OnReqDecreaseGuildUnit	//����� ����� ���������� �κ����� ��������.(��û)
{
	DWORD		dwMsgType;
	SI16		siKind;							// ����.
	SI16		siCount;						// ����.
};


struct	OnResDecreaseGuildUnit  //����� ����� ���������� �κ����� ��������.(����)
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	SI16		siKind;							// ����.
	SI16		siTotalCount;					// ��ܿ� ���� �ִ� �ش� �������� ��ü ����. 
	SI16		siMercenarySlot;				// �뺴 ����.
	SI16		siInventoryPos;					// �κ��丮 ����.
	SI16		siItemID;						// �޾ƿ��� ������ ���̵�.
	SI16		siQuantity;						// �޾ƿ��� ������ ���̵� ����.
};


struct OnReqGetGuildUnitList	// ����� �������� ����Ʈ�� ���´�.(��û)
{
	DWORD		dwMsgType;
	UI32		uiGuildID;		//��� ���̵� �����ش�.
};

struct OnResGetGuildUnitList    // ����� �������� ����Ʈ�� ���´�.(����)
{
	DWORD		dwMsgType;
	SI16		GuildCount[ON_MAX_GUILD_UNIT];	//������ ������ ����Ʈ�� ���´�.
	UI32		uiPopulation;
};

struct OnReqIncreaseGuildSupply		//	����� ����ǰ ������ �ø���(��û)
{
	DWORD		dwMsgType;
	SI32		siSupplyQuantity;	//�ø����� ����ǰ ����.
	SI16		siVillageCode;		//���� �ڵ�
};


//
struct OnResIncreaseGuildSupply		//	����� ����ǰ ������ �ø���(����)
{
	DWORD		dwMsgType;			
	DWORD		dwResponse;			// ���� ���� ����.
	MONEY		uiMoney;			// ����ǰ�� ����µ� ����� ��. siSupplyQuantity * 125
	SI32		siSupplyQuantity;   // �ø� ����ǰ ����.
};


struct OnReqDecreaseGuildSupply		// ����� ����ǰ�� �����´�.(��û)	
									// ���޼����� ��û�Ҽ� �ִ�.
{
	DWORD		dwMsgType;
	SI32		siSupplyQuantity;	// ������ ����ǰ ����. 
	SI16		siVillageCode;		// ���� �ڵ�.
};

struct OnResDecreaseGuildSupply		//	����� ����ǰ�� �����´�.(����).
{
	DWORD		dwMsgType;
	DWORD		dwResponse;			
	SI32		siSupplyQuantity;	// ������ ����ǰ ����.
};

struct OnReqGetGuildSupplyList		// ��� ����ǰ�� ������ �����´�(��û)
{
	DWORD		dwMsgType;
	SI16		siVillageCode;
};

struct OnResGetGuildSupplyList		// ��� ����ǰ�� ������ �����´�(����)
{
	DWORD		dwMsgType;
	SI32		siGuildSupply;		// ����� �� ����ǰ ����.
};


struct OnReqVillageDefenceSoldier	//  ������ ���� �ø���.(��û)
{
	DWORD		dwMsgType;
	SI16		siDefenceSoldierCount;  // �þ�� ������ ��.
	SI16		siVillageCode;			//���� �ڵ�
};

struct OnResVillageDefenceSoldier	//  ������ ���� �ø���.(����)
{
	DWORD		dwMsgType;			
	DWORD		dwResponse;			// ����.
	SI16		siDefenceSoldier;	// �þ ������ �� Total �ƴ�.
	SI16		siSupply;			// �Ҹ�� ��� ����ǰ�� ��
	MONEY		mnMoney;			// �Ҹ�� ��.
};

struct OnReqGetVillageDefenceSoldier
{
	DWORD		dwMsgType;
	SI16		siVillageCode;
};

struct OnResGetVillageDefenceSoldier
{
	DWORD		dwMsgType;
	SI32		siDefenceSoldier;
};

struct	OnChangeVillageDefenceMsg
{
	DWORD		dwMsgType;						// ON_CHANGED_VILLAGE_DEFENCE
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
	VDEF		vdChangedDefence;				// ����� ����.
};

struct	OnReqVillageAttackResult
{
	DWORD		dwMsgType;
//	char		szGuildID[ON_GUILDNAME_LENGTH+1];
};

struct  OnResVillageAttackResult
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	SI16		siVillageCount;
	//siVillageCount�� ������ŭ OnGuildResult�� ���� �´�.
};

struct OnReqGuildChoicePlayer
{
	DWORD		dwMsgType;
	SI16		siVillageCode;
	char		szUserName[ON_ID_LENGTH + 1];
};

struct OnResGuildChoicePlayer
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	SI16		siVillageCode;
	char		szUserName[ON_ID_LENGTH + 1];
};

struct OnReqInvestmentPower
{
	DWORD		dwMsgType;
	SI16		siVillageCode;
	char		szUserName[ON_ID_LENGTH + 1];
};

struct OnResInvestmentPower
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	SI16		siPercent;			//������ �����Ҽ� �ִ� ����.
};



struct	OnReqPIInMiniMapMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_PI_INMINIMAP
	CHAR		szName[ON_ID_LENGTH];			// ĳ���� ID.
};


// OnReqPIInMiniMapMsg�� �⺻���� ���� �޽����� OnResponseMsg�� ���
// dwMsgType	-	ON_RESPONSE_PI_INMINIMAP
// dwResponse	-	ON_RET_ON_FIELD : ����, ON_RET_NOT_CONNECTED : ������, ON_RET_IN_VILLAGE : ���� �ȿ� ����, ON_RET_IN_BATTLE : ���� �߿� ����.

struct	OnResPIInMiniMapOFMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_PI_INMINIMAP
	DWORD		dwResponse;						// ON_RET_ON_VILLAGE �Ǵ� ON_RET_IN_BATTLE
	UI16		uiKind;							// ĳ������ ����.
	UI16		uiMapID;						// Map ID
	UI16		uiX, uiY;						// X, Y ��ǥ	
};

struct	OnResPIInMiniMapIVMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_PI_INMINIMAP
	DWORD		dwResponse;						// ON_RET_IN_VILLAGE
	UI16		uiKind;							// ĳ������ ����.
	UI16		uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)
};

// �������� - ����!
struct	OnOATextMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_TEXT
	DWORD		dwLength;									// �ؽ��� ����

	// �ڿ��� �ؽ�Ʈ�� �� ����´�
};


// SMS Message
struct OnOASMSMsg
{
	DWORD	dwMsgType;			// ON_OFFICIAL_ANNOUNCEMENT
	DWORD	dwResponse;			// ON_RET_OA_SMS_MSG
	DWORD	dwCallCount;		// �����������(��)�� �Ϸù�ȣ
	char	szUserCode[32];		// �����ID : JOYON
	char	szOrgPhone[32];		// �߽Ű����ڹ�ȣ '0161112222'
	char	szDstPhone[32];		// ���Ű����ڹ�ȣ (Ư��)
	char	szCallMessage[100];	// ȣ�� �޽���
	char    szRecvTime[16];		// "YYYYMMDDHHMMSS"
	char	szReserved[22];		// ����...
};

struct	OnOAVillAttackedMsg									// ���� ����
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_VILL_ATTACKED
//	CHAR		szAttackGuildID[ON_GUILDNAME_LENGTH];		// �����ϴ� ����� ID
//	CHAR		szBeAttackedGuildID[ON_GUILDNAME_LENGTH];	// ���� ���ϴ� ����� ID
	UI16		uiVillageCode;								// ���� ���ϴ� ������ �ڵ�.(ID�� �ƴϴ�)	
};

struct	OnOAVillOccupiedMsg									// ������ ���� �Ǿ���
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_VILL_OCCUPIED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// ������ ����� ID.	
	UI16		uiVillageCode;								// ���ɴ��� ������ �ڵ�.(ID�� �ƴϴ�)		
};

struct	OnOAMaxInvChangedMsg								// �ְ� ������ ����
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_MAXINVESTMENT_CHANGED
	CHAR		szID[ON_ID_LENGTH];							// ���ο� �ְ� �������� �̸�.
	UI16		uiVillageCode;								// �ְ� �����ڰ� ����� ������ �ڵ�.(ID�� �ƴϴ�)		
};

struct	OnOAGuildCreatedMsg									// ��� ����
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_GUILD_CREATED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// ������ ����� ID.
};

struct	OnOAGuildDeletedMsg									// ��� ��ü
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_GUILD_DELETED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// ��ü�� ����� ID.
};

struct	OnOAGuildPayingBusinessMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_PAYING_BUSINESS
	UI32		uiBuyPrice;									// �������� �춧�� ����
	UI32		uiSellPrice;								// �������� �ȶ��� ����
	UI16		uiBuyVillageCode;							// �������� ��� ����
	UI16		uiSellVillageCode;							// �������� �Ĵ� ����
	UI16		uiItemCode;									// ������
};

struct	OnOADAIGetMoneyByDestroyVillageMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE
	MONEY		mnMoney;									// ������ �ı��Ͽ� ���� ��.
	UI16		uiVillageCode;								// �ı��� ������ Code
};

// �������� - ��!

struct	OnReqPMOpenMsg
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_OPEN
	UI16		uiItemNum;
	
	// OnBoothItemForSell�� uiItemNum��ŭ �ڵ���´�.
};

struct	OnResPMOpenMsg
{
	DWORD		dwMsgType;			// ON_RESPONSE_PM_OPEN
	DWORD		dwResponse;
	UI16		uiAccount;
};

struct	OnReqPMCloseMsg				
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_CLOSE
	
	// OnBoothItemForSell�� uiItemNum��ŭ �ڵ���´�.
};

struct	OnResPMCloseMsg
{
	DWORD		dwMsgType;			// ON_RESPONSE_PM_CLOSE
	DWORD		dwResponse;
	UI16		uiAccount;
};

struct	OnReqPMBuyItemMsg
{
	DWORD		dwMsgType;					// ON_REQUEST_PM_BUYITEM
	UI16		uiAccount;	
	UI16		uiQuantity;
	UI16		uiItemID;
	UI08		uiSlotID;
	UI08		uiInvenPos;
	UI08		uiFollowerID;	
};

struct	OnResPMBuyItemMsg
{
	DWORD		dwMsgType;					// ON_RESPONSE_PM_BUYITEM
	DWORD		dwResponse;
	UI16		uiQuantity;
	MONEY		NewMoney;		
	UI08		uiBoothSlot;	
};
/*
struct	OnAddBoothItemMsg
{
	DWORD		dwMsgType;			// ON_PM_ADD_BOOTH_ITEM
	UI16		uiAccount;
	UI16		uiItemID;
	UI08		uiSlotID;
};

struct	OnDelBoothItemMsg			
{
	DWORD		dwMsgType;			// ON_PM_DEL_BOOTH_ITEM
	UI16		uiAccount;
	UI16		uiItemID;
	UI08		uiSlotID;
};
*/
struct	OnReqPMItemInfoMsg
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_ITEMINFO
	UI16		uiAccount;
};

struct	OnResPMItemInfoMsg
{
	DWORD		dwMsgType;			// ON_RESPONSE_PM_ITEMINFO
	DWORD		dwResponse;
	UI16		uiItemNum;			// �������� ����
	
	// OnBoothItemInfo ��  uiItemNum��ŭ �ڵ���´�.
};

struct	OnReqChangeBoothItemMsg
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_CHANGEITEM
	UI16		uiItemNum;			// �������� ����

	// OnBoothItemForSell�� uiItemNum��ŭ �ڵ���´�.
};

typedef	OnResponseMsg	OnResChangeBoothItemMsg;	// ON_RESPONSE_PM_CHANGEITEM

struct	OnChangeBoothItemMsg
{
	DWORD				dwMsgType;			// ON_PM_CHANGE_BOOTH_ITEM
	UI16				uiAccount;
	OnBaseBoothItem		BaseBoothItem;
};

struct	OnReqAcceptTradeMsg
{
	DWORD				dwMsgType;				// ON_REQUEST_ACCEPT_TRADE
	BOOL				bAccept;				
};

typedef	OnResponseMsg	OnResAcceptTradeMsg;				// ON_RESPONSE_ACCEPT_TRADE

struct	OnResAcceptTradeOkMsg : public OnResAcceptTradeMsg	// ON_RESPONSE_ACCEPT_TRADE, dwResponse = ON_RET_OK
{
	SI32				siOppLevel;
	CHAR				szOppGuildID[ ON_GUILDNAME_LENGTH + 1 ];
	SI32				siOppGuildClass;
};


typedef	OnResponseMsg	OnResCancelTradeMsg;	// ON_RESPONSE_CANCEL_TRADE

struct	OnReqFinishChooseTradeItemMsg
{
	DWORD				dwMsgType;				// ON_REQUEST_FINISH_CHOOSE_TRADEITEM
	UI08				bFinishChooseItem;	
};

struct	OnResFinishChooseTradeItemMsg
{
	DWORD				dwMsgType;				// ON_RESPONSE_FINISH_CHOOSE_TRADEITEM
	DWORD				dwResponse;
	UI08				bIsMe;					
	UI08				bFinishChooseItem;		
};

struct	OnReqDecideTradeMsg
{
	DWORD				dwMsgType;				// ON_REQUEST_DECIDE_TRADE
	UI08				bDecideTrade;	
};


struct	OnResDecideTradeMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_DECIDE_TRADE
	DWORD		dwResponse;
	BOOL		bIsMe;
};

struct	OnReqChangeTradeGoods
{	
	DWORD				dwMsgType;				// ON_REQUEST_CHANGE_TRADE_GOODS	
	TradeGoodsKind		Kind;					
	LPARAM				lParam1;
	LPARAM				lParam2;
	UI08				uiSlot;				
};

typedef	OnResponseMsg	OnResChangeTradeGoods;	// ON_RESPONSE_CHANGE_TRADE_GOODS

struct	OnChangeTradeGoods
{	
	DWORD				dwMsgType;				// ON_CHANGE_TRADE_GOODS	
	TradeGoodsKind		Kind;					
	LPARAM				lParam1;
	LPARAM				lParam2;
	UI08				uiSlot;				
};

typedef	OnResponseMsg	OnReqCompleteTradeBoookMsg;		// ON_REQUEST_COMPLETE_TRADE_BOOK
typedef OnResponseMsg	OnResCompleteTradeBoookMsg;		// ON_RESPONSE_COMPLETE_TRADE_BOOK

struct	OnCompleteTradeMsg
{
	DWORD				dwMsgType;				// ON_COMPLETE_TRADE		
	UI08				uiTradeFollowerNum;		
	UI08				uiTradeItemNum;
};

struct	OnReqCharDetailInfoMsg					// ON_REQUEST_CHAR_DETAILINFO
{
	DWORD				dwMsgType;				
	UI16				uiAccount;				// ĳ������ Account.
};

struct	OnResCharDetailInfoMsg : public OnResponseMsg		// ON_RESPONSE_CHAR_DETAILINFO
{
	UI16				uiAccount;				// ĳ������ Account.

	// �÷��̾��� ��쿡�� �ڿ� OnCharPlayerDetailInfo�� �ڵ���´�.	
};

struct	OnReqVillageInfoMsg								
{		
	DWORD				dwMsgType;						// ON_REQUEST_VILLAGE_INFO
	UI16				uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
};

struct	OnResVillageInfoMsg : public OnResponseMsg			// ON_RESPONSE_VILLAGE_INFO
{
	UI16				uiVillageCode;					// ������ �ڵ�.(ID�� �ƴϴ�)	
	// dwMsgType�� ON_RET_OK�̸� �̾ OnRecvVillageInfo ����ü�� �ִ�.	
};

struct	OnReqEnterStructureMsg
{	
	DWORD				dwMsgType;						// ON_REQUEST_ENTER_STRUCTURE
	SI32				siStructureKind;				// ���� �ǹ��� �ڵ�.
	LPARAM				lParam1;						// Parameter 1
	LPARAM				lParam2;						// Parameter 2
};

struct	OnChangeWeather
{
	DWORD				dwMsgType;				

	BYTE				fWeather;
};

struct	OnMakeThunder
{
	DWORD				dwMsgType;

	UI16				uiAccount;
};

typedef	OnResponseMsg	OnResEnterStructureMsg;			// ON_RESPONSE_ENTER_STRUCTURE

typedef	OnMsg			OnReqLeaveStructureMsg;			// ON_REQUEST_LEAVE_STRUCTURE

typedef	OnResponseMsg	OnResLeaveStructureMsg;			// ON_RESPONSE_LEAVE_STRUCTURE

typedef	OnMsg			OnReqLeaveFactoryMsg;			// ON_REQUEST_LEAVE_FACTORY

typedef	OnResponseMsg	OnResLeaveFactoryMsg;			// ON_RESPONSE_LEAVE_FACTORY

struct	OnRequest_VMercenary_Delete
{
	DWORD				dwMsgType;
};


struct	OnResponse_VMercenary_Change
{
	DWORD				dwMsgType;

	UI16				uiUniqueID;
	SI32				siKind;
};

struct	OnResponse_VMercenary_ChangeMain
{
	DWORD				dwMsgType;

	SI08				siSlot;
};

struct	OnRequestRefineItem
{
	DWORD				dwMsgType;

	UI16				uiFollowerSlot;
	UI16                uiItemID;
};

struct	OnResponseRefineItem
{
	DWORD				dwMsgType;

	UI16                uiResult;
	UI16				uiFollowerSlot;
	UI16                uiNewItemID;
	UI16                uiNewItemSlot;
};

struct	OnReqRunProgramListMsg					// ON_REQUEST_RUN_PROGRAM_LIST
{
	DWORD				dwMsgType;						// ON_RESPONSE_RUN_PROGRAM_LIST
	DWORD				dwReason;						// ����
};


struct	OnResRunProgramListMsg
{
	DWORD				dwMsgType;						// ON_RESPONSE_RUN_PROGRAM_LIST
	DWORD				dwReason;						// ����
	SI32				siRunProgramNum;				// ���� �������� ���α׷��� ���� ���´�.
	// ���α׷� ����ŭ CHAR[255]�� �ڵ���´�.
};

struct	OnMyVillageAttackedMsg
{
	DWORD				dwMsgType;						// ON_MYVILLAGE_ATTACKED
	UI16				uiVillageCode;					// ���ݴ��ϴ� ������ ID
};

struct	OnRequestGuildCreateCost
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_CREATE_COST
};

struct	OnResponseGuildCreateCost
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_CREATE_COST
	MONEY				mnGuildCreateCost;				// ��� ������ �ʿ��� ���
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ����� ��ܿ��� �����Ͽ� Ư�� �������� �Ӹ��� �� �ְ� �޴� ��Ŷ
struct OnRequestGuildPromotion
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_PROMOTION
	CHAR				szCharID[ON_ID_LENGTH];			// ĳ���� ID
	SI32				siClassCode;					// �Ӹ��� ����
};

struct OnResponseGuildPromotion
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_PROMOTION
	DWORD				dwResponse;
};
//����������������������������������������������������������������������������������������������������������������������������������������������������������




//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ����� ��ܿ��� �����Ͽ� �־��� ���޿��� ���ӽ�ų �� �ְ�޴� ��Ŷ
struct OnRequestGuildDemotion
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_DEMOTION
	CHAR				szCharID[ON_ID_LENGTH];			// ĳ���� ID
};

struct OnResponseGuildDemotion
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_DEMOTION
	DWORD				dwResponse;
};
//����������������������������������������������������������������������������������������������������������������������������������������������������������


//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο� �ʿ��� ���� ���� ��Ȳ�� ��û�� �� �ְ�޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditVillageList
{
	DWORD						dwMsgType;
};

// ���� ���� 0���� ���� response ��Ŷ
struct	OnResAuditVillageList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// ���� ���� 1�� �̻��� ���� response ��Ŷ
struct	OnResAuditVillageListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						siVillageCount;

	// �ڿ� ���� ����(OnAuditVillageInfo)�� siVillageCount��ŭ �ִ�.
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο� �ʿ��� ģ�� ����Ʈ�� ��û�� �� �ְ�޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditFriendList
{
	DWORD						dwMsgType;
};

struct	OnResAuditFriendList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						siFriendCount;
	// siFriendCount�� 1�̻��� ���, �ڿ� ģ�� ����(OnAuditFriendInfo)�� siFriendCount��ŭ �ִ�.
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο��� ģ���� ���Űź� ������ �� �ְ� �޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditFriendSetDenial
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];			// ĳ���� ID
	BOOL						bDenial;						// ���Űź� / ����
};

struct	OnResAuditFriendSetDenial
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο��� ģ�� �Ѹ��� �߰��� �� �ְ� �޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditFriendAddOne
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];			// ĳ���� ID
};

struct	OnResAuditFriendAddOne
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	BOOL						bLogin;
};


//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο��� ģ���� ������ �� �ְ� �޴� ��Ŷ (üũ �ڽ��� �̿��� �������� ���� ���� �ִ�.)
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditFriendRemoveSome
{
	DWORD						dwMsgType;
	UI16						uiCount;
};

struct	OnResAuditFriendRemoveSome
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο��� ģ�� ��ü�� ������ �� �ְ� �޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditFriendRemoveAll
{
	DWORD						dwMsgType;
};

struct	OnResAuditFriendRemoveAll
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ģ���� �α��� ���� ��쿡 �˷��ִ� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnFriendLoggedInOut
{
	DWORD						dwMsgType;
	BOOL						bState;
	CHAR						szID[ON_ID_LENGTH];
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο��� Ư�� ������ ���� ������ ��û�� �� �ְ� �޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditProfitStatistics	//	Ư�� ���� ���� �ֱ� ��� ��û
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};
// ������: ������ OnResProfitStatistics(������ ��û���� ���̴�) ����ü�� �̿��Ѵ�.


//����������������������������������������������������������������������������������������������������������������������������������������������������������
// �뺴�� �߰���ų �� �ְ� �޴� ��Ŷ(�糪�Ϳ� ���� ���)
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnAddFollower
{
	DWORD						dwMsgType;
	OnFollowerInfo				FollowerInfo;
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// �������, ��ȭ��� ����
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqSetAttackMode
{
	DWORD						dwMsgType;
	BOOL						bAttackMode;
};

struct	OnResSetAttackMode
{
	DWORD						dwMsgType;
	UI16						uiAccount;
	BOOL						bAttackMode;
};

struct	OnEventUserRankings
{
	DWORD						dwMsgType;
	UI16						uiPlayerNum;	
};

//����������������������������������������������������������������������������������������������������������������������������������������������������������
// ��ο� �ִ� ��� ������ �ְ� �޴� ��Ŷ
//����������������������������������������������������������������������������������������������������������������������������������������������������������
struct	OnReqAuditGuildMemberInfoMsg
{
	DWORD	dwMsgType;
};

struct	OnRespAuditGuildMemberInfoMsg //strAuditGuildMemberInfo
{
	DWORD	dwMsgType;	
	SI32	siLoginedGuildMemberCount;
	SI32	siNowGuildMemberCount;
	SI32	siSHNExceptMemberCount;  // �� ���� ��ŭ �ݺ�. -1 ����� ��� ���� -2 ��ܿ��� �ƴϴ�.
};

// ���� ����Ʈ�� �Ϸ��� ������ ���� ��Ŷ
struct OnDokdoQuestCompletedUserInfo
{
	DWORD	dwMsgType;	
	UI32	uiQuestCompletedUserCount;
	char	szID[ON_ID_LENGTH];
};

// ��ũ�� ����� ���� ��Ŷ
struct OnReqMacroUsingUserInfo
{
	DWORD	dwMsgType;
	char	szMacroProgName[128];
};


//���� �������� ������ �Ѵٰ� ��û�Ѵ�.
struct OnReqChangeUnit
{
	DWORD dwMsgType;     //�޼��� Ÿ��
	SI16  siChangeKind;  //������ �������� �ڵ�
};

struct OnResChangeUnit
{
	DWORD dwMsgType;   //�޼��� Ÿ��
	DWORD dwResPonse;  //���
	SI32  siMoney;     //�����ϴµ� �ʿ��� ���   
	SI16  uiUniqueID;  //������ �� ���̵�	
	SI16  siHP;        //HP
	SI16  siMP;        //MP
	SI16  siKind;       //���� ���� �ڵ�
	BYTE  bQuantity;   //���	
};

struct OnResChargeMP
{
	DWORD dwMsgType;   //�޼���Ÿ��
	DWORD dwResponse;  //���
	SI16  siNowMP;     //������ MP��
};

//���� ���⸦ ��û�Ͽ���.
struct OnAskGiveUp
{
	DWORD        dwMsgType;        //�޼��� Ÿ��
	UI16         uiVillageCode;    //���� �ڵ�
	MONEY        mnMoney;          //���� ��� 
};

struct OnReqGiveUp
{
	DWORD		dwMsgtype;
	UI16		uiVillageCode;	   // ������ �ڵ�.(ID�� �ƴϴ�)
	UI16        uiResponse;        // 2:ok 3:no         
};

struct OnResGiveUp
{
	DWORD dwMsgType;  //�޼��� Ÿ��
	DWORD dwResponse; //���
};

//-----------------------------------------------------------------------------
// Name: OnReLoadWearItem()
// Code: Linus (2004-07-19)
// Desc: ON_RELOAD_WEAR_ITEM ��ȣ���� ���. Server -> Client ��
//		���� �ð����� 24�ð����� ĳ���͵��� �����ִ� �ð��� ������ ������ �����϶�� ���̴�.
//		�� ����ü �Ĺ̿� OnTimerItemInfo�� siItemCount��ŭ �ٰ� �ȴ�.
//	dwMsgType			- �� �޽��� Ÿ��(ON_RELOAD_WEAR_ITEM)
//	siMercenaryCount	- �뺴 ī��Ʈ 
//-----------------------------------------------------------------------------
struct OnReLoadWearItem
{
	DWORD	dwMsgType;
	SI16	siItemCount;
	//�ڿ� OnTimerItemInfo siItemCount��ŭ �ٴ´�.
};

#define GAMEGUARD_PASSKEY 1004

struct OnCheckGameGuardMsg
{
	DWORD	dwMsgType;
	DWORD	dwGameGuardKey;
};

// ��ũ�� ����� ���� ��Ŷ
struct OnReqGameGuardUserInfo
{
	DWORD	dwMsgType;
	DWORD	dwGameGuardMsgType;
	char	szMemo[128];
};

struct OnResRealTime
{
	DWORD	dwMsgType;
	UI16	uiYear;
	UI16	uiMon;
	UI16	uiDay;
};

#endif
