/************************************************************************************************
	だ橾貲 : OnlineMyData.cpp
	氬渡濠 : 檜彌戮

	濠褐曖 Character縑 渠и 薑爾蒂 婦葬и棻.
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
//	撲貲	:	儅撩濠.
//----------------------------------------------------------------------------------------------------------------
MyData::MyData()
{	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	模資濠.
//----------------------------------------------------------------------------------------------------------------
MyData::~MyData()
{	
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	滲熱菟曖 蟾晦�飛� 熱чж堅 в蹂и 詭賅葬蒂 й渡 嫡朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

VOID	MyData::Initialize()
{
	uiMyCharUniqueID			=	0;			// 頂 議葛攪曖 ID.
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

	bGotoField					=	FALSE;		// в萄 鼻戲煎 釭陛朝陛?
	bSelectNPC					=	FALSE;		// в萄 鼻戲煎 釭陛朝陛?
	bGotoVillage				=	FALSE;
	bGotoPortal					=	FALSE;
	bWaitGotoVillageResponse	=	FALSE;
	bWaitSelectNPCResponse		=	FALSE;
	bWaitGotoPortalResponse		=	FALSE;
	bWaitBattleResponse			=	FALSE;
	bWaitPickUpItemResponse		=	FALSE;

	Followers.Init();

	// 餌辨濠陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛?
	m_bIsInVillageDefenceInterface = FALSE;

	m_bCanTransformSiegeWarfareUnit = false;

	m_bIsSiegeWarfareDay = FALSE;
}

VOID	MyData::Init(cltOnlineWorld* lpWorld)
{	
//	SI32	i;

	pWorld						=	lpWorld;

	uiMyCharUniqueID			=	0;			// 頂 議葛攪曖 ID.
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

	bGotoField					=	FALSE;		// в萄 鼻戲煎 釭陛朝陛?
	bSelectNPC					=	FALSE;		// в萄 鼻戲煎 釭陛朝陛?
	bGotoVillage				=	FALSE;
	bGotoPortal					=	FALSE;
	bWaitGotoVillageResponse	=	FALSE;
	bWaitSelectNPCResponse		=	FALSE;
	bWaitGotoPortalResponse		=	FALSE;
	bWaitBattleResponse			=	FALSE;
	bWaitPickUpItemResponse		=	FALSE;

	Followers.Init();

	// 餌辨濠陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛?
	m_bIsInVillageDefenceInterface = FALSE;

	m_bCanTransformSiegeWarfareUnit = false;

	m_bIsSiegeWarfareDay = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜 贗楚蝶縑憮 餌辨ц湍 詭賅葬蒂 瞪睡 п薯и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::Free()
{
	Followers.Free();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪蒂 撲薑и棻.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMyCharUniqueID(UI16 uiCharUniqueID)
{
	uiMyCharUniqueID	=	uiCharUniqueID;	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
UI16	MyData::GetMyCharUniqueID()
{
	return uiMyCharUniqueID;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪曖 DBAccount蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMyCharDBAccount(UI32 DBAccount)
{
	siMyCharDBAccount	=	DBAccount;	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪曖 DBAccount蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetMyCharDBAccount()
{
	return siMyCharDBAccount;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	釭曖 議葛攪 謙盟蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetMyCharKind(UI16 uiCharKind)
{
	uiMyCharKind	=	uiCharKind;
	Followers.SetFollowerCharID(0, uiCharKind);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	釭曖 議葛攪 謙盟蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	MyData::GetMyCharKind()
{
	return uiMyCharKind;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	釭曖 議葛攪 檜葷蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetMyCharName(char* lpName)
{
	ZeroMemory(cMyCharName, 128);
	strcpy(cMyCharName, lpName);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	釭曖 議葛攪 檜葷蒂 橢橫и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
char*	MyData::GetMyCharName(void)
{
	return (char*)cMyCharName;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪 曖 望萄 雖薑
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
//	撲貲	:	釭曖 議葛攪 曖 望萄 Name 橢晦
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
char*	MyData::GetGuildName(void)
{
	if(fGuild) return (char*)szGuildName;

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪 曖 望萄 Class 橢晦 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGuildClass(void)
{
	return siGuildClass;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	釭曖 議葛攪 曖 望萄 梓嫦 橢晦 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGuildFlag(void)
{
	return siGuildFlag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 絲擊 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetMoney(MONEY uiMoney)
{
	MyData::uiMoney	=	uiMoney > ON_MAX_MONEY ? ON_MAX_MONEY : uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 盪旎擋擊 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY	MyData::GetMoney()
{
	return uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 盪旎擋擊 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetSaving(MONEY uiMoney)
{
	MyData::uiSaving	=	uiMoney > ON_MAX_SAVING ? ON_MAX_SAVING : uiMoney;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 盪旎擋擊 滲唳ж棻.
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
//	撲貲	:	⑷營 釭曖 絲擊 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MONEY	MyData::GetSaving()
{
	return uiSaving;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 瞪癱 褐辨紫蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetBatleCredit(UI32 BatleCredit, UI08 Percent)
{
	uiBatleCredit        = BatleCredit;
	uiBatleCreditPercent = Percent;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 瞪癱 褐辨紫蒂 橢橫螞棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	MyData::GetBatleCredit()
{
	return uiBatleCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 瞪癱 褐辨紫蒂 橢橫螞棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::GetBatleCreditPercent()
{
	return uiBatleCreditPercent;
}

// ⑷營 釭曖 褐辨 蛔晝擊 憲嬴螞棻.
UI08	MyData::GetTradeGrade()
{
	const TradeHeader *pHeader = pWorld->pOnlineTradeParser->GetHeader(uiTradeCredit);
	return pHeader == NULL ? 0 : pHeader->uiTradeGrade;
}

// 棻擠 褐辨 蛔晝擊 憲嬴螞棻.
UI32	MyData::GetNextTradeCredit(UI08 uiGrade)
{
	const TradeHeader *pHeader = pWorld->pOnlineTradeParser->GetCredit(uiGrade);
	return pHeader == NULL ? 0 : pHeader->uiAccuCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 鼠羲 褐辨紫蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetTradeCredit(UI32 TradeCredit, UI08 Percent)
{
	uiTradeCredit        = TradeCredit;	
	uiTradeCreditPercent = Percent;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 鼠羲 褐辨紫蒂 橢橫螞棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	MyData::GetTradeCredit()
{
	return uiTradeCredit;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 釭曖 鼠羲 褐辨紫蒂 橢橫螞棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::GetTradeCreditPercent()
{
	return uiTradeCreditPercent;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊煎 菟橫陛塭堅 撲薑и棻.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoVillage(BOOL bGoVillage, UI16 VillageCode)
{
	bGotoVillage		=	bGoVillage;	
	uiGotoVillageCode	=	VillageCode;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Select NPC 撲薑
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetSelectNPC(BOOL bSelectNPC, SI16 siNPCID)
{
	MyData::bSelectNPC	=	bSelectNPC;	
	siSelectNPCID		=	siNPCID;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Portal煎 菟橫陛塭堅 撲薑и棻.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoPortal(BOOL bGoPortal, SI32 siPortalID)
{
	bGotoPortal		=	bGoPortal;	
	siGotoPortalID	=	siPortalID;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	в萄煎 釭陛塭堅 撲薑и棻.
//----------------------------------------------------------------------------------------------------------------
VOID	MyData::SetGotoField(BOOL bGoField)
{
	bGotoField	=	bGoField;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊煎 菟橫陛撿 ж朝雖 憲嬴螞棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoVillage()
{
	return bGotoVillage;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Select NPC煎 菟橫陛撿 ж朝雖 憲嬴螞棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsSelectNPC()
{
	return bSelectNPC;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Portal煎 菟橫陛撿 ж朝雖 憲嬴螞棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoPortal()
{
	return bGotoPortal;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	菟橫陛朝 葆擊曖 檜葷擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGotoVillage()
{
	return uiGotoVillageCode;
}	

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	Select NPC曖 檜葷擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
SI16	MyData::GetSelectNPC()
{
	return siSelectNPCID;
}	

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	菟橫陛朝 Portal曖 檜葷擊 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
SI32	MyData::GetGotoPortal()
{
	return siGotoPortalID;
}	

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	в萄煎 陛撿 ж朝雖 憲嬴螞棻.	
//----------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsGotoField()
{
	return bGotoField;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱 幗ぷ 醞縑 綠橫氈朝 幗ぷ蒂 瓊朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	MyData::FindEmptyItemBuffer(UI08 uiFollowerSlot)
{
	return Followers.FindEmptyItemBuffer(uiFollowerSlot);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑 蹺陛и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::AddItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	return (UI08)Followers.AddItem(uiFollowerSlot, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 餉薯и棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI08	MyData::DelItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	return (UI08)Followers.DelItem(uiFollowerSlot, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 嬴檜蠱 葬蝶お縑憮 橢橫螞棻
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem)
{
	return Followers.GetItem(uiFollowerSlot, uiIndex, pItem);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	return Followers.GetItem(uiFollowerSlot, uiItemID, siPosInInventory);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱擊 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItem(UI08 uiFollowerSlot, UI16 uiItemID)
{
	return Followers.GetItemByID(uiFollowerSlot, uiItemID);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱檜 氈朝雖 匐餌
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL		MyData::IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory)
{
	return Followers.IsItem(uiFollowerSlot, siPosInInventory);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嬴檜蠱葬蝶お蒂 給溥遽棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	MyData::GetItemList(UI08 uiFollowerSlot)
{
	return Followers.GetItemList(uiFollowerSlot);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitGotoVillageResponse(BOOL bWaitResponse)
{
	bWaitGotoVillageResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Select NPC煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 憲嬴螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitSelectNPCResponse()
{
	return bWaitSelectNPCResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	葆擊煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 憲嬴螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitGotoVillageResponse()
{
	return bWaitGotoVillageResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Select NPC煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitSelectNPCResponse(BOOL bWaitResponse)
{
	bWaitSelectNPCResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Portal煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitGotoPortalResponse(BOOL bWaitResponse)
{
	bWaitGotoPortalResponse	=	bWaitResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Portal煎 菟橫陞陽 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 憲嬴螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitGotoPortalResponse()
{
	return bWaitGotoPortalResponse;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	棻艇 Ы溯檜橫諦 瞪癱蒂 и棻堅 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	MyData::SetWaitBattleResponse(BOOL bWaitRespone)
{	
	bWaitBattleResponse	=	bWaitRespone;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	棻艇 Ы溯檜橫諦 瞪癱蒂 и棻堅 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 憲嬴螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	MyData::IsWaitBattleResponse()
{
	return bWaitBattleResponse;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 鄹朝棻堅 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetWaitPickUpItemResponse(BOOL bWaitResponse)
{
	bWaitPickUpItemResponse	=	bWaitResponse;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 鄹朝棻堅 詭衛雖蒂 爾頂堅 憮幗曖 擬港擊 晦棻葬朝雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::IsWaitPickUpItemResponse()
{
	return bWaitPickUpItemResponse;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	憮幗煎睡攪 嫡擎 釭曖 議葛攪菟曖 渠и 薑爾蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetMyCharacters(UI08 uiCharacterNum, LOGIN::sCharInfo* pMyCharactersInfo)
{
	ZeroMemory(MyCharacters, sizeof(LOGIN::sCharInfo) * LOGIN::CHAR_MAX_NUM);
	memcpy(MyCharacters, pMyCharactersInfo, sizeof(LOGIN::sCharInfo) * uiCharacterNum);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	憮幗煎睡攪 嫡擎 釭曖 議葛攪菟曖 渠и 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蝸煜縑 議葛攪 蹺陛л(議葛儅撩)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	MyData::AddMyCharacter( CHAR *pID, WORD siKind, SI08 siNewSlot )
{
	SI16	i;
	UI08	uiFindEmptySlot;

	// 蹺陛й 議葛曖 蝸煜檜 薑п雖雖 彊戲賊 綴蝸煜擊 瓊朝棻.
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 雖遴棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::DeleteCharacter( UI08 uiSlot )
{
	UI08 i;
	for( i=0; i< LOGIN::CHAR_MAX_NUM ; i++)
	{
		if( MyCharacters[i].slot == uiSlot )		
			ZeroMemory( &MyCharacters[i], sizeof( MyCharacters[i] ) );			
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	綠橫氈朝 蝸煜擊 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	釭曖 議葛攪蒂 摹鷗и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetSelectMyCharacterSlot(UI08 uiSlot)
{
	uiSelectCharacterSlot	=	uiSlot;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	摹鷗и 釭曖 議葛攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	MyData::GetSelectMyCharacterSlot()
{
	return uiSelectCharacterSlot;
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪縑 摹鷗и 議葛攪 蝸煜擊 撲薑и棻.,
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::SetPrevSelectMyCharacterSlot(UI08 uiSlot)
{
	uiPrevSelectCharacterSlot	=	uiSlot;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞪縑 摹鷗и 議葛攪 蝸煜擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	MyData::GetPrevSelectMyCharacterSlot()
{
	return uiPrevSelectCharacterSlot;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 睡ж 蝸煜檣雖 匐餌и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::IsValidFollower(UI08 uiFollowerSlot)
{
	return Followers.IsValidFollower(uiFollowerSlot);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж菟曖 Data 婦葬 Class曖 ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IOnlineFollower* MyData::GetFollowersInterface()
{
	return &Followers;
}
 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж菟曖 渠и 賅萇 薑爾蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	MyData::InitFollowers()
{
	Followers.Init();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж 議葛攪曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	MyData::GetFollowerCharID(UI08 uiFollowerSlot)
{
	return Followers.GetFollowerCharID(uiFollowerSlot);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж蒂 蟾晦�� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo)
{
	return Followers.InitFollower(uiFollowerSlot, lpFollowerInfo, pWorld->pIOnlineCharKI->GetNation(pWorld->pIOnlineCharKI->GetIndex(lpFollowerInfo->uiKind)), g_pItem);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж蒂 熱薑 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::UpdateFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo)
{
	return Followers.UpdataFollower(uiFollowerSlot, lpFollowerInfo, pWorld->pIOnlineCharKI->GetNation(pWorld->pIOnlineCharKI->GetIndex(lpFollowerInfo->uiKind)), g_pItem);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	睡ж蒂 餉薯 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::DeleteFollower(UI08 uiFollowerSlot)
{
	return Followers.DeleteFollower(uiFollowerSlot);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Get Follower Parameter Data
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嬴檜蠱擊 濰雜擊 и棻.
//	熱薑 橾濠	:	2002-07-13 螃�� 8:42:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	MyData::AttachItem(UI08 uiFollowerSlot, UI32 uiItemID, SI16 siGroup)
{
	return	Followers.SetWearItem2(uiFollowerSlot, UI08(siGroup), UI16(uiItemID));	
}

/////////////////////////////////////////////////
// 嬴檜蠱擊 慇朝棻.
BOOL	MyData::StripItem( UI08 uiFollowerSlot, SI16 siGroup )
{
	OnlineFollower	*pFollower = Followers.GetFollower(uiFollowerSlot);
	if(pFollower == NULL )			return FALSE;

	pFollower->Weapon.SetWearItem( (UI08)siGroup, 0 ,0);	
	return TRUE;
}

/////////////////////////////////////////////////
// 嬴檜蠱擊 模賅 и棻.
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

			// 睡�偏ぜ旼衈� 嬴棲賊
			if (bRevive == FALSE)
			{
				// 羹溘檜 0爾棻 濛戲賊 瞳辨腎雖彊朝棻.
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
// 陛雖堅 氈朝 Item曖 熱蒂 橢橫螞棻.
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
// 睡ж 議葛攪 檜葷擊 橢橫螞棻.
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
// 睡ж 議葛攪 檜葷擊 撲薑л
VOID	MyData::SetFollowerCharName( UI08 uiFollowerSlot, CHAR *pName )
{
	if( Followers.IsValidFollower( uiFollowerSlot ) )
		Followers.SetFollowerName( uiFollowerSlot, pName );
}

/////////////////////////////////////////////////
// 寡堅Ь 熱纂蒂 橢橫 螞棻.( 100% )
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
// 奢撩瞪 嶸棉橾 陽 滲褐 瞪 錳楚 輿檣奢 議葛攪 Online Kind ID 橢晦
SI32	MyData::GetPrevCharOnlineKind(void)
{
	return m_siPrevCharOnlineKind;
}

// robypark 2004/11/18 20:3
// 奢撩瞪 嶸棉橾 陽 滲褐 瞪 錳楚 輿檣奢 議葛攪 Online Kind ID 撲薑
VOID	MyData::SetPrevCharOnlineKind(SI32 siPrevCharOnlineKind)
{
	m_siPrevCharOnlineKind = siPrevCharOnlineKind;
}

// 奢撩瞪 嶸棉戲煎 陛棟и雖 橢晦
bool	MyData::CanTransformSiegeWarfareUnit(void)
{
	return m_bCanTransformSiegeWarfareUnit;
}

// 奢撩瞪 嶸棉戲煎 滲褐檜 陛棟и雖 撲薑
VOID	MyData::SetTransformSiegeWarfareUnit(bool bCanTransformSiegeWarfareUnit)
{
	m_bCanTransformSiegeWarfareUnit = bCanTransformSiegeWarfareUnit;
}

// robypark 2004/12/10 20:11
// 餌辨濠陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛?
BOOL	MyData::IsInVillageDefenceInterface(void)
{
	return m_bIsInVillageDefenceInterface;
}

// robypark 2004/12/10 20:11
// 餌辨濠陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛?
void	MyData::SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface)
{
	m_bIsInVillageDefenceInterface = bIsInVillageDefenceInterface;
}

// robypark 2005/1/27 19:00
// 螃棺擎 奢撩擊 ж朝陛?
BOOL	MyData::IsSiegeWarfareDay(void)
{
	return m_bIsSiegeWarfareDay;
}

// robypark 2005/1/27 19:00
// 螃棺擎 奢撩擊 ж朝陛蒂 撲薑
void	MyData::SetSiegeWarfareDay(BOOL bIsSiegeWarfareDay)
{
	m_bIsSiegeWarfareDay = bIsSiegeWarfareDay;
}