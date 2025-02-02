/************************************************************************************************
	だ橾貲 : IOnlineChar.cpp
	氬渡濠 : 檜彌戮

	⑷營 Game縑憮 襄營ж朝 Player曖 薑爾蒂 婦葬и棻.
************************************************************************************************/
#include <GSL.h>
#include <graphics16.h>
#include "OnlineWorld.h"
#include "OnlineChar-Parser.h"
#include "GSCDefine.h"
#include "OnlineCommonStruct.h"
#include <DebugTool.h>
#include "OnlineMyData.h"

#include "..\Gersang\maindata.h"
#include "OnlineMegaText.h"
#include ".\CharInfo\CharFileLoadManager.h"

#include	"FieldHeroInfoMgr.h"

// 奢撩 嶸棉 婦葬濠
#include	"SiegeUnitTableMgr.h"

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠
//----------------------------------------------------------------------------------------------------------------
IOnlineChar::IOnlineChar()
{
	pclOnlineChar	=	NULL;
	pMyOnlineWorld	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------
IOnlineChar::~IOnlineChar()
{
	Free();	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	螞塭檣 議葛攪曖 檣攪む檜蝶蒂 蟾晦�� и棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::Init(cltOnlineWorld *pWorld)
{
	pMyOnlineWorld	=	pWorld;

	// 議葛攪曖 詭賅葬蒂 й渡и棻.
	pclOnlineChar	=	new cltOnlineChar[MAX_ONLINE_CHAR];	

	if(pclOnlineChar)	
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	螞塭檣 議葛攪曖 檣攪む檜蝶蒂 п薯и棻.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Free()
{	
	if(pclOnlineChar)
	{
		AllFree();

		delete [] pclOnlineChar;
		pclOnlineChar =	NULL;
	}

	pMyOnlineWorld	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 蟾晦�� и棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::Init(UI16 uiUniqueID, CHAR *pszID, SI32 siKind, SI16 siX, SI16 siY, DWORD ip, float Ranking, SI32 FightPower, cltOnlineWorld* pWorld,UI16* lpuiEquipWeapon)
{
	SI32	siID;

	// 檜嘐 ID陛 氈朝雖 瓊嬴獄棻.
	siID	=	GetID(uiUniqueID);
	if(siID != 0)	
	{
		// ID陛 檜嘐 氈棻.
		// 蹺陛ж雖 彊嬴紫 脹棻.
		return FALSE;	
	}

	siID	=	FindEmptyID();	

	if(siID != 0)
	{			
		pclOnlineChar[siID].Init(uiUniqueID, siID, pszID, siKind, siX, siY, ip, Ranking, FightPower, pWorld,lpuiEquipWeapon);				
		return TRUE;
	}
	else
	{
		SetMessage("No more ID can be added.");
		return FALSE;		
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	濰雜 Item 雖薑 -薯剪-
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::SetEquipItem(UI16 uiUniqueID, UI16* lpuiEquipWeapon)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);		

	// 嶸�褲� 餌辨濠罹撿 и棻.
	if(siID != 0)
	{	
		return pclOnlineChar[siID].SetEquipItem(lpuiEquipWeapon);
	}	

	return FALSE;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 橈撻棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Free(UI16 uiUniqueID)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);		

	// 嶸�褲� 餌辨濠罹撿 и棻.
	if(siID != 0)
	{	
		pclOnlineChar[siID].Free();							
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 �飛橦� 斜萼棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Draw(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].Draw(fWater);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 �飛橦� 斜萼棻.(評塭棻棲朝 辨煽辨)	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawByVMercenary(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].DrawByVMercenary(fWater);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 奩癱貲п憮 鎰橫遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::DrawTransparent(SI16 siID, BOOL fWater)
{
	pclOnlineChar[siID].DrawTransparent(fWater);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Draw Name
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawName(UI16 uiUniqueID, HDC hdc)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].DrawName(hdc);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪 檜葷 擅縑 Guild 梓嫦擊 鎰朝棻.(虜擒 謝ル陛 -1檜賊 ID 鎰朝 睡碟 擅縑 鎰朝棻.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::DrawGuildFlag(UI16 uiUniqueID, SI16 ScreenX, SI16 ScreenY)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].DrawGuildFlag(ScreenX, ScreenY);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜葷擊 鎰朝棻.(hdc蒂 餌辨ж朝 萄煎辦)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllDrawName(HDC hdc)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 奢撩瞪 嶸棉虜. 濠晦 議葛攪 薯諼
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawName(hdc);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	渠餌蒂 鎰朝棻.(hdc蒂 餌辨ж朝 萄煎辦)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllDrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
			pclOnlineChar[i].DrawSpeech(lpSurface);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	擋暮
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::Action()
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{	
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{				
			pclOnlineChar[i].Action();
		}
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	評塭棻棲朝 辨煽菟 擋暮
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::ActionByVMercenary()
{
	SI32	i;	

	// Map 蟾晦��
	pMyOnlineWorld->pOnlineMap->VMercenaryBufferClear();

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{	
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{				
			pclOnlineChar[i].ActionByVMercenary();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	顫橾 謝ル蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::GetPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)
	{	
		pclOnlineChar[siID].GetPos(psiX, psiY);
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	顫橾曖 滲嬪 謝ル蒂 橢橫螞棻.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetRPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetRPos(psiX, psiY);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 跡ル 嬪纂蒂 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetDPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetDPos(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 棻擠 嬪纂蒂 橢橫螞棻.	
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetNPos(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetNPos(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 棻擠 嬪纂蒂 橢橫螞棻.(評塭棻棲朝 辨煽辨)		
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetNPosByVMercenary(UI16 uiUniqueID, SI16 *psiX, SI16 *psiY)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetNPosByVMercenary(psiX, psiY);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 棻擠 嬪纂曖 綠徽擊 橢橫螞棻.	
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetMovePersent(UI16 uiUniqueID, SI16 *ps)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetMovePersent(ps);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 棻擠 嬪纂曖 綠徽擊 橢橫螞棻.(評塭棻棲朝 辨煽辨)		
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::GetMovePersentByVMercenary(UI16 uiUniqueID, SI16 *ps)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);
	if(siID != 0)
	{	
		pclOnlineChar[siID].GetMovePersentByVMercenary(ps);
	}
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪 貲滄擊 餉薯и棻.
//----------------------------------------------------------------------------------------------------------------
void	IOnlineChar::ClearOrder(UI16 uiUniqueID)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].ClearOrder();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪 貲滄擊 幗ぷ縑 厥朝棻.
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::AddOrder(UI16 uiUniqueID, SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)	
		return pclOnlineChar[siID].AddOrder(siOrder, siOrderParam1, siOrderParam2);
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪 貲滄擊 幗ぷ縑 厥朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::AddOrder(UI16 uiUniqueID, OnOrder Order)
{
	SI32	siID;

	siID	=	GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].AddOrder(Order);
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪曖 UniqueID蒂 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	IOnlineChar::GetUniqueID(SI16 siID)
{
	return pclOnlineChar[siID].GetUniqueID();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	UniqueID蒂 餌辨ж罹 ID(僥濠翮)擊 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetID(UI16 uiUniqueID, CHAR *pszID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetID(pszID);			
	}	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	UniqueID蒂 餌辨ж罹 ID蒂 橢橫螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetID(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return i;
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	UniqueID蒂 餌辨ж罹 Kind蒂 橢朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetKind(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetKind();
	}

	return 0;
}

// Imjin Kind蒂 葬欐л. Client縑憮 餌辨腎朝 議葛攪 謙盟 鼻熱高
SI32	IOnlineChar::GetImjinKind(UI16 uiUniqueID)
{
	SI32	i;	

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetImjinKind();
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	綠橫氈朝 嬴檜蛤蒂 瓊朝棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::FindEmptyID()
{
	SI32	i;

/*
// ⑷營曖 議葛攪熱蒂 だ學ж晦 嬪и 纔蝶お 瑞ず. 
	SI32 count = 0;
	char buffer[128];
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			count++;
		}
	}
	wsprintf(buffer, "議葛攪 熱:%d", count);
	SetMessage(buffer);
*/	
	
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == FALSE)
			return i;
	}

	// 0擊 葬欐ж朝 剪塭賊 ID蒂 瓊雖 跤ц棻朝 匙檜棻.
	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	Get IP
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI32	IOnlineChar::GetIP(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetIP();
	}

	return 0;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	⑷營 檜翕醞檣 寞щ擊 掘и棻.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineChar::GetMovingDirection(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetMovingDirection();
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜 嶸棲觼 嬴檜蛤蒂 陛霞 議葛攪陛 襄營ж朝雖 憲嬴螞棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::IsExistUniqueID(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(UI16(pclOnlineChar[i].GetUniqueID()) == uiUniqueID)
			return TRUE;			
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	嶸棲觼 ID 匐餌
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	IOnlineChar::GetIPToUniqueID(DWORD IP)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			if(pclOnlineChar[i].GetIP() == IP)
			{
				return pclOnlineChar[i].GetUniqueID();
			}
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	檜翕醞檣雖 匐餌
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::IsMoving(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].IsMoving();			
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 檜翕 衛鑑棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetMove(UI16 uiUniqueID, SI32 siDX, SI32 siDY)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetMove(siDX, siDY);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 斜 寞щ擊 и 顫橾 檜翕 衛鑑棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetMoveByDirection(UI16 uiUniqueID, SI32 siDirection)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetMoveByDirection(siDirection);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪蒂 檜翕 Flag
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetWalkingFlag(UI16 uiUniqueID, UI16 Flag)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].SetWalkingFlag(Flag);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	議葛攪曖 渠餌蒂 撲薑и棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::SetSpeech(UI16 uiUniqueID, CHAR *pString, BOOL fNotDel, CHAR* pID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetSpeech(pString, fNotDel, pID);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	渠餌陛 集氈朝雖 匐餌
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineChar::CheckSpeech(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].CheckSpeech();
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	撲貲	:	賅萇 議葛攪蒂 橈撻棻.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineChar::AllFree()
{
	SI32	i;
	
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			pclOnlineChar[i].Free();
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餌辨濠曖 檜葷擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	IOnlineChar::GetCharName(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].GetCharName();
		}
	}	

	//EXIT();

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 瞪癱溘擊 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineChar::GetFightPower(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].GetFightPower();
		}
	}	

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 瞪癱溘擊 雖薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::SetFightPower(UI16 uiUniqueID, SI32 FightPower)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			return pclOnlineChar[i].SetFightPower(FightPower);
		}
	}	

	return FALSE;
}

BOOL	IOnlineChar::SetChangeDirection(UI16 uiUniqueID,SI16 siDirection)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
		{
			pclOnlineChar[i].ChangeDirection(siDirection);
		}
	}	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	錶Щ蒂 п撿ж朝 鼻�窕帡�?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::IsWarp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY,SI16 siDistance)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].IsWarp(siWarpX, siWarpY,siDistance);
	}
	
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	錶Щ蒂 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::Warp(UI16 uiUniqueID, SI16 siWarpX, SI16 siWarpY)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].Warp(siWarpX, siWarpY);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 葆辦蝶陛 陛葬酈朝 議葛攪朝雖 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::IsPointToChar(SI16 siID, LPPOINT pptMouse, SI32 *psiDistance)
{
	return pclOnlineChar[siID].IsPointToChar(pptMouse, psiDistance);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 熱ч醞檣 貲滄擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnOrder	IOnlineChar::GetCurrentOrder(UI16 uiUniqueID)
{
	SI32		i;
	OnOrder		OrderNull = {0, 0, 0};

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetCurrentOrder();
	}

	return OrderNull;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 贗楚蝶蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineChar::GetClass(UI16 uiUniqueID)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			return pclOnlineChar[i].GetClass();
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	頂 輿嬪縑 氈朝 議葛攪菟曖 渠и 薑爾.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::RecvAroundChars(OnResAroundCharInfoMsg *pAroundCharsInfoMsg)
{
	SI32	i, j;
	UI16	uiMyChars[MAX_ONLINE_CHAR];
	UI16	*puiAroundCharsID;
	UI16	*psiCharsPos;

	// 辦摹 頂陛 陛雖堅 氈朝 賅萇 議葛攪菟曖 ID蒂 賅擎棻.
	ZeroMemory(uiMyChars, sizeof(uiMyChars));
	for(i = 0; i < MAX_ONLINE_CHAR; i++)
		uiMyChars[i]	=	pclOnlineChar[i].GetUniqueID();

	puiAroundCharsID	=	(UI16*)((CHAR*)pAroundCharsInfoMsg + sizeof(OnResAroundCharInfoMsg));
	psiCharsPos			=	(UI16*)((CHAR*)puiAroundCharsID + (sizeof(UI16) * pAroundCharsInfoMsg->uiCharNum));

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(uiMyChars[i] != 0)
		{
			for(j = 0; j < pAroundCharsInfoMsg->uiCharNum; j++)
			{
				// 頂陛 陛雖堅 氈朝 議葛攪曖 id陛 氈戲賊 斜 睡碟擎 陛雖堅 氈棻堅 羹觼и棻.(0戲煎 虜萇棻.)				
				if(uiMyChars[i] == puiAroundCharsID[j])
				{
					uiMyChars[i]			=	0;				// 釭醞縑 餉薯ж晦 嬪п憮.
					puiAroundCharsID[j]		=	0;				// 釭醞縑 蹺陛ж晦 嬪п憮.

					// п渡 議葛攪蒂 嬪纂煎 檜翕 衛鑑棻.
					pclOnlineChar[i].AddOrder(ON_ORDER_MOVE, psiCharsPos[j * 2], psiCharsPos[j * 2 + 1]);					
					break;
				}
			}
		}
	}

	// 雖錶撿 й 議葛攪蒂 餉薯и棻.
	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(uiMyChars[i] != 0)
		{
			// 欽 釭朝 餉薯ж雖 彊朝棻
			if(pclOnlineChar[i].GetUniqueID() != pMyOnlineWorld->pMyData->GetMyCharUniqueID())
				pclOnlineChar[i].Free();
		}
	}

	// 頂陛 賅腦朝 議葛攪朝 薑爾蒂 殖塭堅 憮幗縑啪 蹂羶и棻.
	// 雖旎擎 欽牖�� ж釭噶虜 爾魚棻. 釭醞縑 и廓縑 賅諦憮 爾魚棻.
	for(i = 0; i < pAroundCharsInfoMsg->uiCharNum; i++)
	{
		if(puiAroundCharsID[i] != 0)
		{
			pMyOnlineWorld->RPI.Add(puiAroundCharsID[i]);			
			//DWriteStringToFile("IOnlineChar::RecvAroundChars 縑憮 Ы溯檜橫曖 薑爾蒂 殖塭堅 蹂羶и棻.");
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Ы溯檜橫曖 薑爾蒂 撲薑п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == lpBaseInfo->uiAccount)
			pclOnlineChar[i].SetPlayerInfo(lpBaseInfo, fType, lpPlayerInfo);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Ы溯檜橫曖 薑爾蒂 撲薑п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetPlayerInfo(UI16 uiUniqueID, OnCharPlayerDetailInfo* lpPlayerInfo)
{
	SI32		i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		if(pclOnlineChar[i].GetUniqueID() == uiUniqueID)
			pclOnlineChar[i].SetPlayerInfo(lpPlayerInfo);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪縑啪 嬴檜蠱擊 濰雜衛鑑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, UI16 uiUniqueID, BOOL bEffectSound)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetEquip(bEquip, siEquipKind, siEquipID, bEffectSound);
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	(ゎ��/瞪癱 賅萄) ⑷營 濰雜ж堅 氈朝 鼠晦 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	IOnlineChar::GetWeaponDrawID(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetWeaponDrawID();
	}
	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	(ゎ��/瞪癱 賅萄) ⑷營 濰雜ж堅 氈朝 鼠晦 ID蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetWeaponDrawID(UI16 uiUniqueID, SI16 siWeaponDrawID)
{
	SI32	siID;
	
	siID	= GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetWeaponDrawID(siWeaponDrawID);
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Set Battle Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetBattleFlag(UI16 uiUniqueID, UI08 Flag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetBattleFlag(Flag);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Get Battle Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	IOnlineChar::GetBattleFlag(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetBattleFlag();
	}
	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Imoticon 轎溘
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetImoticon(UI16 uiUniqueID, UI16 uiImoticon)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetImoticon(uiImoticon);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Set Find Target Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetFindTargetFlag(UI16 uiUniqueID, UI08 Flag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetFindTargetFlag(Flag);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	望萄 Data 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetGuildData(UI16 uiUniqueID, char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) pclOnlineChar[siID].SetGuildData(lpGuildName, siGuildInGrade, siGuildFlag);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	望萄 Name 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
char*	IOnlineChar::GetGuildName(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetGuildName();
	}

	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	望萄 霜晝 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineChar::GetGuildInGrade(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
	{
		return pclOnlineChar[siID].GetGuildInGrade();
	}
	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	望萄 梓嫦 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineChar::GetGuildFlag(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetGuildFlag();
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞳渠 望萄檣雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::SetIsHostilityGuild(UI16 uiUniqueID, UI08 uiIsHostilityGuild)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].SetIsHostilityGuild(uiIsHostilityGuild);
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	瞳渠 望萄檣雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	IOnlineChar::GetIsHostilityGuild(UI16 uiUniqueID)
{
	SI32	siID;

	siID = GetID(uiUniqueID);

	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetIsHostilityGuild();
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	億煎 摹鷗脹 議葛攪罹睡蒂 撲薑 
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void	IOnlineChar::SetNewSelectedFlag(UI16 uiUniqueID, BOOL flag)
{
	
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) pclOnlineChar[siID].SetNewSelectedFlag(flag);
	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	億煎 摹鷗脹 議葛攪罹睡蒂 橢橫褥.  
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::GetNewSelectedFlag(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetNewSelectedFlag();
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	爾罹還雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::Show(UI16 uiUniqueID, BOOL fShow)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
	{
		return pclOnlineChar[siID].Show(fShow);
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	爾罹還雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::GetShow(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetShow();
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	偃檣鼻薄 嬴檜蠱 薑爾 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SellItemInfo	*IOnlineChar::GetBoothItem(UI16 uiUniqueID, UI08 uiSlot)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetBoothItem(uiSlot);
	}
	return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	偃檣鼻薄 嬴檜蠱 薑爾 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetBoothItem(UI16 uiUniqueID, UI08 uiSlot, SellItemInfo ItemInfo)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetBoothItem(uiSlot, ItemInfo);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	偃檣鼻薄 偃撲 罹睡 橢晦
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineChar::IsBoothOpen(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].IsBoothOpen();
	}
	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	偃檣鼻薄 偃撲 罹睡 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineChar::SetBoothOpen(UI16 uiUniqueID, BOOL bOpenBooth)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetBoothOpen(bOpenBooth);
	}
}

SI08	IOnlineChar::GetDataType(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetDataType();
	}

	return 0;
}

void	IOnlineChar::SetDataType(UI16 uiUniqueID, SI08 fDataType)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetDataType(fDataType);
	}
}

BOOL	IOnlineChar::SetMercenary(UI16 uiUniqueID, SI32 siKind)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].SetMercenary(siKind);
	}

	return FALSE;
}

SI32	IOnlineChar::GetMercenary(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		return pclOnlineChar[siID].GetMercenary();
	}

	return 0;
}

void	IOnlineChar::GetEquip(UI16 uiUniqueID, UI16* lpuiEquipWeapon)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].GetEquip(lpuiEquipWeapon);
	}
}

void	IOnlineChar::SetChangeEffect(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0) 
	{
		pclOnlineChar[siID].SetChangeEffect();
	}
}

VOID IOnlineChar::SetAttackMode(UI16 uiUniqueID, BOOL b)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetAttackMode(b);
}

BOOL IOnlineChar::IsAttackMode(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].IsAttackMode();

	return FALSE;
}

int	IOnlineChar::GetScore(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].GetScore();

	return 0;
}

int IOnlineChar::GetRank(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].GetRank();

	return 0;
}

void IOnlineChar::SetRank(UI16 uiUniqueID, UI16 Rank)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetRank((int)Rank);
}

void IOnlineChar::SetScore(UI16 uiUniqueID, int Score)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		pclOnlineChar[siID].SetScore(Score);
}

BOOL IOnlineChar::IsInvalidBattle(UI16 uiUniqueID)
{
	SI32	siID;
	
	siID = GetID(uiUniqueID);
	
	if(siID != 0)
		return pclOnlineChar[siID].IsInvalidBattle();

	return FALSE;
}

void IOnlineChar::SetSiegeWarfare_Hp(UI16 uiUniqueID,SI16 siFieldHp)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Hp(siFieldHp);
}

void IOnlineChar::SetSiegeWarfare_Damage(UI16 uiUniqueID,SI16 siDamage)
{	
	SI32 siID;
	
	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Damage(siDamage);
}

SI16 IOnlineChar::GetSiegeWarfare_Hp(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_Hp();

	return 0;
}

BOOL IOnlineChar::IsFieldAlive(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].IsFieldAlive();
	return 0;
}

// 衛濛 /////////////////////////////////////////
// robypark 2004/10/27 15:22
// 奢撩瞪 嶸棉 羹溘, 爾晝ヶ, 奢問雖翱 衛除 л熱
// 奢撩瞪 嶸棉 譆渠 羹溘纂 撲薑
void IOnlineChar::SetSiegeWarfare_MaximumHP(UI16 uiUniqueID, SI16 siMaximumHP)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_MaximumHP(siMaximumHP);
}

// 奢撩瞪 嶸棉 譆渠 羹溘纂 橢晦
SI16 IOnlineChar::GetSiegeWarfare_MaximumHP(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_MaximumHP();

	return 0;
}

// 奢撩瞪 嶸棉 爾晝ヶ ⑷營榆 撲薑
void IOnlineChar::SetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID, SI32 siCurrentSupplyGoodsAmount)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_CurrentSupplyGoodsAmount(siCurrentSupplyGoodsAmount);
}

// 奢撩瞪 嶸棉 爾晝ヶ ⑷營榆 橢晦
SI32 IOnlineChar::GetSiegeWarfare_CurrentSupplyGoodsAmount(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_CurrentSupplyGoodsAmount();

	return 0;
}

// 奢撩瞪 嶸棉 爾晝ヶ 譆渠榆 撲薑
void IOnlineChar::SetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID, SI32 siSupplyGoodsAmount)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_SupplyGoodsAmount(siSupplyGoodsAmount);
}

// 奢撩瞪 嶸棉 爾晝ヶ 譆渠榆 橢晦
SI32 IOnlineChar::GetSiegeWarfare_SupplyGoodsAmount(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfare_SupplyGoodsAmount();

	return 0;
}

// 奢撩瞪 嶸棉曖 奢問 雖翱 衛除 撲薑
void IOnlineChar::SetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID, DWORD dwLastAttackDelay)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfareUnit_LastAttackDelay(dwLastAttackDelay);
}

// 奢撩瞪 嶸棉曖 唳婁脹 奢問 雖翱 衛除 撲薑
void IOnlineChar::SetSiegeWarfareUnit_PastAttackDelay(UI16 uiUniqueID, DWORD dwPastAttackDelay)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfareUnit_PastAttackDelay(dwPastAttackDelay);
}

// 奢撩瞪 嶸棉曖 奢問 雖翱 衛除 橢晦
DWORD IOnlineChar::GetSiegeWarfareUnit_LastAttackDelay(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSiegeWarfareUnit_LastAttackDelay();

	return 0;
}
// 部 /////////////////////////////////////////	

// robypark 2004/11/2 17:58
// 奢撩瞪 嶸棉戲煎 滲褐 �膩� 餌辨
void	IOnlineChar::SetSiegeWarfare_TransformEffect(UI16 uiUniqueID, SI32 siSiegeWarfareTransformType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_TransformEffect(siSiegeWarfareTransformType);
}

// robypark 2004/12/2 19:2
// 奢撩瞪 嶸棉檜 爾晝ヶ 婦溼 �膩� 撲薑. 爾晝ヶ擊 嫡懊剪釭(1) 爾晝ヶ檜 夥款陪 唳辦(2)
void	IOnlineChar::SetSiegeWarfare_EffectSupplyGoods(UI16 uiUniqueID, SI32 siEffectSupplyGoodsType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_EffectSupplyGoods(siEffectSupplyGoodsType);
}

// robypark 2004/11/2 18:6
// 奢撩瞪 嶸棉 爾晝ヶ 濠翕 模賅(1%)
void	IOnlineChar::WasteSiegeWarfareSupplyGoods(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].WasteSiegeWarfareSupplyGoods();
}

// robypark 2004/11/3 15:24
// 奢撩瞪 嶸棉檜 葆擊煎 睡攪 奢問嫡朝 �膩� 撲薑
void	IOnlineChar::SetSiegeWarfare_VillageCountAttack(UI16 uiUniqueID, SI32 siEffectVillageConterAttackType)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_VillageCountAttack(siEffectVillageConterAttackType);
}

// robypark 2004/11/24 18:31
// 奢撩瞪 唯堅и 寞橫 �膩� 餌辨
void	IOnlineChar::SetSiegeWarfare_Evasion(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_Evasion();
}

// robypark 2004/11/24 18:31
// 奢撩瞪 纂貲瞳 奢問 �膩� 餌辨
void	IOnlineChar::SetSiegeWarfare_DeathBlow(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSiegeWarfare_DeathBlow();
}

void	IOnlineChar::SetCaptain(UI16 uiUniqueID,BOOL bCaptain)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetCaptain(bCaptain);
}

BOOL	IOnlineChar::GetCaptain(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetCaptain();

	return 0;
}
/*
void	IOnlineChar::SetSupplyCharge(UI16 uiUniqueID, SI16 siSupply)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].SetSupplyCharge(siSupply);
}

SI16	IOnlineChar::GetSupplyCharge(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		return pclOnlineChar[siID].GetSupplyCharge();

	return 0;
}
*/
// robypark 2004/11/12 21:27
// 奢撩瞪 嶸棉曖  羹溘 塽 爾晝ヶ 熱纂(%) 斜葬晦
void	IOnlineChar::DrawSiegeWarfareUnit_StatusBar(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if(siID != 0)
		pclOnlineChar[siID].DrawSiegeWarfareUnit_StatusBar();
}

// robypark 2004/11/13 16:41
// 賅萇 奢撩瞪 嶸棉曖  羹溘 塽 爾晝ヶ 熱纂(%) 斜葬晦
void	IOnlineChar::AllDrawSiegeWarfareUnit_StatusBar(void)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 奢撩瞪 嶸棉虜. 濠晦 濠褐擎 薯諼
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawSiegeWarfareUnit_StatusBar();
			}
		}
	}
}

// robypark 2004/11/22 10:56
// 賅萇 奢撩瞪 嶸棉曖 鼻欽 梓嫦 斜葬晦
void	IOnlineChar::AllDrawSiegeWarfareUnit_GuildFlag(void)
{
	SI32	i;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 蟾晦�降� 脹 議葛攪虜擊 轎溘и棻.
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 奢撩瞪 嶸棉虜. 濠晦 濠褐擎 薯諼
			if ((TRUE == pclOnlineChar[i].IsSiegeWarfareUnit())
				&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID()))
			{
				pclOnlineChar[i].DrawGuildFlag();
			}
		}
	}
}

// robypark 2004/12/16 14:33
// 奢撩瞪 嶸棉檣陛?
BOOL	IOnlineChar::IsSiegeWarfareUnit(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		return pclOnlineChar[siID].IsSiegeWarfareUnit();

	return FALSE;
}

// robypark 2004/12/17 17:20
// 餌辨濠陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛?
BOOL	IOnlineChar::IsInVillageDefenceInterface(UI16 uiUniqueID)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		return pclOnlineChar[siID].IsInVillageDefenceInterface();

	return FALSE;
}

// robypark 2004/12/17 17:20
// 議葛攪陛 ⑷營 熱撩 檣攪む檜蝶 鼻縑 氈朝陛? 撲薑
void	IOnlineChar::SetInVillageDefenceInterface(UI16 uiUniqueID, BOOL bIsInVillageDefenceInterface)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].SetInVillageDefenceInterface(bIsInVillageDefenceInterface);
}

// robypark 2005/1/14 18:39
// 議葛攪蒂 斜萵 陽 斜溥雖晦 衛濛ж朝 嬪纂 橢晦
void	IOnlineChar::GetCharacterDrawCenterPos(UI16 uiUniqueID, SI16 &siCenterX, SI16 &siCenterY)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].GetCharacterDrawCenterPos(siCenterX, siCenterY);
}

// robypark 2005/1/14 19:8
// 賅萇 輿檣奢 議葛攪蒂 Я陪團 �分� 橾奩 輿檣奢 議葛攪煎 滲褐衛鑑棻.
void	IOnlineChar::AllHeroCharacterTransformTo(void)
{
	SI32	i;

	// 獄 裘檜 嬴棲塭賊 Я陪團 婦溼 議葛攪 滲褐 籀葬蒂 ж雖 彊朝棻.
	if (0 != pMyOnlineWorld->pOnlineMap->m_MapIndex)	// 湍瞪, 盪濕剪葬 蛔曖 裘檜塭賊,
		return;

	// 餌辨濠曖 奢撩檜 橈棻賊,
	if (FALSE == pMyOnlineWorld->pMyData->IsSiegeWarfareDay())
		return;

	for(i = 1; i < MAX_ONLINE_CHAR; i++)
	{
		// 蟾晦�降� 脹 議葛攪虜
		if(pclOnlineChar[i].IsInit() == TRUE)
		{
			// 濠晦 議葛攪 薯諼
			if (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != pclOnlineChar[i].GetUniqueID())
			{
				DWORD		IP;
				CHAR		TempBuffer[128];
				
				// 議葛 薑爾 橢晦
				UI16 uiUniqueID, uiKind, uiEquipWeapon[WEAR_ITEM_NUM];
				SI16 siX, siY;
				SI32 siFightPower, siGuildInGrade, siGuildFlag, siMercenaryKind;
				CHAR TempBuffer1[128];
				ZeroMemory(TempBuffer1, sizeof(TempBuffer1));

				// 議葛攪 晦獄 薑爾 橢晦
				uiUniqueID		= pclOnlineChar[i].GetUniqueID();
				uiKind			= pclOnlineChar[i].GetKind();

				// 輿檣奢 議葛攪檣陛?
				// 輿檣奢 婦溼 等檜攪 婦葬濠 橢晦
				CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(uiKind);
				// 輿檣奢 議葛攪陛 嬴棲塭賊, 滲褐й в蹂陛 橈棻.
				if (NULL == pFieldHeroInfo)
					continue;

				// 奢撩瞪 嶸棉檣陛?
				LPSSiegeUnitTable lpsSiegeUnitTable = pMyOnlineWorld->GetSiegeUnitTableMgr()->GetSiegeUnitTable(uiKind);
				// 奢撩瞪 嶸棉橾 唳辦, 滲褐й в蹂陛 橈棻.
				if (lpsSiegeUnitTable)
					continue;

				IP				= GetID(uiUniqueID);
				siFightPower	= pclOnlineChar[i].GetFightPower();
				siGuildInGrade	= pclOnlineChar[i].GetGuildInGrade();
				siGuildFlag		= pclOnlineChar[i].GetGuildFlag();
				siMercenaryKind = pclOnlineChar[i].GetMercenary();

				// 檜葷
				CopyMemory(TempBuffer, pclOnlineChar[i].GetCharName(), ON_ID_LENGTH);
				// 望萄貲
				if(pclOnlineChar[i].GetGuildName())
					CopyMemory(TempBuffer1, pclOnlineChar[i].GetGuildName(), ON_ID_LENGTH);

				// 議葛攪 嬪纂 橢晦
				pclOnlineChar[i].GetPos(&siX, &siY);

				// 議葛攪 濰綠 橢晦
				pclOnlineChar[i].GetEquip(uiEquipWeapon);
				
				// 議葛攪 餉薯
				pclOnlineChar[i].Free();

				// 議葛攪 儅撩(蟾晦��)
				Init(uiUniqueID, (CHAR*)TempBuffer, uiKind, siX, siY, IP, 0, siFightPower, pMyOnlineWorld, (UI16*)uiEquipWeapon);

				// 濰綠 撲薑
				SetEquipItem(uiUniqueID, (UI16*)uiEquipWeapon);
				
				// 鼻欽 塽 辨煽 等檜攪 撲薑
				SetGuildData(uiUniqueID, (CHAR*)TempBuffer1, siGuildInGrade, siGuildFlag);
				SetMercenary(uiUniqueID, siMercenaryKind);
			}
		}
	}
}

// robypark 2005/2/17 21:44
// 葆辦蝶 醴憮 嬴楚 議葛攪檣雖蒂 撲薑и棻.
void	IOnlineChar::SetCursorHoverCharacter(UI16 uiUniqueID, BOOL bHover/* = FALSE*/)
{
	SI32 siID;

	siID = GetID(uiUniqueID);

	if (siID != 0)
		pclOnlineChar[siID].SetCursorHoverCharacter(bHover);
}
