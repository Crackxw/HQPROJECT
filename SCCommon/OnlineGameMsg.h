// 수정 일자 : 2002-02-18 오후 3:58:40 - 양정모

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

	DWORD		dwRoundKey[32]; // Seed 키 
};



//---------------------------- 아이템 현금 판매 관련 ----------------------------------

struct OnResItemReceivedMsg				// 아이템 도착 알림
{
	DWORD		dwMsgType;
	UI16		uiCount;				//	도착한 아이템 개수 
};

struct OnReqItemListMsg					// 아이템 리스트 요청
{
	DWORD		dwMsgType;
};

struct OnResItemListMsg					// 아이템 리스트 반환
{
	DWORD		dwMsgType;
	UI16		uiCount;
	// 뒤에 ItemID( 2byte ) , Quantity( 2byte ) 개수( uiCount ) 만큼 나열..
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



//---------------------------- 공통적으로 사용하는 구조체 -----------------------------//
struct	OnMsg
{
	DWORD		dwMsgType;	
};

struct	OnResponseMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
};



struct OnHeartBeatMsg	// ON_HEARTBEAT_INTERVAL( 1/1000초 단위)에 한번씩 보낸다.
{
	DWORD		dwMsgType;
	DWORD		dwTimer;			// 스피드핵 방지를 위해 클라이언트의 타이머를 보낸다.
	UI16		uiType;
};

struct	OnTextMsg
{
	DWORD		dwMsgType;			// ON_TEXT_MSG
	DWORD		dwResponse;			// 
	SI32		siStringLength;		// 정보 문자열의 길이

	// 뒤에는 CHAR형으로 문자열이 따라온다.
};

//-------------- 게임 서버에 접속하고 로그 아웃할때 보내는 메시지 -------------------//
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

//-------------- 게임 서버 접속 시에 게임 서버로 부터 받는 메시지 ------//

// 로그인 서버및 게임 서버의 응답 헤더.
struct	OnLogInMsgHeader								
{
	DWORD						dwResponse;				// 서버로 부터의 응답.
};

// 게임 서버에 로그인 성공했을 경우의 메시지,
struct	OnGameServerLogInMsg  : public OnResponseMsg			
{		
	// dwResponse가 ON_RET_OK 일 경우 OnGameServerLogInMsgData 가 이어서 온다.
};

//--------------------------------------------------------------------------------------//

//------------------------ 캐릭터를 이동 시킬때 보내는 메시지 --------------------------//
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


// 캐릭터를 워프 시킬때 보내는 메시지
struct	OnReqWarpMsg
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];
	UI16						uiWarpCode;				// warp code
};

struct	OnResWarpMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_WARP
	DWORD						dwResponse;				// ON_RET_OK : 성공, ON_RET_NO : 실패
};

struct	OnResWarpOkMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_WARP
	DWORD						dwResponse;				// ON_RET_OK : 성공, ON_RET_NO : 실패
	UI16						uiMapID;				// 내 새로운 맵의 인덱스.
	UI16						uiX;					// 내 새로운 X 좌표.
	UI16						uiY;					// 내 새로운 Y 좌표.
	UI16						uiPlayerNum;			// 내 주위의 플레이어 수.	
	UI16						uiFieldItemNum;			// 내 주위의 땅에 떨어진 아이템 수. 
	// 주위에 플레이어, 땅에 떨어진 아이템의 정보가 이어서 있다.
};


//------------------------ 마을에 들어간다는 메시지   ----------------------------------//
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
	//자기 자신한테 갈때는 
};

//--------------------------------------------------------------------------------------//

//------------------------ 마을에서 나간다는 메시지   ----------------------------------//
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

//----------------------------- 채팅 관련 메시지  --------------------------------------//
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

//---------------------------- 전투한다는 메시지 ---------------------------------------//
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

//---------------------- 클라이언트에 캐릭터를 추가 및 삭제 하라는 메시지 --------------//
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
	
	// 플레이어나 몬스터 한마리의 대한 정보가 따라온다.
};

struct	OnDelCharMsg
{
	DWORD						dwMsgType;
	UI16						uiCharID;
};
//-------------------------------------------------------------------------------------//

//-------------------------시장에서 아이템을 사고 팔때 사용하는 메시지 ----------------//
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

//---------------------------- 필드에서 아이템을 버리고 줍는데 사용하는 메시지 --------//
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

//------------------------------ 장부에서 사용할때의 메시지 ---------------------------//
// Tab 1. 총자산
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
	UI32						uiTotalUser;				// 전체 등록 사용자수	
	MONEY						siProperty;					// 총 자산.
	MONEY						siBankMoney;				// 전장 잔고 (+)
	MONEY						siLoanMoney;				// 전장 대출 (-)
};

// Tab 2. 모든 계정.
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
	DWORD						dwBankBookNum;	//최대 20개 이다.
	
	// 뒤에는 dwBankBookNum 만큼 통장의 계좌 번호(OnAccountNumber)가 따라온다 (계좌번호에서 마을 얻음)

	// 뒤에는 dwBankBookNum 만큼 각 통장의 들어가 있는 돈(MONEY)이 따라온다.
};

// Tab 3. 부동산 정보.
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

	// 뒤에는 dwOwnNum만큼 부동산 목록이 따라온다. (OnAuditOwnInfo 구조체)
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

	// 원료, 도구, 시설 등등 의 정보가 따라온다.
};

// Tab 4. 마을 정보

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
	// 갱신일 일자.
	// 취급 상품 개수
	// 취급 상품 정보가 온다.
};


// Tab 5. 무장 정보
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
	
	// 뒤에는 dwFollowerNum만큼 uiFollowerID가 따라온다(UI08).

	// 뒤에는 dwFollowerNum만큼 무장 이름이 따라온다
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

	// 레벨, 겸치, 국가, 힘, 민첩, 건강, 지성, 생명력, 마법, 방어율, 마법,
	// 최소공격, 최대공격, 공격 성공, 회피성공 무게
};*/
//-------------------------------------------------------------------------------------//

//------------------------------- 필드에서 일반적인 메시지 ----------------------------//

struct	OnResMoneyInfo : public OnResponseMsg
{	
	MONEY						Money;	
};

struct OnResRelationMsg : public OnResponseMsg
{
	BOOL						bRelation;
};

// 마을 국가코드
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
	DWORD						dwGuildRelation;				// 길드의 관계.
	SI32						siDefenceSoldier;
	SI16						siAttackType;
};


// 마을 안내문
struct	OnReqWriteDescript			// 마을 안내문 쓰기
{
	DWORD						dwMsgType;
	UI16						uiLength;				// 마을 안내문 길이.
	// 뒤에는 uiLength  Byte만큼 Text가 온다.
};

struct	OnResWriteDescript
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};

struct	OnReqVillageDescript		// 마을 안내문 읽기
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};

struct	OnResVillageDescript
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
	CHAR						szGuildID[ON_GUILDNAME_LENGTH];		// 이 마을을 소유한 길드의 ID	
	UI16						uiLength;							// 마을 안내문 길이.
	// 뒤에는 uiLength  Byte만큼 Text가 온다.
};

struct	OnReqImoticonMsg				//	이모티콘 표시
{
	DWORD						dwMsgType;
	UI16						uiImoticonID;
};

struct	OnResImoticonMsg
{
	DWORD						dwMsgType;
	UI16						UserID;					// 표정을 지은 사람
	UI16						uiImoticonID;			// 이모티콘 종류
};

//-------------------------------------------------------------------------------------//

//--------------------------------- 저자 거리에서 메세지 ------------------------------//
// 좌판대에 들어간다는 메세지 (OK,NO,PLAYER)
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

// 좌판대에서 나간다는 메세지
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
// 좌판대에 아이템을 올림
struct	OnReqSetBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
	UI08						uiNum;							// 아이템 개수.( 0~5 개)
	// 뒤에는 uiNum 만큼 OnBoothItem 구조체가 온다.
};

struct	OnResSetBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// 좌판대에 올려 놓은 물건을 취소
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

// 좌판대에 있는 아이템을 요청하는 메세지 (OK,NO,EMPTY)
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

// 좌판대의 아이템을 클릭했을때, 아이템을 사기전에 보여주는 정보 메세지
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
//	uiNum 만큼 OnBoothItemInfo 구조체가 따라 온다.
};

// 좌판대에서 물건을 살때 메세지(산사람: 샀는거 성공여부. 판사람:판거 성공여부. 주변사람모두:보이는거 달라졌으면 달라졌다는 정보)
struct	OnReqBuyBoothItemMsg
{
	DWORD						dwMsgType;
	UI16						uiBoothID;
	UI16						uiItemID;
	UI08						uiFollowerID;
	UI16						uiPos;
	UI16						uiQnt;		// 산것을 넣을 슬롯
};
struct	OnResBuyBoothItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
struct	OnResSellBoothItemMsg		// 판 사람이 받는 메세지.
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiInvenPos;		//	정보를 업데이트 한다. (위치)
	UI16						uiItemID;
	UI16						uiQuantity;		//	(남은 개수- 결과값)
	MONEY						siMoney;		//	변해서 소유하게 된 돈
};

//-------------------------------------------------------------------------------------//


//파티 신청.
struct  OnReqAskPartyMsg
{
	DWORD						dwMsgType;
	UI16						UserID;		// 파티 신청 받은 클라이언트의 ID
};

//파티 신청 받은 클라이언트가 받은 메세지.
struct	OnReceiveAskPartyMsg					
{
	DWORD						dwMsgType;
	UI16						UserID;					// 파티를 보낸 사람의 ID.			
	CHAR						szID[ON_ID_LENGTH + 1];	// 이름.
};


//파티 신청 받은 클라이언트가 보내는 메세지.동의 부동의
struct OnResAskPartyMsg
{
	DWORD						dwMsgType;				
	DWORD						dwResponse;				//OK,NO 의 여부.	
};

//파티 신청 보낸 사람이 받은 클라이언트의 결과에 따라 받은 응답.
struct	OnResAcceptPartyMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// OK일 경우 siNum만큼 파티원의 수를 더해서 보내준다.
	CHAR						szID[ON_ID_LENGTH + 1];
	SI16						siNum;					//siNum만큼 파티원의 수를 더한다.OnPartyInfo 

};

struct	OnResLeavePartyMsg
{
	DWORD						dwMsgType;				// ON_RESPONSE_LEAVE_PARTY
	DWORD						dwResponse;
	SI32						siMembersNum;
	CHAR						szLeavedPlayerID[ ON_ID_LENGTH + 1 ];	
};

//----------------------------------- 거래 관련 메세지 --------------------------------//
// 거래를 요청하는 메세지.
struct	OnReqAskTradeMsg				//	(거래를 요청한 사람)
{	
	DWORD						dwMsgType;
	UI16						UserID;					// 교환하고자 하는 상대	
};

typedef	OnResponseMsg			OnResAskTradeMsg;

struct	OnReceiveAskTradeMsg			//	(거래를 요청 받은 사람)
{
	DWORD						dwMsgType;
	UI16						UserID;					// 거래를 요청한 사람 아이디.	
	CHAR						szID[ON_ID_LENGTH + 1];	// 거래를 요청한 사람 이름.
};

//  물건을 올려 놨을때 메세지.
struct	OnReqPreTradeMsg				//	(물건을 올려놓은 사람)
{
	DWORD						dwMsgType;
	UI08						uiItemNum;			//	아이템의 수
	UI08						uiFollowerNum;		//	용병의 수
	MONEY						Money;				//	돈 액수
	//		아이템수 만큼 아이템교환 구조체가 온다.   --	 TradeItem[uiItemNum] 구조체 배열.
	//		  용병수 만큼 용병교환   구조체가 온다.
};

struct	OnReceivePreTradeMsg			//	(상대방이 받는 메세지)
{
	DWORD						dwMsgType;
	UI08						uiItemNum;			//	아이템의 수
	UI08						uiFollowerNum;		//	용병의 수
	MONEY						Money;				//	돈 액수
	//		아이템수 만큼 아이템교환 구조체가 온다.   --	 TradeItem[uiItemNum] 구조체 배열.
	//		  용병수 만큼 용병교환   구조체가 온다.
};

//	아이템 올려놓고 고정시킬때 - 수락버튼
struct	OnReqConfirmMsg
{
	DWORD						dwMsgType;
	BOOL						bType;					//	수락버튼 On, Off
};

struct	OnReceiveConfirmMsg
{
	DWORD						dwMsgType;
	BOOL						bType;					//	수락버튼 On, Off
};

//  물건을 교환 할때 메세지.
struct	OnReqTradeSuccessMsg				//	(물건을 올려놓은 사람)
{
	DWORD						dwMsgType;
	UI08						uiSendItemNum;		//	아이템의 수	
	UI08						uiRecvItemNum;		//	받는 아이템 수
	UI08						uiSendFollowerNum;	//	보낸 용병의 수
	UI08						uiRecvFollowerNum;	//	받는 용병 수.
	MONEY						SendMoney;			//	주는 돈 액수
	MONEY						RecvMoney;			//	받는 돈 액수
	// uiItemNum만큼 MyItemData 구조체가 온다.
	// uiRecvItemNum만큼 SI16크기의 인벤토리 위치가 온다.
};

// 물건 교환 할때의 응답.
struct	OnResTradeSuccessMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	/*UI08						RecvItemNum;			//	아이템의 수
	UI08						RecvFollowerNum;		//	용병의 수
	MONEY						RecvMoney;				//	돈 액수
	UI08						SendItemNum;			//	아이템의 수
	UI08						SendFollowerNum;		//	용병의 수
	MONEY						SendMoney;				//	돈 액수
	//	A의	아이템수 만큼 아이템교환 구조체가 온다.   --	 TradeItem[uiItemNum] 구조체 배열.
	//	A의  용병수 만큼 용병교환   구조체가 온다.
	//	B의	아이템수 만큼 아이템교환 구조체가 온다.   --	 TradeItem[uiItemNum] 구조체 배열.
	//	B의	  용병수 만큼 용병교환   구조체가 온다.*/
};

// 거래를 취소해서 끝내는 메세지
struct	OnReqCancelTradeMsg
{
	DWORD						dwMsgType;				// 취소 눌렀을 경우.
};


// 아이템 소모 한다는 메세지.
struct	OnReqSpendItemMsg
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;			//	부하
	UI16						uiItemID;				//	아이템
	UI16						uiQuantity;				//	양
	UI16						uiPosInventory;			//	인벤토리 위치
};
 
struct	OnResSpendItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;	
};
//-------------------------------------------------------------------------------------//

//------------------------------ 은행에서 사용할때의 메시지 ---------------------------//
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
	MONEY						siBalance;					// 은행의 잔고를 돌려준다.
	MONEY						siMoney;					// 남은 잔액을 돌려준다.	
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
// 전장 입금
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

// 전장 출금
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


//계좌 종류를 요청한다.
struct OnReqBankAccountSort
{
	DWORD						dwMsgType;
	UI16						uiKind;
};

// 계좌를 만든다.
struct	OnReqBankOpenSavingAccountMsg
{
	DWORD						dwMsgType;
	UI16						uiKind;						// 계좌의 종류	
	MONEY						siFirstDepositMoney;		// 처음에 입금하는 돈
	UI16						uiExpireMonth;				// 만기 날짜 (적금이 아닐 경우 0)
};

// 계좌를 만든다는 메시지의 서버로 부터의 응답
struct	OnResBankOpenSavingAccountMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// 계좌를 만든다는 메시지의 서버로 부터의 응답(성공)
struct	OnResBankOpenSavingAccountOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	OnAccountNumber				AccountNumber;
	MONEY						siMoney;
};

// 통장의 리스트를 달라고 하는 메시지
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
	
	// 뒤에는 dwBankBookNum 만큼 통장의 계좌 번호(OnAccountNumber)가 따라온다

	// 뒤에는 dwBankBookNum 만큼 각 통장의 들어가 있는 돈(MONEY)이 따라온다.
};

// 통장의 리스트를 달라고 하는 메시지
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

struct	OnReqBankMaxCreditLoanMsg					//신용대출의 한도금액
{
	DWORD						dwMSgType;
};


struct	OnResBankMaxCreditLoanMsg
{
	DWORD						dwMSgType;
	DWORD						dwResponse;	
};

struct	OnResBankMaxCreditLoanOkMsg					//신용대출의 한도금액
{
	DWORD						dwMSgType;
	DWORD						dwResponse;	
	MONEY						uiMaxMoney;
	MONEY						siProperty;			//	총 자산.
	//UI32						uiCreditRank;		//	신용도 랭킹
};

// 담보
struct	OnReqCollateralLoanMsg
{
	DWORD						dwMsgType;
	UI16						uiMonth;
	MONEY						siMoney;
	DWORD						dwItemNum;

	// 뒤에는 dwItemNum 만큼 각 아이템을 갖고 있는 사람.	(UI08 uiFollowerID[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 아이템코드가 따라온다.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 아이템별 개수가 따라온다.	(UI16 uiNum[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 인벤토리 위치가 따라온다.	(SI16 siInvenLoc[MAX_COLLATERAL_NUM])
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

// 담보List를 요구하는 메세지 (돈을 갚은 상태일때, 물건만 찾는 것...)
struct	OnReqCollateralListMsg
{
	DWORD						dwMsgType;
};

struct	OnResCollateralListMsg					//자신의 담보 List를 가져오는 메세지
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct	OnResCollateralListOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwItemNum;
	// 뒤에는 dwItemNum 만큼 각 아이템코드가 따라온다.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 아이템별 개수가 따라온다.	(UI08 uiNum[MAX_COLLATERAL_NUM])
};

//List에 있는 모든 담보를 찾아간다는 메세지
struct	OnReqTakeCollateralMsg
{
	DWORD						dwMsgType;
	DWORD						dwItemNum;	
	// 뒤에는 dwItemNum 만큼 각 아이템을 갖고 있는 사람.	(UI08 uiFollowerID[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 아이템코드가 따라온다.		(UI16 uiITemCode[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 아이템별 개수가 따라온다.	(UI16 uiNum[MAX_COLLATERAL_NUM])
	// 뒤에는 dwItemNum 만큼 각 인벤토리 위치가 따라온다.	(SI16 siInvenLoc[MAX_COLLATERAL_NUM])
};

struct	OnResTakeCollateralMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};




//-------------------------------------------------------------------------------------//

//------------------------------ 농장에서 사용할때의 메시지 ---------------------------//
struct	OnReqFarmInfoMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
};

struct	OnResFarmInfoMsg
{
	DWORD						dwMsgType;
	UI32						uiIndustrialComplexNum;

	// uiIndustrialComplexNum 만큼 OnIndustrialComplexInfo 데이타가 이어서 온다.
	//OnIndustrialComplexInfo		IndustrialComplexInfo[ ON_MAX_INDUSTRIAL_COMPLEX_NUM ];
};

struct	OnReqFarmBuyMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장인가?	(0~9)
};

struct	OnResFarmBuyMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// 농장을 판다.
struct	OnReqFarmSellMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장인가?	(0~9)
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
	MONEY						Money;				//	농장을 팔고 받은 돈..
};
// 처음 농장에 들어갈때 받는 메세지
struct OnReqFarmSlotInfoMsg
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장의 슬롯들을 보고 싶은가?	(0~9)
};

struct OnResFarmSlotInfoMsg
{
	DWORD						dwMsgType;							// ON_RESPONSE_FARM_SLOTINFO
	SI16						siMaxConstructibleFactoryNum;		// 최대 건설할 수 있는 공장의 수.
	SI16						siFactoryNum;						// siFactoryNum가 -1 이면 실패이다.

	// 뒤에는 siFactoryNum(지어진 건물 수)만큼 OnFarmSlotList가 온다.
};

/*
struct OnResFarmSlotInfoOkMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	DWORD						dwListNum;			//	건물이 지어진 개수..
	// 뒤에는 dwListNum(지어진 건물 수)만큼 OnFarmSlotList가 온다.
};
*/
struct	OnReqFarmBuildStructMsg							//	농장의 슬롯에 건물을 짓고자 할때.
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	몇번 슬롯인가?
	CHAR						Code[4];			//	지으려는 건물 종류
	SI16						siPay;				//  임금
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
	UI08						uiFarmIndex;		//	몇번 농장 슬롯을 늘리는가?
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
	UI08						uiFarmIndex;		//	몇번 농장 슬롯을 늘리는가?
	UI08						uiSlotIndex;		//	어떤 슬롯을 부실것인가?
};

struct OnResFarmDestroyStructMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmSetEquipmentMsg						//	시설물 설치(업그레이드) 메세지 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에
	UI08						uiUpIndex;			//	어떤것을 설치 하겠는가?
};

struct OnResFarmSetEquipmentMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmUpgradeSlotMsg						// 업그레이드 일을 계획한다.
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	어떤 슬롯에
	SI16						siPay;				//	일1 당 품삯
};
struct OnResFarmUpgradeSlotMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmItemProductMsg						//	아이템 생산 요청 메세지.
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에	
	UI16						uiItemCode;			//	어떤 아이템을
	UI32						uiQuantity;			//	몇개 생산할 것인가?
	UI32						uiPay;				//	일1 당 품삯
};

struct OnResFarmItemProductMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


struct OnReqPlantChangePayMsg						//	임금을 변경하는 메세지
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에	
	SI16						siPay;				//	바뀌는 임금
};

struct OnResPlantChangePayMsg						//	아이템 생산 요청 메세지.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						ChangedMoney;		//	바뀐 돈 액수.
};

struct OnReqPlantCancelPlanMsg						//	임금을 변경하는 메세지
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에	
};

struct OnResPlantCancelPlanMsg						//	아이템 생산 요청 메세지.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	MONEY						ChangedMoney;		//	바뀐 돈 액수.
};

struct OnReqFarmWorkStartMsg
{
	DWORD						dwMsgType;
	UI08						uiSlotIndex;		//	어떤 슬롯에서 일할려는가?
};

struct OnResFarmWorkStartMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqFarmWorkMsg								//	일을 했다는 메세지
{
	DWORD						dwMsgType;
	UI08						uiGameType;			//	미니게임종류
	UI08						uiSlotIndex;		//	어떤 슬롯에
	SI32						siLabor;			//	노동량
	SI08						siStep;				//  몇 단계인지 나타냄
};

struct OnFarmWorkHack
{
	DWORD						dwMsgType;
	UI08						uiType;				// 미니게임 종류에 따라 짝맞추기 +10, 마작 +20, 과일받기 +30
													// 미니게임 중에 시간 해킹 0, 포인트 해킹 1, 작업량 해킹 2, 미니게임 끝난 후 구조체 Labor변경 3
	SI08						siStep;				// 단계
	SI32						siPoint1;			// 변조된 값
	SI32						siPoint2;			// 해킹을 찾기위해 숨겨놓은 원래 값 
};

struct OnResFarmWorkMsg : OnResponseMsg
{		
	UI32						uiSlotRemainLabor;	//	남은 노동량
	MONEY						siPay;				//	일해서 받은 품삯.
};

struct OnReqFarmWorkInfoMsg							//	시설물 설치(업그레이드) 메세지 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;
	UI08						uiFarmIndex;		//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에
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
	SI32						uiNum;				//	생산한 개수
	UI32						uiFinalNum;			//	최종 목표 생산수.(0이면 아이템 생산중이지 않음)
	SI16						siPay;
};

struct OnReqStorageInputItemMsg						//	창고에 아이템을 넣는 함수
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:농장, 1:목장, 2:공장, 3:광산,  (GSCdefine에 있슴.)
	UI08						uiPartID;			//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에
	UI08						uiFollowerID;		//	누구의
	SI16						siPosInInventory;	//	어디에 있는
	UI16						uiItemCode;			//	어떤 아이템을
	UI32						uiQunatity;			//	몇개를 집어 넣는가?
};

struct OnResStorageInputItemMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqStorageOutputItemMsg					//	창고서 아이템을 꺼내는 함수 
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:농장, 1:목장, 2:공장, 3:광산,  (GSCdefine에 있슴.)
	UI08						uiPartID;			//	몇번 농장
	UI08						uiSlotIndex;		//	어떤 슬롯에
	UI08						uiFollowerID;		//	누구의
	UI16						uiItemCode;			//	어떤 아이템을
	SI16						siPosInInventory;
	UI32						uiQunatity;
};

struct OnResStorageOutputItemMsg
{	
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct OnReqStorageInfoMsg			//	창고에 있는 모든 아이템을 얻어온다.(농장, 목장, 광산, 공장 모두에 쓸 예정,)
{
	DWORD						dwMsgType;
	UI08						uiPlantID;			//	0:농장, 1:목장, 2:공장, 3:광산,  (GSCdefine에 있슴.)
	UI08						uiPartID;			//	몇번 건물 (FarmIndex와 같은 개념)
	UI08						uiSlotIndex;		//	창고가 있는 슬롯
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
	UI08						uiNum;				//	아이템 개수	
	// uiNum만큼 STORAGE_ITEM 이 온다.				
};

typedef	OnMsg	OnFarmWorkCompletedMsg;				// ON_FARMWORK_COMPLETED

//-------------------------------------------------------------------------------------//


//------------------- 전투를 하고 다시 온라인 게임으로 돌아간다.  ---------------------//
struct	OnReqReturnOnlineMsg
{
	DWORD						dwMsgType;
	BOOL						dwBattleResult;			// TRUE - 승, FALSE - 패
}; 

struct	OnResReturnOnlineMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// 응답
	UI16						uiFollowerNum;			// 부하들의 수.
	MONEY						siMoney;				// 돈
	SI32						siHealth;				// 배고픔 수치.		
};

struct	OnResReturnOnlineFieldMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// 응답
	UI16						uiFollowerNum;			// 부하들의 수.
	MONEY						siMoney;				// 돈
	SI32						siHealth;				// 배고픔 수치.		
	UI16						uiPlayerNum;			// 내 주위의 플레이어 수.	
	UI16						uiFieldItemNum;			// 내 주위의 땅에 떨어진 아이템 수. 	
	SI32						siEventScore;			// 이벤트 점수.
	SI32						siEventRanking;			// 이벤트 랭킹.
	// 이 뒤에는 아이템, 플레이어, 땅에 떨어진 아이템이 대한 정보가 있다.
};

struct	OnResReturnOnlineVillageMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;				// 응답
	UI16						uiFollowerNum;			// 부하들의 수.
	MONEY						siMoney;				// 돈
	SI32						siHealth;				// 배고픔 수치.		
	UI16						uiVillageCode;			// 마을의 ID.
	OnRecvVillageInfo			VillageInfo;			// 마을의 정보
	// 이 뒤에는 부하들의 정보가 있다.
};

//-------------------------------------------------------------------------------------//

//--------------------- 플레이어(몬스터 포함)의 정보를 달라고 한다. ---------------------//
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
	// 만약 사람 플레이어일 경우 뒤에 ID가 추가된다.

    // 만약 사람 플레이어일 경우 뒤에 그 사용자의 IP가 추가된다.
	*/
};
//-------------------------------------------------------------------------------------//

//--------------------- 내 주위에 있는 캐릭터들의 대한 정보------------------------------//
struct	OnResAroundCharInfoMsg
{
	DWORD						dwMsgType;
	UI16						uiCharNum;
	// 뒤에 캐릭터들의 ID
};
//-------------------------------------------------------------------------------------//


//----------------------------- 마을, 주점에서의 메시지.------------------------------//
struct	OnReqEnterInnMsg
{
	DWORD						dwMsgType;
};

struct	OnResEnterInnMsg
{
	DWORD						dwMsgType;			
};
//-------------------------------------------------------------------------------------//


//-------------------------- 마을, 주점, 대화방에서의 메시지.--------------------------//
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

	// 방의 정보가 uiTotalChatRoom(OnInnChatRoomInfo) 만큼 따라온다.
	
	// 방의 ID가 uiTotalChatRoom(OnInnChatRoomInfo.uiChatRoomNameStringLength)만큼 따라온다.
};

// 대기실에서 메시지
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

	// 뒤에는 방제목이 따라온다.
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
	// 뒤에는 플레이어들의 정보가 온다.(OnVilINNChatPlayerInfo 형)
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

	// 방의 정보가 uiTotalChatRoom(OnInnChatRoomInfo) 만큼 따라온다.
	
	// 방의 ID가 uiTotalChatRoom(OnInnChatRoomInfo.uiChatRoomNameStringLength)만큼 따라온다.
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

//--------------------- 마을, 주점, 게시판에서의 메시지.------------------------------//
// 게시판에 들어가는 메시지
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

	UI08						uiNoticesNum;			// 게시물 글 수.

	// 뒤에는 uiNoticesNum(OnInnNBNoticeInfo) 만큼의 글이 따라온다.
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
	DWORD						dwTotalPageNum;							// 전체 페이지 수.
	DWORD						dwSearchedNoticesNum;					// 전체 게시물 수.
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

	DWORD						dwNoticeNum;				// 수정하려는 글의 인덱스
	
	UI08						uiTitleStringLength;		// 제목의 스트링 개수
	UI16						uiNoticeStringLength;		// 내용물의 스트링 개수

	// 제목이 uiTitleStringLength 바이트 만큼 제목이 뒤따라온다.

	// 새로운 글이 uiNoticeStringLength 바이트 만큼 내용물이 뒤따라온다.
};

struct	OnResModifyNoticeMsg
{
	DWORD						dwMsgType;					// ON_RESPONSE_MODIFY_NOTICE
	UI08						uiResponse;					// ON_RET_OK : 성공, ON_RET_NO : 실패
};

// 짐꾼 고용
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

	// 뒤에 짐꾼 정보(OnInnBuyWorkerInfo)가 dwBuyWorkerListCount만큼 있다.
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
//--------------------------------- 훈련소 관련 메시지 --------------------------------//
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

	// 뒤에 용병 정보(OnInnBuySolderInfo)가 dwBuySolderCount만큼 있다.
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

	// 뒤에 용병 정보(OnDismissalSolderInfo)가 dwDismissalSolderCount만큼 있다.
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
//-------------------------------------2차 전직 장수 양성소 관련 메시지 ---------------------------------//
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

	SI32						dwBuyGeneralExDiscountPercent;		//이건 빠져도 될지도.--;
	SI32						dwBuyGeneralExCount;

	// 뒤에 용병 정보(OnInnBuySolderInfo)가 dwBuyGeneralExCount만큼 있다.
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

	// 뒤에 용병 정보(OnChangeJobSolderInfo)가 dwChangeGeneralExCount만큼 있다.
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

	// 뒤에 용병 정보(OnDismissalSolderInfo)가 dwDismissalGeneralExCount 만큼 있다.
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
//--------------------------------- 장수양성소 관련 메시지 --------------------------------//
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

	// 뒤에 용병 정보(OnInnBuySolderInfo)가 dwBuySolderCount만큼 있다.
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

	// 뒤에 용병 정보(OnChangeJobSolderInfo)가 dwChangeJobSolderCount만큼 있다.
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

	// 뒤에 용병 정보(OnDismissalSolderInfo)가 dwDismissalSolderCount만큼 있다.
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
//--------------------------------- 조련소 관련 메시지 --------------------------------//
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

	// 뒤에 몬스터 정보(OnInnBuySolderInfo)가 dwBuyMonsterListCount만큼 있다.
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

	// 뒤에 몬스터 정보(OnDismissalSolderInfo)가 dwDismissMonsterCount만큼 있다.
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
//--------------------------------- 치료소 관련 메시지 --------------------------------//
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

	// 뒤에 용병 Slot(UI08)이 dwCureWorkerListCount만큼 있다.
	// 뒤에 용병 치료가격(MONEY)이 dwCureWorkerListCount만큼 있다.
	// 뒤에 용병 부활가격(MONEY)이 dwCureWorkerListCount만큼 있다.(체력이 0이면 부활가격이 있으며, 이 가격이 없으면 치료를 할수 없다.)
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
//--------------------------------- 부두 관련 메시지 ----------------------------------//
struct	OnReqWharf_Enter							// 부두에 들어간다.
{
	DWORD						dwMsgType;
};

struct	OnResWharf_Enter							// 부두에 들어간다.(응답)(Ticket List는 바로 ChangeTicketList 메세지를 보내어 처리한다.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// 현재 자신이 가진 Ticket
	SI32						siTicketKind;
};

struct	OnResWharf_ChangeTicketList					// 표의 List를 업데이트 한다.(일정시간마다 보내어 진다.)
{
	DWORD						dwMsgType;

	SI32						siTicketCount;
	SI32						siDiscountPercent;
	// 뒤에 TicketInfo(OnTicketInfo)가 siTicketCount만큼 붙어서 온다.
	//         .
	//         .
	//         .
};

struct	OnReqWharf_BuyTicket						// 부두에서 표를 산다.
{
	DWORD						dwMsgType;

	SI32						siTicketKind;
};

struct	OnResWharf_BuyTicket						// 부두에서 표를 산다.(응답)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	MONEY						NowMoney;

	// 현재 자신이 가진 Ticket
	SI32						siTicketKind;
};

struct	OnReqWharf_Exit								// 부두에서 나간다.
{
	DWORD						dwMsgType;
};

struct	OnResWharf_Exit								// 부두에서 나간다.(응답)(Ticket List는 바로 ChangeTicketList 메세지를 보내어 처리한다.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqWharfWaitRoom_Enter					// 부두대기실에 들어간다.
{
	DWORD						dwMsgType;
};

struct	OnResWharfWaitRoom_Enter					// 부두대기실에 들어간다.(응답)(만약 사용 불가능한 Ticket이라면 못들어가게 한다.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	// 현재 자신이 가진 Ticket
	SI32						siTicketKind;
};

struct	OnReqWharfWaitRoom_Exit						// 부두대기실에서 나간다.(여기서 나간다는 마을로 간다는것이다. 부두 표사는곳으로 이동하는것은 Wharf Enter로 처리한다.)
{
	DWORD						dwMsgType;
};

struct	OnResWharfWaitRoom_Exit						// 부두대기실에서 나간다.(응답)(만약 사용 불가능한 Ticket이라면 못들어가게 한다.)
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnResWharfWaitRoom_MoveInShip				// 부두대기실에서 승선하다.
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

//-------------------------------------------------------------------------------------//
//--------------------------------- 시장 들어가는 메시지 ------------------------------//
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

//-----------------------------관청에서 아이템 이동 -----------------------------------//
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

	SI16						m_Type;				// 들어갈 투자창 종류
} OnReqGovernmentEnterInvestment; 

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;

	// 마을 정보
	SI16						m_Type;						// 들어갈 투자창 종류
	SI32						m_InvestmentMinPercent;		// 최소 투자 Percent
	SI32						m_InvestmentMaxPercent;		// 최대 투자 Percent
	SI32						m_MyInvestmentCanTime;      // 투자할수있는 날짜
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

//	SI16						m_Type;				// 들어갈 투자창 종류
	SI16						siVillageCode;

} OnReqGovernmentGetInvestmentList;

typedef struct
{
	DWORD						dwMsgType;	
	DWORD						dwResponse;

	CHAR						m_MaxInvestmentID[21];		// 최고 투자자 ID

	SI16						m_Type;						// 들어갈 투자창 종류

	MONEY						m_GainingsMoney;			// 마을 수익
	BYTE						m_fGetOut;					// 투자금 회수 Flag
	BYTE						m_GetOutTex;				// 투자금 회수시 세금 Percent

	SI16						m_InvestmentListCount;		// 투자한 사람들 수

	SI32						m_MyInvestmentCanTime;		// 자신이 투자할수 있는 시간(이 시간이 0이면 제한 없음)
	// 뒤에 OnInvestmentInfo 정보가 m_InvestmentListCount 만큼 붙는다.
} OnResGovernmentGetInvestmentList;

typedef struct
{
	DWORD						dwMsgType;	

//	SI16						m_Type;						// 투자할 종류
	MONEY						m_InvestmentMoney;			// 투자 금액
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
	VDEF						CurDefencePower;					// 현재 방어력
	VDEF						MaxDefencePower;					// 최대 방어력
} OnResGovernmentVillageInfo;

struct	OnReqProfitStatistics	//	마을 수익 최근 통계 요청
{
	DWORD						dwMsgType;	
};

struct	OnResProfitStatistics	//	마을 수익 최근 통계 응답
{
	DWORD						dwMsgType;
	SI08						siNum;
	// 뒤에는 siNum 만큼 stCapital 구조체가 온다.
};

struct	OnReqGovernmentInvestmentGetOut		// 관청, 투자한돈 가지고 나가기
{
	DWORD						dwMsgType;

	MONEY						GetOutMoney;
};

struct	OnResGovernmentInvestmentGetOut		// 관청, 투자한돈 가지고 나가기 응답
{
	DWORD						dwMsgType;

	BYTE						dwSuccess;			// 성공여부. 0 : 성공, 1 : 투자금이 부족, 2 : 투자자를 못찾음

	MONEY						GetOutMoney;
	MONEY						Tex;
};

//-------------------------------------------------------------------------------------//

//-----------------------------인벤토리에서 아이템 이동 -------------------------------//

struct	OnResMoveItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqShareItemMsg									//	아이템을 둘로 쪼갠다.
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

//---------------------------------  퀘스트 관련  -------------------------------------//
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

	// 댓가
	SQuestPayData	m_QuestPay[5];

	// 뒤에 Delete 하는 Item 정보가 붙는다.(MercenaryItemData Type)
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

struct	OnReqWearItemInfoMsg	// 초기 정보
{	
	DWORD						dwMsgType;
	UI08						uiFollowerID;
};

struct	OnResWearItemInfoMsg
{
	DWORD						dwMsgType;		
	UI16						uiHelmetID;				//	투구 
	UI16						uiWeaponID;				//	무기
	UI16						uiArmorID;				//	갑옷
	UI16						uiBeltID;				//	허리띠.
	UI16						uiGloveID;				//	장갑
	UI16						uiShoesID;				//	신발
	UI16						uiRRingID;				//	오른손 반지
	UI16						uiLRingID;				//	왼손 반지.
	UI16						uiGuardianID;				//  수호부.
};

struct	OnReqStripItemMsg	//	해제
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;	
	UI08						uiWearLoc;			//	착용한 위치 (0~7)
	UI16						uiItemID;
	UI16						uiPosInventory;		// 현재 쓰던 아이템을 놓을 위치.
};

struct	OnResStripItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnReqWearItemMsg	//	착용
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI08						uiWearLoc;			//	착용하는 위치 (0~8)
	UI16						uiItemID;
	UI16						uiPosInventory;		// 새로 장착할 아이템이 있던 위치.
};

struct	OnResWearItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

struct	OnChangeClothesMsg		//	주변 사람 옷이 바꼈을때 메세지
{
	DWORD						dwMSgType;
	UI16						uiCharacID;				//	옷을 바꿔 입은 사람의 아이디.
	UI16						uiItemID[WEAR_ITEM_NUM];			//	투구, 갑옷, 무기의 아이템 코드
};

struct	OnChangeCharacterCodeMsg //	주변 사람 캐릭터코드가 바꼈을때 메시지
{
	DWORD						dwMSgType;
	UI16						uiCharacID;				//	옷을 바꿔 입은 사람의 아이디.
	UI16						uiNewCharCode;			//	새로운 캐릭터 코드.
};

struct	OnChangeCharacterNationMsg // 국적이 바꼈을때 메시지
{
	DWORD						dwMSgType;	
	UI16						uiNewCharCode;			//	새로운 국가 코드.
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

struct	OnReqReplaceItemMsg	//	바꾸기:	착용+해제
{
	DWORD						dwMsgType;
	UI08						uiFollowerID;
	UI16						uiItemID;
	UI16						uiDropPosInventory;	//	현재 쓰던 아이템을 놓을 위치.
	UI16						uiPickPosInventory;	//	새로 장착할 아이템이 있던 위치.
};

struct	OnResReplaceItemMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
//-------------------------------------------------------------------------------------//

//----------------------------- 필드 몬스터 메시지-------------------------------------//
struct	OnTargetMeMsg		//	몬스터가 자신을 노릴때 메세지
{
	DWORD						dwMsgType;
	UI16						uiMonsterID;
};

struct	OnReleaseMeMsg		//	몬스터가 안 노리게 됐을때 메세지
{
	DWORD						dwMsgType;
	UI16						uiMonsterID;
};
//-------------------------------------------------------------------------------------//

//-----------------------------  시간 관련 메시지 -------------------------------------//
struct	OnChangedTimeMsg
{
	DWORD						dwMsgType;
	SOnlineTime					Time;
	OnRealWorldTime				RealWorldTime;
};
//-------------------------------------------------------------------------------------//


//-----------------------------  던젼 관련 메시지 -------------------------------------//
struct	OnReqMoveMapMsg
{
	DWORD						dwMsgType;
	UI16						uiPortalID;					// 들어가려는 Potal의 ID
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
	
	MONEY						mnMoney;					// 새로운 돈
	UI16						uiAroundCharNum;			// 내 주위의 플레이어 수.	
	UI16						uiAroundItemNum;			// 내 주위의 땅에 떨어진 아이템 수. 
	//  플레이어, 땅에 떨어진 아이템의 대한 정보가 있다.
};
//-------------------------------------------------------------------------------------//

//-----------------------------  배 탑승 및 하차 메시지 -------------------------------//
struct	OnBoardShipMsg
{
	DWORD						dwMsgType;
	UI16						uiAccount;					// 탑승한 배의 ID	

	UI16						uiAroundCharNum;			// 내 주위의 플레이어 수.	
	UI16						uiAroundItemNum;			// 내 주위의 땅에 떨어진 아이템 수. 
	//  플레이어, 땅에 떨어진 아이템의 대한 정보가 있다.
};

struct	OnGetOutShipMsg
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;			// 마을의 ID
	OnRecvVillageInfo			VillageInfo;			// 마을의 정보
};	

struct	OnMoveShipMsg
{
	DWORD						dwMsgType;
	UI16						uiX;
	UI16						uiY;
};

//-------------------------------------------------------------------------------------//

// 보너스를 사용하여 능력치를 증가시키는 메시지
struct	OnReqIncAbilityByBonusMsg
{
	DWORD						dwMsgType;
	AbilityKindType				AbilityKind;		// AbilityKindType 중의 하나여야 한다.		
	UI08						uiFollowerID;		// Bonus를 사용하는 캐릭터의 슬롯.
};

struct	OnResIncAbilityByBonusMsg
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};
// 보너스를 사용하여 능력치를 증가시키는 메시지

// 운영자 명령 관련 메시지
struct	OnCommandMsg
{
	DWORD		dwMsgType;
	SI32		siStringLength;		// 명령 문자열의 길이

	// 뒤에는 CHAR형으로 문자열이 따라온다.
};
// 운영자 명령 관련 메시지

// 상단
struct	OnReqCreateGuildMsg			// 상단을 생성한다.(요구)
{
	DWORD		dwMsgType;			// ON_REQUEST_CREATE_GUILD
	UI08		uiGuildNameLength;	// 상단 이름 문자열의 길이

	// 뒤에는 상단의 이름이 넘어온다.
};

struct	OnResCreateGuildMsg			// 상단을 생성한다.(응답)
{
	DWORD		dwMsgType;			// ON_RESPONSE_CREATE_GUILD
	DWORD		dwResponse;			// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패
};

struct	OnReqDeleteGuildMsg			// 상단을 삭제한다.(요구)
{
	DWORD		dwMsgType;			// ON_REQUEST_DELETE_GUILD
};

struct	OnResDeleteGuildMsg			// 상단을 삭제한다.(응답)
{
	DWORD		dwMsgType;			// ON_RESPONSE_DELETE_GUILD
	DWORD		dwResponse;			// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패
};

struct	OnReqJoinLeaveGuildMsg		// 상단에 가입/탈퇴한다.(요구)
{
	DWORD		dwMsgType;			// ON_REQUEST_JOIN_LEAVE_GUILD
};

struct	OnResJoinLeaveGuildMsg		// 상단에 가입/탈퇴한다.(응답)
{
	DWORD		dwMsgType;			// ON_RESPONSE_JOIN_LEAVE_GUILD
	DWORD		dwResponse;			// 응답. 
};

struct	OnReqGuildWaitingJoinMembersListMsg		// 상단에 참여하기 원하는 참가대기자의 리스트를 얻어온다.(요구)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_WAITING_JOIN_MEMBERS_LIST	

	DWORD		dwPage;							// 총 페이지 중에서 요구하는 페이지
};

struct	OnResGuildWaitingJoinMembersListMsg		// 상단에 참여하기 원하는 참가대기자의 리스트를 얻어온다.(응답)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_WAITING_JOIN_MEMBERS_LIST
	DWORD		dwResponse;						// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패

	DWORD		dwTotalCandidateListPageNum;	// 총 지원자의 페이지 수
	DWORD		dwNowCandidateListPageNum;		// 현재 지원자의 페이지 수
	UI16		uiCandidateMembersNum;			// 이번에 보내는 지원자의 수	

	// strGuildWaitingJoinMemberInfo(OnlineCommon.h 참조)가 uiWaitingJoinMembersNum의 수만큼 나열해 있다.
};

struct	OnReqJoinGuildOkNoMsg					// 상단에 참여하기 원하는 참가 대기자에게 OK, NO를 보낸다.(요구)
{
	DWORD		dwMsgType;						// ON_REQUEST_JOINGUILD_OKNO(요구)
	DWORD		dwResponse;						// ON_RET_OK or ON_RET_NO 
	CHAR		szID[ON_ID_LENGTH];				// 해당 캐릭터의 ID
};

struct	OnResJoinGuildOkNoMsg					// 상단에 참여하기 원하는 참가 대기자에게 OK, NO를 보낸다.(응답)
{
	DWORD		dwMsgType;						// ON_RESPONSE_JOINGUILD_OKNO(응답)
	DWORD		dwResponse;						// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패(이유가 확실한 경우 ON_RET_NO_GLD_ADMIT_FULL, ON_RET_NO_GLD_ADMIT_MAXINVESTOR)
};

struct	OnReqDismissalMsg						// 상단원을 해고한다.(요구)
{
	DWORD		dwMsgType;						// ON_REQUEST_DISMISSAL
	CHAR		szID[ON_ID_LENGTH];				// 해당 캐릭터의 ID
};

struct	OnResDismissalMsg						// 상단원을 해고한다.(응답)
{
	DWORD		dwMsgType;						// ON_RESPONSE_DISMISSAL
	DWORD		dwResponse;						// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패
};

struct	OnReqGuildListMsg						// 상단의 리스트를 요구한다.(요구)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_LIST
};

struct	OnResGuildListMsg						// 상단의 리스트를 요구한다.(요구)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_LIST
	UI32		uiGuildNum;						// 총 길드의 수.	

	// strGuildInfo(OnlineCommon.h 참조)가 uiGuildNum의 수만큼 나열해 있다.
};

struct	OnReqGuildDetailInfoMsg
{
	DWORD		dwMsgType;							// ON_REQUEST_GUILD_DETAIL_INFO
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];		// 길드의 ID
};

struct	OnResGuildDetailInfoMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_DETAIL_INFO
	DWORD		dwResponse;						// ON_RET_OK : 성공, ON_RET_NO : 실패
	
	UI16		uiOwnVillageNum;				// 소유하고 있는 마을의 수.	

	// 소유하고 있는 마을들의 정보(OnGuildDetailInfo)가 uiOwnVillageNum 만큼 뒤따라온다.
};

struct	OnReqGuildMembersListMsg				// 상단원 리스트를 얻어온다.(요구)
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_MEMBERS_LIST
	UI32		uiGuidlID;						// 리스트를 원하는 상단의 ID
	DWORD		dwPage;							// 총 페이지 중에서 요구하는 페이지
};

struct	OnResGuildMembersListMsg				// 상단원 리스트를 얻어온다.(응답)
{
	DWORD		dwMsgType;						// ON_RESPONSE_GUILD_MEMBERS_LIST	
	DWORD		dwResponse;						// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패
	DWORD		dwTotalMemberListPageNum;		// 전체 페이지 수
	DWORD		dwNowMemberListPageNum;			// 현재 페이지 수
	UI32		uiMaxGuildMemebersNum;			// 이 상단의 최대 상단원 수
	UI32		uiGuildMemebersNum;				// 이번 페이지에서 보내는 상단원 수			

	// strGuildMemeberInfo(OnlineCommon.h 참조)가 uiGuildMemebersNum의 수만큼 나열해 있다.
};

struct  OnResAuditGuildMemberListMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_AUDIT_GUILD_MEMBER_LIST
	DWORD		dwResponse;						// 응답. ON_RET_OK : 성공, ON_RET_NO : 실패
	UI32		uiGuildMembersNum;				// 보내는 상단원 수			

	//strAuditGuildMemberInfo 가 uiGuildMembersNum만큼 온다.
};

struct	OnGuildMsg
{
	DWORD		dwMsgType;						// ON_GUILD_MSG
	DWORD		dwResponse;		
};

struct	OnReqGuildSetRelationMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_GUILD_SET_RELATION
	DWORD		dwResponse;						// 동맹 : ON_RET_GLD_ALLY, 적 : ON_RET_GLD_ENEMY, 중립 : ON_RET_GLD_NEUTRAL
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];	// 길드의 ID
};



struct	OnReqGuildChangeFlag
{
	DWORD		dwMsgType;						// ON_REQUEST_CHANGE_GUILD_FLAG
	SI32		siFlag;							// 변경할 깃발.
};

struct	OnResGuildChangeFlag
{
	DWORD		dwMsgType;						// ON_RESPONSE_CHANGE_GUILD_FLAG
	DWORD		dwResponse;						// 응답	
	SI32		siFlag;							// 변경된 깃발.
};

struct	OnReqMaxVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_REQUEST_MAXVILLAGEDEFENCEINVESTMENT
};

struct	OnResMaxVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_RESPONSE_MAXVILLAGEDEFENCEINVESTMENT
	DWORD		dwResponse;						// 응답

	VDEF		NowDef;							// 현재 방어력
	VDEF		MaxDef;							// 최대 방어력
	MONEY		DefPerMoney;					// 방어력 1당 돈
	MONEY		MaxMoney;						// 투자 가능한 최고 돈
};

struct	OnReqSetVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_REQUEST_SETVILLAGEDEFENCEINVESTMENT

	MONEY		InvestmentMoney;				// 투자한돈
};

struct	OnResSetVillageDefenceInvestment
{
	DWORD		dwMsgType;						// ON_RESPONSE_SETVILLAGEDEFENCEINVESTMENT
	DWORD		dwResponse;						// 응답

	VDEF		NowDef;							// 현재 방어력
};

typedef	OnMsg	OnReqUsedFlagList;				// dwMsgType = ON_REQUEST_USED_FLAG_LIST	

struct	OnResUsedFlagList
{
	DWORD		dwMsgType;						// ON_RESPONSE_USED_FLAG_LIST	
	SI32		siUsedFlagNum;					// 사용된 깃발의 수.
	UI32		uiGuildMemberNum;				// 상단원 수.

	// strFlagInfo(OnlineCommon.h 참조)가 siUsedFlagNum만큼 뒤 따라온다.
};
// 상단

// 배고픔 
struct	OnHugryMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HEALTH : Health만 깎일때, ON_RET_HUR_CHANGE_HP : HP가 깎일때.(이때 Health는 0임)
												// ON_RET_HUR_CHANGE_HEALTH의 경우 OnHugryChangeHealthMsg로 Cast.
												// ON_RET_HUR_CHANGE_HP의 경우 OnHugryChangeHPMsg로 Cast.
};

struct	OnHugryChangeHealthMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HEALTH : Health만 깎일때
	SI32		siHealth;						// 현재 배고픔(포만감) 수치.
};

struct	OnHugryChangeHPMsg
{
	DWORD		dwMsgType;						// ON_HUNGRY
	DWORD		dwResponse;						// ON_RET_HUR_CHANGE_HP : HP가 깎일때.(이때 Health는 0임)
	UI08		uiFollowerNum;					// 부하들의 수.
	
	// 뒤에는 부하들의 정보(strHungryFollowerInfo)가 uiFollowerNum만큼 따라온다.
};
// 배고픔 

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
	UI16		uiUniqueID;			// 워프해서 보내는 일반유닛 아이디.
};

//무기 타입설정만.
struct OnReqFieldAttackRdyMsg
{
	DWORD		dwMsgType;		
	UI16		uiUniqueID;		//맞는사람의 아이디.
};

struct OnResFieldAttackRdyMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	DWORD       dwTime;         //공격 실패시 남은 시간   
	UI16		uiAttackCharID;	//때린사람의 아이디.
	UI16		uiEnemyCharID;  //맞는사람의 아이디.
	UI16		uiKind;
	BYTE        bIsLong;        //원거리 유닛인가(0: 아니다 1:맞다) 
};

struct OnReqFieldAttackActionMsg
{
	DWORD		dwMsgType;		
	UI16		uiUniqueID;		//맞는사람의 아이디.
	SI16        siAttackType;	//이놈이 대장옆에 있나 없나
};

struct OnResFieldAttackActionMsg
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	UI16		uiShooterID;	//때린사람의 아이디.
	UI16		uiEnemyID;		//맞은 사람의 아이디.
	SI16		siDamage;
	BOOL		bAlive;
	SI16		siAttackType;	//이놈이 공업했나 상대방이 방업했나.체크
};

struct OnReqFieldAttackDeadMsg
{
	DWORD		dwMsgType;
	UI16		uiShooterID;	//때린사람의 아이디.
};

struct OnResFieldAttackDeadMsg
{
	DWORD		dwMsgType;
	UI16		uiShoorterID;	//때린 사람의 아이디.
	UI16		uiDeadCharID;	//죽는 사람의 아이디.
};

struct OnResDeadPenalty
{
	DWORD		dwMsgType;
};

struct OnChangedWarEnd
{
	DWORD		dwMsgType;
	UI16		uiKind;
	UI16		uiPlayerID;			//	옷을 바꿔 입은 사람의 아이디.
};


struct OnReqLongAttackVillageRdyMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;	//때린 마을의 코드(ID가 아니다.)
};

struct OnResLongAttackVillageRdyMsg  // [2/21/2005] 수정
{
	DWORD		dwMsgType;      // 메세지 타입
	DWORD		dwResponse;     // 응답 종류
	DWORD       dwTime;         // 남은 시간
	UI16		uiAttackCharID;	// 때린사람의 아이디.
	UI16		uiVillageCode;	// 마을의 코드.(ID가 아니다)	
	UI16		uiKind;
	BOOL        bIsLong;        
};

struct OnReqLongAttackVillageActionMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;	//때린 마을의 코드(ID가 아니다.)
	SI16        siAttackType;	
};


struct OnResLongAttackVillageActionMsg
{
	DWORD		dwMsgType;
	UI16		uiVillageCode;				//
	UI16		uiAttackCharID;				// 마을을 공격하는 인간의 아이디.
	SI16		siVillageDefenceDamage;		// 공격에 의해 줄어든 마을 방어력.
	SI16		siDefenceSoldierDamage;		// 공격에 의해 줄어든 수비병 수
	VDEF		vdDefence;					// 현재 마을 방어력
	SI32		siDefenceSoldier;			// 현재 마을 수비병.
	SI16		siPlayerDamage;				// 
	SI16		siAttackType;				//이놈이 공업했나 체크.
};

struct OnReqGetSupplyAtGuildUnit
{
	DWORD		dwMsgType;			//
	UI16		uiTargetID;			// 보급수레의 사람의 AccountID
	SI16		siRequireSupply;	// 보급 받고자 하는 Supply의 양
};

//주변 사람들에게 모두 가는 메세지. 자기 자신일 경우만 Supply 값 넣어주고 나머지 사람은 무시.
struct OnResGetSupplyAtGuildUnit   
{
	DWORD		dwMsgType;			// 
	/*UI16		uiUniqueID;			// 보급받는 사람의 AccountID
	SI16		siSupply;			// 보급받은 Supply
	UI16		uiTargetID;			// 보급수레의 AccountID
	SI16		siSupplyCharge;		// 보급수레의 Supply 양.
	*/
	UI16        uiUniqueID;         //요청한 사람(보급수레)
	SI16        siHeAmount;         //치료해준양
	SI16        siNowMP;            //남은 mp량
	UI16        uiTagetID;          //치료받은 유닛아이디
	SI16        siHP;               //치료받은 현재 Hp
};

struct OnReqVillageAttackTypeCheck  //마을의 AttackType을 수정할수 있나 알아본다.(요청)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;			// 마을 코드
};

struct OnResVillageAttackTypeCheck  //마을의 AttackType을 수정할수 있나 알아본다.(응답)
{
	DWORD	dwMsgType;
	DWORD	dwResponse;
	UI16    uiVillageCode;
};

struct OnReqVillagePlayerOilAttack	//마을의 AttackType을 기름공격으로 바꾼다.(요청)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
};

struct OnResVillagePlayerOilAttack	//마을의 AttackType을 기름공격으로 바꾼다.(응답)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
	DWORD	dwResponse;
	MONEY	mnMoney;
	SI32	siLimitTime;
	UI16	uiUniqueID;				
};

struct OnReqVillagePlayerStoneAttack //마을의 AttackType을 돌공격으로 바꾼다.(요청)
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
};

struct OnResVillagePlayerStoneAttack //마을의 AttackType을 돌공격으로 바꾼다.(응답)
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
	UI16	uiVillageCode;			// 마을 코드.
	VDEF	vdDefenceUp;			// 올라간 마을 방어력.
	MONEY	mnMoney;				// 소비한 유저의 돈.
	SI32	siLimitTime;
	UI16	uiUniqueID;	
	
};

struct OnChangedVillageAttackType    // 시간이 다 되어 마을 AttackType을 원래대로 돌린다. 서버->클라이언트
{
	DWORD	dwMsgType;
	UI16	uiVillageCode;
	SI16	siAttackType;
};


struct  OnReqGuildFriendlyMessageOffer
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//적대 상단을 건 길드이름.
	CHAR    szMessage[81];		
};

struct	OnResGuildFriendlyMessageOffer
{
	DWORD	dwMsgType;
	DWORD	dwResponse;			//ON_RET_OK, ON_RET_NO = 상대방이 아직 메세지를 확인하지 않았다.메세지를 보낼수 없다.
};

struct	OnReqGuildFriendlyMessageAccept
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//메세지를 준 상단의 이름.	
};

struct	OnResGuildFriendlyMessageAccept
{
	DWORD	dwMsgType;
//	DWORD	dwResponse;
};

struct	OnReqGuildFreindlyMessageCancel
{
	DWORD	dwMsgType;
	CHAR	szGuildID[ON_GUILDNAME_LENGTH+1];	//메세지를 준 상단의 이름.
};

struct OnResGuildFriendlyMessageCancel
{
	DWORD	dwMsgType;
//	DWORD	dwResponse;
};


// 마을을 공격한다는 메시지
struct	OnReqAttackVillageMsg
{
	DWORD		dwMsgtype;						// ON_REQUEST_ATTACK_VILLAGE
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)
	SI16        siAttackType;					// Attack Type
};
/*
struct	OnResAttackVillageMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_ATTACK_VILLAGE	
	UI16		uiCharID;						// 공격하는 캐릭터의 ID.
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)	
	SI16		siVillageDefenceDamage;			// 공격에 의해 줄어든 마을 방어력.
	SI16		siDefenceSoldierDamage;			// 공격에 의해 줄어든 수비병 수
	VDEF		vdDefence;						// 현재 마을 방어력
	SI32		siDefenceSoldier;				// 현재 마을 수비병.
	SI16		siPlayerDamage;					// 플레이어가 입는 데미지.
	SI16		siAttackType;			 	    //이놈이 공업했나 체크.
	UI16		uiKind;
}; 
*/
struct	OnResAttackVillageMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_ATTACK_VILLAGE	
	UI16		uiCharID;						// 공격하는 캐릭터의 ID.
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)
	SI16		siVillageDefenceDamage;         // 공격에 의해 줄어든 마을 방어력.
	VDEF		vdDefence;						// 현재 마을 방어력
	SI16		siAttackType;                   // 치명적 공격인가
	UI16		uiKind;
	BYTE        bIsLong;                        // 원거리 공격인가?(1:LONG) 
};

/*
struct	OnReqRepairVillageMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_REPAIR_VILLAGE
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)	
	VDEF		vdDefence;						// 변경된 방어력.
};

struct	OnResRepairVillageMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_REPAIR_VILLAGE
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)	
	VDEF		vdChangedDefence;				// 변경된 방어력.
};
// 마을을 공격한다는 메시지
*/

struct OnReqIncreaseGuildUnit	//상단의 공성유닛을 증가시킨다.(요청)
{
	DWORD		dwMsgType;		
	SI16		siKind;							// 종류.
	SI16		siCount;						// 추가할 갯수.
	UI16		uiVillageCode;					// 마을 코드를 준다.
};


struct OnResIncreaseGuildUnit	//상단의 공성유닛을 증가시킨다.(응답)
{
	DWORD		dwMsgType;
	SI16		siKind;							// 종류.
	SI16		siTotalCount;					// 해당 Kind의 총갯수.
	SI16		siSupply;						// 소모된 보급품의 수.
	MONEY		mnMoney;						// 소모된 돈.
	DWORD		dwResponse;						// 응답.
};	


struct OnReqDecreaseGuildUnit	//대방이 상단의 공성유닛을 인벤으로 가져간다.(요청)
{
	DWORD		dwMsgType;
	SI16		siKind;							// 종류.
	SI16		siCount;						// 갯수.
};


struct	OnResDecreaseGuildUnit  //대방이 상단의 공성유닛을 인벤으로 가져간다.(응답)
{
	DWORD		dwMsgType;
	DWORD		dwResponse;
	SI16		siKind;							// 종류.
	SI16		siTotalCount;					// 상단에 남아 있는 해당 아이템의 전체 갯수. 
	SI16		siMercenarySlot;				// 용병 슬롯.
	SI16		siInventoryPos;					// 인벤토리 슬롯.
	SI16		siItemID;						// 받아오는 아이템 아이디.
	SI16		siQuantity;						// 받아오는 아이템 아이디 갯수.
};


struct OnReqGetGuildUnitList	// 상단의 공성유닛 리스트를 얻어온다.(요청)
{
	DWORD		dwMsgType;
	UI32		uiGuildID;		//상단 아이디를 던져준다.
};

struct OnResGetGuildUnitList    // 상단의 공성유닛 리스트를 얻어온다.(응답)
{
	DWORD		dwMsgType;
	SI16		GuildCount[ON_MAX_GUILD_UNIT];	//수비병을 제외한 리스트를 얻어온다.
	UI32		uiPopulation;
};

struct OnReqIncreaseGuildSupply		//	상단의 보금품 갯수를 늘린다(요청)
{
	DWORD		dwMsgType;
	SI32		siSupplyQuantity;	//늘리려는 보급품 갯수.
	SI16		siVillageCode;		//마을 코드
};


//
struct OnResIncreaseGuildSupply		//	상단의 보금품 갯수를 늘린다(응답)
{
	DWORD		dwMsgType;			
	DWORD		dwResponse;			// 성공 실패 여부.
	MONEY		uiMoney;			// 보급품을 만드는데 사용한 돈. siSupplyQuantity * 125
	SI32		siSupplyQuantity;   // 늘린 보급품 갯수.
};


struct OnReqDecreaseGuildSupply		// 상단의 보급품을 가져온다.(요청)	
									// 보급수레만 요청할수 있다.
{
	DWORD		dwMsgType;
	SI32		siSupplyQuantity;	// 가져올 보급품 갯수. 
	SI16		siVillageCode;		// 마을 코드.
};

struct OnResDecreaseGuildSupply		//	상단의 보급품을 가져온다.(응답).
{
	DWORD		dwMsgType;
	DWORD		dwResponse;			
	SI32		siSupplyQuantity;	// 가져온 보급품 갯수.
};

struct OnReqGetGuildSupplyList		// 상단 보급품의 갯수를 가져온다(요청)
{
	DWORD		dwMsgType;
	SI16		siVillageCode;
};

struct OnResGetGuildSupplyList		// 상단 보급품의 갯수를 가져온다(응답)
{
	DWORD		dwMsgType;
	SI32		siGuildSupply;		// 상단의 총 보급품 갯수.
};


struct OnReqVillageDefenceSoldier	//  수비병의 수를 늘린다.(요청)
{
	DWORD		dwMsgType;
	SI16		siDefenceSoldierCount;  // 늘어나는 수비병의 수.
	SI16		siVillageCode;			//마을 코드
};

struct OnResVillageDefenceSoldier	//  수비병의 수를 늘린다.(응답)
{
	DWORD		dwMsgType;			
	DWORD		dwResponse;			// 응답.
	SI16		siDefenceSoldier;	// 늘어난 수비병의 수 Total 아님.
	SI16		siSupply;			// 소모된 상단 보급품의 수
	MONEY		mnMoney;			// 소모된 돈.
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
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)	
	VDEF		vdChangedDefence;				// 변경된 방어력.
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
	//siVillageCount의 갯수만큼 OnGuildResult가 따라 온다.
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
	SI16		siPercent;			//마을에 투자할수 있는 비율.
};



struct	OnReqPIInMiniMapMsg
{
	DWORD		dwMsgType;						// ON_REQUEST_PI_INMINIMAP
	CHAR		szName[ON_ID_LENGTH];			// 캐릭터 ID.
};


// OnReqPIInMiniMapMsg의 기본적은 응답 메시지는 OnResponseMsg를 사용
// dwMsgType	-	ON_RESPONSE_PI_INMINIMAP
// dwResponse	-	ON_RET_ON_FIELD : 성공, ON_RET_NOT_CONNECTED : 미접속, ON_RET_IN_VILLAGE : 마을 안에 있음, ON_RET_IN_BATTLE : 전투 중에 있음.

struct	OnResPIInMiniMapOFMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_PI_INMINIMAP
	DWORD		dwResponse;						// ON_RET_ON_VILLAGE 또는 ON_RET_IN_BATTLE
	UI16		uiKind;							// 캐릭터의 종류.
	UI16		uiMapID;						// Map ID
	UI16		uiX, uiY;						// X, Y 좌표	
};

struct	OnResPIInMiniMapIVMsg
{
	DWORD		dwMsgType;						// ON_RESPONSE_PI_INMINIMAP
	DWORD		dwResponse;						// ON_RET_IN_VILLAGE
	UI16		uiKind;							// 캐릭터의 종류.
	UI16		uiVillageCode;					// 마을의 코드.(ID가 아니다)
};

// 공지사항 - 시작!
struct	OnOATextMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_TEXT
	DWORD		dwLength;									// 텍스의 길이

	// 뒤에는 텍스트가 뒤 따라온다
};


// SMS Message
struct OnOASMSMsg
{
	DWORD	dwMsgType;			// ON_OFFICIAL_ANNOUNCEMENT
	DWORD	dwResponse;			// ON_RET_OA_SMS_MSG
	DWORD	dwCallCount;		// 위즈정보기술(주)측 일련번호
	char	szUserCode[32];		// 사용자ID : JOYON
	char	szOrgPhone[32];		// 발신가입자번호 '0161112222'
	char	szDstPhone[32];		// 착신가입자번호 (특번)
	char	szCallMessage[100];	// 호출 메시지
	char    szRecvTime[16];		// "YYYYMMDDHHMMSS"
	char	szReserved[22];		// 여유...
};

struct	OnOAVillAttackedMsg									// 마을 공격
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_VILL_ATTACKED
//	CHAR		szAttackGuildID[ON_GUILDNAME_LENGTH];		// 공격하는 상단의 ID
//	CHAR		szBeAttackedGuildID[ON_GUILDNAME_LENGTH];	// 공격 당하는 상단의 ID
	UI16		uiVillageCode;								// 공격 당하는 마을의 코드.(ID가 아니다)	
};

struct	OnOAVillOccupiedMsg									// 마을이 점령 되었다
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_VILL_OCCUPIED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// 점령한 상단의 ID.	
	UI16		uiVillageCode;								// 점령당한 마을의 코드.(ID가 아니다)		
};

struct	OnOAMaxInvChangedMsg								// 최고 투자자 변경
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_MAXINVESTMENT_CHANGED
	CHAR		szID[ON_ID_LENGTH];							// 새로운 최고 투자자의 이름.
	UI16		uiVillageCode;								// 최고 투자자가 변경된 마을의 코드.(ID가 아니다)		
};

struct	OnOAGuildCreatedMsg									// 상단 설립
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_GUILD_CREATED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// 생성된 상단의 ID.
};

struct	OnOAGuildDeletedMsg									// 상단 해체
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_GUILD_DELETED
	CHAR		szGuildID[ON_GUILDNAME_LENGTH];				// 해체된 상단의 ID.
};

struct	OnOAGuildPayingBusinessMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_PAYING_BUSINESS
	UI32		uiBuyPrice;									// 아이템을 살때의 가격
	UI32		uiSellPrice;								// 아이템을 팔때의 가격
	UI16		uiBuyVillageCode;							// 아이템을 사는 마을
	UI16		uiSellVillageCode;							// 아이템을 파는 마을
	UI16		uiItemCode;									// 아이템
};

struct	OnOADAIGetMoneyByDestroyVillageMsg
{
	DWORD		dwMsgType;									// ON_OFFICIAL_ANNOUNCEMENT
	DWORD		dwResponse;									// ON_RET_OA_DAIGETMONEYBYDESTROYVILLAGE
	MONEY		mnMoney;									// 마을을 파괴하여 받은 돈.
	UI16		uiVillageCode;								// 파괴한 마을의 Code
};

// 공지사항 - 끝!

struct	OnReqPMOpenMsg
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_OPEN
	UI16		uiItemNum;
	
	// OnBoothItemForSell이 uiItemNum만큼 뒤따라온다.
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
	
	// OnBoothItemForSell이 uiItemNum만큼 뒤따라온다.
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
	UI16		uiItemNum;			// 아이템의 개수
	
	// OnBoothItemInfo 가  uiItemNum만큼 뒤따라온다.
};

struct	OnReqChangeBoothItemMsg
{
	DWORD		dwMsgType;			// ON_REQUEST_PM_CHANGEITEM
	UI16		uiItemNum;			// 아이템의 개수

	// OnBoothItemForSell이 uiItemNum만큼 뒤따라온다.
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
	UI16				uiAccount;				// 캐릭터의 Account.
};

struct	OnResCharDetailInfoMsg : public OnResponseMsg		// ON_RESPONSE_CHAR_DETAILINFO
{
	UI16				uiAccount;				// 캐릭터의 Account.

	// 플레이어일 경우에는 뒤에 OnCharPlayerDetailInfo가 뒤따라온다.	
};

struct	OnReqVillageInfoMsg								
{		
	DWORD				dwMsgType;						// ON_REQUEST_VILLAGE_INFO
	UI16				uiVillageCode;					// 마을의 코드.(ID가 아니다)	
};

struct	OnResVillageInfoMsg : public OnResponseMsg			// ON_RESPONSE_VILLAGE_INFO
{
	UI16				uiVillageCode;					// 마을의 코드.(ID가 아니다)	
	// dwMsgType가 ON_RET_OK이면 이어서 OnRecvVillageInfo 구조체가 있다.	
};

struct	OnReqEnterStructureMsg
{	
	DWORD				dwMsgType;						// ON_REQUEST_ENTER_STRUCTURE
	SI32				siStructureKind;				// 들어가는 건물의 코드.
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
	DWORD				dwReason;						// 이유
};


struct	OnResRunProgramListMsg
{
	DWORD				dwMsgType;						// ON_RESPONSE_RUN_PROGRAM_LIST
	DWORD				dwReason;						// 이유
	SI32				siRunProgramNum;				// 현재 실행중인 프로그램의 수를 얻어온다.
	// 프로그램 수만큼 CHAR[255]가 뒤따라온다.
};

struct	OnMyVillageAttackedMsg
{
	DWORD				dwMsgType;						// ON_MYVILLAGE_ATTACKED
	UI16				uiVillageCode;					// 공격당하는 마을의 ID
};

struct	OnRequestGuildCreateCost
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_CREATE_COST
};

struct	OnResponseGuildCreateCost
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_CREATE_COST
	MONEY				mnGuildCreateCost;				// 상단 개설시 필요한 비용
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 대방이 상단원을 선택하여 특정 직급으로 임명할 때 주고 받는 패킷
struct OnRequestGuildPromotion
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_PROMOTION
	CHAR				szCharID[ON_ID_LENGTH];			// 캐릭터 ID
	SI32				siClassCode;					// 임명할 직급
};

struct OnResponseGuildPromotion
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_PROMOTION
	DWORD				dwResponse;
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 대방이 상단원을 선택하여 주어진 직급에서 해임시킬 때 주고받는 패킷
struct OnRequestGuildDemotion
{
	DWORD				dwMsgType;						// ON_REQUEST_GUILD_DEMOTION
	CHAR				szCharID[ON_ID_LENGTH];			// 캐릭터 ID
};

struct OnResponseGuildDemotion
{
	DWORD				dwMsgType;						// ON_RESPONSE_GUILD_DEMOTION
	DWORD				dwResponse;
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에 필요한 마을 투자 현황을 요청할 때 주고받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditVillageList
{
	DWORD						dwMsgType;
};

// 마을 수가 0개일 때의 response 패킷
struct	OnResAuditVillageList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

// 마을 수가 1개 이상일 때의 response 패킷
struct	OnResAuditVillageListOK
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						siVillageCount;

	// 뒤에 마을 정보(OnAuditVillageInfo)가 siVillageCount만큼 있다.
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에 필요한 친구 리스트를 요청할 때 주고받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditFriendList
{
	DWORD						dwMsgType;
};

struct	OnResAuditFriendList
{
	DWORD						dwMsgType;
	DWORD						dwResponse;

	SI32						siFriendCount;
	// siFriendCount가 1이상일 경우, 뒤에 친구 정보(OnAuditFriendInfo)가 siFriendCount만큼 있다.
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에서 친구를 수신거부 설정할 때 주고 받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditFriendSetDenial
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];			// 캐릭터 ID
	BOOL						bDenial;						// 수신거부 / 해제
};

struct	OnResAuditFriendSetDenial
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에서 친구 한명을 추가할 때 주고 받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditFriendAddOne
{
	DWORD						dwMsgType;
	CHAR						szCharID[ON_ID_LENGTH];			// 캐릭터 ID
};

struct	OnResAuditFriendAddOne
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
	BOOL						bLogin;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에서 친구를 삭제할 때 주고 받는 패킷 (체크 박스를 이용해 여러명을 보낼 수도 있다.)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에서 친구 전체를 삭제할 때 주고 받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditFriendRemoveAll
{
	DWORD						dwMsgType;
};

struct	OnResAuditFriendRemoveAll
{
	DWORD						dwMsgType;
	DWORD						dwResponse;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 친구가 로그인 했을 경우에 알려주는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnFriendLoggedInOut
{
	DWORD						dwMsgType;
	BOOL						bState;
	CHAR						szID[ON_ID_LENGTH];
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에서 특정 마을의 수익 내역을 요청할 때 주고 받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditProfitStatistics	//	특정 마을 수익 최근 통계 요청
{
	DWORD						dwMsgType;
	UI16						uiVillageCode;
};
// ※주의: 응답은 OnResProfitStatistics(이전에 관청에서 쓰이던) 구조체를 이용한다.


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 용병을 추가시킬 때 주고 받는 패킷(당나귀와 같은 경우)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnAddFollower
{
	DWORD						dwMsgType;
	OnFollowerInfo				FollowerInfo;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 전투모드, 평화모드 설정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 장부에 있는 상단 정보를 주고 받는 패킷
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct	OnReqAuditGuildMemberInfoMsg
{
	DWORD	dwMsgType;
};

struct	OnRespAuditGuildMemberInfoMsg //strAuditGuildMemberInfo
{
	DWORD	dwMsgType;	
	SI32	siLoginedGuildMemberCount;
	SI32	siNowGuildMemberCount;
	SI32	siSHNExceptMemberCount;  // 이 숫자 만큼 반복. -1 변경된 길드 정보 -2 상단원이 아니다.
};

// 독도 퀘스트를 완료한 유저의 정보 패킷
struct OnDokdoQuestCompletedUserInfo
{
	DWORD	dwMsgType;	
	UI32	uiQuestCompletedUserCount;
	char	szID[ON_ID_LENGTH];
};

// 매크로 사용자 정보 패킷
struct OnReqMacroUsingUserInfo
{
	DWORD	dwMsgType;
	char	szMacroProgName[128];
};


//공성 유닛으로 변신을 한다고 요청한다.
struct OnReqChangeUnit
{
	DWORD dwMsgType;     //메세지 타입
	SI16  siChangeKind;  //변신할 공성유닛 코드
};

struct OnResChangeUnit
{
	DWORD dwMsgType;   //메세지 타입
	DWORD dwResPonse;  //결과
	SI32  siMoney;     //변신하는데 필요한 비용   
	SI16  uiUniqueID;  //변신을 한 아이디	
	SI16  siHP;        //HP
	SI16  siMP;        //MP
	SI16  siKind;       //공성 유닛 코드
	BYTE  bQuantity;   //대방	
};

struct OnResChargeMP
{
	DWORD dwMsgType;   //메세지타입
	DWORD dwResponse;  //결과
	SI16  siNowMP;     //현재의 MP량
};

//전투 포기를 요청하였다.
struct OnAskGiveUp
{
	DWORD        dwMsgType;        //메세지 타입
	UI16         uiVillageCode;    //마을 코드
	MONEY        mnMoney;          //포기 비용 
};

struct OnReqGiveUp
{
	DWORD		dwMsgtype;
	UI16		uiVillageCode;	   // 마을의 코드.(ID가 아니다)
	UI16        uiResponse;        // 2:ok 3:no         
};

struct OnResGiveUp
{
	DWORD dwMsgType;  //메세지 타입
	DWORD dwResponse; //결과
};

//-----------------------------------------------------------------------------
// Name: OnReLoadWearItem()
// Code: Linus (2004-07-19)
// Desc: ON_RELOAD_WEAR_ITEM 신호에서 사용. Server -> Client 용
//		게임 시간으로 24시간마다 캐릭터들이 차고있는 시간제 아이템 정보를 갱신하라는 뜻이다.
//		이 구조체 후미에 OnTimerItemInfo이 siItemCount만큼 붙게 된다.
//	dwMsgType			- 이 메시지 타입(ON_RELOAD_WEAR_ITEM)
//	siMercenaryCount	- 용병 카운트 
//-----------------------------------------------------------------------------
struct OnReLoadWearItem
{
	DWORD	dwMsgType;
	SI16	siItemCount;
	//뒤에 OnTimerItemInfo siItemCount만큼 붙는다.
};

#define GAMEGUARD_PASSKEY 1004

struct OnCheckGameGuardMsg
{
	DWORD	dwMsgType;
	DWORD	dwGameGuardKey;
};

// 매크로 사용자 정보 패킷
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
