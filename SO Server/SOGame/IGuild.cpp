#include "SOMain.h"
#include "SOWorld.h"
#include "IGuild.h"
#include "SODatabase.h"
#include "OnlineCommonStruct.h"

SOWorld*	cltIGuild::m_pMyWorld	=	NULL;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:14:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIGuild::cltIGuild()
{
	m_Flags.Create(SO_TOTAL_GUILD_NUM);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:14:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltIGuild::~cltIGuild()
{
	Free();	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	и廓虜 蟾晦�� п還匙菟擊 п遽棻.
//	熱薑 橾濠	:	2002-04-13 螃�� 6:13:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::OnceInit(SI32 siTotalVillageNum)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		// и廓虜 蟾晦�� п還匙菟擊 п遽棻.
		m_Guilds[i].OnceInit(siTotalVillageNum);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬蒂 п薯и棻.
//	熱薑 橾濠	:	2002-02-26 螃�� 1:19:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::Free()
{	
	m_Flags.Release();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽 儅撩衛 в蹂и 旎擋擊 橢橫螞棻.
//	熱薑 橾濠	:	2003-01-27 螃�� 2:24:10 - 寥鼻⑷
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MONEY	cltIGuild::GetGuildCreateCost()
{
	// min(30000000, 2000000 + (鼻欽偃撲�蜈� *20000) )
	// 鼻欽偃撲�蜈� : 錯萄陛 翮萼 檜�譟� 雖旎梱雖 鼻欽檜 偃撲脹 援瞳 �蜈�.
	// 鼻欽 偃撲 綠辨擎 譆堅 3繭虜傖擊 剩望 熱 橈棻.

	return min( (MONEY)30000000, (MONEY)(2000000 + (m_uiMaxGuildID * 20000)) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 儅撩и棻.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:14:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltIGuild::CreateGuild(CHAR *pszGuildName, SOPlayer *pPlayer, SOVillage *pVillage)
{
	SI32					siEmptySlot;		
	DBIKGuildCreateInfo		GuildCreateInfo;		
	UI32					uiNewGuildID;

	SI32					i;

	

	if((siEmptySlot	= FindEmptySlot()) == -1)		return	0;
	
	// 鼻欽曖 檜葷檜 橈戲賊 寰脹棻.
	if(pszGuildName[0] == NULL)						return	0;
	if(pVillage == NULL)							return	0;		

	ZeroMemory(&GuildCreateInfo, sizeof(GuildCreateInfo));

	// 鼻欽曖 檜葷擊 撲薑и棻.
	GuildCreateInfo.siAccount = pPlayer->stAccountInfo.siDBAccount;
	GuildCreateInfo.uiSlot = pPlayer->stAccountInfo.uiSlot;

	strncpy(GuildCreateInfo.szGuildName, pszGuildName, ON_GUILDNAME_LENGTH);		
	
	GuildCreateInfo.uiVillageCode	=	pVillage->uiVillageCode;	

	//GuildCreateInfo.moMoney = ON_GUILD_COST_CREATE_GUILD;
	GuildCreateInfo.moMoney = GetGuildCreateCost();

	
	// 譆渠 望萄嬴檜蛤蒂 ж釭 隸陛衛麵 億 望萄曖 嬴檜蛤煎 餌辨 (檜溢啪п憮 憮幗陛 и廓睡たц擊陽 偽擎 id蒂 乾朝 望萄朝 2偃檜鼻 儅晦雖彊朝棻.)
	m_uiMaxGuildID++;
	uiNewGuildID = m_uiMaxGuildID;
	GuildCreateInfo.uiGuildID = uiNewGuildID;

	// DB縑憮 望萄蒂 儅撩и棻.
	// 鼻欽擊 儅撩и棻.
	m_pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GUILD_CREATE, LPVOID(&GuildCreateInfo), sizeof(GuildCreateInfo));
	
	// 鼻欽擊 撲薑и棻.
	pPlayer->SetGuild(uiNewGuildID);
	
	// 霜晝擊 撲薑и棻.
	pPlayer->SetClassInGuild(m_pMyWorld->clIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI));

	m_Guilds[siEmptySlot].CreateGuild(siEmptySlot, uiNewGuildID, pszGuildName, pPlayer, pVillage);	

	// 望萄除曖 撲薑擊 и棻
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{			
			// 籀擠縑朝 憮煎除縑 醞董瞳檜棻.
			if(i != siEmptySlot)
			{
				SetRelationByIndex(siEmptySlot, i, ON_GUILD_RELATION_NEUTRAL, FALSE,0);			
				SetRelationByIndex(i, siEmptySlot, ON_GUILD_RELATION_NEUTRAL, FALSE,0);
			}			
		}	
	}

	// 鼻欽檜 撲董 腎歷棻朝 奢雖 餌о擊 爾頂遽棻.
	m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_CREATED, LPARAM(pszGuildName));

	return uiNewGuildID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 撲薑и棻.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:14:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::InitGuild(UI32 uiGuildID, CHAR *pszGuildName, CHAR *pszDAIID, SI32 uiGuildMemeberNum, SI32 siFlag)
{
	SI32			siEmptySlot;			
	strFlagInfo		stFlagInfo;
	SI32			siFlagIDInStack = -1;


	siEmptySlot	=	FindEmptySlot();
	if(siEmptySlot == -1)	return FALSE;
	
	if(siFlag != 0)
	{
		stFlagInfo.uiGuildID	=	uiGuildID;
		stFlagInfo.siFlag		=	siFlag;

		// 梓嫦檜 氈棻賊 梓嫦 葬蝶お縑 盪濰и棻.
		siFlagIDInStack			=	m_Flags.Add(stFlagInfo);
	}

	// 望萄蒂 蟾晦�� и棻.
	m_Guilds[siEmptySlot].InitGuild(siEmptySlot, uiGuildID, pszGuildName, pszDAIID, uiGuildMemeberNum, siFlag, siFlagIDInStack);	

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽擊 餉薯и棻.	
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:14:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::DeleteGuild(UI32 uiGuildID,SOVillage* pVillage)
{
	SI32			siFoundSlot;
	SI32			i = 0;	
	strFlagInfo		stFlagInfo;	
	CHAR			szGuildName[ON_GUILDNAME_LENGTH + 1];

	siFoundSlot	=	FindSlot(uiGuildID);

	if(siFoundSlot == -1 )	return FALSE;

	//奢撩醞檣 鼻欽擎 п羹陛 寰脹棻.
//	if(m_Guilds[siFoundSlot].IsWar()) return FALSE;

	
	// 餉薯ж溥朝 望萄諦曖 婦啗蒂 賅舒 蟾晦�倆挬�.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{	
		if(i == siFoundSlot)	continue;

		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(siFoundSlot != i)
				m_Guilds[i].InitRelation(siFoundSlot);
		}
	}
	//鼻欽縑 輿湍 等嘐雖蒂 薯剪и棻.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++ )
	{
		if(pVillage->m_stAttackGuild[i].uiGuildID != 0) continue;

		if(pVillage->m_stAttackGuild[i].uiGuildID == uiGuildID)
		{
			ZeroMemory(&pVillage->m_stAttackGuild[i],sizeof(stAttackGuild) * SO_TOTAL_GUILD_NUM);
		}
	}
	//鼻欽縑 唸婁高擊 幗萼棻.
	ZeroMemory(m_Guilds[siFoundSlot].m_stGuildResult,sizeof(cltGuildResult) * ON_MAX_GUILD_RESULT);
	//鼻欽縑 模樓脹 望萄 嶸棉擊 幗萼棻.
	ZeroMemory(&m_Guilds[siFoundSlot].m_GuildUnit,sizeof(cltGuildUnit));

	m_Guilds[siFoundSlot].SendGuildItemDeleteMessage();

	stFlagInfo.uiGuildID	=	uiGuildID;
	stFlagInfo.siFlag		=	m_Guilds[siFoundSlot].GetFlag();

	// 望萄陛 餉薯 腎晦瞪縑 檜葷擊 盪濰п 菩棻
	ZeroMemory(szGuildName, sizeof(szGuildName));
	strncpy(szGuildName, m_Guilds[siFoundSlot].GetGuildName(), ON_GUILDNAME_LENGTH);

	if(m_Guilds[siFoundSlot].DeleteGuild() == TRUE)
	{		
		if(stFlagInfo.siFlag != -1)
			m_Flags.DeleteType(stFlagInfo);

		// 鼻欽檜 п羹 腎歷棻朝 奢雖 餌о擊 爾頂遽棻.
		m_pMyWorld->OfficialAnnouncement(ON_RET_OA_GUILD_DELETED, LPARAM(szGuildName));				
			
		return	TRUE;
	}
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	綠橫 氈朝 蝸煜擊 瓊朝棻.
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:19:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::FindEmptySlot()
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == FALSE)
			return i;
	}

	return -1;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝸煜擊 瓊朝棻.	
//	熱薑 橾濠	:	2002-02-25 螃瞪 9:25:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::FindSlot(UI32 uiGuildID)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID)
			return i;		
	}

	return -1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝸煜擊 瓊朝棻.
//	熱薑 橾濠	:	2002-03-15 螃�� 5:23:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::FindSlot(UI32 uiGuildID1, UI32 uiGuildID2, SI32 *psiIndex1, SI32 *psiIndex2)
{
	SI32	i;
	
	*psiIndex1	=	*psiIndex2	=	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID1)
			*psiIndex1	=	i;
			
		if(m_Guilds[i].GetGuildID() == uiGuildID2)
			*psiIndex2	=	i;
	}

	if((*psiIndex1 != -1) && (*psiIndex2 != -1))
		return	TRUE;
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝸煜擊 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::FindSlot(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 *psiIndex1, SI32 *psiIndex2)
{
	SI32	i;
	
	*psiIndex1	=	*psiIndex2	=	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() == uiGuildID1)
			*psiIndex1	=	i;
			
		if(strcmp(m_Guilds[i].GetGuildName(), pszGuildID2) == 0)
			*psiIndex2	=	i;
	}

	if((*psiIndex1 != -1) && (*psiIndex2 != -1))
		return	TRUE;
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝸煜擊 瓊朝棻.	
//	熱薑 橾濠	:	2002-03-05 螃瞪 11:01:38 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::FindSlot(CHAR *pszName)
{
	SI32	i;
	CHAR	*pszGuildName;

	if(pszName == NULL)	return	-1;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		pszGuildName	=	m_Guilds[i].GetGuildName();

		if(pszGuildName != NULL)
		{
			if(strcmp(pszGuildName, pszName) == 0)	return i;		
		}		
	}

	return -1;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 鼻欽 詹幗陛 蹺陛腎歷棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 12:49:08 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::AddGuildMember(UI32 uiGuildID, CHAR *pszName)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].AddGuildMember(pszName);			
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑憮 鼻欽 詹幗陛 餉薯腎歷棻.	
//	熱薑 橾濠	:	2002-03-04 螃�� 12:49:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
/*BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, SOPlayer *pPlayer, BOOL bCandidate)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pPlayer->szID, bCandidate, bSendMsg);	
	}	

	return FALSE;
}*/

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑憮 鼻欽 詹幗陛 餉薯腎歷棻.
//	熱薑 橾濠	:	2002-03-06 螃�� 1:22:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::DelGuildMember(UI32 uiGuildID, CHAR *pszName, BOOL bCandidate, BOOL bSendMsg,BOOL bKick)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].DelGuildMember(pszName, bCandidate, bSendMsg,bKick);	
	}	

	return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	雖錳濠蒂 蹺陛и棻.
//	熱薑 橾濠	:	2002-04-13 螃瞪 9:45:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::AddCandidate(UI32 uiGuildID, SOPlayer *pPlayer)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddCandidate(pPlayer);	
	}	

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 葆擊檜 蹺陛腎歷棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 1:45:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::AddVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bInit)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
	{
		m_Guilds[siFoundSlot].AddVillage(pVillage, bInit);			

		if(bInit == FALSE)
		{
			// 蟾晦�俯萺� 嬴棍陽縑虜 奢雖 餌о擊 爾頂遽棻.
			
			// 葆擊檜 п渡 鼻欽縑 模樓腎歷棻朝 詭衛雖蒂 爾頂遽棻.
			m_pMyWorld->OfficialAnnouncement(ON_RET_OA_VILL_OCCUPIED, LPARAM(m_Guilds[siFoundSlot].GetGuildName()), LPARAM(pVillage->uiVillageCode));
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑憮 葆擊檜 餉薯腎歷棻.
//	熱薑 橾濠	:	2002-03-04 螃�� 12:55:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::DelVillage(UI32 uiGuildID, SOVillage *pVillage, BOOL bDeleteGuild)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		if(m_Guilds[siFoundSlot].DelVillage(pVillage, bDeleteGuild) == TRUE)
		{
//			m_pMyWorld->clServer->WriteInfo("Delete Guild List.txt", "Deleting clan because of no possessed town [%s]", m_Guilds[siFoundSlot].GetGuildName());
			writeInfoToFile("Delete Guild List.txt", "Deleting clan because of no possessed town [%s]", m_Guilds[siFoundSlot].GetGuildName());

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// for debug
			if( uiGuildID == 0 )
			{
//				m_pMyWorld->clServer->WriteInfo( "..\\data\\DeletedGuild.txt", "Deleting clan because of no possessed town. But the Clan ID is 0" );
				writeInfoToFile( "DeletedGuild.txt", "Deleting clan because of no possessed town. But the Clan ID is 0" );			
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			m_Guilds[siFoundSlot].SetWarFlag(FALSE);
			// 渦 檜鼻 檜 鼻欽縑朝 葆擊檜 橈棻. 鼻欽擊 薯剪и棻.
			DeleteGuild(uiGuildID, pVillage);

			pVillage->m_bIsFall        = FALSE;
			pVillage->m_bVillageAttack = FALSE;
			
		}
		else //瞪癱ん晦 й匙檣雖 僭橫獄棻.
		{
			if(m_pMyWorld->m_WarTimeParser.IsWarCheck(m_pMyWorld->pRealTime->tm_wday, m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_pMyWorld->m_WarTimeParser.IsWarTime(m_pMyWorld->pRealTime->tm_hour)) return;
			if(m_Guilds[siFoundSlot].GetGiveUpFlag() == ON_GUILD_GIVEUP_OK) return;

			SOPlayer *pPlayer = m_pMyWorld->GetPlayer( m_Guilds[siFoundSlot].GetDAIID());			
			UI16 uiVillageCode = pVillage->uiVillageCode; //葆擊 囀萄
			SI32 siBestIndex = pVillage->m_Government->GetOldBestInvestIndex(); //譆癱曖 檣策蝶 
			MONEY mnMoney = pVillage->m_Government->DelInvestMoney(siBestIndex, 1, FALSE); //瞪癱ん晦 綠辨

			if(pPlayer != NULL)//渠寞檜 煎斜檣擊 ц朝陛?
				pPlayer->SendFieldMsg(ON_ASK_GIVEUP, SO_SFM_ONLYME, LPVOID(uiVillageCode), LPVOID(mnMoney));
			
			m_Guilds[siFoundSlot].SetGiveUpInfo(uiVillageCode, mnMoney, ON_GUILD_GIVEUP_NOT_ANSWER);

			DBIKWarGiveUp           warGiveUp;
			memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

			warGiveUp.uiGuildID     = uiGuildID;
			warGiveUp.uiVillageCode = uiVillageCode;
			warGiveUp.mnMoney       = mnMoney;
			warGiveUp.siFlag        = ON_GUILD_GIVEUP_NOT_ANSWER;

			m_pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP,
				                                  &warGiveUp, sizeof(DBIKWarGiveUp));
			
		}
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-02-26 螃瞪 9:27:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::InitStaticVariable(SOWorld *pWorld)
{
	m_pMyWorld	=	pWorld;
	cltGuild::InitStaticVariable(pWorld);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 鼻欽檜 粕 離氈朝雖 獄棻.
//	熱薑 橾濠	:	2002-02-26 螃�� 5:09:22 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsGuildFull()
{
	if(FindEmptySlot() == -1)	return TRUE;
	else						return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 ID蒂 餌辨ж罹 Guild蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-16 螃�� 3:12:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltGuild*	cltIGuild::GetGuildByID(UI32 uiGuildID)
{
	SI32	siFoundSlot;		

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return &m_Guilds[siFoundSlot];
	}
	else
		return NULL;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-04 螃瞪 11:45:05 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltGuild*	cltIGuild::GetGuildByIndex(SI32 siIndex)
{
	// siIndex曖 望萄陛 嶸�褲捘� 獄棻.
	if(m_Guilds[siIndex].IsValidGuild() == FALSE)	return	NULL;
	else	
		return	&m_Guilds[siIndex];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 檜葷擊 餌辨ж罹 Guild蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃瞪 10:22:59 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltGuild*	cltIGuild::GetGuildByName(CHAR *pszName)
{
	SI32	siFoundSlot;		

	if((siFoundSlot = FindSlot(pszName)) != -1)	
	{
		return &m_Guilds[siFoundSlot];
	}
	else
		return NULL;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 檜葷擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-26 螃�� 5:09:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltIGuild::GetGuildName(UI32 uiGuildID)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].GetGuildName();
	}
	else
		return NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-05 螃瞪 10:59:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltIGuild::GetGuildID(CHAR *pszName)
{
	SI32	siFoundSlot;	

	if(pszName == NULL)	return 0;
		
	if((siFoundSlot = FindSlot(pszName)) != -1)	
	{
		return m_Guilds[siFoundSlot].GetGuildID();
	}
	else
		return NULL;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Char曖 檜葷擊 餌辨ж罹 Guild ID蒂 橢橫螞棻. 
//					瓊朝等 撩奢ц棻賊 TRUE. 褒ぬц棻賊 FALSE.
//	熱薑 橾濠	:	2002-05-27 螃�� 2:37:27 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::GetGuildIDByCharName(CHAR *pszName, UI32 *puiGuildID, SI32 *psiGuildClass)
{	
	SI32			i;

	if(pszName == NULL)	return 0;

	// 詭賅葬縑憮 橢橫螞棻.
	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{	
			if(m_Guilds[i].GetGuildIDByCharName(pszName, puiGuildID, psiGuildClass) == TRUE)
				return	TRUE;	
		}
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	霜晝擊 滲唳衛鑑棻.
//	熱薑 橾濠	:	2002-03-06 螃�� 1:43:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::ChangeClass( UI32 uiGuildID, CHAR *pszName, SI32 siNewClass )
{	
	
	SI32						siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) != -1 )	
	{
		
		return m_Guilds[ siFoundSlot ].ChangeClass( pszName, siNewClass );	
	}	

	return FALSE;


}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	望萄錳縑啪 奢雖餌о擊 爾頂遽棻.
//	熱薑 橾濠	:	2002-03-26 螃�� 3:30:33 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::Notice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)		
		return m_Guilds[siFoundSlot].Notice(pszName, pszNotice);	
	else
		return FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetRelationByIndex(SI32 siGuildIndex1, SI32 siGuildIndex2, SI32 siRelation, BOOL bInit,SI32 siTime)
{	
	m_Guilds[siGuildIndex1].SetRelation(&m_Guilds[siGuildIndex2], siRelation, bInit, siTime);	
	m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, TRUE, siTime);
/*
	switch(siRelation)
	{
	case ON_GUILD_RELATION_ENEMY:		
		// 瞳戲煎 瞳渠瞳檣 婦啗蒂 撲薑й 唳辦 瞳紫 濠翕戲煎 釭縑啪 瞳渠瞳檣 婦啗陛 脹棻.
		m_Guilds[siGuildIndex2].SetRelation(&m_Guilds[siGuildIndex1], siRelation, bInit);	
		break;
	}
*/
	
	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-03-15 螃�� 5:16:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetRelation(UI32 uiGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit,SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot1, siFoundSlot2, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetRelation(UI32 uiGuildID1, CHAR *pszGuildID2, SI32 siRelation, BOOL bInit,SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, pszGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot1, siFoundSlot2, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetRelation(CHAR *pszGuildID1, UI32 uiGuildID2, SI32 siRelation, BOOL bInit, SI32 siTime)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID2, pszGuildID1, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		SetRelationByIndex(siFoundSlot2, siFoundSlot1, siRelation, bInit,siTime);
		return	TRUE;
	}
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽曖 婦啗蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-15 螃�� 5:19:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::GetRelation(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		return	m_Guilds[siFoundSlot1].GetRelation(UI32(siFoundSlot2));	
	}
	else
		return	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 Index煎 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-16 螃�� 2:10:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltIGuild::GetGuildIDByIndex(SI32 siIndex)
{
	if(m_Guilds[siIndex].IsValidGuild() == TRUE)
		return m_Guilds[siIndex].GetGuildID();
	else
		return	0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB縑憮 望萄曖 婦啗蒂 撲薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::SetGuildRelationFromDB()
{
	SI32	i, j;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			// 辦摹 賅萇 鼻欽除曖 婦啗蒂 '醞董'戲煎 撲薑п遽棻.
			for(j = 0; j < SO_TOTAL_GUILD_NUM; j++)
			{
				if(i != j)
				{
					if(m_Guilds[j].IsValidGuild() == TRUE)
						SetRelationByIndex(i, j, ON_GUILD_RELATION_NEUTRAL, TRUE,0);					
				}
			}			
	
			m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GUILD_GETRELATION, LPVOID(m_Guilds[i].GetGuildID()), LPVOID(this));
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	uiGuildID2陛 uiGuildID1曖 瞳檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-19 螃�� 2:56:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsEnemyGuild(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		return	m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2);
	}
	else
		return	FALSE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	uiGuildID2陛 uiGuildID1曖 瞳檣雖 憲嬴螞棻. 憮煎陛 瞳渠婦啗檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-19 螃�� 2:56:29 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsEnemyGuildBoth(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if((m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2) == TRUE) && (m_Guilds[siFoundSlot2].IsEnemyGuild(siFoundSlot1) == TRUE))
			return	TRUE;
	}
	
	return	FALSE;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞳渠婦啗檣雖憲嬴螞棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 2:15:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsHostilityRelation(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;	

	if(uiGuildID1 == uiGuildID2)	return	FALSE;

	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if(m_Guilds[siFoundSlot1].IsEnemyGuild(siFoundSlot2) == TRUE ||
			m_Guilds[siFoundSlot2].IsEnemyGuild(siFoundSlot1) == TRUE)
			return	TRUE;	
	}
	
	return	FALSE;	
}




//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻婦 婦啗縑 婦溼脹 啪歜 Define擊 瞪歎辨 Define戲煎 滲唳и棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 1:55:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltIGuild::RelationDefineToResponseDefine(DWORD dwDefine)
{
	switch(dwDefine)
	{
	case ON_GUILD_RELATION_ALLY:	return ON_RET_GLD_ALLY;
	case ON_GUILD_RELATION_ENEMY:	return ON_RET_GLD_ENEMY;
	case ON_GUILD_RELATION_NEUTRAL:	return ON_RET_GLD_NEUTRAL;
	case ON_GUILD_RELATION_MINE:	return ON_RET_GLD_MINE;
	case ON_GUILD_RELATION_WAR:		return ON_RET_GLD_WAR;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪歎辨 Define擊 鼻婦 婦啗縑 婦溼脹 啪歜 Define戲煎 滲唳и棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 1:55:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
DWORD	cltIGuild::ResponseDefineToRelationDefine(DWORD dwDefine)
{
	switch(dwDefine)
	{
	case ON_RET_GLD_ALLY:		return ON_GUILD_RELATION_ALLY;
	case ON_RET_GLD_ENEMY:		return ON_GUILD_RELATION_ENEMY;
	case ON_RET_GLD_NEUTRAL:	return ON_GUILD_RELATION_NEUTRAL;
	case ON_RET_GLD_MINE:		return ON_GUILD_RELATION_MINE;
	case ON_RET_GLD_WAR:		return ON_GUILD_RELATION_WAR;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	ч熱蒂 雖薑и棻.
//	熱薑 橾濠	:	2002-03-21 螃�� 3:39:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetHNS(SOVillage *pVillage)
{
	SI32	siFoundSlot;

	// GuildID陛 嶸�褲捘� 獄棻.
	if(pVillage->uiGuildID == 0)							return	FALSE;
	if((siFoundSlot = FindSlot(pVillage->uiGuildID)) == -1)	return	FALSE;

	// 鼻欽 Class縑 ч熱蒂 撲薑и棻.
	m_Guilds[siFoundSlot].SetHNS(pVillage);
	return	TRUE;
}
/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 鼻欽檜 爾嶸й 熱 氈朝 譆渠 鼻欽錳 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-29 螃�� 1:56:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltIGuild::GetMaxGuildMemebersNum(UI32 uiGuildID)
{	
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetMaxGuildMemebersNum();
}
*/
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 1:38:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::GetFlag(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetFlag();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	梓嫦擊 撲薑и棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 1:38:44 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetFlag(UI32 uiGuildID, SI32 siFlag)
{
	SI32			siFoundSlot;	
	//SI32			siFlagIDInStack;
	strFlagInfo		stFlagInfo;	

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;	

	// 雖錶撿 ж朝 Ы楚斜曖 薑爾蒂 撲薑и棻.
	stFlagInfo.uiGuildID		=	uiGuildID;
	stFlagInfo.siFlag			=	m_Guilds[siFoundSlot].GetFlag();

	//siFlagIDInStack	=	m_Guilds[siFoundSlot].GetFlagIDInStack();
	
	if(m_Guilds[siFoundSlot].ChangeFlag(siFlag) == TRUE)
	{		
		if(stFlagInfo.siFlag != -1)
			m_Flags.DeleteType(stFlagInfo);		

		stFlagInfo.uiGuildID	=	uiGuildID;
		stFlagInfo.siFlag		=	siFlag;
		m_Guilds[siFoundSlot].SetFlagIDInStack(m_Flags.Add(stFlagInfo));

		return	TRUE;
	}
	else
	{
		return	FALSE;
	}	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨脹 梓嫦曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-04 螃�� 2:41:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::GetUsedFlagList(SI32 *psiFlagNum, strFlagInfo *pstFlagInfo)
{
	*psiFlagNum	=	m_Flags.m_NowUseDataCount;		

	memcpy(pstFlagInfo, m_Flags.m_Data, m_Flags.m_NowUseDataCount * sizeof(strFlagInfo));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨ж朝 梓嫦檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2002-04-11 螃�� 5:00:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsUsedFlag(SI32 siFlag)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(m_Guilds[i].GetFlag() == siFlag)
				return	TRUE;
		}
	}

	return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨ж朝 檜葷檣雖 憲嬴螞棻.
//	熱薑 橾濠	:	2003-01-7 螃�� 5:00:43 - 夢遽鷓
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

BOOL	cltIGuild::IsUsedName(CHAR *szName)
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)	
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if(!strcmp(m_Guilds[i].GetGuildName(),szName))
				return	TRUE;
		}
	}

	return	FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB煎 睡攪 賅萇 鼻欽錳曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-04-22 螃�� 5:07:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::SetMemberInfoFromDB()
{
	SI32	i;

	for(i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{	
			if (m_Guilds[i].SetMemberInfoFromDB() == FALSE)
			{
				
				return FALSE;			
			}

		}
	}
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 鼻欽曖 ⑷營 薑衝 �蛾� 熱蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-05-09 螃�� 2:58:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltIGuild::GetGuildMemberNum(UI32 uiGuildID)
{
	SI32	siFoundSlot;

	if((siFoundSlot = FindSlot(uiGuildID)) == -1)	return	0;

	return	m_Guilds[siFoundSlot].GetGuildMemberNum();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蕾樓п 氈朝 鼻欽錳縑啪 詭衛雖蒂 爾頂遽棻.
//	熱薑 橾濠	:	2002-12-29 螃�� 7:14:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::SendMsgToGuildMemebers( UI32 uiGuildID, SI32 siMsg, SI32 siSendInfo, LPVOID pParam1, LPVOID pParam2, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5 )
{
	SI32	siFoundSlot;

	if( (siFoundSlot = FindSlot( uiGuildID ) ) == -1 )	return;

	m_Guilds[ siFoundSlot ].SendMsgToGuildMemebers( siMsg, siSendInfo, pParam1, pParam2, pParam3, pParam4, pParam5 );
}

/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	鼻欽縑 模樓腎朝 葆擊擊 憲溥遽棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 3:53:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltIGuild::AddVillage(UI32 uiGuildID, UI16 uiVillageCode)
{
	SI32	siFoundSlot;	

	if((siFoundSlot = FindSlot(uiGuildID)) != -1)	
	{
		return m_Guilds[siFoundSlot].AddVillage(uiVillageCode);
	}
}*/


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Guild曖 ID煎 Index蒂 橢橫螞棻.
//	熱薑 橾濠	:	2003-01-01 螃瞪 7:07:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltIGuild::GetGuildIndexByID( UI32 uiGuildID )
{
	return	FindSlot( uiGuildID );	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	
//	熱薑 橾濠	:	2003-01-01 螃瞪 7:10:35 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode,SI16 siRewardSupply)
BOOL	cltIGuild::RewardToGuildDAIForDestroyVillage( UI32 uiGuildID, MONEY mnReward, UI16 uiVillageCode)
{
	cltGuild	*pGuild;

	if( ( pGuild = GetGuildByID( uiGuildID ) ) == NULL )			return FALSE;
	
//	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode ,siRewardSupply);
	pGuild->RewardToGuildDAIForDestroyVillage( mnReward, uiVillageCode );

	return	TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	嶸�褲� Guild Index 檣雖 匐餌и棻.
//	熱薑 橾濠	:	2003-01-01 螃瞪 7:11:42 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltIGuild::IsValidGuildIndex( SI32 siGuildIndex )
{
	if( ( siGuildIndex >= 0 ) && ( siGuildIndex < SO_TOTAL_GUILD_NUM ) )		return	TRUE;

	return	FALSE;
}

BOOL	cltIGuild::SetMaxGuildID()
{
	m_uiMaxGuildID = 0;

	for(UI32 i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].IsValidGuild() == TRUE)
		{
			if (m_uiMaxGuildID < m_Guilds[i].GetGuildID())
			{
				m_uiMaxGuildID = m_Guilds[i].GetGuildID();
			}
		}
	}

	return TRUE;
	
}

VOID	cltIGuild::GuildMemberLogin( UI32 uiGuildID )
{
	SI32	siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) == -1 )			return;
	
	m_Guilds[ siFoundSlot ].GuildMemberLogin();
}
	


VOID	cltIGuild::GuildMemberLogout( UI32 uiGuildID )
{
	SI32	siFoundSlot;	

	if( ( siFoundSlot = FindSlot( uiGuildID ) ) == -1 )			return;
	
	m_Guilds[ siFoundSlot ].GuildMemberLogout();
}

//12衛縑 и廓噶 陳溥憮 鼻欽 羹觼 ж啪莠 虜萇棻.
void	cltIGuild::SetRelationReLoad()
{
	printf("鼻欽婦啗 營撲薑 衛濛 ");
	
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		m_Guilds[i].SetRelationReLoad();
	}

	printf("鼻欽婦啗 營撲薑 部\n");
}

// 奢撩ж朝 陳檣陛蒂 羹觼 п輿賊 脹棻.
// 議葛攪 寞橫釭 奢問縑 噹棻.
BOOL	cltIGuild::IsEnemyRelationWar(UI32 uiGuildID1, UI32 uiGuildID2)
{
	SI32	siFoundSlot1, siFoundSlot2;	

	if(uiGuildID1 == uiGuildID2)	return	FALSE;

	if(FindSlot(uiGuildID1, uiGuildID2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		if(m_Guilds[siFoundSlot1].IsWarGuild(siFoundSlot2) == TRUE ||
			m_Guilds[siFoundSlot2].IsWarGuild(siFoundSlot1) == TRUE)
			return	TRUE;	
	}
	
	return	FALSE;
}

BOOL	cltIGuild::SetMessage(UI32 uiGuild1,UI32 uiGuild2,CHAR* pszMsg)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuild1, uiGuild2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetMessage(&m_Guilds[siFoundSlot2],pszMsg);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessage(UI32 uiGuild,CHAR* pszGuildid2,CHAR* pszMsg)
{
	SI32	siFoundSlot1, siFoundSlot2;
	
	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessage(&m_Guilds[siFoundSlot2],pszMsg);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessageAccept(UI32 uiGuild,CHAR* pszGuildid2)
{
	SI32	siFoundSlot1, siFoundSlot2;

	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessageAccept(&m_Guilds[siFoundSlot2]);
		m_Guilds[siFoundSlot1].SetWarFlag(FALSE);
		m_Guilds[siFoundSlot2].SetWarFlag(FALSE);

		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetFriendlyMessageCancel(UI32 uiGuild,CHAR* pszGuildid2)
{
	SI32	siFoundSlot1, siFoundSlot2;

	if(FindSlot(uiGuild, pszGuildid2, &siFoundSlot1, &siFoundSlot2) == TRUE)	
	{
		m_Guilds[siFoundSlot1].SetFriendlyMessageCancel(&m_Guilds[siFoundSlot2]);
		return	TRUE;
	}
	else
		return	FALSE;
}

BOOL	cltIGuild::SetPowerInvestmentEnd()
{
	for(SI16 i = 0; i< SO_TOTAL_GUILD_NUM ; i++)
	{
		if(m_Guilds[i].GetGuildID() != 0)
		{
			m_Guilds[i].SetPowerInvestmentEnd();
		}
	}
	return TRUE;
}

BOOL	cltIGuild::GetGuildWarCheck(UI32 uiGuildID)
{
	SI32	siFoundSlot1;

	siFoundSlot1 = FindSlot(uiGuildID);

	for(SI16 i = 0 ; i < SO_TOTAL_GUILD_NUM ; i++)
	{
		if(m_Guilds[siFoundSlot1].m_siGuildRelation[i] == ON_GUILD_RELATION_WAR || 
			m_Guilds[siFoundSlot1].m_siGuildRelation[i] == ON_GUILD_RELATION_ENEMY)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//奢撩 衛除檜 腎賊 鼻欽錳縑啪 奢撩 衛除檜 腎歷棻堅 憲溥遽棻.
void   cltIGuild::SendWarTime()
{
	for(SI16 i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{ 
		if(m_Guilds[i].GetGuildID() != 0)
		{
			if(m_Guilds[i].IsWar() == TRUE)
				m_Guilds[i].SendWarTime();
		}
	}
}

//鼻欽縑 陛雖堅氈朝 葆擊擊 匐餌и棻.
SI32 cltIGuild::CheckVillage(UI16 uiVillageCode)
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGuildID() != 0)
		{
			if(m_Guilds[i].CheckVillage(uiVillageCode))
				return i;
		}
	}

	return -1;
}

//奢撩衛除檜 部釭賊 奢撩ん晦 薑爾蒂 蟾晦�� и棻.
VOID cltIGuild::InitGiveUpInfo()
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		if(m_Guilds[i].GetGiveUpFlag() != ON_GUILD_GIVEUP_NORMAL)
		{
			DBIKWarGiveUp           warGiveUp;
			memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));

			warGiveUp.uiGuildID     = m_Guilds[i].GetGuildID();
			warGiveUp.siFlag        = ON_GUILD_GIVEUP_NORMAL;

			m_pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_SET_WARGIVEUP, &warGiveUp, sizeof(DBIKWarGiveUp));			

			m_Guilds[i].SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
		}
	}
}

BOOL cltIGuild::GiveUp(UI32 uiMyGuildID)
{
	if(uiMyGuildID < 0) return FALSE;

	int nSlot = FindSlot(uiMyGuildID);
	if(nSlot < 0) return FALSE;
	
	m_Guilds[nSlot].GiveUp();

	//棻艇鼻欽曖 薑爾紫 熱薑п遽棻.
	UI32 uiOterGuildID;
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		uiOterGuildID = m_Guilds[i].GetGuildID();
		if(uiOterGuildID < 0) continue;
		if(uiMyGuildID == uiOterGuildID) continue;

		if(GetRelation(uiOterGuildID, uiMyGuildID) == ON_GUILD_RELATION_WAR)
		{
			SetRelationByIndex(nSlot, i, ON_GUILD_RELATION_ENEMY, FALSE, 0);
		}
	}

	return TRUE;
}

//DB縑憮 奢撩 ん晦婦溼 薑爾蒂 陛螳螞棻.
VOID cltIGuild::DBGetGiveUpInfo()
{
	for(int i = 0; i < SO_TOTAL_GUILD_NUM; i++)
	{
		UI32 uiGuilD = m_Guilds[i].GetGuildID();
		DBIKWarGiveUp        warGiveUp;
		memset(&warGiveUp, 0, sizeof(DBIKWarGiveUp));
		m_pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_GET_WARGIVEUP, &uiGuilD, &warGiveUp, NULL);

		if(warGiveUp.mnMoney > 0)  
			m_Guilds[i].SetGiveUpInfo(warGiveUp.uiVillageCode, warGiveUp.mnMoney, warGiveUp.siFlag);
		else //奢撩 ん晦 綠辨檜 橈朝唳辦 蟾晦�飛� и棻.
			m_Guilds[i].SetGiveUpInfo(0, 0, ON_GUILD_GIVEUP_NORMAL);
	}
}