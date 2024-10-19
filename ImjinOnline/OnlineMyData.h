/************************************************************************************************
	파일명 : OnlineMyData.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	_ONLINE_MYDATA_H
#define _ONLINE_MYDATA_H

#include <GSL.h>
#include "Online.h"
#include "../SCCommon/Defines.h"
#include "../SCCommon/GSCDefine.h"
#include "../SCCommon/OnlineCommonStruct.h"
#include "../SCCommon/IOnlineFollower.h"
#include "../AuthServer/_LoginDefine.h"
#include "Friend.h"


struct  CItemHeader;


class	MyData
{
public:
	UI16				uiPropertyVillageCode;
	UI08				uiPlantID;	
	cltFriendList		m_GoodFriendList;								// 친구이름 목록
	cltFriendList		m_BadFriendList;								// 수신거부 이름 목록
	SI08				siMaxGeneral;									// 고용 가능한 장군 최대 장군수

	int					nScore;
	int					nRank;

private:		
	cltOnlineWorld*		pWorld;

	// 로그인 하기 전에 쓰이는 변수들
	CHAR				szMyAccountGuid[36];							// 나의 게정 GUID
	LOGIN::sCharInfo	MyCharacters[LOGIN::CHAR_MAX_NUM];				// 내 캐릭터들의 대한 정보.
	UI08				uiPrevSelectCharacterSlot;						// 전에 선택한 캐릭터 슬롯.
	UI08				uiSelectCharacterSlot;							// 선택한 캐릭터 슬롯.

	// 게임 서버에 로그인 한후 쓰이는 변수들
	UI16				uiMyCharUniqueID;								// 내 캐릭터의 ID.
	UI32				siMyCharDBAccount;								// 내 캐릭터의 DBAccount.
	UI16				uiMyCharKind;									// 내 캐릭터의 종류.
	char				cMyCharName[128];								// 내 캐릭터의 이름
	MONEY				uiMoney;										// 내 사용자의 돈.	
	MONEY               uiSaving;                                       // 내 사용자의 저금액.
	UI32				uiBatleCredit;									// 내 사용자의 전투 신용도.
	UI08				uiBatleCreditPercent;							// 내 사용자의 전투 신용도.
	UI32				uiTradeCredit;									// 내 사용자의 무역 신용도.
	UI08				uiTradeCreditPercent;							// 내 사용자의 무역 신용도.
	SI08				siVMercenarySlot;
	
	
	// 내 길드 정보
	BOOL				fGuild;											// 길드에 가입돼어있는지 Flag
	char				szGuildName[ON_GUILDNAME_LENGTH + 1];			// 길드 Name
	SI32				siGuildClass;									// 길드 직위
	SI32				siGuildFlag;									// 길드 깃발

	SI16				siSelectNPCID;									// Select NPC의 ID
	BOOL				bSelectNPC;										// Select NPC 상태인가
	BOOL				bGotoField;										// 필드 상으로 나가는가?
	BOOL				bGotoVillage;									// 마을에 들어가는가?	
	BOOL				bGotoPortal;									// 마을에 들어가는가?	
	BOOL				bWaitSelectNPCResponse;							// 서버로 부터 Select NPC의 응답을 기다린다.
	BOOL				bWaitGotoVillageResponse;						// 서버로 부터 마을로 들어갈 수 있는지의 응답을 기다린다.
	BOOL				bWaitGotoPortalResponse;						// 서버로 부터 마을로 들어갈 수 있는지의 응답을 기다린다.
	BOOL				bWaitBattleResponse;							// 서버로 부터 다른 플레이어와 전투를 할 수 있는지의 응답을 기다린다.
	BOOL				bWaitPickUpItemResponse;						// 서버로 부터 아이템을 줍고 있는 지의 응답을 기다린다.
	UI16				uiGotoVillageCode;								// 들어가는 마을의 Code 	
	SI32				siGotoPortalID;									// 들어가는 Portal의 ID (단순히 x 와 y 의 조합이다.)		
	SI16				siTotalHealth;									// 파티의 총 배고픔	
	IOnlineFollower		Followers;										// 부하들의 데이타.

	// robypark 2004/11/9 20:53
	// 공성전 유닛일 때 상태 값
	SI32				m_siPrevCharOnlineKind;							// 공성전 유닛일 때 사용. 변신 전 주인공 캐릭터 Online Kind ID
	bool				m_bCanTransformSiegeWarfareUnit;				// 공성전 유닛으로 변신이 가능한가?

	// robypark 2004/12/10 20:11
	BOOL				m_bIsInVillageDefenceInterface;					// 사용자가 현재 수성 인터페이스 상에 있는가?

	// robypark 2005/1/27 19:00
	BOOL				m_bIsSiegeWarfareDay;							// 오늘은 공성을 하는가?

public:
	MyData();
	~MyData();

	VOID	Initialize();
	VOID	Init(cltOnlineWorld* lpWorld);							// 변수들의 초기화를 수행하고 필요한 메모리를 할당 받는다.
	VOID	Free();													// 이 클래스에서 사용했던 메모리를 전부 해제한다.
	VOID	SetMyCharUniqueID(UI16 siCharUniqueID);					// 나의 캐릭터를 설정한다.
	UI16	GetMyCharUniqueID();									// 나의 캐릭터를 얻어온다.
	VOID	SetMyCharDBAccount(UI32 DBAccount);						// 나의 캐릭터의 DBAccount를 얻어온다.
	SI32	GetMyCharDBAccount();									// 나의 캐릭터의 DBAccount를 얻어온다.
	VOID	SetMyCharKind(UI16 uiCharKind);							// 나의 캐릭터 종류를 설정한다.
	UI16	GetMyCharKind();										// 나의 캐릭터 종류를 얻어온다.
	VOID	SetMyCharName(char* lpName);							// 나의 캐릭터 이름를 설정한다.
	char*	GetMyCharName(void);									// 나의 캐릭터 이름를 얻어한다.
	VOID	SetGuild(char* lpGuildName, SI32 GuildClass, SI32 GuildFlag);	// 나의 캐릭터 의 길드 지정
	char*	GetGuildName(void);										// 나의 캐릭터 의 길드 Name 얻기
	SI32	GetGuildClass(void);									// 나의 캐릭터 의 길드 Class 얻기 
	SI32	GetGuildFlag(void);										// 나의 캐릭터 의 길드 깃발 얻기 
	VOID	SetMoney(MONEY uiMoney);								// 현재 나의 돈을 설정한다.
	MONEY	GetMoney();												// 현재 나의 돈을 얻어온다.	
	VOID	SetSaving(MONEY uiMoney);								// 현재 나의 저금액을 설정한다.
	MONEY   ModifySaving(MONEY uiMoney);                            // 현재 나의 저금액을 변경한다.
	MONEY	GetSaving();											// 현재 나의 저금액을 얻어온다.		
	VOID	SetBatleCredit(UI32 BatleCredit, UI08 Percent);			// 현재 나의 전투 신용도를 설정한다.
	UI32	GetBatleCredit();										// 현재 나의 전투 신용도를 얻어온다.	
	UI08	GetBatleCreditPercent();								// 현재 나의 전투 신용도를 얻어온다.	
	VOID	SetTradeCredit(UI32 TradeCredit, UI08 Percent);			// 현재 나의 무역 신용도를 설정한다.
	UI32	GetTradeCredit();										// 현재 나의 무역 신용도를 얻어온다.
	UI08	GetTradeCreditPercent();								// 현재 나의 무역 신용도를 얻어온다.
	UI08	GetTradeGrade();										// 현재 나의 신용 등급을 알아온다.
	UI32	GetNextTradeCredit(UI08 uiGrade);						// 다음 신용 등급을 알아온다.
	SI32	GetHealth();											// 파티의 총 배고픔을 얻어온다.
	VOID	SetHealth( SI32	uiHealth );								// 파티의 배고픔을 설정한다.

	// robypark 2004/11/18 20:2
	SI32	GetPrevCharOnlineKind(void);							// 공성전 유닛일 때 변신 전 원래 주인공 캐릭터 Online Kind ID 얻기
	VOID	SetPrevCharOnlineKind(SI32 siPrevCharOnlineKind);		// 공성전 유닛일 때 변신 전 원래 주인공 캐릭터 Online Kind ID 설정
	bool	CanTransformSiegeWarfareUnit(void);									// 공성전 유닛으로 가능한지 얻기
	VOID	SetTransformSiegeWarfareUnit(bool bCanTransformSiegeWarfareUnit);	// 공성전 유닛으로 변신이 가능한지 설정

	VOID	SetSelectNPC(BOOL bSelectNPC, SI16 siNPCID);			// Select NPC 설정
	BOOL	IsSelectNPC();											// Select NPC로 들어가야 하는지 알아온다.
	SI16	GetSelectNPC();											// Select NPC의 이름을 얻어온다.
	BOOL	IsWaitSelectNPCResponse();								// Select NPC로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
	VOID	SetWaitSelectNPCResponse(BOOL bWaitResponse);			// Select NPC로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
	VOID	SetGotoVillage(BOOL bGoVillage, UI16 VillageCode);		// 마을로 들어가라고 설정한다.
	VOID	SetGotoPortal(BOOL bGoPortal, SI32 siPortalID);			// Portal로 들어가라고 설정한다.
	VOID	SetGotoField(BOOL bGoField);							// 필드로 나가라고 설정한다.
	BOOL	IsGotoVillage();										// 마을로 들어가야 하는지 알아온다.
	BOOL	IsGotoPortal();											// Portal로 들어가야 하는지 알아온다.
	SI32	GetGotoVillage();										// 들어가는 마을의 이름을 얻어온다.
	SI32	GetGotoPortal();										// 들어가는 Portal의 이름을 얻어온다.
	BOOL	IsGotoField();											// 필드로 가야 하는지 알아온다.	
	BOOL	IsWaitGotoVillageResponse();							// 마을로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
	VOID	SetWaitGotoVillageResponse(BOOL bWaitResponse);			// 마을로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
	BOOL	IsWaitGotoPortalResponse();								// Portal로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
	VOID	SetWaitGotoPortalResponse(BOOL bWaitResponse);			// Portal로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
	BOOL	IsWaitBattleResponse();									// 다른 플레이어와 전투를 한다고 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
	VOID	SetWaitBattleResponse(BOOL bWaitRespone);				// 다른 플레이어와 전투를 한다고 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
	BOOL	IsWaitPickUpItemResponse();								// 아이템을 줍는다고 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
	VOID	SetWaitPickUpItemResponse(BOOL bWaitResponse);			// 아이템을 줍는다고 메시지를 보내고 서버의 응답을 기다리는지 설정한다.

	UI16				FindEmptyItemBuffer(UI08 uiFollowerSlot);		// 아이템 버퍼 중에 비어있는 버퍼를 찾는다.	

	// robypark 2004/12/10 20:11
	BOOL	IsInVillageDefenceInterface(void);									// 사용자가 현재 수성 인터페이스 상에 있는가?

	// robypark 2004/12/10 20:11
	void	SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface);	// 사용자가 현재 수성 인터페이스 상에 있는가?

	// robypark 2005/1/27 19:00
	// 오늘은 공성을 하는가?
	BOOL	IsSiegeWarfareDay(void);

	// robypark 2005/1/27 19:00
	// 오늘은 공성을 하는가를 설정
	void	SetSiegeWarfareDay(BOOL bIsSiegeWarfareDay);

	// 부하 관련 함수.
	IOnlineFollower*	GetFollowersInterface();															// 부하들의 Data 관리 Class의 포인터를 얻어온다.
	VOID				InitFollowers();																	// 부하들의 대한 모든 정보를 초기화 한다.
	BOOL				InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo);					// 부하를 초기화 한다.
	BOOL				UpdateFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo);		// 부하를 수정 한다.
	BOOL				DeleteFollower(UI08 uiFollowerSlot);												// 부하를 삭제 한다.
	BOOL				IsValidFollower(UI08 uiFollowerSlot);												// 유효한 부하 Slot 인가?
	UI16				GetFollowerCharID(UI08 uiFollowerSlot);												// 부하 캐릭터의 ID를 얻어온다.
	CHAR				*GetFollowerCharName(UI08 uiFollowerSlot);											// 부하 캐릭터 이름을 얻어온다.
	VOID				SetFollowerCharName(UI08 uiFollowerSlot, CHAR *pName);								// 부하 캐릭터 이름을 설정함
	UI08				GetFollowerNum();																	// 부하 캐릭터의 총 수
	

	// Get Follower Parameter Data
	SI32	GetFollowerParameterLevel(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCurExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterPrevExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterNextExp(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterNation(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalStr(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalDex(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalVit(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalInt(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalAC(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMaxLife(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterLife(UI08 uiFollowerSlot);
	VOID	SetFollowerParameterLife(UI08 uiFollowerSlot, SI16 siLife);
	SI32	GetFollowerParameterMaxMana(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMana(UI08 uiFollowerSlot);
	VOID	SetFollowerParameterMana(UI08 uiFollowerSlot, SI16 siMana);
	SI32	GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterMaxWeight(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCurWeight(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterCredit(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterLiftPower(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterBattlePower(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterBattlePower(void);
	SI32	GetFollowerParameterKind( UI08 uiFollowerSlot );
	SI32	GetFollowerParameterBonus(UI08 uiFollowerSlot);
	SI32	GetFollowerParameterTotalItemWeight(UI08 uiFollowerSlot);
	

	// 아이템 처리 관련 함수들.		
	UI08			AddItem(UI08 uiFollowerSlot, MyItemData *pItem);										// 아이템을 아이템 리스트에 추가한다.
	UI08			DelItem(UI08 uiFollowerSlot, MyItemData *pItem	);										// 아이템을 아이템 리스트에서 삭제한다.			
	BOOL			GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem);							// 아이템을 얻어온다.
	MyItemData*		GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory);						// 아이템을 얻어온다.
	MyItemData*		GetItem(UI08 uiFollowerSlot, UI16 uiItemID);											// 아이템을 얻어온다.
	BOOL			IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory);										// 아이템이 있는지 검사
	MyItemData*		GetItemList(UI08 uiFollowerSlot);														// 아이템리스트를 돌려준다.
	BOOL			AttachItem( UI08 uiFollowerSlot, UI32 uiItemID, SI16 siGroup);							// 아이템을 장착을 한다.
	BOOL			StripItem( UI08 uiFollowerSlot, SI16 siGroup );											// 아이템을 벗는다.
	VOID			UseItem( UI08 uiFollowerSlot, SI16 siHp, SI16 siMp, SI16 siHealth);						// 아이템을 소모 한다.
	VOID			UseItemToAll( SI16 siHp, SI16 siMp, BOOL bRevive);			                        	// 모두에게 적용되는 아이템
	VOID			UseItemToAllPer( SI16 siHp, SI16 siMp, SI16 siHealth );									// 모두에게 %만큼 적용이 된다.
	SI32			GetItemCount(UI16 uiItemID);															// 가지고 있는 Item의 수를 얻어온다.

	

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////// 로그인 관련함수 /////////////////////////////////////////////////////////////////////

	VOID				SetMyCharacters(UI08 uiCharacterNum, LOGIN::sCharInfo *pMyCharactersInfo);
	LOGIN::sCharInfo*	GetMyCharacters(UI08 uiSlot);									// uiSlot = 1, 2, 3
	UI08				AddMyCharacter(CHAR *pID, WORD siKind, SI08 siNewSlot = 0);
	VOID				DeleteCharacter( UI08 uiSlot );							
	UI08				GetEmptySlot();
	VOID				SetSelectMyCharacterSlot(UI08 uiSlot);					
	UI08				GetSelectMyCharacterSlot();	
	VOID				SetPrevSelectMyCharacterSlot(UI08 uiSlot);				
	UI08				GetPrevSelectMyCharacterSlot();							

///////////////////////////////////////////////////////////////////////////////////////////////////

	VOID				InitMoveDestInfo();																// 캐릭터가 이동 했을 시의 목적지의 대한 정보를 초기화 한다.
	VOID				SetMoveDestInfo(SI16 siX, SI16 siY, SI32 siPlayerInfo, SI32 siItemInfo);		// 캐릭터가 이동 했을 시의 목적지의 대한 정보를 설정한다.

	BOOL				SetMercenary(SI08 siSlot);
	SI08				GetMercenary(void);
	BOOL				ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser 
													*pCharParser, UI16 uiFollowerID, CItem* pItem);		// 기본스탯을 빼고 모두 보너스로 전환한 수치
};

#endif

