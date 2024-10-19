/************************************************************************************************
	파일명 : OnlineMyData.cpp
	담당자 : 이윤석

	자신의 Character에 대한 정보를 관리한다.
************************************************************************************************/
#include <GSL.h>
#include <Main.h>
#include "OnlineWorld.h"
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include "OnlineItem.h"
#include "OnlineTradeParser.h"
#include "OnlineMercenary-Parser.h"
   

extern CItem				*g_pItem;


//----------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//----------------------------------------------------------------------------------------------------------------
MyData::MyData()
{	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	소멸자.
//----------------------------------------------------------------------------------------------------------------
MyData::~MyData()
{	
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	변수들의 초기화를 수행하고 필요한 메모리를 할당 받는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

VOID	MyData::Initialize()
{
	uiMyCharUniqueID			=	0;			// 내 캐릭터의 ID.
	uiMoney						=	0;
	uiBatleCredit				=	0;
	uiBatleCreditPercent		=	0;
	uiTradeCredit				=	0;
	uiTradeCreditPercent		=	0;
	siTotalHealth				=	ON_MAX_HEALTH;
	siVMercenarySlot			=	0;
	uiPropertyVillageCode		=	0;
	
	fGuild = FALSE;
	ZeroMemory(szGuildName, sizeof(szGuildName));
	siGuildClass = 0;
	siGuildFlag  = 0;

	bGotoField					=	FALSE;		// 필드 상으로 나가는가?
	bSelectNPC					=	FALSE;		// 필드 상으로 나가는가?
	bGotoVillage				=	FALSE;
	bGotoPortal					=	FALSE;
	bWaitGotoVillageResponse	=	FALSE;
	bWaitSelectNPCResponse		=	FALSE;
	bWaitGotoPortalResponse		=	FALSE;
	bWaitBattleResponse			=	FALSE;
	bWaitPickUpItemResponse		=	FALSE;

	Followers.Init();

	// 사용자가 현재 수성 인터페이스 상에 있는가?
	m_bIsInVillageDefenceInterface = FALSE;

	m_bCanTransformSiegeWarfareUnit = false;

	m_bIsSiegeWarfareDay = FALSE;
}

VOID	MyData::Init(cltOnlineWorld* lpWorld)
{	
//	SI32	i;

	pWorld						=	lpWorld;

	uiMyCharUniqueID			=	0;			// 내 캐릭터의 ID.
	uiMoney						=	0;
	uiBatleCredit				=	0;
	uiBatleCreditPercent		=	0;
	uiTradeCredit				=	0;
	uiTradeCreditPercent		=	0;
	siTotalHealth				=	ON_MAX_HEALTH;
	siVMercenarySlot			=	0;
	uiPropertyVillageCode		=	0;
	
	fGuild = FALSE;
	ZeroMemory(szGuildName, sizeof(szGuildName));
	siGuildClass = 0;
	siGuildFlag  = 0;

	bGotoField					=	FALSE;		// 필드 상으로 나가는가?
	bSelectNPC					=	FALSE;		// 필드 상으로 나가는가?
	bGotoVillage				=	FALSE;
	bGotoPortal					=	FALSE;
	bWaitGotoVillageResponse	=	FALSE;
	bWaitSelectNPCResponse		=	FALSE;
	bWaitGotoPortalResponse		=	FALSE;
	bWaitBattleResponse			=	FALSE;
	bWaitPickUpItemResponse		=	FALSE;

	Followers.Init();

	// 사용자가 현재 수성 인터페이스 상에 있는가?
	m_bIsInVillageDefenceInterface = FALSE;

	m_bCanTransformSiegeWarfareUnit = false;

	m_bIsSiegeWarfareDay = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이 클래스에서 사용했던 메모리를 전부 해제한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::Free()
{
	Followers.Free();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터를 설정한다.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMyCharUniqueID(UI16 uiCharUniqueID)
{
	uiMyCharUniqueID	=	uiCharUniqueID;	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
UI16	MyData::GetMyCharUniqueID()
{
	return uiMyCharUniqueID;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터의 DBAccount를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMyCharDBAccount(UI32 DBAccount)
{
	siMyCharDBAccount	=	DBAccount;	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터의 DBAccount를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetMyCharDBAccount()
{
	return siMyCharDBAccount;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	나의 캐릭터 종류를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetMyCharKind(UI16 uiCharKind)
{
	uiMyCharKind	=	uiCharKind;
	Followers.SetFollowerCharID(0, uiCharKind);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	나의 캐릭터 종류를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	MyData::GetMyCharKind()
{
	return uiMyCharKind;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	나의 캐릭터 이름를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetMyCharName(char* lpName)
{
	ZeroMemory(cMyCharName, 128);
	strcpy(cMyCharName, lpName);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	나의 캐릭터 이름를 얻어한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
char*	MyData::GetMyCharName(void)
{
	return (char*)cMyCharName;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터 의 길드 지정
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGuild(char* lpGuildName, SI32 GuildClass, SI32 GuildFlag)
{
	if(lpGuildName)
	{
		fGuild       = TRUE;
		CopyMemory(szGuildName, lpGuildName, ON_GUILDNAME_LENGTH);
		siGuildClass = GuildClass;
		siGuildFlag  = GuildFlag;
	}
	else
	{
		fGuild = FALSE;
		ZeroMemory(szGuildName, sizeof(szGuildName));
		siGuildClass = 0;
		siGuildFlag  = 0;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터 의 길드 Name 얻기
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
char*	MyData::GetGuildName(void)
{
	if(fGuild) return (char*)szGuildName;

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터 의 길드 Class 얻기 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGuildClass(void)
{
	return siGuildClass;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	나의 캐릭터 의 길드 깃발 얻기 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGuildFlag(void)
{
	return siGuildFlag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 돈을 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMoney(MONEY uiMoney)
{
	MyData::uiMoney	=	uiMoney > ON_MAX_MONEY ? ON_MAX_MONEY : uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 저금액을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY	MyData::GetMoney()
{
	return uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 저금액을 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetSaving(MONEY uiMoney)
{
	MyData::uiSaving	=	uiMoney > ON_MAX_SAVING ? ON_MAX_SAVING : uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 저금액을 변경하다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY	MyData::ModifySaving(MONEY uiMoney)
{
	MONEY PrevSaving = MyData::uiSaving;
	MyData::uiSaving += uiMoney;

	MyData::uiSaving	=	MyData::uiSaving > ON_MAX_SAVING ? ON_MAX_SAVING : MyData::uiSaving;

	//MyData::uiSaving  = MyData::uiSaving < 0 ? 0 : MyData::uiSaving; 

	return (MyData::uiSaving - PrevSaving);
	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 돈을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY	MyData::GetSaving()
{
	return uiSaving;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 전투 신용도를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetBatleCredit(UI32 BatleCredit, UI08 Percent)
{
	uiBatleCredit        = BatleCredit;
	uiBatleCreditPercent = Percent;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 전투 신용도를 얻어온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	MyData::GetBatleCredit()
{
	return uiBatleCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 전투 신용도를 얻어온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::GetBatleCreditPercent()
{
	return uiBatleCreditPercent;
}

// 현재 나의 신용 등급을 알아온다.
UI08	MyData::GetTradeGrade()
{
	const TradeHeader *pHeader = pWorld->pOnlineTradeParser->GetHeader(uiTradeCredit);
	return pHeader == NULL ? 0 : pHeader->uiTradeGrade;
}

// 다음 신용 등급을 알아온다.
UI32	MyData::GetNextTradeCredit(UI08 uiGrade)
{
	const TradeHeader *pHeader = pWorld->pOnlineTradeParser->GetCredit(uiGrade);
	return pHeader == NULL ? 0 : pHeader->uiAccuCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 무역 신용도를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetTradeCredit(UI32 TradeCredit, UI08 Percent)
{
	uiTradeCredit        = TradeCredit;	
	uiTradeCreditPercent = Percent;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 무역 신용도를 얻어온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	MyData::GetTradeCredit()
{
	return uiTradeCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 나의 무역 신용도를 얻어온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::GetTradeCreditPercent()
{
	return uiTradeCreditPercent;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을로 들어가라고 설정한다.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoVillage(BOOL bGoVillage, UI16 VillageCode)
{
	bGotoVillage		=	bGoVillage;	
	uiGotoVillageCode	=	VillageCode;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Select NPC 설정
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetSelectNPC(BOOL bSelectNPC, SI16 siNPCID)
{
	MyData::bSelectNPC	=	bSelectNPC;	
	siSelectNPCID		=	siNPCID;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Portal로 들어가라고 설정한다.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoPortal(BOOL bGoPortal, SI32 siPortalID)
{
	bGotoPortal		=	bGoPortal;	
	siGotoPortalID	=	siPortalID;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	필드로 나가라고 설정한다.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoField(BOOL bGoField)
{
	bGotoField	=	bGoField;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	마을로 들어가야 하는지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoVillage()
{
	return bGotoVillage;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Select NPC로 들어가야 하는지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsSelectNPC()
{
	return bSelectNPC;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Portal로 들어가야 하는지 알아온다.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoPortal()
{
	return bGotoPortal;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	들어가는 마을의 이름을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGotoVillage()
{
	return uiGotoVillageCode;
}	

//----------------------------------------------------------------------------------------------------------------
//	설명	:	Select NPC의 이름을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI16	MyData::GetSelectNPC()
{
	return siSelectNPCID;
}	

//----------------------------------------------------------------------------------------------------------------
//	설명	:	들어가는 Portal의 이름을 얻어온다.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGotoPortal()
{
	return siGotoPortalID;
}	

//----------------------------------------------------------------------------------------------------------------
//	설명	:	필드로 가야 하는지 알아온다.	
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoField()
{
	return bGotoField;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템 버퍼 중에 비어있는 버퍼를 찾는다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	MyData::FindEmptyItemBuffer(UI08 uiFollowerSlot)
{
	return Followers.FindEmptyItemBuffer(uiFollowerSlot);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에 추가한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::AddItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	return (UI08)Followers.AddItem(uiFollowerSlot, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에서 삭제한다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::DelItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	return (UI08)Followers.DelItem(uiFollowerSlot, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 아이템 리스트에서 얻어온다
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem)
{
	return Followers.GetItem(uiFollowerSlot, uiIndex, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	return Followers.GetItem(uiFollowerSlot, uiItemID, siPosInInventory);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiItemID)
{
	return Followers.GetItemByID(uiFollowerSlot, uiItemID);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템이 있는지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL		MyData::IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory)
{
	return Followers.IsItem(uiFollowerSlot, siPosInInventory);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이템리스트를 돌려준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItemList(UI08 uiFollowerSlot)
{
	return Followers.GetItemList(uiFollowerSlot);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitGotoVillageResponse(BOOL bWaitResponse)
{
	bWaitGotoVillageResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Select NPC로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitSelectNPCResponse()
{
	return bWaitSelectNPCResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	마을로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitGotoVillageResponse()
{
	return bWaitGotoVillageResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Select NPC로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitSelectNPCResponse(BOOL bWaitResponse)
{
	bWaitSelectNPCResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Portal로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitGotoPortalResponse(BOOL bWaitResponse)
{
	bWaitGotoPortalResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	Portal로 들어갈때 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitGotoPortalResponse()
{
	return bWaitGotoPortalResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	다른 플레이어와 전투를 한다고 메시지를 보내고 서버의 응답을 기다리는지를 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitBattleResponse(BOOL bWaitRespone)
{	
	bWaitBattleResponse	=	bWaitRespone;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	다른 플레이어와 전투를 한다고 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitBattleResponse()
{
	return bWaitBattleResponse;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 줍는다고 메시지를 보내고 서버의 응답을 기다리는지 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetWaitPickUpItemResponse(BOOL bWaitResponse)
{
	bWaitPickUpItemResponse	=	bWaitResponse;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 줍는다고 메시지를 보내고 서버의 응답을 기다리는지 알아온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::IsWaitPickUpItemResponse()
{
	return bWaitPickUpItemResponse;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	서버로부터 받은 나의 캐릭터들의 대한 정보를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetMyCharacters(UI08 uiCharacterNum, LOGIN::sCharInfo* pMyCharactersInfo)
{
	ZeroMemory(MyCharacters, sizeof(LOGIN::sCharInfo) * LOGIN::CHAR_MAX_NUM);
	memcpy(MyCharacters, pMyCharactersInfo, sizeof(LOGIN::sCharInfo) * uiCharacterNum);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	서버로부터 받은 나의 캐릭터들의 대한 정보를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
LOGIN::sCharInfo*	MyData::GetMyCharacters(UI08 uiSlot)
{
	SI32				i;

	if(uiSlot == 0)	return NULL;

	for(i = 0; i < LOGIN::CHAR_MAX_NUM; i++)
	{
		if(MyCharacters[i].slot == uiSlot)
			return &MyCharacters[i];
	}

	return NULL;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	슬롯에 캐릭터 추가함(캐릭생성)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	MyData::AddMyCharacter( CHAR *pID, WORD siKind, SI08 siNewSlot )
{
	SI16	i;
	UI08	uiFindEmptySlot;

	// 추가할 캐릭의 슬롯이 정해지지 않으면 빈슬롯을 찾는다.
	if(!siNewSlot)
		uiFindEmptySlot = GetEmptySlot();
	else
		uiFindEmptySlot = siNewSlot;
	
	for(i = 0; i < LOGIN::CHAR_MAX_NUM; i++)
	{
		if(MyCharacters[i].slot == 0)
		{
			ZeroMemory(&MyCharacters[i], sizeof(LOGIN::sCharInfo));
			MyCharacters[i].slot = uiFindEmptySlot;
			MyCharacters[i].kind = siKind;
			strncpy( MyCharacters[i].name, pID, ON_ID_LENGTH);

			return MyCharacters[i].slot;
		}	
	}

	return 0;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 지운다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::DeleteCharacter( UI08 uiSlot )
{
	UI08 i;
	for( i=0; i< LOGIN::CHAR_MAX_NUM ; i++)
	{
		if( MyCharacters[i].slot == uiSlot )		
			ZeroMemory( &MyCharacters[i], sizeof( MyCharacters[i] ) );			
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	비어있는 슬롯을 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	MyData::GetEmptySlot()
{
	SI16	i;
	
	for(i = 0; i < LOGIN::CHAR_MAX_NUM; i++)
	{
		if(MyCharacters[i].slot == 0)
			return (i + 1);
	}
	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	나의 캐릭터를 선택한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetSelectMyCharacterSlot(UI08 uiSlot)
{
	uiSelectCharacterSlot	=	uiSlot;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	선택한 나의 캐릭터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	MyData::GetSelectMyCharacterSlot()
{
	return uiSelectCharacterSlot;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	전에 선택한 캐릭터 슬롯을 설정한다.,
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::SetPrevSelectMyCharacterSlot(UI08 uiSlot)
{
	uiPrevSelectCharacterSlot	=	uiSlot;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	전에 선택한 캐릭터 슬롯을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	MyData::GetPrevSelectMyCharacterSlot()
{
	return uiPrevSelectCharacterSlot;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	유효한 부하 슬롯인지 검사한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::IsValidFollower(UI08 uiFollowerSlot)
{
	return Followers.IsValidFollower(uiFollowerSlot);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하들의 Data 관리 Class의 포인터를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
IOnlineFollower* MyData::GetFollowersInterface()
{
	return &Followers;
}
 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하들의 대한 모든 정보를 초기화 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	MyData::InitFollowers()
{
	Followers.Init();
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하 캐릭터의 ID를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI16	MyData::GetFollowerCharID(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerCharID(uiFollowerSlot);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하를 초기화 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo)
{
	return Followers.InitFollower(uiFollowerSlot, lpFollowerInfo, pWorld->pIOnlineCharKI->GetNation(pWorld->pIOnlineCharKI->GetIndex(lpFollowerInfo->uiKind)), g_pItem);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하를 수정 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::UpdateFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo)
{
	return Followers.UpdataFollower(uiFollowerSlot, lpFollowerInfo, pWorld->pIOnlineCharKI->GetNation(pWorld->pIOnlineCharKI->GetIndex(lpFollowerInfo->uiKind)), g_pItem);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	부하를 삭제 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::DeleteFollower(UI08 uiFollowerSlot)
{
	return Followers.DeleteFollower(uiFollowerSlot);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get Follower Parameter Data
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	MyData::GetFollowerParameterLevel(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterLevel(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterCurExp(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterCurExp(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterPrevExp(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterPrevExp(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterNextExp(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterNextExp(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterNation(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterNation(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalStr(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalStr(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalDex(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalDex(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalVit(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalVit(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalInt(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalInt(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalAC(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalAC(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalDamageResist(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalMagicResist(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalEquipMinDamage(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterTotalEquipMaxDamage(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterMaxLife(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterMaxLife(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterLife(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterLife(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterMaxMana(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterMaxMana(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterMana(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterMana(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterMoveSpeed(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterMaxWeight(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterMaxWeight(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterTotalItemWeight(UI08 uiFollowerSlot)
{
	return Followers.GetTotalItemWeight(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterCurWeight(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterCurWeight(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterCredit(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterCredit(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterLiftPower(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterLiftPower(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterBattlePower(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerParameterBattlePower(uiFollowerSlot);
}

SI32	MyData::GetFollowerParameterBattlePower(void)
{
	SI32	TempBattlePower;
	SI32	TempCount;

	TempBattlePower = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		TempBattlePower += GetFollowerParameterBattlePower(TempCount);
	}

	return TempBattlePower;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	아이템을 장착을 한다.
//	수정 일자	:	2002-07-13 오후 8:42:27 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	MyData::AttachItem(UI08 uiFollowerSlot, UI32 uiItemID, SI16 siGroup)
{
	return	Followers.SetWearItem2(uiFollowerSlot, UI08(siGroup), UI16(uiItemID));	
}

/////////////////////////////////////////////////
// 아이템을 벗는다.
BOOL	MyData::StripItem( UI08 uiFollowerSlot, SI16 siGroup )
{
	OnlineFollower	*pFollower = Followers.GetFollower(uiFollowerSlot);
	if(pFollower == NULL )			return FALSE;

	pFollower->Weapon.SetWearItem( (UI08)siGroup, 0 ,0);	
	return TRUE;
}

/////////////////////////////////////////////////
// 아이템을 소모 한다.
VOID	MyData::UseItem( UI08 uiFollowerSlot, SI16 siHp, SI16 siMp, SI16 siHealth )
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
	{
		OnlineFollower	*pFollower		=	Followers.GetFollower(uiFollowerSlot);
		SI16			siTempHp		=	pFollower->ParametaBox.IP.GetLife() + siHp;
		SI16			siTempMp		=	pFollower->ParametaBox.IP.GetMana() + siMp;
		//siTotalHealth += siHealth;


		if( siTempHp > pFollower->ParametaBox.GetMaxLife() )	pFollower->ParametaBox.IP.SetLife( pFollower->ParametaBox.GetMaxLife() );
		else													pFollower->ParametaBox.IP.SetLife( siTempHp );

		if( siTempMp > pFollower->ParametaBox.GetMaxMana() )	pFollower->ParametaBox.IP.SetMana( pFollower->ParametaBox.GetMaxMana() );
		else													pFollower->ParametaBox.IP.SetMana( siTempMp );		

		//if( siTotalHealth > ON_MAX_HEALTH )						siTotalHealth	=	ON_MAX_HEALTH;
	}
}

VOID	MyData::UseItemToAll( SI16 siHp, SI16 siMp, BOOL bRevive )
{
	int maxfollower = ON_MAX_FOLLOWER_NUM;
	for (int i = 0; i < maxfollower; i++)
	{
		if( Followers.IsValidFollower( i ) )
		{
			OnlineFollower	*pFollower		=	Followers.GetFollower(i);

			SI16			siTempHp		=	pFollower->ParametaBox.IP.GetLife() + siHp;
			SI16			siTempMp		=	pFollower->ParametaBox.IP.GetMana() + siMp;

			// 부활아이템이 아니면
			if (bRevive == FALSE)
			{
				// 체력이 0보다 작으면 적용되지않는다.
				if (pFollower->ParametaBox.IP.GetLife() <= 0)
				{
					continue;
				}
			}
			
				
			if( siTempHp > pFollower->ParametaBox.GetMaxLife() )	pFollower->ParametaBox.IP.SetLife( pFollower->ParametaBox.GetMaxLife() );
			else													pFollower->ParametaBox.IP.SetLife( siTempHp );
			
			if( siTempMp > pFollower->ParametaBox.GetMaxMana() )	pFollower->ParametaBox.IP.SetMana( pFollower->ParametaBox.GetMaxMana() );
			else													pFollower->ParametaBox.IP.SetMana( siTempMp );		
			
			
		}
	}
}

VOID	MyData::UseItemToAllPer( SI16 siHp, SI16 siMp, SI16 siHealth )
{
	int maxfollower = ON_MAX_FOLLOWER_NUM;
	for (int i = 0; i < maxfollower; i++)
	{
		if( Followers.IsValidFollower( i ) )
		{
			OnlineFollower	*pFollower	=	Followers.GetFollower(i);

			SI16 siTempHp	=	((float)pFollower->ParametaBox.GetMaxLife() * ((float)siHp / 100.0 )) + pFollower->ParametaBox.IP.GetLife();
			SI16 siTempMp	=	((float)pFollower->ParametaBox.GetMaxMana() * ((float)siMp / 100.0 )) + pFollower->ParametaBox.IP.GetMana();
			
			siTempHp = siTempHp >= pFollower->ParametaBox.GetMaxLife() ? pFollower->ParametaBox.GetMaxLife() : siTempHp;
			siTempMp = siTempMp >= pFollower->ParametaBox.GetMaxMana() ? pFollower->ParametaBox.GetMaxMana() : siTempMp;
			
			pFollower->ParametaBox.IP.SetLife( siTempHp );
			pFollower->ParametaBox.IP.SetMana( siTempMp );
		}
	}
}


/////////////////////////////////////////////////
// 가지고 있는 Item의 수를 얻어온다.
SI32	MyData::GetItemCount(UI16 uiItemID)
{
	SI32	TempCount;
	SI32	TempItemCount;

	TempItemCount = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(Followers.IsValidFollower((UI08)TempCount) == TRUE)
		{
			TempItemCount = Followers.GetItemCount(uiItemID);
		}
	}

	return TempItemCount;
}

/////////////////////////////////////////////////
// 부하 캐릭터 이름을 얻어온다.
CHAR	*MyData::GetFollowerCharName(UI08 uiFollowerSlot)
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
	{
		if(uiFollowerSlot == 0)
		{
			return GetMyCharName();
		}
		else
		{
			return Followers.GetFollowerName( uiFollowerSlot );
		}
	}

	return NULL;
}

/////////////////////////////////////////////////
// 부하 캐릭터 이름을 설정함
VOID	MyData::SetFollowerCharName( UI08 uiFollowerSlot, CHAR *pName )
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
		Followers.SetFollowerName( uiFollowerSlot, pName );
}

/////////////////////////////////////////////////
// 배고픔 수치를 얻어 온다.( 100% )
SI32	MyData::GetHealth()
{
	return siTotalHealth;
}

VOID	MyData::SetFollowerParameterLife(UI08 uiFollowerSlot, SI16 siLife )
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
		Followers.SetFollowerParameterLife( uiFollowerSlot, siLife );

	
}

VOID	MyData::SetHealth( SI32 siHealth )
{
	siTotalHealth = (SI16)siHealth;
}

SI32	MyData::GetFollowerParameterKind( UI08 uiFollowerSlot )
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
	{
		if(uiFollowerSlot == 0)
		{
			return GetMyCharKind();
		}
		else
		{
			return Followers.GetFollowerParameterKind( uiFollowerSlot );
		}
	}

	return 0;
}

SI32	MyData::GetFollowerParameterBonus(UI08 uiFollowerSlot)
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
	{
		return Followers.GetFollowerBaseParameterBonus(uiFollowerSlot);
	}

	return -1;
}

UI08	MyData::GetFollowerNum()
{
	return Followers.GetFollowerNum();
}

BOOL	MyData::SetMercenary(SI08 siSlot)
{
	siVMercenarySlot = siSlot;

	return TRUE;
}

SI08	MyData::GetMercenary(void)
{
	return siVMercenarySlot;
}

BOOL	MyData::ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser *pCharParser, UI16 uiFollowerID, CItem* pItem)
{
	SI32 *pBonus = NULL;
	return Followers.ChangeFollowerParameterToBonus(uiFollowerSlot, pCharParser, uiFollowerID , pBonus, pItem);
}

// robypark 2004/11/18 20:3
// 공성전 유닛일 때 변신 전 원래 주인공 캐릭터 Online Kind ID 얻기
SI32	MyData::GetPrevCharOnlineKind(void)
{
	return m_siPrevCharOnlineKind;
}

// robypark 2004/11/18 20:3
// 공성전 유닛일 때 변신 전 원래 주인공 캐릭터 Online Kind ID 설정
VOID	MyData::SetPrevCharOnlineKind(SI32 siPrevCharOnlineKind)
{
	m_siPrevCharOnlineKind = siPrevCharOnlineKind;
}

// 공성전 유닛으로 가능한지 얻기
bool	MyData::CanTransformSiegeWarfareUnit(void)
{
	return m_bCanTransformSiegeWarfareUnit;
}

// 공성전 유닛으로 변신이 가능한지 설정
VOID	MyData::SetTransformSiegeWarfareUnit(bool bCanTransformSiegeWarfareUnit)
{
	m_bCanTransformSiegeWarfareUnit = bCanTransformSiegeWarfareUnit;
}

// robypark 2004/12/10 20:11
// 사용자가 현재 수성 인터페이스 상에 있는가?
BOOL	MyData::IsInVillageDefenceInterface(void)
{
	return m_bIsInVillageDefenceInterface;
}

// robypark 2004/12/10 20:11
// 사용자가 현재 수성 인터페이스 상에 있는가?
void	MyData::SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface)
{
	m_bIsInVillageDefenceInterface = bIsInVillageDefenceInterface;
}

// robypark 2005/1/27 19:00
// 오늘은 공성을 하는가?
BOOL	MyData::IsSiegeWarfareDay(void)
{
	return m_bIsSiegeWarfareDay;
}

// robypark 2005/1/27 19:00
// 오늘은 공성을 하는가를 설정
void	MyData::SetSiegeWarfareDay(BOOL bIsSiegeWarfareDay)
{
	m_bIsSiegeWarfareDay = bIsSiegeWarfareDay;
}